-- license:BSD-3-Clause
-- copyright-holders:Robbbert
---------------------------------------------------------------------------
--
--   hbmame.lua
--
--   HBMAME target makefile
--
---------------------------------------------------------------------------

--------------------------------------------------
-- specify available CPU cores
---------------------------------------------------

CPUS["ADSP21XX"] = true
CPUS["ARM"] = true
CPUS["ARM7"] = true
CPUS["CCPU"] = true
CPUS["DSP16A"] = true
CPUS["H6280"] = true
CPUS["HD6309"] = true
CPUS["I8085"] = true
CPUS["I8089"] = true
CPUS["I86"] = true
CPUS["M6502"] = true
CPUS["M6800"] = true
CPUS["M6805"] = true
CPUS["M6809"] = true
CPUS["M680X0"] = true
CPUS["MB88XX"] = true
CPUS["MCS48"] = true
CPUS["MCS51"] = true
CPUS["NEC"] = true
CPUS["S2650"] = true
CPUS["SH2"] = true
CPUS["SSP1601"] = true
CPUS["TLCS90"] = true
CPUS["TMS32010"] = true
CPUS["TMS32031"] = true
CPUS["TMS340X0"] = true
CPUS["PIC16C5X"] = true
CPUS["V810"] = true
CPUS["Z180"] = true
CPUS["Z80"] = true

--------------------------------------------------
-- specify available sound cores
--------------------------------------------------

SOUNDS["SAMPLES"] = true
SOUNDS["DAC"] = true
SOUNDS["DMADAC"] = true
SOUNDS["BEEP"] = true
SOUNDS["DISCRETE"] = true
SOUNDS["AY8910"] = true
SOUNDS["YM2151"] = true
SOUNDS["YM2203"] = true
SOUNDS["YM2413"] = true
SOUNDS["YM2608"] = true
SOUNDS["YM2610"] = true
SOUNDS["YM2612"] = true
SOUNDS["YM3812"] = true
SOUNDS["YM3526"] = true
SOUNDS["YMF262"] = true
SOUNDS["YMF278B"] = true
SOUNDS["YMZ280B"] = true
SOUNDS["SN76477"] = true
SOUNDS["SN76496"] = true
SOUNDS["POKEY"] = true
SOUNDS["NES_APU"] = true
SOUNDS["ASTROCADE"] = true
SOUNDS["NAMCO_15XX"] = true
SOUNDS["NAMCO_52XX"] = true
SOUNDS["TMS36XX"] = true
SOUNDS["TMS5110"] = true
SOUNDS["TMS5220"] = true
SOUNDS["VLM5030"] = true
SOUNDS["MSM5205"] = true
SOUNDS["OKIM6295"] = true
SOUNDS["UPD7759"] = true
SOUNDS["HC55516"] = true
SOUNDS["K005289"] = true
SOUNDS["K007232"] = true
SOUNDS["K051649"] = true
SOUNDS["K053260"] = true
SOUNDS["K054539"] = true
SOUNDS["SEGAPCM"] = true
SOUNDS["RF5C68"] = true
SOUNDS["QSOUND"] = true
SOUNDS["ES5505"] = true
SOUNDS["SP0250"] = true
SOUNDS["CDDA"] = true
SOUNDS["ICS2115"] = true
SOUNDS["ST0016"] = true
SOUNDS["VOTRAX"] = true
SOUNDS["DIGITALKER"] = true

--------------------------------------------------
-- specify available video cores
--------------------------------------------------

VIDEOS["SEGA315_5124"] = true
VIDEOS["SEGA315_5313"] = true
VIDEOS["BUFSPRITE"] = true
VIDEOS["HD63484"] = true
VIDEOS["MC6845"] = true
VIDEOS["TLC34076"] = true
VIDEOS["TMS34061"] = true

--------------------------------------------------
-- specify available machine cores
--------------------------------------------------

