# SPDX-License-Identifier: GPL-2.0
#
# dai_v4/Makefile — vbc-dai + vbc-phy-v4 + vbc-codec + sprd-fe-dai.
# Vendor membagi ini ke dua blob (snd-soc-sprd-vbc-v4 + snd-soc-sprd-vbc-fe);
# di pohon ini satu objek gabungan (fe-dai memanggil helper vbc antar-file).
# vbc-dai.c #include "vbc-codec.c" (pola vendor) — daftar objek hanya 2 .c.

VXTUX_ASOC := drivers/vxtux/sound/asoc

ccflags-y += -I$(srctree)/$(VXTUX_ASOC)/platform_include \
             -I$(srctree)/$(VXTUX_ASOC)/card/include \
             -I$(srctree)/drivers/vxtux/audio/audiosipc \
             -I$(srctree)/drivers/vxtux/audio/audiosipc/include \
             -I$(srctree)/drivers/vxtux/audio/audiomem \
             -I$(srctree)/drivers/vxtux/audio/agdsp_access \
             -I$(srctree)/drivers/vxtux/audio/mcdt

snd-soc-sprd-vbc-v4-objs := vbc-dai.o vbc-phy-v4.o sprd-fe-dai.o
obj-$(CONFIG_VXTUX_SND_SPRD_VBC_V4) += snd-soc-sprd-vbc-v4.o
