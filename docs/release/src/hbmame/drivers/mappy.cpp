// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mappy.cpp"

ROM_START( crackhea )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "mpx_3.1d",	 0xa000, 0x2000, CRC(52e6c708) SHA1(b9722941438e93325e84691ada4e95620bec73b2) )
	ROM_LOAD( "mp1_2.1c",	 0xc000, 0x2000, CRC(a958a61c) SHA1(e5198703cdf47b2cd7fc9f2a5fde7bf4ab2275db) )
	ROM_LOAD( "mpx_1.1b",	 0xe000, 0x2000, CRC(203766d4) SHA1(1dbc4f42d4c16a08240a221bec27dcc3a8dd7461) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "mp1_4.1k",	 0xe000, 0x2000, CRC(8182dd5b) SHA1(f36b57f7f1e79f00b3f07afe1960bca5f5325ee2) )

	ROM_REGION( 0x1000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "crackh3b.32", 0x0000, 0x1000, CRC(912ffa7e) SHA1(e2fc4992ef7214e62acdcda6605a1e27b2739cbf) )
	
	ROM_REGION( 0x4000, "gfx2", 0 )
	ROM_LOAD( "crackh3m.64", 0x0000, 0x2000, CRC(a263d078) SHA1(344fae10fab0f9646f5a2b39cac09faa9745ec9b) )
	ROM_LOAD( "crackh3n.64", 0x2000, 0x2000, CRC(3758ceb2) SHA1(152ba5a14514c0c160886d5c9a1296dea7a298aa) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "mp1-5.5b",	 0x0000, 0x0020, CRC(56531268) SHA1(2e356706c07f43eeb67783fb122bdc7fed1b3589) ) /* palette */
	ROM_LOAD( "mp1-6.4c",	 0x0020, 0x0100, CRC(50765082) SHA1(f578e14f15783acb2073644db4a2f0d196cc0957) ) /* characters */
	ROM_LOAD( "mp1-7.5k",	 0x0120, 0x0100, CRC(5396bd78) SHA1(2e387e5d8b8cab005f67f821b4db65d0ae8bd362) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "mp1-3.3m",	 0x0000, 0x0100, CRC(16a9166a) SHA1(847cbaf7c88616576c410177e066ae1d792ac0ba) )
ROM_END

ROM_START( housepty )
	ROM_REGION( 0x10000, "maincpu", 0 ) /* 64k for code for the first CPU */
	ROM_LOAD( "housep1d.64", 0xa000, 0x2000, CRC(6f778a5d) SHA1(afe1a317f8f355ad2e83d13835c2e28137d5a5e5) )
	ROM_LOAD( "mp1_2.1c",	 0xc000, 0x2000, CRC(a958a61c) SHA1(e5198703cdf47b2cd7fc9f2a5fde7bf4ab2275db) )
	ROM_LOAD( "housep1b.64", 0xe000, 0x2000, CRC(41a0c5b4) SHA1(74f74d679b42d51b8c850daaadc9df8889403712) )

	ROM_REGION( 0x10000, "sub", 0 ) /* 64k for the second CPU */
	ROM_LOAD( "mp1_4.1k",	 0xe000, 0x2000, CRC(8182dd5b) SHA1(f36b57f7f1e79f00b3f07afe1960bca5f5325ee2) )

	ROM_REGION( 0x1000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "housep3b.32", 0x0000, 0x1000, CRC(8d565a7e) SHA1(b2730de5ea2d19ba1e2851c27a73c6b03225a479) )

	ROM_REGION( 0x4000, "gfx2", 0 )
	ROM_LOAD( "housep3m.64", 0x0000, 0x2000, CRC(23a3c108) SHA1(266e7ca7db5f7d481f99fd295b6bdb2da3b34b93) )
	ROM_LOAD( "housep3n.64", 0x2000, 0x2000, CRC(c6822529) SHA1(1d8658a0e014d67b5b84f33cb19a9bdd8a88f623) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "mp1-5.5b",	 0x0000, 0x0020, CRC(56531268) SHA1(2e356706c07f43eeb67783fb122bdc7fed1b3589) ) /* palette */
	ROM_LOAD( "mp1-6.4c",	 0x0020, 0x0100, CRC(50765082) SHA1(f578e14f15783acb2073644db4a2f0d196cc0957) ) /* characters */
	ROM_LOAD( "mp1-7.5k",	 0x0120, 0x0100, CRC(5396bd78) SHA1(2e387e5d8b8cab005f67f821b4db65d0ae8bd362) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "mp1-3.3m",	 0x0000, 0x0100, CRC(16a9166a) SHA1(847cbaf7c88616576c410177e066ae1d792ac0ba) )
