"""Compare a VXTux-built kernel against the stock SPRD kernel.

Three comparison axes, all reproducible:

  1. CONFIG   - the .config used for the build vs the stock kernel's
                embedded config (IKCFG / "kernel config" magic) and the
                config strings baked into the Image via IKCFG_ST.
  2. SYMBOLS  - the kallsyms symbol table embedded in both Images
                (parsed with the standard kallsyms format) so we can
                diff the exported symbol sets.
  3. STRINGS  - feature strings / driver banners present in each.

Usage:  python3 compare_kernels.py <vxtux_Image> [stock_Image ...]
"""
import gzip
import lzma
import os
import re
import struct
import sys

KALLSYMS_MAGIC = b"\x6b\x73\x79\x6d"  # "ksym"
IKCFG_MAGIC = b"IKCFG_ST"


def find_all(data, needle, limit=64):
    out = []
    idx = 0
    while len(out) < limit:
        i = data.find(needle, idx)
        if i < 0:
            break
        out.append(i)
        idx = i + 1
    return out


def extract_ikcfg(data):
    """Extract the embedded .config (gzip or xz compressed)."""
    for off in find_all(data, IKCFG_MAGIC, 8):
        tail = data[off + 8: off + 16]
        # IKCFG_ST\0 then compression magic
        for comp, dec in (("gzip", gzip.decompress),
                          ("xz", lzma.decompress),
                          ("zlib", None)):
            for probe in range(off + 8, off + 64):
                blob = data[probe:probe + 2]
                if comp == "gzip" and blob[:2] == b"\x1f\x8b":
                    try:
                        return dec(data[probe:]), f"gzip@{probe}"
                    except Exception:
                        pass
                if comp == "xz" and blob == b"\xfd7zXZ\x00":
                    try:
                        return lzma.decompress(data[probe:]), f"xz@{probe}"
                    except Exception:
                        pass
    return None, None


def parse_kallsyms(data):
    """Parse the embedded kallsyms table.

    Layout (kernel/sched/kallsyms.c):
        u32 token_table_offset
        u32 token_table_size
        u16 num_tokens
        u16 token_index_shift
        u32 token_index_offset
        u32 markers_offset
        u32 symbols_offset
        u32 symbols_count   (u32: size / sizeof(relative_symbol))
        u32 names_offset
        u32 names_size
        u32 name_num
        u32 markers_offset2
        u32 markers_end
    All offsets are relative to the start of the kallsyms_offsets table.
    """
    for off in find_all(data, KALLSYMS_MAGIC, 8):
        # the token table follows the magic
        base = off
        try:
            (tt_off, tt_size) = struct.unpack_from("<II", data, base + 4)
            num_tokens = struct.unpack_from("<H", data, base + 12)[0]
            shift = struct.unpack_from("<H", data, base + 14)[0]
            ti_off = struct.unpack_from("<I", data, base + 16)[0]
            markers_off = struct.unpack_from("<I", data, base + 20)[0]
            symbols_off = struct.unpack_from("<I", data, base + 24)[0]
            sym_count = struct.unpack_from("<I", data, base + 28)[0]
            names_off = struct.unpack_from("<I", data, base + 32)[0]
            names_size = struct.unpack_from("<I", data, base + 36)[0]
            name_num = struct.unpack_from("<I", data, base + 40)[0]
        except Exception:
            continue

        if num_tokens == 0 or num_tokens > 0x10000 or sym_count == 0:
            continue
        if sym_count > 5_000_000:
            continue

        # the whole table starts at 'base' (the token table is right after)
        # token_table is at base+4? Actually the layout is:
        #   [0..3] token_table_offset  (points to token table)
        #   ...
        tt = base + tt_off
        if tt + tt_size > len(data):
            continue
        token_data = data[tt:tt + tt_size]

        # token index table
        ti = base + ti_off
        if ti + num_tokens * 2 > len(data):
            continue
        token_index = struct.unpack_from(f"<{num_tokens}H", data, ti)

        # names
        nm = base + names_off
        if nm + names_size > len(data):
            continue
        names = data[nm:nm + names_size]

        # markers
        mk = base + markers_off
        # symbols: array of relative u32 offsets into names
        sy = base + symbols_off
        if sy + sym_count * 4 > len(data):
            continue
        sym_offsets = struct.unpack_from(f"<{sym_count}I", data, sy)

        # marker count
        try:
            num_markers = (sy - (base + markers_off)) // 4
        except Exception:
            num_markers = 0
        if num_markers <= 0 or num_markers > 2_000_000:
            num_markers = 0

        out = set()
        # relative_symbol:  u32 value (offset from name) in bits[0:24],
        #                   u8 type in bits[24:28], u8 bind bits[28:30]
        for i, rel in enumerate(sym_offsets):
            # markers give a rolling base offset into names
            # base_offset = markers[i // 256]
            if num_markers:
                mi = i // 256
                if mi < num_markers:
                    base_off = struct.unpack_from(
                        "<I", data, base + markers_off + mi * 4)[0]
                else:
                    base_off = 0
            else:
                base_off = 0
            name_off = base_off + (rel & 0xFFFFFF)
            if name_off >= len(names):
                continue
            end = names.find(b"\x00", name_off)
            if end < 0:
                continue
            name = names[name_off:end].decode("ascii", "replace")
            if name:
                out.add(name)
        if len(out) > 100:
            return out, off
    return set(), None


