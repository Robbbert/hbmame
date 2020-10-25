// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/lkage.cpp"

/***************************************
    Proyecto Shadows Mame Build Plus
***************************************/

/******************************************************************************************
   Muchos Roms De Mi Proyecto Shadow Se Estar� Deshabilitado. Por
   Los Motivo Es Que Se Empez� Agregar En El HBMame Oficial, Lo Cual Me Ver�
   Obligado Deshabilitando Porque Si No Se Estar�a Duplicado Archivo De Roms.
******************************************************************************************/

/*****************************************
 *
 *         ROM Definici�n(s)
 *
 ****************************************/

  /****************************************
          The Legend of Kage
*****************************************/

ROM_START( lkages01 )
	ROM_REGION( 0x14000, "maincpu", 0 ) /* Z80 code (main CPU) */
	ROM_LOAD( "a54-01-2hc01.37", 0x0000, 0x8000, CRC(f3d05d57) SHA1(f890af7450890990aa8d82e874446f64a52e922c) )
	ROM_LOAD( "a54-02-2hc01.38", 0x8000, 0x8000, CRC(c9d1220b) SHA1(2b5baa4ea3e169c65053e96b38adc376a33b2b6f) )

	ROM_REGION( 0x10000, "audiocpu", 0 ) /* Z80 code (sound CPU) */
	ROM_LOAD( "a54-04.54",   0x0000, 0x8000, CRC(541faf9a) SHA1(b142ff3bd198f700697ec06ea92db3109ab5818e) )

	ROM_REGION( 0x00800, "bmcu:mcu", 0 ) /* 68705 MCU code */
	ROM_LOAD( "a54-09.53",   0x0000, 0x0800, CRC(0e8b8846) SHA1(a4a105462b0127229bb7edfadd2e581c7e40f1cc) )

	ROM_REGION( 0x4000, "user1", 0 ) /* data */
	ROM_LOAD( "a54-03hc01.51",   0x0000, 0x4000, CRC(61a81448) SHA1(41ae1fd8a46294a92d73f3440788438a4688aeec) )

	ROM_REGION( 0x10000, "gfx1", 0 )
	ROM_LOAD( "a54-05-1hc01.84", 0x0000, 0x4000, CRC(dda3c14b) SHA1(585e569b197b7097813ba2f86ef2e12e694bd838) )
	ROM_LOAD( "a54-06-1hc01.85", 0x4000, 0x4000, CRC(8d7e2a3e) SHA1(8f381dfdb8d6bfeac7b5fb470d441b5b2fbe13f0) )
	ROM_LOAD( "a54-07-1hc01.86", 0x8000, 0x4000, CRC(5e2bf6ae) SHA1(477d14a569d2ffeb97f46b9daaff46c90f0d4ac5) )
	ROM_LOAD( "a54-08-1hc01.87", 0xc000, 0x4000, CRC(d56f9148) SHA1(1695cf41fefb79140d5d08634a62878552b1b18d) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "a54-10.2",    0x0000, 0x0200, CRC(17dfbd14) SHA1(f8f0b6dfedd4ba108dad43ccc7697ef4ab9cbf86) ) /* unknown */

	ROM_REGION( 0x0800, "plds", 0 )
	ROM_LOAD( "pal16l8-a54-11.34",  0x0000, 0x0104, CRC(56232113) SHA1(4cdc6732aa3e7fbe8df51966a1295253711ecc8f) )
	ROM_LOAD( "pal16l8-a54-12.76",  0x0200, 0x0104, CRC(e57c3c89) SHA1(a23f91da254055bb990e8bb730564c40b5725f78) )
	ROM_LOAD( "pal16l8a-a54-13.27", 0x0400, 0x0104, CRC(c9b1938e) SHA1(2fd1adc4bde8f07cf4b6314d56b48bb3d7144cc3) )
	ROM_LOAD( "pal16l8a-a54-14.35", 0x0600, 0x0104, CRC(a89c644e) SHA1(b41a077d1d070d9563f924c776930c33a4ff27d0) )
ROM_END

/*************************************
 *
 *  Game driver(s)
 *
 *************************************/
// Proyecto Shadows Mame Build Plus
/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// The Legend of Kage
GAME( 1984, lkages01, lkage, lkage, lkage, lkage_state, init_lkage, ROT0, "hack", "The Legend of Kage (Translation Chinese)", MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )

