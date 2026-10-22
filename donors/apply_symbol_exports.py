"""Apply the EXPORT_SYMBOL fixes so the VXTux drivers export the same
symbols the stock vendor .ko exported.

Why this matters: the stock vendor .ko was loaded as a module, so its
symbols lived in the kernel's export table. Userspace (and other
modules) bind to those symbols. When the VXTux build compiles the
driver into vmlinux instead, the symbols vanish from the export table
unless the source carries EXPORT_SYMBOL(). That is the gap between
"the driver exists" and "the vendor blob is genuinely replaced".

Two kinds of edit:
  EXPORT          the symbol is already global -> append
                  EXPORT_SYMBOL_GPL(sym); after its definition
  UNSTATIC+EXPORT the symbol is static -> drop `static`, then export

Implementation notes:
  - the file is scanned C-aware (block comments, line comments and
    string literals are blanked out) so ASCII-art block comments in
    the Novatek touch driver cannot corrupt the brace depth
  - ALL insertions for a file are collected first, then applied
    bottom-up in one pass, so an insertion never shifts the position
    of a later one (the previous version re-scanned after each
    insertion, which put exports inside function bodies)
  - an export for a function is placed after the closing '}' of the
    function; an export for a variable/object is placed after the
    ';' that ends it
  - the result is validated: brace/paren balance must be unchanged and
    every symbol must carry an EXPORT_SYMBOL in the written file

Usage:
    python3 apply_symbol_exports.py [--apply] [--only MODULE]
"""
import json
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
PLAN = "E:/vxtux_build_repo/donors/symbol_fix_plan.json"

APPLY = "--apply" in sys.argv
ONLY = None
if "--only" in sys.argv:
    ONLY = sys.argv[sys.argv.index("--only") + 1]

IDENT = re.compile(r"^[A-Za-z_]\w*$")
STATIC_RE = re.compile(r"\bstatic\b")
EXTERN_RE = re.compile(r"^\s*extern\b")
EXPORT_RE = re.compile(r"EXPORT_SYMBOL(_GPL)?\s*\(")


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def code_lines(lines):
    """Blank out block comments, line comments and string literals."""
    out = []
    in_block = False
    for line in lines:
        res = []
        i = 0
        n = len(line)
        while i < n:
            c = line[i]
            if in_block:
                if c == "*" and i + 1 < n and line[i + 1] == "/":
                    in_block = False
                    res.append("  ")
                    i += 2
                else:
                    res.append(" ")
                    i += 1
                continue
            if c == "/" and i + 1 < n and line[i + 1] == "*":
                in_block = True
                res.append("  ")
                i += 2
                continue
            if c == "/" and i + 1 < n and line[i + 1] == "/":
                res.append(" " * (n - i))
                break
            if c == '"' or c == "'":
                q = c
                res.append(" ")
                i += 1
                while i < n and line[i] != q:
                    if line[i] == "\\":
                        res.append(" ")
                        i += 1
                        if i < n:
                            res.append(" ")
                            i += 1
                        continue
                    res.append(" ")
                    i += 1
                if i < n:
                    res.append(" ")
                    i += 1
                continue
            res.append(c)
            i += 1
        out.append("".join(res))
    return out


def decls(code):
    """Yield (start, end, kind) for each top-level declaration.

    kind is 'func' (ends with '}'), 'var' (ends with ';') or 'proto'.
    """
    depth = 0
    start = None
    for i, line in enumerate(code):
        if start is None:
            s = line.strip()
            if not s or s.startswith("#"):
                continue
            start = i
        stop = None
        for ch in line:
            if ch in "({[":
                depth += 1
            elif ch in ")}]":
                depth -= 1
                if depth == 0 and ch == "}":
                    stop = ("func", i)
                    break
                if depth < 0:
                    depth = 0
            elif ch == ";" and depth == 0:
                stop = ("var", i)
                break
        if stop:
            joined = "\n".join(code[start:stop[1] + 1])
            kind = stop[0]
            if kind == "var" and "(" in joined and "{" not in joined:
                kind = "proto"
            yield (start, stop[1], kind)
            start = None
    if start is not None:
        yield (start, len(code) - 1, "var")


def decl_name(line):
    """If the line declares exactly one name, return (name, type)."""
    h = line.strip()
    if not h or h.startswith("#") or h.startswith("}"):
        return None, None
    cut = len(h)
    for ch in ("(", "="):
        j = h.find(ch)
        if j >= 0:
            cut = min(cut, j)
    decl = h[:cut]
    if "(" in decl or ")" in decl:
        return None, None
    decl = decl.replace("[", " ").replace("]", " ").replace("*", " * ")
    toks = decl.split()
    if not toks:
        return None, None
    name = toks[-1]
    if not IDENT.match(name):
        return None, None
    type_toks = toks[:-1]
    if not type_toks:
        return None, None
    if not all(IDENT.match(t) or t == "*" for t in type_toks):
        return None, None
    return name, " ".join(type_toks)


