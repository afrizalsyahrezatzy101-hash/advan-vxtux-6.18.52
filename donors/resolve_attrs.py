#!/usr/bin/env python3
"""Resolve the sysfs DEVICE_ATTR names in stock mmdvfs.ko.

struct device_attribute is { show, store, sysfs, extra, ... } with the
attribute name last, so each 0x28-byte (40) record's name is an ABS64
relocation into .rodata.  Read the reloc table, map each dev_attr_*/coeff_attr_*
symbol's address to its name string.
"""
import re
import struct
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
RE = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe"
OBJDUMP = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe"
KO = sys.argv[1] if len(sys.argv) > 1 else "donors/stock_vendor_modules/mmdvfs.ko"

# section offsets
secs = {}
for line in subprocess.run([RE, "-S", "-W", KO], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)", line)
    if m:
        secs[m.group(1)] = (int(m.group(3), 16), int(m.group(4), 16))

# symbols
syms = {}
for line in subprocess.run([NM, KO], capture_output=True, text=True).stdout.splitlines():
    p = line.split()
    if len(p) == 3 and p[1] in "TtDdBbRrWwVv":
        syms[p[2]] = (int(p[0], 16), p[1])

# read raw file
blob = open(KO, "rb").read()


def sec_data(name):
    off, size = secs[name]
    return blob[off:off + size]


def rodata_str(off):
    """off is a .rodata byte offset (file offset == section offset here)."""
    d = sec_data(".rodata")
    if off >= len(d):
        return None
    end = d.find(b"\0", off)
    return d[off:end].decode("utf-8", "replace")


# relocations: (section, offset, symbol_name, symbol_value, addend, type)
relocs = []
cur = None
for line in subprocess.run([RE, "-r", "-W", KO], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r"Relocation section '(\S+)'", line)
    if m:
        cur = m.group(1)
        continue
    m = re.match(r"^([0-9a-f]{16})\s+\S+\s+(\S+)\s+([0-9a-f]{16})\s+(.*)$", line)
    if m and cur:
        rest = m.group(4)
        sm = re.match(r"(\S+)\s*\+\s*(0x[0-9a-f]+|[0-9a-f]+)$", rest)
        if sm:
            sn, add = sm.group(1), int(sm.group(2), 16)
        else:
            sn, add = rest.strip(), 0
        relocs.append((cur, int(m.group(1), 16), m.group(2), sn, add))

# .data base for the rela section
data_sec = ".rela.data.mtx_dvfs_gov"
data_off, data_size = secs[".data"]

attr_names = {}
for sec, off, typ, sn, add in relocs:
    if not sec.startswith(".rela.data"):
        continue
    if sn != ".rodata" or typ != "R_AARCH64_ABS64":
        continue
    s = rodata_str(add)
    if not s or len(s) > 40 or not re.match(r"^[A-Za-z0-9_]+$", s):
        continue
    attr_names[off] = s

# map offset -> owning symbol
ordered = sorted((v[0], n) for n, v in syms.items())
addrs = [a for a, _ in ordered]


def owner(off):
    lo, hi = 0, len(addrs) - 1
    best = None
    while lo <= hi:
        mid = (lo + hi) // 2
        if addrs[mid] <= off:
            best = mid
            lo = mid + 1
        else:
            hi = mid - 1
    return ordered[best][1] if best is not None else None

print(f"# {len(attr_names)} rodata name relocations in .data\n")
seen = {}
for off in sorted(attr_names):
    o = owner(off)
    if o and (o.startswith("dev_attr") or o.startswith("coeff_attr")):
        base = re.sub(r"\.\d+$", "", o)
        seen.setdefault(base, set()).add(attr_names[off])

for k in sorted(seen):
    print(f"{k:40s} -> {sorted(seen[k])}")