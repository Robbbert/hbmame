// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/hh_sm510.cpp"

ROM_START( gnw_bride )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "bride.bin", 0x0000, 0x1000, CRC(ffaf3465) SHA1(0a37d19ed61b3715cf6a2b99968eb3c25a6f0f9f) )

	ROM_REGION( 281161, "svg", 0)
	ROM_LOAD( "gnw_bride.svg", 0x00000, 0x2faa7, CRC(186e4806) SHA1(b29928d0dcc167e24c278c4930bc9d5a09649d53) )
ROM_END


ROM_START( gnw_squeeze )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "squeeze.bin", 0x0000, 0x1000, CRC(5af50d77) SHA1(d307db5d5b8ab48251d122a7494bd06ea9bebd59) )

	ROM_REGION( 126434, "svg_top", 0)
	ROM_LOAD( "gnw_squeeze_top.svg", 0x0000, 0x41f5, CRC(b23dfd9a) SHA1(202b8112b1c15adcddadb50ea78aa99a8bee214b) )

	ROM_REGION( 122870, "svg_bottom", 0)
	ROM_LOAD( "gnw_squeeze_bottom.svg", 0x0000, 0x27f8, CRC(32bdfedd) SHA1(5cd423e1481165460a7a1b59ed2d8abb2587d3df) )
ROM_END


CONS( 2018, gnw_bride,   0, 0, gnw_dkjr,    gnw_dkjr,    gnw_dkjr_state,    empty_init, "Montrose", "Game & Watch: Bride", MACHINE_SUPPORTS_SAVE )
CONS( 2018, gnw_squeeze, 0, 0, gnw_mickdon, gnw_mickdon, gnw_mickdon_state, empty_init, "Montrose", "Game & Watch: Squeeze", MACHINE_SUPPORTS_SAVE )

//  http://montrose.is/games/


