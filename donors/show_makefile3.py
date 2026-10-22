import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")

for i in range(795, 850):
    if i < len(lines):
        t = lines[i].decode("utf-8", "replace")
        print(f"{i+1}: {t}")
