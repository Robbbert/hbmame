// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/pgm.cpp"

ROM_START( pgemeni )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code  */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgemeni.p103",  0x100000, 0x200000, CRC(6cafa56b) SHA1(0b3447357cf9dc10e9473102a109a2c9f63be66b) )

	ROM_REGION( 0x800000, "tiles",  0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgemeni.t100",          0x180000, 0x400000, CRC(42b979dd) SHA1(38ae8a7e07f1169b876bd41e0a9bfe19ecb0256f) )

	ROM_REGION( 0x1800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "pgemeni.a100",          0x0000000, 0x400000, CRC(105d7cee) SHA1(43b19032e9e3a1aeabd7ce11438c6658ac4b5dd0) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "pgemeni.b100",          0x0000000, 0x400000, CRC(b4127373) SHA1(fa731bedec8ff4c19ec68bd44e8ce0b80ec0c716) )

	ROM_REGION( 0x600000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmdemo.m100",          0x400000, 0x200000, CRC(8d89877e) SHA1(7d76d48d64d7ac5411d714a4bb83f37e3e5b8df6) )
ROM_END

ROM_START( pgmdemo )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code  */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmdemo.p103", 0x100000, 0x200000, CRC(d3f6ec45) SHA1(4c171ffff6a04b362f71c42de7bc1d7b57da2c0e) )

	ROM_REGION( 0x800000, "tiles",  0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgmdemo.t100",          0x180000, 0x400000, CRC(0596a59a) SHA1(e48cb703e886a5b71ecc839136c642117aa529ef) )

	ROM_REGION( 0x1800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "pgmdemo.a100",          0x0000000, 0x400000, CRC(5d0e8fa1) SHA1(95380456c8a6c9c48d4987951f689d5b9a7a397c) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "pgmdemo.b100",          0x0000000, 0x400000, CRC(15dd191f) SHA1(a90f63138c9a9bf9215b17a14635518aed7da8a9) )

	ROM_REGION( 0x600000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmdemo.m100",          0x400000, 0x200000, CRC(8d89877e) SHA1(7d76d48d64d7ac5411d714a4bb83f37e3e5b8df6) )
ROM_END

ROM_START( pgmfrog )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code  */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmfrog.p103",  0x100000, 0x200000, CRC(cdec9e8d) SHA1(183753b3f19e9b06c6059206aac85d865bf1dd66) )

	ROM_REGION( 0x800000, "tiles",  0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgmfrog.t100",          0x180000, 0x400000, CRC(8f58b6d8) SHA1(79b3b503f29593170a7faa93d75707682d800c66) )

	ROM_REGION( 0x1800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "pgmfrog.a100",          0x0000000, 0x400000, CRC(dc1eafe6) SHA1(11eb0d02c38df828ae0361bc7624c4658047df67) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "pgmfrog.b100",          0x0000000, 0x400000, CRC(3d44b66f) SHA1(04347002bd942d320c6b8240e4485aec626f2aaa) )

	ROM_REGION( 0x600000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmfrog.m100",          0x400000, 0x200000, CRC(05e2f761) SHA1(c93d94a8f11c41b019fcf9b6a90645416fd2c75b) )
ROM_END

ROM_START( ketarrb )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarrb_v100.u38", 0x000000, 0x200000, CRC(ec7a4f92) SHA1(6351fb386586956fbdb5f0730c481fb539cc267a) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "ket_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	ROM_LOAD( "pgm_t01s.rom", 0x000000, 0x200000, CRC(1a7123a0) SHA1(cc567f577bfbf45427b54d6695b11b74f2578af3) ) // same as standard PGM text bios - surface scratched to remove details
	ROM_LOAD( "t04701w064.u19", 0x180000, 0x800000, CRC(2665b041) SHA1(fb1107778b66f2af0de77ac82e1ee2902f53a959) ) //text-1

	ROM_REGION( 0x1000000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a04701w064.u7", 0x0000000, 0x0800000, CRC(5ef1b94b) SHA1(f10dfa46e0a4d297c3a856aea5b49d648f98935c) ) //image-1
	ROM_LOAD( "a04702w064.u8", 0x0800000, 0x0800000, CRC(26d6da7f) SHA1(f20e07a7994f41b5ed917f8b0119dc5542f3541c) ) //image-2

	ROM_REGION( 0x0800000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b04701w064.u1", 0x0000000, 0x0800000, CRC(1bec008d) SHA1(07d117dc2eebb35727fb18a7c563acbaf25a8d36) ) //bitmap-1

	ROM_REGION( 0x800000, "ics", ROMREGION_ERASE00 ) /* Samples - (8 bit mono 11025Hz) - */
	/* there is a position for the PGM audio bios rom, but it's unpopulated, and the M of PGM has been scratched off the PCB */
	ROM_LOAD( "m04701b032.u17", 0x400000, 0x400000, CRC(b46e22d1) SHA1(670853dc485942fb96380568494bdf3235f446ee) ) //music-1

	ROM_REGION( 0x20000, "sram", 0 ) /* default settings */
	ROM_LOAD( "ket_defaults.nv", 0x0000000, 0x020000, CRC(3ca892d8) SHA1(67430df5217e453ae8140c5653deeadfad8fa684) )
ROM_END

// This crashes in attract mode, but if you start a game before that happens, then it plays ok
ROM_START( orlegendcs )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code  */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0101.s160",   0x100000, 0x200000, CRC(d608ece9) SHA1(0a34bdcfcbb4fad4c499ee559791e29a432bff85) )

	ROM_REGION( 0x800000, "tiles",  0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0100.u8",     0x180000, 0x400000, CRC(61425e1e) SHA1(20753b86fc12003cfd763d903f034dbba8010b32) )

	ROM_REGION( 0x1800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0100.u5",     0x0000000, 0x400000, CRC(8b3bd88a) SHA1(42db3a60c6ba9d83ebe2008c8047d094027f65a7) )
	ROM_LOAD( "a0101.u6",     0x0400000, 0x400000, CRC(3b9e9644) SHA1(5b95ec1d25c3bc3504c93547f5adb5ce24376405) )
	ROM_LOAD( "a0102.u7",     0x0800000, 0x400000, CRC(069e2c38) SHA1(9bddca8c2f5bd80f4abe4e1f062751736dc151dd) )
	ROM_LOAD( "a0103.u8",     0x0c00000, 0x400000, CRC(4460a3fd) SHA1(cbebdb65c17605853f7d0b298018dd8801a25a58) )
	ROM_LOAD( "a0104.u11",    0x1000000, 0x400000, CRC(5f8abb56) SHA1(6c1ddc0309862a141aa0c0f63b641aec9257aaee) )
	ROM_LOAD( "a0105.u12",    0x1400000, 0x400000, CRC(a17a7147) SHA1(44eeb43c6b0ebb829559a20ae357383fbdeecd82) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0100.u9",     0x0000000, 0x400000, CRC(69d2e48c) SHA1(5b5f759007264c07b3b39be8e03a713698e1fc2a) )
	ROM_LOAD( "b0101.u10",    0x0400000, 0x400000, CRC(0d587bf3) SHA1(5347828b0a6e4ddd7a263663d2c2604407e4d49c) )
	ROM_LOAD( "b0102.u15",    0x0800000, 0x400000, CRC(43823c1e) SHA1(e10a1a9a81b51b11044934ff702e35d8d7ab1b08) )

	ROM_REGION( 0x600000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0100.u1",     0x400000, 0x200000, CRC(e5c36c83) SHA1(50c6f66770e8faa3df349f7d68c407a7ad021716) )
ROM_END

// This crashes in attract mode, but if you start a game before that happens, then it plays ok
ROM_START( orlegendh )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code  */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0101h.s160",  0x100000, 0x200000, CRC(d608ece9) SHA1(0a34bdcfcbb4fad4c499ee559791e29a432bff85) )

	ROM_REGION( 0x800000, "tiles",  0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0100.u8",     0x180000, 0x400000, CRC(61425e1e) SHA1(20753b86fc12003cfd763d903f034dbba8010b32) )

	ROM_REGION( 0x1800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0100.u5",     0x0000000, 0x400000, CRC(8b3bd88a) SHA1(42db3a60c6ba9d83ebe2008c8047d094027f65a7) )
	ROM_LOAD( "a0101.u6",     0x0400000, 0x400000, CRC(3b9e9644) SHA1(5b95ec1d25c3bc3504c93547f5adb5ce24376405) )
	ROM_LOAD( "a0102.u7",     0x0800000, 0x400000, CRC(069e2c38) SHA1(9bddca8c2f5bd80f4abe4e1f062751736dc151dd) )
	ROM_LOAD( "a0103.u8",     0x0c00000, 0x400000, CRC(4460a3fd) SHA1(cbebdb65c17605853f7d0b298018dd8801a25a58) )
	ROM_LOAD( "a0104.u11",    0x1000000, 0x400000, CRC(5f8abb56) SHA1(6c1ddc0309862a141aa0c0f63b641aec9257aaee) )
	ROM_LOAD( "a0105.u12",    0x1400000, 0x400000, CRC(a17a7147) SHA1(44eeb43c6b0ebb829559a20ae357383fbdeecd82) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0100.u9",     0x0000000, 0x400000, CRC(69d2e48c) SHA1(5b5f759007264c07b3b39be8e03a713698e1fc2a) )
	ROM_LOAD( "b0101.u10",    0x0400000, 0x400000, CRC(0d587bf3) SHA1(5347828b0a6e4ddd7a263663d2c2604407e4d49c) )
	ROM_LOAD( "b0102.u15",    0x0800000, 0x400000, CRC(43823c1e) SHA1(e10a1a9a81b51b11044934ff702e35d8d7ab1b08) )

	ROM_REGION( 0x600000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0100.u1",     0x400000, 0x200000, CRC(e5c36c83) SHA1(50c6f66770e8faa3df349f7d68c407a7ad021716) )
ROM_END
#if 0
ROM_START( kovassg ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.assg", 0x100000, 0x400000, CRC(b9885d5c) SHA1(be55e6b3639fbf30e4cbc1bb24e388fd2ad74382) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END
#endif
ROM_START( kovassg )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "assg_v315cn.rom", 0x100000, 0x400000, CRC(b9885d5c) SHA1(be55e6b3639fbf30e4cbc1bb24e388fd2ad74382) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000, BAD_DUMP CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) ) // this is the kovsh ARM rom, we intercept and modify protection calls

	ROM_REGION( 0xc00000, "tiles",  0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "lsqh2_t01.rom",0x180000, 0x800000, CRC(d498d97f) SHA1(97a7b6d2ed1170449e7c2899448af7cbbca4c94f) )
	ROM_IGNORE( 0x800000 )  // second half identical

	ROM_REGION( 0x3000000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "lsqh2_a01.rom", 0x0000000, 0x1000000, CRC(25ae3efd) SHA1(083d977602ddb5ad54fbdcba000cd4287de8d463) )
	ROM_LOAD( "lsqh2_a23.rom", 0x1000000, 0x1000000, CRC(7a779112) SHA1(0a7d36b3715063d8eac629b95a9bb3ecd8e54fca) )
	ROM_LOAD( "lsqh2_a45.rom", 0x2000000, 0x1000000, CRC(5d7de052) SHA1(7663b6cf09f65c4644661005a38f9aba84a32913) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "lsqh2_b01.rom", 0x0000000, 0x1000000, CRC(df7ca696) SHA1(7af3d27957a39de7e4873867c9972c05af7e7964) )

	ROM_REGION( 0xc00000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "lsqh2_m01.rom",0x400000, 0x400000, CRC(01af1b65) SHA1(6cf523fa8f1e03f974771611bb9a4e08a4d4443f) )
	ROM_IGNORE( 0x400000 )  // 400000-7fffff empty
	ROM_CONTINUE( 0x800000, 0x400000 )
	ROM_IGNORE( 0x400000 )  // c00000-ffffff empty
ROM_END

ROM_START( kovdf ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.df", 0x100000, 0x400000, CRC(a4c3cfed) SHA1(09c7b51652a90cce21ec3977e522e4894b6f0e14) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kov2h1 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v204-32m.2h1", 0x100000, 0x400000, CRC(1f8cda84) SHA1(9220ef478edc2267c5d96083c2c4ecf465ab9685) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	// supplied rom freezes mame when attract mode starts, this one works
	ROM_LOAD( "kov2p_igs027a_china.bin", 0x000000, 0x04000, CRC(19a0bd95) SHA1(83e9f22512832a51d41c588debe8be7adb3b1df7) )

	ROM_REGION32_LE( 0x400000, "user1", 0 ) /* Protection Data (encrypted external ARM data) */
	ROM_LOAD( "v200-16.rom", 0x000000, 0x200000,  CRC(16a0c11f) SHA1(ce449cef76ebd5657d49b57951e2eb0f132e203e) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t1200.rom",    0x180000, 0x800000, CRC(d7e26609) SHA1(bdad810f82fcf1d50a8791bdc495374ec5a309c6) )

	ROM_REGION( 0x2800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a1200.rom",    0x0000000, 0x0800000, CRC(ceeb81d8) SHA1(5476729443fc1bc9593ae10fbf7cbc5d7290b017) )
	ROM_LOAD( "a1201.rom",    0x0800000, 0x0800000, CRC(21063ca7) SHA1(cf561b44902425a920d5cbea5bf65dd9530b2289) )
	ROM_LOAD( "a1202.rom",    0x1000000, 0x0800000, CRC(4bb92fae) SHA1(f0b6d72ed425de1c69dc8f8d5795ea760a4a59b0) )
	ROM_LOAD( "a1203.rom",    0x1800000, 0x0800000, CRC(e73cb627) SHA1(4c6e48b845a5d1e8f9899010fbf273d54c2b8899) )
	ROM_LOAD( "a1204.rom",    0x2000000, 0x0200000, CRC(14b4b5bb) SHA1(d7db5740eec971f2782fb2885ee3af8f2a796550) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b1200.rom",   0x0000000, 0x0800000,  CRC(bed7d994) SHA1(019dfba8154256d64cd249eb0fa4c451edce34b8) )
	ROM_LOAD( "b1201.rom",   0x0800000, 0x0800000,  CRC(f251eb57) SHA1(56a5fc14ab7822f83379cecb26638e5bb266349a) )

	ROM_REGION( 0x1000000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m1200.rom",    0x800000, 0x800000, CRC(b0d88720) SHA1(44ab137e3f8e15b7cb5697ffbd9b1143d8210c4f) )
ROM_END

ROM_START( kov2h2 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v204-32m.2h2", 0x100000, 0x400000, CRC(7fcc1ab6) SHA1(47f9ab9f7f1b3d7941a70b383072d2d58e07637b) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov2p_igs027a_china.bin", 0x000000, 0x04000, CRC(19a0bd95) SHA1(83e9f22512832a51d41c588debe8be7adb3b1df7) )

	ROM_REGION32_LE( 0x400000, "user1", 0 ) /* Protection Data (encrypted external ARM data) */
	ROM_LOAD( "v200-16.rom", 0x000000, 0x200000,  CRC(16a0c11f) SHA1(ce449cef76ebd5657d49b57951e2eb0f132e203e) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t1200.rom",    0x180000, 0x800000, CRC(d7e26609) SHA1(bdad810f82fcf1d50a8791bdc495374ec5a309c6) )

	ROM_REGION( 0x2800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a1200.rom",    0x0000000, 0x0800000, CRC(ceeb81d8) SHA1(5476729443fc1bc9593ae10fbf7cbc5d7290b017) )
	ROM_LOAD( "a1201.rom",    0x0800000, 0x0800000, CRC(21063ca7) SHA1(cf561b44902425a920d5cbea5bf65dd9530b2289) )
	ROM_LOAD( "a1202.rom",    0x1000000, 0x0800000, CRC(4bb92fae) SHA1(f0b6d72ed425de1c69dc8f8d5795ea760a4a59b0) )
	ROM_LOAD( "a1203.rom",    0x1800000, 0x0800000, CRC(e73cb627) SHA1(4c6e48b845a5d1e8f9899010fbf273d54c2b8899) )
	ROM_LOAD( "a1204.rom",    0x2000000, 0x0200000, CRC(14b4b5bb) SHA1(d7db5740eec971f2782fb2885ee3af8f2a796550) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b1200.rom",   0x0000000, 0x0800000,  CRC(bed7d994) SHA1(019dfba8154256d64cd249eb0fa4c451edce34b8) )
	ROM_LOAD( "b1201.rom",   0x0800000, 0x0800000,  CRC(f251eb57) SHA1(56a5fc14ab7822f83379cecb26638e5bb266349a) )

	ROM_REGION( 0x1000000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m1200.rom",    0x800000, 0x800000, CRC(b0d88720) SHA1(44ab137e3f8e15b7cb5697ffbd9b1143d8210c4f) )
ROM_END

ROM_START( kov2h3 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v204-32m.2h3", 0x100000, 0x400000, CRC(baa217ad) SHA1(f781d96870b7ac7043049ab08174bf2cc48979f5) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov2p_igs027a_china.bin", 0x000000, 0x04000, CRC(19a0bd95) SHA1(83e9f22512832a51d41c588debe8be7adb3b1df7) )

	ROM_REGION32_LE( 0x400000, "user1", 0 ) /* Protection Data (encrypted external ARM data) */
	ROM_LOAD( "v200-16.rom", 0x000000, 0x200000,  CRC(16a0c11f) SHA1(ce449cef76ebd5657d49b57951e2eb0f132e203e) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t1200.rom",    0x180000, 0x800000, CRC(d7e26609) SHA1(bdad810f82fcf1d50a8791bdc495374ec5a309c6) )

	ROM_REGION( 0x2800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a1200.rom",    0x0000000, 0x0800000, CRC(ceeb81d8) SHA1(5476729443fc1bc9593ae10fbf7cbc5d7290b017) )
	ROM_LOAD( "a1201.rom",    0x0800000, 0x0800000, CRC(21063ca7) SHA1(cf561b44902425a920d5cbea5bf65dd9530b2289) )
	ROM_LOAD( "a1202.rom",    0x1000000, 0x0800000, CRC(4bb92fae) SHA1(f0b6d72ed425de1c69dc8f8d5795ea760a4a59b0) )
	ROM_LOAD( "a1203.rom",    0x1800000, 0x0800000, CRC(e73cb627) SHA1(4c6e48b845a5d1e8f9899010fbf273d54c2b8899) )
	ROM_LOAD( "a1204.rom",    0x2000000, 0x0200000, CRC(14b4b5bb) SHA1(d7db5740eec971f2782fb2885ee3af8f2a796550) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b1200.rom",   0x0000000, 0x0800000,  CRC(bed7d994) SHA1(019dfba8154256d64cd249eb0fa4c451edce34b8) )
	ROM_LOAD( "b1201.rom",   0x0800000, 0x0800000,  CRC(f251eb57) SHA1(56a5fc14ab7822f83379cecb26638e5bb266349a) )

	ROM_REGION( 0x1000000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m1200.rom",    0x800000, 0x800000, CRC(b0d88720) SHA1(44ab137e3f8e15b7cb5697ffbd9b1143d8210c4f) )
ROM_END

ROM_START( kov2h4 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v204-32m.2h4", 0x100000, 0x400000, CRC(ee8fb036) SHA1(bf1f3006a5e5026ff9bec8d8cb2e147ec2ee6716) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov2p_igs027a_china.bin", 0x000000, 0x04000, CRC(19a0bd95) SHA1(83e9f22512832a51d41c588debe8be7adb3b1df7) )

	ROM_REGION32_LE( 0x400000, "user1", 0 ) /* Protection Data (encrypted external ARM data) */
	ROM_LOAD( "v200-16.rom", 0x000000, 0x200000,  CRC(16a0c11f) SHA1(ce449cef76ebd5657d49b57951e2eb0f132e203e) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t1200.rom",    0x180000, 0x800000, CRC(d7e26609) SHA1(bdad810f82fcf1d50a8791bdc495374ec5a309c6) )

	ROM_REGION( 0x2800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a1200.rom",    0x0000000, 0x0800000, CRC(ceeb81d8) SHA1(5476729443fc1bc9593ae10fbf7cbc5d7290b017) )
	ROM_LOAD( "a1201.rom",    0x0800000, 0x0800000, CRC(21063ca7) SHA1(cf561b44902425a920d5cbea5bf65dd9530b2289) )
	ROM_LOAD( "a1202.rom",    0x1000000, 0x0800000, CRC(4bb92fae) SHA1(f0b6d72ed425de1c69dc8f8d5795ea760a4a59b0) )
	ROM_LOAD( "a1203.rom",    0x1800000, 0x0800000, CRC(e73cb627) SHA1(4c6e48b845a5d1e8f9899010fbf273d54c2b8899) )
	ROM_LOAD( "a1204.rom",    0x2000000, 0x0200000, CRC(14b4b5bb) SHA1(d7db5740eec971f2782fb2885ee3af8f2a796550) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b1200.rom",   0x0000000, 0x0800000,  CRC(bed7d994) SHA1(019dfba8154256d64cd249eb0fa4c451edce34b8) )
	ROM_LOAD( "b1201.rom",   0x0800000, 0x0800000,  CRC(f251eb57) SHA1(56a5fc14ab7822f83379cecb26638e5bb266349a) )

	ROM_REGION( 0x1000000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m1200.rom",    0x800000, 0x800000, CRC(b0d88720) SHA1(44ab137e3f8e15b7cb5697ffbd9b1143d8210c4f) )
ROM_END

ROM_START( kov2h5 ) // was kov2psv
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "v204-32mv.rom",    0x100000, 0x400000, CRC(fa8d8e5b) SHA1(b7dc3853145bbc063a2953f5840eced1bbd9186e) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov2p_igs027a_china.bin", 0x000000, 0x04000, CRC(19a0bd95) SHA1(83e9f22512832a51d41c588debe8be7adb3b1df7) )

	ROM_REGION32_LE( 0x400000, "user1", 0 ) /* Protection Data (encrypted external ARM data) */
	ROM_LOAD( "v200-16.rom", 0x000000, 0x200000,  CRC(16a0c11f) SHA1(ce449cef76ebd5657d49b57951e2eb0f132e203e) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t1200.rom",    0x180000, 0x800000, CRC(d7e26609) SHA1(bdad810f82fcf1d50a8791bdc495374ec5a309c6) )

	ROM_REGION( 0x2800000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a1200.rom",    0x0000000, 0x0800000, CRC(ceeb81d8) SHA1(5476729443fc1bc9593ae10fbf7cbc5d7290b017) )
	ROM_LOAD( "a1201.rom",    0x0800000, 0x0800000, CRC(21063ca7) SHA1(cf561b44902425a920d5cbea5bf65dd9530b2289) ) // either this or a1201.rom in kov2 is probably bad
	ROM_LOAD( "a1202.rom",    0x1000000, 0x0800000, CRC(4bb92fae) SHA1(f0b6d72ed425de1c69dc8f8d5795ea760a4a59b0) )
	ROM_LOAD( "a1203.rom",    0x1800000, 0x0800000, CRC(e73cb627) SHA1(4c6e48b845a5d1e8f9899010fbf273d54c2b8899) )
	ROM_LOAD( "a1204.rom",    0x2000000, 0x0200000, CRC(14b4b5bb) SHA1(d7db5740eec971f2782fb2885ee3af8f2a796550) ) // either this or a1204.rom in kov2 is probably bad

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b1200.rom",   0x0000000, 0x0800000,  CRC(bed7d994) SHA1(019dfba8154256d64cd249eb0fa4c451edce34b8) )
	ROM_LOAD( "b1201.rom",   0x0800000, 0x0800000,  CRC(f251eb57) SHA1(56a5fc14ab7822f83379cecb26638e5bb266349a) )

	ROM_REGION( 0x1000000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m1200.rom",    0x800000, 0x800000, CRC(b0d88720) SHA1(44ab137e3f8e15b7cb5697ffbd9b1143d8210c4f) )
ROM_END

ROM_START( kovph1 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph1",    0x100000, 0x400000, CRC(390ebbfc) SHA1(e051a7659a14316c35075cfd2a2f2baba7f18f1a) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph2 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600116.119",    0x100000, 0x400000, CRC(30157417) SHA1(6152b82aea174c4201f07e8ebc09f1750140f1ff) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph3 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600948.119",    0x100000, 0x400000, CRC(78aae711) SHA1(af4649201ca775db7c86e8027d9c2511f656fcb1) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph4 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p060006.119",    0x100000, 0x258000, CRC(b071cf49) SHA1(203b946d30640247b8ea5e8a56eeb2a5cb465b62) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph5 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600v93.119",    0x100000, 0x400000, CRC(1fa0e862) SHA1(beb83e1feb7c7bd133e6ad83eba50f381a25ab85) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph6 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600v97.119",    0x100000, 0x400000, CRC(00905e8d) SHA1(3695326f46d03bc2dc46588ee563df9d1ca14b97) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph7 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600lb.119",    0x100000, 0x400000, CRC(8da107cb) SHA1(f6bd684a0edd1f52110e6640bb4d824b789fbb4b) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600lb.rom",    0x180000, 0x800000, CRC(9920e650) SHA1(1d46233869d4e65fd8b84ec520fc7e7e3840c7aa) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600lb.rom",  0x0000000, 0x0800000, CRC(21e272ae) SHA1(71bee0d53a80c406c813e1c6ea99774df2e40f9c) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600lb.rom",  0x0000000, 0x0800000, CRC(b79355de) SHA1(4773a83fe646e27f6223897fea56eb438edc9809) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph8 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0602lw.119",    0x100000, 0x400000, CRC(77cfdb2e) SHA1(bf39922b38e8a7db898bb6cdc15f6ca9c2dc14b7) )
	ROM_LOAD16_WORD_SWAP( "p0601lw.119",    0x100000, 0x271000, CRC(04f76a4b) SHA1(7303956b589e175baa924077953f146b32868182) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600lb.rom",    0x180000, 0x800000, CRC(9920e650) SHA1(1d46233869d4e65fd8b84ec520fc7e7e3840c7aa) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600lw.rom",  0x0000000, 0x0800000, CRC(cca70135) SHA1(2d288df75907e50c7fc2beca5fb9fd895faece55) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602lw.rom",  0x1000000, 0x0800000, CRC(71c8a46c) SHA1(6391eb95e1f2933b0e02518232a4bbcf7c91b73f) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600lw.rom",  0x0000000, 0x0800000, CRC(a4155c55) SHA1(3146774f00dd4156b7423d9983dc6e678aca648d) )
	ROM_LOAD( "b0601lw.rom",  0x0800000, 0x0400000, CRC(a261d5bd) SHA1(19204c5ab9861f8ccfb0b168e2bb4137f9d14dfe) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph9 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600pj.119",    0x100000, 0x400000, CRC(2ca30916) SHA1(506a8b27cd9911348aa481d7276b2bda1b5758ef) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph10 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600qy.120",    0x100000, 0x400000, CRC(399c8cbc) SHA1(c9702db79ea56e6727577a5a9b8f42b700c67a4f) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600lb.rom",    0x180000, 0x800000, CRC(9920e650) SHA1(1d46233869d4e65fd8b84ec520fc7e7e3840c7aa) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph11 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.119",    0x100000, 0x400000, CRC(e4b0875d) SHA1(e8382e131b0e431406dc2a05cc1ef128302d987c) )
	// not working ROM_LOAD16_WORD_SWAP( "p0600s8.119",  0x100000, 0x2bc000, CRC(dc214ff5) SHA1(0a7448dbb18259e8e36e26b38b791bcd39e1d05e) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600s8.rom",    0x180000, 0x800000, CRC(e2528ac5) SHA1(f719c2fb80a9d645432bdba90ea15a2d59427cd2) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph12 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600x.119",    0x100000, 0x400000, CRC(3d5ad108) SHA1(f7129f69f16d1589c2b03c246f16518836f09ad7) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) )
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) )
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) )
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph13 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.119",    0x100000, 0x400000, CRC(910edfdf) SHA1(bb222e503fed23bbc9b4d244ba02b1dd756006f9) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph14 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.119",    0x100000, 0x400000, CRC(b5d87e6e) SHA1(98d3bc1327f209b19c159eb54652cabf2b61b434) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph15 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.119",    0x100000, 0x400000, CRC(c9326627) SHA1(a81a46325473be6b138cf76ee2fa9db655a11313) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph16 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600up.119",    0x100000, 0x400000, CRC(dfd96c08) SHA1(544463a734c50d45ce5549dba22aa7a3a3f75dc6) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600up.rom",    0x180000, 0x800000, CRC(9920e650) SHA1(1d46233869d4e65fd8b84ec520fc7e7e3840c7aa) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph17 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph17", 0x100000, 0x400000, CRC(19032b59) SHA1(6462cfa55c6cd6d3668010e11463f1fb6a9641c8) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.ph17",    0x180000, 0x800000, CRC(84567fa4) SHA1(932fdd8095b499acbac69035c12929a2dc9a5564) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.ph17",    0x400000, 0x400000, CRC(75eaf52b) SHA1(4981992441b35343c8bf23e730bba070ddef3350) )
ROM_END

ROM_START( kovph18 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph18", 0x100000, 0x454f4e, CRC(5b2981c3) SHA1(8b64d86f76991960240126da13729b6cbc8f5513) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.ph18",    0x180000, 0x800000, CRC(d5b8327e) SHA1(5e00c0ff31e7258cc37e389c8bce0a1b8d39d49f) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x854f62, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.ph18",   0x400000, 0x454f62, CRC(24b06143) SHA1(3b712320ee9b4c8df2e0b2a3f05e3ccf11087c0a) )
ROM_END

ROM_START( kovph19 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph19", 0x100000, 0x400000, CRC(e4ec6880) SHA1(025d1dbc0f067d10d7e10567b9f1bc1bdfac481a) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph20 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph20", 0x100000, 0x400000, CRC(5c9acb7a) SHA1(a25c1813efa7151536971fce39cfb2e1e71c7f91) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph21 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph21",    0x100000, 0x400000, CRC(5e3bdea4) SHA1(5b20a0b9aa02f285425ac65c8df3c1a47e4e61f5) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph22 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph22",    0x100000, 0x400000, CRC(06d1f4d5) SHA1(69e2a580c66c45b0221335cc5eeb7ac0bb6e18b7) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph23 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.ph23",    0x100000, 0x2bc000, CRC(dc214ff5) SHA1(0a7448dbb18259e8e36e26b38b791bcd39e1d05e) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.ph23",   0x180000, 0x800000, CRC(e2528ac5) SHA1(f719c2fb80a9d645432bdba90ea15a2d59427cd2) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph24 ) // was kvplusjq
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600jq.119",    0x100000, 0x400000, CRC(a04866b9) SHA1(bae132ae93244e328a887728d4803c4593e8b01e) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovph25 ) // was kovplusq - crashes after 2nd attract cycle
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.120",    0x100000, 0x400000, CRC(6392a1c3) SHA1(ccbe2e736897a1c969e80fbee93ffcc6defb025a) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kov_igs027a.bin", 0x000000, 0x04000, NO_DUMP )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1c00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh1 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600hack.322",    0x100000, 0x400000, CRC(c48df7de) SHA1(d97e3603dcc09c5018f333f21d5069eef89289dd) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsharm.bin", 0x000000, 0x04000,  CRC(9d8ecc0c) SHA1(51f288e8aa3dd1d4b7f24155fea9c6a7f4cd1144) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x400000, CRC(164b3c94) SHA1(f00ea66886ca6bff74bbeaa49e7f5c75c275d5d7) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh2 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600re.322",    0x100000, 0x2a3000, CRC(4c38c199) SHA1(48876b6a9cd85d39c4b49b7a47e66d66f2bfd375) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600re.rom",    0x0000000, 0x0800000, CRC(2f5e017e) SHA1(83655f4c4d5f4155f0f8908870c0a97de9d52907) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600re.rom",    0x0000000, 0x0800000, CRC(c16d355b) SHA1(2f711376d489ba19cf466e3ee1d498c93313fcb5) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh3 ) // same as kovsh103 except for b0602 rom
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.103",    0x100000, 0x400000, CRC(f0b3da82) SHA1(4067beb69c049b51bce6154f4cf880600ca4de11) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.sh3",    0x0c00000, 0x0400000, CRC(c3e0b454) SHA1(7aaa9b1abf9f1e7fe9acbba757ff7fbfa46557d3) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh4 ) // p rom encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh4",    0x100000, 0x400000, CRC(739a92ca) SHA1(69387dfbc5015c3cb43283a189b49ae05f93f3fd) )//CRC(7c78e5f3) SHA1(9b1e4bd63fb1294ebeb539966842273c8dc7683b) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh5 ) // p rom encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh5",    0x100000, 0x400000, CRC(6c61f80f) SHA1(2797971c5a53c983284525603e2d41b2efd8f26d) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x2000000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.sh5",    0x1800000, 0x0800000, CRC(eadc7955) SHA1(7ae5acded2a5c47ccbeb1726f93f14afdd1ede86) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.sh5",    0x0800000, 0x0800000, CRC(bf30ea67) SHA1(9d8049307c4084765eb1a1adb0530811347f2695) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.sh5",    0x400000, 0x400000, CRC(eae5bf80) SHA1(5052849cb1ea573ee98188c594b4a449c195e441) )
ROM_END

ROM_START( kovsh6 ) // p rom encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh6",    0x100000, 0x400000, CRC(8d02a952) SHA1(9af1d7193bba64631b8bd89f6de3a04c0bf7891f) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x2000000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.sh6",    0x1800000, 0x0800000, CRC(cbc57bc1) SHA1(ae1ef43a5caaa1e1a0be6ba7a6598eeaeb284a1e) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.sh6",    0x0800000, 0x0800000, CRC(1885039b) SHA1(23f832ee7493d11f78ea06954da748ff1fa3c595) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.sh5",    0x400000, 0x400000, CRC(eae5bf80) SHA1(5052849cb1ea573ee98188c594b4a449c195e441) )
ROM_END

ROM_START( kovsh7 ) // set has 3 p roms (all encrypted, and 2 b601 roms, and 1 a603 rom, not coded atm
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600h.sh7",   0x100000, 0x400000, CRC(7c05aa5c) SHA1(85aa625187f79a4a7401386ffb6ea3c2492e1be3) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000, BAD_DUMP CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) ) // this is the kovsh ARM rom, we intercept and modify protection calls

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x2000000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0540.rom",    0x1800000, 0x0800000, CRC(4fd3413e) SHA1(5e8f3e421342bf558c77e59635f9b5d713e825c2) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0540.rom",    0x0800000, 0x0800000, CRC(60999757) SHA1(118cf058e67858958bcb931e14f5d160c7de87cc) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh8 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh8", 0x100000, 0x400000, CRC(617e53fb) SHA1(5f8b05c11e08d1bef60cda563d15dc7537b5c8f0) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh9 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh9",    0x100000, 0x400000, CRC(12463832) SHA1(7ace58653ab4e9b49556e0969596aeceb2c6dfae) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh10 ) // not set up yet, most likely to be encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.322",    0x100000, 0x400000, CRC(7c78e5f3) SHA1(9b1e4bd63fb1294ebeb539966842273c8dc7683b) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh11 ) // p rom same as kovsh8
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.322",    0x100000, 0x400000, CRC(7c78e5f3) SHA1(9b1e4bd63fb1294ebeb539966842273c8dc7683b) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh12 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh12", 0x100000, 0x400000, CRC(97330917) SHA1(99bff51d6d007d8f3f45dd36f43e4690ed5f4946) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000, CRC(7ece721c) SHA1(3aa11f11803643f9aa4bdafa35421626d066c4e9) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh13 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh13", 0x100000, 0x400000, CRC(d80651ec) SHA1(90ccb02b9e7c549ad0bd36379c72127cce4159cb) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh14 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh14", 0x100000, 0x400000, CRC(bba16fe0) SHA1(ce9c42cde5d66913f73a34b66b9615a497103d3b) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh15 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh15", 0x100000, 0x400000, CRC(54f6c5d9) SHA1(9abf1fed12261fdddd2e40d30d6ce536e91c1c55) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh16 ) // encrypted
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.sh15", 0x100000, 0x400000, CRC(54f6c5d9) SHA1(9abf1fed12261fdddd2e40d30d6ce536e91c1c55) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

ROM_START( kovsh17 ) // encrypted - was kovshq
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600q.322",    0x100000, 0x400000, CRC(f9e0bdc3) SHA1(fc7080d1255cae6b3a241fbbea24355530de0a8f) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604.rom",    0x1a00000, 0x0400000, CRC(26b59fd3) SHA1(53219376056f4766dc5236735599d982ceb56b84) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602.rom",    0x0c00000, 0x0100000, CRC(9df77934) SHA1(99a3fe337c13702c9aa2373bcd1bb1befd0e2a13) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END

// same problem as MAME kovsgqyza: it crashes after the 2nd attract cycle
ROM_START( kovsgqyzc )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgm_sgqyzc.rom",    0x100000, 0x400000, CRC(09774a3e) SHA1(f5131d07abb048cd3e3549bf0223c9cb485ce7f6) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsgqyzc_igs027a.bin", 0x000000, 0x04000, NO_DUMP ) // bootleg is probably a different device

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	ROM_REGION( 0x2000000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "qyza0603.rom", 0x1800000, 0x0800000, CRC(c8b92220) SHA1(4f9c43970d92ac8a8f1563021022797ae8e32012) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "qyzb0601.rom", 0x0800000, 0x0800000, CRC(64f55882) SHA1(ab9ac1396587c3d78d06f6ec83cab61d6a9faacd) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END




GAME( 2002, ketarrb,    ket,       pgm_arm_type1_cave, pgm,      pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/04/17 BACK. VER)", MACHINE_SUPPORTS_SAVE )
GAME( 1997, orlegendh,  orlegend,  pgm_asic3,          orlegend, pgm_asic3_state,     orlegend,   ROT0,   "Hack", "Oriental Legend Plus / Xi You Shi E Zhuan Plus (ver. 2.0 by Enbrow)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1997, orlegendcs, orlegend,  pgm_asic3,          orlegend, pgm_asic3_state,     orlegend,   ROT0,   "Kryso", "Oriental Legend / Xi Yo Gi Shi Re Zuang (Hack)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 2006, pgemeni,    pgm,       pgm_asic3,          pgm,      pgm_asic3_state,     orlegend,   ROT0,   "Blastar", "P-Gemeni", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 1997, pgmdemo,    pgm,       pgm_asic3,          pgm,      pgm_asic3_state,     orlegend,   ROT0,   "Charles Doty", "Demo - PGM", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2006, pgmfrog,    pgm,       pgm_asic3,          pgm,      pgm_asic3_state,     orlegend,   ROT0,   "Rastersoft", "Frog Feast (PGM)", MACHINE_SUPPORTS_SAVE )
//GAME( 1999, kovassg,    kov,       pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour (hack set assg)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
//GAME( 2008, kovassg,    kovshp,    pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovassg,    ROT0,   "bootleg", "Knights of Valour: Aoshi Sanguo / Sangoku Senki: Aoshi Sanguo (ver. 315CN)", MACHINE_IMPERFECT_SOUND | MACHINE_UNEMULATED_PROTECTION | MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE ) /* need internal rom of IGS027A */
GAME( 1999, kovdf,      kov,       pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour (hack set df)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsgqyzc,  kovplus,   pgm_arm_type1_sim,  sango_ch, pgm_arm_type1_state, kovboot,    ROT0,   "bootleg", "Knights of Valour: SanGuo QunYingZhuan / Sangoku Senki: SanGuo QunYingZhuan (bootleg, set 4)", MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 2001, kov2h1,     kov2p,     pgm_arm_type2,      kov2,     pgm_arm_type2_state, kov2p,      ROT0,   "Hack", "Knights of Valour 2 Plus (hack set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, kov2h2,     kov2p,     pgm_arm_type2,      kov2,     pgm_arm_type2_state, kov2p,      ROT0,   "Hack", "Knights of Valour 2 Plus (hack set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, kov2h3,     kov2p,     pgm_arm_type2,      kov2,     pgm_arm_type2_state, kov2p,      ROT0,   "Hack", "Knights of Valour 2 Plus (hack set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, kov2h4,     kov2p,     pgm_arm_type2,      kov2,     pgm_arm_type2_state, kov2p,      ROT0,   "Hack", "Knights of Valour 2 Plus (hack set 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, kov2h5,     kov2p,     pgm_arm_type2,      kov2,     pgm_arm_type2_state, kov2p,      ROT0,   "IGS", "Knights of Valour 2 Plus (ver M204XX)(hack set 5)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, kovph1,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus 2007 (hack set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph2,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph3,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus A948 (hack set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, kovph4,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "XM", "Knights of Valour Plus 2006 (hack set 4)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph5,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 5)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph6,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 6)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph7,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 7)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph8,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Fangzhoo", "Knights of Valour Plus HACK (hack set 8)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph9,     kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 9)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph10,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 10)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph11,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 11)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph12,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 12)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph13,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 13)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph14,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 14)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE ) // crashes in attract mode, but plays ok
GAME( 1999, kovph15,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 15)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph16,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 16)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 2012, kovph17,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hui Yi", "Knights of Valour Plus 2012 (hack set 17)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, kovph18,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hui Yi", "Knights of Valour Plus 2012 (hack set 18)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, kovph19,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 19)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 2012, kovph20,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 20)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 2012, kovph21,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 21)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph22,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 22)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph23,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 23)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph24,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "Huangxu", "Knights of Valour Plus / Sangoku Senki Plus (hack set 24)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovph25,    kovplus,   pgm_arm_type1_sim,  sango,    pgm_arm_type1_state, kov,        ROT0,   "IGS",  "Knights of Valour Plus / Sangoku Senki Plus (ver. 120)(hack set 25)", MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh1,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh2,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh3,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 3)", MACHINE_SUPPORTS_SAVE )
// Note: kovsh4-17 all have encrypted p-rom, and most of the other altered roms haven't been coded in yet
GAME( 1999, kovsh4,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 4)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh5,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 5)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh6,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 6)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh7,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 7)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh8,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 8)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh9,     kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 9)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh10,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 10)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh11,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 11)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh12,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 12)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh13,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 13)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh14,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 14)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh15,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 15)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh16,    kovsh,     pgm_arm_type1,      kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 16)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovsh17,    kovsh,     pgm_arm_type1_sim,  kovsh,    pgm_arm_type1_state, kovsh,      ROT0,   "Sertsuna", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (ver. 320)(hack set 17)", MACHINE_NOT_WORKING | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )


#if 0
// unused code from mame plus

void pgm_arm_type1_state::kovplus_fix_patch()
{
	UINT8 *mem8 = (UINT8 *)(memregion("maincpu")->base());

	mem8[0x1085D4] = 0x1B;
	mem8[0x1085D5] = 0x3;
	mem8[0x1085D6] = 0x1B;
	mem8[0x1085D7] = 0xA4;
	mem8[0x10EBEE] = 0x10;
	mem8[0x10EBEF] = 0x15;
	mem8[0x10EBF0] = 0x1A;
	mem8[0x10EBF1] = 0x11;
	mem8[0x11D3E4] = 0xC9;
	mem8[0x11D3E5] = 0xF8;
	mem8[0x11D3E6] = 0xB4;
	mem8[0x11D3E7] = 0x9A;
	mem8[0x11D492] = 0xA;
	mem8[0x11D493] = 0x5A;
	mem8[0x11D494] = 0xA;
	mem8[0x11D495] = 0x6D;
	mem8[0x11DAC6] = 0x18;
	mem8[0x11DAC7] = 0x3B;
	mem8[0x11DAC8] = 0xD1;
	mem8[0x11DAC9] = 0xFE;
	mem8[0x11DACA] = 0xE9;
	mem8[0x11DACB] = 0xE;
	mem8[0x11DACC] = 0x90;
	mem8[0x11DACD] = 0x6B;
	mem8[0x11DACE] = 0x90;
	mem8[0x11DACF] = 0xF;
	mem8[0x11DAD1] = 0x41;
	mem8[0x11DE56] = 0x12;
	mem8[0x11DE57] = 0x7F;
	mem8[0x11DE58] = 0xD3;
	mem8[0x11DE59] = 0x1C;
	mem8[0x11DE5A] = 0xEB;
	mem8[0x11DE5B] = 0xEA;
	mem8[0x11DE5D] = 0x86;
	mem8[0x11DE60] = 0x10;
	mem8[0x11DE61] = 0x7F;
	mem8[0x11DE62] = 0xD1;
	mem8[0x11DE63] = 0x82;
	mem8[0x11DE64] = 0xE9;
	mem8[0x11DE65] = 0xB6;
	mem8[0x11DF72] = 0x12;
	mem8[0x11DF73] = 0x53;
	mem8[0x11DF74] = 0xD3;
	mem8[0x11DF75] = 0x84;
	mem8[0x11DF76] = 0xEB;
	mem8[0x11DF77] = 0xB8;
	mem8[0x11DF79] = 0x2A;
	mem8[0x11DF7C] = 0x12;
	mem8[0x11DF7D] = 0x23;
	mem8[0x11DF7E] = 0xD3;
	mem8[0x11DF7F] = 0xDC;
	mem8[0x11DF80] = 0x69;
	mem8[0x11DF81] = 0x7C;
	mem8[0x12E7A4] = 0x95;
	mem8[0x12E7A5] = 0xCA;
	mem8[0x12E7A6] = 0x54;
	mem8[0x12E7A8] = 0x6C;
	mem8[0x12E7A9] = 0x5;
	mem8[0x13B25E] = 0xBA;
	mem8[0x13B25F] = 0xBF;
	mem8[0x1463A0] = 0x95;
	mem8[0x1463A1] = 0x87;
	mem8[0x1463A2] = 0x7F;
	mem8[0x1463A3] = 0x95;
	mem8[0x1463A4] = 0xB4;
	mem8[0x1463A5] = 0x31;
	mem8[0x1463A6] = 0x15;
	mem8[0x1463A8] = 0x1D;
	mem8[0x162408] = 0xB9;
	mem8[0x162409] = 0x63;
	mem8[0x1655E0] = 0x54;
	mem8[0x1655E2] = 0x68;
	mem8[0x1655E4] = 0x4D;
	mem8[0x1655E5] = 0xBA;
	mem8[0x1655E6] = 0xD1;
	mem8[0x1655E7] = 0x23;
	mem8[0x1655E8] = 0xE9;
	mem8[0x1655EA] = 0x28;
	mem8[0x1655EB] = 0x6A;
	mem8[0x183476] = 0xDC;
	mem8[0x183477] = 0xE7;
	mem8[0x183DB6] = 0x3D;
	mem8[0x32A00A] = 0x21;
	mem8[0x32A00C] = 0x46;
	mem8[0x32A00D] = 0x93;
	mem8[0x32A01E] = 0x8B;
	mem8[0x32A020] = 0x7E;
	mem8[0x32A021] = 0xD0;
	mem8[0x381000] = 0x50;
	mem8[0x381001] = 0x45;
	mem8[0x381002] = 0xE9;
	mem8[0x381003] = 0x1C;
	mem8[0x381004] = 0x87;
	mem8[0x381005] = 0x3F;
	mem8[0x381006] = 0x50;
	mem8[0x381007] = 0xE;
	mem8[0x381008] = 0x43;
	mem8[0x381009] = 0x62;
	mem8[0x38100A] = 0xC9;
	mem8[0x38100B] = 0x59;
	mem8[0x38100C] = 0xA7;
	mem8[0x38100D] = 0x4B;
	mem8[0x38100E] = 0xB0;
	mem8[0x38100F] = 0x4;
	mem8[0x381010] = 0x78;
	mem8[0x381011] = 0x67;
	mem8[0x381012] = 0xB9;
	mem8[0x381013] = 0x97;
	mem8[0x382000] = 0x50;
	mem8[0x382001] = 0x1B;
	mem8[0x382002] = 0x63;
	mem8[0x382003] = 0x1C;
	mem8[0x382004] = 0xE9;
	mem8[0x382005] = 0xE3;
	mem8[0x382006] = 0x87;
	mem8[0x382007] = 0xDE;
	mem8[0x382008] = 0xB0;
	mem8[0x382009] = 0x2C;
	mem8[0x38200A] = 0x58;
	mem8[0x38200B] = 0x59;
	mem8[0x38200C] = 0x15;
	mem8[0x38200D] = 0x49;
	mem8[0x383000] = 0x50;
	mem8[0x383001] = 0x1B;
	mem8[0x383002] = 0x60;
	mem8[0x383003] = 0x1C;
	mem8[0x383004] = 0xE9;
	mem8[0x383005] = 0xE3;
	mem8[0x383006] = 0x87;
	mem8[0x383007] = 0xDE;
	mem8[0x383008] = 0xB0;
	mem8[0x383009] = 0x2C;
	mem8[0x38300A] = 0x58;
	mem8[0x38300B] = 0x59;
	mem8[0x38300C] = 0x2F;
	mem8[0x38300D] = 0x49;
	mem8[0x384000] = 0x50;
	mem8[0x384001] = 0x1B;
	mem8[0x384002] = 0x63;
	mem8[0x384003] = 0x1C;
	mem8[0x384004] = 0xE9;
	mem8[0x384005] = 0xE3;
	mem8[0x384006] = 0x87;
	mem8[0x384007] = 0xDE;
	mem8[0x384008] = 0x90;
	mem8[0x384009] = 0x2C;
	mem8[0x38400A] = 0x78;
	mem8[0x38400B] = 0x59;
	mem8[0x38400C] = 0x11;
	mem8[0x38400D] = 0x48;
	mem8[0x385000] = 0x50;
	mem8[0x385001] = 0x1B;
	mem8[0x385002] = 0x60;
	mem8[0x385003] = 0x1C;
	mem8[0x385004] = 0xE9;
	mem8[0x385005] = 0xE3;
	mem8[0x385006] = 0x87;
	mem8[0x385007] = 0xDE;
	mem8[0x385008] = 0x90;
	mem8[0x385009] = 0x2C;
	mem8[0x38500A] = 0x78;
	mem8[0x38500B] = 0x59;
	mem8[0x38500C] = 0xEB;
	mem8[0x38500D] = 0x48;
	mem8[0x386000] = 0x5;
	mem8[0x386001] = 0x5D;
	mem8[0x386002] = 0x71;
	mem8[0x386003] = 0x1C;
	mem8[0x386004] = 0x6D;
	mem8[0x386005] = 0x84;
	mem8[0x386006] = 0x5;
	mem8[0x386007] = 0x40;
	mem8[0x386008] = 0x71;
	mem8[0x386009] = 0x62;
	mem8[0x38600A] = 0xC5;
	mem8[0x38600B] = 0x51;
	mem8[0x38600C] = 0x6C;
	mem8[0x38600D] = 0x97;
	mem8[0x38600E] = 0x79;
	mem8[0x38600F] = 0x4B;
	mem8[0x386010] = 0x90;
	mem8[0x386011] = 0x29;
	mem8[0x386012] = 0x7B;
	mem8[0x386013] = 0x4D;
	mem8[0x386014] = 0xC3;
	mem8[0x386015] = 0xF8;
}

void pgm_arm_type1_state::kovboot_fix_patch()
{
	UINT8 *mem8 = (UINT8 *)(memregion("maincpu")->base());

	mem8[0x1085D4] = 0x71;
	mem8[0x1085D5] = 0x4E;
	mem8[0x1085D6] = 0x71;
	mem8[0x1085D7] = 0x4E;
	mem8[0x11D3E4] = 0x29;
	mem8[0x11D3E5] = 0x42;
	mem8[0x11D3E6] = 0x54;
	mem8[0x11D3E7] = 0x29;
	mem8[0x11D492] = 0x71;
	mem8[0x11D493] = 0x4E;
	mem8[0x11D494] = 0x71;
	mem8[0x11D495] = 0x4E;
	mem8[0x11DAC6] = 0xF9;
	mem8[0x11DAC7] = 0x4E;
	mem8[0x11DAC8] = 0x38;
	mem8[0x11DAC9] = 0x0;
	mem8[0x11DACA] = 0x0;
	mem8[0x11DACB] = 0xF;
	mem8[0x11DACC] = 0x71;
	mem8[0x11DACD] = 0x4E;
	mem8[0x11DACE] = 0x71;
	mem8[0x11DACF] = 0x4E;
	mem8[0x11DAD1] = 0x65;
	mem8[0x11DE56] = 0xF9;
	mem8[0x11DE57] = 0x4E;
	mem8[0x11DE58] = 0x38;
	mem8[0x11DE59] = 0x0;
	mem8[0x11DE5A] = 0x20;
	mem8[0x11DE5B] = 0xF;
	mem8[0x11DE5D] = 0x64;
	mem8[0x11DE60] = 0xF9;
	mem8[0x11DE61] = 0x4E;
	mem8[0x11DE62] = 0x38;
	mem8[0x11DE63] = 0x0;
	mem8[0x11DE64] = 0x30;
	mem8[0x11DE65] = 0x0F;
	mem8[0x11DF72] = 0xF9;
	mem8[0x11DF73] = 0x4E;
	mem8[0x11DF74] = 0x38;
	mem8[0x11DF75] = 0x0;
	mem8[0x11DF76] = 0x40;
	mem8[0x11DF77] = 0x0F;
	mem8[0x11DF79] = 0x64;
	mem8[0x11DF7C] = 0xF9;
	mem8[0x11DF7D] = 0x4E;
	mem8[0x11DF7E] = 0x38;
	mem8[0x11DF7F] = 0x0;
	mem8[0x11DF80] = 0x50;
	mem8[0x11DF81] = 0x0F;
	mem8[0x12E7A4] = 0xF9;
	mem8[0x12E7A5] = 0x4E;
	mem8[0x12E7A6] = 0x38;
	mem8[0x12E7A8] = 0x60;
	mem8[0x12E7A9] = 0x0F;
	mem8[0x13B25E] = 0x71;
	mem8[0x13B25F] = 0x4E;
	mem8[0x1463A0] = 0xF9;
	mem8[0x1463A1] = 0x4E;
	mem8[0x1463A2] = 0x13;
	mem8[0x1463A3] = 0x0;
	mem8[0x1463A4] = 0xD0;
	mem8[0x1463A5] = 0xB5;
	mem8[0x1463A6] = 0x71;
	mem8[0x1463A8] = 0x71;
	mem8[0x162408] = 0xD0;
	mem8[0x162409] = 0x1;
	mem8[0x1655E0] = 0x3C;
	mem8[0x1655E2] = 0x0;
	mem8[0x1655E4] = 0x25;
	mem8[0x1655E5] = 0x0;
	mem8[0x1655E6] = 0xB9;
	mem8[0x1655E7] = 0x90;
	mem8[0x1655E8] = 0x81;
	mem8[0x1655EA] = 0x40;
	mem8[0x1655EB] = 0xB2;
	mem8[0x183476] = 0xB5;
	mem8[0x183477] = 0x1;
	mem8[0x183DB6] = 0x50;
	mem8[0x32A00A] = 0x68;
	mem8[0x32A00C] = 0xF;
	mem8[0x32A00D] = 0x4;
	mem8[0x32A01E] = 0xC0;
	mem8[0x32A020] = 0x13;
	mem8[0x32A021] = 0x2;
	mem8[0x380F00] = 0x39;
	mem8[0x380F01] = 0x52;
	mem8[0x380F02] = 0x80;
	mem8[0x380F03] = 0x0;
	mem8[0x380F04] = 0xEE;
	mem8[0x380F05] = 0xDC;
	mem8[0x380F06] = 0x39;
	mem8[0x380F07] = 0xC;
	mem8[0x380F08] = 0xA;
	mem8[0x380F09] = 0x0;
	mem8[0x380F0A] = 0x80;
	mem8[0x380F0B] = 0x0;
	mem8[0x380F0C] = 0xEE;
	mem8[0x380F0D] = 0xDC;
	mem8[0x380F0E] = 0xF9;
	mem8[0x380F0F] = 0x4E;
	mem8[0x380F10] = 0x11;
	mem8[0x380F11] = 0x0;
	mem8[0x380F12] = 0xD0;
	mem8[0x380F13] = 0xDA;
	mem8[0x380F20] = 0x39;
	mem8[0x380F21] = 0xC;
	mem8[0x380F22] = 0xA;
	mem8[0x380F23] = 0x0;
	mem8[0x380F24] = 0x80;
	mem8[0x380F25] = 0x0;
	mem8[0x380F26] = 0xEE;
	mem8[0x380F27] = 0xDC;
	mem8[0x380F28] = 0xF9;
	mem8[0x380F29] = 0x4E;
	mem8[0x380F2A] = 0x11;
	mem8[0x380F2B] = 0x0;
	mem8[0x380F2C] = 0x5C;
	mem8[0x380F2D] = 0xDE;
	mem8[0x380F30] = 0x39;
	mem8[0x380F31] = 0xC;
	mem8[0x380F32] = 0x9;
	mem8[0x380F33] = 0x0;
	mem8[0x380F34] = 0x80;
	mem8[0x380F35] = 0x0;
	mem8[0x380F36] = 0xEE;
	mem8[0x380F37] = 0xDC;
	mem8[0x380F38] = 0xF9;
	mem8[0x380F39] = 0x4E;
	mem8[0x380F3A] = 0x11;
	mem8[0x380F3B] = 0x0;
	mem8[0x380F3C] = 0x66;
	mem8[0x380F3D] = 0xDE;
	mem8[0x380F40] = 0x39;
	mem8[0x380F41] = 0xC;
	mem8[0x380F42] = 0xA;
	mem8[0x380F43] = 0x0;
	mem8[0x380F44] = 0x80;
	mem8[0x380F45] = 0x0;
	mem8[0x380F46] = 0xEE;
	mem8[0x380F47] = 0xDC;
	mem8[0x380F48] = 0xF9;
	mem8[0x380F49] = 0x4E;
	mem8[0x380F4A] = 0x11;
	mem8[0x380F4B] = 0x0;
	mem8[0x380F4C] = 0x78;
	mem8[0x380F4D] = 0xDF;
	mem8[0x380F50] = 0x39;
	mem8[0x380F51] = 0xC;
	mem8[0x380F52] = 0x9;
	mem8[0x380F53] = 0x0;
	mem8[0x380F54] = 0x80;
	mem8[0x380F55] = 0x0;
	mem8[0x380F56] = 0xEE;
	mem8[0x380F57] = 0xDC;
	mem8[0x380F58] = 0xF9;
	mem8[0x380F59] = 0x4E;
	mem8[0x380F5A] = 0x11;
	mem8[0x380F5B] = 0x0;
	mem8[0x380F5C] = 0x82;
	mem8[0x380F5D] = 0xDF;
	mem8[0x380F60] = 0x6C;
	mem8[0x380F61] = 0x4A;
	mem8[0x380F62] = 0x18;
	mem8[0x380F63] = 0x0;
	mem8[0x380F64] = 0x4;
	mem8[0x380F65] = 0x67;
	mem8[0x380F66] = 0x6C;
	mem8[0x380F67] = 0x42;
	mem8[0x380F68] = 0x18;
	mem8[0x380F69] = 0x0;
	mem8[0x380F6A] = 0xAC;
	mem8[0x380F6B] = 0x8;
	mem8[0x380F6C] = 0x5;
	mem8[0x380F6D] = 0x0;
	mem8[0x380F6E] = 0x10;
	mem8[0x380F6F] = 0x1;
	mem8[0x380F70] = 0xF9;
	mem8[0x380F71] = 0x4E;
	mem8[0x380F72] = 0x12;
	mem8[0x380F73] = 0x0;
	mem8[0x380F74] = 0xAA;
	mem8[0x380F75] = 0xE7;
}

void pgm_arm_type1_state::kovshp_fix_patch()
{
	UINT8 *mem8 = (UINT8 *)(memregion("prot")->base());

	mem8[0x2892] = 0x1;
	mem8[0x2893] = 0x1;
	mem8[0x289F] = 0x1;
	mem8[0x28A5] = 0x1;
	mem8[0x28A8] = 0x1;
	mem8[0x28A9] = 0x1;
	mem8[0x2BF3] = 0x48;
	mem8[0x2BF5] = 0x80;
	mem8[0x2C92] = 0xF;
	mem8[0x2C93] = 0x40;
	mem8[0x2CE1] = 0x6C;
	mem8[0x2CE2] = 0x48;
}

DRIVER_INIT_MEMBER(pgm_arm_type1_state,kovshp)
{
	pgm_basic_init();
	kovshp_fix_patch();    // extra line here
	pgm_kovshp_decrypt(machine());
	pgm_arm7_type1_latch_init();
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x4f0008, 0x4f0009, read16_delegate(FUNC(pgm_arm_type1_state::kovsh_fake_region_r),this));
	m_maincpu->space(AS_PROGRAM).install_write_handler(0x500000, 0x500005, write16_delegate(FUNC(pgm_arm_type1_state::kovshp_asic27a_write_word),this));
}

DRIVER_INIT_MEMBER(pgm_arm_type1_state,kovassga)
{
	pgm_basic_init();
	kovshp_fix_patch();     // extra line here
//  pgm_kovshp_decrypt(machine());
	pgm_arm7_type1_latch_init();
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x4f0008, 0x4f0009, read16_delegate(FUNC(pgm_arm_type1_state::kovsh_fake_region_r),this));
	m_maincpu->space(AS_PROGRAM).install_write_handler(0x500000, 0x500005, write16_delegate(FUNC(pgm_arm_type1_state::kovshp_asic27a_write_word),this));
}

void pgm_arm_type1_state::pgm_decode_kovassg_program()
{
	int i;
	UINT16 *src = (UINT16 *)(memregion("maincpu")->base() + 0x100000);
	std::vector<UINT16> dst(0x400000);

	for (i = 0; i < 0x400000 / 2; i++)
	{
		int j = (i & ~0xffff) | (BITSWAP16(i, 15, 14, 13, 12,  11, 10, 7, 3,  1, 9, 4, 8,  6, 0, 2, 5) ^ 0x019c);

		dst[j] = BITSWAP16(src[j], 13, 9, 10, 11, 2, 0, 12 ,5, 4, 1, 14, 8, 15, 6, 3, 7) ^ 0x9d05;
	}

	memcpy( src, &dst[0], 0x400000 );
}

DRIVER_INIT_MEMBER(pgm_arm_type1_state,kovassg)
{
	pgm_decode_kovassg_program();
	pgm_decode_kovlsqh2_tiles();

	pgm_decode_kovlsqh2_sprites(memregion("sprcol")->base() + 0x0000000);
	pgm_decode_kovlsqh2_sprites(memregion("sprcol")->base() + 0x0800000);
	pgm_decode_kovlsqh2_sprites(memregion("sprcol")->base() + 0x1000000);
	pgm_decode_kovlsqh2_sprites(memregion("sprcol")->base() + 0x1800000);
	pgm_decode_kovlsqh2_sprites(memregion("sprcol")->base() + 0x2000000);
	pgm_decode_kovlsqh2_sprites(memregion("sprcol")->base() + 0x2800000);
	pgm_decode_kovlsqh2_sprites(memregion("sprmask")->base() + 0x0000000);
	pgm_decode_kovlsqh2_sprites(memregion("sprmask")->base() + 0x0800000);

	pgm_decode_kovlsqh2_samples();
	pgm_basic_init();
	pgm_arm7_type1_latch_init();
	/* we only have a china internal ROM dumped for now.. allow region to be changed for debugging (to ensure all alt titles / regions can be seen) */
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x4f0008, 0x4f0009, read16_delegate(FUNC(pgm_arm_type1_state::kovsh_fake_region_r),this));
}

