#!/usr/bin/env python3
"""Map each function of a .ko to the symbols it references.

readelf -r gives the relocation offsets; nm gives the function ranges.
Joining them gives "what does this function actually call / point at",
which is what a decompiler can only guess at for a relocatable object.

Usage:
    python3 donors/ko_calls.py <file.ko> [funcname ...]
"""
import subprocess
import sys

NM = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
      "bin/aarch64-none-linux-gnu-nm.exe")
READELF = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-readelf.exe")
OBJDUMP = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-objdump.exe")


def functions(path):
    """[(start, end, name)] sorted"""
    out = subprocess.run([NM, "-n", path], capture_output=True,
                         text=True).stdout
    fns = []
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 3 and p[1] in "TtWw":
            fns.append((int(p[0], 16), None, p[2]))
    fns.sort()
    res = []
    for i, (a, _, n) in enumerate(fns):
        b = fns[i + 1][0] if i + 1 < len(fns) else a + 0x100000
        res.append((a, b, n))
    return res


def relocs(path):
    """[(offset, type, symbol+addend)] for .text and .data"""
    out = subprocess.run([OBJDUMP, "-r", path], capture_output=True,
                         text=True).stdout
    res = []
    sec = None
    for line in out.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            sec = line.split("[")[1].split("]")[0]
            continue
        if not sec or not sec.startswith((".text", ".init.text", ".exit.text")):
            continue
        p = line.split()
        if len(p) >= 3 and p[0].strip():
            try:
                off = int(p[0], 16)
            except ValueError:
                continue
            sym = p[2]
            if "+" in sym:
                sym, add = sym.split("+", 1)
                sym = f"{sym}+{add}"
            res.append((sec, off, p[1], sym))
    return res


def main():
    path = sys.argv[1]
    only = sys.argv[2:]
    fns = functions(path)
    rel = relocs(path)
    starts = [f[0] for f in fns]

    def owner(off):
        lo, hi = 0, len(starts) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if starts[mid] <= off:
                lo = mid
            else:
                hi = mid - 1
        return fns[lo][2]

    groups = {}
    for sec, off, rtype, sym in rel:
        if sec == ".data":
            continue
        groups.setdefault(owner(off), []).append((off, rtype, sym))

    for _, _, name in fns:
        if only and name not in only:
            continue
        items = groups.get(name, [])
        if not items:
            continue
        print(f"=== {name}")
        seen = {}
        for off, rtype, sym in items:
            seen.setdefault(sym, []).append(f"{off:x}:{rtype}")
        for sym in sorted(seen):
            print(f"    {sym:<42} {', '.join(seen[sym][:6])}")
        print()


if __name__ == "__main__":
    main()
