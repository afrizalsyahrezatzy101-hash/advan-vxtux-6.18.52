#!/usr/bin/env python3
"""Resolve a .ko's relocations into per-function call/format-string tables.

A relocatable .ko has no absolute addresses: every `adrp`/`add` pair that
points at a string is an R_AARCH64_ADR_PRE / ADD_ABS relocation against
.rodata, and every call is an R_AARCH64_CALL26 against the target symbol.
Readelf prints the section+addend, so the string can be read straight out of
the section bytes at that offset.

Usage:
    python3 reloc_map.py <file.ko> [function_regex]
"""
import re
import struct
import subprocess
import sys

READELF = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-readelf.exe")
NM = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
      "bin/aarch64-none-linux-gnu-nm.exe")


def sections(path):
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
    for line in out.splitlines():
        m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+"
                     r"([0-9a-f]+)\s+([0-9a-f]+)", line)
        if not m:
            continue
        name, typ, addr, off, size = m.groups()
        secs[name] = (int(addr, 16), int(off, 16), int(size, 16), typ)
    return secs


def syms(path):
    out = subprocess.run([NM, "-S", path], capture_output=True,
                         text=True).stdout
    res = []
    for line in out.splitlines():
        p = line.split()
        if len(p) < 4:
            continue
        addr, size, typ, name = p[0], p[1], p[2], " ".join(p[3:])
        try:
            res.append((int(addr, 16), int(size, 16), typ, name))
        except ValueError:
            pass
    return res


def cstr(blob, off):
    if off >= len(blob):
        return None
    end = blob.find(b"\0", off)
    if end < 0:
        return None
    try:
        s = blob[off:end].decode("utf-8")
    except UnicodeDecodeError:
        return None
    if not s or any(ord(c) < 9 or (13 < ord(c) < 32) for c in s):
        return None
    return s


def main():
    path = sys.argv[1]
    pat = re.compile(sys.argv[2]) if len(sys.argv) > 2 else None
    secs = sections(path)
    data = open(path, "rb").read()
    symlist = syms(path)

    # relocations
    out = subprocess.run([READELF, "-r", path], capture_output=True,
                         text=True).stdout
    relocs = []
    for line in out.splitlines():
        m = re.match(r"([0-9a-f]{8,16})\s+([0-9a-f]{8,16})\s+(\S+)\s+"
                     r"([0-9a-f]+)\s+(\S+)\s*\+\s*([0-9a-f]+)?", line)
        if not m:
            continue
        off, info, typ, val, name, add = m.groups()
        relocs.append((int(off, 16), typ, name, int(add, 16) if add else 0))

    # which relocation section each entry belongs to (offset in its section)
    cur_sec = None
    sec_of = []
    for line in out.splitlines():
        m = re.match(r"Relocation section '(\S+)'", line)
        if m:
            cur_sec = m.group(1)
            continue
        m = re.match(r"([0-9a-f]{8,16})\s+([0-9a-f]{8,16})\s+(\S+)\s+"
                     r"([0-9a-f]+)\s+(\S+)\s*\+\s*([0-9a-f]+)?", line)
        if m:
            off, info, typ, val, name, add = m.groups()
            sec_of.append((cur_sec, int(off, 16), typ, name,
                           int(add, 16) if add else 0))

    # text symbols for function attribution
    funcs = sorted([(a, s, n) for a, s, t, n in symlist
                     if t.lower() == "t" and s > 0])

    def func_of(addr):
        best = None
        for a, s, n in funcs:
            if a <= addr < a + s:
                best = n
        return best

    by_func = {}
    for sec, off, typ, name, add in sec_of:
        # readelf names the relocation section ".rela.<target>"; the target
        # is the section the offset is relative to.
        tgt = sec[5:] if sec.startswith(".rela") else sec
        f = func_of(off) if tgt.startswith(".text") else None
        key = f or tgt
        entry = by_func.setdefault(key, {"calls": [], "strings": [],
                                         "data": []})
        if typ in ("R_AARCH64_CALL26", "R_AARCH64_JUMP26",
                   "R_AARCH64_CALL26_PREL19"):
            if name not in entry["calls"]:
                entry["calls"].append(name)
        elif name.startswith(".rodata"):
            blob = data[secs[".rodata"][1]:secs[".rodata"][1]
                        + secs[".rodata"][2]]
            s = cstr(blob, add)
            if s and s not in entry["strings"]:
                entry["strings"].append(s)
        elif name.startswith(".data"):
            blob = data[secs[".data"][1]:secs[".data"][1] + secs[".data"][2]]
            s = cstr(blob, add)
            entry["data"].append((name, add, s))
        elif name.startswith(".bss"):
            entry["data"].append((name, add, "<bss>"))

    for f in sorted(by_func):
        if pat and not pat.search(f):
            continue
        e = by_func[f]
        print(f"\n=== {f} ===")
        if e["calls"]:
            print("  calls: " + ", ".join(sorted(e["calls"])))
        for s in e["strings"]:
            print(f"  str: {s!r}")
        for n, a, s in e["data"]:
            print(f"  data: {n}+{a:#x} -> {s!r}")


if __name__ == "__main__":
    main()
