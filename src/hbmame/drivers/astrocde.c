// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/astrocde.c"


//READ8_HANDLER(gorf_speech_r) { return 0; }
//READ8_HANDLER(wow_speech_r) { return 0; }
//CUSTOM_INPUT(gorf_speech_status_r) { return 0; }
//CUSTOM_INPUT(wow_speech_status_r) { return 0; }

//const char *const gorf_sample_names[] = { 0, 0 } ;
//const char *const wow_sample_names[] = { 0, 0 } ;

ROM_START( tst_gorf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gorfmd.bin",   0x0000, 0x1000, CRC(3776a6a1) SHA1(bd6b0a1dd587e8d4b325ef4133b8e3797416d8d2) )
ROM_END

ROM_START( tst_gor1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gorftest.bin", 0x0000, 0x0627, CRC(676137a2) SHA1(d99192a16add7b188b29db7f9497b6c129d81144) )
ROM_END

ROM_START( tst_wow2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "wowmd02b.x1",  0x0000, 0x1000, CRC(0af2f2c8) SHA1(e883afe696c2a444c8691041f1b9e0e015dff517) )
ROM_END

ROM_START( tst_wow3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "wowmd030.x1",  0x0000, 0x1000, CRC(f493ec4d) SHA1(030cd12e7a7e9ac346e8299df83b62480004c954) )
ROM_END

GAME( 2005, tst_gorf, 0, gorf, gorf, astrocde_state, gorf, ROT270, "David Turner", "Test Rom - Gorf", MACHINE_SUPPORTS_SAVE )
GAME( 2006, tst_gor1, 0, gorf, gorf, astrocde_state, gorf, ROT270, "Mark Spaeth", "Test Rom - Gorf Complete Memory Diagnostics", MACHINE_SUPPORTS_SAVE )
GAME( 2003, tst_wow2, 0, wow,  wow,  astrocde_state, wow,  ROT0,   "David Turner", "Test Rom - Wizard of Wor (ver 0.2b)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, tst_wow3, 0, wow,  wow,  astrocde_state, wow,  ROT0,   "David Turner", "Test Rom - Wizard of Wor (ver 0.3)", MACHINE_SUPPORTS_SAVE )
