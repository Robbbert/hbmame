// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/missile.cpp"


ROM_START( missilet ) // only change to 'missile' is the initials in the high score screen
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "035820-02.h1",  0x5000, 0x0800, CRC(7a62ce6a) SHA1(9a39978138dc28fdefe193bfae1b226391e471db) )
	ROM_LOAD( "035821-02.jk1", 0x5800, 0x0800, CRC(df3bd57f) SHA1(0916925d3c94d766d33f0e4badf6b0add835d748) )
	ROM_LOAD( "035822-03e.kl1",0x6000, 0x0800, CRC(1a2f599a) SHA1(2deb1219223032a9c83114e4e8b2fc11a570754c) )
	ROM_LOAD( "035823-02.ln1", 0x6800, 0x0800, CRC(82e552bb) SHA1(d0f22894f779c74ceef644c9f03d840d9545efea) )
	ROM_LOAD( "035824-02.np1.t", 0x7000, 0x0800, CRC(5019acc9) SHA1(3834859ff9d236616ee54f83e17956ad24ebe06e) )
	ROM_LOAD( "035825-02.r1",  0x7800, 0x0800, CRC(f752eaeb) SHA1(0339a6ce6744d2091cc7e07675e509b202b0f380) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "035826-01.l6",  0x0000, 0x0020, CRC(86a22140) SHA1(2beebf7855e29849ada1823eae031fc98220bc43) )
ROM_END


GAME( 2018, missilet, missile, missile, missile,  missile_state, empty_init, ROT0, "Atari", "Missile Command (Tesla variant)", MACHINE_SUPPORTS_SAVE )

