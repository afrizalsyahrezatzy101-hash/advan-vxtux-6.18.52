/* SPDX-License-Identifier: GPL-2.0 */
/*
 * The Spreadtrum mailbox OEM layer, "revision 1".
 *
 * The register offsets and the structure layouts here were recovered
 * from the stock unisoc-mailbox.ko with Ghidra 12.1.4. See
 * drivers/mailbox/sprd-mailbox-r1.c for the full story.
 *
 * Copyright (C) 2015--2016 Spreadtrum Communications Inc.
 */

#ifndef _SPRD_MAILBOX_R1_H
#define _SPRD_MAILBOX_R1_H

#include <linux/io.h>
#include <linux/types.h>

/* the outbox register block, from sprd_mbox_phy_send */
#define SPRD_MBOX_R1_TX_ID		0x00	/* the channel id */
#define SPRD_MBOX_R1_TX_DATA0		0x04	/* data[0] */
#define SPRD_MBOX_R1_TX_DATA1		0x08	/* data[1] */
#define SPRD_MBOX_R1_TX_LEN		0x0c	/* the length */
#define SPRD_MBOX_R1_RX_STS		0x10	/* the RX status, write 1 to clear */
#define SPRD_MBOX_R1_IRQ_STS		0x14	/* bit 0..7 TX, bit 8..15 RX */
#define SPRD_MBOX_R1_RX_DOORBELL		0x18	/* bit 0 triggers the receiver */
#define SPRD_MBOX_R1_RX_ACK		0x1c	/* 3 normal, 4 overflow */
#define SPRD_MBOX_R1_FIFO_DEPTH		0x24
#define SPRD_MBOX_R1_CHAN_ID		0x28	/* the low 16 bits */

/* the FIFO status, from sprd_mbox_phy_outbox_isr */
#define SPRD_MBOX_R1_FIFO_STS		0x1c
#define SPRD_MBOX_R1_FIFO_FULL		BIT(2)
#define SPRD_MBOX_R1_FIFO_WR_SHIFT	16
#define SPRD_MBOX_R1_FIFO_RD_SHIFT	24
#define SPRD_MBOX_R1_FIFO_POS_MASK	GENMASK(7, 0)

/* the per-channel halves, from get_tx_fifo_mask */
#define SPRD_MBOX_R1_TX_WR		0x200
#define SPRD_MBOX_R1_TX_RD		0x204
#define SPRD_MBOX_R1_RX_WR		0x408
#define SPRD_MBOX_R1_RX_RD		0x40c

/* the interrupt mask */
#define SPRD_MBOX_R1_IRQ_MSK		0x1c
#define SPRD_MBOX_R1_IRQ_ALL		0x1f

/* the outbox blocks are 0x1000 apart */
#define SPRD_MBOX_R1_OUTBOX_SPAN	0x1000

/* the per-channel descriptor table has a 0xf8 stride */
#define SPRD_MBOX_R1_CHAN_STRIDE	0xf8

#define SPRD_MBOX_R1_CHAN_MAX		8

struct mbox_chan;

/* one channel of the OEM layer */
struct sprd_mbox_chan {
	struct mbox_chan	*mbox_chan;
	void __iomem		*outbox;
	void __iomem		*inbox;
	u32			index;
	u32			fifo_depth;
	u32			msg[2];
	u8			started;
	u8			nowakeup;
	u32			cur_idx;
};

/* the OEM vtable, the member order is from the stock blob's
 * sprd_mbox_phy_ops_r1 relocation table */
struct sprd_mbox_phy_ops {
	int (*startup)(struct sprd_mbox_chan *chan);
	void (*shutdown)(struct sprd_mbox_chan *chan);
	int (*flush)(struct sprd_mbox_chan *chan, unsigned long timeout);
	int (*send)(struct sprd_mbox_chan *chan, u32 *msg);
	int (*inbox_isr)(struct sprd_mbox_chan *chan);
	int (*outbox_isr)(struct sprd_mbox_chan *chan);
	void (*debug_show)(struct sprd_mbox_chan *chan);
};

/* the driver's own state */
struct sprd_mbox_r1 {
	struct device		*dev;
	void __iomem		*outbox_base;
	void __iomem		*inbox_base;
	u32			chan_num;
	u32			fifo_depth;
	u32			outbox_span;
	u32			chan_id;
	u8			nowakeup;
	u32			cur_idx;
	struct sprd_mbox_phy_ops *phy_ops;
};

int sprd_mbox_phy_r1_init(struct sprd_mbox_r1 *r1);

#endif /* _SPRD_MAILBOX_R1_H */
