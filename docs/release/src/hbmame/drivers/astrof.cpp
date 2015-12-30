// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/astrof.cpp"

ROM_START( astrf_bl )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "afii.6",       0xd000, 0x0800, CRC(d6cd13a4) SHA1(359b00b02f4256f1138c8526214c6a34d2e5b47a) )
	ROM_LOAD( "afii.5",       0xd800, 0x0800, CRC(6fd3c4df) SHA1(73aad03e2588ac9f249d5751eb4a7c7cd12fd3b9) )
	ROM_LOAD( "afii.4",       0xe000, 0x0800, CRC(9612dae3) SHA1(8ee1797c212e06c381972b7b555f240ff317d75d) )
	ROM_LOAD( "afii.3",       0xe800, 0x0800, CRC(5a0fef42) SHA1(92a575abdf17bbb5ed6bc67479049523a985aa75) )
	ROM_LOAD( "afii.2",       0xf000, 0x0800, CRC(69f8a4fc) SHA1(9f9a935f19187640018009ade92f8993912ef6c2) )
	ROM_LOAD( "afii.1",       0xf800, 0x0800, CRC(322c09d2) SHA1(89723e3d998ff9cb9b174bca4b072b412b290c04) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "astrf_bl.clr", 0x0000, 0x0020, CRC(6be69d4b) SHA1(4197cf9110db08e45b5d18aedb0fde96779491c1) )
ROM_END


GAME( 1979, astrf_bl, 0, astrof, astrof, driver_device, 0, ROT90, "Hack", "Astro Fighter (Black Background)", MACHINE_SUPPORTS_SAVE )
