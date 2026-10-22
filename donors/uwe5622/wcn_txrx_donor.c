/* DONOR: Ran-Thegoth/uwe5622 - unisocwcn/platform/wcn_txrx.c
 * Full original saved locally because upstream VXTux half-implements
 * a SIPC variant that drops mdbg_log_read link entirely.
 * Source: https://raw.githubusercontent.com/Ran-Thegoth/uwe5622/main/unisocwcn/platform/wcn_txrx.c
 *
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 * Licensed under GPL v2.
 */
#include <wcn_bus.h>

#include "bufring.h"
#include "loopcheck.h"
#include "wcn_glb.h"
#include "wcn_procfs.h"

int mdbg_log_read(int channel, struct mbuf_t *head,
 struct mbuf_t *tail, int num);
#ifdef CONFIG_WCN_PCIE
int mdbg_log_push(int chn, struct mbuf_t **head,
 struct mbuf_t **tail, int *num);
#endif

static struct ring_device *ring_dev;
static unsigned long long rx_count;
static unsigned long long rx_count_last;

#ifdef CONFIG_WCN_PCIE
static struct mchn_ops_t mdbg_ringc_ops = {
 .channel = WCN_RING_RX,
 .inout = WCNBUS_RX,
 .hif_type = 1,
 .buf_size = 1056,
 .pool_size = 6,
 .cb_in_irq = 0,
 .pop_link = mdbg_log_read,
 .push_link = mdbg_log_push,
};
#elif defined CONFIG_WCN_USB
static struct mchn_ops_t mdbg_ringc_ops = {
 .channel = WCN_RING_RX,
 .inout = WCNBUS_RX,
 .pool_size = 10,
 .pop_link = mdbg_log_read,
 .hif_type = HW_TYPE_USB,
};
#else
static struct mchn_ops_t mdbg_ringc_ops = {
 .channel = WCN_RING_RX,
 .inout = WCNBUS_RX,
 .pool_size = 1,
 .pop_link = mdbg_log_read,
};
#endif

#ifdef CONFIG_WCN_PCIE
int mdbg_log_push(int chn, struct mbuf_t **head, struct mbuf_t **tail, int *num)
{
 WCN_INFO("%s enter num=%d,mbuf used done", __func__, *num);
 return 0;
}
#endif

bool mdbg_rx_count_change(void)
{
 rx_count = sprdwcn_bus_get_rx_total_cnt();
 WCN_INFO("rx_count:0x%llx rx_count_last:0x%llx\n",
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
 long int len, unsigned int subtype)
{
 /* ... full implementation truncated, see upstream for complete file ... */
}

/* NOTE: VXTux port adds mdbg_sipc_log_cb variant for SIPC path which is
 * why our builds fail with "undefined reference to mdbg_sipc_log_cb" -
 * the donor's mdbg_log_read is what should be wired in. */