MACHINES["6522VIA"] = true
MACHINES["RIOT6532"] = true
MACHINES["6821PIA"] = true
MACHINES["68681"] = true
MACHINES["I8257"] = true
MACHINES["AAKARTDEV"] = true
MACHINES["BANKDEV"] = true
MACHINES["EEPROMDEV"] = true
MACHINES["ER2055"] = true
MACHINES["I2CMEM"] = true
MACHINES["I8243"] = true
MACHINES["I8255"] = true
MACHINES["INTELFLASH"] = true
MACHINES["LATCH8"] = true
MACHINES["LDPR8210"] = true
MACHINES["MB14241"] = true
MACHINES["MB87078"] = true
MACHINES["MIOT6530"] = true
MACHINES["NETLIST"] = true
MACHINES["RP5H01"] = true
MACHINES["TIMEKPR"] = true
MACHINES["TMS6100"] = true
MACHINES["UPD1990A"] = true
MACHINES["UPD4992"] = true
MACHINES["V3021"] = true
MACHINES["WD33C93"] = true
MACHINES["WD_FDC"] = true
MACHINES["Z80CTC"] = true
MACHINES["Z80DART"] = true
MACHINES["Z80PIO"] = true

--------------------------------------------------
-- specify available bus cores
--------------------------------------------------

BUSES["GENERIC"] = true
BUSES["NEOGEO"] = true
BUSES["SCSI"] = true

--------------------------------------------------
-- this is the list of driver libraries that
-- comprise MAME plus hbmame.lst which contains
-- the list of drivers
--------------------------------------------------

function linkProjects_hbmame_hbmame(_target, _subtarget)
	links {
		"acorn",
		"atari",
		"atlus",
		"capcom",
		"cave",
		"cinemat",
		"dataeast",
		"exidy",
		"gottlieb",
		"igs",
		"irem",
		"itech",
		"jaleco",
		"konami",
		"midway",
		"namco",
		"neogeo",
		"nghb",
		"nichibut",
		"nintendo",
		"nmk",
		"pacman",
		"phoenix",
		"psikyo",
		"sega",
		"seta",
		"snk",
		"taito",
		"tehkan",
		"toaplan",
		"univers",
		"valadon",
		"shared",
		"misc",
	}
end

function createHBMAMEProjects(_target, _subtarget, _name)
	project (_name)
	targetsubdir(_target .."_" .. _subtarget)
	kind (LIBTYPE)
--	kind "StaticLib"
	uuid (os.uuid("drv-" .. _target .."_" .. _subtarget .. "_" .._name))

--	options {
--		"ForceCPP",
--	}

	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/hbmame/includes",
		MAME_DIR .. "src/hbmame",
		MAME_DIR .. "src/mame",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "src/lib/netlist",
		MAME_DIR .. "3rdparty",
		MAME_DIR .. "3rdparty/zlib",
		GEN_DIR  .. "mame/layout",
		GEN_DIR  .. "hbmame/layout",
	}
end

function createProjects_hbmame_hbmame(_target, _subtarget)
--------------------------------------------------
-- the following files are general components and
-- shared across a number of drivers
--------------------------------------------------

createHBMAMEProjects(_target, _subtarget, "shared")
files {
	MAME_DIR .. "src/mame/machine/ticket.cpp",
}

--------------------------------------------------
-- manufacturer-specific groupings for drivers
--------------------------------------------------

createHBMAMEProjects(_target, _subtarget, "acorn")
files {
	MAME_DIR .. "src/hbmame/drivers/ertictac.cpp",
	MAME_DIR .. "src/mame/machine/archimds.cpp",
	MAME_DIR .. "src/mame/video/archimds.cpp",
}

createHBMAMEProjects(_target, _subtarget, "atari")
files {
	MAME_DIR .. "src/hbmame/drivers/asteroid.cpp",
	MAME_DIR .. "src/mame/machine/asteroid.cpp",
	MAME_DIR .. "src/mame/audio/asteroid.cpp",
	MAME_DIR .. "src/hbmame/drivers/bwidow.cpp",
	MAME_DIR .. "src/mame/audio/bwidow.cpp",
	MAME_DIR .. "src/hbmame/drivers/cball.cpp",
	MAME_DIR .. "src/hbmame/drivers/centiped.cpp",
	MAME_DIR .. "src/mame/video/centiped.cpp",
	MAME_DIR .. "src/hbmame/drivers/gauntlet.cpp",
	MAME_DIR .. "src/mame/video/gauntlet.cpp",
	MAME_DIR .. "src/hbmame/drivers/tempest.cpp",
	MAME_DIR .. "src/mame/machine/atari_vg.cpp",
	MAME_DIR .. "src/mame/machine/atarigen.cpp",
	MAME_DIR .. "src/mame/machine/mathbox.cpp",
	MAME_DIR .. "src/mame/machine/slapstic.cpp",
	MAME_DIR .. "src/mame/audio/llander.cpp",
	MAME_DIR .. "src/mame/video/atarimo.cpp",
	MAME_DIR .. "src/mame/video/avgdvg.cpp",
}

