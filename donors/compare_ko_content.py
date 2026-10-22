"""Compare the CONTENTS of a stock vendor .ko with the VXTux build's
replacement for it.

"100% sama" is not about the file size - a ported GPL driver will
never be byte-identical to a vendor blob. It is about the contract
the kernel sees:

  EXPORTS  the global symbols the module publishes. Everything the
           stock blob exported must still be exported, or userspace
           (and other modules) that bind to it breaks.

  IMPORTS  the symbols the module needs from the kernel. If the
           VXTux replacement imports a symbol the stock one did not,
           that is a new dependency the stock system did not have.

  MODULE   the module metadata (license, alias, depends) has to stay
           compatible, because the loader and the init scripts use it.

The comparison runs nm over both files. The stock .ko and the VXTux
.ko are both aarch64 ELF relocatable objects, so the same nm works on
both.

Usage:
    python3 compare_ko_content.py            # all 31
    python3 compare_ko_content.py --only trusty-tui
    python3 compare_ko_content.py --missing  # only report gaps
"""
import os
import re
import subprocess
import sys

GIT_BIN = r"C:\Program Files\Git\cmd\git.exe"
NM_CANDIDATES = [
    r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe",
    r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe",
]
STOCK_DIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"
BUILT_DIR = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"
STOCK_LIST = "E:/vxtux_build_repo/donors/stock_modules.txt"
BUILT_LIST = os.path.join(BUILT_DIR, "ko_list.txt")

ONLY = None
if "--only" in sys.argv:
    ONLY = sys.argv[sys.argv.index("--only") + 1]
MISSING_ONLY = "--missing" in sys.argv


def find_nm():
    for p in NM_CANDIDATES:
        if os.path.exists(p) and p.endswith("nm.exe"):
            return p
    # look for any llvm-nm / aarch64 nm on PATH
    for exe in ("aarch64-linux-gnu-nm", "aarch64-none-linux-gnu-nm",
                "llvm-nm", "nm"):
        r = subprocess.run(["where", exe] if os.name == "nt" else
                           ["which", exe],
                           capture_output=True, text=True, timeout=60)
        if r.returncode == 0 and r.stdout.strip():
            return r.stdout.strip().splitlines()[0]
    return None


def read_nm(path, nm):
    """Return (exports, imports) for an ELF object."""
    exports, imports = set(), set()
    r = subprocess.run([nm, path], capture_output=True, text=True,
                       timeout=300)
    if r.returncode != 0:
        return None, None, r.stderr.strip()[:200]
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) < 2:
            continue
        typ = parts[-2] if len(parts) >= 3 else parts[0]
        name = parts[-1]
        if typ == "U":
            imports.add(name)
        elif typ in ("T", "D", "B", "R", "W", "V", "G", "t", "d", "b",
                     "r", "g", "C", "A"):
            # lowercase = local, skip; only global counts as an export
            if typ.isupper():
                exports.add(name)
    return exports, imports, None


def main():
    nm = find_nm()
    if not nm:
        print("!! no nm found for aarch64")
        return 1
    print(f"nm: {nm}\n")

    stock = []
    with open(STOCK_LIST, encoding="utf-8", errors="replace") as f:
        for line in f:
            s = re.split(r"[\t|,;]", line.strip())[0].strip()
            if s:
                stock.append(s)

    built = {}
    if os.path.exists(BUILT_LIST):
        with open(BUILT_LIST, encoding="utf-8", errors="replace") as f:
            for line in f:
                p = line.rsplit(" ", 1)
                if len(p) != 2:
                    continue
                name = os.path.basename(p[0].strip())
                if name.endswith(".ko"):
                    built[name[:-3]] = os.path.join(BUILT_DIR, p[0].strip())

    print("=" * 92)
    print(f"{'module':<28} {'exports':<16} {'missing exports':<16} "
          f"{'new imports'}")
    print("=" * 92)
    rows = []
    for s in stock:
        if ONLY and s != ONLY:
            continue
        sp = os.path.join(STOCK_DIR, s + ".ko")
        if not os.path.exists(sp):
            print(f"{s:<28} (no stock .ko)")
            continue
        bp = built.get(s)
        if not bp or not os.path.exists(bp):
            print(f"{s:<28} {'-':<16} {'BUILT-IN / n/a':<16} {'-'}")
            rows.append((s, None, None, None, None))
            continue
        se, si, err = read_nm(sp, nm)
        be, bi, err2 = read_nm(bp, nm)
        if se is None or be is None:
            print(f"{s:<28} nm error: {err or err2}")
            continue
        missing = se - be
        extra = be - se
        newimp = bi - si
        gone = si - bi
        print(f"{s:<28} {len(se):>4} -> {len(be):<4}    "
              f"{len(missing):>4}          {len(newimp)}"
              + (f"  [extra exports: {len(extra)}]" if extra else "")
              + (f"  [dropped imports: {len(gone)}]" if gone else ""))
        if missing:
            print(f"      MISSING EXPORTS: {sorted(missing)[:12]}")
        rows.append((s, len(se), len(be), missing, newimp))

    ok = [r for r in rows if r[3] is not None and not r[3]]
    bad = [r for r in rows if r[3]]
    na = [r for r in rows if r[1] is None]
    print()
    print("=" * 92)
    print(f"identical export set : {len(ok)}")
    print(f"export gaps          : {len(bad)}")
    print(f"not built as a module : {len(na)}")
    for r in bad:
        print(f"   {r[0]:<28} missing {len(r[3])}: "
              f"{sorted(r[3])[:8]}")
    print("=" * 92)
    return 0


if __name__ == "__main__":
    sys.exit(main())
