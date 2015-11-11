// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/exidy.cpp"





/*************************************
 *
 *  Targ Test Rom - note that MAME fails the music tests
 *
 *************************************/

ROM_START( targtest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hrl10a-1", 0x1800, 0x0800, CRC(969744e1) SHA1(e123bdb02b3b5f6a59c1e7c9ef557fe6bb19c62c) )
	ROM_LOAD( "hrl9a-1",  0x2000, 0x0800, CRC(a177a72d) SHA1(0e705e3e32021e55af4414fa0e2ccbc4980ee848) )
	ROM_LOAD( "hrl8a-1",  0x2800, 0x0800, CRC(6e6928a5) SHA1(10c725b27225ac5aad8639b081df68dd61522cf2) )
	ROM_LOAD( "targtest.bin",  0x3000, 0x0800, CRC(22ae0198) SHA1(0139e3c36e4e415641cefae900347dcf81f00716) )
	ROM_LOAD( "hrl6a-1",  0x3800, 0x0800, CRC(a60a1bfc) SHA1(17c0e67e1a0b263b57d70a148cc5d5099fecbb40) )

	ROM_REGION( 0x0400, "gfx1", 0 )
	ROM_LOAD( "hrl11d-1", 0x0000, 0x0400, CRC(9f03513e) SHA1(aa4763e49df65e5686a96431543580b8d8285893) )

	ROM_REGION( 0x0140, "proms", 0 )
	ROM_LOAD( "hrl5c-1",  0x0000, 0x0100, CRC(a24290d0) SHA1(5f2888d168de874021b51c5d19a62fb8165e4454) )
	ROM_LOAD( "stl6d-1",  0x0100, 0x0020, CRC(e26f9053) SHA1(eec35b6aa2c2d305418306bf4a1754a0583f109f) )
	ROM_LOAD( "hrl14h-1", 0x0120, 0x0020, CRC(f76b4fcf) SHA1(197e0cc508ffeb5cefa4046bdfb158939d598225) )

	ROM_REGION( 0x0020, "targ", 0 )
	ROM_LOAD( "hra2b-1",  0x0000, 0x0020, CRC(38e8024b) SHA1(adf1c1770695f7614c95eceb803f662c5b096a76) )
ROM_END



/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 2008, targtest, targ, targ, targ, exidy_state, targ, ROT0, "Timothy Shiels", "Targ Test Rom V1.0", MACHINE_SUPPORTS_SAVE )
