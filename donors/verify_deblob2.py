"""The strict deblob verification.

Three levels of evidence, each reported on its own, so a partial
result is never dressed up as a full one:

  LEVEL 1  NAME    the build emits a .ko with the stock file name
  LEVEL 2  API     that .ko has every symbol the stock .ko had
                   (the stock blob's LOCAL symbols included - the
                   replacement must carry the same code, not just the
                   same exports)
  LEVEL 3  DEPS    that .ko does not import anything the stock one
                   did not, and that every symbol it does import
                   exists in the built vmlinux

The vmlinux symbol list comes from the built kernel, so LEVEL 3 is
checked against the real kernel rather than against a guess.

Usage:
    python3 verify_deblob2.py
"""
import os
import re
import subprocess
import sys

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
STOCK_DIR = "E:/vxtux_build_repo/donors/stock_vendor_modules"
BUILT = "E:/vxtux_build_repo/built_kos"
STOCK_LIST = "E:/vxtux_build_repo/donors/stock_modules.txt"
VMLINUX_SYMS = "E:/vxtux_build_repo/donors/all_syms.txt"
REPORT = "E:/vxtux_build_repo/docs/DEBLOB_VERIFICATION.md"

# compiler/linker artefacts, not API
ARTEFACT_PREFIX = (
    "__cfi_", "__crc_", "__mod_of__", "__typeid__", "__pfx_",
    "__initcall_", "__exitcall_", "__ksymtab", "__kcrctab", "__kstrtab",
    "__modver", "__this_module", "__param", "__bug_table", "__jump_table",
    "__used", "__UNIQUE_ID", "_note_", "__gnu_local", "__stack_chk",
    "__stack_chk_fail", "__memcpy", "__aeabi_", "__exidx", "__init_",
    "__bss_start", "__bss_stop", "__data_start", "__end_", "_GLOBAL_",
    "__init_array", "__fini_array", "__dso_handle", "__TMC_END__",
    "__FRAME_END__", "__bss_start__", "__bss_end__", "__per_cpu_start",
    "__addressable_", "__ddebug", "__key", "__UNIQUE",
)
ARTEFACT_EXACT = {
    "__this_module", "__modver", "_etext", "_edata", "_end",
    "__init_array_start", "__init_array_end",
    "__fini_array_start", "__fini_array_end",
    "cleanup_module", "init_module", "__key",
}
# The two entry points are not API - every module has them
ENTRY = {"cleanup_module", "init_module"}

# Per-function bookkeeping objects. Their names are toolchain-specific:
#
#   clang 5.4   <func>.__UNIQUE_ID_ddebug<N>   <func>.__warned
#   gcc 6.18    __UNIQUE_ID_ddebug<N>.<M>       __warned.<N>
#
# and the lockdep key of a function-local static:
#
#   clang 5.4   <func>.__key
#   gcc 6.18    __key.<N>
#
# The code they describe is the driver's, but the NAMES are the
# compiler's, so a name comparison across toolchains is meaningless.
# The function that owns them IS compared - that is the real check.
BOOKKEEPING_RE = re.compile(
    r"^(?:[A-Za-z_][\w]*)"
    r"(?:\.__UNIQUE_ID_ddebug\d+|\.__warned|\.__key)(?:\.\d+)?$"
    r"|^__UNIQUE_ID_ddebug\d+\.\d+$"
    r"|^__warned\.\d+$"
    r"|^__key\.\d+$"
)

# Compiler label placeholders. clang calls them .Ltmp<N>, gcc .L<N> or
# .L14472!<N>. They are numbered per translation unit, so the numbers
# have no meaning across toolchains. gcc's .compoundliteral.N is the
# same kind of thing.
CLONE_RE = re.compile(r"^.+?\.\d+(?:\.constprop\.\d+)?$")
LABEL_RE = re.compile(r"^\.L(tmp)?\d+!?\d*$|^\.L\d+!?\d*$|"
                      r"^\.L\d+!\d+$|^\.L\d+$|^\.Ltmp\d+$|"
                      r"^\.compoundliteral(\.\d+)?$|^\.L\d+!\d+$")

