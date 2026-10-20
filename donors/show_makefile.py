import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()

lines = d.split(b"\n")
print("Makefile lines:", len(lines))
print("=== lines mentioning io_uring ===")
for i, l in enumerate(lines, 1):
    if b"io_uring" in l:
        print(f"{i}: {l.decode('utf-8','replace')}")
print("=== obj-y += lines ===")
for i, l in enumerate(lines, 1):
    if b"obj-y" in l and b"+=" in l:
        print(f"{i}: {l.decode('utf-8','replace')}")
