"""High-signal scan for tool-redaction corruption in C sources.

Real corruption = an identifier with an ellipsis inside it, e.g.
    sk_pso...eady   ->  sk_psock_data_ready
or a whole identifier replaced by '***' in expression position:
    copied = ***(sk, psock, ...);
    sk->***(sk);

Legit uses of '...' that must NOT be flagged:
    { [0 ... NR_CPUS-1] = -1 }     (GNU range designator)
    pr_debug("*** blah ***")        (string literal)
    /* *** decoration *** */         (comment decoration)
"""
import os
import re
import sys

ROOT = "."
SKIP_DIRS = {".git", "Documentation", "tools/testing"}
EXTS = (".c", ".h", ".S", ".lds", ".mk")

# identifier containing '...' : letter/underscore ... letter/underscore
RE_ELLIPSIS = re.compile(rb"[A-Za-z_][A-Za-z0-9_]*\.\.\.[A-Za-z0-9_]+")
# '***' in expression position
RE_STARS = re.compile(rb"(?:=\s*|\->\s*|\(\s*|\bcall\s+)\*\*\*+\s*(?:\(|\w)")


def strip_strings_and_comments(line):
    """Blank out string literals and comments so patterns don't match them."""
    out = bytearray()
    i = 0
    n = len(line)
    while i < n:
        c = line[i:i+1]
        if c == b'"' or c == b"'":
            q = c
            out += b" "
            i += 1
            while i < n:
                if line[i:i+1] == b"\\":
                    out += b"  "
                    i += 2
                    continue
                if line[i:i+1] == q:
                    out += b" "
                    i += 1
                    break
                out += b" "
                i += 1
            continue
        if line[i:i+2] == b"/*":
            while i < n and line[i:i+2] != b"*/":
                out += b" "
                i += 1
            out += b"  "
            i += 2
            continue
        if line[i:i+2] == b"//":
            out += b" " * (n - i)
            break
        out += c
        i += 1
    return bytes(out)


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
            for i, raw in enumerate(data.split(b"\n"), 1):
                clean = strip_strings_and_comments(raw)
                why = None
                if RE_ELLIPSIS.search(clean):
                    why = "ellipsis-in-ident"
                elif RE_STARS.search(clean):
                    why = "stars-ident"
                if why:
                    hits.append((p, i, raw, why))
    return hits


if __name__ == "__main__":
    hits = scan()
    print(f"TOTAL REAL HITS: {len(hits)}")
    for p, i, raw, why in hits:
        print(f"{p}:{i} [{why}]")
        print(f"    hex: {raw.strip().hex()}")
    sys.exit(0)