ROM_END

ROM_START( marpy )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "mpx_3.1d",	0xa000, 0x2000, CRC(52e6c708) SHA1(b9722941438e93325e84691ada4e95620bec73b2) )
	ROM_LOAD( "mp1_2.1c",	0xc000, 0x2000, CRC(a958a61c) SHA1(e5198703cdf47b2cd7fc9f2a5fde7bf4ab2275db) )
	ROM_LOAD( "mpx_1.1b",	0xe000, 0x2000, CRC(203766d4) SHA1(1dbc4f42d4c16a08240a221bec27dcc3a8dd7461) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "mp1_4.1k",	0xe000, 0x2000, CRC(8182dd5b) SHA1(f36b57f7f1e79f00b3f07afe1960bca5f5325ee2) )

	ROM_REGION( 0x1000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "marpy3b.32", 0x0000, 0x1000, CRC(2aa99047) SHA1(1ad2488066e433f2833b179b16d6500dd68ebecd) )

	ROM_REGION( 0x4000, "gfx2", 0 )
	ROM_LOAD( "marpy3m.64", 0x0000, 0x2000, CRC(ce61b84d) SHA1(33ff0ca22f347aceda084b4b8c3444a18b3d27e6) )
	ROM_LOAD( "marpy3n.64", 0x2000, 0x2000, CRC(5d0572a7) SHA1(9afecab321dd4558efe27b00dc95a5e22154eb68) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "mp1-5.5b",	0x0000, 0x0020, CRC(56531268) SHA1(2e356706c07f43eeb67783fb122bdc7fed1b3589) ) /* palette */
	ROM_LOAD( "mp1-6.4c",	0x0020, 0x0100, CRC(50765082) SHA1(f578e14f15783acb2073644db4a2f0d196cc0957) ) /* characters */
	ROM_LOAD( "mp1-7.5k",	0x0120, 0x0100, CRC(5396bd78) SHA1(2e387e5d8b8cab005f67f821b4db65d0ae8bd362) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "mp1-3.3m",	0x0000, 0x0100, CRC(16a9166a) SHA1(847cbaf7c88616576c410177e066ae1d792ac0ba) )
ROM_END

ROM_START( superpc1 )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "spc1-2.1c",    0xc000, 0x2000, CRC(4bb33d9c) SHA1(dd87f71b4db090a32a6b791079eedd17580cc741) )
	ROM_LOAD( "superpc1.1b",  0xe000, 0x2000, CRC(cec50857) SHA1(d8470064bf8a334accedcb767f5da05d5dce00a1) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "spc-3.1k",     0xf000, 0x1000, CRC(04445ddb) SHA1(ce7d14963d5ddaefdeaf433a6f82c43cd1611d9b) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "superpc1.3c",  0x0000, 0x1000, CRC(1c72ed9e) SHA1(31833bd244ff0b61419566eae4f53bdd76a5a0e8) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "superpc1.3f",  0x0000, 0x2000, CRC(89f07757) SHA1(642ca413cf27496322e4840998cdb711e3c682e0) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "superpac.4c",  0x0000, 0x0020, CRC(9ce22c46) SHA1(d97f53ef4c5ef26659a22ed0de4ce7ef3758c924) ) /* palette */
	ROM_LOAD( "superpac.4e",  0x0020, 0x0100, CRC(1253c5c1) SHA1(df46a90170e9761d45c90fbd04ef2aa1e8c9944b) ) /* chars */
	ROM_LOAD( "superpac.3l",  0x0120, 0x0100, CRC(d4d7026f) SHA1(a486573437c54bfb503424574ad82655491e85e1) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "superpc1.3m",  0x0000, 0x0100, CRC(e35238c7) SHA1(c55bd51a44b4c9899ca65f47ede952ca2714ce8e) )
