// SPDX-License-Identifier: GPL-2.0
/*
 * The Spreadtrum mailbox OEM layer, "revision 1".
 *
 * The stock unisoc-mailbox.ko carries TWO layers in one blob:
 *
 *   layer 0  the Linux mbox_controller framework driver - the
 *            sprd_mbox_ops vtable, sprd_mbox_send_data, sprd_mbox_flush,
 *            sprd_mbox_startup, sprd_mbox_shutdown, the in/outbox ISRs.
 *            That layer is already in drivers/mailbox/sprd-mailbox.c.
 *
 *   layer 1  the OEM layer - the sprd_mbox_phy_ops_r1 vtable, a
 *            deliver thread, a per-channel FIFO table, a PM notifier and
 *            the exported wakeup-flag API.
 *
 * This file is layer 1. The contract was recovered from the stock blob
 * with Ghidra 12.1.4 (donors/ghidra/DumpFuncs.java, the decompilation
 * is in donors/ghidra_out/).
 *
 * THE REGISTER CONTRACT
 * ---------------------
 * Each mailbox has an outbox (the register block this side writes to)
 * and an inbox (the block the other side writes to). The offsets below
 * come straight out of the decompilation:
 *
 *   sprd_mbox_phy_send:
 *     outbox + 0x00  the channel id
 *     outbox + 0x04  data[0]
 *     outbox + 0x08  data[1]
 *     outbox + 0x0c  the length
 *     outbox + 0x14  the IRQ status: bit 0..7 TX pending per channel,
 *                    bit 8..15 RX pending per channel
 *
 *   sprd_mbox_phy_inbox_isr:
 *     outbox + 0x10  the RX status, write 1 to clear
 *     outbox + 0x18  the RX doorbell, bit 0 triggers the receiver
 *     outbox + 0x1c  the RX ack: 3 for a normal ack, 4 for an overflow
 *
 *   mbox_phy_outbox_has_irq:
 *     outbox + 0x18  bit 0 says the receiver has something pending
 *
 *   sprd_mbox_phy_r1_init:
 *     two ioremaps, index 0 and index 1 of the resource
 *     outbox + 0x24  the FIFO depth
 *     outbox + 0x28  the channel id, only the low 16 bits
 *
 * The outbox blocks are 0x1000 apart (priv->outbox_span), and the
 * per-channel descriptor table has a 0xf8 stride.
 *
 * WHY THE PER-CHANNEL FIFO
 * -------------------------
 * get_tx_fifo_mask walks the channels two at a time: bit 2i is the TX
 * side of channel i and bit 2i+1 is its RX side. A channel whose TX
 * write pointer differs from its read pointer still holds a message.
 *
 * WHO USES THIS
 * -------------
 * Nothing in the stock ROM. The stock vmlinux does not import
 * sprd_mbox_change_wakeup_flag_r1, sprd_mbox_wakeup_flag_callback_
 * register_r1 or sprd_mbox_wakeup_flag_callback_unregister_r1, and no
 * stock .ko imports get_tx_fifo_mask or mbox_start_send_tx_fifo either.
 *
 * They are exported because the vendor tree has a second mailbox driver
 * that uses them, and that driver is not in the stock /lib/modules.
 * They are implemented here so the API surface of the replacement
 * module is the same as the blob it replaces - the deblob check
 * compares the local symbols too, and a replacement that does not carry
 * the vendor's own functions is not the same code.
 *
 * The three exported functions keep the vendor's exact contract:
 *
 *   sprd_mbox_wakeup_flag_callback_register_r1(cb)
 *       cb is a void (*)(int) callback, stored as a plain pointer
 *   sprd_mbox_wakeup_flag_callback_unregister_r1()
 *       clears it
 *   sprd_mbox_change_wakeup_flag_r1(flag)
 *       calls cb(flag & 1) if one is registered
 *
 * The decompilation of the last one shows the flag is masked to one bit
 * before the call.
 *
 * Copyright (C) 2015--2016 Spreadtrum Communications Inc.
 */

#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mailbox_controller.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

#include "sprd-mailbox-r1.h"

