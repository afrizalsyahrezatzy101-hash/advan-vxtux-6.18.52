import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()

def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")

# C) __nf_ct_ext_find - insert before nf_ct_ext_add
print("== C) __nf_ct_ext_find ==")
p = "net/netfilter/nf_conntrack_extend.c"
d = rd(p)
if b"void *__nf_ct_ext_find" not in d:
    anchor = b"void *nf_ct_ext_add(struct nf_conn *ct, enum nf_ct_ext_id id, gfp_t gfp)"
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
        print("  anchor still missing")
else:
    print("  present")

# D) register nf_conntrack_count.o in net/netfilter/Makefile
print("== D) Makefile registration ==")
p = "net/netfilter/Makefile"
d = rd(p)
if b"nf_conntrack_count.o" not in d:
    anchor = b"nf_conntrack_extend.o nf_conntrack_acct.o nf_conntrack_seqadj.o"
    if anchor in d:
        d2 = d.replace(anchor, anchor + b" nf_conntrack_count.o")
        wr(p, d2)
        print("  added nf_conntrack_count.o")
    else:
        print("  anchor missing")
else:
    print("  already registered")

# verify
print("== verify ==")
d = rd("net/netfilter/nf_conntrack_extend.c")
print("  __nf_ct_ext_find present:", b"void *__nf_ct_ext_find" in d)
d = rd("net/netfilter/Makefile")
print("  count.o in Makefile:", b"nf_conntrack_count.o" in d)
print("DONE")
