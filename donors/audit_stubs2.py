import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    # Is there any definition of tcp_chrono_start?
    "grep -rn 'tcp_chrono_start' net/ | grep -v 'tcp_chrono_start(' | head; "
    "echo '--- defs (line starts with void tcp_chrono_start) ---'; "
    "grep -rn '^void tcp_chrono_start' net/ include/ 2>/dev/null | head",

    # ip_tunnel_limit_headroom: how is it CALLED? (one arg or two?)
    "grep -n 'ip_tunnel_limit_headroom' net/ipv6/ip6_gre.c net/ipv6/ip6_tunnel.c net/ipv6/sit.c | head -6",

    # Does upstream 6.18 have ip_tunnel_limit_headroom at all? check the tree's
    # own definition
    "grep -rn 'ip_tunnel_limit_headroom' net/ipv4/ net/core/ 2>/dev/null | head -6",

    # kzalloc_flex signature in upstream 6.18: kzalloc_flex(count, size, flags)
    "grep -rn 'kzalloc_flex' include/linux/slab_def.h include/linux/slab.h mm/slab_common.c 2>/dev/null | head -8",
    "sed -n '4470,4485p' net/packet/af_packet.c",

    # amba bus: does drivers/amba exist?
    "ls -la drivers/amba/ 2>&1 | head -5",
    "grep -rn 'amba_bustype' drivers/amba/ 2>/dev/null | head -5",

    # who references amba_bustype (would it even be linked in?)
    "grep -rn 'amba_bustype' drivers/ arch/ include/ 2>/dev/null | grep -v 'include/linux/amba' | head -8",
]


def run(cmd):
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:100]}")
    print(run(c))
    print("-" * 70)
