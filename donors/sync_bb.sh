#!/usr/bin/env bash
# Sync the local tree into the isolated build tree /workspaces/bb_build in ONE
# ssh hop, by tarring the changed files and streaming them over.
#
#   bash donors/sync_bb.sh            # files changed in the last 6 hours
#   bash donors/sync_bb.sh --all      # the whole drivers/ + arch/dts + include
#
# push_lf.py pushes into /workspaces/vxtux-build (shared with the agents); the
# authoritative build runs in /workspaces/bb_build, so this copies there too.
set -u

CS="solid-tribble-9rjjxvjqq5pcpq5g"
DST="/workspaces/bb_build"
HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE/.." || exit 1
TREE="6.18.52-VXTux-UMS512"

case "${1:-}" in
  --all)
    mapfile -t FILES < <(cd "$TREE" && find drivers arch/arm64/boot/dts include \
        -type f \( -name '*.c' -o -name '*.h' -o -name 'Makefile' -o -name 'Kconfig' \) 2>/dev/null)
    ;;
  *)
    mapfile -t FILES < <(cd "$TREE" && find drivers arch/arm64/boot/dts include \
        -type f \( -name '*.c' -o -name '*.h' -o -name 'Makefile' -o -name 'Kconfig' \) \
        -newermt '-6 hours' 2>/dev/null)
    ;;
esac

[ ${#FILES[@]} -eq 0 ] && { echo "nothing to sync"; exit 0; }
echo "syncing ${#FILES[@]} files"

# tar them up in a scratch dir with the tree-relative paths, then stream it
TMP="${TMPDIR:-/tmp}/bbsync"
rm -rf "$TMP"; mkdir -p "$TMP"
n=0
for f in "${FILES[@]}"; do
  mkdir -p "$TMP/$(dirname "$f")"
  cp "$TREE/$f" "$TMP/$f" && n=$((n+1))
done
echo "staged $n files"

if gh codespace ssh -c "$CS" -- "tar -C $DST -xf -" < <(cd "$TMP" && tar -cf - .) 2>/dev/null; then
  echo "OK  streamed into $DST"
else
  echo "FAIL stream"
fi

rm -rf "$TMP"