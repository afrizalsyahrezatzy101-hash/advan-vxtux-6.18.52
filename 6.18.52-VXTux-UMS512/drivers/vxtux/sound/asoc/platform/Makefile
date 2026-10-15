# SPDX-License-Identifier: GPL-2.0
#
# platform/Makefile — pcm-routing (pengganti blob snd-soc-sprd-pcm-routing).

VXTUX_ASOC := drivers/vxtux/sound/asoc

ccflags-y += -I$(srctree)/$(VXTUX_ASOC)/platform_include \
             -I$(srctree)/$(VXTUX_ASOC)/card/include \
             -I$(srctree)/drivers/vxtux/audio/agdsp_access \
             -I$(srctree)/drivers/vxtux/audio/audiomem \
             -I$(srctree)/drivers/vxtux/audio/audiosipc \
             -I$(srctree)/drivers/vxtux/audio/audiosipc/include \
             -I$(srctree)/drivers/vxtux/audio/mcdt

obj-$(CONFIG_VXTUX_SND_SPRD_PCM_ROUTING) += sprd-platform-pcm-routing.o
