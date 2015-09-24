// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_l.c"

ROM_START( evilstonfr )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD( "c67-03fr.ic2", 0x00000, 0x20000, CRC(3fa7e884) SHA1(94716256c199b5f347bbe122d04adbab1a54b1ce) )
	ROM_LOAD( "c67-04fr.ic6", 0x20000, 0x20000, CRC(8c43dc76) SHA1(ee395d55ee8e66c86e12f604774799ca7cfe1858) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "c67-05.ic22",  0x00000, 0x20000, CRC(94d3a642) SHA1(af20aa5bb60a45c05eb1deba23ba30e6640ca235) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "c67-01.ic1",  0x00000, 0x80000, CRC(2f351bf4) SHA1(0fb37abf3413cd11baece1c9bbca5a51b0f28938) )
	ROM_LOAD( "c67-02.ic5",  0x80000, 0x80000, CRC(eb4f895c) SHA1(2c902572fe5a5d4442e4dd29e8a85cb40c384140) )
ROM_END

ROM_START( tldemo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tldemo.ic10",   0x00000, 0x56a4, CRC(fda3d418) SHA1(e0171cc1c035d1f800702c3766825b63f6acd291) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "tldemo.ic9", 0x00000, 0x10000, CRC(792b8c11) SHA1(0592481b586bfca0f8f8eb096bee46690943596b) )
	ROM_LOAD16_BYTE( "tldemo.ic8", 0x00001, 0x10000, CRC(59cfc5ab) SHA1(133084eeacdf978ed13db565f64d155c08ecaade) )
ROM_END

ROM_START( tldemo2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tldemo2.ic10",   0x00000, 0x7142, CRC(1292a22c) SHA1(e351ac5381f7d73de0087586c7fb28d9eccd72cb) )
	// This one below is the old version, works fine in MAME but is buggy in real hardware.
	//ROM_LOAD( "tldemo2.ic10",   0x00000, 0x6514, CRC(efd21b6b) SHA1(1c3bfb64dacfb5a1da55201cd4d4202694e06d37) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "tldemo2.ic9", 0x00000, 0x10000, CRC(fcc75ddb) SHA1(e702480b84379e3106cb6d575d237a8f9701f49e) )
	ROM_LOAD16_BYTE( "tldemo2.ic8", 0x00001, 0x10000, CRC(33e70f71) SHA1(5a0caa0912dd7d76dd7e788e21b4cc7f320156ad) )
ROM_END

GAME( 2007, evilstonfr,  evilston, evilston,  evilston, driver_device, 0, ROT270, "Arkatrad", "Evil Stone (French)", MACHINE_IMPERFECT_SOUND )
GAME( 2014, tldemo,      0,        plotting,  plotting, driver_device, 0, ROT0,   "cmonkey", "Demo - System Timed Cheat Tool", 0 )
GAME( 2015, tldemo2,     0,        plotting,  plotting, driver_device, 0, ROT0,   "cmonkey", "Demo - 256 colours", 0 )
