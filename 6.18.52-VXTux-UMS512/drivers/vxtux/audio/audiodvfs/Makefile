SPRD_SND_TREE   := sound/soc/sprd/
SPRD_AUDIO_ASSISTANT_TREE := drivers/vxtux/audio/
# $(srctree)/ WAJIB (build O=<dir>): path relatif salah resolve saat kbuild
# berjalan dari buildd/. audio-sipc.h tinggal di audiosipc/ (bukan include/).
ccflags-y += -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiomem/ \
         -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiosipc/ \
         -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiosipc/include/ \
         -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/agdsp_access/ \
         -I$(srctree)/$(SPRD_SND_TREE)/include

obj-$(CONFIG_SPRD_HW_DEVICE_DVFS_AGCP)     += sprd_audcp_dvfs.o
