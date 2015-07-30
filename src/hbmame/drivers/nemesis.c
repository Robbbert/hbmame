// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/nemesis.c"

ROM_START( spclone )
	ROM_REGION( 0x80000, "maincpu", 0 )    /* 64k for code */
	ROM_LOAD16_BYTE( "587yq02.18b",  0x00000, 0x10000, CRC(6d49878b) SHA1(95ec8c326cb83d8391f7821868187d68ef734120) )
	ROM_LOAD16_BYTE( "587yq05.18c",  0x00001, 0x10000, CRC(08b8fd27) SHA1(fbbb5f9c795de5a776309cdd7a5678fc0061d52a) )
	ROM_LOAD16_BYTE( "17b.bin",      0x40000, 0x20000, CRC(e5caf6e6) SHA1(f5df4fbc43cfa6e2866558c99dd95ba8dc89dc7a) ) /* Mask rom */
	ROM_LOAD16_BYTE( "17c.bin",      0x40001, 0x20000, CRC(c2f567ea) SHA1(0c38fea53f3d4a9ae0deada5669deca4be8c9fd3) ) /* Mask rom */

	ROM_REGION( 0x10000, "audiocpu", 0 )    /* 64k for sound */
	ROM_LOAD(      "587yq09.11j",      0x00000, 0x08000, CRC(9b72fdc7) SHA1(6634e2c8203cc7a9f5b2135a0b717dc18b0fdf23) )

	ROM_REGION( 0x04000, "vlm", 0 )    /* VLM5030 data? */
	ROM_LOAD(      "587-d08.8g",       0x00000, 0x04000, CRC(f9ac6b82) SHA1(3370fc3a7f82e922e19d54afb3bca7b07fa4aa9a) )

	ROM_REGION( 0x20000, "k007232", 0 )    /* 007232 data */
	ROM_LOAD(      "10a.bin",      0x00000, 0x20000, CRC(09fe0632) SHA1(4c3b29c623d70bbe8a938a0beb4638912c46fb6a) ) /* Mask rom */
ROM_END

GAME( 2007, spclone, 0, salamand, lifefrcj, driver_device, 0, ROT0, "Konami", "Salamander (version YY)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
