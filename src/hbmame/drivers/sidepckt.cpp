// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/sidepckt.cpp"


ROM_START( sidepcktb2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "b-9.2a",    0x04000, 0x4000, CRC(40fd0d85) SHA1(f7f8e30def8c4a1a5630417458517fd2e71a8c38) )
	ROM_LOAD( "b-8.3a",    0x08000, 0x8000, CRC(26e0116a) SHA1(93a64559146cee646e958907393edc6c37bce793) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "dh04.3h",     0x08000, 0x8000, CRC(d076e62e) SHA1(720ff1a6a58697b4a9c7c4f31c24a2cf8a04900a) )

	ROM_REGION( 0x18000, "gfx1", 0 )
	ROM_LOAD( "dh07-e.13k",    0x00000, 0x8000, CRC(9d6f7969) SHA1(583852be0861a89c63ce09eb39146ec379b9e12d) )
	ROM_LOAD( "dh06-e.13j",    0x08000, 0x8000, CRC(580e4e43) SHA1(de152a5d4fbc52d80e3eb9af17835ecb6258d45e) )
	ROM_LOAD( "dh05-e.13h",    0x10000, 0x8000, CRC(05ab71d2) SHA1(6f06d1d1440a5fb05c01f712457d0bb167e93099) )

	ROM_REGION( 0x18000, "gfx2", 0 )
	ROM_LOAD( "dh01.14a",     0x00000, 0x8000, CRC(a2cdfbea) SHA1(0721e538e3306d616f11008f784cf21e679f330d) )
	ROM_LOAD( "dh02.15a",     0x08000, 0x8000, CRC(eeb5c3e7) SHA1(57eda1cc29124e04fe5025a904634d8ca52c0f12) )
	ROM_LOAD( "dh03.17a",     0x10000, 0x8000, CRC(8e18d21d) SHA1(74f0ddf1fcbed386332eba882b4136295b4f096d) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "dh-09.16l",    0x0000, 0x0100, CRC(ce049b4f) SHA1(e4918cef7b319dd40cf1722eb8bf5e79be04fd6c) )
	ROM_LOAD( "dh-08.15l",    0x0100, 0x0100, CRC(cdf2180f) SHA1(123215d096f88b66396d40d7a579380d0b5b2b89) )
ROM_END

GAME( 1986, sidepcktb2, sidepckt, sidepcktb, sidepcktb, sidepckt_state, empty_init,     ROT0, "bootleg", "Side Pocket (bootleg set 2)", MACHINE_SUPPORTS_SAVE )

