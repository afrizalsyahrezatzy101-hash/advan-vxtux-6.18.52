// SPDX-License-Identifier: GPL-2.0-only
/*
 * The VSP register table, for the VSP video driver.
 *
 * This is the driver-side counterpart of
 * drivers/pmdomain/sprd/vsp_regs.c. The header declares
 * `extern struct register_gpr regs[]`, so every module that indexes
 * the table needs its own definition.
 *
 * It must NOT be exported: this driver is built into vmlinux
 * (CONFIG_SPRD_VSP=y) and an exported vmlinux symbol that vmlinux
 * itself uses makes the link fail:
 *
 *   aarch64-linux-gnu-ld: drivers/media/platform/sprd/vsp/vsp_common.o:
 *   relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which
 *   may bind externally can not be used when making a shared object;
 *   recompile with -fPIC
 *
 * The stock blob agrees: sprd-vsp-pw-domain.ko neither defines nor
 * imports `regs` (checked with nm on the blob pulled from the
 * device), and no stock .ko imports it.
 *
 * The table is filled in by vsp_parse_dt() at probe time.
 */

#include "vsp_common.h"

struct register_gpr regs[ARRAY_SIZE(tb_name)];
