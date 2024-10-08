// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/retofinv.cpp"

ROM_START( retofinv4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "a37__03.ic70", 0x0000, 0x2000, CRC(eae7459d) SHA1(c105f6adbd4c09decaad68ed13163d8f9b55e646) )
	ROM_LOAD( "a37__02.ic71", 0x2000, 0x2000, CRC(72895e37) SHA1(42fb904338e9f92a79d587eac401d456e7fb6e55) )
	ROM_LOAD( "a37__01.ic72", 0x4000, 0x2000, CRC(505dd20b) SHA1(3a34b1515bb834ff9e2d86b0b43a752d9619307b) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "a37__04.ic62", 0x0000, 0x2000, CRC(d2899cc1) SHA1(fdbec743b06f4cdcc134ef863e4e71337ad0b2c5) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "a37__05.ic17", 0x0000, 0x2000, CRC(9025abea) SHA1(2f03e8572f23624d7cd1215a55109e97fd66e271) )

	ROM_REGION( 0x0800, "68705:mcu", 0 )
	ROM_LOAD( "a37-09_bootleg.37", 0x00000, 0x0800, CRC(79bd6ded) SHA1(4967e95b4461c1bfb4e933d1804677799014f77b) )

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "4_a37-16.61", 0x0000, 0x2000, CRC(e0de0cc4) SHA1(a9a19ed25de26306a328cc158843f43ade6eede1) )

	ROM_REGION( 0x08000, "gfx2", 0 )
	ROM_LOAD( "4_a37-10.8",  0x0000, 0x2000, CRC(c691fdbe) SHA1(b2f4c41cb0d059c9aa8045fc3e8e684bcc96b309) )
	ROM_LOAD( "4_a37-11.9",  0x2000, 0x2000, CRC(0ff8cf5b) SHA1(46a67a8d7d59cdbf1a4ce9e8b9361a5766880e96) )
	ROM_LOAD( "4_a37-12.10", 0x4000, 0x2000, CRC(90fd7496) SHA1(db2a98f4b07996d178c99a2653e1df1be4619644) )
	ROM_LOAD( "4_a37-13.11", 0x6000, 0x2000, CRC(a2526b5e) SHA1(a31999c86aff8977cbec4bd24056a097aaa738c7) )

	ROM_REGION( 0x04000, "gfx3", 0 )
	ROM_LOAD( "4_a37-14.55", 0x0000, 0x2000, CRC(f233c421) SHA1(77da9750f88d032143968104e0b400ed1a07adca) )
	ROM_LOAD( "4_a37-15.56", 0x2000, 0x2000, CRC(58c3f1c9) SHA1(5e0c46a5cd6adfcccba5cf33271a51a23ff9752a) )

	ROM_REGION( 0x0300, "palette", 0 )
	ROM_LOAD( "a37-06.ic13", 0x0000, 0x0100, CRC(e9643b8b) SHA1(7bbb92a42e7c3effb701fc7b2c24f2470f31b063) )
	ROM_LOAD( "a37-07.ic4",  0x0100, 0x0100, CRC(e8f34e11) SHA1(8f438561b8d46ffff00747ed8baf0ebb6a081615) )
	ROM_LOAD( "a37-08.ic3",  0x0200, 0x0100, CRC(50030af0) SHA1(e748ae0b8702b7d20fb65c254dceee23246b3d13) )

	ROM_REGION( 0x0800, "clut", 0 )
	ROM_LOAD( "82s191n",   0x0000, 0x0800, CRC(93c891e3) SHA1(643a0107717b6a434432dda73a0102e6e8adbca7) )
ROM_END

GAME( 2011, retofinv4, retofinv, retofinvb1, retofinv, retofinv_state, empty_init, ROT90, "Paul Smith", "Return of the Invaders (hack)", 0 )

