// license:BSD-3-Clause
// copyright-holders:Gaston90
#include "../mame/drivers/psikyo.cpp"

/**************
 Sengoku Blade
****************/

ROM_START( tengai01 )  // hack of tengai
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "s01.u40", 0x000000, 0x80000, CRC(aae15ef1) SHA1(e5d056eb0ef26a697544cbeacb5f993217fdbd0a) )
	ROM_LOAD32_WORD_SWAP( "s01.u41", 0x000002, 0x80000, CRC(e8863874) SHA1(f6ac25170516658c368fb86b2b653e3356b42f19) )

	ROM_REGION( 0x020000, "audiocpu", 0 )
	ROM_LOAD( "1-u63.bin", 0x00000, 0x20000, CRC(2025e387) SHA1(334b0eb3b416d46ccaadff3eee6f1abba63285fb) )

	ROM_REGION( 0x001000, "mcu", 0 )
	ROM_LOAD( "4-u59.bin", 0x00000, 0x01000, NO_DUMP )

	ROM_REGION( 0x600000, "gfx1", 0 )
	ROM_LOAD( "u20.bin",  0x000000, 0x200000, CRC(ed42ef73) SHA1(74693fcc83a2654ddb18fd513d528033863d6116) )
	ROM_LOAD( "u22.bin",  0x200000, 0x200000, CRC(8d21caee) SHA1(2a68af8b2be2158dcb152c434e91a75871478d41) )
	ROM_LOAD( "u21.bin",  0x400000, 0x200000, CRC(efe34eed) SHA1(7891495b443a5acc7b2f17fe694584f6cb0afacc) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "u34.bin",  0x000000, 0x400000, CRC(2a2e2eeb) SHA1(f1d99353c0affc5c908985e6f2a5724e5223cccc) )

	ROM_REGION( 0x400000, "ymf", 0 )
	ROM_LOAD( "u61.bin",  0x000000, 0x200000, CRC(a63633c5) SHA1(89e75a40518926ebcc7d88dea86c01ba0bb496e5) )
	ROM_LOAD( "u62.bin",  0x200000, 0x200000, CRC(3ad0c357) SHA1(35f78cfa2eafa93ab96b24e336f569ee84af06b6) )

	ROM_REGION16_LE( 0x040000, "spritelut", 0 )
	ROM_LOAD( "u1.bin",  0x000000, 0x040000, CRC(681d7d55) SHA1(b0b28471440d747adbc4d22d1918f89f6ede1615) )
ROM_END

ROM_START( tengai02 )  // hack of tengaij (Sengoku Ace Episode II)
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "s02.u40", 0x000000, 0x80000, CRC(2cbb7dfd) SHA1(18d19c63b83334b447e5e302f9bf446fcfb9d355) )
	ROM_LOAD32_WORD_SWAP( "s02.u41", 0x000002, 0x80000, CRC(52c74de1) SHA1(fa31ffad0fa0aa61b4a927162f7a32e0b99e325c) )

	ROM_REGION( 0x020000, "audiocpu", 0 )
	ROM_LOAD( "1-u63.bin", 0x00000, 0x20000, CRC(2025e387) SHA1(334b0eb3b416d46ccaadff3eee6f1abba63285fb) )

	ROM_REGION( 0x001000, "mcu", 0 )
	ROM_LOAD( "4-u59.bin", 0x00000, 0x01000, NO_DUMP )

	ROM_REGION( 0x600000, "gfx1", 0 )
	ROM_LOAD( "u20hc02.bin",  0x000000, 0x200000, CRC(f6ab6364) SHA1(9e75cbc0c9526f1d86999acfbcb726ba29ae9ab9) )
	ROM_LOAD( "u22.bin",  0x200000, 0x200000, CRC(8d21caee) SHA1(2a68af8b2be2158dcb152c434e91a75871478d41) )
	ROM_LOAD( "u21.bin",  0x400000, 0x200000, CRC(efe34eed) SHA1(7891495b443a5acc7b2f17fe694584f6cb0afacc) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "s02.u34",  0x000000, 0x400000, CRC(1c712ec8) SHA1(0e6eac62d0c1127d486cd5b6ab964e2f05e5cddb) )

	ROM_REGION( 0x400000, "ymf", 0 )
	ROM_LOAD( "u61.bin",  0x000000, 0x200000, CRC(a63633c5) SHA1(89e75a40518926ebcc7d88dea86c01ba0bb496e5) )
	ROM_LOAD( "u62.bin",  0x200000, 0x200000, CRC(3ad0c357) SHA1(35f78cfa2eafa93ab96b24e336f569ee84af06b6) )

	ROM_REGION16_LE( 0x040000, "spritelut", 0 )
	ROM_LOAD( "u1.bin",  0x000000, 0x040000, CRC(681d7d55) SHA1(b0b28471440d747adbc4d22d1918f89f6ede1615) )
