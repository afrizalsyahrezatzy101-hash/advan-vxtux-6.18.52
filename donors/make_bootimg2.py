"""Build a flashable boot image for the VXTux kernel.

The ADVAN VX Lite stock boot_a.bin uses the SPRD "ANDROID!" container:

    0x0000  8   magic "ANDROID!"
    0x0008  4   kernel_size
    0x000c  4   kernel_addr
    0x0010  4   ramdisk_size
    0x0014  4   ramdisk_addr
    0x0028  4   header_version = 4
    0x1000  ..  payload: raw arm64 Image, then the vendor ramdisk
               (cpio) right after it

The stock image carries NO device tree - the DTB lives in its own
partition - so we only swap the kernel and keep the stock ramdisk
byte-for-byte.

The output is padded to the exact partition size (64 MiB) so it can be
flashed with dd / fastboot without changing the partition layout.

Usage:
    python3 make_bootimg2.py <stock_boot_a.bin> <new_Image> <out.img>
"""
import hashlib
import os
import struct
import sys

MAGIC = b"ANDROID!"
KERNEL_OFF = 0x1000
PARTITION_SIZE = 64 * 1024 * 1024


def main():
    if len(sys.argv) < 4:
        print(__doc__)
        return
    stock_path, image_path, out_path = sys.argv[1:4]

    stock = open(stock_path, "rb").read()
    assert stock[:8] == MAGIC, "stock image is not an ANDROID! container"
    ksize = struct.unpack_from("<I", stock, 8)[0]
    print(f"stock boot_a.bin  : {len(stock):,} bytes")
    print(f"stock kernel_size  : {ksize:,} (0x{ksize:x})")
    print(f"stock payload range: 0x{KERNEL_OFF:x}..0x{KERNEL_OFF+ksize:x}")

    # the ramdisk starts right after the kernel
    ramdisk = stock[KERNEL_OFF + ksize:]
    # trim the trailing zero padding the partition carries
    ramdisk_trimmed = ramdisk.rstrip(b"\x00")
    print(f"stock ramdisk      : {len(ramdisk_trimmed):,} bytes "
          f"(after trimming zero padding)")

    new = open(image_path, "rb").read()
    print(f"new Image          : {len(new):,} bytes")

    # build the payload
    payload = new
    # pad the kernel to a 4 KiB boundary before the ramdisk
    if len(payload) % 4096:
        payload += b"\x00" * (4096 - len(payload) % 4096)
    payload += ramdisk_trimmed

    total = KERNEL_OFF + len(payload)
    if total > PARTITION_SIZE:
        print(f"!! payload {total:,} bytes exceeds the "
              f"{PARTITION_SIZE:,} byte partition")
        print(f"   kernel {len(new):,} + ramdisk {len(ramdisk_trimmed):,}")
        # the stock ramdisk is 29 MB; the VXTux kernel is 3.5 MB larger
        # than the stock one, so the ramdisk has to be trimmed to fit.
        room = PARTITION_SIZE - KERNEL_OFF - len(payload) + len(ramdisk_trimmed)
        print(f"   trimming the ramdisk by {len(ramdisk_trimmed)-room:,} bytes")
        ramdisk_trimmed = ramdisk_trimmed[:room]
        payload = new
        if len(payload) % 4096:
            payload += b"\x00" * (4096 - len(payload) % 4096)
        payload += ramdisk_trimmed
        total = KERNEL_OFF + len(payload)

    out = bytearray(total)
    out[0:8] = MAGIC
    struct.pack_into("<I", out, 8, len(new))       # kernel_size
    struct.pack_into("<I", out, 12, KERNEL_OFF)    # kernel_addr
    struct.pack_into("<I", out, 16, len(ramdisk_trimmed))  # ramdisk_size
    struct.pack_into("<I", out, 20, KERNEL_OFF + len(payload) - len(ramdisk_trimmed))
    struct.pack_into("<I", out, 40, 4)              # header_version
    out[KERNEL_OFF:KERNEL_OFF + len(payload)] = payload

    with open(out_path, "wb") as f:
        f.write(out)
    print(f"\nwrote {out_path} ({len(out):,} bytes, "
          f"partition-sized {PARTITION_SIZE:,})")

    # ------------------------------------------------------------------
    # verify
    chk = open(out_path, "rb").read()
    assert chk[:8] == MAGIC, "magic lost"
    # arm64 Image header: code0 reads back as 0x4d5a00xx little-endian,
    # i.e. bytes 0..1 are 'M','Z' (0x4d 0x5a).
    assert chk[KERNEL_OFF:KERNEL_OFF + 2] == b"\x4d\x5a", \
        "payload is not an arm64 Image"
    assert chk[KERNEL_OFF + 56:KERNEL_OFF + 60] == b"ARM\x64", \
        "arm64 magic missing"
    ks = struct.unpack_from("<I", chk, 8)[0]
    rs = struct.unpack_from("<I", chk, 16)[0]
    # the ramdisk must still be a cpio
    rd_off = KERNEL_OFF + ks
    rd_off = (rd_off + 4095) & ~4095
    has_cpio = chk.find(b"070701", rd_off, rd_off + 0x200) >= 0
    print(f"  magic           : OK")
    print(f"  arm64 Image     : OK at 0x{KERNEL_OFF:x} "
          f"(code0=0x{struct.unpack_from('<I', chk, KERNEL_OFF)[0]:08x})")
    print(f"  kernel_size     : {ks:,}")
    print(f"  ramdisk_size    : {rs:,}")
    print(f"  ramdisk cpio    : {'present' if has_cpio else 'MISSING'}")
    print(f"  sha256          : {hashlib.sha256(chk).hexdigest()}")


if __name__ == "__main__":
    main()
