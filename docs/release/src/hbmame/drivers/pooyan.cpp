// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/pooyan.cpp"


/*************************************
 *
 *  ROM definitions
 *
 *************************************/

// Hold down P2 start while booting, to get into more diagnostic tests (In HBMAME, hold down 2 and hit F3)

ROM_START( pooyandiag )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "diag.4a",      0x0000, 0x2000, CRC(a583d5c9) SHA1(9736e62c4fddafe46374abda069f12b492172c6c) )
	ROM_LOAD( "2.5a",         0x2000, 0x2000, CRC(a1463d98) SHA1(b23cc7e61276c61a78e80fe08c7f0c8adadf2ffe) )
	ROM_LOAD( "3.6a",         0x4000, 0x2000, CRC(fe1a9e08) SHA1(5206893760f188ac71a5e6bd42561cf25fcc3d49) )
	ROM_LOAD( "4.7a",         0x6000, 0x2000, CRC(9e0f9bcc) SHA1(4d9707423ad531ac535db432e329b3d52cbb4559) )

	ROM_REGION( 0x10000, "timeplt_audio:tpsound", 0 )
	ROM_LOAD( "xx.7a",        0x0000, 0x1000, CRC(fbe2b368) SHA1(5689a84ef110bdc0039ad1a6c5778e0b8eccfce0) )
	ROM_LOAD( "xx.8a",        0x1000, 0x1000, CRC(e1795b3d) SHA1(9ab4e5362f9f7d9b46b750e14b1d9d71c57be40f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "8.10g",        0x0000, 0x1000, CRC(931b29eb) SHA1(0325c1c1fdb44e0044b82b7c79b5eeabf5c11ce7) )
	ROM_LOAD( "7.9g",         0x1000, 0x1000, CRC(bbe6d6e4) SHA1(de5447d59a99c4c08c4f40c0b7dd3c3c609c11d4) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "6.9a",         0x0000, 0x1000, CRC(b2d8c121) SHA1(189ad488869f34d7a38b82ef70eb805acfe04312) )
	ROM_LOAD( "5.8a",         0x1000, 0x1000, CRC(1097c2b6) SHA1(c815f0d27593efd23923511bdd13835456ef7f76) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "pooyan.pr1",   0x0000, 0x0020, CRC(a06a6d0e) SHA1(ae131320b66d76d4bc9108da6708f6f874b2e123) )
	ROM_LOAD( "pooyan.pr3",   0x0020, 0x0100, CRC(8cd4cd60) SHA1(e0188ecd5b53a8e6e28c1de80def676740772334) )
	ROM_LOAD( "pooyan.pr2",   0x0120, 0x0100, CRC(82748c0b) SHA1(9ce8eb92e482eba5a9077e9db99841d65b011346) )
ROM_END



/*************************************
 *
 *  Game drivers
 *
 *************************************/

//    YEAR, NAME,        PARENT, MACHINE,INPUT,  CLASS,      INIT,MONITOR,  COMPANY,        FULLNAME,     FLAGS
GAME( 2017, pooyandiag,  pooyan, pooyan, pooyan, pooyan_state, empty_init, ROT90,  "Shoestring", "Pooyan Tester", MACHINE_SUPPORTS_SAVE )