DRIVER_INIT_MEMBER(pgm_arm_type1_state,kovplus)
{
	pgm_basic_init();
	kovplus_fix_patch();     // extra line here
	pgm_kov_decrypt(machine());
	pgm_arm7_type1_latch_init();
	m_curslots = 0;
	m_kov_c0_value = 0;
	m_kov_cb_value = 0;
	m_kov_fe_value = 0;
	arm_sim_handler = &pgm_arm_type1_state::command_handler_kov;
	m_maincpu->space(AS_PROGRAM).install_readwrite_handler(0x500000, 0x500005, read16_delegate(FUNC(pgm_arm_type1_state::pgm_arm7_type1_sim_r),this), write16_delegate(FUNC(pgm_arm_type1_state::pgm_arm7_type1_sim_w),this));
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x4f0000, 0x4f003f, read16_delegate(FUNC(pgm_arm_type1_state::pgm_arm7_type1_sim_protram_r),this));
}

DRIVER_INIT_MEMBER(pgm_arm_type1_state,kovboot)
{
	pgm_basic_init();
//  pgm_kov_decrypt(machine());
	kovboot_fix_patch();     // extra line here
	pgm_arm7_type1_latch_init();
	m_curslots = 0;
	m_kov_c0_value = 0;
	m_kov_cb_value = 0;
	m_kov_fe_value = 0;
	arm_sim_handler = &pgm_arm_type1_state::command_handler_kov;
	m_maincpu->space(AS_PROGRAM).install_readwrite_handler(0x500000, 0x500005, read16_delegate(FUNC(pgm_arm_type1_state::pgm_arm7_type1_sim_r),this), write16_delegate(FUNC(pgm_arm_type1_state::pgm_arm7_type1_sim_w),this));
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x4f0000, 0x4f003f, read16_delegate(FUNC(pgm_arm_type1_state::pgm_arm7_type1_sim_protram_r),this));

}
#endif
