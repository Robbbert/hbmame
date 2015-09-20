// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/toaplan2.c"

ROM_START( kgpe )
	ROM_REGION( 0x100000, "maincpu", 0 )			/* Main 68K code */
	ROM_LOAD16_BYTE( "ma02rom1.bin", 0x000000, 0x080000, CRC(a678b149) SHA1(8c1a631e023dbba0a3fa6cd1b7d10dec1663213a) )
	ROM_LOAD16_BYTE( "ma02rom0.bin", 0x000001, 0x080000, CRC(f226a212) SHA1(526acf3d05fdc88054a772fbea3de2af532bf3d2) )

	ROM_REGION( 0x10000, "audiocpu", 0 )			/* Sound Z80 code */
	ROM_LOAD( "ma02rom2.bin", 0x00000, 0x10000, CRC(dde8a57e) SHA1(f522a3f17e229c71512464349760a9e27778bf6a) )

	ROM_REGION( 0x400000, "gp9001", 0 )
	ROM_LOAD( "ma02rom3.bin",  0x000000, 0x200000, CRC(0e797142) SHA1(a480ccd151e49b886d3175a6deff56e1f2c26c3e) )
	ROM_LOAD( "ma02rom4.bin",  0x200000, 0x200000, CRC(72a6fa53) SHA1(ce92e65205b84361cfb90305a61e9541b5c4dc2f) )

	ROM_REGION( 0x008000, "text", 0 )
	ROM_LOAD( "kgpe.05",  0x000000, 0x008000, CRC(a0707702) SHA1(b6105291c1ae3d0792e69fd2495c31b56fbf61b7) )

	ROM_REGION( 0x80000, "oki", 0 )			/* ADPCM Samples */
	ROM_LOAD( "ma02rom6.bin", 0x00000, 0x80000, CRC(199e7cae) SHA1(0f5e13cc8ec42c80bb4bbff90aba29cdb15213d4) )
ROM_END

GAME( 1994, kgpe, 0, shippumd, kingdmgp, driver_device, 0, ROT270, "Neil Corlett", "Kingdom Grand Prix" , MACHINE_SUPPORTS_SAVE )

/* This game can be obtained from the Raine site, use a hex editor to zero out the last 4 bytes,
     and rename the result to "kgpe.05". Thanks goes to the Raine devs for the idea and the rom,
     and Tourniquet for the byte modification. Without this mod, the rom has the same crc as "ma02rom5.eng"
     from "kingdmgp", which caused all sorts of confusion and problems. */

ROM_START( mahoudcn )
	ROM_REGION( 0x080000, "maincpu", 0 )			/* Main 68K code */
	ROM_LOAD16_WORD_SWAP( "ra_ma_01_01.u65", 0x000000, 0x080000, CRC(970ccc5c) SHA1(c87cab83bde0284e631f02e50068407fee81d941) )
	
	ROM_REGION( 0x10000, "audiocpu", 0 )			/* Sound Z80 code */
	ROM_LOAD( "ra-ma-01_02.u66", 0x00000, 0x10000, CRC(eabfa46d) SHA1(402c99ebf88f9025f74f0a28ced22b7882a65eb3) )

	ROM_REGION( 0x200000, "gp9001", 0 )
	ROM_LOAD( "ra_ma_01_cn.03",  0x000000, 0x100000, CRC(7441e469) SHA1(ba1b127b6878ec45054c7220510a1ec0a8c06a0a) )
	ROM_LOAD( "ra_ma_01_cn.04",  0x100000, 0x100000, CRC(eb9f8159) SHA1(db8e10b847b0fbb34136c1a320ca0a160ac2646e) )
	
	ROM_REGION( 0x008000, "text", 0 )
	ROM_LOAD( "ra_ma_01_05.u81",  0x000000, 0x008000, CRC(c00d1e80) SHA1(53e64c4c0c6309130b37597d13b44a9e95b717d8) )

	ROM_REGION( 0x40000, "oki", 0 )			/* ADPCM Samples */
	ROM_LOAD( "ra-ma01-rom1.u57", 0x00000, 0x40000, CRC(6edb2ab8) SHA1(e3032e8eda2686f30df4b7a088c5a4d4d45782ed) )
ROM_END