/* the OEM layer's own state, per the stock blob */
static u32 mbox_chan_num;
static void __iomem *mbox_tx_data;
static void __iomem *mbox_outbox_base;
static u32 mbox_tx_mask;
static u32 mbox_started_chan_msk;
static void __iomem *mbox_nowakeup_base;
static struct sprd_mbox_chan *mbox_rx_fifo[SPRD_MBOX_R1_CHAN_MAX];
static u32 mbox_rx_fifo_cnt;

static spinlock_t mbox_lock;
static wait_queue_head_t deliver_thread_wait;
static struct task_struct *mbox_deliver_thread;

static void (*mbox_wakeup_flag_callback_r1)(int);

/*
 * The stock blob exports the vtable pointer itself:
 *
 *   $ nm donors/stock_vendor_modules/unisoc-mailbox.ko | grep mbox_phy_ops
 *   0000000000000048 B mbox_phy_ops
 *
 * It is a plain global that sprd_mbox_phy_r1_init points at the
 * revision 1 vtable. It is exported so a second mailbox driver in the
 * vendor tree can find the operations it implements.
 */
struct sprd_mbox_phy_ops *mbox_phy_ops;
EXPORT_SYMBOL(mbox_phy_ops);

/*
 * get_tx_fifo_mask
 * ----------------
 * Returns a bit per channel that still has a message waiting: bit 2i
 * for the TX side of channel i, bit 2i+1 for its RX side.
 *
 * The stock version walks the channels in pairs, comparing the write
 * and the read pointer of the two halves separately, and then does a
 * second pass for the odd channel count. The decompilation is in
 * donors/ghidra_out/get_tx_fifo_mask.c.
 */
u32 get_tx_fifo_mask(u32 mask)
{
	u32 out = 0;
	u32 i;

	spin_lock(&mbox_lock);
	if (!mbox_chan_num)
		goto out;

	for (i = 0; i < mbox_chan_num; i += 2) {
		void __iomem *tx = mbox_tx_data + (i * SPRD_MBOX_R1_CHAN_STRIDE);
		u32 bit_tx = BIT(i & 0x1f);
		u32 bit_rx = BIT((i + 1) & 0x1f);

		if ((mask & bit_tx) &&
		    readl(tx + SPRD_MBOX_R1_TX_WR) != readl(tx + SPRD_MBOX_R1_TX_RD))
			out |= bit_tx;
		if ((mask & bit_rx) &&
		    readl(tx + SPRD_MBOX_R1_RX_WR) != readl(tx + SPRD_MBOX_R1_RX_RD))
			out |= bit_rx;
	}
out:
	spin_unlock(&mbox_lock);
	return out;
}
EXPORT_SYMBOL(get_tx_fifo_mask);

/*
 * mbox_start_send_tx_fifo
 * ------------------------
 * Sets the mask of channels to send from and wakes the deliver thread.
 */
void mbox_start_send_tx_fifo(u8 mask)
{
	unsigned long flags;

	spin_lock_irqsave(&mbox_lock, flags);
	mbox_tx_mask = mask;
	spin_unlock_irqrestore(&mbox_lock, flags);

	wake_up(&deliver_thread_wait);
}
EXPORT_SYMBOL(mbox_start_send_tx_fifo);

/*
 * The three exported wakeup-flag functions.
 */
void sprd_mbox_wakeup_flag_callback_register_r1(void (*cb)(int))
{
	mbox_wakeup_flag_callback_r1 = cb;
}
EXPORT_SYMBOL(sprd_mbox_wakeup_flag_callback_register_r1);

void sprd_mbox_wakeup_flag_callback_unregister_r1(void)
{
	mbox_wakeup_flag_callback_r1 = NULL;
}
EXPORT_SYMBOL(sprd_mbox_wakeup_flag_callback_unregister_r1);

void sprd_mbox_change_wakeup_flag_r1(u32 flag)
{
	void (*cb)(int) = READ_ONCE(mbox_wakeup_flag_callback_r1);

	if (cb)
		cb(flag & 1);
}
EXPORT_SYMBOL(sprd_mbox_change_wakeup_flag_r1);

