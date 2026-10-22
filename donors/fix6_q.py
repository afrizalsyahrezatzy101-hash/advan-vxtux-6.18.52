"""Restore the sound stack: the Makefile chain and the symbol settings.

What was wrong
--------------
drivers/sound/ has no Makefile and drivers/sound/soc/ has no
Makefile. kbuild descends into a directory only when its parent lists
it, so NOTHING under drivers/sound/ was ever compiled. The 196-module
build contained no object from drivers/sound/ at all - the whole
Unisoc ASoC stack (card, codec, codec_sc2730, dai_v4, machine,
platform) was dead code as far as the build was concerned.

The audio helpers under drivers/sound/soc/sprd/ (audiomem,
agdsp_access, audiosipc, audiodspdump, audiodvfs, audiocpboot,
saudio, voice_trigger, mcdt) ALSO exist under drivers/vxtux/audio/,
and THAT is the copy the build uses - their Kconfig symbols are the
CONFIG_SPRD_AUDIO_* ones in drivers/vxtux/audio/Kconfig. The copies
under drivers/sound/soc/sprd/ are near-duplicates that have never
been compiled, and some of them are NOT identical (audiomem uses
vmap() where the vxtux copy uses vm_map_ram(), for instance), so
they are removed rather than left to rot.

The dependency chain
--------------------
The ASoC symbols depend on the audio helpers:

  VXTUX_SND_SPRD_CARD            (y already)
  VXTUX_SND_SPRD_DUMMY_CODEC     (y already)
  VXTUX_SND_SPRD_SC2730_POWER    (y already)
  VXTUX_SND_SPRD_PCM_ROUTING     (y already)
  VXTUX_SND_SPRD_VBC_V4          depends on SPRD_AUDIO_MCDT &&
                                 SPRD_AUDIO_SIPC && VXTUX_SND_SPRD_CARD
  VXTUX_SND_SPRD_SC2730          depends on VXTUX_SND_SPRD_CARD &&
                                 VXTUX_SND_SPRD_VBC_V4

so the helpers have to be on for the DAI and the codec to build:

  CONFIG_SPRD_AUDIO_MCDT
  CONFIG_SPRD_AUDIO_SIPC
  CONFIG_SPRD_AUDIO_MEMORY_MANAGER
  CONFIG_SPRD_AUDIO_AGDSP_ACCESS

and the helpers that the stock ROM ships as modules stay modules:

  CONFIG_SPRD_AUDIO_DSP_DUMP   -> audio-dsp-dump.ko
  CONFIG_SPRD_AUDIO_PIPE        -> audio-pipe.ko / sprd_apipe.ko
  CONFIG_SPRD_APIPE             -> sprd_apipe.ko

Usage:
    python3 fix6_q.py [--apply]
"""
import os
import re
import shutil
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

SOUND_MK = "drivers/sound/Makefile"
SOC_MK = "drivers/sound/soc/Makefile"
SPRD_MK = "drivers/sound/soc/sprd/Makefile"

# the near-duplicate audio helpers: the vxtux copies are the ones that
# are built, so these go
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

# the ASoC proper, behind their own symbols
ASOC = [
    ("card", "VXTUX_SND_SPRD_CARD"),
    ("codec", "VXTUX_SND_SPRD_DUMMY_CODEC"),
    ("codec_sc2730", "VXTUX_SND_SPRD_SC2730"),
    ("dai_v4", "VXTUX_SND_SPRD_VBC_V4"),
    ("machine", "VXTUX_SND_SPRD_SC2730"),
    ("platform", "VXTUX_SND_SPRD_PCM_ROUTING"),
]

# what the helpers and the DAI/codec need
WANT = {
    # the helpers the ASoC stack depends on
    "CONFIG_SPRD_AUDIO_MCDT": "y",
    "CONFIG_SPRD_AUDIO_SIPC": "y",
    "CONFIG_SPRD_AUDIO_MEMORY_MANAGER": "y",
    "CONFIG_SPRD_AUDIO_AGDSP_ACCESS": "y",
    # the DAI and the codec
    "CONFIG_VXTUX_SND_SPRD_VBC_V4": "y",
    "CONFIG_VXTUX_SND_SPRD_SC2730": "y",
}


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
            print(f"   {rel}: already exists")
        elif APPLY:
            write(p, content)
            print(f"   WROTE {rel}")
        else:
            print(f"   (dry run) would create {rel}")

    # ------------------------------------------------------------------
    # 2) the sprd Makefile
    # ------------------------------------------------------------------
    print()
    print("== 2) drivers/sound/soc/sprd/Makefile ==")
    p = os.path.join(TREE, SPRD_MK)
    txt = read(p)
    block = ("\n# The ASoC proper. These were never built either: the files\n"
             "# drivers/sound/Makefile and drivers/sound/soc/Makefile were\n"
             "# missing, so kbuild never descended into this directory.\n"
             "#\n"
             "# They are built-in (the Kconfig says the sound stack is built-in\n"
             "# so snd_soc_* symbols exist in System.map and gate C5 can prove\n"
             "# link), so they are listed with obj- behind their symbol.\n")
    for d, sym in ASOC:
        if f"{d}/" in txt:
            print(f"   {d:16s} already listed")
            continue
        block += f"obj-$(CONFIG_{sym})\t\t+= {d}/\n"
        print(f"   + {d}/ behind CONFIG_{sym}")
    if "obj-$(CONFIG_SPRD_APIPE)" not in txt:
        block += ("\n# The apipe character device, the replacement for the vendor\n"
                  "# sprd_apipe.ko. It has to be obj-$(CONFIG_...), not obj-y: a\n"
                  "# directory in obj-y is always descended into unconditionally,\n"
                  "# and the stock ROM ships sprd_apipe.ko as a loadable module.\n"
                  "obj-$(CONFIG_SPRD_APIPE)\t\t+= audio_pipe/\n")
        print("   + audio_pipe/ behind CONFIG_SPRD_APIPE")
    if APPLY and block.strip():
        write(p, txt.rstrip("\n") + "\n" + block)
        print(f"   WROTE {SPRD_MK}")
    elif not APPLY:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) remove the duplicated helpers
    # ------------------------------------------------------------------
    print()
    print("== 3) remove the duplicated audio helpers ==")
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
    # 4) the config
    # ------------------------------------------------------------------
    print()
    print("== 4) .config ==")
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    for sym, val in WANT.items():
        m = re.search(rf"^(# )?{sym}( is not set|=.*)$", d, re.M)
        cur = m.group(0) if m else "ABSENT"
        print(f"   {sym:38s} {cur}")
        new = f"{sym}={val}"
        if cur == new:
            continue
        if m:
            d = re.sub(rf"^(# )?{sym}( is not set|=.*)$", new, d,
                       count=1, flags=re.M)
        else:
            d = d.rstrip("\n") + "\n" + new + "\n"
        print(f"   {'':38s} -> {val}")
    if APPLY:
        write(cp, d)
        print("   WROTE .config")
    else:
        print("   (dry run)")


if __name__ == "__main__":
    main()
