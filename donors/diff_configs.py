"""Definitive config comparison: stock SPRD kernel vs VXTux 6.18 build.

Compares two kernel .config files option-by-option and classifies the
difference in a way that is actionable for the deblob/mainline effort:

  SAME        - identical setting
  DIFF        - same symbol, different value (e.g. y vs m)
  ONLY_VXTUX  - option exists only in the VXTux config (new mainline feature)
  ONLY_STOCK  - option exists only in the stock config (vendor feature we
                dropped, or a mainline option the stock build never enabled)

Usage:
    python3 diff_configs.py <vxtux.config> <stock.config> [-o report.md]
"""
import os
import sys


def parse(path):
    out = {}
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")
            if not line or line.startswith("#"):
                # keep "# CONFIG_X is not set" as an explicit off value
                if line.startswith("# CONFIG_") and line.endswith(" is not set"):
                    name = line[2:line.index(" is not set")].strip()
                    out[name] = "n"
                continue
            if "=" in line:
                name, val = line.split("=", 1)
                out[name.strip()] = val.strip()
    return out


def group_of(name):
    """Bucket an option into a subsystem for the report."""
    n = name
    if n.startswith("CONFIG_SPRD_"):
        return "SPRD vendor"
    if n.startswith("CONFIG_ARCH_SPRD"):
        return "SPRD vendor"
    if n.startswith("CONFIG_MALI_") or n.startswith("CONFIG_MALI"):
        return "GPU (Mali)"
    if n.startswith("CONFIG_PANFROST"):
        return "GPU (Panfrost)"
    if n.startswith("CONFIG_DRM_"):
        return "GPU (DRM)"
    if "PANFROST" in n or "GONDUL" in n:
        return "GPU (Panfrost)"
    if n.startswith("CONFIG_UBUNTU"):
        return "Ubuntu/SPRD"
    if n.startswith("CONFIG_NET_") or n.startswith("CONFIG_BR_"):
        return "Network"
    if n.startswith("CONFIG_SOUND_") or n.startswith("CONFIG_SND_"):
        return "Audio"
    if n.startswith("CONFIG_USB_") or n.startswith("CONFIG_MUSB"):
        return "USB"
    if n.startswith("CONFIG_MMC_") or n.startswith("CONFIG_SDHCI"):
        return "MMC/SD"
    if n.startswith("CONFIG_TOUCHSCREEN_") or n.startswith("CONFIG_HID_"):
        return "Input/Touch"
    if n.startswith("CONFIG_WCN") or n.startswith("CONFIG_SPRD_WCN"):
        return "WCN (wifi/bt/gnss)"
    if n.startswith("CONFIG_CRYPTO_") or n.startswith("CONFIG_CRYPTO"):
        return "Crypto"
    if n.startswith("CONFIG_DRM"):
        return "GPU (DRM)"
    if n.startswith("CONFIG_EXTCON"):
        return "Extcon"
    if n.startswith("CONFIG_REGULATOR_") or n.startswith("CONFIG_SPRD_REGULATOR"):
        return "Regulator"
    if n.startswith("CONFIG_POWER_") or n.startswith("CONFIG_CHARGER_"):
        return "Power/Charging"
    if n.startswith("CONFIG_PHY_") or n.startswith("CONFIG_SPRD_PHY"):
        return "PHY"
    if n.startswith("CONFIG_PWM_"):
        return "PWM"
    if n.startswith("CONFIG_LED_") or n.startswith("CONFIG_LEDS"):
        return "LED"
    if n.startswith("CONFIG_GPIO_"):
        return "GPIO"
    if n.startswith("CONFIG_I2C_") or n.startswith("CONFIG_PINCTRL"):
        return "I2C/Pinctrl"
    if n.startswith("CONFIG_CLK_") or n.startswith("CONFIG_COMMON_CLK"):
        return "Clock"
    if n.startswith("CONFIG_RTC_"):
        return "RTC"
    if n.startswith("CONFIG_IIO_") or n.startswith("CONFIG_ADC"):
        return "IIO/ADC"
    if n.startswith("CONFIG_NVMEM"):
        return "NVMEM/eFuse"
    if n.startswith("CONFIG_DEVFREQ_") or n.startswith("CONFIG_SPRD_APSYS"):
        return "Devfreq/DVFS"
    if n.startswith("CONFIG_SPRD_SIPC") or n.startswith("CONFIG_SIPC"):
        return "SIPC"
    if n.startswith("CONFIG_SPRD_") or n.startswith("CONFIG_UNISOC"):
        return "SPRD vendor"
    if n.startswith("CONFIG_MEDIA_") or n.startswith("CONFIG_VIDEO_"):
        return "Media/V4L2"
    if n.startswith("CONFIG_JPEG") or n.startswith("CONFIG_SPRD_JPG"):
        return "JPEG"
    if n.startswith("CONFIG_F2FS") or n.startswith("CONFIG_BTRFS"):
        return "Filesystem"
    if n.startswith("CONFIG_EXT4") or n.startswith("CONFIG_FS_"):
        return "Filesystem"
    if n.startswith("CONFIG_SUSPEND") or n.startswith("CONFIG_PM_"):
        return "PM/Suspend"
    if n.startswith("CONFIG_KALLSYMS") or n.startswith("CONFIG_DEBUG_"):
        return "Debug"
    if n.startswith("CONFIG_SECURITY") or n.startswith("CONFIG_CRYPTO"):
        return "Security"
    if n.startswith("CONFIG_VIRT") or n.startswith("CONFIG_KVM"):
        return "Virt"
    return "Other"


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return
    vpath, spath = sys.argv[1], sys.argv[2]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    v = parse(vpath)
    s = parse(spath)

    only_v = sorted(set(v) - set(s))
    only_s = sorted(set(s) - set(v))
    common = sorted(set(v) & set(s))
    diff = [(k, v[k], s[k]) for k in common if v[k] != s[k]]

    print("=" * 78)
    print("CONFIG DIFF  (vxtux vs stock)")
    print(f"  vxtux : {len(v)} options")
    print(f"  stock : {len(s)} options")
    print(f"  common: {len(common)}   same-value: {len(common) - len(diff)}   "
          f"different-value: {len(diff)}")
    print(f"  only in vxtux: {len(only_v)}")
    print(f"  only in stock: {len(only_s)}")

    # bucket summary
    buckets = {}
    for k, vv, sv in diff:
        buckets.setdefault(group_of(k), []).append((k, vv, sv))
    for k in only_s:
        buckets.setdefault(group_of(k), []).append((k, "-", s[k]))
    for k in only_v:
        buckets.setdefault(group_of(k), []).append((k, v[k], "-"))

    print()
    print("--- by subsystem ---")
    for g in sorted(buckets, key=lambda x: -len(buckets[x])):
        print(f"  {g:<22} {len(buckets[g]):>4} differ")

    lines = []
    lines.append("# Kernel Config Diff — VXTux 6.18.52 vs Stock SPRD 5.4\n")
    lines.append(f"- vxtux: `{vpath}` ({len(v)} options)")
    lines.append(f"- stock: `{spath}` ({len(s)} options)")
    lines.append(f"- common: {len(common)} "
                 f"(same value: {len(common)-len(diff)}, "
                 f"different value: {len(diff)})")
    lines.append(f"- only in vxtux: {len(only_v)}")
    lines.append(f"- only in stock: {len(only_s)}\n")

    for g in sorted(buckets, key=lambda x: -len(buckets[x])):
        lines.append(f"## {g} ({len(buckets[g])})\n")
        for k, vv, sv in sorted(buckets[g]):
            if vv == "-":
                lines.append(f"- `{k}` — stock only, value `{sv}`")
            elif sv == "-":
                lines.append(f"- `{k}` — vxtux only, value `{vv}`")
            else:
                lines.append(f"- `{k}` — vxtux=`{vv}` stock=`{sv}`")
        lines.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(lines))
        print(f"\nwrote {outpath}")


if __name__ == "__main__":
    main()
