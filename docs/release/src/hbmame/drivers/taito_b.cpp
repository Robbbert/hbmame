// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_b.cpp"

ROM_START( bubbleb ) // same as pbobble except for ic2
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "pb-1c18.bin", 0x00000, 0x40000, CRC(5de14f49) SHA1(91d537748f26e19a5c32de4b8dad341750de39ef) )
	ROM_LOAD16_BYTE( "pb-ic2.usa",  0x00001, 0x40000, CRC(b3b7566b) SHA1(455342b6988741f8c83ed97ef1a2d3b885cbc9d1) )

	ROM_REGION( 0x20000, "audiocpu", 0 )
	ROM_LOAD( "pb-ic27.bin", 0x00000, 0x20000, CRC(26efa4c4) SHA1(795af8f6d23c2cbe2c811ec9ab1f14a4eee3f99e) )

	ROM_REGION( 0x100000, "tc0180vcu", 0 )
	ROM_LOAD( "pb-ic14.bin", 0x00000, 0x80000, CRC(55f90ea4) SHA1(793c79e5b72171124368ad09dd31235252c541f5) )
	ROM_LOAD( "pb-ic9.bin",  0x80000, 0x80000, CRC(3253aac9) SHA1(916d85aa96e2914630833292a0655b0389b4a39b) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "pb-ic15.bin", 0x000000, 0x100000, CRC(0840cbc4) SHA1(1adbd7aef44fa80832f63dfb8efdf69fd7256a57) )
ROM_END

GAME( 1994, bubbleb, pbobble, pbobble, pbobble, taitob_state, init_taito_b, ROT0, "Taito Corporation", "Bubble Buster", MACHINE_SUPPORTS_SAVE )


ROM_START( crimecb )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "cb99-07.40", 0x00000, 0x20000, CRC(ad3129a0) SHA1(d01f4a2bcbe89d7ecee687fd8032cbe5b52f7b58) )
	ROM_LOAD16_BYTE( "cb99-05.29", 0x00001, 0x20000, CRC(6046c7e7) SHA1(9de2a526e476852cd8a4d3cd39d5360ccc42cd29) )
	ROM_LOAD16_BYTE( "b99-06.39", 0x40000, 0x20000, CRC(1f26aa92) SHA1(10ab253812db83204c136d01d865063a2210cb92) )
	ROM_LOAD16_BYTE( "b99-14.28", 0x40001, 0x20000, CRC(71c8b4d7) SHA1(55068c9cac75200f564b10f98f322e30aaa6849e) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "b99-08.45", 0x00000, 0x10000, CRC(26135451) SHA1(651c77285eb12a13a2fe4069031c6f01150ecba4) )

	ROM_REGION( 0x100000, "tc0180vcu", 0 )
	ROM_LOAD( "b99-02.18", 0x000000, 0x080000, CRC(2a5d4a26) SHA1(94bdfca9365970a80a639027b195b71cebc5ab9c) )
	ROM_LOAD( "b99-01.19", 0x080000, 0x080000, CRC(a19e373a) SHA1(2208c9142473dc2218fd8b97fd6d0c861aeba011) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "b99-03.37", 0x000000, 0x080000, CRC(dda10df7) SHA1(ffbe1423794035e6f049fddb096b7282610b7cee) )
ROM_END

GAME( 1989, crimecb, crimec, crimec, crimec, taitob_state, init_taito_b, ROT0,   "Taito Corporation Japan",   "Crime City (unknown hack)", MACHINE_SUPPORTS_SAVE )

