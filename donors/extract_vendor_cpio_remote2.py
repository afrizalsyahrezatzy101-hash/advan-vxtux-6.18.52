"""Extract the stock vendor ramdisk modules (ASCII newc cpio variant).

The SPRD vendor ramdisk uses a newc cpio whose header fields are ASCII
hex digits. The header is 110 bytes:

    6 bytes  "070701"            magic
    13 x 8   ASCII hex fields    ino, mode, uid, gid, nlink, mtime,
                                filesize, devmajor, devminor,
                                rdevmajor, rdevminor, namesize, check
    then the name (namesize bytes, NUL terminated), then the file data.

The first field therefore starts at byte 6, not byte 0.
"""
import os
import sys

CPIO = sys.argv[1] if len(sys.argv) > 1 else \
    "/workspaces/vxtux-build/stock_kernel/vb.out"
OUTDIR = sys.argv[2] if len(sys.argv) > 2 else \
    "/workspaces/vxtux-build/stock_kernel/vmods"
MANIFEST = "/workspaces/vxtux-build/stock_kernel/stock_modules.txt"


def walk(d):
    """Yield (name, blob, mode) from an ASCII newc cpio."""
    off = 0
    n = len(d)
    while off + 110 <= n:
        if d[off:off + 6] != b"070701":
            break
        try:
            fields = [int(d[off + 6 + i * 8: off + 14 + i * 8], 16)
                      for i in range(13)]
        except Exception:
            break
        mode = fields[1]
        filesize = fields[6]
        namesize = fields[11]
        name_off = off + 110
        name = d[name_off:name_off + namesize - 1].decode("utf-8", "replace")
        data_off = name_off + namesize
        if name.startswith("TRAILER"):
            break
        blob = d[data_off:data_off + filesize]
        yield name, blob, mode
        # next header: try unaligned, then 4-byte aligned
        nxt = data_off + filesize
        for cand in (nxt, (nxt + 3) & ~3, nxt + 1, nxt + 2):
            if d[cand:cand + 6] == b"070701":
                off = cand
                break
        else:
            break


def main():
    os.makedirs(OUTDIR, exist_ok=True)
    d = open(CPIO, "rb").read()
    print(f"cpio: {len(d)} bytes")

    mods = []
    total = 0
    n_entries = 0
    for name, blob, mode in walk(d):
        n_entries += 1
        base = os.path.basename(name)
        if ".ko" in base:
            with open(os.path.join(OUTDIR, base), "wb") as g:
                g.write(blob)
            mods.append((base, len(blob), name, mode))
            total += len(blob)

    print(f"entries walked : {n_entries}")
    print(f"modules        : {len(mods)}  ({total:,} bytes)")
    for k, s, nm, mode in sorted(mods, key=lambda x: -x[1])[:15]:
        print(f"  {k:<48} {s:>10,}")

    with open(MANIFEST, "w") as g:
        for k, s, nm, mode in sorted(mods):
            g.write(f"{k[:-3]}\t{s}\t{nm}\n")
    with open(MANIFEST + ".names", "w") as g:
        for k, s, nm, mode in sorted(mods):
            g.write(f"{k[:-3]}\n")
    print(f"wrote {MANIFEST}")


if __name__ == "__main__":
    main()
