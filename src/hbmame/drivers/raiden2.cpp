// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/raiden2.cpp"

/****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /********
 Zero Team
***********/

ROM_START( zeroteamcs01 ) //zeroteamdw
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_BYTE("b1.u024_pw01.5k",   0x000000, 0x40000, CRC(b47b73b7) SHA1(1ed53b517c7886383772f731fed95b2e8e7b523e) )
	ROM_LOAD32_BYTE("b3.u023_pw01.6k",   0x000002, 0x40000, CRC(81979a9b) SHA1(1b9e216f9f6b4e6d38ac881dfe0e7ee53c523f1b) )
	ROM_LOAD32_BYTE("b2.u025_pw01.6l",   0x000001, 0x40000, CRC(7513250c) SHA1(f7aee5c2facfc663a2864497dc213e944b9e3b7b) )
	ROM_LOAD32_BYTE("b4.u026_pw01.5l",   0x000003, 0x40000, CRC(10341ede) SHA1(b63351b606d2ed59db721b2ab1765a9504c1018b) )

	ROM_REGION( 0x40000, "user2", 0 ) 
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF )
	ROM_LOAD( "5.c.u1110.5b",  0x000000, 0x08000, CRC(efc484ca) SHA1(c34b8e3e7f4c2967bc6414348993478ed637d338) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0x000000, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "b7.u072.5s",  0x000000,   0x010000, CRC(30ec0241) SHA1(a0d0be9458bf97cb9764fb85c988bb816710475e) )
	ROM_LOAD16_BYTE( "b8.u077.5r",  0x000001,   0x010000, CRC(e18b3a75) SHA1(3d52bba8d47d0d9108ee79014fd64d6e856a3fde) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION32_LE( 0x800000, "gfx3", ROMREGION_ERASEFF ) 
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )  
	ROM_LOAD( "6.c.u105.4a", 0x00000, 0x40000,  CRC(b4a6e899) SHA1(175ab656db3c3258ff10eede89890f62435d2298) )

	ROM_REGION( 0x10000, "pals", 0 )    
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP)
ROM_END

ROM_START( zeroteamss01 ) //zeroteams, zeroteam1v4
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_BYTE("1_sel_po01.bin",   0x000000, 0x40000, CRC(c2f5bba6) SHA1(a99655bef65af96c160c5e83b709597edff2a953) )
	ROM_LOAD32_BYTE("3_sel_po01.bin",   0x000002, 0x40000, CRC(aa278057) SHA1(dfe1082c1555b196989f3cbbd2b6207947f5059a) )
	ROM_LOAD32_BYTE("seibu__2.u025.6l",   0x000001, 0x40000, CRC(54f3d359) SHA1(869744185746d55c60d2f48eabe384a8499e00fd) )
	ROM_LOAD32_BYTE("4_sel_po01.bin",   0x000003, 0x40000, CRC(ae82d427) SHA1(1bc5f52791c1508e59d3b2aef04d239657defc20) )

	ROM_REGION( 0x40000, "user2", 0 )  
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF )
	ROM_LOAD( "seibu__5.u1110.5b",  0x000000, 0x08000, CRC(7ec1fbc3) SHA1(48299d6530f641b18764cc49e283c347d0918a47) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0x000000, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "seibu__7.u072.5s",   0x000000,   0x010000,   CRC(9f6aa0f0) SHA1(1caad7092c07723d12a07aa363ae2aa69cb6be0d) )
	ROM_LOAD16_BYTE( "seibu__8.u077.5r",   0x000001,   0x010000,   CRC(68f7dddc) SHA1(6938fa974c6ef028751982fdabd6a3820b0d30a8) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION32_LE( 0x800000, "gfx3", ROMREGION_ERASEFF ) 
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 ) 
	ROM_LOAD( "seibu__6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) )

	ROM_REGION( 0x10000, "pals", 0 ) 
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) 
ROM_END

ROM_START( zeroteamss02 ) //zeroteamb
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_BYTE("1_sel_po02.bin",   0x000000, 0x40000, CRC(c1c66888) SHA1(eb746efd7fea9a258d34c85cc8a739e029ba54d6) )
	ROM_LOAD32_BYTE("3_sel_po02.bin",   0x000002, 0x40000, CRC(f5c02ca4) SHA1(2d0e561b51db5f358f75b0990dc2b4cd43ee8424) )
	ROM_LOAD32_BYTE("2_sel_po02.bin",   0x000001, 0x40000, CRC(8ff58607) SHA1(bfb811ce3d3b645f23a17e36d6277440e2ff454f) )
	ROM_LOAD32_BYTE("4_sel_po02.bin",   0x000003, 0x40000, CRC(58d7c48e) SHA1(8ae055bb869e6a4fb789a0419d3c6ac45a24a312) )

	ROM_REGION( 0x40000, "user2", 0 )  
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF )
	ROM_LOAD( "seibu__5.u1110.5b",  0x000000, 0x08000, CRC(7ec1fbc3) SHA1(48299d6530f641b18764cc49e283c347d0918a47) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0x000000, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "seibu__7.u072.5s",   0x000000,   0x010000,   CRC(9f6aa0f0) SHA1(1caad7092c07723d12a07aa363ae2aa69cb6be0d) )
	ROM_LOAD16_BYTE( "seibu__8.u077.5r",   0x000001,   0x010000,   CRC(68f7dddc) SHA1(6938fa974c6ef028751982fdabd6a3820b0d30a8) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION32_LE( 0x800000, "gfx3", ROMREGION_ERASEFF ) 
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 ) 
	ROM_LOAD( "seibu__6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) )

	ROM_REGION( 0x10000, "pals", 0 ) 
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) 
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Zero Team
GAME( 1993, zeroteamcs01,  zeroteam, zeroteam, zeroteam, raiden2_state, init_zeroteam, ROT0,   "hack", "Zero Team (Special Attack)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, zeroteamss01,  zeroteam, zeroteam, zeroteam, raiden2_state, init_zeroteam, ROT0,   "hack", "Zero Team Selection (1v4 Version 2018-02-19)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, zeroteamss02,  zeroteam, zeroteam, zeroteam, raiden2_state, init_zeroteam, ROT0,   "hack", "Zero Team Selection (Nightmare 2018-10-22)", MACHINE_SUPPORTS_SAVE )
