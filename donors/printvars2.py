import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # Inject a print into the top Makefile temporarily? No - instead ask make
    # to show the variable via a target that uses $(info).
    f"cd {ROOT} && cp Makefile /tmp/Makefile.bak && "
    f"printf '\\n$(info CORE-Y=[$(core-y)])\\n$(info OBJ-Y=[$(obj-y)])\\n$(info DRV=[$(drivers-y)])\\n' >> Makefile && "
    f"make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- HOSTCC=gcc --no-print-directory -f Makefile -n all 2>&1 | head -6; "
    f"cp /tmp/Makefile.bak Makefile; echo RESTORED",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", cmd], capture_output=True, text=True, timeout=300)
    print(r.stdout[:3000])
    print("ERR:", r.stderr[:800])
