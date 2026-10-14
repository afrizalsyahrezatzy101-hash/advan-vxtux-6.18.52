# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_SPRD_MODEM_DEBUG) += sprd_mipi.o
sprd_mipi-objs := core.o \
		  sysfs.o \
		  serdes.o

# Per-SoC variant (donor: bocchi_unisoc54 .../debug_log/sharkl5/, i.e. the
# source of vendor blob mipi_driver.ko). Donor's exact descent line, kept
# under the donor's own symbol so the common core stays independent.
obj-$(CONFIG_SPRD_MIPI_LOG_R5P0) += sharkl5/
