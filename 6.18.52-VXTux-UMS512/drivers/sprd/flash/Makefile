# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (sprd_flash_drv.ko).
# Listing the objects one by one would make kbuild emit a module
# per object, and the stock file name would not be reproduced.

# SPRD camera flash HAL (blob: sprd_flash_drv)
# Port 2026-10-04 from work/donorcatalog/mods-sprd/sprd/common/camera/flash/flash_drv/
obj-$(CONFIG_SPRD_FLASH_DRV) += sprd_flash_drv.o
sprd_flash_drv-y := flash_drv.o
ccflags-y += -I$(src)
