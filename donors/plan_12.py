import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

# stock blob -> the Makefile obj- that builds it (found by inspection)
STOCK_SRC = {
    "apsys-dvfs": ("drivers/devfreq/sprd/apsys/Makefile", "sprd_dvfs_apsys.o"),
    "audio-dsp-dump": ("drivers/sound/soc/sprd/audiodspdump/Makefile", None),
    "audio-pipe": ("drivers/sound/soc/sprd/audiosipc/Makefile", None),
    "audio_sipc": ("drivers/sound/soc/sprd/audiosipc/Makefile", None),
    "mmdvfs": (None, None),
    "nvmem_sprd_cache_efuse":
        ("drivers/soc/sprd/cache_efuse/Makefile", "sprd_cache_efuse.o"),
    "sprd-dmaengine-pcm": (None, None),
    "sprd-sipc-virt-bus": (None, None),
    "sprd-vsp-pw-domain":
        ("drivers/pmdomain/sprd/Makefile", "sprd_vsp_pw_domain.o"),
    "sprd_apipe": (None, None),
    "sprd_flash_drv": (None, None),
    "sprd_time_sync": (None, None),
    "unisoc-mailbox": (None, None),
    "usb_f_vser": (None, None),
}

# the desired .ko name -> the source object(s) that must go into it
# composite: <name>-y := objs
KO_RENAME = {
    "apsys-dvfs": ("drivers/devfreq/sprd/apsys/Makefile",
                   "CONFIG_SPRD_APSYS_DVFS", "apsys-dvfs",
                   ["sprd_dvfs_apsys.o"]),
    "nvmem_sprd_cache_efuse": ("drivers/soc/sprd/cache_efuse/Makefile",
                               "CONFIG_NVMEM_SPRD_CACHE_EFUSE",
                               "nvmem_sprd_cache_efuse",
                               ["sprd_cache_efuse.o"]),
    "sprd-vsp-pw-domain": ("drivers/pmdomain/sprd/Makefile",
                           "CONFIG_SPRD_VSP_PW_DOMAIN",
                           "sprd-vsp-pw-domain",
                           ["sprd_vsp_pw_domain.o", "vsp_regs.o"]),
}

# the config symbols that must be ON for each stock blob
NEED = {
    "apsys-dvfs": ["CONFIG_SPRD_APSYS_DVFS"],
    "audio-dsp-dump": ["CONFIG_SPRD_AUDIO_DSP_DUMP"],
    "audio-pipe": ["CONFIG_SPRD_AUDIO_SIPC"],
    "audio_sipc": ["CONFIG_SPRD_AUDIO_SIPC"],
    "nvmem_sprd_cache_efuse": ["CONFIG_NVMEM_SPRD_CACHE_EFUSE"],
    "sprd-vsp-pw-domain": ["CONFIG_SPRD_VSP_PW_DOMAIN"],
}


def main():
    cfg = open(CFG, encoding="utf-8", errors="replace").read()
    vals = {}
    for line in cfg.splitlines():
        line = line.strip()
        if line.startswith("CONFIG_") and "=" in line:
            k, v = line.split("=", 1)
            vals[k] = v
        elif line.startswith("# CONFIG_") and line.endswith(" is not set"):
            vals[line[2:line.index(" is not set")].strip()] = "n"

    print("=" * 90)
    print("Config symbols needed for the 12 uncovered stock blobs")
    print("=" * 90)
    for st, syms in NEED.items():
        for s in syms:
            print(f"  {st:<28} {s:<40} = {vals.get(s, 'ABSENT')}")

    # which config symbols are tristate in the Kconfig at all?
    print()
    print("=" * 90)
    print("Kconfig type check (must be tristate to be =m)")
    print("=" * 90)
    r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                       capture_output=True, text=True, timeout=300)
    files = [f.replace("\\", "/") for f in r.stdout.splitlines()]
    types = {}
    for f in files:
        if not os.path.basename(f).startswith("Kconfig"):
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
                types.setdefault(cur, ["bool", f])
            elif s.startswith("menuconfig "):
                cur = s[11:].strip()
                types.setdefault(cur, ["bool", f])
            elif cur and s.startswith("\ttristate"):
                types[cur][0] = "tristate"
            elif cur and s.startswith("\tbool"):
                types[cur][0] = "bool"

    for st, syms in NEED.items():
        for s in syms:
            name = s.replace("CONFIG_", "")
            t = types.get(name)
            if t:
                print(f"  {st:<28} {s:<40} type={t[0]:<10} {t[1]}")
            else:
                print(f"  {st:<28} {s:<40} type=NOT-FOUND-IN-KCONFIG")

    # the search for the 8 with no known source
    print()
    print("=" * 90)
    print("Search for the remaining 8 by Kconfig name fragment")
    print("=" * 90)
    for st, frag in [("mmdvfs", "MMC_DVFS"), ("sprd-dmaengine-pcm", "DMAENGINE"),
                     ("sprd-sipc-virt-bus", "SIPC_VIRT"),
                     ("sprd_apipe", "APIPE"), ("sprd_flash_drv", "FLASH"),
                     ("sprd_time_sync", "TIME_SYNC"),
                     ("unisoc-mailbox", "MAILBOX"), ("usb_f_vser", "VSER")]:
        hits = [n for n in types if frag in n]
        print(f"  {st:<24} {frag:<16} -> {hits[:6]}")
        for h in hits[:4]:
            v = vals.get("CONFIG_" + h, "ABSENT")
            print(f"        CONFIG_{h:<40} = {v}  type={types[h][0]}")


if __name__ == "__main__":
    main()
