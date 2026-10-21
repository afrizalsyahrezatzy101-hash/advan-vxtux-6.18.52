/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "[Audio:SMSG] "fmt

#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/syscore_ops.h>
#include <linux/wait.h>

#include "audio-smsg.h"
#include <linux/soc/sprd/sprd_mailbox.h>
#include "sprd-string.h"

#define sp_asoc_pr_dbg pr_debug
#define sp_asoc_pr_info pr_info

#define  AGDSP_BOOT_OK			0xbeee
#define  AGDSP_ASSERT			0x25
#define  AGDSP_COMMUNICATION_TIMEOUT	0x26
#define  WAIT_CHAN_WAKE_UP_COUNT		100
#define MAX_PREV_MSG_PRINTK_COUNT 70
static struct aud_smsg_ipc *aud_smsg_ipcs[AUD_IPC_NR];

static ushort debug_enable;
static ushort assert_trigger;

static AGDSP_DUMP_FUNC dump_func;
static void *dump_func_priv;
module_param_named(debug_enable, debug_enable, ushort, 0644);
module_param_named(assert_trigger, assert_trigger, ushort, 0644);

static int aud_smsg_ch_send(struct aud_smsg_ipc *ipc, struct aud_smsg *msg);
static int aud_smsg_all_ch_notify(struct aud_smsg_ipc *ipc,
	struct aud_smsg *msg);
static void aud_smsg_tx_dump(void);
static void aud_smsg_rx_dump(void);

static void aud_smsg_reset(struct aud_smsg_ipc *ipc)
{
	writel_relaxed(0, (void *) ipc->txbuf_wrptr);
	writel_relaxed(0, (void *) ipc->txbuf_rdptr);

	writel_relaxed(0, (void *) ipc->rxbuf_wrptr);
	writel_relaxed(0, (void *) ipc->rxbuf_rdptr);
}

/*
 * The stock blob carries these as real local functions; gcc inlines both
 * (each has a single caller), which would drop the symbol the verifier
 * compares.  ponytail: keep the noinline until the stock symbol list is
 * satisfied without it; it costs a call per timeout/notify.
 */
static noinline void aud_smsg_dump_func(u32 is_timeout)
{
	aud_smsg_rx_dump();
	aud_smsg_tx_dump();
	if (dump_func)
		dump_func(dump_func_priv, is_timeout);
}

int aud_smsg_register_dump_func(AGDSP_DUMP_FUNC handler, void *priv_data)
{
	dump_func = handler;
	dump_func_priv = priv_data;
	return 0;
}
EXPORT_SYMBOL(aud_smsg_register_dump_func);

/* deklarasi provider (dibuat tools/re/fix_provider_proto.py; menutup -Wmissing-prototypes) */
irqreturn_t aud_smsg_irq_handler(void *ptr, void *dev_id);
irqreturn_t aud_smsg_irq_handler(void *ptr, void *dev_id)
{
	struct aud_smsg_ipc *ipc = (struct aud_smsg_ipc *)dev_id;
	struct aud_smsg *msg;
	uintptr_t rxpos;
	unsigned long flags;

	pr_debug("get smsg: wrptr=%u, rdptr=%u\n",
		readl_relaxed((void *)ipc->rxbuf_wrptr),
		readl_relaxed((void *)ipc->rxbuf_rdptr));

	spin_lock_irqsave(&(ipc->rxpinlock), flags);
	while (readl_relaxed((void *)ipc->rxbuf_wrptr) !=
	       readl_relaxed((void *)ipc->rxbuf_rdptr)) {
		rxpos = (readl_relaxed((void *)ipc->rxbuf_rdptr) &
			 (ipc->rxbuf_size - 1)) * sizeof(struct aud_smsg) +
			ipc->rxbuf_addr;

		msg = (struct aud_smsg *)rxpos;

		if ((msg->channel != AMSG_CH_DSP_LOG) &&
		    (msg->channel != AMSG_CH_DSP_PCM)) {
			pr_debug("%s get smsg: wrptr=%u, rdptr=%u, rxpos=0x%lx\n",
				 __func__,
				 readl_relaxed((void *)ipc->rxbuf_wrptr),
				 readl_relaxed((void *)ipc->rxbuf_rdptr),
				 rxpos);
			pr_debug("%s read smsg: command=%d, channel=%d,parameter0=0x%08x, parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
				 __func__, msg->command, msg->channel,
				 msg->parameter0, msg->parameter1,
				 msg->parameter2, msg->parameter3);
		}
		if (ipc->dsp_ready == false &&
		    msg->command == AGDSP_BOOT_OK &&
		    msg->parameter0 == AGDSP_BOOT_OK &&
		    msg->channel == AGDSP_BOOT_OK) {
			pr_err("peter: dsp ready now\n");
			ipc->dsp_ready = true;
			aud_smsg_all_ch_notify(ipc, msg);
			writel_relaxed(readl_relaxed((void *)ipc->rxbuf_rdptr) +
				       1, (void *)ipc->rxbuf_rdptr);
			continue;
		}
		if ((msg->channel == AMSG_CH_DSP_ASSERT_CTL) &&
			(msg->command == AGDSP_ASSERT)) {
			u32 is_timeout = 0;

			ipc->dsp_ready = false;
			pr_err("peter: dsp assert now:0x%08x,0x%08x,0x%08x,0x%08x\n",
				msg->parameter0, msg->parameter1,
				 msg->parameter2, msg->parameter3);
			aud_smsg_all_ch_notify(ipc, msg);
			writel_relaxed(readl_relaxed((void *)ipc->rxbuf_rdptr) +
				       1, (void *)ipc->rxbuf_rdptr);
			aud_smsg_dump_func(is_timeout);
			aud_smsg_reset(ipc);
			continue;
		}
		if (ipc->dsp_ready)
			aud_smsg_ch_send(ipc, msg);

		writel_relaxed(readl_relaxed((void *)ipc->rxbuf_rdptr) + 1,
			       (void *) ipc->rxbuf_rdptr);
	}
	spin_unlock_irqrestore(&(ipc->rxpinlock), flags);
	/* wake_lock_timeout(&sipc_wake_lock, HZ / 50); */

	return IRQ_HANDLED;
}

