"""Can the 16 built-in substitutes be flipped back to =m?

To be a 1:1 replacement for a stock vendor .ko, the VXTux build should
produce a module with the SAME NAME and the SAME EXPORTED SYMBOLS.
That needs two things:
  1. the Kconfig symbol must be tristate (so =m is possible)
  2. the source must carry EXPORT_SYMBOL() for what the vendor .ko
     exported

This reports, per substitute, the Kconfig type and whether the config
is currently =y because the type forces it or because the source
cannot be built as a module.
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

# the 16 that the config currently builds into vmlinux
BUILTIN = {
    "aes-ce-ccm": "CONFIG_CRYPTO_AES_ARM64_CE_CCM",
    "gpio-pmic-eic-sprd": "CONFIG_GPIO_PMIC_EIC_SPRD",
    "ion_ipc_trusty": "CONFIG_ION_IPC_TRUSTY",
    "novatek_ts_spi": "CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS",
    "nvmem-sc27xx-efuse": "CONFIG_NVMEM_SC27XX_EFUSE",
    "nvmem_sprd_cache_efuse": "CONFIG_NVMEM_SPRD_CACHE_EFUSE",
    "rtc-sc27xx": "CONFIG_RTC_DRV_SC27XX",
    "sc27xx_adc": "CONFIG_SC27XX_ADC",
    "sdhci-sprd": "CONFIG_MMC_SDHCI_SPRD",
    "sprd-sipc-virt-bus": "CONFIG_SPRD_SIPC_VIRT_BUS",
    "sprd-vsp-pw-domain": "CONFIG_SPRD_VSP_PW_DOMAIN",
    "sprd_time_sync": "CONFIG_SPRD_TIME_SYNC",
    "trusty": "CONFIG_TRUSTY",
    "trusty-ipc": "CONFIG_TRUSTY_VIRTIO_IPC",
    "trusty-irq": "CONFIG_TRUSTY",
    "trusty-tui": "CONFIG_TRUSTY_TUI",
    "twofish_common": "CONFIG_CRYPTO_TWOFISH_COMMON",
    "unisoc-mailbox": "CONFIG_SPRD_MAILBOX",
}

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
kcfgs = [f for f in r.stdout.splitlines()
         if os.path.basename(f).startswith("Kconfig")]

blob = {}
for k in kcfgs:
    try:
        blob[k] = open(os.path.join(TREE, k), "r", encoding="utf-8",
                       errors="replace").read()
    except OSError:
        pass

cfg = open(CFG, "r", encoding="utf-8", errors="replace").read()
cfgvals = {}
for line in cfg.splitlines():
    line = line.strip()
    if line.startswith("CONFIG_") and "=" in line:
        k, v = line.split("=", 1)
        cfgvals[k] = v
    elif line.startswith("# CONFIG_") and line.endswith(" is not set"):
        cfgvals[line[2:line.index(" is not set")].strip()] = "n"


def kconfig_entry(sym):
    """Return (file, type, body) for `config <sym>`."""
    name = sym.replace("CONFIG_", "")
    for k, txt in blob.items():
        m = re.search(r"^config " + re.escape(name) + r"\s*$(.*?)^endconfig",
                      txt, re.S | re.M)
        if m:
            body = m.group(1)
            ktype = "bool"
            for line in body.splitlines():
                s = line.strip()
                if s.startswith("tristate"):
                    ktype = "tristate"
                elif s.startswith("bool"):
                    ktype = "bool"
                elif s.startswith("menuconfig"):
                    ktype = "menuconfig(bool)"
                elif s.startswith("depends on") or s.startswith("select") \
                        or s.startswith("default") or s.startswith("help") \
                        or s.startswith("prompt") or s.startswith("if ") \
                        or s.startswith("endif") or s.startswith("depends"):
                    continue
            return k, ktype, body
    return None, None, None


print("=" * 90)
print(f"{'stock .ko':<26} {'config':<38} {'cur':<5} {'type':<16} "
      f"{'=m possible':<12}")
print("-" * 90)
can_m, cannot_m = [], []
for mod, sym in sorted(BUILTIN.items()):
    f, ktype, body = kconfig_entry(sym)
    cur = cfgvals.get(sym, "absent")
    poss = "YES" if ktype == "tristate" else "no (bool)"
    if ktype == "tristate":
        can_m.append((mod, sym, cur, f))
    else:
        cannot_m.append((mod, sym, cur, ktype, f))
    print(f"{mod:<26} {sym:<38} {cur:<5} {str(ktype):<16} {poss:<12}")

print()
print(f"CAN be =m ({len(can_m)}):")
for mod, sym, cur, f in can_m:
    print(f"   {mod:<26} {sym:<38} {f}")
print()
print(f"CANNOT be =m, bool only ({len(cannot_m)}):")
for mod, sym, cur, kt, f in cannot_m:
    print(f"   {mod:<26} {sym:<38} {kt:<8} {f}")

# for the tristate ones, show the dependencies
print()
print("=== dependency chains for the =m candidates ===")
for mod, sym, cur, f in can_m:
    _, _, body = kconfig_entry(sym)
    deps = [l.strip() for l in body.splitlines()
            if l.strip().startswith("depends on")]
    print(f"  {sym}")
    for d in deps:
        for tok in re.findall(r"[A-Z][A-Z0-9_]{3,}", d):
            v = cfgvals.get(tok, "absent")
            flag = "" if v in ("y", "m") else "   <-- BLOCKS"
            print(f"      {tok:<38} = {v}{flag}")
    print()
