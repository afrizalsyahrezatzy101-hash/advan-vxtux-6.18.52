import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

# grep -rn for core-y assignments across top-level makefiles
for cmd in [
    "grep -rn 'core-y' Makefile arch/arm64/Makefile scripts/Makefile.build 2>/dev/null | head -20",
    "grep -rn 'core-y' arch/Makefile 2>/dev/null | head -10",
    "grep -rn 'core-y *:*=' Makefile 2>/dev/null | head -10",
]:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True)
    print(f"$ {cmd}")
    print(r.stdout.strip() or "(none)")
    print("-" * 50)
