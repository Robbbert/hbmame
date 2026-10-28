// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/misc/epos.cpp"

ROM_START( catapult01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "co3223.u10",     0x0000, 0x1000, CRC(50abcfd2) SHA1(13ce04addc7bcaa1ec6659da26b1c13ed9dc28f9) )
	ROM_LOAD( "co3223.u09",     0x1000, 0x1000, CRC(fd5a9a1c) SHA1(512374e8450459537ba2cc41e7d0178052445316) )
	ROM_LOAD( "ai3223.u08",     0x2000, 0x1000, BAD_DUMP CRC(774112f0) SHA1(60d9e19e385c611f6131b6d1e93fd22bb3689663) ) // recreated by AI.
	ROM_LOAD( "co3223.u07",     0x3000, 0x1000, CRC(4113bb99) SHA1(3cebb874dae211d75082209e913d4afa4f621de1) )
	ROM_LOAD( "co3223.u06",     0x4000, 0x1000, CRC(966bb9f5) SHA1(1a217c6f7a88c58e0deae0290bc5ddd2789d18eb) )
	ROM_LOAD( "co3223.u05",     0x5000, 0x1000, CRC(65f9fb9a) SHA1(63b616a736d9e39a8f2f76889fd7c5fe4128a966) )
	ROM_LOAD( "co3223.u04",     0x6000, 0x1000, CRC(648453bc) SHA1(8e4538aedad4d32bd046aad474dbcc689ee8fe53) )
	ROM_LOAD( "co3223.u11",     0x7000, 0x0800, CRC(08fb8c28) SHA1(0b08cc2727a54e0ad7472234be0f637b46bc3253) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "co3223.u66",     0x0000, 0x0020, CRC(e7de76a7) SHA1(101ce85459a59c0d01ce3ea96480f1f8413a788e) )
ROM_END

GAME( 2026, catapult01, 0, tristar8000, catapult, tristar8000_state, empty_init, ROT270, "BrokenToken", "Catapult (repaired by AI)", MACHINE_SUPPORTS_SAVE )

