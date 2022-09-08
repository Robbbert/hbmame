// license:BSD-3-Clause
// copyright-holders:Robbbert
//***************************************************************************************************************
#include "../mame/drivers/mcatadv.cpp"


ROM_START( mcatadvs01 )  // Local text hack to show someone's company name
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mcas01.u30", 0x00000, 0x80000, CRC(009ae1b3) SHA1(7bee7d3a9a6fd67d09acdfbef49f3e2be6db4712) )
	ROM_LOAD16_BYTE( "mcas01.u29", 0x00001, 0x80000, CRC(7fd27089) SHA1(c97f5ce13d8dfeaaed94ec621037e030f7a7cec7) )

	ROM_REGION( 0x020000, "soundcpu", 0 )
	ROM_LOAD( "u9.bin", 0x00000, 0x20000, CRC(fda05171) SHA1(2c69292573ec35034572fa824c0cae2839d23919) )

	ROM_REGION( 0x800000, "sprdata", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mca-u82.bin", 0x000000, 0x100000, CRC(5f01d746) SHA1(11b241456e15299912ee365eedb8f9d5e5ca875d) )
	ROM_LOAD16_BYTE( "mca-u83.bin", 0x000001, 0x100000, CRC(4e1be5a6) SHA1(cb19aad42dba54d6a4a33859f27254c2a3271e8c) )
	ROM_LOAD16_BYTE( "mca-u84.bin", 0x200000, 0x080000, CRC(df202790) SHA1(f6ae54e799af195860ed0ab3c85138cf2f10efa6) )
	ROM_LOAD16_BYTE( "mca-u85.bin", 0x200001, 0x080000, CRC(a85771d2) SHA1(a1817cd72f5bf0a4f24a37c782dc63ecec3b8e68) )
	ROM_LOAD16_BYTE( "mca-u86e",    0x400000, 0x080000, CRC(017bf1da) SHA1(f6446a7219275c0eff62129f59fdfa3a6a3e06c8) )
	ROM_LOAD16_BYTE( "mca-u87e",    0x400001, 0x080000, CRC(bc9dc9b9) SHA1(f525c9f994d5107752aa4d3a499ee376ec75f42b) )

	ROM_REGION( 0x080000, "bg0", 0 )
	ROM_LOAD( "mca-u58.bin", 0x000000, 0x080000, CRC(3a8186e2) SHA1(129c220d72608a8839f779ce1a6cfec8646dbf23) )

	ROM_REGION( 0x280000, "bg1", 0 )
	ROM_LOAD( "mca-u60.bin", 0x000000, 0x100000, CRC(c8942614) SHA1(244fccb9abbb04e33839dd2cd0e2de430819a18c) )
	ROM_LOAD( "mca-u61.bin", 0x100000, 0x100000, CRC(51af66c9) SHA1(1055cf78ea286f02003b0d1bf08c2d7829b36f90) )
	ROM_LOAD( "mca-u100",    0x200000, 0x080000, CRC(b273f1b0) SHA1(39318fe2aaf2792b85426ec6791b3360ac964de3) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "mca-u53.bin", 0x00000, 0x80000, CRC(64c76e05) SHA1(379cef5e0cba78d0e886c9cede41985850a3afb7) )
ROM_END

GAME( 1993, mcatadvs01,  mcatadv,   mcatadv, mcatadv, mcatadv_state, empty_init, ROT0, "Recreativos Zara, S.L.", "Magical Cat Adventure (R.Zara)", MACHINE_SUPPORTS_SAVE )
