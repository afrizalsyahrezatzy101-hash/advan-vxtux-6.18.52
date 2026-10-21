#!/usr/bin/env python3
"""List every NUL-terminated string in a .ko's .rodata with its offset.

    python3 ko_strings.py <file.ko> [section]

Offsets are section-relative, which is what the R_AARCH64_ADR_PRE
relocations carry as their addend.
"""
import re
import subprocess
import sys

READELF = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/"
           "15.2.rel1/bin/aarch64-none-linux-gnu-readelf.exe")


def sections(path):
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
    for line in out.splitlines():
        m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+"
                     r"([0-9a-f]+)\s+([0-9a-f]+)", line)
        if not m:
            continue
        name, typ, addr, off, size = m.groups()
        secs[name] = (int(addr, 16), int(off, 16), int(size, 16))
    return secs


def main():
    path = sys.argv[1]
    want = sys.argv[2] if len(sys.argv) > 2 else ".rodata"
    secs = sections(path)
    data = open(path, "rb").read()
    addr, off, size = secs[want]
    blob = data[off:off + size]
    i = 0
    while i < len(blob):
        j = blob.find(b"\0", i)
        if j < 0:
            j = len(blob)
        s = blob[i:j]
        if s:
            try:
                txt = s.decode("utf-8")
            except UnicodeDecodeError:
                txt = repr(s)
            if all(32 <= ord(c) < 127 or c in "\n\t" for c in txt):
                print(f"{want}+{i:#06x}  {txt!r}")
        i = j + 1


if __name__ == "__main__":
    main()
