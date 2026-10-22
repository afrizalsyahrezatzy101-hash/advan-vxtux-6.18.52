import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"


def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()


def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")


# =====================================================================
# FIX 1: LL_RESERVED_SPACE_EX -> LL_RESERVED_SPACE_EXTRA
#   The call sites use LL_RESERVED_SPACE_EX(dev, hard_header_len)
#   (two args). Upstream 6.18 has LL_RESERVED_SPACE_EXTRA(dev, extra).
#   The VXTux tree's netdevice.h only defines LL_RESERVED_SPACE and
#   LL_RESERVED_SPACE_EXTRA is missing -> add it.
# =====================================================================
print("== FIX 1: netdevice.h LL_RESERVED_SPACE_EXTRA ==")
p = "include/linux/netdevice.h"
d = rd(p)
print("   current defines:")
for i, l in enumerate(d.split(b"\n"), 1):
    if b"LL_RESERVED_SPACE" in l:
        print(f"     {i}: {l.decode('utf-8','replace')}")

# find the LL_RESERVED_SPACE_EXTRA definition in upstream terms
old = b"#define LL_RESERVED_SPACE(dev) \\\n"
if b"LL_RESERVED_SPACE_EXTRA" not in d:
    # upstream: LL_RESERVED_SPACE_EXTRA(dev, extra) = netdev->hard_header_len + extra
    # insert right after LL_RESERVED_SPACE_EXTRA-ish block
    anchor = b"#define LL_RESERVED_SPACE(dev) \\\n"
    i = d.find(anchor)
    if i >= 0:
        # find the end of that define (the line ending with a backslash then a
        # continuation line)
        j = d.find(b"\n", d.find(b"\\\n", i) + 2)
        # safer: locate the macro block and append after it
        k = d.find(b"#define LL_RESERVED_SPACE_EXTRA", i)
        if k < 0:
            # insert after the whole LL_RESERVED_SPACE define block
            # find the closing of the macro: a line that does not end with backslash
            pos = i
            while pos < len(d):
                nl = d.find(b"\n", pos)
                line = d[pos:nl]
                pos = nl + 1
                if not line.rstrip().endswith(b"\\"):
                    break
            add = (b"\n/* VXTux 6.18: upstream renamed this to\n"
                   b" * LL_RESERVED_SPACE_EXTRA(dev, extra). Provide the\n"
                   b" * two-argument form the tree's callers expect. */\n"
                   b"#define LL_RESERVED_SPACE_EXTRA(dev, extra) \\\n"
                   b"\t(netdev->hard_header_len + (extra))\n")
            d = d[:pos] + add + d[pos:]
            wr(p, d)
        else:
            print("   LL_RESERVED_SPACE_EXTRA already defined")
    else:
        print("   anchor not found")
else:
    print("   already present")
