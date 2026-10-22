#!/usr/bin/env python3
"""Dump the stock mmdvfs.ko's data structures, field by field.

A relocatable module has every section at address 0, so a symbol's
nm value is its offset inside its own section. The relocation table
says which symbol (plus addend) fills each 8-byte slot of .data.

    ko_dvfs.py <ko> struct <sym> [<sym> ...]
    ko_dvfs.py <ko> data
    ko_dvfs.py <ko> rodata [prefix]
"""
import struct
import subprocess
import sys

BIN = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
       r"\15.2.rel1\bin")


def tool(name):
    return f"{BIN}\\{name}.exe"


def sections(path):
    out = subprocess.run([tool("aarch64-none-linux-gnu-readelf"), "-SW", path],
                         capture_output=True, text=True).stdout
    secs = {}
    for line in out.splitlines():
        line = line.strip()
        if not line.startswith("["):
            continue
        head = line.split("[")[1].split("]")[0].strip()
        if not head.isdigit():
            continue
        rest = line.split("]", 1)
        if len(rest) != 2:
            continue
        p = rest[1].split()
        # p = [Name, Type, Address, Off, Size, ES, Flg, Lk, Inf, Al]
        if len(p) < 5:
            continue
        try:
            off = int(p[3], 16)
            size = int(p[4], 16)
        except ValueError:
            continue
        secs[p[0]] = (off, size)
    return secs


def symbols(path):
    out = subprocess.run([tool("aarch64-none-linux-gnu-nm"), "-S", "--defined-only",
                          path], capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) < 4:
            continue
        try:
            res[p[3]] = (int(p[0], 16), int(p[1], 16), p[2])
        except ValueError:
            pass
    return res


def symsec(path):
    out = subprocess.run([tool("aarch64-none-linux-gnu-readelf"), "-sW", path],
                         capture_output=True, text=True).stdout
    res = {}
    secnames = {}
    out2 = subprocess.run([tool("aarch64-none-linux-gnu-readelf"), "-SW", path],
                          capture_output=True, text=True).stdout
    for line in out2.splitlines():
        line = line.strip()
        if not line.startswith("["):
            continue
        head = line.split("[")[1].split("]")[0].strip()
        if not head.isdigit():
            continue
        rest = line.split("]", 1)
        if len(rest) != 2:
            continue
        p = rest[1].split()
        if p:
            secnames[int(head)] = p[0]
    for line in out.splitlines():
        p = line.split()
        if len(p) < 8 or not p[0].endswith(":"):
            continue
        ndx = p[6]
        if ndx == "UND":
            continue
        try:
            idx = int(ndx)
        except ValueError:
            continue
        res[p[7]] = secnames.get(idx, "?")
    return res