def summarize(path):
    print("=" * 78)
    print(path)
    sz = os.path.getsize(path)
    data = open(path, "rb").read()
    print(f"  size {sz} bytes ({sz/1024/1024:.2f} MiB)")

    cfg, how = extract_ikcfg(data)
    if cfg:
        n = len(cfg.decode("utf-8", "replace").splitlines())
        print(f"  embedded config: {how}, {n} lines")
    else:
        print("  embedded config: NOT FOUND")

    syms, off = parse_kallsyms(data)
    if syms:
        print(f"  kallsyms @{off}: {len(syms)} symbol names")
    else:
        print("  kallsyms: NOT FOUND")
    return data, cfg, syms


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    vxtux = sys.argv[1]
    stocks = sys.argv[2:]

    vdata, vcfg, vsyms = summarize(vxtux)
    for s in stocks:
        summarize(s)

    if not stocks:
        return
    sdata, scfg, ssyms = summarize(stocks[0])

    if vcfg and scfg:
        vl = set(l for l in vcfg.decode("utf-8", "replace").splitlines()
                 if l and not l.startswith("#"))
        sl = set(l for l in scfg.decode("utf-8", "replace").splitlines()
                 if l and not l.startswith("#"))
        only_v = vl - sl
        only_s = sl - vl
        print()
        print("=" * 78)
        print("CONFIG DIFF (vxtux vs stock)")
        print(f"  vxtux set: {len(vl)}   stock set: {len(sl)}   "
              f"common: {len(vl & sl)}")
        print(f"  only in vxtux: {len(only_v)}")
        print(f"  only in stock: {len(only_s)}")
        for l in sorted(only_v)[:40]:
            print(f"    + {l}")
        if len(only_v) > 40:
            print(f"    ... +{len(only_v)-40} more")
        for l in sorted(only_s)[:40]:
            print(f"    - {l}")
        if len(only_s) > 40:
            print(f"    ... +{len(only_s)-40} more")

    if vsyms and ssyms:
        print()
        print("=" * 78)
        print("SYMBOL DIFF (vxtux vs stock)")
        print(f"  vxtux: {len(vsyms)}   stock: {len(ssyms)}   "
              f"common: {len(vsyms & ssyms)}")
        only_v = vsyms - ssyms
        only_s = ssyms - vsyms
        print(f"  only in vxtux: {len(only_v)}")
        print(f"  only in stock: {len(only_s)}")
        with open("E:/vxtux_build_repo/donors/sym_only_vxtux.txt", "w") as f:
            f.write("\n".join(sorted(only_v)))
        with open("E:/vxtux_build_repo/donors/sym_only_stock.txt", "w") as f:
            f.write("\n".join(sorted(only_s)))
        with open("E:/vxtux_build_repo/donors/sym_common.txt", "w") as f:
            f.write("\n".join(sorted(vsyms & ssyms)))
        print("  wrote sym_only_vxtux.txt / sym_only_stock.txt / sym_common.txt")


if __name__ == "__main__":
    main()
