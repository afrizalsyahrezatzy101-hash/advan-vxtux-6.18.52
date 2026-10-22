"""Undo v32: the drivers/sound/ tree is a duplicate, not a missing chain.

What v32 got wrong
-------------------
v32 concluded that drivers/sound/ was never built because
drivers/sound/Makefile and drivers/sound/soc/Makefile were missing,
and created them. That was wrong.

The tree has TWO copies of the Unisoc ASoC port:

  drivers/vxtux/sound/asoc/   card/ codec/ codec_sc2730/ dai_v4/
                              machine/ platform/   <- the LIVE one
  drivers/sound/soc/sprd/     card/ codec/ codec_sc2730/ dai_v4/
                              machine/ platform/   <- the DEAD one

and the live one is reached through drivers/vxtux/Makefile:

  obj-$(CONFIG_VXTUX_SND_SPRD_ASOC) += sound/

Checked on the build tree:

  $ ls sound/core/*.o
  sound/core/compress_offload.o  sound/core/control.o  ...
  $ find drivers/vxtux/sound -name '*.o' | head
  drivers/vxtux/sound/asoc/codec/dummy-codec.o
  drivers/vxtux/sound/asoc/card/sprd-asoc-card-utils.o  ...
  $ find drivers/sound -name '*.o' | head
  (nothing)

So the ASoC core (sound/core/, at the root of the tree) and the
Unisoc port (drivers/vxtux/sound/asoc/) are BOTH built. The copy
under drivers/sound/ has never produced a single object, and v32
made it build - which would have compiled every ASoC driver TWICE,
once from each tree, with the same symbol names.

There is no missing Makefile chain: drivers/sound/ is not a kernel
subtree at all in this tree, it is a stray copy.

The fix
-------
- delete drivers/sound/ entirely
- delete the drivers/sound/Makefile and drivers/sound/soc/Makefile that
  v32 created

The audio helpers that v32 also removed from drivers/sound/soc/sprd/
were already being built from drivers/vxtux/audio/, so that part of
v32 was harmless, but the directories are gone now either way.

The apipe module
----------------
sprd_apipe.ko is a real gap: the stock ROM ships lib/modules/
sprd_apipe.ko, and the source is drivers/sound/soc/sprd/audio_pipe/
(which v32 was about to build). Before deleting the tree, the source
is moved to drivers/vxtux/audio/audio_pipe/ - the tree that IS built -
and the module is wired there behind CONFIG_SPRD_APIPE.

The source there (drivers/vxtux/audio/audio_pipe/audio_pipe.c) has
the apipe_* API, the same one the stock blob has:

  $ nm donors/stock_vendor_modules/sprd_apipe.ko | grep ' T \| t '
  t apipe_ioctl
  t apipe_open
  t apipe_poll
  t apipe_probe
  t apipe_read
  t apipe_release
  t apipe_write
  t apipe_remove

Usage:
    python3 fix6_r.py [--apply]
"""
import os
import shutil
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

SOUND_DIR = "drivers/sound"
VXTUX_PIPE = "drivers/vxtux/audio/audio_pipe"
DEAD_PIPE = "drivers/sound/soc/sprd/audio_pipe"

PIPE_KCONFIG = """# SPDX-License-Identifier: GPL-2.0
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

PIPE_MAKEFILE = """# The composite module is named after the vendor blob it replaces
# (sprd_apipe.ko). The stock module's own name is sprd_apipe, and it
# exports nothing - apipe_ioctl, apipe_open, apipe_poll, ... are all
# local to it.
#
# NOTE: a single-object module must NOT be written as a composite
# whose object has the same name as the module - kbuild then uses the
# module's own .o as the link input and ld refuses:
#
#   aarch64-linux-gnu-ld: input file '.../audio_pipe.o' is the same
#   as output file
#
# The source came from drivers/sound/soc/sprd/audio_pipe/, a copy of
# the Unisoc ASoC tree that was never built - the live tree is
# drivers/vxtux/sound/asoc/.

