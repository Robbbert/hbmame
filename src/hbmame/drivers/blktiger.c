// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/blktiger.c"

ROM_START( blkdrgnk )
	ROM_REGION( 0x50000, "maincpu", 0 )	/* 64k for code + banked ROMs images */
	ROM_LOAD( "blkdrgnk.5e",  0x00000, 0x08000, CRC(e713b486) SHA1(468a75120080f9529f2b56d66c72c767e7ee86df) )	/* CODE */
	ROM_LOAD( "blkdrgon.6e",  0x10000, 0x10000, CRC(7d39c26f) SHA1(562a3f578e109ae020f65e341c876ad7e510a311) )	/* 0+1 */
	ROM_LOAD( "blkdrgon.8e",  0x20000, 0x10000, CRC(d1bf3757) SHA1(b19f8b986406bde65ac7f0d55d54f87b37f5e42f) )	/* 2+3 */
	ROM_LOAD( "blkdrgon.9e",  0x30000, 0x10000, CRC(4d1d6680) SHA1(e137624c59392de6aaffeded99b024938360bd25) )	/* 4+5 */
	ROM_LOAD( "blkdrgon.10e", 0x40000, 0x10000, CRC(c8d0c45e) SHA1(66c2e5a74c5875a2c8e28740fe944bd943246ce5) )	/* 6+7 */

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "bd-06.1l",  0x0000, 0x8000, CRC(2cf54274) SHA1(87df100c65999ba1e9d358ffd0fe4bba23ae0efb) )

	ROM_REGION( 0x10000, "mcu", 0 )
	ROM_LOAD( "bd.6k",  0x0000, 0x1000, CRC(ac7d14f1) SHA1(46fd6b43f10312e3e8d3c9e0c0fd616af98fdbad) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "blkdrgnk.2n",  0x00000, 0x08000, CRC(3b432217) SHA1(da0b3fe16fb5192f6514bdf5fef85a889c7b2f0a) )	/* characters */

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "blkdrgon.5b",  0x00000, 0x10000, CRC(22d0a4b0) SHA1(f9402ea9ffedcb280497a63c5eb352de9d4ca3fd) )	/* tiles */
	ROM_LOAD( "blkdrgon.4b",  0x10000, 0x10000, CRC(c8b5fc52) SHA1(621e899285ce6302e5b25d133d9cd52c09b7b202) )
	ROM_LOAD( "blkdrgon.9b",  0x20000, 0x10000, CRC(9498c378) SHA1(841934ddef724faf04162c4be4aea1684d8d8e0f) )
	ROM_LOAD( "blkdrgon.8b",  0x30000, 0x10000, CRC(5b0df8ce) SHA1(57d10b48bd61b0224ce21b36bde8d2479e8e5df4) )

	ROM_REGION( 0x40000, "gfx3", 0 )
	ROM_LOAD( "bd-08.5a",  0x00000, 0x10000, CRC(e2f17438) SHA1(3e5fdae07d40febedc59c7c7c4d9c6f0d72b58b5) )	/* sprites */
	ROM_LOAD( "bd-07.4a",  0x10000, 0x10000, CRC(5fccbd27) SHA1(33c55aa9c12b3121ca5c3b4c39a9b152b6946461) )
	ROM_LOAD( "bd-10.9a",  0x20000, 0x10000, CRC(fc33ccc6) SHA1(d492626a88565c2626f98ecb1d74535f1ad68e4c) )
	ROM_LOAD( "bd-09.8a",  0x30000, 0x10000, CRC(f449de01) SHA1(f6b40e9eb2471b89c42ab84f4214295d284db0c3) )

	ROM_REGION( 0x0400, "proms", 0 )	/* PROMs (function unknown) */
	ROM_LOAD( "bd01.8j",   0x0000, 0x0100, CRC(29b459e5) SHA1(0034734a533df3dea16b7b48e072485d7f26f850) )
	ROM_LOAD( "bd02.9j",   0x0100, 0x0100, CRC(8b741e66) SHA1(6c1fda59936a7217b05949f5c54b1f91f4b49dbe) )
	ROM_LOAD( "bd03.11k",  0x0200, 0x0100, CRC(27201c75) SHA1(c54d87f06bfe0b0908389c005014d97156e272c2) )
	ROM_LOAD( "bd04.11l",  0x0300, 0x0100, CRC(e5490b68) SHA1(40f9f92efe7dd97b49144aec02eb509834056915) )
ROM_END

GAME( 1987, blkdrgnk, 0, blktiger, blktiger, driver_device, 0, ROT0, "hack", "Black Dragon (Korean)", MACHINE_SUPPORTS_SAVE )
