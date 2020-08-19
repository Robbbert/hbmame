// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/playch10.cpp"

ROM_START( pc_ark ) /* Arkanoid (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "arkanoid-u1", 0x08000, 0x8000, CRC(95dbb274) SHA1(792420b622b59d40ec0a264bf0bcd5faf9b652dc) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "arkanoid-u2", 0x00000, 0x2000, CRC(bb8dae70) SHA1(91bdab01839e2ad3d9aaa9041e0855ba26583a9f) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_bb2 ) /* Bubble Bobble Part 2 (Power Blade) */
	BIOS_CPU
	ROM_LOAD( "bb2-u3", 0x0c000, 0x2000, CRC(edcc21c6) SHA1(5d73c6a747cfe951dc7c6ddfbb29859e9548aded) ) /* extra bios code for this game */
	BIOS_GFX
	
	ROM_REGION( 0x50000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "bb2-u5", 0x10000, 0x20000, CRC(5f7c1d17) SHA1(48608c9ebd8c0feb9e1126eb4070f185c41d9a19) ) /* banked */
	ROM_RELOAD( 0x30000, 0x20000 )

	ROM_REGION( 0x20000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "bb2-u1", 0x00000, 0x20000, CRC(5e59afae) SHA1(5ed7d7c1ed50dbdc01e0a80e6ed5a246349726aa) )
	
	ROM_REGION( 0x10, "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "31a05a48.prm", 0x00000, 0x10, CRC(31a05a48) SHA1(8b340600feae03bb5cdab852a9879ecffcc8a2b9) )
ROM_END

ROM_START( pc_cch ) /* Circus Charlie (Golf) */
	BIOS_CPU
	ROM_LOAD( "gf-u3",   0x0c000, 0x2000, CRC(882dea87) SHA1(e3bbca36efa66231b933713dec032bbb926b36e5) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "circusch.u1", 0x0c000, 0x4000, CRC(2cb269d5) SHA1(28a700b4fa5c580adf3ef9891506585ef6a40725) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "circusch.u2", 0x00000, 0x2000, CRC(f76c592a) SHA1(c8bc574c43d26d5072869922573edaaab7a37050) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "2cd98ef6.prm", 0x00000, 0x10, CRC(2cd98ef6) SHA1(bd5142c6a29df674ab835c8beafff7e93712d88f) )
ROM_END

ROM_START( pc_ctfrc ) /* Contra Force (Power Blade) */
	BIOS_CPU
	ROM_LOAD( "ctfrc-u3", 0x0c000, 0x2000, CRC(edcc21c6) SHA1(5d73c6a747cfe951dc7c6ddfbb29859e9548aded) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "ctfrc-u5", 0x10000, 0x20000, CRC(1ff7fc0d) SHA1(bbf1fb616c13795c9c8e447a72f009ee3746bd90) ) /* banked */
	ROM_RELOAD( 0x30000, 0x20000 )

	ROM_REGION( 0x20000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "ctfrc-u1", 0x00000, 0x20000, CRC(7ff2a7f8) SHA1(234041d4c880ccd4acd295c9b2a3d6e37089beda) )

	ROM_REGION( 0x10, "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "31a05a48.prm", 0x00000, 0x10, CRC(31a05a48) SHA1(8b340600feae03bb5cdab852a9879ecffcc8a2b9) )
ROM_END

ROM_START( pc_digdg ) /* Dig Dug (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "digdug.u1",    0x08000, 0x8000, CRC(a26ae22e) SHA1(0f95aa5f74ef2b192e355381987cf4624fce5653) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "digdug.u2",    0x00000, 0x2000, CRC(2dabc198) SHA1(79f1e65eea2a2841dc47916cb86fbf992b936bd5) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_dk ) /* Donkey Kong (Golf) */
	BIOS_CPU
	ROM_LOAD( "gf-u3",   0x0c000, 0x2000, CRC(882dea87) SHA1(e3bbca36efa66231b933713dec032bbb926b36e5) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "dk.u1",   0x0c000, 0x4000, CRC(f56a5b10) SHA1(2c4b1d653194df0996d54d9de9188b270d0337d9) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "dk.u2",   0x00000, 0x2000, CRC(e472a17c) SHA1(3e3d5a2213c132fcb86a97089f03a7ebf8384d36) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "2cd98ef6.prm", 0x00000, 0x10, CRC(2cd98ef6) SHA1(bd5142c6a29df674ab835c8beafff7e93712d88f) )
ROM_END

ROM_START( pc_dk3 ) /* Donkey Kong 3 (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "dk3.u1", 0x08000, 0x8000, CRC(01eb0432) SHA1(94546c80c0bd41acc85f6e73a1425125c967d5db) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "dk3.u2", 0x00000, 0x2000, CRC(85d06c57) SHA1(803adef82721452704607fbf70027ff792636325) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_galag ) /* Galaga (1942) */
	BIOS_CPU
	ROM_LOAD( "u3",      0x0c000, 0x2000, CRC(415b8807) SHA1(9d6161bbc6dec5873cc6d8a570141d4af42fa232) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "galaga.u1", 0x08000, 0x8000, CRC(b4c4c5fe) SHA1(16e53935fc998de1eb8a88f73217ef1e2005d026) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "galaga.u2", 0x00000, 0x2000, CRC(e2d5964e) SHA1(62b2cd49a6a4c6f1e2b0742db3b7493e167e9a23) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "29893c7f.prm", 0x00000, 0x10, CRC(29893c7f) SHA1(58478b7de2177c8dc1d6885bd34eeeeb5e46d7a3) )
ROM_END

ROM_START( pc_gyrus ) /* Gyruss (Dr Mario) */
	BIOS_CPU
	ROM_LOAD( "vu-u2",   0x0c000, 0x2000, CRC(4b7869ac) SHA1(37afb84d963233ad92cc424fcf992aa76ea0599f) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x30000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "gyruss-u4",  0x10000, 0x08000, CRC(48ecc48a) SHA1(0afdb50a07ae9610f5c62dc8cca661ca359352be) ) /* banked */
	ROM_RELOAD( 0x18000, 0x08000 )
	ROM_RELOAD( 0x20000, 0x08000 )
	ROM_RELOAD( 0x28000, 0x08000 )

	ROM_REGION( 0x20000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "gyruss-u1",  0x00000, 0x08000, CRC(3eb77902) SHA1(2419437ad701f5e165778af02b888da268c9a74d) )
	ROM_RELOAD( 0x08000, 0x08000 )
	ROM_RELOAD( 0x10000, 0x08000 )
	ROM_RELOAD( 0x18000, 0x08000 )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "1b26e58c.prm", 0x00000, 0x10, CRC(1b26e58c) SHA1(bd2d81d3cc54966ef154b3487d43ecbc316d6d22) )
ROM_END

ROM_START( pc_krsty ) /* Krusty's Funhouse (Power Blade) */
	BIOS_CPU
	ROM_LOAD( "xu-u3",   0x0c000, 0x2000, CRC(c3984e09) SHA1(70d7e5d9cf9b1f358e1be84a0e8c5997b1aae2d9) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "xu-u4",   0x10000, 0x20000, CRC(1f4103e5) SHA1(0a45c9b140df98aac7936c84e732410d796a35d7) ) /* banked */
	ROM_LOAD( "xu-u5",   0x30000, 0x20000, CRC(778c4115) SHA1(beab60c8237ffba27f6730fe7f4c45173cf892e0) ) /* banked */

	ROM_REGION( 0x020000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "xu-u1",    0x00000, 0x20000, CRC(fce8e0ea) SHA1(808f74e9124f273aab7adf5f31c2d5893b06323e) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "0fe6e900.prm", 0x00000, 0x10, CRC(0fe6e900) SHA1(544d8af1aa9186bf76d0a35e78b20e94d3afbcb5) )
ROM_END

ROM_START( pc_mman5 ) /* Megaman 5 (TMNT2) */
	BIOS_CPU
	ROM_LOAD( "2n-u3",    0x0c000, 0x2000, CRC(65298370) SHA1(fd120f43e465a2622f2e2679ace2fb0fe7e709b1) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "2n-u5",    0x10000, 0x40000, CRC(f3c743aa) SHA1(ee907c7279dba52320e35906ab5b0a1bdbbf0bfb) ) /* banked */

	ROM_REGION( 0x40000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "2n-u1",    0x00000, 0x40000, CRC(25e0ae72) SHA1(45d3ffbb5e399c891df6fcbd78b94b866be9b663) )
	
	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "237e8519.prm", 0x00000, 0x10, CRC(237e8519) SHA1(81b368d0784e4172c5cf9f4f4b92e29e05d34ae7) )
