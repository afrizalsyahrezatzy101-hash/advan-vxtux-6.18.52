import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    "ls drivers/soc/sprd/modem/ 2>/dev/null | head -20",
    "cat drivers/soc/sprd/modem/Makefile 2>/dev/null | head -20",
    "cat drivers/soc/sprd/Makefile 2>/dev/null | head -30",
    # where does the sprd sysdump live?
    "find . -path ./io_uring.disabled -prune -o -name '*sysdump*' -print 2>/dev/null | head -10",
    "find . -path ./io_uring.disabled -prune -o -name 'sprd_sysdump*' -print 2>/dev/null | head -10",
    # the modem cp_dump caller context
    "sed -n '310,340p' drivers/soc/sprd/modem/cp_dump/sprd_cp_dump.c",
    # does the modem dir have its own Kconfig that gates cp_dump?
    "cat drivers/soc/sprd/modem/cp_dump/Makefile 2>/dev/null | head -10",
]


def run(cmd):
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:95]}")
    print(run(c))
    print("-" * 70)
