// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/dataeast/fghthist.cpp"

/*****************
 Fighter's History
********************/
ROM_START( fghthist01 )
	ROM_REGION(0x100000, "maincpu", 0 )
	ROM_LOAD32_WORD( "lc00-1_s01.1f", 0x000000, 0x80000, CRC(aeb6551b) SHA1(7aec2a9cc10df80690e66cbdaf1d4b71cfd28cbd) )
	ROM_LOAD32_WORD( "lc01-1_s01.2f", 0x000002, 0x80000, CRC(c4a218f8) SHA1(fcc1a0ad00818b7cd555a0d36e6d213b8c6c76ba) )

	ROM_REGION(0x10000, "audiocpu", 0 )
	ROM_LOAD( "lc02-1.18k",  0x00000,  0x10000,  CRC(5fd2309c) SHA1(2fb7af54d5cd9bf7dd6fb4f6b82aa52b03294f1f) )

	ROM_REGION( 0x100000, "tiles1", 0 )
	ROM_LOAD( "mbf00-8.8a",  0x000000,  0x100000,  CRC(d3e9b580) SHA1(fc4676e0ecc6c32441ff66fa1f990cc3158237db) )

	ROM_REGION( 0x100000, "tiles2", 0 )
	ROM_LOAD( "mbf01-8.9a",  0x000000,  0x100000,  CRC(0c6ed2eb) SHA1(8e37ef4b1f0b6d3370a08758bfd602cb5f221282) )

	ROM_REGION( 0x800000, "sprites1", 0 )
	ROM_LOAD( "mbf02-16.16d",  0x000000,  0x200000,  CRC(c19c5953) SHA1(e6ed26f932c6c86bbd1fc4c000aa2f510c268009) )
	ROM_LOAD( "mbf03-16.17d",  0x200000,  0x200000,  CRC(37d25c75) SHA1(8219d31091b4317190618edd8acc49f97cba6a1e) )
	ROM_LOAD( "mbf04-16.18d",  0x400000,  0x200000,  CRC(f6a23fd7) SHA1(74e5559f17cd591aa25d2ed6c34ac9ed89e2e9ba) )
	ROM_LOAD( "mbf05-16.19d",  0x600000,  0x200000,  CRC(137be66d) SHA1(3fde345183ce04a7a65b4cedfd050d771df7d026) )

	ROM_REGION(0x80000, "oki1", 0 )
	ROM_LOAD( "mbf06.15k",  0x000000,  0x80000,  CRC(fb513903) SHA1(7727a49ff7977f159ed36d097020edef3b5b36ba) )

	ROM_REGION(0x80000, "oki2", 0 )
	ROM_LOAD( "mbf07.16k",  0x000000,  0x80000,  CRC(51d4adc7) SHA1(22106ed7a05db94adc5a783ce34529e29d24d41a) )

	ROM_REGION(512, "proms", 0 )
	ROM_LOAD( "kt-00.8j",  0,  512,  CRC(7294354b) SHA1(14fe42ad5d26d022c0fe9a46a4a9017af2296f40) )

	ROM_REGION( 0x0400, "plds", 0 )
	ROM_LOAD( "ve-00.3d",  0x0000, 0x0117, CRC(384d316c) SHA1(61b50c695d4210c199cf6f7bbe50c8a5ecd1d21c) )
	ROM_LOAD( "ve-01a.4d", 0x0200, 0x0117, CRC(109613c8) SHA1(5991e010c1bc2a827c8ee2c85a9b40e00a3167b3) )
ROM_END

