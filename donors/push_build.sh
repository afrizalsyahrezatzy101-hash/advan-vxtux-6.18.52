#!/usr/bin/env bash
# Push one tree file into the codespace build tree and compile ONE module.
#
#   bash donors/push_build.sh <local-file> <tree-relative-path> <dir> <target.o>
#
# Compiles the .o only: modpost never runs, so no other worker's build
# state is disturbed.
set -u
CS="solid-tribble-9rjjxvjqq5pcpq5g"
REPO="E:/vxtux_build_repo"
LOCAL="$1"; TREE="$2"; DIR="$3"; TARGET="$4"
BASEC="$(basename "$TARGET" .o)"

gh codespace ssh -c "$CS" -- "cat > /workspaces/bb_build/$TREE" < "$LOCAL" || exit 1
gh codespace ssh -c "$CS" -- "cd /workspaces/bb_build && rm -f $DIR/.$BASEC.o.cmd && ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- HOSTCC=gcc HOSTCXX=g++ make $DIR/$TARGET $DIR/$BASEC.ko"
