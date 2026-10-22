import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
UP = "https://raw.githubusercontent.com/torvalds/linux/v6.18"

print("### 1. net/core/netdev_rx_queue.c in the VXTux tree (full) ###")
with open(os.path.join(ROOT, "net/core/netdev_rx_queue.c"), "rb") as f:
    for i, l in enumerate(f.read().split(b"\n"), 1):
        print(f"{i:4}: {l.decode('utf-8','replace')}")

print()
print("### 2. upstream include/linux/netdevice.h LL_RESERVED_SPACE ###")
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", "solid-tribble-9rjjxvjqq5pcpq5g", "--",
     f"curl -sL --max-time 30 {UP}/include/linux/netdevice.h | "
     "grep -n -B 2 -A 6 'LL_RESERVED_SPACE' | head -30"],
    capture_output=True, text=True, timeout=180)
print(r.stdout.strip() or r.stderr.strip())

print()
print("### 3. upstream net/core/netdev_rx_queue.c (real impl?) ###")
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", "solid-tribble-9rjjxvjqq5pcpq5g", "--",
     f"curl -sL --max-time 30 {UP}/net/core/netdev_rx_queue.c | head -60"],
    capture_output=True, text=True, timeout=180)
print(r.stdout.strip() or r.stderr.strip())

print()
print("### 4. local netdevice.h lines 295..320 ###")
with open(os.path.join(ROOT, "include/linux/netdevice.h"), "rb") as f:
    lines = f.read().split(b"\n")
for i in range(294, 322):
    print(f"{i+1:4}: {lines[i].decode('utf-8','replace')}")
