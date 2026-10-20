import os
import subprocess

CODESPACE = "solid-tribble-9rjjxvjqq5pcpq5g"
UP = "https://raw.githubusercontent.com/torvalds/linux/v6.18"

FILES = [
    ("net/packet/af_packet.c", "alloc_pg_vec"),
    ("net/ipv6/ip6_gre.c", "ip_tunnel_limit_headroom"),
    ("net/ipv6/ip6_tunnel.c", "ip_tunnel_limit_headroom"),
    ("net/ipv6/sit.c", "ip_tunnel_limit_headroom"),
    ("include/net/ip_tunnels.h", "limit_headroom"),
    ("include/linux/slab.h", "flex"),
]

for path, needle in FILES:
    url = f"{UP}/{path}"
    cmd = (f"curl -sL --max-time 30 {url} | "
           f"grep -n -B 6 -A 10 '{needle}' | head -40")
    r = subprocess.run(
        ["gh", "codespace", "ssh", "-c", CODESPACE, "--", cmd],
        capture_output=True, text=True, timeout=180)
    print("=" * 74)
    print(f"UPSTREAM {path}  [{needle}]")
    out = (r.stdout.strip() or r.stderr.strip())
    print(out if out else "(no match)")
    print()
