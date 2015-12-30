# license:BSD-3-Clause
# copyright-holders:Robbbert
###########################################################################
#
#   makefile
#
#   Additional makefile for building HBMAME
#
###########################################################################

GEN_FOLDERS += $(GENDIR)/mame/layout/

# MAME layouts that we use
LAYOUTS += \
	$(SRC)/mame/layout/atari_s1.lay \
	$(SRC)/mame/layout/galactic.lay \

# MAME layouts that are needed for compile to succeed, not used by HBMAME
LAYOUTS += \
	$(SRC)/mame/layout/gorf.lay \
	$(SRC)/mame/layout/tenpindx.lay \
	$(SRC)/mame/layout/spacezap.lay \
	$(SRC)/mame/layout/seawolf2.lay \
	$(SRC)/mame/layout/gridiron.lay \
	$(SRC)/mame/layout/turbotag.lay \
	$(SRC)/mame/layout/spyhunt.lay \
	$(SRC)/mame/layout/dpoker.lay \
	$(SRC)/mame/layout/astdelux.lay \
	$(SRC)/mame/layout/outrun.lay \
	$(SRC)/mame/layout/polepos.lay \
	$(SRC)/mame/layout/topracer.lay \
	$(SRC)/mame/layout/armora.lay \
	$(SRC)/mame/layout/barrier.lay \
	$(SRC)/mame/layout/demon.lay \
	$(SRC)/mame/layout/solarq.lay \
	$(SRC)/mame/layout/starcas.lay \
	$(SRC)/mame/layout/sundance.lay \
	$(SRC)/mame/layout/tailg.lay \
	$(SRC)/mame/layout/wotw.lay \
	$(SRC)/mame/layout/ppsatan.lay \
	$(SRC)/mame/layout/konamigt.lay \
	$(SRC)/mame/layout/sfiii2.lay

