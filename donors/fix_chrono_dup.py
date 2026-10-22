import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

# ---------------------------------------------------------------- 1) read the
# header-only shim so we know exactly which declarations exist there
print("=== include/linux/berry_compat.h : relevant declarations ===")
with open(os.path.join(ROOT, "include/linux/berry_compat.h"), "rb") as f:
    hdr = f.read().decode("utf-8", "replace")
for i, l in enumerate(hdr.split("\n"), 1):
    if any(k in l for k in ("kzalloc_flex", "ip_tunnel_limit_headroom",
                            "ip_tunnel_init", "tcp_chrono", "amba_bustype",
                            "past_receiver", "past_sender")):
        print(f"  {i}: {l}")

# ---------------------------------------------------------------- 2) patch
p = "kernel/berry_compat_link.c"
fp = os.path.join(ROOT, p)
with open(fp, "rb") as f:
    d = f.read()

STUB = (b"void tcp_chrono_stop(struct sock *sk, const enum tcp_chrono type)\n"
        b"{\n\t(void)sk;\n\t(void)type;\n}\n"
        b"EXPORT_SYMBOL_GPL(tcp_chrono_stop);\n")

if STUB in d:
    d = d.replace(STUB, b"")
    # also fix the comment block above it
    d = d.replace(
        b"/* ====== tcp_chrono_start / tcp_chrono_stop ======\n"
        b" * The chrono tracking body was dropped from net/ipv4/tcp.c but the\n"
        b" * declaration and its callers remain. No-op stubs keep the link happy.\n"
        b" */\n",
        b"/* ====== tcp_chrono_start ======\n"
        b" * tcp_chrono_stop() has a real implementation in net/ipv4/tcp_output.c,\n"
        b" * but tcp_chrono_start() was dropped upstream: it is still declared in\n"
        b" * include/net/tcp.h and called from tcp_output.c, so it needs a body.\n"
        b" */\n")
    with open(fp, "wb") as f:
        f.write(d)
    print("\nREMOVED tcp_chrono_stop stub")
else:
    print("\ntcp_chrono_stop stub pattern NOT found - dumping file for review")
    with open(fp, "rb") as f:
        for i, l in enumerate(f.read().split(b"\n"), 1):
            print(f"{i}: {l.decode('utf-8','replace')}")

# ---------------------------------------------------------------- 3) verify
with open(fp, "rb") as f:
    out = f.read()
print(f"\n=== kernel/berry_compat_link.c now ({len(out)} bytes) ===")
for i, l in enumerate(out.split(b"\n"), 1):
    t = l.decode("utf-8", "replace")
    if t.strip() and not t.strip().startswith("*") and not t.strip().startswith("/*"):
        print(f"{i}: {t}")
    elif t.strip().startswith("/*") or t.strip().startswith("*"):
        print(f"{i}: {t}")
