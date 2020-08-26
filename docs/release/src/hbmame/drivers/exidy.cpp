// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/exidy.cpp"
#include "../mame/video/victory.cpp"



/*********************
 *
 *  Targ Test Rom
 *
 *********************/

ROM_START( targtest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hrl_10a1.10a",  0x1800, 0x0800, CRC(969744e1) SHA1(e123bdb02b3b5f6a59c1e7c9ef557fe6bb19c62c) )
	ROM_LOAD( "hrl_9a1.9a",    0x2000, 0x0800, CRC(a177a72d) SHA1(0e705e3e32021e55af4414fa0e2ccbc4980ee848) )
	ROM_LOAD( "hrl_8a1.8a",    0x2800, 0x0800, CRC(6e6928a5) SHA1(10c725b27225ac5aad8639b081df68dd61522cf2) )
	ROM_LOAD( "targtest.bin",  0x3000, 0x0800, CRC(22ae0198) SHA1(0139e3c36e4e415641cefae900347dcf81f00716) )
	ROM_LOAD( "hrl_6a3.6a",    0x3800, 0x0800, CRC(a60a1bfc) SHA1(17c0e67e1a0b263b57d70a148cc5d5099fecbb40) )

	ROM_REGION( 0x0400, "gfx1", 0 )
	ROM_LOAD( "hrl_11d-1.11d", 0x0000, 0x0400, CRC(9f03513e) SHA1(aa4763e49df65e5686a96431543580b8d8285893) )

	ROM_REGION( 0x0140, "proms", 0 )
	ROM_LOAD( "hrl_5c-1.5c",   0x0000, 0x0100, CRC(a24290d0) SHA1(5f2888d168de874021b51c5d19a62fb8165e4454) )
	ROM_LOAD( "stl_6d-1.6d",   0x0100, 0x0020, CRC(e26f9053) SHA1(eec35b6aa2c2d305418306bf4a1754a0583f109f) )
	ROM_LOAD( "hrl_14h-1.14h", 0x0120, 0x0020, CRC(f76b4fcf) SHA1(197e0cc508ffeb5cefa4046bdfb158939d598225) )

	ROM_REGION( 0x0020, "targ", 0 )
	ROM_LOAD( "hra2b-1",       0x0000, 0x0020, CRC(38e8024b) SHA1(adf1c1770695f7614c95eceb803f662c5b096a76) )
ROM_END

/*************************************
 *
 *  Teeter Torture free play hack
 *
 *************************************/
ROM_START( teetertfp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "teetertfp.11", 0xa000, 0x1000, CRC(550375ab) SHA1(ae262f30def469cd56fc2f10336b25f766fc1320) )
	ROM_LOAD( "10a-cpu", 0xb000, 0x1000, CRC(3ae7e445) SHA1(e511ce4c553ac58e87b6ee623f8c42d7653de972) )
	ROM_LOAD( "teetertfp.9",  0xc000, 0x1000, CRC(9fab266f) SHA1(b6ad26740f9c87c49b4df122819d1a335fe5545e) )
	ROM_LOAD( "8a-cpu",  0xd000, 0x1000, CRC(68de66e7) SHA1(70a0cc950f16f2c408fae717e6fdb75eb0fd8039) )
	ROM_LOAD( "7a-cpu",  0xe000, 0x1000, CRC(84491333) SHA1(db9f8e4c49057a4574a3784d71e627da7f7a4b44) )
	ROM_LOAD( "6a-cpu",  0xf000, 0x1000, CRC(3600d465) SHA1(84d633e042f73bfd6bf4a4d0ffee1cd2027c65d2) )

	ROM_REGION( 0x8000, "soundbd:audiocpu", 0 )
	ROM_LOAD( "3a-ac",  0x5800, 0x0800, CRC(83b8836f) SHA1(ec0e2de62caea61ceff56e924449213997bff8cd) )
	ROM_LOAD( "4a-ac",  0x6000, 0x0800, CRC(5154c39e) SHA1(e6f011630eb1aa4116a0e5824ad6b65c1be2455f) )
	ROM_LOAD( "5a-ac",  0x6800, 0x0800, CRC(1e1e3916) SHA1(867e586583e07cd01e0e852f6ea52a040995725d) )
	ROM_LOAD( "6a-ac",  0x7000, 0x0800, CRC(80f3357a) SHA1(f1ee638251e8676a526e6367c11866b1d52f5910) )
	ROM_LOAD( "7a-ac",  0x7800, 0x0800, CRC(466addc7) SHA1(0230b5365d6aeee3ca47666a9eadee4141de125b) )

	ROM_REGION( 0x0800, "gfx1", 0 )
	ROM_LOAD( "11d-cpu", 0x0000, 0x0800, CRC(0fe70b00) SHA1(6068be263d7a8e6b71af6f4dceec40bb8d246376) )

	ROM_REGION( 0x140, "proms", 0 )
	ROM_LOAD_OPTIONAL( "tt14h.123",  0x0000, 0x0020, CRC(f76b4fcf) SHA1(197e0cc508ffeb5cefa4046bdfb158939d598225) )
	ROM_LOAD_OPTIONAL( "tt5c.129",   0x0020, 0x0100, CRC(43b35bb7) SHA1(0a0cecea8faff9f3ff4c2ceda0b5b25e8e1cd667) )
	ROM_LOAD_OPTIONAL( "tt6d.123",   0x0120, 0x0020, CRC(e26f9053) SHA1(eec35b6aa2c2d305418306bf4a1754a0583f109f) )
ROM_END


/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 2008, targtest,  targ,    targ,     targ,    exidy_state,  init_targ,     ROT0, "Timothy Shiels", "Targ (Test Rom V1.0)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, teetertfp, teetert, teetert,  teetert, exidy_state,  init_teetert,  ROT0, "DogP", "Teeter Torture (Free Play Hack)", MACHINE_SUPPORTS_SAVE )
