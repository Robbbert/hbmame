// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/ertictac.c"

// worked in 0.128, not now
ROM_START( armdemo )
	ROM_REGION(0x800000, "maincpu", 0 )
	ROM_LOAD32_BYTE( "armdemo.01", 0x00000, 0x10000, CRC(62b840be) SHA1(148b30b1e851e1c012482dde27f4159d1ac56a15) )
	ROM_LOAD32_BYTE( "armdemo.02", 0x00001, 0x10000, CRC(78d41ef7) SHA1(db7b33e91507314371e6b21a5b92bc259dc06559) )
	ROM_LOAD32_BYTE( "armdemo.03", 0x00002, 0x10000, CRC(efd0057f) SHA1(61baccc350623aca6d8b940816459372368c31ef) )
	ROM_LOAD32_BYTE( "armdemo.04", 0x00003, 0x10000, CRC(55eb17b2) SHA1(c55614124214a0ad5617bfbab419f340ee48e6ed) )

	ROM_REGION(0x200000, "vram", ROMREGION_ERASE00)
ROM_END

GAME( 1990, armdemo, 0, ertictac, ertictac, driver_device, 0, ROT0, "Charles Doty", "Demo - ARM" , MACHINE_NOT_WORKING | MACHINE_NO_SOUND_HW )