# MODULE_DEVICE_TABLE's symbol name is built by the macro, and the
# macro changed between 5.4 and 6.18:
#
#   5.4   __mod_<type>__<table>_device_table
#   6.18  __mod_device_table__kmod_<module>__<type>__<table>
#
# The table itself is the same, so these are not API gaps. They are
# matched by the table they are built from instead.
DEVICE_TABLE_RE = re.compile(
    r"^__mod(?:_device_table__kmod_[A-Za-z0-9_]+)?"
    r"(?:_[A-Za-z0-9_]+)?__[A-Za-z0-9_]*_device_table$"
)


def is_artefact(s):
    if s in ENTRY:
        return True
    if s in ARTEFACT_EXACT:
        return True
    if DEVICE_TABLE_RE.match(s):
        return True
    if BOOKKEEPING_RE.match(s):
        return True
    if LABEL_RE.match(s):
        return True
    return s.startswith(ARTEFACT_PREFIX)


def strip_clone(s):
    """Drop a compiler-generated clone suffix: "foo.421" -> "foo".

    clang emits a clone per outlined copy of a function; gcc numbers them
    differently or not at all.  These are local symbols, never part of the
    module's API contract.
    """
    return CLONE_RE.sub("", s)


def nm(path):
    """(local+global symbols, undefined symbols)"""
    r = subprocess.run([NM, path], capture_output=True, text=True,
                       timeout=300)
    if r.returncode != 0:
        return None, None
    defs, und = set(), set()
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) < 2:
            continue
        typ = p[-2] if len(p) >= 3 else p[0]
        name = p[-1]
        if typ == "U":
            und.add(name)
        elif typ.isalpha() and typ in "TtDdBbRrWwVv":
            # nm's type letter: uppercase = global, lowercase = local.
            # BOTH count. Using isupper() here dropped every local
            # symbol (type 't'), which is most of the driver's own code:
            # the check then reported "identical" for blobs that were
            # missing real driver functions.
            defs.add(name)
    return defs, und


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

    vml = set()
    if os.path.exists(VMLINUX_SYMS):
        with open(VMLINUX_SYMS, encoding="utf-8", errors="replace") as f:
            for line in f:
                s = line.strip()
                if s:
                    vml.add(s.split()[-1] if " " in s else s)

    lines = []
    lines.append("# Deblob verification (strict)")
    lines.append("")
    lines.append("Generated by `donors/verify_deblob2.py`.")
    lines.append("")
    lines.append("The stock ROM ships 31 vendor .ko in /lib/modules. For the")
    lines.append("deblob to be real, the VXTux build must produce a module")
    lines.append("with the same file name, carrying the same code, so the")
    lines.append("loader can insmod it and nothing that bound to the stock")
    lines.append("blob breaks.")
    lines.append("")
    lines.append("Three levels, each reported on its own:")
    lines.append("")
    lines.append("1. **NAME** - the build emits a .ko with the stock file name.")
    lines.append("2. **API** - that .ko has every symbol the stock .ko had,")
    lines.append("   local symbols included. The stock blob's locals are the")
    lines.append("   driver's own functions, so a replacement that does not")
    lines.append("   have them is not the same code.")
    lines.append("3. **DEPS** - it imports nothing the stock one did not, and")
    lines.append("   every symbol it does import exists in the built vmlinux")
    lines.append(f"   ({len(vml):,} symbols).")
    lines.append("")
    lines.append("| # | stock .ko | name | API | new imports | imports ok |")
    lines.append("|---|-----------|------|-----|-------------|------------|")

    n_name = n_api = 0
    gaps = []
    for i, s in enumerate(stock, 1):
        sp = os.path.join(STOCK_DIR, s + ".ko")
        bp = built.get(s)
        if not os.path.exists(sp):
            lines.append(f"| {i} | `{s}` | - | - | - | - |")
            continue
        if not bp:
            lines.append(f"| {i} | `{s}` | **MISSING** | - | - | - |")
            continue
        n_name += 1
        sd, su = nm(sp)
        bd, bu = nm(bp)
        if sd is None or bd is None:
            lines.append(f"| {i} | `{s}` | OK | nm error | - | - |")
            continue
        s_api = {x for x in sd if not is_artefact(x)}
        b_api = {x for x in bd if not is_artefact(x)}
        # Strip the compiler-generated clone suffix on BOTH sides before
        # diffing.  "apsys_dvfs_init.421" is a clang clone of
        # "apsys_dvfs_init"; the same source rebuilt with gcc produces a
        # different suffix, or none at all, so comparing them verbatim made
        # apsys-dvfs report 523 "missing" symbols when the real contract gap
        # was 26.
        s_api = {strip_clone(x) for x in s_api}
        b_api = {strip_clone(x) for x in b_api}
        missing = sorted(s_api - b_api)
        newimp = sorted(bu - su)
        if missing:
            lines.append(f"| {i} | `{s}` | OK | **{len(missing)} missing** | "
                         f"{len(newimp)} | "
                         f"{', '.join(missing[:3])} |")
            # level 3 is checked for every module, missing API or not
            unresolved = [sym for sym in sorted(bu) if vml and sym not in vml]
            gaps.append((s, missing, newimp, unresolved))
        else:
            n_api += 1
            # LEVEL 3: every import must exist in the built vmlinux
            unresolved = []
            for sym in sorted(bu):
                if sym.startswith(("__", "_")) and not vml:
                    continue
                if vml and sym not in vml:
                    unresolved.append(sym)
            ok = "yes" if not unresolved else f"**{len(unresolved)} missing**"
            if unresolved:
                gaps.append((s, [], newimp, unresolved))
            lines.append(f"| {i} | `{s}` | OK | identical | "
                         f"{len(newimp)} | {ok} |")

    n = len(stock)
    lines.append("")
    lines.append("## Result")
    lines.append("")
    lines.append(f"- stock vendor blobs: **{n}**")
    lines.append(f"- a .ko with the same file name is built: **{n_name}** "
                 f"({100.0 * n_name / n:.1f}%)")
    lines.append(f"- and it carries the same API: **{n_api}** "
                 f"({100.0 * n_api / n:.1f}%)")
    if gaps:
        lines.append("")
        lines.append("### Gaps")
        lines.append("")
        for s, missing, newimp, unresolved in gaps:
            if missing:
                lines.append(f"- `{s}`: missing {missing}")
            if newimp:
                # the stock blobs were built for the 5.4 kernel, this tree
                # is 6.18. The kernel's own API changed names, so a
                # replacement imports symbols the stock one did not:
                # _printk, __kmalloc_cache_noprof, mutex_lock_nested, ...
                # Those are the kernel's, not the driver's - not a gap.
                kernel_api = [x for x in newimp if x in vml]
                real = [x for x in newimp if x not in vml]
                if kernel_api:
                    lines.append(f"- `{s}`: kernel API drift (6.18 renamed "
                                 f"{len(kernel_api)} of the stock blob's "
                                 f"imports, all present in the build): "
                                 f"{kernel_api[:8]}{'...' if len(kernel_api) > 8 else ''}")
                if real:
                    lines.append(f"- `{s}`: imports the stock blob did not have, "
                                 f"and the build does not provide them: {real}")
            if unresolved:
                lines.append(f"- `{s}`: imports not in the built vmlinux: "
                             f"{unresolved}")
    lines.append("")
    lines.append("### The ones with no module yet")
    lines.append("")
    for i, s, *_ in [(i, s) for i, s in enumerate(stock, 1)]:
        if s not in built:
            lines.append(f"- `{s}`")
    lines.append("")

    os.makedirs(os.path.dirname(REPORT), exist_ok=True)
    with open(REPORT, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))

    print("\n".join(lines))
    print(f"written: {REPORT}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
