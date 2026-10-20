"""Verify each NOT_PRESENT stock module individually.

The stem-based match can miss a driver whose source file has a
different name than the module (e.g. a directory Makefile that builds
several objects). For every remaining stock module this searches the
whole tree for the driver's distinctive tokens and reports what it
finds, so the port gap list is trustworthy.
"""
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

# stock module -> distinctive tokens to look for anywhere in the tree
TARGETS = {
    "aes-ce-ccm": ["aes-ce-ccm", "aes_ce_ccm", "CRYPTO_AES_ARM64_CE_CCM"],
    "apsys-dvfs": ["apsys", "APSYS_DVFS", "apsys-dvfs"],
    "mmdvfs": ["mmdvfs", "MMC_DVFS", "mm-dvfs", "sprd_mmc_dvfs"],
    "novatek_ts_spi": ["novatek", "nvt_ts", "NVT_TS", "novatek_nvt"],
    "nvmem-sc27xx-efuse": ["sc27xx-efuse", "sc27xx_efuse", "efuse"],
    "nvmem_sprd_cache_efuse": ["cache_efuse", "cache-efuse", "SPRD_CACHE_EFUSE"],
    "sprd-dmaengine-pcm": ["dmaengine", "sprd-dma", "SPRD_DMAENGINE"],
    "sprd-sipc-virt-bus": ["sipc", "virt-bus", "virt_bus", "SIPC_VIRT"],
    "sprd_apipe": ["apipe", "APIPE"],
    "sprd_flash_drv": ["sprd_flash", "sprd-flash", "SPRD_FLASH"],
    "sprd_time_sync": ["time_sync", "time-sync", "sprd_ts", "TIME_SYNC"],
    "trusty": ["trusty"],
    "unisoc-mailbox": ["unisoc", "mailbox", "MAILBOX"],
    "usb_f_vser": ["f_vser", "vser", "VSER", "fserial"],
}


def main():
    cfg = open(CFG, "r", encoding="utf-8", errors="replace").read()
    cfg_lines = [l for l in cfg.splitlines() if l.startswith("CONFIG_")]

    r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                       capture_output=True, text=True, timeout=300)
    files = [f.replace("\\", "/") for f in r.stdout.splitlines()]

    # also load the Makefile text of every directory, to catch
    # obj-$(CONFIG_X) += foo.o where foo.c is not in the tree root
    mk_text = {}
    for f in files:
        if f.endswith("Makefile"):
            try:
                mk_text[f] = open(os.path.join(TREE, f), "r",
                                  encoding="utf-8", errors="replace").read()
            except OSError:
                pass

    print("=" * 78)
    for mod, toks in TARGETS.items():
        print(f"--- {mod} ---")
        hits_cfg = []
        for t in toks:
            for l in cfg_lines:
                if t.upper() in l.upper():
                    hits_cfg.append(l)
                    break
        src_hits = []
        for f in files:
            fl = f.lower()
            if any(t.lower() in fl for t in toks):
                src_hits.append(f)
        mk_hits = []
        for mk, txt in mk_text.items():
            for t in toks:
                if t.lower() in txt.lower():
                    mk_hits.append(f"{mk}  [{t}]")
                    break
        for c in sorted(set(hits_cfg)):
            print(f"   cfg: {c}")
        for s in sorted(set(src_hits))[:5]:
            print(f"   src: {s}")
        for m in sorted(set(mk_hits))[:5]:
            print(f"   mk : {m}")
        if not hits_cfg and not src_hits and not mk_hits:
            print("   >>> NOTHING FOUND — genuine port gap")
        print()


if __name__ == "__main__":
    main()
