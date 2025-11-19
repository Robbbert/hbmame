// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/sonson.cpp"

ROM_START( sonson01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ss01.01e",       0x4000, 0x4000, CRC(123f758e) SHA1(0158aa626513e439e9ae2290770a29b6b42cbde9) )
	ROM_LOAD( "ss01.02e",       0x8000, 0x4000, CRC(ea42c668) SHA1(04f9dc2e3ec3119828deb961aaa58236aaffcc6e) )
	ROM_LOAD( "ss.03e",       0xc000, 0x4000, CRC(1fd0e729) SHA1(e04215b0c3d11ce844ab250ff3e1a845dd0b6c3e) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "ss_6.c11",     0xe000, 0x2000, CRC(1135c48a) SHA1(bfc10363fc42fb589088675a6e8e3d1668d8a6b8) )

	ROM_REGION( 0x04000, "gfx1", 0 )
	ROM_LOAD( "ss01_7.b6",      0x00000, 0x2000, CRC(617bfd8d) SHA1(5e9053324729ada8f5f62bd1d140a06b44a50172) )
	ROM_LOAD( "ss01_8.b5",      0x02000, 0x2000, CRC(144a6abb) SHA1(21b560e567ea1a31e848c7a51a8131f5e040f9fb) )

	ROM_REGION( 0x0c000, "gfx2", 0 )
	ROM_LOAD( "ss01_9.m5",      0x00000, 0x2000, CRC(3f3be435) SHA1(3084f9d7473b0d2cf46c04846a696da84dea4ff1) )
	ROM_LOAD( "ss01_10.m6",     0x02000, 0x2000, CRC(89ab9ddb) SHA1(31bc3e3eca70381f86368473b302380dc022a623) )
	ROM_LOAD( "ss01_11.m3",     0x04000, 0x2000, CRC(d1f2858b) SHA1(ef8c848e5a4f8bcd8e4d019f988e5f45e2f01caf) )
	ROM_LOAD( "ss01_12.m4",     0x06000, 0x2000, CRC(647b2f37) SHA1(e2cd1765954a9edbc1e0dde6ff43c63d3cec4bed) )
	ROM_LOAD( "ss01_13.m1",     0x08000, 0x2000, CRC(dc7c8508) SHA1(a16c491b6842afaac476ee5907c2df3edc2fcd33) )
	ROM_LOAD( "ss01_14.m2",     0x0a000, 0x2000, CRC(070a562a) SHA1(427b5ca497d1712a85fbb17c1e631d6075743a28) )

	ROM_REGION( 0x0340, "proms", 0 )
	ROM_LOAD( "ssb4.b2",      0x0000, 0x0020, CRC(c8eaf234) SHA1(d39dfab6dcad6b0a719c466b5290d2d081e4b58d) )
	ROM_LOAD( "ssb5.b1",      0x0020, 0x0020, CRC(0e434add) SHA1(238c281813d6079b9ae877bd0ced33abbbe39442) )
	ROM_LOAD( "ssb2.c4",      0x0040, 0x0100, CRC(c53321c6) SHA1(439d98a98cdf2118b887c725a7759a98e2c377d9) )
	ROM_LOAD( "ssb3.h7",      0x0140, 0x0100, CRC(7d2c324a) SHA1(3dcf09bd3f58bddb9760183d2c1b0fe5d77536ea) )
	ROM_LOAD( "ssb1.k11",     0x0240, 0x0100, CRC(a04b0cfe) SHA1(89ab33c6b0aa313ebda2f11516cea667a9951a81) )
ROM_END


GAME( 2024, sonson01, sonson, sonson, sonson, sonson_state, empty_init, ROT0, "Vag", "Son Son (Greek)", MACHINE_SUPPORTS_SAVE )

