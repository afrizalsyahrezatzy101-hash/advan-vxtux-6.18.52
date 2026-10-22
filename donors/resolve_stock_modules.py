"""Config-aware module comparison: VXTux 6.18.52 vs stock SPRD.

A stock vendor .ko that does not appear in the VXTux .ko list is NOT
necessarily missing - it may have been built into vmlinux (=y) instead
of as a module (=m). This tool resolves each stock module against the
VXTux kernel's own .config and the VXTux source tree, and classifies:

  MODULE       - stock .ko has a VXTux .ko of the same name
  BUILTIN      - the driver is present but compiled into vmlinux (=y)
  DISABLED     - the driver exists in the tree but its config is off
  NOT_PRESENT  - no trace of the driver in the VXTux tree (needs port)

Usage:
    python3 resolve_stock_modules.py <vxtux.config> <vxtux_module_list>
                                    <stock_modules.names> <vxtux_tree>
                                    [-o report.md]
"""
import os
import re
import subprocess
import sys

# stock module -> (config symbols to look for, source path hints)
HINTS = {
    "aes_ce_ccm": (["CONFIG_CRYPTO_AES_ARM64_CE_CCM"],
                   ["arch/arm64/crypto/aes-ce-ccm.c"]),
    "apsys_dvfs": (["CONFIG_SPRD_APSYS_DVFS"],
                   ["devfreq/sprd/apsys/"]),
    "audio_dsp_dump": (["CONFIG_SPRD_AUDIO_DSP_DUMP"],
                       ["sound/soc/sprd/audiodspdump/"]),
    "audio_pipe": (["CONFIG_SPRD_AUDIO_PIPE"],
                   ["sound/soc/sprd/audiosipc/"]),
    "audio_sipc": (["CONFIG_SPRD_AUDIO_SIPC"],
                   ["sound/soc/sprd/audiosipc/"]),
    "gpio_pmic_eic_sprd": (["CONFIG_GPIO_PMIC_EIC_SPRD"],
                           ["gpio/gpio-pmic-eic-sprd.c"]),
    "ion_ipc_trusty": (["CONFIG_SPRD_ION_IPC_TRUSTY"],
                       ["dma-buf/"]),
    "mmdvfs": (["CONFIG_SPRD_MMC_DVFS"], ["drivers/mmc/"]),
    "novatek_ts_spi": (["CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI"],
                       ["input/touchscreen/novatek_nvt_ts_spi/"]),
    "nvmem_sc27xx_efuse": (["CONFIG_NVMEM_SC27XX_EFUSE"],
                           ["nvmem/nvmem-sc27xx-efuse.c"]),
    "nvmem_sprd_cache_efuse": (["CONFIG_NVMEM_SPRD_CACHE_EFUSE"],
                               ["nvmem/"]),
    "rtc_sc27xx": (["CONFIG_RTC_SC27XX"], ["rtc/rtc-sc27xx.c"]),
    "sc27xx_adc": (["CONFIG_SPRD_SARADC_ADC"],
                   ["iio/adc/sc27xx_adc.c"]),
    "sdhci_sprd": (["CONFIG_MMC_SDHCI_SPRD"], ["mmc/host/sdhci-sprd.c"]),
    "sprd_apipe": (["CONFIG_SPRD_APIPE"], ["sound/soc/sprd/"]),
    "sprd_dmaengine_pcm": (["CONFIG_SPRD_DMAENGINE_PCM"],
                            ["sound/soc/sprd/"]),
    "sprd_flash_drv": (["CONFIG_SPRD_FLASH"], ["drivers/mtd/"]),
    "sprd_sipc_virt_bus": (["CONFIG_SPRD_SIPC_VIRT_BUS"], ["soc/sprd/sipc/"]),
    "sprd_time_sync": (["CONFIG_SPRD_TIME_SYNC"], ["drivers/soc/sprd/"]),
    "sprd_vsp_pw_domain": (["CONFIG_SPRD_VSP_PW_DOMAIN"],
                           ["drivers/soc/sprd/"]),
    "trusty": (["CONFIG_SPRD_TRUSTY"], ["drivers/soc/sprd/"]),
    "trusty_ipc": (["CONFIG_SPRD_TRUSTY"], ["drivers/soc/sprd/"]),
    "trusty_irq": (["CONFIG_SPRD_TRUSTY"], ["drivers/soc/sprd/"]),
    "trusty_tui": (["CONFIG_SPRD_TRUSTY"], ["drivers/soc/sprd/"]),
    "twofish_common": (["CONFIG_CRYPTO_TWOFISH_COMMON"], ["crypto/"]),
    "unisoc_mailbox": (["CONFIG_SPRD_MAILBOX"], ["drivers/mailbox/"]),
    "usb_f_vser": (["CONFIG_USB_F_VSER"], ["drivers/usb/gadget/"]),
    "ums512_clk": (["CONFIG_SPRD_UMS512_CLK"], ["clk/sprd/"]),
    "pwm_bl": (["CONFIG_PWM_BL"], ["video/backlight/pwm_bl.c"]),
    "mali_gondul": (["CONFIG_MALI", "CONFIG_DRM_MALI_DISPLAY"],
                    ["drivers/gpu/"]),
    "hxchipset_83102e": (["CONFIG_TOUCHSCREEN_HIMAX_HX83102E"],
                         ["input/touchscreen/sprd/hx83102e/"]),
    "sprd_cpufreq_public": (["CONFIG_SPRD_CPUFREQ"], ["drivers/cpufreq/"]),
    "sprd_hwdvfs_policy": (["CONFIG_SPRD_HWDVFS"], ["drivers/cpufreq/"]),
    "sprd_hwdvfs_ums512_arch": (["CONFIG_SPRD_HWDVFS"],
                                ["drivers/cpufreq/"]),
    "sprd_ddr_dvfs": (["CONFIG_SPRD_DDR_DVFS"], ["drivers/devfreq/"]),
    "sprd_gpu_cooling": (["CONFIG_SPRD_GPU_COOLING"], ["drivers/thermal/"]),
    "sprd_sc27xx_spi": (["CONFIG_SPI_SPRD"], ["drivers/spi/"]),
    "sprd_top_dvfs": (["CONFIG_SPRD_TOP_DVFS"], ["drivers/devfreq/"]),
    "sc2730_regulator": (["CONFIG_REGULATOR_SC2730"],
                         ["regulator/sc2730-regulator.c"]),
}


