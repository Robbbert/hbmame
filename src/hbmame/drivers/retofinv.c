// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/retofinv.c"

ROM_START( retofin4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "a37-03.70", 0x0000, 0x2000, CRC(eae7459d) SHA1(c105f6adbd4c09decaad68ed13163d8f9b55e646) )
	ROM_LOAD( "a37-02.71", 0x2000, 0x2000, CRC(72895e37) SHA1(42fb904338e9f92a79d587eac401d456e7fb6e55) )
	ROM_LOAD( "a37-01.72", 0x4000, 0x2000, CRC(505dd20b) SHA1(3a34b1515bb834ff9e2d86b0b43a752d9619307b) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "a37-04.62", 0x0000, 0x2000, CRC(d2899cc1) SHA1(fdbec743b06f4cdcc134ef863e4e71337ad0b2c5) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "a37-05.17", 0x0000, 0x2000, CRC(9025abea) SHA1(2f03e8572f23624d7cd1215a55109e97fd66e271) )

	ROM_REGION( 0x0800, "68705", 0 )	/* 8k for the microcontroller */
	/* the only available dump is from a bootleg board, and is not the real thing (see notes at top of driver) */
	ROM_LOAD( "a37-09.37", 0x00000, 0x0800, BAD_DUMP CRC(79bd6ded) SHA1(4967e95b4461c1bfb4e933d1804677799014f77b) )

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "a37-16.61", 0x0000, 0x2000, CRC(e0de0cc4) SHA1(a9a19ed25de26306a328cc158843f43ade6eede1) )

	ROM_REGION( 0x08000, "gfx2", 0 )
	ROM_LOAD( "a37-10.8",  0x0000, 0x2000, CRC(c691fdbe) SHA1(b2f4c41cb0d059c9aa8045fc3e8e684bcc96b309) )
	ROM_LOAD( "a37-11.9",  0x2000, 0x2000, CRC(0ff8cf5b) SHA1(46a67a8d7d59cdbf1a4ce9e8b9361a5766880e96) )
	ROM_LOAD( "a37-12.10", 0x4000, 0x2000, CRC(90fd7496) SHA1(db2a98f4b07996d178c99a2653e1df1be4619644) )
	ROM_LOAD( "a37-13.11", 0x6000, 0x2000, CRC(a2526b5e) SHA1(a31999c86aff8977cbec4bd24056a097aaa738c7) )

	ROM_REGION( 0x04000, "gfx3", 0 )
	ROM_LOAD( "a37-14.55", 0x0000, 0x2000, CRC(f233c421) SHA1(77da9750f88d032143968104e0b400ed1a07adca) )
	ROM_LOAD( "a37-15.56", 0x2000, 0x2000, CRC(58c3f1c9) SHA1(5e0c46a5cd6adfcccba5cf33271a51a23ff9752a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "a37-06.13", 0x0000, 0x0100, CRC(e9643b8b) SHA1(7bbb92a42e7c3effb701fc7b2c24f2470f31b063) )	/* palette red bits  */
	ROM_LOAD( "a37-07.4",  0x0100, 0x0100, CRC(e8f34e11) SHA1(8f438561b8d46ffff00747ed8baf0ebb6a081615) )	/* palette green bits */
	ROM_LOAD( "a37-08.3",  0x0200, 0x0100, CRC(50030af0) SHA1(e748ae0b8702b7d20fb65c254dceee23246b3d13) )	/* palette blue bits   */
	ROM_LOAD( "82s191n",   0x0300, 0x0800, CRC(93c891e3) SHA1(643a0107717b6a434432dda73a0102e6e8adbca7) )	/* lookup table */
ROM_END

GAME( 2011, retofin4, 0, retofinv, retofinv, driver_device, 0, ROT90, "Paul Smith", "Return of the Invaders (hack)", 0 )
