import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

for rel in ("include/linux/io_uring/net.h", "include/linux/io_uring.h",
            "include/linux/io_uring/cmd.h"):
    fp = os.path.join(ROOT, rel)
    print("=" * 70)
    print(rel)
    with open(fp, "rb") as f:
        d = f.read()
    lines = d.split(b"\n")
    limit = 30 if "cmd.h" not in rel else 130
    for i, l in enumerate(lines[:limit], 1):
        print(f"  {i}: {l.decode('utf-8','replace')}")

print("=" * 70)
print("kernel/Makefile obj-y / obj- entries")
fp = os.path.join(ROOT, "kernel/Makefile")
with open(fp, "rb") as f:
    d = f.read()
for i, l in enumerate(d.split(b"\n"), 1):
    t = l.decode("utf-8", "replace")
    if "obj-y" in t or "obj-" in t:
        print(f"  {i}: {t}")
