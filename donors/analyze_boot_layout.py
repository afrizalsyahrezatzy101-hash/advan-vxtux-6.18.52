"""Deep structure analysis of the SPRD boot images.

We know from the header dump that the layout is NOT plain AOSP:
  0x00 "ANDROID!"
  0x08 00 22 45 02
  0x0C 88 6d 16 00
  0x10 76 01 00 1a
  0x14 30 06 00 00
  0x2C 04 00 00 00   (header_version = 4)

We map every gzip / lz4 / arm64-Image candidate in the file and report
what each one decompresses to, so we can pin down the stock kernel.
"""
import hashlib
import mmap
import os
import struct
import zlib

FILES = [
    "G:/UMS512/boot_a.bin",
    "G:/UMS512/backup/boot_a_PRE-ARTIX.bin",
    "G:/UMS512/backup/boot_a_POST.bin",
]
ARM64 = b"ARM\x64"


def gz(mm, off, limit=64 * 1024 * 1024):
    try:
        d = zlib.decompressobj(16 + zlib.MAX_WBITS)
        out = d.decompress(mm[off:off + limit])
        out += d.flush()
        return out
    except Exception:
        return None


def arm64_info(data):
    if len(data) < 64:
        return None
    code0, code1 = struct.unpack_from("<II", data, 0)
    text_off, image_size, flags = struct.unpack_from("<QQQ", data, 8)
    magic = data[56:60]
    return (f"code0=0x{code0:x} code1=0x{code1:x} "
            f"text_off=0x{text_off:x} img_size=0x{image_size:x} "
            f"magic={magic!r}")


def main():
    for path in FILES:
        print("=" * 78)
        print(path)
        if not os.path.exists(path):
            print("  MISSING")
            continue
        sz = os.path.getsize(path)
        f = open(path, "rb")
        mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

        # dump header words
        print("  header words:")
        for off in range(8, 0x40, 4):
            v = struct.unpack_from("<I", mm, off)[0]
            if v:
                print(f"    0x{off:02x}: 0x{v:08x} ({v})")

        # what's at 0x1000 (first payload slot)
        print(f"  @0x1000: {mm[0x1000:0x1010].hex()}")
        print(f"  @0x2000: {mm[0x2000:0x2010].hex()}")

        # all gzip candidates
        print("  gzip candidates:")
        idx = 0
        shown = 0
        while shown < 8:
            i = mm.find(b"\x1f\x8b\x08", idx)
            if i < 0:
                break
            raw = gz(mm, i)
            if raw and len(raw) > 10000:
                info = arm64_info(raw)
                tag = " *** ARM64 IMAGE ***" if (raw[56:60] == ARM64) else ""
                print(f"    0x{i:08x} -> {len(raw):>10} bytes  "
                      f"{info or ''}{tag}")
                if shown == 0 and len(raw) > 500000:
                    p = (f"E:/vxtux_build_repo/donors/stock_kernel/"
                         f"{os.path.basename(path)}_gz{i:x}.bin")
                    with open(p, "wb") as g:
                        g.write(raw)
                    print(f"         SAVED {p}")
            idx = i + 1
            shown += 1

        # lz4 candidates
        print("  lz4 candidates:")
        for magic, tag in ((b"\x02\x21\x4c\x18", "legacy"),
                           (b"\x04\x22\x4d\x18", "frame")):
            idx = 0
            n = 0
            while n < 4:
                i = mm.find(magic, idx)
                if i < 0:
                    break
                # legacy frame: 4B magic + 4B size + 4B checksum
                info = ""
                if tag == "legacy":
                    blocksize = struct.unpack_from("<I", mm, i + 4)[0]
                    info = f"legacy block={blocksize}"
                else:
                    flg = mm[i + 4]
                    info = f"frame FLG=0x{flg:02x}"
                print(f"    0x{i:08x} {tag} {info}")
                idx = i + 1
                n += 1

        # raw ARM64 Image header search
        idx = 0
        hits = 0
        while hits < 4:
            i = mm.find(ARM64, idx)
            if i < 0:
                break
            start = i - 56
            if start >= 0:
                code0, code1 = struct.unpack_from("<II", mm, start)
                if (code0 >> 16) == 0x4d5a:
                    print(f"    raw arm64 Image @0x{start:08x} "
                          f"code0=0x{code0:x} code1=0x{code1:x}")
                    hits += 1
            idx = i + 1

        mm.close()
        f.close()
        print()


if __name__ == "__main__":
    main()
