import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "net/ipv6/tcp_ipv6.c"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

# secure_tcpv6_seq_and_ts_off() takes `const struct net *`.
# init_net is a `struct net`, so pass &init_net.
old = b"return secure_tcpv6_seq_and_ts_off(init_net,"
new = b"return secure_tcpv6_seq_and_ts_off(&init_net,"

if old in d:
    d = d.replace(old, new)
    with open(fp, "wb") as f:
        f.write(d)
    print("patched: &init_net")
else:
    print("pattern not found; showing context")
    for i, l in enumerate(d.split(b"\n"), 1):
        if b"secure_tcpv6_seq_and_ts_off" in l:
            print(f"  {i}: {l.decode('utf-8','replace')}")

# show the function
with open(fp, "rb") as f:
    lines = f.read().split(b"\n")
print("--- tcp_v6_init_seq ---")
for i, l in enumerate(lines, 1):
    if 115 <= i <= 130:
        print(f"  {i}: {l.decode('utf-8','replace')}")
