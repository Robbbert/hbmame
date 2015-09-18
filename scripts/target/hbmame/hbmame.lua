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

CPUS["Z80"] = true
CPUS["I8085"] = true
CPUS["I8089"] = true
CPUS["M6502"] = true
CPUS["H6280"] = true
CPUS["I86"] = true
CPUS["NEC"] = true
CPUS["MCS48"] = true
CPUS["MCS51"] = true
CPUS["M6800"] = true
CPUS["M6805"] = true
CPUS["HD6309"] = true
CPUS["M6809"] = true
CPUS["M680X0"] = true
CPUS["S2650"] = true
CPUS["TMS340X0"] = true
CPUS["TMS32010"] = true
CPUS["TMS32031"] = true
CPUS["CCPU"] = true
CPUS["ADSP21XX"] = true
CPUS["ARM"] = true
CPUS["ARM7"] = true
CPUS["SH2"] = true
CPUS["DSP16A"] = true
CPUS["PIC16C5X"] = true
CPUS["V810"] = true
CPUS["MB88XX"] = true
CPUS["SSP1601"] = true

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
	kind "StaticLib"
	uuid (os.uuid("drv-" .. _target .."_" .. _subtarget .. "_" .._name))
	
	options {
		"ForceCPP",
	}
	
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
	MAME_DIR .. "src/mame/machine/ticket.c",
}

--------------------------------------------------
-- manufacturer-specific groupings for drivers
--------------------------------------------------

createHBMAMEProjects(_target, _subtarget, "acorn")
files {
	MAME_DIR .. "src/hbmame/drivers/ertictac.c",
	MAME_DIR .. "src/mame/machine/archimds.c",
	MAME_DIR .. "src/mame/video/archimds.c",
}

createHBMAMEProjects(_target, _subtarget, "atari")
files {
	MAME_DIR .. "src/hbmame/drivers/asteroid.c",
	MAME_DIR .. "src/mame/machine/asteroid.c",
	MAME_DIR .. "src/mame/audio/asteroid.c",
	MAME_DIR .. "src/hbmame/drivers/bwidow.c",
	MAME_DIR .. "src/mame/audio/bwidow.c",
	MAME_DIR .. "src/hbmame/drivers/cball.c",
	MAME_DIR .. "src/hbmame/drivers/centiped.c",
	MAME_DIR .. "src/mame/video/centiped.c",
	MAME_DIR .. "src/hbmame/drivers/gauntlet.c",
	MAME_DIR .. "src/mame/video/gauntlet.c",
	MAME_DIR .. "src/hbmame/drivers/tempest.c",
	MAME_DIR .. "src/mame/machine/atari_vg.c",
	MAME_DIR .. "src/mame/machine/atarigen.c",
	MAME_DIR .. "src/mame/machine/mathbox.c",
	MAME_DIR .. "src/mame/machine/slapstic.c",
	MAME_DIR .. "src/mame/audio/llander.c",
	MAME_DIR .. "src/mame/video/atarimo.c",
	MAME_DIR .. "src/mame/video/avgdvg.c",
}

createHBMAMEProjects(_target, _subtarget, "atlus")
files {
	MAME_DIR .. "src/hbmame/drivers/powerins.c",
	MAME_DIR .. "src/mame/video/powerins.c",
}

createHBMAMEProjects(_target, _subtarget, "capcom")
files {
	MAME_DIR .. "src/hbmame/drivers/1942.c",
	MAME_DIR .. "src/mame/video/1942.c",
	MAME_DIR .. "src/hbmame/drivers/blktiger.c",
	MAME_DIR .. "src/mame/video/blktiger.c",
	MAME_DIR .. "src/hbmame/drivers/commando.c",
	MAME_DIR .. "src/mame/video/commando.c",
	MAME_DIR .. "src/hbmame/drivers/cps1.c",
	MAME_DIR .. "src/hbmame/video/cps1.c",
	MAME_DIR .. "src/hbmame/drivers/cps2.c",
	MAME_DIR .. "src/mame/machine/cps2crpt.c",
	MAME_DIR .. "src/hbmame/drivers/cps3.c",
	MAME_DIR .. "src/mame/audio/cps3.c",
	MAME_DIR .. "src/hbmame/drivers/fcrash.c",
	MAME_DIR .. "src/hbmame/drivers/mitchell.c",
	MAME_DIR .. "src/mame/video/mitchell.c",
	MAME_DIR .. "src/mame/machine/kabuki.c",
}

