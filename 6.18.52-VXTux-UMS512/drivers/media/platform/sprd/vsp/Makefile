# The VSP video driver needs its own copy of the register table:
# vsp_common.h declares `extern struct register_gpr regs[]`, and the
# table must not be exported (an exported vmlinux symbol that vmlinux
# itself uses makes the link fail with "relocation ... may bind
# externally ... recompile with -fPIC").
#
# The stock blob agrees: sprd-vsp-pw-domain.ko neither defines nor
# imports `regs`.

sprd_vsp-objs := sprd_vsp_main.o vsp_common.o vsp_isr_func.o vsp_regs.o
obj-$(CONFIG_SPRD_VSP) += sprd_vsp.o
ccflags-y += -I$(srctree)/drivers/devfreq/apsys/ -I$(srctree)/drivers/devfreq/sprd/apsys/
