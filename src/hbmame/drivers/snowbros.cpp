// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/snowbros.cpp"
#include "../mame/video/kan_pand.cpp"

/***************************************
	Proyecto Shadows Mame Build Plus
***************************************/

/******************************************************************************************
   Muchos Roms De Mi Proyecto Shadow Se Estará Deshabilitado. Por
   Los Motivo Es Que Se Empezó Agregar En El HBMame Oficial, Lo Cual Me Veré
   Obligado Deshabilitando Porque Si No Se Estaría Duplicado Archivo De Roms.
******************************************************************************************/

/*****************************************
 *
 *         ROM Definición(s)
 *
 ****************************************/
 
  /****************************************
        Snow Bros. - Nick & Tom
*****************************************/

ROM_START( snowbross01 )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "sn6hc01.bin",  0x00000, 0x20000, CRC(effe7b36) SHA1(07d34f0e76dc2dfa879c517374e61ee77dab6208) )
	ROM_LOAD16_BYTE( "sn5hc01.bin",  0x00001, 0x20000, CRC(cdd895de) SHA1(7cd64bca3e5e5d9d8fc05b5a846276f0a343fd9c) )

	ROM_REGION( 0x10000, "soundcpu", 0 )    /* 64k for z80 sound code */
	ROM_LOAD( "sbros-4.29",   0x0000, 0x8000, CRC(e6eab4e4) SHA1(d08187d03b21192e188784cb840a37a7bdb5ad32) )

	ROM_REGION( 0x80000, "gfx1", 0 )
	ROM_LOAD( "sbros-1.41",   0x00000, 0x80000, CRC(16f06b3a) SHA1(c64d3b2d32f0f0fcf1d8c5f02f8589d59ddfd428) )
	/* where were these from, a bootleg? */
//  ROM_LOAD( "ch0",          0x00000, 0x20000, CRC(36d84dfe) SHA1(5d45a750220930bc409de30f19282bb143fbf94f) )
//  ROM_LOAD( "ch1",          0x20000, 0x20000, CRC(76347256) SHA1(48ec03965905adaba5e50eb3e42a2813f7883bb4) )
//  ROM_LOAD( "ch2",          0x40000, 0x20000, CRC(fdaa634c) SHA1(1271c74df7da7596caf67caae3c51b4c163a49f4) )
//  ROM_LOAD( "ch3",          0x60000, 0x20000, CRC(34024aef) SHA1(003a9b9ee3aaab3d787894d3d4126d372b19d2a8) )
ROM_END

ROM_START( snowbross02 )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "sn6hc02.bin",  0x00000, 0x20000, CRC(f756cd9d) SHA1(99a43d9233501c1c28cf74829138527bf990a3d7) )
	ROM_LOAD16_BYTE( "sn5hc02.bin",  0x00001, 0x20000, CRC(cdaa04ed) SHA1(d2bde893aa82b84783712c3156aa7f23322f76bf) )

	ROM_REGION( 0x10000, "soundcpu", 0 )    /* 64k for z80 sound code */
	ROM_LOAD( "sbros-4.29",   0x0000, 0x8000, CRC(e6eab4e4) SHA1(d08187d03b21192e188784cb840a37a7bdb5ad32) )

	ROM_REGION( 0x80000, "gfx1", 0 )
	ROM_LOAD( "sbros-1.41",   0x00000, 0x80000, CRC(16f06b3a) SHA1(c64d3b2d32f0f0fcf1d8c5f02f8589d59ddfd428) )
	/* where were these from, a bootleg? */
//  ROM_LOAD( "ch0",          0x00000, 0x20000, CRC(36d84dfe) SHA1(5d45a750220930bc409de30f19282bb143fbf94f) )
//  ROM_LOAD( "ch1",          0x20000, 0x20000, CRC(76347256) SHA1(48ec03965905adaba5e50eb3e42a2813f7883bb4) )
//  ROM_LOAD( "ch2",          0x40000, 0x20000, CRC(fdaa634c) SHA1(1271c74df7da7596caf67caae3c51b4c163a49f4) )
//  ROM_LOAD( "ch3",          0x60000, 0x20000, CRC(34024aef) SHA1(003a9b9ee3aaab3d787894d3d4126d372b19d2a8) )
ROM_END

/*************************************
 *
 *  Game driver(s)
 *
 *************************************/
// Proyecto Shadows Mame Build Plus
/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Snow Bros. - Nick & Tom
GAME( 1990, snowbross01,   snowbros,        snowbros,    snowbros, snowbros_state, 0,    ROT0, "Hacks",                        "Snow Bros. - Nick & Tom (Ex Super Version)", MACHINE_SUPPORTS_SAVE )
GAME( 1990, snowbross02,   snowbros,        snowbros,    snowbros, snowbros_state, 0,    ROT0, "Hacks",                        "Snow Bros. - Nick & Tom (Snowman Brothers Cure Version 2.6A)", MACHINE_SUPPORTS_SAVE )