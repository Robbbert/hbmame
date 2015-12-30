// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mario.cpp"

ROM_START( cenmario )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tma1-c.7f_e-1",     0x0000, 0x2000, CRC(c0c6e014) SHA1(36a04f9ca1c2a583477cb8a6f2ef94e044e08296) )
//	ROM_LOAD( "tma1-c.7e_e-b",     0x2000, 0x2000, CRC(b09ab857) SHA1(35b91cd1c4c3dd2d543a1ea8ff7b951715727792) )
	ROM_LOAD( "mario.7e",     0x2000, 0x2000, CRC(116b3856) SHA1(e372f846d0e5a2b9b47ebd0330293fcc8a12363f) )
	ROM_LOAD( "tma1-c.7d_e-1",     0x4000, 0x2000, CRC(dcceb6c1) SHA1(b19804e69ce2c98cf276c6055c3a250316b96b45) )
//	ROM_LOAD( "tma1-c.7c_e-3",     0xf000, 0x1000, CRC(0d31bd1c) SHA1(a2e238470ba2ea3c81225fec687f61f047c68c59) )
	ROM_LOAD( "mario.7c",     0xf000, 0x1000, CRC(4a63d96b) SHA1(b09060b2c84ab77cc540a27b8f932cb60ec8d442) )

	ROM_REGION( 0x1800, "audiocpu", 0 )	/* sound */
	/* internal rom */
	ROM_FILL(                 0x0000, 0x0800, 0x00)
	/* first half banked */
	ROM_LOAD( "tma1c-a.6k",   0x1000, 0x0800, CRC(06b9ff85) SHA1(111a29bcb9cda0d935675fa26eca6b099a88427f) )
	/* second half always read */
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
	ROM_LOAD( "mario.4p",     0x0000, 0x0200, CRC(afc9bd41) SHA1(90b739c4c7f24a88b6ac5ca29b06c032906a2801) )
ROM_END

GAME( 2002, cenmario, 0, mario, marioj, driver_device, 0, ROT0, "Hack", "Censored Mario Bros.", MACHINE_SUPPORTS_SAVE )
