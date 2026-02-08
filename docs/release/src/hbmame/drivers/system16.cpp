// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/system16.cpp"

/*
    This demo doesn't have any sound programmed in.
    Use left & right arrows to move the sprite around.
*/

namespace {

class hb_sys16 : public segas1x_bootleg_state
{
public:
	hb_sys16(const machine_config &mconfig, device_type type, const char *tag)
		: segas1x_bootleg_state(mconfig, type, tag)
		{ }

	void init_sys16dem();
};

// The original has no sprite end markers, leading to freeze, crash, and bugs.
// We need to add a marker after each group of 8x 16-bit sprite-words.
void hb_sys16::init_sys16dem()
{
	u16 *rom = (u16 *)memregion("sprites")->base();
	u16 buf[0x500],i,j,k = 0;
	for (i = 0; i < 0x500; i++)
		buf[i] = rom[i];
	// Add sprite end markers for each row of pixels
	for (i = 0; i < 0xA0; i++)
	{
		for (j = 0; j < 8; j++)
			rom[k++] = buf[i*8+j];
		rom[k++] = 0x0f;
	}
	// Now, tell the sprite engine that each row now has 9 bytes instead of 8
	rom = (u16 *)memregion("maincpu")->base();
	for (i = 0x2d08/2; i < 0x2d50/2; i+=0x10/2)
		rom[i] |= 9;
	// Lastly, tell the sprite engine the new (16-bit) location of each sprite letter
	rom[0x2d1a/2] |= 0x20;
	rom[0x2d2a/2] |= 0x40;
	rom[0x2d3a/2] |= 0x60;
	rom[0x2d4a/2] |= 0x80;
}

ROM_START( sys16dem )
	ROM_REGION( 0x40000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rom2dm.bin", 0x00000, 0x10000, CRC(671c5eac) SHA1(5fa7f8ba9d31a6c31a0b5a32c2cd869200032461) )
	ROM_LOAD16_BYTE( "rom1dm.bin", 0x00001, 0x10000, CRC(6461ebd9) SHA1(16dc373408342560eb48e3c037a8f42636796560) )

	ROM_REGION( 0x30000, "gfx1", 0 )
	ROM_LOAD( "scr01dm.b9",  0x00000, 0x10000, CRC(2fdb9b09) SHA1(bec9ca842921695b06915825acfba076443a7e8b) )
	ROM_LOAD( "scr02dm.b10", 0x10000, 0x10000, CRC(0e880750) SHA1(d6b7c84c00dfb3c1fcf966587f700f3ea3313b94) )
	ROM_LOAD( "scr03dm.b11", 0x20000, 0x10000, CRC(3e76f2ba) SHA1(9177850b5d0b914a672af414e7e134e15d554fb8) )

	ROM_REGION16_BE( 0x20000, "sprites", 0 )
	ROM_LOAD16_BYTE( "dm-obj0-o.rom", 0x00001, 0x10000, CRC(82ec3f29) SHA1(0c0b28e98348f8981314f8d940ccd17d790bf335) )
	ROM_LOAD16_BYTE( "dm-obj0-e.rom", 0x00000, 0x10000, CRC(6eca3b5c) SHA1(f58ac53d24b167db4787d6fe91e4799f3c3256d6) )

	ROM_REGION( 0x40000, "soundcpu", 0 )
	ROM_LOAD( "epr12168.a7", 0x0000, 0x8000, CRC(bd9ba01b) SHA1(fafa7dc36cc057a50ae4cdf7a35f3594292336f4) )
ROM_END

} // anonymous namespace

GAME( 1988, sys16dem, 0, tetrisbl, tetris, hb_sys16, init_sys16dem, ROT0, "Charles Doty", "Demo - Sega System 16", 0 )

