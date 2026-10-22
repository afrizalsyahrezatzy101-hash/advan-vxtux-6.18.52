"""Split a Ghidra DumpAll output (foo.ko.all.c) into one .c per function.

The header lines look like:  ====================name@addr====================
"""
import os
import re
import sys

HDR = re.compile(r"^={10,}([^@=]+)@([0-9a-fA-F]+)={10,}\s*$")


def split(src, dst):
    os.makedirs(dst, exist_ok=True)
    out = []
    cur = None
    buf = []
    for line in open(src, encoding="utf-8", errors="replace"):
        m = HDR.match(line.rstrip("\n"))
        if m:
            if cur:
                out.append((cur, buf))
            cur, buf = m.group(1), []
        elif cur is not None:
            buf.append(line)
    if cur:
        out.append((cur, buf))
    for name, body in out:
        safe = re.sub(r"[^A-Za-z0-9_.-]", "_", name)
        with open(os.path.join(dst, safe + ".c"), "w", encoding="utf-8") as f:
            f.writelines(body)
    return len(out)


if __name__ == "__main__":
    for pair in sys.argv[1:]:
        s, d = pair.split("=")
        print(s, "->", split(s, d), "files")
