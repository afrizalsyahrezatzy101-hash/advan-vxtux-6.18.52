"""Push files to the codespace with LF line endings.

The tree is edited on Windows, where the tools write CRLF. Pushing
those files verbatim breaks the build in two different ways:

  Kconfig   "warning: ignoring unsupported character '\\r'" and then
            the whole symbol is silently dropped
  Makefile  kbuild is more tolerant, but the stray CR ends up in the
            object lists and in the module names

So every file that is pushed is normalised to LF first, and the
remote size is compared against the LOCAL NORMALISED size - not
against the on-disk size, which would be the CRLF one.

Usage:
    python3 push_lf.py file1 [file2 ...]      # relative to the tree
    python3 push_lf.py --all                   # every changed file
"""
import os
import shutil
import subprocess
import sys

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
LOCAL = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
TMP = os.environ.get("TMPDIR", "/tmp")

# the files that must be on the codespace for the deblob build
DEFAULT = [
    "drivers/Kconfig",
    "drivers/vxtux/audio/Kconfig",
    "drivers/vxtux/audio/audiosipc/Makefile",
    "drivers/vxtux/audio/Makefile",
    "drivers/vxtux/audio/audio_pipe/Makefile",
    "drivers/devfreq/sprd/apsys/Makefile",
    "drivers/sprd/flash/Makefile",
    "drivers/soc/sprd/sipc/Makefile",
    "drivers/sound/soc/sprd/audio_pipe/Makefile",
    "drivers/pmdomain/sprd/Makefile",
    "drivers/pmdomain/Makefile",
    "drivers/soc/sprd/Makefile",
    "drivers/soc/sprd/cache_efuse/Makefile",
    "include/linux/trusty/trusty.h",
    "drivers/trusty/sprd_shm.c",
    "arch/arm64/crypto/aes-ce-ccm-glue.c",
    "drivers/dma-buf/sprd/ion_ipc_trusty.c",
    "drivers/mmc/host/sdhci-sprd.c",
    "drivers/misc/trusty-tui.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx_fw_update.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx_ext_proc.c",
    "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx_mp_ctrlram.c",
]


def to_lf(data):
    return data.replace(b"\r\n", b"\n").replace(b"\r", b"\n")


def main():
    files = sys.argv[1:] if len(sys.argv) > 1 else DEFAULT
    if files and files[0] == "--all":
        files = DEFAULT
    bad = 0
    for rel in files:
        src = os.path.join(LOCAL, rel)
        if not os.path.exists(src):
            print(f"MISSING {rel}")
            bad += 1
            continue
        raw = open(src, "rb").read()
        data = to_lf(raw)
        flat = os.path.join(TMP, rel.replace("/", "__"))
        with open(flat, "wb") as f:
            f.write(data)
        r = subprocess.run(
            ["gh", "codespace", "cp", "-c", CS, flat, f"remote:{ROOT}/{rel}"],
            capture_output=True, text=True, timeout=900)
        if r.returncode != 0:
            print(f"FAIL {rel}: {r.stderr.strip()[:150]}")
            bad += 1
            continue
        r2 = subprocess.run(
            ["gh", "codespace", "ssh", "-c", CS, "--",
             f"stat -c %s {ROOT}/{rel}"],
            capture_output=True, text=True, timeout=300)
        got = r2.stdout.strip()
        ok = got == str(len(data))
        crlf = b"\r\n" in data
        print(f"{'OK  ' if ok else 'BAD '} {rel:<62} "
              f"{len(data):>7,} B  crlf={crlf}")
        if not ok:
            bad += 1
    print()
    print(f"pushed {len(files) - bad}/{len(files)}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
