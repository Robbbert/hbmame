// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/kikikai.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/
 
 /***********
 KiKi KaiKai
**************/

ROM_START( kikikais01 )
	ROM_REGION( 0x28000, "maincpu", 0 )
	ROM_LOAD( "a85-17hc01.h16", 0x00000, 0x08000, CRC(2a13c079) SHA1(0c7b9a4598d5add28edca306a060268945aa8923) )
	ROM_CONTINUE(           0x18000, 0x08000 )
	ROM_LOAD( "a85-16.h18", 0x08000, 0x10000, CRC(4094d750) SHA1(05e0ad177a3eb144b203784ecb6242a0fc5c4d4d) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "a85-11.f6", 0x0000, 0x8000, CRC(cc3539db) SHA1(4239a40fdee65cba613e4b4ec54cf7899480e366) )

	ROM_REGION( 0x800, "mcu", 0 )
	ROM_LOAD( "a85-01.g8",    0x0000, 0x0800, NO_DUMP )

	ROM_REGION( 0x40000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "a85-15hc01.a1", 0x00000, 0x10000, CRC(966b8bd1) SHA1(77fa4215499ab65ea5c513a5b15b6613ac6cdb2b) )
	ROM_LOAD( "a85-14hc01.a3", 0x10000, 0x10000, CRC(d8715990) SHA1(8d729aaf508a3cb5fb53991ef2fde82df531344e) )
	ROM_LOAD( "a85-13hc01.a4", 0x20000, 0x10000, CRC(c9bf0da1) SHA1(c6ade23be083fdcf3b15b12a8f2860affec1d014) )
	ROM_LOAD( "a85-12hc01.a6", 0x30000, 0x10000, CRC(29ab45e0) SHA1(cfff982d7f64fad8516474801e28f62f5982fd67) )

	ROM_REGION( 0x300, "proms", 0 )
	ROM_LOAD( "a85-08.g15", 0x0000, 0x0100, CRC(d15f61a8) SHA1(945c8aa26c85269c10373218bef13e04e25eb1e4) )
	ROM_LOAD( "a85-10.g12", 0x0100, 0x0100, CRC(8fc3fa86) SHA1(d4d86f8e147bbf2a370de428ac20a28b0f146782) )
	ROM_LOAD( "a85-09.g14", 0x0200, 0x0100, CRC(b931c94d) SHA1(fb554084f34c602d1ff7806fb945a06cf14332af) )
ROM_END

/*    YEAR  NAME         PARENT      MACHINE   INPUT     CLASS                       INIT      MONITOR COMPANY                 FULLNAME               FLAGS */
// KiKi KaiKai
GAME( 1986, kikikais01,  kikikai,    kikikai,  kikikai,  kikikai_simulation_state, empty_init, ROT90,   "hack",  "KiKi KaiKai (Translation Chinese)", MACHINE_SUPPORTS_SAVE )

