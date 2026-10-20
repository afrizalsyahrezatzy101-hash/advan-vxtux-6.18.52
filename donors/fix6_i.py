"""Fix the VSP 'regs' symbol properly.

What went wrong
---------------
The previous attempt (v25) moved vsp_regs.o out of the composite
module and into vmlinux (obj-y += vsp_regs.o). That made `regs` an
exported vmlinux symbol, and then:

  aarch64-linux-gnu-ld: drivers/media/platform/sprd/vsp/vsp_common.o:
  relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which may
  bind externally can not be used when making a shared object;
  recompile with -fPIC

A vmlinux symbol that "may bind externally" cannot be used by an
object that is linked into vmlinux, because vmlinux is not
position-independent. So exporting `regs` and also using it inside
vmlinux is a contradiction.

The stock module settles it. Checked with nm on the blob pulled from
the device:

  $ nm donors/stock_vendor_modules/sprd-vsp-pw-domain.ko
  d vsp_pd_driver
  d vsp_pw_on.__UNIQUE_ID_ddebug129
  $ nm -u donors/stock_vendor_modules/sprd-vsp-pw-domain.ko | grep regs
  (nothing)

The stock module neither defines nor imports `regs`. It has its own
copy, local to the module. No stock .ko imports `regs` either.

The fix
--------
Give each consumer its own local copy of the table, and stop
exporting it:

  drivers/pmdomain/sprd/vsp_regs.c      -> local `regs` for the
                                          pw-domain module
  drivers/media/platform/sprd/vsp/vsp_regs.c -> local `regs` for the
                                          built-in vsp driver

Both are `static` in the header's sense: the header declares
`extern struct register_gpr regs[]`, so each .c that defines it
provides the symbol for its own translation unit's link. The two
definitions never meet, because they are in different modules.

The header's `extern` declaration is still needed so that
sprd_vsp_pw_domain.c and sprd_vsp_main.c can index the array.

Also: drivers/pmdomain/Makefile had stopped descending into sprd/,
so vsp_regs.o was not in the build at all - the "undefined reference
to `regs'" was not about the export, it was about the definition
being absent.

Usage:
    python3 fix6_i.py [--apply]
"""
import os
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

PM_MAKEFILE = "drivers/pmdomain/Makefile"
PM_SPRD_MAKEFILE = "drivers/pmdomain/sprd/Makefile"
VSP_DIR = "drivers/media/platform/sprd/vsp"

PM_VSP_REGS = """// SPDX-License-Identifier: GPL-2.0-only
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
"""

VSP_VSP_REGS = """// SPDX-License-Identifier: GPL-2.0-only
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
"""


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) pmdomain/sprd/vsp_regs.c: local, not exported
    # ------------------------------------------------------------------
    print("== 1) pmdomain/sprd/vsp_regs.c: local, not exported ==")
    p = os.path.join(TREE, "drivers/pmdomain/sprd/vsp_regs.c")
    if APPLY:
        write(p, PM_VSP_REGS)
        print("   WROTE drivers/pmdomain/sprd/vsp_regs.c")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 2) the vsp driver's own copy
    # ------------------------------------------------------------------
    print()
    print("== 2) media/platform/sprd/vsp/vsp_regs.c: the driver's copy ==")
    p = os.path.join(TREE, VSP_DIR, "vsp_regs.c")
    if APPLY:
        write(p, VSP_VSP_REGS)
        print(f"   WROTE {VSP_DIR}/vsp_regs.c")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) the vsp Makefile: add the object
    # ------------------------------------------------------------------
    print()
    print("== 3) the vsp Makefile: build the new object ==")
    p = os.path.join(TREE, VSP_DIR, "Makefile")
    txt = read(p)
    if "vsp_regs.o" in txt:
        print("   vsp_regs.o already listed")
    else:
        want = """# The VSP video driver needs its own copy of the register table:
# vsp_common.h declares `extern struct register_gpr regs[]`, and the
# table must not be exported (an exported vmlinux symbol that vmlinux
# itself uses makes the link fail with "relocation ... may bind
# externally ... recompile with -fPIC").
#
# The stock blob agrees: sprd-vsp-pw-domain.ko neither defines nor
# imports `regs`.

sprd_vsp-objs := sprd_vsp_main.o vsp_common.o vsp_isr_func.o vsp_regs.o
obj-$(CONFIG_SPRD_VSP) += sprd_vsp.o
ccflags-y += -I$(srctree)/drivers/devfreq/apsys/ -I$(srctree)/drivers/devfreq/sprd/apsys/
"""
        if APPLY:
            write(p, want)
            print(f"   WROTE {VSP_DIR}/Makefile")
        else:
            print("   (dry run)")

    # ------------------------------------------------------------------
    # 4) pmdomain/sprd/Makefile: vsp_regs.o back into the module
    # ------------------------------------------------------------------
    print()
    print("== 4) pmdomain/sprd/Makefile ==")
    p = os.path.join(TREE, PM_SPRD_MAKEFILE)
    want = """# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (sprd-vsp-pw-domain.ko). The object used to be listed one by one,
# which made kbuild emit a module per object; the composite form keeps
# it in a single .ko with the stock file name, so the loader can
# insmod the same file.
#
# vsp_regs.o DEFINES `regs[]` for this module. It is not exported:
# the built-in VSP video driver has its own copy (see
# drivers/media/platform/sprd/vsp/vsp_regs.c), because an exported
# vmlinux symbol that vmlinux itself uses cannot be linked:
#
#   relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which
#   may bind externally can not be used when making a shared object;
#   recompile with -fPIC

obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o vsp_regs.o

obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
"""
    if APPLY:
        write(p, want)
        print("   WROTE drivers/pmdomain/sprd/Makefile")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 5) pmdomain/Makefile: descend into sprd/ again
    # ------------------------------------------------------------------
    print()
    print("== 5) pmdomain/Makefile: descend into sprd/ ==")
    p = os.path.join(TREE, PM_MAKEFILE)
    want = """# This tree only needs the generic PM domain core plus the Unisoc VSP power
# domain. The per-vendor subdirectories below are listed upstream because those
# platforms ship their own genpd drivers; this tree carries none of them and
# building them only adds unrelated failures. Removing them keeps core.o
# (genpd_dev_pm_attach and friends, which drivers/opp/core.c references) and
# adds nothing else.

# SPDX-License-Identifier: GPL-2.0-only
obj-y\t\t\t\t\t+= core.o governor.o

# sprd/ carries the VSP power-domain module (sprd-vsp-pw-domain.ko,
# which replaces the stock blob) and its register table. It has to be
# descended into, otherwise the module is never built.
obj-y\t\t\t\t\t+= sprd/
"""
    if APPLY:
        write(p, want)
        print("   WROTE drivers/pmdomain/Makefile")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 6) verify
    # ------------------------------------------------------------------
    print()
    print("== verify ==")
    for rel in ("drivers/pmdomain/sprd/vsp_regs.c",
                VSP_DIR + "/vsp_regs.c"):
        t = read(os.path.join(TREE, rel))
        has_def = "struct register_gpr regs[" in t
        has_exp = "EXPORT_SYMBOL(regs)" in t
        print(f"   {rel}: defines regs={has_def} exports regs={has_exp}")
    t = read(os.path.join(TREE, PM_MAKEFILE))
    print("   pmdomain/Makefile descends into sprd/:", "sprd/" in t)


if __name__ == "__main__":
    main()