/*
 * ==== RE B8/R1: dua ekspor blob audio_sipc.ko yang tidak ada di sumber GPL ====
 *
 * Blob vendor audio_sipc.ko (5.4.210, build 2023-11-07) punya dua simbol teks
 * yang tidak ada di fork Tab A8 maupun revisi vendor mana pun:
 *   aud_smsg_set_mboxchan  0x25e4..0x25f0  (12 B, 5 instruksi)
 *   audio_sipc_rx_callback    0x2980..0x29a4  (36 B, 12 instruksi)
 * Beserta dua variabel .bss miliknya: aud_smsg_mboxchan (8 B, static) dan
 * aud_smsg_ipcs (2 pointer). Bukti mentah + tabel offset:
 *   dumped/re/audio_sipc.ko.sym.txt
 *   dumped/re/audio_sipc.ko.aud_smsg_set_mboxchan.dis.txt
 *   dumped/re/audio_sipc.ko.audio_sipc_rx_callback.dis.txt
 * Uraian: dumped/re/R1_audio_4sym.md
 *
 * Keduanya hasil "chan langsung": revisi vendor mengganti pemanggilan
 * mbox_raw_sent(target_id, msg) dengan mbox_send_message(chan, msg) + chan
 * diserahkan sekali dari probe lewat aud_smsg_set_mboxchan(). Port ini tetap
 * mengirim lewat aud_smsg_send() -> mbox_raw_sent(ipc->target_id, ...), yang
 * memakai chan per-target yang sama (lihat vxtux/sipc/sprd_mbox_vendor.c),
 * jadi jalur kirim tidak diubah di sini — yang dipulihkan adalah permukaan
 * ekspor blob supaya modul vendor lain tetap bisa menaut.
 */
struct mbox_chan;   /* cukup forward: blob hanya menyimpan pointer (8 B) */

/* .bss+0x570 di blob (nm: aud_smsg_mboxchan, static, 8 B). */
/*
 * Non-static on purpose: the stock blob carries aud_smsg_mboxchan as a 8-byte
 * bss symbol.  Declaring it static here keeps it out of the module's symbol
 * table entirely, so the verifier reads it as missing.
 */
struct mbox_chan *aud_smsg_mboxchan;
/*
 * msg_index (4 B bss in the stock blob): a monotonically increasing counter
 * used to tag received messages.
 */
unsigned int msg_index __used;

/* Blob 0x25e4: adrp/str/ret — simpan argumen ke aud_smsg_mboxchan. */
void aud_smsg_set_mboxchan(struct mbox_chan *chan)
{
	aud_smsg_mboxchan = chan;
}
EXPORT_SYMBOL(aud_smsg_set_mboxchan);

/*
 * Blob 0x2980: ldr x1, [aud_smsg_ipcs+8]; bl aud_smsg_irq_handler.
 * Argumen pertama x0 diteruskan apa adanya (ptr), argumen kedua diambil dari
 * aud_smsg_ipcs[AUD_IPC_AGDSP] — indeks 1, persis offset +0x568 di blob.
 * Diteruskan ke handler yang sudah ada, jadi tidak ada logika baru.
 */
irqreturn_t audio_sipc_rx_callback(void *ptr)
{
	return aud_smsg_irq_handler(ptr, aud_smsg_ipcs[AUD_IPC_AGDSP]);
}
EXPORT_SYMBOL(audio_sipc_rx_callback);

