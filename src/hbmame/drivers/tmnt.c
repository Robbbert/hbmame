// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/tmnt.c"

ROM_START( ssridersaex )
	ROM_REGION( 0xc0000, "maincpu", 0 ) /* 2*256k and 2*128k for 68000 code */
	ROM_LOAD16_BYTE( "064eac02.8e-ex",  0x000000, 0x40000, CRC(536d4a79) SHA1(dec7f827e3a1300ee32e6134b8141ae99a89b50a) )
	ROM_LOAD16_BYTE( "064eac03.8g-ex",  0x000001, 0x40000, CRC(8ea4c0e8) SHA1(c971df7fdaa801c6351a5df401a7528f000bee06) )
	ROM_LOAD16_BYTE( "064eab04.10e", 0x080000, 0x20000, CRC(ef2315bd) SHA1(2c8b11321cb5fdb78d760fabca666c0d8cc5b298) )
	ROM_LOAD16_BYTE( "064eab05.10g", 0x080001, 0x20000, CRC(51d6fbc4) SHA1(e80de7d155b7f263c48ef4ae2702059be3c18e76) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "064e01.2f",   0x0000, 0x10000, CRC(44b9bc52) SHA1(4654d6e14c6956c40a19cb41155accb63f0da338) )

	ROM_REGION( 0x100000, "k052109", 0 )    /* tiles */
	ROM_LOAD32_WORD( "064e12.16k",   0x000000, 0x080000, CRC(e2bdc619) SHA1(04449deb267b0beacfa33640b593eb16194aa0d9) )
	ROM_LOAD32_WORD( "064e11.12k",   0x000002, 0x080000, CRC(2d8ca8b0) SHA1(7c882f79c2402cf75979c681071007d76e4db9ae) )

	ROM_REGION( 0x200000, "k053245", 0 )   /* graphics (addressable by the main CPU) */
	ROM_LOAD32_WORD( "064e09.7l", 0x000000, 0x100000, CRC(4160c372) SHA1(0b36181e5ccd785c7fb89b9f41e458066a42c3b0) )    /* sprites */
	ROM_LOAD32_WORD( "064e07.3l", 0x000002, 0x100000, CRC(64dd673c) SHA1(bea4d17a71dd21c635866ee69b4892dc9d0ab455) )

	ROM_REGION( 0x100000, "k053260", 0 )    /* samples for the 053260 */
	ROM_LOAD( "064e06.1d",    0x0000, 0x100000, CRC(59810df9) SHA1(a0affc6330bdbfab1447dc0cf13c20ff708c2c71) )

	ROM_REGION( 0x80, "eeprom", 0 ) // default eeprom to prevent game booting upside down with error
	ROM_LOAD( "ssriders_abd.nv", 0x0000, 0x080, CRC(bfdafedf) SHA1(bc786d4fb3495a6baf2ae13c19392c6740a2be6d) )
	//ROM_LOAD( "ssriders_eac.nv", 0x0000, 0x080, CRC(f6d641a7) SHA1(6c680d49e1b07a47b29ff263a6009d226bd120cf) )
ROM_END


//    YEAR, NAME,        PARENT,   MACHINE,  INPUT,     INIT,                    MONITOR,COMPANY,FULLNAME,FLAGS
GAME( 2009, ssridersaex, ssriders, ssriders, ssriders,  driver_device, 0,        ROT0,   "Pipi899","Sunset Riders (2 Players ver ABD EX Super Version)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
