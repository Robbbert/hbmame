// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/vsnes.cpp"

ROM_START( mrio2002 )
	ROM_REGION( 0x10000,"maincpu", 0 ) /* 6502 memory */
	ROM_LOAD( "mds-sm4.1d",  0x8000, 0x2000, CRC(be4d5436) SHA1(08162a7c987f1939d09bebdb676f596c86abf465) )
	ROM_LOAD( "mds-sm4.1c",  0xa000, 0x2000, CRC(0011fc5a) SHA1(5c2c49938a12affc03e64e5bdab307998be20020) )
	ROM_LOAD( "mds-sm4.1b",  0xc000, 0x2000, CRC(b1b87893) SHA1(8563ceaca664cf4495ef1020c07179ca7e4af9f3) )
	ROM_LOAD( "mds-sm4.1a",  0xe000, 0x2000, CRC(1abf053c) SHA1(f17db88ce0c9bf1ed88dc16b9650f11d10835cec) )

	ROM_REGION( 0x4000,"gfx1", 0  ) /* PPU memory */
	ROM_LOAD( "mrio2002.2b", 0x0000, 0x2000, CRC(1feda640) SHA1(f26be31f43dacdaa9d8bffa75f4fcd9d8d04953a) )
	ROM_LOAD( "mds-sm4.2a",  0x2000, 0x2000, CRC(15506b86) SHA1(69ecf7a3cc8bf719c1581ec7c0d68798817d416f) )

	PALETTE_2C04_0004
ROM_END

ROM_START( suprsktr )
	ROM_REGION( 0x10000,"maincpu", 0 ) /* 6502 memory */
	ROM_LOAD( "mds-sm4.1d",  0x8000, 0x2000, CRC(be4d5436) SHA1(08162a7c987f1939d09bebdb676f596c86abf465) )
	ROM_LOAD( "mds-sm4.1c",  0xa000, 0x2000, CRC(0011fc5a) SHA1(5c2c49938a12affc03e64e5bdab307998be20020) )
	ROM_LOAD( "mds-sm4.1b",  0xc000, 0x2000, CRC(b1b87893) SHA1(8563ceaca664cf4495ef1020c07179ca7e4af9f3) )
	ROM_LOAD( "mds-sm4.1a",  0xe000, 0x2000, CRC(1abf053c) SHA1(f17db88ce0c9bf1ed88dc16b9650f11d10835cec) )

	ROM_REGION( 0x4000,"gfx1", 0  ) /* PPU memory */
	ROM_LOAD( "suprsktr.2b", 0x0000, 0x2000, CRC(f3980303) SHA1(b9a25c906d1861c89e2e40e878a34d318daf6619) )
	ROM_LOAD( "mds-sm4.2a",  0x2000, 0x2000, CRC(15506b86) SHA1(69ecf7a3cc8bf719c1581ec7c0d68798817d416f) )

	PALETTE_2C04_0004
ROM_END

GAME( 2002, mrio2002, suprmrio, vsnes, suprmrio, vsnes_state, vsnormal, ROT0, "Nintendo",  "Vs. Super Mario Bros. 2002", MACHINE_SUPPORTS_SAVE )
GAME( 1986, suprsktr, suprmrio, vsnes, suprmrio, vsnes_state, vsnormal, ROT0, "Nintendo",  "Vs. Super Skater Bros.", MACHINE_SUPPORTS_SAVE )
