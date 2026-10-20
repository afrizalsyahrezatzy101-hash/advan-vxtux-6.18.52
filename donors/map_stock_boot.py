"""Map the stock boot_a.bin layout: locate every embedded kernel/firmware
image so we can compare the stock kernel against the VXTux build.

Outputs a report of each candidate region with its type, size and a
short fingerprint (ELF header, gzip header, lz4 frame magic).
"""
import hashlib
import mmap
import os
import struct

BOOT = "G:/UMS512/boot_a.bin"

SIGS = [
    ("gzip", b"\x1f\x8b\x08"),
    ("elf64", b"\x7fELF\x02\x01\x01"),
    ("elf32", b"\x7fELF\x01\x01\x01"),
    ("lz4_legacy", b"\x02\x21\x4c\x18"),
    ("lz4_frame", b"\x04\x22\x4d\x18"),
    ("lzma", b"\x5d\x00\x00\x00"),
    ("cpio_newc", b"070701"),
    ("android_boot", b"ANDROID!"),
    ("sparse", b"\x3a\xff\x26\xed"),
    ("uImage", b"\x27\x05\x19\x56"),
]


def main():
    f = open(BOOT, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    total = len(mm)
    print(f"boot_a.bin: {total} bytes (0x{total:x})")

    found = []
    for name, sig in SIGS:
        idx = 0
        while True:
            i = mm.find(sig, idx)
            if i < 0:
                break
            found.append((i, name))
            idx = i + 1

    found.sort()

    # merge: only keep the first occurrence of each type in each 64K window
    seen = {}
    for off, name in found:
        bucket = off >> 16
        if (bucket, name) in seen:
            continue
        seen[(bucket, name)] = off

    # rebuild as {offset: name}
    by_off = {}
    for (bucket, name), off in seen.items():
        by_off[off] = name

    print()
    print(f"{'offset':>12}  {'type':<12}  detail")
    print("-" * 78)
    for off in sorted(by_off):
        name = by_off[off]
        detail = ""
        if name == "elf64":
            # ELF header: e_type, e_machine, e_entry
            hdr = mm[off:off + 64]
            if len(hdr) >= 24:
                (e_type, e_machine) = struct.unpack_from("<HH", hdr, 16)
                (e_entry,) = struct.unpack_from("<Q", hdr, 24)
                detail = (f"e_type={e_type} e_machine={e_machine} "
                          f"e_entry=0x{e_entry:x}")
        elif name == "gzip":
            hdr = mm[off:off + 10]
            if len(hdr) >= 10:
                flg = hdr[3]
                mtime = struct.unpack_from("<I", hdr, 4)[0]
                detail = f"FLG=0x{flg:02x} mtime={mtime}"
        elif name in ("lz4_legacy", "lz4_frame"):
            hdr = mm[off:off + 15]
            detail = f"magic={hdr[:4].hex()}"
        elif name == "cpio_newc":
            detail = "cpio newc archive"
        elif name == "android_boot":
            detail = "Android boot image header"

        print(f"  0x{off:08x}  {name:<12}  {detail}")

    # The primary kernel candidate: the first ELF64 that is followed
    # closely by a gzip header (kernel + its compressed twin).
    print()
    print("=== candidate stock kernel regions ===")
    elfs = sorted([o for o, n in found if n == "elf64"])
    for off in elfs[:6]:
        print(f"  ELF64 @ 0x{off:08x}")

    # dump the first ELF64's section-ish info by reading program headers
    if elfs:
        off = elfs[0]
        hdr = mm[off:off + 64]
        e_phoff = struct.unpack_from("<Q", hdr, 32)[0]
        e_phentsize = struct.unpack_from("<H", hdr, 54)[0]
        e_phnum = struct.unpack_from("<H", hdr, 56)[0]
        print()
        print(f"  program headers @ 0x{off + e_phoff:x} count={e_phnum}")
        end = off
        for i in range(e_phnum):
            ph = mm[off + e_phoff + i * e_phentsize:
                    off + e_phoff + (i + 1) * e_phentsize]
            if len(ph) < 56:
                break
            p_type, p_flags = struct.unpack_from("<II", ph, 0)
            p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_align = \
                struct.unpack_from("<QQQQQQ", ph, 8)
            print(f"   ph{i}: type={p_type} off=0x{p_offset:x} "
                  f"filesz=0x{p_filesz:x} memsz=0x{p_memsz:x}")
            end = max(end, off + p_offset + p_filesz)
        print(f"  => ELF64 image spans ~0x{off:x}..0x{end:x} "
              f"({end - off} bytes)")

    mm.close()


if __name__ == "__main__":
    main()
