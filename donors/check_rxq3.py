import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmd = ("cd " + ROOT + " && grep -rn -A 15 'struct pp_memory_provider_params {' "
       "include/ 2>/dev/null | head -25")
r = subprocess.run(["bash", "-lc", cmd],
                   capture_output=True, text=True, timeout=300)
print("### local struct pp_memory_provider_params ###")
print(r.stdout.strip() or "(none)")

print()
print("### the #if 0 block, lines 110..215 ###")
with open(os.path.join(ROOT, "net/core/netdev_rx_queue.c"), "rb") as f:
    lines = f.read().split(b"\n")
for i in range(109, min(215, len(lines))):
    print(f"{i+1:4}: {lines[i].decode('utf-8', 'replace')}")
