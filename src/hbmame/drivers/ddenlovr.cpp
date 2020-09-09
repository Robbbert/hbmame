// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/ddenlovr.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/
 
 /***************************
 Mahjong The Dai Chuuka Ken
******************************/

ROM_START( mjdchukas01 ) //mjdchukac
	ROM_REGION( 0x90000+16*0x1000, "maincpu", 0 )
	ROM_LOAD( "2.5b_ps01", 0x00000, 0x80000, CRC(1cd3662f) SHA1(2a9f645c4c72648b14350986696520a6e48556a0) )
	ROM_RELOAD(       0x10000, 0x80000 )

	ROM_REGION( 0x100000, "blitter", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3.11c", 0x000000, 0x080000, CRC(c66553c3) SHA1(6e5380fdb97cc8b52986f3a3a8cac43c0f38cf54) )
	ROM_LOAD16_BYTE( "4.11a", 0x000001, 0x080000, CRC(972852fb) SHA1(157f0a772bf060efc39033b10e63a6cb1022edf6) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "1.1b", 0x00000, 0x80000, CRC(9759c65e) SHA1(cf098c07616b6d2a2ba10ff6ae0006442b675326) )
ROM_END

 /***************************
 Mahjong The Mysterious World
******************************/

ROM_START( mjmysters01 ) //mjmysterc
	ROM_REGION( 0x90000 + 0x1000*8, "maincpu", 0 ) 
	ROM_LOAD( "77t2_ps01.c3", 0x00000, 0x40000, CRC(fd78e257) SHA1(74ebae00a3de7c52cd861cb983d2f5a99afe5d06) )
	ROM_RELOAD(          0x10000, 0x40000 )

	ROM_REGION( 0x1a0000, "blitter", ROMREGION_ERASE00 )  
	ROM_LOAD( "77t6.b12", 0x000000, 0x080000, CRC(a287589a) SHA1(58659dd7e019d1d32efeaec548c84a7ded637c50) )
	ROM_LOAD( "77t5.b11", 0x080000, 0x080000, CRC(a3475059) SHA1(ec86dcea3314b65d391a970680c021899c16449e) )
	ROM_LOAD( "77t4.b10", 0x100000, 0x080000, CRC(f45c24d6) SHA1(0eca68f2ca5722717f27ac0839359966daa2715b) )
	ROM_LOAD( "77t3.b9",  0x180000, 0x020000, CRC(8671165b) SHA1(23fad112909e82ac9d25dbb69bf6334f30fa6540) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "77t1.d1", 0x000000, 0x020000, CRC(09b7a9b2) SHA1(64d9ccbb726bb6c5b362afc92bca2e3db87fd454) )
ROM_END

 /*********************
 Mahjong Reach Ippatsu
************************/

ROM_START( mjreach1s01 ) //mjreach1c
	ROM_REGION( 0x90000+16*0x1000, "maincpu", 0 ) 
	ROM_LOAD( "s01.5b",   0x00000, 0x80000, CRC(f264cd14) SHA1(7c58eb04e021cee8b37b083e4b88faf9764fe8d2) )
	ROM_RELOAD(               0x10000, 0x80000 )

	ROM_REGION( 0x500000, "blitter", 0 )
	ROM_LOAD16_BYTE( "52604.8b",     0x000000, 0x100000, CRC(6ce01bb4) SHA1(800043d8203ab5560ed0b24e0a4e01c14b6a3ac0) )
	ROM_LOAD16_BYTE( "52603.8c",     0x000001, 0x100000, CRC(16d2c169) SHA1(3e50b1109c86d0e8f931ce5a3abf20d807ebabba) )
	ROM_LOAD16_BYTE( "52606.10b",    0x200000, 0x100000, CRC(07fe5dae) SHA1(221ec21c2d84497af5b769d7409f8775be933783) )
	ROM_LOAD16_BYTE( "52605.10c",    0x200001, 0x100000, CRC(b5d57163) SHA1(d6480904bd72d298d48fbcb251b902b0b994cab1) )
	ROM_LOAD16_BYTE( "52608.12b",    0x400000, 0x080000, CRC(2f93dde4) SHA1(8efaa920e485f50ef7f4396cc8c47dfbfc97bd01) )
	ROM_LOAD16_BYTE( "52607.12c",    0x400001, 0x080000, CRC(5e685c4d) SHA1(57c99fb791429d0edb7416cffb4d1d1eb34a2813) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "52601.1c",     0x00000, 0x80000, CRC(52666107) SHA1(1e1c17b1da7ded5fc52422c7e569ef02af1ee11d) )
ROM_END

// Mahjong The Dai Chuuka Ken
GAME( 2006, mjdchukas01,  mjdchuka,  mjchuuka,  mjchuuka, hanakanz_state, empty_init,    ROT0, "hack",   "Mahjong The Dai Chuuka Ken (No AI Cheat 2006-01-06)",  MACHINE_NO_COCKTAIL  )
// Mahjong The Mysterious World
GAME( 2006, mjmysters01,  mjmyster,  mjmyster,  mjmyster, ddenlovr_state, empty_init,    ROT0, "hack",   "Mahjong The Mysterious World (No AI Cheat 2006-01-12)", MACHINE_NO_COCKTAIL  )
// Mahjong Reach Ippatsu
GAME( 2005, mjreach1s01,  mjreach1,  mjreach1,  mjreach1, hanakanz_state, empty_init,    ROT0, "hack",   "Mahjong Reach Ippatsu (No AI Cheat 2005-10-02)",  MACHINE_NO_COCKTAIL  )
