SPRD_SND_TREE   := sound/soc/sprd/
SPRD_AUDIO_ASSISTANT_TREE := drivers/vxtux/audio/
# $(srctree)/ WAJIB (build O=<dir>): sebelumnya path relatif membuat
# audio_mem.h tidak ketemu walau berkasnya ada di pohon sumber.
ccflags-y += -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiomem/ \
		-I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiosipc/ \
		-I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiosipc/include/ \
         -I$(srctree)/$(SPRD_SND_TREE)/include
obj-y     += sprd_audcp_boot.o
