// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/deco32.cpp"

/*****************
 Fighter's History
********************/

ROM_START( fghthist01 )
	ROM_REGION(0x100000, "maincpu", 0 )
	ROM_LOAD32_WORD( "lc00-1_s01.1f", 0x000000, 0x80000, CRC(aeb6551b) SHA1(7aec2a9cc10df80690e66cbdaf1d4b71cfd28cbd) )
	ROM_LOAD32_WORD( "lc01-1_s01.2f", 0x000002, 0x80000, CRC(c4a218f8) SHA1(fcc1a0ad00818b7cd555a0d36e6d213b8c6c76ba) )

	ROM_REGION(0x10000, "audiocpu", 0 )
	ROM_LOAD( "lc02-1.18k",  0x00000,  0x10000,  CRC(5fd2309c) SHA1(2fb7af54d5cd9bf7dd6fb4f6b82aa52b03294f1f) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "mbf00-8.8a",  0x000000,  0x100000,  CRC(d3e9b580) SHA1(fc4676e0ecc6c32441ff66fa1f990cc3158237db) )

	ROM_REGION( 0x100000, "gfx2", 0 )
	ROM_LOAD( "mbf01-8.9a",  0x000000,  0x100000,  CRC(0c6ed2eb) SHA1(8e37ef4b1f0b6d3370a08758bfd602cb5f221282) )

	ROM_REGION( 0x800000, "gfx3", 0 )
	ROM_LOAD( "mbf02-16.16d",  0x000000,  0x200000,  CRC(c19c5953) SHA1(e6ed26f932c6c86bbd1fc4c000aa2f510c268009) )
	ROM_LOAD( "mbf03-16.17d",  0x200000,  0x200000,  CRC(37d25c75) SHA1(8219d31091b4317190618edd8acc49f97cba6a1e) )
	ROM_LOAD( "mbf04-16.18d",  0x400000,  0x200000,  CRC(f6a23fd7) SHA1(74e5559f17cd591aa25d2ed6c34ac9ed89e2e9ba) )
	ROM_LOAD( "mbf05-16.19d",  0x600000,  0x200000,  CRC(137be66d) SHA1(3fde345183ce04a7a65b4cedfd050d771df7d026) )

	ROM_REGION(0x80000, "oki1", 0 )
	ROM_LOAD( "mbf06.15k",  0x000000,  0x80000,  CRC(fb513903) SHA1(7727a49ff7977f159ed36d097020edef3b5b36ba) )

	ROM_REGION(0x80000, "oki2", 0 )
	ROM_LOAD( "mbf07.16k",  0x000000,  0x80000,  CRC(51d4adc7) SHA1(22106ed7a05db94adc5a783ce34529e29d24d41a) )

	ROM_REGION(512, "proms", 0 )
	ROM_LOAD( "kt-00.8j",  0,  512,  CRC(7294354b) SHA1(14fe42ad5d26d022c0fe9a46a4a9017af2296f40) )

	ROM_REGION( 0x0400, "plds", 0 )
	ROM_LOAD( "ve-00.3d",  0x0000, 0x0117, CRC(384d316c) SHA1(61b50c695d4210c199cf6f7bbe50c8a5ecd1d21c) )
	ROM_LOAD( "ve-01a.4d", 0x0200, 0x0117, CRC(109613c8) SHA1(5991e010c1bc2a827c8ee2c85a9b40e00a3167b3) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Fighter's History
GAME( 1993, fghthist01,     fghthist, fghthist,  fghthist, fghthist_state, init_fghthist,  ROT0, "yumeji[2ch]", "Fighter's History (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )

