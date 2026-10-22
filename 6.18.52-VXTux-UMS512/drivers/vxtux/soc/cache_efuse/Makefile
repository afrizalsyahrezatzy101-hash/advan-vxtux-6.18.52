# SPDX-License-Identifier: GPL-2.0
#
# The composite module is named after the vendor blob it replaces
# (nvmem_sprd_cache_efuse.ko). The stock module's own name is
# nvmem_sprd_cache_efuse, and it exports nothing - the symbols it
# uses (efuse_mem_ram_vmap, sprd_efuse_probe, sprd_efuse_read) are
# local to it.
#
# The composite form is used because listing the objects one by one
# would make kbuild emit a module per object, and the stock file name
# would not be reproduced.

obj-$(CONFIG_NVMEM_SPRD_CACHE_EFUSE) += nvmem_sprd_cache_efuse.o
nvmem_sprd_cache_efuse-y := sprd_cache_efuse.o
