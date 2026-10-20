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
            print(f"  [{label}] {n}x {old[:52]!r}")
        else:
            print(f"  [{label}] MISS {old[:52]!r}")
    if d != orig:
        wr(p, d)
    return d != orig

print("== 1) net/ipv6/tcp_ipv6.c: use merged secure_tcpv6_seq_and_ts_off ==")
# tcp_v6_init_ts_off(net, skb)  ->  secure_tcpv6_seq_and_ts_off(net, saddr, daddr, 0, 0).off.ts_off
sub("net/ipv6/tcp_ipv6.c", [
    (b"return secure_tcpv6_ts_off(net,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->daddr.s6_addr32);",
     b"return secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\t  ipv6_hdr(skb)->daddr.s6_addr32,\n"
     b"\t\t\t\t  0, 0).off.ts_off;"),
    (b"return secure_tcpv6_seq(ipv6_hdr(skb)->daddr.s6_addr32,\n"
     b"\t\t\t\tipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\ttcp_hdr(skb)->dest,\n"
     b"\t\t\t\ttcp_hdr(skb)->source);",
     b"return secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\tipv6_hdr(skb)->saddr.s6_addr32,\n"
     b"\t\t\t\tipv6_hdr(skb)->daddr.s6_addr32,\n"
     b"\t\t\t\ttcp_hdr(skb)->dest,\n"
     b"\t\t\t\ttcp_hdr(skb)->source).off.seq;"),
], "tcp_ipv6.c")

print("== 2) check remaining secure_tcpv6_seq/ts_off callers ==")
d = rd("net/ipv6/tcp_ipv6.c")
import re
for m in re.finditer(rb"secure_tcpv6_(seq|ts_off)\s*\(", d):
    s = max(0, m.start() - 200)
    print("  ...", d[s:m.end()+200].decode("utf-8", "replace").replace("\n", " | "))

print("DONE")
