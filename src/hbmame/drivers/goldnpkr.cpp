// license:GPL_2.0
// copyright-holders:Robbbert, Roberto Fresca
#include "../mame/misc/goldnpkr.cpp"

ROM_START( goldnmame )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "goldnmame.12a", 0, 0x8000, CRC(23fc6487) SHA1(8b34f12a081415e363c98e6b1b1f876cd542b8b2) )

	ROM_REGION( 0x6000, "gfx1", ROMREGION_ERASE00 )
	ROM_LOAD( "goldnmame.5a", 0x4000, 0x2000, CRC(87b75bee) SHA1(dced4cf90ac922d32b0521fc85faef9438a0a87d) )

	ROM_REGION( 0x6000, "gfx2", ROMREGION_ERASE00 )
	ROM_LOAD( "u43_2a.bin", 0x0000, 0x2000, CRC(10b34856) SHA1(52e4cc81b36b4c807b1d4471c0f7bea66108d3fd) )
	ROM_LOAD( "u40_4a.bin", 0x2000, 0x2000, CRC(5fc965ef) SHA1(d9ecd7e9b4915750400e76ca604bec8152df1fe4) )
	ROM_COPY( "gfx1",       0x4800, 0x4000, 0x0800 )

	ROM_REGION( 0x100, "proms", 0 )
	ROM_LOAD( "goldnmame.7d", 0, 0x100, CRC(82fd9fbc) SHA1(7949fc7cca333c5352bfa82550f0941ffd3feca0) )
ROM_END


GAMEL( 2024, goldnmame, 0, goldnpkr, goldnpkr, goldnpkr_state, empty_init, ROT0, "Roberto Fresca / Grull Osgo", "Golden Poker (MAME tribute edition)", 0, layout_goldnpkr )