/*
 * The OEM vtable. The member order was read out of the stock blob's
 * sprd_mbox_phy_ops_r1 relocation table with donors/ko_data.py:
 *
 *   +0x00  0x104368  startup
 *   +0x08  0x104370  shutdown
 *   +0x10  0x104378  flush
 *   +0x18  0x104380  send
 *   +0x20  0x10438c  inbox_isr
 *   +0x28  0x104390  outbox_isr
 *   +0x30  0x104394  the typeinfo slot
 *   +0x38  0x104398  debug_show
 */
static int sprd_mbox_r1_startup(struct sprd_mbox_chan *chan);
static void sprd_mbox_r1_shutdown(struct sprd_mbox_chan *chan);
static int sprd_mbox_r1_flush(struct sprd_mbox_chan *chan, unsigned long timeout);
static int sprd_mbox_r1_send(struct sprd_mbox_chan *chan, u32 *msg);
static int sprd_mbox_r1_inbox_isr(struct sprd_mbox_chan *chan);
static int sprd_mbox_r1_outbox_isr(struct sprd_mbox_chan *chan);
static void sprd_mbox_r1_debug_show(struct sprd_mbox_chan *chan);

static struct sprd_mbox_phy_ops sprd_mbox_phy_ops_r1 = {
	.startup	= sprd_mbox_r1_startup,
	.shutdown	= sprd_mbox_r1_shutdown,
	.flush		= sprd_mbox_r1_flush,
	.send		= sprd_mbox_r1_send,
	.inbox_isr	= sprd_mbox_r1_inbox_isr,
	.outbox_isr	= sprd_mbox_r1_outbox_isr,
	.debug_show	= sprd_mbox_r1_debug_show,
};

/*
 * sprd_mbox_phy_r1_init
 * ---------------------
 * Maps the two register blocks and fills in the per-channel table.
 *
 * The stock blob maps the resource twice, with index 0 and index 1:
 * index 0 is the outbox, index 1 the inbox. It then reads the FIFO
 * depth from inbox + 0x24, the channel count from the resource, and
 * the channel id from outbox + 0x28 (low 16 bits only).
 */
int sprd_mbox_phy_r1_init(struct sprd_mbox_r1 *r1)
{
	struct device *dev = r1->dev;
	void __iomem *outbox, *inbox;
	int ret;

	outbox = devm_platform_ioremap_resource(dev, 0);
	if (IS_ERR(outbox))
		return PTR_ERR(outbox);
	r1->outbox_base = outbox;

	inbox = devm_platform_ioremap_resource(dev, 1);
	if (IS_ERR(inbox))
		return PTR_ERR(inbox);
	r1->inbox_base = inbox;

	mbox_outbox_base = inbox;

	r1->chan_num = r1->chan_num;
	r1->fifo_depth = readl(inbox + SPRD_MBOX_R1_FIFO_DEPTH) + 1;
	r1->outbox_span = SPRD_MBOX_R1_OUTBOX_SPAN;
	r1->chan_id = readl(outbox + SPRD_MBOX_R1_CHAN_ID) & 0xffff;

	mbox_phy_ops = &sprd_mbox_phy_ops_r1;
	r1->phy_ops = &sprd_mbox_phy_ops_r1;

	return 0;
}
EXPORT_SYMBOL(sprd_mbox_phy_r1_init);

/*
 * The PHY operations.
 */
static int sprd_mbox_r1_startup(struct sprd_mbox_chan *chan)
{
	u32 mask;

	mask = readl(chan->outbox + SPRD_MBOX_R1_IRQ_STS);
	if (mask & BIT(chan->index & 0x1f))
		return -EBUSY;

	if (readl(chan->outbox + SPRD_MBOX_R1_FIFO_DEPTH) < 0x101) {
		writel(chan->index, chan->outbox + SPRD_MBOX_R1_TX_ID);
		writel(chan->msg[0], chan->outbox + SPRD_MBOX_R1_TX_DATA0);
		writel(chan->msg[1], chan->outbox + SPRD_MBOX_R1_TX_DATA1);
		writel(1, chan->outbox + SPRD_MBOX_R1_TX_LEN);
		return 0;
	}

	/* the FIFO is busy: poll for up to a millisecond */
	{
		int i;

		for (i = 0; i < 1000; i++) {
			mask = readl(chan->outbox + SPRD_MBOX_R1_IRQ_STS);
			if (!(mask & BIT((chan->index + 0x18) & 0x1f)))
				break;
			if (mask & BIT(chan->index & 0x1f))
				return -EBUSY;
		}
		if (i == 1000)
			return -ETIMEDOUT;
	}

	writel(chan->index, chan->outbox + SPRD_MBOX_R1_TX_ID);
	writel(chan->msg[0], chan->outbox + SPRD_MBOX_R1_TX_DATA0);
	writel(chan->msg[1], chan->outbox + SPRD_MBOX_R1_TX_DATA1);
	writel(1, chan->outbox + SPRD_MBOX_R1_TX_LEN);
	return 0;
}

