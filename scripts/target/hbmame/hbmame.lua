-- license:GPL_2.0
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
CPUS["ADSP2106X"] = true -- model2
CPUS["ARM"] = true
CPUS["ARM7"] = true
CPUS["CCPU"] = true
CPUS["DSP16"] = true
CPUS["E1"] = true -- vamphalf
CPUS["ES5510"] = true
CPUS["FR"] = true
CPUS["G65816"] = true -- snesb
CPUS["H8"] = true -- seta2
CPUS["H6280"] = true
CPUS["HD6309"] = true
CPUS["I8085"] = true
CPUS["I8089"] = true
CPUS["I86"] = true
CPUS["I960"] = true -- model2
CPUS["KC80"] = true -- ddenlovr
CPUS["M37710"] = true -- namcona1
CPUS["M6502"] = true
CPUS["M6800"] = true
CPUS["M6805"] = true
CPUS["M6809"] = true
CPUS["M680X0"] = true
CPUS["MB88XX"] = true
CPUS["MB86233"] = true -- model2
CPUS["MB86235"] = true -- model2
CPUS["MCS48"] = true
CPUS["MCS51"] = true
CPUS["MN10200"] = true
CPUS["NEC"] = true
CPUS["PSX"] = true
CPUS["S2650"] = true
CPUS["SCUDSP"] = true -- stv
CPUS["SH"] = true
CPUS["SM510"] = true
CPUS["SPC700"] = true -- snesb
CPUS["SSP1601"] = true
CPUS["TLCS90"] = true
CPUS["TMS320C1X"] = true
CPUS["TMS320C3X"] = true
CPUS["TMS340X0"] = true
CPUS["TMS57002"] = true
CPUS["PIC16C5X"] = true
CPUS["UPD7725"] = true  -- segas32
CPUS["UPD7810"] = true
CPUS["V60"] = true  -- segas32
CPUS["V810"] = true
CPUS["Z180"] = true
CPUS["Z80"] = true
CPUS["Z8000"] = true  -- polepos

--------------------------------------------------
-- specify available sound cores
--------------------------------------------------

--SOUNDS["ASTROCADE"] = true

--------------------------------------------------
-- specify available video cores
--------------------------------------------------

--VIDEOS["BUFSPRITE"] = true

--------------------------------------------------
-- specify available machine cores
--------------------------------------------------

MACHINES["6522VIA"] = true
MACHINES["6821PIA"] = true
MACHINES["6840PTM"] = true -- ginganin
MACHINES["68681"] = true
MACHINES["7200FIFO"] = true
MACHINES["AAKARTDEV"] = true
MACHINES["ACIA6850"] = true -- seta
MACHINES["ACORN_VIDC"] = false -- armdemo, MNW
MACHINES["ADC0804"] = true
MACHINES["ADC0808"] = true
MACHINES["ADC083X"] = true -- seta
MACHINES["ADC0844"] = true
MACHINES["AM9517A"] = true -- model1
MACHINES["AT28C16"] = true -- zn
MACHINES["BANKDEV"] = true
MACHINES["CXD1095"] = true
MACHINES["EEPROMDEV"] = true
MACHINES["ER2055"] = true
MACHINES["FDC_PLL"] = true
MACHINES["I2CMEM"] = true
MACHINES["I8243"] = true
MACHINES["I8251"] = true
MACHINES["I8255"] = true
MACHINES["IDECTRL"] = true
MACHINES["INPUT_MERGER"] = true
MACHINES["INTELFLASH"] = true
MACHINES["GEN_FIFO"] = true
MACHINES["GEN_LATCH"] = true
MACHINES["I8257"] = true
MACHINES["K053252"] = true
MACHINES["K054321"] = true -- mystwarr
MACHINES["LATCH8"] = true
MACHINES["LDPR8210"] = true
MACHINES["MB3773"] = true
MACHINES["MB8421"] = true
MACHINES["MB14241"] = true
MACHINES["MB87078"] = true
MACHINES["MB89352"] = true  -- segas32
MACHINES["MB89374"] = true  -- model1
MACHINES["MCF5206E"] = true -- seta2
MACHINES["MIOT6530"] = true
MACHINES["MSM6242"] = true -- namcona1
MACHINES["MSM6253"] = true
MACHINES["NETLIST"] = true
MACHINES["NSCSI"] = true -- cps3
MACHINES["OUTPUT_LATCH"] = true
MACHINES["PIC8259"] = true -- m92
MACHINES["PIT8253"] = true -- seta
MACHINES["RIOT6532"] = true
MACHINES["RP5C01"] = true
MACHINES["RP5H01"] = true
MACHINES["RSTBUF"] = true -- dynax, ddenlovr
MACHINES["RTC9701"] = true -- cv1k
MACHINES["SEGA_SCU"] = true -- stv
MACHINES["SERFLASH"] = true -- cv1k
MACHINES["SMPC"] = true -- stv
MACHINES["STVCD"] = true -- stv
MACHINES["S_SMP"] = true -- snes
MACHINES["TC0091LVC"] = true
MACHINES["TE7750"] = true
MACHINES["TICKET"] = true
MACHINES["TIMEKPR"] = true
MACHINES["TMP68301"] = true -- seta2
MACHINES["TMS1024"] = true -- docastle
MACHINES["TMS6100"] = true
MACHINES["TTL74153"] = true
MACHINES["TTL74157"] = true
MACHINES["TTL74259"] = true
MACHINES["TTL7474"] = true -- scramble
MACHINES["UPD1990A"] = true
MACHINES["UPD4701"] = true
MACHINES["UPD4992"] = true
MACHINES["UPD7001"] = true
MACHINES["V3021"] = true
MACHINES["WATCHDOG"] = true
MACHINES["WD33C93"] = true
MACHINES["WD33C9X"] = true -- cps3
MACHINES["WD_FDC"] = true
MACHINES["Z80CTC"] = true
MACHINES["Z80DAISY"] = true
MACHINES["Z80PIO"] = true
MACHINES["Z80SIO"] = true -- ddenlovr

--------------------------------------------------
-- specify available bus cores
--------------------------------------------------

BUSES["ATA"] = true
BUSES["GENERIC"] = true
BUSES["NSCSI"] = true
BUSES["NES_CTRL"] = true -- playch10
BUSES["SAT_CTRL"] = true -- stv
BUSES["SCSI"] = true

--------------------------------------------------
-- this is the list of driver libraries that
-- comprise MAME plus hbmame.lst which contains
-- the list of drivers
--------------------------------------------------

function linkProjects_hbmame_hbmame(_target, _subtarget)
	links {
--		"acorn",
		"atari",
		"capcom",
		"cave",
		"cinemat",
		"dataeast",
		"dynax",
		"exidy",
		"fuuki",
		"galaxian",
		"gottlieb",
		"igs",
		"irem",
		"itech",
		"jaleco",
		"kaneko",
		"konami",
		"metro",
		"midway",
		"namco",
		"neogeo",
		"nichibutsu",
		"nintendo",
		"nmk",
		"phoenix",
		"psikyo",
		"puckman",
		"sega",
		"seibu",
		"seta",
		"snk",
		"sony",
		"taito",
		"technos",
		"tecmo",
		"toaplan",
		"unico",
		"universal",
		"valadon",
		"vsystem",
		"misc",
		"devices",
	}
end

function createHBMAMEProjects(_target, _subtarget, _name)
	project (_name)
	targetsubdir(_target .."_" .. _subtarget)
	kind (LIBTYPE)
	uuid (os.uuid("drv-" .. _target .."_" .. _subtarget .. "_" .._name))
