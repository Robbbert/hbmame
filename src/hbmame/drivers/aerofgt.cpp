// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/aerofgt.cpp"

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /**************
 Karate Blazers
*****************/

ROM_START( karatblzas01 ) //karatblza
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "v2_po01.u14",  0x00000, 0x40000, CRC(d3df88c4) SHA1(9a5a9d2ec96f4614fbf861a5cd62f6f5b907b2fb) ) 
	ROM_LOAD16_WORD_SWAP( "v1.u15",     0x40000, 0x40000, CRC(d16ee21b) SHA1(d454cdf22b72a537b9d7ae73deb8136a4f09da47) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) 
	ROM_LOAD( "v5.u92", 0x00000, 0x20000, CRC(97d67510) SHA1(1ffd419e3dec7de1099cd5819b0309f7dd0df80e) ) 

	ROM_REGION( 0x80000, "gfx1", 0 )
	ROM_LOAD( "gha.u55", 0x00000, 0x80000, CRC(3e0cea91) SHA1(bab41715f106d364013b64649441d280bc6893cf) )

	ROM_REGION( 0x80000, "gfx2", 0 )
	ROM_LOAD( "gh9.u61", 0x00000, 0x80000, CRC(5d1676bd) SHA1(6227d489c9c6259a0ac2bef62821fbf94efca8c6) )

	ROM_REGION( 0x400000, "spritegfx", 0 )
	ROM_LOAD( "u42",    0x000000, 0x100000, CRC(65f0da84) SHA1(0bfbc6f4b87583703246704eb9fa13b1b3e6f90e) )
	ROM_LOAD( "v3.u44", 0x100000, 0x020000, CRC(34bdead2) SHA1(99f9a8cac807fcd599db55d2dc624ed92a3862ef) )
	ROM_LOAD( "u43",    0x200000, 0x100000, CRC(7b349e5d) SHA1(8590a328f403e2c697a8d698c08d4adaf01fff62) )
	ROM_LOAD( "v4.u45", 0x300000, 0x020000, CRC(be4d487d) SHA1(6d19c91d0498c43017219f0c10f4845a51ccfa7f) )

	ROM_REGION( 0x100000, "gfx4", 0 )
	ROM_LOAD( "u59.ghb", 0x000000, 0x80000, CRC(158c9cde) SHA1(a2c1b404d40e6c2627691f5c7a3f63484bd5d2de) )
	ROM_LOAD( "ghd.u60", 0x080000, 0x80000, CRC(73180ae3) SHA1(e4eaf6693826d9e72032d0a0e25938a23ab7d792) )

	ROM_REGION( 0x080000, "ymsnd.deltat", 0 )
	ROM_LOAD( "u105.gh8", 0x000000, 0x080000, CRC(7a68cb1b) SHA1(1bdd0000c2d68019b9e5bf8f7ad84a6ae1af8443) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "u104", 0x000000, 0x100000, CRC(5795e884) SHA1(a4178497ad0a1e60ceb87612b218d77b36d2a11b) )
ROM_END

ROM_START( karatblzas02 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "v2_po02.u14",  0x00000, 0x40000, CRC(f3da2f95) SHA1(ece19ff151d1aab321e17ec20ebe6c26e6da27f6) ) 
	ROM_LOAD16_WORD_SWAP( "v1.u15",     0x40000, 0x40000, CRC(d16ee21b) SHA1(d454cdf22b72a537b9d7ae73deb8136a4f09da47) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) 
	ROM_LOAD( "v5.u92", 0x00000, 0x20000, CRC(97d67510) SHA1(1ffd419e3dec7de1099cd5819b0309f7dd0df80e) ) 

	ROM_REGION( 0x80000, "gfx1", 0 )
	ROM_LOAD( "gha.u55", 0x00000, 0x80000, CRC(3e0cea91) SHA1(bab41715f106d364013b64649441d280bc6893cf) )

	ROM_REGION( 0x80000, "gfx2", 0 )
	ROM_LOAD( "gh9.u61", 0x00000, 0x80000, CRC(5d1676bd) SHA1(6227d489c9c6259a0ac2bef62821fbf94efca8c6) )

	ROM_REGION( 0x400000, "spritegfx", 0 )
	ROM_LOAD( "u42",    0x000000, 0x100000, CRC(65f0da84) SHA1(0bfbc6f4b87583703246704eb9fa13b1b3e6f90e) )
	ROM_LOAD( "v3.u44", 0x100000, 0x020000, CRC(34bdead2) SHA1(99f9a8cac807fcd599db55d2dc624ed92a3862ef) )
	ROM_LOAD( "u43",    0x200000, 0x100000, CRC(7b349e5d) SHA1(8590a328f403e2c697a8d698c08d4adaf01fff62) )
	ROM_LOAD( "v4.u45", 0x300000, 0x020000, CRC(be4d487d) SHA1(6d19c91d0498c43017219f0c10f4845a51ccfa7f) )

	ROM_REGION( 0x100000, "gfx4", 0 )
	ROM_LOAD( "u59.ghb", 0x000000, 0x80000, CRC(158c9cde) SHA1(a2c1b404d40e6c2627691f5c7a3f63484bd5d2de) )
	ROM_LOAD( "ghd.u60", 0x080000, 0x80000, CRC(73180ae3) SHA1(e4eaf6693826d9e72032d0a0e25938a23ab7d792) )

	ROM_REGION( 0x080000, "ymsnd.deltat", 0 )
	ROM_LOAD( "u105.gh8", 0x000000, 0x080000, CRC(7a68cb1b) SHA1(1bdd0000c2d68019b9e5bf8f7ad84a6ae1af8443) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "u104", 0x000000, 0x100000, CRC(5795e884) SHA1(a4178497ad0a1e60ceb87612b218d77b36d2a11b) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Karate Blazers
GAME( 2017, karatblzas01,  karatblz, karatblz,   karatblz,  aerofgt_state, empty_init,      ROT0,   "hack",    "Karate Blazers (1V4 2017-09-30)", MACHINE_SUPPORTS_SAVE | MACHINE_NO_COCKTAIL )
GAME( 2009, karatblzas02,  karatblz, karatblz,   karatblz,  aerofgt_state, empty_init,      ROT0,   "pipi899",    "Karate Blazers (Super Version V1 2009-03-11)", MACHINE_SUPPORTS_SAVE | MACHINE_NO_COCKTAIL )

