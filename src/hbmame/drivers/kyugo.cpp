// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/kyugo.cpp"

ROM_START( airwolffr )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "bfr.2s",      0x0000, 0x8000, CRC(ccf90d88) SHA1(6f88c0c4f5985d8033efdbc5e5516552007b7bdf) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "a.7s",        0x0000, 0x8000, CRC(a3c7af5c) SHA1(0f70ca94f3d168d38e0e93252e9441973f72441a) )

	ROM_REGION( 0x01000, "gfx1", 0 )
	ROM_LOAD( "f.4a",        0x00000, 0x1000, CRC(4df44ce9) SHA1(145986009d4ae6f7dd98ce715838d0331dea005d) ) /* chars */

	ROM_REGION( 0x06000, "gfx2", 0 )
	ROM_LOAD( "09h_14.bin",  0x00000, 0x2000, CRC(25e57e1f) SHA1(bef24bced102cd470e10bd4aa19da3c608211258) )
	ROM_LOAD( "10h_13.bin",  0x02000, 0x2000, CRC(cf0de5e9) SHA1(32f3eb4c9298d59aca1dc2530b0e92f64311946d) )
	ROM_LOAD( "11h_12.bin",  0x04000, 0x2000, CRC(4050c048) SHA1(ca21e0750f01342d9791067160339eec436c9458) )

	ROM_REGION( 0x18000, "gfx3", 0 )
	ROM_LOAD( "e.6a",        0x00000, 0x2000, CRC(e8fbc7d2) SHA1(a9a651b4a714f490a051a89fd0327a665353d64b) )
	ROM_CONTINUE(            0x04000, 0x2000 )
	ROM_CONTINUE(            0x02000, 0x2000 )
	ROM_CONTINUE(            0x06000, 0x2000 )
	ROM_LOAD( "d.8a",        0x08000, 0x2000, CRC(c5d4156b) SHA1(f66ec33b67e39f3df016231b00e48c9757e322f3) )
	ROM_CONTINUE(            0x0c000, 0x2000 )
	ROM_CONTINUE(            0x0a000, 0x2000 )
	ROM_CONTINUE(            0x0e000, 0x2000 )
	ROM_LOAD( "c.10a",       0x10000, 0x2000, CRC(de91dfb1) SHA1(9f338542f44905d0b895d99510475113eb860f0d) )
	ROM_CONTINUE(            0x14000, 0x2000 )
	ROM_CONTINUE(            0x12000, 0x2000 )
	ROM_CONTINUE(            0x16000, 0x2000 )

	ROM_REGION( 0x0340, "proms", 0 )
	ROM_LOAD( "01j.bin",     0x0000, 0x0100, CRC(6a94b2a3) SHA1(b1f9bd97aa26c9fb6377ef32d5dd125583361f48) )
	ROM_LOAD( "01h.bin",     0x0100, 0x0100, CRC(ec0923d3) SHA1(26f9eda4260a8b767893b8dea42819f192ef0b20) )
	ROM_LOAD( "01f.bin",     0x0200, 0x0100, CRC(ade97052) SHA1(cc1b4cd57d7bc55ce44de6b89a322ff08eabb1a0) )
	ROM_LOAD( "m1.2c",       0x0320, 0x0020, CRC(83a39201) SHA1(4fdc722c9e20ee152c890342ef0dce18e35e2ef8) )

	ROM_REGION( 0x0400, "plds", 0 )
	ROM_LOAD_OPTIONAL( "pal16l8a.2j",   0x0000, 0x0104, NO_DUMP )
	ROM_LOAD_OPTIONAL( "epl12p6a.9j",   0x0200, 0x0034, CRC(19808f14) SHA1(d5043237be8857d2cecaf7dec079461f6b53efa9) )
	ROM_LOAD_OPTIONAL( "epl12p6a.9k",   0x0300, 0x0034, CRC(f5acad85) SHA1(ee3caeedf3e91793b12895f109eae5417f5c7631) )
ROM_END

GAME( 2007, airwolffr, airwolf, srdmissn, airwolf, kyugo_state, srdmissn, ROT0, "Arkatrad", "Airwolf (French)", MACHINE_SUPPORTS_SAVE )
