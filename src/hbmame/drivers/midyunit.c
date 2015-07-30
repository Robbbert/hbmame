// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/midyunit.c"

ROM_START( mkla4d )
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )	/* sound CPU */
	ROM_LOAD (  "mks-u3.rom", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x200000, "adpcm:oki", 0 )	/* ADPCM */
	ROM_LOAD ( "mks-u12.rom", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(               0x40000, 0x40000 )
	ROM_LOAD ( "mks-u13.rom", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(               0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "user1", 0 )	/* 34010 code */
	ROM_LOAD16_BYTE( "mkg-hacked-u105.la4",  0x00000, 0x80000, CRC(80d5618c) SHA1(9bdfddbc70b61c94c1871abac1de153b8b728761) )
	ROM_LOAD16_BYTE(  "mkg-hacked-u89.la4",  0x00001, 0x80000, CRC(0478ae60) SHA1(186243cacc4d83070ef53b47f19668ca4c08474a) )

	ROM_REGION( 0x800000, "gfx1", 0 )
	ROM_LOAD ( "mkg-u111.rom",  0x000000, 0x80000, CRC(d17096c4) SHA1(01ef390a372c9d94adf138f9543ebb88b89f4c38) )
	ROM_LOAD ( "mkg-u112.rom",  0x080000, 0x80000, CRC(993bc2e4) SHA1(7791edbec2b4b8971a3e790346dd7564ecf16d5c) )
	ROM_LOAD ( "mkg-u113.rom",  0x100000, 0x80000, CRC(6fb91ede) SHA1(a3735b49f93b08c44fbc97e2b5aad394628fbe90) )
	ROM_LOAD ( "mkg-u114.rom",  0x180000, 0x80000, CRC(ed1ff88a) SHA1(6b090b658ee6148af953bd0c9216f37162b6460f) )

 	ROM_LOAD (  "mkg-u95.rom",  0x200000, 0x80000, CRC(a002a155) SHA1(3cf7909e92bcd428063596fc5b9953e0000d6eca) )
 	ROM_LOAD (  "mkg-u96.rom",  0x280000, 0x80000, CRC(dcee8492) SHA1(a912b74d3b26ebd1b1613cc631080f83ececeaf8) )
	ROM_LOAD (  "mkg-u97.rom",  0x300000, 0x80000, CRC(de88caef) SHA1(a7927b504dc56ca5c9048373977fe5743b0a3f0b) )
	ROM_LOAD (  "mkg-u98.rom",  0x380000, 0x80000, CRC(37eb01b4) SHA1(06092460bd137e08d0f8df8560942ed877d40e09) )

	ROM_LOAD ( "mkg-u106.rom",  0x400000, 0x80000, CRC(45acaf21) SHA1(5edd36c55f4e5d3c74fb85171728ec0a58284b12) )
 	ROM_LOAD ( "mkg-u107.rom",  0x480000, 0x80000, CRC(2a6c10a0) SHA1(cc90923c44f2961b945a0fd0f85ecc2ba04af2cb) )
  	ROM_LOAD ( "mkg-u108.rom",  0x500000, 0x80000, CRC(23308979) SHA1(0b36788624a1cf0d3f4c895be5ba967b8dfcf85e) )
 	ROM_LOAD ( "mkg-u109.rom",  0x580000, 0x80000, CRC(cafc47bb) SHA1(8610af6e52f7089ff4acd850c53ab8b4119e4445) )
ROM_END

GAME( 1992, mkla4d, mk, yunit_adpcm_6bit_fast, mkla4, midyunit_state, mkyunit, ROT0, "Midway", "Mortal Kombat (rev 4.0 09/28/92)", MACHINE_SUPPORTS_SAVE )
