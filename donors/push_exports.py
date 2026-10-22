import os
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

FILES = [
    "arch/arm64/crypto/aes-ce-ccm-glue.c",
    "drivers/dma-buf/sprd/ion_ipc_trusty.c",
    "drivers/mmc/host/sdhci-sprd.c",
    "drivers/misc/trusty-tui.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx_fw_update.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx_ext_proc.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx_mp_ctrlram.c",
]

tmp = os.path.join(os.environ.get("TMPDIR", "/tmp"), "vxtux_push")
os.makedirs(tmp, exist_ok=True)

for rel in FILES:
    src = os.path.join(LOCAL, rel)
    if not os.path.exists(src):
        print(f"MISSING {rel}")
        continue
    size = os.path.getsize(src)
    # copy to a flat temp name (gh codespace cp has no mkdir -p)
    flat = os.path.join(tmp, rel.replace("/", "__"))
    import shutil
    shutil.copyfile(src, flat)
    r = subprocess.run(
        ["gh", "codespace", "cp", "-c", CS, flat, f"remote:{ROOT}/{rel}"],
        capture_output=True, text=True, timeout=600)
    if r.returncode != 0:
        print(f"FAIL {rel}: {r.stderr.strip()[:120]}")
        continue
    # verify the size on the remote side
    r2 = subprocess.run(
        ["gh", "codespace", "ssh", "-c", CS, "--",
         f"stat -c %s {ROOT}/{rel}"],
        capture_output=True, text=True, timeout=300)
    got = r2.stdout.strip()
    ok = got == str(size)
    print(f"{'OK  ' if ok else 'BAD '} {rel}  local={size:,} remote={got}")
