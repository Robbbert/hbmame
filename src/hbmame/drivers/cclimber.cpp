// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/cclimber.cpp"

// Crazy Climber
ROM_START( cclimbrm )
	ROM_REGION( 0x6000, "maincpu", 0 )
	ROM_LOAD( "cc11m",         0x0000, 0x1000, CRC(a7d58b7e) SHA1(83a21b1a731895c84f1f3604f36b21b172bb144b) )
	ROM_LOAD( "cc10",          0x1000, 0x1000, CRC(b3c26cef) SHA1(f52cb5482c12a9c5fb56e2e2aec7cab0ed23e5a5) )
	ROM_LOAD( "cc09",          0x2000, 0x1000, CRC(6db0879c) SHA1(c0ba1976c1dcd6edadd78073173a26851ae8dd4f) )
	ROM_LOAD( "cc08",          0x3000, 0x1000, CRC(f48c5fe3) SHA1(79072bbbf37387998ffd031afe8eb569a16fa9bd) )
	ROM_LOAD( "cc07m",         0x4000, 0x1000, CRC(a417e7a3) SHA1(0cc8e306268119e11d8375749147cb7c1a831bd8) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "cc06",         0x0000, 0x0800, CRC(481b64cc) SHA1(3f35c545fc784ed4f969aba2d7be6e13a5ae32b7) )
	ROM_LOAD( "cc04",         0x2000, 0x0800, CRC(332347cb) SHA1(4115ca32af73f1791635b7d9e093bf77088a8222) )
	ROM_LOAD( "cc05",         0x1000, 0x0800, CRC(2c33b760) SHA1(2edea8fe13376fbd51a5586d97aba3b30d78e94b) )
	ROM_LOAD( "cc03",         0x3000, 0x0800, CRC(4e4b3658) SHA1(0d39a8cb5cd6cf06008be60707f9b277a8a32a2d) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "cc02",         0x0000, 0x0800, CRC(14f3ecc9) SHA1(a1b5121abfbe8f07580eb3fa6384352d239a3d75) )
	ROM_LOAD( "cc01",         0x0800, 0x0800, CRC(21c0f9fb) SHA1(44fad56d302a439257216ddac9fd62b3666589f1) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "cclimber.pr1", 0x0000, 0x0020, CRC(751c3325) SHA1(edce2bc883996c1d72dc6c1c9f62799b162d415a) )
	ROM_LOAD( "cclimber.pr2", 0x0020, 0x0020, CRC(ab1940fa) SHA1(8d98e05cbaa6f55770c12e0a9a8ed9c73cc54423) )
	ROM_LOAD( "cclimber.pr3", 0x0040, 0x0020, CRC(71317756) SHA1(1195f0a037e379cc1a3c0314cb746f5cd2bffe50) )

	ROM_REGION( 0x2000, "cclimber_audio:samples", 0 )
	ROM_LOAD( "cc13",         0x0000, 0x1000, CRC(e0042f75) SHA1(86cb31b110742a0f7ae33052c88f42d00deb5468) )
	ROM_LOAD( "cc12",         0x1000, 0x1000, CRC(5da13aaa) SHA1(b2d41e69435d09c456648a10e33f5e1fbb0bc64c) )
ROM_END


