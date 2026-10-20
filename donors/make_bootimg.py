"""Build a flashable boot image for the VXTux kernel.

The ADVAN VX Lite stock boot_a.bin uses the SPRD "ANDROID!" container
with this layout (reverse engineered from the header dump):

    0x0000  8   magic "ANDROID!"
    0x0008  4   kernel_size
    0x000c  4   kernel_addr / offset of the payload
    0x0010  4   ramdisk_size
    0x0014  4   ramdisk offset
    0x0018  ..  reserved (zero)
    0x0028  4   header_version = 4
    0x0100  ..  payload: the arm64 Image starts here (page aligned)

The stock image keeps the raw arm64 Image at 0x1000 (not compressed),
which is what we replicate: take the stock header, replace the kernel
payload with our freshly built Image, and keep the rest of the
partition layout intact.

Usage:
    python3 make_bootimg.py <stock_boot_a.bin> <new_Image> <dtb|-> <out.img>
"""
import os
import struct
import sys

MAGIC = b"ANDROID!"


def read_header(d):
    magic = d[:8]
    if magic != MAGIC:
        raise ValueError("not an ANDROID! image")
    kernel_size, kernel_addr = struct.unpack_from("<II", d, 8)
    ramdisk_size, ramdisk_addr = struct.unpack_from("<II", d, 16)
    hdr_version = struct.unpack_from("<I", d, 40)[0]
    return {
        "kernel_size": kernel_size, "kernel_addr": kernel_addr,
        "ramdisk_size": ramdisk_size, "ramdisk_addr": ramdisk_addr,
        "hdr_version": hdr_version,
    }


def main():
    if len(sys.argv) < 5:
        print(__doc__)
        return
    stock_path, image_path, dtb_path, out_path = sys.argv[1:5]

    stock = open(stock_path, "rb").read()
    hdr = read_header(stock)
    print("stock header:")
    for k, v in hdr.items():
        print(f"  {k:<14} {v} (0x{v:x})")

    new = open(image_path, "rb").read()
    print(f"\nnew Image: {len(new):,} bytes "
          f"(stock kernel was {hdr['kernel_size']:,})")

    # The payload starts at 0x1000 in the stock image.
    PAYLOAD_OFF = 0x1000
    # Everything from the stock payload start up to the end of the
    # partition is the ramdisk/other payload; keep the stock tail so the
    # board keeps its ramdisk.
    tail = stock[PAYLOAD_OFF + hdr["kernel_size"]:]
    print(f"keeping stock tail: {len(tail):,} bytes")

    out = bytearray(stock)          # start from a byte-identical copy
    out[PAYLOAD_OFF:PAYLOAD_OFF + len(new)] = new
    # if the new kernel is smaller, zero the leftover old kernel bytes
    if len(new) < hdr["kernel_size"]:
        out[PAYLOAD_OFF + len(new):PAYLOAD_OFF + hdr["kernel_size"]] = \
            b"\x00" * (hdr["kernel_size"] - len(new))
    # update the header
    struct.pack_into("<I", out, 8, len(new))

    if dtb_path and dtb_path != "-":
        dtb = open(dtb_path, "rb").read()
        print(f"appending DTB: {dtb_path} ({len(dtb):,} bytes)")
        out += dtb
        struct.pack_into("<I", out, 12, len(new) + len(dtb))

    with open(out_path, "wb") as f:
        f.write(out)
    print(f"\nwrote {out_path} ({len(out):,} bytes)")

    # verify
    chk = open(out_path, "rb").read()
    assert chk[:8] == MAGIC
    ks = struct.unpack_from("<I", chk, 8)[0]
    assert chk[PAYLOAD_OFF:PAYLOAD_OFF + 4] == b"\x4d\x5a\x00\x91", \
        "payload is not an arm64 Image"
    assert chk[PAYLOAD_OFF + 56:PAYLOAD_OFF + 60] == b"ARM\x64", \
        "arm64 magic missing"
    print(f"verified: magic OK, kernel_size={ks:,}, arm64 Image at "
          f"0x{PAYLOAD_OFF:x}, ARM\\x64 present")


if __name__ == "__main__":
    main()
