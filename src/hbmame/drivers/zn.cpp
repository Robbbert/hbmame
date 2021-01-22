// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/zn.cpp"

// 12 characters to choose from instead of 10
ROM_START( doapph01 )
	TPS_BIOS

	ROM_REGION32_LE( 0x02800000, "bankedroms", 0 )
	ROM_LOAD16_BYTE( "doapp119.h01", 0x0000001, 0x100000, CRC(f3691595) SHA1(470ea8989229b6400cb7adac30939a2f635dccd9) )
	ROM_LOAD16_BYTE( "doapp120.h01", 0x0000000, 0x100000, CRC(3a8e9786) SHA1(8632bcba3ecd42c112885821792c78f7b0f51cd9) )
	ROM_LOAD( "doapp-0.216",         0x0400000, 0x400000, CRC(acc6c539) SHA1(a744567a3d75634098b1749103307981be9acbdd) )
	ROM_LOAD( "doapp-1.217",         0x0800000, 0x400000, CRC(14b961c4) SHA1(3fae1fcb4665ba8bad391881b26c2d087718d42f) )
	ROM_LOAD( "doapp-2.218",         0x0c00000, 0x400000, CRC(134f698f) SHA1(6422972cf5d30a0f09f0c20f042691d5969207b4) )
	ROM_LOAD( "doapp-3.219",         0x1000000, 0x400000, CRC(1c6540f3) SHA1(8631fde93a1da6325d7b31c7edf12c964f0ac4fc) )
	ROM_LOAD( "doapp-4.220",         0x1400000, 0x400000, CRC(f83bacf7) SHA1(5bd66da993f0db966581dde80dd7e5b377754412) )
	ROM_LOAD( "doapp-5.221",         0x1800000, 0x400000, CRC(e11e8b71) SHA1(b1d1b9532b5f074ce216a603436d5674d136865d) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "mg05", 0x000000, 0x000008, CRC(5748a4ca) SHA1(c88d73f6a646a9ddefdfd84cba70d591759c069f) )
ROM_END

GAME( 1998, doapph01, doapp, coh1002m, zn, tecmo_zn_state, empty_init, ROT0, "yumeji", "Dead Or Alive ++ (Enable Hidden Characters)", 0 )


/****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*****************
 Dead Or Alive ++
********************/

ROM_START( doappnud )
	TPS_BIOS
	ROM_REGION32_LE( 0x02800000, "bankedroms", 0 )
	ROM_LOAD16_BYTE( "doapp119.bin", 0x0000001, 0x100000, CRC(bbe04cef) SHA1(f2dae4810ca78075fc3007a6001531a455235a2e) )
	ROM_LOAD16_BYTE( "doapp120nud.bin", 0x0000000, 0x100000, CRC(cb236b2f) SHA1(b4e69abed13d9166ab0e329e2d1119fbea9e396c) )
	ROM_LOAD( "doapp-0.216",         0x0400000, 0x400000, CRC(acc6c539) SHA1(a744567a3d75634098b1749103307981be9acbdd) )
	ROM_LOAD( "doapp-1.217",         0x0800000, 0x400000, CRC(14b961c4) SHA1(3fae1fcb4665ba8bad391881b26c2d087718d42f) )
	ROM_LOAD( "doapp-2.218",         0x0c00000, 0x400000, CRC(134f698f) SHA1(6422972cf5d30a0f09f0c20f042691d5969207b4) )
	ROM_LOAD( "doapp-3.219",         0x1000000, 0x400000, CRC(1c6540f3) SHA1(8631fde93a1da6325d7b31c7edf12c964f0ac4fc) )
	ROM_LOAD( "doapp-4.220",         0x1400000, 0x400000, CRC(f83bacf7) SHA1(5bd66da993f0db966581dde80dd7e5b377754412) )
	ROM_LOAD( "doapp-5.221",         0x1800000, 0x400000, CRC(e11e8b71) SHA1(b1d1b9532b5f074ce216a603436d5674d136865d) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "mg05", 0x000000, 0x000008, CRC(5748a4ca) SHA1(c88d73f6a646a9ddefdfd84cba70d591759c069f) )
ROM_END

 /*****************
 Street Fighter EX
********************/

