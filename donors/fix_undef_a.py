import os
import re

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
            print(f"  [{label}] MISS: {old[:70]!r}")
    if d != orig:
        wr(p, d)
    return d != orig

# =============================================================
# 1) net/ipv6/tcp_ipv6.c -> use merged secure_tcpv6_seq_and_ts_off
# =============================================================
print("== 1) net/ipv6/tcp_ipv6.c ==")
sub("net/ipv6/tcp_ipv6.c", [
    # tcp_v6_init_ts_off
    (b"return secure_tcpv6_ts_off(net,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->daddr.s6_addr32);",
     b"return secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->daddr.s6_addr32,\n"
     b"\t\t\t\t  0, 0).off.ts_off;"),
    # tcp_v6_init_seq
    (b"return secure_tcpv6_seq(ipv6_hdr(skb)->daddr.s6_addr32,\n"
     b"\t\t\t\tipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\ttcp_hdr(skb)->dest,\n"
     b"\t\t\t\ttcp_hdr(skb)->source);",
     b"return secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\tipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\tipv6_hdr(skb)->daddr.s6_addr32,\n"
     b"\t\t\t\ttcp_hdr(skb)->dest,\n"
     b"\t\t\t\ttcp_hdr(skb)->source).off.seq;"),
    # tcp_v6_connect
    (b"secure_tcpv6_seq(np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t     sk->sk_v6_daddr.s6_addr32,\n"
     b"\t\t\t\t\t     inet->inet_sport,\n"
     b"\t\t\t\t\t     inet->inet_dport));",
     b"secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\t\t     np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t     sk->sk_v6_daddr.s6_addr32,\n"
     b"\t\t\t\t\t     inet->inet_sport,\n"
     b"\t\t\t\t\t     inet->inet_dport).off.seq);"),
    (b"tp->tsoffset = secure_tcpv6_ts_off(net,\n"
     b"\t\t\t\t\t  np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t  sk->sk_v6_daddr.s6_addr32);",
     b"tp->tsoffset = secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\t\t  np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t  sk->sk_v6_daddr.s6_addr32,\n"
     b"\t\t\t\t\t  0, 0).off.ts_off;"),
], "tcp_ipv6.c")

# =============================================================
# 2) include/linux/netdevice.h -> LL_RESERVED_SPACE_EX alias
# =============================================================
print("== 2) include/linux/netdevice.h : LL_RESERVED_SPACE_EX ==")
p = "include/linux/netdevice.h"
d = rd(p)
if b"LL_RESERVED_SPACE_EX" not in d:
    i = d.find(b"#define LL_RESERVED_SPACE_EXTRA(dev,extra)")
    if i >= 0:
        j = i
        while d[j:j+1] == b"\\":
            j = d.find(b"\n", j) + 1
        end = d.find(b"\n", j) + 1
        add = (b"#define LL_RESERVED_SPACE_EX(dev,extra) \\\n"
               b"\tLL_RESERVED_SPACE_EXTRA(dev,extra)\n")
        d = d[:end] + add + d[end:]
        wr(p, d)
        print("  inserted alias")
    else:
        print("  anchor missing")
else:
    print("  already present")

# =============================================================
# 3) bluetooth past_*_capable -> inline no-op
# =============================================================
print("== 3) bluetooth past_*_capable ==")
for cand in ("net/bluetooth/hci_core.h", "include/net/bluetooth/hci.h"):
    fp = os.path.join(ROOT, cand)
    if not os.path.exists(fp):
        continue
    d = rd(cand)
    if b"past_receiver_capable" in d:
        print(f"  already in {cand}")
        break
    add = (b"\n/* ====== HCI PAST (Power Aware Scanning) not ported to VXTux 6.18 ====== */\n"
           b"static inline bool past_receiver_capable(struct hci_dev *hdev)\n"
           b"{\n\t(void)hdev;\n\treturn false;\n}\n"
           b"static inline bool past_sender_capable(struct hci_dev *hdev)\n"
           b"{\n\t(void)hdev;\n\treturn false;\n}\n")
    wr(cand, d + add)
    break
else:
    print("  no candidate header found")

print("DONE")
