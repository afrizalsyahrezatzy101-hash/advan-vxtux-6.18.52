import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # Does root built-in.a really contain net/ipv4/ip_gre.o ?
    "aarch64-linux-gnu-ar t built-in.a 2>/dev/null | grep -c . ",
    "aarch64-linux-gnu-ar t built-in.a 2>/dev/null | grep -E 'ip_gre|ip_tunnel' | head -5",
    # any file anywhere containing 'core-y'
    "grep -rl 'core-y' . --exclude-dir=.git --exclude-dir=Documentation 2>/dev/null | head -20",
    # what is the link command for vmlinux
    "ls -la vmlinux.unstripped.cmd vmlinux.o vmlinux.a 2>&1 | head -5",
    "grep -o 'built-in.a\\|vmlinux.a' scripts/Makefile.vmlinux 2>/dev/null | head -5",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:2500])
    print("-" * 60)
