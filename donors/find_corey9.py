import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")

# find every non-comment line mentioning init/ or kernel/ or fs/ or net/ or drivers/
pat = re.compile(rb"\b(init|kernel|fs|net|drivers|usr|lib|sound|crypto|block|ipc|security|cert|drivers/vxtux)/")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    s = t.strip()
    if s.startswith("#"):
        continue
    if pat.search(l) and ("obj" in s or "core" in s or "+=" in s or ":=" in s):
        print(f"{i}: {t}")
