// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/btime.c"

ROM_START( bnjr )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "bnj12b.bin",  0xa000, 0x2000, CRC(ba3e3801) SHA1(56284076d938c33c1492a07281b936681eb09808) )
	ROM_LOAD( "bnj12c.bin",  0xc000, 0x2000, CRC(fb3a2cdd) SHA1(4a964389cc8035b9264d4cb133eb6d3826e74b95) )
	ROM_LOAD( "bnj12d.bin",  0xe000, 0x2000, CRC(b88bc99e) SHA1(08a4ddea4037f9e14d0d9f4262a1746b0a3a140c) )

	ROM_REGION( 0x10000, "audiocpu", 0 )     /* 64k for the audio CPU */
	ROM_LOAD( "bnj6c.bin",   0xe000, 0x1000, CRC(8c02f662) SHA1(1279d564e65fd3ccac25b1f9fbb40d910de2b544) )
	
	ROM_REGION( 0x6000, "gfx1", 0 )
	ROM_LOAD( "bnjr4e.bin",  0x0000, 0x2000, CRC(876cc672) SHA1(860cfc92752f312ce167fdfb7752fb5e7d7d8817) )
	ROM_LOAD( "bnjr4f.bin",  0x2000, 0x2000, CRC(e5e95e3f) SHA1(c16e03e29229fa7dcc9ed01b43b645fbcc79cc97) )
	ROM_LOAD( "bnjr4h.bin",  0x4000, 0x2000, CRC(570ffa46) SHA1(e397fbca0cc7b8173476d34d4e651752937b7569) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "bnjr10e.bin", 0x0000, 0x1000, CRC(9e523e4c) SHA1(4d76696c7594d84d081d17220dc58c0e7e35a14c) )
	ROM_LOAD( "bnjr10f.bin", 0x1000, 0x1000, CRC(5cf10f28) SHA1(66b2d8cc27ff6dd340054290f6eb2d71c9a1052a) )
ROM_END

ROM_START( pnc )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "s3-3d",        0xc000, 0x1000, CRC(1ab4f2c2) SHA1(c5890b768172cd2e3912b84db5f71546969ad7e2) )
	ROM_LOAD( "s2-3c",        0xd000, 0x1000, CRC(5e46b789) SHA1(00b2510e07eb565cb373db798dd537191b0b7cc8) )
	ROM_LOAD( "s1-3b",        0xe000, 0x1000, CRC(1308a32e) SHA1(da64fe7b76f5ac8ac35460e6c789ab1e986c78ef) )
	ROM_LOAD( "s0-3a",        0xf000, 0x1000, CRC(beb4b1fc) SHA1(166a96b5757946231f3619844366218065412935) )

	ROM_REGION( 0x10000, "audiocpu", 0 )     /* 64k for the audio CPU */
	ROM_LOAD( "sa-1h",        0xe000, 0x1000, CRC(379387ec) SHA1(29d37f04c64ed53a2573962dfa9c0623b89e0045) )

	ROM_REGION( 0x6000, "gfx1", 0 )
	ROM_LOAD( "pnc-s4-11l",   0x0000, 0x1000, CRC(38a40f50) SHA1(c5188fc3eeffb9843a22211b95ef6fec656dedb9) )
	ROM_LOAD( "pnc-s5-11m",   0x1000, 0x1000, CRC(c6c7236a) SHA1(da50b4066c6d39c1c860afdbd353de5576bf9fbf) )
	ROM_LOAD( "pnc-s6-13l",   0x2000, 0x1000, CRC(20779144) SHA1(f580db44ce870cb5945ef341ef5dd674b170ffc6) )
	ROM_LOAD( "pnc-s7-13m",   0x3000, 0x1000, CRC(8911a546) SHA1(e78ff510af33789e6a25227afa269008c049b553) )
	ROM_LOAD( "pnc-s8-15l",   0x4000, 0x1000, CRC(1c8f5112) SHA1(01601dc443e1db13f9fb9b32cc5180d5d19b4c10) )
	ROM_LOAD( "pnc-s9-15m",   0x5000, 0x1000, CRC(b919e43b) SHA1(1bb22f48a7914afc8b4eb8d90b9c6a4d93ae2b24) )
	
	ROM_REGION( 0x0040, "proms", 0 )
	ROM_LOAD( "sc-5m",        0x0000, 0x0020, CRC(2a976ebe) SHA1(f3c1b0d98f431f9cd0d5fa009fafa1115aabe6e5) )	/* palette */
	ROM_LOAD( "sb-4c",        0x0020, 0x0020, CRC(a29b4204) SHA1(7f15cae5c4aaa29638fb45029782dafd2b3d1484) )	/* RAS/CAS logic - not used */
