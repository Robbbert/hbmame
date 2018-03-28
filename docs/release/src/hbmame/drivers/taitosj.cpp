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

GAME( 1982, alpines01, alpine, nomcu, alpine, taitosj_state, alpine, ROT270, "hack", "Alpine Ski (Translation-Chinese)", MACHINE_SUPPORTS_SAVE )