createHBMAMEProjects(_target, _subtarget, "atlus")
files {
	MAME_DIR .. "src/hbmame/drivers/powerins.cpp",
	MAME_DIR .. "src/mame/video/powerins.cpp",
}

createHBMAMEProjects(_target, _subtarget, "capcom")
files {
	MAME_DIR .. "src/hbmame/drivers/1942.cpp",
	MAME_DIR .. "src/mame/video/1942.cpp",
	MAME_DIR .. "src/hbmame/drivers/blktiger.cpp",
	MAME_DIR .. "src/mame/video/blktiger.cpp",
	MAME_DIR .. "src/hbmame/drivers/commando.cpp",
	MAME_DIR .. "src/mame/video/commando.cpp",
	MAME_DIR .. "src/hbmame/drivers/cps1.cpp",
	MAME_DIR .. "src/hbmame/video/cps1.cpp",
	MAME_DIR .. "src/hbmame/drivers/cps2.cpp",
	MAME_DIR .. "src/mame/machine/cps2crpt.cpp",
	MAME_DIR .. "src/hbmame/drivers/cps3.cpp",
	MAME_DIR .. "src/mame/audio/cps3.cpp",
	MAME_DIR .. "src/hbmame/drivers/fcrash.cpp",
	MAME_DIR .. "src/hbmame/drivers/mitchell.cpp",
	MAME_DIR .. "src/mame/video/mitchell.cpp",
	MAME_DIR .. "src/mame/machine/kabuki.cpp",
}

createHBMAMEProjects(_target, _subtarget, "cave")
files {
	MAME_DIR .. "src/hbmame/drivers/cave.cpp",
	MAME_DIR .. "src/mame/video/cave.cpp",
}

createHBMAMEProjects(_target, _subtarget, "cinemat")
files {
	MAME_DIR .. "src/hbmame/drivers/cinemat.cpp",
	MAME_DIR .. "src/mame/audio/cinemat.cpp",
	MAME_DIR .. "src/mame/video/cinemat.cpp",
}

createHBMAMEProjects(_target, _subtarget, "dataeast")
files {
	MAME_DIR .. "src/hbmame/drivers/astrof.cpp",
	MAME_DIR .. "src/mame/audio/astrof.cpp",
	MAME_DIR .. "src/hbmame/drivers/btime.cpp",
	MAME_DIR .. "src/mame/machine/btime.cpp",
	MAME_DIR .. "src/mame/video/btime.cpp",
	MAME_DIR .. "src/hbmame/drivers/dec0.cpp",
	MAME_DIR .. "src/mame/machine/dec0.cpp",
	MAME_DIR .. "src/mame/video/dec0.cpp",
	MAME_DIR .. "src/mame/machine/deco222.cpp",
	MAME_DIR .. "src/mame/machine/decocpu7.cpp",
	MAME_DIR .. "src/hbmame/drivers/pcktgal.cpp",
	MAME_DIR .. "src/mame/video/pcktgal.cpp",
	MAME_DIR .. "src/mame/video/decbac06.cpp",
	MAME_DIR .. "src/mame/video/decmxc06.cpp",
}

createHBMAMEProjects(_target, _subtarget, "exidy")
files {
	MAME_DIR .. "src/hbmame/drivers/exidy.cpp",
	MAME_DIR .. "src/mame/audio/exidy.cpp",
	MAME_DIR .. "src/mame/video/exidy.cpp",
	MAME_DIR .. "src/mame/audio/targ.cpp",
}