ROM_START( sfexs01 )
	CPZN1_BIOS

	ROM_REGION32_LE( 0x80000, "countryrom", 0 )
	ROM_LOAD( "sfee_04a.2h", 0x0000000, 0x080000, CRC(092cfa2e) SHA1(8af38a3f4f89f661233995a672faf486e71b79bc) )

	ROM_REGION32_LE( 0x2400000, "bankedroms", 0 )
	ROM_LOAD( "sfe-05m_ps01.3h", 0x0000000, 0x400000, CRC(ee9ab584) SHA1(82c31637d58fabf41a29bc55f2c23e97a13c6520) )
	ROM_LOAD( "sfe-06m.4h", 0x0400000, 0x400000, CRC(999de60c) SHA1(092882698c411fc5c3bcb43105bf1886f94b8e40) )
	ROM_LOAD( "sfe-07m.5h", 0x0800000, 0x400000, CRC(76117b0a) SHA1(027233199170fa6e5b32f28da2031638c6d3d14a) )
	ROM_LOAD( "sfe-08m.2k", 0x0c00000, 0x400000, CRC(a36bbec5) SHA1(fa22ea50d4d8bed2ded97a346f61b2f5f68769b9) )
	ROM_LOAD( "sfe-09m.3k", 0x1000000, 0x400000, CRC(62c424cc) SHA1(ea19c49b486473b150dbf8541286e225655496db) )
	ROM_LOAD( "sfe-10m.4k", 0x1400000, 0x400000, CRC(83791a8b) SHA1(534969797640834ca692c11d0ce7c3a060fc7e4b) )

	ROM_REGION( 0x40000, "audiocpu", 0 )
	ROM_LOAD( "sfe_02.2e",  0x00000, 0x20000, CRC(1908475c) SHA1(99f68cff2d92f5697eec0846201f6fb317d5dc08) )
	ROM_LOAD( "sfe_03.3e",  0x20000, 0x20000, CRC(95c1e2e0) SHA1(383bbe9613798a3ac6944d18768280a840994e40) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "sfe-01m.3b", 0x0000000, 0x400000, CRC(f5afff0d) SHA1(7f9ac32ba0a3d9c6fef367e36a92d47c9ac1feb3) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "cp04", 0x000000, 0x000008, CRC(e0dc24ae) SHA1(17d6e3dc11308195f4c46a6cd8093db9eaf584e5) )
ROM_END

 /*******************
 Street Fighter EX2
********************/

ROM_START( sfex2s01 )
	CPZN2_BIOS

	ROM_REGION32_LE( 0x80000, "countryrom", 0 )
	ROM_LOAD( "ex2u_04a.2h", 0x0000000, 0x080000, CRC(8dc5317f) SHA1(c35224caf70662a0e45a74cbead294a51f9b9e16) )

	ROM_REGION32_LE( 0x3000000, "bankedroms", 0 )
	ROM_LOAD( "ex2-05m_ps01.3h", 0x0000000, 0x800000, CRC(b78cdfd2) SHA1(64b53a777af2c943b023011afe1919aba452a6d1) )
	ROM_LOAD( "ex2-06m.4h", 0x0800000, 0x800000, CRC(be1075ed) SHA1(36dc673372f30f8b3ff5689ae568c5cd01fe2c07) )
	ROM_LOAD( "ex2-07m.5h", 0x1000000, 0x800000, CRC(6496c6ed) SHA1(054bcecbb04033abea14d9ffe6634b2bd11ca88b) )
	ROM_LOAD( "ex2-08m.2k", 0x1800000, 0x800000, CRC(3194132e) SHA1(d1324fcf0a8528fc683791d6342697a7e08674f4) )
	ROM_LOAD( "ex2-09m.3k", 0x2000000, 0x400000, CRC(075ae585) SHA1(6b88851db618fc3e96f1d740c46c1bc5be0ee21b) )

	ROM_REGION( 0x40000, "audiocpu", 0 )
	ROM_LOAD( "ex2_02.2e",  0x00000, 0x20000, CRC(9489875e) SHA1(1fc9985ff98232c63ea8d05a69f7d77cdf72919f) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "ex2-01m.3a", 0x0000000, 0x400000, CRC(14a5bb0e) SHA1(dfe3c3a53bd4c58743d8039b5344d3afbe2a9c24) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "cp08", 0x000000, 0x000008, CRC(a63d6fa6) SHA1(68995438a1e90ff9aa59090e7e031d51c68c4d73) )
ROM_END

 /************************
 Street Fighter EX2 Plus
***************************/

