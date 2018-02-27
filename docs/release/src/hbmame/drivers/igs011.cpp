// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/igs011.cpp"

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
           Long Hu Bang II
*****************************************/

ROM_START( lhb2s01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "maj2v185hhc01.u29", 0x00000, 0x80000, CRC(16a6f6de) SHA1(b5b463ce68da9541c6dd68d99a7469ab4cca3f78) )

	ROM_REGION( 0x200000, "blitter", 0 )
	ROM_LOAD( "igsm0501.u7", 0x00000, 0x200000, CRC(1c952bd6) SHA1(a6b6f1cdfb29647e81c032ffe59c94f1a10ceaf8) )

	ROM_REGION( 0x80000, "blitter_hi", 0 ) // high order bit of graphics (5th bit)
	/* these are identical ..seems ok as igs number is same, only ic changed */
	ROM_LOAD( "igsm0502.u4", 0x00000, 0x80000, CRC(5d73ae99) SHA1(7283aa3d6b15ceb95db80756892be46eb997ef15) )
	ROM_LOAD( "igsm0502.u5", 0x00000, 0x80000, CRC(5d73ae99) SHA1(7283aa3d6b15ceb95db80756892be46eb997ef15) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "igss0503.u38", 0x00000, 0x80000, CRC(c9609c9c) SHA1(f036e682b792033409966e84292a69275eaa05e5) )  // 2 banks
ROM_END

/*************************************
 *
 *  Game driver(s)
 *
 *************************************/
// Proyecto Shadows Mame Build Plus
/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Long Hu Bang II
GAME( 1996, lhb2s01,         lhb2,        lhb2,            lhb2,      igs011_state, lhb2,         ROT0, "Hacks",                     "Long Hu Bang II (Hong Kong, V185H)(No AI Cheat)",   MACHINE_SUPPORTS_SAVE )