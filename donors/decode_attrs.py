#!/usr/bin/env python3
"""Decode struct device_attribute (show, store, name) triples in stock mmdvfs.ko."""
import re
import struct
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
RE = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe"
KO = sys.argv[1] if len(sys.argv) > 1 else "donors/stock_vendor_modules/mmdvfs.ko"

secs = {}
for line in subprocess.run([RE, "-S", "-W", KO], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)", line)
    if m:
        secs[m.group(1)] = (int(m.group(3), 16), int(m.group(4), 16))

syms = {}
for line in subprocess.run([NM, KO], capture_output=True, text=True).stdout.splitlines():
    p = line.split()
    if len(p) == 3:
        syms[p[2]] = (int(p[0], 16), p[1])

blob = open(KO, "rb").read()


def sec(name):
    off, size = secs[name]
    return blob[off:off + size]


rel = {}          # (sectname, offset) -> (symbol, addend)
cur = None
for line in subprocess.run([RE, "-r", "-W", KO], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r"Relocation section '(\S+)'", line)
    if m:
        cur = m.group(1)
        continue
    m = re.match(r"^([0-9a-f]{16})\s+\S+\s+(\S+)\s+([0-9a-f]{16})\s+(.*)$", line)
    if m and cur and cur.startswith(".rela.data"):
        rest = m.group(4)
        sm = re.match(r"(\S+)\s*\+\s*(0x[0-9a-f]+|[0-9a-f]+)$", rest)
        if sm:
            rel[(cur, int(m.group(1), 16))] = (sm.group(1), int(sm.group(2), 16))
        else:
            rel[(cur, int(m.group(1), 16))] = (rest.strip(), 0)

SEC = ".rela.data.mtx_dvfs_gov"
data = sec(".data")
rodata = sec(".rodata")


def deref(off):
    return rel.get((SEC, off))


def cstr(buf, off):
    end = buf.find(b"\0", off)
    return buf[off:end].decode("utf-8", "replace")


def resolve(off):
    """Return a printable target for the pointer stored at .data+off."""
    r = deref(off)
    if r is None:
        # no relocation: raw value (bss globals)
        (val,) = struct.unpack_from("<Q", data, off)
        return f"0x{val:x}"
    sn, add = r
    if sn == ".rodata":
        return repr(cstr(rodata, add))
    if sn == ".text":
        return f"text+0x{add:x}"
    return f"{sn}+0x{add:x}"


attr_names = sorted((a, n) for n, (a, t) in syms.items()
                    if (n.startswith("dev_attr") or n.startswith("coeff_attr"))
                    and t in "dDbBrR")

print(f"{'symbol':44s} {'+0x00':28s} {'+0x08':28s} {'+0x10':14s} {'+0x18':20s} {'+0x20':20s}")
for a, n in attr_names:
    if a + 0x28 > len(data):
        continue
    cells = []
    for off in (0, 8, 0x10, 0x18, 0x20):
        cells.append(resolve(a + off) if deref(a + off) else
                     hex(struct.unpack_from("<Q", data, a + off)[0]))
    print(f"{n:44s} {cells[0][:27]:28s} {cells[1][:27]:28s} {cells[2][:13]:14s} "
          f"{cells[3][:19]:20s} {cells[4][:19]:20s}")