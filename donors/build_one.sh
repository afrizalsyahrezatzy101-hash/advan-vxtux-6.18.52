#!/usr/bin/env bash
# Compile ONE module in the codespace and report the result.
#
#   bash donors/build_one.sh <dir> <target>
#
# <dir>   e.g. drivers/nvmem
# <target> e.g. nvmem-sc27xx-efuse.o   (the .o, not the .ko, so modpost
#          does not run and no other worker's build state is disturbed)
set -u
CS="solid-tribble-9rjjxvjqq5pcpq5g"
DIR="$1"
TARGET="$2"
BASEC="$(basename "$TARGET" .o)"

gh codespace ssh -c "$CS" -- "cd /workspaces/bb_build && \
  rm -f $DIR/.$BASEC.o.cmd && \
  ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- HOSTCC=gcc HOSTCXX=g++ \
  make $DIR/$TARGET"
