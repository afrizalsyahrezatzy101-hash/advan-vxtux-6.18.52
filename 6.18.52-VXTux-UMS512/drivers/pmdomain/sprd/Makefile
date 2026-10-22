# SPDX-License-Identifier: GPL-2.0
#
# The Unisoc VSP power domain.
#
# On the stock device this is BUILT-IN, and so is the VSP video
# driver that uses it:
#
#   $ nm donors/stock_vendor_modules/sprd-vsp-pw-domain.ko
#   0000000000000434 t vsp_pw_off
#   00000000000004bc t vsp_pw_on
#   $ strings ... | grep name=
#   name=sprd_vsp_pw_domain
#
# vsp_pw_on / vsp_pw_off are LOCAL there, so nothing outside uses
# them, and the stock ROM has no vsp.ko either - the video driver is
# in vmlinux. That is why drivers/media/platform/sprd/vsp/Kconfig
# has `select SPRD_VSP_PW_DOMAIN': a built-in CONFIG_SPRD_VSP=y
# forces this symbol to y as well.
#
# vsp_regs.o DEFINES `regs[]`, which both this driver and the VSP
# video driver index. Since both are built into vmlinux, there must
# be exactly ONE definition - two would be a link error:
#
#   aarch64-linux-gnu-ld: multiple definition of `regs'
#
# It is not exported either. An exported vmlinux symbol that vmlinux
# itself uses cannot be linked - vmlinux is not position-independent:
#
#   relocation R_AARCH64_ADR_PREL_PG_HI21 against symbol `regs' which
#   may bind externally can not be used when making a shared object;
#   recompile with -fPIC
#
# The stock blob agrees: it neither defines nor imports `regs`, and
# no stock .ko imports it.

obj-$(CONFIG_SPRD_VSP_PW_DOMAIN) += sprd-vsp-pw-domain.o
sprd-vsp-pw-domain-y := sprd_vsp_pw_domain.o

# The one and only definition of the register table.
obj-y					+= vsp_regs.o

obj-$(CONFIG_SPRD_CAMSYS_PW_DOMAIN) += sprd_camsys_pw_domain.o
obj-$(CONFIG_SPRD_GPU_PW_DOMAIN) += sprd_gpu_pw_domain.o