createHBMAMEProjects(_target, _subtarget, "cave")
files {
	MAME_DIR .. "src/hbmame/drivers/cave.c",
	MAME_DIR .. "src/mame/video/cave.c",
}

createHBMAMEProjects(_target, _subtarget, "cinemat")
files {
	MAME_DIR .. "src/hbmame/drivers/cinemat.c",
	MAME_DIR .. "src/mame/audio/cinemat.c",
	MAME_DIR .. "src/mame/video/cinemat.c",
}

createHBMAMEProjects(_target, _subtarget, "dataeast")
files {
	MAME_DIR .. "src/hbmame/drivers/astrof.c",
	MAME_DIR .. "src/mame/audio/astrof.c",
	MAME_DIR .. "src/hbmame/drivers/btime.c",
	MAME_DIR .. "src/mame/machine/btime.c",
	MAME_DIR .. "src/mame/video/btime.c",
	MAME_DIR .. "src/hbmame/drivers/dec0.c",
	MAME_DIR .. "src/mame/machine/dec0.c",
	MAME_DIR .. "src/mame/video/dec0.c",
	MAME_DIR .. "src/mame/machine/deco222.c",
	MAME_DIR .. "src/mame/machine/decocpu7.c",
	MAME_DIR .. "src/hbmame/drivers/pcktgal.c",
	MAME_DIR .. "src/mame/video/pcktgal.c",
	MAME_DIR .. "src/mame/video/decbac06.c",
	MAME_DIR .. "src/mame/video/decmxc06.c",
}

createHBMAMEProjects(_target, _subtarget, "exidy")
files {
	MAME_DIR .. "src/hbmame/drivers/exidy.c",
	MAME_DIR .. "src/mame/audio/exidy.c",
	MAME_DIR .. "src/mame/video/exidy.c",
	MAME_DIR .. "src/mame/audio/targ.c",
}

createHBMAMEProjects(_target, _subtarget, "gottlieb")
files {
	MAME_DIR .. "src/hbmame/drivers/gottlieb.c",
	MAME_DIR .. "src/mame/audio/gottlieb.c",
	MAME_DIR .. "src/mame/video/gottlieb.c",
}

createHBMAMEProjects(_target, _subtarget, "igs")
files {
	MAME_DIR .. "src/hbmame/drivers/pgm.c",
	MAME_DIR .. "src/mame/video/pgm.c",
	MAME_DIR .. "src/mame/machine/pgmprot_igs027a_type1.c",
	MAME_DIR .. "src/mame/machine/pgmprot_igs027a_type2.c",
	MAME_DIR .. "src/mame/machine/pgmprot_igs027a_type3.c",
	MAME_DIR .. "src/mame/machine/pgmprot_igs025_igs012.c",
	MAME_DIR .. "src/mame/machine/pgmprot_igs025_igs022.c",
	MAME_DIR .. "src/mame/machine/pgmprot_igs025_igs028.c",
	MAME_DIR .. "src/mame/machine/pgmprot_orlegend.c",
	MAME_DIR .. "src/mame/machine/pgmcrypt.c",
	MAME_DIR .. "src/mame/machine/igs025.c",
	MAME_DIR .. "src/mame/machine/igs022.c",
	MAME_DIR .. "src/mame/machine/igs028.c",
}

createHBMAMEProjects(_target, _subtarget, "irem")
files {
	MAME_DIR .. "src/hbmame/drivers/m52.c",
	MAME_DIR .. "src/mame/video/m52.c",
	MAME_DIR .. "src/mame/machine/irem_cpu.c",
	MAME_DIR .. "src/mame/audio/irem.c",
}

createHBMAMEProjects(_target, _subtarget, "itech")
files {
	MAME_DIR .. "src/hbmame/drivers/itech8.c",
	MAME_DIR .. "src/mame/machine/slikshot.c",
	MAME_DIR .. "src/mame/video/itech8.c",
}