--	addprojectflags() -- no idea if this is needed, seems to work fine without it
	precompiledheaders()

	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/hbmame/includes",
		MAME_DIR .. "src/hbmame",
		MAME_DIR .. "src/mame",
		MAME_DIR .. "src/mame/shared",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "src/lib/netlist",
		MAME_DIR .. "3rdparty",
		MAME_DIR .. "3rdparty/zlib",
		GEN_DIR  .. "mame/layout",
		GEN_DIR  .. "hbmame/layout",
	}
	includedirs {
		ext_includedir("flac"),
		ext_includedir("glm"), -- model1
	}
end

function createProjects_hbmame_hbmame(_target, _subtarget)
--------------------------------------------------
-- manufacturer-specific groupings for drivers
--------------------------------------------------

--createHBMAMEProjects(_target, _subtarget, "acorn")
--files {
--	MAME_DIR .. "src/hbmame/drivers/ertictac.cpp",
--	MAME_DIR .. "src/mame/machine/archimds.cpp",
--}

createHBMAMEProjects(_target, _subtarget, "atari")
files {
	MAME_DIR .. "src/hbmame/drivers/asteroid.cpp",
	MAME_DIR .. "src/mame/atari/asteroid_m.cpp",
	MAME_DIR .. "src/hbmame/drivers/atarigt.cpp",
	MAME_DIR .. "src/mame/atari/atarigt_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/atarisy1.cpp",
	MAME_DIR .. "src/mame/atari/atarirle.cpp",  -- for atarigt
	MAME_DIR .. "src/mame/atari/atarisy1.cpp",
	MAME_DIR .. "src/mame/atari/asteroid_a.cpp",
	MAME_DIR .. "src/hbmame/drivers/bwidow.cpp",
	MAME_DIR .. "src/mame/atari/bwidow_a.cpp",
	MAME_DIR .. "src/hbmame/drivers/cball.cpp",
	MAME_DIR .. "src/hbmame/drivers/centiped.cpp",
	MAME_DIR .. "src/mame/atari/centiped_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/gauntlet.cpp",
	MAME_DIR .. "src/hbmame/drivers/tempest.cpp",
	MAME_DIR .. "src/mame/atari/atarigen.cpp",
	MAME_DIR .. "src/mame/atari/mathbox.cpp",
	MAME_DIR .. "src/mame/atari/slapstic.cpp",
	MAME_DIR .. "src/mame/atari/atarimo.cpp",
	MAME_DIR .. "src/hbmame/drivers/mhavoc.cpp",
	MAME_DIR .. "src/hbmame/drivers/mhavoc_hb.cpp",
	MAME_DIR .. "src/hbmame/machine/mhavoc_hb.cpp",
	MAME_DIR .. "src/mame/atari/atarisy1_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/missile.cpp",
}

createHBMAMEProjects(_target, _subtarget, "capcom")
files {
	MAME_DIR .. "src/hbmame/drivers/1942.cpp",
	MAME_DIR .. "src/mame/capcom/1942_v.cpp",
	MAME_DIR .. "src/mame/capcom/nl_1942.cpp",
	MAME_DIR .. "src/hbmame/drivers/1943.cpp",
	MAME_DIR .. "src/mame/capcom/1943.cpp",
	MAME_DIR .. "src/mame/capcom/1943_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/blktiger.cpp",
	MAME_DIR .. "src/hbmame/drivers/commando.cpp",
	MAME_DIR .. "src/hbmame/drivers/sonson.cpp",
	MAME_DIR .. "src/hbmame/drivers/cps1.cpp",
	MAME_DIR .. "src/hbmame/video/cps1.cpp",
	MAME_DIR .. "src/hbmame/drivers/cps2.cpp",
	MAME_DIR .. "src/hbmame/video/cps2.cpp",
	MAME_DIR .. "src/hbmame/drivers/cps3.cpp",
	MAME_DIR .. "src/mame/capcom/cps3_a.cpp",
	MAME_DIR .. "src/hbmame/drivers/fcrash.cpp",
	MAME_DIR .. "src/hbmame/drivers/mitchell.cpp",
	MAME_DIR .. "src/mame/capcom/kabuki.cpp",
	MAME_DIR .. "src/hbmame/drivers/sf.cpp",
	MAME_DIR .. "src/hbmame/drivers/gng.cpp",
}

createHBMAMEProjects(_target, _subtarget, "cave")
files {
	MAME_DIR .. "src/hbmame/drivers/cave.cpp",
	MAME_DIR .. "src/mame/atlus/cave_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/cv1k.cpp",
	MAME_DIR .. "src/mame/cave/cv1k_v.cpp",
	MAME_DIR .. "src/mame/atlus/sprite013.cpp",
}

createHBMAMEProjects(_target, _subtarget, "cinemat")
files {
	MAME_DIR .. "src/hbmame/drivers/cinemat.cpp",
	MAME_DIR .. "src/mame/cinematronics/cinemat_a.cpp",
	MAME_DIR .. "src/mame/cinematronics/cinemat_v.cpp",
	MAME_DIR .. "src/mame/cinematronics/nl_armora.cpp",    -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_barrier.cpp",   -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_boxingb.cpp",
	MAME_DIR .. "src/mame/cinematronics/nl_cinemat_common.h",
	MAME_DIR .. "src/mame/cinematronics/nl_ripoff.cpp",    -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_solarq.cpp",    -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_spacewar.cpp",  -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_speedfrk.cpp",  -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_starcas.cpp",   -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_starhawk.cpp",  -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_sundance.cpp",  -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_tailg.cpp",     -- not used
	MAME_DIR .. "src/mame/cinematronics/nl_warrior.cpp",
}

createHBMAMEProjects(_target, _subtarget, "dataeast")
files {
	MAME_DIR .. "src/hbmame/drivers/astrof.cpp",
	MAME_DIR .. "src/mame/dataeast/astrof_a.cpp",
	MAME_DIR .. "src/hbmame/drivers/btime.cpp",
	MAME_DIR .. "src/hbmame/drivers/cninja.cpp",
	MAME_DIR .. "src/mame/dataeast/cninja_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/dec0.cpp",
	MAME_DIR .. "src/mame/dataeast/dec0_m.cpp",
	MAME_DIR .. "src/mame/dataeast/dec0_v.cpp",
	MAME_DIR .. "src/mame/dataeast/deco222.cpp",
	MAME_DIR .. "src/mame/dataeast/decocpu7.cpp",
	MAME_DIR .. "src/hbmame/drivers/pcktgal.cpp",
	MAME_DIR .. "src/mame/dataeast/decbac06.cpp",
	MAME_DIR .. "src/mame/dataeast/decmxc06.cpp",
	MAME_DIR .. "src/mame/dataeast/deco16ic.cpp", -- cninja
	MAME_DIR .. "src/mame/dataeast/deco104.cpp",  -- cninja
	MAME_DIR .. "src/mame/dataeast/deco146.cpp", -- cninja
	MAME_DIR .. "src/mame/dataeast/deco156.cpp", -- cninja
	MAME_DIR .. "src/mame/shared/decospr.cpp",  -- cninja
	MAME_DIR .. "src/mame/dataeast/deco_irq.cpp",  -- cninja
	MAME_DIR .. "src/mame/dataeast/decocrpt.cpp",  -- cninja
	MAME_DIR .. "src/hbmame/drivers/deco_mlc.cpp",  -- mlc
	MAME_DIR .. "src/mame/dataeast/deco_mlc_v.cpp",    -- mlc
	MAME_DIR .. "src/hbmame/drivers/fghthist.cpp",
	MAME_DIR .. "src/mame/dataeast/fghthist_v.cpp",
	MAME_DIR .. "src/mame/shared/decobsmt.cpp", -- deco32
	MAME_DIR .. "src/mame/dataeast/deco_ace.cpp", -- deco32
	MAME_DIR .. "src/hbmame/drivers/rohga.cpp",
	MAME_DIR .. "src/hbmame/drivers/sidepckt.cpp",
	MAME_DIR .. "src/mame/dataeast/decocass.cpp",
	MAME_DIR .. "src/mame/dataeast/decocass_m.cpp",
	MAME_DIR .. "src/mame/dataeast/decocass_tape.cpp",
	MAME_DIR .. "src/mame/dataeast/decocass_v.cpp",
}

