import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # Compare with upstream v6.18 arch/arm64/Makefile
    "grep -n 'core-y\\|core-' /tmp/u_arm64_makefile 2>/dev/null | head -10 || echo 'not cached'",
    # Does the tree have an arch/arm64/Makefile that differs a lot?
    "diff <(curl -s https://raw.githubusercontent.com/torvalds/linux/v6.18/arch/arm64/Makefile) arch/arm64/Makefile 2>&1 | head -40",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:3000])
    print("-" * 60)
