import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

# The local ABI:
#   int  __net_mp_open_rxq(struct net_device *dev, unsigned int rxq_idx,
#                          const struct pp_memory_provider_params *p,
#                          struct netlink_ext_ack *extack);
#   void __net_mp_close_rxq(struct net_device *dev, unsigned int rxq_idx,
#                           const struct pp_memory_provider_params *old_p);
# and struct netdev_rx_queue HAS `struct pp_memory_provider_params mp_params;`

# What is inside struct pp_memory_provider_params locally?
r = subprocess.run(
    ["bash", "-lc",
     f"cd {ROOT} && grep -rn -A 15 'struct pp_memory_provider_params {' "
     f"include/ 2>/dev/null | head -25"],
    capture_output=True, text=True, timeout=300)
print("### local struct pp_memory_provider_params ###")
print(r.stdout.strip() or "(none)")

# what does the DISABLED block's __net_mp_open_rxq body look like (lines 115-212)?
print()
print("### the #if 0 block, lines 110..215 ###")
with open(os.path.join(ROOT, "net/core/netdev_rx_queue.c"), "rb") as f:
    lines = f.read().split(b"\n")
for i in range(109, min(215, len(lines))):
    print(f"{i+1:4}: {lines[i].decode('utf-8','replace')}")
