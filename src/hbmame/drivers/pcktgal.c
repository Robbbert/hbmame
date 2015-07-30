// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/pcktgal.c"

ROM_START( pckgalgr )
	ROM_REGION( 0x14000, "maincpu", 0 )	 /* 64k for code + 16k for banks */
	ROM_LOAD( "eb04gr.rom",	   0x10000, 0x4000, CRC(9a7a826c) SHA1(f0c62a760cc05e31568723eaf354a735c28d20ab) )
	ROM_CONTINUE(			   0x04000, 0xc000)
	/* 4000-7fff is banked but code falls through from 7fff to 8000, so */
	/* I have to load the bank directly at 4000. */

	ROM_REGION( 0x18000, "audiocpu", 0 )	 /* 96k for code + 96k for decrypted opcodes */
	ROM_LOAD( "eb03.f2",	   0x10000, 0x8000, CRC(cb029b02) SHA1(fbb3da08ed05ae73fbeeb13e0e2ff735aaf83db8) )
	ROM_CONTINUE(			   0x08000, 0x8000 )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "eb01gr.rom",	   0x00000, 0x10000, CRC(9e5e5743) SHA1(3379bcd36148c37bfc8d2eb2909104b367797856) )
	ROM_LOAD( "eb02gr.rom",	   0x10000, 0x10000, CRC(be11227c) SHA1(c41b10f23ce60829a40851ad024e189bc7e5b07c) )

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "eb00gr.rom",	   0x00000, 0x10000, CRC(5eeb40e8) SHA1(703b2b87954032a383c6f34316df2b5ec6a59488) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "eb05.k14",     0x0000, 0x0200, CRC(3b6198cb) SHA1(d32b364cfce99637998ca83ad21783f80364dd65) ) /* 82s147.084 */
	ROM_LOAD( "eb06.k15",     0x0200, 0x0200, CRC(1fbd4b59) SHA1(84e20329003cf09b849b49e1d83edc330d49f404) ) /* 82s131.101 */
ROM_END

ROM_START( pcktgalk )
	ROM_REGION( 0x14000, "maincpu", 0 )	 /* 64k for code + 16k for banks */
	ROM_LOAD( "eb04.j7",	   0x10000, 0x4000, CRC(8215d60d) SHA1(ac26dfce7e215be21f2a17f864c5e966b8b8322e) )
	ROM_CONTINUE(			   0x04000, 0xc000)
	/* 4000-7fff is banked but code falls through from 7fff to 8000, so */
	/* I have to load the bank directly at 4000. */

	ROM_REGION( 0x18000, "audiocpu", 0 )	 /* 96k for code + 96k for decrypted opcodes */
	ROM_LOAD( "eb03.f2",	   0x10000, 0x8000, CRC(cb029b02) SHA1(fbb3da08ed05ae73fbeeb13e0e2ff735aaf83db8) )
	ROM_CONTINUE(			   0x08000, 0x8000 )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "eb01k.rom",	   0x00000, 0x10000, CRC(3b9f8e29) SHA1(b7ae6d72b9fc1f4964b673346d76a9d24cd5606c) )
	ROM_LOAD( "eb02.d12",	   0x10000, 0x10000, CRC(a9dcd339) SHA1(245824ab86cdfe4b842ce1be0af60f2ff4c6ae07) )

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "eb00.a1",	   0x00000, 0x10000, CRC(6c1a14a8) SHA1(03201197304c5f1d854b8c4f4a5c78336b51f872) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "eb05.k14",     0x0000, 0x0200, CRC(3b6198cb) SHA1(d32b364cfce99637998ca83ad21783f80364dd65) ) /* 82s147.084 */
	ROM_LOAD( "eb06.k15",     0x0200, 0x0200, CRC(1fbd4b59) SHA1(84e20329003cf09b849b49e1d83edc330d49f404) ) /* 82s131.101 */
ROM_END


GAME( 1987, pckgalgr, pcktgal,	pcktgal, pcktgal, pcktgal_state, pcktgal,  ROT0, "GreekRoms", 	"Pocket Gal (Greek)", MACHINE_SUPPORTS_SAVE )
GAME( 1987, pcktgalk, pcktgal,	pcktgal, pcktgal, pcktgal_state, pcktgal,  ROT0, "Hack", 	"Pocket Gal (Korean)", MACHINE_SUPPORTS_SAVE )
