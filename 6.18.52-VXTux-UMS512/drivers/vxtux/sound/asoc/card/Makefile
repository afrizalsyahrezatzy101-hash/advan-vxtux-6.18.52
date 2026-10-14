# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/sound/asoc/card/Makefile — utilitas kartu ASoC Unisoc.
# Port 5.4 -> 6.18 dari fork GPL sound/soc/sprd (sprd-asoc-card-utils dll).
#
# Vendor membagi ini ke modul blob snd-soc-sprd-card (T 53 simbol) — di pohon
# ini dibangun built-in sebagai satu objek gabungan supaya simbol koglobals
# (aon_apb_gpr dll) bisa dipakai lintas berkas tanpa ekspor berlebih.

VXTUX_ASOC := drivers/vxtux/sound/asoc
ccflags-y += -I$(srctree)/$(VXTUX_ASOC)/card/include \
             -I$(srctree)/$(VXTUX_ASOC)/codec \
             -I$(srctree)/$(VXTUX_ASOC)/codec_sc2730 \
             -I$(srctree)/drivers/vxtux/audio/agdsp_access

sprd-asoc-card-utils-objs := sprd-asoc-common.o \
			     sprd-asoc-card-utils.o \
			     sprd-asoc-card-utils-hook.o \
			     sprd-asoc-card-utils-legacy.o \
			     sprd-asoc-dfm.o

obj-$(CONFIG_VXTUX_SND_SPRD_CARD) += sprd-asoc-card-utils.o
