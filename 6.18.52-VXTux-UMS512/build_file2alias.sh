cd /workspaces/bb_build || exit 1
# Build ONLY the host tool file2alias (a host binary; it does not touch any
# kernel .o) so we can confirm the OF aliases that will land in .modinfo.
make -s --no-print-directory ARCH=arm64 HOSTCC=gcc scripts/mod/file2alias 2>&1 | tail -5
RC=$?
echo "FILE2ALIAS_BUILD_RC=$RC"
ls -la scripts/mod/file2alias 2>&1
