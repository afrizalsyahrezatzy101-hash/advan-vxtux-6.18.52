# SPDX-License-Identifier: GPL-2.0
#
# Novatek NT36xxx SPI touchscreen - VXTux port.
#
# ONE composite module, named after the vendor blob it replaces
# (novatek_ts_spi.ko). Same kbuild rule as drivers/gpu/drm/sprd/gsp and
# drivers/media/platform/sprd/vsp: listing the objects on a single
# obj-$(CONFIG_...) line would emit one module PER object, and every
# sibling would then reference nvt_* symbols living in another .ko
# (modpost: undefined symbol ...). Keep the composite-y form.
obj-$(CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI) += novatek_ts_spi.o
novatek_ts_spi-y := nt36xxx.o nt36xxx_fw_update.o nt36xxx_ext_proc.o nt36xxx_mp_ctrlram.o nt36xxx_pen.o
