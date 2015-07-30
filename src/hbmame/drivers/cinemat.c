// license:BSD-3-Clause
// copyright-holders:Robbbert
/* BBUGTEST NOTES

With no documentation whatsoever, I have compiled what I know to share with you all.

I do not know if the graphics are complete, or if all tests are working.

Use the dipswitches to select a test pattern.

Pressing any player key, or start button will make a slight change to the colours.

If you move your mouse to the left, pressing the keys will alter the numbers across the bottom
of the screen. Moving the mouse to the right will revert to changing colours again.

*/


#include "../mame/drivers/cinemat.c"

static MACHINE_CONFIG_DERIVED( bbugtest, cinemat_jmi_32k )
	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_VISIBLE_AREA(0, 1024, 0, 788)
	MCFG_VIDEO_START_OVERRIDE(cinemat_state,cinemat_color)
MACHINE_CONFIG_END

/*************************************
 *
 *  Game drivers
 *
 *************************************/

ROM_START( bbugtest )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 16k for code */
	ROM_LOAD16_BYTE( "bbugtest.t7", 0x0000, 0x1000, CRC(92a460f1) SHA1(eaa8fea38d87f1fd6e2e52b92085fed43498e5ce) )
	ROM_LOAD16_BYTE( "bbugtest.p7", 0x0001, 0x1000, CRC(06d0bde2) SHA1(b0a224b7a74fa739b293ad7383236ffaa012f3c3) )

	ROM_REGION( 0x1a0, "proms", 0 )
	ROM_LOAD("prom.f14", 0x000, 0x100, CRC(9edbf536) SHA1(036ad8a231284e05f44b1106d38fc0c7e041b6e8) )
	ROM_LOAD("prom.e14", 0x100, 0x020, CRC(29dbfb87) SHA1(d8c40ab010b2ea30f29b2c443819e2b69f376c04) )
	ROM_LOAD("prom.d14", 0x120, 0x020, CRC(9a05afbf) SHA1(5d806a42424942ba5ef0b70a1d629315b37f931b) )
	ROM_LOAD("prom.c14", 0x140, 0x020, CRC(07492cda) SHA1(32df9148797c23f70db47b840139c40e046dd710) )
	ROM_LOAD("prom.j14", 0x160, 0x020, CRC(a481ca71) SHA1(ce145d61686f600cc16b77febfd5c783bf8c13b0) )
	ROM_LOAD("prom.e8",  0x180, 0x020, CRC(791ec9e1) SHA1(6f7fcce4aa3be9020595235568381588adaab88e) )
ROM_END

GAME(1981, bbugtest, 0, bbugtest, boxingb, cinemat_state, boxingb, ORIENTATION_FLIP_Y, "Test Rom", "Test Rom - Boxing Bugs", MACHINE_NO_SOUND_HW | MACHINE_SUPPORTS_SAVE )
