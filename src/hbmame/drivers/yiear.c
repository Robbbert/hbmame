// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/yiear.c"

ROM_START( nacho )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "nacho.8",      0x08000, 0x4000, CRC(ad779f38) SHA1(057f05f789261f0b9a0f9d32623c3de10d5ed5e6) )
	ROM_LOAD( "nacho.7",      0x0c000, 0x4000, CRC(6d9b7c36) SHA1(634c68617df9d3078be85ccaefda3719b9e06bab) )

	ROM_REGION( 0x04000, "gfx1", 0 )
	ROM_LOAD( "nacho.1",      0x00000, 0x2000, CRC(7f1139ef) SHA1(04872803bcfd468490aa68b6b4b8c6219d0e3805) )
	ROM_LOAD( "nacho.2",      0x02000, 0x2000, CRC(cd8be306) SHA1(7e6fe875e808def4da7d5b19e291481d736e418c) )

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "nacho.5",      0x00000, 0x4000, CRC(2da87538) SHA1(37f17b8a6e1ad2fc2a94f90e54fa9ae8a5380b45) )
	ROM_LOAD( "nacho.6",      0x04000, 0x4000, CRC(b48393a9) SHA1(d8f4011423027e9f22f3a62e16e9f77e0fbfdb2c) )
	ROM_LOAD( "nacho.3",      0x08000, 0x4000, CRC(75bad724) SHA1(b6fad1771b7600867de3cafddd7555a8ab0f4f98) )
	ROM_LOAD( "nacho.4",      0x0c000, 0x4000, CRC(3ecb72e4) SHA1(4fc494bf3025b3b9fd05fc9f4f6d865039b3ffc8) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "yiear.clr",    0x00000, 0x0020, CRC(c283d71f) SHA1(10cd39f4e951ba6ca5610081c8c1fcd9d68b34d2) )

	ROM_REGION( 0x2000, "vlm", 0 )	/* 8k for the VLM5030 data */
	ROM_LOAD( "a12_9.bin",    0x00000, 0x2000, CRC(f75a1539) SHA1(f139f6cb41351eb81ee47d777db03012aa5fadb1) )
ROM_END

GAME( 2009, nacho, yiear, yiear, yiear, driver_device, 0, ROT0, "Gatinho", "Nacho Libre", MACHINE_SUPPORTS_SAVE )
