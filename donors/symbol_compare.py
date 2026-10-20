"""Honest symbol-gap analysis.

Two corrections over the naive comparison:

1. `init_module` / `cleanup_module` are MODULE LIFECYCLE artefacts.
   A driver built into vmlinux has no such symbols (it is registered
   at boot by the initcall machinery). Their absence is NOT a gap -
   it is the expected consequence of =y instead of =m. They are
   excluded from the comparison.

2. The comparison must be against the substitute that the CONFIG
   actually produces:
      =m -> the built .ko's exports
      =y -> the symbols vmlinux exports
   For =y substitutes, the VXTux source must carry EXPORT_SYMBOL() for
   the symbols the vendor .ko exported, otherwise userspace that
   dlopen'd/linked the vendor blob cannot bind to them.

Output per stock .ko:
   real_missing : symbols the vendor .ko exported that the substitute
                  does not provide
   in_source    : of those, how many exist in the VXTux source but are
                  simply not EXPORT_SYMBOL'd (fixable with one line)
   absent       : how many do not exist in the VXTux source at all
                  (a real port gap)
"""
import os
import re
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
LIFECYCLE = {"init_module", "cleanup_module"}

ROOT = "E:/vxtux_build_repo"
TREE = os.path.join(ROOT, "6.18.52-VXTux-UMS512")
VML = os.path.join(ROOT, "build_output/vxtux_6.18.52/vmlinux")
STOCK = os.path.join(ROOT, "donors/stock_vendor_modules")
BUILT = os.path.join(ROOT, "build_output/vxtux_6.18.52")

# stock .ko -> (config symbol, source file to grep)
MAP = {
    "aes-ce-ccm":             ("CONFIG_CRYPTO_AES_ARM64_CE_CCM",
                              "arch/arm64/crypto/aes-ce-ccm-glue.c"),
    "gpio-pmic-eic-sprd":     ("CONFIG_GPIO_PMIC_EIC_SPRD",
                              "drivers/gpio/gpio-pmic-eic-sprd.c"),
    "ion_ipc_trusty":         ("CONFIG_ION_IPC_TRUSTY",
                              "drivers/dma-buf/sprd/ion_ipc_trusty.c"),
    "nvmem-sc27xx-efuse":     ("CONFIG_NVMEM_SC27XX_EFUSE",
                              "drivers/nvmem/nvmem-sc27xx-efuse.c"),
    "nvmem_sprd_cache_efuse": ("CONFIG_NVMEM_SPRD_CACHE_EFUSE",
                              "drivers/soc/sprd/cache_efuse/sprd_cache_efuse.c"),
    "rtc-sc27xx":             ("CONFIG_RTC_DRV_SC27XX",
                              "drivers/rtc/rtc-sc27xx.c"),
    "sc27xx_adc":             ("CONFIG_SC27XX_ADC",
                              "drivers/iio/adc/sc27xx_adc.c"),
    "sdhci-sprd":             ("CONFIG_MMC_SDHCI_SPRD",
                              "drivers/mmc/host/sdhci-sprd.c"),
    "sprd-vsp-pw-domain":     ("CONFIG_SPRD_VSP_PW_DOMAIN",
                              "drivers/pmdomain/sprd/sprd_vsp_pw_domain.c"),
    "trusty":                 ("CONFIG_TRUSTY", "drivers/trusty/trusty.c"),
    "trusty-ipc":             ("CONFIG_TRUSTY_VIRTIO_IPC",
                              "drivers/trusty/trusty-ipc.c"),
    "trusty-irq":             ("CONFIG_TRUSTY",
                              "drivers/trusty/trusty-irq.c"),
    "trusty-tui":             ("CONFIG_TRUSTY_TUI",
                              "drivers/misc/trusty-tui.c"),
    "twofish_common":         ("CONFIG_CRYPTO_TWOFISH_COMMON",
                              "crypto/twofish_common.c"),
    "unisoc-mailbox":         ("CONFIG_SPRD_MAILBOX",
                              "drivers/mailbox/platform/sprd/mbox.c"),
    "pwm_bl":                 ("CONFIG_BACKLIGHT_PWM",
                              "drivers/video/backlight/pwm_bl.c"),
    "sprd_power_manager":     ("CONFIG_SPRD_POWER_MANAGER",
                              "drivers/soc/sprd/power_manager/sprd_power_manager.c"),
    "trusty-log":             ("CONFIG_TRUSTY_LOG",
                              "drivers/trusty/trusty-log.c"),
    "ums512-clk":             ("CONFIG_SPRD_UMS512_CLK",
                              "drivers/clk/sprd/ums512-clk.c"),
    "novatek_ts_spi":         ("CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS",
                              "drivers/input/touchscreen/novatek_nvt_ts_spi/novatek_nvt_ts_spi.c"),
}


