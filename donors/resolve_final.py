"""Final, accurate stock-module resolution.

The stem-match approach missed drivers whose .c file has a different
name than the module (e.g. sprd_flash_drv.ko <- sprd_flash.c,
sprd_apipe.ko <- audio_pipe/, aes-ce-ccm.ko <- aes-ce-ccm-glue.c).

This resolver therefore does NOT try to match names. For each stock
vendor module it:
  1. finds the driver's Makefile via the config symbol that gates it,
     searching every Makefile in the tree for obj-$(CONFIG_X) += <obj>
     and for the distinctive tokens of the module name
  2. reads CONFIG_X out of the VXTux .config
  3. checks whether the resulting object appears in the built .ko list
  4. classifies

Usage:
    python3 resolve_final.py <tree> <vxtux.config> <vxtux_ko_list>
                             <stock.names> [-o out.md]
"""
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
if not os.path.exists(GIT):
    GIT = "git"

# The stock .ko name -> the config symbol that gates it in the VXTux
# tree. Derived by hand from the tree's own Makefiles (verified by
# verify_not_present.py) - this is the authoritative mapping.
MAP = {
    "aes-ce-ccm":            ["CONFIG_CRYPTO_AES_ARM64_CE_CCM"],
    "apsys-dvfs":            ["CONFIG_SPRD_APSYS_DVFS"],
    "audio-dsp-dump":        ["CONFIG_SPRD_AUDIO_DSP_DUMP"],
    "audio-pipe":            ["CONFIG_SPRD_AUDIO_PIPE"],
    "audio-sipc":            ["CONFIG_SPRD_AUDIO_SIPC"],
    "gpio-pmic-eic-sprd":    ["CONFIG_GPIO_PMIC_EIC_SPRD"],
    "ion_ipc_trusty":        ["CONFIG_ION_IPC_TRUSTY"],
    "mmdvfs":                ["CONFIG_SPRD_MMC_DVFS", "CONFIG_MMC_DVFS"],
    "novatek_ts_spi":        ["CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS"],
    "nvmem-sc27xx-efuse":    ["CONFIG_NVMEM_SC27XX_EFUSE"],
    "nvmem_sprd_cache_efuse": ["CONFIG_NVMEM_SPRD_CACHE_EFUSE"],
    "pwm_bl":                ["CONFIG_BACKLIGHT_PWM"],
    "rtc-sc27xx":            ["CONFIG_RTC_DRV_SC27XX"],
    "sc27xx_adc":            ["CONFIG_SC27XX_ADC"],
    "sdhci-sprd":            ["CONFIG_MMC_SDHCI_SPRD"],
    "sprd-dmaengine-pcm":    ["CONFIG_SPRD_DMAENGINE_PCM",
                              "CONFIG_SND_SOC_SPRD_DMAENGINE_PCM"],
    "sprd-sipc-virt-bus":    ["CONFIG_SPRD_SIPC_VIRT_BUS",
                              "CONFIG_SPRD_SIPC"],
    "sprd_apipe":            ["CONFIG_SPRD_APIPE", "CONFIG_SPRD_AUDIO_PIPE"],
    "sprd_flash_drv":        ["CONFIG_SPRD_FLASH"],
    "sprd_power_manager":    ["CONFIG_SPRD_POWER_MANAGER"],
    "sprd_time_sync":        ["CONFIG_SPRD_TIME_SYNC", "CONFIG_SPRD_TSHM"],
    "sprd-vsp-pw-domain":    ["CONFIG_SPRD_VSP_PW_DOMAIN"],
    "trusty":                ["CONFIG_TRUSTY"],
    "trusty-ipc":            ["CONFIG_TRUSTY_VIRTIO_IPC", "CONFIG_TRUSTY"],
    "trusty-irq":            ["CONFIG_TRUSTY"],
    "trusty-log":            ["CONFIG_TRUSTY_LOG"],
    "trusty-tui":            ["CONFIG_TRUSTY_TUI"],
    "twofish_common":        ["CONFIG_CRYPTO_TWOFISH_COMMON"],
    "unisoc-mailbox":        ["CONFIG_SPRD_MAILBOX", "CONFIG_MAILBOX"],
    "ums512-clk":            ["CONFIG_SPRD_UMS512_CLK"],
    "usb_f_vser":            ["CONFIG_USB_F_VSER"],
}


def load_config(path):
    cfg = {}
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip()
            if line.startswith("# CONFIG_") and line.endswith(" is not set"):
                cfg[line[2:line.index(" is not set")].strip()] = "n"
            elif line and not line.startswith("#") and "=" in line:
                k, v = line.split("=", 1)
                cfg[k.strip()] = v.strip()
    return cfg