createHBMAMEProjects(_target, _subtarget, "dynax")
files {
	MAME_DIR .. "src/hbmame/drivers/ddenlovr.cpp",
	MAME_DIR .. "src/hbmame/drivers/dynax.cpp",
	MAME_DIR .. "src/mame/dynax/dynax_v.cpp",
	MAME_DIR .. "src/mame/dynax/dynax_blitter_rev2.cpp",
	MAME_DIR .. "src/mame/shared/mahjong.cpp",
	MAME_DIR .. "src/mame/dynax/hanafuda.cpp",
}

createHBMAMEProjects(_target, _subtarget, "exidy")
files {
	MAME_DIR .. "src/hbmame/drivers/exidy.cpp",
	MAME_DIR .. "src/mame/shared/exidysound.cpp",
}

createHBMAMEProjects(_target, _subtarget, "fuuki")
files {
	MAME_DIR .. "src/hbmame/drivers/fuukifg3.cpp",
	MAME_DIR .. "src/mame/fuuki/fuukifg3.cpp",
	MAME_DIR .. "src/mame/fuuki/fuukifg2.cpp",
	MAME_DIR .. "src/mame/fuuki/fuukispr.cpp",
	MAME_DIR .. "src/mame/fuuki/fuukitmap.cpp",
}

createHBMAMEProjects(_target, _subtarget, "galaxian")
files {
	MAME_DIR .. "src/hbmame/drivers/galaxian.cpp",
	MAME_DIR .. "src/mame/galaxian/galaxian_a.cpp",
	MAME_DIR .. "src/mame/galaxian/galaxian_v.cpp",
	MAME_DIR .. "src/mame/galaxian/galaxold.cpp",
	MAME_DIR .. "src/mame/galaxian/galaxold_m.cpp",
	MAME_DIR .. "src/mame/galaxian/galaxold_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/scramble.cpp",
	MAME_DIR .. "src/mame/galaxian/scramble_a.cpp",
	MAME_DIR .. "src/mame/galaxian/scramble_m.cpp",
	MAME_DIR .. "src/mame/galaxian/nl_konami.cpp",
}

createHBMAMEProjects(_target, _subtarget, "gottlieb")
files {
	MAME_DIR .. "src/hbmame/drivers/gottlieb.cpp",
	MAME_DIR .. "src/mame/shared/gottlieb_a.cpp",
}

createHBMAMEProjects(_target, _subtarget, "igs")
files {
	MAME_DIR .. "src/hbmame/drivers/igs011.cpp",
	MAME_DIR .. "src/mame/igs/igs011_video.cpp",
	MAME_DIR .. "src/mame/igs/igs023_video.cpp",
	MAME_DIR .. "src/hbmame/drivers/pgm.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_igs027a_type1.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_igs027a_type2.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_igs027a_type3.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_igs025_igs012.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_igs025_igs022.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_igs025_igs028.cpp",
	MAME_DIR .. "src/mame/igs/pgmprot_orlegend.cpp",
	MAME_DIR .. "src/mame/igs/pgmcrypt.cpp",
	MAME_DIR .. "src/mame/igs/igs012.cpp",
	MAME_DIR .. "src/mame/igs/igs025.cpp",
	MAME_DIR .. "src/mame/igs/igs022.cpp",
	MAME_DIR .. "src/mame/igs/igs028.cpp",
	MAME_DIR .. "src/hbmame/drivers/lordgun.cpp",
	MAME_DIR .. "src/mame/igs/lordgun_v.cpp",
	MAME_DIR .. "src/mame/igs/igsmahjong.cpp",
}

createHBMAMEProjects(_target, _subtarget, "irem")
files {
	MAME_DIR .. "src/hbmame/drivers/m52.cpp",
	MAME_DIR .. "src/mame/irem/irem.cpp",
	MAME_DIR .. "src/hbmame/drivers/m92.cpp",
	MAME_DIR .. "src/mame/irem/m92_v.cpp",
	MAME_DIR .. "src/mame/irem/irem_cpu.cpp",
	MAME_DIR .. "src/mame/irem/nl_kidniki.cpp",
}

createHBMAMEProjects(_target, _subtarget, "itech")
files {
	MAME_DIR .. "src/hbmame/drivers/itech8.cpp",
	MAME_DIR .. "src/mame/itech/itech8_m.cpp",
	MAME_DIR .. "src/mame/itech/itech8_v.cpp",
}

createHBMAMEProjects(_target, _subtarget, "jaleco")
files {
	MAME_DIR .. "src/hbmame/drivers/exerion.cpp",
	MAME_DIR .. "src/hbmame/drivers/ginganin.cpp",
	MAME_DIR .. "src/hbmame/drivers/megasys1.cpp",
	MAME_DIR .. "src/mame/jaleco/megasys1.cpp",
	MAME_DIR .. "src/mame/jaleco/megasys1_v.cpp",
	MAME_DIR .. "src/mame/jaleco/jalcrpt.cpp",
	MAME_DIR .. "src/mame/jaleco/ms1_tmap.cpp",
	MAME_DIR .. "src/mame/jaleco/ms1_gatearray.cpp",
}

createHBMAMEProjects(_target, _subtarget, "kaneko")
files {
	MAME_DIR .. "src/hbmame/drivers/kaneko16.cpp",
	MAME_DIR .. "src/mame/kaneko/kaneko16_v.cpp",
	MAME_DIR .. "src/mame/kaneko/kaneko_tmap.cpp",
	MAME_DIR .. "src/mame/kaneko/kaneko_spr.cpp",
	MAME_DIR .. "src/mame/kaneko/kaneko_hit.cpp",
	MAME_DIR .. "src/mame/kaneko/kaneko_calc3.cpp",
	MAME_DIR .. "src/mame/kaneko/kaneko_toybox.cpp",
	MAME_DIR .. "src/hbmame/drivers/snowbros.cpp",
	MAME_DIR .. "src/mame/kaneko/snowbros_v.cpp",
	MAME_DIR .. "src/mame/kaneko/kan_pand.cpp",  -- snowbros
}

