// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mcr3.c"

ROM_START( sargex )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "cpu_3b.bin",   0x00000, 0x8000, CRC(da31a58f) SHA1(29b97caf61f8f59042519a6b501cd1d15099dd59) )
	ROM_LOAD( "cpu_5b.bin",   0x08000, 0x8000, CRC(6800e746) SHA1(018c2b622b3654530ebc2c299b3f745777163d4b) )
	ROM_FILL(                 0x0e000, 0x2000, 0xff )	/* upper 8k is not mapped on monoboard */

	ROM_REGION( 0x10000, "tcs:cpu", 0 )  /* 64k for the Turbo Cheap Squeak */
	ROM_LOAD( "tcs_u5.bin",   0x0c000, 0x2000, CRC(a894ef8a) SHA1(7f53927fc185fff8ba1b1747f0d565e089d879e6) )
	ROM_LOAD( "tcs_u4.bin",   0x0e000, 0x2000, CRC(6ca6faf3) SHA1(4647e633dd11f55a65c3acf81adeb3af93624991) )

	ROM_REGION( 0x04000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "til_15a.bin",  0x00000, 0x2000, CRC(685001b8) SHA1(300abd808efe5b44b022082eebc591c7c255697c) )
	ROM_LOAD( "til_14b.bin",  0x02000, 0x2000, CRC(8449eb45) SHA1(6cc43639998d55fe7ffac7e9b091d35ea169e048) )

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "sargex.8",     0x00000, 0x8000, CRC(9c0a3000) SHA1(e6f7cc05163a6eba9faa5811ed1fcd261ed400d6) )
	ROM_LOAD( "sargex.6",     0x08000, 0x8000, CRC(da7e3dbf) SHA1(20179d03746098d6a7d3d50fbf4d470bef07467f) )
	ROM_LOAD( "sargex.5",     0x10000, 0x8000, CRC(2effe749) SHA1(5c3f2da367b32c1272c51555f7d94d2dee3bcf93) )
	ROM_LOAD( "sargex.4",     0x18000, 0x8000, CRC(66149219) SHA1(2dcd43de7fa674add94b2584e6795460f486a943) )
ROM_END

GAME( 2009, sargex, sarge, mono_tcs, sarge, mcr3_state, sarge, ROT0, "Gatinho", "Sarge Exposed", MACHINE_SUPPORTS_SAVE )
