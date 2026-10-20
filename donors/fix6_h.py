"""Two link-time fixes, both caused by turning a driver into a module
while a BUILT-IN object still uses its symbols.

The rule: a built-in object (vmlinux) cannot reference a module's
symbol. It is the mirror image of the rule that bit us in the other
direction.

1) aud_smsg_* undefined at vmlinux link.

     drivers/vxtux/audio/agdsp_access/vxtux-smsg-shim.h:48: undefined
     reference to `aud_smsg_ch_open'

   The provider (audiosipc/audio-smsg.c) was turned into the module
   audio_sipc.ko, but drivers/sound/soc/sprd/agdsp_access/ is built-in
   and its static inline shim calls aud_smsg_ch_open() directly.

   The tree says this exact thing in drivers/soc/sprd/sipc/Makefile:

     "Inti SIPC: SENGAJA built-in saja. ... Membuat inti SIPC modular
      berarti vmlinux menunjuk simbol modul -> undefined at link."

   So the SIPC core (audio-sipc.o + audio-smsg.o) goes back to being
   built-in, and only audio-pipe.o becomes the module audio-pipe.ko.
   audio-pipe.o is the one that needs aud_smsg_*, and as a module it
   gets them from the exported symbols of the built-in provider -
   which is exactly how a module is supposed to use a built-in.

2) 'regs' undefined in sprd_vsp_main.c.

     drivers/media/platform/sprd/vsp/sprd_vsp_main.c:63: undefined
     reference to `regs'

   vsp_regs.o (which DEFINES regs[]) was moved into the composite
   module sprd-vsp-pw-domain.ko to reproduce the stock file name, but
   sprd_vsp_main.c is built-in (CONFIG_SPRD_VSP=y) and uses regs[].

   The definition has to stay in vmlinux. It is built as a built-in
   object (obj-y += vsp_regs.o) and the pw-domain module gets the
   definition it needs from vmlinux through the exported symbol
   (vsp_regs.o already does EXPORT_SYMBOL(regs)).

   The stock sprd-vsp-pw-domain.ko exports nothing, so nothing of the
   public API is lost by splitting the definition out.

Usage:
    python3 fix6_h.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) the SIPC core back to built-in
    # ------------------------------------------------------------------
    print("== 1) audiosipc: the SIPC core back to built-in ==")
    rel = "drivers/vxtux/audio/audiosipc/Makefile"
    p = os.path.join(TREE, rel)
    want = """# SPDX-License-Identifier: GPL-2.0
#
# The SIPC core (audio-sipc.o + audio-smsg.o) is BUILT-IN, on purpose.
#
# It provides aud_smsg_ch_open / aud_smsg_recv / aud_smsg_send /
# aud_smsg_ch_close, and the built-in drivers/sound/soc/sprd/
# agdsp_access/ calls them through a static inline shim
# (vxtux-smsg-shim.h). A built-in object cannot reference a module's
# symbol, so making this a module broke the vmlinux link:
#
#   vxtux-smsg-shim.h:48: undefined reference to `aud_smsg_ch_open'
#
# The SIPC core therefore stays in vmlinux, and only the pipe becomes
# a module. The pipe is the stock audio-pipe.ko; as a module it gets
# the aud_smsg_* symbols from the built-in provider's export table,
# which is how a module is supposed to use a built-in.
#
# The vendor include paths (drivers/staging/sprd/modem/include and
# sound/soc/sprd/include) do not exist in this tree. The headers that
# are needed are vendored: audio_mem.h (in ../audiomem) and
# sprd-string.h (in include/).

VXTUX_AUDIO_TREE := drivers/vxtux/audio
ccflags-y\t+= -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiosipc/include \\
\t\t   -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiomem/

obj-$(CONFIG_SPRD_AUDIO_SIPC) += audio-sipc.o audio-smsg.o

obj-$(CONFIG_SPRD_AUDIO_PIPE) += audio-pipe.o
audio-pipe-y := audio-pipe.o
"""
    if APPLY:
        write(p, want)
        print(f"  WROTE {rel}")
    else:
        print(f"  (dry run) {rel}")

    # ------------------------------------------------------------------
    # 2) the 'regs' definition stays in vmlinux
    # ------------------------------------------------------------------
    print()
    print("== 2) pmdomain/sprd: the 'regs' definition stays in vmlinux ==")
    rel = "drivers/pmdomain/sprd/Makefile"
    p = os.path.join(TREE, rel)
    want = """# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (sprd-vsp-pw-domain.ko). The object used to be listed one by one,
# which made kbuild emit a module per object; the composite form keeps
# it in a single .ko with the stock file name, so the loader can
# insmod the same file.
#
# vsp_regs.o is NOT part of that module: it DEFINES `regs[]`, and the
# built-in drivers/media/platform/sprd/vsp/sprd_vsp_main.c uses
# regs[]. A built-in object cannot reference a module's symbol:
#
#   sprd_vsp_main.c:63: undefined reference to `regs'
#
# So the definition is built into vmlinux (as vsp_regs.o) and the
# module gets it from there - vsp_regs.o already does
# EXPORT_SYMBOL(regs). The stock sprd-vsp-pw-domain.ko exports
# nothing, so no public API is lost by the split.

obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o

obj-y\t\t\t\t\t+= vsp_regs.o

obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
"""
    if APPLY:
        write(p, want)
        print(f"  WROTE {rel}")
    else:
        print(f"  (dry run) {rel}")

    # ------------------------------------------------------------------
    # 3) the parent must descend into sprd/ again
    # ------------------------------------------------------------------
    print()
    print("== 3) drivers/pmdomain/Makefile: descend into sprd/ again ==")
    rel = "drivers/pmdomain/Makefile"
    p = os.path.join(TREE, rel)
    txt = read(p)
    if "sprd/" not in txt:
        want = """# This tree only needs the generic PM domain core plus the Unisoc VSP power
# domain. The per-vendor subdirectories below are listed upstream because those
# platforms ship their own genpd drivers; this tree carries none of them and
# building them only adds unrelated failures. Removing them keeps core.o
# (genpd_dev_pm_attach and friends, which drivers/opp/core.c references) and
# adds nothing else.

# SPDX-License-Identifier: GPL-2.0-only
obj-y\t\t\t\t\t+= core.o governor.o

# sprd/ is descended into unconditionally; the drivers inside gate
# themselves on their own CONFIG_ symbols (sprd_vsp_pw_domain.ko,
# sprd_camsys_pw_domain.o, sprd_gpu_pw_domain.o, vsp_regs.o).
obj-y\t\t\t\t\t+= sprd/
"""
        if APPLY:
            write(p, want)
            print(f"  WROTE {rel}")
        else:
            print(f"  (dry run) {rel}")
    else:
        print(f"  {rel} already descends into sprd/")

    # ------------------------------------------------------------------
    # 4) the config: the SIPC core built-in again
    # ------------------------------------------------------------------
    print()
    print("== 4) .config: CONFIG_SPRD_AUDIO_SIPC back to =y ==")
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    d2 = re.sub(r"^CONFIG_SPRD_AUDIO_SIPC=m$", "CONFIG_SPRD_AUDIO_SIPC=y",
                d, flags=re.M)
    if d2 == d:
        print("   already =y (or absent)")
    else:
        if APPLY:
            write(cp, d2)
            print("   m -> y")
    if not APPLY:
        print("   (dry run)")


if __name__ == "__main__":
    main()
