# license:GPL_2.0
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
	$(SRC)/mame/layout/galactic.lay \
	$(SRC)/mame/layout/segabill.lay \
	$(SRC)/mame/layout/warrior.lay \
	$(SRC)/mame/layout/bzone.lay \
	$(SRC)/mame/layout/contcirc.lay \
	$(SRC)/mame/layout/invaders.lay \
	$(SRC)/mame/layout/spcenctr.lay \
	$(SRC)/mame/layout/vr.lay

# MAME layouts that are needed for compile to succeed, not used by HBMAME
LAYOUTS += \
	$(SRC)/mame/layout/redbaron.lay \
	$(SRC)/mame/layout/gnw_dualh.lay \
	$(SRC)/mame/layout/gnw_dualv.lay \
	$(SRC)/mame/layout/inttoote.lay \
	$(SRC)/mame/layout/jockeyc.lay \
	$(SRC)/mame/layout/setaroul.lay \
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
	$(SRC)/mame/layout/sfiii2.lay \
	$(SRC)/mame/layout/model1io2.lay \
	$(SRC)/mame/layout/playch10.lay \
	$(SRC)/mame/layout/radr.lay \
	$(SRC)/mame/layout/finallap.lay \
	$(SRC)/mame/layout/segabillv.lay \
	$(SRC)/mame/layout/speedfrk.lay \
	$(SRC)/mame/layout/critcrsh.lay \
	$(SRC)/mame/layout/hh_sm500_test.lay \
	$(SRC)/mame/layout/chasehq.lay \
	$(SRC)/mame/layout/dblaxle.lay \
	$(SRC)/mame/layout/enforceja.lay \