obj-$(CONFIG_SPRD_APIPE) += sprd_apipe.o
sprd_apipe-y := audio_pipe.o
"""


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) move the apipe source into the live tree
    # ------------------------------------------------------------------
    print("== 1) the apipe source moves to the live tree ==")
    src = os.path.join(TREE, DEAD_PIPE, "audio_pipe.c")
    dst = os.path.join(TREE, VXTUX_PIPE, "audio_pipe.c")
    if os.path.exists(src) and not os.path.exists(dst):
        if APPLY:
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.copyfile(src, dst)
            print(f"   copied {DEAD_PIPE}/audio_pipe.c -> {VXTUX_PIPE}/")
        else:
            print("   (dry run) would copy it")
    elif os.path.exists(dst):
        print(f"   {VXTUX_PIPE}/audio_pipe.c already exists")
    else:
        print(f"   !! neither {DEAD_PIPE}/audio_pipe.c nor "
              f"{VXTUX_PIPE}/audio_pipe.c exists")

    # the Makefile and the Kconfig
    p = os.path.join(TREE, VXTUX_PIPE, "Makefile")
    if APPLY:
        write(p, PIPE_MAKEFILE)
        print(f"   WROTE {VXTUX_PIPE}/Makefile")
    else:
        print("   (dry run)")
    p = os.path.join(TREE, VXTUX_PIPE, "Kconfig")
    if APPLY:
        write(p, PIPE_KCONFIG)
        print(f"   WROTE {VXTUX_PIPE}/Kconfig")
    else:
        print("   (dry run)")

    # the parent must descend into it
    p = os.path.join(TREE, "drivers/vxtux/audio/Makefile")
    txt = read(p)
    if "audio_pipe/" in txt:
        print("   the parent already descends into audio_pipe/")
    else:
        if APPLY:
            write(p, txt.rstrip("\n") +
                  "\n\n# The apipe character device, the replacement for the vendor\n"
                  "# sprd_apipe.ko. obj-\$(CONFIG_...), not obj-y: a directory in\n"
                  "# obj-y is always descended into unconditionally, and the stock\n"
                  "# ROM ships sprd_apipe.ko as a loadable module.\n"
                  "obj-$(CONFIG_SPRD_APIPE)\t\t+= audio_pipe/\n")
            print("   WROTE drivers/vxtux/audio/Makefile")
        else:
            print("   (dry run) would add the audio_pipe line")

    # ------------------------------------------------------------------
    # 2) the Kconfig has to be sourced
    # ------------------------------------------------------------------
    print()
    print("== 2) drivers/vxtux/audio/Kconfig: source the apipe Kconfig ==")
    p = os.path.join(TREE, "drivers/vxtux/audio/Kconfig")
    txt = read(p)
    if "audio_pipe/Kconfig" in txt:
        print("   already sourced")
    else:
        if APPLY:
            write(p, txt.rstrip("\n") +
                  "\n\n# The apipe character device, the replacement for the vendor\n"
                  "# sprd_apipe.ko.\n"
                  'source "drivers/vxtux/audio/audio_pipe/Kconfig"\n')
            print("   WROTE drivers/vxtux/audio/Kconfig")
        else:
            print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) delete the dead tree
    # ------------------------------------------------------------------
    print()
    print("== 3) delete the dead drivers/sound/ tree ==")
    p = os.path.join(TREE, SOUND_DIR)
    if not os.path.isdir(p):
        print("   already gone")
    else:
        print("   the tree contains:")
        for root, dirs, fs in os.walk(p):
            rel = os.path.relpath(root, p)
            n = len(fs)
            if n:
                print(f"     {rel}/ ({n} files)")
        if APPLY:
            shutil.rmtree(p)
            print(f"   removed {SOUND_DIR}/")
        else:
            print("   (dry run) would remove it")

    # ------------------------------------------------------------------
    # 4) the config
    # ------------------------------------------------------------------
    print()
    print("== 4) .config: CONFIG_SPRD_APIPE=m ==")
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    if "CONFIG_SPRD_APIPE=m" in d:
        print("   already m")
    else:
        import re
        m = re.search(r"^(# )?CONFIG_SPRD_APIPE( is not set|=.*)$", d, re.M)
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

    # ------------------------------------------------------------------
    # 5) verify
    # ------------------------------------------------------------------
    print()
    print("== verify ==")
    print("   drivers/sound/ gone:",
          not os.path.isdir(os.path.join(TREE, SOUND_DIR)))
    print("   the apipe source is in the live tree:",
          os.path.exists(os.path.join(TREE, VXTUX_PIPE, "audio_pipe.c")))
    t = read(os.path.join(TREE, "drivers/vxtux/audio/Makefile"))
    print("   the parent descends into it:", "audio_pipe/" in t)


if __name__ == "__main__":
    main()
