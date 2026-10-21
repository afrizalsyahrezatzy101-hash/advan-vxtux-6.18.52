"""Dump every NUL-terminated string in a blob's .rodata/.data with its offset.

    python3 donors/dump_strings.py <blob.ko> [section]
"""
import re
import subprocess
import sys

OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")
NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")


def main():
    path = sys.argv[1]
    secs = sys.argv[2:] or [".rodata", ".data"]
    data = open(path, "rb").read()
    # section table: name -> (vma, file off, size)
    out = subprocess.run([OBJDUMP, "-h", path], capture_output=True,
                         text=True).stdout
    table = {}
    for line in out.splitlines():
        m = re.match(r"\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+"
                     r"([0-9a-f]+)\s+([0-9a-f]+)\s+2\*\*(\d)", line)
        # objdump -h columns: idx name size VMA LMA fileoff align
        if m and m.group(1) in secs:
            table[m.group(1)] = (int(m.group(3), 16), int(m.group(5), 16),
                                 int(m.group(2), 16))
    syms = {}
    nout = subprocess.run([NM, path], capture_output=True, text=True).stdout
    for line in nout.splitlines():
        p = line.split()
        if len(p) == 3:
            syms.setdefault(int(p[0], 16), p[2])

    for name, (vma, off, size) in table.items():
        if not size:
            continue
        blob = data[off:off + size]
        i = 0
        while i < len(blob):
            j = blob.find(b"\0", i)
            if j < 0:
                break
            s = blob[i:j]
            if len(s) >= 3 and all(32 <= c < 127 for c in s):
                print(f"{name}+0x{i:04x}  addr=0x{vma + i:06x}  "
                      f"{s.decode()!r}")
            i = j + 1


if __name__ == "__main__":
    main()
