// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/phoenix.cpp"

ROM_START( phoenixr )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "phoenix.45",   0x0000, 0x0800, CRC(5b8c55a8) SHA1(839c1ca9766f730ec3accd48db70f6429a9c3362) )
	ROM_LOAD( "phoenix3.46",  0x0800, 0x0800, CRC(5748f486) SHA1(49e6fd836d26ec24105e95227b24cf668e8a470a) )
	ROM_LOAD( "phoenix.47",   0x1000, 0x0800, CRC(cbbb8839) SHA1(b7f449374cac111081559e39646f973e7e99fd64) )
	ROM_LOAD( "phoenixr.48",  0x1800, 0x0800, CRC(9253e642) SHA1(9f72d7beb22ffc8dffc43765356e34def36efc12) )
	ROM_LOAD( "h5-ic49.5a",   0x2000, 0x0800, CRC(a105e4e7) SHA1(b35142a91b6b7fdf7535202671793393c9f4685f) )
	ROM_LOAD( "h6-ic50.6a",   0x2800, 0x0800, CRC(ac5e9ec1) SHA1(0402e5241d99759d804291998efd43f37ce99917) )
	ROM_LOAD( "h7-ic51.7a",   0x3000, 0x0800, CRC(2eab35b4) SHA1(849bf8273317cc869bdd67e50c68399ee8ece81d) )
	ROM_LOAD( "phoenix3.52",  0x3800, 0x0800, CRC(d2c5c984) SHA1(a9432f9aff8a2f5ca1d347443efc008a177d8ae0) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "r_ic23",       0x0000, 0x0800, CRC(02bc87ea) SHA1(bd672a5c62339e18d627d5bb195a9da2a76fd239) )
	ROM_LOAD( "r_ic24",       0x0800, 0x0800, CRC(675388ee) SHA1(4bef2b7cd28b2311a5775f89ee8756caf405c182) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "r_ic39",       0x0000, 0x0800, CRC(8b1653ba) SHA1(5677fd96d49165b4d71047771e4a0bd8d6fb2400) )
	ROM_LOAD( "r_ic40",       0x0800, 0x0800, CRC(3b811dfb) SHA1(e2a9fada30bdb096ddfd35abc4717e1cac38c943) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "mmi6301.ic40",   0x0000, 0x0100, CRC(79350b25) SHA1(57411be4c1d89677f7919ae295446da90612c8a8) )  /* palette low bits */
	ROM_LOAD( "mmi6301.ic41",   0x0100, 0x0100, CRC(e176b768) SHA1(e2184dd495ed579f10b6da0b78379e02d7a6229f) )  /* palette high bits */
ROM_END

ROM_START( phoenixg )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "phoenix.45",   0x0000, 0x0800, CRC(5b8c55a8) SHA1(839c1ca9766f730ec3accd48db70f6429a9c3362) )
	ROM_LOAD( "phoenix.46",   0x0800, 0x0800, CRC(dbc942fa) SHA1(9fe224e6ced407289dfa571468259a021d942b7d) )
	ROM_LOAD( "phoenix.47",   0x1000, 0x0800, CRC(cbbb8839) SHA1(b7f449374cac111081559e39646f973e7e99fd64) )
	ROM_LOAD( "phoenixg.48",  0x1800, 0x0800, CRC(63d4c322) SHA1(96bac0950940f6a3a7972ed403431c6fb1941667) )
	ROM_LOAD( "phoenix.49",   0x2000, 0x0800, CRC(c8a5d6d6) SHA1(ef6ade323544e3edd4101609138ecf35e8cb9577) )
	ROM_LOAD( "h6-ic50.6a",   0x2800, 0x0800, CRC(ac5e9ec1) SHA1(0402e5241d99759d804291998efd43f37ce99917) )
	ROM_LOAD( "h7-ic51.7a",   0x3000, 0x0800, CRC(2eab35b4) SHA1(849bf8273317cc869bdd67e50c68399ee8ece81d) )
	ROM_LOAD( "phoenix.52",   0x3800, 0x0800, CRC(b9915263) SHA1(f61396077b23364b5b26f62c6923394d23a37eb3) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "ic23.3d",      0x0000, 0x0800, CRC(3c7e623f) SHA1(e7ff5fc371664af44785c079e92eeb2d8530187b) )
	ROM_LOAD( "ic24.4d",      0x0800, 0x0800, CRC(59916d3b) SHA1(71aec70a8e096ed1f0c2297b3ae7dca1b8ecc38d) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "ic39g",        0x0000, 0x0800, CRC(0ed46d30) SHA1(dd21172c11ef60351921ed7067736682237a8db9) )
	ROM_LOAD( "b2-ic40.4b",   0x0800, 0x0800, CRC(0be2ba91) SHA1(af9243ee23377b632b9b7d0b84d341d06bf22480) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "mmi6301.ic40",   0x0000, 0x0100, CRC(79350b25) SHA1(57411be4c1d89677f7919ae295446da90612c8a8) )  /* palette low bits */
	ROM_LOAD( "mmi6301.ic41",   0x0100, 0x0100, CRC(e176b768) SHA1(e2184dd495ed579f10b6da0b78379e02d7a6229f) )  /* palette high bits */