static void aud_smsg_tx_dump(void)
{
	struct aud_smsg_ipc *ipc = aud_smsg_ipcs[AUD_IPC_AGDSP];
	struct aud_smsg *msg;
	uintptr_t rxpos;
	u32 tx_rd;
	u32 max_print_count = MAX_PREV_MSG_PRINTK_COUNT;
	u32 pre_print_count1, pre_print_count2;
	u32 last_pre_msg_pos, last_pre_msg_addr;
	int i;

	tx_rd = readl_relaxed((void *)ipc->txbuf_rdptr);
	last_pre_msg_pos = tx_rd & (ipc->txbuf_size - 1);
	if (last_pre_msg_pos)
		last_pre_msg_pos -= 1;
	else
		last_pre_msg_pos = ipc->rxbuf_size - 1;

	last_pre_msg_addr = ipc->txbuf_addr_p +
		last_pre_msg_pos * sizeof(struct aud_smsg);

	pr_info("%s:txbuf_addr_p:%x, txbuf_rdptr_p:%x, txbuf_wrptr_p:%x",
		__func__,
		ipc->txbuf_addr_p, ipc->txbuf_rdptr_p,
		ipc->txbuf_wrptr_p);
	pr_info("%s tx_smsg: wrptr=%u, rdptr=%u, msg count:%u, last msg addr:%x, rx_rd:%d, last pos:%d\n",
		__func__,
		readl_relaxed((void *)ipc->txbuf_wrptr),
		readl_relaxed((void *)ipc->txbuf_rdptr),
		ipc->txbuf_size,
		last_pre_msg_addr,
		tx_rd,
		last_pre_msg_pos);

	if (max_print_count > ipc->txbuf_size)
		max_print_count = ipc->txbuf_size;

	pr_info("start to print last %d msgs ap sent to audio dsp\n",
		max_print_count);
	pr_info("last_log_pos:%d,total msg count:%d\n", last_pre_msg_pos,
		ipc->txbuf_size);

	pre_print_count2 = (last_pre_msg_pos + 1) > max_print_count
		? max_print_count : (last_pre_msg_pos + 1);
	pre_print_count1 = max_print_count - pre_print_count2;

	for (i = 0; i < pre_print_count1; i++) {
		rxpos = ipc->txbuf_addr + ((ipc->txbuf_size - 1)
			- pre_print_count1
			+ 1+i) * sizeof(struct aud_smsg);
		msg = (struct aud_smsg *)rxpos;
		pr_info("%s : -%d,prev tx read smsg: command=0x%x, channel=0x%x,parameter0=0x%08x,parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, max_print_count - i - 1,
			msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);
	}

	for (i = 0; i < pre_print_count2; i++) {
		rxpos = ipc->txbuf_addr
			+ (last_pre_msg_pos - pre_print_count2 + 1+i)
			* sizeof(struct aud_smsg);
		msg = (struct aud_smsg *)rxpos;
		pr_info("%s : -%d, prev tx read smsg: command=0x%x, channel=0x%x,parameter0=0x%08x, parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, max_print_count - pre_print_count1 - i - 1,
			msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);
	}

	pr_info("start to print msgs ap sent to audio dsp but not being processed, count:%d\n",
		readl_relaxed((void *)ipc->txbuf_wrptr)
		- readl_relaxed((void *)ipc->txbuf_rdptr));
	i = 0;
	while (readl_relaxed((void *)ipc->txbuf_wrptr) != tx_rd) {
		rxpos = (tx_rd &  (ipc->txbuf_size - 1))
			* sizeof(struct aud_smsg)
			+ ipc->txbuf_addr;

		msg = (struct aud_smsg *)rxpos;

		pr_info("%s : smsg:%d, wrptr=%u, rdptr=%u, rx_rd:%u, rxpos=0x%lx\n",
			__func__, i,
			readl_relaxed((void *)ipc->txbuf_wrptr),
			readl_relaxed((void *)ipc->txbuf_rdptr),
			tx_rd,
			rxpos);
		pr_info("%s smsg:%d: command=0x%x, channel=0x%x,parameter0=0x%08x, parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, i,
			msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);
		tx_rd++;
		i++;
	}
}

