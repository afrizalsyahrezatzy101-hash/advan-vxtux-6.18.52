"""Extract the vendor ramdisk from vendor_boot_stock.img and inventory
the .ko modules it contains (the stock vendor driver set).

vendor_boot.img layout (AOSP vendor_boot v3/v4):
  magic "VNDRBOOT" (8) | header_version u32 | page_size u32 |
  kernel_addr u32 | ramdisk_addr u32 | vendor_ramdisk_size u32 |
  cmdline 2048 | tags_addr u32 | name 16 | header_size u32 |
  dtb_size u32 | dtb_addr u64 | ...
Payload: vendor ramdisk (cpio gz) then DTB.
"""
import gzip
import hashlib
import mmap
import os
import struct
import sys

IMG = "G:/UMS512/twrp_zip/vendor_boot_stock.img"
OUTDIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"
os.makedirs(OUTDIR, exist_ok=True)


def list_cpio(data):
    """Walk a newc cpio archive, yielding (name, data)."""
    off = 0
    n = len(data)
    while off + 110 <= n:
        if data[off:off + 6] != b"070701":
            # try to resync: search forward
            nxt = data.find(b"070701", off)
            if nxt < 0 or nxt - off > 4096:
                break
            off = nxt
            continue
        hdr = data[off:off + 110].decode("ascii", "replace")
        try:
            fields = [int(hdr[i * 8:(i + 1) * 8], 16) for i in range(13)]
        except ValueError:
            break
        namesize = fields[11]
        filesize = fields[6]
        name_off = off + 110
        name = data[name_off:name_off + namesize - 1].decode("utf-8", "replace")
        # data follows, 4-byte aligned
        data_off = name_off + namesize
        data_off = (data_off + 3) & ~3
        if name == "TRAILER!!!":
            break
        yield name, data[data_off:data_off + filesize]
        off = (data_off + filesize + 3) & ~3


def main():
    f = open(IMG, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    print(f"{IMG}: {len(mm)} bytes")
    print("magic:", mm[:8])

    # find gzip streams (the vendor ramdisk is cpio.gz)
    idx = 0
    cands = []
    while True:
        i = mm.find(b"\x1f\x8b\x08", idx)
        if i < 0:
            break
        cands.append(i)
        idx = i + 1
    print(f"gzip candidates: {[hex(c) for c in cands[:12]]}")

    import zlib
    best = None
    for off in cands:
        try:
            d = zlib.decompressobj(16 + zlib.MAX_WBITS)
            out = d.decompress(mm[off:off + 200 * 1024 * 1024])
            out += d.flush()
        except Exception:
            continue
        n_ko = out.count(b".ko")
        print(f"  0x{off:x} -> {len(out)} bytes, '.ko' occurrences: {n_ko}")
        if n_ko > 5 and (best is None or len(out) > len(best[1])):
            best = (off, out)

    if best is None:
        print("no vendor ramdisk with .ko found")
        mm.close()
        return

    off, cpio = best
    print(f"\nusing ramdisk at 0x{off:x} ({len(cpio)} bytes)")

    mods = []
    total = 0
    for name, blob in list_cpio(cpio):
        if name.endswith(".ko"):
            fn = os.path.basename(name)
            p = os.path.join(OUTDIR, fn)
            with open(p, "wb") as g:
                g.write(blob)
            mods.append((fn, len(blob), name))
            total += len(blob)
        elif name.endswith((".ko.gz", ".ko.xz", ".ko.zst")):
            fn = os.path.basename(name)
            p = os.path.join(OUTDIR, fn)
            with open(p, "wb") as g:
                g.write(blob)
            mods.append((fn, len(blob), name))
            total += len(blob)

    print(f"extracted {len(mods)} .ko modules, {total:,} bytes total")
    for fn, sz, name in sorted(mods, key=lambda x: -x[1]):
        print(f"  {fn:<45} {sz:>10,}")

    with open("E:/vxtux_build_repo/donors/stock_modules.txt", "w",
              encoding="utf-8") as g:
        for fn, sz, name in sorted(mods):
            g.write(f"{fn[:-3] if fn.endswith('.ko') else fn}\t{sz}\t{name}\n")

    mm.close()


if __name__ == "__main__":
    main()
