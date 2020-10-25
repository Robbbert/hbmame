// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/flstory.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*******************
 The FairyLand Story
**********************/

ROM_START( flstoryjs01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "cpu-a45_ps01.15",   0x0000, 0x4000, CRC(1be138fa) SHA1(eaf3b2810d7f4a95930024ec440bf2ba59230e2d) )
	ROM_LOAD( "cpu-a45_ps01.16",   0x4000, 0x4000, CRC(74b3f227) SHA1(f8ba5c5db92cda691febecdf62b148157adf744a) )
	ROM_LOAD( "cpu-a45_ps01.17",   0x8000, 0x4000, CRC(1309b079) SHA1(0b82af55273aba6433a789b1d6cfbda650ab85c9) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "a45_12.8",     0x0000, 0x2000, CRC(d6f593fb) SHA1(8551ef22c2cdd9df8d7949a178883f56ea56a4a2) )
	ROM_LOAD( "a45_13.9",     0x2000, 0x2000, CRC(451f92f9) SHA1(f4196e6d3420983b74001303936d086a48b10827) )

	ROM_REGION( 0x0800, "bmcu:mcu", 0 )
	ROM_LOAD( "a45-20.mcu",   0x0000, 0x0800, CRC(7d2cdd9b) SHA1(b9a7b4c7d9d58b4b7cab1304beaa9d17f9559419) )

	ROM_REGION( 0x20000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "vid-a45.18",   0x00000, 0x4000, CRC(6f08f69e) SHA1(8f1b7e63a38f855cf26d57aed678da7cf1378fdf) )
	ROM_LOAD( "vid-a45_ps01.06",   0x04000, 0x4000, CRC(04be939f) SHA1(eee61a1de548a9b9d368b689426f564d7d6680f4) )
	ROM_LOAD( "vid-a45.08",   0x08000, 0x4000, CRC(d0b028ca) SHA1(c8bd9136ad3180002961ecfe600fc91a3c891539) )
	ROM_LOAD( "vid-a45_ps01.20",   0x0c000, 0x4000, CRC(51d53e2f) SHA1(30c7665a443af6071a56f64409fea58ace77f547) )
	ROM_LOAD( "vid-a45.19",   0x10000, 0x4000, CRC(2b572dc9) SHA1(9e14428663819e18829c625b4ae91a8a5530eb33) )
	ROM_LOAD( "vid-a45_ps01.07",   0x14000, 0x4000, CRC(20f02f0e) SHA1(ff5c3f171ff9b72fc9d03d776f0e8f16205f4a79) )
	ROM_LOAD( "vid-a45.09",   0x18000, 0x4000, CRC(8336be58) SHA1(b92d37856870c4128a860d8ae02fa647743b99e3) )
	ROM_LOAD( "vid-a45_ps01.21",   0x1c000, 0x4000, CRC(b6e3caca) SHA1(7f045f0d51216710a68093285cafef7b4aeb4b8e) )
ROM_END

/*    YEAR  NAME           PARENT    MACHINE     INPUT     INIT            MONITOR COMPANY         FULLNAME FLAGS */
// The FairyLand Story
GAME( 1985, flstoryjs01,  flstory,  flstory,      flstory,  flstory_state, empty_init, ROT180, "hack", "The FairyLand Story (Translation Chinese)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
