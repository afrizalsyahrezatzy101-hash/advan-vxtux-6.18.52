import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # what does CONFIG_IO_URING gate in the tree?
    "grep -rn 'CONFIG_IO_URING' --include=*.c --include=*.h --include=Kconfig* . 2>/dev/null | grep -v Documentation | grep -v 'io_uring.disabled' | head -25",
    # cmd.h: is the static inline branch also gated?
    "sed -n '100,140p' include/linux/io_uring/cmd.h 2>/dev/null",
    # fuse dev_uring build gate
    "grep -n 'FUSE_IO_URING' fs/fuse/Makefile .config 2>/dev/null | head -5",
    # io_uring Kconfig
    "cat io_uring/Kconfig 2>/dev/null",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:3000])
    print("-" * 60)
