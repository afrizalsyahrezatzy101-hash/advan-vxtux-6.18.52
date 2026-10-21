/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2026 Spreadtrum Communications Inc.
 */

#ifndef _LINUX_SPRD_SYSTIMER_H
#define _LINUX_SPRD_SYSTIMER_H

#include <linux/types.h>

/* the 26MHz counter in the always-on domain */
u32 sprd_systimer_read(void);

/* the 32kHz counter in the always-on domain */
u64 sprd_sysfrt_read(void);

/*
 * Convert a counter value into a boot time in nanoseconds.
 *
 * timer_type 0 is the 26MHz systimer, 1 is the 32kHz sysfrt counter.
 */
u64 sprd_systimer_to_boottime(u64 cnt, int timer_type);

/*
 * Copy the conversion parameters out for a reader that wants to do
 * the conversion itself.
 *
 *   para[0] the boot time of the reference
 *   para[1] the systimer counter at the reference
 *   para[2] the sysfrt counter at the reference
 *   para[3] the systimer multiplier
 *   para[4] the sysfrt multiplier
 */
void get_convert_para(u64 *para);

#endif /* _LINUX_SPRD_SYSTIMER_H */
