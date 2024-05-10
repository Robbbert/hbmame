// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/rohga.cpp"

/*****************
 Schmeiser Robo
******************/

ROM_START( schmeisr01 ) //Description: Press 'Start' to select Celberus
	ROM_REGION(0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "sr001j_s01.8a",  0x000000, 0x80000, CRC(a61407c2) SHA1(64a90c95a3fba9d01cfcb55cd562861cdeaa8f7a) )
	ROM_LOAD16_BYTE( "sr006j_s01.8d",  0x000001, 0x80000, CRC(64afec76) SHA1(8b9bdb2a3db438b710bcd58980ee0411c0e1ea0a) )

	ROM_REGION(0x10000, "audiocpu", 0 )
	ROM_LOAD( "sr013.18p",  0x00000,  0x10000,  CRC(4ac00cbb) SHA1(cbc21e13978ae5e8940c8c22932dc424605c0ba4) )

	ROM_REGION( 0x040000, "gfx1", ROMREGION_ERASE00 )

	ROM_REGION( 0x100000, "gfx2", 0 )
	ROM_LOAD( "sr002-74.9a",  0x000000, 0x080000,  CRC(97e15c7b) SHA1(8697115d4b5ed94a1392034060821d3e354bceb0) )
	ROM_LOAD( "sr003-74.11a", 0x080000, 0x080000,  CRC(ea367971) SHA1(365c27bdef4daa01e926fbcf11ce622186133106) )

	ROM_REGION( 0x200000, "gfx3", 0 )
	ROM_LOAD( "sr007.17d",  0x000000, 0x100000,  CRC(886f80c7) SHA1(c06efc1ce7f51d4e503267e63dc9f762d55ad528) )
	ROM_LOAD( "sr008.18d",  0x100000, 0x100000,  CRC(a74cbc90) SHA1(1aabfec7cd64e7097aa55f0ddc5a2c9e1e25618a) )

	ROM_REGION( 0x400000, "gfx4", 0 )
	ROM_LOAD( "sr004.19a", 0x000000, 0x100000,  CRC(e25434a1) SHA1(136ebb36e9b6caeac885423e8f365008ddcea778) )
	ROM_LOAD( "sr005.20a", 0x100000, 0x100000,  CRC(1630033b) SHA1(e2a5fd7f8839db9d5b41d3cada598a6c07a97368) )
	ROM_LOAD( "sr009.19d", 0x200000, 0x100000,  CRC(7b9d982f) SHA1(55d89ee68ceaf3ca8059177721b6c9a16103b1b4) )
	ROM_LOAD( "sr010.20d", 0x300000, 0x100000,  CRC(6e9e5352) SHA1(357659ff5ab9ce94df3313e9a60125769c7fe10a) )

	ROM_REGION(0x80000, "oki2", 0 )
	ROM_LOAD( "sr011.14p", 0x00000,  0x80000,  CRC(81805616) SHA1(cdca2eb6d12924b9b578b4ce95d5816c7d82f345) )

	ROM_REGION(0x80000, "oki1", 0 )
	ROM_LOAD( "sr012.15p", 0x00000,  0x80000,  CRC(38843d4d) SHA1(0eda60a4d2caa1e57582c354b8be926905d7fb0c) )

	ROM_REGION( 512, "proms", 0 )
	ROM_LOAD( "hb-00.11p", 0x00000,  0x200,  CRC(b7a7baad) SHA1(39781c3412493b985d3616ac31142fc00bbcddf4) )
ROM_END

/*    YEAR  NAME         PARENT     MACHINE   INPUT     CLASS        INIT         MONITOR COMPANY                 FULLNAME FLAGS */
// Schmeiser Robo
GAME( 1993, schmeisr01,  schmeisr,  schmeisr, schmeisr, rohga_state, init_schmeisr, ROT0,   "Yumeji",  "Schmeiser Robo (Enable Celberus)", MACHINE_SUPPORTS_SAVE )

