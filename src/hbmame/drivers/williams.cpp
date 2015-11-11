// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/williams.cpp"

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

ROM_START( robotron201 )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "robotron.sba",  0x0d000, 0x1000, CRC(13797024) SHA1(d426a50e75dabe936de643c83a548da5e399331c) )
	ROM_LOAD( "robotron.sbb",  0x0e000, 0x1000, CRC(e83a2eda) SHA1(4a62fcd2f91dfb609c3d2c300bd9e6cb60edf52e) )
	ROM_LOAD( "robotron.sbc",  0x0f000, 0x1000, CRC(645d543e) SHA1(fad7cea868ebf17347c4bc5193d647bbd8f9517b) )
	ROM_LOAD( "robotron.sb1",  0x10000, 0x1000, CRC(66c7d3ef) SHA1(f6d60e26c209c1df2cc01ac07ad5559daa1b7118) )
	ROM_LOAD( "robotron.sb2",  0x11000, 0x1000, CRC(5bc6c614) SHA1(4d6e82bc29f49100f7751ccfc6a9ff35695b84b3) )
	ROM_LOAD( "robotron201.3", 0x12000, 0x1000, CRC(85eb583e) SHA1(b6c4280415515de6f56b358206dc3bd93a12bfce) )
	ROM_LOAD( "robotron.sb4",  0x13000, 0x1000, CRC(afb1c561) SHA1(aaf89c19fd8f4e8750717169eb1af476aef38a5e) )
	ROM_LOAD( "robotronp.5",   0x14000, 0x1000, CRC(827cb5c9) SHA1(1732d16cd88e0662f1cffce1aeda5c8aa8c31338) )
	ROM_LOAD( "robotron.sb6",  0x15000, 0x1000, CRC(bd2c853d) SHA1(f76ec5432a7939b33a27be1c6855e2dbe6d9fdc8) )
	ROM_LOAD( "robotron.sb7",  0x16000, 0x1000, CRC(49ac400c) SHA1(06eae5138254723819a5e93cfd9e9f3285fcddf5) )
	ROM_LOAD( "robotron.sb8",  0x17000, 0x1000, CRC(3a96e88c) SHA1(7ae38a609ed9a6f62ca003cab719740ed7651b7c) )
	ROM_LOAD( "robotron.sb9",  0x18000, 0x1000, CRC(b124367b) SHA1(fd9d75b866f0ebbb723f84889337e6814496a103) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "robotron.snd", 0xf000, 0x1000, CRC(c56c1d28) SHA1(15afefef11bfc3ab78f61ab046701db78d160ec3) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.4",   0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder.6",   0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

ROM_START( robotronp )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "robotron.sba", 0x0d000, 0x1000, CRC(13797024) SHA1(d426a50e75dabe936de643c83a548da5e399331c) )
	ROM_LOAD( "robotron.sbb", 0x0e000, 0x1000, CRC(e83a2eda) SHA1(4a62fcd2f91dfb609c3d2c300bd9e6cb60edf52e) )
	ROM_LOAD( "robotron.sbc", 0x0f000, 0x1000, CRC(645d543e) SHA1(fad7cea868ebf17347c4bc5193d647bbd8f9517b) )
	ROM_LOAD( "robotron.sb1", 0x10000, 0x1000, CRC(66c7d3ef) SHA1(f6d60e26c209c1df2cc01ac07ad5559daa1b7118) )
	ROM_LOAD( "robotron.sb2", 0x11000, 0x1000, CRC(5bc6c614) SHA1(4d6e82bc29f49100f7751ccfc6a9ff35695b84b3) )
	ROM_LOAD( "robotron.sb3", 0x12000, 0x1000, CRC(e99a82be) SHA1(06a8c8dd0b4726eb7f0bb0e89c8533931d75fc1c) )
	ROM_LOAD( "robotron.sb4", 0x13000, 0x1000, CRC(afb1c561) SHA1(aaf89c19fd8f4e8750717169eb1af476aef38a5e) )
	ROM_LOAD( "robotronp.5",  0x14000, 0x1000, CRC(827cb5c9) SHA1(1732d16cd88e0662f1cffce1aeda5c8aa8c31338) )
	ROM_LOAD( "robotron.sb6", 0x15000, 0x1000, CRC(bd2c853d) SHA1(f76ec5432a7939b33a27be1c6855e2dbe6d9fdc8) )
	ROM_LOAD( "robotron.sb7", 0x16000, 0x1000, CRC(49ac400c) SHA1(06eae5138254723819a5e93cfd9e9f3285fcddf5) )
	ROM_LOAD( "robotron.sb8", 0x17000, 0x1000, CRC(3a96e88c) SHA1(7ae38a609ed9a6f62ca003cab719740ed7651b7c) )
	ROM_LOAD( "robotron.sb9", 0x18000, 0x1000, CRC(b124367b) SHA1(fd9d75b866f0ebbb723f84889337e6814496a103) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "robotron.snd", 0xf000, 0x1000, CRC(c56c1d28) SHA1(15afefef11bfc3ab78f61ab046701db78d160ec3) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.4",   0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder.6",   0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

ROM_START( robotrontie )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "robotrontie.a", 0x0d000, 0x1000, CRC(952bea55) SHA1(80f51d8e7ec62518afad7e56a47e0756f83f813c) )
	ROM_LOAD( "robotrontie.b", 0x0e000, 0x1000, CRC(4c05fd3c) SHA1(0d727458454826fd8222e4022b755d686ccb065f) )
	ROM_LOAD( "robotron.sbc",  0x0f000, 0x1000, CRC(645d543e) SHA1(fad7cea868ebf17347c4bc5193d647bbd8f9517b) )
	ROM_LOAD( "robotron.sb1",  0x10000, 0x1000, CRC(66c7d3ef) SHA1(f6d60e26c209c1df2cc01ac07ad5559daa1b7118) )
	ROM_LOAD( "robotron.sb2",  0x11000, 0x1000, CRC(5bc6c614) SHA1(4d6e82bc29f49100f7751ccfc6a9ff35695b84b3) )
	ROM_LOAD( "robotron.sb3",  0x12000, 0x1000, CRC(e99a82be) SHA1(06a8c8dd0b4726eb7f0bb0e89c8533931d75fc1c) )
	ROM_LOAD( "robotrontie.4", 0x13000, 0x1000, CRC(e8238019) SHA1(0ce29f4bf6bdee677c8e80c2d5e66fc556ba349f) )
	ROM_LOAD( "robotronp.5",   0x14000, 0x1000, CRC(827cb5c9) SHA1(1732d16cd88e0662f1cffce1aeda5c8aa8c31338) )
	ROM_LOAD( "robotron.sb6",  0x15000, 0x1000, CRC(bd2c853d) SHA1(f76ec5432a7939b33a27be1c6855e2dbe6d9fdc8) )
	ROM_LOAD( "robotrontie.7", 0x16000, 0x1000, CRC(3ecf4620) SHA1(3c670a1f8df35d18451c82f220a02448bf5ef5ac) )
	ROM_LOAD( "robotrontie.8", 0x17000, 0x1000, CRC(752d7a46) SHA1(85dd58d14d527ca75d6c546d6271bf8ee5a82c8c) )
	ROM_LOAD( "robotron.sb9",  0x18000, 0x1000, CRC(b124367b) SHA1(fd9d75b866f0ebbb723f84889337e6814496a103) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "robotron.snd", 0xf000, 0x1000, CRC(c56c1d28) SHA1(15afefef11bfc3ab78f61ab046701db78d160ec3) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.4",   0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder.6",   0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

/*******************************************************
 *
 *  Game Driver
 *
 *******************************************************/

GAME( 1981, defender2,   defender, williams, stargate, williams_state, stargate, ROT0, "Jeff Vavasour", "Defender II", MACHINE_SUPPORTS_SAVE )
GAME( 1982, splatsc1,    0,        williams, splat,    williams_state, joust,    ROT0, "Williams", "Splat! (sc1 hardware)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, robotron201, robotron, williams, robotron, williams_state, robotron, ROT0, "Sean Riddle", "Robotron: 2084 (Hacked Patched Blue-201)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, robotronp,   robotron, williams, robotron, williams_state, robotron, ROT0, "Christian Gingras / Sean Riddle", "Robotron: 2084 (patched)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, robotrontie, robotron, williams, robotron, williams_state, robotron, ROT0, "Vid Kidz", "Robotron: 2084 (Tie Die)", MACHINE_SUPPORTS_SAVE )
