// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/segac2.cpp"

/* http://frogfeast.rastersoft.net/SegaC2.html
	It appears from the source that the sound is going through a Z80 - but the C2 hardware doesn't have one...
	Also there are unmapped writes to A00000-A00100, A11100, A11200. */

ROM_START( c2frog )
	ROM_REGION( 0x1000000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "epr13114.32", 0x000000, 0x020000, CRC(d7039bd3) SHA1(82b9e3a760ebc845afe7bd78f22b897e1a530651) )
	ROM_LOAD16_BYTE( "epr13113.31", 0x000001, 0x020000, CRC(754b111a) SHA1(5542fc3b955df843e15e7c98aebc4f80c7d44b69) )
ROM_END

GAME( 2007, c2frog, 0, segac, columns, segac2_state, columns, ROT0, "Rastersoft", "Frog Feast (Sega C2 hardware)", MACHINE_NO_SOUND | MACHINE_SUPPORTS_SAVE )
