// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/system1.cpp"

ROM_START( wbmlbit )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "wbml.01",      0x20000, 0x8000, CRC(66482638) SHA1(887f93015f0effa2d0fa1f1f59082f75ac072221) )
	ROM_CONTINUE(             0x00000, 0x8000 )
	ROM_LOAD( "wbml.02it",    0x30000, 0x8000, CRC(680f2186) SHA1(bb7b43a8d8438ec9dc55f219db284934a57ea81a) )
	ROM_CONTINUE(             0x10000, 0x8000 )
	ROM_LOAD( "wbml.03",      0x38000, 0x8000, CRC(d57ba8aa) SHA1(16f095cb78e31af5ce76d36c20fe4c3e0d027aea) )
	ROM_CONTINUE(             0x18000, 0x8000 )

	ROM_REGION( 0x2000, "maincpu:key", 0 ) /* MC8123 key */
	ROM_LOAD( "317-0043.key",  0x0000, 0x2000, CRC(e354abfc) SHA1(07b0d3c51301ebb25909234b6220a3ed20dbcc7d) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "epr-11037.126", 0x0000, 0x8000, CRC(7a4ee585) SHA1(050436106cced5dcbf40a3d94d48202eedddc3ad) )

	ROM_REGION( 0x18000, "tiles", 0 )
	ROM_LOAD( "wbml.08it",    0x00000, 0x8000, CRC(bc2846d3) SHA1(6777ef6ca5b92600aa6038d773cdd8263639b1cb) )
	ROM_LOAD( "wbml.09it",    0x08000, 0x8000, CRC(c61fd58e) SHA1(f17dba3195e8b3c3d443d696cce63604cdc64a5e) )
	ROM_LOAD( "wbml.10it",    0x10000, 0x8000, CRC(e0386c02) SHA1(bdfa5fa2bbf641730865d74057203e41dcae80fa) )

	ROM_REGION( 0x20000, "sprites", 0 )
	ROM_LOAD( "epr-11028.87",  0x00000, 0x8000, CRC(af0b3972) SHA1(413825f66b84c7e45aa1855131482abead8f7f3b) )
	ROM_LOAD( "epr-11027.86",  0x08000, 0x8000, CRC(277d8f1d) SHA1(7854673503ed03d276abe971805a11f8c992f6d6) )
	ROM_LOAD( "epr-11030.89",  0x10000, 0x8000, CRC(f05ffc76) SHA1(f3dbb518240f86430840c3d4cda04bac79c20f69) )
	ROM_LOAD( "epr-11029.88",  0x18000, 0x8000, CRC(cedc9c61) SHA1(dbe5744f9b6f2a406b52b910dd4e133db7bce6b2) )

	ROM_REGION( 0x0300, "color_proms", 0 )
	ROM_LOAD( "pr11026.20",   0x0000, 0x0100, CRC(27057298) SHA1(654be7abb937bb0720263ee6512e31194662effe) )
	ROM_LOAD( "pr11025.14",   0x0100, 0x0100, CRC(41e4d86b) SHA1(a86e8bb0a465d01b04410edfbb82eb96f12b909f) )
	ROM_LOAD( "pr11024.8",    0x0200, 0x0100, CRC(08d71954) SHA1(df045dbfb3d669e4d42fbdba1e7191cd046f7b47) )

	ROM_REGION( 0x0100, "lookup_proms", 0 )
	ROM_LOAD( "pr5317.37",    0x0000, 0x0100, CRC(648350b8) SHA1(c7986aa9127ef5b50b845434cb4e81dff9861cd2) )
ROM_END

ROM_START( wbmlkb )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "wbml.01",      0x20000, 0x8000, CRC(66482638) SHA1(887f93015f0effa2d0fa1f1f59082f75ac072221) )
	ROM_CONTINUE(             0x00000, 0x8000 )
	ROM_LOAD( "m-6k.bin",     0x30000, 0x8000, CRC(7dd0b45a) SHA1(ae715437c225013bade54cf021be708c5145a84f) )
	ROM_CONTINUE(             0x10000, 0x8000 )
	ROM_LOAD( "m-7.bin",      0x38000, 0x8000, CRC(11881703) SHA1(b5e4d477158e7653b0fef5a4806be7b4871e917d) )
	ROM_CONTINUE(             0x18000, 0x8000 )

	ROM_REGION( 0x2000, "maincpu:key", 0 ) /* MC8123 key */
	ROM_LOAD( "317-0043.key",  0x0000, 0x2000, CRC(e354abfc) SHA1(07b0d3c51301ebb25909234b6220a3ed20dbcc7d) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "epr-11037.126", 0x0000, 0x8000, CRC(7a4ee585) SHA1(050436106cced5dcbf40a3d94d48202eedddc3ad) )

	ROM_REGION( 0x18000, "tiles", 0 )
	ROM_LOAD( "epr11034.4k",  0x00000, 0x8000, CRC(7e631c73) SHA1(ef7bc9c8b15083680f8169fa8d322cfbfa7d54ac) )
	ROM_LOAD( "epr11035.5k",  0x08000, 0x8000, CRC(4c67a9a6) SHA1(eac1f97aac3f5b0ce96dd082c7741033caa4b55b) )
	ROM_LOAD( "epr11036.6k",  0x10000, 0x8000, CRC(0440b0c7) SHA1(f0e321dc6c979807852c375c8a6905fd32336dde) )

	ROM_REGION( 0x20000, "sprites", 0 )
	ROM_LOAD( "epr-11028.87",  0x00000, 0x8000, CRC(af0b3972) SHA1(413825f66b84c7e45aa1855131482abead8f7f3b) )
	ROM_LOAD( "epr-11027.86",  0x08000, 0x8000, CRC(277d8f1d) SHA1(7854673503ed03d276abe971805a11f8c992f6d6) )
	ROM_LOAD( "epr-11030.89",  0x10000, 0x8000, CRC(f05ffc76) SHA1(f3dbb518240f86430840c3d4cda04bac79c20f69) )
	ROM_LOAD( "epr-11029.88",  0x18000, 0x8000, CRC(cedc9c61) SHA1(dbe5744f9b6f2a406b52b910dd4e133db7bce6b2) )

	ROM_REGION( 0x0300, "color_proms", 0 )
	ROM_LOAD( "pr11026.20",   0x0000, 0x0100, CRC(27057298) SHA1(654be7abb937bb0720263ee6512e31194662effe) )
	ROM_LOAD( "pr11025.14",   0x0100, 0x0100, CRC(41e4d86b) SHA1(a86e8bb0a465d01b04410edfbb82eb96f12b909f) )
	ROM_LOAD( "pr11024.8",    0x0200, 0x0100, CRC(08d71954) SHA1(df045dbfb3d669e4d42fbdba1e7191cd046f7b47) )

	ROM_REGION( 0x0100, "lookup_proms", 0 )
	ROM_LOAD( "pr5317.37",    0x0000, 0x0100, CRC(648350b8) SHA1(c7986aa9127ef5b50b845434cb4e81dff9861cd2) )
