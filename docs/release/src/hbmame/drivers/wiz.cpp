// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/wiz.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*******
 Stinger
**********/

ROM_START( stingers01 )
	ROM_REGION( 0xc000, "maincpu", 0 )
	ROM_LOAD( "1-5j.bin",     0x0000, 0x2000, CRC(1a2ca600) SHA1(473e89f2c49f6e6f38df5d6fc2267ffecf84c6c8) )
	ROM_LOAD( "2-6j_ps01.bin",     0x2000, 0x2000, CRC(dd73346b) SHA1(5410b58a137b6168ec3a1a008e3a1fbd37d57096) )
	ROM_LOAD( "3-8j.bin",     0x4000, 0x2000, CRC(404c932e) SHA1(c23eac49e06ff38564062c0e8c8cdadf877f1d6a) )
	ROM_LOAD( "4-9j.bin",     0x6000, 0x2000, CRC(2d570f91) SHA1(31d54d9fd5254c33f07c605bd6112c7eb53c42a1) )
	ROM_LOAD( "5-10j.bin",    0x8000, 0x2000, CRC(c841795c) SHA1(e03860813c03ca1c737935accc2b5fe87c6b624a) )

	ROM_REGION( 0x2000, "audiocpu", 0 )
	ROM_LOAD( "6-9f.bin",     0x0000, 0x2000, CRC(79757f0c) SHA1(71be938c32c6a84618763761786ecc5d7d47581a) )

	ROM_REGION( 0x6000,  "gfx1", 0 )
	ROM_LOAD( "7-9e.bin",     0x0000, 0x2000, CRC(775489be) SHA1(5fccede323895626cf2eabd606ed21282aa36356) )
	ROM_LOAD( "8-11e.bin",    0x2000, 0x2000, CRC(43c61b3f) SHA1(5cdb6a5096b42406c2f2784d37e4e39207c35d40) )
	ROM_LOAD( "9-14e.bin",    0x4000, 0x2000, CRC(c9ed8fc7) SHA1(259d7681b663adb1c5fe057e2ef08469ddcbd3c3) )

	ROM_REGION( 0x6000,  "gfx2", 0 )
	ROM_LOAD( "10-9h.bin",    0x0000, 0x2000, CRC(6fc3a22d) SHA1(6875b86d60a06aa329d8ff18d0eb48d158074c5d) )
	ROM_LOAD( "11-11h.bin",   0x2000, 0x2000, CRC(3df1f57e) SHA1(e365ee4cc8c055cc39abb4598ad80597d3ae19c7) )
	ROM_LOAD( "12-14h.bin",   0x4000, 0x2000, CRC(2fbe1391) SHA1(669edc154164944d82dfccda328774ea4a2318ba) )

	ROM_REGION( 0x0300,  "proms", 0 )
	ROM_LOAD( "stinger.a7",   0x0000, 0x0100, CRC(52c06fc2) SHA1(b416077fcfabe0dbb1ca30752de6a219ea896f75) )
	ROM_LOAD( "stinger.b7",   0x0100, 0x0100, CRC(9985e575) SHA1(b0d609968917121325760f8d4777066abdb7ccfc) )
	ROM_LOAD( "stinger.a8",   0x0200, 0x0100, CRC(76b57629) SHA1(836763948753b7fed97c9e5d90a16dc4ba68f42a) )
ROM_END

ROM_START( stinger2s01 )
	ROM_REGION( 0xc000, "maincpu", 0 )
	ROM_LOAD( "n1.bin",       0x0000, 0x2000, CRC(f2d2790c) SHA1(0e5e92ef45b5bc27b0818f83c89b3bda0e701403) )
	ROM_LOAD( "n2_ps01.bin",  0x2000, 0x2000, CRC(17723c86) SHA1(229dd847a58cf0fbd8a3b52c41178636e6ad9425) )
	ROM_LOAD( "n3.bin",       0x4000, 0x2000, CRC(f1794d36) SHA1(7954500f489c0bc58cda8e7ffc2e4474759fdc33) )
	ROM_LOAD( "n4.bin",       0x6000, 0x2000, CRC(230ba682) SHA1(c419ffebd021d41b3f5021948007fb6bcdb1cdf7) )
	ROM_LOAD( "n5.bin",       0x8000, 0x2000, CRC(a03a01da) SHA1(28fecac7a821ac4718242919840266a907160df0) )

	ROM_REGION( 0x2000, "audiocpu", 0 )
	ROM_LOAD( "6-9f.bin",     0x0000, 0x2000, CRC(79757f0c) SHA1(71be938c32c6a84618763761786ecc5d7d47581a) )

	ROM_REGION( 0x6000,  "gfx1", 0 )    /* sprites/chars */
	ROM_LOAD( "7-9e.bin",     0x0000, 0x2000, CRC(775489be) SHA1(5fccede323895626cf2eabd606ed21282aa36356) )
	ROM_LOAD( "8-11e.bin",    0x2000, 0x2000, CRC(43c61b3f) SHA1(5cdb6a5096b42406c2f2784d37e4e39207c35d40) )
	ROM_LOAD( "9-14e.bin",    0x4000, 0x2000, CRC(c9ed8fc7) SHA1(259d7681b663adb1c5fe057e2ef08469ddcbd3c3) )

	ROM_REGION( 0x6000,  "gfx2", 0 )    /* sprites/chars */
	ROM_LOAD( "10.bin",       0x0000, 0x2000, CRC(f6721930) SHA1(fb903f1deb5f093ff5fe129e213966af58a68339) )
	ROM_LOAD( "11.bin",       0x2000, 0x2000, CRC(a4404e63) SHA1(50ae99748547af20e04f6c6c8c7eba85f967b9dc) )
	ROM_LOAD( "12.bin",       0x4000, 0x2000, CRC(b60fa88c) SHA1(2d3bca35076625251933989f5e566d5d3290542b) )

	ROM_REGION( 0x0300,  "proms", 0 )
	ROM_LOAD( "stinger.a7",   0x0000, 0x0100, CRC(52c06fc2) SHA1(b416077fcfabe0dbb1ca30752de6a219ea896f75) )
	ROM_LOAD( "stinger.b7",   0x0100, 0x0100, CRC(9985e575) SHA1(b0d609968917121325760f8d4777066abdb7ccfc) )
	ROM_LOAD( "stinger.a8",   0x0200, 0x0100, CRC(76b57629) SHA1(836763948753b7fed97c9e5d90a16dc4ba68f42a) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Stinger
GAME( 1983, stingers01,  stinger, stinger, stinger,  wiz_state, init_stinger, ROT90,  "Wayder", "Stinger (Bug fix - Taxiing Speed)", MACHINE_IMPERFECT_SOUND | MACHINE_IMPERFECT_COLORS | MACHINE_SUPPORTS_SAVE )
GAME( 1983, stinger2s01, stinger, stinger, stinger2, wiz_state, init_stinger, ROT90,  "Wayder", "Stinger (Bug fix - Taxiing Speed)(prototype)", MACHINE_IMPERFECT_SOUND | MACHINE_IMPERFECT_COLORS | MACHINE_SUPPORTS_SAVE )


