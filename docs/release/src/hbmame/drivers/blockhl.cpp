// license:GPL_2.0
// copyright-holders:Robbbert

#include "../mame/drivers/blockhl.cpp"

ROM_START( quarth01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "quarth01.e21", 0x00000, 0x10000, CRC(e67357b9) SHA1(1c4a16a3db5e441604414ae4755cf9ef95139d95) )

	ROM_REGION( 0x08000, "audiocpu", 0 )
	ROM_LOAD( "973d01.g6",  0x00000, 0x08000, CRC(eeee9d92) SHA1(6c6c324b1f6f4fba0aa12e0d1fc5dbab133ef669) )

	ROM_REGION( 0x20000, "k052109", 0 )
	ROM_LOAD32_BYTE( "973e07.k15", 0x00000, 0x08000, CRC(0bd6b0f8) SHA1(6c59cf637354fe2df424eaa89feb9c1bc1f66a92) )
	ROM_LOAD32_BYTE( "973e08.k18", 0x00001, 0x08000, CRC(104d0d5f) SHA1(595698911513113d01e5b565f5b073d1bd033d3f) )
	ROM_LOAD32_BYTE( "973e09.k20", 0x00002, 0x08000, CRC(bd3a6f24) SHA1(eb45db3a6a52bb2b25df8c2dace877e59b4130a6) )
	ROM_LOAD32_BYTE( "973e10.k23", 0x00003, 0x08000, CRC(cf5e4b86) SHA1(43348753894c1763b26dbfc70245dac92048db8f) )

	ROM_REGION( 0x20000, "k051960", 0 )
	ROM_LOAD32_BYTE( "973e06.k12", 0x00000, 0x08000, CRC(0d58af85) SHA1(2efd661d614fb305a14cfe1aa4fb17714f215d4f) )
	ROM_LOAD32_BYTE( "973e05.k9",  0x00001, 0x08000, CRC(15d822cb) SHA1(70ecad5e0a461df0da6e6eb23f43a7b643297f0d) )
	ROM_LOAD32_BYTE( "973e04.k7",  0x00002, 0x08000, CRC(d70f4a2c) SHA1(25f835a17bacf2b8debb2eb8a3cff90cab3f402a) )
	ROM_LOAD32_BYTE( "973e03.k4",  0x00003, 0x08000, CRC(2c5a4b4b) SHA1(e2991dd78b9cd96cf93ebd6de0d4e060d346ab9c) )
ROM_END


//**************************************************************************
//  GAME DRIVERS
//**************************************************************************

//    YEAR  NAME     PARENT   MACHINE  INPUT    CLASS          INIT        ROT   COMPANY   FULLNAME          FLAGS
GAME( 2023, quarth01,  blockhl, blockhl, blockhl, blockhl_state, empty_init, ROT0, "Paul Goes", "Quarth (Cooperative, 2023-02-04)", MACHINE_SUPPORTS_SAVE )

