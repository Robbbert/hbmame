// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/toaplan/snowbro2.cpp"

/*************
 Snow Bros 2
**************/
ROM_START( snowbro2s01 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "s01.4", 0x000000, 0x080000, CRC(1ad75ae7) SHA1(4c9d7480df4b94aaee5616939547456a0c560f19) )

	ROM_REGION( 0x300000, "gp9001", 0 )
	ROM_LOAD( "rom2-l", 0x000000, 0x100000, CRC(e9d366a9) SHA1(e87e3966fce3395324b90db6c134b3345104c04b) )
	ROM_LOAD( "rom2-h", 0x100000, 0x080000, CRC(9aab7a62) SHA1(611f6a15fdbac5d3063426a365538c1482e996bf) )
	ROM_LOAD( "rom3-l", 0x180000, 0x100000, CRC(eb06e332) SHA1(7cd597bfffc153d178530c0f0903bebd751c9dd1) )
	ROM_LOAD( "rom3-h", 0x280000, 0x080000, CRC(df4a952a) SHA1(b76af61c8437caca573ff1312832898666a611aa) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "rom4", 0x00000, 0x80000, CRC(638f341e) SHA1(aa3fca25f099339ece1878ea730c5e9f18ec4823) )
ROM_END

ROM_START( snowbro2s02 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "s02.4", 0x000000, 0x080000, CRC(2f219f0a) SHA1(49057ca9f736cd7c8c4588554599250b4aaf6cd1) )

	ROM_REGION( 0x300000, "gp9001", 0 )
	ROM_LOAD( "rom2-l", 0x000000, 0x100000, CRC(e9d366a9) SHA1(e87e3966fce3395324b90db6c134b3345104c04b) )
	ROM_LOAD( "rom2-h", 0x100000, 0x080000, CRC(9aab7a62) SHA1(611f6a15fdbac5d3063426a365538c1482e996bf) )
	ROM_LOAD( "rom3-l", 0x180000, 0x100000, CRC(eb06e332) SHA1(7cd597bfffc153d178530c0f0903bebd751c9dd1) )
	ROM_LOAD( "rom3-h", 0x280000, 0x080000, CRC(df4a952a) SHA1(b76af61c8437caca573ff1312832898666a611aa) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "rom4", 0x00000, 0x80000, CRC(638f341e) SHA1(aa3fca25f099339ece1878ea730c5e9f18ec4823) )
ROM_END

/*    YEAR  NAME          PARENT           MACHINE   INPUT     CLASS           INIT           MONITOR COMPANY       FULLNAME FLAGS */
// Snow Bros 2
GAME( 1994, snowbro2s01,  snowbro2,        snowbro2, snowbro2, snowbro2_state, empty_init,    ROT0,   "RockyWall", "Snow Bros 2 (Ex Super Version)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, snowbro2s02,  snowbro2,        snowbro2, snowbro2, snowbro2_state, empty_init,    ROT0,   "DDJ",       "Snow Bros 2 (Change Character)", MACHINE_SUPPORTS_SAVE )

