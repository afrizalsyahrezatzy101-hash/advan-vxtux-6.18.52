"""Reconstruct the apsys-dvfs.ko data tables from its ELF relocations.

The blob is a relocatable object (ET_REL): every section address is 0, so a
symbol's "value" is its offset inside its own section.

The .text section's function boundaries come from the Ghidra export
(donors/ghidra_out/apsys-dvfs.ko.funcs.txt), because the ELF symbol table
only carries the "real" (non-clone) names.
"""
import struct, sys, os
from collections import defaultdict

KO = r"E:\vxtux_build_repo\donors\stock_vendor_modules\apsys-dvfs.ko"
FUNCS = r"E:\vxtux_build_repo\donors\ghidra_out\apsys-dvfs.ko.funcs.txt"
data = open(KO, "rb").read()

# ---- function table from the Ghidra export
funcs = []          # (addr, name)
for line in open(FUNCS, encoding="utf-8", errors="replace"):
    p = line.split()
    if len(p) >= 2 and p[1] == "Function":
        funcs.append((int(p[0], 16), p[0]))
funcs.sort()
FADDR = [f[0] for f in funcs]
FNAME = [f[1] for f in funcs]

def text_func_at(addr):
    import bisect
    i = bisect.bisect_right(FADDR, addr) - 1
    if i < 0:
        return "text+0x%x" % addr
    return FNAME[i]

# ---- ELF
e_shoff = struct.unpack_from("<Q", data, 0x28)[0]
e_shentsize = struct.unpack_from("<H", data, 0x3a)[0]
e_shnum = struct.unpack_from("<H", data, 0x3c)[0]
e_shstrndx = struct.unpack_from("<H", data, 0x3e)[0]
secs = []
for i in range(e_shnum):
    off = e_shoff + i * e_shentsize
    name, typ, flags, addr, offset, size, link, info, align, entsize = \
        struct.unpack_from("<IIQQQQIIQQ", data, off)
    secs.append(dict(name=name, type=typ, flags=flags, addr=addr, offset=offset,
                     size=size, link=link, info=info, entsize=entsize))
shstr = secs[e_shstrndx]
for s in secs:
    o = shstr["offset"] + s["name"]
    s["name"] = data[o:data.index(b"\0", o)].decode()

def sec(name):
    for s in secs:
        if s["name"] == name:
            return s

symtab = sec(".symtab")
strtab = secs[symtab["link"]]
syms = []
for i in range(symtab["size"] // 24):
    off = symtab["offset"] + i * 24
    nm, info, other, shndx, value, size = struct.unpack_from("<IBBHQQ", data, off)
    o = strtab["offset"] + nm
    name = data[o:data.index(b"\0", o)].decode()
    syms.append(dict(name=name, info=info, shndx=shndx, value=value, size=size,
                     bind=info >> 4, type=info & 0xf))
by_name = defaultdict(list)
for s in syms:
    if s["name"]:
        by_name[s["name"]].append(s)

TEXTIDX = [i for i, s in enumerate(secs) if s["name"] == ".text"][0]
RODATAIDX = [i for i, s in enumerate(secs) if s["name"] == ".rodata"][0]
DATAIDX = [i for i, s in enumerate(secs) if s["name"] == ".data"][0]
BSSIDX = [i for i, s in enumerate(secs) if s["name"] == ".bss"][0]

def cstr(secidx, off):
    s = secs[secidx]
    o = s["offset"] + off
    return data[o:data.index(b"\0", o)].decode("utf-8", "replace")

def resolve(shndx, addend):
    if shndx == TEXTIDX:
        return text_func_at(addend)
    if shndx == RODATAIDX:
        return ".rodata+0x%x  /* %r */" % (addend, cstr(RODATAIDX, addend))
    if shndx == DATAIDX:
        return ".data+0x%x" % addend
    if shndx == BSSIDX:
        return ".bss+0x%x" % addend
    if shndx == 0:
        for nm, lst in by_name.items():
            for s in lst:
                if s["value"] == addend and s["shndx"] in (0, 0xfffffff0, 0xffffffffffffffff):
                    return nm
        return "ABS+0x%x" % addend
    return secs[shndx]["name"] + "+0x%x" % addend

def relocs_of(secname):
    s = None
    for c in secs:
        if c["name"].startswith(".rela" + secname):
            s = c
            break
    if s is None:
        return []
    out = []
    for i in range(s["size"] // 24):
        off = s["offset"] + i * 24
        r_off, r_info, r_addend = struct.unpack_from("<QQq", data, off)
        rsym = r_info >> 32
        out.append((r_off, syms[rsym]["shndx"], r_addend, syms[rsym]["name"]))
    return out

def dump(secname, start, size, label=""):
    s = sec(secname)
    raw = data[s["offset"] + start: s["offset"] + start + size]
    rel = {r[0]: r for r in relocs_of(secname) if start <= r[0] < start + size}
    print("--- %s @ %s+0x%x (size 0x%x)" % (label, secname, start, size))
    for i in range(0, size, 8):
        v = struct.unpack_from("<Q", raw, i)[0]
        if start + i in rel:
            _, sh, ad, sn = rel[start + i]
            print("  +0x%03x: %016x  -> %s" % (i, v, resolve(sh, ad)))
        else:
            print("  +0x%03x: 0x%x" % (i, v))

def dumpsym(name):
    for s in sorted(by_name.get(name, []), key=lambda x: x["value"]):
        print("%-44s sec=%-22s val=0x%06x size=0x%-5x type=%d bind=%d" % (
            s["name"], secs[s["shndx"]]["name"] if s["shndx"] < len(secs) else str(s["shndx"]),
            s["value"], s["size"], s["type"], s["bind"]))

if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "syms":
        for w in sys.argv[2:]:
            dumpsym(w)
    elif cmd == "dump":
        for w in sys.argv[2:]:
            s = sorted(by_name.get(w, []), key=lambda x: x["value"])[0]
            dump(secs[s["shndx"]]["name"], s["value"], s["size"] or 0x40, w)
    elif cmd == "relocs":
        for r in sorted(relocs_of(sys.argv[2])):
            print("0x%06x -> %s" % (r[0], resolve(r[1], r[2])))
    elif cmd == "str":
        for a in sys.argv[2:]:
            print(repr(cstr(RODATAIDX, int(a, 16))))
    elif cmd == "text":
        for a in [int(x, 16) for x in sys.argv[2:]]:
            print(hex(a), text_func_at(a))
