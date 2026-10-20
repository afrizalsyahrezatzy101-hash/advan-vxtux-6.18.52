import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"


def show(rel, a, b, label=""):
    fp = os.path.join(ROOT, rel)
    print("=" * 70)
    print(f"{rel}  lines {a}..{b}  {label}")
    with open(fp, "rb") as f:
        lines = f.read().split(b"\n")
    for i in range(a - 1, min(b, len(lines))):
        print(f"{i+1:5}: {lines[i].decode('utf-8','replace')}")


show("include/net/ip_tunnels.h", 340, 400)
show("include/net/tcp.h", 2090, 2120)
show("net/ipv4/tcp_output.c", 2800, 2860)

# kzalloc_flex / kmalloc_flex in slab.h
print("=" * 70)
print("include/linux/slab.h : kzalloc_flex / kmalloc_flex")
with open(os.path.join(ROOT, "include/linux/slab.h"), "rb") as f:
    lines = f.read().split(b"\n")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "flex" in t:
        print(f"{i:5}: {t}")

# amba bus
print("=" * 70)
print("drivers/amba contents")
try:
    for fn in sorted(os.listdir(os.path.join(ROOT, "drivers/amba"))):
        print("  ", fn)
except Exception as e:
    print("  ", e)
