#!/usr/bin/env python3
"""Per-function call + format-string map of a relocatable .ko.

objdump -d -r interleaves the RELA entries with the instructions, so the
offset of each relocation tells exactly which function it belongs to.
For .rodata references the addend is the string's section offset, so the
string can be read straight out of the section bytes.

Usage:
    python3 ko_fmap.py <file.ko> [function_regex]
"""
import re
import subprocess
import sys

OBJDUMP = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-objdump.exe")
READELF = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-readelf.exe")
NM = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
      "bin/aarch64-none-linux-gnu-nm.exe")

TEXT = re.compile(r"^\s*([0-9a-f]+):\s")
REL = re.compile(r"^\s*([0-9a-f]+):\s+R_AARCH64_(\S+)\s+(\S+?)(?:\s*\+\s*([0-9a-f]+))?\s*$")
SYM = re.compile(r"^([0-9a-f]+)\s+([0-9a-f]+)\s+(\S)\s+(.*)$")


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
        secs[name] = (int(addr, 16), int(off, 16), int(size, 16))
    return secs


def text_syms(path):
    out = subprocess.run([NM, "-S", path], capture_output=True,
                         text=True).stdout
    res = []
    for line in out.splitlines():
        m = SYM.match(line)
        if not m:
            continue
        addr, size, typ, name = m.groups()
        if typ.lower() != "t" or int(size, 16) == 0:
            continue
        res.append((int(addr, 16), int(size, 16), name))
    res.sort()
    return res


def cstr(blob, off):
    if off >= len(blob):
        return None
    end = blob.find(b"\0", off)
    if end < 0:
        return None
    s = blob[off:end]
    try:
        txt = s.decode("utf-8")
    except UnicodeDecodeError:
        return None
    return txt or None


def main():
    path = sys.argv[1]
    pat = re.compile(sys.argv[2]) if len(sys.argv) > 2 else None
    secs = sections(path)
    data = open(path, "rb").read()
    funcs = text_syms(path)

    def func_of(addr):
        for a, s, n in funcs:
            if a <= addr < a + s:
                return n
        return None

    out = subprocess.run([OBJDUMP, "-d", "-r", "--no-show-raw-insn", path],
                         capture_output=True, text=True).stdout

    # per-section relocation streams
    cur = None
    per_sec = {}
    for line in out.splitlines():
        m = re.match(r"Disassembly of section (\S+):", line)
        if m:
            cur = m.group(1)
            per_sec.setdefault(cur, [])
            continue
        m = REL.match(line)
        if m and cur:
            off, typ, name, add = m.groups()
            # objdump prints the addend glued to the section name
            # (".rodata+0x1db"), so split it back out.
            if "+" in name and add is None:
                name, _, a = name.partition("+")
                try:
                    add = int(a, 16)
                except ValueError:
                    add = 0
            per_sec[cur].append((int(off, 16), typ, name,
                                 int(add, 16) if isinstance(add, str) and add
                                 else (add or 0)))

    def rodata_str(off):
        if ".rodata" not in secs:
            return None
        a, o, sz = secs[".rodata"]
        return cstr(data[o:o + sz], off)

    result = {}
    for sec, rels in per_sec.items():
        if not sec.startswith(".text"):
            continue
        for off, typ, name, add in rels:
            f = func_of(off)
            if not f:
                continue
            e = result.setdefault(f, {"calls": set(), "strs": set()})
            if typ in ("CALL26", "JUMP26", "CALL26_PREL19", "JUMP16"):
                if not name.startswith("."):
                    e["calls"].add(name)
            elif name.startswith(".rodata"):
                s = rodata_str(add)
                if s:
                    e["strs"].add(s)

    for f in sorted(result):
        if pat and not pat.search(f):
            continue
        e = result[f]
        print(f"\n=== {f} ===")
        print("  calls: " + ", ".join(sorted(e["calls"])))
        for s in sorted(e["strs"]):
            print(f"  str: {s!r}")


if __name__ == "__main__":
    main()
