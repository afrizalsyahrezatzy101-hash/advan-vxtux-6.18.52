# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (sprd-vsp-pw-domain.ko). The object used to be listed one by one,
# which made kbuild emit a module per object; the composite form keeps
# it in a single .ko with the stock file name, so the loader can
# insmod the same file.
#
# vsp_regs.o is NOT part of that module: it DEFINES `regs[]`, and the
# built-in drivers/media/platform/sprd/vsp/sprd_vsp_main.c uses
# regs[]. A built-in object cannot reference a module's symbol:
#
#   sprd_vsp_main.c:63: undefined reference to `regs'
#
# So the definition is built into vmlinux (as vsp_regs.o) and the
# module gets it from there - vsp_regs.o already does
# EXPORT_SYMBOL(regs). The stock sprd-vsp-pw-domain.ko exports
# nothing, so no public API is lost by the split.

obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o

obj-y					+= vsp_regs.o

obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
