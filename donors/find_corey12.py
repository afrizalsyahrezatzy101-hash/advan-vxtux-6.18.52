import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    "sed -n '1,30p' scripts/Makefile.build 2>/dev/null",
    # grep the whole tree (not just Makefiles) for 'init/' as an obj-y entry
    "grep -rn 'obj-y.*init/' --include=Makefile* . 2>/dev/null | grep -v Documentation | head -5",
    # maybe Kbuild.include or scripts/Makefile.lib has it
    "grep -rn 'init/' scripts/Makefile.lib scripts/Kbuild.include scripts/Makefile.build 2>/dev/null | head -10",
    # Check the top-level Makefile for the literal string 'init'
    "grep -n 'init' Makefile 2>/dev/null | grep -v 'initcall\\|Initial\\|init_\\|initial' | head -20",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:3000])
    print("-" * 60)
