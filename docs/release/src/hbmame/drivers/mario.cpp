// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mario.cpp"

ROM_START( cenmario )
	ROM_REGION( 0x10000, "maincpu", 0 ) // cpu roms from marioj
	ROM_LOAD( "tma1c-a1.7f",  0x0000, 0x2000, CRC(b64b6330) SHA1(f7084251ac325bbfa3fb804da16a50622e1fd213) )
	ROM_LOAD( "tma1c-a2.7e",  0x2000, 0x2000, CRC(290c4977) SHA1(5af266be0ddc883c6548c90e4a9084024a1e91a0) )
	ROM_LOAD( "tma1c-a1.7d",  0x4000, 0x2000, CRC(f8575f31) SHA1(710d0e72fcfce700ed2a22fb9c7c392cc76b250b) )
	ROM_LOAD( "tma1c-a2.7c",  0xf000, 0x1000, CRC(a3c11e9e) SHA1(d0612b0f8c2ea4e798f551922a04a324f4ed5f3d) )

	ROM_REGION( 0x1800, "audiocpu", ROMREGION_ERASE00 )
	ROM_LOAD( "tma1c-a.6k",   0x1000, 0x0800, CRC(06b9ff85) SHA1(111a29bcb9cda0d935675fa26eca6b099a88427f) )
	ROM_CONTINUE(             0x0800, 0x0800)

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "cenmario.3f",  0x0000, 0x1000, CRC(27bcd321) SHA1(d8f1fc65dc17af2f80d67f3e2f3edabc79eba1b3) )
	ROM_LOAD( "cenmario.3j",  0x1000, 0x1000, CRC(de1c6962) SHA1(2779715d62981baf2ea3a3f501128deb0f5559ab) )

	ROM_REGION( 0x6000, "gfx2", 0 )
	ROM_LOAD( "cenmario.7m",  0x0000, 0x1000, CRC(41c78d70) SHA1(f5ad2b84439a9c2e7aed0a38a507bb714ad6eb3a) )
	ROM_LOAD( "cenmario.7n",  0x1000, 0x1000, CRC(659341a4) SHA1(77207279ebe9c5e33f8bfa4cdbbc7a04d0198c84) )
	ROM_LOAD( "cenmario.7p",  0x2000, 0x1000, CRC(b9e32b12) SHA1(c8c8491c3724dc10f91100299347753bdc16a091) )
	ROM_LOAD( "cenmario.7s",  0x3000, 0x1000, CRC(a5cc7370) SHA1(ac81cd6193207670ffbfdc4b2e9c15805c0eecee) )
	ROM_LOAD( "cenmario.7t",  0x4000, 0x1000, CRC(e416cbea) SHA1(ffce92c8ce25d5086379ec1245ae11c64b7719f4) )
	ROM_LOAD( "cenmario.7u",  0x5000, 0x1000, CRC(19eef39f) SHA1(41ee5184eeee10d7b502f5a06a3b006242756e57) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "tma1-c-4p.4p",     0x0000, 0x0200, CRC(afc9bd41) SHA1(90b739c4c7f24a88b6ac5ca29b06c032906a2801) )
ROM_END

GAME( 2002, cenmario, mario, mario, marioj, driver_device, 0, ROT0, "Marks Hacks", "Censored Mario Bros.", MACHINE_SUPPORTS_SAVE )
