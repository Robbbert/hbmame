// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/playch10.cpp"

ROM_START( pc_ark )	/* Arkanoid (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "arkanoid-u1", 0x08000, 0x8000, CRC(95dbb274) SHA1(792420b622b59d40ec0a264bf0bcd5faf9b652dc) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "arkanoid-u2", 0x00000, 0x2000, CRC(bb8dae70) SHA1(91bdab01839e2ad3d9aaa9041e0855ba26583a9f) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_bb2 ) /* Bubble Bobble Part 2 (Power Blade) */
	BIOS_CPU
	ROM_LOAD( "bb2-u3", 0x0c000, 0x2000, CRC(edcc21c6) SHA1(5d73c6a747cfe951dc7c6ddfbb29859e9548aded) ) /* extra bios code for this game */
	BIOS_GFX
	
	ROM_REGION( 0x50000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "bb2-u5", 0x10000, 0x20000, CRC(5f7c1d17) SHA1(48608c9ebd8c0feb9e1126eb4070f185c41d9a19) ) /* banked */
	ROM_RELOAD( 0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "bb2-u1", 0x00000, 0x20000, CRC(5e59afae) SHA1(5ed7d7c1ed50dbdc01e0a80e6ed5a246349726aa) )
	
	ROM_REGION( 0x0100, "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "31a05a48.prm", 0x00000, 0x10, CRC(31a05a48) SHA1(8b340600feae03bb5cdab852a9879ecffcc8a2b9) )
ROM_END

ROM_START( pc_cch )	/* Circus Charlie (Golf) */
	BIOS_CPU
	ROM_LOAD( "gf-u3",   0x0c000, 0x2000, CRC(882dea87) SHA1(e3bbca36efa66231b933713dec032bbb926b36e5) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "circusch.u1", 0x0c000, 0x4000, CRC(2cb269d5) SHA1(28a700b4fa5c580adf3ef9891506585ef6a40725) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "circusch.u2", 0x00000, 0x2000, CRC(f76c592a) SHA1(c8bc574c43d26d5072869922573edaaab7a37050) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "2cd98ef6.prm", 0x00000, 0x10, CRC(2cd98ef6) SHA1(bd5142c6a29df674ab835c8beafff7e93712d88f) )
ROM_END

ROM_START( pc_ctfrc ) /* Contra Force (Power Blade) */
	BIOS_CPU
	ROM_LOAD( "ctfrc-u3", 0x0c000, 0x2000, CRC(edcc21c6) SHA1(5d73c6a747cfe951dc7c6ddfbb29859e9548aded) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "ctfrc-u5", 0x10000, 0x20000, CRC(1ff7fc0d) SHA1(bbf1fb616c13795c9c8e447a72f009ee3746bd90) ) /* banked */
	ROM_RELOAD( 0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "ctfrc-u1", 0x00000, 0x20000, CRC(7ff2a7f8) SHA1(234041d4c880ccd4acd295c9b2a3d6e37089beda) )

	ROM_REGION( 0x0100, "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "31a05a48.prm", 0x00000, 0x10, CRC(31a05a48) SHA1(8b340600feae03bb5cdab852a9879ecffcc8a2b9) )
ROM_END

ROM_START( pc_digdg )	/* Dig Dug (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "digdug.u1",    	 0x08000, 0x8000, CRC(a26ae22e) SHA1(0f95aa5f74ef2b192e355381987cf4624fce5653) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "digdug.u2",		 0x00000, 0x2000, CRC(2dabc198) SHA1(79f1e65eea2a2841dc47916cb86fbf992b936bd5) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_dk )	/* Donkey Kong (Golf) */
	BIOS_CPU
	ROM_LOAD( "gf-u3",   0x0c000, 0x2000, CRC(882dea87) SHA1(e3bbca36efa66231b933713dec032bbb926b36e5) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "dk.u1",   0x0c000, 0x4000, CRC(f56a5b10) SHA1(2c4b1d653194df0996d54d9de9188b270d0337d9) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "dk.u2",   0x00000, 0x2000, CRC(e472a17c) SHA1(3e3d5a2213c132fcb86a97089f03a7ebf8384d36) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "2cd98ef6.prm", 0x00000, 0x10, CRC(2cd98ef6) SHA1(bd5142c6a29df674ab835c8beafff7e93712d88f) )
ROM_END

ROM_START( pc_dk3 )	/* Donkey Kong 3 (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "dk3.u1", 0x08000, 0x8000, CRC(01eb0432) SHA1(94546c80c0bd41acc85f6e73a1425125c967d5db) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "dk3.u2", 0x00000, 0x2000, CRC(85d06c57) SHA1(803adef82721452704607fbf70027ff792636325) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_galag )	/* Galaga (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "galaga.u1", 0x08000, 0x8000, CRC(b4c4c5fe) SHA1(16e53935fc998de1eb8a88f73217ef1e2005d026) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "galaga.u2", 0x00000, 0x2000, CRC(e2d5964e) SHA1(62b2cd49a6a4c6f1e2b0742db3b7493e167e9a23) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_gyrus )	/* Gyruss (Dr Mario) */
	BIOS_CPU
	ROM_LOAD( "vu-u2",   0x0c000, 0x2000, CRC(4b7869ac) SHA1(37afb84d963233ad92cc424fcf992aa76ea0599f) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x30000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "gyruss-u4",  0x10000, 0x08000, CRC(48ecc48a) SHA1(0afdb50a07ae9610f5c62dc8cca661ca359352be) )	/* banked */
	ROM_RELOAD(			0x18000, 0x08000 )
	ROM_RELOAD(			0x20000, 0x08000 )
	ROM_RELOAD(			0x28000, 0x08000 )

	ROM_REGION( 0x020000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "gyruss-u1",  0x00000, 0x08000, CRC(3eb77902) SHA1(2419437ad701f5e165778af02b888da268c9a74d) )
	ROM_RELOAD(			0x08000, 0x08000 )
	ROM_RELOAD(			0x10000, 0x08000 )
	ROM_RELOAD(			0x18000, 0x08000 )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "1b26e58c.prm", 0x00000, 0x10, CRC(1b26e58c) SHA1(bd2d81d3cc54966ef154b3487d43ecbc316d6d22) )
ROM_END

ROM_START( pc_krsty )	/* Krusty's Funhouse (Power Blade) */
	BIOS_CPU
	ROM_LOAD( "xu-u3",   0x0c000, 0x2000, CRC(c3984e09) SHA1(70d7e5d9cf9b1f358e1be84a0e8c5997b1aae2d9) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "xu-u4",   0x10000, 0x20000, CRC(1f4103e5) SHA1(0a45c9b140df98aac7936c84e732410d796a35d7) )	/* banked */
	ROM_LOAD( "xu-u5",   0x30000, 0x20000, CRC(778c4115) SHA1(beab60c8237ffba27f6730fe7f4c45173cf892e0) )	/* banked */

	ROM_REGION( 0x020000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "xu-u1",    0x00000, 0x20000, CRC(fce8e0ea) SHA1(808f74e9124f273aab7adf5f31c2d5893b06323e) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "0fe6e900.prm", 0x00000, 0x10, CRC(0fe6e900) SHA1(544d8af1aa9186bf76d0a35e78b20e94d3afbcb5) )
ROM_END

ROM_START( pc_mman5 )	/* Megaman 5 (TMNT2) */
	BIOS_CPU
	ROM_LOAD( "2n-u3",    0x0c000, 0x2000, CRC(65298370) SHA1(fd120f43e465a2622f2e2679ace2fb0fe7e709b1) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "2n-u5",    0x10000, 0x40000, CRC(f3c743aa) SHA1(ee907c7279dba52320e35906ab5b0a1bdbbf0bfb) )	/* banked */

	ROM_REGION( 0x040000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "2n-u1",    0x00000, 0x40000, CRC(25e0ae72) SHA1(45d3ffbb5e399c891df6fcbd78b94b866be9b663) )
	
	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "237e8519.prm", 0x00000, 0x10, CRC(237e8519) SHA1(81b368d0784e4172c5cf9f4f4b92e29e05d34ae7) )
