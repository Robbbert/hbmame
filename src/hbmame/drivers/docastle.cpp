// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/docastle.cpp"

ROM_START( sdreturn )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "2764.p1",      0x0000, 0x2000, CRC(95c86f8e) SHA1(9fe44911e0aa8d4c7299472a31c401e064d63d17) )
	ROM_LOAD( "2764.l1",      0x4000, 0x2000, CRC(e9a65ba7) SHA1(fbee57d68352fd4062aac55cd1070f001714d0a3) )
	ROM_LOAD( "2764.k1",      0x6000, 0x2000, CRC(b1195d3d) SHA1(095ad2ee1f53be3203830263cb0c9efbe4710c56) )
	ROM_LOAD( "2764.n1",      0x8000, 0x2000, CRC(6a8160d1) SHA1(12101c351bf800319172c459b5e7c69cb5603806) )

	ROM_REGION( 0x10000, "slave", 0 )
	ROM_LOAD( "27128.p7",     0x0000, 0x4000, CRC(8b06d461) SHA1(2434478810c6301197997be76505f5fc6beba5d3) )

	ROM_REGION( 0x10000, "cpu3", 0 )
	ROM_LOAD( "bprom2.bin",   0x0000, 0x0200, CRC(2747ca77) SHA1(abc0ca05925974c4b852827605ee2f1caefb8524) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "27128.a3",     0x0000, 0x4000, CRC(4be96dcf) SHA1(f9b45e6297cbbc4d1ee2df7ac377c5daf5181b0f) )

	ROM_REGION( 0x8000, "gfx2", 0 )
	ROM_LOAD( "sdreturn.m4",  0x0000, 0x2000, CRC(e8779d04) SHA1(f185efbdc7410e3adba85de482e1b06778b17fe9) )
	ROM_LOAD( "sdreturn.l4",  0x2000, 0x2000, CRC(bc217d6c) SHA1(abeaea49aad85f62943d7a3c30c8655fb29d3e62) )
	ROM_LOAD( "sdreturn.j4",  0x4000, 0x2000, CRC(04824549) SHA1(5056db06c06d76580a85fc65d1e473e77eb48ee9) )
	ROM_LOAD( "sdreturn.h4",  0x6000, 0x2000, CRC(c23c36c2) SHA1(22461c04ea2cfd39c395ca7e42a50636fb1c3625) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "dorunrun.clr", 0x0000, 0x0100, CRC(d5bab5d5) SHA1(7a465fe30b6008793d33f6e07086c89111e1e407) )
ROM_END

GAME( 2002, sdreturn, dorunrun, dorunrun, dorunrun, driver_device, 0, ROT0, "Hack", "Sticky Do Returns", MACHINE_SUPPORTS_SAVE )

