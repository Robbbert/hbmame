// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/toaplan/batsugun.cpp"

/****************
 Batsugun
*******************/
ROM_START( batsugun01 ) // batsugunt
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "s01.1a", 0x000000, 0x080000, CRC(63d28fd3) SHA1(aa3e111d596f408b735e4f79ab654eea24bd6f0c) )

	ROM_REGION( 0x400000, "gp9001_0", 0 )
	ROM_LOAD( "tp030_3l.bin", 0x000000, 0x100000, CRC(3024b793) SHA1(e161db940f069279356fca2c5bf2753f07773705) )
	ROM_LOAD( "tp030_3h.bin", 0x100000, 0x100000, CRC(ed75730b) SHA1(341f0f728144a049486d996c9bb14078578c6879) )
	ROM_LOAD( "tp030_4l.bin", 0x200000, 0x100000, CRC(fedb9861) SHA1(4b0917056bd359b21935358c6bcc729262be6417) )
	ROM_LOAD( "tp030_4h.bin", 0x300000, 0x100000, CRC(d482948b) SHA1(31be7dc5cff072403b783bf203b9805ffcad7284) )

	ROM_REGION( 0x200000, "gp9001_1", 0 )
	ROM_LOAD( "tp030_5.bin",  0x000000, 0x100000, CRC(bcf5ba05) SHA1(40f98888a29cdd30cda5dfb60fdc667c69b0fdb0) )
	ROM_LOAD( "tp030_6.bin",  0x100000, 0x100000, CRC(0666fecd) SHA1(aa8f921fc51590b5b05bbe0b0ad0cce5ff359c64) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "tp030_2.bin", 0x00000, 0x40000, CRC(276146f5) SHA1(bf11d1f6782cefcad77d52af4f7e6054a8f93440) )
ROM_END


/*    YEAR  NAME          PARENT           MACHINE   INPUT     CLASS           INIT           MONITOR COMPANY       FULLNAME FLAGS */
// Batsugun
GAME( 1993, batsugun01,   batsugun,        batsugun, batsugun, batsugun_state, empty_init,  ROT270, "Eezezy", "Batsugun Transparent (Crazy Gun Transparent Version, 2015-03-22)", MACHINE_SUPPORTS_SAVE )

