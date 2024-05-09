// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taotaido.cpp"

/**********
 Tao Taido
************/

ROM_START( taotaido01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "tt0-u90.bin", 0x00000, 0x80000, CRC(69d4cca7) SHA1(f1aba74fef8fe4271d19763f428fc0e2674d08b3) )
	ROM_LOAD16_WORD_SWAP( "tt1-u91.s01", 0x80000, 0x80000, CRC(37eb4426) SHA1(af40965065f8e96f384fb3f3b669e78d511a05ca) )

	ROM_REGION( 0x20000, "audiocpu", 0 )
	ROM_LOAD( "3-u113.bin", 0x000000, 0x20000, CRC(a167c4e4) SHA1(d32184e7040935cd440d4d82c66491b710ec87a8) )

	ROM_REGION( 0x100000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "u104.bin",     0x000000, 0x100000, CRC(e89387a9) SHA1(1deeee056af367d1a5aa0722dd3d6c68a82d0489) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "u127.bin",     0x00000, 0x200000, CRC(0cf0cb23) SHA1(a87e7159db2fa0d50446cbf45ec9fbf585b8f396) )

	ROM_REGION( 0x600000, "gfx1", 0 )
	ROM_LOAD( "u86.bin", 0x000000, 0x200000, CRC(908e251e) SHA1(5a135787f3263bfb195f8fd1e814c580d840531f) )
	ROM_LOAD( "u87.bin", 0x200000, 0x200000, CRC(c4290ba6) SHA1(4132ffad4668f1dd3f708f009e18435e7dd60120) )
	ROM_LOAD( "u88.bin", 0x400000, 0x200000, CRC(407d9aeb) SHA1(d532c7b80f6c192dba86542fb6eb3ef24fbbbdb9) )

	ROM_REGION( 0x200000, "gfx2", 0 )
	ROM_LOAD( "u15.bin", 0x000000, 0x200000, CRC(e95823e9) SHA1(362583944ad4fdde4f9e29928cf34376c7ad931f) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
//Tao Taido
GAME( 1993, taotaido01,     taotaido, taotaido, taotaido6, taotaido_state, empty_init, ROT0, "DDJ", "Tao Taido (Easy Move)", MACHINE_SUPPORTS_SAVE )