createHBMAMEProjects(_target, _subtarget, "gottlieb")
files {
	MAME_DIR .. "src/hbmame/drivers/gottlieb.cpp",
	MAME_DIR .. "src/mame/audio/gottlieb.cpp",
	MAME_DIR .. "src/mame/video/gottlieb.cpp",
}

createHBMAMEProjects(_target, _subtarget, "igs")
files {
	MAME_DIR .. "src/hbmame/drivers/pgm.cpp",
	MAME_DIR .. "src/mame/video/pgm.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_igs027a_type1.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_igs027a_type2.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_igs027a_type3.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_igs025_igs012.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_igs025_igs022.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_igs025_igs028.cpp",
	MAME_DIR .. "src/mame/machine/pgmprot_orlegend.cpp",
	MAME_DIR .. "src/mame/machine/pgmcrypt.cpp",
	MAME_DIR .. "src/mame/machine/igs025.cpp",
	MAME_DIR .. "src/mame/machine/igs022.cpp",
	MAME_DIR .. "src/mame/machine/igs028.cpp",
}

createHBMAMEProjects(_target, _subtarget, "irem")
files {
	MAME_DIR .. "src/hbmame/drivers/m52.cpp",
	MAME_DIR .. "src/mame/video/m52.cpp",
	MAME_DIR .. "src/mame/machine/irem_cpu.cpp",
	MAME_DIR .. "src/mame/audio/irem.cpp",
}

createHBMAMEProjects(_target, _subtarget, "itech")
files {
	MAME_DIR .. "src/hbmame/drivers/itech8.cpp",
	MAME_DIR .. "src/mame/machine/slikshot.cpp",
	MAME_DIR .. "src/mame/video/itech8.cpp",
}

createHBMAMEProjects(_target, _subtarget, "jaleco")
files {
	MAME_DIR .. "src/hbmame/drivers/exerion.cpp",
	MAME_DIR .. "src/mame/video/exerion.cpp",
}

createHBMAMEProjects(_target, _subtarget, "konami")
files {
	MAME_DIR .. "src/hbmame/drivers/1943.cpp",
	MAME_DIR .. "src/mame/video/1943.cpp",
	MAME_DIR .. "src/mame/audio/hyprolyb.cpp",
	MAME_DIR .. "src/hbmame/drivers/nemesis.cpp",
	MAME_DIR .. "src/mame/video/nemesis.cpp",
	MAME_DIR .. "src/hbmame/drivers/shaolins.cpp",
	MAME_DIR .. "src/mame/video/shaolins.cpp",
	MAME_DIR .. "src/hbmame/drivers/tmnt.cpp",
	MAME_DIR .. "src/mame/video/tmnt.cpp",
	MAME_DIR .. "src/hbmame/drivers/trackfld.cpp",
	MAME_DIR .. "src/mame/machine/konami1.cpp",
	MAME_DIR .. "src/mame/audio/trackfld.cpp",
	MAME_DIR .. "src/mame/video/trackfld.cpp",
	MAME_DIR .. "src/mame/video/tutankhm.cpp",
	MAME_DIR .. "src/hbmame/drivers/yiear.cpp",
	MAME_DIR .. "src/mame/video/yiear.cpp",
	MAME_DIR .. "src/mame/video/konami_helper.cpp",
	MAME_DIR .. "src/mame/video/k051960.cpp",
	MAME_DIR .. "src/mame/video/k052109.cpp",
	MAME_DIR .. "src/mame/video/k053251.cpp",
	MAME_DIR .. "src/mame/video/k053244_k053245.cpp",
	MAME_DIR .. "src/mame/video/k054000.cpp",
	MAME_DIR .. "src/mame/video/k053936.cpp",
}

