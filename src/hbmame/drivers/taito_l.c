// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_l.c"

ROM_START( tldemo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tldemo.ic10",   0x00000, 0x56a4, CRC(fda3d418) SHA1(e0171cc1c035d1f800702c3766825b63f6acd291) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "tldemo.ic9", 0x00000, 0x10000, CRC(792b8c11) SHA1(0592481b586bfca0f8f8eb096bee46690943596b) )
	ROM_LOAD16_BYTE( "tldemo.ic8", 0x00001, 0x10000, CRC(59cfc5ab) SHA1(133084eeacdf978ed13db565f64d155c08ecaade) )
ROM_END

ROM_START( tldemo2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tldemo2.ic10",   0x00000, 0x6514, CRC(efd21b6b) SHA1(1c3bfb64dacfb5a1da55201cd4d4202694e06d37) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "tldemo2.ic9", 0x00000, 0x10000, CRC(fcc75ddb) SHA1(e702480b84379e3106cb6d575d237a8f9701f49e) )
	ROM_LOAD16_BYTE( "tldemo2.ic8", 0x00001, 0x10000, CRC(33e70f71) SHA1(5a0caa0912dd7d76dd7e788e21b4cc7f320156ad) )
ROM_END

GAME( 2014, tldemo,  0, plotting,  plotting,  driver_device, 0, ROT0,   "cmonkey", "Demo - System Timed Cheat Tool", 0 )
GAME( 2015, tldemo2, 0, plotting,  plotting,  driver_device, 0, ROT0,   "cmonkey", "Demo - 256 colours", 0 )
