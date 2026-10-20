// SPDX-License-Identifier: GPL-2.0-only
/*
 * The VSP power-domain register table.
 *
 * vsp_common.h declares `extern struct register_gpr regs[]` and both
 * sprd_vsp_pw_domain.c and the VSP video driver index it. In the 5.4
 * vendor tree the definition lives in
 * drivers/misc/sprd_vsp/vsp_common.c:32. That file is not part of
 * this port - only vsp_common.h was recovered from the donor.
 *
 * The table is populated at probe time by the VSP video driver
 * (drivers/media/platform/sprd/vsp, CONFIG_SPRD_VSP - ported
 * 2026-10-09 from the iscle_ums512 5.4 donor). Before that driver
 * loads, every entry stays {NULL, 0, 0}.
 *
 * That is the vendor's own defined fallback: vsp_pw_on()/vsp_pw_off()
 * both test `regs[PMU_VSP_AUTO_SHUTDOWN].gpr == NULL` first and return
 * -1 via the "skip power on/off" path, so no regmap call is ever made
 * on a NULL handle.
 *
 * WHY THIS IS A SEPARATE .c AND NOT AN EXPORT
 *
 * The table must NOT be exported. The built-in VSP video driver
 * (sprd_vsp_main.c) and vsp_common.c index regs[] too, and an
 * exported vmlinux symbol that is also used inside vmlinux makes the
 * link fail:
 *
 *   aarch64-linux-gnu-ld: drivers/media/platform/sprd/vsp/vsp_common.o:
 *   relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which
 *   may bind externally can not be used when making a shared object;
 *   recompile with -fPIC
 *
 * The stock blob agrees: sprd-vsp-pw-domain.ko neither defines nor
 * imports `regs` (checked with nm on the blob pulled from the
 * device), and no stock .ko imports it. Each module has its own
 * copy.
 *
 * Copyright (C) 2015--2016 Spreadtrum Communications Inc.
 */

#include "vsp_common.h"

/* MODULE_LICENSE/MODULE_DESCRIPTION butuh <linux/module.h> - vsp_common.h
 * tidak menyertakannya (modpost gagal "missing MODULE_LICENSE() in
 * vsp_regs.o" saat build device 2026-10-09). */
#include <linux/module.h>

struct register_gpr regs[ARRAY_SIZE(tb_name)];

/* modpost menuntut MODULE_LICENSE/MODULE_DESCRIPTION di SETIAP .o yang masuk
 * modul (bukan cukup di sprd_vsp_pw_domain.c) - tanpa ini build device
 * gagal "ERROR: modpost: missing MODULE_LICENSE() in vsp_regs.o". */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Unisoc VSP syscon register table for the pw-domain driver");
