# SPDX-License-Identifier: GPL-2.0
#
# TWO composite modules, each named after the vendor blob it replaces:
#   audio_sipc.ko  <- audio-sipc.o audio-smsg.o   (the SIPC layer)
#   audio-pipe.ko  <- audio-pipe.o               (the chardev pipe)
#
# The stock ROM shipped these as two separate blobs, so they have to be
# two separate modules. Listing the objects one by one would make
# kbuild emit a module per object with the wrong names.
#
# The vendor include paths (drivers/staging/sprd/modem/include and
# sound/soc/sprd/include) do not exist in this tree. The headers that
# are needed are vendored: audio_mem.h (in ../audiomem) and
# sprd-string.h (in include/).

VXTUX_AUDIO_TREE := drivers/vxtux/audio
ccflags-y	+= -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiosipc/include \
		   -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiomem/

obj-$(CONFIG_SPRD_AUDIO_SIPC) += audio_sipc.o
audio_sipc-y := audio-sipc.o audio-smsg.o

obj-$(CONFIG_SPRD_AUDIO_PIPE) += audio-pipe.o
audio-pipe-y := audio-pipe.o
