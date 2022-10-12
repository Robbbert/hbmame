// license:CC BY-NC-ND 3.0 (copying this code into another project is not permitted)
// copyright-holders:Robbbert
#include "../mame/drivers/gyruss.cpp"

ROM_START( gyrutest ) // Hold 2 hit F3 for more diagnostics
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "gyrussk.1",    0x0000, 0x2000, CRC(c673b43d) SHA1(7c464fb154bac35dd6e2f547e157addeb8798194) )
	ROM_LOAD( "gyrussk.2",    0x2000, 0x2000, CRC(a4ec03e4) SHA1(08c33ad7fcc2ad5e5787a1050284e3f8164f4618) )
	ROM_LOAD( "gyrussk.3",    0x4000, 0x2000, CRC(27454a98) SHA1(030c7df225652ee20d5ef64d005eb011dc89a27d) )
	ROM_LOAD( "gyrutest.14j", 0x6000, 0x2000, CRC(7f28f9e4) SHA1(10f2de94f17ace513410cb358f67632b5b201896) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gyrussk.9",    0xe000, 0x2000, CRC(822bf27e) SHA1(36d5bea2392a7d3476dd797dc05602705cfa23ef) )

	ROM_REGION( 0x6000, "audiocpu", 0 )
	ROM_LOAD( "gyrussk.1a",   0x0000, 0x2000, CRC(f4ae1c17) SHA1(ae568c96a31d910afe30d2b7eeb9ed1ed07290e3) )
	ROM_LOAD( "gyrussk.2a",   0x2000, 0x2000, CRC(ba498115) SHA1(9cd1f42898cc590f39ba7cb3c975b0b3d3062eba) )

	ROM_REGION( 0x1000, "audio2", 0 )
	ROM_LOAD( "gyrussk.3a",   0x0000, 0x1000, CRC(3f9b5dea) SHA1(6e807da02c2885b18e8cc2199f12f6be9040bf75) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "gyrussk.6",    0x0000, 0x2000, CRC(c949db10) SHA1(fcb8bcbd2bdd751fecb322a33c8a92fb6f07a7ab) )
	ROM_LOAD( "gyrussk.5",    0x2000, 0x2000, CRC(4f22411a) SHA1(763bcd039f8c1838a0d7da7d4dadc14a26e25596) )
	ROM_LOAD( "gyrussk.8",    0x4000, 0x2000, CRC(47cd1fbc) SHA1(8203c4ff0b1cd7b4dbc708e300bfeac1e7366e09) )
	ROM_LOAD( "gyrussk.7",    0x6000, 0x2000, CRC(8e8d388c) SHA1(8f2928d71c02aba977d67575d6e34d69bda2b9d4) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gyrussk.4",    0x0000, 0x2000, CRC(27d8329b) SHA1(564ff945465a23d93a93137ad277298770dfa06a) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "gyrussk.pr3",  0x0000, 0x0020, CRC(98782db3) SHA1(b891e43b25187faca8002919ccb44d744daa3594) )
	ROM_LOAD( "gyrussk.pr1",  0x0020, 0x0100, CRC(7ed057de) SHA1(c04069ae1e2c62f9b3048844cd8cf5e1b03b7d3c) )
	ROM_LOAD( "gyrussk.pr2",  0x0120, 0x0100, CRC(de823a81) SHA1(1af94b2a6a319a89b238a5076a2867f1cfd279b0) )
ROM_END

ROM_START( gyrutest2 ) // Hold 2 hit F3 for more diagnostics
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "gyrutest2.11j",  0x0000, 0x2000, CRC(68a8fa9b) SHA1(7f19e95d173779a918c7af985c9a69d31afde0d2) )
	ROM_LOAD( "gyrussk.2",    0x2000, 0x2000, CRC(a4ec03e4) SHA1(08c33ad7fcc2ad5e5787a1050284e3f8164f4618) )
	ROM_LOAD( "gyrussk.3",    0x4000, 0x2000, CRC(27454a98) SHA1(030c7df225652ee20d5ef64d005eb011dc89a27d) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gyrussk.9",    0xe000, 0x2000, CRC(822bf27e) SHA1(36d5bea2392a7d3476dd797dc05602705cfa23ef) )

	ROM_REGION( 0x6000, "audiocpu", 0 )
	ROM_LOAD( "gyrussk.1a",   0x0000, 0x2000, CRC(f4ae1c17) SHA1(ae568c96a31d910afe30d2b7eeb9ed1ed07290e3) )
	ROM_LOAD( "gyrussk.2a",   0x2000, 0x2000, CRC(ba498115) SHA1(9cd1f42898cc590f39ba7cb3c975b0b3d3062eba) )

	ROM_REGION( 0x1000, "audio2", 0 )
	ROM_LOAD( "gyrussk.3a",   0x0000, 0x1000, CRC(3f9b5dea) SHA1(6e807da02c2885b18e8cc2199f12f6be9040bf75) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "gyrussk.6",    0x0000, 0x2000, CRC(c949db10) SHA1(fcb8bcbd2bdd751fecb322a33c8a92fb6f07a7ab) )
	ROM_LOAD( "gyrussk.5",    0x2000, 0x2000, CRC(4f22411a) SHA1(763bcd039f8c1838a0d7da7d4dadc14a26e25596) )
	ROM_LOAD( "gyrussk.8",    0x4000, 0x2000, CRC(47cd1fbc) SHA1(8203c4ff0b1cd7b4dbc708e300bfeac1e7366e09) )
	ROM_LOAD( "gyrussk.7",    0x6000, 0x2000, CRC(8e8d388c) SHA1(8f2928d71c02aba977d67575d6e34d69bda2b9d4) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gyrussk.4",    0x0000, 0x2000, CRC(27d8329b) SHA1(564ff945465a23d93a93137ad277298770dfa06a) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "gyrussk.pr3",  0x0000, 0x0020, CRC(98782db3) SHA1(b891e43b25187faca8002919ccb44d744daa3594) )
	ROM_LOAD( "gyrussk.pr1",  0x0020, 0x0100, CRC(7ed057de) SHA1(c04069ae1e2c62f9b3048844cd8cf5e1b03b7d3c) )
	ROM_LOAD( "gyrussk.pr2",  0x0120, 0x0100, CRC(de823a81) SHA1(1af94b2a6a319a89b238a5076a2867f1cfd279b0) )
ROM_END

GAME( 2021, gyrutest,  gyruss, gyruss, gyruss, gyruss_state, empty_init, ROT90, "Shoestring", "Gyruss Test Rom (2021-06-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, gyrutest2, gyruss, gyruss, gyruss, gyruss_state, empty_init, ROT90, "Shoestring", "Gyruss Test Rom (2021-09-29)", MACHINE_SUPPORTS_SAVE )

