// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/bloodbro.cpp"

/* ROMs */

ROM_START( bloodbros01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "2s01.u021.7n",  0x00001, 0x20000, CRC(e5724503) SHA1(eca13cd75f776cd82bd16574ff44a1a5462311ab) )
	ROM_LOAD16_BYTE( "1s01.u022.8n",  0x00000, 0x20000, CRC(8855bc32) SHA1(d322c339f284bd53d6753f816c3f5054be12a497) )
	ROM_LOAD16_BYTE( "4.u023.7l",  0x40001, 0x20000, CRC(fd951c2c) SHA1(f4031bf303c67c82f2f78f7456f78382d8c1ac85) )
	ROM_LOAD16_BYTE( "3.u024.8l",  0x40000, 0x20000, CRC(18d3c460) SHA1(93b86af1199f0fedeaf1fe64d27ffede4b819e42) )

	ROM_REGION( 0x20000, "audiocpu", 0 )
	ROM_LOAD( "bb_07.u1016.6a",   0x000000, 0x08000, CRC(411b94e8) SHA1(6968441f64212c0935afeca68f07deaadf86d614) )
	ROM_CONTINUE(            0x010000, 0x08000 )
	ROM_COPY( "audiocpu", 0x000000, 0x018000, 0x08000 )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "bb_05.u061.6f", 0x00000, 0x10000, CRC(04ba6d19) SHA1(7333075c3323756d51917418b5234d785a9bee00) )
	ROM_LOAD( "bb_06.u063.6d", 0x10000, 0x10000, CRC(7092e35b) SHA1(659d30b2e2fd9ffa34a47e98193c8f0a87ac1315) )

	ROM_REGION( 0x100000, "gfx2", 0 )
	ROM_LOAD( "blood_bros_bk__=c=1990_tad_corp.u064.4d", 0x00000, 0x100000, CRC(1aa87ee6) SHA1(e7843c1e8a0f3a685f0b5d6e3a2eb3176c410847) )  /* Background+Foreground */

	ROM_REGION( 0x100000, "gfx3", 0 )
	ROM_LOAD( "blood_bros_obj__=c=1990_tad_corp.u078.2n", 0x00000, 0x100000, CRC(d27c3952) SHA1(de7306432b682f238b911507ad7aa2fa8acbee80) ) /* sprites */

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "bb_08.u095.5a",  0x00000, 0x20000, CRC(deb1b975) SHA1(08f2e9a0a23171201b71d381d091edcd3787c287) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "cb006.u083.6c", 0x0000, 0x0100, CRC(b2b89a74) SHA1(1878823801048d677aef9702feedd5bf775e62d0) )
ROM_END

GAME( 2009, bloodbros01,  bloodbro, bloodbro, bloodbro, bloodbro_state, empty_init, ROT0, "pipi899", "Blood Bros (Ex Super Version 2009-03-09)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
