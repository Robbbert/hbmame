// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/commando.cpp"

ROM_START( comdelux )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "cm04.9m",     0x0000, 0x8000, CRC(8438b694) SHA1(e154478d8f1b635355bd777370acabe49cb9d309) )
	ROM_LOAD( "cm03.8m",     0x8000, 0x4000, CRC(35486542) SHA1(531a85c9e03970ce037be84f2240c2df6f6e3ec1) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "cm02.9f",     0x0000, 0x4000, CRC(f9cc4a74) SHA1(ee8dd73919c6f47f62cc6d999de9510db9f79b8f) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "vt01.5d",     0x0000, 0x4000, CRC(505726e0) SHA1(2435c87c9c9d78a6e703cf0e1f6a0288207fcd4c) )	// characters

	ROM_REGION( 0x18000, "gfx2", 0 )
	ROM_LOAD( "vt11.5a",     0x00000, 0x4000, CRC(7b2e1b48) SHA1(5d49e1d8146e4ef744445b68f35677302e875a85) ) // SCR X (tiles)
	ROM_LOAD( "vt12.6a",     0x04000, 0x4000, CRC(81b417d3) SHA1(5ec7e3f0c8069384a5f6eb39232c228b9d7b8c0c) ) // SCR X
	ROM_LOAD( "vt13.7a",     0x08000, 0x4000, CRC(5612dbd2) SHA1(9e4e1a22b6cbf60607b9a81dae34482ae55f7c47) ) // SCR Y
	ROM_LOAD( "vt14.8a",     0x0c000, 0x4000, CRC(2b2dee36) SHA1(8792278464fa3da47176582025f6673a15a581e2) ) // SCR Y
	ROM_LOAD( "vt15.9a",     0x10000, 0x4000, CRC(de70babf) SHA1(6717e23baf55f84d3143fb432140a7c3e102ac26) ) // SCR Z
	ROM_LOAD( "vt16.10a",    0x14000, 0x4000, CRC(14178237) SHA1(f896e71c7004349c9a46155edfd9f0aaa186065d) ) // SCR Z

	ROM_REGION( 0x18000, "gfx3", 0 )
	ROM_LOAD( "dx_vt05.7e",  0x00000, 0x4000, CRC(b1839dd4) SHA1(7172718a0a5f5ba1c6785545dea196e03d3c58c3) )	/* sprites */
	ROM_LOAD( "vt06.8e",     0x04000, 0x4000, CRC(26fee521) SHA1(2fbfc73ee860f72a20229a01d4da9f5cc2e858d3) )
	ROM_LOAD( "dx_vt07.9e",  0x08000, 0x4000, CRC(4cb1cd67) SHA1(317cfbbff00870b7f7d8023b135d3a96dc0e73fb) )
	ROM_LOAD( "dx_vt08.7h",  0x0c000, 0x4000, CRC(42ff8a11) SHA1(94d0fdbb98670953e586f75030ec48b5af2138a9) )
	ROM_LOAD( "vt09.8h",     0x10000, 0x4000, CRC(98703982) SHA1(ba9a9b0dcadd4f52502828408c4a19b0bd518351) )
	ROM_LOAD( "dx_vt10.9h",  0x14000, 0x4000, CRC(7650a262) SHA1(e4d59c2dda161d49849b1d20782a2126fa0743c7) )

	ROM_REGION( 0x600, "proms", 0 )
	ROM_LOAD( "vtb1.1d", 	 0x0000, 0x0100, CRC(3aba15a1) SHA1(8b057f6e26155dd9e48bde182e680fce4519f600) )	/* red */
	ROM_LOAD( "vtb2.2d", 	 0x0100, 0x0100, CRC(88865754) SHA1(ca6dddca98baf00a65b2fb70b69cf4704ef8c831) )	/* green */
	ROM_LOAD( "vtb3.3d", 	 0x0200, 0x0100, CRC(4c14c3f6) SHA1(644ac17c7413f094ec9a15cba87bbd421b26321f) )	/* blue */
	ROM_LOAD( "vtb4.1h", 	 0x0300, 0x0100, CRC(b388c246) SHA1(038f9851699331ad887b6281a9df053dca3db8fd) )	/* palette selector (not used) */
	ROM_LOAD( "vtb5.6l", 	 0x0400, 0x0100, CRC(712ac508) SHA1(5349d722ab6733afdda65f6e0a98322f0d515e86) )	/* interrupt timing (not used) */
	ROM_LOAD( "vtb6.6e", 	 0x0500, 0x0100, CRC(0eaf5158) SHA1(bafd4108708f66cd7b280e47152b108f3e254fc9) )	/* video timing (not used) */
ROM_END

