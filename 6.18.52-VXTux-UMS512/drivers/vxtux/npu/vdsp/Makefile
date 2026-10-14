# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/npu/vdsp/Makefile
#
# sprd_vdsp.ko — driver VDSP (NPU/video DSP; firmware vdsp_firmware.bin di
# /vendor/firmware). Port donor catalog mods-sprd/sprd/kernel4.14/npu/vdsp
# (varian Cadence/XRP; Kbuild donor: KO_MODULE_NAME := sprd_vdsp).
#
# Kbuild donor memilih subdir lewat BSP_BOARD_CAMERA_MODULE_VDSP_DEVICE
# (env Android: "Cadence" atau "Ceva"); di sini ditetapkan eksplisit ke
# Cadence supaya deterministik — dan Kbuild/Makefile donor sengaja TIDAK
# disalin (Kbuild memakai wildcard $(src), pola external-module).
#
# Pemetaan dependensi donor -> tree (dipilih saat audit 2026-10-14):
#   "ion.h" + ion_new_alloc()   -> ion.h lokal (compat, sesama direktori)
#                                  -> include/linux/ion.h shim marohinmark
#                                  UMS512 5.4, ion_alloc() signature sama
#   sprd_ion_*                  -> drivers/dma-buf/sprd (in-tree, ter-EXPORT)
#   sprd_iommu_*                -> include/linux/sprd_iommu.h + unisoc-iommu
#   sprd_dvfs_vdsp.h /
#   vdsp_dvfs_sharkl5pro.h      -> drivers/devfreq/sprd/apsys (diambil dari
#                                  mirror GitHub iscle W20.13.3, GPL-2)
#   vdsp_dvfs_notifier_call_chain -> inline stub: guard
#         CONFIG_SPRD_APSYS_DVFS_DEVFREQ tidak ada di tree ini, jadi eksternal
#         vendor sprd_dvfs_vdsp.c tidak ikut dipakai (dvfs chain = no-op).
# Error API 4.14 -> 6.18 diperbaiki satu per satu saat compile loop.

obj-$(CONFIG_SPRD_VDSP) := sprd_vdsp.o

sprd_vdsp-y := $(patsubst $(src)/%.c,%.o,$(wildcard $(src)/Cadence/xrp/*.c))

ccflags-y += -I$(src)
ccflags-y += -I$(src)/Cadence/xrp
ccflags-y += -I$(src)/common
ccflags-y += -I$(srctree)/drivers/devfreq/sprd/apsys
ccflags-y += -DDEBUG
ccflags-y += -DKERNEL_VERSION_414

ifeq ($(CONFIG_COMPAT),y)
ccflags-y += -DCONFIG_COMPAT
endif

# TEMP-DIAG
$(info VDBG src=[$(src)] srcroot=[$(srcroot)] cfg=[$(CONFIG_SPRD_VDSP)] objm=[$(obj-m)] y=[$(sprd_vdsp-y)] wc=[$(wildcard $(src)/Cadence/xrp/*.c)])