createHBMAMEProjects(_target, _subtarget, "jaleco")
files {
	MAME_DIR .. "src/hbmame/drivers/exerion.c",
	MAME_DIR .. "src/mame/video/exerion.c",
}

createHBMAMEProjects(_target, _subtarget, "konami")
files {
	MAME_DIR .. "src/hbmame/drivers/1943.c",
	MAME_DIR .. "src/mame/video/1943.c",
	MAME_DIR .. "src/mame/audio/hyprolyb.c",
	MAME_DIR .. "src/hbmame/drivers/nemesis.c",
	MAME_DIR .. "src/mame/video/nemesis.c",
	MAME_DIR .. "src/hbmame/drivers/shaolins.c",
	MAME_DIR .. "src/mame/video/shaolins.c",
	MAME_DIR .. "src/hbmame/drivers/tmnt.c",
	MAME_DIR .. "src/mame/video/tmnt.c",
	MAME_DIR .. "src/hbmame/drivers/trackfld.c",
	MAME_DIR .. "src/mame/machine/konami1.c",
	MAME_DIR .. "src/mame/audio/trackfld.c",
	MAME_DIR .. "src/mame/video/trackfld.c",
	MAME_DIR .. "src/mame/video/tutankhm.c",
	MAME_DIR .. "src/hbmame/drivers/yiear.c",
	MAME_DIR .. "src/mame/video/yiear.c",
	MAME_DIR .. "src/mame/video/konami_helper.c",
	MAME_DIR .. "src/mame/video/k051960.c",
	MAME_DIR .. "src/mame/video/k052109.c",
	MAME_DIR .. "src/mame/video/k053251.c",
	MAME_DIR .. "src/mame/video/k053244_k053245.c",
	MAME_DIR .. "src/mame/video/k054000.c",
	MAME_DIR .. "src/mame/video/k053936.c",
}

createHBMAMEProjects(_target, _subtarget, "midway")
files {
	MAME_DIR .. "src/hbmame/drivers/astrocde.c",
	MAME_DIR .. "src/mame/video/astrocde.c",
	MAME_DIR .. "src/mame/audio/gorf.c",
	MAME_DIR .. "src/mame/audio/wow.c",
	MAME_DIR .. "src/hbmame/drivers/mcr.c",
	MAME_DIR .. "src/mame/machine/mcr.c",
	MAME_DIR .. "src/mame/video/mcr.c",
	MAME_DIR .. "src/hbmame/drivers/mcr3.c",
	MAME_DIR .. "src/mame/video/mcr3.c",
	MAME_DIR .. "src/mame/drivers/mcr68.c",
	MAME_DIR .. "src/mame/machine/mcr68.c",
	MAME_DIR .. "src/mame/video/mcr68.c",
	MAME_DIR .. "src/hbmame/drivers/midtunit.c",
	MAME_DIR .. "src/mame/machine/midtunit.c",
	MAME_DIR .. "src/mame/video/midtunit.c",
	MAME_DIR .. "src/hbmame/drivers/midwunit.c",
	MAME_DIR .. "src/mame/machine/midwunit.c",
	MAME_DIR .. "src/hbmame/drivers/midyunit.c",
	MAME_DIR .. "src/mame/machine/midyunit.c",
	MAME_DIR .. "src/mame/video/midyunit.c",
	MAME_DIR .. "src/hbmame/drivers/williams.c",
	MAME_DIR .. "src/mame/machine/williams.c",
	MAME_DIR .. "src/mame/audio/williams.c",
	MAME_DIR .. "src/mame/video/williams.c",
	MAME_DIR .. "src/mame/machine/midwayic.c",
	MAME_DIR .. "src/mame/audio/midway.c",
	MAME_DIR .. "src/mame/audio/cage.c",
	MAME_DIR .. "src/mame/audio/dcs.c",
}

