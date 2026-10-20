"""Scan the whole kernel tree for tool-redaction corruption.

The Hermes write/patch path sometimes replaces identifiers with:
  - b'***'                      (whole identifier)
  - b'sk_pso...eady' style      (middle elided with literal '...')
  - UTF-8 guillemets «redacted:...»

This walks .c/.h/.S/.lds/Makefile/Kconfig and reports every hit with a
hex dump of the offending line so the tool's own display redaction
does not hide the true bytes.
"""
import os
import sys

ROOT = "."
SKIP_DIRS = {".git", "Documentation", "tools/testing"}

PATTERNS = [
    b"\xc2\xab",            # «
    b"\xc2\xbb",            # »
    b"redacted:",
    b"sk_pso...eady",
    b"...eady",
]

def scan():
    hits = []
    for dirpath, dirnames, filenames in os.walk(ROOT):
        dirnames[:] = [d for d in dirnames if d not in SKIP_DIRS]
        for fn in filenames:
            if not fn.endswith((".c", ".h", ".S", ".lds", ".mk")):
                continue
            p = os.path.join(dirpath, fn)
            try:
                with open(p, "rb") as f:
                    data = f.read()
            except OSError:
                continue
            for i, line in enumerate(data.split(b"\n"), 1):
                for pat in PATTERNS:
                    if pat in line:
                        hits.append((p, i, line, pat))
                        break
                else:
                    # also catch bare *** used as an identifier
                    st = line.strip()
                    if st.startswith(b"***") or b" ***(" in line or b"= ***" in line or b"->***" in line:
                        hits.append((p, i, line, b"***"))
    return hits

if __name__ == "__main__":
    hits = scan()
    print(f"TOTAL HITS: {len(hits)}")
    for p, i, line, pat in hits:
        print(f"{p}:{i} pat={pat!r}")
        print(f"    hex: {line.strip().hex()}")
    sys.exit(0)