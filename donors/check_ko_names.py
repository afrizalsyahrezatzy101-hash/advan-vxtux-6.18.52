"""Check that flipping =y -> =m yields a .ko with the SAME NAME as the
stock vendor .ko (the loader scripts bind by name)."""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

# config symbol -> the stock .ko name it must produce
CANDS = {
    "CONFIG_CRYPTO_AES_ARM64_CE_CCM": "aes-ce-ccm",
    "CONFIG_GPIO_PMIC_EIC_SPRD": "gpio-pmic-eic-sprd",
    "CONFIG_ION_IPC_TRUSTY": "ion_ipc_trusty",
    "CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS": "novatek_ts_spi",
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
    "CONFIG_TRUSTY_LOG": "trusty-log",
    "CONFIG_SPRD_POWER_MANAGER": "sprd_power_manager",
    "CONFIG_SPRD_UMS512_CLK": "ums512-clk",
    "CONFIG_BACKLIGHT_PWM": "pwm_bl",
}

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
mks = [f for f in r.stdout.splitlines()
       if os.path.basename(f) == "Makefile"]
mks = [m.replace("\\", "/") for m in mks]

txt = {}
for m in mks:
    try:
        txt[m] = open(os.path.join(TREE, m), "r", encoding="utf-8",
                      errors="replace").read()
    except OSError:
        pass

print(f"{'config symbol':<38} {'stock .ko':<26} {'obj line':<34} match")
print("-" * 120)
ok = 0
bad = []
for sym, want in sorted(CANDS.items()):
    name = sym.replace("CONFIG_", "")
    found = None
    for m, t in txt.items():
        for mm in re.finditer(
                r"obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*([^\n]*)", t):
            found = (m, mm.group(1).strip())
            break
        if found:
            break
    if found:
        m, objs = found
        objs_clean = objs.replace("\\", "").strip()
        # the .ko name comes from the first .o
        ko = objs_clean.split()[0].replace(".o", "") if objs_clean else "?"
        match = "YES" if ko == want else f"NO (gives {ko}.ko)"
        if ko != want:
            bad.append((sym, want, ko, m))
        else:
            ok += 1
        print(f"{sym:<38} {want:<26} {objs_clean[:32]:<34} {match}")
    else:
        bad.append((sym, want, "?", "?"))
        print(f"{sym:<38} {want:<26} {'(no obj-$(...) line found)':<34} ?")

print()
print(f"exact name match: {ok}/{len(CANDS)}")
for sym, want, got, m in bad:
    print(f"  MISMATCH {sym}: want {want}.ko, tree gives {got}.ko  [{m}]")
