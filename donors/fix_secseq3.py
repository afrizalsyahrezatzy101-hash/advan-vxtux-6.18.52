import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "net/ipv6/tcp_ipv6.c"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

# Match secure_tcpv6_seq( ... ) -> 4 args, wrap into merged helper
# secure_tcpv6_seq(a, b, c, d)  ->  secure_tcpv6_seq_and_ts_off(net, a, b, c, d).off.seq
pat_seq = re.compile(
    rb"secure_tcpv6_seq\(\s*"
    rb"(?P<a>[^,()]+?)\s*,\s*"
    rb"(?P<b>[^,()]+?)\s*,\s*"
    rb"(?P<c>[^,()]+?)\s*,\s*"
    rb"(?P<d>[^,()]+?)\s*\)",
    re.S)
pat_ts = re.compile(
    rb"secure_tcpv6_ts_off\(\s*"
    rb"(?P<net>[^,()]+?)\s*,\s*"
    rb"(?P<a>[^,()]+?)\s*,\s*"
    rb"(?P<b>[^,()]+?)\s*\)",
    re.S)

def repl_seq(m):
    return (b"secure_tcpv6_seq_and_ts_off(net, " +
            m.group("a") + b", " + m.group("b") + b", " +
            m.group("c") + b", " + m.group("d") + b").off.seq")

def repl_ts(m):
    return (b"secure_tcpv6_seq_and_ts_off(" + m.group("net") + b", " +
            m.group("a") + b", " + m.group("b") + b", 0, 0).off.ts_off")

d2, n1 = pat_seq.subn(repl_seq, d)
d2, n2 = pat_ts.subn(repl_ts, d2)
print(f"secure_tcpv6_seq replaced: {n1}")
print(f"secure_tcpv6_ts_off replaced: {n2}")

with open(fp, "wb") as f:
    f.write(d2)

with open(fp, "rb") as f:
    out = f.read()
print("residual:", [m.group(0) for m in re.finditer(rb"secure_tcpv6_(seq|ts_off)\s*\(", out)])
print("merged calls:", out.count(b"secure_tcpv6_seq_and_ts_off("))

# show the patched regions as text (may be redacted in display, that's fine)
for m in re.finditer(rb"secure_tcpv6_seq_and_ts_off\(", out):
    s = max(0, m.start() - 60)
    e = min(len(out), m.end() + 220)
    print("---")
    print(out[s:e].decode("utf-8", "replace"))
