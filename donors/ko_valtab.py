"""Extract the stock apsys-dvfs.ko's `<soc>_<blk>_val_to_*` string
tables.

Each function is a one-liner: it indexes a pointer table with the
level number and returns the string, or an "invalid level" string when
the level is out of range:

    const char *sharkl5pro_dpu_val_to_freq(u32 level)
    {
            if (level < 5)
                    return dpu_freq_str[level];
            else
                    return "invalid level";
    }

The pointer table lives in .data and is filled in by ABS64 relocs that
point into .rodata.str1.1.

    python ko_valtab.py <blob.ko> <func> [...]
"""
import re
import subprocess
import sys

READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")


def sections(path):
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
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
            secs[p[0]] = (int(p[3], 16), int(p[4], 16))
        except ValueError:
            continue
    return secs


def symbols(path):
    out = subprocess.run([READELF, "-sW", path], capture_output=True,
                         text=True).stdout
    res = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) < 8 or p[3] != "FUNC":
            continue
        try:
            v = int(p[1], 16)
        except ValueError:
            continue
        res.setdefault(p[7], (v, int(p[2])))
    return res


def relocs(path):
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
        mm = re.search(r"\+\s*([0-9a-fA-F]+)\s*$", rest)
        if mm:
            addend = int(mm.group(1), 16)
        res[cur][off] = (typ, name, addend)
    return res


def section_bytes(path, secname):
    out = subprocess.run([READELF, "-x", secname, path],
                         capture_output=True, text=True).stdout
    d = bytearray()
    for line in out.splitlines():
        m = re.match(r"^\s+0x([0-9a-f]+)\s+((?:[0-9a-f]{8}\s+){1,4})", line)
        if m:
            for w in m.group(2).split():
                d += bytes.fromhex(w)
    return bytes(d)


def main():
    path = sys.argv[1]
    wants = sys.argv[2:]
    secs = sections(path)
    sm = symbols(path)
    rel = relocs(path)

    raw = {}
    for n in (".rodata", ".data"):
        if n in secs:
            raw[n] = section_bytes(path, n)

    def cstr(sec, addr):
        data = raw.get(sec)
        if data is None or not (0 <= addr < len(data)):
            return None
        e = data.find(b"\0", addr)
        return data[addr:e].decode("utf-8", "replace")

    # the .data relocs fill the pointer tables
    dentries = {}
    for rs, ents in rel.items():
        if rs.startswith(".rela.data"):
            dentries.update(ents)
    # the .text relocs let us find the table base the function uses
    tentries = {}
    for rs, ents in rel.items():
        if rs.startswith(".rela.text"):
            tentries.update(ents)

    for fn in wants:
        if fn not in sm:
            print(f"!! {fn}: not found")
            continue
        addr, size = sm[fn]
        # the table base: the ADRP/ADD pair that computes the pointer
        # table address, and the "invalid level" string it falls back to
        base = None
        inval = None
        fmt = None
        for off, (typ, name, addend) in sorted(tentries.items()):
            if not (addr <= off < addr + size):
                continue
            if typ != "R_AARCH64_ADD_ABS_LO12_NC" or name != ".rodata":
                continue
            s = cstr("rodata", addend)
            # the table is an array of pointers, so the first entry is
            # itself a pointer into .rodata; the fallback is a plain
            # string
            if s is None:
                continue
            if base is None and s in rows_cache:
                base = addend
            else:
                if base is None:
                    base = addend
                elif inval is None:
                    inval = s
        if base is None:
            print(f"!! {fn}: no table base found")
            continue
        rows = []
        o = base
        while True:
            r = entries.get(o)
            if r is None:
                break
            typ, name, addend = r
            if typ != "R_AARCH64_ABS64":
                break
            rows.append(cstr("rodata", addend))
            o += 8
        print(f"=== {fn}  .rodata+{base:#x}  {len(rows)} entries")
        for i, s in enumerate(rows):
            print(f"   [{i}] {s!r}")
        print(f"   fallback: {inval!r}")
        print()


if __name__ == "__main__":
    main()
