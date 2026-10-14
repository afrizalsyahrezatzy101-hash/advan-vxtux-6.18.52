# SPDX-License-Identifier: GPL-2.0
#
# machine/Makefile — mesin kartu (bagian blob snd-soc-sprd-card):
# vbc-rxpx-codec-sc27xx (compatible "sprd,vbc-v4-codec-sc2730").

VXTUX_ASOC := drivers/vxtux/sound/asoc

ccflags-y += -I$(srctree)/$(VXTUX_ASOC)/platform_include \
             -I$(srctree)/$(VXTUX_ASOC)/card/include \
             -I$(srctree)/$(VXTUX_ASOC)/codec_sc2730 \
             -I$(srctree)/drivers/vxtux/audio/agdsp_access

obj-$(CONFIG_VXTUX_SND_SPRD_SC2730) += vbc-rxpx-codec-sc27xx.o
