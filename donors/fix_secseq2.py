import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "net/ipv6/tcp_ipv6.c"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

pairs = [
    # WRITE_ONCE(tp->write_seq, secure_tcpv6_seq(...))
    (b"secure_tcpv6_seq(np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t     sk->sk_v6_daddr.s6_addr32,\n"
     b"\t\t\t\t\t     inet->inet_sport,\n"
     b"\t\t\t\t\t     inet->inet_dport));",
     b"secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\t\t     np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t     sk->sk_v6_daddr.s6_addr32,\n"
     b"\t\t\t\t\t     inet->inet_sport,\n"
     b"\t\t\t\t\t     inet->inet_dport).off.seq);"),
    # tp->tsoffset = secure_tcpv6_ts_off(net, ...)
    (b"tp->tsoffset = secure_tcpv6_ts_off(net,\n"
     b"\t\t\t\t\t  np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t  sk->sk_v6_daddr.s6_addr32);",
     b"tp->tsoffset = secure_tcpv6_seq_and_ts_off(net,\n"
     b"\t\t\t\t\t  np->saddr.s6_addr32,\n"
     b"\t\t\t\t\t  sk->sk_v6_daddr.s6_addr32,\n"
     b"\t\t\t\t\t  0, 0).off.ts_off;"),
]

for old, new in pairs:
    n = d.count(old)
    print(f"count={n} for {old[:46]!r}")
    d = d.replace(old, new)

with open(fp, "wb") as f:
    f.write(d)

# verify
with open(fp, "rb") as f:
    out = f.read()
import re
left = [m.group(0) for m in re.finditer(rb"secure_tcpv6_(seq|ts_off)\s*\(", out)]
print("residual old-style calls:", left)
print("new-style calls:", out.count(b"secure_tcpv6_seq_and_ts_off("))
print("bytes:", len(out))
