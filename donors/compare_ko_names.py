"""Compare the .ko files the VXTux build produces against the stock
vendor .ko list, by NAME.

The deblob goal is: for every stock vendor blob, the VXTux build must
produce a module with the same name, so the loader can insmod it and
the blob is genuinely replaced rather than merely "not needed".

Usage:
    python3 compare_ko_names.py
"""
import os

BUILD = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"
STOCK = "E:/vxtux_build_repo/donors/stock_modules.txt"

# the stock ROM's vendor .ko list, as pulled from the device
STOCK_LINES = []


def load_stock():
    p = STOCK
    if not os.path.exists(p):
        # try the other known locations
        for alt in ("E:/vxtux_build_repo/donors/stock_modules.txt",
                    "E:/vxtux_build_repo/work/blob_map/stock_modules.txt",
                    "E:/vxtux_build_repo/donors/stock_ko_list.txt"):
            if os.path.exists(alt):
                p = alt
                break
    out = []
    with open(p, encoding="utf-8", errors="replace") as f:
        for line in f:
            s = line.strip()
            if not s or s.startswith("#"):
                continue
            # the file is 'name<TAB>size<TAB>path' or 'name|size|path'
            for sep in ("\t", "|", ",", ";"):
                if sep in s:
                    s = s.split(sep)[0]
                    break
            s = s.strip()
            if s.endswith(".ko"):
                s = s[:-3]
            if s:
                out.append(s)
    return p, sorted(set(out))


def main():
    sp, stock = load_stock()
    print(f"stock list: {sp} -> {len(stock)} modules\n")

    built = {}
    with open(os.path.join(BUILD, "ko_list.txt"), encoding="utf-8",
              errors="replace") as f:
        for line in f:
            parts = line.rsplit(" ", 1)
            if len(parts) != 2:
                continue
            path, size = parts[0].strip(), parts[1].strip()
            name = os.path.basename(path)
            if name.endswith(".ko"):
                name = name[:-3]
            built.setdefault(name, []).append((path, int(size)))

    print("=" * 84)
    print("STOCK VENDOR .ko  ->  VXTux BUILD")
    print("=" * 84)
    covered, missing = [], []
    for s in stock:
        if s in built:
            p, z = built[s][0]
            covered.append(s)
            print(f"  OK      {s:<34} {z:>10,}  {p}")
        else:
            missing.append(s)
            print(f"  MISSING {s:<34}")

    print()
    print("=" * 84)
    print(f"covered {len(covered)}/{len(stock)}  "
          f"missing {len(missing)}/{len(stock)}  "
          f"({100.0 * len(covered) / max(1, len(stock)):.1f}%)")
    print("=" * 84)
    if missing:
        print("\nstill missing:")
        for m in missing:
            print(f"   {m}")

    # the VXTux-only modules (not a problem, but worth knowing)
    extra = sorted(set(built) - set(stock))
    print(f"\nVXTux-only modules: {len(extra)}")


if __name__ == "__main__":
    main()
