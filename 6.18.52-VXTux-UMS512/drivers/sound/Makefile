# SPDX-License-Identifier: GPL-2.0
#
# This tree has one sound subsystem: the Unisoc ASoC port under
# drivers/sound/soc/sprd/.
#
# The file was MISSING, and kbuild descends into a directory only when
# its parent lists it, so the whole ASoC stack - card, codec, dai_v4,
# platform, codec_sc2730, machine, mcdt, saudio, voice_trigger and the
# audio helpers - was never compiled at all. The 196-module build
# contained no object from drivers/sound/.
#
# Same silent-descent trap as drivers/soc/sprd/modem/ and
# drivers/vxtux/audio/audio_pipe/: the sources are there, the Kconfig
# is there, and nothing builds them.

obj-y					+= soc/
