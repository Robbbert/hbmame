// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/snk6502.cpp"

ROM_START( vantris )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "sk4_ic07.bin", 0x4000, 0x1000, CRC(6a29e354) SHA1(ff953962ebc14a28cfc96f8e269cb1e1c188ed8a) )
	ROM_LOAD( "sk4_ic08.bin", 0x5000, 0x1000, CRC(302bba54) SHA1(1944f229481328a0635fafda65054106f42a532a) )
	ROM_LOAD( "sk4_ic09.bin", 0x6000, 0x1000, CRC(424755f6) SHA1(b4762b40c7ed70d4b90319a1a30983a41a096afb) )
	ROM_LOAD( "sk4_ic10.bin", 0x7000, 0x1000, CRC(54603274) SHA1(31571a560dbe300417b3ed5b114fa1d9ef742da9) )
	ROM_LOAD( "vtr4_ic13.bin", 0x8000, 0x1000, CRC(f9f367a5 ))
	ROM_RELOAD(               0xf000, 0x1000 )	/* for the reset and interrupt vectors */
	ROM_LOAD( "sk4_ic14.bin", 0x9000, 0x1000, CRC(0d5b47d0) SHA1(922621c23f33fe756cb6baa12e5465c4e64f2dda) )
	ROM_LOAD( "sk4_ic15.bin", 0xa000, 0x1000, CRC(8549b8f8) SHA1(375bc6f7e15564d5cf7e00c44e2651793c56d6ca) )
	ROM_LOAD( "sk4_ic16.bin", 0xb000, 0x1000, CRC(062e0be2) SHA1(45aaf315a62f37460e32d3ba99caaacf4c994810) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vtr5_ic50.bin", 0x0000, 0x0800, CRC(a7061526) SHA1(f5f5860a9dc468cb4177348e0b0592e5adda28f1) )
	ROM_LOAD( "vtr5_ic51.bin", 0x0800, 0x0800, CRC(67bd31ed) SHA1(24f03cf097e5382500a85483351f21913a98fdd5) )

	ROM_REGION( 0x0040, "proms", 0 )
	ROM_LOAD( "vtr5_ic7.bin", 0x0000, 0x0020, CRC(90f17fc1) SHA1(fc92864178cbceccd6f7f2c75249b5c568bdbea8) ) /* foreground colors */
	ROM_RELOAD(               0x0020, 0x0020 ) /* background colors */

	ROM_REGION( 0x1000, "snk6502", 0 )	/* sound ROMs */
	ROM_LOAD( "vtr4_ic51.bin", 0x0000, 0x0800, CRC(fd4d8bb0) SHA1(d59ec7569147263aed50041b15aed2ca4f395c23) ) /* sound ROM 1 */
	ROM_LOAD( "vtr4_ic52.bin", 0x0800, 0x0800, CRC(cc4a0b6f) SHA1(251b24d60083d516c4ba686d75b41e04d10f7198) ) /* sound ROM 2 */

	ROM_REGION( 0x5800, "speech", 0 )	/* space for the speech ROMs (not supported) */
	//ROM_LOAD( "hd38882.bin",  0x0000, 0x4000, NO_DUMP )	/* HD38882 internal ROM */
	ROM_LOAD_OPTIONAL( "sk6_ic07.bin", 0x4000, 0x0800, CRC(2b7cbae9) SHA1(3d44a0232d7c94d8170cc06e90cc30bd57c99202) )
	ROM_LOAD_OPTIONAL( "sk6_ic08.bin", 0x4800, 0x0800, CRC(3b7e9d7c) SHA1(d9033188068b2aaa1502c89cf09f955eded8fa7a) )
	ROM_LOAD_OPTIONAL( "sk6_ic11.bin", 0x5000, 0x0800, CRC(c36df041) SHA1(8b51934229b961180d1edb99be3a4d337d37f66f) )
ROM_END

GAME( 1998, vantris, 0, vanguard, vanguard, driver_device, 0, ROT90, "Norbert Kehrer", "Vantris 98", MACHINE_SUPPORTS_SAVE ) // http://web.utanet.at/nkehrer/vantris.html