createHBMAMEProjects(_target, _subtarget, "konami")
files {
	MAME_DIR .. "src/hbmame/drivers/blockhl.cpp",
	MAME_DIR .. "src/hbmame/drivers/contra.cpp",
	MAME_DIR .. "src/hbmame/drivers/dbz.cpp",
	MAME_DIR .. "src/hbmame/drivers/gradius3.cpp",
	MAME_DIR .. "src/hbmame/drivers/gyruss.cpp",
	MAME_DIR .. "src/hbmame/drivers/hyperspt.cpp",
	MAME_DIR .. "src/mame/konami/hyprolyb.cpp",
	MAME_DIR .. "src/hbmame/drivers/mystwarr.cpp",
	MAME_DIR .. "src/mame/konami/mystwarr_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/nemesis.cpp",
	MAME_DIR .. "src/mame/konami/nemesis_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/pooyan.cpp",
	MAME_DIR .. "src/hbmame/drivers/shaolins.cpp",
	MAME_DIR .. "src/hbmame/drivers/timeplt.cpp",
	MAME_DIR .. "src/hbmame/drivers/tmnt2.cpp",
	MAME_DIR .. "src/hbmame/drivers/trackfld.cpp",
	MAME_DIR .. "src/mame/konami/trackfld_a.cpp",
	MAME_DIR .. "src/hbmame/drivers/konamigx.cpp",
	MAME_DIR .. "src/mame/konami/konamigx_m.cpp",
	MAME_DIR .. "src/mame/konami/konamigx_v.cpp",
	MAME_DIR .. "src/mame/konami/konami1.cpp",
	MAME_DIR .. "src/hbmame/drivers/twin16.cpp",
	MAME_DIR .. "src/mame/konami/twin16_v.cpp",
	MAME_DIR .. "src/mame/konami/tutankhm_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/yiear.cpp",
	MAME_DIR .. "src/mame/konami/konami_helper.cpp",
	MAME_DIR .. "src/mame/konami/k007121.cpp",
	MAME_DIR .. "src/mame/konami/k007452.cpp",
	MAME_DIR .. "src/mame/konami/k051960.cpp",
	MAME_DIR .. "src/mame/konami/k052109.cpp",
	MAME_DIR .. "src/mame/konami/k053244_k053245.cpp",
	MAME_DIR .. "src/mame/konami/k053246_k053247_k055673.cpp",
	MAME_DIR .. "src/mame/konami/k053250.cpp",
	MAME_DIR .. "src/mame/konami/k053251.cpp",
	MAME_DIR .. "src/mame/konami/k054000.cpp",
	MAME_DIR .. "src/mame/konami/k054156_k054157_k056832.cpp",
	MAME_DIR .. "src/mame/konami/k054338.cpp",
	MAME_DIR .. "src/mame/konami/k055555.cpp",
	MAME_DIR .. "src/hbmame/machine/galser.cpp",
	MAME_DIR .. "src/mame/shared/timeplt_a.cpp",
}

createHBMAMEProjects(_target, _subtarget, "metro")
files {
	MAME_DIR .. "src/hbmame/drivers/rabbit.cpp",
}

createHBMAMEProjects(_target, _subtarget, "midway")
files {
	MAME_DIR .. "src/hbmame/drivers/invaders.cpp",
	MAME_DIR .. "src/hbmame/drivers/spcenctr.cpp",
	MAME_DIR .. "src/hbmame/drivers/astrocde.cpp",
	MAME_DIR .. "src/mame/bally/astrocde_v.cpp",
	MAME_DIR .. "src/mame/bally/csd.cpp",
	MAME_DIR .. "src/hbmame/drivers/mcr.cpp",
	MAME_DIR .. "src/mame/bally/mcr_m.cpp",
	MAME_DIR .. "src/mame/bally/mcr_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/mcr3.cpp",
	MAME_DIR .. "src/mame/bally/mcr3_v.cpp",
	MAME_DIR .. "src/mame/bally/mcr68.cpp",
	MAME_DIR .. "src/mame/bally/midway_sound.cpp",
	MAME_DIR .. "src/hbmame/drivers/midtunit.cpp",
	MAME_DIR .. "src/mame/williams/midtunit_m.cpp",
	MAME_DIR .. "src/mame/williams/midtunit_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/midwunit.cpp",
	MAME_DIR .. "src/mame/williams/midwunit_m.cpp",
	MAME_DIR .. "src/hbmame/drivers/midyunit.cpp",
	MAME_DIR .. "src/mame/williams/midyunit_m.cpp",
	MAME_DIR .. "src/mame/williams/midyunit_v.cpp",
	MAME_DIR .. "src/mame/williams/wmg.cpp",
	MAME_DIR .. "src/hbmame/drivers/williams.cpp",
	MAME_DIR .. "src/mame/williams/williams_m.cpp",
	MAME_DIR .. "src/mame/williams/williams_v.cpp",
	MAME_DIR .. "src/mame/shared/williamssound.cpp",
	MAME_DIR .. "src/mame/williams/midwayic.cpp",
	MAME_DIR .. "src/mame/shared/cage.cpp",
	MAME_DIR .. "src/mame/shared/dcs.cpp",
	MAME_DIR .. "src/mame/shared/ballysound.cpp",
	MAME_DIR .. "src/mame/shared/s11c_bg.cpp",
	MAME_DIR .. "src/mame/williams/williamsblitter.cpp",
}

createHBMAMEProjects(_target, _subtarget, "namco")
files {
	MAME_DIR .. "src/hbmame/drivers/galaga.cpp",
	MAME_DIR .. "src/mame/namco/galaga_a.cpp",
	MAME_DIR .. "src/mame/namco/galaga_v.cpp",
	MAME_DIR .. "src/mame/namco/bosco.cpp",
	MAME_DIR .. "src/mame/namco/digdug.cpp",
	MAME_DIR .. "src/mame/namco/xevious_m.cpp",
	MAME_DIR .. "src/mame/namco/xevious.cpp",
	MAME_DIR .. "src/mame/namco/starfield_05xx.cpp",
	MAME_DIR .. "src/hbmame/drivers/mappy.cpp",
	MAME_DIR .. "src/mame/namco/mappy_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/pacland.cpp",
	MAME_DIR .. "src/hbmame/drivers/polepos.cpp",
	MAME_DIR .. "src/mame/namco/polepos_a.cpp",
	MAME_DIR .. "src/mame/namco/polepos_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/rallyx.cpp",
	MAME_DIR .. "src/mame/namco/rallyx_v.cpp",
	MAME_DIR .. "src/mame/namco/namcoio.cpp",
	MAME_DIR .. "src/mame/namco/namco06.cpp",
	MAME_DIR .. "src/mame/namco/namco50.cpp",
	MAME_DIR .. "src/mame/namco/namco51.cpp",
	MAME_DIR .. "src/mame/namco/namco53.cpp",
	MAME_DIR .. "src/mame/namco/namco52.cpp",
	MAME_DIR .. "src/mame/namco/namco54.cpp",
	MAME_DIR .. "src/hbmame/drivers/namcona1.cpp",
	MAME_DIR .. "src/mame/namco/namcona1_v.cpp",
	MAME_DIR .. "src/mame/namco/namcomcu.cpp",
	MAME_DIR .. "src/hbmame/drivers/namcos2.cpp",
	MAME_DIR .. "src/mame/namco/namcos2_m.cpp",
	MAME_DIR .. "src/mame/namco/namcos2_v.cpp",
	MAME_DIR .. "src/mame/namco/namcos2_roz.cpp",
	MAME_DIR .. "src/mame/namco/namcos1_sprite.cpp",
	MAME_DIR .. "src/mame/namco/namcos2_sprite.cpp",
	MAME_DIR .. "src/mame/namco/namco_c45road.cpp",
	MAME_DIR .. "src/mame/namco/namco65.cpp",
	MAME_DIR .. "src/mame/namco/namco68.cpp",
	MAME_DIR .. "src/hbmame/drivers/namcos86.cpp",
	MAME_DIR .. "src/mame/namco/namcos86_v.cpp",
	MAME_DIR .. "src/mame/namco/namco_c116.cpp",
	MAME_DIR .. "src/mame/namco/namco_c123tmap.cpp",
	MAME_DIR .. "src/mame/namco/namco_c139.cpp",
	MAME_DIR .. "src/mame/namco/namco_c148.cpp",
	MAME_DIR .. "src/mame/namco/namco_c169roz.cpp",
	MAME_DIR .. "src/mame/namco/namco_cus4xtmap.cpp",
	MAME_DIR .. "src/mame/shared/namco_c355spr.cpp",
}

