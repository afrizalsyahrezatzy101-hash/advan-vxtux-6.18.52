/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * REPLACEMENT FILE — prepared 2026-10-20 against VXTux 6.18.52.
 *
 * Drop-in for drivers/net/wireless/sprd/sprdwcn/platform/wcn_txrx.c.
 *
 * Strategy: take Ran-Thegoth/uwe5622 donor (a working Linux 6.x reference for
 * the same UWE5622 / AW859A chip used by Unisoc T618), and graft it onto the
 * VXTux runtime surface:
 *
 *   - VXTux include paths preserved: <misc/wcn_bus.h>, "bufring.h",
 *     "loopcheck.h", "wcn_glb.h", "wcn_procfs.h", "mdbg_type.h",
 *     "../include/wcn_dbg.h", <wcn-618-compat.h>, <linux/module.h>.
 *     PCIE: "edma_engine.h" (path added by sprdwcn/platform/Makefile ccflags-y).
 *
 *   - VXTux structs preserved: ring_device (with rw_wake_lock / rw_lock /
 *     mdbg_read_mutex / rx_head / rx_task / flag_smp), ring_rx_data,
 *     plus the static ring_reg_flag atomic and the LOG_BUF_NUM/LOG_BUF_SIZE
 *     PCIE DMA prep path.
 *
 *   - VXTux runtime preserved: mdbg_ring_rx_task(), mdbg_comm_write(),
 *     mdbg_tx_cb(), mdbg_send()/mdbg_receive(), mdbg_ring_init(),
 *     mdbg_ring_remove(), mdbg_pt_ring_reg/unreg(), prepare_free_buf_for_log(),
 *     free_prepare_buf(), mdbg_content_len(), mdbg_read_release(),
 *     mdbg_rx_count_change().
 *
 *   - Donor signature WIN: pop_link is unconditionally mdbg_log_read(), with
 *     the SAME signature as upstream (int channel, struct mbuf_t *head,
 *     struct mbuf_t *tail, int num). This drops the broken
 *     mdbg_log_cb / mdbg_sipc_log_cb split that VXTux introduced.
 *
 * Why the donor's mdbg_log_read replaces VXTux's mdbg_log_cb:
 *
 *   VXTux had:
 *
 *     #if IS_ENABLED(CONFIG_WCN_PCIE)
 *       static int mdbg_log_cb(...) {  /* synchronous, in IRQ ctx */ }
 *       static int mdbg_log_push(...) { edma_dump_*... }
 *     #if IS_ENABLED(CONFIG_WCN_SIPC)
 *       static int mdbg_sipc_log_cb(...) { /* SIPC variant */ }
 *     #else
 *       static int mdbg_log_read(...) { /* workqueue, non-PCIe */ }
 *     #endif
 *
 *   Three problems with that layout against the current kernel tree:
 *
 *     1. sprdwcn/sipc/wcn_sipc.c provides its OWN pop_link callbacks
 *        (tx_pop_link/rx_pop_link in wcn_sipc_test.c); nothing in the
 *        kernel tree references mdbg_sipc_log_cb. The symbol is orphaned.
 *
 *     2. The PCIE path's mdbg_log_cb writes the ring synchronously inside
 *        IRQ context and never pushes the buf list back — sprdwcn_bus_push_list
 *        is called, but the workqueue hop that does the PUB_HEAD_RSV +
 *        bus_puh_t extraction is missing, so PCIE log mbufs accumulate
 *        until the pool drains. The workqueue path (mdbg_ring_rx_task) handles
 *        both PCIE and non-PCIE uniformly via sprdwcn_bus_push_list.
 *
 *     3. The PCIE #if/.chn_config.sipc_ch initializer references the SIPC
 *   union only on the non-PCIE branch — but on the PCIE branch the struct
 *   still gets a default-initialized chn_config; harmless, but the dual
 *   pop_link functions force mdbg_log_push to be defined in two flavors.
 *
 *   Donor collapses both branches to mdbg_log_read; we follow suit.
 */

#include <misc/wcn_bus.h>

#include "bufring.h"
#if IS_ENABLED(CONFIG_WCN_PCIE)
#include "edma_engine.h"
#endif
#include "loopcheck.h"
#include "wcn_glb.h"
#include "wcn_procfs.h"
#include "mdbg_type.h"
#include "../include/wcn_dbg.h"
#include <wcn-618-compat.h>	/* 5.4 -> 6.18 shim */
#include <linux/module.h>

#define	LOG_BUF_NUM	16
#define	LOG_BUF_SIZE	1500

