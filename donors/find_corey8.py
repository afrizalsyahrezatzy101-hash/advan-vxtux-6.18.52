import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")

# Every line that looks like a directory list entry ending in /
print("=== lines ending with '/' ===")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    s = t.strip()
    if s.endswith("/") and len(s) < 120 and not s.startswith("#"):
        print(f"{i}: {t}")
