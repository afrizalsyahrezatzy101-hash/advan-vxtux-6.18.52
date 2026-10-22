"""Extract the per-SoC ip_dvfs_map_cfg tables from the stock blob.

Each get_*_dvfs_table() copies a static table of

    struct ip_dvfs_map_cfg {
        u32 map_index;
        u32 volt_level;
        u32 clk_level;
        u32 clk_rate;
        char *volt_val;
    };

out of .data (or .rodata) into the caller's array. The function's
adrp/add pair gives the table base, and the copy offsets give the
number of entries.

    python ko_maptbl.py <blob.ko> <func> [...]
"""
import re
import struct
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
            res.setdefault(p[3], (int(p[0], 16), int(p[1], 16), p[2]))
        elif len(p) == 3 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.setdefault(p[2], (int(p[0], 16), 0, p[1]))
    return res


def readelf_relocs(path):
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
        name = p[4]
        addend = 0
        if len(p) >= 7 and p[5] == "+":
            try:
                addend = int(p[6], 16)
            except ValueError:
                addend = 0
        res[cur][off] = (p[2], name, addend)
    return res


def section_bytes(path, secname):
    out = subprocess.run([READELF, "-x", secname, path],
                         capture_output=True, text=True).stdout
    data = bytearray()
    for line in out.splitlines():
        m = re.match(r"^\s+0x([0-9a-f]+)\s+((?:[0-9a-f]{8}\s+){1,4})", line)
        if m:
            for w in m.group(2).split():
                data += bytes.fromhex(w)
    return bytes(data)


def disasm(path, start, end):
    out = subprocess.run([OBJDUMP, "-d", f"--start-address={start:#x}",
                          f"--stop-address={end:#x}", path],
                         capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        m = re.match(r"^\s+([0-9a-f]+):\s+([0-9a-f]{8})\s+(\S+)\s*(.*)$", line)
        if m:
            res[int(m.group(1), 16)] = (int(m.group(2), 16), m.group(3),
                                        m.group(4).strip())
    return res


def extract(path, func):
    """-> (section, offset, [(map_index, volt, clk, clk_rate, volt_val)])"""
    syms = nm_symbols(path)
    rel = readelf_relocs(path)
    secs = {n: section_bytes(path, n) for n in (".data", ".rodata")}
    if func not in syms:
        return None
    addr, size, _ = syms[func]
    ins = disasm(path, addr, addr + size)

    # the adrp that computes the table base
    base = None
    base_sec = None
    for a, (word, mn, ops) in sorted(ins.items()):
        if mn != "adrp":
            continue
        reg = re.match(r"x(\d+),", ops)
        if not reg:
            continue
        xn = int(reg.group(1))
        for a2 in range(a + 4, min(a + 16, addr + size), 4):
            w2, m2, o2 = ins.get(a2, (0, "", ""))
            if m2 != "add":
                continue
            mm = re.match(r"x(\d+), x(\d+), #(0x[0-9a-f]+|\d+)", o2)
            if not (mm and int(mm.group(1)) == xn
                    and int(mm.group(2)) == xn):
                continue
            imm = int(mm.group(3), 0)
            for sec, ents in rel.items():
                if not sec.startswith(".rela.text"):
                    continue
                r = ents.get(a2)
                if r and r[1] in (".data", ".rodata"):
                    # the ADD_ABS_LO12_NC relocation already carries the
                    # full symbol offset, so no page/lo12 split is needed
                    base = r[2]
                    base_sec = "." + r[1][1:]
            break
        if base is not None:
            break
    if base is None:
        return None

    # the last store offset tells us the entry count
    last = 0
    for a, (word, mn, ops) in ins.items():
        if mn != "str":
            continue
        mm = re.match(r"w\d+, \[x\d+, #(\d+)\]", ops)
        if mm:
            last = max(last, int(mm.group(1)))
    # entries are 24 bytes; the highest copied dword is at (n-1)*24+12
    n = (last - 12) // 24 + 1

    data = secs[base_sec]
    out = []
    for k in range(n):
        o = base + k * 24
        if o + 24 > len(data):
            break
        mi, vl, cl, cr = struct.unpack_from("<IIII", data, o)
        vp = struct.unpack_from("<Q", data, o + 16)[0]
        vs = ""
        for sn, sd in secs.items():
            if 0 < vp < len(sd):
                e = sd.find(b"\0", vp)
                vs = sd[vp:e].decode("utf-8", "replace")
                break
        out.append((mi, vl, cl, cr, vs))
    return base_sec, base, out


def main():
    path = sys.argv[1]
    wants = sys.argv[2:]
    for w in wants:
        r = extract(path, w)
        if r is None:
            print(f"!! {w}: not found")
            continue
        sec, off, rows = r
        print(f"=== {w}  {sec}+{off:#x}  {len(rows)} entries")
        for i, (mi, vl, cl, cr, vs) in enumerate(rows):
            print(f"   [{i}] map_index={mi} volt_level={vl} clk_level={cl} "
                  f"clk_rate={cr} volt_val={vs!r}")
        print()


if __name__ == "__main__":
    main()
