// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/hh_sm510.cpp"

ROM_START( gnw_bride )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "bride.bin", 0x0000, 0x1000, CRC(ffaf3465) SHA1(0a37d19ed61b3715cf6a2b99968eb3c25a6f0f9f) )

	ROM_REGION( 281161, "screen", 0)
	ROM_LOAD( "gnw_bride.svg", 0x00000, 0x2faa7, CRC(186e4806) SHA1(b29928d0dcc167e24c278c4930bc9d5a09649d53) )
ROM_END

CONS( 2018, gnw_bride,   0, 0, gnw_dkjr,    gnw_dkjr,    gnw_dkjr_state,    empty_init, "Montrose", "Game & Watch: Bride", MACHINE_SUPPORTS_SAVE )


ROM_START( gnw_squeeze )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "squeeze.bin", 0x0000, 0x1000, CRC(5af50d77) SHA1(d307db5d5b8ab48251d122a7494bd06ea9bebd59) )

	ROM_REGION( 126434, "screen_top", 0)
	ROM_LOAD( "gnw_squeeze_top.svg", 0x0000, 0x41f5, CRC(b23dfd9a) SHA1(202b8112b1c15adcddadb50ea78aa99a8bee214b) )

	ROM_REGION( 122870, "screen_bottom", 0)
	ROM_LOAD( "gnw_squeeze_bottom.svg", 0x0000, 0x27f8, CRC(32bdfedd) SHA1(5cd423e1481165460a7a1b59ed2d8abb2587d3df) )
ROM_END

CONS( 2018, gnw_squeeze, 0, 0, gnw_mickdon, gnw_mickdon, gnw_mickdon_state, empty_init, "Montrose", "Game & Watch: Squeeze", MACHINE_NOT_WORKING | MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )

//  http://montrose.is/games/


ROM_START( spicegirls )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "n63.program", 0x0000, 0x1000, CRC(303aa6f7) SHA1(296689be1ee05238e52e9882812868b2ea96202c) )

	ROM_REGION( 0x100, "maincpu:melody", 0 )
	ROM_LOAD( "n63.melody", 0x000, 0x100, CRC(77c1a5a3) SHA1(c00ae3b7c64dd9db96eab520fe674a40571fc15f) )

	ROM_REGION( 0x12effe, "screen", 0)
	ROM_LOAD( "tmchammer1.svg", 0, 0x12effe, CRC(2780e107) SHA1(ddada21ef480bccee7c4c1a54b3842e03be85921) )
ROM_END

CONS( 2019, spicegirls, 0, 0, tmchammer, tmchammer, tmchammer_state, empty_init, "Pagent Tech", "Spice Girls", MACHINE_SUPPORTS_SAVE )