ROM_END

ROM_START( tengai03 )  // tengaik
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "s03.u40", 0x000000, 0x80000, CRC(279f7493) SHA1(bb39c50258fe30e4cc4b3da1f2acc05c362f8ddf) )
	ROM_LOAD32_WORD_SWAP( "s03.u41", 0x000002, 0x80000, CRC(59fe486e) SHA1(a6dfbe2e19926c9c1491a5aba480018732c57293) )

	ROM_REGION( 0x020000, "audiocpu", 0 )
	ROM_LOAD( "1-u63.bin", 0x00000, 0x20000, CRC(2025e387) SHA1(334b0eb3b416d46ccaadff3eee6f1abba63285fb) )

	ROM_REGION( 0x001000, "mcu", 0 )
	ROM_LOAD( "4-u59.bin", 0x00000, 0x01000, NO_DUMP )

	ROM_REGION( 0x600000, "gfx1", 0 )
	ROM_LOAD( "s03.u20",  0x000000, 0x200000, CRC(0bdc0c2d) SHA1(78f62237ef64c94e549fdf7f87f448a9cec02e67) )
	ROM_LOAD( "u22.bin",  0x200000, 0x200000, CRC(8d21caee) SHA1(2a68af8b2be2158dcb152c434e91a75871478d41) )
	ROM_LOAD( "s03.u21",  0x400000, 0x200000, CRC(435669ca) SHA1(4eaf8531e750d5d4bea6b3f4eaa39f7267fe7206) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "s03.u34",  0x000000, 0x400000, CRC(801fc802) SHA1(023c2d6515a91188791d923046365d6453c62808) )

	ROM_REGION( 0x400000, "ymf", 0 )
	ROM_LOAD( "u61.bin",  0x000000, 0x200000, CRC(a63633c5) SHA1(89e75a40518926ebcc7d88dea86c01ba0bb496e5) )
	ROM_LOAD( "u62.bin",  0x200000, 0x200000, CRC(3ad0c357) SHA1(35f78cfa2eafa93ab96b24e336f569ee84af06b6) )

	ROM_REGION16_LE( 0x040000, "spritelut", 0 )
	ROM_LOAD( "u1.bin",  0x000000, 0x040000, CRC(681d7d55) SHA1(b0b28471440d747adbc4d22d1918f89f6ede1615) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Tengai
GAME( 2009, tengai01,    tengai,   s1945,    tengai,   psikyo_state, init_tengai,   ROT0,   "Pipi899", "Tengai (Ex Super Version, 2009-10-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, tengai02,    tengai,   s1945,    tengaij,  psikyo_state, init_tengai,   ROT0,   "hack", "Tengai (Easy Type, 2019-03-08)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, tengai03,    tengai,   s1945,    tengaij,  psikyo_state, init_tengai,   ROT0,   "hack", "Tengai (Korean)", MACHINE_SUPPORTS_SAVE )