createHBMAMEProjects(_target, _subtarget, "neogeo")
files {
	MAME_DIR .. "src/hbmame/drivers/neogeo.cpp",
	MAME_DIR .. "src/hbmame/drivers/neogeohb.cpp",
	MAME_DIR .. "src/hbmame/drivers/neogeo1.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof94.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof95.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof96.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof97.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof98.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof99.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof2000.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof2001.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof2002.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof2003.cpp",
	MAME_DIR .. "src/hbmame/drivers/kof2k2re.cpp",
	MAME_DIR .. "src/hbmame/drivers/mgd2.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslug.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslug2.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslug3.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslug4.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslug5.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslug6.cpp",
	MAME_DIR .. "src/hbmame/drivers/mslugx.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngdoubledr.cpp",
	MAME_DIR .. "src/hbmame/drivers/nggaroup.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngkofmisc.cpp",
	MAME_DIR .. "src/hbmame/drivers/nglastblade.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngrbff.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngrotd.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngsamsho.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngsengoku.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngsvc.cpp",
	MAME_DIR .. "src/hbmame/drivers/ngwh.cpp",
	MAME_DIR .. "src/hbmame/video/neogeo.cpp",
	MAME_DIR .. "src/hbmame/drivers/neogeo_noslot.cpp",
	MAME_DIR .. "src/hbmame/video/neogeo_spr.cpp",
	MAME_DIR .. "src/hbmame/machine/ng_memcard.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo/prot.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo/banked_cart.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo_ctrl/ctrl.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo_ctrl/joystick.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo_ctrl/mahjong.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo_ctrl/dial.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo_ctrl/irrmaze.cpp",
	MAME_DIR .. "src/hbmame/bus/neogeo_ctrl/kizuna4p.cpp",
--	MAME_DIR .. "src/mame/drivers/neogeocd.cpp",
}

createHBMAMEProjects(_target, _subtarget, "nichibutsu")
files {
	MAME_DIR .. "src/hbmame/drivers/cclimber.cpp",
	MAME_DIR .. "src/hbmame/drivers/terracre.cpp",
	MAME_DIR .. "src/mame/nichibutsu/nb1412m2.cpp",
	MAME_DIR .. "src/mame/shared/cclimber_a.cpp",
}

createHBMAMEProjects(_target, _subtarget, "nintendo")
files {
	MAME_DIR .. "src/hbmame/drivers/dkong.cpp",
	MAME_DIR .. "src/mame/nintendo/dkong_a.cpp",
	MAME_DIR .. "src/mame/nintendo/dkong_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/mario.cpp",
	MAME_DIR .. "src/mame/nintendo/nl_mario.cpp",
	MAME_DIR .. "src/hbmame/drivers/playch10.cpp",
	MAME_DIR .. "src/mame/nintendo/snes.cpp",
	MAME_DIR .. "src/mame/nintendo/snes_m.cpp",
	MAME_DIR .. "src/hbmame/drivers/snesb.cpp",
	MAME_DIR .. "src/hbmame/drivers/vsnes.cpp",

}

createHBMAMEProjects(_target, _subtarget, "nmk")
files {
	MAME_DIR .. "src/hbmame/drivers/nmk16.cpp",
	MAME_DIR .. "src/mame/nmk/nmk16_v.cpp",
	MAME_DIR .. "src/mame/nmk/nmk16spr.cpp",
	MAME_DIR .. "src/mame/nmk/nmk004.cpp",
	MAME_DIR .. "src/mame/nmk/nmk214.cpp",
	MAME_DIR .. "src/mame/nmk/nmk_irq.cpp",
	MAME_DIR .. "src/mame/shared/seibusound.cpp",
}

createHBMAMEProjects(_target, _subtarget, "puckman")
files {
	MAME_DIR .. "src/hbmame/drivers/jrpacman.cpp",
	MAME_DIR .. "src/hbmame/drivers/puckman.cpp",
	MAME_DIR .. "src/hbmame/video/puckman.cpp",
	MAME_DIR .. "src/hbmame/drivers/pengo.cpp",
}

createHBMAMEProjects(_target, _subtarget, "phoenix")
files {
	MAME_DIR .. "src/hbmame/drivers/phoenix.cpp",
	MAME_DIR .. "src/mame/phoenix/phoenix_a.cpp",
	MAME_DIR .. "src/mame/phoenix/phoenix_v.cpp",
	MAME_DIR .. "src/mame/phoenix/pleiads.cpp",
}

createHBMAMEProjects(_target, _subtarget, "psikyo")
files {
	MAME_DIR .. "src/hbmame/drivers/psikyo.cpp",
	MAME_DIR .. "src/mame/psikyo/psikyo_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/psikyosh.cpp",
	MAME_DIR .. "src/mame/psikyo/psikyosh_v.cpp",
}

