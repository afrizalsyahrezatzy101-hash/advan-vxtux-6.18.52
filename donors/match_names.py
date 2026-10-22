"""Name-level match between the 31 stock vendor blobs and the build.

The 31 blobs are what the stock ROM ships in /lib/modules. For the
deblob to be real, the build has to produce a module that (a) has the
same file name, or (b) at least covers the same API, so the loader
can insmod it and nothing that bound to the stock blob breaks.

This is the name-matching pass, but it also reports the near misses:
a module that carries the same API under a different file name is a
rename away from a real match, and that is worth knowing.

Usage:
    python3 match_names.py
"""
import os
import re
import subprocess

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
STOCK_DIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"
BUILT = "E:/vxtux_build_repo/built_kos"
STOCK_LIST = "E:/vxtux_build_repo/donors/stock_modules.txt"

ARTEFACT_PREFIX = (
    "__cfi_", "__crc_", "__mod_of__", "__typeid__", "__pfx_",
    "__initcall_", "__exitcall_", "__ksymtab", "__kcrctab", "__kstrtab",
    "__modver", "__this_module", "__param", "__bug_table", "__jump_table",
    "__used", "__UNIQUE_ID", "_note_", "__gnu_local", "__stack_chk",
    "__stack_chk_fail", "__memcpy", "__aeabi_", "__exidx", "__init_",
    "__bss_start", "__bss_stop", "__data_start", "__end_", "_GLOBAL_",
    "__init_array", "__fini_array", "__dso_handle", "__TMC_END__",
    "__FRAME_END__", "__bss_start__", "__bss_end__", "__per_cpu_start",
)
ARTEFACT_EXACT = {
    "__this_module", "__modver", "_etext", "_edata", "_end",
    "__init_array_start", "__init_array_end",
    "__fini_array_start", "__fini_array_end",
}


def is_artefact(s):
    return s in ARTEFACT_EXACT or s.startswith(ARTEFACT_PREFIX)


def nm_api(path):
    """The exported API, artefacts removed."""
    r = subprocess.run([NM, path], capture_output=True, text=True,
                       timeout=300)
    if r.returncode != 0:
        return None
    api = set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) < 2:
            continue
        typ = p[-2] if len(p) >= 3 else p[0]
        name = p[-1]
        if typ.isalpha() and typ.isupper() and not is_artefact(name):
            api.add(name)
    return api


def main():
    stock = []
    with open(STOCK_LIST, encoding="utf-8", errors="replace") as f:
        for line in f:
            s = re.split(r"[\t|,;]", line.strip())[0].strip()
            if s:
                stock.append(s)

    built = {}
    for root, _, fs in os.walk(BUILT):
        for fn in fs:
            if fn.endswith(".ko"):
                built[fn[:-3]] = os.path.join(root, fn)

    # the API of every built module, for the near-miss search
    built_api = {n: nm_api(p) for n, p in built.items()}

    print("=" * 88)
    print("NAME MATCH: the stock blob's file name -> a module the build emits")
    print("=" * 88)
    exact = []
    for s in stock:
        hit = s in built
        alt = s.replace("-", "_")
        if not hit and alt in built:
            print(f"  {s:28s} -> {alt}.ko   (name differs by - vs _)")
            exact.append(s)
        elif hit:
            exact.append(s)
        else:
            print(f"  {s:28s} -> MISSING")

    print()
    print("=" * 88)
    print("API COVERAGE: for the ones with no name match, is any built")
    print("module an API superset of the stock blob?")
    print("=" * 88)
    unresolved = []
    for s in stock:
        if s in exact:
            continue
        sp = os.path.join(STOCK_DIR, s + ".ko")
        if not os.path.exists(sp):
            continue
        sa = nm_api(sp)
        if not sa:
            continue
        best = None
        for n, ba in built_api.items():
            if not ba:
                continue
            cov = len(sa & ba) / len(sa)
            if cov >= 0.9 and (best is None or cov > best[1]):
                best = (n, cov)
        if best:
            print(f"  {s:28s} -> {best[0]}.ko covers "
                  f"{100 * best[1]:.0f}% of the API")
        else:
            unresolved.append(s)
            print(f"  {s:28s} -> no built module covers its API")

    print()
    print("=" * 88)
    print(f"exact file-name match: {len(exact)}/{len(stock)} "
          f"({100.0 * len(exact) / len(stock):.1f}%)")
    print(f"still unresolved:      {len(unresolved)}")
    for u in unresolved:
        print(f"    {u}")
    print("=" * 88)


if __name__ == "__main__":
    main()
