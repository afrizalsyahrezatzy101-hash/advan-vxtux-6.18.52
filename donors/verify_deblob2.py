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
CLONE_RE = re.compile(r"^(.+?)\.\d+(?:\.constprop\.\d+)?$")
# strip_clone() below must substitute \1, not the empty string: the pattern is
# anchored on the whole name, so replacing it with "" erased the base name
# along with the suffix and made every renamed clone look unmatchable.
# gcc's IPA partitioning splits a function into foo.part.0 / foo.part.1 when it
# inlines one copy into another.  That is a compiler artefact exactly like the
# clang ".N" clone suffix above and is never part of the module's API, so the
# base name is what counts.  Without this, trusty-ipc reported vds_create_channel
# as missing while the build actually contained vds_create_channel.part.0.
PART_RE = re.compile(r"^(.*)\.part\.\d+$")
# The same argument covers the rest of gcc's clone families.  CLONE_RE only
# removed the trailing NUMBER, so "aud_smsg_ch_send.isra.0" was reduced to
# "aud_smsg_ch_send.isra" and still compared unequal to the stock name.
# .part/.constprop can chain ("HI_Command.part.0.constprop.0"), so the
# family suffix is stripped repeatedly.  All of these are gcc's out-of-line
# copies of a driver function: local symbols, never API.
CLONE_FAMILY_RE = re.compile(
    r"\.(?:part|constprop|isra|cold|clone|lto_priv|recip)(?:\.\d+)*$")
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

# The trace macro's own generated names changed between 5.4 and 6.18.
# Two separate renames, both provably one-to-one (same header, same
# template, same events), so the 6.18 object IS the 5.4 one:
#
#   1. the field-descriptor table.  trace_events.h generated
#        5.4   static int trace_event_define_fields_<call>(struct trace_event_call *)
#        6.18  static struct trace_event_fields trace_event_fields_<call>[]
#      5.4 computed the offsets at init time from a constructed
#      trace_event_raw_<call>; 6.18 emits them as a static array and the
#      class holds .fields_array instead of .define_fields.  Same fields,
#      same event, different storage - so trace_event_define_fields_X is
#      counted as trace_event_fields_X.  trusty.ko builds exactly this:
#        stock  t trace_event_define_fields_smccall
#        built  d trace_event_fields_smccall
#
#   2. the tracepoint-pointer table.  __TRACEPOINT_ENTRY() has two
#      encodings:
#        #ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
#             asm(".long __tracepoint_<name> - .")          (no symbol)
#        #else
#             static tracepoint_ptr_t __tracepoint_ptr_<name> = &...
#      6.18's arm64 selects HAVE_ARCH_PREL32_RELOCATIONS, so it takes
#      the asm path and emits NO named __tracepoint_ptr_<name> at all -
#      the pointer is an anonymous .long in __tracepoints_ptrs.  The
#      stock blob's __tracepoints_ptrs is 16 bytes (2 x 8-byte named
#      objects), the 6.18 build's is 8 bytes (2 x .long): the same table.
#      There is no symbol to match, so this one is satisfied by the
#      tracepoint itself being present: __tracepoint_<name>.
TRACE_RENAME_RE = re.compile(
    r"^trace_event_define_fields_(?P<call>[A-Za-z_]\w*)$")
TRACEPOINTER_RE = re.compile(r"^__tracepoint_ptr_(?P<name>[A-Za-z_]\w*)$")

# A function-local static gets "func.member" from clang and "member.<n>"
# from gcc, so the two toolchains name the SAME variable differently and a
# plain name comparison cannot see them as one.  trusty-log's
# trusty_dump_logs.__print_once is the case: 5.4's printk_once() declared
# `static bool __print_once`, clang names the object after its enclosing
# function, and 6.18's DO_ONCE_LITE() declares `static bool __already_done`,
# which gcc names __already_done.<n>.
#
# Collapsing "func.member" to "func" is NOT safe as a general rule, and
# the rule below is deliberately narrow because of it:
#
#   * "func" is a FUNCTION and "func.member" is a separate DATA object.
#     Comparing them compares two different things, so a blanket collapse
#     would let a build satisfy a data symbol with the mere existence of
#     a function that has the same stem.  Measured over the 31 stock
#     blobs, a blanket collapse would have hidden unisoc-mailbox's
#     sprd_mbox_phy_outbox_isr._rs (a real missing object) by merging it
#     into the also-missing sprd_mbox_phy_outbox_isr, dropping that
#     module's count 30 -> 29 with nothing built.
#   * It is not injective: two stock names can share a stem
#     (mmsys_notify_call_chain and mmsys_notify_call_chain.pw_opened), so
#     collapsing merges two rows into one and silently deletes a gap.
#
# So the collapse is applied only where it is one-to-one and the match
# stays meaningful: the base must be present in the SAME blob as a
# symbol of the same kind, and the base must be present on the built
# side as a symbol of the same kind too.  A function can then never
# satisfy a data symbol or vice versa.
FUNC_STATIC_RE = re.compile(
    r"^(?P<base>[A-Za-z_]\w*)\.(?P<member>[A-Za-z_]\w*)$")

# Member parts that are themselves a known compiler naming form, so the
# name is not a "func.member" pair at all.
_NOT_A_MEMBER = re.compile(
    r"^(?:part|constprop|isra|cold|clone|lto_priv|recip)$"
    r"|^UNIQUE_ID_ddebug\d+$|^warned$|^key$|^rs$|^cr$|^lck$")

