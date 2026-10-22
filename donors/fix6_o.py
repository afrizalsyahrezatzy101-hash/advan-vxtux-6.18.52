"""Restore the missing drivers/sound/Makefile chain.

What was wrong
--------------
The tree has the whole Unisoc ASoC stack under drivers/sound/soc/sprd/:

  card/ codec/ dai_v4/ platform/ codec_sc2730/ machine/ mcdt/ saudio/
  voice_trigger/ audiomem/ agdsp_access/ audiosipc/ audiodspdump/
  audiocpboot/ audiodvfs/ audio_pipe/

but there is NO drivers/sound/Makefile and NO drivers/sound/soc/Makefile.
kbuild descends into a directory only when its parent lists it, so
none of that was ever compiled. The 196-module build never contained
a single object from drivers/sound/.

That is the same silent-descent trap the tree already hit with
drivers/soc/sprd/modem/ and with drivers/vxtux/audio/audio_pipe/:
the sources are there, the Kconfig is there, and nothing builds them.

The fix
-------
  drivers/sound/Makefile       -> obj-y += soc/
  drivers/sound/soc/Makefile    -> obj-y += sprd/
  drivers/sound/soc/sprd/Makefile -> the subdirectories, each behind
                                     its own CONFIG_ symbol

The sprd sound Kconfig uses the VXTUX_SND_SPRD_* symbols, and the
Makefiles use the CONFIG_SPRD_* ones, so the two have to agree. The
mapping is checked in donors/sound_chain.py.

Every subdirectory is listed with obj-$(CONFIG_...), not obj-y: a
directory in obj-y is always descended into unconditionally, and the
APIPE one has to be a real module (the stock ROM ships
sprd_apipe.ko as a loadable module).

Whether the ASoC stack itself ends up built-in or modular is decided
by the CONFIG_ symbols, and the Kconfig says the sound stack is
built-in:

  # All bool symbols: sound stack built built-in (SOUND/SND/SND_SOC=y)
  # so snd_soc_* symbols exist in System.map and gate C5 can prove link.

Usage:
    python3 fix6_o.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

SOUND_MAKEFILE = """# SPDX-License-Identifier: GPL-2.0
#
# This tree has one sound subsystem: the Unisoc ASoC port under
# drivers/sound/soc/sprd/.
#
# The file was MISSING, and kbuild descends into a directory only when
# its parent lists it, so the whole ASoC stack - card, codec, dai_v4,
# platform, codec_sc2730, machine, mcdt, saudio, voice_trigger and the
# audio helpers - was never compiled at all. The 196-module build
# contained no object from drivers/sound/.
#
# Same silent-descent trap as drivers/soc/sprd/modem/ and
# drivers/vxtux/audio/audio_pipe/: the sources are there, the Kconfig
# is there, and nothing builds them.

obj-y\t\t\t\t\t+= soc/
"""

SOC_MAKEFILE = """# SPDX-License-Identifier: GPL-2.0
#
# The Unisoc ASoC port. See drivers/sound/Makefile for why this file
# was missing.

obj-y\t\t\t\t\t+= sprd/
"""


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) drivers/sound/Makefile
    # ------------------------------------------------------------------
    print("== 1) drivers/sound/Makefile ==")
    p = os.path.join(TREE, "drivers/sound/Makefile")
    if os.path.exists(p):
        print("   already exists - not touching it")
    elif APPLY:
        write(p, SOUND_MAKEFILE)
        print("   WROTE drivers/sound/Makefile")
    else:
        print("   (dry run) would create it")

    # ------------------------------------------------------------------
    # 2) drivers/sound/soc/Makefile
    # ------------------------------------------------------------------
    print()
    print("== 2) drivers/sound/soc/Makefile ==")
    p = os.path.join(TREE, "drivers/sound/soc/Makefile")
    if os.path.exists(p):
        print("   already exists - not touching it")
    elif APPLY:
        write(p, SOC_MAKEFILE)
        print("   WROTE drivers/sound/soc/Makefile")
    else:
        print("   (dry run) would create it")

    # ------------------------------------------------------------------
    # 3) the sprd subdirectories: map the Makefile symbol to the Kconfig one
    # ------------------------------------------------------------------
    print()
    print("== 3) the sprd subdirectories ==")
    sprd = os.path.join(TREE, "drivers/sound/soc/sprd")
    kconfig = read(os.path.join(sprd, "Kconfig"))
    ksyms = set(re.findall(r"^\s*config\s+([A-Z0-9_]+)", kconfig, re.M))

    for d in sorted(os.listdir(sprd)):
        mk = os.path.join(sprd, d, "Makefile")
        if not os.path.isfile(mk):
            continue
        txt = read(mk)
        syms = re.findall(r"^obj-\$\((CONFIG_[A-Z0-9_]+)\)", txt, re.M)
        mods = re.findall(r"^obj-\$\(CONFIG_[A-Z0-9_]+\)\s*\+=\s*"
                          r"([a-z0-9_.-]+\.o)", txt, re.M)
        if not syms and not mods:
            print(f"   {d:16s} (no obj- line - checked separately)")
            continue
        for s in syms:
            # the Makefile symbol is CONFIG_SPRD_*, the Kconfig one is
            # VXTUX_SND_SPRD_*
            base = s[len("CONFIG_"):]
            alt = "VXTUX_SND_SPRD_" + base[len("SPRD_"):] \
                if base.startswith("SPRD_") else None
            where = "Kconfig" if s[len("CONFIG_"):] in ksyms else (
                "Kconfig (as %s)" % alt if alt in ksyms else "NOT IN KCONFIG")
            print(f"   {d:16s} {s:34s} -> {where}")

    # ------------------------------------------------------------------
    # 4) the sprd Makefile: the subdirectories
    # ------------------------------------------------------------------
    print()
    print("== 4) drivers/sound/soc/sprd/Makefile: the subdirectories ==")
    p = os.path.join(sprd, "Makefile")
    txt = read(p)
    # which subdirs are already listed?
    listed = set(re.findall(r"obj-\$\(CONFIG_[A-Z0-9_]+\)\s*\+=\s*"
                            r"([a-z0-9_]+)/", txt))
    objy = set(re.findall(r"obj-y\s*\+=\s*([a-z0-9_]+)/", txt))
    print("   already via obj-\$(CONFIG_):", sorted(listed))
    print("   already via obj-y:           ", sorted(objy))
    missing = []
    for d in sorted(os.listdir(sprd)):
        mk = os.path.join(sprd, d, "Makefile")
        if not os.path.isfile(mk):
            continue
        if d in listed or d in objy:
            continue
        t = read(mk)
        syms = re.findall(r"^obj-\$\((CONFIG_[A-Z0-9_]+)\)", t, re.M)
        if syms:
            missing.append((d, syms[0]))
        else:
            missing.append((d, None))
    for d, s in missing:
        print(f"   NOT LISTED: {d:16s} {s or '(no obj- line)'}")


if __name__ == "__main__":
    main()
