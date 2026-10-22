"""Extract the embedded IKCFG config blob from a kernel Image.

IKCFG_ST marker layout (kernel/configs.c):
    "IKCFG_ST"  -> gzip/xz compressed .config
    "IKCFG_ED"  -> end marker
The compressed blob starts a few bytes after the "IKCFG_ST" marker.
"""
import gzip
import lzma
import os
import re
import sys


def extract(path):
    data = open(path, "rb").read()
    print(f"{path}: {len(data)} bytes")

    for marker in (b"IKCFG_ST",):
        idx = 0
        while True:
            i = data.find(marker, idx)
            if i < 0:
                break
            idx = i + 1
            print(f"  {marker.decode()} @ 0x{i:x}")
            # scan forward for a compression magic
            for probe in range(i, min(i + 64, len(data))):
                blob = data[probe:probe + 8]
                if blob[:2] == b"\x1f\x8b":
                    try:
                        cfg = gzip.decompress(data[probe:])
                        print(f"    GZIP at +{probe-i}: {len(cfg)} bytes")
                        return cfg
                    except Exception as e:
                        print(f"    gzip at +{probe-i} failed: {e}")
                if blob == b"\xfd7zXZ\x00":
                    try:
                        cfg = lzma.decompress(data[probe:])
                        print(f"    XZ at +{probe-i}: {len(cfg)} bytes")
                        return cfg
                    except Exception as e:
                        print(f"    xz at +{probe-i} failed: {e}")
    return None


def main():
    out_dir = "E:/vxtux_build_repo/donors/stock_kernel"
    os.makedirs(out_dir, exist_ok=True)
    for path in sys.argv[1:]:
        cfg = extract(path)
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
            # print a few recognisable markers
            for key in ("CONFIG_LOCALVERSION", "CONFIG_SPRD_",
                        "CONFIG_ARM64", "CONFIG_PANFROST",
                        "CONFIG_MALI", "CONFIG_GONDUL"):
                hits = [l for l in lines if l.startswith(key)]
                if hits:
                    print(f"      {hits[0][:90]}")
        else:
            print("  no config extracted")
        print()


if __name__ == "__main__":
    main()
