# SPDX-License-Identifier: GPL-2.0
#
# Scoped to the gsp_lite_r2p0 core plus the gsp_interface_pike2 interface,
# because the T618 is a "pike2" part (DT compatible "sprd,gsp-lite_r2p0-pike2").
# The r6p0 / r7p0 / r8p0 / r9p0 / lite_r3p0 cores and the sharkl3 / sharkl5 /
# sharkl5pro / sharkle / roc1 / qogirl6 / qogirn6pro interfaces are still in the
# tree but are not built: they belong to other SoCs, and r8p0/r9p0 additionally
# need the Unisoc Trusty SMC headers, which do not exist in this tree.
# gsp_ipc_trusty.c is likewise left unbuilt for the same Trusty reason.
#
# ONE composite module, named after the vendor blob it replaces (sprd-gsp.ko).
#
# Why: this file used to list nine objects on a single
# obj-$(CONFIG_DRM_SPRD_GSP) += line. kbuild turns "obj-m += a.o b.o ..." into
# one module PER object, so gsp_dev.ko, gsp_kcfg.ko, gsp_layer.ko, gsp_sync.ko,
# gsp_sysfs.ko, gsp_workqueue.ko and gsp_interface.ko each ended up referencing
# gsp_core_* symbols that live in a sibling module -- and gsp_core.c exports
# none of them (no EXPORT_SYMBOL anywhere in this directory). modpost therefore
# hard-fails with undefined symbols and asks for MODULE_LICENSE per object. The
# donor tree never hit this because everything there is obj-y (=y built-in,
# 5.4 fork_t618 drivers/gpu/drm/sprd/gsp/Makefile): with =y there is no module
# boundary to cross. Same bug, same fix as drivers/media/platform/sprd/vsp/.
#
# Objects that live in a subdirectory are listed here by relative path: a
# subdirectory Makefile runs in a sub-make and cannot append to a composite
# variable of its parent, so gsp_interface/Makefile and gsp_lite_r2p0/Makefile
# are deliberately no longer listed in obj- and are not descended into
# (Documentation/kbuild/makefiles.rst; in-tree precedent
# drivers/hid/amd-sfh-hid/Makefile).
#
# MODULE_LICENSE("GPL") is declared once, in gsp_dev.c (the module entry).
# Do not add one per object: all of these objects link into the same module.

# ccflags-y, NOT subdir-ccflags-y: subdir-ccflags-y only propagates to
# subdirectories, while every object below -- including the ones living in
# gsp_lite_r2p0/ and gsp_interface/ -- is compiled from this Makefile now.
# gsp_dev.c includes sprd_drm.h and sprd_drm_gsp.h, which live one level up in
# the parent sprd/ directory, alongside the other Unisoc DRM headers.
ccflags-y += -I$(src) -I$(src)/gsp_lite_r2p0 -I$(src)/gsp_interface -I$(src)/..

# gsp_ion_stub.c supplies the two Unisoc ION helpers gsp_layer.c calls
# (sprd_ion_is_reserved, sprd_ion_get_phys_addr). No real ION provider
# exists in this tree, so they are documented stubs that fail at runtime;
# without them modpost hard-errors on the undefined symbols.
sprd-gsp-objs := gsp_dev.o gsp_core.o gsp_interface.o gsp_kcfg.o gsp_layer.o \
	gsp_ion_stub.o gsp_sync.o gsp_sysfs.o gsp_workqueue.o \
	gsp_interface/gsp_interface_pike2.o \
	gsp_lite_r2p0/gsp_lite_r2p0_core.o gsp_lite_r2p0/gsp_lite_r2p0_coef_cal.o

obj-$(CONFIG_DRM_SPRD_GSP) += sprd-gsp.o
