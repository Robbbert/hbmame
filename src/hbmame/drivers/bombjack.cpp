// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/bombjack.cpp"

ROM_START( bmbjckgr )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "09_j01b.bin",  0x0000, 0x2000, CRC(c668dc30) SHA1(51dd6a2688b42e9f28f0882bd76f75be7ec3222a) )
	ROM_LOAD( "10_l01b.bin",  0x2000, 0x2000, CRC(52a1e5fb) SHA1(e1cdc4b4efbc6c7a1e4fa65019486617f2acba1b) )
	ROM_LOAD( "11_m01b.bin",  0x4000, 0x2000, CRC(b68a062a) SHA1(43bae56494ac0202aaa8f1ed5c1ed1bff775b2b8) )
	ROM_LOAD( "12g_n01b.bin", 0x6000, 0x2000, CRC(b457909f) SHA1(807b30742a89a327721596c4f5d091912f0ab4ec) )
	ROM_LOAD( "13g.1r",       0xc000, 0x2000, CRC(f8e14e2c) SHA1(a5dff87ad26eff6a07313594d9bce2ea568c8f26) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for sound board */
	ROM_LOAD( "01_h03t.bin",  0x0000, 0x2000, CRC(8407917d) SHA1(318face9f7a7ab6c7eeac773995040425e780aaf) )

	ROM_REGION( 0x3000, "chars", 0 )
	ROM_LOAD( "03g_e08t.bin", 0x0000, 0x1000, CRC(df120e35) SHA1(b389320cd95fe3ac7bfb613098140d169c0485b1) )	/* chars */
	ROM_LOAD( "04g_h08t.bin", 0x1000, 0x1000, CRC(2af09b2b) SHA1(4ce87353a941c6527c9426e578ed2a6c7286175e) )
	ROM_LOAD( "05g_k08t.bin", 0x2000, 0x1000, CRC(4362417c) SHA1(680a5c06c938e7cca33880bc18a2b592a4204ec8) )

	ROM_REGION( 0x6000, "tiles", 0 )
	ROM_LOAD( "06_l08t.bin",  0x0000, 0x2000, CRC(51eebd89) SHA1(515128a3971fcb97b60c5b6bdd2b03026aec1921) )	/* background tiles */
	ROM_LOAD( "07_n08t.bin",  0x2000, 0x2000, CRC(9dd98e9d) SHA1(6db6006a6e20ff7c243d88293ca53681c4703ea5) )
	ROM_LOAD( "08_r08t.bin",  0x4000, 0x2000, CRC(3155ee7d) SHA1(e7897dca4c145f10b7d975b8ef0e4d8aa9354c25) )

	ROM_REGION( 0x6000, "sprites", 0 )
	ROM_LOAD( "16g_m07b.bin", 0x0000, 0x2000, CRC(e33942bf) SHA1(a9fad16f9b83faf1c0948ad15186959ec17eb448) )	/* sprites */
	ROM_LOAD( "15g_l07b.bin", 0x2000, 0x2000, CRC(1bb8ec79) SHA1(14c8b6ee9f22491330bbf14356fa4ab3c62cb811) )
	ROM_LOAD( "14g_j07b.bin", 0x4000, 0x2000, CRC(0a9b3106) SHA1(bfd1c430e3e96896ebf4db962f755353a9a57be8) )

	ROM_REGION( 0x1000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "02_p04t.bin",  0x0000, 0x1000, CRC(398d4a02) SHA1(ac18a8219f99ba9178b96c9564de3978e39c59fd) )
ROM_END

/* E720 - rom 13 = the old rom11+rom12; rom 14 = the old rom09+rom10+alterations */
ROM_START( bombjckb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "bombjckb.14",  0x0000, 0x4000, CRC(129f94f7) SHA1(499625812166c5288efe74d75c8fbc7ce8f197eb) )
	ROM_LOAD( "bombjckb.13",  0x4000, 0x4000, CRC(87fa35ad) SHA1(42d82658609cbc88928fb2fdf579eb117d4b0cbc) )
	ROM_LOAD( "13.1r",        0xc000, 0x2000, CRC(70e0244d) SHA1(67654155e42821ea78a655f869fb81c8d6387f63) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for sound board */
	ROM_LOAD( "01_h03t.bin",  0x0000, 0x2000, CRC(8407917d) SHA1(318face9f7a7ab6c7eeac773995040425e780aaf) )

	ROM_REGION( 0x3000, "chars", 0 )
	ROM_LOAD( "03_e08t.bin",  0x0000, 0x1000, CRC(9f0470d5) SHA1(94ef52ef47b4399a03528fe3efeac9c1d6983446) )	/* chars */
	ROM_LOAD( "04_h08t.bin",  0x1000, 0x1000, CRC(81ec12e6) SHA1(e29ba193f21aa898499187603b25d2e226a07c7b) )
	ROM_LOAD( "05_k08t.bin",  0x2000, 0x1000, CRC(e87ec8b1) SHA1(a66808ef2d62fca2854396898b86bac9be5f17a3) )

	ROM_REGION( 0x6000, "tiles", 0 )
	ROM_LOAD( "06_l08t.bin",  0x0000, 0x2000, CRC(51eebd89) SHA1(515128a3971fcb97b60c5b6bdd2b03026aec1921) )	/* background tiles */
	ROM_LOAD( "07_n08t.bin",  0x2000, 0x2000, CRC(9dd98e9d) SHA1(6db6006a6e20ff7c243d88293ca53681c4703ea5) )
	ROM_LOAD( "08_r08t.bin",  0x4000, 0x2000, CRC(3155ee7d) SHA1(e7897dca4c145f10b7d975b8ef0e4d8aa9354c25) )

	ROM_REGION( 0x6000, "sprites", 0 )
	ROM_LOAD( "16_m07b.bin",  0x0000, 0x2000, CRC(94694097) SHA1(de71bcd67f97d05527f2504fc8430be333fb9ec2) )	/* sprites */
	ROM_LOAD( "15_l07b.bin",  0x2000, 0x2000, CRC(013f58f2) SHA1(20c64593ab9fcb04cefbce0cd5d17ce3ff26441b) )
	ROM_LOAD( "14_j07b.bin",  0x4000, 0x2000, CRC(101c858d) SHA1(ed1746c15cdb04fae888601d940183d5c7702282) )

	ROM_REGION( 0x1000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "02_p04t.bin",  0x0000, 0x1000, CRC(398d4a02) SHA1(ac18a8219f99ba9178b96c9564de3978e39c59fd) )
ROM_END

ROM_START( bombjred )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "09_j01b.bin",  0x0000, 0x2000, CRC(c668dc30) SHA1(51dd6a2688b42e9f28f0882bd76f75be7ec3222a) )
	ROM_LOAD( "10_l01b.bin",  0x2000, 0x2000, CRC(52a1e5fb) SHA1(e1cdc4b4efbc6c7a1e4fa65019486617f2acba1b) )
	ROM_LOAD( "11_m01b.bin",  0x4000, 0x2000, CRC(b68a062a) SHA1(43bae56494ac0202aaa8f1ed5c1ed1bff775b2b8) )
	ROM_LOAD( "12_n01b.bin",  0x6000, 0x2000, CRC(1d3ecee5) SHA1(8b3c49e21ea4952cae7042890d1be2115f7d6fda) )
	ROM_LOAD( "13.1r",        0xc000, 0x2000, CRC(70e0244d) SHA1(67654155e42821ea78a655f869fb81c8d6387f63) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for sound board */
	ROM_LOAD( "01_h03t.bin",  0x0000, 0x2000, CRC(8407917d) SHA1(318face9f7a7ab6c7eeac773995040425e780aaf) )

	ROM_REGION( 0x3000, "chars", 0 )
	ROM_LOAD( "bombjred.03",  0x0000, 0x1000, CRC(8732fa2b) SHA1(e9919bd812a44d6edaf7a08e6cb857cdee2af057) )	/* chars */
	ROM_LOAD( "bombjred.04",  0x1000, 0x1000, CRC(dc392a25) SHA1(8ace01ef128b92ff04d668fdd3c2be9cb9a25e4d) )
	ROM_LOAD( "bombjred.05",  0x2000, 0x1000, CRC(ad9d7a8c) SHA1(8ae81bf891544c825115925b38a53c89c1ca2f65) )

	ROM_REGION( 0x6000, "tiles", 0 )
	ROM_LOAD( "06_l08t.bin",  0x0000, 0x2000, CRC(51eebd89) SHA1(515128a3971fcb97b60c5b6bdd2b03026aec1921) )	/* background tiles */
	ROM_LOAD( "07_n08t.bin",  0x2000, 0x2000, CRC(9dd98e9d) SHA1(6db6006a6e20ff7c243d88293ca53681c4703ea5) )
	ROM_LOAD( "08_r08t.bin",  0x4000, 0x2000, CRC(3155ee7d) SHA1(e7897dca4c145f10b7d975b8ef0e4d8aa9354c25) )

	ROM_REGION( 0x6000, "sprites", 0 )
	ROM_LOAD( "bombjred.16",  0x0000, 0x2000, CRC(bdabd682) SHA1(3caf238295ba4d033d50b8e2a51e6c2bd0ad6bde) )	/* sprites */
	ROM_LOAD( "bombjred.15",  0x2000, 0x2000, CRC(a213f039) SHA1(67e247c4d3fe2b4791d3c5de9bb242c21f31c84d) )
	ROM_LOAD( "bombjred.14",  0x4000, 0x2000, CRC(498513fe) SHA1(81851994da287982a67a4e47231404f534b6b9df) )

	ROM_REGION( 0x1000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "02_p04t.bin",  0x0000, 0x1000, CRC(398d4a02) SHA1(ac18a8219f99ba9178b96c9564de3978e39c59fd) )
ROM_END

GAME( 1984, bmbjckgr, bombjack, bombjack, bombjack, driver_device, 0, ROT90, "Hack", "Bomb Jack (Greek)", MACHINE_SUPPORTS_SAVE )
GAME( 1984, bombjckb, bombjack, bombjack, bombjack, driver_device, 0, ROT90, "bootleg", "Bomb Jack (Bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2008, bombjred, bombjack, bombjack, bombjack, driver_device, 0, ROT90, "hack", "Bomb Jack Red", MACHINE_SUPPORTS_SAVE )