ROM_END

ROM_START( pc_pacm )	/* Pacman (Golf) */
	BIOS_CPU
	ROM_LOAD( "gf-u3",   0x0c000, 0x2000, CRC(882dea87) SHA1(e3bbca36efa66231b933713dec032bbb926b36e5) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "pacman.u1",   0x0c000, 0x4000, CRC(de2070ab) SHA1(a9324a94fa3d5ae6396a90c3d6d8399de8f93bd1) )

	ROM_REGION( 0x02000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "pacman.u2",	 0x00000, 0x2000, CRC(db6c9324) SHA1(f0d0fa49fbccc3b005742fc72a8dd3dce63909d1) )

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "2cd98ef6.prm", 0x00000, 0x10, CRC(2cd98ef6) SHA1(bd5142c6a29df674ab835c8beafff7e93712d88f) )
ROM_END

ROM_START( pc_parsl ) /* Parasol Stars - The Story of Bubble Bobble 3 (Ninja Gaiden) */
	BIOS_CPU
	ROM_LOAD( "parsl-u2", 0x0c000, 0x2000, CRC(7505de96) SHA1(a9cbe6d4d2d33aeecb3e041315fbb266c886ebf1) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x30000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "parsl-u4", 0x10000, 0x20000, CRC(15382139) SHA1(50991602680588a9e7b03e06d45ee607fc00db13) ) /* banked */

	ROM_REGION( 0x020000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "parsl-u1", 0x00000, 0x20000, CRC(af5400dc) SHA1(68258fedf7e2ba3afcf20e8261ad93d282c10bd9) ) /* banked */

	ROM_REGION( 0x0100, "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "ec5641d6.prm", 0x00000, 0x10, CRC(ec5641d6) SHA1(05f546aec5a9db167688a9abbac922f5ced7f7c5) )
ROM_END

ROM_START( pc_skykd )	/* Sky Kid (Ninja Gaiden) */
	BIOS_CPU
	ROM_LOAD( "u2ng",    0x0c000, 0x2000, CRC(7505de96) SHA1(a9cbe6d4d2d33aeecb3e041315fbb266c886ebf1) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x30000, "cart", 0 )  /* 64k for code */
	ROM_LOAD( "skykid.u4",    0x10000, 0x20000, CRC(c1918ec3) SHA1(e34bd36d9a7df7197b06a0a537b9d786f2303a98) )	/* banked */

	ROM_REGION( 0x020000, "gfx2", 0 )	/* cart gfx */
	ROM_LOAD( "skykid.u1",    0x00000, 0x20000, CRC(0f58923f) SHA1(805cc304ab0e20337c0b0dcd5ca7f2b7bf8e4656) )	/* banked */

	ROM_REGION( 0x0100,  "rp5h01", 0 )	/* rp5h01 data */
	ROM_LOAD( "ec5641d6.prm", 0x00000, 0x10, CRC(ec5641d6) SHA1(05f546aec5a9db167688a9abbac922f5ced7f7c5) )
ROM_END

GAME( 2002, pc_ark,   playch10, playch10, playch10, playch10_state, pc_hrz,   ROT0, "NES->PC-10", "Arkanoid (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_bb2,   playch10, playch10, playch10, playch10_state, pcgboard, ROT0, "NES->PC-10", "Bubble Bobble 2 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_cch,   playch10, playch10, playch10, playch10_state, playch10, ROT0, "NES->PC-10", "Circus Charlie (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_ctfrc, playch10, playch10, playch10, playch10_state, pcgboard, ROT0, "NES->PC-10", "Contra Force (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_digdg, playch10, playch10, playch10, playch10_state, pc_hrz,   ROT0, "NES->PC-10", "Dig Dug (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_dk,    playch10, playch10, playch10, playch10_state, playch10, ROT0, "NES->PC-10", "Donkey Kong (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_dk3,   playch10, playch10, playch10, playch10_state, pc_hrz,   ROT0, "NES->PC-10", "Donkey Kong 3 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_galag, playch10, playch10, playch10, playch10_state, pc_hrz,   ROT0, "NES->PC-10", "Galaga (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_gyrus, playch10, playch10, playch10, playch10_state, pcfboard, ROT0, "NES->PC-10", "Gyruss (PlayChoice-10)", MACHINE_SUPPORTS_SAVE | MACHINE_IMPERFECT_GRAPHICS )
GAME( 2002, pc_krsty, playch10, playch10, playch10, playch10_state, pcgboard, ROT0, "NES->PC-10", "Krusty's Funhouse (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_mman5, playch10, playch10, playch10, playch10_state, pcgboard, ROT0, "NES->PC-10", "Megaman 5 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_pacm,  playch10, playch10, playch10, playch10_state, playch10, ROT0, "NES->PC-10", "Pacman (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_parsl, playch10, playch10, playch10, playch10_state, pcfboard, ROT0, "NES->PC-10", "Parasol Stars-The Story of Bubble Bobble 3 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_skykd, playch10, playch10, playch10, playch10_state, pcfboard, ROT0, "NES->PC-10", "Sky Kid (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
