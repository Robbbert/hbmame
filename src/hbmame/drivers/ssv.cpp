// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/ssv.cpp"

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*************
 Survival Arts
****************/

ROM_START( survartss01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "prl-r6_ps01.u4", 0x000000, 0x080000, CRC(9daf2473) SHA1(f2375ba98f84373c00109e55841dab2c582d0ba6) )
	ROM_LOAD16_BYTE( "prh-r5_ps01.u3", 0x000001, 0x080000, CRC(5d0d1f56) SHA1(a46a8eaf07db5200219b954ffc954759328cbadb) )

	ROM_REGION( 0x1800000, "gfx1", 0 )
	ROM_LOAD( "si001-01.u27", 0x0000000, 0x200000, CRC(8b38fbab) SHA1(c4a67b24b33d4eef7b0f885bd69cae6c67bd3981) )
	ROM_LOAD( "si001-04.u26", 0x0200000, 0x200000, CRC(34248b54) SHA1(077198f8de1622b71c580e34d5ad1b6bf3229fe9) )
	ROM_LOAD( "si001-07.u25", 0x0400000, 0x200000, CRC(497d6151) SHA1(a9860c75943c0fd2991660ce2a9505edc6c2fa46) )

	ROM_LOAD( "si001-02.u23", 0x0600000, 0x200000, CRC(cb4a2dbd) SHA1(26cdd1b54a3fa1dc3c3a8945d1a3562e9c62ace6) )
	ROM_LOAD( "si001-05.u22", 0x0800000, 0x200000, CRC(8f092381) SHA1(6c49f1f5b3c31bd7c6a93ba0450d9f64fd512633) )
	ROM_LOAD( "si001-08.u21", 0x0a00000, 0x200000, CRC(182b88c4) SHA1(a5b6a3e1fd67f036b1255385e81b6a3eb69f9f3f) )

	ROM_LOAD( "si001-03.u17", 0x0c00000, 0x200000, CRC(92fdf652) SHA1(cf7aeb3a1e8ffe34cf24cb919a0ab3cc90202fa9) )
	ROM_LOAD( "si001-06.u16", 0x0e00000, 0x200000, CRC(9a62f532) SHA1(7e7ba1224e52b33a9bd14058230efc871178c4f8) )
	ROM_LOAD( "si001-09.u15", 0x1000000, 0x200000, CRC(0955e393) SHA1(0be9134190706eaee49177034b0536b05c4bc7ac) )

	ROM_FILL( 0x1200000, 0x600000, 0x000000 )

	ROM_REGION16_BE( 0x400000, "ensoniq.2", 0 )
	ROM_LOAD16_WORD_SWAP( "si001-10.u9", 0x000000, 0x100000, CRC(5642b333) SHA1(84936af8b3882e116b279e422075f35aabdd232f) )
	ROM_LOAD16_WORD_SWAP( "si001-11.u8", 0x100000, 0x100000, CRC(a81e6ea6) SHA1(499f070500895ed7b6785b42fb6bbf973fc6dc04) )
	ROM_LOAD16_WORD_SWAP( "si001-12.u7", 0x200000, 0x100000, CRC(e9b2b45b) SHA1(17fd27cdb8a0b9932cb1e71e0547c0d9d6fc7d06) )
	ROM_LOAD16_WORD_SWAP( "si001-13.u6", 0x300000, 0x100000, CRC(d66a7e26) SHA1(57b659daef00421b6742963f792bd5e020f625c9) )

	ROM_REGION( 0x0200, "plds", 0 )
	ROM_LOAD_OPTIONAL( "gal16v8b.u5",   0x0000, 0x0117, CRC(378ce368) SHA1(cc068c9b63e6f91165f9caec581645ad45b2d3d0) )
ROM_END

//    YEAR  NAME            PARENT    MACHINE    INPUT      CLASS       INIT  MONITOR COMPANY   FULLNAME                            FLAGS
// Survival Arts
GAME( 1993,  survartss01,  survarts,  survarts, survarts, ssv_state, init_ssv, ROT0,   "DDJ",  "Survival Arts (Easy Move)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
