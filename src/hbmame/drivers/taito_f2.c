// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_f2.c"

ROM_START( f2demo )
	ROM_REGION( 0x80000, "maincpu", 0 )     /* 512k for 68000 code */
	ROM_LOAD16_BYTE( "lq09.bin",  0x00000, 0x20000, CRC(809a968b) SHA1(06e8be120869371d33c61fa14c48fe4e5fd659ec) )
	ROM_LOAD16_BYTE( "lq11.bin",  0x00001, 0x20000, CRC(7ba3a5cb) SHA1(08f5392296cceadcae0cd34e9dfe76c835583231) )

	ROM_REGION( 0x080000, "gfx1", 0 )	/* SCR */
	ROM_LOAD( "f2d_scr.bin", 0x000000, 0x80000, CRC(b178fb05) SHA1(3d33dd822622579c75b55971926f6eb04d33f5a6) )	/* TC0100SCN #1 */

	ROM_REGION( 0x100000, "gfx2", 0 )	/* OBJ */
	ROM_LOAD( "f2d_obj0.bin", 0x00000, 0x80000, CRC(1bb8aa37) SHA1(9051270485a7a995779c80b4e692b8395503d6c3) )
	ROM_LOAD( "f2d_obj1.bin", 0x80000, 0x80000, CRC(75660aac) SHA1(6a521e1d2a632c26e53b83d2cc4b0edecfc1e68c) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )		/* sound cpu */
	ROM_LOAD( "c49-08.9",    0x00000, 0x04000, CRC(413c310c) SHA1(cecb1c0c9fe3c8b744f95ce29009650a289107ab) )
	ROM_CONTINUE(            0x10000, 0x0c000 )	/* banked stuff */

	ROM_REGION( 0x80000, "ymsnd", 0 )	/* ADPCM samples */
	ROM_LOAD( "lk_snd.bin",  0x00000, 0x80000, CRC(474d45a4) SHA1(20cb818d753a185973098007e645f1aa75c5528d) )

	/* no Delta-T samples */
ROM_END

GAME( 1990, f2demo, 0, liquidk, liquidk, driver_device, 0, ROT180, "Charles Doty", "Demo - Taito F2", MACHINE_SUPPORTS_SAVE )