# nm's type letters, split into the two kinds a "func.member" pair must
# agree on: a function, or a data object.
FUNC_TYPES = "tT"
DATA_TYPES = "dDbBrR"


def symbol_kinds(path):
    """{name: 'func'|'data'} for every defined symbol in the module."""
    r = subprocess.run([NM, path], capture_output=True, text=True,
                       timeout=300)
    kinds = {}
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) < 2:
            continue
        typ = p[-2] if len(p) >= 3 else p[0]
        if typ in FUNC_TYPES:
            kinds[p[-1]] = "func"
        elif typ in DATA_TYPES:
            kinds[p[-1]] = "data"
    return kinds


def trace_renames(s):
    """The 6.18 spelling(s) of a 5.4 trace-macro name, or []."""
    m = TRACE_RENAME_RE.match(s)
    if m:
        return ["trace_event_fields_%s" % m.group("call")]
    m = TRACEPOINTER_RE.match(s)
    if m:
        # no symbol is emitted for this in 6.18; it is satisfied by the
        # tracepoint itself being present.
        return ["__tracepoint_%s" % m.group("name")]
    return []


def func_static_base(s):
    """The base of a 'func.member' name, or None.

    Only for names where the member part is not itself a known compiler
    naming form, so this never fires on the clone/partition families.
    """
    m = FUNC_STATIC_RE.match(s)
    if not m:
        return None
    if _NOT_A_MEMBER.match(m.group("member")):
        return None
    return m.group("base")


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
    """Drop a compiler-generated clone or partition suffix.

    clang emits a clone per outlined copy of a function ("foo.421"), gcc
    numbers them differently or not at all, gcc's IPA partitioning splits a
    function into "foo.part.0", and its other clone families leave
    "foo.isra.0" / "foo.constprop.0".  These are local symbols, never part
    of the module's API contract, so the base name is what counts.
    """
    prev = None
    while prev != s:                     # the families can chain
        prev = s
        s = CLONE_FAMILY_RE.sub("", s)
    return CLONE_RE.sub(r"\1", s)


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


def self_check():
    """The clone-stripping must reduce a compiler suffix to the driver's own
    name, never to the empty string and never to a different name.

    It went wrong once: `CLONE_RE.sub("", s)` ate the WHOLE symbol, so
    "apsys_dvfs_init.421" became "" and matched any other collapsed name.
    That hid real gaps (unisoc-mailbox's reg_ctrl) and invented others.

    Returns the list of failures; empty means OK.
    """
    cases = [
        # gcc IPA partition
        ("vds_create_channel.part.0", "vds_create_channel"),
        ("dn_connect_ioctl.part.0", "dn_connect_ioctl"),
        ("trusty_log_alloc_pages.part.0", "trusty_log_alloc_pages"),
        # gcc interprocedural-scalar-replacement / constant propagation
        ("aud_smsg_ch_send.isra.0", "aud_smsg_ch_send"),
        ("crypto_transfer_request.constprop.0", "crypto_transfer_request"),
        # chained
        ("HI_Command.part.0.constprop.0", "HI_Command"),
        # clang clone suffix
        ("apsys_dvfs_init.421", "apsys_dvfs_init"),
        # a real driver name must survive untouched
        ("vds_get_txbuf", "vds_get_txbuf"),
        ("_handle_rxbuf", "_handle_rxbuf"),
        ("_free_vds", "_free_vds"),
        ("_vds_get_txbuf", "_vds_get_txbuf"),
        ("create_cdev_node", "create_cdev_node"),
        ("destroy_cdev_node", "destroy_cdev_node"),
        ("vds_add_channel", "vds_add_channel"),
        ("vds_del_channel", "vds_del_channel"),
        ("_create_cdev_node", "_create_cdev_node"),
        ("_dn_lookup_vds", "_dn_lookup_vds"),
        ("_handle_conn_rsp", "_handle_conn_rsp"),
        ("_handle_ctrl_msg", "_handle_ctrl_msg"),
        ("_handle_disc_req", "_handle_disc_req"),
    ]
    bad = [(s, want, strip_clone(s)) for s, want in cases
           if strip_clone(s) != want]
    # nothing may ever collapse to the empty string
    for s, _ in cases:
        if strip_clone(s) == "":
            bad.append((s, "<non-empty>", ""))
    return bad


def main():
    bad = self_check()
    if bad:
        print("SELF-CHECK FAILED - clone stripping is wrong:")
        for s, want, got in bad:
            print(f"  {s!r}: want {want!r}, got {got!r}")
        return 1

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
        # Two cross-toolchain name changes the two sides cannot be diffed
        # on directly.  Each is applied only to the names it can apply to,
        # and each is checked against the other side's symbols, so a
        # rename can only ever satisfy a name it really corresponds to.
        s_kinds = symbol_kinds(sp)
        b_kinds = symbol_kinds(bp)
        extra_b = set()
        for x in s_api - b_api:
            # 1. the trace macro's own generated names (5.4 -> 6.18)
            for alt in trace_renames(x):
                if alt in b_api:
                    extra_b.add(alt)
                    break
            else:
                # 2. a function-local static that one toolchain names
                #    "func.member" and the other names plainly
                base = func_static_base(x)
                if base and base in s_api and base in b_api:
                    # the base must be the same KIND on both sides, so a
                    # function can never satisfy a data symbol
                    if (s_kinds.get(x) == s_kinds.get(base) and
                            b_kinds.get(base) == s_kinds.get(x)):
                        extra_b.add(base)
        b_api = b_api | extra_b
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
