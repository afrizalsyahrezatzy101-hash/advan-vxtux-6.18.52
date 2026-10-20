import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

print("### netdev_rx_queue.c: where are the definitions? ###")
with open(os.path.join(ROOT, "net/core/netdev_rx_queue.c"), "rb") as f:
    lines = f.read().split(b"\n")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if ("__net_mp_open_rxq" in t or "__net_mp_close_rxq" in t
            or t.startswith("#if 0") or t.startswith("#endif")):
        print(f"{i:4}: {t}")

print()
print("### does the local struct netdev_rx_queue have mp_params.mp_ops? ###")
r = subprocess.run(
    ["bash", "-lc",
     f"cd {ROOT} && grep -n -A 25 'struct netdev_rx_queue {{' include/net/netdev_rx_queue.h 2>/dev/null | head -30"],
    capture_output=True, text=True, timeout=300)
print(r.stdout.strip() or "(none)")

print()
print("### page_pool memory_provider: does mp_params have mp_ops? ###")
r = subprocess.run(
    ["bash", "-lc",
     f"cd {ROOT} && grep -n -B 2 -A 12 'struct pp_memory_provider_params' include/net/page_pool/memory_provider.h 2>/dev/null | head -25"],
    capture_output=True, text=True, timeout=300)
print(r.stdout.strip() or "(none)")

print()
print("### __netif_get_rx_queue available? ###")
r = subprocess.run(
    ["bash", "-lc",
     f"cd {ROOT} && grep -rn '__netif_get_rx_queue' include/ net/core/*.c 2>/dev/null | head -5"],
    capture_output=True, text=True, timeout=300)
print(r.stdout.strip() or "(none)")