createHBMAMEProjects(_target, _subtarget, "sega")
files {
	MAME_DIR .. "src/hbmame/drivers/segas24.cpp",
	MAME_DIR .. "src/mame/sega/megaplay.cpp",
	MAME_DIR .. "src/mame/sega/megatech.cpp",
	MAME_DIR .. "src/hbmame/drivers/segac2.cpp",
	MAME_DIR .. "src/mame/sega/segae.cpp",
	MAME_DIR .. "src/hbmame/drivers/segag80r.cpp",
	MAME_DIR .. "src/mame/sega/segag80.cpp",
	MAME_DIR .. "src/mame/sega/segag80_m.cpp",
	MAME_DIR .. "src/mame/sega/segag80r_a.cpp",
	MAME_DIR .. "src/mame/sega/segag80r_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/segaorun.cpp",
	MAME_DIR .. "src/hbmame/drivers/system1.cpp",
	MAME_DIR .. "src/mame/sega/system1_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/system16.cpp",
	MAME_DIR .. "src/mame/sega/system16_v.cpp",
	MAME_DIR .. "src/mame/sega/nl_segas16b.cpp",
	MAME_DIR .. "src/mame/sega/315_5296.cpp",
	MAME_DIR .. "src/mame/sega/fd1089.cpp",
	MAME_DIR .. "src/mame/sega/fd1094.cpp",
	MAME_DIR .. "src/mame/sega/segaspeech.cpp",
	MAME_DIR .. "src/mame/sega/segausb.cpp",
	MAME_DIR .. "src/mame/sega/nl_segausb.cpp",
	MAME_DIR .. "src/mame/sega/nl_segaspeech.cpp",
	MAME_DIR .. "src/mame/sega/nl_astrob.cpp",
	MAME_DIR .. "src/mame/sega/nl_elim.cpp",      -- not used
	MAME_DIR .. "src/mame/sega/nl_spacfury.cpp",  -- not used
	MAME_DIR .. "src/mame/sega/segaic16.cpp",
	MAME_DIR .. "src/mame/sega/segaic16_m.cpp",
	MAME_DIR .. "src/mame/sega/segaic16_road.cpp",
	MAME_DIR .. "src/mame/sega/sega16sp.cpp",
	MAME_DIR .. "src/hbmame/drivers/segas16b.cpp",
	MAME_DIR .. "src/mame/sega/segas16b_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/segas18.cpp",
	MAME_DIR .. "src/mame/sega/segas18.cpp",
	MAME_DIR .. "src/mame/sega/segas18_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/segas32.cpp",
	MAME_DIR .. "src/mame/sega/segas32_m.cpp",
	MAME_DIR .. "src/mame/sega/segas32_v.cpp",
	MAME_DIR .. "src/mame/sega/s32comm.cpp",
	MAME_DIR .. "src/mame/sega/m2comm.cpp", -- from here, for model2
	MAME_DIR .. "src/mame/sega/dsbz80.cpp",
	MAME_DIR .. "src/mame/sega/dsb2.cpp",
	MAME_DIR .. "src/hbmame/drivers/model2.cpp",
	MAME_DIR .. "src/mame/sega/model2rd.ipp",
	MAME_DIR .. "src/mame/sega/model2_m.cpp",
	MAME_DIR .. "src/mame/sega/model2_v.cpp",
	MAME_DIR .. "src/mame/shared/segam1audio.cpp",
	MAME_DIR .. "src/mame/sega/segaic24.cpp",
	MAME_DIR .. "src/mame/sega/315_5195.cpp",
	MAME_DIR .. "src/mame/sega/315_5338a.cpp",
	MAME_DIR .. "src/mame/sega/315_5649.cpp",
	MAME_DIR .. "src/mame/sega/315-5881_crypt.cpp",
	MAME_DIR .. "src/mame/sega/315-5838_317-0229_comp.cpp",
	MAME_DIR .. "src/mame/sega/model1io.cpp",
	MAME_DIR .. "src/hbmame/drivers/segahang.cpp",
	MAME_DIR .. "src/hbmame/drivers/segaxbd.cpp",
	MAME_DIR .. "src/hbmame/drivers/stv.cpp", -- from here, for stv
	MAME_DIR .. "src/mame/sega/saturn_m.cpp",
	MAME_DIR .. "src/mame/sega/saturn_v.cpp",
	MAME_DIR .. "src/mame/sega/saturn_scu.cpp",
	MAME_DIR .. "src/mame/sega/saturn_cd_hle.cpp",
	MAME_DIR .. "src/mame/sega/segabill.cpp",
	MAME_DIR .. "src/mame/sega/smpc.cpp",
	MAME_DIR .. "src/hbmame/drivers/model1.cpp", -- from here, for model1
	MAME_DIR .. "src/mame/sega/model1_m.cpp",
	MAME_DIR .. "src/mame/sega/model1_v.cpp",
	MAME_DIR .. "src/mame/sega/m1comm.cpp",
	MAME_DIR .. "src/mame/sega/model1io2.cpp",
	MAME_DIR .. "src/mame/sega/m50dass.cpp",
}

createHBMAMEProjects(_target, _subtarget, "seibu")
files {
	MAME_DIR .. "src/hbmame/drivers/bloodbro.cpp",
	MAME_DIR .. "src/hbmame/drivers/cabal.cpp",
	MAME_DIR .. "src/mame/seibu/seibu_crtc.cpp",
	MAME_DIR .. "src/mame/seibu/seibu_helper.cpp",
	MAME_DIR .. "src/hbmame/drivers/raiden2.cpp",
	MAME_DIR .. "src/mame/seibu/raiden2_v.cpp",
	MAME_DIR .. "src/mame/seibu/r2crypt.cpp",
	MAME_DIR .. "src/hbmame/drivers/r2dx_v33.cpp",
	MAME_DIR .. "src/mame/seibu/seibucop.cpp",
	MAME_DIR .. "src/mame/seibu/seibucop_dma.ipp",
	MAME_DIR .. "src/mame/seibu/seibucop_cmd.ipp",
	MAME_DIR .. "src/mame/seibu/seibuspi_m.cpp",
	MAME_DIR .. "src/hbmame/drivers/toki.cpp",
	MAME_DIR .. "src/hbmame/drivers/wiz.cpp",
	MAME_DIR .. "src/hbmame/drivers/mustache.cpp",
	MAME_DIR .. "src/mame/seibu/t5182.cpp",
	MAME_DIR .. "src/mame/seibu/sei80bu.cpp",
	MAME_DIR .. "src/mame/seibu/sei021x_sei0220_spr.cpp",
	MAME_DIR .. "src/mame/seibu/sei25x_rise1x_spr.cpp",
}

createHBMAMEProjects(_target, _subtarget, "seta")
files {
	MAME_DIR .. "src/mame/seta/st0016.cpp",
	MAME_DIR .. "src/hbmame/drivers/simple_st0016.cpp",
	MAME_DIR .. "src/hbmame/drivers/seta.cpp",
	MAME_DIR .. "src/hbmame/drivers/seta2.cpp",
	MAME_DIR .. "src/hbmame/drivers/ssv.cpp",
	MAME_DIR .. "src/mame/seta/ssv_v.cpp",
	MAME_DIR .. "src/mame/seta/st0020.cpp",
	MAME_DIR .. "src/mame/seta/x1_012.cpp",
}

createHBMAMEProjects(_target, _subtarget, "snk")
files {
	MAME_DIR .. "src/hbmame/drivers/snk6502.cpp",
	MAME_DIR .. "src/mame/snk/snk6502_a.cpp",
	MAME_DIR .. "src/mame/snk/snk6502_v.cpp",
}

createHBMAMEProjects(_target, _subtarget, "sony")
files {
	MAME_DIR .. "src/hbmame/drivers/zn.cpp",
	MAME_DIR .. "src/mame/sony/znmcu.cpp",
	MAME_DIR .. "src/mame/shared/rax.cpp",
	MAME_DIR .. "src/mame/sony/taito_zm.cpp",
}