static void aud_smsg_rx_dump(void)
{
	struct aud_smsg_ipc *ipc = aud_smsg_ipcs[AUD_IPC_AGDSP];
	struct aud_smsg *msg;
	uintptr_t rxpos;
	u32 rx_rd;
	u32 max_print_count = MAX_PREV_MSG_PRINTK_COUNT;
	u32 pre_print_count1, pre_print_count2;
	u32 last_pre_msg_pos, last_pre_msg_addr;
	int i;

	rx_rd = readl_relaxed((void *)ipc->rxbuf_rdptr);
	last_pre_msg_pos = rx_rd & (ipc->rxbuf_size - 1);
	if (last_pre_msg_pos)
		last_pre_msg_pos -= 1;
	else
		last_pre_msg_pos = ipc->rxbuf_size - 1;

	last_pre_msg_addr = ipc->rxbuf_addr_p +
		last_pre_msg_pos * sizeof(struct aud_smsg);

	pr_info("%s:rxbuf_addr_p:%x, rxbuf_rdptr_p:%x, rxbuf_wrptr_p:%x",
		__func__,
		ipc->rxbuf_addr_p, ipc->rxbuf_rdptr_p,
		ipc->rxbuf_wrptr_p);
	pr_info("%s rx_smsg: wrptr=%u, rdptr=%u, size:%u, last msg addr:%x, rx_rd:%d, last pos:%d\n",
		__func__,
		readl_relaxed((void *)ipc->rxbuf_wrptr),
		readl_relaxed((void *)ipc->rxbuf_rdptr),
		ipc->rxbuf_size,
		last_pre_msg_addr,
		rx_rd,
		last_pre_msg_pos);

	if (max_print_count > ipc->rxbuf_size)
		max_print_count = ipc->rxbuf_size;
	pr_info("start to print last %d msgs audio dsp sent to ap\n",
		max_print_count);
	pr_info("last_log_pos:%d,total msg count:%d\n", last_pre_msg_pos,
		ipc->rxbuf_size);

	pre_print_count2 = (last_pre_msg_pos + 1) > max_print_count
		? max_print_count : (last_pre_msg_pos + 1);
	pre_print_count1 = max_print_count - pre_print_count2;

	for (i = 0; i < pre_print_count1; i++) {
		rxpos = ipc->rxbuf_addr + ((ipc->rxbuf_size - 1)
			- pre_print_count1
			+ 1+i) * sizeof(struct aud_smsg);
		msg = (struct aud_smsg *)rxpos;
		pr_info("%s : -%d,prev rx read smsg: command=0x%x, channel=0x%x,parameter0=0x%08x,parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, max_print_count - i - 1,
			msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);
	}

	for (i = 0; i < pre_print_count2; i++) {
		rxpos = ipc->rxbuf_addr +
			(last_pre_msg_pos - pre_print_count2 + 1+i)
			* sizeof(struct aud_smsg);
		msg = (struct aud_smsg *)rxpos;
		pr_info("%s : -%d, prev rx read smsg: command=0x%x, channel=0x%x,parameter0=0x%08x, parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, max_print_count - pre_print_count1 - i - 1,
			msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);
	}

	pr_info("start to print msgs ap sent to audio dsp but not being processed, count:%d\n",
		readl_relaxed((void *)ipc->rxbuf_wrptr)
		- readl_relaxed((void *)ipc->rxbuf_rdptr));
	i = 0;
	while (readl_relaxed((void *)ipc->rxbuf_wrptr) != rx_rd) {
		rxpos = (rx_rd &  (ipc->rxbuf_size - 1))
			* sizeof(struct aud_smsg)
			+ ipc->rxbuf_addr;

		msg = (struct aud_smsg *)rxpos;

		pr_info("%s : smsg:%d, wrptr=%u, rdptr=%u, rx_rd:%u, rxpos=0x%lx\n",
			__func__, i,
			readl_relaxed((void *)ipc->rxbuf_wrptr),
			readl_relaxed((void *)ipc->rxbuf_rdptr),
			rx_rd,
			rxpos);
		pr_info("%s smsg:%d: command=0x%x, channel=0x%x,parameter0=0x%08x, parameter1=0x%08x,parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, i,
			msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);
		rx_rd++;
		i++;
	}
}

int aud_smsg_ipc_create(u8 dst, struct aud_smsg_ipc *ipc)
{
	if (!ipc->irq_handler)
		ipc->irq_handler = (void *)aud_smsg_irq_handler;

	spin_lock_init(&(ipc->txpinlock));
	spin_lock_init(&(ipc->rxpinlock));
	spin_lock_init(&(ipc->ch_rx_pinlock));

	aud_smsg_ipcs[dst] = ipc;
	ipc->dsp_ready  = true;

#ifdef CONFIG_SPRD_MAILBOX
{
	int rval;
	/* explicitly call irq handler in case of missing irq on boot */
	/*((irq_handler_t)ipc->irq_handler)((int)(&ipc->target_id), ipc);*/
	aud_smsg_irq_handler(&ipc->target_id, ipc);

	rval = mbox_register_irq_handle(ipc->target_id, aud_smsg_irq_handler, ipc);
	if (rval != 0) {
		pr_err("%s, Failed to register irq handler in mailbox: %s\n",
				__func__, ipc->name);
		return rval;
	}
}
#endif
	return 0;
}

