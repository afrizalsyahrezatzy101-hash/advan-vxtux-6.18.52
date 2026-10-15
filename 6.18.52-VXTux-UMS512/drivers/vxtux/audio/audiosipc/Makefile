# SPRD Audio SIPC Support
#
# Port 6.18: include path vendor lama (drivers/staging/sprd/modem/include dan
# sound/soc/sprd/include) sudah tidak ada di tree ini. Header yang dibutuhkan
# divendor-kan: audio_mem.h (di ../audiomem) dan sprd-string.h (di include/).

VXTUX_AUDIO_TREE := drivers/vxtux/audio
ccflags-y	+= -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiosipc/include \
		   -I$(srctree)/$(VXTUX_AUDIO_TREE)/audiomem/

obj-$(CONFIG_SPRD_AUDIO_SIPC) += audio-sipc.o audio-smsg.o audio-pipe.o
