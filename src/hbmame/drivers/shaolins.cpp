// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/shaolins.cpp"

ROM_START( kfjohnny ) // same as kicker except for gfx roms
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "477-l03.d9",   0x6000, 0x2000, CRC(2598dfdd) SHA1(70a9d81b73bbd4ff6b627a3e4102d5328a946d20) )
	ROM_LOAD( "477-l04.d10",  0x8000, 0x4000, CRC(0cf0351a) SHA1(a9da783b29a63a46912a29715e8d11dc4cd22265) )
	ROM_LOAD( "477-l05.d11",  0xC000, 0x4000, CRC(654037f8) SHA1(52d098386fe87ae97d4dfefab0bd3a902f66d70b) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "kfjohnny.a10", 0x0000, 0x2000, CRC(be0310d0) SHA1(e615aec557c656121761cf77d9ec4a6d1a6ddbea) )
	ROM_LOAD( "kfjohnny.a11", 0x2000, 0x2000, CRC(fab8c880) SHA1(b90ae46ee72a000983ec68884f4ea0e406030ba3) )

	ROM_REGION( 0x8000, "gfx2", 0 )
	ROM_LOAD( "kfjohnny.h15", 0x0000, 0x4000, CRC(f7fbbb9c) SHA1(4b85c1b50a6232be2f6e7b2e497dca86dda387d8) )
	ROM_LOAD( "kfjohnny.h14", 0x4000, 0x4000, CRC(84bc0cfd) SHA1(2140fe066262a5bdeb1e89d2d6cfa737f88de25d) )

	ROM_REGION( 0x0500, "proms", 0 )
	ROM_LOAD( "477j10.a12",   0x0000, 0x0100, CRC(b09db4b4) SHA1(d21176cdc7def760da109083eb52e5b6a515021f) )
	ROM_LOAD( "477j11.a13",   0x0100, 0x0100, CRC(270a2bf3) SHA1(c0aec04bd3bceccddf5f5a814a560a893b29ef6b) )
	ROM_LOAD( "477j12.a14",   0x0200, 0x0100, CRC(83e95ea8) SHA1(e0bfa20600488f5c66233e13ea6ad857f62acb7c) )
	ROM_LOAD( "477j09.b8",    0x0300, 0x0100, CRC(aa900724) SHA1(c5343273d0a7101b8ba6876c4f22e43d77610c75) )
	ROM_LOAD( "477j08.f16",   0x0400, 0x0100, CRC(80009cf5) SHA1(a367f3f55d75a9d5bf4d43f9d77272eb910a1344) )
ROM_END

GAME( 2002, kfjohnny, kicker, shaolins, shaolins, driver_device, 0, ROT90, "Smitdogg", "Kung-Fu Johnny", MACHINE_SUPPORTS_SAVE )
