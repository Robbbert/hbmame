// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/pacland.c"

ROM_START( mariopac )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 128k for code */
	ROM_LOAD( "pl5_01b.bin",  0x08000, 0x4000, CRC(b0ea7631) SHA1(424afa6f397310c7af39c9e8b580aa9ccd42c39c) )
	ROM_LOAD( "pl5_02.bin",   0x0C000, 0x4000, CRC(d903e84e) SHA1(25338726227bfbec65847879aac5228a6a307db4) )
	/* all the following are banked at 0x4000-0x5fff */
	ROM_LOAD( "pl1-3",        0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
	ROM_LOAD( "pl1-4",        0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
	ROM_LOAD( "pl1-5",        0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
	ROM_LOAD( "pl3_06.bin",   0x1c000, 0x4000, CRC(2ffe3319) SHA1(c2540321cd5a1fe29ecb077abdf8f997893192e9) )

	ROM_REGION( 0x10000, "mcu", 0 )	/* 64k for code */
	ROM_LOAD( "pl1-7",        0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
	ROM_LOAD( "cus60",        0xf000, 0x1000, CRC(6ef08fb3) SHA1(4842590d60035a0059b0899eb2d5f58ae72c2529) ) /* microcontroller */

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "mp_pl2_12.bin",0x00000, 0x2000, CRC(abba0923) SHA1(2286b0d162e5502160895940d0c9637abe5d6d2f) )	/* chars */

	ROM_REGION( 0x02000, "gfx2", 0 )
	ROM_LOAD( "pl4_13.bin",   0x00000, 0x2000, CRC(3ae582fd) SHA1(696b2cfadb6b071de8e43d20cd65b37713ca3b30) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "mp_pl1-9",     0x00000, 0x4000, CRC(98439154) SHA1(7bad68705ae3d9fa6e03ad7dc9421cf6063e1669) )	/* sprites */
	ROM_LOAD( "mp_pl1-8",     0x04000, 0x4000, CRC(d0c1f225) SHA1(bcaa68922e49b7257959471641d3c91e505a8d53) )
	ROM_LOAD( "mp_pl1-10",    0x08000, 0x4000, CRC(6005b277) SHA1(acacffe38edfc2d325dc286d182253ee5bb62489) )
	ROM_LOAD( "mp_pl1-11",    0x0c000, 0x4000, CRC(eeed9880) SHA1(e037dd299895db22a88c1d32dda157833b1e8a09) )

	ROM_REGION( 0x1400, "proms", 0 )
	ROM_LOAD( "pl1-2.bin",    0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )	/* red and green component */
	ROM_LOAD( "pl1-1.bin",    0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )	/* blue component */
	ROM_LOAD( "pl1-5.bin",    0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )	/* foreground lookup table */
	ROM_LOAD( "pl1-4.bin",    0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )	/* background lookup table */
	ROM_LOAD( "pl1-3.bin",    0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )	/* sprites lookup table */
ROM_END

ROM_START( paclandb )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 128k for code */
	ROM_LOAD( "pl5_01b.bin",  0x08000, 0x4000, CRC(b0ea7631) SHA1(424afa6f397310c7af39c9e8b580aa9ccd42c39c) )
	ROM_LOAD( "pl5_02.bin",   0x0C000, 0x4000, CRC(d903e84e) SHA1(25338726227bfbec65847879aac5228a6a307db4) )
	/* all the following are banked at 0x4000-0x5fff */
	ROM_LOAD( "pl1-3",        0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
	ROM_LOAD( "pl1-4",        0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
	ROM_LOAD( "pl1-5",        0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
	ROM_LOAD( "pl3_06.bin",   0x1c000, 0x4000, CRC(2ffe3319) SHA1(c2540321cd5a1fe29ecb077abdf8f997893192e9) )

	ROM_REGION( 0x10000, "mcu", 0 )	/* 64k for code */
	ROM_LOAD( "pl1-7",        0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
	ROM_LOAD( "cus60",        0xf000, 0x1000, CRC(6ef08fb3) SHA1(4842590d60035a0059b0899eb2d5f58ae72c2529) ) /* microcontroller */

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "pl2_12.bin",   0x00000, 0x2000, CRC(a63c8726) SHA1(b15903fa2267375280af03af0a7157e1b0bcb86d) )	/* chars */

	ROM_REGION( 0x02000, "gfx2", 0 )
	ROM_LOAD( "pl4_13.bin",   0x00000, 0x2000, CRC(3ae582fd) SHA1(696b2cfadb6b071de8e43d20cd65b37713ca3b30) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "pl1-9",        0x00000, 0x4000, CRC(f5d5962b) SHA1(8d008a9bc06dc562c241955d9c551647b5c1f4e9) )	/* sprites */
	ROM_LOAD( "pl1-8",        0x04000, 0x4000, CRC(a2ebfa4a) SHA1(4a2a2b43a23a7a46266751415d1bde118143429c) )
	ROM_LOAD( "pl1-10",       0x08000, 0x4000, CRC(c7cf1904) SHA1(7ca8ed20ee32eb8609ac96b4e4fcb3b6027b598a) )
	ROM_LOAD( "pl1-11",       0x0c000, 0x4000, CRC(6621361a) SHA1(4efa40adba803006e86d5e12514983d4132b5efb) )

	ROM_REGION( 0x1400, "proms", 0 )
	ROM_LOAD( "pl1-2.bin",    0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )	/* red and green component */
	ROM_LOAD( "pl1-1.bin",    0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )	/* blue component */
	ROM_LOAD( "pl1-5.bin",    0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )	/* foreground lookup table */
	ROM_LOAD( "pl1-4.bin",    0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )	/* background lookup table */
	ROM_LOAD( "pl1-3.bin",    0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )	/* sprites lookup table */
ROM_END

ROM_START( paclandd )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 128k for code */
	ROM_LOAD( "pl5_01b.bin",  0x08000, 0x4000, CRC(b0ea7631) SHA1(424afa6f397310c7af39c9e8b580aa9ccd42c39c) )
	ROM_LOAD( "pl5_02.bin",   0x0C000, 0x4000, CRC(d903e84e) SHA1(25338726227bfbec65847879aac5228a6a307db4) )
	/* all the following are banked at 0x4000-0x5fff */
	ROM_LOAD( "pl1-3",        0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
	ROM_LOAD( "pl1-4",        0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
	ROM_LOAD( "pl1-5",        0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
	ROM_LOAD( "pl3_06.bin",   0x1c000, 0x4000, CRC(2ffe3319) SHA1(c2540321cd5a1fe29ecb077abdf8f997893192e9) )

	ROM_REGION( 0x10000, "mcu", 0 )	/* 64k for code */
	ROM_LOAD( "pl1-7",        0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
	ROM_LOAD( "cus60",        0xf000, 0x1000, CRC(6ef08fb3) SHA1(4842590d60035a0059b0899eb2d5f58ae72c2529) ) /* microcontroller */

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "pl2_12.bin",   0x00000, 0x2000, CRC(a63c8726) SHA1(b15903fa2267375280af03af0a7157e1b0bcb86d) )	/* chars */

	ROM_REGION( 0x02000, "gfx2", 0 )
	ROM_LOAD( "pl4_13.bin",   0x00000, 0x2000, CRC(3ae582fd) SHA1(696b2cfadb6b071de8e43d20cd65b37713ca3b30) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "pldd1-9",      0x00000, 0x4000, CRC(3dba9085) SHA1(3ade36e6c04ef40d4dcdf53cc374c600de9dbb75) )	/* sprites */
	ROM_LOAD( "pldd1-8",      0x04000, 0x4000, CRC(7f089223) SHA1(129344647ae4898c6d70f15e96c13eca12a5f6ca) )
	ROM_LOAD( "pldd1-10",     0x08000, 0x4000, CRC(3f4b4d81) SHA1(e6acf8164a9d9fc31a12722e466c4dfded3022fe) )
	ROM_LOAD( "pldd1-11",     0x0c000, 0x4000, CRC(52c2cb37) SHA1(47833d98ccbee1098910778ea1a26f58b485a76a) )

	ROM_REGION( 0x1400, "proms", 0 )
	ROM_LOAD( "pl1-2.bin",    0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )	/* red and green component */
	ROM_LOAD( "pl1-1.bin",    0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )	/* blue component */
	ROM_LOAD( "pl1-5.bin",    0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )	/* foreground lookup table */
	ROM_LOAD( "pl1-4.bin",    0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )	/* background lookup table */
	ROM_LOAD( "pl1-3.bin",    0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )	/* sprites lookup table */
ROM_END

GAME( 2001, mariopac, pacland, pacland, pacland, driver_device, 0, ROT0, "Mark", "Mario in Pac-Land", MACHINE_SUPPORTS_SAVE )
GAME( 1984, paclandb, pacland, pacland, pacland, driver_device, 0, ROT0, "Namco", "Pac-Land (Benelux Edition)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, paclandd, pacland, pacland, pacland, driver_device, 0, ROT0, "Blue Justice", "Pac-Land", MACHINE_SUPPORTS_SAVE )
