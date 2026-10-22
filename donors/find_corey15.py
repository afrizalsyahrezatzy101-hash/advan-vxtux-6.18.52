import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # Is the root built-in.a rebuilt by make?
    "make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- HOSTCC=gcc --no-print-directory -n built-in.a 2>&1 | tail -5",
    # git history: when was core-y removed from Makefile?
    "git log --oneline -8 -- Makefile 2>&1 | head -10",
    # does any historical version of Makefile have core-y +=?
    "git log -p --all -- Makefile 2>/dev/null | grep -n 'core-y' | head -20",
    # maybe it's in arch/Makefile
    "grep -n 'core' arch/Makefile 2>/dev/null | head -10",
    "ls arch/Makefile 2>&1",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=600)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:2500])
    print("-" * 60)