// Crazy Kong
ROM_START( ckong01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "d05-07.bin",   0x0000, 0x1000, CRC(b27df032) SHA1(57f9be139c610405e3c2fddd7093dfb1277e450e) )
	ROM_LOAD( "f05-08.bin",   0x1000, 0x1000, CRC(5dc1aaba) SHA1(42b9e5946ffce7c156d114bde68f37c2c34853c4) )
	ROM_LOAD( "h05-09.bin",   0x2000, 0x1000, CRC(c9054c94) SHA1(1aa08d2501ee620759fd5c111e12f6d432c25294) )
	ROM_LOAD( "k05-10.bin",   0x3000, 0x1000, CRC(069c4797) SHA1(03be185e6914ec7f3770ce3da4eb49cdb97adc85) )
	ROM_LOAD( "l05-11.bin",   0x4000, 0x1000, CRC(ae159192) SHA1(d467256a3a366e246243e7828ff4a45d4c146e2c) )
	ROM_LOAD( "n05-12.bin",   0x5000, 0x1000, CRC(966bc9ab) SHA1(4434fc620169ffea1b1f227b61674e1daf79b54b) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "n11-06.bin",   0x0000, 0x1000, CRC(2dcedd12) SHA1(dfdcfc21bcba7c8e148ee54daae511ca78c58e70) )
	ROM_LOAD( "l11-05.bin",   0x1000, 0x1000, CRC(fa7cbd91) SHA1(0208d2ebc59f3600005476b6987472685bc99d67) )
	ROM_LOAD( "k11-04.bin",   0x2000, 0x1000, CRC(3375b3bd) SHA1(a00b3c31cff123aab6ac0833aabfdd663302971a) )
	ROM_LOAD( "h11-03.bin",   0x3000, 0x1000, CRC(5655cc11) SHA1(5195e9b2a60c54280b48b32ee8248090904dbc51) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "c11-02f.bin",  0x0000, 0x0800, CRC(881ed76e) SHA1(e516d898b68da7040f7fb8c7dbc47e0526349e4c) )
	ROM_LOAD( "a11-01f.bin",  0x0800, 0x0800, CRC(d49aa505) SHA1(7dec6d62e874fb23f827ffddb72d85777a5c39f7) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "prom.v6",      0x0000, 0x0020, CRC(b3fc1505) SHA1(5b94adde0428a26b815c7eb9b3f3716470d349c7) )
	ROM_LOAD( "prom.u6",      0x0020, 0x0020, CRC(26aada9e) SHA1(f59645e606ea4f0dd0fc4ea47dd03f526c534941) )
	ROM_LOAD( "prom.t6",      0x0040, 0x0020, CRC(676b3166) SHA1(29b9434cd34d43ea5664e436e2a24b54f8d88aac) )

	ROM_REGION( 0x2000, "cclimber_audio:samples", 0 )
	ROM_LOAD( "falcon13",     0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "falcon12",     0x1000, 0x1000, CRC(9003ffbd) SHA1(fd016056aabc23957643f37230f03842294f795e) )
ROM_END


// Crazy Kong Part II
ROM_START( ckongpt2s01 )
	ROM_REGION( 0x6000, "maincpu", 0 )
	ROM_LOAD( "7s01.5d",      0x0000, 0x1000, CRC(8dcf7df0) SHA1(78807b5d97c2a3cb0572774226ecbb4deeb04473) )
	ROM_LOAD( "8s01.5e",      0x1000, 0x1000, CRC(8930f958) SHA1(568e4825f1488a93c36dd210f8666efc90eddf1a) )
	ROM_LOAD( "9.5h",         0x2000, 0x1000, CRC(c9054c94) SHA1(1aa08d2501ee620759fd5c111e12f6d432c25294) )
	ROM_LOAD( "10s01.5k",     0x3000, 0x1000, CRC(a12afa47) SHA1(8ccf76efbbee2645f11b88a6101a6216a464bd8c) )
	ROM_LOAD( "11s01.5l",     0x4000, 0x1000, CRC(c6f2913e) SHA1(5ff0cb5b9c4616cdf870a45cae5833cb0c3b3556) )
	ROM_LOAD( "12s01.5n",     0x5000, 0x1000, CRC(34c7a93b) SHA1(10897cdb946b520f18021fba07d559a9cc0fb4de) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "6s01.11n",     0x0000, 0x1000, CRC(b6201ddb) SHA1(4aa1ed5273aedc78e55e9401f6f17705a264aa58) )
	ROM_LOAD( "5s01.11l",     0x1000, 0x1000, CRC(090b509a) SHA1(e83a4f05a622e04b712244bb146c734fc5a4ae92) )
	ROM_LOAD( "4s01.11k",     0x2000, 0x1000, CRC(a9e9ebc2) SHA1(b631ee88061e3889ea65e9c1d3448e64d98a6412) )
	ROM_LOAD( "3s01.11h",     0x3000, 0x1000, CRC(bac1e8a5) SHA1(a06d17e3a18709e82283bcf641989fb4dcff9d53) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "2s01.11c",     0x0000, 0x0800, CRC(eeb3360a) SHA1(3dcf6a2c35b46fe06af008083900d305f18df15e) )
	ROM_LOAD( "1s01.11a",     0x0800, 0x0800, CRC(b8873065) SHA1(24677b74136bf5c064f7de2b62ad6edf9f14f2e6) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "s01.v6",       0x0000, 0x0020, CRC(fc2b00f0) SHA1(56b6f78324b51ad7ba8a9da49b9f748b1a9a55e3) )
	ROM_LOAD( "s01.u6",       0x0020, 0x0020, CRC(d488c634) SHA1(2c33cec23528d6eae01ab8207078e256de0f8f36) )
	ROM_LOAD( "s01.t6",       0x0040, 0x0020, CRC(cccec69c) SHA1(a8681ec247342ea18d3823d6c2475cf3b9fbcfb9) )

	ROM_REGION( 0x2000, "cclimber_audio:samples", 0 )
	ROM_LOAD( "14.5s",        0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "13.5p",        0x1000, 0x1000, CRC(9003ffbd) SHA1(fd016056aabc23957643f37230f03842294f795e) )
