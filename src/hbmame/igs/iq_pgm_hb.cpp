// license:BSD-3-Clause
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
#include "iq_pgm.cpp"
/**********
 Homebrew
***********/
ROM_START( pgmsnake )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmsnake.p1",  0x100000, 0x080000, CRC(d8a86ce2) SHA1(eccdfc8b84e32aadd0300b7691af02bb209e0c95) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS

	ROM_REGION16_LE( 0x2000000, "sprcol", ROMREGION_ERASE00 )

	ROM_REGION16_LE( 0x1000000, "sprmask", ROMREGION_ERASE00 )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
ROM_END

/*****************
 The Killing Blade
********************/
ROM_START( killbld01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "s01.u9", 0x100000, 0x200000, CRC(1e8134e0) SHA1(023588da0da4f5d9edf5a66cb00529bf1aa65870) )

	ROM_REGION( 0x010000, "igs022", 0 )
	ROM_LOAD( "kb_u2_v109.u2", 0x000000, 0x010000,  CRC(de3eae63) SHA1(03af767ef764055bda528b5cc6a24b9e1218cca8) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgm_t0300.u14",    0x180000, 0x400000, CRC(0922f7d9) SHA1(4302b4b7369e13f315fad14f7d6cad1321101d24) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgm_a0300.u9",   0x0000000, 0x0400000,  CRC(3f9455d3) SHA1(3718ce00ad93975383aafc14e5a74dc297b011a1) )
	ROM_LOAD( "pgm_a0301.u10",  0x0400000, 0x0400000,  CRC(92776889) SHA1(6d677837fefff47bfd1c6166322f69f89989a5e2) )
	ROM_LOAD( "pgm_a0303.u11",  0x0800000, 0x0400000,  CRC(33f5cc69) SHA1(9cacd5058d4bb25b77f71658bbbbd4b38d0a6b6a) )
	ROM_LOAD( "pgm_a0306.u12",  0x0c00000, 0x0400000,  CRC(cc018a8e) SHA1(37752d46f238fb57c0ab5a4f96b1e013f2077347) )
	ROM_LOAD( "pgm_a0307.u2",   0x1000000, 0x0400000,  CRC(bc772e39) SHA1(079cc42a190cb916f02b59bca8fa90e524acefe9) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgm_b0300.u13",    0x0000000, 0x0400000, CRC(7f876981) SHA1(43555a200929ad5ecc42137fc9aeb42dc4f50d20) )
	ROM_LOAD( "pgm_b0302.u14",    0x0400000, 0x0400000, CRC(eea9c502) SHA1(04b3972c7111ea59a3cceab6ad124080c4ce3520) )
	ROM_LOAD( "pgm_b0303.u15",    0x0800000, 0x0200000, CRC(77a9652e) SHA1(2342f643d37945fbda224a5034c013796e5134ca) )

	ROM_REGION( 0x800000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgm_m0300.u1",     0x400000, 0x400000, CRC(93159695) SHA1(50c5976c9b681bd3d1ebefa3bfa9fe6e72dcb96f) )
ROM_END

ROM_START( killbld02 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "s02.u9", 0x100000, 0x200000, CRC(d82a1938) SHA1(71ed4109d5e451fed359e826ca53a8ed48e827a7) )

	ROM_REGION( 0x010000, "igs022", 0 )
	ROM_LOAD( "kb_u2_v109.u2", 0x000000, 0x010000,  CRC(de3eae63) SHA1(03af767ef764055bda528b5cc6a24b9e1218cca8) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgm_t0300.u14",    0x180000, 0x400000, CRC(0922f7d9) SHA1(4302b4b7369e13f315fad14f7d6cad1321101d24) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgm_a0300.u9",   0x0000000, 0x0400000,  CRC(3f9455d3) SHA1(3718ce00ad93975383aafc14e5a74dc297b011a1) )
	ROM_LOAD( "pgm_a0301.u10",  0x0400000, 0x0400000,  CRC(92776889) SHA1(6d677837fefff47bfd1c6166322f69f89989a5e2) )
	ROM_LOAD( "pgm_a0303.u11",  0x0800000, 0x0400000,  CRC(33f5cc69) SHA1(9cacd5058d4bb25b77f71658bbbbd4b38d0a6b6a) )
	ROM_LOAD( "pgm_a0306.u12",  0x0c00000, 0x0400000,  CRC(cc018a8e) SHA1(37752d46f238fb57c0ab5a4f96b1e013f2077347) )
	ROM_LOAD( "pgm_a0307.u2",   0x1000000, 0x0400000,  CRC(bc772e39) SHA1(079cc42a190cb916f02b59bca8fa90e524acefe9) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgm_b0300.u13",    0x0000000, 0x0400000, CRC(7f876981) SHA1(43555a200929ad5ecc42137fc9aeb42dc4f50d20) )
	ROM_LOAD( "pgm_b0302.u14",    0x0400000, 0x0400000, CRC(eea9c502) SHA1(04b3972c7111ea59a3cceab6ad124080c4ce3520) )
	ROM_LOAD( "pgm_b0303.u15",    0x0800000, 0x0200000, CRC(77a9652e) SHA1(2342f643d37945fbda224a5034c013796e5134ca) )

	ROM_REGION( 0x800000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgm_m0300.u1",     0x400000, 0x400000, CRC(93159695) SHA1(50c5976c9b681bd3d1ebefa3bfa9fe6e72dcb96f) )
ROM_END

ROM_START( killbld03 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "s03.u9", 0x100000, 0x200000, CRC(5f44677a) SHA1(b36ed941ad5617749fb0a2913e1718cb6707cf5b) )

	ROM_REGION( 0x010000, "igs022", 0 )
	ROM_LOAD( "kb_u2_v109.u2", 0x000000, 0x010000,  CRC(de3eae63) SHA1(03af767ef764055bda528b5cc6a24b9e1218cca8) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgm_t0300.u14",    0x180000, 0x400000, CRC(0922f7d9) SHA1(4302b4b7369e13f315fad14f7d6cad1321101d24) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgm_a0300.u9",   0x0000000, 0x0400000,  CRC(3f9455d3) SHA1(3718ce00ad93975383aafc14e5a74dc297b011a1) )
	ROM_LOAD( "pgm_a0301.u10",  0x0400000, 0x0400000,  CRC(92776889) SHA1(6d677837fefff47bfd1c6166322f69f89989a5e2) )
	ROM_LOAD( "pgm_a0303.u11",  0x0800000, 0x0400000,  CRC(33f5cc69) SHA1(9cacd5058d4bb25b77f71658bbbbd4b38d0a6b6a) )
	ROM_LOAD( "pgm_a0306.u12",  0x0c00000, 0x0400000,  CRC(cc018a8e) SHA1(37752d46f238fb57c0ab5a4f96b1e013f2077347) )
	ROM_LOAD( "pgm_a0307.u2",   0x1000000, 0x0400000,  CRC(bc772e39) SHA1(079cc42a190cb916f02b59bca8fa90e524acefe9) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgm_b0300.u13",    0x0000000, 0x0400000, CRC(7f876981) SHA1(43555a200929ad5ecc42137fc9aeb42dc4f50d20) )
	ROM_LOAD( "pgm_b0302.u14",    0x0400000, 0x0400000, CRC(eea9c502) SHA1(04b3972c7111ea59a3cceab6ad124080c4ce3520) )
	ROM_LOAD( "pgm_b0303.u15",    0x0800000, 0x0200000, CRC(77a9652e) SHA1(2342f643d37945fbda224a5034c013796e5134ca) )

	ROM_REGION( 0x800000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgm_m0300.u1",     0x400000, 0x400000, CRC(93159695) SHA1(50c5976c9b681bd3d1ebefa3bfa9fe6e72dcb96f) )
ROM_END

ROM_START( killbld04 ) //killbldqy
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "s04.u9", 0x100000, 0x200000, CRC(98c85d0e) SHA1(933fba3d5764ef8889e584e3afd285af545c802a) )

	ROM_REGION( 0x010000, "igs022", 0 )
	ROM_LOAD( "kb_u2_v109.u2", 0x000000, 0x010000,  CRC(de3eae63) SHA1(03af767ef764055bda528b5cc6a24b9e1218cca8) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgm_t0300.u14",    0x180000, 0x400000, CRC(0922f7d9) SHA1(4302b4b7369e13f315fad14f7d6cad1321101d24) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgm_a0300.u9",   0x0000000, 0x0400000,  CRC(3f9455d3) SHA1(3718ce00ad93975383aafc14e5a74dc297b011a1) )
	ROM_LOAD( "pgm_a0301.u10",  0x0400000, 0x0400000,  CRC(92776889) SHA1(6d677837fefff47bfd1c6166322f69f89989a5e2) )
	ROM_LOAD( "pgm_a0303.u11",  0x0800000, 0x0400000,  CRC(33f5cc69) SHA1(9cacd5058d4bb25b77f71658bbbbd4b38d0a6b6a) )
	ROM_LOAD( "pgm_a0306.u12",  0x0c00000, 0x0400000,  CRC(cc018a8e) SHA1(37752d46f238fb57c0ab5a4f96b1e013f2077347) )
	ROM_LOAD( "pgm_a0307.u2",   0x1000000, 0x0400000,  CRC(bc772e39) SHA1(079cc42a190cb916f02b59bca8fa90e524acefe9) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgm_b0300.u13",    0x0000000, 0x0400000, CRC(7f876981) SHA1(43555a200929ad5ecc42137fc9aeb42dc4f50d20) )
	ROM_LOAD( "pgm_b0302.u14",    0x0400000, 0x0400000, CRC(eea9c502) SHA1(04b3972c7111ea59a3cceab6ad124080c4ce3520) )
	ROM_LOAD( "pgm_b0303.u15",    0x0800000, 0x0200000, CRC(77a9652e) SHA1(2342f643d37945fbda224a5034c013796e5134ca) )

	ROM_REGION( 0x800000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgm_m0300.u1",     0x400000, 0x400000, CRC(93159695) SHA1(50c5976c9b681bd3d1ebefa3bfa9fe6e72dcb96f) )
ROM_END

ROM_START( killbld05 ) //killbldqy
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "s05.u9", 0x100000, 0x200000, CRC(6f2ca623) SHA1(a1ce4a83bbef10d7c3dd18e74472a97a50db89f4) )

	ROM_REGION( 0x010000, "igs022", 0 )
	ROM_LOAD( "kb_u2_v109.u2", 0x000000, 0x010000,  CRC(de3eae63) SHA1(03af767ef764055bda528b5cc6a24b9e1218cca8) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgm_t0300.u14",    0x180000, 0x400000, CRC(0922f7d9) SHA1(4302b4b7369e13f315fad14f7d6cad1321101d24) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgm_a0300.u9",   0x0000000, 0x0400000,  CRC(3f9455d3) SHA1(3718ce00ad93975383aafc14e5a74dc297b011a1) )
	ROM_LOAD( "pgm_a0301.u10",  0x0400000, 0x0400000,  CRC(92776889) SHA1(6d677837fefff47bfd1c6166322f69f89989a5e2) )
	ROM_LOAD( "pgm_a0303.u11",  0x0800000, 0x0400000,  CRC(33f5cc69) SHA1(9cacd5058d4bb25b77f71658bbbbd4b38d0a6b6a) )
	ROM_LOAD( "pgm_a0306.u12",  0x0c00000, 0x0400000,  CRC(cc018a8e) SHA1(37752d46f238fb57c0ab5a4f96b1e013f2077347) )
	ROM_LOAD( "pgm_a0307.u2",   0x1000000, 0x0400000,  CRC(bc772e39) SHA1(079cc42a190cb916f02b59bca8fa90e524acefe9) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgm_b0300.u13",    0x0000000, 0x0400000, CRC(7f876981) SHA1(43555a200929ad5ecc42137fc9aeb42dc4f50d20) )
	ROM_LOAD( "pgm_b0302.u14",    0x0400000, 0x0400000, CRC(eea9c502) SHA1(04b3972c7111ea59a3cceab6ad124080c4ce3520) )
	ROM_LOAD( "pgm_b0303.u15",    0x0800000, 0x0200000, CRC(77a9652e) SHA1(2342f643d37945fbda224a5034c013796e5134ca) )

	ROM_REGION( 0x800000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgm_m0300.u1",     0x400000, 0x400000, CRC(93159695) SHA1(50c5976c9b681bd3d1ebefa3bfa9fe6e72dcb96f) )
ROM_END

GAME( 2023, pgmsnake,    pgm,       pgm,                 iq_pgm,      iq_pgm,           init_pgm,        ROT0,   "IQ_132", "PGM test program", MACHINE_SUPPORTS_SAVE )
// The Killing Blade
GAME( 1998, killbld01,   killbld,   pgm_022_025_killbld, iq_killbld,  iq_pgm_022_025,   init_killbld,    ROT0,   "DDJ", "The Killing Blade (Always Have Super Move)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, killbld02,   killbld,   pgm_022_025_killbld, iq_killbld,  iq_pgm_022_025,   init_killbld,    ROT0,   "DDJ", "The Killing Blade (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, killbld03,   killbld,   pgm_022_025_killbld, iq_killbld,  iq_pgm_022_025,   init_killbld,    ROT0,   "DDJ", "The Killing Blade (Enable Hidden Boss)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, killbld04,   killbld,   pgm_022_025_killbld, iq_killbld,  iq_pgm_022_025,   init_killbld,    ROT0,   "hack", "The Killing Blade (Qun Ying Edition, 2018-06-22)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, killbld05,   killbld,   pgm_022_025_killbld, iq_killbld,  iq_pgm_022_025,   init_killbld,    ROT0,   "hack", "The Killing Blade (Simplified, 2018-08-24)", MACHINE_SUPPORTS_SAVE )

