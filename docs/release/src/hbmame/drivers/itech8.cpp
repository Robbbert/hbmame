// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/itech8.cpp"

ROM_START( itecdemo )
	ROM_REGION( 0x1c000, "maincpu", 0 )
	ROM_LOAD( "itecdemo.u5", 0x04000, 0x4000, CRC(749a1336) SHA1(f3f1220fbb9b61214e7b4e675a6ab5bbe9db50af) )
	//ROM_LOAD( "itecdemo.u5", 0x04000, 0x4000, CRC(d96cbf81) SHA1(c2631e6beaa478def680d08302c65dbb71730a31) ) // wrong colours
	ROM_CONTINUE(        0x10000, 0xc000 )
	ROM_COPY( "maincpu", 0x14000, 0x8000, 0x8000 )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "itecdemo.u27", 0x08000, 0x8000, CRC(f0cd7332) SHA1(770b5d9b531495df0ff6350942cbd5147a875e9b) )

	ROM_REGION( 0xc0000, "grom", 0 )
	ROM_LOAD( "grom0d.bin", 0x00000, 0x20000, CRC(e4f9fe36) SHA1(ad8cb4eb1d5d685bb22ca6fee5f1efcd682f8b38) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD_OPTIONAL( "srom0d.bin", 0x00000, 0x20000, CRC(7ee8cdcd) SHA1(67dfd19f3eb3649d6f3f6631e44d0bd36b8d8d19) )

	ROM_REGION( 0x0300, "plds", 0 )
	ROM_LOAD_OPTIONAL( "chaos.pal", 0x0000, 0x0300, CRC(2110ab0c) SHA1(f9db81bd6fdbaecca11ef2488fca885a00f8f2a2) )
ROM_END


GAME( 1989, itecdemo, 0, stratab_hi, gtg, itech8_state, empty_init, ROT0, "Charles Doty", "Demo - Itec", 0 )
