// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/nintendo/vsnes.cpp"

ROM_START( mrio2002 )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "mds-sm4-4__1dor6d_e.1d or 6d",  0x0000, 0x2000, CRC(be4d5436) SHA1(08162a7c987f1939d09bebdb676f596c86abf465) )
	ROM_LOAD( "mds-sm4.1c",                    0x2000, 0x2000, CRC(0011fc5a) SHA1(5c2c49938a12affc03e64e5bdab307998be20020) )
	ROM_LOAD( "mds-sm4-4__1bor6b_e.1b or 6b",  0x4000, 0x2000, CRC(b1b87893) SHA1(8563ceaca664cf4495ef1020c07179ca7e4af9f3) )
	ROM_LOAD( "mds-sm4-4__1aor6a_e.1a or 6a",  0x6000, 0x2000, CRC(1abf053c) SHA1(f17db88ce0c9bf1ed88dc16b9650f11d10835cec) )

	ROM_REGION( 0x4000, "gfx1", 0  )
	ROM_LOAD( "mrio2002.2b",                   0x0000, 0x2000, CRC(1feda640) SHA1(f26be31f43dacdaa9d8bffa75f4fcd9d8d04953a) )
	ROM_LOAD( "mds-sm4-4__2aor8a_e.2a or 8a",  0x2000, 0x2000, CRC(15506b86) SHA1(69ecf7a3cc8bf719c1581ec7c0d68798817d416f) )

	PALETTE_2C04_0004("ppu1:palette")
ROM_END

ROM_START( suprsktr )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "mds-sm4-4__1dor6d_e.1d or 6d",  0x0000, 0x2000, CRC(be4d5436) SHA1(08162a7c987f1939d09bebdb676f596c86abf465) )
	ROM_LOAD( "mds-sm4.1c",                    0x2000, 0x2000, CRC(0011fc5a) SHA1(5c2c49938a12affc03e64e5bdab307998be20020) )
	ROM_LOAD( "mds-sm4-4__1bor6b_e.1b or 6b",  0x4000, 0x2000, CRC(b1b87893) SHA1(8563ceaca664cf4495ef1020c07179ca7e4af9f3) )
	ROM_LOAD( "mds-sm4-4__1aor6a_e.1a or 6a",  0x6000, 0x2000, CRC(1abf053c) SHA1(f17db88ce0c9bf1ed88dc16b9650f11d10835cec) )

	ROM_REGION( 0x4000, "gfx1", 0  )
	ROM_LOAD( "suprsktr.2b",                   0x0000, 0x2000, CRC(f3980303) SHA1(b9a25c906d1861c89e2e40e878a34d318daf6619) )
	ROM_LOAD( "mds-sm4-4__2aor8a_e.2a or 8a",  0x2000, 0x2000, CRC(15506b86) SHA1(69ecf7a3cc8bf719c1581ec7c0d68798817d416f) )

	PALETTE_2C04_0004("ppu1:palette")
ROM_END

ROM_START( drmario01 )
	ROM_REGION( 0x10000, "prg", 0 )
	ROM_LOAD( "dm_s01-uiprg",                  0x00000, 0x10000, CRC(a0c56a2a) SHA1(0aeb5ff1f8c6308f723e88003ea6282914d22121) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "dm_s01-u3chr",                  0x0000, 0x8000, CRC(ac94c651) SHA1(9ac7c97501d915c6f0041de3be421423f5de0448) )

	PALETTE_2C04_0003("ppu1:palette")
ROM_END

ROM_START( drluigi )
	ROM_REGION( 0x10000, "prg", 0 )
	ROM_LOAD( "dl-uiprg",                  0x00000, 0x10000, CRC(bcef3c87) SHA1(797f09e3c7b07a1b0828d44b0302a81a9d62a0e8) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "dl-u3chr",                  0x0000, 0x8000, CRC(07f8d222) SHA1(f6a0e418ab6da29eda25d0b871df072c16ded4fe) )

	ROM_REGION( 0xc0, "ppu1:palette", 0 )
	ROM_LOAD( "dl_rp2c04-0003.pal", 0x00, 0xc0, CRC(c40eea5f) SHA1(2b4a03f709441e8d4344d6e39060b8d9a376074f) )
