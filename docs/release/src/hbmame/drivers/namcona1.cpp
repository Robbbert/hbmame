// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/namcona1.cpp"


/**************
 Emeraldia
***************/
ROM_START(emeralda01) // emeraldah
	ROM_REGION(0x200000, "maincpu", 0)
	ROM_LOAD16_BYTE("s01.6c",      0x000001, 0x080000, CRC(9b60320f) SHA1(7265f6eb84d35a495fbe012c29332ecc8a94a457) )
	ROM_LOAD16_BYTE("s01.6f",      0x000000, 0x080000, CRC(5411cbec) SHA1(253ff3c46ce2ebbac0df2ad0b3dda255135878db) )
	ROM_LOAD16_BYTE("em2-ep1l.7c", 0x100001, 0x080000, CRC(6c3e5b53) SHA1(72b941e28c7fda8cb81240a8226386fe55c14e2d))
	ROM_LOAD16_BYTE("em2-ep1u.7f", 0x100000, 0x080000, CRC(dee15a81) SHA1(474a264029bd77e4205773a7461dea695e65933f))

	ROM_REGION16_BE(0x800000, "maskrom", ROMREGION_ERASE00)
	/* no mask roms */
ROM_END

/**************
 Knuckle Heads
***************/
ROM_START(knckhead01)
	ROM_REGION(0x200000, "maincpu", 0)
	ROM_LOAD16_BYTE("s01.6c", 0x000001, 0x080000, CRC(7b85851d) SHA1(d283f98972c88f67cfdc281934d289d97ca5a707))
	ROM_LOAD16_BYTE("s01.6f", 0x000000, 0x080000, CRC(15cb1ed1) SHA1(e659999f21be66df87f00ffc8bc990ffe8173f9f))
	ROM_LOAD16_BYTE("kh1-ep1l.7c", 0x100001, 0x080000, CRC(27e6ab4e) SHA1(66f397cc2117c1e73652c4800c0937e6d8116380))
	ROM_LOAD16_BYTE("kh1-ep1u.7f", 0x100000, 0x080000, CRC(487b2434) SHA1(2d62db85ceac1fca61c39e4db92c96ae80ba3323))

	ROM_REGION16_BE(0x800000, "maskrom", 0)
	ROM_LOAD16_BYTE("kh1-ma0l.2c", 0x000001, 0x100000, CRC(7b2db5df) SHA1(ecc392c4683cf0718d986e73336b69952d324548))
	ROM_LOAD16_BYTE("kh1-ma0u.2f", 0x000000, 0x100000, CRC(6983228b) SHA1(5f3eeb780e9d91445b4c11da63d4ca580e654f34))
	ROM_LOAD16_BYTE("kh1-ma1l.3c", 0x200001, 0x100000, CRC(b24f93e6) SHA1(3d8951485dc8a2810da9ddf2f4896efa31779bf4))
	ROM_LOAD16_BYTE("kh1-ma1u.3f", 0x200000, 0x100000, CRC(18a60348) SHA1(298e0e0e7649e872791c3c99c81a19f273e9eb8a))
	ROM_LOAD16_BYTE("kh1-ma2l.4c", 0x400001, 0x100000, CRC(82064ee9) SHA1(0b984565d17e580f49fff982a1621ef90e14c064))
	ROM_LOAD16_BYTE("kh1-ma2u.4f", 0x400000, 0x100000, CRC(17fe8c3d) SHA1(88c45076477725faa5f8a23512e65a40385bb27d))
	ROM_LOAD16_BYTE("kh1-ma3l.5c", 0x600001, 0x100000, CRC(ad9a7807) SHA1(c40f18a68306e76acd89ccb3fc82b8106556912e))
	ROM_LOAD16_BYTE("kh1-ma3u.5f", 0x600000, 0x100000, CRC(efeb768d) SHA1(15d016244549f3ea0d19f5cfb04bcebd65ac6134))
ROM_END

/*    YEAR  NAME        PARENT       MACHINE    INPUT          CLASS           INIT           MONITOR COMPANY                 FULLNAME FLAGS */
// Emeraldia
GAME(2022, emeralda01,  emeralda,    namcona2,  namcona1_joy,  namcona2_state, init_emeraldj, ROT0, "Bankbank", "Emeraldia (New Rotate)", MACHINE_SUPPORTS_SAVE )
// Knuckle Heads
GAME(1992, knckhead01,  knckhead,    namcona2,  namcona1_joy,  namcona2_state, init_knckhead, ROT0, "DDj", "Knuckle Heads (Easy Move)", MACHINE_SUPPORTS_SAVE )