def nm(path):
    if not os.path.exists(path):
        return None
    r = subprocess.run([NM, "--defined-only", "--extern-only", path],
                       capture_output=True, text=True, timeout=180)
    if r.returncode != 0:
        return None
    out = set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) >= 3 and p[1] in ("T", "D", "R", "B", "W"):
            out.add(p[2])
    return out


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


def grep_source(rel, syms):
    """Which of syms appear in the VXTux source file (as a definition)?"""
    p = os.path.join(TREE, rel)
    if not os.path.exists(p):
        # try a directory of sources
        d = os.path.dirname(rel)
        if not os.path.isdir(os.path.join(TREE, d)):
            return set(), set()
        txt = ""
        for fn in os.listdir(os.path.join(TREE, d)):
            if fn.endswith(".c") or fn.endswith(".h"):
                try:
                    txt += open(os.path.join(TREE, d, fn), "r",
                                encoding="utf-8", errors="replace").read()
                except OSError:
                    pass
    else:
        txt = open(p, "r", encoding="utf-8", errors="replace").read()
    in_src, exported = set(), set()
    for s in syms:
        if re.search(r"\b" + re.escape(s) + r"\b", txt):
            in_src.add(s)
            if re.search(r"EXPORT_SYMBOL(_GPL)?\s*\(\s*" +
                         re.escape(s) + r"\s*\)", txt):
                exported.add(s)
    return in_src, exported


