#include "../mame/drivers/taito_l.c"

ROM_START( tldemo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "taitoldemo.ic10",   0x00000, 0x56a4, CRC(fda3d418) SHA1(e0171cc1c035d1f800702c3766825b63f6acd291) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "taitoldemo.ic9", 0x00000, 0x10000, CRC(792b8c11) SHA1(0592481b586bfca0f8f8eb096bee46690943596b) )
	ROM_LOAD16_BYTE( "taitoldemo.ic8", 0x00001, 0x10000, CRC(59cfc5ab) SHA1(133084eeacdf978ed13db565f64d155c08ecaade) )
ROM_END

GAME( 2014, tldemo, 0, plotting,  plotting,  driver_device, 0, ROT0,   "cmonkey", "Demo - Taito-L", 0 )
