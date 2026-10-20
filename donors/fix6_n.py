"""Build sprd_apipe.ko, the seventh missing stock blob.

What the stock ROM ships
------------------------
Two different audio-pipe blobs, with different APIs:

  lib/modules/audio-pipe.ko    name=audio_pipe    aud_pipe_ioctl,
                                                    aud_pipe_open, ...
  lib/modules/sprd_apipe.ko    name=sprd_apipe    apipe_ioctl,
                                                    apipe_open, apipe_poll, ...

The tree has a source for each:

  drivers/vxtux/audio/audiosipc/audio-pipe.c   -> aud_pipe_*  -> audio-pipe.ko
  drivers/vxtux/audio/audio_pipe/audio_pipe.c -> apipe_*     -> sprd_apipe.ko

The second one was never built, for two reasons:

  1. drivers/vxtux/audio/Makefile does not descend into audio_pipe/,
     so the directory is dead.
  2. The copy that IS descended into -
     drivers/sound/soc/sprd/audio_pipe/ - is gated on
     CONFIG_SPRD_AUDIO_MEMORY_MANAGER (built-in) rather than on a
     module symbol, so it can never be a .ko, and it builds the same
     apipe_* driver under the name sprd_apipe.o.

The two audio_pipe.c copies differ, so they are not interchangeable:
drivers/sound/soc/sprd/audio_pipe/audio_pipe.c is the one that the
sound tree descends into, and drivers/vxtux/audio/audio_pipe/ is the
vxtux one. This change makes the vxtux one the module, and leaves
the sound tree's copy alone (it is built-in, and the sound tree needs
its own copy of the driver for the card's sake).

Wait - that would build the same driver twice, once built-in and
once as a module, which is what the v29 comment in
drivers/vxtux/audio/Makefile warns about:

  # The apipe driver lives in drivers/sound/soc/sprd/audio_pipe/
  # (the corrected copy) and is built as sprd_apipe.ko there.
  # A second copy here would build the same driver twice.

So the right move is the other way round: the sound tree's copy is
the one that should become the module, because that is the copy the
stock blob was recovered from, and the vxtux copy stays out of the
build.

The sound tree's copy needs a module symbol of its own instead of
borrowing CONFIG_SPRD_AUDIO_MEMORY_MANAGER (which is built-in and
unrelated to the pipe).

Usage:
    python3 fix6_n.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

LIVE_DIR = "drivers/sound/soc/sprd/audio_pipe"
DEAD_DIR = "drivers/vxtux/audio/audio_pipe"


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) the live copy's Makefile: its own module symbol
    # ------------------------------------------------------------------
    print("== 1) drivers/sound/soc/sprd/audio_pipe/Makefile ==")
    rel = LIVE_DIR + "/Makefile"
    p = os.path.join(TREE, rel)
    want = """# SPRD audio pipe (blob: sprd_apipe)
# Port 2026-10-04 from gpl-source/fork_t618/.../drivers/misc/sprd_audio/audio_pipe/
#
# The composite module is named after the vendor blob it replaces
# (sprd_apipe.ko). The stock module's own name is sprd_apipe and it
# exports nothing - apipe_ioctl, apipe_open, apipe_poll, ... are all
# local to it.
#
# It is gated on CONFIG_SPRD_APIPE, a symbol of its own. It used to
# borrow CONFIG_SPRD_AUDIO_MEMORY_MANAGER, which is a different
# driver (the audio memory manager) and is built into vmlinux, so
# this could never be a loadable module - and the stock ROM ships
# sprd_apipe.ko as one.
obj-$(CONFIG_SPRD_APIPE) += sprd_apipe.o
sprd_apipe-y := audio_pipe.o
ccflags-y += -I$(srctree)/drivers/sound/soc/sprd/include
"""
    if APPLY:
        write(p, want)
        print(f"   WROTE {rel}")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 2) the dead copy: it is not descended into, so it stays out
    # ------------------------------------------------------------------
    print()
    print("== 2) drivers/vxtux/audio/audio_pipe/ ==")
    p = os.path.join(TREE, DEAD_DIR, "Makefile")
    if os.path.exists(p):
        want = """# NOT BUILT.
#
# The apipe driver that replaces the stock sprd_apipe.ko is built
# from drivers/sound/soc/sprd/audio_pipe/ - that is the copy the
# stock blob was recovered from, and it is where the module lives.
#
# This copy is kept for reference only: building it would build the
# same driver a second time.
"""
        if APPLY:
            write(p, want)
            print(f"   WROTE {DEAD_DIR}/Makefile")
        else:
            print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) the Kconfig: CONFIG_SPRD_APIPE
    # ------------------------------------------------------------------
    print()
    print("== 3) a Kconfig for CONFIG_SPRD_APIPE ==")
    kp = os.path.join(TREE, LIVE_DIR, "Kconfig")
    want = """# SPDX-License-Identifier: GPL-2.0
config SPRD_APIPE
	tristate "SPRD audio pipe (apipe)"
	depends on HAS_IOMEM
	help
	  The apipe character device, the replacement for the vendor
	  sprd_apipe.ko.

	  Ported 2026-10-04 from the vendor sources in
	  work/donorcatalog/mods-sprd/sprd/common/audio/audio_pipe/.
	  The register/dispatch contract was recovered from the stock
	  blob with radare2.

	  The stock ROM ships lib/modules/sprd_apipe.ko, so this has to
	  be a loadable module: the blob is insmod'ed by the stock init
	  scripts and the loader looks the file up by name.
"""
    if APPLY:
        write(kp, want)
        print(f"   WROTE {LIVE_DIR}/Kconfig")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 4) the sound tree's Kconfig has to source it
    # ------------------------------------------------------------------
    print()
    print("== 4) drivers/sound/soc/sprd/Kconfig: source the apipe Kconfig ==")
    rel = "drivers/sound/soc/sprd/Kconfig"
    p = os.path.join(TREE, rel)
    txt = read(p)
    if 'audio_pipe/Kconfig' in txt:
        print("   already sourced")
    else:
        lines = txt.rstrip("\n").split("\n")
        lines.append("")
        lines.append("# The apipe character device, the replacement for the vendor")
        lines.append("# sprd_apipe.ko.")
        lines.append('source "drivers/sound/soc/sprd/audio_pipe/Kconfig"')
        if APPLY:
            write(p, "\n".join(lines) + "\n")
            print(f"   WROTE {rel}")
        else:
            print("   (dry run)")

    # ------------------------------------------------------------------
    # 5) the config
    # ------------------------------------------------------------------
    print()
    print("== 5) .config: CONFIG_SPRD_APIPE=m ==")
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    m = re.search(r"^(# )?CONFIG_SPRD_APIPE( is not set|=.*)$", d, re.M)
    print(f"   before: {m.group(0) if m else 'ABSENT'}")
    if m and m.group(0) == "CONFIG_SPRD_APIPE=m":
        print("   already m")
    else:
        if m:
            d = re.sub(r"^(# )?CONFIG_SPRD_APIPE( is not set|=.*)$",
                       "CONFIG_SPRD_APIPE=m", d, count=1, flags=re.M)
        else:
            d = d.rstrip("\n") + "\nCONFIG_SPRD_APIPE=m\n"
        if APPLY:
            write(cp, d)
            print("   WROTE .config")
        else:
            print("   (dry run)")


if __name__ == "__main__":
    main()
