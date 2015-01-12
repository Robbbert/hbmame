#include "../mame/drivers/williams.c"

ROM_START( defender2 )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "10",         0x0d000, 0x1000, CRC(4d82fc2f) SHA1(b11dc3f3f2100ab14ad5b26cd333b212c454cc8d) )
	ROM_LOAD( "11",         0x0e000, 0x1000, CRC(561fad3f) SHA1(597851e78423f30341f682f3fc62bf24cf9e8a91) )
	ROM_LOAD( "12",         0x0f000, 0x1000, CRC(836eacca) SHA1(f788fc2a25adffa21e02a3dfe2074e8719405200) )
	ROM_LOAD( "01",         0x10000, 0x1000, CRC(88824d18) SHA1(f003a5a9319c4eb8991fa2aae3f10c72d6b8e81a) )
	ROM_LOAD( "02",         0x11000, 0x1000, CRC(afc614c5) SHA1(087c6da93318e8dc922d3d22e0a2af7b9759701c) )
	ROM_LOAD( "03",         0x12000, 0x1000, CRC(6dd57970) SHA1(1fdee9180bfb835f35ac786116e0f8d7833f2c3f) )
	ROM_LOAD( "04",         0x13000, 0x1000, CRC(a8b4bf0f) SHA1(6b4d47c2899fe9f14f9dab5928499f12078c437d) )
	ROM_LOAD( "05",         0x14000, 0x1000, CRC(2d306074) SHA1(54f871983699113e31bb756d4ca885c26c2d66b4) )
	ROM_LOAD( "06",         0x15000, 0x1000, CRC(53598dde) SHA1(54b02d944caf95283c9b6f0160e75ea8c4ccc97b) )
	ROM_LOAD( "07",         0x16000, 0x1000, CRC(55b5b0d2) SHA1(dcf392a899fb6c225c4e31034e242e575c3dc94d) )
	ROM_LOAD( "08",         0x17000, 0x1000, CRC(b4f22d4d) SHA1(2486e895628df9ae1268d9eb2434f240dba99f70) )
	ROM_LOAD( "09",         0x18000, 0x1000, CRC(6df7463d) SHA1(b80c2872fc47b4b604967f176e3931cd992dd692) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "sg.snd",      0xf800, 0x0800, CRC(2fcf6c4d) SHA1(9c4334ac3ff15d94001b22fc367af40f9deb7d57) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.4",   0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder.5",   0x0200, 0x0200, CRC(f921c5fe) SHA1(9cebb8bb935315101d248140d1b4503993ebdf8a) )
ROM_END

ROM_START( splatsc1 )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "splatsc1.10", 0x0d000, 0x1000, CRC(6cc2988a) SHA1(bd0648185ab859afc981d4e8ab954ed1f39303e4) )
	ROM_LOAD( "splatsc1.11", 0x0e000, 0x1000, CRC(ee960115) SHA1(b3a03b14f7ee97d55b948ecc09c5e4006c31f0ce) )
	ROM_LOAD( "splatsc1.12", 0x0f000, 0x1000, CRC(21916fce) SHA1(6896afd10c30d90b807b60f90fced7114b1812e3) )
	ROM_LOAD( "splat.01",    0x10000, 0x1000, CRC(1cf26e48) SHA1(6ba4de6cc7d1359ed450da7bae1000552373f873) )
	ROM_LOAD( "splat.02",    0x11000, 0x1000, CRC(ac0d4276) SHA1(710aba98909d5d63c4b9b08579021f9c026b3111) )
	ROM_LOAD( "splat.03",    0x12000, 0x1000, CRC(74873e59) SHA1(727c9da682fd10353f3969ef02e9f1826d8cb77a) )
	ROM_LOAD( "splat.04",    0x13000, 0x1000, CRC(70a7064e) SHA1(7e6440585462b68b62d6d571d83635bf17149f1a) )
	ROM_LOAD( "splatsc1.05", 0x14000, 0x1000, CRC(6989e923) SHA1(383d831084cf79e3961125142d1e3bf4fad1ac66) )
	ROM_LOAD( "splatsc1.06", 0x15000, 0x1000, CRC(82b22e55) SHA1(d71c6598f6a43f47f6752c803069efeb877bf1f0) )
	ROM_LOAD( "splatsc1.07", 0x16000, 0x1000, CRC(e0c4230c) SHA1(2795732725f63e766ee23744147bf002373bc5f7) )
	ROM_LOAD( "splatsc1.08", 0x17000, 0x1000, CRC(42ff589f) SHA1(72c9b40c9291a6b97f637c6424265b30227d87c6) )
	ROM_LOAD( "splatsc1.09", 0x18000, 0x1000, CRC(f67564be) SHA1(3fda9032953e4f75df838616d1c7023a1f2e9861) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "splat.snd",   0xf000, 0x1000, CRC(a878d5f3) SHA1(f3347a354cb54ca228fe0971f0ae3bc778e2aecf) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.4",   0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder.6",   0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END


/*******************************************************
 *
 *  Game Driver
 *
 *******************************************************/

GAME( 1981, defender2, defender, williams, stargate, williams_state, stargate, ROT0, "Williams", "Defender II", GAME_SUPPORTS_SAVE )
GAME( 1982, splatsc1, 0, williams, splat,    williams_state, joust,    ROT0, "Williams", "Splat! (sc1 hardware)", GAME_SUPPORTS_SAVE )
