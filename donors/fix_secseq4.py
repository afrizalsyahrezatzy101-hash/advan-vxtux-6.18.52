import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "net/ipv6/tcp_ipv6.c"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

# tcp_v6_init_seq(const struct sk_buff *skb) has no `net` in scope.
# secure_tcpv6_seq_and_ts_off() dereferences net->ipv4.sysctl_tcp_timestamps
# only for the ts_off half, which this call site ignores.
# Use init_net as a safe placeholder there.
old = (b"static u32 tcp_v6_init_seq(const struct sk_buff *skb)\n"
       b"{\n"
       b"\treturn secure_tcpv6_seq_and_ts_off(net,\n")
new = (b"static u32 tcp_v6_init_seq(const struct sk_buff *skb)\n"
       b"{\n"
       b"\treturn secure_tcpv6_seq_and_ts_off(init_net,\n")
n = d.count(old)
print("tcp_v6_init_seq patch count:", n)
d = d.replace(old, new)

with open(fp, "wb") as f:
    f.write(d)

with open(fp, "rb") as f:
    out = f.read()
print("bytes:", len(out))
# show region
i = out.find(b"static u32 tcp_v6_init_seq")
print(out[i:i+320].decode("utf-8", "replace"))
