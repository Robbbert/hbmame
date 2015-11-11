// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mitchell.cpp"

ROM_START( mitcdemo )
	ROM_REGION( 2*0x50000, "maincpu", 0 )	/* 320k for code */
	ROM_LOAD( "md_04.bin",    0x50000, 0x08000, CRC(f0ed5c22) SHA1(b8e26a09fca279d627bd700c31a82ffc18483192) )   /* Decrypted opcode + data */
	ROM_CONTINUE(             0x00000, 0x08000 )
	ROM_LOAD_OPTIONAL( "pang_02.bin",  0x60000, 0x20000, CRC(3f15bb61) SHA1(4f74ee25f32a201482840158b4d4c7aca1cda684) )   /* Decrypted op codes */
	ROM_LOAD_OPTIONAL( "pang_03.bin",  0x10000, 0x20000, CRC(0c8477ae) SHA1(a31a8c00407dfc3017d56e29fac6114b73248030) )   /* Decrypted data */

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "md_09.bin",    0x000000, 0x20000, CRC(f76acc9a) SHA1(31004f441fc56efb692e2e52c4ecc44748185d0a) )	/* chars */
	/* 40000-7ffff empty */
	ROM_LOAD( "md_11.bin",    0x080000, 0x20000, CRC(48f92e6f) SHA1(6c51cccf5bcf7b3bfe6579a95516a850e99c9b76) )
	/* c0000-fffff empty */

	ROM_REGION( 0x040000, "gfx2", 0 )
	ROM_LOAD( "md10.bin",     0x000000, 0x20000, CRC(709a27b6) SHA1(b7e8d124ee207e75c420762d336a97f4cf4054f6) )	/* sprites */
	ROM_LOAD( "md9.bin",      0x020000, 0x20000, CRC(47de1ec7) SHA1(45bff13a6cdf58bc425fdc3cd2ae22d40d69a2c2) )

	ROM_REGION( 0x80000, "oki", 0 )	/* OKIM */
	ROM_LOAD_OPTIONAL( "bb1.bin",      0x00000, 0x20000, CRC(c52e5b8e) SHA1(933b954bfdd2d67e28b032ffabde192531249c1f) )
ROM_END

GAME( 1989, mitcdemo, 0, pang, pang, mitchell_state, pangb, ROT0, "Charles Doty", "Demo - Mitchell", MACHINE_SUPPORTS_SAVE )
