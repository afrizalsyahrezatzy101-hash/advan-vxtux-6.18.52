"""Build a flashable boot image for the VXTux kernel (SPRD container).

Verified layout of the ADVAN VX Lite stock boot_a.bin (64 MiB):

    0x00000000  8      magic "ANDROID!"
    0x00000008  4      kernel_size
    0x0000000c  4      kernel_addr
    0x00000010  4      ramdisk_size
    0x00000014  4      ramdisk_addr
    0x00000028  4      header_version = 4
    0x00001000  ..     raw arm64 Image (kernel_size bytes)
    0x02454000  ..     lz4-legacy vendor ramdisk (1,469,836 bytes)
    0x025bad8c  ..     secondary payload (mostly zero padding, 1,852
                       non-zero bytes) + zero fill to 64 MiB

The stock image carries NO device tree: the DTB lives in its own
partition, so only the kernel is swapped. The vendor ramdisk and the
secondary payload are kept byte-for-byte.

Because the VXTux kernel is 3,512 KiB larger than the stock one, the
zero-filled padding region absorbs the difference and the secondary
payload is relocated to keep it inside the partition.

Usage:
    python3 make_bootimg3.py <stock_boot_a.bin> <new_Image> <out.img>
"""
import hashlib
import os
import struct
import sys

MAGIC = b"ANDROID!"
KERNEL_OFF = 0x1000
RD_OFF = 0x2454000
RD_LEN = 0x25bad8c - 0x2454000      # 1,469,836
SEC_OFF = 0x25bad8c                # secondary payload start
PARTITION_SIZE = 64 * 1024 * 1024


def main():
    if len(sys.argv) < 4:
        print(__doc__)
        return
    stock_path, image_path, out_path = sys.argv[1:4]

    stock = open(stock_path, "rb").read()
    assert stock[:8] == MAGIC, "not an ANDROID! container"
    ksize = struct.unpack_from("<I", stock, 8)[0]
    rdsize = struct.unpack_from("<I", stock, 16)[0]
    print(f"stock boot_a.bin : {len(stock):,} bytes")
    print(f"  kernel_size    : {ksize:,} (0x{ksize:x})")
    print(f"  ramdisk_size   : {rdsize:,} (0x{rdsize:x})")

    # ---- carve the stock payload apart --------------------------------
    vendor_ramdisk = stock[RD_OFF:RD_OFF + RD_LEN]
    assert struct.unpack_from("<I", vendor_ramdisk, 0)[0] == 0x184C2102, \
        "vendor ramdisk lz4 magic not where expected"
    secondary = stock[SEC_OFF:PARTITION_SIZE]
    print(f"  vendor ramdisk : {len(vendor_ramdisk):,} bytes (lz4 legacy)")
    print(f"  secondary      : {len(secondary):,} bytes "
          f"({sum(1 for b in secondary if b):,} non-zero)")

    new = open(image_path, "rb").read()
    print(f"\nnew Image        : {len(new):,} bytes "
          f"(delta {len(new) - ksize:+,})")

    # ---- rebuild ------------------------------------------------------
    out = bytearray(PARTITION_SIZE)
    out[0:8] = MAGIC
    out[KERNEL_OFF:KERNEL_OFF + len(new)] = new

    # the vendor ramdisk goes right after the (padded) kernel
    kpad = len(new)
    if kpad % 4096:
        kpad += 4096 - kpad % 4096
    rd_at = KERNEL_OFF + kpad
    out[rd_at:rd_at + RD_LEN] = vendor_ramdisk

    # the secondary payload follows it
    sec_at = rd_at + RD_LEN
    sec = secondary.rstrip(b"\x00")
    out[sec_at:sec_at + len(sec)] = sec

    # header
    struct.pack_into("<I", out, 8, len(new))      # kernel_size
    struct.pack_into("<I", out, 12, KERNEL_OFF)   # kernel_addr
    struct.pack_into("<I", out, 16, RD_LEN)       # ramdisk_size
    struct.pack_into("<I", out, 20, rd_at)        # ramdisk_addr
    struct.pack_into("<I", out, 40, 4)            # header_version

    with open(out_path, "wb") as f:
        f.write(out)
    print(f"\nwrote {out_path} ({len(out):,} bytes)")

    # ---- verify -------------------------------------------------------
    chk = open(out_path, "rb").read()
    assert chk[:8] == MAGIC
    assert chk[KERNEL_OFF:KERNEL_OFF + 2] == b"\x4d\x5a", "not an arm64 Image"
    assert chk[KERNEL_OFF + 56:KERNEL_OFF + 60] == b"ARM\x64"
    ks = struct.unpack_from("<I", chk, 8)[0]
    rs = struct.unpack_from("<I", chk, 16)[0]
    ra = struct.unpack_from("<I", chk, 20)[0]
    lz4_ok = chk[ra:ra + 4] == b"\x02\x21\x4c\x18"
    print(f"  magic        : OK")
    print(f"  arm64 Image  : OK at 0x{KERNEL_OFF:x} "
          f"(code0=0x{struct.unpack_from('<I', chk, KERNEL_OFF)[0]:08x})")
    print(f"  kernel_size  : {ks:,}")
    print(f"  ramdisk_size : {rs:,} at 0x{ra:x}  lz4 magic: "
          f"{'OK' if lz4_ok else 'MISSING'}")
    print(f"  sha256       : {hashlib.sha256(chk).hexdigest()}")


if __name__ == "__main__":
    main()
