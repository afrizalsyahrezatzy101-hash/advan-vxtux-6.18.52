# SPRD Audio Dsp Log Support

SPRD_SND_TREE	:= sound/soc/sprd/
SPRD_AUDIO_ASSISTANT_TREE := drivers/vxtux/audio/
# $(srctree)/ WAJIB: dengan O=<dir> kbuild menjalankan kompilasi dari buildd/, jadi
# path relatif seperti drivers/vxtux/audio/audiomem/ tidak pernah ketemu dan
# header port (audio_mem.h, agdsp_access.h) dilaporkan "file not found" meski
# berkasnya ada di pohon sumber.
ccflags-y += -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiomem/ \
	     -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiosipc/ \
	     -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/audiosipc/include/ \
	     -I$(srctree)/$(SPRD_AUDIO_ASSISTANT_TREE)/agdsp_access/ \
	     -I$(srctree)/$(SPRD_SND_TREE)/include

# Komposit SAH: SPRD_AUDIO_DSP_DUMP kini tristate (=m untuk uji insmod).
# anchor + -y = SATU modul berisi kedua objek, PERSIS seperti artefak stok:
# `dumped/re/hidden/vramdisk_ko/audio-dsp-dump.ko` memuat simbol audio_dsp_*
# DAN audio_sblock_* dalam satu .ko (lld -r), dan tak satu pun blob stok
# mengimpor audio_sblock_* -> tak ada modul terpisah di device.
# Anchor WAJIB berbeda dari nama anggota: `audio_dsp_dump.o` sebagai anchor dan
# sebagai anggota sekaligus membuat kbuild memotong dependensi sirkular dan
# `ld` melink objek ke dirinya sendiri (build #13: duplicate symbol di .text).
obj-$(CONFIG_SPRD_AUDIO_DSP_DUMP) += audio-dsp-dump.o
audio-dsp-dump-y := audio_sblock.o audio_dsp_dump.o
