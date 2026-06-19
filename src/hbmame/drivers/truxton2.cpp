// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/toaplan/truxton2.cpp"

/****************
 Truxton II
*******************/
// Tatsujin Ou New Version https://shmups.system11.org/viewtopic.php?f=1&t=68027
ROM_START( truxton2s01 ) // truxton2nv
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD( "s01.1", 0x000000, 0x080000, CRC(599ed4e3) SHA1(43176fbd6440fe7baa00c02a763b641ea2581a4e) )

	ROM_REGION( 0x200000, "gp9001", 0 )
	ROM_LOAD( "tp024_4.bin", 0x000000, 0x100000, CRC(805c449e) SHA1(fdf985344145bd320b88b9b0c25e73066c9b2ada) )
	ROM_LOAD( "tp024_3.bin", 0x100000, 0x100000, CRC(47587164) SHA1(bac493e2d5507286b984957b289c929335d27eaa) )

	ROM_REGION( 0x80000, "oki", 0 )         /* ADPCM Samples */
	ROM_LOAD( "tp024_2.bin", 0x00000, 0x80000, CRC(f2f6cae4) SHA1(bb4e8c36531bed97ced4696ca12fd40ede2531aa) )
ROM_END


/*    YEAR  NAME          PARENT           MACHINE   INPUT     CLASS           INIT           MONITOR COMPANY       FULLNAME FLAGS */
// Truxton II
GAME( 2021, truxton2s01,  truxton2,        truxton2, truxton2, truxton2_state, empty_init,    ROT270, "mycophobia", "Truxton II / Tatsujin Oh (New Version)", MACHINE_SUPPORTS_SAVE )