createHBMAMEProjects(_target, _subtarget, "namco")
files {
	MAME_DIR .. "src/hbmame/drivers/galaga.c",
	MAME_DIR .. "src/mame/audio/galaga.c",
	MAME_DIR .. "src/mame/video/galaga.c",
	MAME_DIR .. "src/mame/video/bosco.c",
	MAME_DIR .. "src/mame/video/digdug.c",
	MAME_DIR .. "src/mame/machine/xevious.c",
	MAME_DIR .. "src/mame/video/xevious.c",
	MAME_DIR .. "src/hbmame/drivers/galaxian.c",
	MAME_DIR .. "src/mame/audio/galaxian.c",
	MAME_DIR .. "src/hbmame/video/galaxian.c",
	MAME_DIR .. "src/hbmame/drivers/mappy.c",
	MAME_DIR .. "src/mame/video/mappy.c",
	MAME_DIR .. "src/hbmame/drivers/pacland.c",
	MAME_DIR .. "src/mame/video/pacland.c",
	MAME_DIR .. "src/mame/drivers/polepos.c",
	MAME_DIR .. "src/mame/audio/polepos.c",
	MAME_DIR .. "src/mame/video/polepos.c",
	MAME_DIR .. "src/hbmame/drivers/rallyx.c",
	MAME_DIR .. "src/mame/video/rallyx.c",
	MAME_DIR .. "src/mame/machine/namcoio.c",
	MAME_DIR .. "src/mame/machine/namco06.c",
	MAME_DIR .. "src/mame/machine/namco50.c",
	MAME_DIR .. "src/mame/machine/namco51.c",
	MAME_DIR .. "src/mame/machine/namco53.c",
	MAME_DIR .. "src/mame/audio/namco52.c",
	MAME_DIR .. "src/mame/audio/namco54.c",
	MAME_DIR .. "src/mame/audio/timeplt.c",
}

createHBMAMEProjects(_target, _subtarget, "neogeo")
files {
	MAME_DIR .. "src/mame/drivers/neogeo.c",
	MAME_DIR .. "src/mame/video/neogeo.c",
	MAME_DIR .. "src/mame/drivers/neogeo_noslot.c",
	MAME_DIR .. "src/mame/video/neogeo_spr.c",
	MAME_DIR .. "src/mame/machine/neocrypt.c",
	MAME_DIR .. "src/mame/machine/ng_memcard.c",
}

createHBMAMEProjects(_target, _subtarget, "nghb")
files {
	MAME_DIR .. "src/hbmame/drivers/nghb.c",
	MAME_DIR .. "src/hbmame/video/nghb.c",
	MAME_DIR .. "src/hbmame/video/nghb_spr.c",
	MAME_DIR .. "src/hbmame/machine/nghb_boot.c",
	MAME_DIR .. "src/hbmame/machine/nghb_crypt.c",
	MAME_DIR .. "src/hbmame/machine/nghb_prot.c",
	MAME_DIR .. "src/hbmame/machine/nghb_memc.c",
	MAME_DIR .. "src/emu/imagedev/cartslot.c",
}

createHBMAMEProjects(_target, _subtarget, "nichibut")
files {
	MAME_DIR .. "src/hbmame/drivers/cclimber.c",
	MAME_DIR .. "src/mame/machine/cclimber.c",
	MAME_DIR .. "src/mame/audio/cclimber.c",
	MAME_DIR .. "src/mame/video/cclimber.c",
	MAME_DIR .. "src/hbmame/drivers/terracre.c",
	MAME_DIR .. "src/mame/video/terracre.c",
}

createHBMAMEProjects(_target, _subtarget, "nintendo")
files {
	MAME_DIR .. "src/hbmame/drivers/dkong.c",
	MAME_DIR .. "src/mame/audio/dkong.c",
	MAME_DIR .. "src/mame/video/dkong.c",
	MAME_DIR .. "src/hbmame/drivers/mario.c",
	MAME_DIR .. "src/mame/audio/mario.c",
	MAME_DIR .. "src/mame/video/mario.c",
	MAME_DIR .. "src/hbmame/drivers/mmagic.c",
	MAME_DIR .. "src/hbmame/drivers/playch10.c",
	MAME_DIR .. "src/mame/machine/playch10.c",
	MAME_DIR .. "src/mame/video/playch10.c",
	MAME_DIR .. "src/hbmame/drivers/vsnes.c",
	MAME_DIR .. "src/mame/machine/vsnes.c",
	MAME_DIR .. "src/mame/video/vsnes.c",
	MAME_DIR .. "src/mame/video/ppu2c0x.c",
	MAME_DIR .. "src/devices/machine/z80dma.c",

}

