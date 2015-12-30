// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/tehkanwc.cpp"

ROM_START( ballonwc )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-1.bin",    0x0000, 0x4000, CRC(34d6d5ff) SHA1(72f4d408b8a7766d348f6a229d395e0c98215c40) )
	ROM_LOAD( "twc-2.bin",    0x4000, 0x4000, CRC(7017a221) SHA1(4b4700af0a6ff64f976db369ba4b9d97cee1fd5f) )
	ROM_LOAD( "twc-3.bin",    0x8000, 0x4000, CRC(8b662902) SHA1(13bcd4bf23e34dd7193545561e05bb2cb2c95f9b) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for code */
	ROM_LOAD( "twc-4.bin",    0x0000, 0x8000, CRC(70a9f883) SHA1(ace04359265271eb37512a89eb0217eb013aecb7) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-6.bin",    0x0000, 0x4000, CRC(e3112be2) SHA1(7859e51b4312dc5df01c88e1d97cf608abc7ca72) )

	ROM_REGION( 0x04000, "gfx1", 0 )
	ROM_LOAD( "twc-12.bin",   0x00000, 0x4000, CRC(a9e274f8) SHA1(02b46e1b149a856f0be74a23faaeb792935b66c7) )	/* fg tiles */

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "blwc-8.bin",   0x00000, 0x8000, CRC(a0f75389) SHA1(60c5117430ac3471768efd0341ea533f49e8b1bd) )	/* sprites */
	ROM_LOAD( "twc-7.bin",    0x08000, 0x8000, CRC(59faebe7) SHA1(85dad90928369601e039467d575750539410fcf6) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "twc-11.bin",   0x00000, 0x8000, CRC(669389fc) SHA1(a93e8455060ce5242cb65f78e47b4840aa13ab13) )	/* bg tiles */
	ROM_LOAD( "twc-9.bin",    0x08000, 0x8000, CRC(347ef108) SHA1(bb9c2f51d65f28655404e10c3be44d7ade98711b) )

	ROM_REGION( 0x8000, "adpcm", 0 )	/* ADPCM samples */
	ROM_LOAD( "twc-5.bin",    0x0000, 0x4000,  CRC(444b5544) SHA1(0786d6d9ada7fe49c8ab9751b049095474d2e598) )
ROM_END

ROM_START( tehkangc )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-1.bin",    0x0000, 0x4000, CRC(34d6d5ff) SHA1(72f4d408b8a7766d348f6a229d395e0c98215c40) )
	ROM_LOAD( "twc-2.bin",    0x4000, 0x4000, CRC(7017a221) SHA1(4b4700af0a6ff64f976db369ba4b9d97cee1fd5f) )
	ROM_LOAD( "twc-3.bin",    0x8000, 0x4000, CRC(8b662902) SHA1(13bcd4bf23e34dd7193545561e05bb2cb2c95f9b) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for code */
	ROM_LOAD( "twc-4.bin",    0x0000, 0x8000, CRC(70a9f883) SHA1(ace04359265271eb37512a89eb0217eb013aecb7) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-6.bin",    0x0000, 0x4000, CRC(e3112be2) SHA1(7859e51b4312dc5df01c88e1d97cf608abc7ca72) )

	ROM_REGION( 0x04000, "gfx1", 0 )
	ROM_LOAD( "tgc-12.bin",   0x00000, 0x4000, CRC(dbf5cede) SHA1(ec2d21f62ab331ce64c7765f6b05a8c3c69e8ddd) )	/* fg tiles */

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "tgc-8.bin",    0x00000, 0x8000, CRC(1177c8a0) SHA1(be0e0c763f2ac08ede546884e616073eb9a5517d) )	/* sprites */
	ROM_LOAD( "tgc-7.bin",    0x08000, 0x8000, CRC(7f01d5a4) SHA1(4494ca82c9d16a4047591de7b588a8ab53ce5170) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "twc-11.bin",   0x00000, 0x8000, CRC(669389fc) SHA1(a93e8455060ce5242cb65f78e47b4840aa13ab13) )	/* bg tiles */
	ROM_LOAD( "twc-9.bin",    0x08000, 0x8000, CRC(347ef108) SHA1(bb9c2f51d65f28655404e10c3be44d7ade98711b) )

	ROM_REGION( 0x8000, "adpcm", 0 )	/* ADPCM samples */
	ROM_LOAD( "twc-5.bin",    0x0000, 0x4000, CRC(444b5544) SHA1(0786d6d9ada7fe49c8ab9751b049095474d2e598) )
ROM_END

ROM_START( tankwc )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-1.bin",    0x0000, 0x4000, CRC(34d6d5ff) SHA1(72f4d408b8a7766d348f6a229d395e0c98215c40) )
	ROM_LOAD( "twc-2.bin",    0x4000, 0x4000, CRC(7017a221) SHA1(4b4700af0a6ff64f976db369ba4b9d97cee1fd5f) )
	ROM_LOAD( "twc-3.bin",    0x8000, 0x4000, CRC(8b662902) SHA1(13bcd4bf23e34dd7193545561e05bb2cb2c95f9b) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for code */
	ROM_LOAD( "twc-4.bin",    0x0000, 0x8000, CRC(70a9f883) SHA1(ace04359265271eb37512a89eb0217eb013aecb7) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-6.bin",    0x0000, 0x4000, CRC(e3112be2) SHA1(7859e51b4312dc5df01c88e1d97cf608abc7ca72) )

	ROM_REGION( 0x04000, "gfx1", 0 )
	ROM_LOAD( "tkwc-12.bin",  0x00000, 0x4000, CRC(cf5e084a) SHA1(bba909898135467a8cbe8ca7ba7d14254ad52135) )	/* fg tiles */

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "tkwc-8.bin",   0x00000, 0x8000, CRC(068210d1) SHA1(f2f21fa9452ba6d47ccdb4de3ddab833956d3fea) )	/* sprites */
	ROM_LOAD( "tkwc-7.bin",   0x08000, 0x8000, CRC(8e7dca59) SHA1(4ae3d6c0faf99d16db5b3c49ae96aa87741f635c) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "tkwc-11.bin",  0x00000, 0x8000, CRC(1f8536e3) SHA1(de44a066fc5862bac7de039053262ec167b70c35) )	/* bg tiles */
	ROM_LOAD( "tkwc-9.bin",   0x08000, 0x8000, CRC(f88f52ed) SHA1(33fa29582f81f2da7dcb671ccb0c7121b95b545f) )

	ROM_REGION( 0x8000, "adpcm", 0 )	/* ADPCM samples */
	ROM_LOAD( "twc-5.bin",    0x0000, 0x4000, CRC(444b5544) SHA1(0786d6d9ada7fe49c8ab9751b049095474d2e598) )
ROM_END

ROM_START( twc_gr )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-1gr.bin",  0x0000, 0x4000, CRC(0686377f) SHA1(69c4d63c216e0b640ad514568a07c414a94bae7c) )
	ROM_LOAD( "twc-2.bin",    0x4000, 0x4000, CRC(7017a221) SHA1(4b4700af0a6ff64f976db369ba4b9d97cee1fd5f) )
	ROM_LOAD( "twc-3gr.bin",  0x8000, 0x4000, CRC(9a10118d) SHA1(1319e465620ab297bf70aed6eed426b68580a327) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for code */
	ROM_LOAD( "twc-4gr.bin",  0x0000, 0x8000, CRC(8a13d6ac) SHA1(cc9b7a0836d6b4ff5f037a165451f6cf2f967d21) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for code */
	ROM_LOAD( "twc-6.bin",    0x0000, 0x4000, CRC(e3112be2) SHA1(7859e51b4312dc5df01c88e1d97cf608abc7ca72) )

	ROM_REGION( 0x04000, "gfx1", 0 )
	ROM_LOAD( "twc-12gr.bin", 0x00000, 0x4000, CRC(17808225) SHA1(259c0162c4eef4eceb889f4a1a142b541376e04d) )	/* fg tiles */

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "twc-8.bin",    0x00000, 0x8000, CRC(055a5264) SHA1(fe294ba57c2c858952e2fab0be1b8859730846cb) )	/* sprites */
	ROM_LOAD( "twc-7.bin",    0x08000, 0x8000, CRC(59faebe7) SHA1(85dad90928369601e039467d575750539410fcf6) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "twc-11gr.bin", 0x00000, 0x8000, CRC(ceab1e34) SHA1(f803965ef8b69a5951656f890f0e411dfdeef7ab) )	/* bg tiles */
	ROM_LOAD( "twc-9gr.bin",  0x08000, 0x8000, CRC(b120fbee) SHA1(a7012ec3ecbedfb3d04477bdda6f87f1fe819919) )

	ROM_REGION( 0x8000, "adpcm", 0 )	/* ADPCM samples */
	ROM_LOAD( "twc-5.bin",    0x0000, 0x4000, CRC(444b5544) SHA1(0786d6d9ada7fe49c8ab9751b049095474d2e598) )
ROM_END

GAME( 1985, ballonwc, tehkanwc, tehkanwc, tehkanwc, driver_device, 0, ROT0,  "Grand Master Peter", "Ballon World Cup", MACHINE_SUPPORTS_SAVE )
GAME( 1985, tehkangc, tehkanwc, tehkanwc, tehkanwc, driver_device, 0, ROT0,  "Grand Master Peter", "Tehkan Greek Cup", MACHINE_SUPPORTS_SAVE )
GAME( 2003, tankwc,   tehkanwc, tehkanwc, tehkanwc, driver_device, 0, ROT0,  "Kazzy", "Tank World Cup", MACHINE_SUPPORTS_SAVE )
GAME( 2004, twc_gr,   tehkanwc, tehkanwc, tehkanwc, driver_device, 0, ROT0,  "GreekRoms", "Tehkan World Cup (Greek)", MACHINE_SUPPORTS_SAVE )

/* Notes on E602 - Tehkan World Cup (set 3 bootleg) (1986)
	- Year is 1986 instead of 1985 - seems this is possibly the Spanish version
	- Rom 1 is overdumped, after fixing this, noticed last byte changed from 27 to FF
	- 4 bytes in rom 9 changed - this rom makes the gfx on the title screen
	- Could not see any visual or other differences
	- Therefore, game not added */
