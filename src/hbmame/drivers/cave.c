// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/cave.c"

ROM_START( ddonpacha )
	ROM_REGION( 0x100000, "maincpu", 0 ) /* 68000 Code */
	ROM_LOAD16_BYTE( "ddonarr.u27.bin", 0x000000, 0x080000, CRC(44b899ae) SHA1(798ec437d861b94fcd90c99a7015dd420887c788) )
	ROM_LOAD16_BYTE( "ddonarr.u26.bin", 0x000001, 0x080000, CRC(727a09a8) SHA1(91876386855f19e8a3d8d1df71dfe9b3d98e9ea9) )

	ROM_REGION( 0x800000 * 2, "sprites0", 0 ) /* Sprites: * 2 */
	ROM_LOAD( "u50.bin", 0x000000, 0x200000, CRC(14b260ec) SHA1(33bda210302428d5500115d0c7a839cdfcb67d17) )
	ROM_LOAD( "ddonarr.u51.bin", 0x200000, 0x200000, CRC(0f3e5148) SHA1(3016f4d075940feae691389606cd2aa7ac53849e) )
	ROM_LOAD( "u52.bin", 0x400000, 0x200000, CRC(02492ee0) SHA1(64d9cc64a4ad189a8b03cf6a749ddb732b4a0014) )
	ROM_LOAD( "u53.bin", 0x600000, 0x200000, CRC(cb4c10f0) SHA1(a622e8bd0c938b5d38b392b247400b744d8be288) )

	ROM_REGION( 0x200000, "layer0", 0 ) /* Layer 0 */
	ROM_LOAD( "u60.bin", 0x000000, 0x200000, CRC(903096a7) SHA1(a243e903fef7c4a7b71383263e82e42acd869261) )

	ROM_REGION( 0x200000, "layer1", 0 ) /* Layer 1 */
	ROM_LOAD( "u61.bin", 0x000000, 0x200000, CRC(d89b7631) SHA1(a66bb4955ca58fab8973ca37a0f971e9a67ce017) )

	ROM_REGION( 0x200000, "layer2", 0 ) /* Layer 2 */
	ROM_LOAD( "ddonarr.u62.bin", 0x000000, 0x200000, CRC(42e4c6c5) SHA1(4d282f7592f5fc5e11839c57f39cae20b8422aa1) )

	ROM_REGION( 0x400000, "ymz", 0 ) /* Samples */
	ROM_LOAD( "u6.bin", 0x000000, 0x200000, CRC(9dfdafaf) SHA1(f5cb450cdc78a20c3a74c6dac05c9ac3cba08327) )
	ROM_LOAD( "u7.bin", 0x200000, 0x200000, CRC(795b17d5) SHA1(cbfc29f1df9600c82e0fdae00edd00da5b73e14c) )

	ROM_REGION16_BE( 0x80, "eeprom", 0 )
//	ROM_LOAD16_WORD( "eeprom-ddonpach.bin", 0x0000, 0x0080, CRC(315fb546) SHA1(7f597107d1610fc286413e0e93c794c80c0c554f) )
	ROM_LOAD16_WORD( "eeprom-ddonpacha.bin", 0x0000, 0x0080, CRC(2df16438) SHA1(4881b70589a97e2420feb6d6e6737273beeff303) )
ROM_END

