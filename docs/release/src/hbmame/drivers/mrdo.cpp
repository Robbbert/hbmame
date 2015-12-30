// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mrdo.cpp"

ROM_START( mrdigdo )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "mrd-d1",        0x0000, 0x2000, CRC(3dcd9359) SHA1(bfe00450ee8822f437d87514f051ad1be6de9463) )
	ROM_LOAD( "mrd-d2",        0x2000, 0x2000, CRC(710058d8) SHA1(168cc179f2266bbf9437445bef9ff7d3358a8e6b) )
	ROM_LOAD( "mrd-d3",        0x4000, 0x2000, CRC(467d12d8) SHA1(7bb85e6a780de1c0c224229ee571cab39098f78d) )
	ROM_LOAD( "mrd-d4",        0x6000, 0x2000, CRC(fce9afeb) SHA1(26236a42c1c620975d4480c4315d0c6f112429b6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mrd-d9",        0x0000, 0x1000, CRC(b83b8a84) SHA1(138a4fd1b7470def1b4e8aa4cfcf4c4b6bebf35b) )
	ROM_LOAD( "mrd-d10",       0x1000, 0x1000, CRC(061cabb9) SHA1(cdc8876f1c402565835db01a78906200430bcf72) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "r8-08.bin",     0x0000, 0x1000, CRC(dbdc9ffa) SHA1(93f29fc106283eecbba3fd69cf3c4658aa38ab9f) )
	ROM_LOAD( "n8-07.bin",     0x1000, 0x1000, CRC(4b9973db) SHA1(8766c51a345a5e63446e65614c6f665ab5fbe0d7) )

	ROM_REGION( 0x2000, "gfx3", 0 )
	ROM_LOAD( "mrd-d5",        0x0000, 0x1000, CRC(401f9fa5) SHA1(7483150ae429c9841d04ec4d79eda59bf1db8330) )
	ROM_LOAD( "mrd-d6",        0x1000, 0x1000, CRC(ff401f59) SHA1(5f34363ffb5e470470c3a0d9ff704f4d9b814ecd) )

	ROM_REGION( 0x0080, "proms", 0 )
	ROM_LOAD( "u02--2.bin",    0x0000, 0x0020, CRC(238a65d7) SHA1(a5b20184a1989db23544296331462ec4d7be7516) )	/* palette (high bits) */
	ROM_LOAD( "t02--3.bin",    0x0020, 0x0020, CRC(ae263dc0) SHA1(7072c100b9d692f5bb12b0c9e304425f534481e2) )	/* palette (low bits) */
	ROM_LOAD( "f10--1.bin",    0x0040, 0x0020, CRC(16ee4ca2) SHA1(fcba4d103708b9711452009cd29c4f88d2f64cd3) )	/* sprite color lookup table */
	ROM_LOAD( "j10--4.bin",    0x0060, 0x0020, CRC(ff7fe284) SHA1(3ac8e30011c1fcba0ee8f4dc932f82296c3ba143) )	/* timing (not used) */
ROM_END

ROM_START( mrdofast )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "a4-01.bin",    0x0000, 0x2000, CRC(03dcfba2) SHA1(c15e3d0c4225e0ca120bcd28aca39632575f8e11) )
	ROM_LOAD( "f-c4-02.bin",  0x2000, 0x2000, CRC(894d12e4) SHA1(ff89eca0506727f1973b14a13d848799e7ada7ec) )
	ROM_LOAD( "e4-03.bin",    0x4000, 0x2000, CRC(358f5dc2) SHA1(9fed1f5d1d04935d1b77687c8b2f3bfce970dc08) )
	ROM_LOAD( "f4-04.bin",    0x6000, 0x2000, CRC(f4190cfc) SHA1(24f5125d900f944294d4eda068b710c8f1c6d39f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s8-09.bin",    0x0000, 0x1000, CRC(aa80c5b6) SHA1(76f9f90deb74598470e7ed565237da38dd07e4e9) )
	ROM_LOAD( "u8-10.bin",    0x1000, 0x1000, CRC(d20ec85b) SHA1(9762bbe34d3fa209ea719807c723f57cb6bf4e01) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "r8-08.bin",    0x0000, 0x1000, CRC(dbdc9ffa) SHA1(93f29fc106283eecbba3fd69cf3c4658aa38ab9f) )
	ROM_LOAD( "n8-07.bin",    0x1000, 0x1000, CRC(4b9973db) SHA1(8766c51a345a5e63446e65614c6f665ab5fbe0d7) )

	ROM_REGION( 0x2000, "gfx3", 0 )
	ROM_LOAD( "h5-05.bin",    0x0000, 0x1000, CRC(e1218cc5) SHA1(d946613a1cf1c97f7533a4f8c2d0078d1b7daaa8) )
	ROM_LOAD( "k5-06.bin",    0x1000, 0x1000, CRC(b1f68b04) SHA1(25709cd81c03df51f27cd730fecf86a1daa9e27e) )

	ROM_REGION( 0x0080, "proms", 0 )
	ROM_LOAD( "u02--2.bin",   0x0000, 0x0020, CRC(238a65d7) SHA1(a5b20184a1989db23544296331462ec4d7be7516) )	/* palette (high bits) */
	ROM_LOAD( "t02--3.bin",   0x0020, 0x0020, CRC(ae263dc0) SHA1(7072c100b9d692f5bb12b0c9e304425f534481e2) )	/* palette (low bits) */
	ROM_LOAD( "f10--1.bin",   0x0040, 0x0020, CRC(16ee4ca2) SHA1(fcba4d103708b9711452009cd29c4f88d2f64cd3) )	/* sprite color lookup table */
	ROM_LOAD( "j10--4.bin",   0x0060, 0x0020, CRC(ff7fe284) SHA1(3ac8e30011c1fcba0ee8f4dc932f82296c3ba143) )	/* timing (not used) */
ROM_END

ROM_START( mrpac )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "a4-01.bin",    0x0000, 0x2000, CRC(03dcfba2) SHA1(c15e3d0c4225e0ca120bcd28aca39632575f8e11) )
	ROM_LOAD( "c4-02.bin",    0x2000, 0x2000, CRC(0ecdd39c) SHA1(c64b3363593911a676c647bf3dba8fe063fcb0de) )
	ROM_LOAD( "e4-03.bin",    0x4000, 0x2000, CRC(358f5dc2) SHA1(9fed1f5d1d04935d1b77687c8b2f3bfce970dc08) )
	ROM_LOAD( "f4-04.bin",    0x6000, 0x2000, CRC(f4190cfc) SHA1(24f5125d900f944294d4eda068b710c8f1c6d39f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mp-s8-09.bin", 0x0000, 0x1000, CRC(930908e5) SHA1(13666166bb6a38c0b7a8d4133a0fb6fdd7aee372) )
	ROM_LOAD( "mp-u8-10.bin", 0x1000, 0x1000, CRC(ead6d3d6) SHA1(2dc959ca1e9e2cbdc0098ce92b9a11dc24643a38) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "r8-08.bin",    0x0000, 0x1000, CRC(dbdc9ffa) SHA1(93f29fc106283eecbba3fd69cf3c4658aa38ab9f) )
	ROM_LOAD( "n8-07.bin",    0x1000, 0x1000, CRC(4b9973db) SHA1(8766c51a345a5e63446e65614c6f665ab5fbe0d7) )
	
	ROM_REGION( 0x2000, "gfx3", 0 )
	ROM_LOAD( "mp-h5-05.bin", 0x0000, 0x1000, CRC(de213e01) SHA1(61f3d3081dded0cc753640632127e52e4ddf0a75) )
	ROM_LOAD( "mp-k5-06.bin", 0x1000, 0x1000, CRC(c2e376a4) SHA1(d7e5fbdcd2b66ec56524e37eee18f91af96443dd) )

	ROM_REGION( 0x0080, "proms", 0 )
	ROM_LOAD( "u02--2.bin",   0x0000, 0x0020, CRC(238a65d7) SHA1(a5b20184a1989db23544296331462ec4d7be7516) )	/* palette (high bits) */
	ROM_LOAD( "t02--3.bin",   0x0020, 0x0020, CRC(ae263dc0) SHA1(7072c100b9d692f5bb12b0c9e304425f534481e2) )	/* palette (low bits) */
	ROM_LOAD( "f10--1.bin",   0x0040, 0x0020, CRC(16ee4ca2) SHA1(fcba4d103708b9711452009cd29c4f88d2f64cd3) )	/* sprite color lookup table */
	ROM_LOAD( "j10--4.bin",   0x0060, 0x0020, CRC(ff7fe284) SHA1(3ac8e30011c1fcba0ee8f4dc932f82296c3ba143) )	/* timing (not used) */
ROM_END

ROM_START( mrpacb )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "a4-01.bin",    0x0000, 0x2000, CRC(03dcfba2) SHA1(c15e3d0c4225e0ca120bcd28aca39632575f8e11) )
	ROM_LOAD( "c4-02.bin",    0x2000, 0x2000, CRC(0ecdd39c) SHA1(c64b3363593911a676c647bf3dba8fe063fcb0de) )
	ROM_LOAD( "e4-03.bin",    0x4000, 0x2000, CRC(358f5dc2) SHA1(9fed1f5d1d04935d1b77687c8b2f3bfce970dc08) )
	ROM_LOAD( "f4-04.bin",    0x6000, 0x2000, CRC(f4190cfc) SHA1(24f5125d900f944294d4eda068b710c8f1c6d39f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mpb-s8-09.bin", 0x0000, 0x1000, CRC(ff01d8a0) SHA1(d3d674d8e5bfc6a133fc047bbfb21d179a322c07) )
	ROM_LOAD( "mpb-u8-10.bin", 0x1000, 0x1000, CRC(c1e9b7e8) SHA1(ea455a616597c58bfa88b5ba1069b3fb33d84a7f) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "r8-08.bin",    0x0000, 0x1000, CRC(dbdc9ffa) SHA1(93f29fc106283eecbba3fd69cf3c4658aa38ab9f) )
	ROM_LOAD( "n8-07.bin",    0x1000, 0x1000, CRC(4b9973db) SHA1(8766c51a345a5e63446e65614c6f665ab5fbe0d7) )
	
	ROM_REGION( 0x2000, "gfx3", 0 )
	ROM_LOAD( "mp-h5-05.bin", 0x0000, 0x1000, CRC(de213e01) SHA1(61f3d3081dded0cc753640632127e52e4ddf0a75) )
	ROM_LOAD( "mp-k5-06.bin", 0x1000, 0x1000, CRC(c2e376a4) SHA1(d7e5fbdcd2b66ec56524e37eee18f91af96443dd) )

	ROM_REGION( 0x0080, "proms", 0 )
	ROM_LOAD( "u02--2.bin",   0x0000, 0x0020, CRC(238a65d7) SHA1(a5b20184a1989db23544296331462ec4d7be7516) )	/* palette (high bits) */
	ROM_LOAD( "t02--3.bin",   0x0020, 0x0020, CRC(ae263dc0) SHA1(7072c100b9d692f5bb12b0c9e304425f534481e2) )	/* palette (low bits) */
	ROM_LOAD( "f10--1.bin",   0x0040, 0x0020, CRC(16ee4ca2) SHA1(fcba4d103708b9711452009cd29c4f88d2f64cd3) )	/* sprite color lookup table */
	ROM_LOAD( "j10--4.bin",   0x0060, 0x0020, CRC(ff7fe284) SHA1(3ac8e30011c1fcba0ee8f4dc932f82296c3ba143) )	/* timing (not used) */
ROM_END

ROM_START( newdigdo )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "a4-01.bin",    0x0000, 0x2000, CRC(03dcfba2) SHA1(c15e3d0c4225e0ca120bcd28aca39632575f8e11) )
	ROM_LOAD( "c4-02.bin",    0x2000, 0x2000, CRC(0ecdd39c) SHA1(c64b3363593911a676c647bf3dba8fe063fcb0de) )
	ROM_LOAD( "e4-03.bin",    0x4000, 0x2000, CRC(358f5dc2) SHA1(9fed1f5d1d04935d1b77687c8b2f3bfce970dc08) )
	ROM_LOAD( "f4-04.bin",    0x6000, 0x2000, CRC(f4190cfc) SHA1(24f5125d900f944294d4eda068b710c8f1c6d39f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "nd-s8-09.bin", 0x0000, 0x1000, CRC(48f2bd6d) SHA1(d741f58d5b7e74c498eee11cb8b959f7eec73671) )
	ROM_LOAD( "nd-u8-10.bin", 0x1000, 0x1000, CRC(7be2fc3e) SHA1(763f95812e35671b3815385cec5ccdaeca4b2b2a) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "nd-r8-08.bin", 0x0000, 0x1000, CRC(069a3e35) SHA1(09a361baed6417fdb4ecc1d8dd09ae9b2ccdef99) )
	ROM_LOAD( "nd-n8-07.bin", 0x1000, 0x1000, CRC(7ef6f5e4) SHA1(8839f6670010c0249b19f306c200c29775dc0351) )

	ROM_REGION( 0x2000, "gfx3", 0 )
	ROM_LOAD( "nd-h5-05.bin", 0x0000, 0x1000, CRC(4c8f6cf8) SHA1(92a0a5b8178fec5f7c4214c03d7d691872fed59a) )
	ROM_LOAD( "nd-k5-06.bin", 0x1000, 0x1000, CRC(1a5f18fc) SHA1(58cbad50c1001dfcb302c28db183e511907e6f86) )

	ROM_REGION( 0x0080, "proms", 0 )
	ROM_LOAD( "u02--2.bin",   0x0000, 0x0020, CRC(238a65d7) SHA1(a5b20184a1989db23544296331462ec4d7be7516) )	/* palette (high bits) */
	ROM_LOAD( "t02--3.bin",   0x0020, 0x0020, CRC(ae263dc0) SHA1(7072c100b9d692f5bb12b0c9e304425f534481e2) )	/* palette (low bits) */
	ROM_LOAD( "f10--1.bin",   0x0040, 0x0020, CRC(16ee4ca2) SHA1(fcba4d103708b9711452009cd29c4f88d2f64cd3) )	/* sprite color lookup table */
	ROM_LOAD( "j10--4.bin",   0x0060, 0x0020, CRC(ff7fe284) SHA1(3ac8e30011c1fcba0ee8f4dc932f82296c3ba143) )	/* timing (not used) */
ROM_END

ROM_START( stickydo )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "a4-01.bin",    0x0000, 0x2000, CRC(03dcfba2) SHA1(c15e3d0c4225e0ca120bcd28aca39632575f8e11) )
	ROM_LOAD( "c4-02.bin",    0x2000, 0x2000, CRC(0ecdd39c) SHA1(c64b3363593911a676c647bf3dba8fe063fcb0de) )
	ROM_LOAD( "e4-03.bin",    0x4000, 0x2000, CRC(358f5dc2) SHA1(9fed1f5d1d04935d1b77687c8b2f3bfce970dc08) )
	ROM_LOAD( "f4-04.bin",    0x6000, 0x2000, CRC(f4190cfc) SHA1(24f5125d900f944294d4eda068b710c8f1c6d39f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "st-s8-09.bin", 0x0000, 0x1000, CRC(0becc7bc) SHA1(d5ef21aa6f8daaf98f4c6c809504edea4fde35a1) )
	ROM_LOAD( "st-u8-10.bin", 0x1000, 0x1000, CRC(fc08364e) SHA1(69edf2405836511035f46cddb339e57b4eb5b2fe) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "nd-r8-08.bin", 0x0000, 0x1000, CRC(069a3e35) SHA1(09a361baed6417fdb4ecc1d8dd09ae9b2ccdef99) )
	ROM_LOAD( "nd-n8-07.bin", 0x1000, 0x1000, CRC(7ef6f5e4) SHA1(8839f6670010c0249b19f306c200c29775dc0351) )

	ROM_REGION( 0x2000, "gfx3", 0 )
	ROM_LOAD( "st-h5-05.bin", 0x0000, 0x1000, CRC(64d2f54f) SHA1(f301c0190f780bc60d03e5b752bde5e81ce5e93c) )
	ROM_LOAD( "st-k5-06.bin", 0x1000, 0x1000, CRC(af9b0d35) SHA1(da2d469514a3b95362dace2f48d320234a695e28) )

	ROM_REGION( 0x0080, "proms", 0 )
	ROM_LOAD( "u02--2.bin",   0x0000, 0x0020, CRC(238a65d7) SHA1(a5b20184a1989db23544296331462ec4d7be7516) )	/* palette (high bits) */
	ROM_LOAD( "t02--3.bin",   0x0020, 0x0020, CRC(ae263dc0) SHA1(7072c100b9d692f5bb12b0c9e304425f534481e2) )	/* palette (low bits) */
	ROM_LOAD( "f10--1.bin",   0x0040, 0x0020, CRC(16ee4ca2) SHA1(fcba4d103708b9711452009cd29c4f88d2f64cd3) )	/* sprite color lookup table */
	ROM_LOAD( "j10--4.bin",   0x0060, 0x0020, CRC(ff7fe284) SHA1(3ac8e30011c1fcba0ee8f4dc932f82296c3ba143) )	/* timing (not used) */
ROM_END

GAME( 1998, mrdigdo,  mrdo, mrdo,   mrdo, driver_device, 0, ROT270, "Justin Clark", "Mr. Do! (Dig Dug sprites)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, mrdofast, mrdo, mrdo,   mrdo, driver_device, 0, ROT270, "Evil Exidy Dump", "Mr. Do! (Speedup Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, mrpac,    mrdo, mrdo,   mrdo, driver_device, 0, ROT270, "Twisty", "Mr. Pac! (Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, mrpacb,   mrdo, mrdo,   mrdo, driver_device, 0, ROT270, "Twisty", "Mr. Pac! (Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, newdigdo, mrdo, mrdo,   mrdo, driver_device, 0, ROT270, "Vic Twenty George, Jerky, Justin Clark", "Mr. Do! (Dig Dug sprites, new version)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, stickydo, mrdo, mrdo,   mrdo, driver_device, 0, ROT270, "Marcks Hacks", "Mr. Sticky Do!", MACHINE_SUPPORTS_SAVE )
