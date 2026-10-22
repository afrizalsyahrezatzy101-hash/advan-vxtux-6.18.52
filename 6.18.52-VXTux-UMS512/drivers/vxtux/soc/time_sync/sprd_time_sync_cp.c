// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Spreadtrum Communications Inc.
 *
 * sprd_time_sync_cp - the control point of the time correction device.
 *
 * The module is loaded on demand and offers a single entry point, so
 * that a user space daemon can ask for a fresh time snapshot without
 * opening the character device.
 */

#include <linux/module.h>

/* the snapshot push lives in the sprd_time_sync module */
void sprd_send_ap_time(void);

int sprd_time_sync_fn(void)
{
	sprd_send_ap_time();
	return 1;
}
EXPORT_SYMBOL(sprd_time_sync_fn);

MODULE_DESCRIPTION("sprd_time_sync control point");
MODULE_LICENSE("GPL");
