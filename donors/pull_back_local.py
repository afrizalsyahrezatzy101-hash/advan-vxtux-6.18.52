import os
import shutil
import subprocess
import sys

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CODESPACE = "solid-tribble-9rjjxvjqq5pcpq5g"

FILES = [
    "include/linux/berry_compat.h",
    "include/linux/netdevice.h",
    "include/net/bluetooth/hci.h",
    "include/linux/skmsg.h",
    "include/net/xsk_buff_pool.h",
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

tmp = os.path.join(os.environ.get("TMPDIR", "/tmp"), "vxtux_pullback")
os.makedirs(tmp, exist_ok=True)

for rel in FILES:
    remote = f"remote:{ROOT}/{rel}"
    local_tmp = os.path.join(tmp, rel.replace("/", "__"))
    r = subprocess.run(
        ["gh", "codespace", "cp", "-c", CODESPACE, remote, local_tmp],
        capture_output=True, text=True)
    if r.returncode != 0 or not os.path.exists(local_tmp):
        print(f"FAIL  {rel}: {r.stderr.strip()[:120]}")
        continue
    size = os.path.getsize(local_tmp)
    dest = os.path.join(LOCAL, rel)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    shutil.copyfile(local_tmp, dest)
    # verify
    with open(dest, "rb") as f:
        got = f.read()
    ok = len(got) == size
    print(f"{'OK  ' if ok else 'BAD '} {rel} ({size} bytes)")

# remove stale local file not present on codespace
stale = os.path.join(LOCAL, "kernel", "berry_compat.c")
if os.path.exists(stale):
    os.remove(stale)
    print("REMOVED stale kernel/berry_compat.c")
