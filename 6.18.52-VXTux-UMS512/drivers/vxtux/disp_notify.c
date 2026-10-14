// SPDX-License-Identifier: GPL-2.0
/*
 * vxtux_disp_notify.c — chain notifier blank/unblank gaya MSM untuk
 * driver touch vendor (himax/chipone) di kernel 6.18-VXTux-UMS512.
 * Event diemit oleh lapisan display (port DRM Fase 14).
 *
 * LinDroid/VXTux v1.0
 */
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/msm_drm_notify.h>

BLOCKING_NOTIFIER_HEAD(vxtux_disp_notify_list);
EXPORT_SYMBOL_GPL(vxtux_disp_notify_list);

/* Chain USB-state untuk driver touch vendor (chipone cts_oem) */
BLOCKING_NOTIFIER_HEAD(vxtux_usb_notify_list);
EXPORT_SYMBOL_GPL(vxtux_usb_notify_list);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("VXTux display blank/unblank notifier (MSM-compatible API)");
