"""Dump a stock blob's rodata/data symbol: raw bytes + relocations.

    python3 donors/dump_sym.py <blob.ko> <sym> [<sym>...]

Relocations are what turn the raw words into pointers, so a data table
like ump9620_data is only readable once both are shown.
"""
import re
import subprocess
import sys

OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")


def sections(path):
    out = subprocess.run([OBJDUMP, "-h", path], capture_output=True,
                         text=True).stdout
    secs = {}
    for line in out.splitlines():
        m = re.match(r"\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+"
                     r"([0-9a-f]+)\s+([0-9a-f]+)\s+2\*\*(\d)", line)
        if m:
            secs[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16),
                                int(m.group(5), 16))
    return secs


def relocs(path):
    out = subprocess.run([OBJDUMP, "-r", path], capture_output=True,
                         text=True).stdout
    rel = {}
    for line in out.splitlines():
        m = re.match(r"([0-9a-f]{16})\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s*(\S*)",
                     line)
        if m:
            rel.setdefault(m.group(1).lstrip("0") or "0", []).append(
                (int(m.group(4), 16), m.group(3), m.group(5)))
    return rel


def syms(path):
    out = subprocess.run([NM, path], capture_output=True, text=True).stdout
    s = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) == 3:
            s[p[2]] = (int(p[0], 16), p[1])
    return s


def main():
    path = sys.argv[1]
    want = sys.argv[2:]
    secs, rel, sy = sections(path), relocs(path), syms(path)
    data = open(path, "rb").read()
    for w in want:
        if w not in sy:
            print(f"== {w}: NOT FOUND")
            continue
        addr, typ = sy[w]
        sec = None
        for name, (size, vma, off) in secs.items():
            if vma <= addr < vma + size and size:
                sec = (name, size, vma, off)
                break
        print(f"== {w}  addr=0x{addr:x} type={typ} section="
              f"{sec[0] if sec else '?'}")
        if not sec:
            continue
        name, size, vma, off = sec
        start = off + (addr - vma)
        n = min(64, off + size - start)
        blob = data[start:start + n]
        for i in range(0, len(blob), 8):
            w8 = blob[i:i + 8]
            if len(w8) < 8:
                break
            val = int.from_bytes(w8, "little")
            tags = []
            for raddr, rtype, rsym in rel.get(hex(addr + i)[2:], []):
                tags.append(f"{rsym}+{raddr - (addr + i):#x}")
            print(f"   +0x{i:02x}  0x{val:016x}  {val:>20d}"
                  f"   {' '.join(tags)}")
        for raddr, rtype, rsym in rel.get(hex(addr)[2:], []):
            print(f"   RELOC @+0x{raddr - addr:x}: {rtype} {rsym}")


if __name__ == "__main__":
    main()
