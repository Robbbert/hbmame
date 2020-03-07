// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mitchell.cpp"

ROM_START( mitcdemo )
	ROM_REGION( 0xa0000, "maincpu", 0 )
	ROM_LOAD( "md_04.bin",    0x50000, 0x08000, CRC(f0ed5c22) SHA1(b8e26a09fca279d627bd700c31a82ffc18483192) )
	ROM_CONTINUE(             0x00000, 0x08000 )
	ROM_LOAD_OPTIONAL( "pang_02.bin",  0x60000, 0x20000, CRC(3f15bb61) SHA1(4f74ee25f32a201482840158b4d4c7aca1cda684) )
	ROM_LOAD_OPTIONAL( "pang_03.bin",  0x10000, 0x20000, CRC(0c8477ae) SHA1(a31a8c00407dfc3017d56e29fac6114b73248030) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "md_09.bin",    0x00000, 0x20000, CRC(f76acc9a) SHA1(31004f441fc56efb692e2e52c4ecc44748185d0a) )
	ROM_LOAD( "md_11.bin",    0x80000, 0x20000, CRC(48f92e6f) SHA1(6c51cccf5bcf7b3bfe6579a95516a850e99c9b76) )

	ROM_REGION( 0x040000, "gfx2", 0 )
	ROM_LOAD( "md10.bin",     0x00000, 0x20000, CRC(709a27b6) SHA1(b7e8d124ee207e75c420762d336a97f4cf4054f6) )
	ROM_LOAD( "md9.bin",      0x20000, 0x20000, CRC(47de1ec7) SHA1(45bff13a6cdf58bc425fdc3cd2ae22d40d69a2c2) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD_OPTIONAL( "bb1.bin",      0x00000, 0x20000, CRC(c52e5b8e) SHA1(933b954bfdd2d67e28b032ffabde192531249c1f) )
ROM_END

GAME( 1989, mitcdemo, 0, pang, pang, mitchell_state, init_pangb, ROT0, "Charles Doty", "Demo - Mitchell", MACHINE_SUPPORTS_SAVE )



//PSmame (c) gaston90 used with permission

 /********************************************
     Proyecto Shadows Mame Build Plus
**********************************************/

/*****
 Pang
*******/

ROM_START( pangbolds01 ) //pangbold3
	ROM_REGION( 2*0x50000, "maincpu", 0 )
	ROM_LOAD( "4_ps01.6l",  0x50000, 0x08000, CRC(f5e6e2fa) SHA1(7ec78f8e99d2fd90ad626ef5b1c36ef667842827) )
	ROM_CONTINUE(      0x00000, 0x08000 )
	ROM_LOAD( "2.3l",  0x60000, 0x20000, CRC(3f15bb61) SHA1(4f74ee25f32a201482840158b4d4c7aca1cda684) )
	ROM_LOAD( "3.5l",  0x10000, 0x20000, CRC(ce6375e4) SHA1(fdd40d82553fcd4d2762ecfd898d0e3112dfde79) )

	ROM_REGION( 0x100000, "gfx1", ROMREGION_ERASEFF )
	ROM_LOAD( "pwe_02.1e", 0x000000, 0x20000, CRC(3a5883f5) SHA1(a8a33071e10f5992e80afdb782c334829f9ae27f) )
	ROM_LOAD( "pw_03.2e",  0x020000, 0x20000, CRC(79a8ed08) SHA1(c1e43889e29b80c7fe2c09b11eecde24450a1ff5) )
	ROM_LOAD( "pwe_04.1g", 0x080000, 0x20000, CRC(166a16ae) SHA1(7f907c78b7ac8c99e3d79761a6ae689c77e3a1f5) )
	ROM_LOAD( "pw_05.2g",  0x0a0000, 0x20000, CRC(2fb3db6c) SHA1(328814d28569fec763975a8ae4c2767517a680af) )

	ROM_REGION( 0x040000, "gfx2", 0 )
	ROM_LOAD( "8.7o",     0x000000, 0x10000, CRC(f3188aa1) SHA1(f59da8986c0c7d74185211eae1d1cc3f59a54f82) )
	ROM_LOAD( "7.5o",     0x010000, 0x10000, CRC(011da14b) SHA1(3af9c5ca263b3df98b4f4c88d5428a115ddebef8) )
	ROM_LOAD( "6.3o",     0x020000, 0x10000, CRC(0e25e797) SHA1(88c99e544923142256c93ed2b71f06489f6a90a8) )
	ROM_LOAD( "5.1o",     0x030000, 0x10000, CRC(6daa4e27) SHA1(23411928de911b6303efa3a229646001459e4c70) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "1.1a",      0x00000, 0x10000, CRC(b6463907) SHA1(b79e0dca10c639b7f0ea9cbc49300b80708d46fa) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Pang
GAME( 1989, pangbolds01,    pang,     pang,       pang,     mitchell_state, init_pangb,      ROT0,   "bootleg",    "Pang (bootleg, set 7)[bootleg]", MACHINE_SUPPORTS_SAVE )


