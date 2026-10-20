"""Scan the kernel tree for tool-redaction corruption, avoiding comment-separator
false positives.

Real corruption shapes seen so far:
  b'sk_pso...eady'          literal '...' inside an identifier
  b'\xc2\xabredacted:'     guillemet redaction marker
  b'***'                    whole identifier replaced (context: '= ***', '->***',
                            '***(', '***;', ' *** ')
A bare run of '*' used as a comment separator is NOT corruption.
"""
import os
import sys

ROOT = "."
SKIP_DIRS = {".git", "Documentation", "tools/testing"}
EXTS = (".c", ".h", ".S", ".lds", ".mk")


def is_comment_sep(line):
    s = line.strip()
    if not s:
        return True
    if all(c in b"*/ \t" for c in s):
        return True
    return False


def scan():
    hits = []
    for dirpath, dirnames, filenames in os.walk(ROOT):
        dirnames[:] = [d for d in dirnames if d not in SKIP_DIRS]
        for fn in filenames:
            if not fn.endswith(EXTS):
                continue
            p = os.path.join(dirpath, fn)
            try:
                with open(p, "rb") as f:
                    data = f.read()
            except OSError:
                continue
            for i, line in enumerate(data.split(b"\n"), 1):
                if is_comment_sep(line):
                    continue
                why = None
                if b"\xc2\xab" in line or b"redacted:" in line:
                    why = "guillemet"
                elif b"..." in line and b"sk_" in line:
                    why = "ellipsis-ident"
                elif b"***" in line:
                    why = "stars-ident"
                if why:
                    hits.append((p, i, line, why))
    return hits


if __name__ == "__main__":
    hits = scan()
    print(f"TOTAL HITS: {len(hits)}")
    for p, i, line, why in hits:
        print(f"{p}:{i} [{why}]")
        print(f"    hex: {line.strip().hex()}")
    sys.exit(0)