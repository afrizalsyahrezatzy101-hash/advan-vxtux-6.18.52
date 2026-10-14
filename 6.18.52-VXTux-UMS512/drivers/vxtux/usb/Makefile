# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/usb — port driver USB pendukung hasil RE/blob.

obj-$(CONFIG_VXTUX_SC27XX_TYPEC)	+= sc27xx_typec.o

# B42 (2026-09-26): penyedia ABI rantai RNDIS vendor. Nama objek sengaja
# SAMA dengan nama modul blob (sprd_u_ether.ko / sprd_usb_f_rndis.ko) supaya
# =m menghasilkan .ko yang bisa menggantikan blob-nya langsung.
obj-$(CONFIG_VXTUX_SPRD_U_ETHER)	+= sprd_u_ether.o
obj-$(CONFIG_VXTUX_SPRD_F_RNDIS)	+= sprd_usb_f_rndis.o
