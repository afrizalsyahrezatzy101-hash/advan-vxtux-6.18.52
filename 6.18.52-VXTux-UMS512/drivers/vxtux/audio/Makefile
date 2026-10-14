# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/audio/Makefile — infrastruktur audio Unisoc (AGDSP/AGCP)
#
# Semua simbol default n (lihat Kconfig): aktifkan satu per satu setelah API
# 6.18 sub-sistem itu selesai. Urutan aman: audio_mem -> agdsp_access ->
# audio_sipc -> dsp_dump/saudio -> cpboot/dvfs.

# Nama direktori sengaja sama dengan fork GPL supaya Makefile internal
# (dari vendor) tetap valid dan diff saat upstreaming tetap kecil.
obj-$(CONFIG_SPRD_AUDIO_MEMORY_MANAGER)	+= audiomem/
obj-$(CONFIG_SPRD_AUDIO_AGDSP_ACCESS)	+= agdsp_access/
obj-$(CONFIG_SPRD_AUDIO_SIPC)		+= audiosipc/
obj-$(CONFIG_SPRD_AUDIO_DSP_DUMP)	+= audiodspdump/
obj-$(CONFIG_SPRD_AUDIO_SAUDIO)		+= saudio/
obj-$(CONFIG_SPRD_AUDIO_PIPE)		+= audio_pipe/
obj-$(CONFIG_SPRD_AUDIO_CPBOOT)		+= audiocpboot/
obj-$(CONFIG_SPRD_HW_DEVICE_DVFS_AGCP)	+= audiodvfs/
# mcdt/ = SATU direktori, DUA varian saling meniadakan (r1p0/r2p0). Descent
# direktori ini dikendalikan SATU simbol umbrella (SPRD_AUDIO_MCDT); pemilihan
# variannya ada di mcdt/Kconfig lewat `choice`, jadi direktori tidak pernah
# muncul dua kali di obj-y (kelas cacat yang sama pernah terjadi di
# sprdwcn/Makefile untuk platform/).
obj-$(CONFIG_SPRD_AUDIO_MCDT)		+= mcdt/
obj-$(CONFIG_SPRD_AUDIO_VOICE_TRIGGER)	+= voice_trigger/
