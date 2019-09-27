// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/namcos86.cpp"

/***********************
 The Return of Ishtar
*************************/

ROM_START( roishtarx )
	ROM_REGION( 0x10000, "cpu1", 0 )
	ROM_LOAD( "ri1_2_ps.9d",     0x04000, 0x2000, CRC(5f818982) SHA1(6f247a416f717cd1d60b54cb20b0e8433596736e) )
	ROM_LOAD( "ri1_1c_ps.9c",    0x08000, 0x8000, CRC(6a2fd981) SHA1(091ad9d3f8b7f783ad7ec71f0dc8ad03906b94ca) )

	ROM_REGION( 0x10000, "cpu2", 0 )
	ROM_LOAD( "ri1_3_ps.12c",    0x8000, 0x8000, CRC(e6a3edf6) SHA1(fcc6cb695ca560d056881677e712644368ad1ae6) )

	ROM_REGION( 0x06000, "gfx1", 0 )
	ROM_LOAD( "ri1_14.7r",    0x00000, 0x04000, CRC(de8154b4) SHA1(70a65e4656cf9fcf5c54e84c628ec95393e856fb) )
	ROM_LOAD( "ri1_15.7s",    0x04000, 0x02000, CRC(4298822b) SHA1(5aad41fd719c2f310ae485caaacda129c9f2ac94) )

	ROM_REGION( 0x06000, "gfx2", 0 )
	ROM_LOAD( "ri1_12.4r",    0x00000, 0x04000, CRC(557e54d3) SHA1(d22969deefcb3c3443d08a215f1ec2e874650b19) )
	ROM_LOAD( "ri1_13.4s",    0x04000, 0x02000, CRC(9ebe8e32) SHA1(5990a86bfbf2669e512e8ca875c69b4c60c4d108) )

	ROM_REGION( 0x40000, "gfx3", 0 )
	ROM_LOAD( "ri1_5.12h",    0x00000, 0x8000, CRC(46b59239) SHA1(bb08e57cd5864f41e27a07dcf449896570d2203d) )
	ROM_LOAD( "ri1_6.12k",    0x08000, 0x8000, CRC(94d9ef48) SHA1(a13b345b8fe30dea8e85698782674859c385e79a) )
	ROM_LOAD( "ri1_7.12l",    0x10000, 0x8000, CRC(da802b59) SHA1(b6551db5cd9c3d674cdf1dc59f581ee435a7eeb7) )
	ROM_LOAD( "ri1_8.12m",    0x18000, 0x8000, CRC(16b88b74) SHA1(9ef3ebf686a539e911bb7a3a4c02d0e2dca616d0) )
	ROM_LOAD( "ri1_9.12p",    0x20000, 0x8000, CRC(f3de3c2a) SHA1(02705bfd37f8996c5fc9c5bf2a99e859083a75e6) )
	ROM_LOAD( "ri1_10.12r",   0x28000, 0x8000, CRC(6dacc70d) SHA1(c7db40a0e90c9717f8a2f1507daff997856a3b91) )
	ROM_LOAD( "ri1_11.12t",   0x30000, 0x8000, CRC(fb6bc533) SHA1(a840af58d6db66518520bc7d88867a09a2e502c2) )

	ROM_REGION( 0x1420, "proms", 0 )
	ROM_LOAD( "ri1-1.3r",     0x0000, 0x0200, CRC(29cd0400) SHA1(a9b0d09492710e72e34155cd6a7b7c1a34c56b20) )
	ROM_LOAD( "ri1-2.3s",     0x0200, 0x0200, CRC(02fd278d) SHA1(db104fc7acf2739def902180981eb7ba10ec3dda) )
	ROM_LOAD( "ri1-3.4v",     0x0400, 0x0800, CRC(cbd7e53f) SHA1(77ef70be4e8a21948d697649352a5e3527086cf2) )
	ROM_LOAD( "ri1-4.5v",     0x0c00, 0x0800, CRC(22921617) SHA1(7304cb5a86f524f912feb8b58801393cce5d3b09) )
	ROM_LOAD( "ri1-5.6u",     0x1400, 0x0020, CRC(e2188075) SHA1(be079ace2070433d4d90c757aef3e415b4e21455) )

	ROM_REGION( 0x10000, "mcu", 0 )
	ROM_LOAD( "ri1_4.6b",       0x0000, 0x4000, CRC(552172b8) SHA1(18b35cb116baba362831fc046241895198b07a53) )
	ROM_CONTINUE(               0x8000, 0x4000 )
	ROM_LOAD( "cus60-60a1.mcu", 0xf000, 0x1000, CRC(076ea82a) SHA1(22b5e62e26390d7d5cacc0503c7aa5ed524204df) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// The Return of Ishtar
GAME( 1986, roishtarx, roishtar,        roishtar, roishtar,  namcos86_state, init_namco86, ROT0,   "hack",   "The Return of Ishtar (Another tower)", MACHINE_SUPPORTS_SAVE )

