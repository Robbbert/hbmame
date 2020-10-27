// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/dynax.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /**********************
 Mahjong Electron Base
*************************/

ROM_START( mjelct3s01 ) //mjelct3h
	ROM_REGION( 0x50000, "maincpu", 0 )
	ROM_LOAD( "se-3010_ps01", 0x00000, 0x20000, CRC(676a6322) SHA1(08f8d715b8e76c8575ab6a3fdba8067eb0f8badc) )
	ROM_RELOAD(          0x10000, 0x08000 )
	ROM_CONTINUE(        0x28000, 0x08000 )
	ROM_CONTINUE(        0x20000, 0x08000 )
	ROM_CONTINUE(        0x18000, 0x08000 )

	ROM_REGION( 0x200000, "blitter", 0 )
	ROM_LOAD( "eb-01.rom", 0x000000, 0x100000, CRC(e5c41448) SHA1(b8322e32b0cb3d771316c9c4f7be91de6e422a24) )
	ROM_LOAD( "eb-02.rom", 0x100000, 0x080000, CRC(e1f1b431) SHA1(04a612aff4c30cb8ea741f228bfa7e4289acfee8) )
ROM_END

ROM_START( mjelctrns01 )
	ROM_REGION( 0x50000, "maincpu", 0 )
	ROM_LOAD( "u27b-020_ps01", 0x00000, 0x20000, CRC(bb163451) SHA1(03c4ef439a5bca1a4281bf6f77f7356fdbd24027) )
	ROM_CONTINUE(         0x00000, 0x20000 )
	ROM_RELOAD(           0x10000, 0x20000 )
	ROM_CONTINUE(         0x28000, 0x08000 )
	ROM_CONTINUE(         0x20000, 0x08000 )
	ROM_CONTINUE(         0x18000, 0x08000 )
	ROM_CONTINUE(         0x10000, 0x08000 )

	ROM_REGION( 0x300000, "blitter", 0 )
	ROM_LOAD( "eb-01.rom", 0x000000, 0x100000, CRC(e5c41448) SHA1(b8322e32b0cb3d771316c9c4f7be91de6e422a24) )
	ROM_LOAD( "eb-02_ps01.rom", 0x100000, 0x080000, CRC(f5b354d1) SHA1(d3f35d090de9af3f50aae9ff11de731950256212) )
	ROM_LOAD( "mj-1c020",  0x200000, 0x040000, CRC(f8e8d91b) SHA1(409e276157b328e7bbba5dda6a4c7adc020d519a) )
ROM_END

 /*****************
 Mahjong Tenkaigen
********************/

ROM_START( tenkaibbs01 ) //tenkaibbc
	ROM_REGION( 0x50000, "maincpu", 0 )
	ROM_LOAD( "tdh-12m_ps01.u11", 0x00000, 0x40000, CRC(58eae7cd) SHA1(75d1a86a6fa5816b9e6ee781e334a947149bcccd) )
	ROM_RELOAD(              0x10000, 0x40000 )

	ROM_REGION( 0x300000, "blitter", 0 )
	ROM_LOAD( "lzc-01.u6",  0x000000, 0x100000, CRC(786698e3) SHA1(9ddf4e31f454fb3c7969b1433771e95a976de741) )
	ROM_RELOAD(             0x200000, 0x100000 )
	ROM_LOAD( "lzc-02.u19", 0x100000, 0x100000, CRC(90a19443) SHA1(8f593c00e39dd5acc76b058591019d117967a17b) )
	ROM_LOAD( "lzc-03.u15", 0x200000, 0x020000, CRC(5b552cdf) SHA1(86aaa02dcf8dab670d818287c91c8cb296362401) )  // 1xxxxxxxxxxxxxxxx = 0xFF
ROM_END

// Mahjong Electron Base
GAME( 2004, mjelct3s01,  mjelctrn, mjelctrn, mjelct3,  dynax_state, init_mjelct3,  ROT180, "hack",   "Mahjong Electron Base (parts 2 & 3, No AI Cheat 2004-09-17)",   MACHINE_SUPPORTS_SAVE )
GAME( 2005, mjelctrns01, mjelctrn, mjelctrn, mjelctrn, dynax_state, init_mjelct3,  ROT180, "hack",   "Mahjong Electron Base (parts 2 & 4, No AI Cheat 2005-09-03)",   MACHINE_SUPPORTS_SAVE )
// Mahjong Tenkaigen
GAME( 2006, tenkaibbs01, tenkai, tenkai, tenkai, dynax_state, empty_init, ROT0, "hack",  "Mahjong Tenkaigen (No AI Cheat 2006-12-29)",     MACHINE_SUPPORTS_SAVE )
