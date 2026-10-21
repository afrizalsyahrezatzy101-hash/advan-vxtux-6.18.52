"""Check a built .ko against the stock blob for a list of required symbols.

    python3 donors/check_syms.py <built.ko> <stock.ko> <sym> [<sym>...]

Prints which of the wanted symbols the build carries (after the
verifier's clone-stripping) and which are still missing.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from verify_deblob2 import is_artefact, nm, strip_clone  # noqa: E402


def main():
    built, stock = sys.argv[1], sys.argv[2]
    want = sys.argv[3:]
    bd, _ = nm(built)
    sd, _ = nm(stock)
    b = {strip_clone(x) for x in bd if not is_artefact(x)}
    s = {strip_clone(x) for x in sd if not is_artefact(x)}
    miss = []
    for w in want:
        if w in b:
            print(f"  OK      {w}")
        else:
            print(f"  MISSING {w}")
            miss.append(w)
    print(f"\n{len(want) - len(miss)}/{len(want)} present")
    if miss:
        print("still missing:", miss)
    return 1 if miss else 0


if __name__ == "__main__":
    sys.exit(main())
