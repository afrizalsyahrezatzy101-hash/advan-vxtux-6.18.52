#!/bin/bash
# Regenerate this report's gdb capture on the codespace.
# Codespace: solid-tribble-9rjjxvjqq5pcpq5g
# Workspace:  /workspaces/vxtux-build/6.18.52-VXTux-UMS512
set -x
cd /workspaces/vxtux-build/6.18.52-VXTux-UMS512 || exit 9

CF="-g -O0 -std=gnu11 -Wall -I ./scripts/include"

# ---------- 1. The requested command, verbatim, against a -g -O0 build ----------
gcc $CF -c scripts/mod/modpost.c    -o /tmp/modpost_dbg.o
gcc $CF -c scripts/mod/file2alias.c -o /tmp/file2alias_dbg.o
gcc $CF -c scripts/mod/sumversion.c -o /tmp/sumversion_dbg.o
gcc $CF -c scripts/mod/symsearch.c  -o /tmp/symsearch_dbg.o
gcc -g -o /tmp/modpost_dbg /tmp/modpost_dbg.o /tmp/file2alias_dbg.o \
	/tmp/sumversion_dbg.o /tmp/symsearch_dbg.o

gdb --batch \
  -ex "set print pretty on" \
  -ex run -ex bt full -ex quit \
  --args /tmp/modpost_dbg -o /tmp/s0.symvers -e ./init/main.o ./init/version.o

# ---------- 2. Repro build that DOES crash: Bionic-style ELF64_R_SYM (i)>>8 ----------
rm -rf /tmp/crashrepro; mkdir -p /tmp/crashrepro
cp scripts/mod/modpost.h /tmp/crashrepro/modpost.h.CURRENT
git show 4212680b^:6.18.52-VXTux-UMS512/scripts/mod/modpost.h \
	> /tmp/crashrepro/modpost.h.BUGGY
# inject the Bionic encoding after the ELF_R_SYM alias block
python3 - <<'PY'
s = open("/tmp/crashrepro/modpost.h.BUGGY").read()
inject = """
#undef  ELF64_R_SYM
#undef  ELF64_R_TYPE
#define ELF64_R_SYM(i)   ((i) >> 8)
#define ELF64_R_TYPE(i)  ((i) & 0xff)
#undef  ELF_R_SYM
#undef  ELF_R_TYPE
#define ELF_R_SYM        ELF64_R_SYM
#define ELF_R_TYPE       ELF64_R_TYPE
"""
s = s.replace("#define bswap(x) \\", inject + "#define bswap(x) \\", 1)
open("/tmp/crashrepro/modpost.h.BIONIC", "w").write(s)
PY
cp /tmp/crashrepro/modpost.h.BIONIC scripts/mod/modpost.h
gcc $CF -c scripts/mod/modpost.c    -o /tmp/crashrepro/B_modpost.o
gcc $CF -c scripts/mod/file2alias.c -o /tmp/crashrepro/B_file2alias.o
gcc $CF -c scripts/mod/sumversion.c -o /tmp/crashrepro/B_sumversion.o
gcc $CF -c scripts/mod/symsearch.c  -o /tmp/crashrepro/B_symsearch.o
gcc -g -o /tmp/crashrepro/modpost_BIONICBUG /tmp/crashrepro/B_*.o
cp /tmp/crashrepro/modpost.h.CURRENT scripts/mod/modpost.h   # restore

gdb --batch \
  -ex "set print pretty on" \
  -ex "set pagination off" \
  -ex run -ex bt -ex "bt full" -ex "info registers" \
  -ex "thread apply all bt" \
  -ex 'disassemble $pc-48,$pc+32' \
  -ex "frame 0" -ex "info args" -ex "info locals" -ex list -ex quit \
  --args /tmp/crashrepro/modpost_BIONICBUG -o /tmp/s0.symvers \
         -e ./init/main.o ./init/version.o
/tmp/crashrepro/modpost_BIONICBUG -o /tmp/s0.symvers \
	-e ./init/main.o ./init/version.o >/dev/null 2>&1
echo "RC=$?"          # expect 139

# ---------- 3. gcc -E : what ELF_R_SYM expands to ----------
gcc -E -I scripts/include scripts/mod/modpost.c 2>/dev/null > /tmp/pp_fixed.c
awk '/^static void get_rel_type_and_sym/,/^\}/' /tmp/pp_fixed.c

git --no-pager diff --stat HEAD -- scripts/mod/    # must show only modpost.c