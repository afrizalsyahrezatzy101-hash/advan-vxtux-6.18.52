"""Put the VSP register table back to ONE definition.

What the stock ROM actually shows
---------------------------------
The blob pulled from the device:

  $ nm donors/stock_vendor_modules/sprd-vsp-pw-domain.ko
  0000000000000434 t vsp_pw_off
  00000000000004bc t vsp_pw_on
  0000000000000000 d vsp_pw_on.__UNIQUE_ID_ddebug129
  $ strings ... | grep -E '^(name|depends|vermagic)'
  name=sprd_vsp_pw_domain
  depends=
  vermagic=5.4.210 SMP preempt mod_unload modversions aarch64

  - vsp_pw_on / vsp_pw_off are LOCAL ('t'), not exported ('T'). So
    nothing outside this module uses them.
  - The module name is sprd_vsp_pw_domain, and there is no vsp.ko in
    the stock ROM at all: the VSP video driver is built into
    vmlinux.

So the VSP video driver and the VSP power-domain driver are BOTH
built-in on the stock device, and the power-domain one is loaded
early (it is the provider of the power domains the video driver
attaches to).

The `select SPRD_VSP_PW_DOMAIN` in
drivers/media/platform/sprd/vsp/Kconfig says the same thing: a
built-in CONFIG_SPRD_VSP=y forces SPRD_VSP_PW_DOMAIN to y as well.
That is why olddefconfig kept promoting m -> y, and it is correct -
the stock ROM has no separate vsp.ko to load.

Consequences
------------
Both objects end up in vmlinux, so `regs` must be defined ONCE, not
once per module:

  - the copy in drivers/media/platform/sprd/vsp/ is removed
  - the definition stays in drivers/pmdomain/sprd/vsp_regs.c, which
    is built-in (obj-y += vsp_regs.o), and the VSP video driver uses
    it through the header's `extern` declaration

That is the state the tree had before v25, except that vsp_regs.o is
now built-in in the pmdomain directory instead of being part of the
pw-domain module - because the pw-domain module is no longer a module
(it is built-in, like the stock one).

The stock file name 'sprd-vsp-pw-domain.ko' cannot be reproduced
exactly: the stock module's own name is 'sprd_vsp_pw_domain'. The
blob is loaded by the stock init scripts by path, so the file name on
disk is what matters, not the module name inside.

Usage:
    python3 fix6_j.py [--apply]
"""
import os
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

VSP_DIR = "drivers/media/platform/sprd/vsp"
PM_MAKEFILE = "drivers/pmdomain/Makefile"
PM_SPRD_MAKEFILE = "drivers/pmdomain/sprd/Makefile"


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) the VSP driver's own copy goes away again
    # ------------------------------------------------------------------
    print("== 1) remove the duplicate copy in the vsp driver ==")
    p = os.path.join(TREE, VSP_DIR, "vsp_regs.c")
    if os.path.exists(p):
        if APPLY:
            os.remove(p)
            print("   removed drivers/media/platform/sprd/vsp/vsp_regs.c")
        else:
            print("   (dry run) would remove it")
    else:
        print("   already gone")

    # ------------------------------------------------------------------
    # 2) the vsp Makefile: back to the three original objects
    # ------------------------------------------------------------------
    print()
    print("== 2) the vsp Makefile ==")
    p = os.path.join(TREE, VSP_DIR, "Makefile")
    want = """sprd_vsp-objs := sprd_vsp_main.o vsp_common.o vsp_isr_func.o
obj-$(CONFIG_SPRD_VSP) += sprd_vsp.o
ccflags-y += -I$(srctree)/drivers/devfreq/apsys/ -I$(srctree)/drivers/devfreq/sprd/apsys/
"""
    if APPLY:
        write(p, want)
        print("   WROTE the vsp Makefile")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) pmdomain/sprd/Makefile: the definition is built-in
    # ------------------------------------------------------------------
    print()
    print("== 3) pmdomain/sprd/Makefile ==")
    p = os.path.join(TREE, PM_SPRD_MAKEFILE)
    want = """# SPDX-License-Identifier: GPL-2.0
#
# The Unisoc VSP power domain.
#
# On the stock device this is BUILT-IN, and so is the VSP video
# driver that uses it:
#
#   $ nm donors/stock_vendor_modules/sprd-vsp-pw-domain.ko
#   0000000000000434 t vsp_pw_off
#   00000000000004bc t vsp_pw_on
#   $ strings ... | grep name=
#   name=sprd_vsp_pw_domain
#
# vsp_pw_on / vsp_pw_off are LOCAL there, so nothing outside uses
# them, and the stock ROM has no vsp.ko either - the video driver is
# in vmlinux. That is why drivers/media/platform/sprd/vsp/Kconfig
# has `select SPRD_VSP_PW_DOMAIN': a built-in CONFIG_SPRD_VSP=y
# forces this symbol to y as well.
#
# vsp_regs.o DEFINES `regs[]`, which both this driver and the VSP
# video driver index. Since both are built into vmlinux, there must
# be exactly ONE definition - two would be a link error:
#
#   aarch64-linux-gnu-ld: multiple definition of `regs'
#
# It is not exported either. An exported vmlinux symbol that vmlinux
# itself uses cannot be linked - vmlinux is not position-independent:
#
#   relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which
#   may bind externally can not be used when making a shared object;
#   recompile with -fPIC
#
# The stock blob agrees: it neither defines nor imports `regs`, and
# no stock .ko imports it.

obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o

# The one and only definition of the register table.
obj-y\t\t\t\t\t+= vsp_regs.o

obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
"""
    if APPLY:
        write(p, want)
        print("   WROTE drivers/pmdomain/sprd/Makefile")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 4) the config: the pw domain is built-in
    # ------------------------------------------------------------------
    print()
    print("== 4) .config: CONFIG_SPRD_VSP_PW_DOMAIN back to =y ==")
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    d2 = d.replace("CONFIG_SPRD_VSP_PW_DOMAIN=m",
                   "CONFIG_SPRD_VSP_PW_DOMAIN=y")
    if d2 == d:
        print("   already =y")
    else:
        if APPLY:
            write(cp, d2)
            print("   m -> y")

    # ------------------------------------------------------------------
    # 5) verify
    # ------------------------------------------------------------------
    print()
    print("== verify ==")
    t = read(os.path.join(TREE, PM_SPRD_MAKEFILE))
    print("   vsp_regs.o is built-in:", "obj-y" in t and "vsp_regs.o" in t)
    print("   the vsp driver has no copy:",
          not os.path.exists(os.path.join(TREE, VSP_DIR, "vsp_regs.c")))
    t = read(os.path.join(TREE, VSP_DIR, "Makefile"))
    print("   the vsp Makefile lists 3 objects:",
          "vsp_isr_func.o" in t and "vsp_regs.o" not in t)


if __name__ == "__main__":
    main()
