# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/net/wireless/sprdwcn/fm/driver/Makefile
#
# sprd_fm.ko — radio FM MARLIN3 (blob: sprd_fm.ko).
#
# Daftar objek PERSIS dari Makefile dalam-tree referensi
# (gpl-source/hadenix_ums512_wcn/drivers/wcn/fm/driver/Makefile):
#
#     obj-$(CONFIG_WCN_FM) += sprd_fm.o
#     sprd_fm-objs := fm_sdio/fm_rf_marlin3.o fm_sdio/fmdrv_main.o \
#                     fm_sdio/fmdrv_ops.o fm_sdio/fmdrv_rds_parser.o

ccflags-y += -DDEBUG

obj-$(CONFIG_WCN_FM) += sprd_fm.o
sprd_fm-objs := fm_sdio/fm_rf_marlin3.o fm_sdio/fmdrv_main.o fm_sdio/fmdrv_ops.o fm_sdio/fmdrv_rds_parser.o