ROM_END

ROM_START( stictime )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "aa04.9b",      0xc000, 0x1000, CRC(368a25b5) SHA1(ed3f3712423979dcb351941fa85dce6a0a7bb16b) )
	ROM_LOAD( "aa06.13b",     0xd000, 0x1000, CRC(b4ba400d) SHA1(8c77397e934907bc47a739f263196a0f2f81ba3d) )
	ROM_LOAD( "aa05.10b",     0xe000, 0x1000, CRC(8005bffa) SHA1(d0da4e360039f6a8d8142a4e8e05c1f90c0af68a) )
	ROM_LOAD( "aa07.15b",     0xf000, 0x1000, CRC(086440ad) SHA1(4a32bc92f8ff5fbe112f56e62d2c03da8851a7b9) )

	ROM_REGION( 0x10000, "audiocpu", 0 )     /* 64k for the audio CPU */
	ROM_LOAD( "ab14.12h",     0xe000, 0x1000, CRC(f55e5211) SHA1(27940026d0c6212d1138d2fd88880df697218627) )

	ROM_REGION( 0x6000, "gfx1", 0 )
	ROM_LOAD( "stic_aa12.7k", 0x0000, 0x1000, CRC(0550e118) SHA1(74ab46a307ea9d61e38ba0b541bf509436cd9290) )    /* charset #1 */
	ROM_LOAD( "ab13.9k",      0x1000, 0x1000, CRC(ac01042f) SHA1(e64b6381a9298eaf74e79fa5f1ea8e9596c58a49) )
	ROM_LOAD( "stic_ab10.10k",0x2000, 0x1000, CRC(ba774d51) SHA1(68806bbe93aeea82d7aefc376b69af66858c6028) )
	ROM_LOAD( "ab11.12k",     0x3000, 0x1000, CRC(d4848014) SHA1(0a55b091cd4e7f317c35defe13d5051b26042eee) )
	ROM_LOAD( "stic_aa8.13k", 0x4000, 0x1000, CRC(00a15086) SHA1(e78047ebffdc642583d363b12dd8fbd1368eb3c4) )
	ROM_LOAD( "ab9.15k",      0x5000, 0x1000, CRC(8dec15e6) SHA1(b72633de6268ce16742bba4dcba835df860d6c2f) )

	ROM_REGION( 0x1800, "gfx2", 0 )
	ROM_LOAD( "stic_ab00.1b", 0x0000, 0x0800, CRC(4010c652) SHA1(418eae0ef70c62c3f69becf167bd40a4faba176b) )    /* charset #2 */
	ROM_LOAD( "ab01.3b",      0x0800, 0x0800, CRC(25b49078) SHA1(4abdcbd4f3362c3e4463a1274731289f1a72d2e6) )
	ROM_LOAD( "stic_ab02.4b", 0x1000, 0x0800, CRC(1def5b40) SHA1(845a6b1105757d4d921a4d1f9378a3731f6460b3) )

	ROM_REGION( 0x0800, "bg_map", 0 )	/* background tilemaps */
	ROM_LOAD( "ab03.6b",      0x0000, 0x0800, CRC(d26bc1f3) SHA1(737af6e264183a1f151f277a07cf250d6abb3fd8) )
ROM_END