ROM_END

GAME( 1987, wbmlbit, wbml, sys2xb, wbml, system1_state, init_bootsys2, ROT0, "Michele", "Wonder Boy in Monster Land (Italian - v0.99)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
GAME( 1987, wbmlkb,  wbml, sys2xb, wbml, system1_state, init_bootsys2, ROT0, "Aneue Bannzai", "Wonder Boy in Monster Land (Translation Korean)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )


// LOST ROMS - raflsiau - this was included in various versions of MAME (this code from 0.61), but the story goes that the
//                        dumper's hard drive crashed before the roms could be sent to Mamedev, and thus lost forever.
//                        The only changed roms are those in "maincpu" - the remainder is identical to raflesia

ROM_START( raflsiau )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "epr-7433.129",   0x0000, 0x2000, CRC(6f4931b0 ) )
	ROM_LOAD( "epr-7434.130",   0x2000, 0x2000, CRC(ec46e21b ) )
	ROM_LOAD( "epr-7435.131",   0x4000, 0x2000, CRC(e035ff6b ) )
	ROM_LOAD( "epr-7436.132",   0x6000, 0x2000, CRC(6527aae7 ) )
	ROM_LOAD( "epr-7437.133",   0x8000, 0x2000, CRC(e13dd5e4 ) )
	ROM_LOAD( "epr-7438.134",   0xa000, 0x2000, CRC(a0aa4729 ) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "epr-7420.120",   0x0000, 0x2000, CRC(14387666) SHA1(9cb18e3002c32f658e4725707069f9cd2f496507) )

	ROM_REGION( 0xc000, "tiles", 0 )
	ROM_LOAD( "epr-7419.62",    0x0000, 0x2000, CRC(bfd5f34c) SHA1(78c4d380d5558212e535c3262223137447d64818) )
	ROM_LOAD( "epr-7418.61",    0x2000, 0x2000, CRC(f8cbc9b6) SHA1(48be9337f704a11ac1fdeb64a3b3518c796bcdd0) )
	ROM_LOAD( "epr-7417.64",    0x4000, 0x2000, CRC(e63501bc) SHA1(5cfd19241c54782c262bbb23c6f682534e77feb7) )
	ROM_LOAD( "epr-7416.63",    0x6000, 0x2000, CRC(093e5693) SHA1(78bb1c4651bd63a9f776766d2eac4f1c09242ed5) )
	ROM_LOAD( "epr-7415.66",    0x8000, 0x2000, CRC(1a8d6bd6) SHA1(b04ee35f603c6c9923ba888914eb43a8b7753d92) )
	ROM_LOAD( "epr-7414.65",    0xa000, 0x2000, CRC(5d20f218) SHA1(bdc0185d133f7bbe287106882bacde846634ffa4) )

	ROM_REGION( 0x10000, "sprites", 0 )
	ROM_LOAD( "epr-7407.117",   0x0000, 0x4000, CRC(f09fc057) SHA1(c6f06144b708055b31fbcba9f38b63736db789d8) )
	ROM_LOAD( "epr-7409.04",    0x4000, 0x4000, CRC(819fedb8) SHA1(e63f0422814423be91d8e1937a13d19693a1a5fc) )
	ROM_LOAD( "epr-7408.110",   0x8000, 0x4000, CRC(3189f33c) SHA1(8476c2c01920f0492cf643929d4f023f3afe0164) )
	ROM_LOAD( "epr-7410.05",    0xc000, 0x4000, CRC(ced74789) SHA1(d0ad845bfe83412ac8d43125e1c50d0581a5b47e) )

	ROM_REGION( 0x0100, "lookup_proms", 0 )
	ROM_LOAD( "pr-5317.76",     0x0000, 0x0100, CRC(648350b8) SHA1(c7986aa9127ef5b50b845434cb4e81dff9861cd2) )
ROM_END

// machine config is guess
//    YEAR   NAME      PARENT     MACHINE CONFIG    INPUTS      CLASS          INIT
GAME( 1986, raflsiau, raflesia, sys1pio,           raflesia,  system1_state, init_bank00, ROT270, "Coreland / Sega", "Rafflesia (not encrypted)", MACHINE_SUPPORTS_SAVE )

