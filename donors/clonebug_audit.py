"""What strip_clone()'s bug costs: count symbols erased by the .sub("",...).

CLONE_RE is anchored with ^...$ but the replacement is "", so any symbol
carrying a ".<digits>" suffix is deleted whole instead of having the suffix
trimmed.  Show how many symbols that swallows per module.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from verify_deblob2 import (ARTEFACT_PREFIX, BUILT, CLONE_RE, ENTRY,  # noqa
                            is_artefact, nm, PART_RE, STOCK_DIR, STOCK_LIST)

GOOD = re.compile(r"^.+?\.\d+(?:\.constprop\.\d+)?$")


def base(s):
    s = PART_RE.sub(r"\1", s)
    m = re.match(r"^(.*)\.\d+(?:\.constprop\.\d+)?$", s)
    return m.group(1) if m else s


stock = []
for line in open(STOCK_LIST, encoding="utf-8", errors="replace"):
    s = re.split(r"[\t|,;]", line.strip())[0].strip()
    if s:
        stock.append(s)

built = {}
for root, _, fs in os.walk(BUILT):
    for fn in fs:
        if fn.endswith(".ko"):
            built[fn[:-3]] = os.path.join(root, fn)

tot_bug = 0
for n in stock:
    sp, bp = os.path.join(STOCK_DIR, n + ".ko"), built.get(n)
    if not (os.path.exists(sp) and bp):
        continue
    for tag, p in (("stock", sp), ("built", bp)):
        d, _ = nm(p)
        if d is None:
            continue
        erased = [x for x in d if not is_artefact(x)
                  and GOOD.match(x) and not x.endswith("__crc")]
        if erased:
            print(f"{n:24s} {tag:5s} {len(erased):3d} erased by the bug: "
                  f"{sorted(erased)[:6]}")
            tot_bug += len(erased)
print("total symbols erased:", tot_bug)