"""Extract the stock kernel from the lz4-compressed region of boot_a.bin.

Android boot images with lz4 use the legacy frame format
(0x184C2102 magic) or the frame format (0x184D2204).
"""
import hashlib
import mmap
import os
import struct
import subprocess
import sys

BOOT = "G:/UMS512/boot_a.bin"
OUT = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(OUT, exist_ok=True)

LEGACY_MAGIC = b"\x02\x21\x4c\x18"
FRAME_MAGIC = b"\x04\x22\x4d\x18"


def main():
    f = open(BOOT, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

    cands = []
    idx = 0
    while True:
        i = mm.find(LEGACY_MAGIC, idx)
        if i < 0:
            break
        cands.append((i, "legacy"))
        idx = i + 1
    idx = 0
    while True:
        i = mm.find(FRAME_MAGIC, idx)
        if i < 0:
            break
        cands.append((i, "frame"))
        idx = i + 1

    print("lz4 candidates:")
    for off, kind in cands:
        print(f"  0x{off:x} ({kind})")

    if not cands:
        print("no lz4 found")
        return

    for off, kind in cands:
        blob = mm[off:off + 40 * 1024 * 1024]
        raw = os.path.join(OUT, f"lz4_{off:x}.bin")
        with open(raw, "wb") as g:
            g.write(blob)
        print(f"  dumped {raw} ({len(blob)} bytes)")

    mm.close()

    # try decompressing with python-lz4 or the lz4 CLI
    for off, kind in cands:
        raw = os.path.join(OUT, f"lz4_{off:x}.bin")
        out = os.path.join(OUT, f"lz4_{off:x}.out")
        r = subprocess.run(["lz4", "-d", "-f", raw, out],
                           capture_output=True, text=True)
        if r.returncode == 0 and os.path.exists(out):
            data = open(out, "rb").read()
            print(f"\n0x{off:x} ({kind}) decompressed -> {len(data)} bytes")
            print(f"  sha256 {hashlib.sha256(data).hexdigest()}")
            print(f"  first 32: {data[:32].hex()}")
            if len(data) >= 64:
                magic = data[56:60]
                print(f"  ARM magic: {magic!r} (arm64={'ARM\\x64'})")
        else:
            print(f"0x{off:x} ({kind}): lz4 CLI failed: {r.stderr.strip()[:200]}")


if __name__ == "__main__":
    main()
