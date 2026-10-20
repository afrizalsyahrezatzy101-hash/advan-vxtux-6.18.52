import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "arch/arm64/Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")
print("total:", len(lines))
for i in range(140, 200):
    if i < len(lines):
        t = lines[i].decode("utf-8", "replace")
        if t.strip():
            print(f"{i+1}: {t}")
