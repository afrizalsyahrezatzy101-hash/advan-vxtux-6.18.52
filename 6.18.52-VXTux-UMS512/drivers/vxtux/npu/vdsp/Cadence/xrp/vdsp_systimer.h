/* SPDX-License-Identifier: GPL-2.0 */
/*
 * vdsp_systimer.h — compat VXTux untuk <linux/soc/sprd/sprd_systimer.h>.
 *
 * Header vendor milik driver SoC sprd_systimer (counter free-running
 * 26/32kHz) yang TIDAK ikut diport. Satu-satunya pemakaian donor di
 * seluruh subtree npu/vdsp adalah sprd_sysfrt_read() pada dua pr_debug
 * timing di xvp_main.c. Vendor sendiri punya fallback identik berbentuk
 * static inline yang mengembalikan nol ketika driver absen — kita
 * mengembalikan nanoklock monotonik supaya jejak timing tetap berguna.
 */
#ifndef VXTUX_NPU_VDSP_SYSTIMER_H
#define VXTUX_NPU_VDSP_SYSTIMER_H

#include <linux/ktime.h>

static inline u64 sprd_sysfrt_read(void)
{
	return ktime_get_ns();
}

#endif /* VXTUX_NPU_VDSP_SYSTIMER_H */
