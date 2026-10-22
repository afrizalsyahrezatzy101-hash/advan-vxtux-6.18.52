# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (mmdvfs.ko).  Listing the objects one by one would make kbuild emit a
# module per object, and the stock file name would not be reproduced.
obj-$(CONFIG_SPRD_MM_DVFS) += mmdvfs.o

mmdvfs-y := sprd_mm_dvfs_core.o sprd_mm_dvfs_ip.o sprd_mm_dvfs_pqueue.o \
	    sprd_mm_dvfs_sysfs.o sprd_mm_dvfs_stock.o sprd_mm_dvfs_mmsys.o
