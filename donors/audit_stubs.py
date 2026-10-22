import os
import re
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
STUB = "kernel/berry_compat_link.c"

STUBS = [
    "ip_tunnel_init",
    "ip_tunnel_limit_headroom",
    "tcp_chrono_start",
    "tcp_chrono_stop",
    "kzalloc_flex",
    "amba_bustype",
]

with open(os.path.join(ROOT, STUB), "rb") as f:
    src = f.read().decode("utf-8", "replace")

# what signature does each stub use?
print("=== stub definitions in berry_compat_link.c ===")
for s in STUBS:
    m = re.search(r"^[^\n/]*\b" + s + r"\s*\([^)]*\)\s*$", src, re.M)
    m2 = re.search(r"^.*\b" + s + r"\s*\([^)]*\)\s*\{", src, re.M)
    line = m2.group(0).strip() if m2 else "(not a function)"
    print(f"  {s}:\n      {line}")

# what does the TREE declare / define for these?
print()
print("=== tree declarations & definitions ===")
for s in STUBS:
    r = subprocess.run(
        ["bash", "-lc",
         f"cd {ROOT} && grep -rn --include=*.c --include=*.h "
         f"'{s}' . 2>/dev/null | grep -v '{STUB}' | head -8"],
        capture_output=True, text=True, timeout=300)
    print(f"--- {s} ---")
    out = r.stdout.strip()
    print("   " + (out.replace("\n", "\n   ") if out else "(no other reference)"))

# where is the tcp_chrono implementation?
print()
print("=== tcp_chrono real implementation ===")
for cmd in [
    "grep -rn 'void tcp_chrono_start\\|void tcp_chrono_stop' net/ include/ 2>/dev/null | head -10",
    "grep -n 'ip_tunnel_limit_headroom\\|ip_tunnel_init' include/net/ip_tunnels.h | head -10",
    "grep -rn 'kzalloc_flex' include/linux/slab.h mm/*.c net/packet/af_packet.c 2>/dev/null | head -8",
]:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print("   " + (r.stdout.strip().replace("\n", "\n   ") or "(none)"))