ROM_START( comdlux1 )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "cm04.9m",     0x0000, 0x8000, CRC(8438b694) SHA1(e154478d8f1b635355bd777370acabe49cb9d309) )
	ROM_LOAD( "cm03.8m",     0x8000, 0x4000, CRC(35486542) SHA1(531a85c9e03970ce037be84f2240c2df6f6e3ec1) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "cm02.9f",     0x0000, 0x4000, CRC(f9cc4a74) SHA1(ee8dd73919c6f47f62cc6d999de9510db9f79b8f) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "vt01.5d",     0x0000, 0x4000, CRC(505726e0) SHA1(2435c87c9c9d78a6e703cf0e1f6a0288207fcd4c) )	// characters

	ROM_REGION( 0x18000, "gfx2", 0 )
	ROM_LOAD( "vt11.5a",     0x00000, 0x4000, CRC(7b2e1b48) SHA1(5d49e1d8146e4ef744445b68f35677302e875a85) ) // SCR X (tiles)
	ROM_LOAD( "vt12.6a",     0x04000, 0x4000, CRC(81b417d3) SHA1(5ec7e3f0c8069384a5f6eb39232c228b9d7b8c0c) ) // SCR X
	ROM_LOAD( "vt13.7a",     0x08000, 0x4000, CRC(5612dbd2) SHA1(9e4e1a22b6cbf60607b9a81dae34482ae55f7c47) ) // SCR Y
	ROM_LOAD( "vt14.8a",     0x0c000, 0x4000, CRC(2b2dee36) SHA1(8792278464fa3da47176582025f6673a15a581e2) ) // SCR Y
	ROM_LOAD( "vt15.9a",     0x10000, 0x4000, CRC(de70babf) SHA1(6717e23baf55f84d3143fb432140a7c3e102ac26) ) // SCR Z
	ROM_LOAD( "vt16.10a",    0x14000, 0x4000, CRC(14178237) SHA1(f896e71c7004349c9a46155edfd9f0aaa186065d) ) // SCR Z

	ROM_REGION( 0x18000, "gfx3", 0 )
	ROM_LOAD( "dx1_vt05.7e", 0x00000, 0x4000, CRC(91865879) SHA1(151b3202ea16141f53bf7ae96ac2171b70564ee5) )	/* sprites */
	ROM_LOAD( "vt06.8e",     0x04000, 0x4000, CRC(26fee521) SHA1(2fbfc73ee860f72a20229a01d4da9f5cc2e858d3) )
	ROM_LOAD( "dx_vt07.9e",  0x08000, 0x4000, CRC(4cb1cd67) SHA1(317cfbbff00870b7f7d8023b135d3a96dc0e73fb) )
	ROM_LOAD( "dx1_vt08.7h", 0x0c000, 0x4000, CRC(ba3a06f7) SHA1(9ae9fbd490308e27b71bc1dcd54f3e4e420feb3f) )
	ROM_LOAD( "vt09.8h",     0x10000, 0x4000, CRC(98703982) SHA1(ba9a9b0dcadd4f52502828408c4a19b0bd518351) )
	ROM_LOAD( "dx_vt10.9h",  0x14000, 0x4000, CRC(7650a262) SHA1(e4d59c2dda161d49849b1d20782a2126fa0743c7) )

	ROM_REGION( 0x600, "proms", 0 )
	ROM_LOAD( "vtb1.1d",     0x0000, 0x0100, CRC(3aba15a1) SHA1(8b057f6e26155dd9e48bde182e680fce4519f600) )	/* red */
	ROM_LOAD( "vtb2.2d",     0x0100, 0x0100, CRC(88865754) SHA1(ca6dddca98baf00a65b2fb70b69cf4704ef8c831) )	/* green */
	ROM_LOAD( "vtb3.3d",	 0x0200, 0x0100, CRC(4c14c3f6) SHA1(644ac17c7413f094ec9a15cba87bbd421b26321f) )	/* blue */
	ROM_LOAD( "vtb4.1h",	 0x0300, 0x0100, CRC(b388c246) SHA1(038f9851699331ad887b6281a9df053dca3db8fd) )	/* palette selector (not used) */
	ROM_LOAD( "vtb5.6l", 	 0x0400, 0x0100, CRC(712ac508) SHA1(5349d722ab6733afdda65f6e0a98322f0d515e86) )	/* interrupt timing (not used) */
	ROM_LOAD( "vtb6.6e", 	 0x0500, 0x0100, CRC(0eaf5158) SHA1(bafd4108708f66cd7b280e47152b108f3e254fc9) )	/* video timing (not used) */
ROM_END

GAME( 2002, comdelux, commando, commando, commandou, commando_state, commando, ROT270, "Twisty", "Commando Deluxe", MACHINE_SUPPORTS_SAVE )
GAME( 1985, comdlux1, commando, commando, commandou, commando_state, commando, ROT270, "Twisty", "Commando Deluxe Again", MACHINE_SUPPORTS_SAVE )
