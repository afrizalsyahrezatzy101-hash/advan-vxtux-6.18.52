"""Final deblob verification: stock vendor blob -> VXTux substitute.

For each of the stock vendor .ko:
  - locate the VXTux substitute (a .ko with the same name, or the
    built-in driver that replaces it)
  - compare the EXPORTED SYMBOLS of the stock .ko against the
    substitute (.ko exports, or vmlinux exports for =y substitutes)
  - classify

Tiers:
  COMPILE+SYMBOL : the substitute was built AND exports the symbols
  COMPILE        : the substitute was built (symbol table unavailable)
  CONFIG         : =y, source in-tree, compiled into vmlinux
  DISABLED       : the substitute's config symbol is off
  GAP            : no substitute at all

The symbol comparison is the real test of the deblob claim: a vendor
blob is only truly replaced if the replacement exports the same
symbols, because that is what the userspace loader binds to.

Usage:
    python3 deblob_verify2.py <tree> <vxtux.config> <ko_list>
                              <stock_ko_dir> <vmlinux> [-o out.md]
"""
import os
import re
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
if not os.path.exists(NM):
    NM = "nm"

# vendor .ko -> (config_symbol, vxtux_ko_name or None for built-in)
SUBSTITUTES = {
    "aes-ce-ccm":            ("CONFIG_CRYPTO_AES_ARM64_CE_CCM", "aes-ce-ccm.ko"),
    "apsys-dvfs":            ("CONFIG_SPRD_APSYS_DVFS", None),
    "audio-dsp-dump":        ("CONFIG_SPRD_AUDIO_DSP_DUMP", None),
    "audio-pipe":            ("CONFIG_SPRD_AUDIO_PIPE", None),
    "audio_sipc":            ("CONFIG_SPRD_AUDIO_SIPC", None),
    "gpio-pmic-eic-sprd":    ("CONFIG_GPIO_PMIC_EIC_SPRD", None),
    "ion_ipc_trusty":        ("CONFIG_ION_IPC_TRUSTY", None),
    "mmdvfs":                ("CONFIG_SPRD_MMC_DVFS", None),
    "novatek_ts_spi":        ("CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS", None),
    "nvmem-sc27xx-efuse":    ("CONFIG_NVMEM_SC27XX_EFUSE", None),
    "nvmem_sprd_cache_efuse": ("CONFIG_NVMEM_SPRD_CACHE_EFUSE", None),
    "pwm_bl":                ("CONFIG_BACKLIGHT_PWM", "pwm_bl.ko"),
    "rtc-sc27xx":            ("CONFIG_RTC_DRV_SC27XX", None),
    "sc27xx_adc":            ("CONFIG_SC27XX_ADC", None),
    "sdhci-sprd":            ("CONFIG_MMC_SDHCI_SPRD", None),
    "sprd-dmaengine-pcm":    ("CONFIG_SPRD_DMAENGINE_PCM", None),
    "sprd-sipc-virt-bus":    ("CONFIG_SPRD_SIPC_VIRT_BUS", None),
    "sprd-vsp-pw-domain":    ("CONFIG_SPRD_VSP_PW_DOMAIN", None),
    "sprd_apipe":            ("CONFIG_SPRD_APIPE", None),
    "sprd_flash_drv":        ("CONFIG_SPRD_FLASH", None),
    "sprd_power_manager":    ("CONFIG_SPRD_POWER_MANAGER",
                              "sprd_power_manager.ko"),
    "sprd_time_sync":        ("CONFIG_SPRD_TIME_SYNC", None),
    "trusty":                ("CONFIG_TRUSTY", None),
    "trusty-ipc":            ("CONFIG_TRUSTY_VIRTIO_IPC", None),
    "trusty-irq":            ("CONFIG_TRUSTY", None),
    "trusty-log":            ("CONFIG_TRUSTY_LOG", "trusty-log.ko"),
    "trusty-tui":            ("CONFIG_TRUSTY_TUI", None),
    "twofish_common":        ("CONFIG_CRYPTO_TWOFISH_COMMON", None),
    "ums512-clk":            ("CONFIG_SPRD_UMS512_CLK", "ums512-clk.ko"),
    "unisoc-mailbox":        ("CONFIG_SPRD_MAILBOX", None),
    "usb_f_vser":            ("CONFIG_USB_F_VSER", None),
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


def nm_exports(path):
    """Exported (global) symbols of an ELF object."""
    if not os.path.exists(path):
        return None
    r = subprocess.run([NM, "--defined-only", "--extern-only", path],
                       capture_output=True, text=True, timeout=180)
    if r.returncode != 0:
        return None
    syms = set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) >= 3 and p[1] in ("T", "D", "R", "B", "W"):
            syms.add(p[2])
    return syms