ROM_END

static INPUT_PORTS_START( frombelow )
	PORT_INCLUDE( vsnes_rev )

	PORT_START("DSW0")
	PORT_DIPNAME( 0x01, 0x00, "Free Play" ) PORT_DIPLOCATION("SW1:!1")
	PORT_DIPSETTING(    0x00, "Off" )
	PORT_DIPSETTING(    0x01, "On" )
	PORT_DIPNAME( 0x02, 0x00, DEF_STR( Coinage ) )      PORT_DIPLOCATION("SW1:!2")
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPNAME( 0x1c, 0x00, "PPU Type" )          PORT_DIPLOCATION("SW1:!3,!4,!5")
	PORT_DIPSETTING(    0x00, "RP2C04-0001" )
	PORT_DIPSETTING(    0x04, "RP2C04-0002" )
	PORT_DIPSETTING(    0x08, "RP2C04-0003" )
	PORT_DIPSETTING(    0x0c, "RP2C04-0004" )
	PORT_DIPSETTING(    0x10, "2C03/2C05" )
	PORT_DIPSETTING(    0x14, "2C02 (NES)" )
	PORT_DIPNAME( 0x20, 0x00, "Disable Music" )  PORT_DIPLOCATION("SW1:!6")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x20, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, "Disable SFX" )  PORT_DIPLOCATION("SW1:!7")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x00, "Disable Demo Sounds" )  PORT_DIPLOCATION("SW1:!8")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x80, DEF_STR( On ) )
INPUT_PORTS_END

ROM_START( frombelow ) // vs.frombelowgame.com
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "from_below_vs_2020_12_21_v_0_8_0.nes.prg.1a.bin", 0x6000, 0x2000, CRC(37445c70) SHA1(494cb76e5d79a86da1f584d7cd71790f657de82c) )
	ROM_LOAD( "from_below_vs_2020_12_21_v_0_8_0.nes.prg.1b.bin", 0x4000, 0x2000, CRC(d991120e) SHA1(bbcb7c069d90124f61b0aecece438aed5f50996d) )
	ROM_LOAD( "from_below_vs_2020_12_21_v_0_8_0.nes.prg.1c.bin", 0x2000, 0x2000, CRC(345ae82e) SHA1(531ceb32bed3e641aef5c875c9cca540944e77cc) )
	ROM_LOAD( "from_below_vs_2020_12_21_v_0_8_0.nes.prg.1d.bin", 0x0000, 0x2000, CRC(1aebf43b) SHA1(45e5aa6a8a7df503c1488acf6bd406aed17075af) )

	ROM_REGION( 0x4000, "gfx1", 0  )
	ROM_LOAD( "from_below_vs_2020_12_21_v_0_8_0.nes.chr.2b.bin", 0x0000, 0x2000, CRC(7f2e1b5b) SHA1(949af8437d6789c1b1e60a242bc1e1da2d07b468) )

	PALETTE_2C04_0001("ppu1:palette")
ROM_END

static INPUT_PORTS_START( vs_urban )
	PORT_INCLUDE( vsnes_rev )

	PORT_START("DSW0")  // bit 0 and 1 read from bit 3 and 4 on $4016, rest of the bits read on $4017
	PORT_DIPNAME( 0x07, 0x00, DEF_STR( Coinage ) )    PORT_DIPLOCATION("SW1:!1,!2,!3")
	PORT_DIPSETTING(    0x03, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x05, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0x07, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x18, 0x00, DEF_STR( Difficulty ) ) PORT_DIPLOCATION("SW1:!4,!5")
	PORT_DIPSETTING(    0x00, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Medium ) )
	PORT_DIPSETTING(    0x18, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x60, 0x00, DEF_STR( Lives ) )      PORT_DIPLOCATION("SW1:!6,!7")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x40, "4" )
	PORT_DIPSETTING(    0x20, "5" )
	PORT_DIPSETTING(    0x60, "6" )
	PORT_DIPUNKNOWN_DIPLOC( 0x80, 0x00, "SW1:!8" )
INPUT_PORTS_END

