// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/igs011.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***************
 Long Hu Bang II
******************/

ROM_START( lhb2s01 ) //lhb2c
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "maj2v185hhc01.u29", 0x00000, 0x80000, CRC(16a6f6de) SHA1(b5b463ce68da9541c6dd68d99a7469ab4cca3f78) )

	ROM_REGION( 0x200000, "blitter", 0 )
	ROM_LOAD( "igsm0501.u7", 0x00000, 0x200000, CRC(1c952bd6) SHA1(a6b6f1cdfb29647e81c032ffe59c94f1a10ceaf8) )

	ROM_REGION( 0x80000, "blitter_hi", 0 )
	ROM_LOAD( "igsm0502.u4", 0x00000, 0x80000, CRC(5d73ae99) SHA1(7283aa3d6b15ceb95db80756892be46eb997ef15) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "igss0503.u38", 0x00000, 0x80000, CRC(c9609c9c) SHA1(f036e682b792033409966e84292a69275eaa05e5) )
ROM_END

 /***************
 Wanli Changcheng
******************/

ROM_START( wlccs01 ) //wlccc
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "wlcc4096_ps01.rom", 0x00000, 0x80000, CRC(3891db63) SHA1(935d6af042d5871d67ff3381ea654427b6bc30ee) )
	ROM_CONTINUE        (                 0x00000, 0x80000 )

	ROM_REGION( 0x280000, "blitter", 0 )
	ROM_LOAD( "m0201-ig.160", 0x000000, 0x200000, CRC(ec54452c) SHA1(0ee7ffa3d4845af083944e64faf5a1c78247aaa2) )
	ROM_LOAD( "wlcc.gfx",     0x200000, 0x080000, CRC(1f7ad299) SHA1(ab0a8fb31906519b9352ba172def48456e8d565c) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "m0202.snd", 0x00000, 0x80000, CRC(220949aa) SHA1(1e0dba168a0687d32aaaed42714ae24358f4a3e7) )
	ROM_CONTINUE(             0x00000, 0x80000 )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Long Hu Bang II
GAME( 1996, lhb2s01, lhb2, lhb2, lhb2, igs011_state, init_lhb2, ROT0, "whistler_wmz", "Long Hu Bang II (No AI Cheat)",   MACHINE_SUPPORTS_SAVE )
// Wanli Changcheng
GAME( 2006, wlccs01, xymg, wlcc, wlcc, igs011_state, init_wlcc, ROT0, "hack",          "Wanli Changcheng (No AI Cheat 2006-01-01)", MACHINE_SUPPORTS_SAVE )
