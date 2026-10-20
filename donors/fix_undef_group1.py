import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()

def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"WROTE {p} ({len(d)} bytes)")

def patch(p, old, new, label=""):
    d = rd(p)
    if old in d:
        d2 = d.replace(old, new)
        wr(p, d2)
        print(f"  [{label or p}] replaced {d.count(old)} occurrence(s)")
        return True
    print(f"  [{label or p}] NOT FOUND: {old[:60]!r}")
    return False

# ---------------------------------------------------------------
# 1) LL_RESERVED_SPACE_EX -> macro alias for LL_RESERVED_SPACE_EXTRA
#    include/linux/netdevice.h
# ---------------------------------------------------------------
print("== 1) LL_RESERVED_SPACE_EX ==")
p = "include/linux/netdevice.h"
d = rd(p)
i = d.find(b"#define LL_RESERVED_SPACE_EXTRA(dev,extra)")
if i >= 0:
    # find end of that macro (continuation lines end with backslash)
    j = i
    while d[j:j+1] == b"\\":
        j = d.find(b"\n", j) + 1
    end = d.find(b"\n", j) + 1
    add = (b"#define LL_RESERVED_SPACE_EX(dev,extra) \\\n"
           b"\tLL_RESERVED_SPACE_EXTRA(dev,extra)\n")
    if b"LL_RESERVED_SPACE_EX" not in d:
        d = d[:end] + add + d[end:]
        wr(p, d)
        print("  inserted alias after LL_RESERVED_SPACE_EXTRA")
    else:
        print("  already present")
else:
    print("  LL_RESERVED_SPACE_EXTRA not found")

# ---------------------------------------------------------------
# 2) past_receiver_capable / past_sender_capable
#    net/bluetooth/mgmt.c and hci_sync.c reference them but the
#    tree has no HCI "PAST" (Power Aware Scanning) support.
#    Provide inline no-op helpers.
# ---------------------------------------------------------------
print("== 2) past_*_capable ==")
p = "net/bluetooth/hci_core.h"
if not os.path.exists(os.path.join(ROOT, p)):
    p = "include/net/bluetooth/hci.h"
d = rd(p)
if b"past_receiver_capable" not in d:
    add = (b"\n/* ====== HCI PAST (Power Aware Scanning) not ported to VXTux 6.18 ====== */\n"
           b"static inline bool past_receiver_capable(struct hci_dev *hdev)\n"
           b"{\n\t(void)hdev;\n\treturn false;\n}\n"
           b"static inline bool past_sender_capable(struct hci_dev *hdev)\n"
           b"{\n\t(void)hdev;\n\treturn false;\n}\n")
    d = d + add
    wr(p, d)
    print(f"  appended to {p}")
else:
    print("  already present")

# ---------------------------------------------------------------
# 3) secure_tcpv6_seq / secure_tcpv6_ts_off
#    net/core/secure_seq.c only defines the merged helper.
#    Add the two split accessors there.
# ---------------------------------------------------------------
print("== 3) secure_tcpv6_seq / secure_tcpv6_ts_off ==")
p = "net/core/secure_seq.c"
d = rd(p)
if b"EXPORT_SYMBOL(secure_tcpv6_seq_and_ts_off);" in d and b"\nu32 secure_tcpv6_seq(" not in d:
    anchor = b"EXPORT_SYMBOL(secure_tcpv6_seq_and_ts_off);"
    i = d.find(anchor) + len(anchor)
    add = (b"\n"
           b"u32 secure_tcpv6_seq(const __be32 *saddr, const __be32 *daddr,\n"
           b"\t\t     __be16 sport, __be16 dport)\n"
           b"{\n"
           b"\tunion tcp_seq_and_ts_off st;\n"
           b"\n"
           b"\tst = secure_tcpv6_seq_and_ts_off(current->net ? current->net : init_net,\n"
           b"\t\t\t\t\t saddr, daddr, sport, dport);\n"
           b"\treturn st.off.seq;\n"
           b"}\n"
           b"EXPORT_SYMBOL(secure_tcpv6_seq);\n"
           b"\n"
           b"u32 secure_tcpv6_ts_off(const struct net *net, const __be32 *saddr,\n"
           b"\t\t\tconst __be32 *daddr)\n"
           b"{\n"
           b"\tunion tcp_seq_and_ts_off st;\n"
           b"\n"
           b"\tst = secure_tcpv6_seq_and_ts_off(net, saddr, daddr, 0, 0);\n"
           b"\treturn st.off.ts_off;\n"
           b"}\n"
           b"EXPORT_SYMBOL(secure_tcpv6_ts_off);\n")
    d = d[:i] + add + d[i:]
    wr(p, d)
else:
    print("  already present or anchor missing")

print("DONE")
