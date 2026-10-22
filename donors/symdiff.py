"""Per-module stock-vs-built symbol diff, reusing verify_deblob2's rules.

Usage: python3 donors/symdiff.py sc27xx_adc audio_sipc nvmem-sc27xx-efuse
Prints the missing list (and the reverse, "extra") for each module.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from verify_deblob2 import is_artefact, strip_clone, nm, BUILT  # noqa: E402

STOCK_DIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"


def find_built(name):
    for root, _, fs in os.walk(BUILT):
        if name + ".ko" in fs:
            return os.path.join(root, name + ".ko")
    return None


def main(names):
    for n in names:
        sp = os.path.join(STOCK_DIR, n + ".ko")
        bp = find_built(n)
        if not os.path.exists(sp) or not bp:
            print(f"== {n}: stock={os.path.exists(sp)} built={bool(bp)}")
            continue
        sd, su = nm(sp)
        bd, bu = nm(bp)
        s_api = {strip_clone(x) for x in sd if not is_artefact(x)}
        b_api = {strip_clone(x) for x in bd if not is_artefact(x)}
        miss = sorted(s_api - b_api)
        extra = sorted(b_api - s_api)
        print(f"== {n}: {len(miss)} missing, {len(extra)} extra "
              f"(built at {bp})")
        for m in miss:
            print(f"   - {m}")
        if "-v" in sys.argv:
            for m in extra:
                print(f"   + {m}")


if __name__ == "__main__":
    main([a for a in sys.argv[1:] if not a.startswith("-")])