ROM_START( vs_urban )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "urb.1a", 0x6000, 0x2000, CRC(7ccff543) SHA1(9f369b2b55482019d6f4d0c19ede54a60788e4a3) )
	ROM_LOAD( "urb.1b", 0x4000, 0x2000, CRC(82b79f28) SHA1(5ccda9a889924e60f329caaa927647347ccfb7e7) )
	ROM_LOAD( "urb.1c", 0x2000, 0x2000, CRC(de14ac45) SHA1(8a8f4ebbbd7584dcfaf291a6acbca9435b599a17) )
	ROM_LOAD( "urb.1d", 0x0000, 0x2000, CRC(b31a2c0c) SHA1(4fb747b71bc308789f0d6b17d2e6a1a833102a36) )

	ROM_REGION( 0x4000, "gfx1", 0  )
	ROM_LOAD( "urb.2b", 0x0000, 0x2000, CRC(ddb9a263) SHA1(7309276e29c0073bc49ad9dc77bb6cacbca7270a) )
	ROM_LOAD( "urb.2a", 0x2000, 0x2000, CRC(329b8b6d) SHA1(ae11f8d84327fcde0d80e2cc951be9648d4e4b10) )

	PALETTE_2C04_0003("ppu1:palette")
ROM_END

ROM_START( mightybj01 )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "mbj01.1d",  0x0000, 0x2000, CRC(cbb4bcd0) SHA1(d02d8289b1c9232db454000638d60743b9e5783a) )
	ROM_LOAD( "mbj01.1c",  0x2000, 0x2000, CRC(336b71d9) SHA1(e9a9079e5040b79e418c0364c401a2fa37aefb4f) )
	ROM_LOAD( "mbj01.1b",  0x4000, 0x2000, CRC(9501ccf0) SHA1(1f2675898c939e79236e4ec300fab7fc060058ff) )
	ROM_LOAD( "mbj01.1a",  0x6000, 0x2000, CRC(25832a5b) SHA1(dda726855c6446a7f4f8c40363f14e9fb3b9e452) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mbj01.2b",  0x0000, 0x2000, CRC(f98394b3) SHA1(6e274cd015fe5304df02a79bd0dec55658f64e50) )

	PALETTE_STANDARD("ppu1:palette")
ROM_END

ROM_START( smgolf01 )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "mds-gf4-2 f.1d or 6d",  0x0000, 0x2000, CRC(4a723087) SHA1(87dc063d295f55871598a1e1eb4e62ce298b2f0c) )
	ROM_LOAD( "mds-gf4-2 f.1c.s01",  0x2000, 0x2000, CRC(b8634d7e) SHA1(3095a432d5ebfb864ad0ccf18ae562ad83aab663) )
	ROM_LOAD( "mds-gf4-2 f.1b or 6b",  0x4000, 0x2000, CRC(6783652f) SHA1(7165ee59d3787cb56eed4791351da07f4bcc68ed) )
	ROM_LOAD( "mds-gf4-2 f.1a or 6a",  0x6000, 0x2000, CRC(bfc17263) SHA1(9e3b46fe08be893935138247ed3168d19d55312e) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "mds-gf4-2 f.2b or 8b",  0x0000, 0x2000, CRC(2782a3e5) SHA1(4e57aab58fb250da951a9aecd21d6aa79e697bcb) )
	ROM_LOAD( "mds-gf4-2 f.2a or 8a",  0x2000, 0x2000, CRC(6e93fdef) SHA1(44f46421adabbc40135c681592cb5226b7c9012a) )

	PALETTE_2C04_0002("ppu1:palette")
ROM_END

ROM_START( ladygolfe01 )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "mds-lg4 e.1d or 6d", 0x0000, 0x2000, CRC(408ea247) SHA1(48cbcf9b87e522b20599bcf2b61df81bfd55db7b) )
	ROM_LOAD( "mds-lg4 e.1c.s01", 0x2000, 0x2000,  CRC(824a9d05) SHA1(33639da3df62aa462c4e49d16bcb8eff307e9e56) )
	ROM_LOAD( "mds-lg4 e.1b or 6b", 0x4000, 0x2000, CRC(67f40126) SHA1(891c2bc2b6e1ee1d58baea22714133cae6e38b8d) )
	ROM_LOAD( "mds-lg4 e.1a or 6a", 0x6000, 0x2000, CRC(44393845) SHA1(5c36dcb115d4233a1e03faa28e95d2662953ca91) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "mds-lg4 e.2b or 8b", 0x0000, 0x2000, CRC(95618947) SHA1(e8f09bffa3fa1a1cac8fa25df9fba962951c1fb3) )
	ROM_LOAD( "mds-lg4 e.2a or 8a", 0x2000, 0x2000, CRC(d07407b1) SHA1(b998b46fe83e76fac3d7b71495d1da8580a731f9) )

	PALETTE_2C04_0002("ppu1:palette")
