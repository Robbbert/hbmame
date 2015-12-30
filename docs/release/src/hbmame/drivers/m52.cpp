// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/m52.cpp"

ROM_START( spatrol )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mpa-1.3m",      0x0000, 0x1000, CRC(5873a860) SHA1(8c03726d6e049c3edbc277440184e31679f78258) )
	ROM_LOAD( "mpa-2.3l",      0x1000, 0x1000, CRC(f4b85974) SHA1(dfb2efb57378a20af6f20569f4360cde95596f93) )
	ROM_LOAD( "mpa-3.3k",      0x2000, 0x1000, CRC(2e1a598c) SHA1(112c3c9678db8a8540a8df3708020c87fd10c91b) )
	ROM_LOAD( "mpa-4.3j",      0x3000, 0x1000, CRC(dd05b587) SHA1(727961b0dafa4a96b580d51013336db2a18aff1e) )

	ROM_REGION( 0x8000, "irem_audio:iremsound", 0 )
	ROM_LOAD( "mp-s1.1a",     0x7000, 0x1000, CRC(561d3108) SHA1(4998c68a9e9a8002251fa8f07aa1082444a9dc80) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "spatrol.3e",   0x0000, 0x1000, CRC(130709e8) SHA1(fa3d0aca3179073c92a39c50f535d945f9b58725) )       /* chars */
	ROM_LOAD( "spatrol.3f",   0x1000, 0x1000, CRC(1ac58abc) SHA1(1a945c4efa6ef4a7d68cb6cb7b91a76be956837e) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "spatrol.3m",   0x0000, 0x1000, CRC(09e18f9a) SHA1(9f41aacbc5352b8783422fdffef949426bada588) )       /* sprites */
	ROM_LOAD( "spatrol.3n",   0x1000, 0x1000, CRC(36b1891e) SHA1(e204c259ca9c37d0843b5c877debf2171c40249a) )

	ROM_REGION( 0x1000, "gfx3", 0 )
	ROM_LOAD( "spatrol.3l",   0x0000, 0x1000, CRC(49877023) SHA1(a76b18afb20ab416318812f66bfdd57733441cb5) )       /* background graphics */

	ROM_REGION( 0x1000, "gfx4", 0 )
	ROM_LOAD( "mpe-2.3k",     0x0000, 0x1000, CRC(c7aa1fb0) SHA1(14c6c76e1d0db2c0745e5d6d33ea6945fac8e9ee) )

	ROM_REGION( 0x1000, "gfx5", 0 )
	ROM_LOAD( "spatrol.3h",   0x0000, 0x1000, CRC(091045ad) SHA1(715b5b964fe06ab015e30ce6e38d6737f0033510) )

	ROM_REGION( 0x0340, "proms", 0 )
	ROM_LOAD( "mpc-4.2a",     0x0000, 0x0200, CRC(07f99284) SHA1(dfc52958f2520e1ce4446dd4c84c91413bbacf76) )
	ROM_LOAD( "mpc-3.1m",     0x0200, 0x0020, CRC(6a57eff2) SHA1(2d1c12dab5915da2ccd466e39436c88be434d634) ) /* background palette */
	ROM_LOAD( "mpc-1.1f",     0x0220, 0x0020, CRC(26979b13) SHA1(8c41a8cce4f3384c392a9f7a223a50d7be0e14a5) ) /* sprite palette */
	ROM_LOAD( "mpc-2.2h",     0x0240, 0x0100, CRC(7ae4cd97) SHA1(bc0662fac82ffe65f02092d912b2c2b0c7a8ac2b) ) /* sprite lookup table */
ROM_END

GAME( 2008, spatrol,  mpatrol, m52, mpatrol, driver_device, 0, ROT0, "Gatinho", "Sponge Patrol", MACHINE_SUPPORTS_SAVE )

