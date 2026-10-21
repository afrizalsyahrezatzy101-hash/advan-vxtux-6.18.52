# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/sipc/Makefile — kerangka SIPC Unisoc (port fork GPL 5.4 -> 6.18)
#
# Shim API mailbox vendor: wajib ada sebelum apa pun yang memakai SIPC.
# (SPRD_MAILBOX = bool, default y; penyedia mainline sprd-mailbox = y.)
obj-$(CONFIG_SPRD_MAILBOX)	+= sprd_mbox_vendor.o

# Inti SIPC: SENGAJA built-in saja. Bukti call-graph (2026-09-23):
# agdsp_access (=y) -> smsg_* (smsg.c), dan sound ASoC (=y) -> agdsp_*;
# audio-smsg.o hanya dibangun di bawah SPRD_AUDIO_SIPC (=y). Membuat inti
# SIPC modular berarti vmlinux menunjuk simbol modul -> undefined at link.
obj-$(CONFIG_SPRD_SIPC)		+= sipc.o smsg.o smem.o sbuf.o sblock.o \
			   sipc_debugfs.o sprd_actions_queue.o

# slog_bridge talks to the USB VSER gadget function (usb_f_vserial.ko, a
# MODULE) through kernel_vser_*. A built-in object cannot import a module
# symbol, so slog_bridge is a module of its own: it imports the built-in
# SIPC core (sblock_*) and the usb_f_vserial module.
obj-$(CONFIG_SPRD_SIPC_SLOG_BRIDGE) += slog_bridge.o

# Transport SBLK khusus WCN (vendor swcn_blk.c): konsumen klosennya (wcn_sipc,
# lapisan integrate) tidak dibangun di papan SDIO ini, jadi transport ini
# dibangun sebagai MODUL agar bisa di-insmod/diuji satu per satu (QEMU).
# Komposit sendiri: swcnblk.c memanggil sblock_* dari inti SIPC (=y) sehingga
# swcnblk.ko cukup satu objek.
obj-$(CONFIG_SPRD_SIPC_SWCNBLK)	+= swcnblk.o

# LTE/modem transports (2026-10-10): sipx MUST be =y (bool provider of the
# 12 sipx_* symbols seth.o undefs); seth/spipe/spool =m so they insmod one
# at a time during bring-up. Insmod order: mailbox -> sipc-core -> sipx ->
# seth/spool/spipe.
obj-$(CONFIG_SPRD_SIPC_ZERO_COPY_SIPX) += sipx.o
obj-$(CONFIG_SPRD_SIPC_SETH)    += seth.o
obj-$(CONFIG_SPRD_SIPC_SPOOL)   += spool.o
obj-$(CONFIG_SPRD_SIPC_SPIPE)   += spipe.o

# Virtual SIPC bus (blob sprd-sipc-virt-bus.ko).  ONE composite module
# named after the vendor blob it replaces, so the built file is
# sprd-sipc-virt-bus.ko and KBUILD_MODNAME is sprd_sipc_virt_bus.
#
# kbuild derives the module name from the obj-$(CONFIG_X) target, not
# from the .c file name, so the target has to carry the hyphens.  A
# single object, so no xxx-objs composite is needed.
obj-$(CONFIG_SPRD_SIPC_VIRT_BUS) += sprd-sipc-virt-bus.o
sprd-sipc-virt-bus-y := sprd_sipc_virt_bus.o
