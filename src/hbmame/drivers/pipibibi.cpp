// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/toaplan/pipibibi.cpp"

/*************
 Pipi & Bibis
**************/
ROM_START( pipibibs01 )
	ROM_REGION( 0x040000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "s01.1", 0x000000, 0x020000, CRC(38da1e2f) SHA1(890c5cc8e45ad709f48167c47dac0543da448175) )
	ROM_LOAD16_BYTE( "s01.2", 0x000001, 0x020000, CRC(7f6f32b3) SHA1(39b4ff18c266b6c9c86760379740618a0a479242) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "tp025-5.bin", 0x0000, 0x8000, CRC(bf8ffde5) SHA1(79c09cc9a0ea979f5af5a7e5ad671ea486f5f43e) )

	ROM_REGION( 0x200000, "gp9001", 0 )
	ROM_LOAD( "tp025-4.bin", 0x000000, 0x100000, CRC(ab97f744) SHA1(c1620e614345dbd5c6567e4cb6f55c61b900d0ee) )
	ROM_LOAD( "tp025-3.bin", 0x100000, 0x100000, CRC(7b16101e) SHA1(ae0119bbfa0937d18c4fbb0a3ef7cdc3b9fa6b56) )
ROM_END

ROM_START( pipibibs02 )
	ROM_REGION( 0x040000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "s02.1", 0x000000, 0x020000, CRC(28cec0bf) SHA1(f08a33249df902c76bbe717dbb8c25e9be2497eb) )
	ROM_LOAD16_BYTE( "s02.2", 0x000001, 0x020000, CRC(f3df3f3f) SHA1(96245fab49f23af4957e163509afdae7f51d11e7) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "tp025-5.bin", 0x0000, 0x8000, CRC(bf8ffde5) SHA1(79c09cc9a0ea979f5af5a7e5ad671ea486f5f43e) )

	ROM_REGION( 0x200000, "gp9001", 0 )
	ROM_LOAD( "tp025-4.bin", 0x000000, 0x100000, CRC(ab97f744) SHA1(c1620e614345dbd5c6567e4cb6f55c61b900d0ee) )
	ROM_LOAD( "tp025-3.bin", 0x100000, 0x100000, CRC(7b16101e) SHA1(ae0119bbfa0937d18c4fbb0a3ef7cdc3b9fa6b56) )
ROM_END

/*    YEAR  NAME          PARENT           MACHINE   INPUT     CLASS           INIT           MONITOR COMPANY       FULLNAME FLAGS */
// Pipi & Bibis
GAME( 1991, pipibibs01,   pipibibs,        pipibibs, pipibibs, pipibibi_state, empty_init,    ROT0,   "DDJ",       "Pipi & Bibis (Stage Select V1)", MACHINE_SUPPORTS_SAVE )
GAME( 1991, pipibibs02,   pipibibs,        pipibibs, pipibibs, pipibibi_state, empty_init,    ROT0,   "DDJ",       "Pipi & Bibis (Stage Select V2)", MACHINE_SUPPORTS_SAVE )