createHBMAMEProjects(_target, _subtarget, "midway")
files {
	MAME_DIR .. "src/hbmame/drivers/astrocde.cpp",
	MAME_DIR .. "src/mame/video/astrocde.cpp",
	MAME_DIR .. "src/mame/audio/gorf.cpp",
	MAME_DIR .. "src/mame/audio/wow.cpp",
	MAME_DIR .. "src/hbmame/drivers/mcr.cpp",
	MAME_DIR .. "src/mame/machine/mcr.cpp",
	MAME_DIR .. "src/mame/video/mcr.cpp",
	MAME_DIR .. "src/hbmame/drivers/mcr3.cpp",
	MAME_DIR .. "src/mame/video/mcr3.cpp",
	MAME_DIR .. "src/mame/drivers/mcr68.cpp",
	MAME_DIR .. "src/mame/machine/mcr68.cpp",
	MAME_DIR .. "src/mame/video/mcr68.cpp",
	MAME_DIR .. "src/hbmame/drivers/midtunit.cpp",
	MAME_DIR .. "src/mame/machine/midtunit.cpp",
	MAME_DIR .. "src/mame/video/midtunit.cpp",
	MAME_DIR .. "src/hbmame/drivers/midwunit.cpp",
	MAME_DIR .. "src/mame/machine/midwunit.cpp",
	MAME_DIR .. "src/hbmame/drivers/midyunit.cpp",
	MAME_DIR .. "src/mame/machine/midyunit.cpp",
	MAME_DIR .. "src/mame/video/midyunit.cpp",
	MAME_DIR .. "src/hbmame/drivers/williams.cpp",
	MAME_DIR .. "src/mame/machine/williams.cpp",
	MAME_DIR .. "src/mame/audio/williams.cpp",
	MAME_DIR .. "src/mame/video/williams.cpp",
	MAME_DIR .. "src/mame/machine/midwayic.cpp",
	MAME_DIR .. "src/mame/audio/midway.cpp",
	MAME_DIR .. "src/mame/audio/cage.cpp",
	MAME_DIR .. "src/mame/audio/dcs.cpp",
}

createHBMAMEProjects(_target, _subtarget, "namco")
files {
	MAME_DIR .. "src/hbmame/drivers/galaga.cpp",
	MAME_DIR .. "src/mame/audio/galaga.cpp",
	MAME_DIR .. "src/mame/video/galaga.cpp",
	MAME_DIR .. "src/mame/video/bosco.cpp",
	MAME_DIR .. "src/mame/video/digdug.cpp",
	MAME_DIR .. "src/mame/machine/xevious.cpp",
	MAME_DIR .. "src/mame/video/xevious.cpp",
	MAME_DIR .. "src/hbmame/drivers/galaxian.cpp",
	MAME_DIR .. "src/mame/audio/galaxian.cpp",
	MAME_DIR .. "src/hbmame/video/galaxian.cpp",
	MAME_DIR .. "src/hbmame/drivers/mappy.cpp",
	MAME_DIR .. "src/mame/video/mappy.cpp",
	MAME_DIR .. "src/hbmame/drivers/pacland.cpp",
	MAME_DIR .. "src/mame/video/pacland.cpp",
	MAME_DIR .. "src/mame/drivers/polepos.cpp",
	MAME_DIR .. "src/mame/audio/polepos.cpp",
	MAME_DIR .. "src/mame/video/polepos.cpp",
	MAME_DIR .. "src/hbmame/drivers/rallyx.cpp",
	MAME_DIR .. "src/mame/video/rallyx.cpp",
	MAME_DIR .. "src/mame/machine/namcoio.cpp",
	MAME_DIR .. "src/mame/machine/namco06.cpp",
	MAME_DIR .. "src/mame/machine/namco50.cpp",
	MAME_DIR .. "src/mame/machine/namco51.cpp",
	MAME_DIR .. "src/mame/machine/namco53.cpp",
	MAME_DIR .. "src/mame/audio/namco52.cpp",
	MAME_DIR .. "src/mame/audio/namco54.cpp",
	MAME_DIR .. "src/mame/audio/timeplt.cpp",
}

createHBMAMEProjects(_target, _subtarget, "neogeo")
files {
	MAME_DIR .. "src/mame/drivers/neogeo.cpp",
	MAME_DIR .. "src/mame/video/neogeo.cpp",
	MAME_DIR .. "src/mame/drivers/neogeo_noslot.cpp",
	MAME_DIR .. "src/mame/video/neogeo_spr.cpp",
	MAME_DIR .. "src/mame/machine/neocrypt.cpp",
	MAME_DIR .. "src/mame/machine/ng_memcard.cpp",
}

