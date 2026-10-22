"""Read the pointer tables out of a relocatable kernel module.

In a .ko every section has vaddr 0, so a symbol's value is its offset
inside its own section. A pointer slot in .rodata holds the *addend* of
its relocation, which for a function pointer is the offset of the
target function inside .text. Matching that offset against nm's symbol
list gives the function name.

    python ko_ptrs.py <blob.ko> <table_symbol> [...]
"""
import re
import struct
import subprocess
import sys

OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")


def nm_symbols(path):
    """-> {value: (name, type, size)} for every defined symbol"""
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


def section_bytes(path, secname):
    out = subprocess.run([READELF, "-x", secname, path],
                         capture_output=True, text=True).stdout
    data = bytearray()
    for line in out.splitlines():
        m = re.match(r"^\s+0x[0-9a-f]+\s+((?:[0-9a-f]{2,8}\s+){1,4})", line)
        if m:
            for w in m.group(1).split():
                data += bytes.fromhex(w)
    return bytes(data)


def rela_by_section(path):
    """-> {section: {offset: (type, symname, addend)}}

    readelf -rW prints:
        Offset  Info  Type  Sym.Value  Symbol's Name + Addend
    """
    out = subprocess.run([READELF, "-rW", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        s = line.strip()
        if s.startswith("Relocation section"):
            m = re.search(r"'\.(rela\S*)'", s)
            cur = m.group(1) if m else None
            if cur:
                cur = cur[len("rela"):]
            res.setdefault(cur, {})
            continue
        if cur is None or not s or not s[0].isdigit():
            continue
        p = s.split()
        if len(p) < 4:
            continue
        off = int(p[0], 16)
        typ = p[2]
        name = p[4] if len(p) > 4 else None
        addend = 0
        if name and "+" in name:
            name, a = name.split("+", 1)
            try:
                addend = int(a.strip(), 16)
            except ValueError:
                addend = 0
        if name is None:
            continue
        res[cur][off] = (typ, name, addend)
    return res


def main():
    path = sys.argv[1]
    syms = nm_symbols(path)
    rel = rela_by_section(path)
    secs = {s: section_bytes(path, s) for s in (".rodata", ".data")
            if section_bytes(path, s)}
    ro = secs.get(".rodata", b"")

    for want in sys.argv[2:]:
        # find the symbol
        entry = next((v for v, s in syms.items() if s[0] == want), None)
        if entry is None:
            print(f"!! {want}: not found")
            continue
        size = syms[entry][2]
        print(f"=== {want} (rodata+{entry:#x}, {size} bytes, {size//8} slots) ===")
        for i in range(size // 8):
            off = entry + i * 8
            raw = struct.unpack_from("<Q", ro, off)[0]
            # the relocation for this slot tells us the real target
            tgt = None
            for rsec, ents in rel.items():
                if off in ents:
                    typ, name, addend = ents[off]
                    tgt = (typ, name, addend)
                    break
            # the addend is the offset of the target inside its section
            resolved = None
            if tgt:
                typ, name, addend = tgt
                if name in syms:
                    resolved = syms[name][0]
                elif addend in syms:
                    resolved = syms[addend][0]
                else:
                    resolved = f"{name}+{addend:#x}"
            print(f"  [{i:2d}] raw={raw:#018x}  -> {resolved}")
        print()


if __name__ == "__main__":
    main()
