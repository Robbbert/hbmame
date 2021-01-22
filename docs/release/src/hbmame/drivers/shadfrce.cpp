// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/shadfrce.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***********
 Shadow Force
**************/

ROM_START( shadfrces01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "32a12-011_ps01.34", 0x00001, 0x40000, CRC(6b9afcd6) SHA1(2d87e3cbb430762df0782416871af363158938b2) )
	ROM_LOAD16_BYTE( "32a13-010_ps01.26", 0x00000, 0x40000, CRC(52d1861a) SHA1(2d9a37c3cfaa0ed382c738f5ddb289dd1b37bc64) )
	ROM_LOAD16_BYTE( "32a14-0.33",  0x80001, 0x40000, CRC(08279be9) SHA1(1833526b23feddb58b21874070ad2bf3b6be8dca) )
	ROM_LOAD16_BYTE( "32a15-0.14",  0x80000, 0x40000, CRC(bfcadfea) SHA1(1caa9fc30d8622ce4c7221039c446e99cc8f5346) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "32j10-0.42",  0x00000, 0x10000, CRC(65daf475) SHA1(7144332b2d17af8645e22e1926b33113db0d20e2) )

	ROM_REGION( 0x020000, "chars", 0 )
	ROM_LOAD( "32a11-0.55",  0x00000, 0x20000, CRC(cfaf5e77) SHA1(eab76e085f695c74cc868aaf95f04ff2acf66ee9) )

	ROM_REGION( 0xa00000, "sprites", 0 )
	ROM_LOAD( "32j4-0.12",  0x000000, 0x200000, CRC(1ebea5b6) SHA1(35bd49dda9ad75326d45ffb10c87d83fc4f1b7a8) )
	ROM_LOAD( "32j5-0.13",  0x200000, 0x200000, CRC(600026b5) SHA1(5641246300d7e20dcff1eae004647faaee6cd1c6) )
	ROM_LOAD( "32j6-0.24",  0x400000, 0x200000, CRC(6cde8ebe) SHA1(750933798235951fe24b2e667c33f692612c0aa0) )
	ROM_LOAD( "32j7-0.25",  0x600000, 0x200000, CRC(bcb37922) SHA1(f3eee73c8b9f4873a7f1cc42e334e7502eaee3c8) )
	ROM_LOAD( "32j8-0.32",  0x800000, 0x200000, CRC(201bebf6) SHA1(c89d2895ea5b19daea1f88542419f4e10f437c73) )

	ROM_REGION( 0x300000, "tiles", 0 )
	ROM_LOAD( "32j1-0.4",  0x000000, 0x100000, CRC(f1cca740) SHA1(339079b95ca137e66b4f032ad67a0adf58cca100) )
	ROM_LOAD( "32j2-0.5",  0x100000, 0x100000, CRC(5fac3e01) SHA1(20c30f4c76e303285ae37e596afe86aa4812c3b9) )
	ROM_LOAD( "32j3-0.6",  0x200000, 0x100000, CRC(d297925e) SHA1(5bc4d37bf0dc54114884c816b94a64ef1ccfeda5) )

	ROM_REGION( 0x080000, "oki", 0 )
	ROM_LOAD( "32j9-0.76",  0x000000, 0x080000, CRC(16001e81) SHA1(67928d2024f963aee91f1498b6f4c76101d2f3b8) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Shadow Force
GAME( 1993, shadfrces01,   shadfrce,  shadfrce, shadfrce, shadfrce_state, empty_init, ROT0, "KrysoFun", "Shadow Force (Enable All Characters)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )



