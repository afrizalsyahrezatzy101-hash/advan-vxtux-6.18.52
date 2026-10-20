# SPDX-License-Identifier: GPL-2.0
#
# ONE composite module, named after the vendor blob it replaces
# (audio-pipe.ko).
# Listing the objects one by one would make kbuild emit a module
# per object, and the stock file name would not be reproduced.

# SPRD Audio Pipe (apipe) — port 6.18
#
# Include path vendor lama (sound/soc/sprd/include) tidak ada di tree ini;
# simbol CONFIG yang benar untuk modul ini adalah SPRD_AUDIO_PIPE (lihat
# drivers/vxtux/audio/Kconfig dan Makefile induk).

obj-$(CONFIG_SPRD_AUDIO_PIPE) += audio-pipe.o
audio-pipe-y := audio_pipe.o
