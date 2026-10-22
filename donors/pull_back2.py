import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

FILES = [
    "net/packet/af_packet.c",
    "net/core/netdev_rx_queue.c",
    "drivers/usb/phy/Kconfig",
    ".config",
    "kernel/berry_compat_link.c",
    "include/net/bluetooth/hci_core.h",
    "net/ipv6/tcp_ipv6.c",
]

tmp = os.path.join(os.environ.get("TMPDIR", "/tmp"), "vxtux_pull2")
os.makedirs(tmp, exist_ok=True)

for rel in FILES:
    local_tmp = os.path.join(tmp, rel.replace("/", "__"))
    r = subprocess.run(
        ["gh", "codespace", "cp", "-c", CS, f"remote:{ROOT}/{rel}", local_tmp],
        capture_output=True, text=True, timeout=600)
    if r.returncode != 0 or not os.path.exists(local_tmp):
        print(f"FAIL {rel}: {r.stderr.strip()[:120]}")
        continue
    size = os.path.getsize(local_tmp)
    dest = os.path.join(LOCAL, rel)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    shutil.copyfile(local_tmp, dest)
    with open(dest, "rb") as f:
        ok = len(f.read()) == size
    print(f"{'OK  ' if ok else 'BAD '} {rel} ({size:,} bytes)")
