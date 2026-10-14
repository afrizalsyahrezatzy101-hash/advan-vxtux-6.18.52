# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/sound/asoc/codec/Makefile — codec kecil ASoC Unisoc.
#   dummy-codec       : DAI dummy playback (blob snd-soc-sprd-dummy-codec)
#   sprd-audio-power  : manajer daya audio SC27xx (blob snd-soc-sprd-codec-
#                       sc2730-power; T-nya hanya typeinfo artifact)

VXTUX_ASOC := drivers/vxtux/sound/asoc
ccflags-y += -I$(srctree)/$(VXTUX_ASOC)/card/include \
             -I$(srctree)/$(VXTUX_ASOC)/codec_sc2730 \
             -I$(srctree)/drivers/vxtux/audio/agdsp_access

obj-$(CONFIG_VXTUX_SND_SPRD_DUMMY_CODEC) += dummy-codec.o
obj-$(CONFIG_VXTUX_SND_SPRD_SC2730_POWER) += sprd-audio-power.o