createHBMAMEProjects(_target, _subtarget, "nghb")
files {
	MAME_DIR .. "src/hbmame/drivers/nghb.cpp",
	MAME_DIR .. "src/hbmame/video/nghb.cpp",
	MAME_DIR .. "src/hbmame/video/nghb_spr.cpp",
	MAME_DIR .. "src/hbmame/machine/nghb_boot.cpp",
	MAME_DIR .. "src/hbmame/machine/nghb_crypt.cpp",
	MAME_DIR .. "src/hbmame/machine/nghb_prot.cpp",
	MAME_DIR .. "src/hbmame/machine/nghb_memc.cpp",
	MAME_DIR .. "src/emu/imagedev/cartslot.cpp",
}

createHBMAMEProjects(_target, _subtarget, "nichibut")
files {
	MAME_DIR .. "src/hbmame/drivers/cclimber.cpp",
	MAME_DIR .. "src/mame/machine/cclimber.cpp",
	MAME_DIR .. "src/mame/audio/cclimber.cpp",
	MAME_DIR .. "src/mame/video/cclimber.cpp",
	MAME_DIR .. "src/hbmame/drivers/terracre.cpp",
	MAME_DIR .. "src/mame/video/terracre.cpp",
}

createHBMAMEProjects(_target, _subtarget, "nintendo")
files {
	MAME_DIR .. "src/hbmame/drivers/dkong.cpp",
	MAME_DIR .. "src/mame/audio/dkong.cpp",
	MAME_DIR .. "src/mame/video/dkong.cpp",
	MAME_DIR .. "src/hbmame/drivers/mario.cpp",
	MAME_DIR .. "src/mame/audio/mario.cpp",
	MAME_DIR .. "src/mame/video/mario.cpp",
	MAME_DIR .. "src/hbmame/drivers/mmagic.cpp",
	MAME_DIR .. "src/hbmame/drivers/playch10.cpp",
	MAME_DIR .. "src/mame/machine/playch10.cpp",
	MAME_DIR .. "src/mame/video/playch10.cpp",
	MAME_DIR .. "src/hbmame/drivers/vsnes.cpp",
	MAME_DIR .. "src/mame/machine/vsnes.cpp",
	MAME_DIR .. "src/mame/video/vsnes.cpp",
	MAME_DIR .. "src/mame/video/ppu2c0x.cpp",
	MAME_DIR .. "src/devices/machine/z80dma.cpp",

}

createHBMAMEProjects(_target, _subtarget, "nmk")
files {
	MAME_DIR .. "src/hbmame/drivers/nmk16.cpp",
	MAME_DIR .. "src/mame/video/nmk16.cpp",
	MAME_DIR .. "src/mame/machine/nmk004.cpp",
	MAME_DIR .. "src/mame/audio/seibu.cpp",
}

createHBMAMEProjects(_target, _subtarget, "pacman")
files {
	MAME_DIR .. "src/hbmame/drivers/jrpacman.cpp",
	MAME_DIR .. "src/hbmame/drivers/pacman.cpp",
	MAME_DIR .. "src/hbmame/video/pacman.cpp",
	MAME_DIR .. "src/hbmame/drivers/pengo.cpp",
}

createHBMAMEProjects(_target, _subtarget, "phoenix")
files {
	MAME_DIR .. "src/hbmame/drivers/phoenix.cpp",
	MAME_DIR .. "src/mame/audio/phoenix.cpp",
	MAME_DIR .. "src/mame/video/phoenix.cpp",
	MAME_DIR .. "src/mame/audio/pleiads.cpp",
}

createHBMAMEProjects(_target, _subtarget, "psikyo")
files {
	MAME_DIR .. "src/hbmame/drivers/psikyosh.cpp",
	MAME_DIR .. "src/mame/video/psikyosh.cpp",
}

