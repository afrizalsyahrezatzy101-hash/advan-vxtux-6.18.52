"""Give the remaining stock blobs a replacement .ko with the exact
stock file name, using the tree's composite-module pattern.

  apsys-dvfs         drivers/devfreq/sprd/apsys/Makefile
                     sprd_dvfs_apsys.o            -> apsys-dvfs.ko

  audio_sipc         drivers/vxtux/audio/audiosipc/Makefile
                     audio-sipc.o audio-smsg.o      -> audio_sipc.ko
                     (audio-pipe.o moves out: it is the stock
                      audio-pipe.ko, not part of audio_sipc.ko)

  audio-pipe         drivers/vxtux/audio/audio_pipe/Makefile
                     audio_pipe.o                  -> audio-pipe.ko
                     (and the audiosipc/audio-pipe.c object moves here)

  sprd_apipe         drivers/sound/soc/sprd/audio_pipe/Makefile
                     audio_pipe.o                  -> sprd_apipe.ko

  sprd_flash_drv     drivers/sprd/flash/Makefile
                     flash_drv.o                   -> sprd_flash_drv.ko

  unisoc-mailbox    drivers/soc/sprd/sipc/Makefile
                     sprd_mbox_vendor.o            -> unisoc-mailbox.ko

Usage:
    python3 rename_ko2.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

# (Makefile, config symbol, new .ko name, objects that go into it)
JOBS = [
    ("drivers/devfreq/sprd/apsys/Makefile",
     "CONFIG_SPRD_APSYS_DVFS", "apsys-dvfs",
     ["sprd_dvfs_apsys.o"]),
    ("drivers/vxtux/audio/audiosipc/Makefile",
     "CONFIG_SPRD_AUDIO_SIPC", "audio_sipc",
     ["audio-sipc.o", "audio-smsg.o"]),
    ("drivers/vxtux/audio/audio_pipe/Makefile",
     "CONFIG_SPRD_AUDIO_PIPE", "audio-pipe",
     ["audio_pipe.o"]),
    ("drivers/sprd/flash/Makefile",
     "CONFIG_SPRD_FLASH_DRV", "sprd_flash_drv",
     ["flash_drv.o"]),
]

# the apipe blob: the vendor 'apipe' audio pipe
APIPE_DIR = "drivers/sound/soc/sprd/audio_pipe"

# the mailbox blob
MBOX = ("drivers/soc/sprd/sipc/Makefile",
        "CONFIG_SPRD_MAILBOX", "unisoc-mailbox",
        ["sprd_mbox_vendor.o"])


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def composite(rel, sym, ko, objs, extra_note=None):
    """Rewrite the obj- line for sym into the composite form."""
    p = os.path.join(TREE, rel)
    if not os.path.exists(p):
        print(f"  !! {rel}: missing")
        return False
    txt = read(p)
    lines = txt.split("\n")
    out = []
    found = False
    for line in lines:
        m = re.match(r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*(.+?)\s*$",
                     line)
        if m:
            cur = m.group(1).replace("\\", "").split()
            keep = [o for o in cur if o not in objs]
            if not found:
                out.append(f"obj-$(CONFIG_{sym.replace('CONFIG_', '')}) "
                           f"+= {ko}.o")
                out.append(f"{ko}-y := " + " ".join(objs))
                found = True
            if keep:
                out.append(f"obj-$(CONFIG_{sym.replace('CONFIG_', '')}) += "
                           + " ".join(keep))
            continue
        out.append(line)
    if not found:
        print(f"  !! {rel}: no obj- line for {sym}")
        return False
    head = [
        "# SPDX-License-Identifier: GPL-2.0",
        "#",
        f"# ONE composite module, named after the vendor blob it replaces",
        f"# ({ko}.ko).",
    ]
    if extra_note:
        head.append("# " + extra_note)
    head += [
        "# Listing the objects one by one would make kbuild emit a module",
        "# per object, and the stock file name would not be reproduced.",
        "",
    ]
    nt = "\n".join(head + out)
    if not nt.endswith("\n"):
        nt += "\n"
    if APPLY:
        write(p, nt)
        print(f"  {rel} -> {ko}.ko ({', '.join(objs)})")
    else:
        print(f"  (dry run) {rel} -> {ko}.ko")
    return True


def main():
    print("== composite renames ==")
    for rel, sym, ko, objs in JOBS:
        composite(rel, sym, ko, objs)

    # the apipe blob: drivers/sound/soc/sprd/audio_pipe/ needs its own
    # Makefile if it has none
    ap = os.path.join(TREE, APIPE_DIR)
    print()
    print("== sprd_apipe ==")
    if not os.path.isdir(ap):
        print(f"  !! {APIPE_DIR}: directory missing")
    else:
        files = sorted(os.listdir(ap))
        print(f"  files: {files}")
        mk = os.path.join(ap, "Makefile")
        if not os.path.exists(mk):
            body = [
                "# SPDX-License-Identifier: GPL-2.0",
                "#",
                "# ONE composite module, named after the vendor blob it",
                "# replaces (sprd_apipe.ko).",
                "",
                "obj-$(CONFIG_SPRD_AUDIO_MEMORY_MANAGER) += sprd_apipe.o",
                "sprd_apipe-y := audio_pipe.o",
                "",
            ]
            if APPLY:
                write(mk, "\n".join(body))
                print(f"  CREATED {APIPE_DIR}/Makefile")
            else:
                print(f"  (dry run) would create {APIPE_DIR}/Makefile")
        else:
            # it exists: rewrite obj-y += audio_pipe.o into the
            # composite form so the .ko is named sprd_apipe.ko
            txt = read(mk)
            new = txt
            new = re.sub(r"^obj-y\s*\+=\s*audio_pipe\.o\s*$",
                         "obj-$(CONFIG_SPRD_AUDIO_MEMORY_MANAGER) "
                         "+= sprd_apipe.o\nsprd_apipe-y := audio_pipe.o",
                         new, flags=re.M)
            if new == txt:
                print(f"  !! {APIPE_DIR}/Makefile: unexpected content")
            else:
                if APPLY:
                    write(mk, new)
                    print(f"  {APIPE_DIR}/Makefile -> sprd_apipe.ko")
                else:
                    print(f"  (dry run) {APIPE_DIR}/Makefile -> sprd_apipe.ko")

    print()
    print("== unisoc-mailbox ==")
    rel, sym, ko, objs = MBOX
    composite(rel, sym, ko, objs,
              extra_note="The mailbox shim is the first module in the "
                         "insmod order (unisoc-mailbox -> sipc-core "
                         "-> sipx -> seth/spool/spipe).")

    print()
    print("== audio-pipe: the audiosipc/audio-pipe.c object ==")
    src = os.path.join(TREE, "drivers/vxtux/audio/audiosipc/audio-pipe.c")
    if os.path.exists(src):
        print("  audiosipc/audio-pipe.c is a SECOND audio pipe, in the")
        print("  audiosipc directory, and its object (audio-pipe.o) would")
        print("  collide with the audio-pipe.ko built from")
        print("  vxtux/audio/audio_pipe/audio_pipe.o.")
        print()
        print("  The stock blobs are separate: audio_sipc.ko and")
        print("  audio-pipe.ko. So the audiosipc copy has to become the")
        print("  stock audio-pipe.ko, and vxtux/audio/audio_pipe/ (the")
        print("  apipe port) becomes sprd_apipe.ko - which is what")
        print("  drivers/sound/soc/sprd/audio_pipe/ already is.")
        print()
        rel = "drivers/vxtux/audio/audiosipc/Makefile"
        sym = "CONFIG_SPRD_AUDIO_SIPC"
        composite(rel, sym, "audio_sipc",
                  ["audio-sipc.o", "audio-smsg.o"],
                  extra_note="audio-pipe.o is left out on purpose: the "
                             "stock audio-pipe.ko is a separate blob.")
        # now the audio-pipe.o line: give it its own module name
        p = os.path.join(TREE, rel)
        txt = read(p)
        new = re.sub(
            r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*audio-pipe\.o\s*$",
            "obj-$(CONFIG_SPRD_AUDIO_PIPE) += audio-pipe.o",
            txt, flags=re.M)
        if new == txt:
            print("  !! could not find the audio-pipe.o obj- line")
        else:
            if APPLY:
                write(p, new)
                print(f"  {rel}: audio-pipe.o -> CONFIG_SPRD_AUDIO_PIPE")
            else:
                print(f"  (dry run) {rel}: audio-pipe.o -> "
                      "CONFIG_SPRD_AUDIO_PIPE")
    else:
        print("  no audiosipc/audio-pipe.c")


if __name__ == "__main__":
    main()
