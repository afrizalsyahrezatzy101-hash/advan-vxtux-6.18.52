# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/audio/mcdt/Makefile — dua varian MCDT, saling meniadakan.
#
# Umbrella (descent direktori ini) = CONFIG_SPRD_AUDIO_MCDT di audio/Makefile.
# Di sini yang dipilih adalah VARIAN-nya, dan itu dijamin `choice` di Kconfig:
# tepat satu dari kedua simbol berikut bernilai y.
#
# Wiring 2026-09-19: baris descent r2p0 sebelumnya TIDAK ADA, sementara
# mcdt_r2p0/Makefile memakai CONFIG_SPRD_AUDIO_MCDT_R2P0 yang tidak ada di
# Kconfig mana pun -- cabang yang disebut dua kali tapi tak pernah bisa dicapai.
# Keduanya sekarang ada, dan ketercapaiannya dibuktikan objek yang terbentuk
# lewat tools/re/probe_wiring.sh (bukan diasumsikan dari isi Makefile).

obj-$(CONFIG_SPRD_AUDIO_MCDT_R1P0)	+= mcdt_r1p0/
obj-$(CONFIG_SPRD_AUDIO_MCDT_R2P0)	+= mcdt_r2p0/
