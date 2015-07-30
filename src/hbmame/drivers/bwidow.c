// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/bwidow.c"

/*************************************
 *
 *  ROM definitions
 *
 *************************************/


ROM_START( spacwarp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	/* Vector ROM */
	ROM_LOAD( "spacwarp.106",   0x2800, 0x0800, CRC(23a25ff9) SHA1(e4117e6c453408079bd4a9e7b30a9b82e7b1712d) )
	ROM_LOAD( "136006.107",     0x3000, 0x1000, CRC(d8dd0461) SHA1(58060b20b2511d30d2ec06479d21840bdd0b53c6) )
	/* Program ROM */
	ROM_LOAD( "spacwarp.201",   0x4000, 0x1000, CRC(0acb38e9) SHA1(dcdeeedc798d7735d03249559556d55b8256abc6) )
	ROM_LOAD( "spacwarp.102",   0x5000, 0x1000, CRC(75fae2df) SHA1(7510e93781fe444ff10aac92abf4b882a65999aa) )
	ROM_LOAD( "136006.103",     0x6000, 0x1000, CRC(ee72da63) SHA1(d36d62cdf7fe76ee9cdbfc2e76ac5d90f22986ba) )
	ROM_LOAD( "spacwarp.104",   0x7000, 0x1000, CRC(aae05ec6) SHA1(14629e8375e1ae12dce96a742f1a0777e5114edb) )
	ROM_LOAD( "spacwarp.105",   0x8000, 0x1000, CRC(41cddb2d) SHA1(8dcd7f72ca1099b04f3d29730b5071d3cb55888e) )

	ROM_RELOAD(              0x9000, 0x1000 )
	ROM_RELOAD(              0xa000, 0x1000 )
	ROM_RELOAD(              0xb000, 0x1000 )
	ROM_RELOAD(              0xc000, 0x1000 )
	ROM_RELOAD(              0xd000, 0x1000 )
	ROM_RELOAD(              0xe000, 0x1000 )
	ROM_RELOAD(              0xf000, 0x1000 )   /* for reset/interrupt vectors */

	/* AVG PROM */
	ROM_REGION( 0x100, "user1", 0 )
	ROM_LOAD( "136002-125.n4",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

GAME( 1999, spacwarp, 0, spacduel, spacduel, driver_device, 0, ROT0, "Jeff Hendrix", "Space War (Proto on Space Duel HW)", MACHINE_NOT_WORKING )
