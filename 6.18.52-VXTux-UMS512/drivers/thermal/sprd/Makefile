# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/thermal/Makefile — LinDroid/VXTux v1.0

# VXTux: salinan lama. Driver pindah ke drivers/vxtux/thermal/ (arah pohon
# port); dua-duanya terpasang di obj-y -> LD vmlinux.o gagal "multiple
# definition of create_gpu_cooling_device". Lokasi lama dipertahankan sebagai
# referensi tapi TIDAK dibangun - sama seperti drivers/net/wireless/sprd/sprdwcn.
# obj-$(CONFIG_VXTUX_SPRD_GPU_COOLING)	+= sprd_gpu_cooling.o
obj-$(CONFIG_SPRD_TSENSOR_THERMAL)	+= sc27xx_tsensor_thermal.o
obj-$(CONFIG_SPRD_SOC_THM)	+= sprd_soc_thm.o
obj-$(CONFIG_SPRD_CPU_COOLING)	+= sprd_cpu_cooling.o
