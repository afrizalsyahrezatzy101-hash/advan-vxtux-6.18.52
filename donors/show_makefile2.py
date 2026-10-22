import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")

# find where sub-directories are recursed into
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "obj-y" in t or "core-y" in t or "scripts/Makefile" in t or "MACHINE" in t:
        print(f"{i}: {t}")
print("--- lines 2100-2198 (tail) ---")
for i in range(2099, min(2198, len(lines))):
    t = lines[i].decode("utf-8", "replace")
    if t.strip():
        print(f"{i+1}: {t}")
