// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/cclimber.c"

INPUT_PORTS_START( ckonga2 )	/* one less life than normal */
	PORT_INCLUDE( ckong )
	PORT_MODIFY("DSW")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPSETTING(    0x01, "3" )
	PORT_DIPSETTING(    0x02, "4" )
	PORT_DIPSETTING(    0x03, "5" )
INPUT_PORTS_END

INPUT_PORTS_START( ckonga4 )	/* two less lives than normal */
	PORT_INCLUDE( ckong )
	PORT_MODIFY("DSW")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "1" )
	PORT_DIPSETTING(    0x01, "2" )
	PORT_DIPSETTING(    0x02, "3" )
	PORT_DIPSETTING(    0x03, "4" )
INPUT_PORTS_END

ROM_START( cclimbrm )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "cc11m",         0x0000, 0x1000, CRC(a7d58b7e) SHA1(83a21b1a731895c84f1f3604f36b21b172bb144b) )
	ROM_LOAD( "cc10",          0x1000, 0x1000, CRC(b3c26cef) SHA1(f52cb5482c12a9c5fb56e2e2aec7cab0ed23e5a5) )
	ROM_LOAD( "cc09",          0x2000, 0x1000, CRC(6db0879c) SHA1(c0ba1976c1dcd6edadd78073173a26851ae8dd4f) )
	ROM_LOAD( "cc08",          0x3000, 0x1000, CRC(f48c5fe3) SHA1(79072bbbf37387998ffd031afe8eb569a16fa9bd) )
	ROM_LOAD( "cc07m",         0x4000, 0x1000, CRC(a417e7a3) SHA1(0cc8e306268119e11d8375749147cb7c1a831bd8) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "cc06",         0x0000, 0x0800, CRC(481b64cc) SHA1(3f35c545fc784ed4f969aba2d7be6e13a5ae32b7) )
	/* empty hole - Crazy Kong has an additional ROM here */
	ROM_LOAD( "cc04",         0x2000, 0x0800, CRC(332347cb) SHA1(4115ca32af73f1791635b7d9e093bf77088a8222) )
	/* empty hole - Crazy Kong has an additional ROM here */
	ROM_LOAD( "cc05",         0x1000, 0x0800, CRC(2c33b760) SHA1(2edea8fe13376fbd51a5586d97aba3b30d78e94b) )
	/* empty hole - Crazy Kong has an additional ROM here */
	ROM_LOAD( "cc03",         0x3000, 0x0800, CRC(4e4b3658) SHA1(0d39a8cb5cd6cf06008be60707f9b277a8a32a2d) )
	/* empty hole - Crazy Kong has an additional ROM here */

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "cc02",         0x0000, 0x0800, CRC(14f3ecc9) SHA1(a1b5121abfbe8f07580eb3fa6384352d239a3d75) )
	ROM_LOAD( "cc01",         0x0800, 0x0800, CRC(21c0f9fb) SHA1(44fad56d302a439257216ddac9fd62b3666589f1) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "cclimber.pr1", 0x0000, 0x0020, CRC(751c3325) SHA1(edce2bc883996c1d72dc6c1c9f62799b162d415a) )
	ROM_LOAD( "cclimber.pr2", 0x0020, 0x0020, CRC(ab1940fa) SHA1(8d98e05cbaa6f55770c12e0a9a8ed9c73cc54423) )
	ROM_LOAD( "cclimber.pr3", 0x0040, 0x0020, CRC(71317756) SHA1(1195f0a037e379cc1a3c0314cb746f5cd2bffe50) )

	ROM_REGION( 0x2000, "samples", 0 )	/* samples */
	ROM_LOAD( "cc13",         0x0000, 0x1000, CRC(e0042f75) SHA1(86cb31b110742a0f7ae33052c88f42d00deb5468) )
	ROM_LOAD( "cc12",         0x1000, 0x1000, CRC(5da13aaa) SHA1(b2d41e69435d09c456648a10e33f5e1fbb0bc64c) )
ROM_END

/* E376 and E449 are basically the same bootleg of "ckonga". One gfx rom in E376 is changed, and we use it here.
	Changes: One less life than normal, and gfx corruptions on the stage with the incinerator in the middle. */
