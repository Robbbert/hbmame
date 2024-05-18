// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/scramble.cpp"

ROM_START( troopyx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ic2cb.bin",   0x0000, 0x1000, CRC(a1798961) SHA1(45a1f6183016229fced3b459c95c99d83408151a) )
	ROM_LOAD( "ic2eb.bin",   0x1000, 0x1000, CRC(7f3572f9) SHA1(2f89f743a32378ed4ac4184627ed9be007c3334a) )
	ROM_LOAD( "ic2fb.bin",   0x2000, 0x1000, CRC(42e666fd) SHA1(caa6a2b07098ef1d6203309ddd3a591194b4ac70) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "ic5c.bin", 0x0000, 0x1000, CRC(af42a371) SHA1(edacbb29df34fdf400a5c726d851af1479a34c70) )
	ROM_LOAD( "ic5d.bin", 0x1000, 0x1000, CRC(862b8902) SHA1(91dcbc634f7c7ed78dfbd0be5cf1e0631429cfbf) )
	ROM_LOAD( "ic5e.bin", 0x2000, 0x1000, CRC(a0396cc8) SHA1(c8266b58b144a4bc564f3a2503d5b953c0ba6ca7) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "troopyx.ic5h",      0x0000, 0x0800, CRC(9a40b385) SHA1(4a3b3cafe5c064f3b485a769c045eba5fc7c12ce) )
	ROM_LOAD( "ic5f_neu.bin",      0x0800, 0x0800, CRC(cbbfefc2) SHA1(2378949275b8d3fc69551b00d9b2c654b91fd780) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "82s123", 0x0000, 0x0020, CRC(4e3caeab) SHA1(a25083c3e36d28afdefe4af6e6d4f3155e303625) )
ROM_END

GAME( 2020, troopyx, mrkougar, mrkougb,  mrkougar, scramble_state, init_mrkougar, ROT90, "chaneman", "Troopy (fixed graphics)", MACHINE_SUPPORTS_SAVE )

