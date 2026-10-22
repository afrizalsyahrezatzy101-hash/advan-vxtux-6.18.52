"""Extract the stock vendor ramdisk modules from the VXTux codespace.

The vendor ramdisk cpio inside the SPRD vendor_boot image uses the
"070701" newc header but the header fields are ASCII hex digits (not
binary), and the file names are plain text (no NUL terminator, the
length comes from the namesize field).

Parse that variant and write every .ko out, with a manifest.
"""
import os
import sys

CPIO = "/workspaces/vxtux-build/stock_kernel/vb.out"
OUTDIR = "/workspaces/vxtux-build/stock_kernel/vmods"
MANIFEST = "/workspaces/vxtux-build/stock_kernel/stock_modules.txt"


def parse_ascii_newc(d):
    """Walk an ASCII-hex newc cpio. Yields (name, blob, mode)."""
    off = 0
    n = len(d)
    while off + 110 <= n:
        if d[off:off + 6] != b"070701":
            break
        try:
            hdr = d[off:off + 110].decode("ascii")
            f = [int(hdr[i * 8:(i + 1) * 8], 16) for i in range(13)]
        except Exception:
            break
        mode = f[1]
        filesize = f[6]
        namesize = f[11]
        name_off = off + 110
        name = d[name_off:name_off + namesize].decode("utf-8", "replace")
        data_off = name_off + namesize
        if name.startswith("TRAILER"):
            break
        blob = d[data_off:data_off + filesize]
        yield name, blob, mode
        off = data_off + filesize
        # no padding in this variant (fields are ASCII, offsets stay flat)


def main():
    os.makedirs(OUTDIR, exist_ok=True)
    d = open(CPIO, "rb").read()
    print(f"cpio: {len(d)} bytes")

    mods = []
    total = 0
    allfiles = 0
    for name, blob, mode in parse_ascii_newc(d):
        allfiles += 1
        base = os.path.basename(name)
        if ".ko" in base:
            p = os.path.join(OUTDIR, base)
            with open(p, "wb") as g:
                g.write(blob)
            mods.append((base, len(blob), name, mode))
            total += len(blob)

    print(f"entries walked: {allfiles}")
    print(f"modules extracted: {len(mods)}, {total:,} bytes")
    for k, s, nm, mode in sorted(mods, key=lambda x: -x[1])[:15]:
        print(f"  {k:<48} {s:>10,}")

    with open(MANIFEST, "w") as g:
        for k, s, nm, mode in sorted(mods):
            g.write(f"{k[:-3]}\t{s}\t{nm}\n")
    print(f"wrote {MANIFEST}")

    # also a plain list for the comparison tool
    with open(MANIFEST + ".names", "w") as g:
        for k, s, nm, mode in sorted(mods):
            g.write(f"{k[:-3]}\n")


if __name__ == "__main__":
    main()
