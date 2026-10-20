"""Compare the CONTENTS of a stock vendor .ko with the VXTux build's
replacement for it, separating the real API from the compiler
artefacts.

"100% sama" cannot mean byte-identical - a ported GPL driver will
never match a vendor blob byte for byte. It means the kernel-visible
contract is the same:

  REAL EXPORTS  the global symbols the module publishes. Every one the
                stock blob exported must still be exported, or the
                userspace and the modules that bind to it break.

  NEW IMPORTS   symbols the replacement needs that the stock one did
                not. These are not automatically wrong (a port may use
                a different kernel API), but they are new coupling.

  ARTEFACTS     symbols the COMPILER generates, whose presence depends
                on the stock kernel's build config, not on the driver:
                  __cfi_check / __cfi_check_fail   CONFIG_CFI_CLANG
                  __crc_<name>                      CONFIG_MODULE_REL_CRCS
                  __mod_of__<table>                 of_match_table modinfo
                  __typeid__<type>                  CONFIG_RTTI
                  __pfx_<fn>, __initcall_*, __ksymtab*, __kcrctab*, ...
                The stock ROM was built by the vendor with a config we
                do not have, so these are compared separately and never
                counted as a gap in the driver's API.

Usage:
    python3 compare_ko_content2.py [--only NAME] [--verbose]
"""
import os
import re
import subprocess
import sys

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
STOCK_DIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"
BUILT_DIR = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"
STOCK_LIST = "E:/vxtux_build_repo/donors/stock_modules.txt"
BUILT_LIST = os.path.join(BUILT_DIR, "ko_list.txt")

ONLY = None
if "--only" in sys.argv:
    ONLY = sys.argv[sys.argv.index("--only") + 1]
VERBOSE = "--verbose" in sys.argv

# compiler-generated symbols: present or absent purely because of the
# stock kernel's .config, not because of the driver
ARTEFACT_PREFIX = (
    "__cfi_", "__crc_", "__mod_of__", "__typeid__", "__pfx_",
    "__initcall_", "__exitcall_", "__ksymtab", "__kcrctab", "__kstrtab",
    "__modver", "__this_module", "__param", "__bug_table", "__jump_table",
    "__used", "__UNIQUE_ID", "_note_", "__gnu_local", "__stack_chk",
    "__stack_chk_fail", "__memcpy", "__aeabi_", "__aeabi_unwind",
    "__exidx", "__init_", "__bss_start", "__bss_stop", "__data_start",
    "__end_", "_GLOBAL_", "__init_array", "__fini_array",
    "__dso_handle", "__TMC_END__", "__FRAME_END__", "__bss_start__",
)
ARTEFACT_EXACT = {
    "__this_module", "__modver", "__init_array_start", "__init_array_end",
    "__fini_array_start", "__fini_array_end", "_etext", "_edata", "_end",
}


def is_artefact(sym):
    if sym in ARTEFACT_EXACT:
        return True
    return sym.startswith(ARTEFACT_PREFIX)


def read_nm(path):
    exports, imports = set(), set()
    r = subprocess.run([NM, path], capture_output=True, text=True,
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
        elif typ.isupper() and typ.isalpha():
            exports.add(name)
    return exports, imports, None


def main():
    stock = []
    with open(STOCK_LIST, encoding="utf-8", errors="replace") as f:
        for line in f:
            s = re.split(r"[\t|,;]", line.strip())[0].strip()
            if s:
                stock.append(s)

    built = {}
    ko_root = os.path.join(BUILT_DIR, "ko")
    if os.path.isdir(ko_root):
        for root, _, fs in os.walk(ko_root):
            for fn in fs:
                if fn.endswith(".ko"):
                    full = os.path.join(root, fn)
                    rel = os.path.relpath(full, ko_root).replace("\\", "/")
                    built[fn[:-3]] = full
    elif os.path.exists(BUILT_LIST):
        with open(BUILT_LIST, encoding="utf-8", errors="replace") as f:
            for line in f:
                p = line.rsplit(" ", 1)
                if len(p) != 2:
                    continue
                name = os.path.basename(p[0].strip())
                if name.endswith(".ko"):
                    built[name[:-3]] = os.path.join(BUILT_DIR, p[0].strip())

    print("=" * 100)
    print(f"{'module':<26} {'stock API':<10} {'vxtux API':<10} "
          f"{'MISSING API':<12} {'new imports':<12} artefact gap")
    print("=" * 100)

    perfect, gaps, nomod = [], [], []
    for s in stock:
        if ONLY and s != ONLY:
            continue
        sp = os.path.join(STOCK_DIR, s + ".ko")
        if not os.path.exists(sp):
            continue
        bp = built.get(s)
        if not bp or not os.path.exists(bp):
            nomod.append(s)
            print(f"{s:<26} {'-':<10} {'-':<10} {'BUILT-IN':<12} {'-':<12} -")
            continue
        se, si, e1 = read_nm(sp)
        be, bi, e2 = read_nm(bp)
        if se is None or be is None:
            print(f"{s:<26} nm error: {e1 or e2}")
            continue

        s_api = {x for x in se if not is_artefact(x)}
        b_api = {x for x in be if not is_artefact(x)}
        s_art = se - s_api
        b_art = be - b_api
        missing = s_api - b_api
        extra = b_api - s_api
        newimp = bi - si
        artgap = s_art - b_art

        print(f"{s:<26} {len(s_api):>4}      {len(b_api):>4}      "
              f"{len(missing):>4}        {len(newimp):>4}        "
              f"{len(artgap)}")
        if missing:
            print(f"      MISSING API: {sorted(missing)}")
        if extra:
            print(f"      extra API  : {sorted(extra)}")
        if newimp and VERBOSE:
            print(f"      new imports: {sorted(newimp)}")
        if not missing:
            perfect.append(s)
        else:
            gaps.append((s, sorted(missing)))

    print()
    print("=" * 100)
    print(f"API IDENTICAL to the stock blob : {len(perfect)}")
    for p in perfect:
        print(f"   OK  {p}")
    print(f"API GAPS                         : {len(gaps)}")
    for s, m in gaps:
        print(f"   {s:<26} {m}")
    print(f"not built as a module            : {len(nomod)}")
    for s in nomod:
        print(f"   {s}")
    print("=" * 100)


if __name__ == "__main__":
    main()
