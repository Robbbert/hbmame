// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/asuka.c"

ROM_START( asukac )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "b68-13c.ic23", 0x00000, 0x20000, CRC(356a3151) SHA1(a2929a30bcd45962a4fc08a980e82d9914078c29) )
	ROM_LOAD16_BYTE( "b68-12c.ic8",  0x00001, 0x20000, CRC(8bd073d4) SHA1(53453cb0dd4aa606215922ad8363e536524dc0f3) )
	ROM_LOAD16_WORD( "b68-03c.ic30", 0x80000, 0x80000, CRC(3d0d7798) SHA1(a63d9fc043e8ba0b79a7606c17faac406e0b92ec) )	/* Fix ROM */

	ROM_REGION( 0x80000, "gfx1", 0 )
	ROM_LOAD( "b68-01c.ic3",  0x00000, 0x80000, CRC(d5dec087) SHA1(1f8888c15cb08c1c0d27ce29cb71106360c9c852) )	/* SCR tiles (8 x 8) */

	ROM_REGION( 0xa0000, "gfx2", 0 )
	ROM_LOAD       ( "b68-02c.ic6", 0x00000, 0x80000, CRC(09927d23) SHA1(b9761286feaa14c4eb6fc87ec35e7245c318d687) )	/* Sprites (16 x 16) */
	ROM_LOAD16_BYTE( "b68-07.ic5", 0x80000, 0x10000, CRC(c113acc8) SHA1(613c61a78df73dcb0b9c9018ae829e865baac772) )
	ROM_LOAD16_BYTE( "b68-06.ic4", 0x80001, 0x10000, CRC(f517e64d) SHA1(8be491bfe0f7eed58521de9d31da677acf635c23) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "b68-11.ic27", 0x00000, 0x04000, CRC(c378b508) SHA1(1b145fe736b924f298e02532cf9f26cc18b42ca7) )
	ROM_CONTINUE(            0x10000, 0x0c000 )	/* banked stuff */

	ROM_REGION( 0x10000, "ymsnd", 0 )
	ROM_LOAD( "b68-10.ic24", 0x00000, 0x10000, CRC(387aaf40) SHA1(47c583564ef1d49ece15f97221b2e073e8fb0544) )
ROM_END

GAME( 1988, asukac, asuka, asuka, asuka,  driver_device, 0, ROT270, "Taito Corporation", "Asuka & Asuka (Chinese)", MACHINE_SUPPORTS_SAVE )
