// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taitosj.cpp"

ROM_START( alpines01 )
	ROM_REGION( 0x12000, "maincpu", 0 )
	ROM_LOAD( "s01.rh16.069",     0x0000, 0x1000, CRC(9d859b68) SHA1(caa672e29a7a3fd595fb3be3cffd2e2f48ddc239) )
	ROM_LOAD( "s01.rh17.068",     0x1000, 0x1000, CRC(2dfd6b46) SHA1(7d65e87173b6fabbf1525223fea1279923286b38) )
	ROM_LOAD( "rh18.067",     0x2000, 0x1000, CRC(753bdd87) SHA1(37b97dd4a5d53df9a86593fd0a53c95475fa09d0) )
	ROM_LOAD( "rh19.066",     0x3000, 0x1000, CRC(3efb3fcd) SHA1(29fb6405ced78662c4d98deeac5593d7bc42d954) )
	ROM_LOAD( "s01.rh20.065",     0x4000, 0x1000, CRC(8c51030e) SHA1(676c1a79420dc9d707335b22a50b69fc7a1004b8) )
	ROM_LOAD( "rh21.064",     0x5000, 0x1000, CRC(74109145) SHA1(728714ec24962da1c54fc35dc3688d555a4ad101) )
	ROM_LOAD( "rh22.055",     0x6000, 0x1000, CRC(efa82a57) SHA1(b9b275014572c4c67558516d0c3c36d01e84e9ef) )
	ROM_LOAD( "rh23.054",     0x7000, 0x1000, CRC(77c25acf) SHA1(a48bf7044afa7388f68e05fdb2e63c2b04945462) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "rh13.070",     0x0000, 0x1000, CRC(dcad1794) SHA1(1d5479f10cdcc437241bb17c22204fb3ee60f8cb) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "s01.rh24.001",     0x0000, 0x1000, CRC(d8f21891) SHA1(f89934ae12f3361aa5cd96d55fee53473b97149f) )
	ROM_LOAD( "s01.rh25.002",     0x1000, 0x1000, CRC(5b528f71) SHA1(abd5cac0a2eb3bc068e890b287cf0e348d8e7df9) )
	ROM_LOAD( "rh26.003",     0x2000, 0x1000, CRC(13da2a9b) SHA1(e3dd30a1036ec81b3867dc1c0d20449422d50c31) )
	ROM_LOAD( "rh27.004",     0x3000, 0x1000, CRC(425b52b0) SHA1(1a3046a7d12ad8107750abfb8a801cf9cd372d0f) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "eb16.22",      0x0000, 0x0100, CRC(b833b5ea) SHA1(d233f1bf8a3e6cd876853ffd721b9b64c61c9047) )
ROM_END

GAME( 1982, alpines01, alpine, nomcu, alpine, taitosj_state, init_alpine, ROT270, "hack", "Alpine Ski (Translation Chinese)", MACHINE_SUPPORTS_SAVE )


/****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /**********
 Front Line
*************/

ROM_START( frontlins01 )
	ROM_REGION( 0x12000, "maincpu", 0 )
	ROM_LOAD( "fl69_po01.u69",     0x00000, 0x1000, CRC(2a3500bb) SHA1(2ff28197fc91c57bb8554b0b0eb0136ec7cbd04e) )
	ROM_LOAD( "fl68_po01.u68",     0x01000, 0x1000, CRC(f2c6a7f9) SHA1(876b52854366b110413ba9875b2ee9f436b0ae78) )
	ROM_LOAD( "fl67.u67",     0x02000, 0x1000, CRC(3fa1ba12) SHA1(06eaccc75a4a950ed509c0dd203eeb7120849e74) )
	ROM_LOAD( "fl66.u66",     0x03000, 0x1000, CRC(4a3db285) SHA1(0b486523d4ae302962dcb4ca042754fd96208259) )
	ROM_LOAD( "fl65.u65",     0x04000, 0x1000, CRC(da00ec70) SHA1(652eb07c1e98ed04042a334ad8e27fd3da8dd6a2) )
	ROM_LOAD( "fl64.u64",     0x05000, 0x1000, CRC(9fc90a20) SHA1(2d1bc248ed68dbb1993c360a9f2e2dbe26c216fb) )
	ROM_LOAD( "fl55.u55",     0x06000, 0x1000, CRC(359242c2) SHA1(63bd845b2d881946a7904e4df1db3d78a60b57ad) )
	ROM_LOAD( "fl54.u54",     0x07000, 0x1000, CRC(d234c60f) SHA1(b45bf432a64b7aaf3762d72a762b5eca198d5b3d) )
	ROM_LOAD( "aa1_10_po01.8",     0x0e000, 0x1000, CRC(78556a20) SHA1(ebf41e49261f7f557d3e298c93ddddde57258697) )
	ROM_LOAD( "fl53_po01.u53",     0x10000, 0x1000, CRC(ea89040c) SHA1(03676c6c9a5f12d81d78b55dcb6f833e6e034c79) )
	ROM_LOAD( "fl52.u52",     0x11000, 0x1000, CRC(cb223d34) SHA1(a1a4530ed25064c6cabe34c52bb239e3656e4ced) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "aa1_11.ic70",     0x0000, 0x1000, CRC(15f4ed8c) SHA1(ec096234e4e594100180eb99c8c57eb97b9f57e2) )
	ROM_LOAD( "aa1_12.ic71",     0x1000, 0x1000, CRC(c3eb38e7) SHA1(427e5deb6a6e22d8c34923209a818f79d50e59d4) )

	ROM_REGION( 0x0800, "bmcu:mcu", 0 )
	ROM_LOAD( "aa1_13.ic24",       0x0000, 0x0800, CRC(7e78bdd3) SHA1(9eeb0e969fd013b9db074a15b0463216453e9364) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "fl1.u1",       0x0000, 0x1000, CRC(e82c9f46) SHA1(eaab468bb5e46e9c714e6f84e65f954331fdbc56) )
	ROM_LOAD( "fl2.u2",       0x1000, 0x1000, CRC(123055d3) SHA1(6aaddd8ebb418c7c8584eb74ad13cd5accd5a196) )
	ROM_LOAD( "fl3.u3",       0x2000, 0x1000, CRC(7ea46347) SHA1(b924a614abe01f7ca6a31463864d6cc55a47946e) )
	ROM_LOAD( "fl4.u4",       0x3000, 0x1000, CRC(9e2cff10) SHA1(0932c15eacccab5a3a931dd40c1a35b5a4ca1cd5) )
	ROM_LOAD( "fl5.u5",       0x4000, 0x1000, CRC(630b4be1) SHA1(780f75fdea68917a08f5f00da3831eaa26fd4405) )
	ROM_LOAD( "fl6.u6",       0x5000, 0x1000, CRC(9e092d58) SHA1(8388870bb40c7a2e3b4ede74c37c71c3a3d1a607) )
	ROM_LOAD( "fl7.u7",       0x6000, 0x1000, CRC(613682a3) SHA1(b681f3a4e70f207ce140adfac1388900d5013317) )
	ROM_LOAD( "fl8_po01.u8",       0x7000, 0x1000, CRC(7f3e197a) SHA1(87863cef9bb381f3bd644d49f4f551716d09c7af) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "eb16.ic22",      0x0000, 0x0100, CRC(b833b5ea) SHA1(d233f1bf8a3e6cd876853ffd721b9b64c61c9047) )
ROM_END


/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Front Line
GAME( 1982, frontlins01, frontlin,        mcu,      frontlin, taitosj_state, init_taitosj, ROT270, "hack", "Front Line (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
