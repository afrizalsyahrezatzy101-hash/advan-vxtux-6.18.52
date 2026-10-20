"""Compare the VXTux-built kernel Image against the stock SPRD kernel Image.

Axes:
  1. arm64 Image header (code0/code1, text_offset, image_size, flags)
  2. Embedded .config (IKCFG) - option-by-option diff
  3. Driver banner strings - which subsystems are present in each
  4. Section/symbol census via the kernel's own symbol table if present

Usage:
    python3 compare_images.py <vxtux_Image> <stock_Image> [-o report.md]
"""
import hashlib
import os
import re
import struct
import sys
import zlib

GZIP_WBITS = 16 + zlib.MAX_WBITS

# Driver / subsystem banners that identify a subsystem in the image.
MARKERS = [
    ("SPRD modem", b"SPRD_MODEM"),
    ("SPRD cp_dump", b"cp_dump"),
    ("SPRD pmic", b"sprd_pmic"),
    ("SPRD sysdump", b"minidump_save_extend"),
    ("SPRD SIPC", b"sipc"),
    ("SPRD wcn", b"wcn"),
    ("Mali GPU", b"mali"),
    ("Panfrost GPU", b"panfrost"),
    ("Panfrost MJOLL", b"panfrost_mjoll"),
    ("DRM", b"drm"),
    ("extcon", b"extcon"),
    ("MUSB", b"musb"),
    ("USB PHY", b"usb_add_phy"),
    ("Himax touch", b"hxchipset"),
    ("Novatek touch", b"novatek"),
    ("sc27xx ADC", b"sc27xx_adc"),
    ("f2fs", b"f2fs"),
    ("btrfs", b"btrfs"),
    ("IPv6", b"tcp_ipv6"),
    ("netfilter", b"nf_conntrack"),
    ("io_uring", b"io_uring"),
    ("kexec", b"kexec"),
    ("KVM", b"kvm"),
    ("BPF", b"bpf_prog_run"),
]


def image_header(d):
    code0, code1 = struct.unpack_from("<II", d, 0)
    text_off, image_size, flags = struct.unpack_from("<QQQ", d, 8)
    magic = d[56:60]
    return {
        "code0": code0, "code1": code1, "text_offset": text_off,
        "image_size": image_size, "flags": flags, "magic": magic,
    }


def extract_ikcfg(d):
    """Return (config_text, how) or (None, None)."""
    idx = 0
    while True:
        i = d.find(b"IKCFG_ST", idx)
        if i < 0:
            break
        idx = i + 1
        # the compressed blob can precede or follow the marker
        for lo, hi in ((0, i), (i, min(len(d), i + 0x400000))):
            for probe in range(lo, hi):
                if d[probe:probe + 2] == b"\x1f\x8b":
                    try:
                        o = zlib.decompressobj(GZIP_WBITS)
                        out = o.decompress(d[probe:])
                        out += o.flush()
                        if len(out) > 1000:
                            return out.decode("utf-8", "replace"), f"gzip@0x{probe:x}"
                    except Exception:
                        pass
                if d[probe:probe + 6] == b"\xfd7zXZ\x00":
                    try:
                        import lzma
                        out = lzma.decompress(d[probe:])
                        if len(out) > 1000:
                            return out.decode("utf-8", "replace"), f"xz@0x{probe:x}"
                    except Exception:
                        pass
    return None, None


def parse_config(text):
    out = {}
    for line in text.splitlines():
        line = line.strip()
        if not line:
            continue
        if line.startswith("# CONFIG_") and line.endswith(" is not set"):
            out[line[2:line.index(" is not set")].strip()] = "n"
        elif line.startswith("#"):
            continue
        elif "=" in line:
            k, v = line.split("=", 1)
            out[k.strip()] = v.strip()
    return out


def scan_markers(d):
    found = {}
    for name, sig in MARKERS:
        if sig in d:
            found[name] = d.count(sig)
    return found


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return
    vpath, spath = sys.argv[1], sys.argv[2]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    v = open(vpath, "rb").read()
    s = open(spath, "rb").read()

    vh, sh = image_header(v), image_header(s)
    vcfg_text, vhow = extract_ikcfg(v)
    scfg_text, show = extract_ikcfg(s)
    vcfg = parse_config(vcfg_text) if vcfg_text else {}
    scfg = parse_config(scfg_text) if scfg_text else {}
    vm = scan_markers(v)
    sm = scan_markers(s)

    L = []
    L.append("# Kernel Image Comparison — VXTux 6.18.52 vs Stock SPRD\n")
    L.append("## 1. arm64 Image header\n")
    L.append("| field | VXTux | stock |")
    L.append("|---|---|---|")
    for k in ("code0", "code1", "text_offset", "image_size", "flags"):
        L.append(f"| {k} | `0x{vh[k]:x}` | `0x{sh[k]:x}` |")
    L.append(f"| magic | `{vh['magic']!r}` | `{sh['magic']!r}` |")
    L.append(f"| file size | {len(v):,} | {len(s):,} |")
    L.append(f"| sha256 | `{hashlib.sha256(v).hexdigest()}` | "
             f"`{hashlib.sha256(s).hexdigest()}` |")
    L.append("")

    L.append("## 2. Embedded .config\n")
    L.append(f"- VXTux: {'present (' + vhow + ')' if vcfg_text else 'NOT FOUND'}"
             f" — {len(vcfg)} options")
    L.append(f"- stock: {'present (' + show + ')' if scfg_text else 'NOT FOUND'}"
             f" — {len(scfg)} options")
    if vcfg and scfg:
        only_v = sorted(set(vcfg) - set(scfg))
        only_s = sorted(set(scfg) - set(vcfg))
        diff = sorted(k for k in set(vcfg) & set(scfg)
                      if vcfg[k] != scfg[k])
        L.append(f"- common: {len(set(vcfg) & set(scfg))}, "
                 f"same value: {len(set(vcfg) & set(scfg)) - len(diff)}, "
                 f"different value: {len(diff)}")
        L.append(f"- only in VXTux: {len(only_v)}")
        L.append(f"- only in stock: {len(only_s)}")
        L.append("")
        L.append("### 2a. Same symbol, different value\n")
        L.append("| option | VXTux | stock |")
        L.append("|---|---|---|")
        for k in diff:
            L.append(f"| `{k}` | `{vcfg[k]}` | `{scfg[k]}` |")
        L.append("")
        L.append("### 2b. Only in stock (vendor features the VXTux build dropped)\n")
        for k in only_s:
            L.append(f"- `{k}` = `{scfg[k]}`")
        L.append("")
        L.append("### 2c. Only in VXTux (mainline options the stock build never had)\n")
        for k in only_v:
            L.append(f"- `{k}` = `{vcfg[k]}`")
        L.append("")

    L.append("## 3. Subsystem markers present in the image\n")
    L.append("| subsystem | VXTux | stock |")
    L.append("|---|---|---|")
    for name, _ in MARKERS:
        a = vm.get(name, 0)
        b = sm.get(name, 0)
        mark = lambda n: ("yes" if n else "—")
        L.append(f"| {name} | {mark(a)} ({a}) | {mark(b)} ({b}) |")
    L.append("")

    text = "\n".join(L)
    print(text[:3000])
    print()
    print("=" * 70)
    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write(text)
        print(f"wrote {outpath} ({len(text):,} chars)")


if __name__ == "__main__":
    main()