ROM_START( pwrinst2h )
	ROM_REGION( 0x200000, "maincpu", 0 )        /* 68000 code */
	ROM_LOAD16_BYTE( "pwrinst2h.u45", 0x000000, 0x80000, CRC(7b33bc43) SHA1(a68eb94e679f03c354932b8c5cd1bb2922fec0aa) )
	ROM_LOAD16_BYTE( "pwrinst2h.u44", 0x000001, 0x80000, CRC(8f6f6637) SHA1(024b12c0fe40e27c79e38bd7601a9183a62d75fd) )
	ROM_LOAD16_BYTE( "g02.u43", 0x100000, 0x80000, CRC(178e3d24) SHA1(926234f4196a5d5e3bd1438abbf73355f2c65b06) )
	ROM_LOAD16_BYTE( "g02.u42", 0x100001, 0x80000, CRC(a0b4ee99) SHA1(c6df4aa2543b04d8bda7683f503e5eb763e506af) )

	ROM_REGION16_BE( 0x100000, "user1", ROMREGION_ERASE00 ) /* 68000 extra data roms */
	/* not used */

	ROM_REGION( 0x20000, "audiocpu", 0 )        /* Z80 code */
	ROM_LOAD( "g02.u3a", 0x00000, 0x20000, CRC(ebea5e1e) SHA1(4d3af9e5f29d0c1b26563f51250039c9e8bd3735) )

	ROM_REGION( 0xe00000 * 2, "sprites0", 0 )        /* Sprites: * 2 */
	ROM_LOAD( "g02.u61", 0x000000, 0x200000, CRC(91e30398) SHA1(2b59a5e40bed2a988382054fe30d92808dad3348) )
	ROM_LOAD( "g02.u62", 0x200000, 0x200000, CRC(d9455dd7) SHA1(afa69fe9a540cd78b8cfecf09cffa1401c01141a) )
	ROM_LOAD( "g02.u63", 0x400000, 0x200000, CRC(4d20560b) SHA1(ceaee8cf0b69cc366b95ddcb689a5594d79e5114) )
	ROM_LOAD( "g02.u64", 0x600000, 0x200000, CRC(b17b9b6e) SHA1(fc6213d8322cda4c7f653e2d7d6d314ce84c97b7) )
	ROM_LOAD( "g02.u65", 0x800000, 0x200000, CRC(08541878) SHA1(138cf077a49a26440a3da1bdc2c399a208359e57) )
	ROM_LOAD( "g02.u66", 0xa00000, 0x200000, CRC(becf2a36) SHA1(f8b386d0292b1dc745b7253a3df51d1aa8d5e9db) )
	ROM_LOAD( "g02.u67", 0xc00000, 0x200000, CRC(52fe2b8b) SHA1(dd50aa62f7db995e28f47de9b3fb749aeeaaa5b0) )

	ROM_REGION( 0x200000, "layer0", 0 ) /* Layer 0 */
	ROM_LOAD( "g02.u78", 0x000000, 0x200000, CRC(1eca63d2) SHA1(538942b43301f950e3d5139461331c54dc90129d) )

	ROM_REGION( 0x100000, "layer1", 0 ) /* Layer 1 */
	ROM_LOAD( "g02.u81", 0x000000, 0x100000, CRC(8a3ff685) SHA1(4a59ec50ec4470453374fe10f76d3e894494b49f) )

	ROM_REGION( 0x100000, "layer2", 0 ) /* Layer 2 */
	ROM_LOAD( "g02.u89", 0x000000, 0x100000, CRC(373e1f73) SHA1(ec1ae9fab37eee41be8e1bc6dad03809b62fdbce) )

	ROM_REGION( 0x080000, "layer3", 0 ) /* Layer 3 */
	ROM_LOAD( "g02.82a", 0x000000, 0x080000, CRC(4b3567d6) SHA1(d3e14783b312d2bea9722a8e3c22bcec81e26166) )

	ROM_REGION( 0x440000, "oki1", 0 )   /* OKIM6295 #1 Samples */
	/* Leave the 0x40000 bytes addressable by the chip empty */
	ROM_LOAD( "g02.u53", 0x040000, 0x200000, CRC(c4bdd9e0) SHA1(a938a831e789ddf6f3cc5f3e5f3877ec7bd62d4e) )
	ROM_LOAD( "g02.u54", 0x240000, 0x200000, CRC(1357d50e) SHA1(433766177ce9d6933f90de85ba91bfc6d8d5d664) )

	ROM_REGION( 0x440000, "oki2", 0 )   /* OKIM6295 #2 Samples */
	/* Leave the 0x40000 bytes addressable by the chip empty */
	ROM_LOAD( "g02.u55", 0x040000, 0x200000, CRC(2d102898) SHA1(bd81f4cd2ba100707db0c5bb1419f0b23c998574) )
	ROM_LOAD( "g02.u56", 0x240000, 0x200000, CRC(9ff50dda) SHA1(1121685e387c20e228032f2b0f5cbb606376fc15) )
ROM_END

GAME( 2012, ddonpacha,  ddonpach, ddonpach, cave,     cave_state, ddonpach, ROT270, "trap15", "DoDonPachi Arrange (v1.1)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, pwrinst2h,  pwrinst2, pwrinst2, metmqstr, cave_state, pwrinst2j,ROT0,   "TT", "Power Instinct 2 (hack)", MACHINE_SUPPORTS_SAVE )
