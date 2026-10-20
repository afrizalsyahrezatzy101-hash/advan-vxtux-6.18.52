import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # Makefile.defconf might set core-y
    "grep -rn 'core-y\\|core-' scripts/Makefile.defconf 2>/dev/null | head -10",
    "grep -c . scripts/Makefile.defconf 2>/dev/null",
    # maybe there is a separate file included
    "grep -n 'include ' Makefile 2>/dev/null | head -30",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:2500])
    print("-" * 60)
