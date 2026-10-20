import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

# the 12 stock blobs with no .ko produced, and the source hints
TARGETS = {
    "apsys-dvfs": ["apsys", "dvfs"],
    "audio_sipc": ["audiosipc", "audio_sipc", "sipc"],
    "mmdvfs": ["mmdvfs", "mm_dvfs"],
    "nvmem_sprd_cache_efuse": ["cache_efuse"],
    "sprd-dmaengine-pcm": ["dmaengine", "pcm"],
    "sprd-sipc-virt-bus": ["sipc", "virt"],
    "sprd-vsp-pw-domain": ["pmdomain", "vsp_pw"],
    "sprd_apipe": ["apipe"],
    "sprd_flash_drv": ["flash"],
    "sprd_time_sync": ["time_sync", "timesync", "tshm"],
    "unisoc-mailbox": ["mailbox"],
    "usb_f_vser": ["vser", "f_vser"],
}

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
files = [f.replace("\\", "/") for f in r.stdout.splitlines()]

for t, hints in TARGETS.items():
    print("=" * 88)
    print(f"### {t}")
    # candidate directories
    cands = []
    for f in files:
        fl = f.lower()
        if not any(h in fl for h in hints):
            continue
        if f.endswith("Makefile"):
            cands.append(f)
    if not cands:
        print("   (no Makefile matched the hints)")
    for c in sorted(cands)[:14]:
        d = os.path.dirname(c)
        print(f"   {c}")
        try:
            txt = open(os.path.join(TREE, c), "r", encoding="utf-8",
                       errors="replace").read()
        except OSError:
            continue
        for m in re.finditer(r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*"
                             r"([^\n]*)", txt):
            print(f"        obj-({m.group(1)}) += {m.group(2).strip()}")