createHBMAMEProjects(_target, _subtarget, "sega")
files {
	MAME_DIR .. "src/mame/machine/megadriv.cpp",
	MAME_DIR .. "src/mame/drivers/megaplay.cpp",
	MAME_DIR .. "src/mame/drivers/megatech.cpp",
	MAME_DIR .. "src/hbmame/drivers/segac2.cpp",
	MAME_DIR .. "src/mame/drivers/segae.cpp",
	MAME_DIR .. "src/hbmame/drivers/segag80r.cpp",
	MAME_DIR .. "src/mame/machine/segag80.cpp",
	MAME_DIR .. "src/mame/audio/segag80r.cpp",
	MAME_DIR .. "src/mame/video/segag80r.cpp",
	MAME_DIR .. "src/hbmame/drivers/segaorun.cpp",
	MAME_DIR .. "src/mame/video/segaorun.cpp",
	MAME_DIR .. "src/hbmame/drivers/system1.cpp",
	MAME_DIR .. "src/mame/video/system1.cpp",
	MAME_DIR .. "src/hbmame/drivers/system16.cpp",
	MAME_DIR .. "src/mame/video/system16.cpp",
	MAME_DIR .. "src/mame/machine/315_5296.cpp",
	MAME_DIR .. "src/mame/machine/fd1089.cpp",
	MAME_DIR .. "src/mame/machine/fd1094.cpp",
	MAME_DIR .. "src/mame/machine/mc8123.cpp",
	MAME_DIR .. "src/mame/machine/segaic16.cpp",
	MAME_DIR .. "src/mame/machine/segacrpt.cpp",
	MAME_DIR .. "src/mame/machine/segacrp2.cpp",
	MAME_DIR .. "src/mame/audio/segasnd.cpp",
	MAME_DIR .. "src/mame/video/segaic16.cpp",
	MAME_DIR .. "src/mame/video/segaic16_road.cpp",
	MAME_DIR .. "src/mame/video/sega16sp.cpp",
	MAME_DIR .. "src/hbmame/drivers/segas16b.cpp",
	MAME_DIR .. "src/mame/video/segas16b.cpp",
}

createHBMAMEProjects(_target, _subtarget, "seta")
files {
	MAME_DIR .. "src/mame/machine/st0016.cpp",
	MAME_DIR .. "src/hbmame/drivers/simple_st0016.cpp",
}

createHBMAMEProjects(_target, _subtarget, "snk")
files {
	MAME_DIR .. "src/hbmame/drivers/snk6502.cpp",
	MAME_DIR .. "src/mame/audio/snk6502.cpp",
	MAME_DIR .. "src/mame/video/snk6502.cpp",
}

createHBMAMEProjects(_target, _subtarget, "taito")
files {
	MAME_DIR .. "src/hbmame/drivers/arkanoid.cpp",
	MAME_DIR .. "src/mame/machine/arkanoid.cpp",
	MAME_DIR .. "src/mame/video/arkanoid.cpp",
	MAME_DIR .. "src/hbmame/drivers/asuka.cpp",
	MAME_DIR .. "src/mame/machine/bonzeadv.cpp",
	MAME_DIR .. "src/mame/video/asuka.cpp",
	MAME_DIR .. "src/hbmame/drivers/bublbobl.cpp",
	MAME_DIR .. "src/mame/machine/bublbobl.cpp",
	MAME_DIR .. "src/mame/video/bublbobl.cpp",
	MAME_DIR .. "src/hbmame/drivers/rastan.cpp",
	MAME_DIR .. "src/mame/video/rastan.cpp",
	MAME_DIR .. "src/hbmame/drivers/retofinv.cpp",
	MAME_DIR .. "src/mame/machine/retofinv.cpp",
	MAME_DIR .. "src/mame/video/retofinv.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_b.cpp",
	MAME_DIR .. "src/mame/video/taito_b.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_f2.cpp",
	MAME_DIR .. "src/mame/video/taito_f2.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_f3.cpp",
	MAME_DIR .. "src/mame/video/taito_f3.cpp",
	MAME_DIR .. "src/mame/machine/cchip.cpp",
	MAME_DIR .. "src/hbmame/drivers/volfied.cpp",
	MAME_DIR .. "src/mame/machine/volfied.cpp",
	MAME_DIR .. "src/mame/video/volfied.cpp",
	MAME_DIR .. "src/mame/audio/taitosnd.cpp",
	MAME_DIR .. "src/mame/audio/taito_en.cpp",
	MAME_DIR .. "src/mame/machine/taitoio.cpp",
	MAME_DIR .. "src/mame/video/taito_helper.cpp",
	MAME_DIR .. "src/mame/video/pc080sn.cpp",
	MAME_DIR .. "src/mame/video/pc090oj.cpp",
	MAME_DIR .. "src/mame/video/tc0100scn.cpp",
	MAME_DIR .. "src/mame/video/tc0280grd.cpp",
	MAME_DIR .. "src/mame/video/tc0360pri.cpp",
	MAME_DIR .. "src/mame/video/tc0480scp.cpp",
	MAME_DIR .. "src/mame/video/tc0110pcr.cpp",
	MAME_DIR .. "src/mame/video/tc0180vcu.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_l.cpp",
	MAME_DIR .. "src/mame/video/taito_l.cpp",
}