createHBMAMEProjects(_target, _subtarget, "taito")
files {
	MAME_DIR .. "src/hbmame/drivers/kikikai.cpp",
	MAME_DIR .. "src/hbmame/drivers/lkage.cpp",
	MAME_DIR .. "src/hbmame/drivers/arkanoid.cpp",
	MAME_DIR .. "src/mame/taito/arkanoid_m.cpp",
	MAME_DIR .. "src/mame/taito/arkanoid_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/asuka.cpp",
	MAME_DIR .. "src/hbmame/drivers/bublbobl.cpp",
	MAME_DIR .. "src/mame/taito/bublbobl_m.cpp",
	MAME_DIR .. "src/mame/taito/bublbobl_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/flstory.cpp",
	MAME_DIR .. "src/hbmame/drivers/rastan.cpp",
	MAME_DIR .. "src/hbmame/drivers/retofinv.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_b.cpp",
	MAME_DIR .. "src/mame/taito/taito_b_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_f2.cpp",
	MAME_DIR .. "src/mame/taito/taito_f2_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_f3.cpp",
	MAME_DIR .. "src/mame/taito/taito_f3_v.cpp",
	MAME_DIR .. "src/mame/taito/taitocchip.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_z.cpp",
	MAME_DIR .. "src/mame/taito/taito_z_v.cpp",
	MAME_DIR .. "src/mame/taito/tc0150rod.cpp",   -- taito_z
	MAME_DIR .. "src/hbmame/drivers/volfied.cpp",
	MAME_DIR .. "src/mame/shared/taitosnd.cpp",
	MAME_DIR .. "src/mame/taito/taito_en.cpp",
	MAME_DIR .. "src/mame/taito/taitoio.cpp",
	MAME_DIR .. "src/mame/taito/taito_helper.cpp",
	MAME_DIR .. "src/hbmame/drivers/tnzs.cpp",
	MAME_DIR .. "src/mame/taito/tnzs_video.cpp",
	MAME_DIR .. "src/mame/taito/pc080sn.cpp",
	MAME_DIR .. "src/mame/taito/pc090oj.cpp",
	MAME_DIR .. "src/mame/taito/tc0100scn.cpp",
	MAME_DIR .. "src/mame/taito/tc0280grd.cpp",
	MAME_DIR .. "src/mame/taito/tc0360pri.cpp",
	MAME_DIR .. "src/mame/taito/tc0480scp.cpp",
	MAME_DIR .. "src/mame/taito/tc0110pcr.cpp",
	MAME_DIR .. "src/mame/taito/tc0180vcu.cpp",
	MAME_DIR .. "src/hbmame/drivers/taito_l.cpp",
	MAME_DIR .. "src/mame/shared/taito68705.cpp",
	MAME_DIR .. "src/mame/taito/taitosjsec.cpp",
	MAME_DIR .. "src/hbmame/drivers/taitosj.cpp",
	MAME_DIR .. "src/mame/taito/taitosj_m.cpp",
	MAME_DIR .. "src/mame/taito/taitosj_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/warriorb.cpp",
}

createHBMAMEProjects(_target, _subtarget, "technos")
files {
	MAME_DIR .. "src/hbmame/drivers/shadfrce.cpp",
	MAME_DIR .. "src/hbmame/drivers/ddragon.cpp",
	MAME_DIR .. "src/mame/technos/ddragon_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/ddragon3.cpp",
	MAME_DIR .. "src/mame/technos/ddragon3_v.cpp",
	MAME_DIR .. "src/hbmame/drivers/wwfsstar.cpp",
}

createHBMAMEProjects(_target, _subtarget, "tecmo")
files {
	MAME_DIR .. "src/hbmame/drivers/solomon.cpp",
	MAME_DIR .. "src/hbmame/drivers/tecmosys.cpp",
	MAME_DIR .. "src/mame/tecmo/tecmosys_m.cpp",
	MAME_DIR .. "src/mame/tecmo/tecmosys_v.cpp",
	MAME_DIR .. "src/mame/tecmo/tecmo_mix.cpp",
	MAME_DIR .. "src/hbmame/drivers/bombjack.cpp",
	MAME_DIR .. "src/hbmame/drivers/tecmo.cpp",
	MAME_DIR .. "src/hbmame/drivers/tecmo16.cpp",
	MAME_DIR .. "src/hbmame/drivers/tehkanwc.cpp",
	MAME_DIR .. "src/mame/tecmo/wc90.cpp",
	MAME_DIR .. "src/hbmame/drivers/wc90b.cpp",
	MAME_DIR .. "src/mame/shared/tecmo_spr.cpp",
}

createHBMAMEProjects(_target, _subtarget, "toaplan")
files {
	MAME_DIR .. "src/mame/toaplan/toaplan1.cpp",
	MAME_DIR .. "src/hbmame/drivers/batsugun.cpp",
	MAME_DIR .. "src/hbmame/drivers/pipibibi.cpp",
	MAME_DIR .. "src/hbmame/drivers/raizing.cpp",
	MAME_DIR .. "src/hbmame/drivers/snowbro2.cpp",
	MAME_DIR .. "src/hbmame/drivers/truxton2.cpp",
	MAME_DIR .. "src/hbmame/drivers/twincobr.cpp",
	MAME_DIR .. "src/mame/toaplan/twincobr_m.cpp",
	MAME_DIR .. "src/mame/toaplan/twincobr_v.cpp",
	MAME_DIR .. "src/mame/toaplan/wardner.cpp",
	MAME_DIR .. "src/mame/toaplan/gp9001.cpp",
	MAME_DIR .. "src/mame/toaplan/toaplan_dsp.cpp",
	MAME_DIR .. "src/mame/toaplan/toaplan_scu.cpp",
	MAME_DIR .. "src/hbmame/drivers/dt7.cpp",
	MAME_DIR .. "src/mame/toaplan/toaplan_coincounter.cpp",
	MAME_DIR .. "src/mame/toaplan/toaplan_txtilemap.cpp",
}

createHBMAMEProjects(_target, _subtarget, "unico")
files {
	MAME_DIR .. "src/hbmame/drivers/silkroad.cpp",
}

createHBMAMEProjects(_target, _subtarget, "universal")
files {
	MAME_DIR .. "src/hbmame/drivers/docastle.cpp",
	MAME_DIR .. "src/hbmame/drivers/mrdo.cpp",
	MAME_DIR .. "src/mame/universal/mrdo_v.cpp",
}

createHBMAMEProjects(_target, _subtarget, "valadon")
files {
	MAME_DIR .. "src/hbmame/drivers/bagman.cpp",
	MAME_DIR .. "src/mame/valadon/bagman_m.cpp",
	MAME_DIR .. "src/mame/valadon/bagman_v.cpp",
}

createHBMAMEProjects(_target, _subtarget, "vsystem")
files {
	MAME_DIR .. "src/mame/vsystem/vs9209.cpp",
	MAME_DIR .. "src/mame/vsystem/vsystem_gga.cpp",
	MAME_DIR .. "src/mame/vsystem/vsystem_spr.cpp",
	MAME_DIR .. "src/mame/vsystem/vsystem_spr2.cpp",
	MAME_DIR .. "src/hbmame/drivers/pspikes.cpp",
	MAME_DIR .. "src/hbmame/drivers/taotaido.cpp",
}

createHBMAMEProjects(_target, _subtarget, "misc")
files {
	MAME_DIR .. "src/hbmame/drivers/attackfc.cpp",
	MAME_DIR .. "src/hbmame/drivers/claybust.cpp",
	MAME_DIR .. "src/hbmame/drivers/dotrikun.cpp",
	MAME_DIR .. "src/hbmame/drivers/galactic.cpp",
	MAME_DIR .. "src/hbmame/drivers/schaser.cpp",
	MAME_DIR .. "src/hbmame/drivers/sflush.cpp",
	MAME_DIR .. "src/hbmame/drivers/skylove.cpp",
	MAME_DIR .. "src/hbmame/drivers/monaco.cpp",
	MAME_DIR .. "src/hbmame/drivers/kyugo.cpp",
	MAME_DIR .. "src/mame/orca/kyugo.cpp",
	MAME_DIR .. "src/hbmame/drivers/mcatadv.cpp",
	MAME_DIR .. "src/mame/misc/mcatadv.cpp",
	MAME_DIR .. "src/mame/ice/skimaxx.cpp",
	MAME_DIR .. "src/hbmame/drivers/timelimit.cpp",
	MAME_DIR .. "src/hbmame/drivers/vamphalf.cpp",
	MAME_DIR .. "src/mame/misc/vamphalf_prot.cpp",
	MAME_DIR .. "src/hbmame/drivers/hh_sm510.cpp",
	MAME_DIR .. "src/hbmame/drivers/pgm_hb.cpp",
}