/*****************
 Night Slashers
********************/
ROM_START( nslasheru01 )
	ROM_REGION(0x100000, "maincpu", 0 )
	ROM_LOAD32_WORD( "s01.00.f1", 0x000000, 0x80000, CRC(f3e2646e) SHA1(b90e01d3ef70cd22acb730b66d6bc1c413f73f4e) )
	ROM_LOAD32_WORD( "s01.01.f2", 0x000002, 0x80000, CRC(29804ee6) SHA1(95edfe61372f69aacc0b45a85e3c1754f7780d4a) )

	ROM_REGION(0x10000, "audiocpu", 0 )
	ROM_LOAD( "02.l18",  0x00000,  0x10000, CRC(5e63bd91) SHA1(a6ac3c8c50f44cf2e6cf029aef1c974d1fc16ed5) )

	ROM_REGION( 0x200000, "tiles1", 0 )
	ROM_LOAD( "mbh-00.8c",  0x000000,  0x200000,  CRC(a877f8a3) SHA1(79253525f360a73161894f31e211e4d6b38d307a) )

	ROM_REGION( 0x200000, "tiles2", 0 )
	ROM_LOAD( "mbh-01.9c",  0x000000,  0x200000,  CRC(1853dafc) SHA1(b1183c0db301cbed9f079c782202dbfc553b198e) )

	ROM_REGION( 0x640000, "sprites1", 0 )
	ROM_LOAD40_WORD_SWAP( "mbh-02.14c",  0x000003,  0x200000, CRC(b2f158a1) SHA1(4f8c0b324813db198fe1dad7fff4185b828b94de) )
	ROM_LOAD40_WORD_SWAP( "mbh-04.16c",  0x000001,  0x200000, CRC(eecfe06d) SHA1(2df817fe5e2ea31207b217bb03dc58979c05d0d2) )
	ROM_LOAD40_BYTE(      "mbh-06.18c",  0x000000,  0x100000, CRC(038c2127) SHA1(5bdb215305f1a419fde27a83b623a38b9328e560) )
	ROM_LOAD40_WORD_SWAP( "mbh-03.15c",  0x500003,  0x080000, CRC(787787e3) SHA1(531444e3f28aa9a7539a5a76ca94a9d6b97274c5) )
	ROM_LOAD40_WORD_SWAP( "mbh-05.17c",  0x500001,  0x080000, CRC(1d2b7c17) SHA1(ae0b8e0448a1a8180fb424fb0bc8a4302f8ff602) )
	ROM_LOAD40_BYTE(      "mbh-07.19c",  0x500000,  0x040000, CRC(bbd22323) SHA1(6ab665b2e6d04cdadc48c52e15098e978b61fe10) )

	ROM_REGION( 0x100000, "sprites2", 0 )
	ROM_LOAD( "mbh-08.16e",  0x000000,  0x80000,  CRC(cdd7f8cb) SHA1(910bbe8783c0ba722e9d6399b332d658fa059fdb) )
	ROM_LOAD( "mbh-09.18e",  0x080000,  0x80000,  CRC(33fa2121) SHA1(eb0e99d29b1ad9995df28e5b7cfc89d53efb53c3) )

	ROM_REGION(0x80000, "oki1", 0 )
	ROM_LOAD( "mbh-10.14l", 0x000000,  0x80000,  CRC(c4d6b116) SHA1(c5685bce6a6c6a74ca600ebf766ba1007f0dc666) )

	ROM_REGION(0x80000, "oki2", 0 )
	ROM_LOAD( "mbh-11.16l", 0x000000,  0x80000,  CRC(0ec40b6b) SHA1(9fef44149608ae2a00f6a75a6f77f2efcab6e78e) )

	ROM_REGION(0x200, "prom", 0 )
	ROM_LOAD( "ln-00.j7", 0x000000,  0x200, CRC(5e83eaf3) SHA1(95f5eb8e56dff6c2dce7c39a6dd458bfc38fe1cf) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Fighter's History
GAME( 1993, fghthist01,     fghthist, fghthist,  fghthist, fghthist_state, init_fghthist,  ROT0, "yumeji[2ch]", "Fighter's History (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Night Slashers
GAME( 2016, nslasheru01,    nslasher, nslasheru, nslasher, nslasher_state, init_nslasher,  ROT0, "dejp1993", "Night Slashers (US, Uncensored Effects, v1.0, 2026-10-05)", MACHINE_SUPPORTS_SAVE )