int aud_smsg_ipc_destroy(u8 dst)
{
	/* kthread_stop(ipc->thread); */
	aud_smsg_ipcs[dst] = NULL;

	return 0;
}

/* ****************************************************************** */

int aud_smsg_ch_open(u8 dst, uint16_t channel)
{
	struct aud_smsg_ipc *ipc = aud_smsg_ipcs[dst];
	struct aud_smsg_channel *ch;

	if (!ipc) {
		pr_err("ERR:%s ipc ENODEV\n", __func__);
		return -EPROBE_DEFER;
	}

	if (ipc->states[channel] == CHAN_STATE_OPENED)
		return -1;

	ch = kzalloc(sizeof(struct aud_smsg_channel), GFP_KERNEL);
	if (!ch)
		return -ENOMEM;

	atomic_set(&(ipc->busy[channel]), 1);
	init_waitqueue_head(&(ch->rxwait));
	mutex_init(&(ch->rxlock));
	ipc->channels[channel] = ch;
	ipc->states[channel] = CHAN_STATE_OPENED;
	atomic_dec(&(ipc->busy[channel]));

	return 0;
}
EXPORT_SYMBOL(aud_smsg_ch_open);

int aud_smsg_ch_close(u8 dst, uint16_t channel)
{
	struct aud_smsg_ipc *ipc = NULL;
	struct aud_smsg_channel *ch = NULL;
	int count = 0;

	if (!aud_smsg_ipcs[dst])
		return 0;

	ipc = aud_smsg_ipcs[dst];
	ch = ipc->channels[channel];
	if (!ch)
		return 0;

	ipc->states[channel] = CHAN_STATE_FREE;
	/* maybe channel has been free for aud_smsg_ch_open failed */
	if (ipc->channels[channel]) {
		/* guarantee that channel resource isn't used in irq handler */
		while (atomic_read(&ipc->busy[channel])
			&& (count < WAIT_CHAN_WAKE_UP_COUNT)) {
			pr_err("channel %d, is busy\n", channel);
			wake_up(&ch->rxwait);
			count++;
			msleep(1000);
		}
		kfree(ch);
		ipc->channels[channel] = NULL;
	}

	/* finally, update the channel state*/
	ipc->states[channel] = CHAN_STATE_UNUSED;

	return 0;
}
EXPORT_SYMBOL(aud_smsg_ch_close);

static int aud_smsg_ch_send(struct aud_smsg_ipc *ipc, struct aud_smsg *msg)
{
	u32 wr;
	struct aud_smsg_channel *ch;

	if (msg->channel >= AMSG_CH_NR)
		return -1;

	atomic_inc(&(ipc->busy[msg->channel]));
	if (ipc->states[msg->channel] == CHAN_STATE_OPENED) {
		ch = ipc->channels[msg->channel];
		if ((int)(readl_relaxed((void *)ch->wrptr)
			- readl_relaxed((void *)ch->rdptr)) >= SMSG_CACHE_NR) {
			pr_err("aud_smsg_recv timeout and command full\n");
		} else {
			/* write smsg to cache */
			wr = readl_relaxed((void *)ch->wrptr)
				& (SMSG_CACHE_NR - 1);
			unalign_memcpy(&(ch->caches[wr]), msg,
				       sizeof(struct aud_smsg));
			writel_relaxed(readl_relaxed((void *)ch->wrptr) + 1,
				ch->wrptr);
		}
		wake_up_interruptible_all(
			&(ipc->channels[msg->channel]->rxwait));
	}
	atomic_dec(&(ipc->busy[msg->channel]));

	return 0;
}

static noinline int aud_smsg_all_ch_notify(struct aud_smsg_ipc *ipc,
					  struct aud_smsg *msg)
{
	int i = 0;
	unsigned long flags;

	spin_lock_irqsave(&(ipc->ch_rx_pinlock), flags);
	for (i = 0; i < AMSG_CH_NR; i++) {
		msg->channel = i;
		aud_smsg_ch_send(ipc, msg);
	}
	spin_unlock_irqrestore(&(ipc->ch_rx_pinlock), flags);

	return 0;
}

/* deklarasi provider (dibuat tools/re/fix_provider_proto.py; menutup -Wmissing-prototypes) */
int aud_smsg_wakeup_all_ch(struct aud_smsg_ipc *ipc);
int aud_smsg_wakeup_all_ch(struct aud_smsg_ipc *ipc)
{
	int i = 0;

	for (i = 0; i < AMSG_CH_NR; i++) {
		if (ipc->states[i] == CHAN_STATE_OPENED)
			wake_up_interruptible_all(&(ipc->channels[i]->rxwait));
	}

	return 0;
}

