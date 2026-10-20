"""Parse the Android boot image header of the stock boot_a.bin and
extract the stock kernel (Image) plus its size, so we can compare it
against the VXTux 6.18 build.

struct boot_img_hdr (AOSP v4):
  magic[8]
  kernel_size, kernel_addr
  ramdisk_size, ramdisk_addr
  second_size, second_addr
  tags_addr, page_size
  header_version
  os_version
  name[16]
  cmdline[512]
  id[32]
  extra_cmdline[1024]
"""
import gzip
import hashlib
import mmap
import os
import struct

BOOT = "G:/UMS512/boot_a.bin"
OUT = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(OUT, exist_ok=True)

HDR_FMT = "<8sIIIIIIIII16s512s32s1024s"


def main():
    f = open(BOOT, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

    if mm[:8] != b"ANDROID!":
        print("not an android boot image")
        return

    h = struct.unpack_from(HDR_FMT, mm, 0)
    kernel_size = h[1]
    kernel_addr = h[2]
    ramdisk_size = h[3]
    ramdisk_addr = h[4]
    second_size = h[5]
    second_addr = h[6]
    tags_addr = h[7]
    page_size = h[8]
    hdr_version = h[9]
    os_version = h[10]
    name = h[11].split(b"\x00")[0].decode("utf-8", "replace")
    cmdline = h[12].split(b"\x00")[0].decode("utf-8", "replace")

    def ver(v):
        return f"{v >> 14}.{(v >> 7) & 0x7f}.{v & 0x7f}"

    print(f"header_version : {hdr_version}")
    print(f"os_version     : {ver(os_version)}")
    print(f"page_size      : {page_size}")
    print(f"kernel_size    : {kernel_size} (0x{kernel_size:x})")
    print(f"kernel_addr    : 0x{kernel_addr:08x}")
    print(f"ramdisk_size   : {ramdisk_size} (0x{ramdisk_size:x})")
    print(f"ramdisk_addr   : 0x{ramdisk_addr:08x}")
    print(f"second_size    : {second_size}")
    print(f"name           : {name}")
    print(f"cmdline        : {cmdline[:200]}")

    koff = page_size
    kdata = mm[koff:koff + kernel_size]
    print()
    print(f"kernel at file offset 0x{koff:x}, {len(kdata)} bytes")
    print(f"  first 16 bytes: {kdata[:16].hex()}")

    if kdata[:2] == b"\x1f\x8b":
        print("  -> gzip compressed")
        try:
            raw = gzip.decompress(kdata)
            print(f"  -> decompressed {len(raw)} bytes")
            p = os.path.join(OUT, "stock_Image")
            with open(p, "wb") as g:
                g.write(raw)
            print(f"  -> saved {p}")
            print(f"  -> sha256 {hashlib.sha256(raw).hexdigest()}")
            if len(raw) >= 64:
                code0, code1 = struct.unpack_from("<II", raw, 0)
                text_off, image_size, flags = struct.unpack_from("<QQQ", raw, 8)
                print(f"  -> arm64 Image hdr: code0=0x{code0:x} code1=0x{code1:x} "
                      f"text_offset=0x{text_off:x} image_size=0x{image_size:x}")
        except Exception as e:
            print(f"  -> gzip decompress failed: {e}")
            with open(os.path.join(OUT, "stock_Image.gz"), "wb") as g:
                g.write(kdata)
    else:
        print(f"  -> unknown magic {kdata[:4].hex()}")
        with open(os.path.join(OUT, "stock_kernel_raw"), "wb") as g:
            g.write(kdata)

    mm.close()


if __name__ == "__main__":
    main()
