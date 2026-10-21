cd /workspaces/bb_build || exit 1
mkdir -p /tmp/agent0 || exit 1

# Extract the exact kbuild compile command for our module object, rewrite
# the -o target to /tmp/agent0/, and re-run it.  Nothing is written inside
# the source tree, so this cannot disturb any other build.
CMD=$(python3 - <<'PYEOF'
import re
d = open('drivers/vxtux/sipc/.sprd_sipc_virt_bus.o.cmd').read().splitlines()
for line in d:
    if 'savedcmd_' in line and 'sprd_sipc_virt_bus.o :=' in line:
        c = line.split(' := ', 1)[1]
        c = c.replace('-Wp,-MMD,drivers/vxtux/sipc/.sprd_sipc_virt_bus.o.d',
                      '-Wp,-MMD,/tmp/agent0/sprd_sipc_virt_bus.o.d')
        c = re.sub(r'-o drivers/vxtux/sipc/sprd_sipc_virt_bus\.o',
                   '-o /tmp/agent0/sprd_sipc_virt_bus.o', c)
        c = c.replace('-c drivers/vxtux/sipc/sprd_sipc_virt_bus.c',
                      '-c drivers/vxtux/sipc/sprd_sipc_virt_bus.c')
        print(c)
        break
PYEOF
)

if [ -z "$CMD" ]; then
    echo "FAILED: could not extract compile command"
    exit 1
fi
echo "COMPILE_CMD_LEN=${#CMD}"
echo "=== running compile to /tmp/agent0 ==="
eval "$CMD"
RC=$?
echo "COMPILE_RC=$RC"
ls -la /tmp/agent0/
exit $RC
