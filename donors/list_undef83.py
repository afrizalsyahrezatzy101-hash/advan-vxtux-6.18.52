import os
import re
import subprocess
from collections import Counter

LOG = "/workspaces/codespace_build83.log"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

r = subprocess.run(["bash", "-lc", f"grep -o \"undefined reference to .[^']*'\" {LOG} | "
                                  f"sed \"s/undefined reference to .//; s/.$//\" | sort -u"],
                   capture_output=True, text=True, timeout=300)
syms = [s.strip() for s in r.stdout.strip().splitlines() if s.strip()]
print(f"=== {len(syms)} unique undefined symbols ===")
for s in syms:
    print("  ", s)

# for each, look for a definition in the tree
print()
print("=== search for a definition for each ===")
for s in syms:
    q = subprocess.run(
        ["bash", "-lc",
         f"cd {ROOT} && grep -rn --include=*.c --include=*.h "
         f"-E '(^|[^A-Za-z0-9_]){s}([^A-Za-z0-9_]|$)' . 2>/dev/null | "
         f"grep -v 'berry_compat' | head -4"],
        capture_output=True, text=True, timeout=300)
    lines = [l for l in q.stdout.strip().splitlines() if l.strip()]
    # a real definition looks like "type name(...)" at line start
    defs = [l for l in lines
            if re.search(r"\b" + re.escape(s) + r"\s*\(", l)
            and not l.strip().startswith("//")
            and "=" not in l.split(s)[0][-1:]]
    print(f"--- {s} ---")
    if not lines:
        print("    NO REFERENCE ANYWHERE (pure stub needed)")
    for l in lines[:4]:
        print("   ", l.strip()[:160])