def relocs(path):
    """(section) -> {offset: (type, symname, addend)}"""
    out = subprocess.run([tool("aarch64-none-linux-gnu-readelf"), "-rW", path],
                         capture_output=True, text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        s = line.strip()
        if s.startswith("Relocation section"):
            cur = s.split("'")[1] if "'" in s else None
            continue
        if not s or not s[0].isdigit():
            continue
        p = s.split()
        if len(p) < 5:
            continue
        off = int(p[0], 16)
        typ = p[2]
        rest = p[4:]
        if not rest:
            continue
        name = rest[0]
        addend = 0
        for f in rest[1:]:
            if f.startswith("+"):
                try:
                    addend = int(f[1:], 16)
                except ValueError:
                    pass
            else:
                try:
                    addend = int(f, 16)
                except ValueError:
                    pass
        res.setdefault(cur, {})[off] = (typ, name, addend)
    return res


def read_section(path, secs, secname, soff, length):
    """soff is SECTION-relative"""
    if secname not in secs:
        return None
    base, size = secs[secname]
    if soff + length > size or length < 0:
        return None
    with open(path, "rb") as f:
        f.seek(base + soff)
        return f.read(length)


def cstr_at(path, secs, soff):
    blob = read_section(path, secs, ".rodata", soff, 4096)
    if blob is None:
        return None
    end = blob.find(b"\0")
    if end < 0:
        end = len(blob)
    return blob[:end].decode("utf-8", "replace")


class Ko:
    def __init__(self, path):
        self.path = path
        self.secs = sections(path)
        self.syms = symbols(path)
        self.symsec = symsec(path)
        self.rel = relocs(path)
        self.text = sorted((v, sz, n) for n, (v, sz, t) in self.syms.items()
                           if self.symsec.get(n) == ".text" and t in "Tt")
        self.data = sorted((v, sz, n) for n, (v, sz, t) in self.syms.items()
                           if self.symsec.get(n) == ".data" and t in "Dd")
        self.bysec = {}
        for n, (v, sz, t) in self.syms.items():
            self.bysec.setdefault(self.symsec.get(n, "?"), []).append((v, sz, n))
        for k in self.bysec:
            self.bysec[k].sort()

    def resolve(self, table, so):
        """so is SECTION-relative; nm values are section-relative too"""
        best = None
        for v, sz, n in table:
            if v <= so < v + max(sz, 1):
                if best is None or v > table[best][0]:
                    best = (v, sz, n)
        return best[2] if best else None

    def sym_at(self, secname, section_off):
        """section-relative offset -> the symbol that starts there"""
        return self.resolve(self.bysec.get(secname, []), section_off)

    def cstr(self, secname, section_off):
        return cstr_at(self.path, self.secs, section_off)

    def reloc(self, secname, off):
        for rsec, entries in self.rel.items():
            if rsec.startswith(".rela" + secname):
                return entries.get(off)
        return None

    def describe(self, secname, off):
        """-> (kind, value) for the 8-byte slot at off"""
        r = self.reloc(secname, off)
        if r:
            typ, sym, add = r
            if sym == ".rodata":
                return ("str", cstr_at(self.path, self.secs, add))
            if sym == ".text":
                n = self.sym_at(".text", add)
                return ("func", n or f".text+0x{add:x}")
            if sym == ".data":
                n = self.sym_at(".data", add)
                return ("data", n or f".data+0x{add:x}")
            if sym == ".bss":
                return ("bss", f"+0x{add:x}")
            return (sym, f"+0x{add:x}")
        blob = read_section(self.path, self.secs, secname, off, 8)
        if blob is None:
            return ("?", None)
        return ("raw", struct.unpack("<Q", blob)[0])


def cmd_struct(ko, names):
    for name in names:
        if name not in ko.syms:
            print(f"--- {name}: NOT FOUND")
            continue
        val, size, typ = ko.syms[name]
        sec = ko.symsec.get(name, "?")
        print(f"--- {name}: {sec}+0x{val:x} size=0x{size:x} type={typ}")
        for i in range(0, size, 8):
            off = val + i
            kind, v = ko.describe(sec, off)
            if kind == "raw" and v == 0:
                continue
            print(f"    +0x{i:04x}  {kind:6s} {v!r}")
        print()


def cmd_ofmatch(ko, names):
    """dump an of_device_match[] table: 16 bytes per entry"""
    for name in names:
        if name not in ko.syms:
            print(f"--- {name}: NOT FOUND")
            continue
        val, size, typ = ko.syms[name]
        sec = ko.symsec.get(name, "?")
        print(f"--- {name}: {sec}+0x{val:x} size=0x{size:x}")
        for i in range(0, size, 16):
            kind, v = ko.describe(sec, val + i)
            kind2, v2 = ko.describe(sec, val + i + 8)
            print(f"    [{i // 16}] {kind}:{v!r}  {kind2}:{v2!r}")
        print()


def cmd_table(ko, names):
    """dump an ip_dvfs_map_cfg[] table, field by field"""
    for name in names:
        if name not in ko.syms:
            print(f"--- {name}: NOT FOUND")
            continue
        val, size, typ = ko.syms[name]
        print(f"--- {name}: .data+0x{val:x} size=0x{size:x} ({size // 0x38} entries)")
        for i in range(0, size, 0x38):
            off = val + i
            fields = []
            for fo in range(0, 0x38, 8):
                kind, v = ko.describe(".data", off + fo)
                fields.append(f"+0x{fo:02x} {kind}:{v!r}")
            print(f"    [{i // 0x38}] " + "  ".join(fields))
        print()


def cmd_data(ko):
    size = ko.secs[".data"][1]
    for off in range(0, size, 8):
        kind, v = ko.describe(".data", off)
        if kind == "raw" and (v == 0 or v is None):
            continue
        print(f"  .data+0x{off:04x}  {kind:6s} {v!r}")


def cmd_rodata(ko, prefix=None):
    size = ko.secs[".rodata"][1]
    blob = read_section(ko.path, ko.secs, ".rodata", 0, size)
    i = 0
    while i < size:
        end = blob.find(b"\0", i)
        if end < 0:
            end = size
        s = blob[i:end]
        if len(s) >= 1 and all(32 <= c < 127 for c in s):
            t = s.decode("utf-8", "replace")
            if prefix is None or prefix in t:
                print(f"  .rodata+0x{i:04x}  {t!r}")
        i = end + 1


def selftest(ko):
    """assert the address model holds: a .data slot's relocation must
    resolve to a real symbol, and a .rodata slot to a real string."""
    assert ko.secs[".text"] == (0x8000, 0xf868), ko.secs[".text"]
    assert ko.secs[".rodata"] == (0x4588, 0x2f28), ko.secs[".rodata"]
    # a known data symbol lives in .data
    assert ko.symsec.get("mtx_dvfs_ops") == ".data", ko.symsec.get("mtx_dvfs_ops")
    # the .text offset of a known function
    assert ko.sym_at(".text", 0x4aa0) == "dvfs_target", ko.sym_at(".text", 0x4aa0)
    # a .rodata offset resolves to a string
    assert ko.cstr(".rodata", 0x1521) == "mtx-dvfs", ko.cstr(".rodata", 0x1521)
    print("selftest OK")


def main():
    ko = Ko(sys.argv[1])
    cmd = sys.argv[2]
    if cmd == "selftest":
        selftest(ko)
    elif cmd == "struct":
        cmd_struct(ko, sys.argv[3:])
    elif cmd == "table":
        cmd_table(ko, sys.argv[3:])
    elif cmd == "ofmatch":
        cmd_ofmatch(ko, sys.argv[3:])
    elif cmd == "data":
        cmd_data(ko)
    elif cmd == "rodata":
        cmd_rodata(ko, sys.argv[3] if len(sys.argv) > 3 else None)


if __name__ == "__main__":
    main()