ROM_START( sfex2ps01 )
	CPZN2_BIOS

	ROM_REGION32_LE( 0x80000, "countryrom", 0 )
	ROM_LOAD( "x2pu_04.2h", 0x0000000, 0x080000, CRC(2938118c) SHA1(4bdeeb9aa3dd54ef44aa3fc73d78d65297b1ed25) )

	ROM_REGION32_LE( 0x3000000, "bankedroms", 0 )
	ROM_LOAD( "x2p-05mp_ps01.3h", 0x0000000, 0x800000, CRC(08e6a6f4) SHA1(b18bb531fade80a6639cd6202c6e490f8a3144bd) )
	ROM_LOAD( "x2p-06m.4h", 0x0800000, 0x800000, CRC(4cd53a45) SHA1(39499ea6c9aa51c71f4fe44cc02f93d5a39e14ec) )
	ROM_LOAD( "x2p-07m.5h", 0x1000000, 0x800000, CRC(11207c2a) SHA1(0182652819f1c3a36e7b42e34ef86d2455a2dd90) )
	ROM_LOAD( "x2p-08m.2k", 0x1800000, 0x800000, CRC(3560c2cc) SHA1(8b0ce22d954387f7bb032b5220d1014ef68741e8) )
	ROM_LOAD( "x2p-09m.3k", 0x2000000, 0x800000, CRC(344aa227) SHA1(69dc6f511939bf7fa25c2531ecf307a7565fe7a8) )
	ROM_LOAD( "x2p-10m.4k", 0x2800000, 0x800000, CRC(2eef5931) SHA1(e5227529fb68eeb1b2f25813694173a75d906b52) )

	ROM_REGION( 0x40000, "audiocpu", 0 )
	ROM_LOAD( "x2p_02.2e",  0x00000, 0x20000, CRC(3705de5e) SHA1(847007ca271da64bf13ffbf496d4291429eee27a) )
	ROM_LOAD( "x2p_03.3e",  0x20000, 0x20000, CRC(6ae828f6) SHA1(41c54165e87b846a845da581f408b96979288158) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "x2p-01m.3a", 0x0000000, 0x400000, CRC(14a5bb0e) SHA1(dfe3c3a53bd4c58743d8039b5344d3afbe2a9c24) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "cp12", 0x000000, 0x000008, CRC(7cc2ed68) SHA1(a409ae837665700bdc4e3aa7c41a418d5b792940) )
ROM_END

 /************************
 Street Fighter EX Plus
***************************/

ROM_START( sfexps01 )
	CPZN1_BIOS

	ROM_REGION32_LE( 0x80000, "countryrom", 0 )
	ROM_LOAD( "sfpu_04a.2h", 0x0000000, 0x080000, CRC(4617adc2) SHA1(200307904349ad7e5d7d76d8c904b6b10424c7ef) )

	ROM_REGION32_LE( 0x2400000, "bankedroms", 0 )
	ROM_LOAD( "sfp-05m_ps01.3h", 0x0000000, 0x400000, CRC(449cf487) SHA1(1bad501d328169e04dab371b669e2952541db408) )
	ROM_LOAD( "sfp-06m.4h", 0x0400000, 0x400000, CRC(1d504758) SHA1(bd56141aba35dbb5b318445ba5db12eff7442221) )
	ROM_LOAD( "sfp-07m.5h", 0x0800000, 0x400000, CRC(0f585f30) SHA1(24ffdbc360f8eddb702905c99d315614327861a7) )
	ROM_LOAD( "sfp-08m.2k", 0x0c00000, 0x400000, CRC(65eabc61) SHA1(bbeb3bcd8dd8f7f88ed82412a81134a3d6f6ffd9) )
	ROM_LOAD( "sfp-09m.3k", 0x1000000, 0x400000, CRC(15f8b71e) SHA1(efb28fbe750f443550ee9718385355aae7e858c9) )
	ROM_LOAD( "sfp-10m.4k", 0x1400000, 0x400000, CRC(c1ecf652) SHA1(616e14ff63d38272730c810b933a6b3412e2da17) )

	ROM_REGION( 0x40000, "audiocpu", 0 )
	ROM_LOAD( "sfe_02.2e",  0x00000, 0x20000, CRC(1908475c) SHA1(99f68cff2d92f5697eec0846201f6fb317d5dc08) )
	ROM_LOAD( "sfe_03.3e",  0x20000, 0x20000, CRC(95c1e2e0) SHA1(383bbe9613798a3ac6944d18768280a840994e40) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "sfe-01m.3b", 0x0000000, 0x400000, CRC(f5afff0d) SHA1(7f9ac32ba0a3d9c6fef367e36a92d47c9ac1feb3) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "cp04", 0x000000, 0x000008, CRC(e0dc24ae) SHA1(17d6e3dc11308195f4c46a6cd8093db9eaf584e5) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Dead Or Alive ++
GAME( 1998, doappnud,     doapp,  coh1002m,    zn,       tecmo_zn_state, empty_init, ROT0, "Yumeji",   "Dead Or Alive ++ (Kasumi Sexy Hack)", 0 )
// Street Fighter EX
GAME( 1996, sfexs01,      sfex,   coh1002c,    zn6b,     zn1_state, empty_init, ROT0, "yumeji",  "Street Fighter EX (Enable Hidden Characters)", 0 )
// Street Fighter EX2
GAME( 1998, sfex2s01,     sfex2,  coh3002c,    zn6b,     zn2_state, empty_init, ROT0, "yumeji",  "Street Fighter EX2 (Enable Hidden Characters)", 0 )
// Street Fighter EX2 Plus
GAME( 1999, sfex2ps01,    sfex2p, coh3002c,    zn6b,     zn2_state, empty_init, ROT0, "yumeji",  "Street Fighter EX2 Plus (Enable Hidden Characters)", 0 )
// Street Fighter EX Plus
GAME( 1997, sfexps01,     sfexp,  coh1002c,    zn6b,     zn1_state, empty_init, ROT0, "yumeji",  "Street Fighter EX Plus (Enable Hidden Characters)", 0 )