createHBMAMEProjects(_target, _subtarget, "pacman")
files {
	MAME_DIR .. "src/hbmame/drivers/jrpacman.c",
	MAME_DIR .. "src/hbmame/drivers/pacman.c",
	MAME_DIR .. "src/hbmame/video/pacman.c",
	MAME_DIR .. "src/hbmame/drivers/pengo.c",
}

createHBMAMEProjects(_target, _subtarget, "phoenix")
files {
	MAME_DIR .. "src/hbmame/drivers/phoenix.c",
	MAME_DIR .. "src/mame/audio/phoenix.c",
	MAME_DIR .. "src/mame/video/phoenix.c",
	MAME_DIR .. "src/mame/audio/pleiads.c",
}

createHBMAMEProjects(_target, _subtarget, "psikyo")
files {
	MAME_DIR .. "src/hbmame/drivers/psikyosh.c",
	MAME_DIR .. "src/mame/video/psikyosh.c",
}

createHBMAMEProjects(_target, _subtarget, "sega")
files {
	MAME_DIR .. "src/mame/machine/megadriv.c",
	MAME_DIR .. "src/mame/drivers/megaplay.c",
	MAME_DIR .. "src/mame/drivers/megatech.c",
	MAME_DIR .. "src/hbmame/drivers/segac2.c",
	MAME_DIR .. "src/mame/drivers/segae.c",
	MAME_DIR .. "src/hbmame/drivers/segag80r.c",
	MAME_DIR .. "src/mame/machine/segag80.c",
	MAME_DIR .. "src/mame/audio/segag80r.c",
	MAME_DIR .. "src/mame/video/segag80r.c",
	MAME_DIR .. "src/hbmame/drivers/segaorun.c",
	MAME_DIR .. "src/mame/video/segaorun.c",
	MAME_DIR .. "src/hbmame/drivers/system1.c",
	MAME_DIR .. "src/mame/video/system1.c",
	MAME_DIR .. "src/hbmame/drivers/system16.c",
	MAME_DIR .. "src/mame/video/system16.c",
	MAME_DIR .. "src/mame/machine/315_5296.c",
	MAME_DIR .. "src/mame/machine/fd1089.c",
	MAME_DIR .. "src/mame/machine/fd1094.c",
	MAME_DIR .. "src/mame/machine/mc8123.c",
	MAME_DIR .. "src/mame/machine/segaic16.c",
	MAME_DIR .. "src/mame/machine/segacrpt.c",
	MAME_DIR .. "src/mame/machine/segacrp2.c",
	MAME_DIR .. "src/mame/audio/segasnd.c",
	MAME_DIR .. "src/mame/video/segaic16.c",
	MAME_DIR .. "src/mame/video/segaic16_road.c",
	MAME_DIR .. "src/mame/video/sega16sp.c",
}

createHBMAMEProjects(_target, _subtarget, "seta")
files {
	MAME_DIR .. "src/mame/machine/st0016.c",
	MAME_DIR .. "src/hbmame/drivers/simple_st0016.c",
}

createHBMAMEProjects(_target, _subtarget, "snk")
files {
	MAME_DIR .. "src/hbmame/drivers/snk6502.c",
	MAME_DIR .. "src/mame/audio/snk6502.c",
	MAME_DIR .. "src/mame/video/snk6502.c",
}