ROM_END

/* E374 - Very similar to phoenixc, no year, some slight gfx differences, and the proms are different. */
ROM_START( phoenixd )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "phoenix.45",   0x0000, 0x0800, CRC(5b8c55a8) SHA1(839c1ca9766f730ec3accd48db70f6429a9c3362) )
	ROM_LOAD( "phoenix.46",   0x0800, 0x0800, CRC(dbc942fa) SHA1(9fe224e6ced407289dfa571468259a021d942b7d) )
	ROM_LOAD( "phoenix.47",   0x1000, 0x0800, CRC(cbbb8839) SHA1(b7f449374cac111081559e39646f973e7e99fd64) )
	ROM_LOAD( "phoenixd.48",  0x1800, 0x0800, CRC(6e51f009) SHA1(f91ed67543a675be9337f2a73c179f3fbea2c33e) )
	ROM_LOAD( "phoenixc.49",  0x2000, 0x0800, CRC(1a1ce0d0) SHA1(c2825eef5d461e16ca2172daff94b3751be2f4dc) )
	ROM_LOAD( "cond06c.bin",  0x2800, 0x0800, CRC(8c83bff7) SHA1(3dfb090d7e3a9ae8da882b06e166c48555eaf77c) )
	ROM_LOAD( "vautor07.1m",  0x3000, 0x0800, CRC(079ac364) SHA1(55b17c069b191cd1752e78068ef683b33c094e56) )
	ROM_LOAD( "phoenixc.52",  0x3800, 0x0800, CRC(8424d7c4) SHA1(1b5fa7d8be9e8750a4148dfefc17e96c86ed084d) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "ic23.3d",      0x0000, 0x0800, CRC(3c7e623f) SHA1(e7ff5fc371664af44785c079e92eeb2d8530187b) )
	ROM_LOAD( "ic24.4d",      0x0800, 0x0800, CRC(59916d3b) SHA1(71aec70a8e096ed1f0c2297b3ae7dca1b8ecc38d) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "phoenixd.3b",  0x0000, 0x0800, CRC(31c06c22) SHA1(41e13e2e14f098ef0c6985dbcd2d85f83eb3b44f) )
	ROM_LOAD( "phoenixc.40",  0x0800, 0x0800, CRC(4178aa4f) SHA1(5350f8f62cc7c223c38008bc83140b7a19147d81) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "mmi6301.ic41", 0x0100, 0x0100, CRC(e176b768) SHA1(e2184dd495ed579f10b6da0b78379e02d7a6229f) )
	ROM_RELOAD(               0x0000, 0x0100 )	/* the dump had 2 identical proms with different names */
ROM_END

/* E617 - rom 4 not dumped. 2nd rom in GFX2 not dumped. Proms not dumped. The only new one is rom 8.
		Since rom 4 contains the manufacturer information, the maker is unknown. */
ROM_START( phoenix4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "phoenix.45",   0x0000, 0x0800, CRC(5b8c55a8) SHA1(839c1ca9766f730ec3accd48db70f6429a9c3362) )
	ROM_LOAD( "phoenix.46",   0x0800, 0x0800, CRC(dbc942fa) SHA1(9fe224e6ced407289dfa571468259a021d942b7d) )
	ROM_LOAD( "phoenix.47",   0x1000, 0x0800, CRC(cbbb8839) SHA1(b7f449374cac111081559e39646f973e7e99fd64) )
	ROM_LOAD( "phoenix4.48",  0x1800, 0x0800, NO_DUMP )
	ROM_LOAD( "phoenixc.49",  0x2000, 0x0800, CRC(1a1ce0d0) SHA1(c2825eef5d461e16ca2172daff94b3751be2f4dc) )
	ROM_LOAD( "h6-ic50.6a",   0x2800, 0x0800, CRC(ac5e9ec1) SHA1(0402e5241d99759d804291998efd43f37ce99917) )
	ROM_LOAD( "vautor07.1m",  0x3000, 0x0800, CRC(079ac364) SHA1(55b17c069b191cd1752e78068ef683b33c094e56) )
	ROM_LOAD( "falcon2.8a",   0x3800, 0x0800, CRC(d16d971b) SHA1(d8eb980cc23210ac29e4258b5f8a1a84f6f20e76) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "ic23.3d",      0x0000, 0x0800, CRC(3c7e623f) SHA1(e7ff5fc371664af44785c079e92eeb2d8530187b) )
	ROM_LOAD( "ic24.4d",      0x0800, 0x0800, CRC(59916d3b) SHA1(71aec70a8e096ed1f0c2297b3ae7dca1b8ecc38d) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "phoenixc.39",  0x0000, 0x0800, CRC(bb0525ed) SHA1(86db1c7584fb3846bfd47535e1585eeb7fbbb1fe) )
	ROM_LOAD( "phoenixc.40",  0x0800, 0x0800, CRC(4178aa4f) SHA1(5350f8f62cc7c223c38008bc83140b7a19147d81) ) /* not dumped, this is a substitute */

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "mmi6301.ic40",   0x0000, 0x0100, CRC(79350b25) SHA1(57411be4c1d89677f7919ae295446da90612c8a8) )  /* palette low bits */
	ROM_LOAD( "mmi6301.ic41",   0x0100, 0x0100, CRC(e176b768) SHA1(e2184dd495ed579f10b6da0b78379e02d7a6229f) )  /* palette high bits */
