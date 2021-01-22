// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/lordgun.cpp"

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***************
 Alien Challenge
******************/

ROM_START( alienchas01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD( "igsc0102_po01.u81", 0x00000, 0x200000, CRC(3d1b0a5e) SHA1(bb3bd4d3a7f8903d8f098fba7748ee4a58bd8490) )
	ROM_IGNORE( 0x254F4C )

	ROM_REGION( 0x010000, "soundcpu", 0 )
	ROM_LOAD( "hfh_s.u86", 0x00000, 0x10000, CRC(5728a9ed) SHA1(e5a9e4a1a2cc6c848b08608bc8727bc739270873) )

	ROM_REGION( 0x300000, "tiles0", 0 )
	ROM_LOAD( "igst0101.u9",  0x000000, 0x100000, CRC(2ce12d7b) SHA1(aa93a82e5f4015c46bb705efb2051b62cd5d7e04) )
	ROM_LOAD( "igst0102.u10", 0x100000, 0x100000, CRC(542a76a0) SHA1(6947b50a024d0053c1eaf9da8c90652bab875142) )
	ROM_LOAD( "igst0103.u11", 0x200000, 0x100000, CRC(adf5698a) SHA1(4b798f8acc5d7581c7e0989260863ae0ca654acd) )

	ROM_REGION( 0x600000, "tiles1", 0 )
	ROM_LOAD( "igsb0101.u8", 0x000000, 0x200000, CRC(5c995f7e) SHA1(4f08cf13e313c6802c924b914c73cab4b450da61) )
	ROM_LOAD( "igsb0102.u7", 0x200000, 0x200000, CRC(a2ae9baf) SHA1(338ee260c33448568f138ca00e1d4edda4da018f) )
	ROM_LOAD( "igsb0103.u6", 0x400000, 0x200000, CRC(11b927af) SHA1(2f15e5cea1b86cde3b679bdd0f3d79672d0ddd3e) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD( "igsa0101.u3", 0x000000, 0x400000, CRC(374d07c4) SHA1(87e9bfe32cbfe9964ba7253847fbd14aa3c8ed20) )
	ROM_LOAD( "igsa0102.u2", 0x400000, 0x400000, CRC(dbeee7ac) SHA1(e0eb0d73d9230aa6f69f5ac25d44fa19affebe88) )
	ROM_LOAD( "igsa0103.u1", 0x800000, 0x400000, CRC(e5f19041) SHA1(c92a29bbbcb9a1f63364c665e3e0f9679add4389) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "hfh_g.u65", 0x00000, 0x40000, CRC(ec469b57) SHA1(ba1668078987ad51f47bcd3e61c51a0cf2545350) )

	ROM_REGION( 0x40000, "oki2", 0 )
	ROM_LOAD( "hfh_g.u66", 0x00000, 0x40000, CRC(7cfcd98e) SHA1(3b03123160adfd3404a9e0c4c68420930e80ae48) )

	ROM_REGION( 0x200000, "ymf", 0 )
	ROM_LOAD( "yrw801-m", 0x000000, 0x200000, CRC(2a9d8d43) SHA1(32760893ce06dbe3930627755ba065cc3d8ec6ca) )
ROM_END

ROM_START( alienchas02 ) //alienchal
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD( "igsc0102_po02.u81", 0x00000, 0x200000, CRC(cfd429ae) SHA1(4c23dba96f80ffefbabcb5da7bb769cbf4d00b40) )

	ROM_REGION( 0x010000, "soundcpu", 0 )
	ROM_LOAD( "hfh_s.u86", 0x00000, 0x10000, CRC(5728a9ed) SHA1(e5a9e4a1a2cc6c848b08608bc8727bc739270873) )

	ROM_REGION( 0x300000, "tiles0", 0 )
	ROM_LOAD( "igst0101.u9",  0x000000, 0x100000, CRC(2ce12d7b) SHA1(aa93a82e5f4015c46bb705efb2051b62cd5d7e04) )
	ROM_LOAD( "igst0102.u10", 0x100000, 0x100000, CRC(542a76a0) SHA1(6947b50a024d0053c1eaf9da8c90652bab875142) )
	ROM_LOAD( "igst0103.u11", 0x200000, 0x100000, CRC(adf5698a) SHA1(4b798f8acc5d7581c7e0989260863ae0ca654acd) )

	ROM_REGION( 0x600000, "tiles1", 0 )
	ROM_LOAD( "igsb0101.u8", 0x000000, 0x200000, CRC(5c995f7e) SHA1(4f08cf13e313c6802c924b914c73cab4b450da61) )
	ROM_LOAD( "igsb0102.u7", 0x200000, 0x200000, CRC(a2ae9baf) SHA1(338ee260c33448568f138ca00e1d4edda4da018f) )
	ROM_LOAD( "igsb0103.u6", 0x400000, 0x200000, CRC(11b927af) SHA1(2f15e5cea1b86cde3b679bdd0f3d79672d0ddd3e) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD( "igsa0101.u3", 0x000000, 0x400000, CRC(374d07c4) SHA1(87e9bfe32cbfe9964ba7253847fbd14aa3c8ed20) )
	ROM_LOAD( "igsa0102.u2", 0x400000, 0x400000, CRC(dbeee7ac) SHA1(e0eb0d73d9230aa6f69f5ac25d44fa19affebe88) )
	ROM_LOAD( "igsa0103.u1", 0x800000, 0x400000, CRC(e5f19041) SHA1(c92a29bbbcb9a1f63364c665e3e0f9679add4389) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "hfh_g.u65", 0x00000, 0x40000, CRC(ec469b57) SHA1(ba1668078987ad51f47bcd3e61c51a0cf2545350) )

	ROM_REGION( 0x40000, "oki2", 0 )
	ROM_LOAD( "hfh_g.u66", 0x00000, 0x40000, CRC(7cfcd98e) SHA1(3b03123160adfd3404a9e0c4c68420930e80ae48) )

	ROM_REGION( 0x200000, "ymf", 0 )
	ROM_LOAD( "yrw801-m", 0x000000, 0x200000, CRC(2a9d8d43) SHA1(32760893ce06dbe3930627755ba065cc3d8ec6ca) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Alien Challenge
GAME( 2015, alienchas01,  aliencha,        aliencha, aliencha, lordgun_state, empty_init,   ROT0, "hack", "Alien Challenge (Huang Feihong Ultimate Simplified Infinite Killing BOSS Edition 2015-07-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, alienchas02,  aliencha,        aliencha, aliencha, lordgun_state, empty_init,   ROT0, "hack", "Alien Challenge (Simplified Edition 2018-07-05)", MACHINE_SUPPORTS_SAVE )

