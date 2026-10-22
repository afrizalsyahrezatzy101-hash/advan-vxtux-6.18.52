import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

# Ask make directly what core-y and obj-y are.
cmd = (f"cd {ROOT} && make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- "
       f"HOSTCC=gcc --no-print-directory -f scripts/Makefile.printvars 2>&1 | head -40")
r = subprocess.run(["bash", "-lc", cmd], capture_output=True, text=True, timeout=300)
print(r.stdout[:3000])
print("STDERR:", r.stderr[:1000])
