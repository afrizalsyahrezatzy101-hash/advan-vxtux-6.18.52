# The composite module is named after the vendor blob it replaces
# (sprd_apipe.ko). The stock module's own name is sprd_apipe.
#
# NOTE: a single-object module must NOT be written as a composite
# whose object has the same name as the module - kbuild then uses the
# module's own .o as the link input and ld refuses:
#
#   aarch64-linux-gnu-ld: input file '.../audio_pipe.o' is the same
#   as output file

obj-$(CONFIG_SPRD_AUDIO_PIPE) += sprd_apipe.o
