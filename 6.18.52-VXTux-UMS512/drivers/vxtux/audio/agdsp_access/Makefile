# SPRD Audio AGDSP Access Support

# $(srctree)/ wajib: dengan build O=<dir>, `-I` relatif diresolusi terhadap CWD
# proses kbuild = direktori keluaran, bukan pohon sumber (kelas cacat yang sama
# membuat mcdt_hw.h "file not found" di audio/mcdt/mcdt_r2p0).
#
# Catatan: drivers/staging/sprd/modem/include TIDAK ADA di pohon 6.18 dan tidak
# ada berkas agdsp_access yang meng-include dari sana -- jalur itu warisan
# vendor (modem staging belum diport). Dibiarkan sebagai penanda, tapi
# di-$(srctree)-kan supaya tidak diam-diam menunjuk ke tempat lain.
ccflags-y += -I$(srctree)/drivers/staging/sprd/modem/include/ \
             -I$(srctree)/drivers/vxtux/audio/audiosipc/include
obj-$(CONFIG_SPRD_AUDIO_AGDSP_ACCESS) += agdsp_access.o
