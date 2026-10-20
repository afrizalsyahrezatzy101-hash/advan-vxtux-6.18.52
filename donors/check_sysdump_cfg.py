import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    "grep -E 'CONFIG_SPRD_SYSDUMP' .config | head -3",
    "grep -rn 'SPRD_SYSDUMP' drivers/soc/sprd/modem/Kconfig drivers/soc/sprd/Kconfig 2>/dev/null | head -10",
    "grep -rn 'config SPRD_SYSDUMP' . --include=Kconfig 2>/dev/null | head -5",
    # is the sprd_cp_dump.c the one being built? check for a second copy
    "find . -path ./io_uring.disabled -prune -o -name 'sprd_cp_dump.c' -print 2>/dev/null",
]


def run(cmd):
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:95]}")
    print(run(c))
    print("-" * 70)
