// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/segas24.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***********
 Gain Ground
***************/

ROM_START( ggrounds01 )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "epr-12187.ic2", 0x000000, 0x20000, CRC(e83783f3) SHA1(4b3b32df7de85aef9cd77c8a4ffc17e10466b638) )
	ROM_LOAD16_BYTE( "epr-12186.ic1", 0x000001, 0x20000, CRC(ce76319d) SHA1(0ede61f0700f9161285c768fa97636f0e42b96f8) )

	ROM_REGION( 0x2000, "subcpu:key", 0 )
	ROM_LOAD( "317-0058-03d.key", 0x0000, 0x2000,  CRC(e1785bbd) SHA1(b4bebb2829299f1c0815d6a5f317a2526b322f63) )

	ROM_REGION( 0x1c2000, "floppy", 0)
	ROM_LOAD( "ds3-5000-03d-rev-ahc01.img", 0x000000, 0x1c2000, CRC(8e31bbea) SHA1(d419245a52c266630d15dfb6927a7c445f068459) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Gain Ground
GAME( 1988, ggrounds01,   gground,  system24_floppy_fd1094, gground,  segas24_state, init_gground,  ROT270, "Wayder", "Gain Ground (Restore Bug 4-8 Enemy Count)", 0 )

