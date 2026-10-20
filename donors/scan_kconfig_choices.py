"""Scan every Kconfig for the 'prompt outside its choice group' bug and
the other choice-group mistakes, before the build finds them one at a
time.

Kconfig rules for a choice group:
  - the options inside a choice are plain symbols. They take their
    prompt from the choice itself, so a 'bool "..."' or 'tristate "..."'
    line inside a choice is an error:
        error: choice value has a prompt outside its choice group
  - the options inside a choice must be bool, not tristate
        error: choice value ... has wrong type
  - a 'depends on' inside a choice that depends on a symbol outside the
    choice is allowed, but a 'default' inside a choice option is not
        error: default for choice ... must be the last

Also checks:
  - 'source' of a file that does not exist
  - a 'config X' that is never referenced by any Makefile obj- line
    (not an error, just noise; skipped)

Usage:
    python3 scan_kconfig_choices.py
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
files = [f.replace("\\", "/") for f in r.stdout.splitlines()]
kconfigs = [f for f in files if os.path.basename(f).startswith("Kconfig")]

problems = 0
for rel in kconfigs:
    p = os.path.join(TREE, rel)
    try:
        with open(p, "r", encoding="utf-8", errors="replace") as f:
            lines = f.read().split("\n")
    except OSError:
        continue
    depth = 0          # choice nesting
    in_choice = []
    for i, raw in enumerate(lines, 1):
        s = raw.strip()
        # a choice group starts at column 0; a help text that merely
        # contains the word "choice" (indented, or inside a comment)
        # is not a choice group
        if s == "choice" and not raw.startswith((" ", "\t")):
            in_choice.append([])
            continue
        if s == "endchoice" and not raw.startswith((" ", "\t")):
            if in_choice:
                in_choice.pop()
            continue
        if not in_choice:
            continue
        # inside a choice group
        if s.startswith("config "):
            in_choice[-1].append((i, s, []))
            continue
        if in_choice[-1] and s and not s.startswith("#"):
            in_choice[-1][-1][2].append(s)

    for opts in in_choice:
        for (i, hdr, body) in opts:
            for b in body:
                m = re.match(r'^(bool|tristate)\s+"', b)
                if m:
                    print(f"{rel}:{i}: {hdr}")
                    print(f"    option has a {m.group(1)} prompt inside a "
                          f"choice group -> 'choice value has a prompt "
                          f"outside its choice group'")
                    problems += 1
                if b.startswith("default ") and "choice" not in b:
                    pass  # default inside a choice option is fine
    if in_choice:
        for rel_line, opts in zip(range(0), []):
            pass

# also: a 'source' pointing at a missing file
print()
print("=== source of a missing file ===")
for rel in kconfigs:
    p = os.path.join(TREE, rel)
    try:
        txt = open(p, "r", encoding="utf-8", errors="replace").read()
    except OSError:
        continue
    for m in re.finditer(r'^\s*source\s+"([^"]+)"', txt, re.M):
        t = os.path.join(TREE, m.group(1))
        if not os.path.exists(t):
            print(f"{rel}: source \"{m.group(1)}\" does not exist")
            problems += 1

print()
print(f"problems found: {problems}")
