import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    # every single mention of ip_tunnel_limit_headroom, with context
    "grep -rn 'ip_tunnel_limit_headroom' . --include=*.c --include=*.h 2>/dev/null | head -20",
    # is berry_compat.h included by those files?
    "grep -n '#include' net/ipv6/ip6_gre.c | head -30",
    # upstream slab.h: kzalloc_flex signature
    "grep -rn 'kzalloc_flex' include/ 2>/dev/null | head -8",
    # what does the local tree's include/linux/slab.h have?
    "grep -n 'flex' include/linux/slab.h include/linux/slab_def.h 2>/dev/null | head -10",
    # local af_packet.c around the call - full statement
    "grep -n -B 3 -A 3 'kzalloc_flex' net/packet/af_packet.c",
    # upstream 6.18 af_packet.c alloc_pg_vec
    "grep -n -A 12 'static struct pgv \\*alloc_pg_vec' net/packet/af_packet.c",
]


def run(cmd):
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:95]}")
    print(run(c))
    print("-" * 70)