ROM_END


ROM_START( ckongpt2s02 ) // Sock Master hack of ckongpt2a
	ROM_REGION( 0x6000, "maincpu", 0 )
	ROM_LOAD( "7s02.5d",         0x0000, 0x1000, CRC(d75ef02a) SHA1(37c71cd00dfda1339a467a59866301f397a1782f) )
	ROM_LOAD( "8s02.5e",         0x1000, 0x1000, CRC(345c84d1) SHA1(d154b68fae9f797b4fb093daed0daaed7fb79ebf) )
	ROM_LOAD( "9s02.5h",         0x2000, 0x1000, CRC(de8c097d) SHA1(72abdc16583d37c013b8438524d45d905ef2c271) )
	ROM_LOAD( "10s02.dat",       0x3000, 0x1000, CRC(e75efd30) SHA1(6c11c50da307861aaed83ea885616be08d4a7ebd) )
	ROM_LOAD( "11s02.5l",        0x4000, 0x1000, CRC(31609813) SHA1(19d7f75f2de2a3632a47b45e733495d476090467) )
	ROM_LOAD( "12s02.5n",        0x5000, 0x1000, CRC(75cb6387) SHA1(6701b3eee6fc44852d1b36ca52e214811171009b) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "6.11n",        0x0000, 0x1000, CRC(2dcedd12) SHA1(dfdcfc21bcba7c8e148ee54daae511ca78c58e70) )
	ROM_LOAD( "5.11l",        0x1000, 0x1000, CRC(fa7cbd91) SHA1(0208d2ebc59f3600005476b6987472685bc99d67) )
	ROM_LOAD( "4.11k",        0x2000, 0x1000, CRC(3375b3bd) SHA1(a00b3c31cff123aab6ac0833aabfdd663302971a) )
	ROM_LOAD( "3.11h",        0x3000, 0x1000, CRC(5655cc11) SHA1(5195e9b2a60c54280b48b32ee8248090904dbc51) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "2.11c",        0x0000, 0x0800, CRC(d1352c31) SHA1(da726a63a8be830d695afeddc1717749af8c9d47) )
	ROM_LOAD( "1.11a",        0x0800, 0x0800, CRC(a7a2fdbd) SHA1(529865f8bbfbdbbf34ac39c70ef17e6d5bd0f845) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "prom.v6",      0x0000, 0x0020, CRC(b3fc1505) SHA1(5b94adde0428a26b815c7eb9b3f3716470d349c7) )
	ROM_LOAD( "prom.u6",      0x0020, 0x0020, CRC(26aada9e) SHA1(f59645e606ea4f0dd0fc4ea47dd03f526c534941) )
	ROM_LOAD( "prom.t6",      0x0040, 0x0020, CRC(676b3166) SHA1(29b9434cd34d43ea5664e436e2a24b54f8d88aac) )

	ROM_REGION( 0x2000, "cclimber_audio:samples", 0 )
	ROM_LOAD( "14.5s",        0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "13.5p",        0x1000, 0x1000, CRC(9003ffbd) SHA1(fd016056aabc23957643f37230f03842294f795e) )
ROM_END


