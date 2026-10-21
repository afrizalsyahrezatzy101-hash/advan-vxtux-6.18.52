"""Resolve a .ko's relocations against its own symbol table.

The readelf text output is ambiguous for section symbols (".text + 2330"
is a section-relative offset, and the printed "Symbol's Value" column is
the section address, not the target). This parses the ELF directly, so
every relocation resolves to a real symbol name and offset.

Usage:
    python3 donors/elf_reloc.py <file.ko> [--sec .rodata] [--sym NAME]

    --sec   only relocations whose target section matches
    --sym   only show relocations that land in/next to this symbol
"""
import struct
import sys

R_AARCH64_ABS64 = 257


class Elf:
    def __init__(self, path):
        self.path = path
        with open(path, "rb") as fh:
            self.data = fh.read()
        d = self.data
        if d[:4] != b"\x7fELF":
            raise SystemExit("not an ELF")
        self.is64 = d[4] == 2
        little = d[5] == 1
        e = "<" if little else ">"
        self.e = e
        (self.e_type, self.e_machine, _ver, self.e_entry, self.e_phoff,
         self.e_shoff, self.e_flags, self.e_ehsize, self.e_phentsize,
         self.e_phnum, self.e_shentsize, self.e_shnum,
         self.e_shstrndx) = struct.unpack_from(e + "HHIQQQIHHHHHH", d, 16)

        # section headers
        self.shdrs = []
        for i in range(self.e_shnum):
            off = self.e_shoff + i * self.e_shentsize
            (name, typ, flags, addr, offset, size, link, info, align,
             entsize) = struct.unpack_from(e + "IIQQQQIIQQ", d, off)
            self.shdrs.append(dict(name=name, type=typ, flags=flags,
                                   addr=addr, offset=offset, size=size,
                                   link=link, info=info, entsize=entsize))

        # section name string table
        shstr = self.shdrs[self.e_shstrndx]
        self.shstrtab = d[shstr["offset"]:shstr["offset"] + shstr["size"]]

        # symbol tables
        self.syms = []
        for sh in self.shdrs:
            if sh["type"] != 2:  # SHT_SYMTAB
                continue
            strtab = self.shdrs[sh["link"]]
            st = d[strtab["offset"]:strtab["offset"] + strtab["size"]]
            n = sh["size"] // 24
            for i in range(n):
                off = sh["offset"] + i * 24
                (nameoff, info, other, shndx, value, size) = \
                    struct.unpack_from(e + "IBBHQQ", d, off)
                nm = self._cstr(st, nameoff)
                self.syms.append(dict(name=nm, info=info, shndx=shndx,
                                      value=value, size=size))

    def _cstr(self, buf, off):
        end = buf.find(b"\0", off)
        return buf[off:end].decode("utf-8", "replace")

    def secname(self, idx):
        return self._cstr(self.shstrtab, self.shdrs[idx]["name"])

    def symname(self, idx):
        s = self.syms[idx]
        if s["name"]:
            return s["name"]
        return "<sec %s>" % self.secname(s["shndx"])

    def sec_index(self, name):
        for i, sh in enumerate(self.shdrs):
            if self.secname(i) == name:
                return i
        return None

    def relocs(self):
        """Yield (target_secname, offset, symname, symval, symshndx, addend)"""
        e = self.e
        d = self.data
        for i, sh in enumerate(self.shdrs):
            if sh["type"] != 4:  # SHT_RELA
                continue
            tgt = self.secname(sh["info"])
            n = sh["size"] // 24
            for k in range(n):
                off = sh["offset"] + k * 24
                (roff, rinfo, radd) = struct.unpack_from(e + "QQq", d, off)
                symidx = rinfo >> 32
                rtype = rinfo & 0xffffffff
                s = self.syms[symidx]
                yield (tgt, roff, self.symname(symidx), s["value"],
                       s["shndx"], radd, rtype)


def fmt(elf, sec_filter=None, sym_filter=None):
    # a map of (section name) -> list of (value, size, name)
    by_sec = {}
    for s in elf.syms:
        if not s["name"] or s["shndx"] >= len(elf.shdrs):
            continue
        sn = elf.secname(s["shndx"])
        by_sec.setdefault(sn, []).append((s["value"], s["size"], s["name"]))
    for k in by_sec:
        by_sec[k].sort()

    def resolve(sn, val):
        cands = by_sec.get(sn, [])
        for v, sz, nm in cands:
            if v == val:
                return nm
        # nearest preceding
        best = None
        for v, sz, nm in cands:
            if v <= val:
                best = (v, sz, nm)
            else:
                break
        if best:
            v, sz, nm = best
            if sz:
                return "%s+0x%x" % (nm, val - v)
            return "%s+0x%x" % (nm, val - v)
        return None

    for (tgt, roff, nm, val, shndx, add, rtype) in elf.relocs():
        if sec_filter and tgt != sec_filter:
            continue
        # the symbol is a SECTION symbol: the target is section+addend
        if shndx < len(elf.shdrs) and not nm.startswith("<sec "):
            if elf.syms[0] is not None:
                pass
        # find the symtab entry
        pass

    out = []
    for (tgt, roff, nm, val, shndx, add, rtype) in elf.relocs():
        if sec_filter and tgt != sec_filter:
            continue
        # locate the symbol record to know if it is a section symbol
        # (shndx != SHN_UNDEF and name empty) -> target = section + addend
        sidx = None
        for i, s in enumerate(elf.syms):
            if s["name"] == nm and s["shndx"] == shndx and s["value"] == val:
                sidx = i
                break
        is_secsym = sidx is not None and not nm
        if is_secsym:
            sn = elf.secname(shndx)
            tgtstr = "%s+0x%x" % (sn, add)
            resolved = resolve(sn, add)
            if resolved:
                tgtstr = resolved
        else:
            tgtstr = "%s+0x%x" % (nm, add if add else val)
        if sym_filter and sym_filter not in tgtstr:
            continue
        out.append("  0x%06x -> %s" % (roff, tgtstr))
    return out


if __name__ == "__main__":
    args = sys.argv[1:]
    path = args[0]
    sec = None
    sym = None
    i = 1
    while i < len(args):
        if args[i] == "--sec":
            sec = args[i + 1]; i += 2
        elif args[i] == "--sym":
            sym = args[i + 1]; i += 2
        else:
            i += 1
    elf = Elf(path)
    for l in fmt(elf, sec, sym):
        print(l)
