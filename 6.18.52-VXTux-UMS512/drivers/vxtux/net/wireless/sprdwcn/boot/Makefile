# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/net/wireless/sprdwcn/boot/Makefile
#
# Vendor menulis -Idrivers/misc/sprdwcn/... (lokasi asli fork). Setelah
# port, pohonnya ada di drivers/vxtux/net/wireless/sprdwcn, jadi keempat
# -I itu menunjuk direktori yang tidak ada. Induk (../Makefile) sudah
# menambahkan subdir-ccflags-y dengan -I yang benar, tapi ccflags-y di
# sini tetap dipertahankan (eksplisit) supaya berkas ini bisa dibaca
# mandiri tanpa melihat ke atas.
ccflags-y += -I$(srctree)/$(src)/../include
ccflags-y += -I$(srctree)/$(src)/../platform
ccflags-y += -I$(srctree)/$(src)/../platform/rf

# Penjaga tetap nama vendor: WCN_INTEG (chip integrated). Simbol ini
# WAJIB =y di fragmen karena WCN_SIPC/WCN_PLATFORM bergantung padanya.
obj-$(CONFIG_WCN_INTEG) += wcn_integrate_dev.o
obj-$(CONFIG_WCN_INTEG) += wcn_integrate_boot.o
obj-$(CONFIG_WCN_INTEG) += wcn_integrate.o