def main():
    cfg = load_config(os.path.join(TREE, ".config"))
    vml = nm(VML) or set()

    built = {}
    for root, _, files in os.walk(BUILT):
        for fn in files:
            if fn.endswith(".ko"):
                built[fn] = os.path.join(root, fn)

    stock_files = {}
    for fn in os.listdir(STOCK):
        if fn.endswith(".ko"):
            stock_files[fn[:-3]] = os.path.join(STOCK, fn)

    rows = []
    for s in sorted(stock_files):
        sym, rel = MAP.get(s, (None, None))
        val = cfg.get(sym, "absent") if sym else "absent"

        ss = nm(stock_files[s]) or set()
        need = {x for x in ss if not x.startswith("__")} - LIFECYCLE

        ko = None
        for cand in (f"{s.replace('_','-')}.ko", f"{s}.ko"):
            if cand in built:
                ko = built[cand]
                break
        sub = nm(ko) if ko else (vml if val == "y" else set())

        missing = sorted(need - sub)
        in_src, exported = ({}, {})
        if missing and rel:
            in_src, exported = grep_source(rel, missing)

        fixable = sorted(set(missing) & set(in_src)) if missing else []
        absent = sorted(set(missing) - set(in_src)) if missing else []

        rows.append({
            "name": s, "sym": sym, "val": val, "ko": ko,
            "need": len(need), "cov": len(need) - len(missing),
            "missing": missing, "fixable": fixable, "absent": absent,
            "in_src": sorted(in_src), "exported": sorted(exported),
        })

    tot_need = sum(r["need"] for r in rows)
    tot_cov = sum(r["cov"] for r in rows)
    tot_fix = sum(len(r["fixable"]) for r in rows)
    tot_abs = sum(len(r["absent"]) for r in rows)
    perfect = sum(1 for r in rows if r["need"] and not r["missing"])
    nomod = sum(1 for r in rows if not r["need"])

    print("=" * 78)
    print("HONEST SYMBOL COMPARISON (module-lifecycle symbols excluded)")
    print(f"  stock .ko analysed      : {len(rows)}")
    print(f"  symbols needing match   : {tot_need}")
    print(f"  symbols matched         : {tot_cov}")
    print(f"  --> SYMBOL COVERAGE     : {tot_cov}/{tot_need} "
          f"({100.0 * tot_cov / max(1, tot_need):.1f}%)")
    print(f"  modules with 100% match : {perfect} (of {tot_need} symbols "
          f"across {sum(1 for r in rows if r['need'])} modules)")
    print(f"  fixable (in source, not exported): {tot_fix}")
    print(f"  truly absent from source        : {tot_abs}")
    print()
    for r in rows:
        if not r["need"]:
            print(f"  {r['name']:<26} (no comparable symbols)")
            continue
        tag = "100%" if not r["missing"] else \
            f"{r['cov']}/{r['need']}"
        print(f"  {r['name']:<26} {tag:<12} {r['val']:<8} "
              f"fixable={len(r['fixable'])} absent={len(r['absent'])}")
    print()
    print("--- symbols present in the VXTux source but not EXPORT_SYMBOL'd ---")
    for r in rows:
        for s in r["fixable"]:
            print(f"  {r['name']:<26} {s}")
    print()
    print("--- symbols absent from the VXTux source (real port gap) ---")
    for r in rows:
        for s in r["absent"]:
            print(f"  {r['name']:<26} {s}")

    L = ["# Honest Symbol Comparison — VXTux 6.18.52 vs stock vendor .ko\n"]
    L.append("`init_module`/`cleanup_module` are excluded: they are module "
             "lifecycle artefacts, absent by design when a driver is "
             "built into vmlinux instead of loaded as a module.\n")
    L.append(f"- Stock .ko analysed: {len(rows)}")
    L.append(f"- Symbols needing a match: {tot_need}")
    L.append(f"- Symbols matched: **{tot_cov}/{tot_need} "
             f"({100.0 * tot_cov / max(1, tot_need):.1f}%)**")
    L.append(f"- Modules at 100%: **{perfect}/"
             f"{sum(1 for r in rows if r['need'])}**")
    L.append(f"- In source but not exported (one-line fix): {tot_fix}")
    L.append(f"- Absent from source (real port gap): {tot_abs}")
    L.append("")
    L.append("| stock .ko | config | value | matched | fixable | absent |")
    L.append("|---|---|---|---|---|---|")
    for r in rows:
        L.append(f"| `{r['name']}` | `{r['sym'] or '-'}` | `{r['val']}` | "
                 f"{r['cov']}/{r['need']} | {len(r['fixable'])} | "
                 f"{len(r['absent'])} |")
    L.append("")
    if tot_fix:
        L.append("## Fixable: present in source, missing EXPORT_SYMBOL\n")
        L.append("| stock .ko | symbol |")
        L.append("|---|---|")
        for r in rows:
            for s in r["fixable"]:
                L.append(f"| `{r['name']}` | `{s}` |")
        L.append("")
    if tot_abs:
        L.append("## Real port gap: symbol absent from the VXTux source\n")
        L.append("| stock .ko | symbol |")
        L.append("|---|---|")
        for r in rows:
            for s in r["absent"]:
                L.append(f"| `{r['name']}` | `{s}` |")
        L.append("")

    with open(os.path.join(ROOT, "donors/SYMBOL_COMPARISON.md"), "w",
              encoding="utf-8") as f:
        f.write("\n".join(L))
    print("wrote donors/SYMBOL_COMPARISON.md")


if __name__ == "__main__":
    main()
