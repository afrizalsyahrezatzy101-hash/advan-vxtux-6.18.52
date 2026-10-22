"""Final mapping of the two audio pipe sources to the two stock blobs.

Evidence (from the symbol comparison, donors/which_source_for_blob.py):

  audio_sipc.ko   58 exports, 29 of them defined by
                  drivers/vxtux/audio/audiosipc/audio-sipc.c (15)
                  drivers/vxtux/audio/audiosipc/audio-smsg.c (14)
                  -> the composite audio_sipc.ko

  audio-pipe.ko   5 exports, none of them API. Its source defines the
                  aud_pipe_* / audio_pipe_* family:
                  drivers/vxtux/audio/audiosipc/audio-pipe.c
                  -> the composite audio-pipe.ko

  sprd_apipe.ko   5 exports, none of them API. Its source defines the
                  apipe_* family:
                  drivers/sound/soc/sprd/audio_pipe/audio_pipe.c
                  -> the composite sprd_apipe.ko

  drivers/vxtux/audio/audio_pipe/audio_pipe.c is a DUPLICATE of the
  sound/soc/sprd copy (only include ordering and a comment differ, and
  the sound/soc/sprd one is the corrected one: <linux/of.h> instead of
  <linux/of_device.h> for struct of_device_id). It is removed so the
  apipe driver is built exactly once.

Usage:
    python3 fix_audio_pipes.py [--apply]
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
    # 1) the audiosipc Makefile: audio_sipc.ko (audio-sipc.o +
    #    audio-smsg.o) and audio-pipe.ko (audio-pipe.o)
    rel = "drivers/vxtux/audio/audiosipc/Makefile"
    p = os.path.join(TREE, rel)
    txt = read(p)
    print("== audiosipc/Makefile ==")
    print(txt)

    want = f"""# SPDX-License-Identifier: GPL-2.0
#
# TWO composite modules, each named after the vendor blob it replaces:
#   audio_sipc.ko  <- audio-sipc.o audio-smsg.o   (the SIPC layer)
#   audio-pipe.ko  <- audio-pipe.o               (the chardev pipe)
#
# The stock ROM shipped these as two separate blobs, so they have to be
# two separate modules. Listing the objects one by one would make
# kbuild emit a module per object with the wrong names.
#
# The vendor include paths (drivers/staging/sprd/modem/include and
# sound/soc/sprd/include) do not exist in this tree. The headers that
# are needed are vendored: audio_mem.h (in ../audiomem) and
# sprd-string.h (in include/).

VXTUX_AUDIO_TREE := drivers/vxtux/audio
ccflags-y\t+= -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiosipc/include \\
\t\t   -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiomem/

obj-$(CONFIG_SPRD_AUDIO_SIPC) += audio_sipc.o
audio_sipc-y := audio-sipc.o audio-smsg.o

obj-$(CONFIG_SPRD_AUDIO_PIPE) += audio-pipe.o
audio-pipe-y := audio-pipe.o
"""
    if APPLY:
        write(p, want)
        print("  WROTE (audio_sipc.ko + audio-pipe.ko)")
    else:
        print("  (dry run) would write audio_sipc.ko + audio-pipe.ko")

    # 2) the duplicate apipe copy: drop it, keep the corrected one
    print()
    print("== duplicate apipe source ==")
    dup_dir = "drivers/vxtux/audio/audio_pipe"
    keep_dir = "drivers/sound/soc/sprd/audio_pipe"
    if not os.path.isdir(os.path.join(TREE, dup_dir)):
        print("  no duplicate dir")
    else:
        # point the vxtux side at the kept copy instead of compiling a
        # second copy of the same driver
        mp = os.path.join(TREE, "drivers/vxtux/audio/Makefile")
        mt = read(mp)
        new = re.sub(
            r"^obj-\$\(CONFIG_SPRD_AUDIO_PIPE\)\s*\+=\s*audio_pipe/\s*$",
            "# The apipe driver lives in drivers/sound/soc/sprd/audio_pipe/\n"
            "# (the corrected copy) and is built as sprd_apipe.ko there.\n"
            "# A second copy here would build the same driver twice.",
            mt, flags=re.M)
        if new == mt:
            print("  !! could not find the audio_pipe/ obj- line")
        else:
            if APPLY:
                write(mp, new)
                print(f"  drivers/vxtux/audio/Makefile: dropped "
                      f"{dup_dir}/")
            else:
                print(f"  (dry run) would drop {dup_dir}/ from "
                      "drivers/vxtux/audio/Makefile")

    # 3) the kept apipe copy: name it sprd_apipe.ko
    print()
    print("== sprd_apipe ==")
    kp = os.path.join(TREE, keep_dir, "Makefile")
    kt = read(kp)
    print(kt)
    new = kt.replace(
        "obj-y += audio_pipe.o",
        "# ONE composite module, named after the vendor blob it replaces\n"
        "# (sprd_apipe.ko).\n"
        "obj-$(CONFIG_SPRD_AUDIO_MEMORY_MANAGER) += sprd_apipe.o\n"
        "sprd_apipe-y := audio_pipe.o")
    if new == kt:
        print("  !! unexpected content, not changed")
    elif APPLY:
        write(kp, new)
        print(f"  {keep_dir}/Makefile -> sprd_apipe.ko")
    else:
        print(f"  (dry run) {keep_dir}/Makefile -> sprd_apipe.ko")


if __name__ == "__main__":
    main()
