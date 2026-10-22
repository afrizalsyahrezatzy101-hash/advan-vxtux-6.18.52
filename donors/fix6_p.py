"""Restore drivers/sound/Makefile and remove the duplicated audio trees.

What was wrong
--------------
drivers/sound/ has no Makefile and drivers/sound/soc/ has no
Makefile. kbuild descends into a directory only when its parent lists
it, so NOTHING under drivers/sound/ was ever compiled. The 196-module
build contained no object from drivers/sound/ at all.

The stack under drivers/sound/soc/sprd/ splits in two:

  A. the ASoC proper - card, codec, codec_sc2730, dai_v4, machine,
     platform - whose Kconfig symbols are VXTUX_SND_SPRD_* and live in
     drivers/sound/soc/sprd/Kconfig. These are the sound stack, and
     the Kconfig says they are built-in:

       # All bool symbols: sound stack built built-in (SOUND/SND/
       # SND_SOC=y) so snd_soc_* symbols exist in System.map and gate
       # C5 can prove link.

  B. the audio helpers - audiomem, agdsp_access, audiosipc,
     audiodspdump, audiodvfs, audiocpboot, saudio, voice_trigger,
     mcdt, audio_pipe - whose Kconfig symbols are CONFIG_SPRD_AUDIO_*
     and live in drivers/vxtux/audio/Kconfig.

Every one of the group B directories ALSO exists under
drivers/vxtux/audio/, and THAT is the copy the build uses. The copies
under drivers/sound/soc/sprd/ are near-duplicates (they differ in
the Makefile and in a couple of files) and they have never been
compiled.

So the fix is:

  - create drivers/sound/Makefile and drivers/sound/soc/Makefile, so
    the ASoC proper is descended into
  - list the group A directories in drivers/sound/soc/sprd/Makefile
    behind their VXTUX_SND_SPRD_* symbols
  - DELETE the group B duplicates under drivers/sound/soc/sprd/,
    because their symbols are declared in drivers/vxtux/audio/ and
    the vxtux copies are the ones that are built

Deleting the duplicates is the point: keeping both would build the
same driver twice, once from each tree, which is exactly the trap
the comment in drivers/vxtux/audio/Makefile warns about.

Usage:
    python3 fix6_p.py [--apply]
"""
import os
import shutil
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

SOUND_MK = "drivers/sound/Makefile"
SOC_MK = "drivers/sound/soc/Makefile"
SPRD_MK = "drivers/sound/soc/sprd/Makefile"

# group B: the audio helpers that live in drivers/vxtux/audio/ and are
# built from there. Their copies under drivers/sound/soc/sprd/ are
# duplicates and are removed.
DUPLICATES = [
    "audiomem",
    "agdsp_access",
    "audiosipc",
    "audiodspdump",
    "audiodvfs",
    "audiocpboot",
    "saudio",
    "voice_trigger",
    "mcdt",
]

# group A: the ASoC proper, with their Kconfig symbols
ASOC = [
    ("card", "VXTUX_SND_SPRD_CARD"),
    ("codec", "VXTUX_SND_SPRD_DUMMY_CODEC"),
    ("codec_sc2730", "VXTUX_SND_SPRD_SC2730"),
    ("dai_v4", "VXTUX_SND_SPRD_VBC_V4"),
    ("machine", "VXTUX_SND_SPRD_SC2730"),
    ("platform", "VXTUX_SND_SPRD_PCM_ROUTING"),
]