ROM_END

ROM_START( pc_pacm ) /* Pacman (Golf) */
	BIOS_CPU
	ROM_LOAD( "gf-u3",   0x0c000, 0x2000, CRC(882dea87) SHA1(e3bbca36efa66231b933713dec032bbb926b36e5) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "pacman.u1",   0x0c000, 0x4000, CRC(de2070ab) SHA1(a9324a94fa3d5ae6396a90c3d6d8399de8f93bd1) )

	ROM_REGION( 0x02000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "pacman.u2",   0x00000, 0x2000, CRC(db6c9324) SHA1(f0d0fa49fbccc3b005742fc72a8dd3dce63909d1) )

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "2cd98ef6.prm", 0x00000, 0x10, CRC(2cd98ef6) SHA1(bd5142c6a29df674ab835c8beafff7e93712d88f) )
ROM_END

ROM_START( pc_parsl ) /* Parasol Stars - The Story of Bubble Bobble 3 (Ninja Gaiden) */
	BIOS_CPU
	ROM_LOAD( "parsl-u2", 0x0c000, 0x2000, CRC(7505de96) SHA1(a9cbe6d4d2d33aeecb3e041315fbb266c886ebf1) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x30000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "parsl-u4", 0x10000, 0x20000, CRC(15382139) SHA1(50991602680588a9e7b03e06d45ee607fc00db13) ) /* banked */

	ROM_REGION( 0x20000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "parsl-u1", 0x00000, 0x20000, CRC(af5400dc) SHA1(68258fedf7e2ba3afcf20e8261ad93d282c10bd9) ) /* banked */

	ROM_REGION( 0x10, "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "ec5641d6.prm", 0x00000, 0x10, CRC(ec5641d6) SHA1(05f546aec5a9db167688a9abbac922f5ced7f7c5) )
ROM_END

ROM_START( pc_skykd ) /* Sky Kid (Ninja Gaiden) */
	BIOS_CPU
	ROM_LOAD( "u2ng",    0x0c000, 0x2000, CRC(7505de96) SHA1(a9cbe6d4d2d33aeecb3e041315fbb266c886ebf1) ) /* extra bios code for this game */
	BIOS_GFX

	ROM_REGION( 0x30000, "cart", 0 ) /* 64k for code */
	ROM_LOAD( "skykid.u4",    0x10000, 0x20000, CRC(c1918ec3) SHA1(e34bd36d9a7df7197b06a0a537b9d786f2303a98) ) /* banked */

	ROM_REGION( 0x20000, "gfx2", 0 ) /* cart gfx */
	ROM_LOAD( "skykid.u1",    0x00000, 0x20000, CRC(0f58923f) SHA1(805cc304ab0e20337c0b0dcd5ca7f2b7bf8e4656) ) /* banked */

	ROM_REGION( 0x10,  "rp5h01", 0 ) /* rp5h01 data */
	ROM_LOAD( "ec5641d6.prm", 0x00000, 0x10, CRC(ec5641d6) SHA1(05f546aec5a9db167688a9abbac922f5ced7f7c5) )
ROM_END

GAME( 2002, pc_ark,   playch10, playch10, playch10, playch10_state, init_pc_hrz,   ROT0, "NES->PC-10", "Arkanoid (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_bb2,   playch10, playch10, playch10, playch10_state, init_pcgboard, ROT0, "NES->PC-10", "Bubble Bobble 2 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_cch,   playch10, playch10, playch10, playch10_state, init_playch10, ROT0, "NES->PC-10", "Circus Charlie (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_ctfrc, playch10, playch10, playch10, playch10_state, init_pcgboard, ROT0, "NES->PC-10", "Contra Force (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_digdg, playch10, playch10, playch10, playch10_state, init_pc_hrz,   ROT0, "NES->PC-10", "Dig Dug (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_dk,    playch10, playch10, playch10, playch10_state, init_playch10, ROT0, "NES->PC-10", "Donkey Kong (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_dk3,   playch10, playch10, playch10, playch10_state, init_pc_hrz,   ROT0, "NES->PC-10", "Donkey Kong 3 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_galag, playch10, playch10, playch10, playch10_state, init_pc_hrz,   ROT0, "NES->PC-10", "Galaga (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_gyrus, playch10, playch10, playch10, playch10_state, init_pcfboard, ROT0, "NES->PC-10", "Gyruss (PlayChoice-10)", MACHINE_SUPPORTS_SAVE | MACHINE_IMPERFECT_GRAPHICS )
GAME( 2002, pc_krsty, playch10, playch10, playch10, playch10_state, init_pcgboard, ROT0, "NES->PC-10", "Krusty's Funhouse (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_mman5, playch10, playch10, playch10, playch10_state, init_pcgboard, ROT0, "NES->PC-10", "Megaman 5 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_pacm,  playch10, playch10, playch10, playch10_state, init_playch10, ROT0, "NES->PC-10", "Pacman (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_parsl, playch10, playch10, playch10, playch10_state, init_pcfboard, ROT0, "NES->PC-10", "Parasol Stars-The Story of Bubble Bobble 3 (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, pc_skykd, playch10, playch10, playch10, playch10_state, init_pcfboard, ROT0, "NES->PC-10", "Sky Kid (PlayChoice-10)", MACHINE_SUPPORTS_SAVE )

/******************
 Super Mario Bros
********************/

ROM_START( pc_smb_ps01 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps01",    0x08000, 0x8000, CRC(6557eeee) SHA1(3c457b8c53ada71cbbaf534a566ce9c4fd5a6554) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps02 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps02",    0x08000, 0x8000, CRC(dcac454d) SHA1(2150479ee1b6b210eafb671794f8e72c8794a81e) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps02",    0x00000, 0x2000, CRC(d371bfbc) SHA1(92ddefb8523bc79a4895a0c0dd41e531619690b3) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps03 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps03",    0x08000, 0x8000, CRC(055a9072) SHA1(3e6e559ee2ef22d1514ce73dd68b1cf23a57dda1) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps04 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps04",    0x08000, 0x8000, CRC(4db902f8) SHA1(552c360531fb9de80e773f8504eb8e17f440e7b6) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps05 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps05",    0x08000, 0x8000, CRC(08d0d8f5) SHA1(549e01980ad33a425925d9eefeb34811ce149f15) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps05",    0x00000, 0x2000, CRC(90b06693) SHA1(2f7627cc2a8f19661d8ac8c2f19044af0a6f57dc) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps06 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps06",    0x08000, 0x8000, CRC(a511d786) SHA1(53d616a93c3cdc926871cbe2ca0b5a7335a71707) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps06",    0x00000, 0x2000, CRC(735da4d2) SHA1(f57dc2208911c43f0742d475ee7f377a3d20253c) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps07 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps07",    0x08000, 0x8000, CRC(955432fd) SHA1(22bfa098a556c7f5375ea2619ca8a8aa862dcb5f) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps07",    0x00000, 0x2000, CRC(d67d3e9f) SHA1(f983c01cbbbc32fc15a7b35e557d2daf01773c0a) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps08 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps08",    0x08000, 0x8000, CRC(9b223439) SHA1(35659931d575ecefc12fd63980d59334d11df5ff) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps08",    0x00000, 0x2000, CRC(2690eea3) SHA1(14c328cbf5f34ddf64f879f8cd1852204c680e96) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps09 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps09",    0x08000, 0x8000, CRC(35fa8c08) SHA1(0716d4f8ef7edf71e9de470c0b9cbb75d01002bc) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps09",    0x00000, 0x2000, CRC(20d1ad42) SHA1(5a849c38cb1bc86197d580cd89530245f20dffa5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps10 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps10",    0x08000, 0x8000, CRC(886ef164) SHA1(6076a54bbc1cdb8c78e230285bca5defbce518cc) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps11 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps11",    0x08000, 0x8000, CRC(ef2f7b6f) SHA1(0c309f1d472d7f77906b53b81a0435becd2a8189) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps11",    0x00000, 0x2000, CRC(52b5f665) SHA1(16a45f269e348984868f63b6bfe6d111876ec843) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps12 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps12",    0x08000, 0x8000, CRC(86139119) SHA1(24aba6624f0356b9a9a096d50659f09031a7c51d) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps12",    0x00000, 0x2000, CRC(6c9054d4) SHA1(8d6c4321c0a7a0ccb58c70863bb2ef4434d7f9f7) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps13 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps13",    0x08000, 0x8000, CRC(967a605f) SHA1(31b332f6bc338e058a7b958dca285066c405b697) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps14 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps14",    0x08000, 0x8000, CRC(c85ad443) SHA1(1d126e7ae5dfffb93810f693ce2253165836ba71) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps14",    0x00000, 0x2000, CRC(938ec91c) SHA1(f045cfe0d734ffc8d356d0cf3c56b06084fb4ecf) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps15 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps15",    0x08000, 0x8000, CRC(cfc697b9) SHA1(3cc38edfd1d5aae351c68a168498bdec8c6bd82d) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps16 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps16",    0x08000, 0x8000, CRC(bbbf02cc) SHA1(e1beb9a7bb35f8781ef10be8a40301ba0a12ea08) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps16",    0x00000, 0x2000, CRC(628488bd) SHA1(02dfc7593860c32a32c2bc32fb40a908b4984d0c) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps17 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps17",    0x08000, 0x8000, CRC(885acc2b) SHA1(448ed0051cb89a9d432b82e28a5e48a666f64fd0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps17",    0x00000, 0x2000, CRC(6e05ce19) SHA1(801766397a70349e66989af0bd9d1c07cacdde78) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps18 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps18",    0x08000, 0x8000, CRC(da2dc26e) SHA1(8a8ef6c74bb49f815be5765a1fd985b98575335c) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps19 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps19",    0x08000, 0x8000, CRC(00a1fd08) SHA1(e120ae69734fee92d270594f7a4b1cfbe27ed556) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps19",    0x00000, 0x2000, CRC(078a4304) SHA1(05056bace92931aeaa6b86a13e44f2da75e7a765) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps20 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps20",    0x08000, 0x8000, CRC(c3337278) SHA1(133713662f060d333e3c149fed30d0c91c59d8c3) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps20",    0x00000, 0x2000, CRC(3ccb959e) SHA1(97b63bc78de519045654de9cc688538bd8436661) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps21 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps21",    0x08000, 0x8000, CRC(473157dc) SHA1(9f1aac18c57f6fc740b378bf18749bf216678ee3) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps22 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps22",    0x08000, 0x8000, CRC(43d8c03d) SHA1(1ffdbd1dc71e2d37a558133eb3fb5c151ae068a6) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps22",    0x00000, 0x2000, CRC(ee35a20f) SHA1(dfb44485a5488240f8e320bc7963d3f1f2bfa00d) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps23 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps23",    0x08000, 0x8000, CRC(a00997dc) SHA1(84eac05bc2e305a805cf2a979dc03abde43483b5) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps23",    0x00000, 0x2000, CRC(e5473352) SHA1(dabf71676e2cacc419bd4cab953f9d80c6d5f261) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps24 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps24",    0x08000, 0x8000, CRC(898ff66f) SHA1(0d4b2a9feb5a2a8e693b4d0a3ecf516788db2349) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps24",    0x00000, 0x2000, CRC(5f00ce7f) SHA1(9d43f8ef2be2c30af89ed7fad9d68fe620b5519a) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps25 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps25",    0x08000, 0x8000, CRC(c7484f2c) SHA1(1fe40dde2cb1d5e1f35441d56ba941a7ffcceb79) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps26 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps26",    0x08000, 0x8000, CRC(2c6c2ec6) SHA1(23b4776137bd9f879c96230612a089d4457e2142) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps27 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps27",    0x08000, 0x8000, CRC(af0e6b6b) SHA1(f429efaff1857d1e3ced433c4defb095cd598ee1) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps28 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps28",    0x08000, 0x8000, CRC(62b608a6) SHA1(3658d3300e7de5ba419c3562d5987edb14f24514) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps28",    0x00000, 0x2000, CRC(c6001011) SHA1(24b1f69244c827e96e8b7d133c58bf7cf26dc563) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps29 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps29",    0x08000, 0x8000, CRC(fddc08c9) SHA1(fc790b7027fa299edec9ddcd9ff27ced7afa05c7) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps29",    0x00000, 0x2000, CRC(0feb1254) SHA1(b3e7cc137d88d6ceff59436eb3c3af13b120aed1) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps30 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps30",    0x08000, 0x8000, CRC(eff21c00) SHA1(10abf6217cc9243ee70909af9602503e4fdde6a3) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps30",    0x00000, 0x2000, CRC(bd41dd7a) SHA1(f21725d6a429844c10fd6f26e838eb9d579882aa) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps31 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps31",    0x08000, 0x8000, CRC(8ea5298a) SHA1(c48437c1867dfe3efdde1c191f7233f4ea69d98e) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps31",    0x00000, 0x2000, CRC(05e0f434) SHA1(cdd1fca7da1d10d4e500f5bec19c497646bed3c3) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps32 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps32",    0x08000, 0x8000, CRC(7817ee90) SHA1(9ca63ab8dfbb388e1d0a654f642fb2c8d189f033) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps32",    0x00000, 0x2000, CRC(db91b704) SHA1(9e1dd6e5d3b63eb860fc80d022eb0e641978c8a6) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps33 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps33",    0x08000, 0x8000, CRC(025073bd) SHA1(bc5aa7139e3378826f7048ee0b13c56359814e1e) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps33",    0x00000, 0x2000, CRC(871099a7) SHA1(76657cf0a92fea8c3aa60535163d66d14e2fbffa) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps34 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps34",    0x08000, 0x8000, CRC(f3646bf7) SHA1(60bc52905aee6807a47074d21166ab57202aa3f8) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps35 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps35",    0x08000, 0x8000, CRC(4b0856cf) SHA1(062b29d88c03c5c6f8c540060fde825cf1c3a6f0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps36 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps36",    0x08000, 0x8000, CRC(2849d729) SHA1(f87b131cd55e48102151043128a87da704329fd4) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps36",    0x00000, 0x2000, CRC(6c5efd2f) SHA1(54edf404aa9d1efcc1b551424b638809257c1e8f) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps37 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps37",    0x08000, 0x8000, CRC(4410f405) SHA1(21fe5b402c104ae4c13d16e61a7809403c5296db) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps37",    0x00000, 0x2000, CRC(c2fb3a43) SHA1(dd8acd1406fab75efb7ed01a357f9a76f4275d48) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps38 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps38",    0x08000, 0x8000, CRC(7c727fe1) SHA1(ff12129516c73bda2e60f9af6304a6dea1a4030f) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps38",    0x00000, 0x2000, CRC(8d791a7e) SHA1(b4119c229c7f4760a93e74234a5b15aa1bd400ef) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps39 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps17",    0x08000, 0x8000, CRC(885acc2b) SHA1(448ed0051cb89a9d432b82e28a5e48a666f64fd0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps39",    0x00000, 0x2000, CRC(168a5a8c) SHA1(08f672cac72d113ed18112d451ca376e624a706c) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps40 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps40",    0x08000, 0x8000, CRC(6a57e949) SHA1(12263a3a99e50ecf17cc20b2e85aef2f2795f513) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps40",    0x00000, 0x2000, CRC(bb1c49f2) SHA1(15e5c19043dff81e92a2b96f654ff287ceb97574) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps41 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps41",    0x08000, 0x8000, CRC(b90bc087) SHA1(1529efacde9cbb9177ba9e5bd0c28dac6b5701de) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps41",    0x00000, 0x2000, CRC(678f9338) SHA1(d8c44aa0672603ded63900bfcfd1ad27c9f2e763) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps42 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps42",    0x08000, 0x8000, CRC(455d428d) SHA1(a83bf1c55483027616e1abe4fe562577f6f874d6) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps43 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps43",    0x08000, 0x8000, CRC(8a043d97) SHA1(d1dc2c0fc560137072a006225de22f718e439110) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps43",    0x00000, 0x2000, CRC(034f063c) SHA1(3e5251551193a2b0474665a2d1f645e5c6be000d) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps44 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps44",    0x08000, 0x8000, CRC(2d9bfc93) SHA1(03b7fc7373dff1da3acc31a06af6c9bd2830cacd) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps44",    0x00000, 0x2000, CRC(5eaf89de) SHA1(79319a4a455b0ce81de6e5f9336d215f6ee3a7b4) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps45 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps45",    0x08000, 0x8000, CRC(48dcf7f9) SHA1(de73b057a607cbb5c7c63b615f8ca1a5f79d420b) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps46 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps46",    0x08000, 0x8000, CRC(69607c24) SHA1(a90272033ac700c5569aaf4049eb9572ccc3a9ad) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps46",    0x00000, 0x2000, CRC(6eab0daf) SHA1(dc5624e6808e78aae8116dc967687c8f793a61d1) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps47 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps17",    0x08000, 0x8000, CRC(885acc2b) SHA1(448ed0051cb89a9d432b82e28a5e48a666f64fd0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps47",    0x00000, 0x2000, CRC(25a1f9ee) SHA1(8ee3b3244f483199c26cc0dde4df9aaacde3e1e8) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps48 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps48",    0x08000, 0x8000, CRC(1d2f0bfa) SHA1(a5dc6f29a6eccc9e1dcea85bd5d8178700fd8c64) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps48",    0x00000, 0x2000, CRC(8ec8c986) SHA1(abb85d129d2368aaaed924d1c3b53cc784b81a30) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps49 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps49",    0x08000, 0x8000, CRC(5396db08) SHA1(6234baf5a35c82cb7ab931d67829f0399d2efbf2) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps49",    0x00000, 0x2000, CRC(e4615dd5) SHA1(2776057d8bad8cc31eb60244e1c6d39173bbb1a5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps50 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps50",    0x08000, 0x8000, CRC(2321a47f) SHA1(cd65791584f76bc117fcc6545297e8ee23893e0a) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps51 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps51",    0x08000, 0x8000, CRC(9246fde9) SHA1(21b8ac3c119dd0160a442d5363b5a3898dc8c476) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps51",    0x00000, 0x2000, CRC(57b68cc5) SHA1(884f2598244a0fab6edb498a03d057d638be9639) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps52 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps52",    0x08000, 0x8000, CRC(e19cee9d) SHA1(d7fdc39ad48226905ee8d68cc9f904f952f0ae84) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps53 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps53",    0x08000, 0x8000, CRC(718876fe) SHA1(38738de146a874db0efaa003291ccf686d4d5041) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps53",    0x00000, 0x2000, CRC(113dae1c) SHA1(4e593311efc582772159e18cb3063d5035b80955) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps54 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps54",    0x08000, 0x8000, CRC(89b5939a) SHA1(c9a48486c8434ccd1408272b1d579abb5cbeb7d0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps54",    0x00000, 0x2000, CRC(ffc9a3f7) SHA1(712dda159220aa35bc7c47cff89b82e546011921) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps55 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps55",    0x08000, 0x8000, CRC(50f5f554) SHA1(1c67d813fc64ff18097e605aabc4289dec834866) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps55",    0x00000, 0x2000, CRC(59e9b87d) SHA1(13b8e811275cf85ba43ab190df1c62ca8c354370) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps56 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps17",    0x08000, 0x8000, CRC(885acc2b) SHA1(448ed0051cb89a9d432b82e28a5e48a666f64fd0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps56",    0x00000, 0x2000, CRC(b9a1218e) SHA1(2f3cfde73c25640517413f3712a744886f18fd2c) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps57 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps17",    0x08000, 0x8000, CRC(885acc2b) SHA1(448ed0051cb89a9d432b82e28a5e48a666f64fd0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps57",    0x00000, 0x2000, CRC(b0f35afb) SHA1(53de8232cdc52ab7f517e4cbf1a3c5bdf5faef1a) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps58 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps58",    0x08000, 0x8000, CRC(f6844d13) SHA1(ea65580e42de7226eec3ee75621bb73e6f9f199f) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps58",    0x00000, 0x2000, CRC(b7e74b76) SHA1(49d13b90c93cd0700c510a1314858218a1ffa29a) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps59 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps17",    0x08000, 0x8000, CRC(885acc2b) SHA1(448ed0051cb89a9d432b82e28a5e48a666f64fd0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps59",    0x00000, 0x2000, CRC(e9a2f860) SHA1(d6c81524bf7e667cc84f9f78547f3a6687db4ce1) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps60 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps60",    0x08000, 0x8000, CRC(dff85f0b) SHA1(5f16f6af2becd7daecbf4c61b3dc40402cb843cf) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps60",    0x00000, 0x2000, CRC(16e2a73b) SHA1(6902271155e5135c2105d394154e228b464cf429) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps61 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps61",    0x08000, 0x8000, CRC(5ad5c7bb) SHA1(790e9761231823783a7bd110b822d76a20f001b2) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps61",    0x00000, 0x2000, CRC(c0665808) SHA1(9794b16c2af21fcb6e07c7eefdc34c4e4f444997) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps62 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps62",    0x08000, 0x8000, CRC(ef78ca04) SHA1(5a6b0b4867e9e0877c26400dcf5509125d267f9c) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps62",    0x00000, 0x2000, CRC(00685ea2) SHA1(38dff92028442ce512837eefb3e382fbb6092b80) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps63 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps63",    0x08000, 0x8000, CRC(19db1398) SHA1(7db2e6498892f6e76fa1ecf9b3cf1611eb05b38a) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps63",    0x00000, 0x2000, CRC(50048792) SHA1(db1d8fb841484f4687aa61662178e979e02566dc) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps64 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps64",    0x08000, 0x8000, CRC(0d8f4f17) SHA1(073ea5edda5196120e33a40e57453d096e2445c6) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps65 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps65",    0x08000, 0x8000, CRC(e08edc63) SHA1(b0321d3b8945d7455ae800fb6d48348685213168) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps65",    0x00000, 0x2000, CRC(071f2111) SHA1(b42f34eb5c3d39a07a98551c4ac83df231ac0fb7) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps66 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps66",    0x08000, 0x8000, CRC(ba6357fb) SHA1(e58aab0aa35b1281252004bdd81d0d98164a0d00) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps67 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps67",    0x08000, 0x8000, CRC(d8ccedbe) SHA1(30a20fd7745ea0262a76e48d9212539d6ce67a7a) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps67",    0x00000, 0x2000, CRC(2135c6d1) SHA1(f2a5eb06da4d054c75a4bdd7183058f78f2802a3) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps68 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps68",    0x08000, 0x8000, CRC(2bc0a7a9) SHA1(728b0ccd139865c66a282894c1f888650b22bfda) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps68",    0x00000, 0x2000, CRC(d9ab0057) SHA1(ade3a520e0c2257d4871ba9e290fe3e3dca03bbc) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps69 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps69",    0x08000, 0x8000, CRC(d3ef5902) SHA1(f52f3092d890772603c876bf7512f7c41a86e1fb) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps69",    0x00000, 0x2000, CRC(490679c2) SHA1(c7dbd4247e19f4fed9c106fc598119dd4f0c2c7a) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps70 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps70",    0x08000, 0x8000, CRC(f0d05e82) SHA1(ee4acbda7d82b820b449a6b4fa0ad95a95ccc685) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps70",    0x00000, 0x2000, CRC(8c01e7f4) SHA1(af4e44591c2e99401ff4d962575a631f1fe6a0db) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps71 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps71",    0x08000, 0x8000, CRC(12d66e24) SHA1(f32e17a76ef772dee42813af7fc167e8ae1813c0) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm",    0x00000, 0x2000, CRC(867b51ad) SHA1(394badaf0b0bdd0ea279a1bca89a9d9ddc00b1b5) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps72 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps72",    0x08000, 0x8000, CRC(a4956d9a) SHA1(69f1b70ad797e923b94a8a6b24673a7a565508f2) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps72",    0x00000, 0x2000, CRC(fc5cd67e) SHA1(3923b3498b93c028ec5b0babe58af7de5ef11c37) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps73 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps73",    0x08000, 0x8000, CRC(23958ac3) SHA1(0551e4df66f73880ec7a666af839ec91d7391555) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps72",    0x00000, 0x2000, CRC(fc5cd67e) SHA1(3923b3498b93c028ec5b0babe58af7de5ef11c37) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

ROM_START( pc_smb_ps74 )
	BIOS_CPU
	ROM_LOAD( "u3sm",    0x0c000, 0x2000, CRC(4b5f717d) SHA1(c39c90f9503c4692af4a8fdb3e18ef7cf04e897f) )
	BIOS_GFX

	ROM_REGION( 0x10000, "cart", 0 )
	ROM_LOAD( "u1sm_ps74",    0x08000, 0x8000, CRC(c28daf74) SHA1(6b2bbd0ffd8bcc36daafd2d11696a789e1b269eb) )

	ROM_REGION( 0x02000, "gfx2", 0 )   
	ROM_LOAD( "u2sm_ps74",    0x00000, 0x2000, CRC(a94709ce) SHA1(6b697ee59252f0c4e30efa1e0b62d80876ed6712) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(bd82d775) SHA1(e15c369d638156eeb0cd141aeeec877c62810b64) )
ROM_END

/******************
 Super Mario Bros 2
********************/

ROM_START( pc_smb2_ps01 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps01",   0x10000, 0x20000, CRC(de4ed9dc) SHA1(c2234d608267849fbe2fe837478a1d506ba68b19) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1_ps01",    0x00000, 0x20000, CRC(1d24625c) SHA1(1e9abdfb9237b4c5699eadd13e9b0faaa186fd48) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps02 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps02",   0x10000, 0x20000, CRC(e6f152c0) SHA1(83826a57094eafebbe7e5a43075774710d2d590f) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1_ps02",    0x00000, 0x20000, CRC(43fd715f) SHA1(2f789330e289f5315d8bc0bc013d4ff4e3158a01) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps03 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps03",   0x10000, 0x20000, CRC(5a8ee185) SHA1(5d96c5cb48afada79df49dafd6496db95c7fec3a) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1_ps03",    0x00000, 0x20000, CRC(f1db4ecf) SHA1(5469b891346f8432d6a74c42ddfce827efcfba04) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps04 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps04",   0x10000, 0x20000, CRC(fd4fb826) SHA1(5a931a9f93869716ef18e25a2c9bc77b9db57175) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1",    0x00000, 0x20000, CRC(f2ba1170) SHA1(d9976b677ad222b76fbdaf31713374e2f283d44e) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps05 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps05",   0x10000, 0x20000, CRC(e8fe2bd0) SHA1(ba0f3037e859f670f16c6f52417f786cb6844452) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1_ps05",    0x00000, 0x20000, CRC(107e7be8) SHA1(ed056afaa512d1b98665d6341c4f175c3b30c8ff) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps06 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps06",   0x10000, 0x20000, CRC(2a057850) SHA1(1ed183538ae0b8523a8d3d94e4d92f951010ab8f) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1",    0x00000, 0x20000, CRC(f2ba1170) SHA1(d9976b677ad222b76fbdaf31713374e2f283d44e) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps07 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps07",   0x10000, 0x20000, CRC(e1c209a3) SHA1(00bbea1608bb0875120e5d053cd4b3e860c6a75e) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1",    0x00000, 0x20000, CRC(f2ba1170) SHA1(d9976b677ad222b76fbdaf31713374e2f283d44e) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

ROM_START( pc_smb2_ps08 )
	BIOS_CPU
	ROM_LOAD( "mw-u3",   0x0c000, 0x2000, CRC(beaeb43a) SHA1(c7dd186d6167e39924a000eb80bd33beedb2b8c8) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "mw-u5_ps08",   0x10000, 0x20000, CRC(c283e72d) SHA1(9c399f8a6660e57269de2bfe619559be858daed4) )
	ROM_RELOAD(          0x30000, 0x20000 )

	ROM_REGION( 0x020000, "gfx2", 0 )   
	ROM_LOAD( "mw-u1_ps08",    0x00000, 0x20000, CRC(ce6e51d3) SHA1(f0b6fcc21d87891de69632e543df5435cc70e16e) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(372f4e84) SHA1(cdf221d49f3b454997d696f213d60b5dce0ce9fb) )
ROM_END

/******************
 Super Mario Bros 3
********************/

ROM_START( pc_smb3_ps01 ) 
	BIOS_CPU
	ROM_LOAD( "u3um",    0x0c000, 0x2000, CRC(45e92f7f) SHA1(9071d5f18639ac58d6d4d72674856f9ecab911f0) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "u4um_ps01",    0x10000, 0x20000, CRC(80a11ad0) SHA1(1e2c01834d2dc9334030a0858ee8f8ffc43eb2e2) )
	ROM_LOAD( "u5um_ps01",    0x30000, 0x20000, CRC(09daacf2) SHA1(b8d53ad91cf554fedf11167d56c082961d7b989a) ) 

	ROM_REGION( 0x020000, "gfx2", 0 )
	ROM_LOAD( "u1um",    0x00000, 0x20000, CRC(c2928c49) SHA1(2697d1f21b72a6d8e7d2a2d2c51c9c5550f68b56) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(e48f4945) SHA1(66fe537cfe540317d6194847321ce4a9bdf0bba4) )
ROM_END

ROM_START( pc_smb3_ps02 ) 
	BIOS_CPU
	ROM_LOAD( "u3um",    0x0c000, 0x2000, CRC(45e92f7f) SHA1(9071d5f18639ac58d6d4d72674856f9ecab911f0) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "u4um_ps02",    0x10000, 0x20000, CRC(51cb8ff4) SHA1(4169278b2ed45191cc19d824bb9cbfc346e314e5) )
	ROM_LOAD( "u5um_ps02",    0x30000, 0x20000, CRC(0e2268cb) SHA1(992a3a9fc0e1a940af3b5af3888754f18fb8a57f) ) 

	ROM_REGION( 0x020000, "gfx2", 0 )
	ROM_LOAD( "u1um_ps02",    0x00000, 0x20000, CRC(84742a1e) SHA1(287d2b3f9f7ab9d3deb26a8d2d0c31f84f8b5fff) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(e48f4945) SHA1(66fe537cfe540317d6194847321ce4a9bdf0bba4) )
ROM_END

ROM_START( pc_smb3_ps03 ) 
	BIOS_CPU
	ROM_LOAD( "u3um",    0x0c000, 0x2000, CRC(45e92f7f) SHA1(9071d5f18639ac58d6d4d72674856f9ecab911f0) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "u4um_ps03",    0x10000, 0x20000, CRC(e760a21e) SHA1(398676f59f3e189b0b6b68b864820fa3cb26e474) )
	ROM_LOAD( "u5um_ps03",    0x30000, 0x20000, CRC(82ed2a33) SHA1(6bc7b838b52ed8a32553f8b2f90f950095c5cc54) ) 

	ROM_REGION( 0x020000, "gfx2", 0 )
	ROM_LOAD( "u1um_ps03",    0x00000, 0x20000, CRC(a1d5edf4) SHA1(4aee98d6f15430c03e6b549e4b93ba64bba47e18) )

	ROM_REGION( 0x10, "rp5h01", 0 )
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(e48f4945) SHA1(66fe537cfe540317d6194847321ce4a9bdf0bba4) )
ROM_END

ROM_START( pc_smb3_ps04 ) 
	BIOS_CPU
	ROM_LOAD( "u3um",    0x0c000, 0x2000, CRC(45e92f7f) SHA1(9071d5f18639ac58d6d4d72674856f9ecab911f0) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "u4um",    0x10000, 0x20000, CRC(590b4d7c) SHA1(ac45940b71215a3a48983e22e1c7e71a71642b91) ) 
	ROM_LOAD( "u5um_ps04",    0x30000, 0x20000, CRC(95625dc7) SHA1(363cfe756b06d7239d2d28c76d0dd66dd332fe41) ) 

	ROM_REGION( 0x020000, "gfx2", 0 )  
	ROM_LOAD( "u1um",    0x00000, 0x20000, CRC(c2928c49) SHA1(2697d1f21b72a6d8e7d2a2d2c51c9c5550f68b56) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(e48f4945) SHA1(66fe537cfe540317d6194847321ce4a9bdf0bba4) )
ROM_END

ROM_START( pc_smb3_ps05 ) 
	BIOS_CPU
	ROM_LOAD( "u3um",    0x0c000, 0x2000, CRC(45e92f7f) SHA1(9071d5f18639ac58d6d4d72674856f9ecab911f0) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "u4um_ps05",    0x10000, 0x20000, CRC(33f07d65) SHA1(5068e53a8c6fa993261de77c9b27da31611155a1) ) 
	ROM_LOAD( "u5um_ps05",    0x30000, 0x20000, CRC(8100803b) SHA1(ef4edf72f6b719ad0cbcc0a43a5903441196b79f) ) 

	ROM_REGION( 0x020000, "gfx2", 0 )  
	ROM_LOAD( "u1um_ps05",    0x00000, 0x20000, CRC(a0ae2b4b) SHA1(5e026ad8a6b2a8120e386471d5178625bda04525) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(e48f4945) SHA1(66fe537cfe540317d6194847321ce4a9bdf0bba4) )
ROM_END

ROM_START( pc_smb3_ps06 ) 
	BIOS_CPU
	ROM_LOAD( "u3um",    0x0c000, 0x2000, CRC(45e92f7f) SHA1(9071d5f18639ac58d6d4d72674856f9ecab911f0) )
	BIOS_GFX

	ROM_REGION( 0x50000, "cart", 0 )
	ROM_LOAD( "u4um_ps06",    0x10000, 0x20000, CRC(64cb6e51) SHA1(b8150233b10c75f0a1e14138f736b330600e6383) ) 
	ROM_LOAD( "u5um_ps06",    0x30000, 0x20000, CRC(1d6041b9) SHA1(f17eef1669d81d192b15f877afac6bc710b7a5ab) ) 

	ROM_REGION( 0x020000, "gfx2", 0 )  
	ROM_LOAD( "u1um_ps05",    0x00000, 0x20000, CRC(a0ae2b4b) SHA1(5e026ad8a6b2a8120e386471d5178625bda04525) )

	ROM_REGION( 0x10, "rp5h01", 0 ) 
	ROM_LOAD( "security.prm", 0x00, 0x10, CRC(e48f4945) SHA1(66fe537cfe540317d6194847321ce4a9bdf0bba4) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Super Mario Bros
GAME( 2011, pc_smb_ps01,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Air Jump 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps02,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Air Swimming 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps03,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Always An Adult 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps04,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Always Bullet 2013-04-15)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps05,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Another Revision 2011-12-23)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps06,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Babio - Super Baby Bros 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps07,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Blocker 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps08,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Bullet 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps09,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Burn The Bank 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps10,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Change Gold Coin Attribute To Vine 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb_ps11,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Chinese Version 1 2012-01-15)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb_ps12,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Chinese Version 2 2012-01-15)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps13,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Original Edition japonés 2015-07-06)(PlayChoice-10)", 0 )
GAME( 2020, pc_smb_ps14,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Cresent 2 2020-08-11)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps15,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Double Jump 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps16,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Downmario 2011-12-23)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps17,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Enemy Hidden Version 2011-12-23)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps18,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Forced Scroll Fast 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps19,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Forever 2011-12-23)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps20,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Girl Mario 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps21,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Golden Body Invincible 2013-12-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps22,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Goomba's Revenge 2013-03-17)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps23,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Happy Experience Of Super Mario 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps24,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Hidden Coins And 1up Show Up 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps25,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Infinite Jump 2013-04-15)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps26,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Inverted Water Pipe 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps27,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Invincible Jumping Off The Cliff Undead Beta Version 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps28,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (kamikaze Marie Dress Version 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb_ps29,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (kamikazev 3 2012-01-13)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps30,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Kirby 2013-03-16)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps31,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Lady Opera - Swf Panic v1 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps32,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Lady Opera - Swf Panic v1 [No ani] 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps33,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Legend Of Zelda 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps34,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Life Limit Correction 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps35,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Limit Modification 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps36,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Liu Mingjia's Adventure 2011-12-23)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps37,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Luigi's Chronicles - Googie's 5th A 2013-03-17)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps38,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Luigi's Chronicles - Googie's 5th B 2013-03-17)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps39,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mario Fro 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps40,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mario's Moon Adventure 2013-03-16)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps41,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mario In Zebes 2015-09-20)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb_ps42,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mary 2012-09-28)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps43,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Midget 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps44,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mikamari Kanji 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps45,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mods Invincible 2011-12-23)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps46,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Mortal Kombat Bros Sub Zero Mythologies Quest 2013-03-16)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps47,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Naked Mario 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps48,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Nameless B 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps49,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Pandamar 2013-03-17)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps50,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Row Of Cherry Trees (Sakura) 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps51,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Sakura 2015-09-20)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb_ps52,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Self-Running Full Version 2012-01-03)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps53,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (X9 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps54,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Bjc 2013-03-17)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps55,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Sorrowful 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps56,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Strange Mario Bros 2015-09-20)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps57,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Super Bizzario Bros 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps58,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Super Catholic Bros 2015-09-20)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps59,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Super Cigarette Bros 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps60,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Super Little Bird Nightmare Edition 2015-09-20)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps61,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Super Tricky Mario 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps62,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (The Enemy Will Fire Bullets (Discontinuous) 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps63,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (The More The Better 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps64,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (The Second Generation Japanese Version Of The Red Piranha 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps65,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Thorn 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps66,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Time Loop 2013-04-15)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps67,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Time World Score 2011-12-30)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps68,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Tl05 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps69,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Unlimited Jump 2011-12-28)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps70,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Upside-Down Map 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb_ps71,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Various Enhancements 2011-12-29)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps72,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Yoona(cu)Hard 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb_ps73,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Yoona(cu)Normal 2013-03-19)(PlayChoice-10)", 0 )
GAME( 2015, pc_smb_ps74,    pc_smb, playch10, playch10, playch10_state, init_playch10, ROT0, "hack",                                 "Super Mario Bros. (Zzt 2015-09-20)(PlayChoice-10)", 0 )
//Super Mario Bros. 2
GAME( 2014, pc_smb2_ps01,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (107 Hack 2014-04-27)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb2_ps02,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (458 Hack 2012-01-22)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb2_ps03,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (760 Hack 2012-01-22)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb2_ps04,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (All Floating 2013-11-25)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb2_ps05,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (Edition Chinese 2012-04-13)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb2_ps06,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (Mod Invincible 2012-01-03)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb2_ps07,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (Princess Floating Infinitely 2012-01-03)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb2_ps08,   pc_smb2,playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 2 (U.S.A Edition 2013-11-25)(PlayChoice-10)", 0 )
//Super Mario Bros. 3
GAME( 2012, pc_smb3_ps01,  pc_smb3, playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 3 (135 Hack 2012-01-23)(PlayChoice-10)", 0 )
GAME( 2014, pc_smb3_ps02,  pc_smb3, playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 3 (320 Hack 2014-09-29)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb3_ps03,  pc_smb3, playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 3 (412 Hack 2012-01-22)(PlayChoice-10)", 0 )
GAME( 2012, pc_smb3_ps04,  pc_smb3, playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 3 (Key Substitution Version 2013-11-25)(PlayChoice-10)", 0 )
GAME( 2013, pc_smb3_ps05,  pc_smb3, playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 3 (Mod Invincible 2013-12-19)(PlayChoice-10)", 0 )
GAME( 2011, pc_smb3_ps06,  pc_smb3, playch10, playch10, playch10_state, init_pcgboard, ROT0, "hack",                                 "Super Mario Bros. 3 (Tiaobao Edition 2011-11-02)(PlayChoice-10)", 0 )

