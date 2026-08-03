// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/atari/mhavoc.cpp"


ROM_START( mhavocrv2 ) // doesn't say return to vax on-screen. Release date unknown.
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "136025.210",   0x0000, 0x2000, CRC(c67284ca) SHA1(d9adad80c266d36429444f483cac4ebcf1fec7b8) )

	ROM_REGION( 0x18000, "alpha", 0 )
	ROM_LOAD( "136025.916a",  0x08000, 0x4000, CRC(8613f09b) SHA1(20befacfce4263a45b0674369d0e7de495fdc4fd) )
	ROM_LOAD( "136025.917",   0x0c000, 0x4000, CRC(21889079) SHA1(d1ad6d9fa1432912e376bca50ceeefac2bfd6ac3) )

	ROM_LOAD( "136025.915a",  0x10000, 0x4000, CRC(39dc3ac1) SHA1(5be399340e6688b070f236768e2122f5f5a596fa) )
	ROM_LOAD( "136025.918a",  0x14000, 0x4000, CRC(d60ff5bd) SHA1(eac5fd92eb9ed7f8efff6821037ef5156e0b765f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "136025.106",   0x0000, 0x4000, CRC(2ca83c76) SHA1(cc1adca32f70af30c4590e9fd6b056b051ccdb38) )
	ROM_LOAD( "136025.907",   0x4000, 0x4000, CRC(4deea2c9) SHA1(c4107581748a3f2d2084de2a4f120abd67a52189) )

	ROM_REGION( 0x4000, "gamma", 0 )
	ROM_LOAD( "136025.908",   0x0000, 0x4000, CRC(c52ec664) SHA1(08120a385f71b17ec02a3c2ef856ff835a91773e) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

GAME( 20??, mhavocrv2, mhavoc, mhavocrv, mhavocrv, mhavocrv_state, empty_init, ROT0, "hack (JMA)", "Major Havoc - Return to Vax 1.8", MACHINE_SUPPORTS_SAVE )

