// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/dotrikun.cpp"

// http://www.chrismcovell.com/dottorikun.html

ROM_START( dotrimjr )
	ROM_REGION( 0x4000, "maincpu", 0 )
	ROM_LOAD( "dotrimjr.bin", 0x0000, 0x4000, CRC(4ba6d2f5) SHA1(db805e9121ecbd41fac4593b58d7f071e7dbc720) )
ROM_END

GAMEL( 2016, dotrimjr, 0, dotrikun, dotrikun, driver_device, 0, ROT0, "Chris Covell", "Dottori-Man Jr", MACHINE_SUPPORTS_SAVE | MACHINE_NO_SOUND_HW, layout_dotrikun )


/***************************************************************************

Minesweeper driver, originally for Dotrikun hardware

Driver by Robbbert - April 2007

CPU   : Z-80 (4MHz)
SOUND : (none)

****************************************************************************

Notes from the Readme files

MINE SWEEPER for SEGA's Dottorikun PCB

Distributed by Arcade roms from Japan (j-rom)

This game is the Dottorikun port of the Windows' Mine Sweeper.
It has "Time Limit", "more Stage", etc...

Usage:

You must burn DT_MINEJ.BIN to 27128 and replace it to the ROM of
Dottorikun's board. I assume you can find this board easily...

Rules:

* The timer limit is 999
* If you hit a mine, many counts added to the timer
* There are no mines at the start point and the corner

Option:

This game supports BACKUP and SOUND. If you want to play
the sound correctly, you must modify the board. If not,
some noise appears on the screen.

* THIS MODIFY IS QUITE DIFFICULT, USE AT YOUR OWN RISK! *
*    AND DON'T ASK ABOUT THIS MODIFYING INFORMATION!    *

Backup Support:

* You have to install the RESET-IC (PST518, etc.)
* Check the power on/off
* Install the Lithium battery or super capacity condensor
* Connect the inverted RESET signal to RAM's 18pin (use 74HC04, etc.)
  (You must backup both RAM and HC04)

Sound Support:

You must get the sound board of Wing War (Guns'n Wings).
You can hear the sonic sound if you use this sound board!

* Cut IC17's 10pin from the board
* Use shielded cable, Connect CN2's 6pin on the sound board to the above pin
* And the shield connects GND and CN2's 5pin
* Connect IC17 10pin's board side to the GND
* This sound board has 2 set of STEREO, you have to mix them

Secrets:

* Push start when the power on : RAM CLEAR
* right+up lever and push 1 and 2 button : SOUND TEST

* left+up and push 1, 2, COIN, START when the checking Backup RAM is
  finished, you can see the Ending scene.

* Push 1 and 2 when the Ending scene is changed, you can see the
  secret picture (^^)

Robbbert's notes:

In the above, when they say 1 and 2, they mean Ctrl and Alt
By using the debugger, you can see the sound test by setting the pc to 105F,
and the ending scene by setting the pc to 0F30.

- Backup nvram added (April 2007)
- Hooked port 20 to the colour generator (July 2008)
- Hooked up a sound click to port 20 (May 2010)
- Merged back into dotrikun driver (August 2016)

Colours:
	White (normal play / attract mode)
	Yellow (new record time)
	Green (new record level - must be at least level 3)
	Purple (stepped on a bomb)
	Red (game over)

***************************************************************************/
#include "machine/nvram.h"
#include "sound/beep.h"

class mineswp_state : public dotrikun_state
{
public:
	mineswp_state(const machine_config &mconfig, device_type type, const char *tag)
		: dotrikun_state(mconfig, type, tag)
		, m_beep(*this, "beeper")
	{ }

	DECLARE_WRITE8_MEMBER(mineswp_color_w);
private:
	virtual void machine_start() override;
	required_device<beep_device> m_beep;
};


/*******************************************************************

    Palette Setting.

*******************************************************************/


WRITE8_MEMBER( mineswp_state::mineswp_color_w )
{
	// d0-d2: fg palette
	// d3: sound bit
	// d4-d7: N/C
	m_screen->update_now();
	m_color = data & 7;
	m_beep->set_state(BIT(data, 3));
}



/*******************************************************************

    Address maps

*******************************************************************/
static ADDRESS_MAP_START( mineswp_map, AS_PROGRAM, 8, mineswp_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x8000, 0x85ff) AM_RAM_WRITE(vram_w) AM_SHARE("vram")
	AM_RANGE(0x8600, 0x86df) AM_RAM
	AM_RANGE(0x86e0, 0x86ff) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0x8700, 0x87ff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( mineswp_io, AS_IO, 8, mineswp_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_MIRROR(0xff) AM_READ_PORT("INPUTS") AM_WRITE(mineswp_color_w)
ADDRESS_MAP_END




/*******************************************************************

    Machine driver

*******************************************************************/
void mineswp_state::machine_start()
{
	m_beep->set_state(0);
}

static MACHINE_CONFIG_DERIVED_CLASS( mineswp, dotrikun, mineswp_state )
	/* basic machine hardware */
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mineswp_map)
	MCFG_CPU_IO_MAP(mineswp_io)

	MCFG_NVRAM_ADD_1FILL("nvram")

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("beeper", BEEP, 950) // guess
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.40)
MACHINE_CONFIG_END


/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( mineswp )
	ROM_REGION( 0x4000, "maincpu", 0 )
	ROM_LOAD( "mineswp.bin", 0x0000, 0x4000, CRC(1cece483) SHA1(c425a09b640721c9a5b499cdb79848dc18ee66b9) )
	// fix programming error
	ROM_FILL(0x0005, 1, 0x31)
	ROM_FILL(0x0006, 1, 0x00)
	ROM_FILL(0x0007, 1, 0x88)
	ROM_FILL(0x0008, 1, 0xCD)
	ROM_FILL(0x0009, 1, 0xA1)
	ROM_FILL(0x000A, 1, 0x03)
ROM_END


GAMEL( 1998, mineswp, 0, mineswp, dotrikun, driver_device, 0, ROT0, "J-Rom", "Mine Sweeper [h]", MACHINE_SUPPORTS_SAVE | MACHINE_NO_SOUND_HW, layout_dotrikun )