createHBMAMEProjects(_target, _subtarget, "taito")
files {
	MAME_DIR .. "src/hbmame/drivers/arkanoid.c",
	MAME_DIR .. "src/mame/machine/arkanoid.c",
	MAME_DIR .. "src/mame/video/arkanoid.c",
	MAME_DIR .. "src/hbmame/drivers/bublbobl.c",
	MAME_DIR .. "src/mame/machine/bublbobl.c",
	MAME_DIR .. "src/mame/video/bublbobl.c",
	MAME_DIR .. "src/hbmame/drivers/rastan.c",
	MAME_DIR .. "src/mame/video/rastan.c",
	MAME_DIR .. "src/hbmame/drivers/retofinv.c",
	MAME_DIR .. "src/mame/machine/retofinv.c",
	MAME_DIR .. "src/mame/video/retofinv.c",
	MAME_DIR .. "src/hbmame/drivers/taito_b.c",
	MAME_DIR .. "src/mame/video/taito_b.c",
	MAME_DIR .. "src/hbmame/drivers/taito_f2.c",
	MAME_DIR .. "src/mame/video/taito_f2.c",
	MAME_DIR .. "src/hbmame/drivers/taito_f3.c",
	MAME_DIR .. "src/mame/video/taito_f3.c",
	MAME_DIR .. "src/mame/machine/cchip.c",
	MAME_DIR .. "src/hbmame/drivers/volfied.c",
	MAME_DIR .. "src/mame/machine/volfied.c",
	MAME_DIR .. "src/mame/video/volfied.c",
	MAME_DIR .. "src/mame/audio/taitosnd.c",
	MAME_DIR .. "src/mame/audio/taito_en.c",
	MAME_DIR .. "src/mame/machine/taitoio.c",
	MAME_DIR .. "src/mame/video/taito_helper.c",
	MAME_DIR .. "src/mame/video/pc080sn.c",
	MAME_DIR .. "src/mame/video/pc090oj.c",
	MAME_DIR .. "src/mame/video/tc0100scn.c",
	MAME_DIR .. "src/mame/video/tc0280grd.c",
	MAME_DIR .. "src/mame/video/tc0360pri.c",
	MAME_DIR .. "src/mame/video/tc0480scp.c",
	MAME_DIR .. "src/mame/video/tc0110pcr.c",
	MAME_DIR .. "src/mame/video/tc0180vcu.c",
	MAME_DIR .. "src/hbmame/drivers/taito_l.c",
	MAME_DIR .. "src/mame/video/taito_l.c",
}

createHBMAMEProjects(_target, _subtarget, "tehkan")
files {
	MAME_DIR .. "src/mame/video/tecmo_spr.c",
	MAME_DIR .. "src/hbmame/drivers/bombjack.c",
	MAME_DIR .. "src/mame/video/bombjack.c",
	MAME_DIR .. "src/hbmame/drivers/tecmo.c",
	MAME_DIR .. "src/mame/video/tecmo.c",
	MAME_DIR .. "src/hbmame/drivers/tehkanwc.c",
	MAME_DIR .. "src/mame/video/tehkanwc.c",
}

createHBMAMEProjects(_target, _subtarget, "toaplan")
files {
	MAME_DIR .. "src/hbmame/drivers/toaplan2.c",
	MAME_DIR .. "src/mame/video/toaplan2.c",
	MAME_DIR .. "src/mame/video/gp9001.c",
	MAME_DIR .. "src/hbmame/drivers/twincobr.c",
	MAME_DIR .. "src/mame/machine/twincobr.c",
	MAME_DIR .. "src/mame/video/twincobr.c",
	MAME_DIR .. "src/mame/drivers/wardner.c",
	MAME_DIR .. "src/mame/video/toaplan_scu.c",
	MAME_DIR .. "src/mame/machine/nmk112.c",
}

createHBMAMEProjects(_target, _subtarget, "univers")
files {
	MAME_DIR .. "src/hbmame/drivers/docastle.c",
	MAME_DIR .. "src/mame/machine/docastle.c",
	MAME_DIR .. "src/mame/video/docastle.c",
	MAME_DIR .. "src/hbmame/drivers/mrdo.c",
	MAME_DIR .. "src/mame/video/mrdo.c",
}

createHBMAMEProjects(_target, _subtarget, "valadon")
files {
	MAME_DIR .. "src/hbmame/drivers/bagman.c",
	MAME_DIR .. "src/mame/machine/bagman.c",
	MAME_DIR .. "src/mame/video/bagman.c",
}

createHBMAMEProjects(_target, _subtarget, "misc")
files {
	MAME_DIR .. "src/hbmame/drivers/mineswp.c",
	MAME_DIR .. "src/hbmame/drivers/ir.c",
	MAME_DIR .. "src/hbmame/drivers/schaser.c",
	MAME_DIR .. "src/hbmame/drivers/spacmiss.c",
	MAME_DIR .. "src/hbmame/drivers/monaco.c",
	MAME_DIR .. "src/hbmame/drivers/atari_s1.c",
	MAME_DIR .. "src/mame/machine/genpin.c",
}

end