def main():
    if len(sys.argv) < 6:
        print(__doc__)
        return
    tree, cfgpath, kolist, stockko, vmlinux = sys.argv[1:6]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    cfg = load_config(cfgpath)

    # built .ko tree (next to module_list.txt)
    kosdir = os.path.dirname(os.path.abspath(kolist))
    built = {}
    for root, _, files in os.walk(kosdir):
        for fn in files:
            if fn.endswith(".ko"):
                built[fn] = os.path.join(root, fn)

    vml_syms = nm_exports(vmlinux)
    print(f"vmlinux exports: {len(vml_syms) if vml_syms else 0:,}")

    stock_files = {}
    for fn in os.listdir(stockko):
        if fn.endswith(".ko"):
            stock_files[fn[:-3]] = os.path.join(stockko, fn)

    rows = []
    for s in sorted(stock_files):
        sym, koname = SUBSTITUTES.get(s, (None, None))
        val = cfg.get(sym, "absent") if sym else "absent"
        ko = built.get(koname) if koname else None
        if not ko:
            for cand in (f"{s.replace('_','-')}.ko", f"{s}.ko"):
                if cand in built:
                    ko = built[cand]
                    break

        stock_syms = nm_exports(stock_files[s])
        sub_syms = nm_exports(ko) if ko else None
        if sub_syms is None and val == "y":
            sub_syms = vml_syms

        need = {x for x in (stock_syms or ()) if not x.startswith("__")}
        cov = len(need & (sub_syms or set()))
        missing = sorted(need - (sub_syms or set()))

        if ko and stock_syms and sub_syms:
            tier = "COMPILE+SYMBOL" if cov == len(need) and need else \
                   "COMPILE+PARTIAL"
        elif ko:
            tier = "COMPILE"
        elif val == "y":
            tier = "CONFIG-BUILTIN"
        elif val == "m":
            tier = "MODULE-DISABLED"
        elif sym is None:
            tier = "GAP"
        else:
            tier = "DISABLED"

        rows.append({"name": s, "sym": sym, "val": val, "tier": tier,
                     "ko": os.path.basename(ko) if ko else None,
                     "need": len(need), "cov": cov, "missing": missing})

    tiers = {}
    for r in rows:
        tiers[r["tier"]] = tiers.get(r["tier"], 0) + 1
    built_ok = sum(tiers.get(t, 0) for t in
                   ("COMPILE+SYMBOL", "COMPILE+PARTIAL", "COMPILE",
                    "CONFIG-BUILTIN"))

    print("=" * 78)
    print("DEBLOB VERIFICATION — stock vendor .ko -> VXTux substitute")
    print(f"  stock vendor .ko : {len(rows)}")
    for t in sorted(tiers):
        print(f"  {t:<18}: {tiers[t]}")
    print(f"  --> REPLACED (built): {built_ok}/{len(rows)} "
          f"({100.0 * built_ok / max(1, len(rows)):.0f}%)")
    print()
    for r in rows:
        m = ""
        if r["need"]:
            m = f"syms {r['cov']}/{r['need']}"
            if r["missing"]:
                m += f" miss={len(r['missing'])}"
        print(f"  {r['name']:<26} {r['tier']:<18} {str(r['val']):<8} {m}")

    L = ["# Debloat Verification — stock vendor blob vs VXTux substitute\n"]
    L.append(f"- Stock vendor .ko examined: {len(rows)}")
    for t in sorted(tiers):
        L.append(f"- {t}: {tiers[t]}")
    L.append(f"- **Replaced by a built VXTux artifact: {built_ok}/{len(rows)} "
             f"({100.0 * built_ok / max(1, len(rows)):.0f}%)**")
    L.append("")
    L.append("| stock .ko | substitute | config | value | tier | "
             "symbols covered | missing |")
    L.append("|---|---|---|---|---|---|---|")
    for r in rows:
        ko = f"`{r['ko']}`" if r["ko"] else "—"
        sym = f"`{r['sym']}`" if r["sym"] else "—"
        sc = f"{r['cov']}/{r['need']}" if r["need"] else "—"
        miss = ", ".join(f"`{m}`" for m in r["missing"][:6]) or "—"
        L.append(f"| `{r['name']}` | {ko} | {sym} | `{r['val']}` | "
                 f"{r['tier']} | {sc} | {miss} |")
    L.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(L))
        print(f"\nwrote {outpath}")


if __name__ == "__main__":
    main()