static struct ring_device *ring_dev;
static unsigned long long rx_count;
static unsigned long long rx_count_last;
static atomic_t ring_reg_flag;

/*
 * Single pop_link callback used by both PCIE and non-PCIE mchn_ops_t below.
 * Donor signature: (int channel, struct mbuf_t *head, struct mbuf_t *tail, int num).
 * Defer the actual ring write to a workqueue so we never kmalloc / mutex
 * inside the IRQ context the bus layer invokes us from.
 */
static int mdbg_log_read(int channel, struct mbuf_t *head,
			 struct mbuf_t *tail, int num)
{
	struct ring_rx_data *rx;

	if (atomic_read(&ring_reg_flag) == 0) {
		WCN_INFO("mdbg ring has do unreg, so discard it\n");
		sprdwcn_bus_push_list(channel, head, tail, num);
		return 0;
	}

	if (ring_dev) {
		mutex_lock(&ring_dev->mdbg_read_mutex);
		rx = kmalloc(sizeof(*rx), GFP_KERNEL);
		if (!rx) {
			WCN_ERR("mdbg ring low memory\n");
			mutex_unlock(&ring_dev->mdbg_read_mutex);
			sprdwcn_bus_push_list(channel, head, tail, num);
			return 0;
		}
		mutex_unlock(&ring_dev->mdbg_read_mutex);
		spin_lock_bh(&ring_dev->rw_lock);
		rx->channel = channel;
		rx->head = head;
		rx->tail = tail;
		rx->num = num;
		list_add_tail(&rx->entry, &ring_dev->rx_head);
		spin_unlock_bh(&ring_dev->rw_lock);
		schedule_work(&ring_dev->rx_task);
	}

	return 0;
}

#if IS_ENABLED(CONFIG_WCN_PCIE)
/*
 * Push side: PCIE eDMA completion. No log mbufs to return; just observe
 * the channel registers so a hung DMA shows up in dmesg.
 */
static int mdbg_log_push(int chn, struct mbuf_t **head,
			 struct mbuf_t **tail, int *num)
{
	WCN_INFO("%s enter num=%d, chn=%d, mbuf used done\n",
		 __func__, *num, chn);
	edma_dump_glb_reg();
	edma_dump_chn_reg(chn);

	return 0;
}
#endif

#if IS_ENABLED(CONFIG_WCN_PCIE)
static struct mchn_ops_t mdbg_ringc_ops = {
	.channel = WCN_RING_RX,
	.inout = WCNBUS_RX,
	.hif_type = 1,
	/* bt cp log buffer size: 360*4, so setting as 1500 */
	.buf_size = 1500,
	/* bt log buf num + pld buf num = 4 + 8 */
	.pool_size = 16,
	.cb_in_irq = 0,
	.pop_link = mdbg_log_read,
	.push_link = mdbg_log_push,
};
#else
static struct mchn_ops_t mdbg_ringc_ops = {
	.channel = WCN_RING_RX,
	.inout = WCNBUS_RX,
	.pool_size = 1,
	.pop_link = mdbg_log_read,
};
#endif

bool mdbg_rx_count_change(void)
{
	rx_count = sprdwcn_bus_get_rx_total_cnt();

	WCN_DBG("rx_count:0x%llx rx_count_last:0x%llx\n",
		rx_count, rx_count_last);

	if ((rx_count == 0) && (rx_count_last == 0)) {
		return true;
	} else if (rx_count != rx_count_last) {
		rx_count_last = rx_count;
		return true;
	} else {
		return false;
	}
}

int mdbg_read_release(unsigned int fifo_id)
{
	return 0;
}

long mdbg_content_len(void)
{
	if (unlikely(!ring_dev))
		return 0;

	return mdbg_ring_readable_len(ring_dev->ring);
}

