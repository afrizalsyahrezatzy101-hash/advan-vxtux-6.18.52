# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it
# replaces (nvmem_sprd_cache_efuse.ko). The objects used to be listed one by
# one, which made kbuild emit a module per object; the
# composite form keeps them in a single .ko with the stock
# file name, so the loader can insmod the same file.

obj-$(CONFIG_NVMEM_SPRD_CACHE_EFUSE) += nvmem_sprd_cache_efuse.o
nvmem_sprd_cache_efuse-y := sprd_cache_efuse.o
# SPDX-License-Identifier: GPL-2.0
# drivers/vxtux/soc/cache_efuse/Makefile