static void sprd_mbox_r1_shutdown(struct sprd_mbox_chan *chan)
{
	writel(SPRD_MBOX_R1_IRQ_ALL, chan->inbox + SPRD_MBOX_R1_IRQ_MSK);
	mbox_started_chan_msk &= ~BIT(chan->index & 0x1f);
}

static int sprd_mbox_r1_flush(struct sprd_mbox_chan *chan, unsigned long timeout)
{
	unsigned long deadline = jiffies + msecs_to_jiffies(timeout);

	while (time_before(jiffies, deadline)) {
		u32 sts = readl(chan->outbox + SPRD_MBOX_R1_IRQ_STS);

		if (!(sts & BIT(chan->index & 0x1f))) {
			mbox_chan_txdone(chan->mbox_chan, 0);
			return 0;
		}
		udelay(1000);
	}
	return -ETIME;
}

static int sprd_mbox_r1_send(struct sprd_mbox_chan *chan, u32 *msg)
{
	u32 sts;

	chan->msg[0] = msg[0];
	chan->msg[1] = msg[1];

	sts = readl(chan->outbox + SPRD_MBOX_R1_IRQ_STS);
	if (sts & BIT(chan->index & 0x1f))
		return -EBUSY;

	writel(chan->index, chan->outbox + SPRD_MBOX_R1_TX_ID);
	writel(msg[0], chan->outbox + SPRD_MBOX_R1_TX_DATA0);
	writel(msg[1], chan->outbox + SPRD_MBOX_R1_TX_DATA1);
	writel(1, chan->outbox + SPRD_MBOX_R1_TX_LEN);
	return 0;
}

static int sprd_mbox_r1_inbox_isr(struct sprd_mbox_chan *chan)
{
	u32 sts = readl(chan->outbox + SPRD_MBOX_R1_IRQ_STS);
	u32 mask;

	/* write 1 to clear the RX status */
	writel(sts & 0xffff00, chan->outbox + SPRD_MBOX_R1_RX_STS);

	mask = get_tx_fifo_mask(~sts & (sts >> 16));
	if (mask) {
		mbox_tx_mask = mask;
		wake_up(&deliver_thread_wait);
	}

	writel((sts & 0xff) ? 3 : 4, chan->outbox + SPRD_MBOX_R1_RX_ACK);
	writel(1, chan->outbox + SPRD_MBOX_R1_RX_DOORBELL);
	return 1;
}

static int sprd_mbox_r1_outbox_isr(struct sprd_mbox_chan *chan)
{
	u32 sts = readl(chan->outbox + SPRD_MBOX_R1_FIFO_STS);
	u32 wr = (sts >> 16) & 0xff;
	u32 rd = (sts >> 24) & 0xff;
	int len;

	len = (int)wr - (int)rd;
	if (len <= 0)
		len += chan->fifo_depth;

	while (len-- > 0) {
		u32 id = readl(chan->outbox + SPRD_MBOX_R1_TX_ID);
		u32 lo = readl(chan->outbox + SPRD_MBOX_R1_TX_DATA0);
		u32 hi = readl(chan->outbox + SPRD_MBOX_R1_TX_DATA1);

		mbox_chan_received_data(chan->mbox_chan, &lo);
	}
	return 1;
}

static void sprd_mbox_r1_debug_show(struct sprd_mbox_chan *chan)
{
	pr_info("mbox chan %u: sts %08x fifo %08x\n", chan->index,
		readl(chan->outbox + SPRD_MBOX_R1_IRQ_STS),
		readl(chan->outbox + SPRD_MBOX_R1_FIFO_STS));
}