ROM_END

ROM_START( ladygolf01 )
	ROM_REGION( 0x8000, "prg", 0 )
	ROM_LOAD( "lg-1d",  0x0000, 0x2000, CRC(8b2ab436) SHA1(145a75f30f1fab5b1babf01ada9ed23f59c2c18d) )
	ROM_LOAD( "lg-1c.s01",  0x2000, 0x2000, CRC(4acd65c4) SHA1(02c6e1a4491e25f1e7daa962906bc76fc076619c) )
	ROM_LOAD( "lg-1b",  0x4000, 0x2000, CRC(dcdd8220) SHA1(563028f8db9ad221d8ac8f8096b4587b822eedb7) )
	ROM_LOAD( "lg-1a",  0x6000, 0x2000, CRC(26a3cb3b) SHA1(00131637eb76154c4f04eb54707e0e7b453d4580) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "mds-lg4 e.2b or 8b", 0x0000, 0x2000, CRC(95618947) SHA1(e8f09bffa3fa1a1cac8fa25df9fba962951c1fb3) )
	ROM_LOAD( "mds-lg4 e.2a or 8a", 0x2000, 0x2000, CRC(d07407b1) SHA1(b998b46fe83e76fac3d7b71495d1da8580a731f9) )

	PALETTE_2C04_0002("ppu1:palette")
ROM_END


GAME( 2002, mrio2002,    suprmrio,     vsnes,    suprmrio,  vs_uni_state, init_vsnormal, ROT0, "Nintendo",  "Vs. Super Mario Bros. 2002", 0 )
GAME( 2025, mightybj01,  nvs_mightybj, mightybj, mightybj,  vs_uni_state, init_vsnormal, ROT0, "Vag", "Mighty Bomb Jack (Greek)", 0 )
GAME( 1986, suprsktr,    suprmrio,     vsnes,    suprmrio,  vs_uni_state, init_vsnormal, ROT0, "Nintendo",  "Vs. Super Skater Bros.", 0 )
GAME( 2002, drmario01,   drmario,      vsnes,    drmario,   vs_uni_state, init_drmario,  ROT0, "liujunusa", "Vs. Dr. Mario (Chinese, 2002-10)", 0 )
GAME( 2019, drluigi,     drmario,      vsnes,    drmario,   vs_uni_state, init_drmario,  ROT0, "Szemigi", "Dr. Luigi", 0 )
GAME( 2020, frombelow,   suprmrio,     vsnes,    frombelow, vs_uni_state, init_vsnormal, ROT0, "Matt Hughson", "Vs. From Below (beta 0.8.0, 2020-12-21)", 0 )
GAME( 2020, vs_urban,    0,            vsnes,    vs_urban,  vs_uni_state, init_vsnormal, ROT0, "Nintendo", "Vs. Urban Champion", 0 )
GAME( 1984, smgolf01,    0,            vsnes,    golf,      vs_uni_state, init_vsnormal, ROT0, "rhodesmichael03", "Vs. Stroke & Match Golf (Men Version, set GF4-2 F, Easier CPU, v1.0, 2026-09-26)",       0 )
GAME( 1984, ladygolfe01, smgolf01,     vsnes,    golf4s,    vs_uni_state, init_vsnormal, ROT0, "rhodesmichael03", "Vs. Stroke & Match Golf (Ladies Version, set LG4 E, Easier CPU, v1.0, 2026-09-26)",      0 )
GAME( 1984, ladygolf01,  smgolf01,     vsnes,    golf,      vs_uni_state, init_vsnormal, ROT0, "rhodesmichael03", "Vs. Stroke & Match Golf (Ladies Version, set LG4 ?, Easier CPU, v1.0, 2026-09-26)",      0 )

