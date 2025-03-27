// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/mustache.cpp"

ROM_START( mustache01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "europe.h18", 0x0000, 0x8000, CRC(123bd9b8) SHA1(33a7cba5c3a54b0b1a15dd1e24d298b6f7274321) )
	ROM_LOAD( "europe.h16", 0x8000, 0x4000, CRC(d3e9c06e) SHA1(c20f8375e3710effa5705a9d7ce54d03a8e08a83) )

	ROM_REGION( 0x8000, "t5182_z80", 0 )
	ROM_LOAD( "mustache.e5", 0x0000, 0x8000, CRC(efbb1943) SHA1(3320e9eaeb776d09ed63f7dedc79e720674e6718) )

	ROM_REGION( 0x0c000, "gfx1", 0 )
	ROM_LOAD( "mustache.a13", 0x0000,  0x4000, CRC(9baee4a7) SHA1(31bcec838789462e67e54ebe7256db9fc4e51b69) )
	ROM_LOAD( "mustache.a14", 0x4000,  0x4000, CRC(8155387d) SHA1(5f0a394c7671442519a831b0eeeaba4eecd5a406) )
	ROM_LOAD( "mustache.a16", 0x8000,  0x4000, CRC(4db4448d) SHA1(50a94fd65c263d95fd24b4009dbb87707929fdcb) )

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "mustache.a4", 0x00000,  0x8000, CRC(d5c3bbbf) SHA1(914e3feea54246476701f492c31bd094ad9cea10) )
	ROM_LOAD( "mustache.a7", 0x08000,  0x8000, CRC(e2a6012d) SHA1(4e4cd1a186870c8a88924d5bff917c6889da953d) )
	ROM_LOAD( "mustache.a5", 0x10000,  0x8000, CRC(c975fb06) SHA1(4d166bd79e19c7cae422673de3e095ad8101e013) )
	ROM_LOAD( "mustache.a8", 0x18000,  0x8000, CRC(2e180ee4) SHA1(a5684a25c337aeb4effeda7982164d35bc190af9) )

	ROM_REGION( 0x1300, "proms", 0 )
	ROM_LOAD( "mustache.c3",0x0000, 0x0100, CRC(68575300) SHA1(bc93a38df91ad8c2f335f9bccc98b52376f9b483) )
	ROM_LOAD( "mustache.c2",0x0100, 0x0100, CRC(eb008d62) SHA1(a370fbd1affaa489210ea36eb9e365263fb4e232) )
	ROM_LOAD( "mustache.c1",0x0200, 0x0100, CRC(65da3604) SHA1(e4874d4152a57944d4e47306250833ea5cd0d89b) )
	ROM_LOAD( "mustache.b6",0x0300, 0x1000, CRC(5f83fa35) SHA1(cb13e63577762d818e5dcbb52b8a53f66e284e8f) )
ROM_END

GAME( 1987, mustache01, mustache, mustache, mustache, mustache_state, init_mustache, ROT90, "Seibu Kaihatsu", "Mustache Boy (Europe)", MACHINE_SUPPORTS_SAVE )

