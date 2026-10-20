import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "/e/vxtux_build_repo/6.18.52-VXTux-UMS512"

# Files changed on the codespace that must be mirrored back to the local repo
FILES = [
    "include/linux/berry_compat.h",
    "include/linux/netdevice.h",
    "include/net/bluetooth/hci.h",
    "include/linux/skmsg.h",
    "include/net/xsk_buff_pool.h",
    "net/bluetooth/hci.h",
    "net/ipv6/tcp_ipv6.c",
    "net/netfilter/nf_conntrack_extend.c",
    "net/netfilter/nf_conntrack_count.c",
    "net/netfilter/Makefile",
    "net/vmw_vsock/vsock_bpf.c",
    "net/xdp/xsk_buff_pool.c",
    "net/unix/unix_bpf.c",
    "kernel/berry_compat_link.c",
    "kernel/Makefile",
    ".config",
]

for rel in FILES:
    rp = os.path.join(ROOT, rel)
    lp = os.path.join(LOCAL, rel)
    if not os.path.exists(rp):
        print(f"SKIP (missing on codespace): {rel}")
        continue
    os.makedirs(os.path.dirname(lp), exist_ok=True)
    with open(rp, "rb") as f:
        data = f.read()
    with open(lp, "wb") as f:
        f.write(data)
    print(f"COPIED {rel} ({len(data)} bytes)")

# remove the stale local file that the codespace doesn't have
stale = os.path.join(LOCAL, "kernel/berry_compat.c")
if os.path.exists(stale):
    os.remove(stale)
    print("REMOVED stale kernel/berry_compat.c")
