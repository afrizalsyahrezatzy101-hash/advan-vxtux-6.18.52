"""Find and extract the stock kernel Image from the available boot images.

Candidates on G:/UMS512:
  boot_a.bin            - raw flash dump (64 MiB), SPRD header layout
  root/boot_a.img       - extracted boot image
  twrp_zip/boot.img     - TWRP boot image
  twrp_mm/boot_b_twrp.img
  unlocked2/boot_a-magisk.img
  backup/boot_a_PRE-ARTIX.bin, boot_a_POST.bin
"""
import gzip
import hashlib
import mmap
import os
import struct
import subprocess
import zlib

CANDIDATES = [
    "G:/UMS512/root/boot_a.img",
    "G:/UMS512/twrp_zip/boot.img",
    "G:/UMS512/twrp_mm/boot_b_twrp.img",
    "G:/UMS512/unlocked2/boot_a-magisk.img",
    "G:/UMS512/backup/boot_a_PRE-ARTIX.bin",
    "G:/UMS512/backup/boot_a_POST.bin",
    "G:/UMS512/backup/boot_b_v4_vxtux_dtb.img",
]
OUT = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(OUT, exist_ok=True)

ARM64_MAGIC_OFFSET = 56
ARM64_MAGIC = b"ARM\x64"


def try_decompress(mm, off, limit=48 * 1024 * 1024):
    """Try gzip at off."""
    try:
        d = zlib.decompressobj(16 + zlib.MAX_WBITS)
        out = d.decompress(mm[off:off + limit])
        out += d.flush()
        return out if len(out) > 100000 else None
    except Exception:
        return None


def scan(path):
    print("=" * 74)
    print(path)
    if not os.path.exists(path):
        print("  MISSING")
        return
    sz = os.path.getsize(path)
    print(f"  size {sz} (0x{sz:x})")
    f = open(path, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

    head = mm[:8]
    if head == b"ANDROID!":
        print("  Android boot image")
        # try the standard AOSP header
        ks = struct.unpack_from("<I", mm, 8)[0]
        page = struct.unpack_from("<I", mm, 36)[0]
        print(f"  kernel_size={ks} page_size={page}")
        if 0 < ks < sz and page:
            koff = page
            kd = mm[koff:koff + ks]
            print(f"  kernel first16: {kd[:16].hex()}")
            if kd[:2] == b"\x1f\x8b":
                raw = try_decompress(mm, koff)
                if raw:
                    print(f"  GZIP -> {len(raw)} bytes")
                    _save(raw, path, "bootimg_gzip")
    else:
        print(f"  head: {head.hex()}")

    # brute scan for gzip streams and arm64 Image
    best = None
    idx = 0
    n = 0
    while n < 12:
        i = mm.find(b"\x1f\x8b\x08", idx)
        if i < 0:
            break
        raw = try_decompress(mm, i)
        if raw and len(raw) > 1_000_000:
            print(f"  gzip @0x{i:x} -> {len(raw)} bytes")
            if len(raw) >= 64 and raw[ARM64_MAGIC_OFFSET:ARM64_MAGIC_OFFSET + 4] == ARM64_MAGIC:
                print(f"    *** ARM64 Image magic found ***")
            if best is None or len(raw) > len(best[1]):
                best = (i, raw)
        idx = i + 1
        n += 1

    # also look for a raw arm64 Image header
    idx = 0
    while True:
        i = mm.find(ARM64_MAGIC, idx)
        if i < 0:
            break
        # arm64 Image: magic at 56, and code0/code1 at 0
        start = i - ARM64_MAGIC_OFFSET
        if start >= 0:
            code0, code1 = struct.unpack_from("<II", mm, start)
            if code0 == 0x4d5a0000 or (code0 & 0xffff0000) == 0x4d5a0000:
                print(f"  arm64 Image candidate @0x{start:x} "
                      f"code0=0x{code0:x} code1=0x{code1:x}")
        idx = i + 1

    if best:
        _save(best[1], path, "gzip")

    mm.close()
    f.close()


def _save(raw, src, tag):
    name = os.path.basename(src).replace(".img", "").replace(".bin", "")
    p = os.path.join(OUT, f"{name}_{tag}.bin")
    with open(p, "wb") as g:
        g.write(raw)
    print(f"  SAVED {p} ({len(raw)} bytes) sha256={hashlib.sha256(raw).hexdigest()[:32]}")


if __name__ == "__main__":
    for c in CANDIDATES:
        try:
            scan(c)
        except Exception as e:
            print(f"  ERROR: {e}")