ROM_START( batsugunt )
	ROM_REGION( 0x080000, "maincpu", 0 )			/* Main 68K code */
	ROM_LOAD16_WORD_SWAP( "tp030_01t.bin", 0x000000, 0x080000, CRC(63d28fd3) SHA1(aa3e111d596f408b735e4f79ab654eea24bd6f0c) )

	/* Secondary CPU is a Toaplan marked chip, (TS-007-Spy  TOA PLAN) */
	/* It's a NEC V25 (PLCC94) (program uploaded by main CPU) */

	ROM_REGION( 0x400000, "gp9001", 0 )
	ROM_LOAD( "tp030_3l.bin", 0x000000, 0x100000, CRC(3024b793) SHA1(e161db940f069279356fca2c5bf2753f07773705) )
	ROM_LOAD( "tp030_3h.bin", 0x100000, 0x100000, CRC(ed75730b) SHA1(341f0f728144a049486d996c9bb14078578c6879) )
	ROM_LOAD( "tp030_4l.bin", 0x200000, 0x100000, CRC(fedb9861) SHA1(4b0917056bd359b21935358c6bcc729262be6417) )
	ROM_LOAD( "tp030_4h.bin", 0x300000, 0x100000, CRC(d482948b) SHA1(31be7dc5cff072403b783bf203b9805ffcad7284) )

	ROM_REGION( 0x200000, "gp9001_1", 0 )
	ROM_LOAD( "tp030_5.bin",  0x000000, 0x100000, CRC(bcf5ba05) SHA1(40f98888a29cdd30cda5dfb60fdc667c69b0fdb0) )
	ROM_LOAD( "tp030_6.bin",  0x100000, 0x100000, CRC(0666fecd) SHA1(aa8f921fc51590b5b05bbe0b0ad0cce5ff359c64) )

	ROM_REGION( 0x40000, "oki", 0 )			/* ADPCM Samples */
	ROM_LOAD( "tp030_2.bin", 0x00000, 0x40000, CRC(276146f5) SHA1(bf11d1f6782cefcad77d52af4f7e6054a8f93440) )
ROM_END

ROM_START( bgareggz )
	ROM_REGION( 0x100000, "maincpu", 0 )            /* Main 68K code */
	ROM_LOAD16_BYTE( "prg0z.bin", 0x000000, 0x080000, CRC(6f4af466) SHA1(fb77f3b86c0b25941ae0efe70ca779d06f66f02b) )
	ROM_LOAD16_BYTE( "prg1z.bin", 0x000001, 0x080000, CRC(b4dc9a48) SHA1(9310a8c1de2a64e5597206a9974f25351d7825c2) )

	ROM_REGION( 0x20000, "audiocpu", 0 )            /* Sound Z80 code + bank */
	ROM_LOAD( "snd.bin", 0x00000, 0x20000, CRC(68632952) SHA1(fb834db83157948e2b420b6051102a9c6ac3969b) )

	ROM_REGION( 0x800000, "gp9001", 0 )
	ROM_LOAD( "rom4.bin",  0x000000, 0x200000, CRC(b333d81f) SHA1(5481465f1304334fd55798be2f44324c57c2dbcb) )
	ROM_LOAD( "rom3.bin",  0x200000, 0x200000, CRC(51b9ebfb) SHA1(30e0c326f5175aa436df8dba08f6f4e08130b92f) )
	ROM_LOAD( "rom2.bin",  0x400000, 0x200000, CRC(b330e5e2) SHA1(5d48e9d56f99d093b6390e0af1609fd796df2d35) )
	ROM_LOAD( "rom1.bin",  0x600000, 0x200000, CRC(7eafdd70) SHA1(7c8da8e86c3f9491719b1d7d5d285568d7614f38) )

	ROM_REGION( 0x010000, "text", 0 )
	ROM_LOAD( "text.u81", 0x00000, 0x08000, CRC(e67fd534) SHA1(987d0edffc2c243a13d4567319ea3d185eaadbf8) )

	ROM_REGION( 0x140000, "oki", 0 )        /* ADPCM Samples */
	ROM_LOAD( "rom5.bin", 0x040000, 0x100000, CRC(f6d49863) SHA1(3a3c354852adad06e8a051511abfab7606bce382) )
ROM_END


GAME( 1993, mahoudcn,   sstriker, mahoudai, mahoudai,   driver_device,  0,       ROT270, "Raizing (Able license)", "Mahou Daisakusen (Chinese)", MACHINE_SUPPORTS_SAVE )
GAME( 1993, batsugunt,  batsugun, batsugun, batsugun,   toaplan2_state, dogyuun, ROT270, "Eezezy", "Batsugun Transparent (HACK BY EEZEZY(LB70))", MACHINE_SUPPORTS_SAVE )
GAME( 1996, bgareggz,   bgaregga, bgaregga, bgaregga,   toaplan2_state, bgaregga,ROT270, "Zack", "Battle Garegga (Europe / USA / Japan / Asia) (Zakk version)", MACHINE_SUPPORTS_SAVE )