ROM_START( ckonga2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ckonga2.07",   0x0000, 0x1000, CRC(1c21386f) SHA1(ce1a0c469dd09d81485cede6c8fbaf6299a9decf) )
	ROM_LOAD( "f05-08.bin",   0x1000, 0x1000, CRC(5dc1aaba) SHA1(42b9e5946ffce7c156d114bde68f37c2c34853c4) )
	ROM_LOAD( "h05-09.bin",   0x2000, 0x1000, CRC(c9054c94) SHA1(1aa08d2501ee620759fd5c111e12f6d432c25294) )
	ROM_LOAD( "10.dat",       0x3000, 0x1000, CRC(c3beb501) SHA1(14f49c45fc7c91799034c5a51fca310f0a66b1d7) )
	ROM_LOAD( "l05-11.bin",   0x4000, 0x1000, CRC(ae159192) SHA1(d467256a3a366e246243e7828ff4a45d4c146e2c) )
	ROM_LOAD( "n05-12.bin",   0x5000, 0x1000, CRC(966bc9ab) SHA1(4434fc620169ffea1b1f227b61674e1daf79b54b) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "n11-06.bin",   0x0000, 0x1000, CRC(2dcedd12) SHA1(dfdcfc21bcba7c8e148ee54daae511ca78c58e70) )
	ROM_LOAD( "ckonga2.05",   0x1000, 0x1000, CRC(cd3b5dde) SHA1(2319a2be04d70989b01f4fc703756ba6e1c1f388) )
	ROM_LOAD( "k11-04.bin",   0x2000, 0x1000, CRC(3375b3bd) SHA1(a00b3c31cff123aab6ac0833aabfdd663302971a) )
	ROM_LOAD( "h11-03.bin",   0x3000, 0x1000, CRC(5655cc11) SHA1(5195e9b2a60c54280b48b32ee8248090904dbc51) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "c11-02.bin",   0x0000, 0x0800, CRC(d1352c31) SHA1(da726a63a8be830d695afeddc1717749af8c9d47) )
	ROM_LOAD( "a11-01.bin",   0x0800, 0x0800, CRC(a7a2fdbd) SHA1(529865f8bbfbdbbf34ac39c70ef17e6d5bd0f845) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "prom.v6",      0x0000, 0x0020, CRC(b3fc1505) SHA1(5b94adde0428a26b815c7eb9b3f3716470d349c7) )
	ROM_LOAD( "prom.u6",      0x0020, 0x0020, CRC(26aada9e) SHA1(f59645e606ea4f0dd0fc4ea47dd03f526c534941) )
	ROM_LOAD( "prom.t6",      0x0040, 0x0020, CRC(676b3166) SHA1(29b9434cd34d43ea5664e436e2a24b54f8d88aac) )

	ROM_REGION( 0x2000, "samples", 0 )	/* samples */
	ROM_LOAD( "cc13j.bin",    0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "cc12j.bin",    0x1000, 0x1000, CRC(9003ffbd) SHA1(fd016056aabc23957643f37230f03842294f795e) )
ROM_END

/* E448 and E589 are basically the same bootleg of "ckonga". One prom in E448 is changed, and we use it here.
	Changes: Two less lives than normal. */
ROM_START( ckonga3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ckonga4.07",   0x0000, 0x1000, CRC(9b5e7592) SHA1(c2b44765eef7d6a8f9f1fb8a7cefd011ebf2a330) )
	ROM_LOAD( "f05-08.bin",   0x1000, 0x1000, CRC(5dc1aaba) SHA1(42b9e5946ffce7c156d114bde68f37c2c34853c4) )
	ROM_LOAD( "h05-09.bin",   0x2000, 0x1000, CRC(c9054c94) SHA1(1aa08d2501ee620759fd5c111e12f6d432c25294) )
	ROM_LOAD( "10.dat",       0x3000, 0x1000, CRC(c3beb501) SHA1(14f49c45fc7c91799034c5a51fca310f0a66b1d7) )
	ROM_LOAD( "l05-11.bin",   0x4000, 0x1000, CRC(ae159192) SHA1(d467256a3a366e246243e7828ff4a45d4c146e2c) )
	ROM_LOAD( "n05-12.bin",   0x5000, 0x1000, CRC(966bc9ab) SHA1(4434fc620169ffea1b1f227b61674e1daf79b54b) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "n11-06.bin",   0x0000, 0x1000, CRC(2dcedd12) SHA1(dfdcfc21bcba7c8e148ee54daae511ca78c58e70) )
	ROM_LOAD( "l11-05.bin",   0x1000, 0x1000, CRC(fa7cbd91) SHA1(0208d2ebc59f3600005476b6987472685bc99d67) )
	ROM_LOAD( "k11-04.bin",   0x2000, 0x1000, CRC(3375b3bd) SHA1(a00b3c31cff123aab6ac0833aabfdd663302971a) )
	ROM_LOAD( "h11-03.bin",   0x3000, 0x1000, CRC(5655cc11) SHA1(5195e9b2a60c54280b48b32ee8248090904dbc51) )

	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "c11-02.bin",   0x0000, 0x0800, CRC(d1352c31) SHA1(da726a63a8be830d695afeddc1717749af8c9d47) )
	ROM_LOAD( "a11-01.bin",   0x0800, 0x0800, CRC(a7a2fdbd) SHA1(529865f8bbfbdbbf34ac39c70ef17e6d5bd0f845) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "ckonga4.v6",   0x0000, 0x0020, CRC(d3b84067) SHA1(1d025c28c47c4c7508b0dfd534de87e923262630) )
	ROM_LOAD( "prom.u6",      0x0020, 0x0020, CRC(26aada9e) SHA1(f59645e606ea4f0dd0fc4ea47dd03f526c534941) )
	ROM_LOAD( "prom.t6",      0x0040, 0x0020, CRC(676b3166) SHA1(29b9434cd34d43ea5664e436e2a24b54f8d88aac) )

	ROM_REGION( 0x2000, "samples", 0 )	/* samples */
	ROM_LOAD( "cc13j.bin",    0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "cc12j.bin",    0x1000, 0x1000, CRC(9003ffbd) SHA1(fd016056aabc23957643f37230f03842294f795e) )
ROM_END

ROM_START( ckongfix )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
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

	ROM_REGION( 0x2000, "samples", 0 )	/* samples */
	ROM_LOAD( "cc13j.bin",    0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "cc12j.bin",    0x1000, 0x1000, CRC(9003ffbd) SHA1(fd016056aabc23957643f37230f03842294f795e) )
ROM_END

/* This is a gfx hack of monkeyd. Should monkeyd get correct colours, it must be applied here too. */
ROM_START( tongypac )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "ck7.bin",      0x0000, 0x1000, CRC(2171cac3) SHA1(7b18bfe44c32fb64b675bbbe2136344522c79b09) )
	ROM_LOAD( "ck8.bin",      0x1000, 0x1000, CRC(88b83ff7) SHA1(4afc494cc264aaa4614da6aed02ce062d9c20850) )
	ROM_LOAD( "ck9.bin",      0x2000, 0x1000, CRC(cff2af47) SHA1(1757428cefad13855a623162101ec01c04006c94) )
	ROM_LOAD( "ck10.bin",     0x3000, 0x1000, CRC(520fa4de) SHA1(6edbaf727756cd33bde94492d72654aa12dbd7e1) )
	ROM_LOAD( "md5l.bin",     0x4000, 0x1000, CRC(d1db1bb0) SHA1(fe7d700c7f9eca9c389be3717ebebf3e7dc63aa2) )
	/* no ROM at 5000 */

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "tpac6.bin",    0x0000, 0x1000, CRC(918e3c52) SHA1(bc630759a525615aba1a06a0781e26a1a5c2c377) )
	ROM_LOAD( "tpac5.bin",    0x1000, 0x1000, CRC(2bbc69e7) SHA1(5741aa6944e9b936289368d533c2b19d34320260) )
	ROM_LOAD( "tpac4.bin",    0x2000, 0x1000, CRC(8d5890b6) SHA1(92d48d75084df2e2a3fa9d92a3c6c059f0781d81) )
	ROM_LOAD( "tpac3.bin",    0x3000, 0x1000, CRC(6b16145b) SHA1(ef17fad07b81c9dae5c6e16a86afc4a8f6a7ff8c) )
	
	ROM_REGION( 0x1000, "gfx2", 0 )
	ROM_LOAD( "ck2.bin",      0x0000, 0x0800, CRC(f67c80f1) SHA1(d1fbcce1b6242f810e106ff50812636e3168ebc1) )
	ROM_LOAD( "ck1.bin",      0x0800, 0x0800, CRC(80eb517d) SHA1(fef4111f656c58b28e7eac5aa5b5cc7e07ccb2fd) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "ck6v.bin",     0x0000, 0x0020, BAD_DUMP CRC(751c3325) SHA1(edce2bc883996c1d72dc6c1c9f62799b162d415a)  )
	ROM_LOAD( "ck6u.bin",     0x0020, 0x0020, BAD_DUMP CRC(ab1940fa) SHA1(8d98e05cbaa6f55770c12e0a9a8ed9c73cc54423)  )
	ROM_LOAD( "ck6t.bin",     0x0040, 0x0020, BAD_DUMP CRC(b4e827a5) SHA1(31a5a5ad54417a474d22bb16c473415d99a2b6f1)  )

	ROM_REGION( 0x2000, "samples", 0 )	/* samples */
	ROM_LOAD( "cc13j.bin",    0x0000, 0x1000, CRC(5f0bcdfb) SHA1(7f79bf6de117348f606696ed7ea1937bbf926612) )
	ROM_LOAD( "ck12.bin",     0x1000, 0x1000, CRC(2eb23b60) SHA1(c9e7dc584562aceb374193655fbacb7df6c9c731) )
ROM_END

GAME( 1980, cclimbrm, 0,     cclimber, cclimber, cclimber_state, cclimber, ROT0,   "Tim Arcadecollecting", "Crazy Climber (US - modded)", MACHINE_SUPPORTS_SAVE ) // http://www.arcadecollecting.com/crazy/cchack
GAME( 1981, ckonga2,  ckong, cclimber, ckonga2,  driver_device,  0,        ROT270, "bootleg", "Crazy Kong Part II -1", MACHINE_SUPPORTS_SAVE )
GAME( 1981, ckonga3,  ckong, cclimber, ckonga4,  driver_device,  0,        ROT270, "bootleg", "Crazy Kong Part II -2", MACHINE_SUPPORTS_SAVE )
GAME( 1981, ckongfix, ckong, cclimber, ckong,    driver_device,  0,        ROT270, "Pinny", "Crazy Kong Part II (Graphic Fix)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, tongypac, ckong, cclimber, ckong,    driver_device,  0,        ROT270, "Grand Master Peter", "Tongy Paccy", MACHINE_IMPERFECT_COLORS | MACHINE_SUPPORTS_SAVE )