static long int mdbg_comm_write(char *buf,
				size_t len, unsigned int subtype)
{
	unsigned char *send_buf = NULL;
	char *str = NULL;
	struct mbuf_t *head = NULL;
	struct mbuf_t *tail = NULL;
	int num = 1;
	size_t rsvlen;

	if (unlikely(marlin_get_module_status() != true)) {
		WCN_WARN("WCN module have not open\n");
		return -EIO;
	}
#if IS_ENABLED(CONFIG_WCN_SIPC)
	rsvlen = 0;
#else
	rsvlen = PUB_HEAD_RSV;
#endif
	send_buf = kzalloc(len + rsvlen + 1, GFP_KERNEL);
	if (!send_buf)
		return -ENOMEM;
	memcpy(send_buf + rsvlen, buf, len);

	str = strstr(send_buf + rsvlen, SMP_HEAD_STR);
	if (!str)
		if (len > ARMLOG_HEAD - 1)
			str = strstr(send_buf + rsvlen + ARMLOG_HEAD,
				     SMP_HEAD_STR);

	if (str) {
		int ret;

		/* for arm log to pc */
		WCN_INFO("smp len:%lu,str:%s\n", len, str);
		str[sizeof(SMP_HEAD_STR)] = 0;
		ret = kstrtol(&str[sizeof(SMP_HEAD_STR) - 1], 10,
			      &ring_dev->flag_smp);
		WCN_INFO("smp ret:%d, flag_smp:%ld\n", ret,
			 ring_dev->flag_smp);
		kfree(send_buf);
	} else {
		if (!sprdwcn_bus_list_alloc(
			    mdbg_proc_ops[MDBG_AT_TX_OPS].channel,
			    &head, &tail, &num)) {
			head->buf = send_buf;
			head->len = len;
			head->next = NULL;
			sprdwcn_bus_push_list(
				mdbg_proc_ops[MDBG_AT_TX_OPS].channel,
				head, tail, num);
		}
	}

	return len;
}

static void mdbg_ring_rx_task(struct work_struct *work)
{
	struct ring_rx_data *rx = NULL;
	struct mdbg_ring_t *ring = NULL;
	struct mbuf_t *mbuf_node;
	int i;
#if !IS_ENABLED(CONFIG_WCN_PCIE)
	struct bus_puh_t *puh = NULL;
#endif

	if (unlikely(!ring_dev)) {
		WCN_ERR("ring_dev is NULL\n");
		return;
	}

	spin_lock_bh(&ring_dev->rw_lock);
	rx = list_first_entry_or_null(&ring_dev->rx_head,
				      struct ring_rx_data, entry);
	if (rx) {
		list_del(&rx->entry);
	} else {
		WCN_WARN("tasklet something err\n");
		spin_unlock_bh(&ring_dev->rw_lock);
		return;
	}
	if (!list_empty(&ring_dev->rx_head))
		schedule_work(&ring_dev->rx_task);
	ring = ring_dev->ring;
	spin_unlock_bh(&ring_dev->rw_lock);

	for (i = 0, mbuf_node = rx->head; i < rx->num; i++,
	     mbuf_node = mbuf_node->next) {
#if !IS_ENABLED(CONFIG_WCN_PCIE)
		rx->addr = mbuf_node->buf + PUB_HEAD_RSV;
		puh = (struct bus_puh_t *)mbuf_node->buf;
		mdbg_ring_write(ring, rx->addr, puh->len);
#else
		mdbg_ring_write(ring, mbuf_node->buf, mbuf_node->len);
#endif
	}
	sprdwcn_bus_push_list(mdbg_ringc_ops.channel,
			      rx->head, rx->tail, rx->num);
	wake_up_log_wait();
	kfree(rx);
}

/* deklarasi provider (dibuat tools/re/fix_provider_proto.py; menutup -Wmissing-prototypes) */
long int mdbg_send(char *buf, size_t len, unsigned int subtype);
long int mdbg_send(char *buf, size_t len, unsigned int subtype)
{
	long int sent_size = 0;

	WCN_DEBUG("BYTE MODE");
	__pm_stay_awake(ring_dev->rw_wake_lock);
	sent_size = mdbg_comm_write(buf, len, subtype);
	__pm_relax(ring_dev->rw_wake_lock);

	return sent_size;
}
EXPORT_SYMBOL_GPL(mdbg_send);

long int mdbg_receive(void *buf, int len)
{
	return mdbg_ring_read(ring_dev->ring, buf, len);
}

int mdbg_tx_cb(int channel, struct mbuf_t *head,
	       struct mbuf_t *tail, int num)
{
#if !IS_ENABLED(CONFIG_WCN_PCIE)
	struct mbuf_t *mbuf_node;
	int i;

	mbuf_node = head;
	for (i = 0; i < num; i++, mbuf_node = mbuf_node->next) {
		kfree(mbuf_node->buf);
		mbuf_node->buf = NULL;
	}
#endif
	WCN_INFO("%s, chn:%d\n", __func__, channel);
	/* PCIe buf is witebuf[], not kmalloc, no need to free */
	sprdwcn_bus_list_free(channel, head, tail, num);

	return 0;
}

#if IS_ENABLED(CONFIG_WCN_PCIE)
static struct dma_buf log_buf[LOG_BUF_NUM];

