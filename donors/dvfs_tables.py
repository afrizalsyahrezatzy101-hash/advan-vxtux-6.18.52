#!/usr/bin/env python3
"""Read the stock apsys-dvfs.ko's SoC lookup tables.

The val_to_* functions are one-liners: they index a pointer table with
the level number and return the string, or the "invalid" string when
the level is out of range:

    undefined * sharkl5pro_dpu_val_to_freq(uint param_1)
    {
      if (param_1 < 5)
        puVar1 = (&PTR_s_153_6M_00105d80)[(int)param_1];
      else
        puVar1 = &DAT_001023b6;      // the "invalid level" string
      return puVar1;
    }

So the table is an array of pointers into .rodata.str1.1, and the
strings are the frequency or voltage labels.

This reads the tables out of the stock blob: the pointer table, the
relocations that fill it in, and the strings they point at.
"""
import re
import struct
import subprocess
import sys

READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")
OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")


def sections(path):
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
        p = rest[1].split()
        if len(p) < 5:
            continue
        name = p[0]
        try:
            off = int(p[3], 16)
            size = int(p[4], 16)
        except ValueError:
            continue
        secs[name] = (off, size)
    return secs


def relocs(path):
    """(section) -> {offset: (type, symname, addend)}"""
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
            if f.startswith("0x"):
                continue
            if all(c in "0123456789abcdef" for c in f.lower()) and len(f) <= 8:
                if name is not None:
                    addend = int(f, 16)
                continue
            if name is None:
                name = f
        if name is None:
            continue
        res.setdefault(cur, {})[off] = (typ, name, addend)
    return res


def cstring_at(path, secs, addr):
    """the string at the given section-relative addr, or None

    In a module every section has address 0, so a symbol's nm address
    is its offset inside its own section. The caller has to say which
    section the address is in.
    """
    return None


def read_section(path, secs, secname, off, length):
    """raw bytes at a section-relative offset"""
    if secname not in secs:
        return None
    base, size = secs[secname]
    if off + length > size:
        return None
    with open(path, "rb") as f:
        f.seek(base + off)
        return f.read(length)


def main():
    path = sys.argv[1]
    secs = sections(path)
    rel = relocs(path)
    for rsec, entries in sorted(rel.items()):
        if not rsec.startswith(".rela.rodata"):
            continue
        # the table is a run of 8-byte pointers
        offs = sorted(entries)
        if not offs:
            continue
        print(f"=== {rsec} ===")
        for off in offs:
            typ, sym, addend = entries[off]
            if "64" not in typ:
                continue
            # the string is at sym's address + addend
            # sym is a local label in .rodata.str1.1, so the symbol
            # value plus the addend is the string address
            s = cstring_at(path, secs, addend)
            print(f"  +0x{off:x}  {sym} + 0x{addend:x}"
                  f"  -> {s!r}")


if __name__ == "__main__":
    main()
