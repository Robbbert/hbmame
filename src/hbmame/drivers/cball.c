// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/cball.c"
/***************************************************************************

    Atari Cannonball (prototype) driver

    Note: canballmash.6m is an approximate replacement created by MASH.

    The maincpu rom has heaps of illegal '02' opcodes.

***************************************************************************/


ROM_START( cballfix )
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD_NIB_LOW ( "canball.1e", 0x7400, 0x0400, CRC(0b34823b) SHA1(0db6b9f78f7c07ee7d35f2bf048ba61fe43b1e26) )
	ROM_LOAD_NIB_HIGH( "canball.1l", 0x7400, 0x0400, CRC(b43ca275) SHA1(a03e03f6366877cfdcec71030a5fb2c5171c8d8a) )
	ROM_LOAD_NIB_LOW ( "canball.1f", 0x7800, 0x0400, CRC(29b4e1f7) SHA1(8cef944b6e0153c304aa2d4cfdc530b8a4eef021) )
	ROM_LOAD_NIB_HIGH( "canball.1k", 0x7800, 0x0400, CRC(a4d1cf12) SHA1(99de6470efd16e57d72019e065f55bc740f3c7fc) )
	ROM_LOAD_NIB_LOW ( "canball.1h", 0x7c00, 0x0400, CRC(13f55937) SHA1(7514c27e60944c4e00992c8ecbc5115f8ff948bb) )
	ROM_LOAD_NIB_HIGH( "canball.1j", 0x7c00, 0x0400, CRC(5b905d69) SHA1(2408dd6e44c51c0c9bdb82d2d33826c03f8308c4) )

	ROM_REGION( 0x0200, "gfx1", 0 ) /* tiles */
	ROM_LOAD_NIB_LOW ( "canballmash.6m", 0x0000, 0x0200, BAD_DUMP CRC(bec9eb19) SHA1(8d84d783a4615e58fa4138e039658922ce50e967) )
	ROM_LOAD_NIB_HIGH( "canball.6l", 0x0000, 0x0200, CRC(5b1c9e88) SHA1(6e9630db9907170c53942a21302bcf8b721590a3) )

	ROM_REGION( 0x0200, "gfx2", 0 ) /* sprites */
	ROM_LOAD_NIB_LOW ( "canball.5l", 0x0000, 0x0200, CRC(3d0d1569) SHA1(1dfcf5cf9468d476c4b7d76a261c6fec87a99f93) )
	ROM_LOAD_NIB_HIGH( "canball.5k", 0x0000, 0x0200, CRC(c5fdd3c8) SHA1(5aae148439683ff1cf0005a810c81fdcbed525c3) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "canball.6h", 0x0000, 0x0100, CRC(b8094b4c) SHA1(82dc6799a19984f3b204ee3aeeb007e55afc8be3) ) /* sync */
ROM_END


GAME( 2004, cballfix, cball, cball, cball, driver_device, 0, ROT0, "MASH", "Cannonball (Atari, prototype) with new rom", MACHINE_NO_SOUND | MACHINE_WRONG_COLORS | MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
