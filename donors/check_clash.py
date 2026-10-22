"""Check for symbol-name clashes before un-static'ing anything.

If we turn a `static` function into a global one, its name enters the
kernel's global namespace. If another object in vmlinux already
defines that name, the link fails with a duplicate-symbol error.
This checks the whole tree for each candidate.
"""
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
VML = "E:/vxtux_build_repo/build_output/vxtux_6.18.52/vmlinux"
NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"

# the 5 symbols that must lose `static`
STATIC_CANDIDATES = [
    ("ce_aes_ccm_auth_data", "arch/arm64/crypto/aes-ce-ccm-glue.c"),
    ("ion_handle_msg", "drivers/dma-buf/sprd/ion_ipc_trusty.c"),
    ("ion_tipc", "drivers/dma-buf/sprd/ion_ipc_trusty.c"),
    ("mmc_send_tuning_cmd", "drivers/mmc/host/sdhci-sprd.c"),
    ("trusty_tui_driver", "drivers/misc/trusty-tui.c"),
]

print("=== clash check: is the name already a global in vmlinux? ===")
r = subprocess.run([NM, "--defined-only", "--extern-only", VML],
                   capture_output=True, text=True, timeout=300)
vml_globals = set()
for line in r.stdout.splitlines():
    p = line.split()
    if len(p) >= 3:
        vml_globals.add(p[2])

for sym, rel in STATIC_CANDIDATES:
    in_vml = sym in vml_globals
    # also search the tree for other definitions
    rr = subprocess.run([GIT, "-C", TREE, "grep", "-l", "-w", sym, "--",
                         "*.c", "*.h"], capture_output=True, text=True,
                        timeout=300)
    files = [f for f in rr.stdout.splitlines()]
    others = [f for f in files
              if not f.endswith(rel.replace("/", "\\")) and rel not in f]
    print(f"  {sym:<26} in_vmlinux_globals={in_vml}")
    print(f"       referenced in {len(files)} file(s); other defs in: "
          f"{others[:4] if others else 'none'}")
    if in_vml:
        print(f"       *** CLASH: already defined in vmlinux ***")
    print()
