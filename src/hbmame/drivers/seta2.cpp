// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/seta2.cpp"

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*********
 Guardians
************/

ROM_START( grdianss01 )
	ROM_REGION( 0x200000, "maincpu", 0 )   
	ROM_LOAD16_BYTE( "u2_po01.bin", 0x000000, 0x080000, CRC(90b1aa01) SHA1(c837230f1a14f6097ad3474dad5fb18d18f4a917) )
	ROM_LOAD16_BYTE( "u3_po01.bin", 0x000001, 0x080000, CRC(dbaabf39) SHA1(45e483675883512e5dd3a3baf8b867da77a96eb3) )
	ROM_LOAD16_BYTE( "u4.bin", 0x100000, 0x080000, CRC(bb52447b) SHA1(61433f683210ab2bc2cf1cc4b5b7a39cc5b6493d) )
	ROM_LOAD16_BYTE( "u5.bin", 0x100001, 0x080000, CRC(9c164a3b) SHA1(6d688c7af9e7e8e8d54b2e4dfbf41f59c79242eb) )

	ROM_REGION( 0x2000000, "sprites", ROMREGION_ERASE)  
	ROM_LOAD( "u16.bin",  0x0000000, 0x400000, CRC(6a65f265) SHA1(6cad11f718f8bbcff464d41eb4717460769237ed) )
	ROM_LOAD( "u20.bin",  0x0600000, 0x200000, CRC(a7226ab7) SHA1(408580dd35c568ffef1ebbd87359e3ec1f867020) )
	ROM_CONTINUE(         0x0400000, 0x200000 )

	ROM_LOAD( "u15.bin",  0x0800000, 0x400000, CRC(01672dcd) SHA1(f61f60e3343cc5b6ccee391ee529966a141566db) )
	ROM_LOAD( "u19.bin",  0x0e00000, 0x200000, CRC(c0c998a0) SHA1(498fb1877527ed37412537f06a2c39ff0c60f146) )
	ROM_CONTINUE(         0x0c00000, 0x200000 )

	ROM_LOAD( "u18.bin",  0x1000000, 0x400000, CRC(967babf4) SHA1(42a6311576417c44aeaceb8ba6bb3cd7794e4882) )
	ROM_LOAD( "u22.bin",  0x1600000, 0x200000, CRC(6239997a) SHA1(87b6d6f30f152f625f82fd858c1290176c7e156e) )
	ROM_CONTINUE(         0x1400000, 0x200000 )

	ROM_LOAD( "u17.bin",  0x1800000, 0x400000, CRC(0fad0629) SHA1(1bdc8e7c5e39e83d327f14a672ec81b049112da6) )
	ROM_LOAD( "u21.bin",  0x1e00000, 0x200000, CRC(6f95e466) SHA1(28482fad16a3ac9302f152d81552e6f84a44f3e4) )
	ROM_CONTINUE(         0x1c00000, 0x200000 )

	ROM_REGION( 0x200000, "x1snd", 0 )
	ROM_LOAD( "u32.bin", 0x100000, 0x100000, CRC(cf0f3017) SHA1(8376d3a674f71aec72f52c72758fbc53d9feb1a1) )
ROM_END

ROM_START( grdianss02 )
	ROM_REGION( 0x200000, "maincpu", 0 )   
	ROM_LOAD16_BYTE( "u2_po02.bin", 0x000000, 0x080000, CRC(9ad3bcde) SHA1(677125dd4a9c8b7324ab2a9d0cfef9c6990fbcf1) )
	ROM_LOAD16_BYTE( "u3_po02.bin", 0x000001, 0x080000, CRC(96c804e1) SHA1(023c51ccbe74da1c892252c14369215587315e52) )
	ROM_LOAD16_BYTE( "u4.bin", 0x100000, 0x080000, CRC(bb52447b) SHA1(61433f683210ab2bc2cf1cc4b5b7a39cc5b6493d) )
	ROM_LOAD16_BYTE( "u5.bin", 0x100001, 0x080000, CRC(9c164a3b) SHA1(6d688c7af9e7e8e8d54b2e4dfbf41f59c79242eb) )

	ROM_REGION( 0x2000000, "sprites", ROMREGION_ERASE)  
	ROM_LOAD( "u16.bin",  0x0000000, 0x400000, CRC(6a65f265) SHA1(6cad11f718f8bbcff464d41eb4717460769237ed) )
	ROM_LOAD( "u20.bin",  0x0600000, 0x200000, CRC(a7226ab7) SHA1(408580dd35c568ffef1ebbd87359e3ec1f867020) )
	ROM_CONTINUE(         0x0400000, 0x200000 )

	ROM_LOAD( "u15.bin",  0x0800000, 0x400000, CRC(01672dcd) SHA1(f61f60e3343cc5b6ccee391ee529966a141566db) )
	ROM_LOAD( "u19.bin",  0x0e00000, 0x200000, CRC(c0c998a0) SHA1(498fb1877527ed37412537f06a2c39ff0c60f146) )
	ROM_CONTINUE(         0x0c00000, 0x200000 )

	ROM_LOAD( "u18.bin",  0x1000000, 0x400000, CRC(967babf4) SHA1(42a6311576417c44aeaceb8ba6bb3cd7794e4882) )
	ROM_LOAD( "u22.bin",  0x1600000, 0x200000, CRC(6239997a) SHA1(87b6d6f30f152f625f82fd858c1290176c7e156e) )
	ROM_CONTINUE(         0x1400000, 0x200000 )

	ROM_LOAD( "u17.bin",  0x1800000, 0x400000, CRC(0fad0629) SHA1(1bdc8e7c5e39e83d327f14a672ec81b049112da6) )
	ROM_LOAD( "u21.bin",  0x1e00000, 0x200000, CRC(6f95e466) SHA1(28482fad16a3ac9302f152d81552e6f84a44f3e4) )
	ROM_CONTINUE(         0x1c00000, 0x200000 )

	ROM_REGION( 0x200000, "x1snd", 0 )
	ROM_LOAD( "u32.bin", 0x100000, 0x100000, CRC(cf0f3017) SHA1(8376d3a674f71aec72f52c72758fbc53d9feb1a1) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Guardians
GAME( 200?, grdianss01,   grdians,        grdians,  grdians,  seta2_state,    empty_init,    ROT0,   "Unknown", "Guardians (Electric Shadow 2 Satsuki Version)",      MACHINE_NO_COCKTAIL | MACHINE_IMPERFECT_GRAPHICS )
GAME( 200?, grdianss02,   grdians,        grdians,  grdians,  seta2_state,    empty_init,    ROT0,   "Unknown", "Guardians (Electric God Magic 2 Unparalleled Edition)",      MACHINE_NO_COCKTAIL | MACHINE_IMPERFECT_GRAPHICS )

