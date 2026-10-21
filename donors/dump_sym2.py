"""Dump a stock blob's data symbol with relocations resolved to names.

    python3 donors/dump_sym2.py <blob.ko> <sym> [<sym>...]

Unlike objdump -r, relocations are kept per section, so a .rodata
pointer into .text is not confused with a .data one (all sections
share vma 0 in a .ko).
"""
import re
import subprocess
import sys

OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")

REL_RE = re.compile(r"^([0-9a-f]{8,16})\s+(\S+)\s+(\S+)(?:\s+([0-9a-f]+))?")
SEC_RE = re.compile(r"^\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+"
                    r"([0-9a-f]+)\s+([0-9a-f]+)\s+2\*\*(\d)")


def secs(path):
    out = subprocess.run([OBJDUMP, "-h", path], capture_output=True,
                         text=True).stdout
    t = {}
    for line in out.splitlines():
        m = SEC_RE.match(line)
        # objdump -h columns: idx name size VMA LMA fileoff align
        if m:
            t[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16),
                             int(m.group(5), 16))
    return t


def relocs(path):
    out = subprocess.run([OBJDUMP, "-r", path], capture_output=True,
                         text=True).stdout
    per = {}
    cur = None
    for line in out.splitlines():
        m = re.match(r"RELOCATION RECORDS FOR \[(\S+)\]:", line)
        if m:
            cur = m.group(1)
            per[cur] = []
            continue
        m = REL_RE.match(line)
        if m and cur:
            per[cur].append((int(m.group(1), 16), m.group(2), m.group(3),
                             int(m.group(4), 16) if m.group(4) else 0))
    return per


def syms(path):
    out = subprocess.run([NM, path], capture_output=True, text=True).stdout
    byaddr, byname = {}, {}
    for line in out.splitlines():
        p = line.split()
        if len(p) == 3:
            a, t, n = int(p[0], 16), p[1], p[2]
            byaddr.setdefault(a, n)
            byname[n] = (a, t)
    return byaddr, byname


def resolve(val, addend, symtab, byaddr):
    """Turn a relocated word into a name."""
    for base, name in byaddr.items():
        if base == val:
            return name
    return None


def main():
    path = sys.argv[1]
    want = sys.argv[2:]
    st, rl, (byaddr, byname) = secs(path), relocs(path), syms(path)
    data = open(path, "rb").read()
    for w in want:
        if w not in byname:
            print(f"== {w}: NOT FOUND")
            continue
        addr, typ = byname[w]
        sec = None
        for name, (size, vma, off, align) in st.items():
            if size and vma <= addr < vma + size:
                sec = (name, size, vma, off)
                break
        if not sec:
            print(f"== {w}: no section (abs)")
            continue
        name, size, vma, off = sec
        print(f"== {w}  {typ} in {name} @0x{addr:x} (size ~)")
        rmap = {r[0]: r for r in rl.get(name, [])}
        # the symbol's own extent: to the next symbol in the same section
        nxt = min([a for a in byaddr if a > addr and
                   any(a == v[0] for v in st.values())] or [addr + 0x100])
        end = min(nxt, addr + 0x200)
        for off_in in range(0, end - addr, 4):
            a = addr + off_in
            fo = off + (a - vma)
            raw = data[fo:fo + 4]
            if len(raw) < 4:
                break
            u32 = int.from_bytes(raw, "little")
            note = ""
            if a in rmap:
                _, rtype, sym, add = rmap[a]
                note = f"  RELOC {rtype} {sym}"
            elif a + 4 in rmap and rmap[a + 4][1] == "R_AARCH64_ABS64":
                _, rtype, sym, add = rmap[a + 4]
                note = f"  RELOC(hi) {rtype} {sym}"
            print(f"   +0x{off_in:02x}  0x{u32:08x} {u32:>12d}{note}")


if __name__ == "__main__":
    main()
