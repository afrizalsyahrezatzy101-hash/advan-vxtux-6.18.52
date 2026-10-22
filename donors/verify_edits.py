import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

print("=== kernel/Makefile obj-y region ===")
with open(os.path.join(ROOT, "kernel/Makefile"), "rb") as f:
    lines = f.read().split(b"\n")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "obj-y" in t or "berry_compat" in t:
        print(f"{i}: {t}")

print()
print("=== kernel/berry_compat_link.c ===")
fp = os.path.join(ROOT, "kernel/berry_compat_link.c")
print("exists:", os.path.exists(fp))
if os.path.exists(fp):
    with open(fp, "rb") as f:
        d = f.read()
    for i, l in enumerate(d.split(b"\n"), 1):
        print(f"{i}: {l.decode('utf-8','replace')}")