static int free_prepare_buf(struct dma_buf *dm)
{
	struct wcn_pcie_info *pcie_dev;

	pcie_dev = get_wcn_device_info();
	if (!pcie_dev) {
		WCN_ERR("%s:PCIE device link error\n", __func__);
		return -1;
	}

	if (dm->vir && dm->phy)
		dmfree(pcie_dev, dm);

	return 0;
}

int prepare_free_buf_for_log(int chn, int size, int num)
{
	int ret, i;
	struct mbuf_t *mbuf, *head, *tail;
	struct wcn_pcie_info *pcie_dev;

	pcie_dev = get_wcn_device_info();
	if (!pcie_dev) {
		WCN_ERR("%s:PCIE device link error\n", __func__);
		return -1;
	}
	ret = sprdwcn_bus_list_alloc(chn, &head, &tail, &num);
	if (ret != 0)
		return -1;
	for (i = 0, mbuf = head; i < num; i++) {
		ret = dmalloc(pcie_dev, &log_buf[i], size);
		if (ret != 0)
			return -1;
		mbuf->buf = (unsigned char *)(log_buf[i].vir);
		mbuf->phy = (unsigned long)(log_buf[i].phy);
		mbuf->len = log_buf[i].size;
		memset(mbuf->buf, 0x0, mbuf->len);
		mbuf = mbuf->next;
		WCN_DBG("dma_alloc_coherent(0x%x) vir=0x%lx, phy=0x%lx\n",
			 log_buf[i].size, log_buf[i].vir, log_buf[i].phy);
	}

	ret = sprdwcn_bus_push_list(chn, head, tail, num);

	return ret;
}
#endif

void mdbg_pt_ring_reg(void)
{
	WCN_INFO("%s\n", __func__);
	atomic_set(&ring_reg_flag, 0x1);
	sprdwcn_bus_chn_init(&mdbg_ringc_ops);
#if IS_ENABLED(CONFIG_WCN_PCIE)
	prepare_free_buf_for_log(15, LOG_BUF_SIZE, LOG_BUF_NUM);
#endif
}

void mdbg_pt_ring_unreg(void)
{
#if IS_ENABLED(CONFIG_WCN_PCIE)
	int i;
#endif

	WCN_INFO("%s\n", __func__);
	atomic_set(&ring_reg_flag, 0x0);
	sprdwcn_bus_chn_deinit(&mdbg_ringc_ops);
#if IS_ENABLED(CONFIG_WCN_PCIE)
	for (i = 0; i < LOG_BUF_NUM; i++)
		free_prepare_buf(&log_buf[i]);
#endif
}

int mdbg_ring_init(void)
{
	int err = 0;

	ring_dev = kmalloc(sizeof(struct ring_device), GFP_KERNEL);
	if (!ring_dev)
		return -ENOMEM;

	ring_dev->ring = mdbg_ring_alloc(MDBG_RX_RING_SIZE);
	if (!(ring_dev->ring)) {
		WCN_ERR("Ring malloc error.");
		kfree(ring_dev);
		return -MDBG_ERR_MALLOC_FAIL;
	}

	ring_dev->rw_wake_lock =
		wakeup_source_register(NULL, "mdbg_wake_lock");
	spin_lock_init(&ring_dev->rw_lock);
	mutex_init(&ring_dev->mdbg_read_mutex);
	INIT_LIST_HEAD(&ring_dev->rx_head);
	INIT_WORK(&ring_dev->rx_task, mdbg_ring_rx_task);
	ring_dev->flag_smp = 0;
#if !IS_ENABLED(CONFIG_WCN_PCIE)
	mdbg_pt_ring_reg();
#endif
	WCN_INFO("success!");

	mdbg_dev->ring_dev = ring_dev;

	return err;
}

void mdbg_ring_remove(void)
{
	struct ring_rx_data *pos, *next;

	MDBG_FUNC_ENTERY;
#if !IS_ENABLED(CONFIG_WCN_PCIE)
	mdbg_pt_ring_unreg();
#endif
	wakeup_source_unregister(ring_dev->rw_wake_lock);
	cancel_work_sync(&ring_dev->rx_task);
	mdbg_ring_destroy(ring_dev->ring);
	list_for_each_entry_safe(pos, next, &ring_dev->rx_head, entry) {
		list_del(&pos->entry);
		kfree(pos);
	}
	kfree(ring_dev);
	ring_dev = NULL;
}

/* 6.18/modpost: modul WAJIB menyatakan lisensi; build built-in (=y) tidak
 * memeriksa ini, jadi cacatnya baru muncul saat objek dibangun sebagai modul
 * (`.ko`) - ditemukan lewat build modular WCN 2026-09-18. */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Unisoc WCN port VXTux (6.18) - donor-merged txrx");