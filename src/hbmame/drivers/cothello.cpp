// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/nintendo/cothello.cpp"

ROM_START( cothello01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "13s01.ic13", 0x0000, 0x0400, CRC(a9d35efb) SHA1(392c67e84c8dc911f83cd4301de35e84f8c0e2c7) )
	ROM_LOAD( "12s01.ic12", 0x0400, 0x0400, CRC(8cad1572) SHA1(c81dc0dff9d7695c4eacc65e430e9c27f31d7541) )
	ROM_LOAD( "11s01.ic11", 0x0800, 0x0400, CRC(ee58ea9e) SHA1(f705f333ef51ae72c35a6e17ae58386e01d04b7f) )
ROM_END

GAMEL(2026, cothello01, 0, cothello, cothello, cothello_state, empty_init, ROT0, "Michael Rhodes", "Computer Othello (English)", MACHINE_SUPPORTS_SAVE, layout_cothello )