def load_config(path):
    cfg = {}
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if line.startswith("# CONFIG_") and line.endswith(" is not set"):
                cfg[line[2:line.index(" is not set")].strip()] = "n"
            elif line and not line.startswith("#") and "=" in line:
                k, v = line.split("=", 1)
                cfg[k.strip()] = v.strip()
    return cfg


def load_names(path):
    out = set()
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if line:
                out.add(os.path.basename(line).replace(".ko", "")
                        .replace("-", "_"))
    return out


def find_in_tree(tree, name):
    """Look for the driver's source in the VXTux tree."""
    stems = [name.replace("_", "-"), name]
    hits = []
    for root, dirs, files in os.walk(tree):
        # skip huge/irrelevant dirs
        rel = os.path.relpath(root, tree)
        if any(p in rel for p in (".git", "io_uring.disabled",
                                  "Documentation", "tools", "scripts")):
            dirs[:] = []
            continue
        for fn in files:
            if not fn.endswith(".c"):
                continue
            stem = fn[:-2]
            if stem in stems or stem.replace("-", "_") == name:
                hits.append(os.path.join(rel, fn))
        if len(hits) > 4:
            break
    return hits


def main():
    if len(sys.argv) < 5:
        print(__doc__)
        return
    cfgpath, modlist, stocknames, tree = sys.argv[1:5]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    cfg = load_config(cfgpath)
    vmods = load_names(modlist)
    smods = load_names(stocknames)

    rows = []
    for s in sorted(smods):
        hints, paths = HINTS.get(s, ([], []))
        vals = {h: cfg.get(h) for h in hints}
        present = [v for v in vals.values() if v and v != "n"]
        src = []
        for p in paths:
            full = os.path.join(tree, p)
            if os.path.isdir(full):
                src.extend(find_in_tree(tree, os.path.basename(p.rstrip("/"))))
            elif os.path.exists(full):
                src.append(p)
        if s in vmods:
            status = "MODULE"
        elif present:
            status = "BUILTIN"
        elif src:
            status = "DISABLED"
        else:
            status = "NOT_PRESENT"
        rows.append((s, status, vals, src))

    counts = {}
    for _, st, _, _ in rows:
        counts[st] = counts.get(st, 0) + 1

    print("=" * 78)
    print("STOCK MODULE RESOLUTION (config-aware)")
    print(f"  vxtux .ko built : {len(vmods)}")
    print(f"  stock .ko       : {len(smods)}")
    for st in ("MODULE", "BUILTIN", "DISABLED", "NOT_PRESENT"):
        print(f"  {st:<13} : {counts.get(st, 0)}")

    print()
    for st in ("NOT_PRESENT", "DISABLED", "BUILTIN", "MODULE"):
        sel = [r for r in rows if r[1] == st]
        if not sel:
            continue
        print(f"--- {st} ({len(sel)}) ---")
        for s, _, vals, src in sel:
            vs = ", ".join(f"{k}={v}" for k, v in vals.items() if v)
            sp = ", ".join(src[:2]) if src else ""
            print(f"  {s:<26} {vs:<44} {sp[:60]}")
        print()

    L = ["# Stock Vendor Module Resolution — VXTux 6.18.52\n"]
    L.append(f"- VXTux modules built: {len(vmods)}")
    L.append(f"- Stock vendor modules: {len(smods)}")
    for st in ("MODULE", "BUILTIN", "DISABLED", "NOT_PRESENT"):
        L.append(f"- {st}: {counts.get(st, 0)}")
    L.append("")
    for st in ("NOT_PRESENT", "DISABLED", "BUILTIN", "MODULE"):
        sel = [r for r in rows if r[1] == st]
        if not sel:
            continue
        L.append(f"## {st} ({len(sel)})\n")
        L.append("| stock module | VXTux config | source in VXTux tree |")
        L.append("|---|---|---|")
        for s, _, vals, src in sel:
            vs = ", ".join(f"`{k}={v}`" for k, v in vals.items() if v) or "—"
            sp = ", ".join(f"`{x}`" for x in src[:2]) or "—"
            L.append(f"| `{s}` | {vs} | {sp} |")
        L.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(L))
        print(f"wrote {outpath}")


if __name__ == "__main__":
    main()
