"""Locate the vendor ramdisk payload inside vendor_boot_stock.img.

The SPRD vendor_boot header is v4:
    magic[8] "VNDRBOOT"
    u32 header_version
    u32 page_size
    u32 kernel_addr
    u32 ramdisk_addr
    u32 ramdisk_size
    u32 cmdline[...]

The ramdisk_addr field is 0x5400000 but the payload there is zeros, so
the layout is SPRD-specific. We scan the whole file for the cpio
"070701" signature and for every compression magic, and report what
each candidate decompresses to.
"""
import mmap
import os
import struct
import zlib

IMG = "G:/UMS512/twrp_zip/vendor_boot_stock.img"


def main():
    f = open(IMG, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    n = len(mm)
    print(f"{IMG}: {n} bytes")

    # cpio newc scan
    print("--- cpio newc candidates ---")
    idx = 0
    hits = 0
    while hits < 10:
        i = mm.find(b"070701", idx)
        if i < 0:
            break
        # look at the filename right after the 110-byte header
        namesize = struct.unpack_from("<I", mm, i + 94)[0]
        if 0 < namesize < 256:
            nm = mm[i + 110: i + 110 + namesize - 1]
            if all(32 <= c < 127 for c in nm):
                print(f"  0x{i:08x} namesize={namesize} name={nm.decode()[:60]!r}")
                hits += 1
        idx = i + 1

    # compression magics
    for name, sig in [("gzip", b"\x1f\x8b\x08"), ("xz", b"\xfd7zXZ\x00"),
                      ("zstd", b"\x28\xb5\x2f\xfd"), ("lz4f", b"\x04\x22\x4d\x18"),
                      ("lz4l", b"\x02\x21\x4c\x18"), ("lzop", b"\x89LZO")]:
        idx = 0
        found = 0
        while found < 5:
            i = mm.find(sig, idx)
            if i < 0:
                break
            idx = i + 1
            found += 1
            size = 0
            if name == "gzip":
                try:
                    d = zlib.decompressobj(16 + zlib.MAX_WBITS)
                    out = d.decompress(mm[i:i + 120 * 1024 * 1024])
                    size = len(out)
                    ko = out.count(b".ko")
                except Exception:
                    size = -1
                    ko = 0
            else:
                size = -1
                ko = 0
            if found <= 3:
                print(f"  {name} @0x{i:08x} decompressed={size} .ko={ko}")

    # where are the .ko bytes? search for 'mali' or ELF module magic
    print("--- ELF module magic scan ---")
    idx = 0
    cnt = 0
    while cnt < 6:
        i = mm.find(b"\x7fELF", idx)
        if i < 0:
            break
        # EI_CLASS=2(64) EI_DATA=1(LE) e_type=3 (ET_REL) at offset 16
        if mm[i + 4] == 2 and mm[i + 5] == 1:
            etype = struct.unpack_from("<H", mm, i + 16)[0]
            mach = struct.unpack_from("<H", mm, i + 18)[0]
            if etype == 1 or etype == 3:  # REL or EXEC
                print(f"  ELF @0x{i:08x} type={etype} machine={mach}")
                cnt += 1
        idx = i + 1

    # what about the 'mali' string
    for pat in (b"mali", b".ko", b"hxchipset"):
        i = mm.find(pat)
        print(f"  {pat!r} first @ 0x{i:x}" if i >= 0 else f"  {pat!r} NOT FOUND")

    mm.close()


if __name__ == "__main__":
    main()