# audio_pipe is the odd one: the module has to be built, and the only
# copy that is wired to a symbol is the vxtux one (CONFIG_SPRD_APIPE,
# defined in drivers/sound/soc/sprd/audio_pipe/Kconfig). The vxtux
# copy of the SOURCE is the one the stock blob was recovered from.
AUDIO_PIPE_KCONFIG = """# SPDX-License-Identifier: GPL-2.0
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


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) the missing chain
    # ------------------------------------------------------------------
    print("== 1) the missing Makefile chain ==")
    for rel, content in (
        (SOUND_MK,
         "# SPDX-License-Identifier: GPL-2.0\n"
         "#\n"
         "# This tree has one sound subsystem: the Unisoc ASoC port under\n"
         "# drivers/sound/soc/sprd/.\n"
         "#\n"
         "# The file was MISSING, and kbuild descends into a directory only\n"
         "# when its parent lists it, so the whole ASoC stack was never\n"
         "# compiled at all. The 196-module build contained no object from\n"
         "# drivers/sound/.\n"
         "#\n"
         "# Same silent-descent trap as drivers/soc/sprd/modem/ and\n"
         "# drivers/vxtux/audio/audio_pipe/: the sources are there, the\n"
         "# Kconfig is there, and nothing builds them.\n"
         "\n"
         "obj-y\t\t\t\t\t+= soc/\n"),
        (SOC_MK,
         "# SPDX-License-Identifier: GPL-2.0\n"
         "#\n"
         "# The Unisoc ASoC port. See drivers/sound/Makefile for why this\n"
         "# file was missing.\n"
         "\n"
         "obj-y\t\t\t\t\t+= sprd/\n"),
    ):
        p = os.path.join(TREE, rel)
        if os.path.exists(p):
            print(f"   {rel}: already exists - not touching it")
        elif APPLY:
            write(p, content)
            print(f"   WROTE {rel}")
        else:
            print(f"   (dry run) would create {rel}")

    # ------------------------------------------------------------------
    # 2) the sprd Makefile: the ASoC directories
    # ------------------------------------------------------------------
    print()
    print("== 2) drivers/sound/soc/sprd/Makefile: the ASoC directories ==")
    p = os.path.join(TREE, SPRD_MK)
    txt = read(p)
    add = []
    for d, sym in ASOC:
        if f"{d}/" in txt:
            print(f"   {d:16s} already listed")
            continue
        add.append((d, sym))
    if add:
        block = ("\n# The ASoC proper. These were never built either: the file\n"
                 "# drivers/sound/Makefile and drivers/sound/soc/Makefile were\n"
                 "# missing, so kbuild never descended into this directory.\n"
                 "#\n"
                 "# They are built-in (the Kconfig says the sound stack is\n"
                 "# built-in so snd_soc_* symbols exist in System.map), so they\n"
                 "# are listed with obj- behind their symbol.\n")
        for d, sym in add:
            block += f"obj-$(CONFIG_{sym})\t\t+= {d}/\n"
        if APPLY:
            write(p, txt.rstrip("\n") + "\n" + block)
            print(f"   WROTE {SPRD_MK}")
            for d, sym in add:
                print(f"     + {d}/ behind CONFIG_{sym}")
        else:
            print("   (dry run)")
            for d, sym in add:
                print(f"     + {d}/ behind CONFIG_{sym}")

    # ------------------------------------------------------------------
    # 3) audio_pipe: its own symbol, and it stays
    # ------------------------------------------------------------------
    print()
    print("== 3) audio_pipe: the module has to be built ==")
    p = os.path.join(TREE, "drivers/sound/soc/sprd/audio_pipe/Kconfig")
    if APPLY:
        write(p, AUDIO_PIPE_KCONFIG)
        print("   WROTE drivers/sound/soc/sprd/audio_pipe/Kconfig")
    else:
        print("   (dry run)")
    # and the parent must descend into it behind the symbol
    p = os.path.join(TREE, SPRD_MK)
    txt = read(p)
    if "obj-$(CONFIG_SPRD_APIPE)" not in txt:
        if APPLY:
            write(p, txt.rstrip("\n") +
                  "\n\n# The apipe character device, the replacement for the vendor\n"
                  "# sprd_apipe.ko. It has to be obj-$(CONFIG_...), not obj-y: a\n"
                  "# directory in obj-y is always descended into unconditionally,\n"
                  "# and the stock ROM ships sprd_apipe.ko as a loadable module.\n"
                  "obj-$(CONFIG_SPRD_APIPE)\t\t+= audio_pipe/\n")
            print("   WROTE the sprd Makefile with the apipe line")
        else:
            print("   (dry run) would add the apipe line")

    # ------------------------------------------------------------------
    # 4) remove the duplicated group B directories
    # ------------------------------------------------------------------
    print()
    print("== 4) remove the duplicated audio helpers ==")
    for d in DUPLICATES:
        p = os.path.join(TREE, "drivers/sound/soc/sprd", d)
        if not os.path.isdir(p):
            print(f"   {d:16s} already gone")
            continue
        if APPLY:
            shutil.rmtree(p)
            print(f"   removed drivers/sound/soc/sprd/{d}/")
        else:
            print(f"   (dry run) would remove drivers/sound/soc/sprd/{d}/")

    # ------------------------------------------------------------------
    # 5) verify
    # ------------------------------------------------------------------
    print()
    print("== verify ==")
    print("   drivers/sound/Makefile:      ",
          os.path.exists(os.path.join(TREE, SOUND_MK)))
    print("   drivers/sound/soc/Makefile:   ",
          os.path.exists(os.path.join(TREE, SOC_MK)))
    t = read(os.path.join(TREE, SPRD_MK))
    for d, sym in ASOC:
        print(f"   {d:16s} listed: {f'{d}/' in t}")
    print("   audio_pipe listed:",
          "obj-$(CONFIG_SPRD_APIPE)" in t)
    for d in DUPLICATES:
        ex = os.path.isdir(os.path.join(TREE, "drivers/sound/soc/sprd", d))
        vx = os.path.isdir(os.path.join(TREE, "drivers/vxtux/audio", d))
        if ex:
            print(f"   !! {d} still in sound/soc/sprd/ (vxtux copy: {vx})")


if __name__ == "__main__":
    main()
