"""Extract the val_to_freq / val_to_volt string tables from the stock blob.

Each val_to_* function has this shape:

    if (level >= N) { printk("..."); return "undefined"; }
    return table[level];

`table` is an array of char pointers in .rodata. Two relocations give
it away:

    adrp x8, <page>        R_AARCH64_ADR_PREL_PG_HI21  .rodata + T
    add  x8, x8, #<lo12>   R_AARCH64_ADD_ABS_LO12_NC   .rodata + T

The table base is T. The table's own entries are R_AARCH64_ABS64
relocations to .rodata strings.

    python ko_valto.py <blob.ko> [function ...]
"""
import re
import subprocess
import sys

READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")
OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")


def nm_symbols(path):
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


def readelf_relocs(path):
    """-> {rela_section: {offset: (type, symname, addend)}}"""
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
        if len(p) >= 7 and p[5] == "+":
            try:
                addend = int(p[6], 16)
            except ValueError:
                addend = 0
        res[cur][off] = (typ, name, addend)
    return res


def section_bytes(path, secname):
    out = subprocess.run([READELF, "-x", secname, path],
                         capture_output=True, text=True).stdout
    data = bytearray()
    for line in out.splitlines():
        m = re.match(r"^\s+0x([0-9a-f]+)\s+((?:[0-9a-f]{2,8}\s+){1,4})", line)
        if m:
            for w in m.group(2).split():
                data += bytes.fromhex(w)
    return bytes(data)


def cstr(data, off):
    end = data.find(b"\0", off)
    if end < 0:
        return None
    return data[off:end].decode("utf-8", "replace")


def disasm(path, start, end):
    """-> {addr: (mnemonic, op_str)}"""
    out = subprocess.run([OBJDUMP, "-d", f"--start-address={start:#x}",
                          f"--stop-address={end:#x}", path],
                         capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        m = re.match(r"^\s+([0-9a-f]+):\s+[0-9a-f]{8}\s+(\S+)\s*(.*)$", line)
        if m:
            res[int(m.group(1), 16)] = (m.group(2), m.group(3).strip())
    return res


def find_tables(path, syms, wants):
    """-> {func_name: (table_base, [strings])}"""
    rel = readelf_relocs(path)
    rodata = section_bytes(path, ".rodata")

    # the .rodata pointer-table relocations
    rodrel = {}
    for sec, ents in rel.items():
        if sec.startswith(".rela.rodata"):
            for off, (typ, name, addend) in ents.items():
                rodrel[off] = (name, addend)

    # every relocation, for the adrp/add pairs
    allrel = {}
    for sec, ents in rel.items():
        for off, v in ents.items():
            allrel[(sec, off)] = v

    out = {}
    for want in wants:
        entry = next((a for a, (n, t, s) in syms.items()
                      if n == want and t == "T"), None)
        if entry is None:
            continue
        size = syms[entry][2]
        ins = disasm(path, entry, entry + size)

        table = None
        limit = None
        for addr, (mn, ops) in sorted(ins.items()):
            if mn == "cmp":
                mm = re.match(r"w(\d+), #(0x[0-9a-f]+|\d+)", ops)
                if mm and int(mm.group(1)) == 0:
                    limit = int(mm.group(2), 0)
            if mn != "adrp":
                continue
            r = re.match(r"x(\d+),", ops)
            if not r:
                continue
            xn = int(r.group(1))
            addend = None
            for a2 in range(addr + 4, min(addr + 20, entry + size), 4):
                m2, o2 = ins.get(a2, ("", ""))
                if m2 != "add":
                    continue
                mm = re.match(r"x(\d+), x(\d+), #", o2)
                if not (mm and int(mm.group(1)) == xn
                        and int(mm.group(2)) == xn):
                    continue
                for sec in (".rela.text.unlikely.__cfi_check_fail",
                            ".rela.text"):
                    rel_ = allrel.get((sec, a2))
                    if rel_ and rel_[1] == ".rodata":
                        addend = rel_[2]
                        break
                break
            if addend is None:
                continue

            # is the result used as a table base? look for the ldr with
            # a scaled index
            for a3, (m3, o3) in sorted(ins.items()):
                if m3 != "ldr":
                    continue
                mm = re.match(r"x\d+, \[x(\d+), (w\d+|x\d+), sxtw #3\]", o3)
                if not mm or int(mm.group(1)) != xn:
                    continue
                table = addend
                break
            if table is not None:
                break

        if table is None:
            continue

        strs = []
        i = 0
        while True:
            if limit is not None and i >= limit:
                break
            off = table + i * 8
            if off not in rodrel:
                break
            name, addend = rodrel[off]
            if name != ".rodata":
                break
            strs.append(cstr(rodata, addend))
            i += 1
        out[want] = (table, strs, limit)
    return out


def main():
    path = sys.argv[1]
    syms = nm_symbols(path)
    wants = sys.argv[2:]
    if not wants:
        wants = sorted(n for a, (n, t, s) in syms.items()
                       if "val_to_" in n and t == "T")
    res = find_tables(path, syms, wants)
    for want in wants:
        if want not in res:
            print(f"!! {want}: no table found")
            continue
        table, strs, limit = res[want]
        print(f"=== {want}  table=.rodata+{table:#x}  n={len(strs)} "
              f"(bound {limit}) ===")
        for i, s in enumerate(strs):
            print(f"   [{i}] {s!r}")
        print()


if __name__ == "__main__":
    main()
