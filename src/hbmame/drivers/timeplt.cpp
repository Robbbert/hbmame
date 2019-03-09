// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/timeplt.cpp"


/*************************************
 *
 *  ROM definitions
 *
 *************************************/


// At normal use, the rom screen is flipped.
// For proper diagnostics, you must adjust the dips so the SW2 switches are all up (3rd switch doesn't matter).
// You can then press 1 to get the screen right way up. Follow on-screen prompts for other tests.


ROM_START( timepltdiag )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tm1",          0x0000, 0x2000, CRC(1551f1b9) SHA1(c72f30988ac00cbe6549b71c3bcb414511e8b997) )
	ROM_LOAD( "tm2",          0x2000, 0x2000, CRC(58636cb5) SHA1(ab517efa93ae7be780af55faea82a6e83edd828c) )
	ROM_LOAD( "tm3",          0x4000, 0x2000, CRC(ff4e0d83) SHA1(ef98a1abb45b22d7498a0aca520f43bbee248b22) )
	ROM_LOAD( "diag.5h",      0x6000, 0x2000, CRC(020eac50) SHA1(5459248dbd590393936af4ceef8fabc4a30b0315) )

	ROM_REGION( 0x10000, "timeplt_audio:tpsound", 0 )
	ROM_LOAD( "tm7",          0x0000, 0x1000, CRC(d66da813) SHA1(408fca4515e8af84211df3e204c8776b2f8adb23) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "tm6",          0x0000, 0x2000, CRC(c2507f40) SHA1(07221875e3f81d9def67c57a7ccd82d52ce65e01) )

	ROM_REGION( 0x4000, "gfx2", 0 )
	ROM_LOAD( "tm4",          0x0000, 0x2000, CRC(7e437c3e) SHA1(cbe2ccd2cd503af62f009cd5aab73aa7366230b1) )
	ROM_LOAD( "tm5",          0x2000, 0x2000, CRC(e8ca87b9) SHA1(5dd30d3fb9fd8cf9e6a8e37e7ea858c7fd038a7e) )

	ROM_REGION( 0x0240, "proms", 0 )
	ROM_LOAD( "timeplt.b4",   0x0000, 0x0020, CRC(34c91839) SHA1(f62e279e21fce171231d3139be7adabe1f4b8c2e) )
	ROM_LOAD( "timeplt.b5",   0x0020, 0x0020, CRC(463b2b07) SHA1(9ad275365eba4869f94749f39ff8705d92056a10) )
	ROM_LOAD( "timeplt.e9",   0x0040, 0x0100, CRC(4bbb2150) SHA1(678433b21aae1daa938e32d3293eeed529a42ef9) )
	ROM_LOAD( "timeplt.e12",  0x0140, 0x0100, CRC(f7b7663e) SHA1(151bd2dff4e4ef76d6438c1ab2cae71f987b9dad) )
ROM_END



/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 2015, timepltdiag, timeplt, timeplt,  timeplt,  timeplt_state, empty_init, ROT90,  "Shoestring", "Time Pilot Tester", MACHINE_SUPPORTS_SAVE )
