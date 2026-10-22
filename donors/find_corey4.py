import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    "grep -rn 'core-y *\\+=' --include=Makefile . 2>/dev/null | grep -v Documentation | head -10",
    "grep -rn 'core-y' arch/arm64/Makefile arch/Makefile scripts/Makefile.defconf 2>/dev/null | head -10",
    "grep -rn 'core-y' arch/arm64/ 2>/dev/null | head -10",
    "head -30 vmlinux.unstripped.cmd 2>/dev/null || ls vmlinux* 2>/dev/null",
    "grep -o 'arch/arm64/boot/Image' .vmlinux.objs 2>/dev/null | head -2; head -3 .vmlinux.objs 2>/dev/null",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True)
    print(f"$ {cmd}")
    out = r.stdout.strip()
    print(out[:2000] if out else "(none)")
    print("-" * 60)
