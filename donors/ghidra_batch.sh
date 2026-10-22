#!/usr/bin/env bash
# Batch-import every stock vendor blob into Ghidra headless and dump its
# function + data symbol lists.
#
#   bash donors/ghidra_batch.sh
#
# Idempotent: a blob already present in E:\ghidra_proj is re-used, only the
# dump step re-runs. Output lands in donors/ghidra_out/<blob>.funcs.txt and
# .data.txt.
set -u

PROJ="E:\\ghidra_proj"
GH="E:\\tools\\ghidra_headless.cmd"
OUT="E:\\vxtux_build_repo\\donors\\ghidra_out"
BLOBS="E:\\vxtux_build_repo\\donors\\stock_vendor_modules"

cd /e/vxtux_build_repo || exit 1
mkdir -p donors/ghidra_out

for ko in donors/stock_vendor_modules/*.ko; do
  base=$(basename "$ko")
  name="${base%.ko}"
  safe=$(echo "$name" | tr -c 'A-Za-z0-9_-' '_')

  if [ ! -s "donors/ghidra_out/$base.funcs.txt" ]; then
    echo "=== $base: import + dump ==="
    rm -rf "/e/ghidra_proj/$safe" 2>/dev/null
    cmd.exe /c "$GH $PROJ $safe -import $(cygpath -w "$ko") -processor AARCH64:LE:64:v8A" 2>&1 \
      | grep -E "Import succeeded|ERROR|Exception" | head -2
    cmd.exe /c "$GH $PROJ $safe -process -scriptPath E:\\vxtux_build_repo\\donors\\ghidra \
      -postScript DumpSyms.java list" 2>&1 \
      | grep -E "wrote|ERROR|Exception" | head -3
  else
    echo "=== $base: cached ==="
  fi
done

echo ""
echo "=== result ==="
ls -la donors/ghidra_out/*.funcs.txt 2>/dev/null | awk '{print $5, $9}' | tail -35