// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/gottlieb.cpp"

ROM_START( 711qbert )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "mqb-rom2.bin", 0xa000, 0x2000, CRC(6860f957) SHA1(ebd68aeb6d54868295bd20cf64ee0187a52df0e3) )
	ROM_LOAD( "mqb-rom1.bin", 0xc000, 0x2000, CRC(11f0a4e4) SHA1(a805e51c40042fae209ace277abd9b35a990905b) )
	ROM_LOAD( "mqb-rom0.bin", 0xe000, 0x2000, CRC(12a90cb2) SHA1(a33203aea79fe43d1233a16e3fdddaceac6e4a20) )

	ROM_REGION( 0x10000, "r1sound:audiocpu", 0 ) /* 64k for sound cpu */
	ROM_LOAD( "mqb-snd1.bin", 0xf000, 0x0800, CRC(e704b450) SHA1(d509f54658e9f0264b9ab865a6f36e5423a28904) )
	ROM_RELOAD(               0x7000, 0x0800 ) /* A15 is not decoded */
	ROM_LOAD( "mqb-snd2.bin", 0xf800, 0x0800, CRC(c6a98bf8) SHA1(cc5b5bb5966f5d79226f1f665a3f9fc934f4ef7f) )
	ROM_RELOAD(               0x7800, 0x0800 ) /* A15 is not decoded */

	ROM_REGION( 0x2000, "bgtiles", 0 )
	ROM_LOAD( "qb-bg0.bin",   0x0000, 0x1000, CRC(7a9ba824) SHA1(12aa6df499eb6996ee35f56acac403ff6290f844) ) /* chars */
	ROM_LOAD( "qb-bg1.bin",   0x1000, 0x1000, CRC(22e5b891) SHA1(5bb67e333255c0ea679ab4312256a8a71a950db8) )

	ROM_REGION( 0x8000, "sprites", 0 )
	ROM_LOAD( "mqb-fg3.bin",  0x0000, 0x2000, CRC(ee595eda) SHA1(11777d95ba79bd0ec7b964b76c1dc129db857816) )	/* sprites */
	ROM_LOAD( "mqb-fg2.bin",  0x2000, 0x2000, CRC(59884c78) SHA1(5e77ef46ccd55f79a5fa90521baa7c22e3783fe5) )
	ROM_LOAD( "mqb-fg1.bin",  0x4000, 0x2000, CRC(2a60e3ad) SHA1(9ed83017f6c8e44337ad76c68b095f2c3300aadb) )
	ROM_LOAD( "mqb-fg0.bin",  0x6000, 0x2000, CRC(b11ad9d8) SHA1(5264598f33aa76455ae4107d0f265c2a372ed67a) )
ROM_END

ROM_START( pbert )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "qb-rom2.bin",  0xa000, 0x2000, CRC(fe434526) SHA1(4cfc5d52dd6c82163e035af82d6112c0c93a3797) )
	ROM_LOAD( "qb-rom1.bin",  0xc000, 0x2000, CRC(55635447) SHA1(ca6acdef1c9e06b33efe1f0a2df2dfb03723cfbe) )
	ROM_LOAD( "qb-rom0.bin",  0xe000, 0x2000, CRC(8e318641) SHA1(7f8f66d1e6a7905e93cce07fc92e8801370b7194) )

	ROM_REGION( 0x10000, "r1sound:audiocpu", 0 )	/* 64k for sound cpu */
	ROM_LOAD( "qb-snd1.bin",  0xf000, 0x800, CRC(15787c07) SHA1(8b7d03fbf2ebaa71b3a7e2f636a0d1bb9b796e43) )
	ROM_RELOAD(               0x7000, 0x800 ) /* A15 is not decoded */
	ROM_LOAD( "qb-snd2.bin",  0xf800, 0x800, CRC(58437508) SHA1(09d8053e7e99679b602dcda230d64db7fe6cb7f5) )
	ROM_RELOAD(               0x7800, 0x800 ) /* A15 is not decoded */

	ROM_REGION( 0x2000, "bgtiles", 0 )
	ROM_LOAD( "pb-bg0.bin",   0x0000, 0x1000, CRC(14e193e9) SHA1(5d67b3da848d4a90a4f9bf7f6145ce1aa33188f7) )	/* chars */
	ROM_LOAD( "pb-bg1.bin",   0x1000, 0x1000, CRC(a91180f9) SHA1(c8a3ceae19afe9a0ad1e05bed99585e6bacbc09b) )

	ROM_REGION( 0x8000, "sprites", 0 )
	ROM_LOAD( "pb-fg3.bin",   0x0000, 0x2000, CRC(26def34c) SHA1(b36dd87e4c36d201aaec0c283e5f3d1ad4056d5d) )	/* sprites */
	ROM_LOAD( "pb-fg2.bin",   0x2000, 0x2000, CRC(cf3309ca) SHA1(5be9a28b3681990729f0582f9d2c3dd0314bdd15) )
	ROM_LOAD( "pb-fg1.bin",   0x4000, 0x2000, CRC(a5f3a203) SHA1(4d7d45e5ba87acd8c44421f2c7fb2129a1866f03) )
	ROM_LOAD( "pb-fg0.bin",   0x6000, 0x2000, CRC(495d90e9) SHA1(d4d28c30901eb09da8933c03b2178152f0fc7b84) )
ROM_END

