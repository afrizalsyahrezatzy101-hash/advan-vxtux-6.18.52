#!/usr/bin/env python3
"""Compare each stock vendor blob against the 6.18 tree's built symbol surface.

For every blob in donors/stock_vendor_modules/ take its exported symbol
surface (nm -g, minus compiler/toolchain bookkeeping) and report how many of
those symbols the tree provides - either from a built module (Module.symvers)
or from vmlinux (built-in; provider == "vmlinux" in Module.symvers).

A blob whose symbol surface is empty after filtering genuinely exports
nothing (module-local), which is reported as API-free, not as a failure.

Usage:
    python3 donors/blob_gap.py donors/bb_Module.symvers
"""
import os
import re
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
BLOBS = "donors/stock_vendor_modules"

# compiler/toolchain bookkeeping that is not part of the driver's API
SKIP = re.compile(
    r"^(__cfi|__this_module$|__mod_of__|__typeid__|__crc_|__kstrtab_|__ksymtab_"
    r"|__UNIQUE_ID|__warned|__key|__param|__exit|__pfx_|__func__|__used"
    r"|__stack_chk|__fentry__|__llvm|__mod|__this|__exidx|__gcc_except"
    r"|\.L|cleanup_module$|init_module$)"
)

# stock blob name -> substrings identifying the tree module that carries the API
MAPPING = {
    "aes-ce-ccm.ko": ["ce_aes_ccm", "aes_ce_ccm"],
    "apsys-dvfs.ko": ["apsys_dvfs", "dpu_dvfs", "dpu_devfreq"],
    "audio-dsp-dump.ko": ["audio_dsp_dump", "audio_sblock"],
    "audio_sipc.ko": ["audio_sipc"],
    "audio-pipe.ko": ["audio_pipe", "apipe"],
    "gpio-pmic-eic-sprd.ko": ["gpio_pmic_eic", "pmic_eic"],
    "ion_ipc_trusty.ko": ["ion_ipc_trusty"],
    "mmdvfs.ko": ["sprd_mm_dvfs", "mm_dvfs"],
    "novatek_ts_spi.ko": ["novatek_nvt_ts_spi", "nt36"],
    "nvmem_sprd_cache_efuse.ko": ["sprd_cache_efuse"],
    "nvmem-sc27xx-efuse.ko": ["sc27xx_efuse"],
    "pwm_bl.ko": ["pwm_bl", "pwm_backlight"],
    "rtc-sc27xx.ko": ["rtc_sc27xx", "sc27xx_rtc"],
    "sc27xx_adc.ko": ["sc27xx_adc"],
    "sdhci-sprd.ko": ["sdhci_sprd", "sdhci-sprd"],
    "sprd_apipe.ko": ["apipe"],
    "sprd_flash_drv.ko": ["sprd_flash"],
    "sprd_power_manager.ko": ["sprd_power_manager", "sc27xx_pmu"],
    "sprd_time_sync.ko": ["sprd_time_sync"],
    "sprd-dmaengine-pcm.ko": ["sprd_dmaengine_pcm"],
    "sprd-sipc-virt-bus.ko": ["sprd_sipc_virt_bus"],
    "sprd-vsp-pw-domain.ko": ["vsp_regs", "sprd_vsp"],
    "trusty.ko": ["trusty_core"],
    "trusty-ipc.ko": ["trusty_ipc"],
    "trusty-irq.ko": ["trusty_irq"],
    "trusty-log.ko": ["trusty_log"],
    "trusty-tui.ko": ["trusty_tui"],
    "twofish_common.ko": ["twofish"],
    "ums512-clk.ko": ["ums512_clk", "sc9863", "sprd_clk"],
    "unisoc-mailbox.ko": ["sprd_mailbox", "unisoc_mailbox", "sprd-mailbox"],
    "usb_f_vser.ko": ["usb_f_vserial", "f_vserial"],
}


def blob_syms(path):
    """Return (set_of_symbols, error_or_None). An empty set is valid."""
    try:
        r = subprocess.run([NM, "-g", "--defined-only", path],
                           capture_output=True, text=True, timeout=120)
    except Exception as e:
        return None, str(e)
    if r.returncode != 0:
        return None, r.stderr.strip()[:120]
    syms = set()
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) < 3:
            continue
        typ, name = parts[-2], parts[-1]
        if typ not in ("T", "D", "B", "R", "W", "V", "A"):
            continue
        if SKIP.match(name):
            continue
        syms.add(name)
    return syms, None


def load_symvers(path):
    """symbol -> provider module path (or 'vmlinux' for built-in)."""
    prov = {}
    with open(path, errors="replace") as fh:
        for line in fh:
            f = line.rstrip("\n").split("\t")
            if len(f) >= 4:
                prov[f[1]] = f[2]
    return prov


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    symvers = load_symvers(sys.argv[1])
    total = covered = 0
    apifree = 0
    rows = []
    for blob in sorted(os.listdir(BLOBS)):
        if not blob.endswith(".ko"):
            continue
        syms, err = blob_syms(os.path.join(BLOBS, blob))
        if syms is None:
            print("ERR  %-28s %s" % (blob, err))
            continue
        if not syms:
            apifree += 1
            rows.append((blob, "APIFREE", 0, 0, []))
            continue
        hints = MAPPING.get(blob, [])
        hit, miss = [], []
        for s in syms:
            mod = symvers.get(s)
            if mod is None:
                miss.append(s)
            elif mod == "vmlinux" or any(h in mod for h in hints):
                hit.append(s)
            else:
                miss.append(s)  # exists, but in an unrelated module
        total += len(syms)
        covered += len(hit)
        rows.append((blob, "OK" if not miss else "GAP",
                     len(hit), len(syms), sorted(miss)))
    for blob, flag, h, t, miss in rows:
        if flag == "APIFREE":
            print("--  %-28s (no exported API)" % blob)
        elif flag == "OK":
            print("OK  %-28s %3d/%3d" % (blob, h, t))
        else:
            print("GAP %-28s %3d/%3d  missing: %s"
                  % (blob, h, t, ", ".join(miss[:8])))
    print("\nTOTAL %d/%d = %.1f%%   (api-free blobs: %d)"
          % (covered, total, 100.0 * covered / max(total, 1), apifree))
    return 0


if __name__ == "__main__":
    sys.exit(main())
