# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it
# replaces (sprd-vsp-pw-domain.ko). The objects used to be listed one by
# one, which made kbuild emit a module per object; the
# composite form keeps them in a single .ko with the stock
# file name, so the loader can insmod the same file.

# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o vsp_regs.o
obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
