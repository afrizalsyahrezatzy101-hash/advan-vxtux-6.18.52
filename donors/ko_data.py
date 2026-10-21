"""Read a data symbol out of a relocatable .ko and resolve its pointers.

Why
---
The stock vendor blobs are .ko files - relocatable objects, not linked
binaries. A data symbol like mbox_phy_ops holds 0 in the file and the
real target is in a RELA section. Ghidra resolves those, but only for
the sections it managed to map, and the .data section of a .ko is
sometimes not readable ("Unable to read bytes at ram:001001a0").

So this reads the bytes straight out of the ELF and applies the
relocations by hand:

  .data  0x198  size 0x148
  .rodata 0x2e0 size 0xaf0
  .text  0x1000 size 0x239c

Usage:
    python3 donors/ko_data.py <file.ko> <symbol> [symbol...]
"""
import struct
import subprocess
import sys

READELF = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-readelf.exe")
NM = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
      "bin/aarch64-none-linux-gnu-nm.exe")
OBJCOPY = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-objcopy.exe")

R_AARCH64_ABS64 = 257
R_AARCH64_ABS32 = 258


def sections(path):
    """name -> (addr, file offset, size) for the PROGBITS sections

    readelf -SW prints:

      [ 1] __ksymtab_gpl     PROGBITS        0000000000000000 000040 000048 00   A  0   0  8

    so the fields after the type are: address, file offset, size.
    """
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
    for line in out.splitlines():
        line = line.strip()
        if not line.startswith("["):
            continue
        # strip the "[ 1]" prefix
        rest = line.split("]", 1)
        if len(rest) != 2:
            continue
        parts = rest[1].split()
        if len(parts) < 5 or parts[1] != "PROGBITS":
            continue
        name = parts[0]
        try:
            addr = int(parts[2], 16)
            off = int(parts[3], 16)
            size = int(parts[4], 16)
        except ValueError:
            continue
        secs[name] = (addr, off, size)
    return secs


def syms(path):
    """name -> (addr, size, type)

    nm -S prints "addr size type name" but the size column is only
    there with -S, and the type column can be a space for undefined
    symbols, so the fields are taken from the right.
    """
    out = subprocess.run([NM, "-S", path], capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) < 3:
            continue
        try:
            addr = int(p[0], 16)
        except ValueError:
            continue
        if len(p) >= 4 and all(c in "0123456789abcdef" for c in p[1]):
            size = int(p[1], 16)
            typ = p[2]
            name = p[3]
        else:
            size = 0
            typ = p[1]
            name = p[2]
        res.setdefault(name, (addr, size, typ))
    return res


def relocs(path):
    """(section name) -> {offset: (type, symname, addend)}

    readelf -rW prints:

        Relocation section '.rela.rodata.sprd_mbox_ops' at offset 0x8c50
        contains 16 entries:
            Offset             Info             Type
            Symbol's Value  Symbol's Name + Addend
        0000000000000960  0000010f00000101 R_AARCH64_ABS64
        0000000000000000 .text + 237c

    so the fields are: offset, info, type, symbol value, symbol name
    (with the addend after a '+'), and sometimes a trailing one.
    """
    out = subprocess.run([READELF, "-rW", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        s = line.strip()
        if s.startswith("Relocation section"):
            cur = s.split("'")[1] if "'" in s else None
            continue
        if not s or not s[0].isdigit():
            continue
        p = s.split()
        if len(p) < 5:
            continue
        off = int(p[0], 16)
        typ = p[2]
        # the symbol's name and addend: the last two fields, and the
        # addend may be a separate field ("name" "+" "237c" or "name 237c")
        name = None
        addend = 0
        for f in p[3:]:
            if f == "+":
                continue
            if f.startswith("+"):
                try:
                    addend = int(f[1:], 16)
                except ValueError:
                    pass
                continue
            if f.startswith("0x") or f.startswith("-"):
                continue
            if all(c in "0123456789abcdef" for c in f.lower()) and len(f) <= 8:
                # a bare number: the addend, or the symbol value
                try:
                    v = int(f, 16)
                except ValueError:
                    continue
                if name is not None:
                    addend = v
                continue
            if name is None:
                name = f
        if name is None:
            continue
        res.setdefault(cur, {})[off] = (typ, name, addend)
    return res


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return 1
    path = sys.argv[1]
    secs = sections(path)
    sm = syms(path)
    rel = relocs(path)

    for want in sys.argv[2:]:
        if want not in sm:
            print(f"{want}: NOT FOUND")
            continue
        addr, size, typ = sm[want]
        print(f"=== {want}  addr=0x{addr:x} size={size} type={typ}")

        # find the section that holds it
        sec = None
        for n, (a, o, sz) in secs.items():
            if a <= addr < a + sz:
                sec = (n, a, o, sz)
                break
        if not sec:
            print("  (not in a PROGBITS section)")
            continue
        n, sa, so, ssz = sec
        data = open(path, "rb").read()[so:so + ssz]
        base = addr - sa

        # the relocations that apply inside this symbol
        for rsec, entries in rel.items():
            if not rsec:
                continue
            # '.rela.rodata.sprd_mbox_ops' -> the section is '.rodata'
            rbase = rsec[5:] if rsec.startswith(".rela") else rsec
            if not rbase.startswith("."):
                rbase = "." + rbase
            if not rbase.startswith(n):
                continue
            for off, (rt, symname, addend) in sorted(entries.items()):
                if base <= off < base + max(size, 8):
                    tgt = sm.get(symname)
                    if tgt:
                        print(f"  +0x{off - base:02x} -> {symname}"
                              f" + 0x{addend:x} = 0x{tgt[0] + addend:x}")
                    else:
                        print(f"  +0x{off - base:02x} -> {symname}"
                              f" + 0x{addend:x}")

        # dump the raw words
        words = []
        for i in range(0, min(size, 128), 8):
            w = struct.unpack_from("<Q", data, base + i)[0]
            words.append((base + i, w))
        for o, w in words:
            print(f"  0x{o:05x}: 0x{w:016x}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
