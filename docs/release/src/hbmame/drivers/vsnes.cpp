// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/vsnes.cpp"

ROM_START( mrio2002 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mds-sm4-4__1dor6d_e.1d or 6d",  0x8000, 0x2000, CRC(be4d5436) SHA1(08162a7c987f1939d09bebdb676f596c86abf465) )
	ROM_LOAD( "mds-sm4.1c",  0xa000, 0x2000, CRC(0011fc5a) SHA1(5c2c49938a12affc03e64e5bdab307998be20020) )
	ROM_LOAD( "mds-sm4-4__1bor6b_e.1b or 6b",  0xc000, 0x2000, CRC(b1b87893) SHA1(8563ceaca664cf4495ef1020c07179ca7e4af9f3) )
	ROM_LOAD( "mds-sm4-4__1aor6a_e.1a or 6a",  0xe000, 0x2000, CRC(1abf053c) SHA1(f17db88ce0c9bf1ed88dc16b9650f11d10835cec) )

	ROM_REGION( 0x4000, "gfx1", 0  )
	ROM_LOAD( "mrio2002.2b", 0x0000, 0x2000, CRC(1feda640) SHA1(f26be31f43dacdaa9d8bffa75f4fcd9d8d04953a) )
	ROM_LOAD( "mds-sm4-4__2aor8a_e.2a or 8a",  0x2000, 0x2000, CRC(15506b86) SHA1(69ecf7a3cc8bf719c1581ec7c0d68798817d416f) )

	PALETTE_2C04_0004("ppu1:palette")
ROM_END

ROM_START( suprsktr )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mds-sm4-4__1dor6d_e.1d or 6d",  0x8000, 0x2000, CRC(be4d5436) SHA1(08162a7c987f1939d09bebdb676f596c86abf465) )
	ROM_LOAD( "mds-sm4.1c",  0xa000, 0x2000, CRC(0011fc5a) SHA1(5c2c49938a12affc03e64e5bdab307998be20020) )
	ROM_LOAD( "mds-sm4-4__1bor6b_e.1b or 6b",  0xc000, 0x2000, CRC(b1b87893) SHA1(8563ceaca664cf4495ef1020c07179ca7e4af9f3) )
	ROM_LOAD( "mds-sm4-4__1aor6a_e.1a or 6a",  0xe000, 0x2000, CRC(1abf053c) SHA1(f17db88ce0c9bf1ed88dc16b9650f11d10835cec) )

	ROM_REGION( 0x4000, "gfx1", 0  )
	ROM_LOAD( "suprsktr.2b", 0x0000, 0x2000, CRC(f3980303) SHA1(b9a25c906d1861c89e2e40e878a34d318daf6619) )
	ROM_LOAD( "mds-sm4-4__2aor8a_e.2a or 8a",  0x2000, 0x2000, CRC(15506b86) SHA1(69ecf7a3cc8bf719c1581ec7c0d68798817d416f) )

	PALETTE_2C04_0004("ppu1:palette")
ROM_END

GAME( 2002, mrio2002, suprmrio, vsnes, suprmrio, vsnes_state, init_vsnormal, ROT0, "Nintendo",  "Vs. Super Mario Bros. 2002", 0 )
GAME( 1986, suprsktr, suprmrio, vsnes, suprmrio, vsnes_state, init_vsnormal, ROT0, "Nintendo",  "Vs. Super Skater Bros.", 0 )


// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*************
 Vs. Dr. Mario
****************/

ROM_START( drmarios01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "dmhc01-uiprg",  0x10000, 0x10000, CRC(a0c56a2a) SHA1(0aeb5ff1f8c6308f723e88003ea6282914d22121) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "dmhc01-u3chr",  0x0000, 0x8000, CRC(ac94c651) SHA1(9ac7c97501d915c6f0041de3be421423f5de0448) )

	PALETTE_2C04_0003("ppu1:palette")
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Vs. Dr. Mario
GAME( 2002, drmarios01,  drmario,         vsnes,         drmario,  vsnes_state, init_drmario,  ROT0, "liujunusa", "Vs. Dr. Mario (Translation Chinese 2002-10-XX)", 0 )

