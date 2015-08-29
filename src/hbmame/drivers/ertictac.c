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
	// note all instructions are 4 bytes, and the bytes are in reverse order to what you see in the debugger
	ROM_FILL(0x423, 1, 0x03) // set up control (no video without this)
	ROM_FILL(0x422, 1, 0x6E)
	ROM_FILL(0x421, 1, 0x05)
	ROM_FILL(0x420, 1, 0xA4)
	ROM_FILL(0x427, 1, 0xE5)
	ROM_FILL(0x426, 1, 0x1F)
	ROM_FILL(0x425, 1, 0x00)
	ROM_FILL(0x424, 1, 0x0C)
	ROM_FILL(0x42B, 1, 0xE5)
	ROM_FILL(0x42A, 1, 0x80)
	ROM_FILL(0x428, 2, 0x00)
	ROM_FILL(0x42F, 1, 0xE3) // i/o byte (has no effect)
	ROM_FILL(0x42E, 1, 0xA0)
	ROM_FILL(0x42D, 1, 0x06)
	ROM_FILL(0x42C, 1, 0x33)
	ROM_FILL(0x433, 1, 0xE2)
	ROM_FILL(0x432, 1, 0x80)
	ROM_FILL(0x431, 1, 0x08)
	ROM_FILL(0x430, 1, 0x05)
	ROM_FILL(0x437, 1, 0xE2)
	ROM_FILL(0x436, 1, 0x80)
	ROM_FILL(0x435, 1, 0x00)
	ROM_FILL(0x434, 1, 0x18)
	ROM_FILL(0x43B, 1, 0xE3)
	ROM_FILL(0x43A, 1, 0xA0)
	ROM_FILL(0x439, 1, 0x10)
	ROM_FILL(0x438, 1, 0x00)
	ROM_FILL(0x43F, 1, 0xE5)
	ROM_FILL(0x43E, 1, 0xC0)
	ROM_FILL(0x43D, 1, 0x10)
	ROM_FILL(0x43C, 1, 0x00)
	ROM_FILL(0x443, 1, 0xEA) // back to normal programming
	ROM_FILL(0x442, 1, 0xFF)
	ROM_FILL(0x441, 1, 0xFE)
	ROM_FILL(0x440, 1, 0xF7)
	ROM_FILL(0x001, 1, 0x01) // jump to this lot

	ROM_REGION(0x200000, "vram", ROMREGION_ERASE00)
ROM_END

GAME( 1990, armdemo, 0, ertictac, ertictac, driver_device, 0, ROT0, "Charles Doty", "Demo - ARM" , MACHINE_NOT_WORKING | MACHINE_NO_SOUND_HW )
