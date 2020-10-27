// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/ginganin.cpp"


// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

/*****************
 Ginga NinkyouDen
*******************/

ROM_START( ginganins01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "gn_02.bin", 0x00000, 0x10000, CRC(4a4e012f) SHA1(7c94a5b6b71e037af355f3aa4623be1f585db8dc) )
	ROM_LOAD16_BYTE( "gn_01.bin", 0x00001, 0x10000, CRC(30256fcb) SHA1(dc15e0da88ae5cabe0150f7290508c3d58c06c11) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "gn_05.bin", 0x00000, 0x10000, CRC(e76e10e7) SHA1(b16f10a1a01b7b04221c9bf1b0d157e936bc5fb5) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "gn_15_ps01.bin", 0x000000, 0x10000, CRC(eba72bc2) SHA1(5a3496f0eb9e0942b6957f3d0121e84a72f6d032) )
	ROM_LOAD( "gn_14.bin", 0x010000, 0x10000, CRC(e73fe668) SHA1(fa39fddd7448d3fc6b539506e33b951db205afa1) )

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "gn_12.bin", 0x000000, 0x10000, CRC(c134a1e9) SHA1(8bace0f0169e61f1b7254393fa9cad6dca09c335) )
	ROM_LOAD( "gn_13.bin", 0x010000, 0x10000, CRC(1d3bec21) SHA1(305823c78cad9288f918178e1c24cb0459ba2a6e) )

	ROM_REGION( 0x04000, "gfx3", 0 )
	ROM_LOAD( "gn_10_ps01.bin", 0x000000, 0x04000, CRC(5d5ce06f) SHA1(0df6867ed9af78a4e86455f64d3d57c68d148af0) )

	ROM_REGION( 0x50000, "gfx4", 0 )
	ROM_LOAD( "gn_06.bin", 0x000000, 0x10000, CRC(bdc65835) SHA1(53222fc3ec15e641289abb754657b0d59b88b66b) )
	ROM_CONTINUE(          0x040000, 0x10000 )
	ROM_LOAD( "gn_07_ps01.bin", 0x010000, 0x10000, CRC(d12bae11) SHA1(2f1a8289b3a69bd2624e03d48f75312478aa120e) )
	ROM_LOAD( "gn_08.bin", 0x020000, 0x10000, CRC(f7c73c18) SHA1(102700e2217bcd1532af56ee6a00ad608c8217db) )
	ROM_LOAD( "gn_09.bin", 0x030000, 0x10000, CRC(a5e07c3b) SHA1(cdda02cd847330575612cb33d1bb38a5d50a3e6d) )

	ROM_REGION( 0x08000, "bgrom", 0 )
	ROM_LOAD( "gn_11.bin", 0x00000, 0x08000, CRC(f0d0e605) SHA1(0c541e8e036573be1d99ecb71fdb4568ca8cc269) )

	ROM_REGION( 0x20000, "ymsnd", 0 )
	ROM_LOAD( "gn_04.bin", 0x00000, 0x10000, CRC(0ed9133b) SHA1(77f628e8ec28016efac2d906146865ca4ec54bd5) )
	ROM_LOAD( "gn_03.bin", 0x10000, 0x10000, CRC(f1ba222c) SHA1(780c0bd0045bac1e1bb3209576383db90504fbf3) )
ROM_END

/*    YEAR  NAME           PARENT    MACHINE     INPUT     INIT            MONITOR COMPANY         FULLNAME FLAGS */
// Ginga NinkyouDen
GAME( 1987, ginganins01,  ginganin,        ginganin, ginganin, ginganin_state, init_ginganin, ROT0, "Nunimmanse", "Ginga NinkyouDen (Korean Translation)", MACHINE_SUPPORTS_SAVE )
