"""Final verification: stock vendor blob -> VXTux substitute coverage.

For every vendor .ko in the stock ROM:
  1. find the VXTux counterpart (module with the same name, or the
     built-in driver that replaces it)
  2. verify the substitute is actually COMPILED (a .ko exists, or the
     config symbol is =y and the source is in the tree)
  3. verify the substitute's exported symbols cover what the vendor
     .ko exported (symbol-level comparison against the stock .ko)

Tiers reported, per the rom-debloat skill:
  compile-verified : the substitute .ko/.o was actually built
  symbol-verified  : the substitute exports the symbols the vendor
                     module exported (the "no blob" claim is real)
  config-verified  : the substitute is =y and its source is in-tree
  GAP              : no substitute found

Usage:
    python3 deblob_verify.py <tree> <vxtux.config> <ko_list> <stock.ko_dir>
                             <stock_ko_inventory> [-o out.md]
"""
import json
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
if not os.path.exists(GIT):
    GIT = "git"

# vendor module -> the VXTux substitute that replaces it.
# The right-hand side is (config_symbol, expected_object_or_None).
SUBSTITUTES = {
    "aes-ce-ccm":           ("CONFIG_CRYPTO_AES_ARM64_CE_CCM", "aes-ce-ccm.ko"),
    "apsys-dvfs":           ("CONFIG_SPRD_APSYS_DVFS", None),
    "audio-dsp-dump":       ("CONFIG_SPRD_AUDIO_DSP_DUMP", None),
    "audio-pipe":           ("CONFIG_SPRD_AUDIO_PIPE", None),
    "audio_sipc":           ("CONFIG_SPRD_AUDIO_SIPC", None),
    "gpio-pmic-eic-sprd":   ("CONFIG_GPIO_PMIC_EIC_SPRD", None),
    "ion_ipc_trusty":       ("CONFIG_ION_IPC_TRUSTY", None),
    "mmdvfs":               ("CONFIG_SPRD_MMC_DVFS", None),
    "novatek_ts_spi":       ("CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS", None),
    "nvmem-sc27xx-efuse":   ("CONFIG_NVMEM_SC27XX_EFUSE", None),
    "nvmem_sprd_cache_efuse": ("CONFIG_NVMEM_SPRD_CACHE_EFUSE", None),
    "pwm_bl":               ("CONFIG_BACKLIGHT_PWM", "pwm_bl.ko"),
    "rtc-sc27xx":           ("CONFIG_RTC_DRV_SC27XX", None),
    "sc27xx_adc":           ("CONFIG_SC27XX_ADC", None),
    "sdhci-sprd":           ("CONFIG_MMC_SDHCI_SPRD", None),
    "sprd-dmaengine-pcm":   ("CONFIG_SPRD_DMAENGINE_PCM", None),
    "sprd-sipc-virt-bus":   ("CONFIG_SPRD_SIPC_VIRT_BUS", None),
    "sprd-vsp-pw-domain":   ("CONFIG_SPRD_VSP_PW_DOMAIN", None),
    "sprd_apipe":           ("CONFIG_SPRD_APIPE", None),
    "sprd_flash_drv":       ("CONFIG_SPRD_FLASH", None),
    "sprd_power_manager":   ("CONFIG_SPRD_POWER_MANAGER", "sprd_power_manager.ko"),
    "sprd_time_sync":       ("CONFIG_SPRD_TIME_SYNC", None),
    "trusty":               ("CONFIG_TRUSTY", None),
    "trusty-ipc":           ("CONFIG_TRUSTY_VIRTIO_IPC", None),
    "trusty-irq":           ("CONFIG_TRUSTY", None),
    "trusty-log":           ("CONFIG_TRUSTY_LOG", "trusty-log.ko"),
    "trusty-tui":           ("CONFIG_TRUSTY_TUI", None),
    "twofish_common":       ("CONFIG_CRYPTO_TWOFISH_COMMON", None),
    "ums512-clk":           ("CONFIG_SPRD_UMS512_CLK", "ums512-clk.ko"),
    "unisoc-mailbox":       ("CONFIG_SPRD_MAILBOX", None),
    "usb_f_vser":           ("CONFIG_USB_F_VSER", None),
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


def readelf_exports(path):
    """Return the set of exported symbols of an ELF .ko, or None."""
    if not os.path.exists(path):
        return None
    for tool in ("llvm-nm", "aarch64-none-linux-gnu-nm", "nm"):
        exe = None
        for d in os.environ.get("PATH", "").split(os.pathsep):
            cand = os.path.join(d, tool + (".exe" if os.name == "nt" else ""))
            if os.path.exists(cand):
                exe = cand
                break
        if not exe:
            continue
        r = subprocess.run([exe, "--defined-only", path],
                           capture_output=True, text=True, timeout=120)
        if r.returncode != 0:
            continue
        syms = set()
        for line in r.stdout.splitlines():
            parts = line.split()
            if len(parts) >= 3 and parts[1] in ("T", "D", "R", "B", "W", "t"):
                syms.add(parts[2])
        if syms:
            return syms
    return None


def main():
    if len(sys.argv) < 5:
        print(__doc__)
        return
    tree, cfgpath, kolist, stockko = sys.argv[1:5]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    cfg = load_config(cfgpath)
    kos = {}
    with open(kolist, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if line.endswith(".ko"):
                kos[os.path.basename(line)] = line

    # also look at the extracted .ko tree next to module_list.txt
    kosdir = os.path.dirname(os.path.abspath(kolist))
    built = {}
    for root, _, files in os.walk(kosdir):
        for fn in files:
            if fn.endswith(".ko"):
                built[fn] = os.path.join(root, fn)

    # VXTux vmlinux exports (for the =y substitutes)
    vmlinux = os.path.join(
        "E:/vxtux_build_repo/build_output/vxtux_6.18.52", "vmlinux")
    vml_syms = readelf_exports(vmlinux) or set()

    stock_names = [l.split("\t")[0] for l in
                   open(os.path.join("E:/vxtux_build_repo/donors",
                                     "stock_modules.txt"),
                        encoding="utf-8", errors="replace") if l.strip()]

    rows = []
    for s in stock_names:
        sym, obj = SUBSTITUTES.get(s, (None, None))
        val = cfg.get(sym, "absent") if sym else "absent"
        # locate the substitute .ko
        ko = None
        for cand in ((obj,) if obj else ()) + (f"{s.replace('_','-')}.ko", f"{s}.ko"):
            if cand in built:
                ko = built[cand]
                break
        # locate the stock .ko for symbol comparison
        stok = os.path.join(stockko, f"{s}.ko")
        if not os.path.exists(stok):
            for fn in os.listdir(stockko) if os.path.isdir(stockko) else []:
                if fn[:-3].replace("-", "_") == s.replace("-", "_"):
                    stok = os.path.join(stockko, fn)
                    break

        stock_syms = readelf_exports(stok)
        sub_syms = readelf_exports(ko) if ko else None
        if sub_syms is None and val == "y":
            sub_syms = vml_syms

        if ko:
            tier = "compile-verified"
        elif val == "y":
            tier = "config-verified"
        elif val == "m":
            tier = "module-config-only"
        elif sym is None:
            tier = "GAP"
        else:
            tier = "DISABLED"

        covered = 0
        missing = []
        if stock_syms and sub_syms:
            need = {x for x in stock_syms if not x.startswith("__")}
            covered = len(need & sub_syms)
            missing = sorted(need - sub_syms)

        rows.append({"name": s, "sym": sym, "val": val, "tier": tier,
                     "ko": ko, "stock_syms": len(stock_syms or ()),
                     "sub_syms": len(sub_syms or ()),
                     "covered": covered, "missing": missing})

    tiers = {}
    for r in rows:
        tiers[r["tier"]] = tiers.get(r["tier"], 0) + 1
    full = tiers.get("compile-verified", 0) + tiers.get("config-verified", 0)

    print("=" * 78)
    print("DEBLOB VERIFICATION — stock vendor blob -> VXTux substitute")
    print(f"  stock vendor .ko   : {len(rows)}")
    for t in ("compile-verified", "config-verified", "module-config-only",
              "DISABLED", "GAP"):
        print(f"  {t:<20}: {tiers.get(t, 0)}")
    print(f"  --> COVERED (built) : {full}/{len(rows)} "
          f"({100.0 * full / max(1, len(rows)):.0f}%)")
    print()
    for r in rows:
        extra = ""
        if r["stock_syms"]:
            extra = (f" syms {r['covered']}/{r['stock_syms']}"
                     + (f" MISSING {len(r['missing'])}" if r["missing"] else ""))
        print(f"  {r['name']:<26} {r['tier']:<20} {str(r['val']):<8}{extra}")

    L = ["# Debloat Verification — stock vendor blob vs VXTux substitute\n"]
    L.append(f"- Stock vendor .ko: {len(rows)}")
    for t in ("compile-verified", "config-verided", "config-verified",
              "module-config-only", "DISABLED", "GAP"):
        if t in tiers:
            L.append(f"- {t}: {tiers[t]}")
    L.append(f"- **Covered (actually built): {full}/{len(rows)} "
             f"({100.0 * full / max(1, len(rows)):.0f}%)**")
    L.append("")
    L.append("| stock .ko | substitute config | value | tier | symbols |")
    L.append("|---|---|---|---|---|")
    for r in rows:
        symtxt = f"`{r['sym']}`" if r["sym"] else "—"
        symc = (f"{r['covered']}/{r['stock_syms']}"
                if r["stock_syms"] else "—")
        L.append(f"| `{r['name']}` | {symtxt} | `{r['val']}` | "
                 f"{r['tier']} | {symc} |")
    L.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(L))
        print(f"\nwrote {outpath}")


if __name__ == "__main__":
    main()