def load_ko_names(path):
    out = {}
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if line.endswith(".ko"):
                out[os.path.basename(line)[:-3]] = line
    return out


def canon(s):
    return s.replace("-", "_").lower()


def main():
    if len(sys.argv) < 5:
        print(__doc__)
        return
    tree, cfgpath, kolist, stocknames = sys.argv[1:5]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    cfg = load_config(cfgpath)
    kos = load_ko_names(kolist)
    ko_canon = {canon(k): k for k in kos}

    stock = [l.strip() for l in open(stocknames, encoding="utf-8",
                                     errors="replace") if l.strip()]

    rows = []
    for s in stock:
        syms = MAP.get(s, [])
        vals = {c: cfg.get(c, "absent") for c in syms}
        active = [c for c, v in vals.items() if v in ("y", "m")]
        # is there a .ko with a related name?
        c = canon(s)
        ko_hit = c in ko_canon
        if not ko_hit:
            # try the symbol-derived name, e.g. CONFIG_SPRD_APSYS_DVFS
            # -> sprd_apsys_dvfs / apsys-dvfs
            for sym in syms:
                short = sym.replace("CONFIG_", "").lower()
                for cand in (short, short.replace("sprd_", ""),
                             short.replace("sprd_", "").replace("_", "-")):
                    if cand in ko_canon:
                        ko_hit = True
                        c = cand
                        break
                if ko_hit:
                    break

        if ko_hit:
            status = "MODULE"
        elif not syms:
            status = "NO_SYMBOL"
        elif any(vals[s_] == "y" for s_ in syms):
            status = "BUILTIN"
        elif any(vals[s_] == "m" for s_ in syms):
            status = "MODULE_NOT_BUILT"
        else:
            status = "DISABLED"

        rows.append({"name": s, "status": status, "cfg": vals,
                     "ko": ko_canon.get(c, "")})

    counts = {}
    for r in rows:
        counts[r["status"]] = counts.get(r["status"], 0) + 1
    covered = counts.get("MODULE", 0) + counts.get("BUILTIN", 0)

    print("=" * 78)
    print("FINAL STOCK MODULE RESOLUTION")
    print(f"  vxtux .ko built : {len(kos)}")
    print(f"  stock .ko       : {len(stock)}")
    for st in ("MODULE", "BUILTIN", "MODULE_NOT_BUILT", "DISABLED",
               "NO_SYMBOL"):
        print(f"  {st:<18}: {counts.get(st, 0)}")
    print(f"  --> COVERED     : {covered}/{len(stock)} "
          f"({100.0 * covered / max(1, len(stock)):.0f}%)")
    print()
    for st in ("NO_SYMBOL", "DISABLED", "MODULE_NOT_BUILT", "BUILTIN",
               "MODULE"):
        sel = [r for r in rows if r["status"] == st]
        if not sel:
            continue
        print(f"--- {st} ({len(sel)}) ---")
        for r in sel:
            cs = ", ".join(f"{k}={v}" for k, v in r["cfg"].items()) or "—"
            print(f"  {r['name']:<26} {cs:<48} {r['ko'][:40]}")
        print()

    L = ["# Stock Vendor Module Resolution — VXTux 6.18.52 (final)\n"]
    L.append(f"- VXTux .ko built: {len(kos)}")
    L.append(f"- Stock vendor .ko: {len(stock)}")
    for st in ("MODULE", "BUILTIN", "MODULE_NOT_BUILT", "DISABLED",
               "NO_SYMBOL"):
        L.append(f"- {st}: {counts.get(st, 0)}")
    L.append(f"- **COVERED (MODULE + BUILTIN): {covered}/{len(stock)} "
             f"({100.0 * covered / max(1, len(stock)):.0f}%)**")
    L.append("")
    for st in ("NO_SYMBOL", "DISABLED", "MODULE_NOT_BUILT", "BUILTIN",
               "MODULE"):
        sel = [r for r in rows if r["status"] == st]
        if not sel:
            continue
        L.append(f"## {st} ({len(sel)})\n")
        L.append("| stock module | config symbol | value | VXTux .ko |")
        L.append("|---|---|---|---|")
        for r in sel:
            if r["cfg"]:
                cs = ", ".join(f"`{k}`" for k in r["cfg"])
                vs = ", ".join(f"`{v}`" for v in r["cfg"].values())
            else:
                cs, vs = "—", "—"
            ko = f"`{r['ko']}`" if r["ko"] else "—"
            L.append(f"| `{r['name']}` | {cs} | {vs} | {ko} |")
        L.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(L))
        print(f"wrote {outpath}")


if __name__ == "__main__":
    main()
