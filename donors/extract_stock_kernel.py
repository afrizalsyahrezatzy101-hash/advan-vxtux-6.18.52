"""Parse the Android boot image header of the stock boot_a.bin and
extract the stock kernel (Image) plus its size, so we can compare it
against the VXTux 6.18 build.
"""
import gzip
import hashlib
import mmap
import os
import struct

BOOT = "G:/UMS512/boot_a.bin"
OUT = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(OUT, exist_ok=True)

# struct boot_img_hdr (AOSP)
# magic[8] kernel_size kernel_addr ramdisk_size ramdisk_addr
# second_size second_addr tags_addr page_size header_version os_version...
HDR_FMT = "<8sIIIIIIII16sIIIIIHH"
HDR_LEN = struct.calcsize(HDR_FMT)


def main():
    f = open(BOOT, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

    magic = mm[:8]
    print("magic:", magic)
    if magic != b"ANDROID!":
        print("not an android boot image")
        return

    hdr = struct.unpack_from(HDR_FMT, mm, 0)
    (_, kernel_size, kernel_addr, ramdisk_size, ramdisk_addr,
     second_size, second_addr, tags_addr, page_size, hdr_version,
     os_version, name, cmdline, id_, extra_cmdline) = hdr

    def ver(v):
        return f"{v >> 14}.{(v >> 7) & 0x7f}.{v & 0x7f}"

    print(f"header_version : {hdr_version}")
    print(f"os_version     : {ver(os_version)}")
    print(f"page_size      : {page_size}")
    print(f"kernel_size    : {kernel_size} (0x{kernel_size:x})")
    print(f"kernel_addr    : 0x{kernel_addr:08x}")
    print(f"ramdisk_size   : {ramdisk_size}")
    print(f"second_size    : {second_size}")
    print(f"name           : {name.split(chr(0))[0]}")
    print(f"cmdline        : {cmdline.split(chr(0))[0]}")

    # kernel starts after the header, page-aligned
    koff = page_size
    kdata = mm[koff:koff + kernel_size]
    print()
    print(f"kernel at file offset 0x{koff:x}, {len(kdata)} bytes")
    print(f"  first 16 bytes: {kdata[:16].hex()}")

    # identify
    if kdata[:2] == b"\x1f\x8b":
        print("  -> gzip compressed")
        try:
            raw = gzip.decompress(kdata)
            print(f"  -> decompressed {len(raw)} bytes")
            with open(os.path.join(OUT, "stock_Image_decompressed"), "wb") as g:
                g.write(raw)
            print(f"  -> saved stock_Image_decompressed")
            print(f"  -> sha256 {hashlib.sha256(raw).hexdigest()}")
            with open(os.path.join(OUT, "stock_Image.gz"), "wb") as g:
                g.write(kdata)
            # also look for the arm64 Image header inside
            with open(os.path.join(OUT, "stock_Image.gz.sha256"), "w") as g:
                g.write(hashlib.sha256(kdata).hexdigest() + "  stock_Image.gz\n")
        except Exception as e:
            print(f"  -> gzip decompress failed: {e}")
    elif kdata[:4] == b"\x7fELF":
        print("  -> ELF")
    else:
        print(f"  -> unknown magic, first 4 = {kdata[:4].hex()}")

    # ramdisk
    roff = koff + kernel_size
    roff = (roff + page_size - 1) & ~(page_size - 1)
    rdata = mm[roff:roff + ramdisk_size]
    print()
    print(f"ramdisk at 0x{roff:x}, {ramdisk_size} bytes, first 8: {rdata[:8].hex()}")
    with open(os.path.join(OUT, "stock_ramdisk.img"), "wb") as g:
        g.write(rdata)
    print("  -> saved stock_ramdisk.img")

    mm.close()


if __name__ == "__main__":
    main()
