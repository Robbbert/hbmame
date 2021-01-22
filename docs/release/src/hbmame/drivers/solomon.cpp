// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/solomon.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***************
 Solomon no Kagi
******************/

ROM_START( solomonjs01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "slmn_06hc01.bin",  0x00000, 0x4000, CRC(bbc0d319) SHA1(2d3ff5f640e691c4147f8b6fe97bc5e1fefdac85) )
	ROM_LOAD( "slmn_07hc01.bin",  0x08000, 0x4000, CRC(2465a83a) SHA1(bd7212205e353d221b97ced4dddcb6d339c1c3c0) )
	ROM_CONTINUE(             0x04000, 0x4000 )
	ROM_LOAD( "slmn_08.bin",  0x0f000, 0x1000, CRC(b924d162) SHA1(6299b791ec874bc3ef0424b277ec8a736c8cdd9a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "1.3jk",  0x0000, 0x4000, CRC(fa6e562e) SHA1(713036c0a80b623086aa674bb5f8a135b6fedb01) )

	ROM_REGION( 0x10000, "gfx1", 0 )
	ROM_LOAD( "slmn_12hc01.bin",  0x00000, 0x08000, CRC(e80594fc) SHA1(f6617f76e287d683b5b2de97c2cd8c452ad234bf) )
	ROM_LOAD( "slmn_11hc01.bin",  0x08000, 0x08000, CRC(9dab2e9a) SHA1(63a96425394b312e07022aba34fbb36836161994) )

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "slmn_10hc01.bin",  0x00000, 0x08000, CRC(cf11872e) SHA1(91546375057d67f94ed3db6418112825706c2ac5) )
	ROM_LOAD( "slmn_09hc01.bin",  0x08000, 0x08000, CRC(60122e98) SHA1(5947dc82bda6065c1103107f1137183dca0847de) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "slmn_02hc01.bin",  0x00000, 0x04000, CRC(bb5c5400) SHA1(cc7aa14cdcc5940319f091aee821ce272ca1db42) )
	ROM_LOAD( "slmn_03hc01.bin",  0x04000, 0x04000, CRC(66137d2b) SHA1(a010c93adb98580fb9a8681e2e0d67dfee740d0b) )
	ROM_LOAD( "slmn_04hc01.bin",  0x08000, 0x04000, CRC(33299a3a) SHA1(5e25008f437618180c6296314568d62bd038cc8a) )
	ROM_LOAD( "slmn_05hc01.bin",  0x0c000, 0x04000, CRC(5856d9b0) SHA1(509ea27714bbeed72915d854f05a29693a85575c) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Solomon no Kagi
GAME( 1986, solomonjs01, solomon, solomon, solomon, solomon_state, empty_init, ROT0, "hack", "Solomon no Kagi (Translation Chinese)", MACHINE_SUPPORTS_SAVE )

