"""Decide which VXTux source corresponds to which stock blob, by
comparing the symbols.

The stock ROM shipped audio_sipc.ko and audio-pipe.ko as two separate
blobs, and the tree has TWO candidate sources for the pipe:
  drivers/vxtux/audio/audiosipc/audio-pipe.c
  drivers/vxtux/audio/audio_pipe/audio_pipe.c

Which one is the stock audio-pipe.ko? The stock blobs' own symbol
lists answer that: the blob exports the symbols the source defines.

Usage:
    python3 which_source_for_blob.py
"""
import os
import re
import subprocess

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
STOCK_DIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

# the candidate sources for the audio pipe
CANDIDATES = {
    "audio-pipe.ko": [
        "drivers/vxtux/audio/audiosipc/audio-pipe.c",
        "drivers/vxtux/audio/audio_pipe/audio_pipe.c",
        "drivers/sound/soc/sprd/audio_pipe/audio_pipe.c",
    ],
    "audio_sipc.ko": [
        "drivers/vxtux/audio/audiosipc/audio-sipc.c",
        "drivers/vxtux/audio/audiosipc/audio-smsg.c",
    ],
    "sprd_apipe.ko": [
        "drivers/sound/soc/sprd/audio_pipe/audio_pipe.c",
        "drivers/vxtux/audio/audio_pipe/audio_pipe.c",
    ],
}


def nm_syms(path, kinds=("export",)):
    r = subprocess.run([NM, path], capture_output=True, text=True,
                       timeout=300)
    if r.returncode != 0:
        return None, None
    ex, im = set(), set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) < 2:
            continue
        typ = p[-2] if len(p) >= 3 else p[0]
        name = p[-1]
        if typ == "U":
            im.add(name)
        elif typ.isalpha() and typ.isupper():
            ex.add(name)
    return ex, im


def src_defines(path):
    """The global function names a .c file defines."""
    txt = open(os.path.join(TREE, path), "r", encoding="utf-8",
               errors="replace").read()
    names = set()
    for m in re.finditer(
            r"^(?:[A-Za-z_][\w \t\*]*?)\b([a-z_][a-z0-9_]*)\s*\([^;{]*\)\s*\{",
            txt, re.M):
        names.add(m.group(1))
    return names


def main():
    for blob, cands in CANDIDATES.items():
        sp = os.path.join(STOCK_DIR, blob)
        if not os.path.exists(sp):
            print(f"== {blob}: no stock blob")
            continue
        se, si = nm_syms(sp)
        print("=" * 90)
        print(f"== {blob}: {len(se)} exports, {len(si)} imports")
        print("   stock exports:", sorted(se))
        for c in cands:
            p = os.path.join(TREE, c)
            if not os.path.exists(p):
                print(f"   - {c}: NOT PRESENT")
                continue
            d = src_defines(c)
            hit = d & se
            print(f"   - {c}")
            print(f"       defines {len(d)} functions, "
                  f"{len(hit)} of them are stock exports: "
                  f"{sorted(hit)[:10]}")
        print()


if __name__ == "__main__":
    main()