createHBMAMEProjects(_target, _subtarget, "devices")
files {
	MAME_DIR .. "src/devices/bus/ata/hdd.cpp",
	MAME_DIR .. "src/devices/bus/pccard/ataflash.cpp",
--	MAME_DIR .. "src/devices/bus/rs232/rs232.cpp",
	MAME_DIR .. "src/devices/cpu/z80/mc8123.cpp",
	MAME_DIR .. "src/devices/machine/atahle.cpp",
	MAME_DIR .. "src/devices/machine/atastorage.cpp",
	MAME_DIR .. "src/devices/machine/cat702.cpp",
	MAME_DIR .. "src/devices/machine/ds2430a.cpp",
	MAME_DIR .. "src/devices/machine/mb87030.cpp",
	MAME_DIR .. "src/devices/machine/mos6530.cpp",
	MAME_DIR .. "src/devices/machine/nandflash.cpp",
	MAME_DIR .. "src/devices/machine/nmk112.cpp",
	MAME_DIR .. "src/devices/machine/segacrpt_device.cpp",
	MAME_DIR .. "src/devices/machine/segacrp2_device.cpp",
	MAME_DIR .. "src/devices/machine/z80dma.cpp",
	MAME_DIR .. "src/devices/sound/astrocde.cpp",
	MAME_DIR .. "src/devices/sound/ay8910.cpp",
	MAME_DIR .. "src/devices/sound/beep.cpp",
	MAME_DIR .. "src/devices/sound/bsmt2000.cpp",
	MAME_DIR .. "src/devices/sound/c140.cpp",
	MAME_DIR .. "src/devices/sound/c6280.cpp",
	MAME_DIR .. "src/devices/sound/cdda.cpp",
	MAME_DIR .. "src/devices/sound/dac.cpp",
	MAME_DIR .. "src/devices/sound/digitalk.cpp",
	MAME_DIR .. "src/devices/sound/discrete.cpp",
	MAME_DIR .. "src/devices/sound/dmadac.cpp",
	MAME_DIR .. "src/devices/sound/es5506.cpp",
	MAME_DIR .. "src/devices/sound/esqpump.cpp",
	MAME_DIR .. "src/devices/sound/gew.cpp",
	MAME_DIR .. "src/devices/sound/i5000.cpp",
	MAME_DIR .. "src/devices/sound/ics2115.cpp",
	MAME_DIR .. "src/devices/sound/iremga20.cpp",
	MAME_DIR .. "src/devices/sound/hc55516.cpp",
	MAME_DIR .. "src/devices/sound/k005289.cpp",
	MAME_DIR .. "src/devices/sound/k007232.cpp",
	MAME_DIR .. "src/devices/sound/k051649.cpp",
	MAME_DIR .. "src/devices/sound/k053260.cpp",
	MAME_DIR .. "src/devices/sound/k054321.cpp",
	MAME_DIR .. "src/devices/sound/k054539.cpp",
	MAME_DIR .. "src/devices/sound/k056800.cpp",
	MAME_DIR .. "src/devices/sound/lc7535.cpp",
	MAME_DIR .. "src/devices/sound/mb87077.cpp",
	MAME_DIR .. "src/devices/sound/msm5205.cpp",
	MAME_DIR .. "src/devices/sound/msm5232.cpp",
	MAME_DIR .. "src/devices/sound/mpeg_audio.cpp",
	MAME_DIR .. "src/devices/sound/multipcm.cpp",
	MAME_DIR .. "src/devices/sound/n63701x.cpp",
	MAME_DIR .. "src/devices/sound/namco.cpp",
	MAME_DIR .. "src/devices/sound/nes_apu.cpp",
	MAME_DIR .. "src/devices/sound/okiadpcm.cpp",
	MAME_DIR .. "src/devices/sound/okim6295.cpp",
	MAME_DIR .. "src/devices/sound/okim9810.cpp",
	MAME_DIR .. "src/devices/sound/pokey.cpp",
	MAME_DIR .. "src/devices/sound/qs1000.cpp",
	MAME_DIR .. "src/devices/sound/qsound.cpp",
	MAME_DIR .. "src/devices/sound/qsoundhle.cpp",
	MAME_DIR .. "src/devices/sound/rf5c68.cpp",
	MAME_DIR .. "src/devices/sound/scsp.cpp",
	MAME_DIR .. "src/devices/sound/scspdsp.cpp",
	MAME_DIR .. "src/devices/sound/s_dsp.cpp",
	MAME_DIR .. "src/devices/sound/segapcm.cpp",
	MAME_DIR .. "src/devices/sound/sn76477.cpp",
	MAME_DIR .. "src/devices/sound/sn76496.cpp",
	MAME_DIR .. "src/devices/sound/snkwave.cpp",
	MAME_DIR .. "src/devices/sound/sp0250.cpp",
	MAME_DIR .. "src/devices/sound/spkrdev.cpp",
	MAME_DIR .. "src/devices/sound/spu.cpp",
	MAME_DIR .. "src/devices/sound/spu_tables.cpp",
	MAME_DIR .. "src/devices/sound/spureverb.cpp",
	MAME_DIR .. "src/devices/sound/st0016.cpp",
	MAME_DIR .. "src/devices/sound/ta7630.cpp",
	MAME_DIR .. "src/devices/sound/tc8830f.cpp",
	MAME_DIR .. "src/devices/sound/tms36xx.cpp",
	MAME_DIR .. "src/devices/sound/tms5110.cpp",
	MAME_DIR .. "src/devices/sound/tms5220.cpp",
	MAME_DIR .. "src/devices/sound/upd7759.cpp",
	MAME_DIR .. "src/devices/sound/vlm5030.cpp",
	MAME_DIR .. "src/devices/sound/votrax.cpp",
	MAME_DIR .. "src/devices/sound/x1_010.cpp",
	MAME_DIR .. "src/devices/sound/ymf271.cpp",
	MAME_DIR .. "src/devices/sound/ymopl.cpp",
	MAME_DIR .. "src/devices/sound/ymopm.cpp",
	MAME_DIR .. "src/devices/sound/ymopn.cpp",
	MAME_DIR .. "src/devices/sound/ymz280b.cpp",
	MAME_DIR .. "src/devices/sound/ymz770.cpp",
	MAME_DIR .. "src/devices/sound/zsg2.cpp",
	MAME_DIR .. "src/devices/video/315_5124.cpp",
	MAME_DIR .. "src/devices/video/315_5313.cpp",
	MAME_DIR .. "src/devices/video/avgdvg.cpp",
	MAME_DIR .. "src/devices/video/bufsprite.cpp",
	MAME_DIR .. "src/devices/video/hd44780.cpp",
	MAME_DIR .. "src/devices/video/hd63484.cpp",
	MAME_DIR .. "src/devices/video/k053936.cpp",
	MAME_DIR .. "src/devices/video/mc6845.cpp",
	MAME_DIR .. "src/devices/video/ppu2c0x.cpp",
	MAME_DIR .. "src/devices/video/psx.cpp",
	MAME_DIR .. "src/devices/video/snes_ppu.cpp",
	MAME_DIR .. "src/devices/video/tlc34076.cpp",
	MAME_DIR .. "src/devices/video/tmap038.cpp",
	MAME_DIR .. "src/devices/video/tms34061.cpp",
	MAME_DIR .. "src/devices/video/x1_001.cpp",
	MAME_DIR .. "src/devices/video/x1_020_dx_101.cpp",
}

end

