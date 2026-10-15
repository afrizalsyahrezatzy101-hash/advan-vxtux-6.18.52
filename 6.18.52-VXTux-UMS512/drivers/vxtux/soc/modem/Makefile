# 2026-10-11: sprd_modem.o REMOVED. Its source (sprd_modem.c, a firmware-load
# stub written 2026-10-04) was deleted in the 2026-10-10 restructure and exists
# in NO donor, NO archive and NO tree (host or device); nothing in the tree ever
# called its symbols. The orphaned obj- line broke every full rebuild with
# "No rule to make target 'sprd_modem.o'" (build_1011b.log, VMLINUX_RC=2, died
# 2026-10-11 08:42 UTC). The three REAL vendor ports below are gated by their
# own symbols and are unaffected.

# Port 2026-10-04: the real modem drivers. Without these lines the directory
# built sprd_modem.o alone and none of cproc/cp_dump/modem_loader could ever be
# linked, which is why /proc/cptl never appeared.
obj-$(CONFIG_SPRD_CPROC)      += cproc/
obj-$(CONFIG_SPRD_CP_DUMP)    += cp_dump/
obj-$(CONFIG_SPRD_MODEM_LOADER) += modem_loader/
