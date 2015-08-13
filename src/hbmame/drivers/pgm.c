// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/pgm.c"

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

ROM_START( ketarr )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarr_v100.u38", 0x000000, 0x200000, CRC(d4c7a8ab) SHA1(65d104d17bd4fd03a2b44297a003ba03d746c7ee) )

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

ROM_START( ketarrf )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarrf_v100.u38", 0x000000, 0x200000, CRC(6ad17aa4) SHA1(791bd1a107433a3811c8a79ea26a73e66ddd296f) )

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

ROM_START( ketarr15 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarr15_v100.u38", 0x000000, 0x200000, CRC(552a7d95) SHA1(4f3fb13f34d58a7482e1d26623d38aa0b54ca8dd) )

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

ROM_START( ketarrs15 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarrs15_v100.u38", 0x000000, 0x200000, CRC(a95e71e0) SHA1(182c12e3581ebb20176d8abca41ee62aadcd63e0) )

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

ROM_START( ketarr151 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarr151_v100.u38", 0x000000, 0x200000, CRC(2b7c030d) SHA1(9aaba1242d7ce29915a31d40341da82985927f9d) )

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

ROM_START( ketarrs151 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	/* doesn't use a separate BIOS rom */
	ROM_LOAD16_WORD_SWAP( "ketarrs151_v100.u38", 0x000000, 0x200000, CRC(35c984e4) SHA1(d4517f318de0c40a3b30e41374f33bb355581434) )

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

ROM_START( ketarr17 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "ketarr17_v100.u38", 0x000000, 0x200000, CRC(2cb80b89) SHA1(e1aa072b8344890486e11795e02703aa2d234bb1) )

	ROM_REGION( 0x4000, "prot", 0 )
	ROM_LOAD( "ket_igs027a.bin", 0x000000, 0x04000, CRC(ab54d286) SHA1(897256b6709e1a4da9daba92b6bde39ccfccd8c1) )

	ROM_REGION( 0xc00000, "tiles", 0 )
	ROM_LOAD( "pgm_t01s.rom", 0x000000, 0x200000, CRC(1a7123a0) SHA1(cc567f577bfbf45427b54d6695b11b74f2578af3) )
	ROM_LOAD( "t04701w064.u19", 0x180000, 0x800000, CRC(2665b041) SHA1(fb1107778b66f2af0de77ac82e1ee2902f53a959) ) //text-1

	ROM_REGION( 0x1000000, "sprcol", 0 )
	ROM_LOAD( "a04701w064.u7", 0x0000000, 0x0800000, CRC(5ef1b94b) SHA1(f10dfa46e0a4d297c3a856aea5b49d648f98935c) ) //image-1
	ROM_LOAD( "a04702w064.u8", 0x0800000, 0x0800000, CRC(26d6da7f) SHA1(f20e07a7994f41b5ed917f8b0119dc5542f3541c) ) //image-2

	ROM_REGION( 0x0800000, "sprmask", 0 )
	ROM_LOAD( "b04701w064.u1", 0x0000000, 0x0800000, CRC(1bec008d) SHA1(07d117dc2eebb35727fb18a7c563acbaf25a8d36) ) //bitmap-1

	ROM_REGION( 0x800000, "ics", ROMREGION_ERASE00 )
	ROM_LOAD( "m04701b032.u17", 0x400000, 0x400000, CRC(b46e22d1) SHA1(670853dc485942fb96380568494bdf3235f446ee) ) //music-1

	ROM_REGION( 0x20000, "sram", 0 ) /* default settings */
	ROM_LOAD( "ket_defaults.nv", 0x0000000, 0x020000, CRC(3ca892d8) SHA1(67430df5217e453ae8140c5653deeadfad8fa684) )
ROM_END

#if 0
ROM_START( kov )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.117",    0x100000, 0x400000, CRC(c4d19fe6) SHA1(14ef31539bfbc665e76c9703ee01b12228344052) )

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
#endif
ROM_START( kovp2k7 )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600.2007",    0x100000, 0x400000, CRC(390ebbfc) SHA1(e051a7659a14316c35075cfd2a2f2baba7f18f1a) )

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

ROM_START( kovpl116 )
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

ROM_START( kovpl948 )
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

ROM_START( kovpls06 )
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

ROM_START( kovpls93 )
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

ROM_START( kovpls97 )
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

ROM_START( kovpluslb )
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

ROM_START( kovpluslw )
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

ROM_START( kovpluspj )
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

ROM_START( kovplusq )
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

ROM_START( kovpluss8 )
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

ROM_START( kovplusx )
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

ROM_START( kovplusbe )
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

ROM_START( kovplusdz )
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

ROM_START( kovplusti )
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

ROM_START( kovplusup )
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

ROM_START( kovshhack )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600hack.322",    0x100000, 0x400000, CRC(c48df7de) SHA1(d97e3603dcc09c5018f333f21d5069eef89289dd) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsharm.bin", 0x000000, 0x04000,  CRC(9d8ecc0c) SHA1(51f288e8aa3dd1d4b7f24155fea9c6a7f4cd1144) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x400000, CRC(164b3c94) SHA1(f00ea66886ca6bff74bbeaa49e7f5c75c275d5d7) )

	/* all roms below need checking to see if they're the same on this board */
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
#if 0
ROM_START( kovshpa )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600pa.322",    0x100000, 0x400000, CRC(e251e8e4) SHA1(af5b7c81632a39e1450d932951bed634c76b84e8) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	/* all roms below need checking to see if they're the same on this board */
	ROM_REGION( 0x1e00000, "sprcol", 0 ) /* Sprite Colour Data */
	ROM_LOAD( "a0600.rom",    0x0000000, 0x0800000, CRC(d8167834) SHA1(fa55a99629d03b2ea253392352f70d2c8639a991) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0601.rom",    0x0800000, 0x0800000, CRC(ff7a4373) SHA1(7def9fca7513ad5a117da230bebd2e3c78679041) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0602.rom",    0x1000000, 0x0800000, CRC(e7a32959) SHA1(3d0ed684dc5b269238890836b2ce7ef46aa5265b) ) // FIXED BITS (xxxxxxxx1xxxxxxx)
	ROM_LOAD( "a0603.rom",    0x1800000, 0x0400000, CRC(ec31abda) SHA1(ee526655369bae63b0ef0730e9768b765c9950fc) )
	ROM_LOAD( "a0604pa.rom",    0x1a00000, 0x0400000, CRC(11b65f5b) SHA1(0b17c79cb1424097ae0c23472da6ebbf3dd07ce3) )

	ROM_REGION( 0x1000000, "sprmask", 0 ) /* Sprite Masks + Colour Indexes */
	ROM_LOAD( "b0600.rom",    0x0000000, 0x0800000, CRC(7d3cd059) SHA1(00cf994b63337e0e4ebe96453daf45f24192af1c) )
	ROM_LOAD( "b0601.rom",    0x0800000, 0x0400000, CRC(a0bb1c2f) SHA1(0542348c6e27779e0a98de16f04f9c18158f2b28) )
	ROM_LOAD( "b0602pa.rom",    0x0c00000, 0x0400000, CRC(47f93db4) SHA1(9467271ae6180a25a18f3c7ee2b0d660007385c5) )

	ROM_REGION( 0x800000, "ics", 0 ) /* Samples - (8 bit mono 11025Hz) - */
	PGM_AUDIO_BIOS
	ROM_LOAD( "m0600.rom",    0x400000, 0x400000, CRC(3ada4fd6) SHA1(4c87adb25d31cbd41f04fbffe31f7bc37173da76) )
ROM_END
#endif
ROM_START( kovshre )
	ROM_REGION( 0x600000, "maincpu", 0 ) /* 68000 Code */
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "p0600re.322",    0x100000, 0x2a3000, CRC(4c38c199) SHA1(48876b6a9cd85d39c4b49b7a47e66d66f2bfd375) )

	ROM_REGION( 0x4000, "prot", 0 ) /* ARM protection ASIC - internal rom */
	ROM_LOAD( "kovsh_v100_china.asic", 0x000000, 0x04000,  CRC(0f09a5c1) SHA1(621b38c05f33277608d58b49822aebc930ae4870) )

	ROM_REGION( 0xc00000, "tiles", 0 ) /* 8x8 Text Tiles + 32x32 BG Tiles */
	PGM_VIDEO_BIOS
	ROM_LOAD( "t0600.rom",    0x180000, 0x800000, CRC(4acc1ad6) SHA1(0668dbd5e856c2406910c6b7382548b37c631780) )

	/* all roms below need checking to see if they're the same on this board */
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



GAME( 2002, ketarr,     ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/04/17 ARRANGE VER (v1.0))", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2002, ketarrb,    ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/04/17 BACK. VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2002, ketarrf,    ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/04/17 FAST. VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2002, ketarr15,   ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/06/26 ARRANGE 1.5 VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2002, ketarrs15,  ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/06/27 MR.STOIC 1.5 VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2002, ketarr151,  ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/06/26 ARRANGE 1.51 VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2002, ketarrs151, ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2012/06/27 MR.STOIC 1.51 VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2014, ketarr17,   ket,       pgm_arm_type1_cave, pgm,   pgm_arm_type1_state, ket,        ROT270, "trap15", "Ketsui: Kizuna Jigoku Tachi (2014/07/16 ARRANGE 1.7 VER)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 2006, pgemeni,    pgm,       pgm_asic3,          pgm,   pgm_asic3_state,     orlegend,   ROT0,   "Blastar", "P-Gemeni", 0 )	// has no sound
GAME( 1997, pgmdemo,    pgm,       pgm_asic3,          pgm,   pgm_asic3_state,     orlegend,   ROT0,   "Charles Doty", "Demo - PGM", 0 )	// has no sound
GAME( 2006, pgmfrog,    pgm,       pgm_asic3,          pgm,   pgm_asic3_state,     orlegend,   ROT0,   "Rastersoft", "Frog Feast (PGM)", 0 )
GAME( 1999, kovp2k7,    kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 1)", MACHINE_IMPERFECT_SOUND | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpl116,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 2)", MACHINE_IMPERFECT_SOUND | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpl948,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpls06,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 4)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpls93,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 5)", MACHINE_IMPERFECT_SOUND | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpls97,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 6)", MACHINE_IMPERFECT_SOUND | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpluslb,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 7)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpluslw,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 8)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpluspj,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 9)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovplusq,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 10)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovpluss8,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 11)", MACHINE_IMPERFECT_SOUND | MACHINE_UNEMULATED_PROTECTION | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovplusx,   kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 12)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovplusbe,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 13)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovplusdz,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 14)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE ) // crashes in attract mode, but plays ok
GAME( 1999, kovplusti,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 15)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovplusup,  kovplus,   pgm_arm_type1_sim,  sango, pgm_arm_type1_state, kov,        ROT0,   "Hack", "Knights of Valour Plus (hack set 16)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovshhack,  kovsh,     pgm_arm_type1,      kovsh, pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 1)", MACHINE_SUPPORTS_SAVE )
//GAME( 1999, kovshpa,    kovsh,     pgm_arm_type1,      kovsh, pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 2)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1999, kovshre,    kovsh,     pgm_arm_type1,      kovsh, pgm_arm_type1_state, kovsh,      ROT0,   "Hack", "Knights of Valour Super Heroes / Sangoku Senki Super Heroes (hack set 3)", MACHINE_SUPPORTS_SAVE )
