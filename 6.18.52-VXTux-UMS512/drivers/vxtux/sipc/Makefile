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
				   sipc_debugfs.o slog_bridge.o sprd_actions_queue.o

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
