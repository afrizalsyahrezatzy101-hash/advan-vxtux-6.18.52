#!/usr/bin/env python3
"""Add the missing EXPORT_SYMBOL() lines for symbols that already exist in
the tree but are not exported, so a module that imports them can link.

Usage:
    python3 donors/export_missing.py donors/bb_Module.symvers            # report
    python3 donors/export_missing.py donors/bb_Module.symvers --apply     # patch

Only symbols that are genuinely defined in the tree are exported; each
EXPORT_SYMBOL is placed right after the definition's closing brace so the
export sits in the same translation unit.
"""
import os
import re
import subprocess
import sys

NM = r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe"
ROOT = "6.18.52-VXTux-UMS512"
BLOBS = "donors/stock_vendor_modules"

SKIP = re.compile(
    r"^(__cfi|__this_module$|__mod_of__|__typeid__|__crc_|__kstrtab_|__ksymtab_"
    r"|__UNIQUE_ID|__warned|__key|__param|__exit|__pfx_|__func__|__used"
    r"|__stack_chk|__fentry__|__llvm|__mod|__this|__exidx|__gcc_except"
    r"|\.L|cleanup_module$|init_module$)"
)

# blob -> (tree module path substrings, the symbols that must live in it)
MAPPING = {
    "aes-ce-ccm.ko": ["ce_aes_ccm"],
    "apsys-dvfs.ko": ["apsys_dvfs", "dpu_dvfs"],
    "audio-dsp-dump.ko": ["audio_dsp_dump", "audio_sblock"],
    "audio_sipc.ko": ["audio_sipc"],
    "mmdvfs.ko": ["sprd_mm_dvfs"],
    "novatek_ts_spi.ko": ["novatek_nvt_ts_spi", "nt36"],
    "sdhci-sprd.ko": ["sdhci_sprd", "sdhci-sprd"],
    "sprd-dmaengine-pcm.ko": ["sprd_dmaengine_pcm"],
    "sprd_flash_drv.ko": ["sprd_flash"],
    "sprd_time_sync.ko": ["sprd_time_sync"],
    "trusty.ko": ["trusty_core", "trusty-mem"],
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


INDEX_PATH = "donors/.defindex.json"


def load_index():
    """symbol -> file that defines it as a global (not static)."""
    import json
    if os.path.exists(INDEX_PATH):
        with open(INDEX_PATH, encoding="utf-8") as fh:
            return json.load(fh)
    return {}


def find_defs(sym, index):
    """Return the file that defines `sym` as a global (not static)."""
    p = index.get(sym)
    return (p, None) if p else (None, None)


def export_at_end(path, syms):
    raw = open(path, "rb").read()
    nl = b"\r\n" if b"\r\n" in raw else b"\n"
    text = raw.decode("utf-8", errors="replace")
    todo = [s for s in syms
            if not re.search(r"EXPORT_SYMBOL(_GPL)?\(%s\)" % re.escape(s), text)]
    if not todo:
        return 0
    block = nl.join(("EXPORT_SYMBOL(%s);" % s).encode() for s in todo)
    open(path, "wb").write(raw + nl + block + nl)
    return len(todo)


def main():
    apply = "--apply" in sys.argv
    symvers = load_symvers(sys.argv[1])
    index = load_index()
    per_file = {}
    for blob, hints in sorted(MAPPING.items()):
        bp = os.path.join(BLOBS, blob)
        if not os.path.exists(bp):
            continue
        for s in blob_syms(bp):
            mod = symvers.get(s)
            if mod is not None and (mod == "vmlinux" or
                                    any(h in mod for h in hints)):
                continue
            p, _ = find_defs(s, index)
            if p:
                per_file.setdefault(p, []).append(s)

    total = 0
    for p, syms in sorted(per_file.items()):
        syms = sorted(set(syms))
        if apply:
            n = export_at_end(p, syms)
        else:
            n = len(syms)
        total += n
        print("%-4s %-70s %d" % ("APPLY" if apply else "WANT", p, n))
    print("\n%s %d symbols across %d files"
          % ("exported" if apply else "would export", total, len(per_file)))


if __name__ == "__main__":
    sys.exit(main())
