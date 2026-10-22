import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
TMP = os.environ.get("TMPDIR", "/tmp")

FILES = [
    ".config",
    "drivers/soc/sprd/cache_efuse/Makefile",
    "drivers/pmdomain/sprd/Makefile",
    "drivers/soc/sprd/Makefile",
    "drivers/pmdomain/Makefile",
    "drivers/vxtux/audio/Kconfig",
]

for rel in FILES:
    src = os.path.join(LOCAL, rel)
    if not os.path.exists(src):
        print(f"MISSING {rel}")
        continue
    size = os.path.getsize(src)
    flat = os.path.join(TMP, rel.replace("/", "__"))
    shutil.copyfile(src, flat)
    r = subprocess.run(
        ["gh", "codespace", "cp", "-c", CS, flat, f"remote:{ROOT}/{rel}"],
        capture_output=True, text=True, timeout=900)
    if r.returncode != 0:
        print(f"FAIL {rel}: {r.stderr.strip()[:150]}")
        continue
    r2 = subprocess.run(
        ["gh", "codespace", "ssh", "-c", CS, "--",
         f"stat -c %s {ROOT}/{rel}"],
        capture_output=True, text=True, timeout=300)
    got = r2.stdout.strip()
    print(f"{'OK  ' if got == str(size) else 'BAD '} {rel}  "
          f"local={size:,} remote={got}")
