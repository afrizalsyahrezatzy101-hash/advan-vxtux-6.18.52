"""Extract, for every (soc, block, op) in the stock apsys-dvfs.ko, the
register offset the op writes to (or reads from).

The per-SoC op clones are byte-for-byte identical apart from the
register offset, so the offsets are the only thing that has to be
transcribed faithfully.

    python ko_regoff.py <blob.ko>
"""
import json
import os
import re
import subprocess
import sys

D = r"E:\vxtux_build_repo\donors\ghidra_out"
TABLES = r"E:\vxtux_build_repo\donors\optbl.json"


def body(fn):
    p = os.path.join(D, fn + ".c")
    if not os.path.exists(p):
        return None
    t = open(p, encoding="utf-8", errors="replace").read()
    return t.split("\n\n", 1)[-1]


def canon(t):
    t = re.sub(r"_\d+\(", "_(", t)
    t = re.sub(r"\b_?DAT_\w+", "X", t)
    t = re.sub(r"\bmap_table_\w+", "X", t)
    t = re.sub(r"\s+", " ", t)
    return t


def main():
    tabs = json.load(open(TABLES))
    blocks = {}
    for t, slots in tabs.items():
        m = re.match(r"^(.*)_(apsys|dpu|vsp|vdsp|gsp)_dvfs_ops$", t)
        blocks.setdefault(m.group(2), {})[m.group(1)] = slots

    out = {}
    for blk, socs in blocks.items():
        for soc, slots in socs.items():
            key = f"{soc}_{blk}"
            out[key] = []
            for fn in slots:
                if fn.startswith("<"):
                    out[key].append(None)
                    continue
                b = body(fn)
                if b is None:
                    out[key].append({"fn": fn, "body": None})
                    continue
                # the register offset: the last "+ 0x..." in the body
                offs = re.findall(r"\+\s*(0x[0-9a-f]+)\)", b)
                out[key].append({
                    "fn": fn,
                    "canon": canon(b),
                    "offs": offs,
                })
    json.dump(out, open(r"E:\vxtux_build_repo\donors\regoff.json", "w"),
              indent=1)

    # report: for each block, the distinct op bodies (canon) and the
    # offsets each SoC uses
    for blk, socs in blocks.items():
        print(f"######## {blk}")
        n = len(next(iter(socs.values())))
        for i in range(n):
            variants = {}
            for soc, slots in socs.items():
                e = out[f"{soc}_{blk}"][i]
                if e is None:
                    variants.setdefault("NULL", []).append((soc, None))
                elif e.get("canon") is None:
                    variants.setdefault("MISSING", []).append((soc, e["fn"]))
                else:
                    variants.setdefault(e["canon"], []).append(
                        (soc, tuple(e["offs"])))
            if len(variants) == 1:
                k = list(variants)[0]
                if k == "NULL":
                    print(f"  [{i:2d}] NULL")
                else:
                    print(f"  [{i:2d}] SAME: {variants[k]}")
            else:
                print(f"  [{i:2d}] {len(variants)} variants:")
                for k, v in variants.items():
                    if k in ("NULL", "MISSING"):
                        print(f"        {k}: {v}")
                    else:
                        print(f"        {len(v)}x {v}")


if __name__ == "__main__":
    main()
