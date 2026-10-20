"""Extract the stock vendor ramdisk modules (ASCII newc cpio variant).

The SPRD vendor ramdisk cpio has ASCII-hex newc headers but the file
offsets are NOT 4-byte aligned, so a sequential walk desynchronises.
Instead we locate every "070701" magic in the buffer and parse each
candidate header independently, which is robust to alignment.

For each entry whose name ends in .ko we write the blob out.
"""
import os
import sys

CPIO = sys.argv[1] if len(sys.argv) > 1 else \
    "/workspaces/vxtux-build/stock_kernel/vb.out"
OUTDIR = sys.argv[2] if len(sys.argv) > 2 else \
    "/workspaces/vxtux-build/stock_kernel/vmods"
MANIFEST = sys.argv[3] if len(sys.argv) > 3 else \
    "/workspaces/vxtux-build/stock_kernel/stock_modules.txt"


def parse_header(d, off):
    """Parse one ASCII newc header at off. Returns (fields, name) or None."""
    if d[off:off + 6] != b"070701":
        return None
    try:
        fields = [int(d[off + 6 + i * 8: off + 14 + i * 8], 16)
                  for i in range(13)]
    except Exception:
        return None
    filesize = fields[6]
    namesize = fields[11]
    if namesize == 0 or namesize > 4096:
        return None
    name = d[off + 110: off + 110 + namesize - 1]
    try:
        name = name.decode("utf-8")
    except Exception:
        return None
    if any(c < 32 or c > 126 for c in map(ord, name)):
        return None
    return fields, name


def main():
    os.makedirs(OUTDIR, exist_ok=True)
    d = open(CPIO, "rb").read()
    print(f"cpio: {len(d)} bytes")

    mods = {}
    n_entries = 0
    off = 0
    while True:
        i = d.find(b"070701", off)
        if i < 0:
            break
        off = i + 1
        r = parse_header(d, i)
        if r is None:
            continue
        fields, name = r
        n_entries += 1
        base = os.path.basename(name)
        if ".ko" not in base:
            continue
        filesize = fields[6]
        data_off = i + 110 + fields[11]
        blob = d[data_off:data_off + filesize]
        if len(blob) != filesize:
            continue
        # sanity: ELF module?
        if blob[:4] != b"\x7fELF":
            continue
        mods[base] = (len(blob), name, blob)

    print(f"entries parsed : {n_entries}")
    print(f"modules        : {len(mods)}")
    total = 0
    for base, (sz, name, blob) in sorted(mods.items(), key=lambda kv: -kv[1][0]):
        with open(os.path.join(OUTDIR, base), "wb") as g:
            g.write(blob)
        total += sz
    print(f"total bytes    : {total:,}")
    for base, (sz, name, _) in sorted(mods.items(), key=lambda kv: -kv[1][0])[:15]:
        print(f"  {base:<48} {sz:>10,}")

    with open(MANIFEST, "w") as g:
        for base, (sz, name, _) in sorted(mods.items()):
            g.write(f"{base[:-3]}\t{sz}\t{name}\n")
    with open(MANIFEST + ".names", "w") as g:
        for base in sorted(mods):
            g.write(f"{base[:-3]}\n")
    print(f"wrote {MANIFEST}")


if __name__ == "__main__":
    main()
