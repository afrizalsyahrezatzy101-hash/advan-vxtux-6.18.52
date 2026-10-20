import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # where is the top-level built-in.a rule?
    "grep -rn 'built-in.a' Makefile scripts/Makefile.build scripts/Makefile.lib 2>/dev/null | head -10",
    # does the top Makefile have a 'core-y' set by scripts/Makefile.lib?
    "grep -rn 'core-y\\|core-' scripts/Makefile.lib scripts/Makefile.build 2>/dev/null | head -10",
    # what's in the top-level built-in.a right now?
    "aarch64-linux-gnu-ar t built-in.a 2>/dev/null | head -10",
    # ARCH_CORE env at build time?
    "grep -rn 'ARCH_CORE' scripts/ 2>/dev/null | head -10",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:1500])
    print("-" * 60)
