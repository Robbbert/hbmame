// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/snesb.cpp"

/*******************
 Sonic Blast Man 2
********************/

ROM_START( sblast2b01 )
	ROM_REGION( 0x180000, "user3", ROMREGION_ERASEFF )

	// not used
	ROM_REGION(0x100, "sound_ipl", 0)
	ROM_LOAD("spc700.rom", 0, 0x40, CRC(44bb3a40) SHA1(97e352553e94242ae823547cd853eecda55c20f0) )

	// not used
	ROM_REGION(0x800, "user6", ROMREGION_ERASEFF)

	ROM_REGION( 0x180000, "user7", 0 )
	ROM_LOAD( "1.s01", 0x000000, 0x0080000, CRC(b1263f6a) SHA1(707e418121d867dd9baee7a4c2f737f832c9d9e2) )
	ROM_LOAD( "2.bin", 0x080000, 0x0080000, CRC(57d2b6e9) SHA1(1a7b347101f67b254e2f86294d501b0669431644) )
	ROM_LOAD( "3.bin", 0x100000, 0x0080000, CRC(9e63a5ce) SHA1(1d18606fbb28b55a921fc37e1af1aff4caae9003) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Sonic Blast Man 2
GAME( 1997, sblast2b01, sblast2b, sblast2b, sblast2b, snesb_state, init_sblast2b, ROT0, "RockyWall",  "Sonic Blast Man 2 Special Turbo (Level Sequence Repair)", 0 )

