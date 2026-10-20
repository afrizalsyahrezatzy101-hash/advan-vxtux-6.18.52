# SPRD audio pipe (blob: sprd_apipe)
# Port 2026-10-04 from gpl-source/fork_t618/.../drivers/misc/sprd_audio/audio_pipe/
#
# The composite module is named after the vendor blob it replaces
# (sprd_apipe.ko). The stock module's own name is sprd_apipe and it
# exports nothing - apipe_ioctl, apipe_open, apipe_poll, ... are all
# local to it.
#
# It is gated on CONFIG_SPRD_APIPE, a symbol of its own. It used to
# borrow CONFIG_SPRD_AUDIO_MEMORY_MANAGER, which is a different
# driver (the audio memory manager) and is built into vmlinux, so
# this could never be a loadable module - and the stock ROM ships
# sprd_apipe.ko as one.
obj-$(CONFIG_SPRD_APIPE) += sprd_apipe.o
sprd_apipe-y := audio_pipe.o
ccflags-y += -I$(srctree)/drivers/sound/soc/sprd/include
