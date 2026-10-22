set -e
cd /tmp/agent0/modpost

# modpost needs a vmlinux ELF to resolve the kernel symbols our module
# imports (__platform_driver_register, devm_of_platform_populate, ...).
# Use the real vmlinux from the main build tree.
VL=/workspaces/bb_build/vmlinux
if [ ! -f "$VL" ]; then
    echo "NO_VMLINUX"
    exit 2
fi

echo "=== modpost run (with real vmlinux) ==="
/workspaces/bb_build/scripts/mod/modpost -m -o modpost_out.txt \
    -I /workspaces/bb_build/Module.symvers \
    "$VL" .modpost 2>&1 | head -30 || true

echo "=== generated .mod.c alias lines ==="
for f in *.mod.c; do
    [ -f "$f" ] || continue
    echo "--- $f ---"
    grep -E 'MODULE_INFO\((alias|name|license|description|author)' "$f" | head -20
done
