// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/snk6502.cpp"

ROM_START( vantris )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "vantris.ic13", 0x8000, 0x1000, CRC(f9f367a5) SHA1(49249a73951ae9a8a965954d8b03627b5fcf4fe3) )
	ROM_RELOAD(               0xf000, 0x1000 ) // for the reset and interrupt vectors

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vantris.ic50", 0x0000, 0x0800, CRC(a7061526) SHA1(f5f5860a9dc468cb4177348e0b0592e5adda28f1) )
	ROM_LOAD( "vantris.ic51", 0x0800, 0x0800, CRC(67bd31ed) SHA1(24f03cf097e5382500a85483351f21913a98fdd5) )

	ROM_REGION( 0x0040, "proms", 0 )
	ROM_LOAD( "vantris.ic7",  0x0000, 0x0020, CRC(90f17fc1) SHA1(fc92864178cbceccd6f7f2c75249b5c568bdbea8) )
	ROM_RELOAD(               0x0020, 0x0020 )

	ROM_REGION( 0x1000, "snk6502", 0 )
	ROM_LOAD( "vantris.ic51", 0x0000, 0x0800, CRC(fd4d8bb0) SHA1(d59ec7569147263aed50041b15aed2ca4f395c23) )
	ROM_LOAD( "sk4_ic52.bin", 0x0800, 0x0800, CRC(cc4a0b6f) SHA1(251b24d60083d516c4ba686d75b41e04d10f7198) )

	ROM_REGION( 0x5800, "speech", ROMREGION_ERASE00 )
ROM_END

GAME( 1998, vantris, 0, vanguard, vanguard, driver_device, 0, ROT90, "Norbert Kehrer", "Vantris 98", MACHINE_SUPPORTS_SAVE ) // http://web.utanet.at/nkehrer/vantris.html
