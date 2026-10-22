"""Flip the substitute drivers from =y to =m so the VXTux build emits a
MODULE with the same name as the stock vendor .ko.

Why this matters for "100% identical":

The stock ROM ships these drivers as loadable modules:
    pwm_bl.ko, sprd_power_manager.ko, trusty-log.ko, ums512-clk.ko
    aes-ce-ccm.ko, gpio-pmic-eic-sprd.ko, nvmem-sc27xx-efuse.ko
    nvmem_sprd_cache_efuse.ko, rtc-sc27xx.ko, sc27xx_adc.ko
    sdhci-sprd.ko, sprd-vsp-pw-domain.ko, trusty.ko, trusty-ipc.ko
    trusty-irq.ko, trusty-tui.ko, twofish_common.ko,
    ion_ipc_trusty.ko, novatek_ts_spi.ko

When the VXTux build compiles them into vmlinux instead, the userspace
loader scripts and the initramfs cannot insmod them, and the symbols
live in a different place. Turning the symbol back to =m makes the
build produce the same .ko file name, so the deblob is a real
drop-in replacement rather than a "the code exists somewhere" claim.

Each symbol is checked: it must be tristate in Kconfig, and its
obj-$(...) line must produce the exact .ko name the stock blob has.

Usage:
    python3 flip_to_module.py [--apply]
"""
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")
APPLY = "--apply" in sys.argv

# config symbol -> the .ko name the stock blob has
TARGETS = {
    "CONFIG_CRYPTO_AES_ARM64_CE_CCM": "aes-ce-ccm",
    "CONFIG_GPIO_PMIC_EIC_SPRD": "gpio-pmic-eic-sprd",
    "CONFIG_ION_IPC_TRUSTY": "ion_ipc_trusty",
    "CONFIG_NVMEM_SC27XX_EFUSE": "nvmem-sc27xx-efuse",
    "CONFIG_NVMEM_SPRD_CACHE_EFUSE": "nvmem_sprd_cache_efuse",
    "CONFIG_RTC_DRV_SC27XX": "rtc-sc27xx",
    "CONFIG_SC27XX_ADC": "sc27xx_adc",
    "CONFIG_MMC_SDHCI_SPRD": "sdhci-sprd",
    "CONFIG_SPRD_VSP_PW_DOMAIN": "sprd-vsp-pw-domain",
    "CONFIG_TRUSTY": "trusty",
    "CONFIG_TRUSTY_VIRTIO_IPC": "trusty-ipc",
    "CONFIG_TRUSTY_TUI": "trusty-tui",
    "CONFIG_CRYPTO_TWOFISH_COMMON": "twofish_common",
    "CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI": "novatek_ts_spi",
}


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                       capture_output=True, text=True, timeout=300)
    files = [f.replace("\\", "/") for f in r.stdout.splitlines()]

    # 1) the .ko each symbol produces
    ko_of = {}
    mks = [f for f in files if os.path.basename(f) == "Makefile"]
    for f in mks:
        try:
            txt = open(os.path.join(TREE, f), "r", encoding="utf-8",
                       errors="replace").read()
        except OSError:
            continue
        for m in re.finditer(r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*"
                             r"([^\n]*)", txt):
            sym, objs = m.group(1), m.group(2).replace("\\", "").strip()
            if not objs:
                continue
            first = objs.split()[0]
            if first.endswith(".o"):
                ko_of.setdefault(sym, set()).add(first[:-2])
            else:
                ko_of.setdefault(sym, set()).add(first.rstrip("/"))

    # 2) the Kconfig type - line based, because a regex over the whole
    #    Kconfig file is fragile (the '$' anchor interacts badly with
    #    the greedy body match)
    kcfg = {}
    for f in files:
        base = os.path.basename(f)
        if not base.startswith("Kconfig"):
            continue
        try:
            lines = open(os.path.join(TREE, f), "r", encoding="utf-8",
                         errors="replace").read().split("\n")
        except OSError:
            continue
        cur = None
        for line in lines:
            s = line.rstrip()
            if s.startswith("config "):
                cur = s[7:].strip()
                kcfg.setdefault(cur, ["bool", f])
            elif s.startswith("menuconfig "):
                cur = s[11:].strip()
                kcfg.setdefault(cur, ["bool", f])
            elif cur and s.startswith("\ttristate"):
                kcfg[cur][0] = "tristate"
            elif cur and s.startswith("\tbool"):
                kcfg[cur][0] = "bool"

    cfg = read(CFG)
    vals = {}
    for line in cfg.splitlines():
        line = line.strip()
        if line.startswith("CONFIG_") and "=" in line:
            k, v = line.split("=", 1)
            vals[k] = v
        elif line.startswith("# CONFIG_") and line.endswith(" is not set"):
            vals[line[2:line.index(" is not set")].strip()] = "n"

    print("=" * 88)
    print(f"{'config symbol':<38} {'now':<5} {'type':<10} {'ko':<26} "
          f"{'want':<24} ok")
    print("-" * 88)
    changes = []
    for sym, want in sorted(TARGETS.items()):
        name = sym.replace("CONFIG_", "")
        ktype, kfile = kcfg.get(name, ("?", "?"))
        kos = ko_of.get(sym, set())
        ko = sorted(kos)[0] if kos else "?"
        cur = vals.get(sym, "absent")
        ok = (ktype == "tristate") and (want in kos)
        print(f"{sym:<38} {str(cur):<5} {ktype:<10} {ko:<26} {want:<24} "
              f"{'YES' if ok else 'NO'}")
        if ok and cur in ("y", "m"):
            changes.append((sym, cur, want))

    if not changes:
        print("\nnothing to flip")
        return

    print()
    for sym, cur, want in changes:
        print(f"  {sym}: {cur} -> m")
    if APPLY:
        out = cfg
        for sym, cur, want in changes:
            out = out.replace(f"\n{sym}={cur}", f"\n{sym}=m")
        write(CFG, out)
        # verify
        chk = read(CFG)
        bad = [s for s, c, w in changes if f"\n{s}=m" not in chk]
        print(f"\nWROTE {CFG} ({len(out):,} bytes); "
              f"verify failures: {bad if bad else 'none'}")
    else:
        print("\ndry run - re-run with --apply")


if __name__ == "__main__":
    main()
