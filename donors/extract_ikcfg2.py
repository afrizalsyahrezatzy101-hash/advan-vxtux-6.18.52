"""Extract the embedded IKCFG config blob from a kernel Image.

In the SPRD/UMS512 kernel the layout is:

    <gzip blob> IKCFG_ST IKCFG_ED

i.e. the compression markers trail the compressed data instead of
leading it (the opposite of the upstream CONFIG_IKCONFIG layout where
the marker precedes the blob). So we scan backwards from the marker for
the gzip magic.
"""
import gzip
import lzma
import os
import sys
import zlib

GZIP_WBITS = 16 + zlib.MAX_WBITS


def try_blob(data, start, end):
    """Try to decompress data[start:end] as gzip or xz."""
    for probe in range(start, end):
        blob = data[probe:probe + 8]
        if blob[:2] == b"\x1f\x8b":
            try:
                out = zlib.decompressobj(GZIP_WBITS)
                res = out.decompress(data[probe:])
                res += out.flush()
                if len(res) > 100:
                    return res, f"gzip@0x{probe:x}"
            except Exception:
                pass
        if blob == b"\xfd7zXZ\x00":
            try:
                res = lzma.decompress(data[probe:])
                if len(res) > 100:
                    return res, f"xz@0x{probe:x}"
            except Exception:
                pass
    return None, None


def extract(path):
    data = open(path, "rb").read()
    print(f"{path}: {len(data)} bytes")

    idx = 0
    while True:
        i = data.find(b"IKCFG_ST", idx)
        if i < 0:
            break
        idx = i + 1
        print(f"  IKCFG_ST @ 0x{i:x}")

        # the compressed blob ends right before the marker
        for back in (0, 1, 2, 3, 4, 8, 16):
            start = i - back
            res, how = try_blob(data, max(0, i - 0x200000), i + 4)
            if res:
                print(f"    {how}: {len(res)} bytes")
                return res, how
            break

        # fallback: the blob starts right after the marker
        res, how = try_blob(data, i, min(len(data), i + 0x200000))
        if res:
            print(f"    {how}: {len(res)} bytes")
            return res, how

    # Last resort: find every gzip stream in the file and see if any
    # decompresses to something that looks like a kernel .config
    print("  scanning all gzip streams...")
    pos = 0
    while True:
        j = data.find(b"\x1f\x8b\x08", pos)
        if j < 0:
            break
        pos = j + 1
        try:
            out = zlib.decompressobj(GZIP_WBITS)
            res = out.decompress(data[j:j + 0x400000])
            res += out.flush()
        except Exception:
            continue
        if len(res) > 2000:
            head = res[:200].decode("utf-8", "replace")
            if "CONFIG_" in head and "=" in head:
                print(f"    gzip@0x{j:x} -> {len(res)} bytes LOOKS LIKE CONFIG")
                return res, f"gzip@0x{j:x}"
    return None, None


def main():
    out_dir = "E:/vxtux_build_repo/donors/stock_kernel"
    os.makedirs(out_dir, exist_ok=True)
    for path in sys.argv[1:]:
        cfg, how = extract(path)
        if cfg:
            text = cfg.decode("utf-8", "replace")
            name = os.path.basename(path)
            p = os.path.join(out_dir, f"{name}.config")
            with open(p, "w", encoding="utf-8", errors="replace") as f:
                f.write(text)
            lines = text.splitlines()
            on = [l for l in lines if l and not l.startswith("#")]
            print(f"    -> {len(lines)} lines, {len(on)} options set")
            print(f"    -> saved {p}")
            for key in ("CONFIG_LOCALVERSION", "CONFIG_ARM64",
                        "CONFIG_PANFROST", "CONFIG_MALI",
                        "CONFIG_SPRD"):
                hits = [l for l in lines if l.startswith(key)]
                if hits:
                    print(f"      {hits[0][:90]}")
        else:
            print("  no config extracted")
        print()


if __name__ == "__main__":
    main()
