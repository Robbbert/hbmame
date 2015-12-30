// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/system1.cpp"

ROM_START( wbmlbit )
	ROM_REGION( 0x40000, "maincpu", 0 ) /* 256k for code */
	ROM_LOAD( "wbml.01",      0x20000, 0x8000, CRC(66482638) SHA1(887f93015f0effa2d0fa1f1f59082f75ac072221) ) /* Unencrypted opcodes */
	ROM_CONTINUE(             0x00000, 0x8000 )             /* Now load the operands in RAM */
	ROM_LOAD( "wbml.02it",    0x30000, 0x8000, CRC(680f2186) SHA1(bb7b43a8d8438ec9dc55f219db284934a57ea81a) ) /* Unencrypted opcodes */
	ROM_CONTINUE(             0x10000, 0x8000 )
	ROM_LOAD( "wbml.03",      0x38000, 0x8000, CRC(d57ba8aa) SHA1(16f095cb78e31af5ce76d36c20fe4c3e0d027aea) ) /* Unencrypted opcodes */
	ROM_CONTINUE(             0x18000, 0x8000 )

	ROM_REGION( 0x10000, "soundcpu", 0 ) /* 64k for sound cpu */
	ROM_LOAD( "epr11037.126", 0x0000, 0x8000, CRC(7a4ee585) SHA1(050436106cced5dcbf40a3d94d48202eedddc3ad) )

	ROM_REGION( 0x18000, "tiles", 0 )
	ROM_LOAD( "wbml.08it",    0x00000, 0x8000, CRC(bc2846d3) SHA1(6777ef6ca5b92600aa6038d773cdd8263639b1cb) )
	ROM_LOAD( "wbml.09it",    0x08000, 0x8000, CRC(c61fd58e) SHA1(f17dba3195e8b3c3d443d696cce63604cdc64a5e) )
	ROM_LOAD( "wbml.10it",    0x10000, 0x8000, CRC(e0386c02) SHA1(bdfa5fa2bbf641730865d74057203e41dcae80fa) )

	ROM_REGION( 0x20000, "sprites", 0 ) /* 128k for sprites data */
	ROM_LOAD( "epr11028.87",  0x00000, 0x8000, CRC(af0b3972) SHA1(413825f66b84c7e45aa1855131482abead8f7f3b) )
	ROM_LOAD( "epr11027.86",  0x08000, 0x8000, CRC(277d8f1d) SHA1(7854673503ed03d276abe971805a11f8c992f6d6) )
	ROM_LOAD( "epr11030.89",  0x10000, 0x8000, CRC(f05ffc76) SHA1(f3dbb518240f86430840c3d4cda04bac79c20f69) )
	ROM_LOAD( "epr11029.88",  0x18000, 0x8000, CRC(cedc9c61) SHA1(dbe5744f9b6f2a406b52b910dd4e133db7bce6b2) )

	ROM_REGION( 0x0300, "palette", 0 )
	ROM_LOAD( "pr11026.20",   0x0000, 0x0100, CRC(27057298) SHA1(654be7abb937bb0720263ee6512e31194662effe) )
	ROM_LOAD( "pr11025.14",   0x0100, 0x0100, CRC(41e4d86b) SHA1(a86e8bb0a465d01b04410edfbb82eb96f12b909f) )
	ROM_LOAD( "pr11024.8",    0x0200, 0x0100, CRC(08d71954) SHA1(df045dbfb3d669e4d42fbdba1e7191cd046f7b47) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "pr5317.37",    0x0000, 0x0100, CRC(648350b8) SHA1(c7986aa9127ef5b50b845434cb4e81dff9861cd2) )
ROM_END

ROM_START( wbmlkb )
	ROM_REGION( 0x40000, "maincpu", 0 ) /* 256k for code */
	ROM_LOAD( "wbml.01",      0x20000, 0x8000, CRC(66482638) SHA1(887f93015f0effa2d0fa1f1f59082f75ac072221) ) /* Unencrypted opcodes */
	ROM_CONTINUE(             0x00000, 0x8000 )             /* Now load the operands in RAM */
	ROM_LOAD( "m-6k.bin",     0x30000, 0x8000, CRC(7dd0b45a) SHA1(ae715437c225013bade54cf021be708c5145a84f) ) /* Unencrypted opcodes */
	ROM_CONTINUE(             0x10000, 0x8000 )
	ROM_LOAD( "m-7.bin",      0x38000, 0x8000, CRC(11881703) SHA1(b5e4d477158e7653b0fef5a4806be7b4871e917d) ) /* Unencrypted opcodes */
	ROM_CONTINUE(             0x18000, 0x8000 )

	ROM_REGION( 0x10000, "soundcpu", 0 ) /* 64k for sound cpu */
	ROM_LOAD( "epr11037.126", 0x0000, 0x8000, CRC(7a4ee585) SHA1(050436106cced5dcbf40a3d94d48202eedddc3ad) )

	ROM_REGION( 0x18000, "tiles", 0 )
	ROM_LOAD( "epr11034.4k",  0x00000, 0x8000, CRC(7e631c73) SHA1(ef7bc9c8b15083680f8169fa8d322cfbfa7d54ac) )
	ROM_LOAD( "epr11035.5k",  0x08000, 0x8000, CRC(4c67a9a6) SHA1(eac1f97aac3f5b0ce96dd082c7741033caa4b55b) )
	ROM_LOAD( "epr11036.6k",  0x10000, 0x8000, CRC(0440b0c7) SHA1(f0e321dc6c979807852c375c8a6905fd32336dde) )

	ROM_REGION( 0x20000, "sprites", 0 ) /* 128k for sprites data */
	ROM_LOAD( "epr11028.87",  0x00000, 0x8000, CRC(af0b3972) SHA1(413825f66b84c7e45aa1855131482abead8f7f3b) )
	ROM_LOAD( "epr11027.86",  0x08000, 0x8000, CRC(277d8f1d) SHA1(7854673503ed03d276abe971805a11f8c992f6d6) )
	ROM_LOAD( "epr11030.89",  0x10000, 0x8000, CRC(f05ffc76) SHA1(f3dbb518240f86430840c3d4cda04bac79c20f69) )
	ROM_LOAD( "epr11029.88",  0x18000, 0x8000, CRC(cedc9c61) SHA1(dbe5744f9b6f2a406b52b910dd4e133db7bce6b2) )

	ROM_REGION( 0x0300, "palette", 0 )
	ROM_LOAD( "pr11026.20",   0x0000, 0x0100, CRC(27057298) SHA1(654be7abb937bb0720263ee6512e31194662effe) )
	ROM_LOAD( "pr11025.14",   0x0100, 0x0100, CRC(41e4d86b) SHA1(a86e8bb0a465d01b04410edfbb82eb96f12b909f) )
	ROM_LOAD( "pr11024.8",    0x0200, 0x0100, CRC(08d71954) SHA1(df045dbfb3d669e4d42fbdba1e7191cd046f7b47) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "pr5317.37",    0x0000, 0x0100, CRC(648350b8) SHA1(c7986aa9127ef5b50b845434cb4e81dff9861cd2) )
ROM_END

GAME( 1987, wbmlbit, wbml, sys2, wbml, system1_state, bootsys2, ROT0, "Michele", "Wonder Boy in Monster Land (Italian - v0.99)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
GAME( 1987, wbmlkb,  wbml, sys2, wbml, system1_state, bootsys2, ROT0, "hack", "Wonder Boy in Monster Land (Korean)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
