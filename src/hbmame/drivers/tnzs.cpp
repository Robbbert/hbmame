// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/tnzs.cpp"

/************************
 Dr. Toppel's Adventure
*************************/

ROM_START( drtoppel01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "b19__09__s01.11c", 0x00000, 0x10000, CRC(6364a970) SHA1(e541b87afd91be7eb0cd15cbc5ee80feff82e12e) )
	ROM_LOAD( "b19__10.9c",  0x10000, 0x10000, CRC(7e72fd25) SHA1(6035e4db75e6dc57b13bb6e92217d1c2d0ffdfd2) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "b19__15.3e", 0x00000, 0x10000, BAD_DUMP CRC(37a0d3fb) SHA1(f65fb9382af5f5b09725c39b660c5138b3912f53) )

	ROM_REGION( 0x10000, "mcu", 0 )
	ROM_LOAD( "b06__14.1g", 0x0000, 0x0800, CRC(28907072) SHA1(21c7017af8a8ceb8e43d7e798f48518b136fd45c) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "b19-01.23c1000.13a", 0x00000, 0x20000, CRC(a7e8a0c1) SHA1(a2f017ae5b6472d4202f126d0247b3fe4b1321d1) )
	ROM_LOAD( "b19-02.23c1000.12a", 0x20000, 0x20000, CRC(790ae654) SHA1(5fd6b89918e1539e00c918959b96d2a9394c8abe) )
	ROM_LOAD( "b19-03.23c1000.10a", 0x40000, 0x20000, CRC(495c4c5a) SHA1(a23b512cda4c0c535df5508a52faebe401c1797d) )
	ROM_LOAD( "b19-04.23c1000.8a",  0x60000, 0x20000, CRC(647007a0) SHA1(10ec35a15091967038bb26fb116c47d730f69edc) )
	ROM_LOAD( "b19-05.23c1000.7a",  0x80000, 0x20000, CRC(49f2b1a5) SHA1(5e98bb421afaa02471ad02213ea6ca23ff2f0e27) )
	ROM_LOAD( "b19-06.23c1000.5a",  0xa0000, 0x20000, CRC(2d39f1d0) SHA1(2aa89a5cc7f026c8db9922b183319ff66ac4a071) )
	ROM_LOAD( "b19-07.23c1000.4a",  0xc0000, 0x20000, CRC(8bb06f41) SHA1(a0c182d473317f2cdb31bdf39a2593c032002305) )
	ROM_LOAD( "b19-08.23c1000.2a",  0xe0000, 0x20000, CRC(3584b491) SHA1(d0aca90708be241bbd3a1097220a85083337a4bc) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "b19-13.am27s29.15f", 0x0000, 0x200, CRC(6a547980) SHA1(c82f8dfad028565b4b4e5be1167f2f290c929090) )
	ROM_LOAD( "b19-12.am27s29.16f", 0x0200, 0x200, CRC(5754e9d8) SHA1(8c7d29e22c90b1f72929b95675dc15e431aae044) )
ROM_END

/**********
 Insector X
************/

ROM_START( insectx01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "b97__03_s01.u32", 0x00000, 0x20000, CRC(2f478739) SHA1(b073170bbf14a974d7ab28d8c078c4e8823f259f) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "b97__07.u38", 0x00000, 0x10000, CRC(324b28c9) SHA1(db77a4ac60196d0f0f35dbc5c951ec29d6392463) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "b97__01_ps01.u1", 0x00000, 0x80000, CRC(4b6ec740) SHA1(f6065bdc73e9997e24e19a664eb161cad6948c95) )
	ROM_LOAD( "b97__02_ps01.u2", 0x80000, 0x80000, CRC(78e9ecc4) SHA1(a6921d434d0c3dc6ac6ee4aad7ac8b1d901d0daf) )
ROM_END

/*    YEAR  NAME          PARENT           MACHINE   INPUT     INIT                 MONITOR COMPANY       FULLNAME FLAGS */
// Dr. Toppel's Adventure
GAME( 1987, drtoppel01,  drtoppel,   extrmatn, drtoppel, extrmatn_state, empty_init, ROT90,  "Taito Corporation Japan",   "Dr. Toppel's Adventure (World, alt?)", MACHINE_SUPPORTS_SAVE ) /* Possible region hack */
// Insector X
GAME( 1989, insectx01,   insectx,    insectx,  insectx,  insectx_state,  empty_init, ROT0,   "hack", "Insector X (Chinese)", MACHINE_SUPPORTS_SAVE )
