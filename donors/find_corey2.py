import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    "grep -rn 'core-y' --include=Makefile --include='Makefile.*' . 2>/dev/null | grep -v '^./Documentation' | head -20",
    "grep -n 'core-y\\|SUBDIRS\\|obj-y' scripts/Makefile.build 2>/dev/null | head -15",
    "grep -rn 'init/ kernel/ fs/ net/' Makefile 2>/dev/null | head -5",
    "sed -n '1195,1235p' Makefile 2>/dev/null",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True)
    print(f"$ {cmd}")
    print(r.stdout.strip() or "(none)")
    print("-" * 60)
