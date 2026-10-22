"""Resolve the stock apsys-dvfs.ko op tables into named function pointers.

The blob is built with CFI, so every function pointer in .rodata points at
a 4-byte CFI thunk in .text, not at the function itself. Each thunk is a
single `b` instruction whose target is fixed up by a relocation in
.rela.text.unlikely.__cfi_check_fail.

So the resolution is:
    rodata slot  --reloc-->  .text + thunk_off
    thunk_off    --reloc-->  .text + real_off
    real_off     --nm-->     function name

    python ko_ops.py <blob.ko>
"""
import re
import subprocess
import sys

READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")


def readelf_relocs(path):
    """-> {rela_section_name: {offset: (type, symname, addend)}}"""
    out = subprocess.run([READELF, "-rW", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        s = line.strip()
        m = re.match(r"Relocation section '(.+)' at offset .* contains (\d+) "
                     r"entries", s)
        if m:
            cur = m.group(1)
            res[cur] = {}
            continue
        if cur is None or not s or not s[0].isdigit():
            continue
        p = s.split()
        if len(p) < 5:
            continue
        off = int(p[0], 16)
        typ = p[2]
        name = p[4]
        addend = 0
        # readelf prints the columns as "value  name + addend", e.g.
        # "0000000000000000 .text + 11bd0"
        if len(p) >= 7 and p[5] == "+":
            try:
                addend = int(p[6], 16)
            except ValueError:
                addend = 0
        elif "+" in name:
            name, a = name.split("+", 1)
            try:
                addend = int(a.strip(), 16)
            except ValueError:
                addend = 0
        res[cur][off] = (typ, name, addend)
    return res


def nm_symbols(path):
    """-> {value: (name, type, size)}"""
    out = subprocess.run([NM, "-S", "--defined-only", path],
                         capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.setdefault(int(p[0], 16), (p[3], p[2], int(p[1], 16)))
        elif len(p) == 3 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.setdefault(int(p[0], 16), (p[2], p[1], 0))
    return res


def resolve(path):
    syms = nm_symbols(path)
    rel = readelf_relocs(path)

    # .text + off -> the real function offset
    thunk = {}
    for sec, ents in rel.items():
        if not sec.startswith(".rela.text"):
            continue
        for off, (typ, name, addend) in ents.items():
            if name == ".text":
                thunk[off] = addend

    # offset -> function name, for .text symbols
    by_off = {a: s[0] for a, s in syms.items() if s[1] in "tT"}

    # rodata slot -> thunk -> real
    def name_at(slot_off):
        for sec, ents in rel.items():
            if "rodata" not in sec:
                continue
            if slot_off in ents:
                typ, name, addend = ents[slot_off]
                if name == ".text":
                    real = thunk.get(addend)
                    if real is None:
                        return f".text+{addend:#x}(?)"
                    return by_off.get(real, f".text+{real:#x}")
                return f"{name}+{addend:#x}"
        return None

    return syms, name_at


def main():
    path = sys.argv[1]
    syms, name_at = resolve(path)
    for addr, (name, typ, size) in sorted(syms.items()):
        if not name.endswith("_dvfs_ops"):
            continue
        print(f"=== {name}  (rodata+{addr:#x}, {size} bytes, "
              f"{size // 8} slots) ===")
        for i in range(size // 8):
            slot = addr + i * 8
            print(f"   [{i:2d}] {name_at(slot)}")
        print()


if __name__ == "__main__":
    main()