int aud_smsg_wakeup_ch(u8 dst, u8 channel)
{
	int i;
	struct aud_smsg_ipc *ipc;

	ipc = aud_smsg_ipcs[dst];
	if (!ipc)
		return -ENODEV;

	for (i = 0; i < AMSG_CH_NR; i++) {
		if (ipc->states[i] == CHAN_STATE_OPENED &&
			channel == i) {
			ipc->wakeup[i] = 1;
			wake_up_interruptible_all(&(ipc->channels[i]->rxwait));
		}
	}
	return 0;
}
EXPORT_SYMBOL(aud_smsg_wakeup_ch);

int aud_smsg_send(u8 dst, struct aud_smsg *msg)
{
	unsigned long long command;
	unsigned long long channel;
	unsigned long long parameter0;
	struct aud_smsg_ipc *ipc;
	uintptr_t txpos;
	int rval = 0;
	unsigned long flags;
	/*
	 * The stock blob keeps the last SMSG_TX_RING_MSG_COUNT packed mailbox
	 * words here (0xa0 bytes = 20 * u64) and hands the newest one to the
	 * channel, instead of calling mbox_raw_sent() with the id.  It is a
	 * debug ring: only msg_index walks it.  Nothing in this file reads it
	 * back, so gcc drops the static unless it is marked used.
	 *
	 * The asm label pins the stock's own symbol name.  clang 5.4 spells a
	 * function-static "<enclosing function>.<member>"; gcc numbers its
	 * own ("msg_val.1") and nothing in the tree maps one to the other, so
	 * without the label the verifier reads the variable as missing.
	 */
	static unsigned long long msg_val[SMSG_TX_RING_MSG_COUNT]
		__asm__("aud_smsg_send.msg_val") __attribute__((used));

	pr_info("%s: dst=%d, channel=%d\n", __func__, dst, msg->channel);
	ipc = aud_smsg_ipcs[dst];
	if (!ipc)
		return -ENODEV;

	if (!ipc->channels[msg->channel]) {
		pr_err(" ERR: channel is null, %s, channel %d not ope\n",
			__func__, msg->channel);
		return -ENODEV;
	}

	if ((ipc->states[msg->channel] != CHAN_STATE_OPENED) ||
		(ipc->dsp_ready == false)) {
		pr_err("ERR: %s, channel %d not opened, ipc state : %d!,ipc->dsp_ready %d\n",
			__func__, msg->channel, ipc->states[msg->channel],
			ipc->dsp_ready);
		return -EINVAL;
	}

	pr_debug("%s: command=%d, channel=%d, parameter0=0x%08x,parameter1=0x%08x, parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);

	spin_lock_irqsave(&(ipc->txpinlock), flags);
	if ((int)(readl_relaxed((void *)ipc->txbuf_wrptr) -
		readl_relaxed((void *)ipc->txbuf_rdptr)) >= ipc->txbuf_size) {
		pr_warn("%s txbuf is full!\n", __func__);
		rval = -EBUSY;
		goto send_failed;
	}
	if (ipc->dsp_ready == false) {
		pr_err("%s: dsp not ready\n", __func__);
		goto send_failed;
	}
	/* calc txpos and write smsg */
	txpos = (readl_relaxed((void *)ipc->txbuf_wrptr)
		& (ipc->txbuf_size - 1))
		*sizeof(struct aud_smsg) + ipc->txbuf_addr;
	unalign_memcpy((void *)txpos, msg, sizeof(struct aud_smsg));

	sp_asoc_pr_dbg("%s: wrptr=%u, rdptr=%u, txpos=0x%lx\n",
			__func__, readl_relaxed((void *)ipc->txbuf_wrptr),
			readl_relaxed((void *)ipc->txbuf_rdptr), txpos);

	/* update wrptr */
	writel_relaxed(readl_relaxed((void *)ipc->txbuf_wrptr) + 1,
		(void *) ipc->txbuf_wrptr);

	command = msg->command;
	channel = msg->channel;
	parameter0 = msg->parameter0;
	/*
	 * The stock blob stores the packed word in a 20-entry ring before
	 * ringing the channel (its 5.4 mbox_send_message() took the channel
	 * alone; the word went out with it).  mbox_raw_sent() is this tree's
	 * equivalent of that pair - it splits the word and calls
	 * mbox_send_message() + mbox_chan_txdone() - so the ring keeps the
	 * history and the transport stays the same.
	 */
	msg_val[msg_index] = (command << 48) | (channel << 32) | parameter0;
	if (mbox_raw_sent(ipc->target_id, msg_val[msg_index]) < 0)
		pr_err("%s: mbox_send_message failed\n", __func__);
	msg_index = (msg_index + 1) % SMSG_TX_RING_MSG_COUNT;

send_failed:
	spin_unlock_irqrestore(&(ipc->txpinlock), flags);

	return rval;
}
EXPORT_SYMBOL(aud_smsg_send);

int aud_smsg_recv(u8 dst, struct aud_smsg *msg, int timeout)
{
	struct aud_smsg_ipc *ipc = aud_smsg_ipcs[dst];
	struct aud_smsg_channel *ch;
	u32 rd;
	int rval = 0;

	if (!ipc) {
		pr_err("%s no ipc  init failed\n", __func__);
		return -ENODEV;
	}

	atomic_inc(&(ipc->busy[msg->channel]));

	ch = ipc->channels[msg->channel];

	if (!ch) {
		pr_err("%s, channel %d not opened!\n", __func__, msg->channel);
		atomic_dec(&(ipc->busy[msg->channel]));
		return -ENODEV;
	}

	sp_asoc_pr_dbg("%s: dst=%d, channel=%d, timeout=%d\n",
			__func__, dst, msg->channel, timeout);

	if (timeout == 0) {
		if (!mutex_trylock(&(ch->rxlock))) {
			sp_asoc_pr_info("%s busy!%d\n", __func__,
					ipc->busy[msg->channel].counter);
			atomic_dec(&(ipc->busy[msg->channel]));
			return -EBUSY;
		}

		/* no wait */
		if (readl_relaxed((void *)ch->wrptr)
			== readl_relaxed((void *)ch->rdptr)) {
			sp_asoc_pr_info("warning %s cache is empty!\n",
			__func__);
			rval = -ENODATA;
			goto recv_failed;
		}
	} else if (timeout < 0) {
		mutex_lock(&(ch->rxlock));
		/* wait forever */
		rval = wait_event_interruptible(ch->rxwait,
				(readl_relaxed((void *)ch->wrptr) !=
				readl_relaxed((void *)ch->rdptr)) ||
				(ipc->states[msg->channel] == CHAN_STATE_FREE)
				|| (ipc->dsp_ready == false) ||
				ipc->wakeup[msg->channel]);
		if (rval < 0) {
			sp_asoc_pr_info("%s wait interrupted!\n", __func__);
			goto recv_failed;
		}

		if ((ipc->states[msg->channel] == CHAN_STATE_FREE)
			|| (readl_relaxed((void *)ch->wrptr)
			== readl_relaxed((void *)ch->rdptr))) {
			pr_info("warning %s smsg channel %d is free ipc->dsp_ready %d!\n",
				__func__, msg->channel, ipc->dsp_ready);
			if (ipc->dsp_ready == false)
				rval = -EPIPE;
			else
				rval = -EIO;
			goto recv_failed;
		}
	} else {
		mutex_lock(&(ch->rxlock));
		/* wait timeout */
		rval = wait_event_interruptible_timeout(ch->rxwait,
			(readl_relaxed((void *)ch->wrptr)
			!= readl_relaxed((void *)ch->rdptr))
			|| (ipc->states[msg->channel] == CHAN_STATE_FREE)
			|| (ipc->dsp_ready == false), timeout);
		if (rval < 0) {
			sp_asoc_pr_info("warning %s wait interrupted!\n",
				__func__);
			goto recv_failed;
		} else if (rval == 0) {
			unsigned long flags_tx, flags_rx;
			struct aud_smsg msg;
			u32 is_timeout = 1;

			sp_asoc_pr_info("warning %s wait timeout!\n", __func__);
			rval = -ETIME;
			pr_err("audio_smsg.c: timeout and reset dsp\n");
			spin_lock_irqsave(&(ipc->txpinlock), flags_tx);
			spin_lock_irqsave(&(ipc->rxpinlock), flags_rx);
			ipc->dsp_ready = false;
			aud_smsg_dump_func(is_timeout);
			aud_smsg_reset(ipc);
			msg.channel = AMSG_CH_DSP_BTHAL;
			msg.command = AGDSP_COMMUNICATION_TIMEOUT;
			msg.parameter0 = AGDSP_COMMUNICATION_TIMEOUT;
			aud_smsg_ch_send(ipc, (struct aud_smsg *)&msg);
			msg.channel = AMSG_CH_DSP_ASSERT_CTL;
			aud_smsg_ch_send(ipc, (struct aud_smsg *)&msg);
			spin_unlock_irqrestore(&(ipc->rxpinlock), flags_rx);
			spin_unlock_irqrestore(&(ipc->txpinlock), flags_tx);
			aud_smsg_wakeup_all_ch(ipc);
			goto recv_failed;
		}

		if ((ipc->states[msg->channel] == CHAN_STATE_FREE)
			|| (readl_relaxed((void *)ch->wrptr)
				== readl_relaxed((void *)ch->rdptr))) {
			pr_err("%s smsg channel is free!,dsp_ready= %d, channel %d\n",
				__func__, ipc->dsp_ready, msg->channel);
			rval = -EIO;
			goto recv_failed;
		}
	}

	/* read smsg from cache */
	rd = readl_relaxed((void *)ch->rdptr) & (SMSG_CACHE_NR - 1);
	unalign_memcpy(msg, (&(ch->caches[rd])), sizeof(struct aud_smsg));
	writel_relaxed(readl_relaxed((void *)ch->rdptr) + 1, ch->rdptr);

	sp_asoc_pr_dbg("%s: wrptr=%d, rdptr=%d, rd=%d\n",
			__func__, (unsigned int) readl_relaxed(ch->wrptr),
			(unsigned int)readl_relaxed(ch->rdptr), rd);
	pr_debug("%s: command=%d, channel=%d, parameter0=0x%08x,parameter1=0x%08x, parameter2=0x%08x, parameter3=0x%08x\n",
			__func__, msg->command, msg->channel,
			msg->parameter0, msg->parameter1,
			msg->parameter2, msg->parameter3);

recv_failed:
	ipc->wakeup[msg->channel] = 0;
	mutex_unlock(&(ch->rxlock));
	atomic_dec(&(ipc->busy[msg->channel]));

	return rval;
}
EXPORT_SYMBOL(aud_smsg_recv);

#if defined(CONFIG_DEBUG_FS)
static int aud_smsg_debug_show(struct seq_file *m, void *private)
{
	struct aud_smsg_ipc *spc = NULL;
	int i, j;

	for (i = 0; i < AUD_IPC_NR; i++) {
		spc = aud_smsg_ipcs[i];
		if (!spc)
			continue;
		seq_printf(m, "sipc: %s:\n", spc->name);
		seq_printf(m, "dst: 0x%0x\n", spc->dst);
		seq_printf(m, "txbufAddr: 0x%0x, txbufsize: 0x%0x,txbufrdptr: [0x%0x]=%u, txbufwrptr: [0x%0x]=%u\n",
			   (u32)spc->txbuf_addr,
			   spc->txbuf_size,
			   (u32)spc->txbuf_rdptr,
			   readl_relaxed((void *)spc->txbuf_rdptr),
			   (u32)spc->txbuf_wrptr,
			   readl_relaxed((void *)spc->txbuf_wrptr));
		seq_printf(m, "rxbufAddr: 0x%0x, rxbufsize: 0x%0x,rxbufrdptr: [0x%0x]=%u, rxbufwrptr: [0x%0x]=%u\n",
			  (u32) spc->rxbuf_addr,
			  spc->rxbuf_size,
			 (u32)spc->rxbuf_rdptr,
			 (u32) readl_relaxed((void *)spc->rxbuf_rdptr),
			 (u32) spc->rxbuf_wrptr,
			 (u32)readl_relaxed((void *)spc->rxbuf_wrptr));

		for (j = 0;  j < AMSG_CH_NR; j++) {
			seq_printf(m, "channel[%d] states: %d\n",
				j, spc->states[j]);
		}
	}

	return 0;
}

static int aud_smsg_debug_open(struct inode *inode,
	struct file *file)
{
	return single_open(file, aud_smsg_debug_show,
		inode->i_private);
}

static const struct file_operations aud_smsg_debug_fops = {
	.open = aud_smsg_debug_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

/* deklarasi provider (dibuat tools/re/fix_provider_proto.py; menutup -Wmissing-prototypes) */
int aud_smsg_init_debugfs(void *root);
int aud_smsg_init_debugfs(void *root)
{
	if (!root)
		return -ENXIO;
	debugfs_create_file("audio-smsg-sipc",
		0444, (struct dentry *)root,
		NULL, &aud_smsg_debug_fops);

	return 0;
}

#endif /* CONFIG_DEBUG_FS */

static int aud_smsg_syscore_suspend(void)
{
	int ret = 0;/* has_wake_lock(WAKE_LOCK_SUSPEND) ? -EAGAIN : 0; */
	return ret;
}

static struct syscore_ops aud_smsg_syscore_ops = {
	.suspend	= aud_smsg_syscore_suspend,
};

/* deklarasi provider (dibuat tools/re/fix_provider_proto.py; menutup -Wmissing-prototypes) */
int aud_smsg_suspend_init(void);
int  aud_smsg_suspend_init(void)
{
	register_syscore_ops(&aud_smsg_syscore_ops);

	return 0;
}

MODULE_AUTHOR("Chen Gaopeng");
MODULE_DESCRIPTION("Audio SIPC/SMSG driver");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL(aud_smsg_init_debugfs);
EXPORT_SYMBOL(aud_smsg_ipc_create);
EXPORT_SYMBOL(aud_smsg_ipc_destroy);
EXPORT_SYMBOL(aud_smsg_irq_handler);
EXPORT_SYMBOL(aud_smsg_suspend_init);
EXPORT_SYMBOL(aud_smsg_wakeup_all_ch);
