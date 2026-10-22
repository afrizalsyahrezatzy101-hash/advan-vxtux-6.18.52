import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")

for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "built-in.a" in t or "core-" in t and "+=" in t:
        print(f"{i}: {t}")
