"""Show exactly which symbols are missing from each substitute."""
import os
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
VML = "E:/vxtux_build_repo/build_output/vxtux_6.18.52/vmlinux"
STOCK = "E:/vxtux_build_repo/donors/stock_vendor_modules"
BUILT = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"

MODS = ["gpio-pmic-eic-sprd", "rtc-sc27xx", "sc27xx_adc",
        "nvmem-sc27xx-efuse", "sdhci-sprd", "unisoc-mailbox",
        "sprd-vsp-pw-domain", "aes-ce-ccm", "trusty-irq",
        "trusty-tui", "audio-pipe", "sprd_apipe", "sprd-sipc-virt-bus",
        "sprd_time_sync", "usb_f_vser", "sprd_flash_drv",
        "sprd-dmaengine-pcm", "audio_sipc", "audio-dsp-dump", "mmdvfs",
        "novatek_ts_spi", "apsys-dvfs"]


def nm(path, extern=True):
    cmd = [NM, "--defined-only"]
    if extern:
        cmd.append("--extern-only")
    cmd.append(path)
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=180)
    if r.returncode != 0:
        return None
    out = set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) >= 3 and p[1] in ("T", "D", "R", "B", "W"):
            out.add(p[2])
    return out


vml = nm(VML)
print(f"vmlinux global exports: {len(vml):,}\n")

for m in MODS:
    sp = os.path.join(STOCK, f"{m}.ko")
    if not os.path.exists(sp):
        sp2 = os.path.join(STOCK, f"{m.replace('_','-')}.ko")
        sp = sp2 if os.path.exists(sp2) else None
    if not sp:
        print(f"{m}: no stock .ko")
        continue
    ss = nm(sp)
    need = {x for x in ss if not x.startswith("__")}
    # also try non-extern (local) symbols of the stock module
    allss = nm(sp, extern=False) or set()
    need_all = {x for x in allss if not x.startswith("__")}
    miss_ext = sorted(need - vml)
    miss_all = sorted(need_all - vml)
    print(f"--- {m} ({os.path.basename(sp)}) ---")
    print(f"    stock global syms : {len(need)}")
    print(f"    missing (global)  : {len(miss_ext)} -> {miss_ext[:8]}")
    print(f"    missing (all)     : {len(miss_all)} -> {miss_all[:8]}")
    print()
