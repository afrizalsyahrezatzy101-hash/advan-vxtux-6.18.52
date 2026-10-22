import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
TMP = os.environ.get("TMPDIR", "/tmp")

FILES = [
    # the Kconfig wiring: without this every drivers/vxtux symbol is
    # dropped again by syncconfig
    "drivers/Kconfig",
    "drivers/vxtux/audio/Kconfig",
    # the composite .ko renames
    "drivers/devfreq/sprd/apsys/Makefile",
    "drivers/vxtux/audio/audiosipc/Makefile",
    "drivers/vxtux/audio/Makefile",
    "drivers/sprd/flash/Makefile",
    "drivers/soc/sprd/sipc/Makefile",
    "drivers/sound/soc/sprd/audio_pipe/Makefile",
    # the modpost namespace fix
    "drivers/trusty/sprd_shm.c",
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

# the config script
sh = "E:/vxtux_build_repo/donors/set_deblob_config.sh"
flat = os.path.join(TMP, "set_deblob_config.sh")
shutil.copyfile(sh, flat)
r = subprocess.run(["gh", "codespace", "cp", "-c", CS, flat,
                    "remote:/tmp/set_deblob_config.sh"],
                   capture_output=True, text=True, timeout=600)
print("config script:", "OK" if r.returncode == 0 else r.stderr[:150])