def find_defs(lines, wanted):
    """Return {sym: (start, end, kind, is_static, is_extern)}."""
    code = code_lines(lines)
    found = {}
    for start, end, kind in decls(code):
        for li in range(start, end + 1):
            name, _ = decl_name(code[li])
            if name is None or name not in wanted:
                continue
            seg = "\n".join(code[start:li + 1])
            is_static = bool(STATIC_RE.search(seg))
            is_extern = bool(EXTERN_RE.match(code[start]))
            found.setdefault(name, (start, end, kind, is_static, is_extern))
            break
    return found


def balance(text):
    b = p = 0
    for c in code_lines(text.split("\n")):
        b += c.count("{") - c.count("}")
        p += c.count("(") - c.count(")")
    return (b, p)


def main():
    plan = json.load(open(PLAN, encoding="utf-8"))
    total_edit = 0
    total_skip = 0
    changed = {}
    failures = []

    work = {}
    for mod, entries in plan.items():
        if ONLY and mod != ONLY:
            continue
        for e in entries:
            if not e.get("file"):
                continue
            if e["action"] not in ("EXPORT", "UNSTATIC+EXPORT"):
                continue
            work.setdefault(e["file"], []).append(e["sym"])

    for rel, syms in sorted(work.items()):
        path = os.path.join(TREE, rel)
        if not os.path.exists(path):
            print(f"  SKIP {rel}: missing")
            continue
        txt = read(path)
        lines = txt.split("\n")
        orig = list(lines)

        pending = set(syms)
        inserts = []      # (line_index, text)
        unstatic = []    # (line_index,)

        for _ in range(len(syms) + 2):
            if not pending:
                break
            found = find_defs(lines, pending)
            progressed = False
            for sym in sorted(pending):
                if sym not in found:
                    continue
                start, end, kind, is_static, is_extern = found[sym]
                if kind == "proto":
                    pending.discard(sym)
                    continue
                if is_extern and kind == "var":
                    pending.discard(sym)
                    continue
                if is_static:
                    unstatic.append(start)
                inserts.append((end + 1, f"EXPORT_SYMBOL_GPL({sym});"))
                pending.discard(sym)
                progressed = True
                print(f"  {rel}:{start + 1} {sym} -> EXPORT_SYMBOL_GPL")
            if not progressed:
                break

        for sym in sorted(pending):
            print(f"  !! {rel}: could not locate {sym}")
            total_skip += 1
            failures.append((rel, sym))

        if not inserts:
            continue

        # apply bottom-up so earlier offsets stay valid
        for idx, text in sorted(inserts, key=lambda x: -x[0]):
            lines.insert(idx, text)
        for idx in sorted(set(unstatic), reverse=True):
            lines[idx] = STATIC_RE.sub("", lines[idx], count=1)

        new_txt = "\n".join(lines)
        if balance(new_txt) != balance(txt):
            print(f"  !! {rel}: balance changed, NOT writing")
            continue
        # the export must not sit inside a function body
        code = code_lines(new_txt.split("\n"))
        bad = []
        for i, line in enumerate(code):
            m = EXPORT_RE.search(line)
            if not m:
                continue
            depth = 0
            for j in range(i - 1, -1, -1):
                depth += code[j].count("}") - code[j].count("{")
                if depth < 0:
                    break
            else:
                j = -1
            if j >= 0:
                bad.append(i + 1)
        if bad:
            print(f"  !! {rel}: export inside a body at lines {bad[:5]}, "
                  f"NOT writing")
            continue

        if APPLY:
            write(path, new_txt)
            changed[rel] = os.path.getsize(path)
            bad2 = [s for s in syms
                    if not re.search(r"EXPORT_SYMBOL(_GPL)?\s*\(\s*" +
                                     re.escape(s) + r"\s*\)", new_txt)]
            if bad2:
                print(f"  !! {rel}: post-verify FAILED for {bad2}")
            total_edit += len(inserts)
        else:
            print(f"  (dry run) {rel} would change")

    print()
    print(f"edits: {total_edit}, skipped: {total_skip}")
    if failures:
        print(f"could not locate ({len(failures)}):")
        for rel, sym in failures:
            print(f"   {rel}: {sym}")
    if APPLY:
        print("WROTE:")
        for f, n in sorted(changed.items()):
            print(f"   {f} ({n:,} bytes)")
    else:
        print("dry run - nothing written. Re-run with --apply")


if __name__ == "__main__":
    main()
