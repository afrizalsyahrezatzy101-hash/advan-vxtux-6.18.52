"""Find every place that tests a config symbol with #ifdef / #if
defined() for the symbols that were flipped from =y to =m.

Why this matters: when a tristate symbol is =m, autoconf.h defines
CONFIG_FOO_MODULE and NOT CONFIG_FOO. So

    #ifdef CONFIG_FOO        <- FALSE when =m
    #if defined(CONFIG_FOO)  <- FALSE when =m

take the WRONG branch. If that branch is the "feature not present,
provide a stub" branch, the .c file's real definition collides with
the header's static inline stub -> "redefinition of ...".

The correct test is

    #if IS_ENABLED(CONFIG_FOO)

which is true for =y, =m and undefined, and needs <linux/kconfig.h>.

This finds the sites so they can all be fixed in one commit.

Usage:
    python3 survey_ifdef_config.py
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

# every symbol that is now =m because of the deblob work
FLIPPED = [
    "CONFIG_CRYPTO_AES_ARM64_CE_CCM",
    "CONFIG_CRYPTO_TWOFISH_COMMON",
    "CONFIG_GPIO_PMIC_EIC_SPRD",
    "CONFIG_ION_IPC_TRUSTY",
    "CONFIG_MMC_SDHCI_SPRD",
    "CONFIG_NVMEM_SC27XX_EFUSE",
    "CONFIG_NVMEM_SPRD_CACHE_EFUSE",
    "CONFIG_RTC_DRV_SC27XX",
    "CONFIG_SC27XX_ADC",
    "CONFIG_SPRD_APSYS_DVFS",
    "CONFIG_SPRD_AUDIO_DSP_DUMP",
    "CONFIG_SPRD_AUDIO_SIPC",
    "CONFIG_SPRD_AUDIO_MEMORY_MANAGER",
    "CONFIG_SPRD_VSP_PW_DOMAIN",
    "CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI",
    "CONFIG_TRUSTY",
    "CONFIG_TRUSTY_LOG",
    "CONFIG_TRUSTY_TUI",
    "CONFIG_TRUSTY_VIRTIO",
    "CONFIG_TRUSTY_VIRTIO_IPC",
]

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
files = [f.replace("\\", "/") for f in r.stdout.splitlines()]
code = [f for f in files
        if f.endswith((".c", ".h")) and "/Documentation/" not in f]

hits = []
for rel in code:
    p = os.path.join(TREE, rel)
    try:
        with open(p, "r", encoding="utf-8", errors="replace") as f:
            txt = f.read()
    except OSError:
        continue
    for i, line in enumerate(txt.split("\n"), 1):
        s = line.strip()
        for sym in FLIPPED:
            if not s.startswith("#"):
                continue
            if re.search(r"#\s*ifdef\s+" + re.escape(sym) + r"\b", s) or \
               re.search(r"#\s*if\s+defined\(\s*" + re.escape(sym) +
                         r"\s*\)", s) or \
               re.search(r"#\s*if\s+!defined\(\s*" + re.escape(sym) +
                         r"\s*\)", s):
                hits.append((rel, i, sym, s))
                break

print("=" * 100)
print(f"#ifdef / #if defined() sites for the flipped symbols: {len(hits)}")
print("=" * 100)
by_sym = {}
for rel, i, sym, s in hits:
    by_sym.setdefault(sym, []).append((rel, i, s))

for sym in FLIPPED:
    h = by_sym.get(sym)
    if not h:
        continue
    print(f"\n{sym}  ({len(h)} site(s))")
    for rel, i, s in h:
        print(f"   {rel}:{i}   {s}")

print()
print("=" * 100)
print("files that need <linux/kconfig.h> for IS_ENABLED()")
print("=" * 100)
for sym, h in sorted(by_sym.items()):
    for rel, i, s in h:
        p = os.path.join(TREE, rel)
        try:
            txt = open(p, "r", encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        has = "kconfig.h" in txt
        print(f"   {rel:<62} kconfig.h: {'yes' if has else 'NO'}")
