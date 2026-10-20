# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (sprd-vsp-pw-domain.ko). The object used to be listed one by one,
# which made kbuild emit a module per object; the composite form keeps
# it in a single .ko with the stock file name, so the loader can
# insmod the same file.
#
# vsp_regs.o DEFINES `regs[]` for this module. It is not exported:
# the built-in VSP video driver has its own copy (see
# drivers/media/platform/sprd/vsp/vsp_regs.c), because an exported
# vmlinux symbol that vmlinux itself uses cannot be linked:
#
#   relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which
#   may bind externally can not be used when making a shared object;
#   recompile with -fPIC

obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o vsp_regs.o

obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
