import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()

def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")

def sub(p, pairs, label=""):
    d = rd(p)
    orig = d
    for old, new in pairs:
        n = d.count(old)
        if n:
            d = d.replace(old, new)
            print(f"  [{label}] {n}x OK")
        else:
            print(f"  [{label}] MISS: {old[:64]!r}")
    if d != orig:
        wr(p, d)
    return d != orig

# =============================================================
# A) kernel/berry_compat_link.c - link-time stubs
# =============================================================
print("== A) kernel/berry_compat_link.c ==")
p = "kernel/berry_compat_link.c"
d = rd(p)
add = b"""
/* ====== amba_bustype: drivers/amba/ is an empty stub in VXTux 6.18 ====== */
const struct bus_type amba_bustype = {
	.name		= "amba",
	.match		= amba_match,
};
EXPORT_SYMBOL_GPL(amba_bustype);
"""
if b"amba_bustype" not in d:
    d = d + add
    wr(p, d)
else:
    print("  already present")

# =============================================================
# B) drivers/usb/phy/phy.c must be built -> enable CONFIG_USB_PHY
# =============================================================
print("== B) CONFIG_USB_PHY ==")
p = ".config"
d = rd(p)
if b"\nCONFIG_USB_PHY=y" not in d:
    d2 = d.replace(b"\nCONFIG_USB_SUPPORT=y",
                   b"\nCONFIG_USB_SUPPORT=y\nCONFIG_USB_PHY=y")
    if d2 != d:
        wr(p, d2)
        print("  enabled CONFIG_USB_PHY")
    else:
        print("  anchor not found")
else:
    print("  already enabled")

# =============================================================
# C) __nf_ct_ext_find - missing from nf_conntrack_extend.c
# =============================================================
print("== C) __nf_ct_ext_find ==")
p = "net/netfilter/nf_conntrack_extend.c"
d = rd(p)
if b"__nf_ct_ext_find" in d and b"void *__nf_ct_ext_find" not in d:
    anchor = b"static void nf_ct_ext_destroy"
    if anchor in d:
        add = (b"void *__nf_ct_ext_find(const struct nf_ct_ext *ext, u8 id)\n"
               b"{\n"
               b"\tstruct nf_ct_ext *new = (void *)ext;\n"
               b"\n"
               b"\tif (!new || id >= NF_CT_EXT_MAX)\n"
               b"\t\treturn NULL;\n"
               b"\n"
               b"\treturn new->offsets[id] ? (void *)new + new->offsets[id] : NULL;\n"
               b"}\n"
               b"EXPORT_SYMBOL_GPL(__nf_ct_ext_find);\n\n")
        i = d.find(anchor)
        d = d[:i] + add + d[i:]
        wr(p, d)
        print("  inserted __nf_ct_ext_find")
    else:
        print("  anchor not found")
else:
    print("  present or not needed")

# =============================================================
# D) nf_conncount_count - net/netfilter/nf_conntrack_count.c missing
# =============================================================
print("== D) nf_conncount_count ==")
p = "net/netfilter/nf_conntrack_count.c"
if not os.path.exists(os.path.join(ROOT, p)):
    src = (b"// SPDX-License-Identifier: GPL-2.0\n"
           b"/* V18: Berry compat - nf_conntrack_count stub.\n"
           b" *\n"
           b" * The upstream nf_conntrack_count.c is not present in the VXTux tree but\n"
           b" * CONFIG_NF_CONNTRACK_COUNT is on, so nf_conncount_count() is\n"
           b" * referenced without a definition. Provide a minimal implementation.\n"
           b" */\n"
           b"#include <linux/types.h>\n"
           b"#include <linux/slab.h>\n"
           b"#include <net/netfilter/nf_conntrack_count.h>\n"
           b"\n"
           b"unsigned int nf_conncount_count(struct net *net,\n"
           b"\t\t\t\t  const struct nf_conntrack_zone *zone,\n"
           b"\t\t\t\t  bool *early_drop)\n"
           b"{\n"
           b"\t(void)net;\n"
           b"\t(void)zone;\n"
           b"\tif (early_drop)\n"
           b"\t\t*early_drop = false;\n"
           b"\treturn 0;\n"
           b"}\n"
           b"EXPORT_SYMBOL_GPL(nf_conncount_count);\n")
    wr(p, src)
    # register in Makefile
    mp = "net/netfilter/Makefile"
    md = rd(mp)
    if b"nf_conntrack_count.o" not in md:
        md2 = md.replace(b"obj-$(CONFIG_NF_CONNTRACK)\t\t+= nf_conntrack_core.o",
                         b"obj-$(CONFIG_NF_CONNTRACK)\t\t+= nf_conntrack_core.o \\\n"
                         b"\t\t\t\t   nf_conntrack_count.o")
        if md2 == md:
            print("  Makefile anchor missing - manual fix needed")
        else:
            wr(mp, md2)
            print("  added to net/netfilter/Makefile")
else:
    print("  file exists")

print("DONE")
