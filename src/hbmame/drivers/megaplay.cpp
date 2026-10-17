// license:GPL_2.0
// copyright-holders:zanac
#include "../mame/sega/megaplay.cpp"

#include "machine/nvram.h"

/* The Addams Family ARCADE (Mega Play)

    Homebrew conversion of The Addams Family ARCADE (itself a hack of the
    Mega Drive game) for the Mega Play board.
    https://github.com/zanac/The-Addams-Family-Mega-Play-Arcade

    The game keeps its high-score table in the 256-byte slot area that the
    BIOS backs up to its battery RAM (Z80 0x6800-0x77ff) at the end of a game,
    so that RAM is saved as NVRAM. Coinage and free play come from the BIOS
    DSW A/B; DSW C holds the game settings. */

namespace {

class mplay_hb_state : public mplay_state
{
public:
	using mplay_state::mplay_state;

	void megaplay_nvram(machine_config &config)
	{
		megaplay(config);
		NVRAM(config, "ic3_ram", nvram_device::DEFAULT_ALL_0); // BIOS battery RAM
	}
};

static INPUT_PORTS_START ( mp_addams )
	PORT_INCLUDE( megaplay )

	PORT_MODIFY("DSW1") // DSW C  (per game settings)
	PORT_DIPNAME( 0x03, 0x03, "1UP Every" ) PORT_DIPLOCATION("SW3:1,2")
	PORT_DIPSETTING( 0x03, "3 Pickups" )
	PORT_DIPSETTING( 0x02, "2 Pickups" )
	PORT_DIPSETTING( 0x01, "1 Pickup" )
	PORT_DIPNAME( 0x0c, 0x0c, DEF_STR( Lives ) ) PORT_DIPLOCATION("SW3:3,4")
	PORT_DIPSETTING( 0x0c, "3" )
	PORT_DIPSETTING( 0x08, "5" )
	PORT_DIPSETTING( 0x04, "7" )
	PORT_DIPSETTING( 0x00, "99" )
INPUT_PORTS_END

ROM_START( mp_addams )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "epr-26013.ic1", 0x000000, 0x200000, CRC(87ecace1) SHA1(d6529b4b692a9515ccf6722f26c011660154ae7a) )
	// Game instruction ROM copied to 0x300000 - 0x310000 (odd / even bytes equal)

	ROM_REGION( 0x8000, "user1", 0 ) // Game instructions
	ROM_LOAD( "epr-15175-20.ic2", 0x000000, 0x08000, CRC(dcfde4e7) SHA1(3f5a1b4de94fad4ccd404b95a53fdb2e1c4773e6) )

	ROM_REGION( 0x20000, "mtbios", 0 ) // BIOS
	MEGAPLAY_BIOS

	MEGAPLAY_PLDS
ROM_END

} // anonymous namespace

GAME( 2026, mp_addams, megaplay, megaplay_nvram, mp_addams, mplay_hb_state, init_megaplay, ROT0, "hack", "The Addams Family ARCADE (Mega Play)", MACHINE_IMPERFECT_GRAPHICS )
