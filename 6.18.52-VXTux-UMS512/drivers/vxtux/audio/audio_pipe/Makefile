# The composite module is named after the vendor blob it replaces
# (sprd_apipe.ko). The stock module's own name is sprd_apipe, and it
# exports nothing - apipe_ioctl, apipe_open, apipe_poll, ... are all
# local to it.
#
# NOTE: a single-object module must NOT be written as a composite
# whose object has the same name as the module - kbuild then uses the
# module's own .o as the link input and ld refuses:
#
#   aarch64-linux-gnu-ld: input file '.../audio_pipe.o' is the same
#   as output file
#
# The source came from drivers/sound/soc/sprd/audio_pipe/, a copy of
# the Unisoc ASoC tree that was never built - the live tree is
# drivers/vxtux/sound/asoc/.

obj-$(CONFIG_SPRD_APIPE) += sprd_apipe.o
sprd_apipe-y := audio_pipe.o
