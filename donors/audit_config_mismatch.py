"""Check every stock vendor .ko against the config for a MISMATCH
between the config symbol and the driver that the symbol actually
builds.

The Novatek case was a config mistake: the symbol that was =y built an
unrelated upstream driver, while the driver that actually replaces the
stock blob was =n. This checks the other gap modules for the same
pattern - the source exists in the tree, but the config that would
build it is off, and some OTHER symbol is on instead.
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

# the modules still not covered -> the config symbol that would build
# the driver that replaces the stock blob
GAPS = {
    "apsys-dvfs": ["CONFIG_SPRD_APSYS_DVFS"],
    "audio-dsp-dump": ["CONFIG_SPRD_AUDIO_DSP_DUMP"],
    "audio-pipe": ["CONFIG_SPRD_AUDIO_PIPE"],
    "audio_sipc": ["CONFIG_SPRD_AUDIO_SIPC"],
    "mmdvfs": ["CONFIG_SPRD_MMC_DVFS", "CONFIG_MMC_DVFS",
               "CONFIG_SPRD_MMC_CQ_DVFS"],
    "sprd-dmaengine-pcm": ["CONFIG_SPRD_DMAENGINE_PCM",
                            "CONFIG_SND_SOC_SPRD_DMAENGINE_PCM"],
    "sprd_apipe": ["CONFIG_SPRD_APIPE", "CONFIG_SPRD_AUDIO_PIPE"],
    "sprd_flash_drv": ["CONFIG_SPRD_FLASH"],
    "sprd-sipc-virt-bus": ["CONFIG_SPRD_SIPC_VIRT_BUS",
                           "CONFIG_SPRD_SIPC"],
    "sprd_time_sync": ["CONFIG_SPRD_TIME_SYNC", "CONFIG_SPRD_TSHM"],
    "unisoc-mailbox": ["CONFIG_SPRD_MAILBOX", "CONFIG_MAILBOX",
                       "CONFIG_SPRD_MAILBOX_R1"],
    "usb_f_vser": ["CONFIG_USB_F_VSER"],
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

    r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                       capture_output=True, text=True, timeout=300)
    files = [f.replace("\\", "/") for f in r.stdout.splitlines()]

    # the .ko each symbol would build, from the Makefiles
    ko_of = {}
    for f in files:
        if os.path.basename(f) != "Makefile":
            continue
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
            ko = first.replace(".o", "") if first.endswith(".o") else None
            ko_of.setdefault(sym, set()).add(ko or f"dir:{first}")

    print("=" * 90)
    print("CONFIG-vs-DRIVER audit for the still-uncovered stock modules")
    print("=" * 90)
    for mod, syms in sorted(GAPS.items()):
        print(f"\n--- {mod} (stock .ko) ---")
        for sym in syms:
            v = vals.get(sym, "absent")
            kos = sorted(k for k in ko_of.get(sym, ()) if k)
            print(f"    {sym:<38} = {str(v):<8} builds: "
                  f"{', '.join(kos) if kos else '(no obj- line found)'}")
        # which symbols in the tree mention this driver name?
        hits = []
        for f in files:
            base = os.path.basename(f)
            if base.endswith(".c") and mod.replace("-", "_") in base:
                hits.append(f)
        for h in hits[:3]:
            print(f"    source present: {h}")

    # also: which config symbols are ON that build a .ko with the same
    # name as a stock blob but from a DIFFERENT source?
    print()
    print("=" * 90)
    print("Reverse check: a config symbol that is ON and builds a .ko "
          "named like a stock blob")
    print("=" * 90)
    stock = [l.split("\t")[0] for l in
             open("E:/vxtux_build_repo/donors/stock_modules.txt",
                  encoding="utf-8", errors="replace") if l.strip()]
    for s in stock:
        want = s.replace("_", "-")
        for sym, v in vals.items():
            if v not in ("y", "m"):
                continue
            for ko in ko_of.get(sym, ()):
                if not ko:
                    continue
                if ko == want and sym not in GAPS.get(s, []):
                    print(f"    {sym:<44} = {v:<4} -> {ko}.ko  "
                          f"(stock blob {s}.ko)")


if __name__ == "__main__":
    main()
