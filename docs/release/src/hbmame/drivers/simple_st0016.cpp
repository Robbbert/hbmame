// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/simple_st0016.cpp"

ROM_START( renjuk )
	ROM_REGION( 0x290000, "maincpu", 0 )
	ROM_LOAD( "renjyu-1.u31k",0x000000, 0x200000, CRC(2bd052c5) SHA1(724662d6d2ead1ef9dce1fa95d7c15d35fcb5a51) )
	ROM_LOAD( "rnj2.u32",     0x200000, 0x080000, CRC(2015289c) SHA1(5223b6d3dbe4657cd63cf5b527eaab84cf23587a) )
ROM_END

GAME(  1994, renjuk, 0, renju, renju, st0016_state, renju, ROT0, "Visco", "Renju Kizoku (Korean)", 0 )