// Monkey Donkey
/* This is a gfx hack of monkeyd. Should monkeyd get correct colours, it must be applied here too. */
ROM_START( tongypac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "falcon7",      0x0000, 0x1000, CRC(2171cac3) SHA1(7b18bfe44c32fb64b675bbbe2136344522c79b09) )
	ROM_LOAD( "falcon8",      0x1000, 0x1000, CRC(88b83ff7) SHA1(4afc494cc264aaa4614da6aed02ce062d9c20850) )
	ROM_LOAD( "falcon9",      0x2000, 0x1000, CRC(cff2af47) SHA1(1757428cefad13855a623162101ec01c04006c94) )
	ROM_LOAD( "ck10.bin",     0x3000, 0x1000, CRC(520fa4de) SHA1(6edbaf727756cd33bde94492d72654aa12dbd7e1) )
	ROM_LOAD( "md5l.bin",     0x4000, 0x1000, CRC(d1db1bb0) SHA1(fe7d700c7f9eca9c389be3717ebebf3e7dc63aa2) )
	/* no ROM at 5000 */

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "tpac6.bin",    0x0000, 0x1000, CRC(918e3c52) SHA1(bc630759a525615aba1a06a0781e26a1a5c2c377) )
	ROM_LOAD( "tpac5.bin",    0x1000, 0x1000, CRC(2bbc69e7) SHA1(5741aa6944e9b936289368d533c2b19d34320260) )
	ROM_LOAD( "tpac4.bin",    0x2000, 0x1000, CRC(8d5890b6) SHA1(92d48d75084df2e2a3fa9d92a3c6c059f0781d81) )
	ROM_LOAD( "tpac3.bin",    0x3000, 0x1000, CRC(6b16145b) SHA1(ef17fad07b81c9dae5c6e16a86afc4a8f6a7ff8c) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "falcon2",      0x0000, 0x0800, CRC(f67c80f1) SHA1(d1fbcce1b6242f810e106ff50812636e3168ebc1) )
	ROM_LOAD( "falcon1",      0x0800, 0x0800, CRC(80eb517d) SHA1(fef4111f656c58b28e7eac5aa5b5cc7e07ccb2fd) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "ck6v.bin",     0x0000, 0x0020, CRC(751c3325) SHA1(edce2bc883996c1d72dc6c1c9f62799b162d415a) )
	ROM_LOAD( "ck6u.bin",     0x0020, 0x0020, CRC(ab1940fa) SHA1(8d98e05cbaa6f55770c12e0a9a8ed9c73cc54423) )
	ROM_LOAD( "ck6t.bin",     0x0040, 0x0020, CRC(b4e827a5) SHA1(31a5a5ad54417a474d22bb16c473415d99a2b6f1) )
	// These colours are better but far from perfect
//  ROM_LOAD( "prom.v6",      0x0000, 0x0020, CRC(b3fc1505) SHA1(5b94adde0428a26b815c7eb9b3f3716470d349c7) )
//  ROM_LOAD( "prom.u6",      0x0020, 0x0020, CRC(26aada9e) SHA1(f59645e606ea4f0dd0fc4ea47dd03f526c534941) )
//  ROM_LOAD( "prom.t6",      0x0040, 0x0020, CRC(676b3166) SHA1(29b9434cd34d43ea5664e436e2a24b54f8d88aac) )

	ROM_REGION( 0x2000, "cclimber_audio:samples", 0 )
	ROM_LOAD( "falcon13",     0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "ck12.bin",     0x1000, 0x1000, CRC(2eb23b60) SHA1(c9e7dc584562aceb374193655fbacb7df6c9c731) )
ROM_END


GAME( 1980, cclimbrm,  cclimber, cclimberx, cclimber, cclimber_state, init_cclimber, ROT0,   "Tim Arcadecollecting", "Crazy Climber (US - modded)", MACHINE_SUPPORTS_SAVE ) // http://www.arcadecollecting.com/crazy/cchack
GAME( 1981, ckong01,   ckong,    cclimber,  ckong,    cclimber_state, empty_init,    ROT270, "Pinny", "Crazy Kong Part II (Graphic Fix)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, ckongpt2s01,ckongpt2, cclimber,  ckong,    cclimber_state, empty_init,    ROT270, "Paul Goes", "Crazy Kong Part II with DK colours and graphics", MACHINE_SUPPORTS_SAVE )
GAME( 2023, ckongpt2s02,ckongpt2, cclimber,  ckong,    cclimber_state, empty_init,    ROT270, "John Kowalski", "Crazy Kong Part II 2023 revision", MACHINE_SUPPORTS_SAVE )
GAME( 1981, tongypac,  ckong,    cclimber,  ckong,    cclimber_state, empty_init,    ROT270, "Grand Master Peter", "Tongy Paccy", MACHINE_IMPERFECT_COLORS | MACHINE_SUPPORTS_SAVE )

