# SPDX-License-Identifier: GPL-2.0
#
# codec_sc2730/Makefile — sprd-codec + headset (pengganti blob
# snd-soc-sprd-codec-sc2730; power manager sudah ada di codec/).

VXTUX_ASOC := drivers/vxtux/sound/asoc

ccflags-y += -I$(srctree)/$(VXTUX_ASOC)/codec_sc2730 \
             -I$(srctree)/$(VXTUX_ASOC)/card/include \
             -I$(srctree)/$(VXTUX_ASOC)/codec \
             -I$(srctree)/$(VXTUX_ASOC)/platform_include \
             -I$(srctree)/drivers/vxtux/audio/agdsp_access

snd-soc-sprd-codec-sc2730-objs := sprd-codec.o sprd-headset-sc2730.o
obj-$(CONFIG_VXTUX_SND_SPRD_SC2730) += snd-soc-sprd-codec-sc2730.o