ROM_END

ROM_START( superpcn )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "spc-2.1c",     0xc000, 0x2000, CRC(1a38c30e) SHA1(ae0ee9f3df0991a80698fe745a7a853a4bb60710) )
	ROM_LOAD( "spc-1.1b",     0xe000, 0x2000, CRC(730e95a9) SHA1(ca73c8bcb03c2f5c05968c707a5d3f7f9956b886) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "spc-3.1k",     0xf000, 0x1000, CRC(04445ddb) SHA1(ce7d14963d5ddaefdeaf433a6f82c43cd1611d9b) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "sp1-6.3c",     0x0000, 0x1000, CRC(91c5935c) SHA1(10579edabc26a0910253fab7d41b4c19ecdaaa09) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "spv-2.3f",     0x0000, 0x2000, CRC(670a42f2) SHA1(9171922df07e31fd1dc415766f7d2cc50a9d10dc) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "superpac.4c",  0x0000, 0x0020, CRC(9ce22c46) SHA1(d97f53ef4c5ef26659a22ed0de4ce7ef3758c924) ) /* palette */
	ROM_LOAD( "superpac.4e",  0x0020, 0x0100, CRC(1253c5c1) SHA1(df46a90170e9761d45c90fbd04ef2aa1e8c9944b) ) /* chars */
	ROM_LOAD( "superpac.3l",  0x0120, 0x0100, CRC(d4d7026f) SHA1(a486573437c54bfb503424574ad82655491e85e1) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "superpac.3m",  0x0000, 0x0100, CRC(ad43688f) SHA1(072f427453efb1dda8147da61804fff06e1bc4d5) )
ROM_END

ROM_START( superpcp )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "spc-2.1c",     0xc000, 0x2000, CRC(1a38c30e) SHA1(ae0ee9f3df0991a80698fe745a7a853a4bb60710) )
	ROM_LOAD( "spc-1.1b",     0xe000, 0x2000, CRC(730e95a9) SHA1(ca73c8bcb03c2f5c05968c707a5d3f7f9956b886) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "spc-3.1k",     0xf000, 0x1000, CRC(04445ddb) SHA1(ce7d14963d5ddaefdeaf433a6f82c43cd1611d9b) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "spv-1.3c",     0x0000, 0x1000, CRC(78337e74) SHA1(11222adb55e6bce508896ccb1f6dbab0c1d44e5b) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "spcp-2.3f",    0x0000, 0x2000, CRC(0d52ef4d) SHA1(9db8d36d80db8622dd129e8db01ef507f0e5e926) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "superpac.4c",  0x0000, 0x0020, CRC(9ce22c46) SHA1(d97f53ef4c5ef26659a22ed0de4ce7ef3758c924) ) /* palette */
	ROM_LOAD( "superpac.4e",  0x0020, 0x0100, CRC(1253c5c1) SHA1(df46a90170e9761d45c90fbd04ef2aa1e8c9944b) ) /* chars */
	ROM_LOAD( "superpac.3l",  0x0120, 0x0100, CRC(d4d7026f) SHA1(a486573437c54bfb503424574ad82655491e85e1) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "superpac.3m",  0x0000, 0x0100, CRC(ad43688f) SHA1(072f427453efb1dda8147da61804fff06e1bc4d5) )
ROM_END