// E444
ROM_START( qbertb )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "qb-rom2.bin",  0xa000, 0x2000, CRC(fe434526) SHA1(4cfc5d52dd6c82163e035af82d6112c0c93a3797) )
	ROM_LOAD( "qb-rom1.bin",  0xc000, 0x2000, CRC(55635447) SHA1(ca6acdef1c9e06b33efe1f0a2df2dfb03723cfbe) )
	ROM_LOAD( "qbertb.rom0",  0xe000, 0x2000, CRC(415b4fc7) SHA1(e55fb1912260435be253597fd337e04219b7a034) )

	ROM_REGION( 0x10000, "r1sound:audiocpu", 0 )	/* 64k for sound cpu */
	ROM_LOAD( "qbertb.snd",   0xf000, 0x1000, CRC(ebcedba9) SHA1(94aee8e32bdc80bbc5dc1423ca97597bdb9d808c) )
	ROM_RELOAD(               0x7000, 0x1000 ) /* A15 is not decoded */

	ROM_REGION( 0x2000, "bgtiles", 0 )
	ROM_LOAD( "qb-bg0.bin",   0x0000, 0x1000, CRC(7a9ba824) SHA1(12aa6df499eb6996ee35f56acac403ff6290f844) )	/* chars */
	ROM_LOAD( "qb-bg1.bin",   0x1000, 0x1000, CRC(22e5b891) SHA1(5bb67e333255c0ea679ab4312256a8a71a950db8) )

	ROM_REGION( 0x8000, "sprites", 0 )
	ROM_LOAD( "qbertb.fg3",   0x1000, 0x1000, CRC(983e3e05) SHA1(14f21543c3301b15d179b3864676e76ad5dfcaf8) )	/* sprites */
	ROM_LOAD( "qbertb.fg2",   0x3000, 0x1000, CRC(b3e6c7bc) SHA1(38e34e8712c5f677fa3fada68bc4c318e9bf7ca6) )
	ROM_LOAD( "qbertb.fg1",   0x5000, 0x1000, CRC(6733d069) SHA1(3b4ac832f2475d51ae7586d3eb80e355afb64222) )
	ROM_LOAD( "qbertb.fg0",   0x7000, 0x1000, CRC(3081c200) SHA1(137d95a2a58e2ed4da7145a539d1a1942c80674c) )
ROM_END

ROM_START( vbert )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "qb-rom2.bin",  0xa000, 0x2000, CRC(fe434526) SHA1(4cfc5d52dd6c82163e035af82d6112c0c93a3797) )
	ROM_LOAD( "qb-rom1.bin",  0xc000, 0x2000, CRC(55635447) SHA1(ca6acdef1c9e06b33efe1f0a2df2dfb03723cfbe) )
	ROM_LOAD( "qb-rom0.bin",  0xe000, 0x2000, CRC(8e318641) SHA1(7f8f66d1e6a7905e93cce07fc92e8801370b7194) )

	ROM_REGION( 0x10000, "r1sound:audiocpu", 0 )	/* 64k for sound cpu */
	ROM_LOAD( "qb-snd1.bin",  0xf000, 0x800, CRC(15787c07) SHA1(8b7d03fbf2ebaa71b3a7e2f636a0d1bb9b796e43) )
	ROM_RELOAD(               0x7000, 0x800 ) /* A15 is not decoded */
	ROM_LOAD( "qb-snd2.bin",  0xf800, 0x800, CRC(58437508) SHA1(09d8053e7e99679b602dcda230d64db7fe6cb7f5) )
	ROM_RELOAD(               0x7800, 0x800 ) /* A15 is not decoded */

	ROM_REGION( 0x2000, "bgtiles", 0 )
	ROM_LOAD( "vb-bg0.bin",   0x0000, 0x1000, CRC(8963b939) SHA1(1416f56938163b4cdc212758eb2c3f6b773c7e32) )	/* chars */
	ROM_LOAD( "vb-bg1.bin",   0x1000, 0x1000, CRC(96033c96) SHA1(0a5780377bfd3baeeb2d899aa230e503539f7c92) )

	ROM_REGION( 0x8000, "sprites", 0 )
	ROM_LOAD( "vb-fg3.bin",   0x0000, 0x2000, CRC(8cb4bdb4) SHA1(7b9b626b93daa4d2639a1e87ace61b1d254af96c) )	/* sprites */
	ROM_LOAD( "vb-fg2.bin",   0x2000, 0x2000, CRC(79883cc3) SHA1(01cfca389021747e2d750fc8bb251abb2d5fe7a8) )
	ROM_LOAD( "vb-fg1.bin",   0x4000, 0x2000, CRC(b0fe9bd5) SHA1(be2990904efff19adbe18eb674727214ffae9ad3) )
	ROM_LOAD( "vb-fg0.bin",   0x6000, 0x2000, CRC(4c3db270) SHA1(212f1c353511fb4166a7ad0135dd295a2a16af8d) )
ROM_END

GAME( 1982, 711qbert, qbert, qbert, qbert, gottlieb_state, qbert, ROT270, "Hack", "7-11 Q*bert", 0 )
GAME( 2003, pbert,    qbert, qbert, qbert, gottlieb_state, qbert, ROT270, "Grand Master Peter", "P*bert", 0 )
GAME( 1982, qbertb,   qbert, qbert, qbert, gottlieb_state, qbert, ROT270, "bootleg", "Q*bert", 0 )
GAME( 2003, vbert,    qbert, qbert, qbert, gottlieb_state, qbert, ROT270, "Grand Master Peter", "V*bert", 0 )
