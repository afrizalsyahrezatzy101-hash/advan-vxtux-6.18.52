import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()

# find every occurrence of 'core' in the Makefile
for m in re.finditer(rb"core[-_a-zA-Z]*", d):
    ln = d[:m.start()].count(b"\n") + 1
    s = max(0, m.start() - 60)
    e = min(len(d), m.end() + 80)
    print(f"{ln}: {d[s:e].decode('utf-8','replace')}")
    print("  ---")
