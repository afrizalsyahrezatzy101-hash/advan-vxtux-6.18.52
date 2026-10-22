import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()

def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")

# =============================================================
# A) kernel/berry_compat_link.c - link-time stubs
# =============================================================
print("== A) kernel/berry_compat_link.c ==")
p = "kernel/berry_compat_link.c"
d = rd(p)
if b"amba_bustype" not in d:
    d += b"""
/* ====== amba_bustype: drivers/amba/ is an empty stub in VXTux 6.18 ====== */
const struct bus_type amba_bustype = {
	.name		= "amba",
};
EXPORT_SYMBOL_GPL(amba_bustype);
"""
    wr(p, d)
else:
    print("  already present")

# =============================================================
# B) CONFIG_USB_PHY
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
# C) __nf_ct_ext_find in net/netfilter/nf_conntrack_extend.c
# =============================================================
print("== C) __nf_ct_ext_find ==")
p = "net/netfilter/nf_conntrack_extend.c"
d = rd(p)
if b"void *__nf_ct_ext_find" not in d:
    anchor = b"static void nf_ct_ext_destroy"
    if anchor in d:
        add = (b"void *__nf_ct_ext_find(const struct nf_ct_ext *ext, u8 id)\n"
               b"{\n"
               b"\tif (!ext || id >= NF_CT_EXT_NUM || !ext->offset[id])\n"
               b"\t\treturn NULL;\n"
               b"\n"
               b"\treturn (void *)ext + ext->offset[id];\n"
               b"}\n"
               b"EXPORT_SYMBOL_GPL(__nf_ct_ext_find);\n\n")
        i = d.find(anchor)
        d = d[:i] + add + d[i:]
        wr(p, d)
        print("  inserted __nf_ct_ext_find")
    else:
        print("  anchor not found")
else:
    print("  present")

# =============================================================
# D) nf_conncount_count stub file
# =============================================================
print("== D) nf_conncount_count ==")
p = "net/netfilter/nf_conntrack_count.c"
if not os.path.exists(os.path.join(ROOT, p)):
    src = (b"// SPDX-License-Identifier: GPL-2.0\n"
           b"/* V18: Berry compat - nf_conntrack_count stub.\n"
           b" *\n"
           b" * The upstream nf_conntrack_count.c is not present in the VXTux tree but\n"
           b" * CONFIG_NF_CONNTRACK_COUNT is enabled, so nf_conncount_count() is\n"
           b" * referenced without a definition. Provide a minimal implementation.\n"
           b" */\n"
           b"#include <linux/types.h>\n"
           b"#include <linux/slab.h>\n"
           b"#include <net/netfilter/nf_conntrack_count.h>\n"
           b"\n"
           b"unsigned int nf_conncount_count(struct net *net,\n"
           b"\t\t\t\tstruct nf_conncount_data *data,\n"
           b"\t\t\t\tconst u32 *key,\n"
           b"\t\t\t\tconst struct nf_conntrack_tuple *tuple,\n"
           b"\t\t\t\tconst struct nf_conntrack_zone *zone)\n"
           b"{\n"
           b"\t(void)net;\n"
           b"\t(void)data;\n"
           b"\t(void)key;\n"
           b"\t(void)tuple;\n"
           b"\t(void)zone;\n"
           b"\treturn 0;\n"
           b"}\n"
           b"EXPORT_SYMBOL_GPL(nf_conncount_count);\n")
    wr(p, src)
    mp = "net/netfilter/Makefile"
    md = rd(mp)
    if b"nf_conntrack_count.o" not in md:
        anchor = b"obj-$(CONFIG_NF_CONNTRACK)\t\t+= nf_conntrack_core.o"
        if anchor in md:
            md2 = md.replace(anchor,
                             anchor + b" \\\n\t\t\t\t   nf_conntrack_count.o")
            wr(mp, md2)
            print("  added to net/netfilter/Makefile")
        else:
            print("  Makefile anchor missing")
else:
    print("  file exists")

print("DONE")
