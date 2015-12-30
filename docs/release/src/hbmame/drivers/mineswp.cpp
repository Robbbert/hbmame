// license:BSD-3-Clause
// copyright-holders:Robbbert
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
- Made everything static (July 2008)
- Hooked port 20 to the colour generator (July 2008)
- Changed interrupt code as the core's VIDEO_INT has become flaky (May 2010)
- Hooked up a sound click to port 20 (May 2010)

Colours:	White (normal play / attract mode)
		Yellow (new record time)
		Green (new record level - must be at least level 3)
		Purple (stepped on a bomb)
		Red (game over)

***************************************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "machine/nvram.h"
#include "sound/beep.h"

class mineswp_state : public driver_device
{
public:
	mineswp_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_p_videoram(*this, "videoram")
		, m_beep(*this, "beeper")
	{ }

	DECLARE_WRITE8_MEMBER(color_w);
	UINT32 screen_update_mineswp(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
private:
	UINT8 m_color;
	virtual void machine_start();
	virtual void machine_reset();
	required_shared_ptr<UINT8> m_p_videoram;
	required_device<beep_device> m_beep;
};


/*******************************************************************

    Palette Setting.

*******************************************************************/


WRITE8_MEMBER( mineswp_state::color_w )
{
	m_color = data & 7;
	m_beep->set_state(BIT(data, 3));
}


/*******************************************************************

    Video updating

*******************************************************************/
UINT32 mineswp_state::screen_update_mineswp(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	UINT8 x,y,z,data;

	pen_t bg = rgb_t(pal1bit(m_color >> 3), pal1bit(m_color >> 4), pal1bit(m_color >> 5));
	pen_t fg = rgb_t(pal1bit(m_color >> 0), pal1bit(m_color >> 1), pal1bit(m_color >> 2));

	for (y = 0; y < 96; y++)
	{
		x = 0;
		for (z = 0; z < 16; z++)
		{
			data = m_p_videoram[(y<<4) | z];

			bitmap.pix32(y, x++) = BIT(data, 7) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 6) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 5) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 4) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 3) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 2) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 1) ? fg : bg;
			bitmap.pix32(y, x++) = BIT(data, 0) ? fg : bg;
		}
	}

	return 0;
}


/*******************************************************************

    Address maps

*******************************************************************/
static ADDRESS_MAP_START( mineswp_map, AS_PROGRAM, 8, mineswp_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM AM_REGION("roms", 0)
	AM_RANGE(0x8000, 0x85ff) AM_RAM AM_SHARE("videoram")
	AM_RANGE(0x8600, 0x86df) AM_RAM
	AM_RANGE(0x86e0, 0x86ff) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0x8700, 0x87ff) AM_RAM
	AM_RANGE(0xfffc, 0xffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( mineswp_io, AS_IO, 8, mineswp_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_READ_PORT("IN0")
	AM_RANGE(0x20, 0x20) AM_WRITE(color_w)
ADDRESS_MAP_END


/*******************************************************************

    Inputs

*******************************************************************/
static INPUT_PORTS_START( mineswp )
	PORT_START  ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN1 )
INPUT_PORTS_END


/*******************************************************************

    Machine driver

*******************************************************************/
void mineswp_state::machine_start()
{
	m_beep->set_frequency(950);    /* guess */
	m_beep->set_state(0);
	save_item(NAME(m_color));
}

void mineswp_state::machine_reset()
{
	m_color = 0;
}

static MACHINE_CONFIG_START( mineswp, mineswp_state )

	/* basic machine hardware */
	MCFG_CPU_ADD  ("maincpu", Z80, XTAL_4MHz)
	MCFG_CPU_PROGRAM_MAP(mineswp_map)
	MCFG_CPU_IO_MAP(mineswp_io)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", mineswp_state,  irq0_line_hold)
	MCFG_NVRAM_ADD_1FILL("nvram")

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500))
	MCFG_SCREEN_SIZE(128, 96)
	MCFG_SCREEN_VISIBLE_AREA(0, 127, 0, 95)
	MCFG_SCREEN_UPDATE_DRIVER(mineswp_state, screen_update_mineswp)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("beeper", BEEP, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.40)
MACHINE_CONFIG_END


/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( mineswp )
	ROM_REGION( 0x4000, "roms", 0 ) /* 64k for code */
	ROM_LOAD( "14479b.mpr", 0x0000, 0x4000, CRC(1cece483) SHA1(c425a09b640721c9a5b499cdb79848dc18ee66b9) )
ROM_END

GAME( 1998, mineswp, 0, mineswp, mineswp, driver_device, 0, ROT0, "J-Rom", "Mine Sweeper [h]", MACHINE_SUPPORTS_SAVE )

