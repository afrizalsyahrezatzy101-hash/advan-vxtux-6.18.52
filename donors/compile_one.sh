#!/bin/bash
# Compile ONE file of the mm-dvfs driver to /tmp/agent5, without touching
# the shared kbuild state of the codespace.
#
#   compile_one.sh <file.c> [outname]
#
# The compile command is copied verbatim out of the .o.cmd file of a
# previously built object in the same directory, with only the -o target
# and the source path rewritten.  Nothing is written into the source tree.

set -u
BUILD=/workspaces/bb_build
OUT=/tmp/agent5
SRC="$1"
NAME="${2:-$(basename "$SRC" .c)}"

mkdir -p "$OUT"

# Borrow the command from any existing .o.cmd in the tree; the flags are
# identical for every object in the tree.
CMD=$(python3 - "$BUILD" <<'PYEOF'
import glob, os, re, sys
build = sys.argv[1]
cands = sorted(glob.glob(os.path.join(build, "drivers/devfreq/sprd/*.o.cmd")))
if not cands:
    cands = sorted(glob.glob(os.path.join(build, "drivers/**/*.o.cmd"), recursive=True))
for c in cands:
    for line in open(c, errors="replace"):
        if line.startswith("savedcmd_") and " := " in line:
            print(line.split(" := ", 1)[1].strip())
            sys.exit(0)
sys.exit(1)
PYEOF
)
if [ -z "$CMD" ]; then
    echo "FAILED: no savedcmd found"
    exit 1
fi

# Rewrite the output target and the source file; leave every flag alone.
CMD=$(printf '%s' "$CMD" | sed \
    -e 's#-o [^ ]*\.o #-o '"$OUT/$NAME.o"' #' \
    -e 's#-Wp,-MMD,[^ ]*\.o\.d #-Wp,-MMD,'"$OUT/$NAME.o.d"'#' \
    -e 's#-c [^ ]*\.c #-c '"$SRC"'#')

cd "$BUILD" || exit 1
eval "$CMD"
RC=$?
echo "COMPILE_RC=$RC  ($SRC)"
exit $RC
