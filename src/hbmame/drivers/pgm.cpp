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

 /*************
 The Gladiator
****************/
ROM_START( theglads01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",      0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "theglad_igs027a_v100_overseas.bin", 0x0188, 0x3e78, CRC(02fe6f52) SHA1(0b0ddf4507856cfc5b7d4ef7e4c5375254c2a024) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "v107_ph01.u26", 0x000000, 0x200000,  CRC(aba60d88) SHA1(953f6476871c2ad32b195e231c4a07ec3ddfb5b5) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads02 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",      0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "theglad_igs027a_v100_overseas.bin", 0x0188, 0x3e78, CRC(02fe6f52) SHA1(0b0ddf4507856cfc5b7d4ef7e4c5375254c2a024) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "v107_ph02.u26", 0x000000, 0x200000,  CRC(8187ce9d) SHA1(3670eb61c458a8e5aa32a087574de865a51263ee) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads03 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",      0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "theglad_igs027a_v100_overseas.bin", 0x0188, 0x3e78, CRC(02fe6f52) SHA1(0b0ddf4507856cfc5b7d4ef7e4c5375254c2a024) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "v107_ph03.u26", 0x000000, 0x200000,  CRC(78959ea7) SHA1(a4591431bbdbfd2ae2912264bad268e91862d240) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads04 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",      0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "theglad_igs027a_v100_overseas.bin", 0x0188, 0x3e78, CRC(02fe6f52) SHA1(0b0ddf4507856cfc5b7d4ef7e4c5375254c2a024) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "v107_ph04.u26", 0x000000, 0x200000,  CRC(8c8eb98e) SHA1(6868da4136d6594586997abe0629c61ff5303214) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads05 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",       0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "thegladpcb_igs027a_v100_japan.bin", 0x0188, 0x3e78, CRC(d7f06e2d) SHA1(9c3aca7a487f5329d84731e2c63d5ed591bf9d24) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "u2_pw01.rom", 0x000000, 0x200000,  CRC(e9ebb722) SHA1(03a4f4c9541b7c605381537f4490b236d6993f2f) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads06 ) //theglad100
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",       0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "thegladpcb_igs027a_v100_japan.bin", 0x0188, 0x3e78, CRC(d7f06e2d) SHA1(9c3aca7a487f5329d84731e2c63d5ed591bf9d24) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "u2_pw02.rom", 0x000000, 0x200000,  CRC(90061701) SHA1(0eeef7257c2ea706e037551b0def27d8fae5889e) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads07 ) //theglad100
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",       0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "thegladpcb_igs027a_v100_japan.bin", 0x0188, 0x3e78, CRC(d7f06e2d) SHA1(9c3aca7a487f5329d84731e2c63d5ed591bf9d24) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "u2_pw03.rom", 0x000000, 0x200000,  CRC(8ec8724b) SHA1(bd11dc3be6e1277e612a7c159f5a4f5d84742b12) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

ROM_START( theglads08 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v101_u6.u6",  0x100000, 0x080000, CRC(f799e866) SHA1(dccc3c903357c40c3cf85ac0ae8fc12fb0f853a6) )

	ROM_REGION( 0x4000, "prot", ROMREGION_ERASE00 )
	//ROM_LOAD( "theglad_igs027a_execute_only_area", 0x0000, 0x00188, NO_DUMP )
	ROM_LOAD( "theglad_igs027a_v100_overseas_pw01.bin", 0x0188, 0x3e78, CRC(bb1f6013) SHA1(ac6109c64ed775dd38e732fc0f517db026f4239f) )

	ROM_REGION32_LE( 0x800000, "user1", 0 )
	ROM_LOAD( "v101_pw01.u26", 0x000000, 0x200000, CRC(9629273b) SHA1(3004d44079ddd680ea426b00b2bcff5e9e123e81) )

	ROM_REGION( 0xa00000, "tiles", 0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "igs_t04601w64m.u33",   0x180000, 0x800000, CRC(e5dab371) SHA1(2e3c93958eb0326b6b84b95c2168626f26bbac76) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "igs_a04601w64m.u2",    0x0000000, 0x0800000,  CRC(d9b2e004) SHA1(8e1882b800fe9f12d7d49303e7417ba5b6f8ef85) )
	ROM_LOAD( "igs_a04602w64m.u4",    0x0800000, 0x0800000,  CRC(14f22308) SHA1(7fad54704e8c97eab723f53dfb50fb3e7bb606d2) )
	ROM_LOAD( "igs_a04603w64m.u6",    0x1000000, 0x0800000,  CRC(8f621e17) SHA1(b0f87f378e0115d0c95017ca0f1b0d508827a7c6) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "igs_b04601w64m.u11",   0x0000000, 0x0800000, CRC(ee72bccf) SHA1(73c25fe659f6c903447066e4ef83d2f580449d76) )
	ROM_LOAD( "igs_b04602w32m.u12",   0x0800000, 0x0400000, CRC(7dba9c38) SHA1(a03d509274e8f6a500a7ebe2da5aab8bed4e7f2f) )

	ROM_REGION( 0x1000000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "igs_w04601b64m.u1",    0x400000, 0x800000, CRC(5f15ddb3) SHA1(c38dcef8e06802a84e42a7fc9fa505475fc3ac65) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Homebrew
GAME( 2006, pgemeni,     pgm,       pgm,                pgm,      pgm_state,           init_pgm,        ROT0,   "Blastar", "P-Gemeni (2006-01-23)", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2005, pgmdemo,     pgm,       pgm,                pgm,      pgm_state,           init_pgm,        ROT0,   "Charles Doty", "Demo - PGM", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2006, pgmfrog,     pgm,       pgm,                pgm,      pgm_state,           init_pgm,        ROT0,   "Rastersoft", "Frog Feast (PGM)", MACHINE_SUPPORTS_SAVE )
// The Gladiator
GAME( 2003, theglads01,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglad,    ROT0,   "Hooooook", "The Gladiator (Ex Super Version v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, theglads02,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglad,    ROT0,   "hack", "The Gladiator (Infinite Power)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, theglads03,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglad,    ROT0,   "hack", "The Gladiator (Excalibur Vol. 2017 + Enhanced, 2017-01-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, theglads04,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglad,    ROT0,   "hack", "The Gladiator (Excalibur Vol.2 Enhanced version 2018, 2018-08-03)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, theglads05,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglada,   ROT0,   "hack", "The Gladiator (Excalibur Vol. 2017 + Enhanced, 2017-10-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, theglads06,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglada,   ROT0,   "hack", "The Gladiator (Full Attack Enhanced Edition 2020, 2020-09-06)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, theglads07,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglada,   ROT0,   "hack", "The Gladiator (Full Attack Enhanced Edition 2020, 2020-09-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, theglads08,    theglad,  pgm_arm_type3,      theglad, pgm_arm_type3_state, init_theglad,    ROT0,   "hack", "The Gladiator (Excalibur Vol. 2017 + Enhanced, 2017-11-04)", MACHINE_SUPPORTS_SAVE )

