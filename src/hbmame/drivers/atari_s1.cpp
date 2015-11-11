// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/atari_s1.cpp"

/*-------------------------------------------------------------------
	The Atarians (working bootleg)
Game modified to work in emulation from original set (atarians)

Except, it doesn't work.
-------------------------------------------------------------------*/
ROM_START(atarianb)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("atarianb.e00", 0x7000, 0x0800, CRC(74fc86e4) SHA1(135d75e5c03feae0929fa84caa3c802353cdd94e))
	ROM_LOAD("atarian.e0", 0x7800, 0x0800, CRC(45cb0427) SHA1(e286930ca36bdd0f79acefd142d2a5431fa8005b))

	ROM_REGION(0x0200, "proms", 0)
	ROM_LOAD("82s130.bin", 0x0000, 0x0200, CRC(da1f77b4) SHA1(b21fdc1c6f196c320ec5404013d672c35f95890b))
ROM_END

GAME( 2002, atarianb, atarians, atari_s1, atari_s1, driver_device, 0, ROT0, "Atari / Gaston", "The Atarians (modified for emulation)", MACHINE_NOT_WORKING | MACHINE_MECHANICAL)
