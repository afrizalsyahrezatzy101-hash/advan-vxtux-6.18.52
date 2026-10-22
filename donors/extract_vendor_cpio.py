"""Extract the stock vendor ramdisk (cpio) and its .ko modules from
G:/UMS512/twrp_zip/vendor_boot_stock.img.

The SPRD vendor_boot image stores the vendor ramdisk as a RAW (uncompressed)
cpio newc archive starting a few bytes into the first page after the
header. The 0x184C2102 lz4 magic that appears at the same place is part
of the vendor container framing, not the actual compression.

Usage:
    python3 extract_vendor_cpio.py [image] [outdir]
"""
import hashlib
import mmap
import os
import struct
import sys

IMG = sys.argv[1] if len(sys.argv) > 1 else \
    "G:/UMS512/twrp_zip/vendor_boot_stock.img"
OUTDIR = sys.argv[2] if len(sys.argv) > 2 else \
    "E:/vxtux_build_repo/donors/stock_vendor_modules"
os.makedirs(OUTDIR, exist_ok=True)


def list_cpio(data, off=0):
    """Walk a newc cpio archive from off, yielding (name, data, mode)."""
    n = len(data)
    count = 0
    while off + 110 <= n:
        if data[off:off + 6] != b"070701":
            break
        try:
            fields = [int(data[off + i * 8:off + (i + 1) * 8], 16)
                      for i in range(13)]
        except ValueError:
            break
        mode = fields[1]
        filesize = fields[6]
        namesize = fields[11]
        name_off = off + 110
        name = data[name_off:name_off + namesize - 1].decode("utf-8", "replace")
        data_off = (name_off + namesize + 3) & ~3
        if name == "TRAILER!!!":
            break
        blob = data[data_off:data_off + filesize]
        yield name, blob, mode
        off = (data_off + filesize + 3) & ~3
        count += 1


def main():
    f = open(IMG, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    print(f"{IMG}: {len(mm)} bytes")

    start = mm.find(b"070701", 0x1000)
    print(f"cpio newc at 0x{start:x}")

    mods = []
    total = 0
    for name, blob, mode in list_cpio(mm, start):
        base = os.path.basename(name)
        if ".ko" in base:
            # strip compression suffix for the .ko name
            p = os.path.join(OUTDIR, base)
            with open(p, "wb") as g:
                g.write(blob)
            kname = base.split(".ko")[0] + ".ko"
            mods.append((kname, len(blob), name, mode))
            total += len(blob)

    print(f"\nmodules extracted: {len(mods)}, {total:,} bytes")
    for kname, sz, name, mode in sorted(mods, key=lambda x: -x[1]):
        print(f"  {kname:<48} {sz:>10,}  mode={oct(mode)}")

    with open("E:/vxtux_build_repo/donors/stock_modules.txt", "w",
              encoding="utf-8") as g:
        for kname, sz, name, mode in sorted(mods):
            g.write(f"{kname[:-3]}\t{sz}\t{name}\n")
    print("\nwrote donors/stock_modules.txt")
    mm.close()


if __name__ == "__main__":
    main()
