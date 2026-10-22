"""Extract the stock kernel Image from boot_a.bin.

The SPRD/ADVAN boot image does not use the plain AOSP header layout, so
we locate the gzip stream directly (the stock kernel is gzip-compressed)
and decompress it, then validate the arm64 Image header.
"""
import gzip
import hashlib
import io
import mmap
import os
import struct
import zlib

BOOT = "G:/UMS512/boot_a.bin"
OUT = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(OUT, exist_ok=True)


def try_gzip_at(mm, off):
    """Return decompressed bytes if a gzip stream starts at off."""
    try:
        d = zlib.decompressobj(16 + zlib.MAX_WBITS)
        out = d.decompress(mm[off:off + 40 * 1024 * 1024])
        out += d.flush()
        if len(out) > 100000:
            return out
    except Exception:
        return None
    return None


def main():
    f = open(BOOT, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    print(f"boot_a.bin = {len(mm)} bytes")

    # Collect all gzip start offsets
    offsets = []
    idx = 0
    while True:
        i = mm.find(b"\x1f\x8b\x08", idx)
        if i < 0:
            break
        offsets.append(i)
        idx = i + 1
    print(f"gzip candidates: {[hex(o) for o in offsets]}")

    results = []
    for off in offsets:
        raw = try_gzip_at(mm, off)
        if raw is None:
            print(f"  0x{off:x}: not a valid gzip stream (or too small)")
            continue
        # arm64 Image header check
        info = ""
        if len(raw) >= 64:
            code0, code1 = struct.unpack_from("<II", raw, 0)
            text_off, image_size, flags = struct.unpack_from("<QQQ", raw, 8)
            is_arm64 = (raw[56:60] == b"ARM\x64")
            info = (f"arm64_magic={is_arm64} code0=0x{code0:x} "
                    f"code1=0x{code1:x} text_off=0x{text_off:x} "
                    f"image_size=0x{image_size:x}")
        print(f"  0x{off:x}: decompressed {len(raw)} bytes  {info}")
        results.append((off, raw, info))

    mm.close()

    # pick the biggest arm64-looking one
    best = None
    for off, raw, info in results:
        if "arm64_magic=True" in info:
            if best is None or len(raw) > len(best[1]):
                best = (off, raw, info)

    if best is None:
        print("\nNo arm64 Image found; saving largest decompress result")
        if not results:
            print("NOTHING EXTRACTED")
            return
        best = max(results, key=lambda r: len(r[1]))

    off, raw, info = best
    p = os.path.join(OUT, "stock_Image")
    with open(p, "wb") as g:
        g.write(raw)
    print(f"\nSAVED {p}")
    print(f"  source offset : 0x{off:x}")
    print(f"  size          : {len(raw)} bytes")
    print(f"  sha256        : {hashlib.sha256(raw).hexdigest()}")
    print(f"  {info}")


if __name__ == "__main__":
    main()
