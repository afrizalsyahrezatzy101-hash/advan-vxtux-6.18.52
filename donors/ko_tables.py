"""Dump the stock apsys-dvfs.ko op tables and their string tables.

A Linux module gives every section address 0, so a symbol's nm address is
the offset inside its own section. objdump -r prints the relocations
per section, which is what fills the pointer slots of the op tables.

    python ko_tables.py <blob.ko> ops      # the 19 op tables
    python ko_tables.py <blob.ko> strings   # the .rodata strings
"""
import re
import subprocess
import sys

OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")


def reloc_sections(path):
    """-> {section: [(offset, type, value)]}"""
    out = subprocess.run([OBJDUMP, "-r", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            cur = line.split("[")[1].split("]")[0]
            res[cur] = []
            continue
        if cur is None:
            continue
        p = line.split()
        if len(p) == 3 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res[cur].append((int(p[0], 16), p[1], p[2]))
    return res


def symbols(path):
    """-> [(addr, size, type, name)]"""
    out = subprocess.run([NM, "-S", "--defined-only", path],
                         capture_output=True, text=True).stdout
    res = []
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.append((int(p[0], 16), int(p[1], 16), p[2], p[3]))
        elif len(p) == 3 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.append((int(p[0], 16), 0, p[1], p[2]))
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


def cstrings(path, secname):
    """-> [(offset, string)] for every NUL terminated string in the section"""
    data = section_bytes(path, secname)
    res = []
    i = 0
    while i < len(data):
        j = data.find(b"\0", i)
        if j < 0:
            break
        if j > i:
            res.append((i, data[i:j].decode("utf-8", "replace")))
        i = j + 1
    return res


def main():
    path = sys.argv[1]
    what = sys.argv[2] if len(sys.argv) > 2 else "ops"
    syms = symbols(path)
    rel = reloc_sections(path)

    if what == "ops":
        for addr, size, typ, name in syms:
            if not name.endswith("_dvfs_ops"):
                continue
            print(f"=== {name}  (rodata+{addr:#x}, {size} bytes) ===")
            for rsec, ents in rel.items():
                if "rodata" not in rsec:
                    continue
                for off, rtyp, val in ents:
                    if not (addr <= off < addr + size):
                        continue
                    slot = (off - addr) // 8
                    print(f"   [{slot:2d}] +{off - addr:#05x} {rtyp:18s} {val}")
            print()

    elif what == "strings":
        for secname in (".rodata.str1.1", ".rodata"):
            try:
                strs = cstrings(path, secname)
            except Exception:
                continue
            if not strs:
                continue
            print(f"=== {secname} ===")
            for off, s in strs:
                print(f"   {off:#08x} {s!r}")
            print()


if __name__ == "__main__":
    main()
