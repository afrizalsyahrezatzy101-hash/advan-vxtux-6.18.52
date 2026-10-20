# SPDX-License-Identifier: GPL-2.0
#
# The SIPC core (audio-sipc.o + audio-smsg.o) is BUILT-IN, on purpose.
#
# It provides aud_smsg_ch_open / aud_smsg_recv / aud_smsg_send /
# aud_smsg_ch_close, and the built-in drivers/sound/soc/sprd/
# agdsp_access/ calls them through a static inline shim
# (vxtux-smsg-shim.h). A built-in object cannot reference a module's
# symbol, so making this a module broke the vmlinux link:
#
#   vxtux-smsg-shim.h:48: undefined reference to `aud_smsg_ch_open'
#
# The SIPC core therefore stays in vmlinux, and only the pipe becomes
# a module. The pipe is the stock audio-pipe.ko; as a module it gets
# the aud_smsg_* symbols from the built-in provider's export table,
# which is how a module is supposed to use a built-in.
#
# The vendor include paths (drivers/staging/sprd/modem/include and
# sound/soc/sprd/include) do not exist in this tree. The headers that
# are needed are vendored: audio_mem.h (in ../audiomem) and
# sprd-string.h (in include/).

VXTUX_AUDIO_TREE := drivers/vxtux/audio
ccflags-y	+= -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiosipc/include \
		   -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiomem/

obj-$(CONFIG_SPRD_AUDIO_SIPC) += audio-sipc.o audio-smsg.o

obj-$(CONFIG_SPRD_AUDIO_PIPE) += audio-pipe.o
audio-pipe-y := audio-pipe.o
