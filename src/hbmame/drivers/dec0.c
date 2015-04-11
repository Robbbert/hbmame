#include "../mame/drivers/dec0.c"

ROM_START( baddudef )
	ROM_REGION( 0x60000, "maincpu", 0 )	/* 6*64k for 68000 code, middle 0x20000 unused */
	ROM_LOAD16_BYTE( "baddudef.4",   0x00000, 0x10000, CRC(1a0667ae) SHA1(a530bfe91aa8b489ef7c1368b81983e760d0d993) )
	ROM_LOAD16_BYTE( "baddudef.1",   0x00001, 0x10000, CRC(14d71614) SHA1(282fe93c04791e48a963ee78e69753e54c51e779) )
	ROM_LOAD16_BYTE( "ei06.6c",   0x40000, 0x10000, CRC(3ff8da57) SHA1(eea8125a3eac33d76d22e72b69633eaae138efe5) )
	ROM_LOAD16_BYTE( "ei03.6a",   0x40001, 0x10000, CRC(f8f2bd94) SHA1(622c66fea00cabb2cce16bf621b07d38a660708d) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* Sound CPU */
	ROM_LOAD( "ei07.8a",   0x8000, 0x8000, CRC(9fb1ef4b) SHA1(f4dd0773be93c2ad8b0faacd12939c531b5aa130) )

	ROM_REGION( 0x10000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "ei25.15j",  0x00000, 0x08000, CRC(bcf59a69) SHA1(486727e19c12ea55b47e2ef773d0d0471cf50083) )
	ROM_LOAD( "baddudef.26",  0x08000, 0x08000, CRC(bb8d93d0) SHA1(64a462c86cf270b3aa3e359b0f423a816e9dafb6) )

	ROM_REGION( 0x40000, "gfx2", 0 ) /* tiles */
	ROM_LOAD( "ei18.14d",  0x00000, 0x10000, CRC(05cfc3e5) SHA1(a0163921c77dc9706463a402c3dd45ec4341cd21) )
	ROM_LOAD( "ei20.17d",  0x10000, 0x10000, CRC(e11e988f) SHA1(0c59f0d8d1abe414c7e1ebd49d454179fed2cd00) )
	ROM_LOAD( "ei22.14f",  0x20000, 0x10000, CRC(b893d880) SHA1(99e228174677f2e3e96154f77bfa9bf0f1c0a6a5) )
	ROM_LOAD( "ei24.17f",  0x30000, 0x10000, CRC(6f226dda) SHA1(65ebb16a292c57d49c135fce7ed7537146226eb5) )

	ROM_REGION( 0x20000, "gfx3", 0 ) /* tiles */
	ROM_LOAD( "ei30.9j",   0x08000, 0x08000, CRC(982da0d1) SHA1(d819a587905624d793988f2ea726783da527d9f2) )
	ROM_CONTINUE(          0x00000, 0x08000 )   /* the two halves are swapped */
	ROM_LOAD( "ei28.9f",   0x18000, 0x08000, CRC(f01ebb3b) SHA1(1686690cb0c87d9e687b2abb4896cf285ab8378f) )
	ROM_CONTINUE(          0x10000, 0x08000 )

	ROM_REGION( 0x80000, "gfx4", 0 ) /* sprites */
	ROM_LOAD( "ei15.16c",  0x00000, 0x10000, CRC(a38a7d30) SHA1(5cb1fb97605829fc733c79a7e169fa52adc6863b) )
	ROM_LOAD( "ei16.17c",  0x10000, 0x08000, CRC(17e42633) SHA1(405f5296a741901677cca978a1b287d894eb1e54) )
	ROM_LOAD( "ei11.16a",  0x20000, 0x10000, CRC(3a77326c) SHA1(4de81752329cde6210a9c250a9f8ebe3dad9fe92) )
	ROM_LOAD( "ei12.17a",  0x30000, 0x08000, CRC(fea2a134) SHA1(525dd5f48993db1fe1e3c095442884178f75e8e0) )
	ROM_LOAD( "ei13.13c",  0x40000, 0x10000, CRC(e5ae2751) SHA1(4e4a3c68b11e9b0c8da70121b23296128063d4e9) )
	ROM_LOAD( "ei14.14c",  0x50000, 0x08000, CRC(e83c760a) SHA1(d08db381658b8b3288c5eaa9048a906126e0f712) )
	ROM_LOAD( "ei09.13a",  0x60000, 0x10000, CRC(6901e628) SHA1(1162c8cee20450780774cad54a9af40ebf0f0826) )
	ROM_LOAD( "ei10.14a",  0x70000, 0x08000, CRC(eeee8a1a) SHA1(2bf8378ff38f6a7c7cbd4cbd489de25cb1f0fe71) )

	ROM_REGION( 0x40000, "oki", 0 ) /* ADPCM samples */
	ROM_LOAD( "ei08.2c",   0x0000, 0x10000, CRC(3c87463e) SHA1(f17c98507b562e91e9b27599614b3249fe68ff7a) )
ROM_END

// There are 3 screens. Press 1 very lightly to advance.
// On first screen, press Left & Right to move 'CHAOS' around.
ROM_START( decodemo )
	ROM_REGION( 0x60000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "decodemo.4",   0x00000, 0x10000, CRC(7cb53ab2) SHA1(c06f8173f5b27b647b186e1c3a198ec22dab4950) )
	ROM_LOAD16_BYTE( "decodemo.1",   0x00001, 0x10000, CRC(920dd0bf) SHA1(e69e60121812716b55f4e9c4b3f4993171c5296a) )
	ROM_LOAD16_BYTE( "decodemo.6",   0x40000, 0x10000, CRC(d7978eeb) SHA1(1adc95bebe9eea8c112d40cd04ab7a8d75c4f961) )
	ROM_LOAD16_BYTE( "decodemo.3",   0x40001, 0x10000, CRC(d7978eeb) SHA1(1adc95bebe9eea8c112d40cd04ab7a8d75c4f961) )

	// The instruction [33C0 0030 C003] [move.w D0, $30C003.l] is considered illegal, we need to patch out all 3 occurrences
	//ROM_FILL( 0x04ba, 1, 0x4e )
	//ROM_FILL( 0x04bb, 1, 0x71 )			/* write a nop instruction [4e71] */
	//ROM_COPY( "maincpu", 0x04ba, 0x04bc, 2 )	/* The faulty instruction is 6 bytes long, so we need 3 nops */
	//ROM_COPY( "maincpu", 0x04ba, 0x04be, 2 )
	//ROM_COPY( "maincpu", 0x04ba, 0x04e8, 6 )	/* copy the 3 nops to 0x4e8 */
	//ROM_COPY( "maincpu", 0x04ba, 0x096e, 6 )	/* and 0x96e */

	// Text stopped working when bac06 device was rewritten. We send a 1 to pf_control_0 to fix it.
	ROM_FILL( 0x04d8, 1, 0x33 )
	ROM_FILL( 0x04d9, 1, 0xfc )
	ROM_FILL( 0x04da, 6, 0x00 )
	ROM_FILL( 0x04db, 1, 0x01 )
	ROM_FILL( 0x04dd, 1, 0x24 )
	ROM_FILL( 0x04e0, 1, 0x67 )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* Sound CPU */
	ROM_LOAD( "ei07.8a",   0x8000, 0x8000, CRC(9fb1ef4b) SHA1(f4dd0773be93c2ad8b0faacd12939c531b5aa130) )

	ROM_REGION( 0x10000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "decodemo.25",  0x00000, 0x08000, CRC(011ffca6) SHA1(5188431849b4613152fd7bdba6a3ff0a4fd6424b) )
	ROM_LOAD( "decodemo.26",  0x08000, 0x08000, CRC(14b56f44) SHA1(66c294b27400d3f3cafd7b1f633681df7195a14b) )

	ROM_REGION( 0x40000, "gfx2", 0 ) /* tiles */
	ROM_LOAD( "decodemo.18",  0x00000, 0x10000, CRC(53ac4058) SHA1(b6bd668085a6f2c21c66f38955e27ab384a5ebf1) )
	ROM_LOAD( "decodemo.20",  0x10000, 0x10000, CRC(64f1ca83) SHA1(a8922226dc4f67873c0eb32a0da5e920b9e08e49) )
	ROM_LOAD( "decodemo.22",  0x20000, 0x10000, CRC(e93c5aef) SHA1(bd31b2418a3e3c76c0018d43e9b106895218dae7) )
	ROM_LOAD( "decodemo.24",  0x30000, 0x10000, CRC(ae2556dc) SHA1(ffcc1f7e8ec25ba35d20cedb6cb4010ea9ff409d) )

	ROM_REGION( 0x20000, "gfx3", 0 ) /* tiles */
	ROM_LOAD( "decodemo.30",  0x08000, 0x08000, CRC(9b02b8f8) SHA1(665764ac534ad370545f33d210846e69db558f6f) )
	ROM_CONTINUE(             0x00000, 0x08000 )	/* the two halves are swapped */
	ROM_LOAD( "decodemo.28",  0x18000, 0x08000, CRC(596b92f2) SHA1(a15f4435a3dcfa7d6a45610fccb8607ae9b8fa19) )
	ROM_CONTINUE(             0x10000, 0x08000 )

	ROM_REGION( 0x80000, "gfx4", 0 ) /* sprites */
	ROM_LOAD( "decodemo.15",  0x00000, 0x10000, CRC(4a051829) SHA1(ed560a3ffb9b1d83c3291145e0e938016dd1fa5a) )
	ROM_LOAD( "decodemo.16",  0x10000, 0x08000, CRC(011ffca6) SHA1(5188431849b4613152fd7bdba6a3ff0a4fd6424b) )
	ROM_LOAD( "decodemo.11",  0x20000, 0x10000, CRC(d7978eeb) SHA1(1adc95bebe9eea8c112d40cd04ab7a8d75c4f961) )
	ROM_LOAD( "decodemo.12",  0x30000, 0x08000, CRC(011ffca6) SHA1(5188431849b4613152fd7bdba6a3ff0a4fd6424b) )
	ROM_LOAD( "decodemo.13",  0x40000, 0x10000, CRC(a58d1d9e) SHA1(ebd22872e0230da23d0e63ef7fbe2b5988cd93bd) )
	ROM_LOAD( "decodemo.14",  0x50000, 0x08000, CRC(011ffca6) SHA1(5188431849b4613152fd7bdba6a3ff0a4fd6424b) )
	ROM_LOAD( "decodemo.9",   0x60000, 0x10000, CRC(4d1de58f) SHA1(1979fb6d9cdc25033c75fd3f0c30fb56e040ddc0) )
	ROM_LOAD( "decodemo.10",  0x70000, 0x08000, CRC(011ffca6) SHA1(5188431849b4613152fd7bdba6a3ff0a4fd6424b) )

	ROM_REGION( 0x40000, "oki", 0 ) /* ADPCM samples */
	ROM_LOAD( "ei08.2c",   0x0000, 0x10000, CRC(3c87463e) SHA1(f17c98507b562e91e9b27599614b3249fe68ff7a) )
ROM_END

GAME( 2007, baddudef, baddudes, baddudes, baddudes, dec0_state, baddudes, ROT0, "Arkatrad", "Bad Dudes vs. Dragonninja (French)", GAME_SUPPORTS_SAVE )
GAME( 1996, decodemo, baddudes, baddudes, baddudes, dec0_state, baddudes, ROT0, "Charles Doty", "Demo - Data East", GAME_SUPPORTS_SAVE )
