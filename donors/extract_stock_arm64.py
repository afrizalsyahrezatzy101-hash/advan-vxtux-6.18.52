"""Extract the stock arm64 kernel Image from the SPRD boot images.

The SPRD boot header puts the kernel at a page boundary right after the
header. The arm64 Image header starts with:
    0x00  u32 code0   (0x4d5a0000 | flags)
    0x04  u32 code1   (0x4d5a____)
    0x08  u64 text_offset
    0x10  u64 image_size
    0x18  u64 flags
    0x38  char magic[4] = "ARM\\x64"
    0x3c  u32 res2
So the arm64 'MZ' magic is at offset 0 and "ARM\\x64" sits at +0x38 (56).
"""
import hashlib
import mmap
import os
import struct

OUT = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(OUT, exist_ok=True)

SOURCES = {
    "stock": "G:/UMS512/boot_a.bin",
    "pre_artix": "G:/UMS512/backup/boot_a_PRE-ARTIX.bin",
    "post": "G:/UMS512/backup/boot_a_POST.bin",
    "vxtux_dtb": "G:/UMS512/backup/boot_b_v4_vxtux_dtb.img",
}


def parse(path, label):
    f = open(path, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    print("=" * 74)
    print(f"{label}: {path}")

    # The kernel starts at the first 0x1000 page and begins with the
    # arm64 Image header. Find "ARM\x64" then back up 56 bytes.
    i = mm.find(b"ARM\x64", 0x1000)
    if i < 0:
        print("  no arm64 magic found")
        mm.close()
        f.close()
        return
    start = i - 56
    code0, code1 = struct.unpack_from("<II", mm, start)
    text_off, image_size, flags = struct.unpack_from("<QQQ", mm, start + 8)
    print(f"  arm64 Image @ 0x{start:x}")
    print(f"    code0=0x{code0:08x} code1=0x{code1:08x}")
    print(f"    text_offset=0x{text_off:x} image_size=0x{image_size:x} "
          f"flags=0x{flags:x}")
    print(f"    image_size = {image_size} bytes "
          f"({image_size / 1024 / 1024:.2f} MiB)")

    # sanity: code0/code1 must be MZ-style. The Image header stores
    # the magic little-endian, so 0x4d5a0091 reads back as 0x91005a4d.
    if (code0 & 0xFFFF) != 0x5A4D:
        print("    !! code0 low half is not MZ, likely a false positive")
        mm.close()
        f.close()
        return

    data = mm[start:start + image_size]
    p = os.path.join(OUT, f"{label}_Image")
    with open(p, "wb") as g:
        g.write(data)
    sha = hashlib.sha256(data).hexdigest()
    print(f"  SAVED {p}  ({len(data)} bytes)")
    print(f"  sha256 {sha}")

    # the payload after the Image (ramdisk / dtb)
    after = start + image_size
    print(f"  payload after Image at 0x{after:x}: {mm[after:after+16].hex()}")
    mm.close()
    f.close()
    return p, sha


if __name__ == "__main__":
    for label, path in SOURCES.items():
        try:
            parse(path, label)
        except Exception as e:
            print(f"  ERROR {label}: {e}")