ROM_START( superpcs )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "spc1-2.1c",    0xc000, 0x2000, CRC(4bb33d9c) SHA1(dd87f71b4db090a32a6b791079eedd17580cc741) )
	ROM_LOAD( "superpcs.1b",  0xe000, 0x2000, CRC(8df321c2) SHA1(f8e0a004042be3003534b226a80ab7104f684e26) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "spc-3.1k",     0xf000, 0x1000, CRC(04445ddb) SHA1(ce7d14963d5ddaefdeaf433a6f82c43cd1611d9b) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "superpcs.3c",  0x0000, 0x1000, CRC(3a4f438d) SHA1(ca333d02c94380d4fc4b56c8cc5f7269157ac371) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "superpcs.3f",  0x0000, 0x2000, CRC(d477484e) SHA1(edf724a3a53c0bfd18aaa55e452b47c8b54b223e) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "superpcs.4c",  0x0000, 0x0020, CRC(5508bfda) SHA1(29aaf77e2a1dda241da31ad83e43186b9ec491f1) ) /* palette */
	ROM_LOAD( "superpac.4e",  0x0020, 0x0100, CRC(1253c5c1) SHA1(df46a90170e9761d45c90fbd04ef2aa1e8c9944b) ) /* chars */
	ROM_LOAD( "superpac.3l",  0x0120, 0x0100, CRC(d4d7026f) SHA1(a486573437c54bfb503424574ad82655491e85e1) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "superpcs.3m",  0x0000, 0x0100, CRC(06acd480) SHA1(4963bd69e8718cf3fd08f06a5f03fe153c380af4) )
ROM_END

ROM_START( vectrsup )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "spc-2.1c",     0xc000, 0x2000, CRC(1a38c30e) SHA1(ae0ee9f3df0991a80698fe745a7a853a4bb60710) )
	ROM_LOAD( "spc-1.1b",     0xe000, 0x2000, CRC(730e95a9) SHA1(ca73c8bcb03c2f5c05968c707a5d3f7f9956b886) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "spc-3.1k",     0xf000, 0x1000, CRC(04445ddb) SHA1(ce7d14963d5ddaefdeaf433a6f82c43cd1611d9b) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vecsup-1.3c",  0x0000, 0x1000, CRC(618db633) SHA1(3930a12c0d76d55b3ff40cc9deca20b01c6c9f9b) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "vecsup-2.3f",  0x0000, 0x2000, CRC(62c09c6c) SHA1(f1c65b594c293b6a4eb459eac80fe28929c64eb6) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "superpac.4c",  0x0000, 0x0020, CRC(9ce22c46) SHA1(d97f53ef4c5ef26659a22ed0de4ce7ef3758c924) ) /* palette */
	ROM_LOAD( "superpac.4e",  0x0020, 0x0100, CRC(1253c5c1) SHA1(df46a90170e9761d45c90fbd04ef2aa1e8c9944b) ) /* chars */
	ROM_LOAD( "superpac.3l",  0x0120, 0x0100, CRC(d4d7026f) SHA1(a486573437c54bfb503424574ad82655491e85e1) ) /* sprites */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "superpac.3m",  0x0000, 0x0100, CRC(ad43688f) SHA1(072f427453efb1dda8147da61804fff06e1bc4d5) )
ROM_END

GAME( 1998, crackhea, mappy, 	mappy, 	  mappy,    mappy_state,   mappy,    ROT90, "Cary Barnhard", "Crackhead", MACHINE_SUPPORTS_SAVE ) // transparency error
GAME( 1998, housepty, mappy, 	mappy, 	  mappy,    mappy_state,   mappy,    ROT90, "Jerky", "House Party", MACHINE_SUPPORTS_SAVE )
GAME( 2001, marpy,    mappy, 	mappy,    mappy,    mappy_state,   mappy,    ROT90, "Mark", "Marpy", MACHINE_SUPPORTS_SAVE ) // transparency error
GAME( 2011, superpc1, superpac,	superpac, superpac, mappy_state,   superpac, ROT90, "Paul Smith", "Super Pac-Man (Super Locksmith sound and gfx hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, superpcn, superpac,	superpac, superpac, mappy_state,   superpac, ROT90, "Namco", "Super Pac-Man (Namco)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, superpcp, superpac,	superpac, superpac, mappy_state,   superpac, ROT90, "Blue Justice", "Super Pac-Man Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2011, superpcs, superpac,	superpac, superpac, mappy_state,   superpac, ROT90, "Paul Smith", "Super Pac-Man (always small)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vectrsup, superpac,	superpac, superpac, mappy_state,   superpac, ROT90, "T-Bone", "Super Pac-Man (Vector sim)", MACHINE_SUPPORTS_SAVE )