createHBMAMEProjects(_target, _subtarget, "tehkan")
files {
	MAME_DIR .. "src/mame/video/tecmo_spr.cpp",
	MAME_DIR .. "src/hbmame/drivers/bombjack.cpp",
	MAME_DIR .. "src/mame/video/bombjack.cpp",
	MAME_DIR .. "src/hbmame/drivers/tecmo.cpp",
	MAME_DIR .. "src/mame/video/tecmo.cpp",
	MAME_DIR .. "src/hbmame/drivers/tehkanwc.cpp",
	MAME_DIR .. "src/mame/video/tehkanwc.cpp",
}

createHBMAMEProjects(_target, _subtarget, "toaplan")
files {
	MAME_DIR .. "src/hbmame/drivers/toaplan1.cpp",
	MAME_DIR .. "src/mame/machine/toaplan1.cpp",
	MAME_DIR .. "src/mame/video/toaplan1.cpp",
	MAME_DIR .. "src/hbmame/drivers/toaplan2.cpp",
	MAME_DIR .. "src/mame/video/toaplan2.cpp",
	MAME_DIR .. "src/mame/video/gp9001.cpp",
	MAME_DIR .. "src/hbmame/drivers/twincobr.cpp",
	MAME_DIR .. "src/mame/machine/twincobr.cpp",
	MAME_DIR .. "src/mame/video/twincobr.cpp",
	MAME_DIR .. "src/mame/drivers/wardner.cpp",
	MAME_DIR .. "src/mame/video/toaplan_scu.cpp",
	MAME_DIR .. "src/mame/machine/nmk112.cpp",
}

createHBMAMEProjects(_target, _subtarget, "univers")
files {
	MAME_DIR .. "src/hbmame/drivers/docastle.cpp",
	MAME_DIR .. "src/mame/machine/docastle.cpp",
	MAME_DIR .. "src/mame/video/docastle.cpp",
	MAME_DIR .. "src/hbmame/drivers/mrdo.cpp",
	MAME_DIR .. "src/mame/video/mrdo.cpp",
}

createHBMAMEProjects(_target, _subtarget, "valadon")
files {
	MAME_DIR .. "src/hbmame/drivers/bagman.cpp",
	MAME_DIR .. "src/mame/machine/bagman.cpp",
	MAME_DIR .. "src/mame/video/bagman.cpp",
}

createHBMAMEProjects(_target, _subtarget, "misc")
files {
	MAME_DIR .. "src/hbmame/drivers/mineswp.cpp",
	MAME_DIR .. "src/hbmame/drivers/ir.cpp",
	MAME_DIR .. "src/hbmame/drivers/schaser.cpp",
	MAME_DIR .. "src/hbmame/drivers/spacmiss.cpp",
	MAME_DIR .. "src/hbmame/drivers/monaco.cpp",
	MAME_DIR .. "src/hbmame/drivers/atari_s1.cpp",
	MAME_DIR .. "src/mame/machine/genpin.cpp",
	MAME_DIR .. "src/hbmame/drivers/kyugo.cpp",
	MAME_DIR .. "src/mame/video/kyugo.cpp",
}

end

