import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

print("### local netdevice.h LL_RESERVED_SPACE block ###")
with open(os.path.join(ROOT, "include/linux/netdevice.h"), "rb") as f:
    lines = f.read().split(b"\n")
for i in range(295, 318):
    print(f"{i+1:4}: {lines[i].decode('utf-8','replace')}")

print()
print("### upstream 6.18 netdevice.h LL_RESERVED_SPACE block ###")
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", "solid-tribble-9rjjxvjqq5pcpq5g", "--",
     "curl -sL --max-time 30 https://raw.githubusercontent.com/torvalds/linux/v6.18/"
     "include/linux/netdevice.h | grep -n -A 12 'define LL_RESERVED_SPACE' | head -20"],
    capture_output=True, text=True, timeout=180)
print(r.stdout.strip() or r.stderr.strip())

print()
print("### who uses LL_RESERVED_SPACE_EX in the local tree ###")
r = subprocess.run(
    ["bash", "-lc",
     f"cd {ROOT} && grep -rn 'LL_RESERVED_SPACE_EX' . --include=*.c --include=*.h 2>/dev/null | head -20"],
    capture_output=True, text=True, timeout=300)
print(r.stdout.strip() or "(none)")
