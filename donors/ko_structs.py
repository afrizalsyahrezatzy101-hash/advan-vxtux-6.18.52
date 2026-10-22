#!/usr/bin/env python3
"""Dump data symbols of a .ko with their relocations resolved.

ko_data.py picks the containing section by address range, which is wrong for
a .ko: every PROGBITS section starts at address 0. The symbol table knows
which section a symbol lives in (st_shndx), and the relocation sections name
their target section - so use both.

Usage:
    python3 donors/ko_structs.py <file.ko> <symbol> [<symbol>...]
"""
import struct
import subprocess
import sys

READELF = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-readelf.exe")


def section_table(path):
    """index -> (name, addr, offset, size)"""
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
    for line in out.splitlines():
        line = line.strip()
        if not line.startswith("["):
            continue
        rest = line.split("]", 1)
        if len(rest) != 2:
            continue
        parts = rest[1].split()
        if len(parts) < 5:
            continue
        try:
            idx = int(line[1:line.index("]")].strip())
        except ValueError:
            continue
        name = parts[0]
        try:
            addr = int(parts[2], 16)
            off = int(parts[3], 16)
            size = int(parts[4], 16)
        except (ValueError, IndexError):
            continue
        secs[idx] = (name, addr, off, size)
    return secs


def symbol_table(path):
    """name -> (value, size, type, shndx)"""
    out = subprocess.run([READELF, "-sW", path], capture_output=True,
                         text=True).stdout
    syms = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) < 8 or not parts[0].endswith(":"):
            continue
        try:
            val = int(parts[1], 16)
            size = int(parts[2], 16)
            typ = parts[3]
            shndx = int(parts[6])
        except ValueError:
            continue
        name = parts[7]
        if name in syms:
            continue
        syms[name] = (val, size, typ, shndx)
    return syms


def reloc_sections(path):
    """target section name -> {offset: (type, sym, addend)}"""
    out = subprocess.run([READELF, "-rW", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        if line.startswith("Relocation section"):
            name = line.split("'")[1]
            if name.startswith(".rela"):
                name = name[5:]
            if not name.startswith("."):
                name = "." + name
            cur = name
            res.setdefault(cur, {})
            continue
        if cur is None:
            continue
        parts = line.split()
        if len(parts) < 3 or not parts[0].strip():
            continue
        try:
            off = int(parts[0], 16)
        except ValueError:
            continue
        rtype = parts[2]
        sym = parts[3].split("+")[0] if len(parts) > 3 else "?"
        add = 0
        if len(parts) > 4:
            try:
                add = int(parts[4], 16)
            except ValueError:
                add = 0
        res[cur][off] = (rtype, sym, add)
    return res


def main():
    path = sys.argv[1]
    secs = section_table(path)
    syms = symbol_table(path)
    rmap = reloc_sections(path)
    blob = open(path, "rb").read()

    for want in sys.argv[2:]:
        if want not in syms:
            print(f"=== {want}: NOT FOUND")
            continue
        val, size, typ, shndx = syms[want]
        if shndx not in secs:
            print(f"=== {want}  size={size} type={typ}  (no section {shndx})")
            continue
        name, sa, so, ssz = secs[shndx]
        print(f"=== {want}  size={size} type={typ}  section={name}"
              f" off=0x{so:x}")
        if name.startswith(".bss"):
            continue
        data = blob[so:so + ssz]
        rel = rmap.get(name, {})
        for i in range(0, size, 8):
            off = val + i
            w = struct.unpack_from("<Q", data, off)[0] if off + 8 <= len(data) else 0
            r = rel.get(off)
            if r:
                rtype, s, add = r
                tgt = syms.get(s)
                where = f"0x{tgt[0]:x}" if tgt else "?"
                print(f"  +0x{i:03x}  {rtype:18s} -> {s} + 0x{add:x}"
                      f"   [{where}]")
            else:
                print(f"  +0x{i:03x}  0x{w:016x}")


if __name__ == "__main__":
    main()