ROM_END

/* E189 - No proms were dumped. Using those from pleiads. */
ROM_START( pleiads2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ic47.r1",      0x0000, 0x0800, CRC(fa98cb73) SHA1(d01138536e90a0a92d3e356fe354648e431a106c) )
	ROM_LOAD( "ic48.r2",      0x0800, 0x0800, CRC(b254217c) SHA1(312a33cca09d5d2d18992f28eb051230a90db6e3) )
	ROM_LOAD( "ic47.bin",     0x1000, 0x0800, CRC(0951829e) SHA1(03c0598dfe248ce14683ff18e59adb2e72731336) )
	ROM_LOAD( "ic48.bin",     0x1800, 0x0800, CRC(4972f5ce) SHA1(9175cc924c335d01ee47f3771276cdc90028fcc5) )
	ROM_LOAD( "ic51.r5",      0x2000, 0x0800, CRC(49c629bc) SHA1(fd7937d0c114c8d9c1efaa9918ae3df2af41f032) )
	ROM_LOAD( "ic50.bin",     0x2800, 0x0800, CRC(f1a8a00d) SHA1(5c183e3a73fa882ffec3cb9219fb5619e625591a) )
	ROM_LOAD( "ic53.r7",      0x3000, 0x0800, CRC(037b319c) SHA1(2ff7a7777a63326e2abca2d1881df33a8e3f8561) )
	ROM_LOAD( "ic52.bin",     0x3800, 0x0800, CRC(b3db08c2) SHA1(d5b1b77dcf2d76498f30d5f880635f5acfac7dfd) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "ic23.bin",     0x0000, 0x0800, CRC(4e30f9e7) SHA1(da023a94725dc40107cd97e4decfd4dc0f9f00ee) )
	ROM_LOAD( "ic24.bin",     0x0800, 0x0800, CRC(5188fc29) SHA1(421dedc674c6dde7abf01412df035a8eb8e6db9b) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "ic39.bin",     0x0000, 0x0800, CRC(85866607) SHA1(cd240bd056f761b2f9e2142049434f02cae3e315) )
	ROM_LOAD( "ic40.bin",     0x0800, 0x0800, CRC(a841d511) SHA1(8349008ab1d8ef08775b54170c37deb1d391fffc) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "7611-5.26",    0x0000, 0x0100, CRC(7a1bcb1e) SHA1(bdfab316ea26e2063879e7aa78b6ae2b55eb95c8) )   /* palette low bits */
	ROM_LOAD( "7611-5.33",    0x0100, 0x0100, CRC(e38eeb83) SHA1(252880d80425b2e697146e76efdc6cb9f3ba0378) )   /* palette high bits */
ROM_END


GAME( 2000, phoenixr, phoenix, phoenix,  phoenix , driver_device, 0, ROT90, "CYBERYOGI =CO= Windler", "PhoenixR (GDR hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, phoenixg, phoenix, phoenix,  phoenixt, driver_device, 0, ROT90, "GreekRoms", "Phoenix (Greek)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, phoenixd, phoenix, phoenix,  phoenix , driver_device, 0, ROT90, "G.G.I. Corporation", "Phoenix (GGI)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, phoenix4, phoenix, phoenix,  phoenix , driver_device, 0, ROT90, "unknown", "Phoenix", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pleiads2, 0,       pleiads,  pleiads,  driver_device, 0, ROT90, "ESG", "Pleiads", MACHINE_SUPPORTS_SAVE )
