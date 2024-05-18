// license:GPL_2.0
// copyright-holders:Robbbert
/*
Local numbering scheme: I made this up as a convenience - it is NOT OFFICIAL


Number    Date         Have Hacks?    Name
---------------------------------------------------------------------------------------------------------------
P101      1997         No             Dragon World 2
P102      1997-04      Yes            Oriental Legend
P103      1998         No             China Dragon 3
P104      1998         No             China Dragon 3 EX
P105      1998         Yes            The Killing Blade
P106      1998         Yes            Oriental Legend Special
P107      1999         Yes            Knights of Valour
P108      1999         Yes            Knights of Valour Plus
P109      1999         Yes            Knights of Valour Superheroes
P110      1999         No             Photo Y2K
P111      1999         No             Puzzle Star
P112      1999         No             Puzzli 2
P113      2000         No             Dragon World 3 Special
P114      2000         Yes            Knights of Valour 2
P115      2001-05-21   No             Bee Storm: DoDonPachi 2
P116      2001         No             Dragon World: Pretty Chance
P117      2001         No             Dragon World 2001
P118      2001         Yes            Knights of Valour 2 Plus: Nine Dragons
P119      2001-04      Yes            Martial Masters
P120      2001         No             Photo Y2K 2
P121      2001         No             Puzzli 2 Super
P122      2002-06      No             Demon Front
P123      2002-10-25   No             DoDonPachi Dai-Ou-Jou
P124      2002         No             DoDonPachi DaiOuJou Black Label
P125      2003-01-28   Yes            Ketsui: Kizuna Jigoku Tachi
P126      2003-10-15   No             ESP Galuda
P127      2003         Yes            The Gladiator
P128      2003         No             Happy 6-in-1
P129      2004         Yes            Knights of Valour Superheroes Plus
P130      2004         No             Oriental Legend Special Plus
P131      2004         No             Shiny 3-in-1
P132      2005         No             The Killing Blade Plus
P133      2005         No             Spectral vs Generation
*/
#include "../mame/drivers/pgm.cpp"
/**********
 Homebrew
***********/
ROM_START( pgemeni )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgemeni.p1",  0x100000, 0x200000, CRC(6cafa56b) SHA1(0b3447357cf9dc10e9473102a109a2c9f63be66b) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgemeni.t1",          0x180000, 0x400000, CRC(42b979dd) SHA1(38ae8a7e07f1169b876bd41e0a9bfe19ecb0256f) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgemeni.a1",          0x0000000, 0x400000, CRC(105d7cee) SHA1(43b19032e9e3a1aeabd7ce11438c6658ac4b5dd0) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgemeni.b1",          0x0000000, 0x400000, CRC(b4127373) SHA1(fa731bedec8ff4c19ec68bd44e8ce0b80ec0c716) )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmdemo.m1",          0x400000, 0x200000, CRC(8d89877e) SHA1(7d76d48d64d7ac5411d714a4bb83f37e3e5b8df6) )
ROM_END

ROM_START( pgmdemo )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmdemo.p1", 0x100000, 0x200000, CRC(d3f6ec45) SHA1(4c171ffff6a04b362f71c42de7bc1d7b57da2c0e) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgmdemo.t1",          0x180000, 0x400000, CRC(0596a59a) SHA1(e48cb703e886a5b71ecc839136c642117aa529ef) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgmdemo.a1",          0x0000000, 0x400000, CRC(5d0e8fa1) SHA1(95380456c8a6c9c48d4987951f689d5b9a7a397c) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgmdemo.b1",          0x0000000, 0x400000, CRC(15dd191f) SHA1(a90f63138c9a9bf9215b17a14635518aed7da8a9) )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmdemo.m1",          0x400000, 0x200000, CRC(8d89877e) SHA1(7d76d48d64d7ac5411d714a4bb83f37e3e5b8df6) )
ROM_END

ROM_START( pgmfrog )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmfrog.p1",  0x100000, 0x200000, CRC(cdec9e8d) SHA1(183753b3f19e9b06c6059206aac85d865bf1dd66) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgmfrog.t1",          0x180000, 0x400000, CRC(8f58b6d8) SHA1(79b3b503f29593170a7faa93d75707682d800c66) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgmfrog.a1",          0x0000000, 0x400000, CRC(dc1eafe6) SHA1(11eb0d02c38df828ae0361bc7624c4658047df67) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgmfrog.b1",          0x0000000, 0x400000, CRC(3d44b66f) SHA1(04347002bd942d320c6b8240e4485aec626f2aaa) )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmfrog.m1",          0x400000, 0x200000, CRC(05e2f761) SHA1(c93d94a8f11c41b019fcf9b6a90645416fd2c75b) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Homebrew
GAME( 2006, pgemeni,     pgm,       pgm,                pgm,      pgm_state,           init_pgm,        ROT0,   "Blastar", "P-Gemeni (2006-01-23)", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2005, pgmdemo,     pgm,       pgm,                pgm,      pgm_state,           init_pgm,        ROT0,   "Charles Doty", "Demo - PGM", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2006, pgmfrog,     pgm,       pgm,                pgm,      pgm_state,           init_pgm,        ROT0,   "Rastersoft", "Frog Feast (PGM)", MACHINE_SUPPORTS_SAVE )

