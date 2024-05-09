// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/tecmo16.cpp"



ROM_START( fstarfrc01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "fstarfc01.rom", 0x00000, 0x40000, CRC(28a0b648) SHA1(b9b1ad7a7823b0c4e96bf1a3c4ede0192a78fe38) )
	ROM_LOAD16_BYTE( "fstarfc02.rom", 0x00001, 0x40000, CRC(cd49179f) SHA1(b1093a999c5c918cf399589fba6d175cd6899e6a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "fstarf07.rom", 0x00000, 0x10000, CRC(e0ad5de1) SHA1(677237341e837061b6cc02200c0752964caed907) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "fstarfc03.rom", 0x00000, 0x20000, CRC(4bedd245) SHA1(55a5dfac11dbd0c615025d25d24a5917a54955c7) )

	ROM_REGION( 0x100000, "gfx2", 0 )
	ROM_LOAD16_BYTE( "fstarf05.rom", 0x00000, 0x80000, CRC(77a281e7) SHA1(a87a90c2c856d45785cb56185b1a7dff3404b5cb) )
	ROM_LOAD16_BYTE( "fstarf04.rom", 0x00001, 0x80000, CRC(398a920d) SHA1(eecc167803f48517348d68ce70f15e87eac204bb) )

	ROM_REGION( 0x100000, "gfx3", 0 )
	ROM_LOAD16_BYTE( "fstarf09.rom", 0x00000, 0x80000, CRC(d51341d2) SHA1(e46c319158046d407d4387cb2d8f0b6cfd7be576) )
	ROM_LOAD16_BYTE( "fstarf06.rom", 0x00001, 0x80000, CRC(07e40e87) SHA1(22867e52a8267ae8ae0ff0dba6bb846cb3e1b63d) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "fstarf08.rom", 0x00000, 0x20000, CRC(f0ad5693) SHA1(a0202801bb9f9c86175ca7989fbc9efa47183188) )
ROM_END


GAME( 1992, fstarfrc01, fstarfrc, base, fstarfrc, tecmo16_state, empty_init, ROT90, "Tecmo", "Final Star Force (Chinese)", MACHINE_SUPPORTS_SAVE )

