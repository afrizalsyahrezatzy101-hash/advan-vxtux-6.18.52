# This tree only needs the generic PM domain core plus the Unisoc VSP power
# domain. The per-vendor subdirectories below are listed upstream because those
# platforms ship their own genpd drivers; this tree carries none of them and
# building them only adds unrelated failures. Removing them keeps core.o
# (genpd_dev_pm_attach and friends, which drivers/opp/core.c references) and
# adds nothing else.

# SPDX-License-Identifier: GPL-2.0-only
obj-y					+= core.o governor.o

# sprd/ carries the VSP power-domain module (sprd-vsp-pw-domain.ko,
# which replaces the stock blob) and its register table. It has to be
# descended into, otherwise the module is never built.
obj-y					+= sprd/