ROM_START( vecbtime )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "ab05a1.12b",       0xb000, 0x1000, CRC(0a98b230) SHA1(aeee4f6f0aaa27575b80261d03c5453cc6ebd646) )
	ROM_LOAD( "ab04.9b",          0xc000, 0x1000, CRC(797e5f75) SHA1(35ea5fa4b8f3494adf7774b3946ed2540ac826ff) )
	ROM_LOAD( "ab06.13b",         0xd000, 0x1000, CRC(c77f3f64) SHA1(f283087fad0a102fe92be7ce80ed18e64dc93b67) )
	ROM_LOAD( "ab05.10b",         0xe000, 0x1000, CRC(b0d3640f) SHA1(6ba28971714ece6f1c04fa2dbf1f9f216ded7cfa) )
	ROM_LOAD( "ab07.15b",         0xf000, 0x1000, CRC(a142f862) SHA1(39d7ef172d18874885f1b1542e885cc4287dc344) )

	ROM_REGION( 0x10000, "audiocpu", 0 )     /* 64k for the audio CPU */
	ROM_LOAD( "ab14.12h",         0xe000, 0x1000, CRC(f55e5211) SHA1(27940026d0c6212d1138d2fd88880df697218627) )

	ROM_REGION( 0x6000, "gfx1", 0 )
	ROM_LOAD( "vec_ab12.7k",      0x0000, 0x1000, CRC(b02d859c) SHA1(0d506c52dd2a76d62bd980087fbb5c3808aedf56) )    /* charset #1 */
	ROM_LOAD( "vec_ab13.9k",      0x1000, 0x1000, CRC(d9757010) SHA1(fc70cfbd7bb7c23d0b5880f42ef92df711c6a46d) )
	ROM_LOAD( "vec_ab10.10k",     0x2000, 0x1000, CRC(2b71a1db) SHA1(424c3fceeeb030c0cf89e8f872957a75d5475cfb) )
	ROM_LOAD( "vec_ab11.12k",     0x3000, 0x1000, CRC(a98fea41) SHA1(1ddbb8b26bca2b8c6ab4d9b1bfd8b21db2b2d97b) )
	ROM_LOAD( "vec_ab8.13k",      0x4000, 0x1000, CRC(0ca7828f) SHA1(eb7db2607b10e8276a97a20030bdbeeeb7b6935b) )
	ROM_LOAD( "vec_ab9.15k",      0x5000, 0x1000, CRC(1e460ff7) SHA1(6ac3ec08e3c79490262324fd4a93a6eb1188554b) )
	
	ROM_REGION( 0x1800, "gfx2", 0 )
	ROM_LOAD( "vec_ab00.1b",      0x0000, 0x0800, CRC(03dbe9b6) SHA1(4844d490a767d15ed5230868cbd25dadf70cf1e3) )    /* charset #2 */
	ROM_LOAD( "vec_ab01.3b",      0x0800, 0x0800, CRC(698089da) SHA1(6b6e61536d1247a8784a4657cdc50e48b002335d) )
	ROM_LOAD( "vec_ab02.4b",      0x1000, 0x0800, CRC(4464ccef) SHA1(1fa91178c55c1120910be89f724b909a356796e8) )

	ROM_REGION( 0x0800, "bg_map", 0 )	/* background tilemaps */
	ROM_LOAD( "ab03.6b",          0x0000, 0x0800, CRC(d26bc1f3) SHA1(737af6e264183a1f151f277a07cf250d6abb3fd8) )
ROM_END


GAME( 2002, bnjr,     0,       bnj,      bnj,   btime_state,   lnc,     ROT270, "DaveC", "Bump 'n' Jump Reverse Mod", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pnc,      0,       lnc,      lnc,   btime_state,   lnc,     ROT270, "Data East Corporation", "Pac'n'Chase", MACHINE_SUPPORTS_SAVE )
GAME( 2002, stictime, btime,   btime,    btime, btime_state,   btime,   ROT270, "Marks", "Burger Time (Mr Sticky)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vecbtime, btime,   btime,    btime, btime_state,   btime,   ROT270, "T-Bone", "Burgertime (Vector sim)", MACHINE_SUPPORTS_SAVE )
