// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/segag80r.cpp"

ROM_START( astrob2h )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "829b",     0x0000, 0x0800, CRC(14ae953c) SHA1(eb63d1b95faa5193db7fa6ab245e99325d519b5e) ) /* U25 */
	ROM_LOAD( "888",      0x0800, 0x0800, CRC(42601744) SHA1(6bb58384c28b2105746a2f410f5e0979609db9bf) ) /* U1 */
	ROM_LOAD( "889bh",    0x1000, 0x0800, CRC(9fcdc62f) SHA1(6dbf7e1ad62548263ca5edaa81d38cbc7a9a281f) ) /* U2 */
	ROM_LOAD( "890",      0x1800, 0x0800, CRC(26f5b4cf) SHA1(be45e802f976b8847689ae8de7159843ac9100eb) ) /* U3 */
	ROM_LOAD( "891bh",    0x2000, 0x0800, CRC(74f906dc) SHA1(edf9efc0ec25f578214b26010d99385c0bace3ad) ) /* U4 */
	ROM_LOAD( "892",      0x2800, 0x0800, CRC(2d3c949b) SHA1(17e3c5300793f2345ff6e28e82cd7a22f1d6e41f) ) /* U5 */
	ROM_LOAD( "893",      0x3000, 0x0800, CRC(ccdb1a76) SHA1(1c8f0555e397c5558bbfca1fa1487cc32aca8592) ) /* U6 */
	ROM_LOAD( "894",      0x3800, 0x0800, CRC(66ae5ced) SHA1(81bb6e3adcc76ffbeafefecce5fe5541a7eefc37) ) /* U7 */
	ROM_LOAD( "895",      0x4000, 0x0800, CRC(202cf3a3) SHA1(26fcccfb3e94b2a01d38c14daa66713c223efb18) ) /* U8 */
	ROM_LOAD( "896",      0x4800, 0x0800, CRC(b603fe23) SHA1(3128877355a9c5bba5cd22e9addf4c8b79ee39d2) ) /* U9 */
	ROM_LOAD( "897",      0x5000, 0x0800, CRC(989198c6) SHA1(3344bf7272e388571026c4e68a2e4e5e0ebbc5e3) ) /* U10 */
	ROM_LOAD( "898",      0x5800, 0x0800, CRC(ef2bab04) SHA1(108a9812cb9d1ec4629b0306c45ba164f94ab426) ) /* U11 */
	ROM_LOAD( "899",      0x6000, 0x0800, CRC(e0d189ee) SHA1(dcab31d64e6b2d248a82cbae9e37afe031dfc6cd) ) /* U12 */
	ROM_LOAD( "900",      0x6800, 0x0800, CRC(682d4604) SHA1(6ac0d2d8ff407cc7e10b460736ae7fbc21148640) ) /* U13 */
	ROM_LOAD( "901",      0x7000, 0x0800, CRC(9ed11c61) SHA1(dd965c06d2013acdabd958e713109eeb049d5d5e) ) /* U14 */
	ROM_LOAD( "902",      0x7800, 0x0800, CRC(b4d6c330) SHA1(922a562b5f1a8a286e6777ba7d141bd0db6e2a92) ) /* U15 */
	ROM_LOAD( "903",      0x8000, 0x0800, CRC(84acc38c) SHA1(86bed143ac2d95116e50e77b5c262d67156c6a59) ) /* U16 */
	ROM_LOAD( "904",      0x8800, 0x0800, CRC(5eba3097) SHA1(e785d1c1cea50aa25e5eea5e58a0c48fd53208c6) ) /* U17 */
	ROM_LOAD( "905",      0x9000, 0x0800, CRC(4f08f9f4) SHA1(755a825b18ed50caa7bf274a0a5c3a1b00b1c070) ) /* U18 */
	ROM_LOAD( "906",      0x9800, 0x0800, CRC(58149df1) SHA1(2bba56576a225ca47ce31a5b6dcc491546dfffec) ) /* U19 */

	ROM_REGION( 0x0800, "audiocpu", 0 )
	ROM_LOAD( "808b.speech-u7", 0x0000, 0x0800, CRC(5988c767) SHA1(3b91a8cd46aa7e714028cc40f700fea32287afb1) )

	ROM_REGION( 0x4000, "speech", 0 )
	ROM_LOAD( "809a.speech-u6", 0x0000, 0x0800, CRC(893f228d) SHA1(41c08210d322105f5446cfaa1258c194dd078a34) )
	ROM_LOAD( "810.speech-u5",  0x0800, 0x0800, CRC(ff0163c5) SHA1(158a12f9bf01d25c7e98f34fce56df51d49e5a85) )
	ROM_LOAD( "811.speech-u4",  0x1000, 0x0800, CRC(219f3978) SHA1(728edb9251f7cde237fa3b005971366a099c6342) )
	ROM_LOAD( "812a.speech-u3", 0x1800, 0x0800, CRC(410ad0d2) SHA1(9b5f05bb64a6ecfe3543025a10c6ec67de797333) )
ROM_END

GAME( 1981, astrob2h, 0, astrob, astrob2, segag80r_state, astrob, ROT270, "Clay Cowgill", "Astro Blaster (ver 2 Hack)", 0 )
