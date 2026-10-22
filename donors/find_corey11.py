import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # Does arch/arm64/Makefile include something that sets core-y?
    "grep -n 'include ' arch/arm64/Makefile 2>/dev/null | head -20",
    # The real question: what does the top-level built-in.a rule depend on?
    "grep -n 'built-in.a' scripts/Makefile.build 2>/dev/null | head",
    "sed -n '455,480p' scripts/Makefile.build 2>/dev/null",
    # maybe vmlinux.a uses ARCH_CORE with a different name
    "grep -rn 'ARCH_CORE' . --include=Makefile --include='Makefile.*' 2>/dev/null | grep -v Documentation | head -10",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:2500])
    print("-" * 60)
