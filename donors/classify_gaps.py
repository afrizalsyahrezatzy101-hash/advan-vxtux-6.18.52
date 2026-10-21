#!/usr/bin/env python3
"""Classify the symbols blob_gap.py reports as missing.

For every missing symbol, look it up in the 6.18 tree source:

  PRESENT-UNEXPORTED  the symbol exists in the tree but has no
                      EXPORT_SYMBOL -> the fix is one line per symbol
  ABSENT              the symbol does not exist anywhere in the tree
                      -> it needs real implementation work

Usage:
    python3 donors/classify_gaps.py donors/bb_Module.symvers
"""
import os
import re
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
ROOT = "6.18.52-VXTux-UMS512"
SRC_DIRS = ["drivers", "arch/arm64/boot/dts", "include"]

SKIP = re.compile(
    r"^(__cfi|__this_module$|__mod_of__|__typeid__|__crc_|__kstrtab_|__ksymtab_"
    r"|__UNIQUE_ID|__warned|__key|__param|__exit|__pfx_|__func__|__used"
    r"|__stack_chk|__fentry__|__llvm|__mod|__this|__exidx|__gcc_except"
    r"|\.L|cleanup_module$|init_module$)"
)

MAPPING = {
    "aes-ce-ccm.ko": ["ce_aes_ccm", "aes_ce_ccm"],
    "apsys-dvfs.ko": ["apsys_dvfs", "dpu_dvfs", "dpu_devfreq"],
    "audio-dsp-dump.ko": ["audio_dsp_dump", "audio_sblock"],
    "audio_sipc.ko": ["audio_sipc"],
    "mmdvfs.ko": ["sprd_mm_dvfs", "mm_dvfs"],
    "novatek_ts_spi.ko": ["novatek_nvt_ts_spi", "nt36"],
    "sdhci-sprd.ko": ["sdhci_sprd", "sdhci-sprd"],
    "sprd_flash_drv.ko": ["sprd_flash"],
    "sprd_time_sync.ko": ["sprd_time_sync"],
    "sprd-dmaengine-pcm.ko": ["sprd_dmaengine_pcm"],
    "trusty.ko": ["trusty_core"],
    "trusty-log.ko": ["trusty_log"],
}


def blob_syms(path):
    r = subprocess.run([NM, "-g", "--defined-only", path],
                       capture_output=True, text=True, timeout=120)
    out = set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) < 3 or p[-2] not in ("T", "D", "B", "R", "W", "V", "A"):
            continue
        if SKIP.match(p[-1]):
            continue
        out.add(p[-1])
    return out


def load_symvers(path):
    prov = {}
    for line in open(path, errors="replace"):
        f = line.rstrip("\n").split("\t")
        if len(f) >= 4:
            prov[f[1]] = f[2]
    return prov


def scan_tree():
    """symbol -> list of files that define it."""
    defs = {}
    exported = set()
    for d in SRC_DIRS:
        for root, _dirs, files in os.walk(os.path.join(ROOT, d)):
            if ".git" in root:
                continue
            for f in files:
                if not f.endswith((".c", ".h")):
                    continue
                p = os.path.join(root, f)
                try:
                    txt = open(p, errors="replace").read()
                except OSError:
                    continue
                for m in re.finditer(r"EXPORT_SYMBOL(?:_GPL)?\(([A-Za-z_]\w*)\)", txt):
                    exported.add(m.group(1))
                # a definition: the symbol at the start of a line, or after
                # a type keyword, not inside a call
                for m in re.finditer(
                        r"^[A-Za-z_][\w \t\*]*?\b([a-z_][a-z0-9_]*)\s*(\(|=|\[|;)",
                        txt, re.M):
                    defs.setdefault(m.group(1), []).append(p)
    return defs, exported


def main():
    symvers = load_symvers(sys.argv[1])
    defs, exported = scan_tree()
    print("tree: %d candidate definitions, %d EXPORT_SYMBOLs\n"
          % (len(defs), len(exported)))

    unexported = {}
    absent = {}
    for blob, hints in sorted(MAPPING.items()):
        path = os.path.join("donors/stock_vendor_modules", blob)
        if not os.path.exists(path):
            continue
        for s in blob_syms(path):
            mod = symvers.get(s)
            if mod is not None and (mod == "vmlinux" or
                                    any(h in mod for h in hints)):
                continue                      # already covered
            files = defs.get(s)
            if not files:
                absent.setdefault(blob, []).append(s)
            else:
                unexported.setdefault(blob, []).append((s, files[0]))

    print("=== PRESENT BUT NOT EXPORTED (one-line fix each) ===")
    n = 0
    for blob, items in sorted(unexported.items()):
        print("\n%s  (%d)" % (blob, len(items)))
        for s, f in items:
            n += 1
            print("   %-44s %s" % (s, f))
    print("\ntotal unexported-but-present: %d" % n)

    print("\n=== ABSENT FROM THE TREE (needs implementation) ===")
    m = 0
    for blob, items in sorted(absent.items()):
        print("%-28s %3d  %s" % (blob, len(items), ", ".join(items[:10])))
        m += len(items)
    print("total absent: %d" % m)


if __name__ == "__main__":
    sys.exit(main())
