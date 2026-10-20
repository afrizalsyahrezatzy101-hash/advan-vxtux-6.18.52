# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (apsys-dvfs.ko).
# Listing the objects one by one would make kbuild emit a module
# per object, and the stock file name would not be reproduced.

# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_SPRD_APSYS_DVFS) += apsys-dvfs.o
apsys-dvfs-y := sprd_dvfs_apsys.o
