// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/nmk16.cpp"

//tdragonp moved to mnw.cpp

// http://daifukkat.su/hacks/td2pract/  describes all about this hack.
ROM_START( tdragon2p )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "6p.rom",      0x00000, 0x80000, CRC(f99f4bf8) SHA1(34dfc188d94b89efdaa61cfc40af88868d7f22db) )

	ROM_REGION( 0x20000, "audiocpu", 0 )
	ROM_LOAD( "5.bin",    0x00000, 0x20000, CRC(b870be61) SHA1(ea5d45c3a3ab805e55806967f00167cf6366212e) )

	ROM_REGION( 0x020000, "fgtile", 0 )
	ROM_LOAD( "1.bin",    0x000000, 0x020000, CRC(d488aafa) SHA1(4d05e7ca075b638dd90ae4c9f224817a8a3ae9f3) )

	ROM_REGION( 0x200000, "bgtile", 0 )
	ROM_LOAD( "ww930914.2", 0x000000, 0x200000, CRC(f968c65d) SHA1(fd6d21bba53f945b1597d7d0735bc62dd44d5498) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_WORD_SWAP( "ww930917.7", 0x000000, 0x200000, CRC(b98873cb) SHA1(cc19200865176e940ff68e12de81f029b51c2084) )
	ROM_LOAD16_WORD_SWAP( "ww930918.8", 0x200000, 0x200000, CRC(baee84b2) SHA1(b325b00e6147266dbdc840e03556004531dc2038) )

	ROM_REGION( 0x240000, "oki1", 0 )
	ROM_LOAD( "ww930916.4", 0x040000, 0x200000, CRC(07c35fe6) SHA1(33547bd88764704310f2ef8cf3bfe21ceb56d5b7) )

	ROM_REGION( 0x240000, "oki2", 0 )
	ROM_LOAD( "ww930915.3", 0x040000, 0x200000, CRC(82025bab) SHA1(ac6053700326ea730d00ec08193e2c8a2a019f0b) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "9.bpr",  0x0000, 0x0100, CRC(435653a2) SHA1(575b4a46ea65179de3042614da438d2f6d8b572e) )
	ROM_LOAD( "10.bpr", 0x0100, 0x0100, CRC(e6ead349) SHA1(6d81b1c0233580aa48f9718bade42d640e5ef3dd) )
ROM_END

GAME( 1993, tdragon2p,tdragon2, tdragon2, tdragon2, nmk16_state, init_banked_audiocpu,  ROT270, "Trap15", "Thunder Dragon 2 (Practice Mode)", MACHINE_NO_COCKTAIL | MACHINE_IMPERFECT_GRAPHICS )

