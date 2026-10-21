"""Dump the per-SoC DVFS op tables of the stock apsys-dvfs.ko.

For each `<soc>_<block>_dvfs_ops` table, resolve every 8-byte slot to
the function symbol it points at, using the .rela.rodata relocations
that fill the table in.

    python ko_optbl.py <blob.ko> [table-name ...]
"""
import bisect
import re
import subprocess
import sys

READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")


def sections(path):
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
    order = []
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
        try:
            off = int(p[3], 16)
            size = int(p[4], 16)
        except ValueError:
            continue
        secs[p[0]] = (off, size)
        order.append(p[0])
    return secs, order


def symbols(path):
    """{name: (value, size, type, section_name)} using the section index"""
    out = subprocess.run([READELF, "-sW", path], capture_output=True,
                         text=True).stdout
    secs, order = sections(path)
    res = {}
    for line in out.splitlines():
        m = re.match(r"^\s*\d+:\s+([0-9a-f]{16})\s+(\d+)\s+(\S+)\s+"
                     r"(\S+)\s+(\S+)\s+(\S+)\s+(.*)$", line)
        if not m:
            continue
        value = int(m.group(1), 16)
        size = int(m.group(2))
        typ = m.group(3)
        ndx = m.group(6)
        name = m.group(7).strip()
        # readelf's section index is 1-based and section 0 is the NULL
        # entry, which order[] already holds at index 0
        sec = order[int(ndx)] if ndx.isdigit() and int(ndx) < len(order) else None
        if not name:
            continue
        res.setdefault(name, (value, size, typ, sec))
    return res


def relocs(path):
    """{relsec: {offset: (type, symname, addend)}}"""
    out = subprocess.run([READELF, "-rW", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        s = line.strip()
        if s.startswith("Relocation section"):
            cur = s.split("'")[1] if "'" in s else None
            res.setdefault(cur, {})
            continue
        m = re.match(r"^([0-9a-f]{16})\s+[0-9a-f]{16}\s+(\S+)\s+"
                     r"([0-9a-f]{16})\s+(\S+)\s*(.*)$", s)
        if not m:
            continue
        off = int(m.group(1), 16)
        typ = m.group(2)
        name = m.group(4)
        rest = m.group(5).strip()
        addend = 0
        # readelf prints the addend in hex after a '+', e.g. "+ 54fc"
        mm = re.search(r"\+\s*([0-9a-fA-F]+)\s*$", rest)
        if mm:
            addend = int(mm.group(1), 16)
        res[cur][off] = (typ, name, addend)
    return res


def main():
    path = sys.argv[1]
    wants = sys.argv[2:]
    secs, order = sections(path)
    sm = symbols(path)
    rel = relocs(path)

    # per-section sorted list of (start, end, name) for function symbols
    fns = {}
    for n, (v, sz, t, sec) in sm.items():
        if sec and t == "FUNC":
            fns.setdefault(sec, []).append((v, v + sz, n))
    for sec in fns:
        fns[sec].sort()

    # the kernel builds with CONFIG_CFI_CLANG, which puts a 4-byte
    # "b __cfi_check_fail" thunk in front of every function. The op
    # tables point at the thunk, so follow the JUMP26 reloc to the
    # real function.
    thunks = {}
    for rs, ents in rel.items():
        if not rs.startswith(".rela.text"):
            continue
        for off, (typ, name, addend) in ents.items():
            if typ == "R_AARCH64_JUMP26" and name == ".text":
                thunks[off] = addend

    def resolve(name, addend):
        if name in sm:
            v, sz, t, sec = sm[name]
            if t == "FUNC":
                return name
        if name.startswith("."):
            # the op table points at a CFI thunk; follow it to the
            # real function
            if addend in thunks:
                addend = thunks[addend]
            cands = fns.get(name, [])
            i = bisect.bisect_right(cands, (addend, float("inf"), ""))
            for j in range(max(0, i - 4), i):
                s, e, n = cands[j]
                if s <= addend < e:
                    return n
            return f"{name}+{addend:#x}"
        return name

    tables = {}
    for n, (v, sz, t, sec) in sm.items():
        if t == "OBJECT" and n.endswith("_dvfs_ops") and sz > 8:
            tables[n] = (v, sz, sec or ".rodata")

    for n, (a, sz, sec) in sorted(tables.items(), key=lambda x: x[1][0]):
        if wants and n not in wants:
            continue
        relsec = ".rela" + sec
        entries = {}
        for rs, ents in rel.items():
            if rs == relsec or rs.startswith(relsec + "."):
                entries.update(ents)
        print(f"=== {n}   {sec}+{a:#x}  size={sz:#x} ({sz // 8} slots)")
        for i in range(sz // 8):
            off = a + i * 8
            r = entries.get(off)
            if r is None:
                print(f"   [{i:2d}]  <no reloc>")
                continue
            typ, name, addend = r
            print(f"   [{i:2d}]  {resolve(name, addend)}")
        print()


if __name__ == "__main__":
    main()
