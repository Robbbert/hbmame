/*******************************************************
 Puckman and Pacman

 Puckman is made by Namco and has these ghost names:
 * Normal:
 **  Oikake - Akabei
 **  Machibuse - Pinky
 **  Kimagure - Aosuke
 **  Otoboke - Guzuta
 * Alternate:
 **  Urchin - Macky
 **  Romp - Micky
 **  Stylist - Mucky
 **  Crybaby - Mocky

 Clones include Hangly Man, and Newpuck-X

 Pac-Man is made by Midway and has these ghost names:
 * Normal:
 **  Shadow - Blinky
 **  Speedy - Pinky
 **  Bashful - Inky
 **  Pokey - Clyde
 * Alternate:
 **  AAAAAAAA - BBBBBBB
 **  CCCCCCCC - DDDDDDD
 **  EEEEEEEE - FFFFFFF
 **  GGGGGGGG - HHHHHHH

 Clones include most hacks

 See the MAME source code for hardware information.
********************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "pacman.h"
#include "sound/namco.h"
#include "machine/nvram.h"



/*************************************
 *
 *  Constants
 *
 *************************************/

#define MASTER_CLOCK		(XTAL_18_432MHz)

#define PIXEL_CLOCK		(MASTER_CLOCK/3)

/* H counts from 128->511, HBLANK starts at 144 and ends at 240 */
#define HTOTAL			(384)
#define HBEND			(0)		/*(96+16)*/
#define HBSTART			(288)	/*(16)*/

#define VTOTAL			(264)
#define VBEND			(0)		/*(16)*/
#define VBSTART			(224)	/*(224+16)*/



/*************************************
 *
 *  Machine init
 *
 *************************************/


/*************************************
 *
 *  Interrupts
 *
 *************************************/

WRITE8_MEMBER(pacman_state::irq_mask_w)
{
	m_irq_mask = data & 1;
}

WRITE8_MEMBER(pacman_state::pacman_interrupt_vector_w)
{
	m_maincpu->set_input_line_vector(0, data);
	m_maincpu->set_input_line(0, CLEAR_LINE);
}

/* bit0 flags if memory poke already done */

INTERRUPT_GEN_MEMBER( pacman_state::vblank_irq )
{
	UINT8 cheat_exist = ioport("FAKE")->read_safe(120);

	/* always signal a normal VBLANK */
	if(m_irq_mask)
		device.execute().set_input_line(0, HOLD_LINE);

	if (cheat_exist != 120)
	{
		UINT8 cheating = ((cheat_exist & 7) > 2);		/* are we cheating? */

		if (cheating != (m_speedcheat & 1))	/* if status just changed, alter program bytes */
		{
			UINT8 *RAM = machine().root_device().memregion("maincpu")->base();
			m_speedcheat = (m_speedcheat & 2) | cheating;

			if (cheating)						/* activate the cheat */
			{
				if (RAM[0x180b] == 0xbe) RAM[0x180b] = 1;	/* pacman */
				else
				if (RAM[0x182d] == 0xbe) RAM[0x182d] = 1;	/* pacplus */
			}
			else							/* remove the cheat */
			{
				if (RAM[0x180b] == 1) RAM[0x180b] = 0xbe;	/* pacman */
				else
				if (RAM[0x182d] == 1) RAM[0x182d] = 0xbe;	/* pacplus */
			}
		}
	}
}


/*************************************
 *
 *  LEDs/coin counters
 *
 *************************************/

WRITE8_MEMBER(pacman_state::pacman_leds_w)
{
	set_led_status(machine(), offset,data & 1);
}


WRITE8_MEMBER(pacman_state::pacman_coin_counter_w)
{
	coin_counter_w(machine(), offset,data & 1);
}


WRITE8_MEMBER(pacman_state::pacman_coin_lockout_global_w)
{
	coin_lockout_global_w(machine(), ~data & 0x01);
}


READ8_MEMBER( pacman_state::pacman_read_nop )
{
	return 0xbf;
}

/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

static ADDRESS_MAP_START( pacman_map, AS_PROGRAM, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0x7fff)		/* A15 not connected at the CPU */
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x2000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x2000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x2000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x2000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x2000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x5080) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")
ADDRESS_MAP_END

static ADDRESS_MAP_START( woodpek_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0xa000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0xa000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0xa000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0x8000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_MIRROR(0x8000) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_MIRROR(0x8000) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_MIRROR(0x8000) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_MIRROR(0x8000) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_MIRROR(0x8000) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_MIRROR(0x8000) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_MIRROR(0x8000) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_MIRROR(0x8000) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x5080) AM_MIRROR(0x8000) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0x8000) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0x8000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_MIRROR(0x8000) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_MIRROR(0x8000) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0x8000) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0xbfff) AM_ROM
	AM_RANGE(0xff00, 0xffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( patched_opcodes_map, AS_DECRYPTED_OPCODES, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_MIRROR(0x8000) AM_ROM AM_SHARE("patched_opcodes")
ADDRESS_MAP_END
/*************************************
 *
 *  Main CPU port handlers
 *
 *************************************/

static ADDRESS_MAP_START( writeport, AS_IO, 8, pacman_state )
	AM_RANGE(0x0000, 0xffff) AM_WRITE(pacman_interrupt_vector_w)
ADDRESS_MAP_END

/*************************************
 *
 *  Port definitions
 *
 *************************************/

/* No alternate ghost names, no fake input port - for mspacman and mspacatk */
INPUT_PORTS_START( mspacpls )
	PORT_START ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	/* Press this while playing pacman to instantly finish the level - HBMAME */
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME("Finish Level (Cheat)") PORT_CODE(KEYCODE_8)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE1 )

	PORT_START ("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_SERVICE( 0x10, IP_ACTIVE_LOW )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )	/* Also invincibility when playing */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )	/* Also speed-up when playing */
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Cabinet ) ) PORT_DIPLOCATION("SW1:1")
	PORT_DIPSETTING(    0x80, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )

	PORT_START ("DSW1")
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Coinage ) ) PORT_DIPLOCATION("SW1:2,3")
	PORT_DIPSETTING(    0x03, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x08, DEF_STR( Lives ) ) PORT_DIPLOCATION("SW1:4,5")
	PORT_DIPSETTING(    0x00, "1" )
	PORT_DIPSETTING(    0x04, "2" )
	PORT_DIPSETTING(    0x08, "3" )
	PORT_DIPSETTING(    0x0c, "5" )
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW1:6,7")
	PORT_DIPSETTING(    0x00, "10000" )
	PORT_DIPSETTING(    0x10, "15000" )
	PORT_DIPSETTING(    0x20, "20000" )
	PORT_DIPSETTING(    0x30, DEF_STR( None ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Difficulty ) ) PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x40, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START ("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

/* No alternate ghost names */
INPUT_PORTS_START( mspacman )
	PORT_INCLUDE( mspacpls )

	PORT_START ("FAKE")
	/* This fake input port is used to get the status of the fire button */
	/* and activate the speedup cheat if it is. */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME( "Speed (Cheat)" )
	PORT_DIPNAME( 0x06, 0x02, "Speed Cheat" )
	PORT_DIPSETTING(    0x00, "Disabled" )
	PORT_DIPSETTING(    0x02, "Enabled with Button" )
	PORT_DIPSETTING(    0x04, "Enabled Always" )
INPUT_PORTS_END


/* No fake dip */
INPUT_PORTS_START( pacman0 )
	PORT_INCLUDE( mspacpls )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x80, 0x80, "Ghost Names" ) PORT_DIPLOCATION("SW1:9")
	PORT_DIPSETTING(    0x80, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Alternate ) )
INPUT_PORTS_END


/* for newpuc2, pheartsb - no alternate ghost names, and the bonuses are 10k higher */
INPUT_PORTS_START( newpuc2 )
	PORT_INCLUDE( mspacman )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW1:6,7")
	PORT_DIPSETTING(    0x00, "20000" )
	PORT_DIPSETTING(    0x10, "25000" )
	PORT_DIPSETTING(    0x20, "30000" )
	PORT_DIPSETTING(    0x30, DEF_STR( None ) )
INPUT_PORTS_END


/* for newpuc2b, nwpuc2b - no alternate ghost names, and the bonuses are a lot higher */
INPUT_PORTS_START( newpuc2b )
	PORT_INCLUDE( mspacman )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW1:6,7")
	PORT_DIPSETTING(    0x00, "30000" )
	PORT_DIPSETTING(    0x10, "60000" )
	PORT_DIPSETTING(    0x20, "90000" )
	PORT_DIPSETTING(    0x30, DEF_STR( None ) )
INPUT_PORTS_END

/* for spuckman - alternate ghost names, and the bonuses are 10k higher */
INPUT_PORTS_START( spuckman )
	PORT_INCLUDE( newpuc2 )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x80, 0x80, "Ghost Names" ) PORT_DIPLOCATION("SW1:9")
	PORT_DIPSETTING(    0x80, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Alternate ) )
INPUT_PORTS_END

/* All options available */
INPUT_PORTS_START( pacman )
	PORT_INCLUDE( pacman0 )

	PORT_START ("FAKE")
	/* This fake input port is used to get the status of the fire button */
	/* and activate the speedup cheat if it is. */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME( "Speed (Cheat)" )
	PORT_DIPNAME( 0x06, 0x02, "Speed Cheat" )
	PORT_DIPSETTING(    0x00, "Disabled" )
	PORT_DIPSETTING(    0x02, "Enabled with Button" )
	PORT_DIPSETTING(    0x04, "Enabled Always" )
INPUT_PORTS_END


/*************************************
 *
 *  Graphics layouts
 *
 *************************************/

static const gfx_layout tilelayout =
{
	8,8,	/* 8*8 characters */
	RGN_FRAC(1,2),    /* 256 characters */
	2,  /* 2 bits per pixel */
	{ 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 }, /* bits are packed in groups of four */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	16*8    /* every char takes 16 bytes */
};


static const gfx_layout spritelayout =
{
	16,16,	/* 16*16 sprites */
	RGN_FRAC(1,2),	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3, 24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 32*8, 33*8, 34*8, 35*8, 36*8, 37*8, 38*8, 39*8 },
	64*8	/* every sprite takes 64 bytes */
};


static gfx_layout xtilelayout =
{
	16,16,	/* 16*16 characters */
	RGN_FRAC(1,2),    /* 256 characters */
	2,  /* 2 bits per pixel */
	{ 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 0, 1, 2, 3, 128, 129, 130, 131, 256, 257, 258, 259, 384, 385, 386, 387 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8 },
	64*8    /* every char takes 64 bytes */
};


static gfx_layout xspritelayout =
{
	32,32,	/* 32*32 sprites */
	RGN_FRAC(1,2),	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 0, 1, 2, 3, 256, 257, 258, 259, 512, 513, 514, 515, 768, 769, 770, 771,
			1024+0, 1024+1, 1024+2, 1024+3, 1024+256, 1024+257, 1024+258, 1024+259,
			1024+512, 1024+513, 1024+514, 1024+515,	1024+768, 1024+769, 1024+770, 1024+771 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8,
			16*8, 17*8, 18*8, 19*8, 20*8, 21*8, 22*8, 23*8,	24*8, 25*8, 26*8, 27*8, 28*8, 29*8, 30*8, 31*8 },
	256*8	/* every sprite takes 256 bytes */
};


static GFXDECODE_START( pacman )
	GFXDECODE_ENTRY( "gfx1", 0x0000, tilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x1000, spritelayout, 0, 32 )
GFXDECODE_END

static GFXDECODE_START( pacmanx )
	GFXDECODE_ENTRY( "gfx1", 0x0000, xtilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x4000, xspritelayout, 0, 32 )
GFXDECODE_END



/*************************************
 *
 *  Machine drivers
 *
 *************************************/

static MACHINE_CONFIG_START( pacman, pacman_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, MASTER_CLOCK/6)
	MCFG_CPU_PROGRAM_MAP(pacman_map)
	MCFG_CPU_IO_MAP(writeport)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", pacman_state, vblank_irq)
	MCFG_WATCHDOG_VBLANK_INIT(16)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_RAW_PARAMS(PIXEL_CLOCK, HTOTAL, HBEND, HBSTART, VTOTAL, VBEND, VBSTART)
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_pacman)
	MCFG_VIDEO_START_OVERRIDE(pacman_state,pacman)
	MCFG_SCREEN_PALETTE("palette")
	MCFG_PALETTE_ADD_INIT_BLACK("palette", 128*4)
	MCFG_PALETTE_INDIRECT_ENTRIES(32)
	MCFG_PALETTE_INIT_OWNER(pacman_state,pacman)
	MCFG_GFXDECODE_ADD("gfxdecode", "palette", pacman)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("namco", NAMCO, MASTER_CLOCK/6/32)
	MCFG_NAMCO_AUDIO_VOICES(3)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( pacmanx, pacman )
	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_RAW_PARAMS(PIXEL_CLOCK<<2, HTOTAL<<1, HBEND<<1, HBSTART<<1, VTOTAL<<1, VBEND<<1, VBSTART<<1)
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_pacmanx)
	MCFG_VIDEO_START_OVERRIDE(pacman_state,pacmanx)
	MCFG_GFXDECODE_MODIFY("gfxdecode", pacmanx)
MACHINE_CONFIG_END


static MACHINE_CONFIG_DERIVED( woodpek, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(woodpek_map)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( pacmanp, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_DECRYPTED_OPCODES_MAP(patched_opcodes_map)
MACHINE_CONFIG_END

/*************************************
 *
 *  Driver initialization
 *
 *************************************/

static UINT8 hbmame_pacplus_decode(offs_t addr, UINT8 e)
{
	static const unsigned char swap_xor_table[6][9] =
	{
		{ 7,6,5,4,3,2,1,0, 0x00 },
		{ 7,6,5,4,3,2,1,0, 0x28 },
		{ 6,1,3,2,5,7,0,4, 0x96 },
		{ 6,1,5,2,3,7,0,4, 0xbe },
		{ 0,3,7,6,4,2,1,5, 0xd5 },
		{ 0,3,4,6,7,2,1,5, 0xdd }
	};

	static const int picktable[32] =
	{
		0,2,4,2,4,0,4,2,2,0,2,2,4,0,4,2,
		2,2,4,0,4,2,4,0,0,4,0,4,4,2,4,2
	};

	unsigned int method = 0;
	const unsigned char *tbl;


	/* pick method from bits 0 2 5 7 9 of the address */
	method = picktable[
		(addr & 0x001) |
		((addr & 0x004) >> 1) |
		((addr & 0x020) >> 3) |
		((addr & 0x080) >> 4) |
		((addr & 0x200) >> 5)];

	/* switch method if bit 11 of the address is set */
	if ((addr & 0x800) == 0x800)
		method ^= 1;

	tbl = swap_xor_table[method];
	return BITSWAP8(e,tbl[0],tbl[1],tbl[2],tbl[3],tbl[4],tbl[5],tbl[6],tbl[7]) ^ tbl[8];
}

DRIVER_INIT_MEMBER(pacman_state,pacplus)
{
	offs_t i;

	/* CPU ROMs */
	UINT8 *RAM = machine().root_device().memregion("maincpu")->base();

	for (i = 0; i < 0x4000; i++)
		RAM[i] = hbmame_pacplus_decode(i,RAM[i]);
}

void pacman_state::eyes_decode(UINT8 *data)
{
	int j;
	UINT8 swapbuffer[8];

	for (j = 0; j < 8; j++)
		swapbuffer[j] = data[BITSWAP16(j,15,14,13,12,11,10,9,8,7,6,5,4,3,0,1,2)];

	for (j = 0; j < 8; j++)
		data[j] = BITSWAP8(swapbuffer[j],7,4,5,6,3,2,1,0);
}

DRIVER_INIT_MEMBER(pacman_state,eyes)
{
	int i;

	/* CPU ROMs */

	/* Data lines D3 and D5 swapped */
	UINT8 *RAM = machine().root_device().memregion("maincpu")->base();
	for (i = 0; i < 0x4000; i++)
		RAM[i] = BITSWAP8(RAM[i],7,6,3,4,5,2,1,0);


	/* Graphics ROMs */

	/* Data lines D4 and D6 and address lines A0 and A2 are swapped */
	RAM = machine().root_device().memregion("gfx1")->base();
	for (i = 0;i < machine().root_device().memregion("gfx1")->bytes();i += 8)
		eyes_decode(&RAM[i]);
}


/*************************************
 *
 *  ROM definitions
 *
 *************************************/

#define PACMAN_SOUND_PROMS \
	ROM_REGION( 0x0200, "namco", 0 ) \
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) ) \
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) \

#define PACMAN_PROMS \
	ROM_REGION( 0x0120, "proms", 0 ) \
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) ) \
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) ) \
	PACMAN_SOUND_PROMS \

#define PACPLUS_PROMS \
	ROM_REGION( 0x0120, "proms", 0 ) \
	ROM_LOAD( "pacplus.7f",   0x0000, 0x0020, CRC(063dd53a) SHA1(2e43b46ec3b101d1babab87cdaddfa944116ec06) ) \
	ROM_LOAD( "pacplus.4a",   0x0020, 0x0100, CRC(e271a166) SHA1(cf006536215a7a1d488eebc1d8a2e2a8134ce1a6) ) \
	PACMAN_SOUND_PROMS \

ROM_START( abortman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "abortman.5e",  0x0000, 0x1000, CRC(1c4ef687) SHA1(0adea2380f8fd43ea90a4460eea9be2a17e7a825) )
	ROM_LOAD( "abortman.5f",  0x1000, 0x1000, CRC(38a22ac4) SHA1(5503132e4d1ece4a3d71ad3635a60ec151bc0417) )

	PACMAN_PROMS
ROM_END

ROM_START( baby2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "baby2.6j",     0x3000, 0x1000, CRC(260b87b8) SHA1(cf4ace417b7b640678f809c92f690cebc977dd68) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "baby2.5e",     0x0000, 0x1000, CRC(6f7d8d57) SHA1(2e786c5a61168455d31b7c45c97891f65243a40b) )
	ROM_LOAD( "baby2.5f",     0x1000, 0x1000, CRC(b6d77a1e) SHA1(63ada2a7f154ce7bd756c5002a34e5f4002c2473) )

	PACMAN_PROMS
ROM_END

ROM_START( baby3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "baby3.6j",     0x3000, 0x1000, CRC(6c0e22c8) SHA1(bcc684d7f5fb7fe73d3dcfdd80999242789dc9c9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "baby3.5e",     0x0000, 0x1000, CRC(22174b65) SHA1(ed90d2c515e2446b369861e20ed34987f10a3dc8) )
	ROM_LOAD( "baby2.5f",     0x1000, 0x1000, CRC(b6d77a1e) SHA1(63ada2a7f154ce7bd756c5002a34e5f4002c2473) )

	PACMAN_PROMS
ROM_END

ROM_START( baby4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "baby4.6j",     0x3000, 0x1000, CRC(f8cd7ebb) SHA1(c7b8e690e96f9b863d31caddc81076d556058671) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "baby3.5e",     0x0000, 0x1000, CRC(22174b65) SHA1(ed90d2c515e2446b369861e20ed34987f10a3dc8) )
	ROM_LOAD( "baby2.5f",     0x1000, 0x1000, CRC(b6d77a1e) SHA1(63ada2a7f154ce7bd756c5002a34e5f4002c2473) )

	PACMAN_PROMS
ROM_END

ROM_START( brakman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "brakman.6j",   0x3000, 0x1000, CRC(2d2af2e5) SHA1(8355f30b106e53499d3c1e1174aee6572869b428) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "brakman.5e",   0x0000, 0x1000, CRC(a004abe7) SHA1(c2c8d871b17c9a1f8902f8ec3e46e0991e747f45) )
	ROM_LOAD( "brakman.5f",   0x1000, 0x1000, CRC(30ed6264) SHA1(5bc2753aa6f2e49f6c1ce5810cf561d27ecb38e6) )

	PACMAN_PROMS
ROM_END

ROM_START( bucaneer )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "bucaneer.6k",  0x3000, 0x1000, CRC(c4d9169a) SHA1(f34a51d9fa90739214ab9e837b2602c992a73576) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "bucaneer.5e",  0x0000, 0x0800, CRC(4060c077) SHA1(78d4aa5243246f73533fc0886438dc1fa6f7ebe5) )
	ROM_LOAD( "bucaneer.5h",  0x0800, 0x0800, CRC(e3861283) SHA1(61cf8ed24902910e98438d9e2e2745f226ad2a13) )
	ROM_LOAD( "bucaneer.5f",  0x1000, 0x0800, CRC(09f66dec) SHA1(2d3649341fed19bac15ec274f7d747de46a3edb2) )
	ROM_LOAD( "bucaneer.5j",  0x1800, 0x0800, CRC(653314e7) SHA1(c466a421917b3502e9115ebda1b2d11f7f586de8) )
	/* cannot do the "mario" trick here because bucco does odd things when going up */

	PACMAN_PROMS
ROM_END

ROM_START( caterpil )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "caterpil.6e",  0x0000, 0x1000, CRC(b53c5650) SHA1(54f1fee2494cfc0452051a6f134c5c3b691b4ffb) )
	ROM_LOAD( "caterpil.6f",  0x1000, 0x1000, CRC(53845efb) SHA1(e6db734a7a806aead30422de4073511da55a17f1) )
	ROM_LOAD( "caterpil.6h",  0x2000, 0x1000, CRC(a474357e) SHA1(0fb93ca3364b5955428bc59e2f6d9f186182a280) )
	ROM_LOAD( "caterpil.6j",  0x3000, 0x1000, CRC(5df0ea3b) SHA1(dce379bb4cf5fa7c253cc16702360cbc3b637f37) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "caterpil.5e",  0x0000, 0x1000, CRC(0387907d) SHA1(014745c480ee4b81c161cfa7a011df673059a1a8) )
	ROM_LOAD( "caterpil.5f",  0x1000, 0x1000, CRC(aa450714) SHA1(8a888ce6573623d15913ff297a027d4056eb2875) )

	PACMAN_PROMS
ROM_END

ROM_START( chthang )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( chtpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "chtpac.6f",    0x1000, 0x1000, CRC(72cea888) SHA1(87d0c2f8e7be0b9f28fc46b5711ddb8a85023942) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( chtpac3d )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "chtpac3d.6f",  0x1000, 0x1000, CRC(fe975805) SHA1(a213628c19d8998960c45f756d865dfce404ab57) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman3d.6j",  0x3000, 0x1000, CRC(959e930e) SHA1(0ef24017be05ddf85248c4d00f0e21cbf658f78c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman3d.5e",  0x0000, 0x1000, CRC(aa203d45) SHA1(7d2fb9bc8718601efc33d2469452a08018167e60) )
	ROM_LOAD( "pacman3d.5f",  0x1000, 0x1000, CRC(d1830540) SHA1(5d276774997e3be8fc9c21d55562288acd1cffcc) )

	PACMAN_PROMS
ROM_END

ROM_START( chtpman2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "chtpac.6f",    0x1000, 0x1000, CRC(72cea888) SHA1(87d0c2f8e7be0b9f28fc46b5711ddb8a85023942) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "newpuck2.6j",  0x3000, 0x1000, CRC(0e3bdbde) SHA1(11679a3e60cb98ef56e9f0c349d421a72b7097f6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( chtpop )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "chtpac.6f",    0x1000, 0x1000, CRC(72cea888) SHA1(87d0c2f8e7be0b9f28fc46b5711ddb8a85023942) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacpopey.5f",  0x1000, 0x1000, CRC(af2b610b) SHA1(c787b1db588125f0770dc2bf4626ad22f0faee72) )

	PACMAN_PROMS
ROM_END

ROM_START( chtpuck )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "chtpac.6f",    0x1000, 0x1000, CRC(72cea888) SHA1(87d0c2f8e7be0b9f28fc46b5711ddb8a85023942) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "fastpuck.6j",  0x3000, 0x1000, CRC(2e64a3ba) SHA1(f86a921173f32211b18d023c2701664d13ae23be) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( crazypac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "crazypac.6j",  0x3000, 0x1000, CRC(9f754a51) SHA1(447d95f90c6481911189173f95dbd5ea23467f67) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( ctrpllrp )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "ctrpllrp.6k",  0x0800, 0x0800, CRC(f39846d3) SHA1(bc1137a45898210523cf8da1e6a2425e7c322884) )
	ROM_LOAD( "ctrpllrp.6f",  0x1000, 0x0800, CRC(afa149a8) SHA1(207b842854ac9e015e12a2dae41105438cda1df9) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "ctrpllrp.6h",  0x2000, 0x0800, CRC(6bb282a1) SHA1(a96f25dc0f49ebe7e528e3297a112d778c6c3030) )
	ROM_LOAD( "ctrpllrp.6n",  0x2800, 0x0800, CRC(fa2140f5) SHA1(123d31e653e8af78c6153702eca2e136c427ed64) )
	ROM_LOAD( "ctrpllrp.6j",  0x3000, 0x0800, CRC(86c91e0e) SHA1(52af6a3af5b1363859f790470ca5860ef2a08566) )
	ROM_LOAD( "ctrpllrp.6p",  0x3800, 0x0800, CRC(3d28134e) SHA1(45a257a0aca74e2ab36dd70097220d8be29cc87b) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "ctrpllrp.5e",  0x0000, 0x0800, CRC(1c4617be) SHA1(2b2b10f1256b4612e3e01ed1c8e2d7ccb6989f5d) )
	ROM_LOAD( "ctrpllrp.5h",  0x0800, 0x0800, CRC(46f72fef) SHA1(daf334c78fdb73d43d524b733b763b290c602ae2) )
	ROM_LOAD( "ctrpllrp.5f",  0x1000, 0x0800, CRC(ba9ec199) SHA1(626ab2eedf4c8d307dfad3b8863a67f8c34dda97) )
	ROM_LOAD( "ctrpllrp.5j",  0x1800, 0x0800, CRC(41c09655) SHA1(cc639e660443b9dcb33f9aefe9af5d332591c466) )

	PACMAN_PROMS
ROM_END

ROM_START( dizzy )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "dizzy.6j",     0x3000, 0x1000, CRC(bf05bf15) SHA1(9a440a7a67731553269ad72dc2340de1fb9a89a8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "dizzy.5e",     0x0000, 0x1000, CRC(f999a8d9) SHA1(4feecc8f06ea9149751f0f97828babca1bdf3fd4) )
	ROM_LOAD( "dizzy.5f",     0x1000, 0x1000, CRC(7cb7b9b1) SHA1(3bd09964a2d49c65a3619ddfc3635066a7097328) )

	PACMAN_PROMS
ROM_END

ROM_START( eltonpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "eltonpac.5e",  0x0000, 0x1000, CRC(9c093e5b) SHA1(bfdd082a6f1e0819aeda2601f06dff90cb8e8f3f) )
	ROM_LOAD( "eltonpac.5f",  0x1000, 0x1000, CRC(c2825641) SHA1(4ea373b18ec1fc7325f33dbe9f44ee93ad6f6cb4) )

	PACMAN_PROMS
ROM_END

ROM_START( europac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "europac.6j",   0x3000, 0x1000, CRC(a4e82007) SHA1(577b44661de47dcffac7c06e7f9fa1e5d4e085ed) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "europac.5e",   0x0000, 0x1000, CRC(00208da4) SHA1(a7ef38f2c133681344d67634f8a7362f6f0712b3) )
	ROM_LOAD( "europac.5f",   0x1000, 0x1000, CRC(0d36dabf) SHA1(50572528e230a37a413cdc9e2bd7f2ea76fffa09) )

	PACMAN_PROMS
ROM_END

ROM_START( fasthang )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "fasthang.6f",  0x1000, 0x1000, CRC(1b1014bc) SHA1(c7ff86832ccd207ce8505291ae309ee0240d0586) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( fastpop )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacmanf.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacpopey.5f",  0x1000, 0x1000, CRC(af2b610b) SHA1(c787b1db588125f0770dc2bf4626ad22f0faee72) )

	PACMAN_PROMS
ROM_END

ROM_START( fastpuck )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "pacmanf.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "fastpuck.6j",  0x3000, 0x1000, CRC(2e64a3ba) SHA1(f86a921173f32211b18d023c2701664d13ae23be) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( fpnleash )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "punleash.1",   0x0000, 0x1000, CRC(eddb20e7) SHA1(07f6cabac3c59c2e3f522da877e845b3ceb2cffb) )
	ROM_LOAD( "punleash.2",   0x1000, 0x1000, CRC(57fe8b4d) SHA1(629800fe793b53c27ee8c84a2e49caf7fe056f28) )
	ROM_LOAD( "punleash.3",   0x2000, 0x1000, CRC(94c63bb1) SHA1(872dbca70b45285dccfd0e24cb50225a5c3ac005) )
	ROM_LOAD( "punleash.4",   0x3000, 0x1000, CRC(fe7734d5) SHA1(f246baab82ea8e9b3cfd3dc5911445ab08223c1f) )
	ROM_LOAD( "punleash.5",   0x8000, 0x1000, CRC(ef155ffb) SHA1(c5bcd57c2a336fa8a018b192b8542954271f1bac) )
	ROM_LOAD( "punleash.6",   0x9000, 0x1000, CRC(70d15899) SHA1(9a7cff5811b591211eb23cee125fd77263f8d4fe) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "fpnleash.5e",  0x0000, 0x1000, CRC(1f69d1d0) SHA1(8f3834358a074d0cb6c1d56e3b0e623193f8b9ec) )
	ROM_LOAD( "punleash.5f",  0x1000, 0x1000, CRC(98d3d364) SHA1(f8023d1772d377d090a9bfebf194bbbce8643f1d) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "punleash.7f",  0x0000, 0x0020, CRC(357c2523) SHA1(f0c4cea06b17932cc15f6db2971406eaba3558b2) )
	ROM_LOAD( "punleash.4a",  0x0020, 0x0100, CRC(5ff3b85a) SHA1(6b01b9c9302ab41fe83c140c3ff217d1d66773dd) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( fstpman2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "pacmanf.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "newpuck2.6j",  0x3000, 0x1000, CRC(0e3bdbde) SHA1(11679a3e60cb98ef56e9f0c349d421a72b7097f6) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( hangly )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( hangly2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly2.6j",   0x3000, 0x0800, CRC(51305374) SHA1(6197b606a0eedb11135d9f4f7a89aecc23fb2d33) )
	ROM_LOAD( "hangly2.6p",   0x3800, 0x0800, CRC(427c9d4d) SHA1(917bc3d571cbdd24d88327ecabfb5b3f6d39af0a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmanh.5e",   0x0000, 0x1000, CRC(299fb17a) SHA1(ad97adc2122482a9018bacd137df9d8f409ddf85) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( hangly3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "hangly3.6k",	  0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "hangly3.6h",   0x2000, 0x0800, CRC(08419c4a) SHA1(7e5001adad401080c788737c1d2349f218750442) )
	ROM_LOAD( "hangly3.6n",   0x2800, 0x0800, CRC(ab74b51f) SHA1(1bce8933ed7807eb7aca9670df8994f8d1a8b5b7) )
	ROM_LOAD( "hangly3.6j",   0x3000, 0x0800, CRC(5039b082) SHA1(086a6ac4742734167d283b1121fce29d8ac4a6cd) )
	ROM_LOAD( "hangly3.6p",   0x3800, 0x0800, CRC(931770d7) SHA1(78fcf88e07ec5126c12c3297b62ca388809e947c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "hangly3.5e",   0x0000, 0x0800, CRC(5f4be3cc) SHA1(eeb0e1e44549b99eab481d9ac016b4359e19fe30) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( hanglyad )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "hanglyad.5e",  0x0000, 0x1000, CRC(93fd3682) SHA1(792ddb541aeab304b5facfcf929c3bba18150346) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( hanglyx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( hangly2x )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly2.6j",   0x3000, 0x0800, CRC(51305374) SHA1(6197b606a0eedb11135d9f4f7a89aecc23fb2d33) )
	ROM_LOAD( "hangly2.6p",   0x3800, 0x0800, CRC(427c9d4d) SHA1(917bc3d571cbdd24d88327ecabfb5b3f6d39af0a) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmodx.5e",   0x0000, 0x4000, CRC(a05bc552) SHA1(5c2350aeac14cafebfcb7ff012427b1d3c6adf62) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( hangly3x )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "hangly3.6k",	  0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "hangly3.6h",   0x2000, 0x0800, CRC(08419c4a) SHA1(7e5001adad401080c788737c1d2349f218750442) )
	ROM_LOAD( "hangly3.6n",   0x2800, 0x0800, CRC(ab74b51f) SHA1(1bce8933ed7807eb7aca9670df8994f8d1a8b5b7) )
	ROM_LOAD( "hangly3.6j",   0x3000, 0x0800, CRC(5039b082) SHA1(086a6ac4742734167d283b1121fce29d8ac4a6cd) )
	ROM_LOAD( "hangly3.6p",   0x3800, 0x0800, CRC(931770d7) SHA1(78fcf88e07ec5126c12c3297b62ca388809e947c) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "hangly3x.5e",  0x0000, 0x4000, CRC(54abb087) SHA1(a57b59325172902c3fae57c5a23e588d5ac7b8ca) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( hearts )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "hearts.5e",    0x0000, 0x1000, CRC(5be0a4e4) SHA1(7314e670c81debe4ae19bb5ff4da5718a63ef084) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( hellowp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hellowp.6e",   0x0000, 0x00f3, CRC(699152c6) SHA1(cf8ec6a969177146ccc6155d896b693afbb70dd8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( hm1000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm1000.5e",    0x0000, 0x1000, CRC(16d9f7ce) SHA1(a9ac72aa5e3dcaaa2e4d03b6ae372e81d77817f8) )
	ROM_LOAD( "pm1000.5f",    0x1000, 0x1000, CRC(fd06d923) SHA1(a9d61bdcb6841bf07ce8ad08e1be2cf5ef349129) )

	PACMAN_PROMS
ROM_END

ROM_START( hm2000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm2000.5e",    0x0000, 0x1000, CRC(cba39624) SHA1(d815cb5be44b3d29cd1bdd4b8c68484e5330e4a4) )
	ROM_LOAD( "pm2000.5f",    0x1000, 0x1000, CRC(a2b31528) SHA1(69f1c4b34e581c0e6d75b9726cbefa08f8a97fdb) )

	PACMAN_PROMS
ROM_END

ROM_START( hmba5000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba3000.5e",  0x0000, 0x1000, CRC(5ebb61c5) SHA1(cfd491170d1d3d0c0632d5d81629e98cfd100fb6) )
	ROM_LOAD( "pmba3000.5f",  0x1000, 0x1000, CRC(d9a055aa) SHA1(00c6867b70062672da3b8c254feff4354e35aa7a) )

	PACMAN_PROMS
ROM_END

ROM_START( hmba7000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba6000.5e",  0x0000, 0x1000, CRC(7961ed4e) SHA1(595311b7a77d1379c9564d42a34581cc35a53875) )
	ROM_LOAD( "pmba6000.5f",  0x1000, 0x1000, CRC(417080fd) SHA1(13e8e812bca25b64e7ce072d86189fd15b1882b0) )

	PACPLUS_PROMS
ROM_END

ROM_START( hmbabymz )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmbamaz.5e",   0x0000, 0x1000, CRC(9a8cd8a1) SHA1(653d3c900b0be5e7ffe424207419bf86229b8c48) )
	ROM_LOAD( "hmbabymz.5f",  0x1000, 0x1000, CRC(f0bd55bd) SHA1(310a375e3b3ec2766399418ce30233cc2263961e) )

	PACMAN_PROMS
ROM_END

ROM_START( hmbluep )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm1000.5e",    0x0000, 0x1000, CRC(16d9f7ce) SHA1(a9ac72aa5e3dcaaa2e4d03b6ae372e81d77817f8) )
	ROM_LOAD( "hmbluep.5f",   0x1000, 0x1000, CRC(ceb721be) SHA1(7eb60764bb7f9aa63a9f2a9a3052c2b988f5a530) )

	PACMAN_PROMS
ROM_END

ROM_START( hmgrenp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmgrenp.5e",   0x0000, 0x1000, CRC(5807a438) SHA1(8f4160627948747e92c3c5dc386068e4561fd9aa) )
	ROM_LOAD( "hmgrenp.5f",   0x1000, 0x1000, CRC(1fcc0cf2) SHA1(8920525a7fccdc3fea2ee52687b4881ca84ebaad) )

	PACPLUS_PROMS
ROM_END

ROM_START( hmhearts )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "hangly3.6k",   0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "newpuc2.6m",   0x1800, 0x0800, CRC(70810ccf) SHA1(3941678606aab1e53356a6781e24d84e83cc88ce) )
	ROM_LOAD( "hangly3.6h",   0x2000, 0x0800, CRC(08419c4a) SHA1(7e5001adad401080c788737c1d2349f218750442) )
	ROM_LOAD( "newpuc2.6n",   0x2800, 0x0800, CRC(3f250c58) SHA1(53bf2270c26f10f7e97960cd4c96e09e16b9bdf3) )
	ROM_LOAD( "hmhearts.6j",  0x3000, 0x0800, CRC(603b70b7) SHA1(a8e408e33fb16f0879793d416c3074dc48c875db) )
	ROM_LOAD( "hmhearts.6p",  0x3800, 0x0800, CRC(71293b1f) SHA1(b507a74866d34ecf0e6d32c91f9b5655817f6c3f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacheart.5e",  0x0000, 0x0800, CRC(c62bbabf) SHA1(f6f28ae33c2ab274105283b22b49ad243780a95e) )
	ROM_LOAD( "newpuc2.5h",   0x0800, 0x0800, CRC(777c70d3) SHA1(ed5ccbeb1102ec9f837577de3aa51317c32520d6) )
	ROM_LOAD( "newpuc2.5f",   0x1000, 0x0800, CRC(ca8c184c) SHA1(833aa845824ed80777b62f03df36a920ad7c3656) )
	ROM_LOAD( "newpuc2.5j",   0x1800, 0x0800, CRC(7dc75a81) SHA1(d3fe1cad3b594052d8367685febb2335b0ad62f4) )

	PACMAN_PROMS
ROM_END

ROM_START( hmpopey )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacpopey.5f",  0x1000, 0x1000, CRC(af2b610b) SHA1(c787b1db588125f0770dc2bf4626ad22f0faee72) )

	PACMAN_PROMS
ROM_END

ROM_START( hmplus )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacplus.5e",   0x0000, 0x1000, CRC(022c35da) SHA1(57d7d723c7b029e3415801f4ce83469ec97bb8a1) )
	ROM_LOAD( "hmplus.5f",    0x1000, 0x1000, CRC(7a444bda) SHA1(71f93790843482d9389c6d66a7ee5196119185fb) )

	PACPLUS_PROMS
ROM_END

ROM_START( homercid )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "homercid.6e",  0x0000, 0x1041, CRC(2768e80e) SHA1(aaf78f4a1bea4aba3e44275ccfbc2ee641a3b8ff) )
	ROM_LOAD( "homercid.6f",  0x1000, 0x1000, CRC(3858d8b7) SHA1(44560196ca888a82669d94687c964373fd0ffc4d) )
	ROM_LOAD( "homercid.6h",  0x2000, 0x1003, CRC(d61413d0) SHA1(a5ea1fdc50e2d185440e38a01837a6c5a5b89484) )
	ROM_LOAD( "homercid.6j",  0x3000, 0x1003, CRC(e8b53076) SHA1(c8fe2c2c637a87356a95176bc333ca28f63ae00a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "homercid.5e",  0x0000, 0x1000, CRC(4879edc5) SHA1(abde239a818a7025e629e59490479da700d70538) )
	ROM_LOAD( "homercid.5f",  0x1000, 0x1000, CRC(475e51c9) SHA1(478d1907f448f40f50b1b2ca1ddabc5b818e7003) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "homercid.7f",  0x0000, 0x0020, CRC(2b7e868a) SHA1(28976b3b7a2ea112da1d7af0366090f8a172dbb6) )
	ROM_LOAD( "homercid.4a",  0x0020, 0x0100, CRC(9717a6be) SHA1(00497d00c45d48570de0ab6ca8fea42cdb38eb85) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( jacman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "jacman.6j",    0x3000, 0x1000, CRC(1f52ef8b) SHA1(0a803352a964de9567bc314f8ace1c612082e3d0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "jacman.5e",    0x0000, 0x1000, CRC(809bd73e) SHA1(81989a21d089eb76297926905729efcd69abeb87) )
	ROM_LOAD( "jacman.5f",    0x1000, 0x1000, CRC(0509d3e6) SHA1(e589543b9a2f1c81115c03b4b88096ac84826e62) )

	PACMAN_PROMS
ROM_END

ROM_START( joyman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacheart.6e",  0x0000, 0x0800, CRC(d844b679) SHA1(c4486198b3126bb8e05a308c53787e51065f77ae) )
	ROM_LOAD( "joyman.6k",    0x0800, 0x0800, CRC(ab9c8f29) SHA1(3753b8609c30d85d89acf745cf9303b77be440fd) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "joyman.6m",    0x1800, 0x0800, CRC(b3c8d32e) SHA1(8b336fca1300820308cd5c4efc60bf2ba4199302) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "pacheart.6n",  0x2800, 0x0800, CRC(888f3c3e) SHA1(c2b5917bf13071131dd53ea76f0da86706db2d80) )
	ROM_LOAD( "joyman.6j",    0x3000, 0x0800, CRC(00b553f8) SHA1(57f2e4a6da9f00935fead447b2123a8b95e5d672) )
	ROM_LOAD( "joyman.6p",    0x3800, 0x0800, CRC(5d5ce992) SHA1(ced7ed39cfc7ec7b2c0459e275577976109ee82f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "joyman.5e",    0x0000, 0x0800, CRC(39b557bc) SHA1(0f602ec84cb25fced89699e430b95b5ae93c83bd) )
	ROM_LOAD( "joyman.5h",    0x0800, 0x0800, CRC(33e0289e) SHA1(c1b910bdc61e560a8c34298deb11401f718e7330) )
	ROM_LOAD( "joyman.5f",    0x1000, 0x0800, CRC(338771a6) SHA1(7cd68cc428986255d0de29aae894900519e7fda5) )
	ROM_LOAD( "joyman.5j",    0x1800, 0x0800, CRC(f4f0add5) SHA1(d71c54ef55a755ec1316623d183b4f615ef7c055) )

	PACMAN_PROMS
ROM_END

/* No gfx roms and no proms included. Using puckman ones. Dumped by Andy Welburn. */
ROM_START( jpmmunch )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck1.6e",  0x0000, 0x0800, CRC(2c0fa0ab) SHA1(37680e4502771ae69d51d07ce43f65b9b2dd2a49) )
	ROM_LOAD( "jpmmunch.6k",  0x0800, 0x0800, CRC(afeca2f1) SHA1(1e6d6c75eeb3a354ce2dc88da62caf9e7d53d0cb) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "puckman.6m",   0x1800, 0x0800, CRC(d3e8914c) SHA1(c2f00e1773c6864435f29c8b7f44f2ef85d227d3) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "jpmmunch.6n",  0x2800, 0x0800, CRC(93f344c5) SHA1(987c7fa18a774a47c045fa1dc7dff37457cb8983) )
	ROM_LOAD( "jpmmunch.6j",  0x3000, 0x0800, CRC(258580a2) SHA1(c594329963215971dba7cd857dfde5323f610dba) )
	ROM_LOAD( "jpmmunch.6p",  0x3800, 0x0800, CRC(b4d7ee8c) SHA1(38dfea4443f13c15ed17e0eb8f7ca24ce14d6ca8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( mazeman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "mazeman.6f",   0x1000, 0x1000, CRC(77e44852) SHA1(01c2e54cf9adc3bd92ab02eaa471638d8f17754d) )
	ROM_LOAD( "mazeman.6h",   0x2000, 0x1000, CRC(6d16a528) SHA1(3ef42b46564bf3b8347f6bddb1940368a23fa515) )
	ROM_LOAD( "mazeman.6j",   0x3000, 0x1000, CRC(3c3ff208) SHA1(2c6a8f9e3f130c3c0a9fa535b1ff76ac26a07f9f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mazeman.5e",   0x0000, 0x1000, CRC(38870a32) SHA1(2b65a6309b204b89dcb6329e43f55301ecf69227) )
	ROM_LOAD( "mazeman.5f",   0x1000, 0x1000, CRC(1fd18b80) SHA1(2b10b572840e5977adda490d5fbddef98b65575e) )

	PACMAN_PROMS
ROM_END

ROM_START( namcosil )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "namcosil.6f",  0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "namcosil.6j",  0x3000, 0x1000, CRC(57a07f6e) SHA1(8a7a1d4662e4fc5c5374ad1cad09fd4da1f8398a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( newpuc2 )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "newpuc2.6e",   0x0000, 0x0800, CRC(69496a98) SHA1(2934051d6305cc3654951bc1aacf2b8902f463fe) )
	ROM_LOAD( "newpuc2.6k",   0x0800, 0x0800, CRC(158fc01c) SHA1(2f7a1e24d259fdc716ef8e7354a87780595f3c4e) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "newpuc2.6m",   0x1800, 0x0800, CRC(70810ccf) SHA1(3941678606aab1e53356a6781e24d84e83cc88ce) )
	ROM_LOAD( "newpuc2.6h",   0x2000, 0x0800, CRC(81719de8) SHA1(e886d04ac0e20562a4bd2df7676bdf9aa98665d7) )
	ROM_LOAD( "newpuc2.6n",   0x2800, 0x0800, CRC(3f250c58) SHA1(53bf2270c26f10f7e97960cd4c96e09e16b9bdf3) )
	ROM_LOAD( "newpuc2.6j",   0x3000, 0x0800, CRC(e6675736) SHA1(85d0bb79bc96acbc67fcb70ff4d453c870a6c8ea) )
	ROM_LOAD( "newpuc2.6p",   0x3800, 0x0800, CRC(1f81e765) SHA1(442d8a82e79ae842f1ffb46369c632c1d0b83161) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "newpuc2.5h",   0x0800, 0x0800, CRC(777c70d3) SHA1(ed5ccbeb1102ec9f837577de3aa51317c32520d6) )
	ROM_LOAD( "newpuc2.5f",   0x1000, 0x0800, CRC(ca8c184c) SHA1(833aa845824ed80777b62f03df36a920ad7c3656) )
	ROM_LOAD( "newpuc2.5j",   0x1800, 0x0800, CRC(7dc75a81) SHA1(d3fe1cad3b594052d8367685febb2335b0ad62f4) )

	PACMAN_PROMS
ROM_END

ROM_START( newpuc2b )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "newpuc2.6k",   0x0800, 0x0800, CRC(158fc01c) SHA1(2f7a1e24d259fdc716ef8e7354a87780595f3c4e) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "newpuc2.6m",   0x1800, 0x0800, CRC(70810ccf) SHA1(3941678606aab1e53356a6781e24d84e83cc88ce) )
	ROM_LOAD( "newpuc2b.6h",  0x2000, 0x0800, CRC(f5e4b2b1) SHA1(68464f61cc50931f6cd4bb493dd703c139500825) )
	ROM_LOAD( "newpuc2.6n",   0x2800, 0x0800, CRC(3f250c58) SHA1(53bf2270c26f10f7e97960cd4c96e09e16b9bdf3) )
	ROM_LOAD( "newpuc2b.6j",  0x3000, 0x0800, CRC(f068e009) SHA1(a30763935e116559d535654827230bb21a5734bb) )
	ROM_LOAD( "newpuc2b.6p",  0x3800, 0x0800, CRC(1fadcc2f) SHA1(2d636cfc2b52b671ac5a26a03b1195e2cf8d4718) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "newpuc2.5h",   0x0800, 0x0800, CRC(777c70d3) SHA1(ed5ccbeb1102ec9f837577de3aa51317c32520d6) )
	ROM_LOAD( "newpuc2.5f",   0x1000, 0x0800, CRC(ca8c184c) SHA1(833aa845824ed80777b62f03df36a920ad7c3656) )
	ROM_LOAD( "newpuc2.5j",   0x1800, 0x0800, CRC(7dc75a81) SHA1(d3fe1cad3b594052d8367685febb2335b0ad62f4) )

	PACMAN_PROMS
ROM_END

/* This was from the Italian dumping team, called "puckmanbootleg.zip". They did not dump the 2nd, 4th, 6th, 8th
   program roms, so I have used the ones from pacheart for now. Also, it has the standard maze instead of the
   newpuck one -  but this could be correct? */
ROM_START( newpuc1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck1.6e",  0x0000, 0x0800, CRC(2c0fa0ab) SHA1(37680e4502771ae69d51d07ce43f65b9b2dd2a49) )
	ROM_LOAD( "pacheart.6k",  0x0800, 0x0800, CRC(b9152a38) SHA1(b6be2cb6bc7dd123503eb6bf1165dd1c99456813) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "pacheart.6m",  0x1800, 0x0800, CRC(842d6574) SHA1(40e32d09cc8d701eb318716493a68cf3f95d3d6d) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "pacheart.6n",  0x2800, 0x0800, CRC(888f3c3e) SHA1(c2b5917bf13071131dd53ea76f0da86706db2d80) )
	ROM_LOAD( "newpuck1.6j",  0x3000, 0x0800, CRC(565687c6) SHA1(9ee8e8197e87e468669279e738fe518e5480363c) )
	ROM_LOAD( "pacheart.6p",  0x3800, 0x0800, CRC(1a21a381) SHA1(d5367a327d19fb57ba5e484bd4fda1b10953c040) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "newpuck1.5e",  0x0000, 0x0800, CRC(3ed40275) SHA1(bf56f9a21bc1dacee438d88eb5bc59f20906b00c) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( newpuck2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "newpuck2.6j",  0x3000, 0x1000, CRC(0e3bdbde) SHA1(11679a3e60cb98ef56e9f0c349d421a72b7097f6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( newpuc2x )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "newpuc2.6e",   0x0000, 0x0800, CRC(69496a98) SHA1(2934051d6305cc3654951bc1aacf2b8902f463fe) )
	ROM_LOAD( "newpuc2.6k",   0x0800, 0x0800, CRC(158fc01c) SHA1(2f7a1e24d259fdc716ef8e7354a87780595f3c4e) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "newpuc2.6m",   0x1800, 0x0800, CRC(70810ccf) SHA1(3941678606aab1e53356a6781e24d84e83cc88ce) )
	ROM_LOAD( "newpuc2.6h",   0x2000, 0x0800, CRC(81719de8) SHA1(e886d04ac0e20562a4bd2df7676bdf9aa98665d7) )
	ROM_LOAD( "newpuc2.6n",   0x2800, 0x0800, CRC(3f250c58) SHA1(53bf2270c26f10f7e97960cd4c96e09e16b9bdf3) )
	ROM_LOAD( "newpuc2.6j",   0x3000, 0x0800, CRC(e6675736) SHA1(85d0bb79bc96acbc67fcb70ff4d453c870a6c8ea) )
	ROM_LOAD( "newpuc2.6p",   0x3800, 0x0800, CRC(1f81e765) SHA1(442d8a82e79ae842f1ffb46369c632c1d0b83161) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "newpuc2x.5e",  0x0000, 0x4000, CRC(0a64f4c8) SHA1(1f3456b14b2b847869db6eeb3c115e3042b24672) )
	ROM_LOAD( "newpuc2x.5f",  0x4000, 0x4000, CRC(4ba7164c) SHA1(902e6f0bfeed8f40b83cf0eb7de6440d91ee811d) )

	PACMAN_PROMS
ROM_END

ROM_START( newpuck3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "newpuck3.6h",  0x2000, 0x1000, CRC(ee3c8685) SHA1(17d80ea2766370dcec04070de67cde69d2c3b04b) )
	ROM_LOAD( "newpuck3.6j",  0x3000, 0x1000, CRC(6ee64463) SHA1(5b692313dbe448b6b016f9dd2403757e16578049) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "newpuck3.5e",  0x0000, 0x1000, CRC(6209b614) SHA1(23434b89f25d6164dca619192a282c60a63418e1) )
	ROM_LOAD( "newpuck3.5f",  0x1000, 0x1000, CRC(923515bc) SHA1(a3605a632f4b47616e0c6e4e1cdf1ef9c71bb0e0) )

	PACMAN_PROMS
ROM_END

ROM_START( newpuckx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "fastpuck.6j",  0x3000, 0x1000, CRC(2e64a3ba) SHA1(f86a921173f32211b18d023c2701664d13ae23be) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( nwpuc2b )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "nwpuc2b.6e",   0x0000, 0x1000, CRC(532bd09f) SHA1(ed362ae0b38a00747128046a14d68d2a676953ae) )
	ROM_LOAD( "nwpuc2b.6f",   0x1000, 0x1000, CRC(b9062f57) SHA1(cdfb2264fc5874cd18848fde936ea1314ce74cba) )
	ROM_LOAD( "nwpuc2b.6h",   0x2000, 0x1000, CRC(519b3c57) SHA1(cc03d67035f9d590fadef899d9a2d978a4978b39) )
	ROM_LOAD( "nwpuc2b.6j",   0x3000, 0x1000, CRC(154cc118) SHA1(e5bd6033653edcfadac467573613e1b7dbf9eb44) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "nwpuc2b.5e",   0x0000, 0x1000, CRC(58357313) SHA1(a1778318e07510eeb5b23a5d650f5a5f9fc0fb5a) )
	ROM_LOAD( "nwpuc2b.5f",   0x1000, 0x1000, CRC(4b044c87) SHA1(3e1a497563918d1ddd730efb9622abe650a258a1) )

	PACMAN_PROMS
ROM_END

ROM_START( nwpuc2bx )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "newpuc2.6k",   0x0800, 0x0800, CRC(158fc01c) SHA1(2f7a1e24d259fdc716ef8e7354a87780595f3c4e) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "newpuc2.6m",   0x1800, 0x0800, CRC(70810ccf) SHA1(3941678606aab1e53356a6781e24d84e83cc88ce) )
	ROM_LOAD( "nwepuc2b.6h",  0x2000, 0x0800, CRC(f5e4b2b1) SHA1(68464f61cc50931f6cd4bb493dd703c139500825) )
	ROM_LOAD( "newpuc2.6n",   0x2800, 0x0800, CRC(3f250c58) SHA1(53bf2270c26f10f7e97960cd4c96e09e16b9bdf3) )
	ROM_LOAD( "newpuc2b.6j",  0x3000, 0x0800, CRC(f068e009) SHA1(a30763935e116559d535654827230bb21a5734bb) )
	ROM_LOAD( "newpuc2b.6p",  0x3800, 0x0800, CRC(1fadcc2f) SHA1(2d636cfc2b52b671ac5a26a03b1195e2cf8d4718) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "newpuc2x.5e",  0x0000, 0x4000, CRC(0a64f4c8) SHA1(1f3456b14b2b847869db6eeb3c115e3042b24672) )
	ROM_LOAD( "newpuc2x.5f",  0x4000, 0x4000, CRC(4ba7164c) SHA1(902e6f0bfeed8f40b83cf0eb7de6440d91ee811d) )

	PACMAN_PROMS
ROM_END

ROM_START( nwpuckxx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck2.6e",  0x0000, 0x1000, CRC(a8ae23c5) SHA1(1481a4f083b563350744f9d25b1bcd28073875d6) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "newpuck2.6h",  0x2000, 0x1000, CRC(197443f8) SHA1(119aab12a9e1052c7b9a1f81e563740b41429a8c) )
	ROM_LOAD( "fastpuck.6j",  0x3000, 0x1000, CRC(2e64a3ba) SHA1(f86a921173f32211b18d023c2701664d13ae23be) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( pac2600 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pac2600.6f",   0x1000, 0x1000, CRC(0a27d11d) SHA1(b7b8da84b3a472974ed2f53a82d56bc8e81cef5c) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pac2600.6j",   0x3000, 0x1000, CRC(ab23fe0c) SHA1(1ae9882ee1c33ea18cf40c2ef5563cc3b546dcd5) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pac2600.5e",   0x0000, 0x1000, CRC(87d8f5ce) SHA1(fe421ee0f5ef70b56f0760473b003a886bed3d72) )
	ROM_LOAD( "pac2600.5f",   0x1000, 0x1000, CRC(91bd671f) SHA1(ae1745a03090a747baf78f8554ea8620fa2b9d81) )

	PACMAN_PROMS
ROM_END

ROM_START( pac2600a )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pac2600a.6j",  0x3000, 0x1000, CRC(d168bdfa) SHA1(0edc9c435385f9bce1939ad5ea41225ddf1db16f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pac2600a.5e",  0x0000, 0x1000, CRC(20e327e3) SHA1(857e4d67ae9f2f9b9b60fd4f9c1327b5056e3705) )
	ROM_LOAD( "pac2600a.5f",  0x1000, 0x1000, CRC(feb98199) SHA1(66a84d5813de4441e6dd70e91fbdb482f999da8f) )

	PACMAN_PROMS
ROM_END

ROM_START( pacbaby )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacbaby.6j",   0x3000, 0x1000, CRC(4e4ac798) SHA1(deab2d93a678abee576c891bdfb7ab93d5718792) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacbaby2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacbaby2.6j",  0x3000, 0x1000, CRC(742453ba) SHA1(21707992ea3489c8bc95b14db33f62b6aaa24eb4) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacbaby3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacbaby3.6j",  0x3000, 0x1000, CRC(b82f2b73) SHA1(63619220b9ed4dfeb4f462b41d9685870716590a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacbell )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacbell.6j",   0x3000, 0x1000, CRC(4b428215) SHA1(acdcc4d7ccfc58c6c0473e8ee548b5e02dcafcfd) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacelec )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacelec.6j",   0x3000, 0x1000, CRC(9e5c763d) SHA1(df56eb705c7dfaf00b084aa6744d7fd5fa4f47d3) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacfnt )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacfnt.5e",    0x0000, 0x1000, CRC(f2561d07) SHA1(1b971c2a907673dac0bf676f599a8a20fcb4a01b) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacheart )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacheart.6e",  0x0000, 0x0800, CRC(d844b679) SHA1(c4486198b3126bb8e05a308c53787e51065f77ae) )
	ROM_LOAD( "pacheart.6k",  0x0800, 0x0800, CRC(b9152a38) SHA1(b6be2cb6bc7dd123503eb6bf1165dd1c99456813) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "pacheart.6m",  0x1800, 0x0800, CRC(842d6574) SHA1(40e32d09cc8d701eb318716493a68cf3f95d3d6d) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "pacheart.6n",  0x2800, 0x0800, CRC(888f3c3e) SHA1(c2b5917bf13071131dd53ea76f0da86706db2d80) )
	ROM_LOAD( "pacheart.6j",  0x3000, 0x0800, CRC(f5265c10) SHA1(9a320790d7a03fd6192a92d30b3e9c754bbc6a9d) )
	ROM_LOAD( "pacheart.6p",  0x3800, 0x0800, CRC(1a21a381) SHA1(d5367a327d19fb57ba5e484bd4fda1b10953c040) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacheart.5e",  0x0000, 0x0800, CRC(c62bbabf) SHA1(f6f28ae33c2ab274105283b22b49ad243780a95e) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "newpuc2.5f",   0x1000, 0x0800, CRC(ca8c184c) SHA1(833aa845824ed80777b62f03df36a920ad7c3656) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( pacinvis )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacinvis.6j",  0x3000, 0x1000, CRC(1b4e96dc) SHA1(f26207c9b8277078a7c428f35938164ea6326f21) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacjail )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacjail.6j",   0x3000, 0x1000, CRC(cc31f185) SHA1(cd184a55f775bc9a70d0bfa932d2993b8987135b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacjr1 )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacjr1.6j",    0x3000, 0x1000, CRC(fcfb21a3) SHA1(74c103f78cb75cb01ec4b611e341ed3d37fb0b94) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacjr1.5e",    0x0000, 0x1000, CRC(9f3c32d4) SHA1(e7e40207f2a70768ce38cbb1e62c02ca4bed3a14) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( pacjr2 )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacjr2.6j",    0x3000, 0x1000, CRC(cc26c905) SHA1(213adaaa54cd084dbf73bd6d6ed9f9f4e4a03a5f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacjr1.5e",    0x0000, 0x1000, CRC(9f3c32d4) SHA1(e7e40207f2a70768ce38cbb1e62c02ca4bed3a14) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( pacjr3 )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacjr3.6j",    0x3000, 0x1000, CRC(d4d8cb9b) SHA1(06bcf32e5fe9af7161178bd6295a6ea48f791f37) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacjr1.5e",    0x0000, 0x1000, CRC(9f3c32d4) SHA1(e7e40207f2a70768ce38cbb1e62c02ca4bed3a14) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( pacjr4 )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacjr4.6j",    0x3000, 0x1000, CRC(42ca024b) SHA1(567331f4050fc0de5d1fda304fa27556c86968db) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacjr1.5e",    0x0000, 0x1000, CRC(9f3c32d4) SHA1(e7e40207f2a70768ce38cbb1e62c02ca4bed3a14) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( pacm255 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacman25 )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "namcosil.6f",  0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pacman25.3",   0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pacman25.4",   0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "pacman25.5",   0x8000, 0x1000, CRC(19a22304) SHA1(6cc0e4921148b7a7eedf5d807bd63062b745e1c6) )
	ROM_LOAD( "pacman25.6",   0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "pacman25.7",   0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "pacman25.8",   0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )
	/* This colour table is configurable to whatever you want */
	ROM_FILL(0x8fc0, 1, 0x01)	// level 1
	ROM_FILL(0x8fc1, 1, 0x11)	// level 2
	ROM_FILL(0x8fc2, 1, 0x03)
	ROM_FILL(0x8fc3, 1, 0x12)
	ROM_FILL(0x8fc4, 1, 0x09)
	ROM_FILL(0x8fc5, 1, 0x1f)
	ROM_FILL(0x8fc6, 1, 0x0e)
	ROM_FILL(0x8fc7, 1, 0x14)
	ROM_FILL(0x8fc8, 1, 0x15)
	ROM_FILL(0x8fc9, 1, 0x16)
	ROM_FILL(0x8fca, 1, 0x17)
	ROM_FILL(0x8fcb, 1, 0x18)
	ROM_FILL(0x8fcc, 1, 0x10)
	ROM_FILL(0x8fcd, 1, 0x1d)
	ROM_FILL(0x8fce, 1, 0x1f)
	ROM_FILL(0x8fcf, 1, 0x07)	// level 16
	ROM_COPY( "maincpu", 0x8fc0, 0x8fd0, 0x10 )	/* copy the above to levels 17-32 */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman25.5e",  0x0000, 0x1000, CRC(11d47ae1) SHA1(fbd47432f258e56f1323766d5b2844f4a2e6e7ac) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacman3d )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman3d.6f",  0x1000, 0x1000, CRC(96364259) SHA1(ec2f36a44fd0c13dfd7dc571b9499bc9dbaa247d) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman3d.6j",  0x3000, 0x1000, CRC(959e930e) SHA1(0ef24017be05ddf85248c4d00f0e21cbf658f78c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman3d.5e",  0x0000, 0x1000, CRC(aa203d45) SHA1(7d2fb9bc8718601efc33d2469452a08018167e60) )
	ROM_LOAD( "pacman3d.5f",  0x1000, 0x1000, CRC(d1830540) SHA1(5d276774997e3be8fc9c21d55562288acd1cffcc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacman6 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "pacman6.map",  0x8000, 0x1000, CRC(de753fd9) SHA1(467cd6dd572a3e6f0b2e0120d520df919433655a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmulti.5e",  0x0000, 0x1000, CRC(46cdda4a) SHA1(2c38050a4f5d0c9e1b4f3e054dcac25cde63ae2a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmanf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacmanf.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmanfx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacmanf.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

/* This set has the fixes proposed by Don Hodges at the pages:
	http://donhodges.com/how_high_can_you_get2.htm
	http://donhodges.com/pacman_pinky_explanation.htm
	These are: AI fixes for Pinky and Inky; fix for the kill screen on level 256.
	Patches invented by Don Hodges; coded into a ROM by Krick. */
ROM_START( pacmanp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacmanp.6h",   0x2000, 0x1000, CRC(65625778) SHA1(8d29d46cff897a4d89367a3cc02627bcee1ef38e) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmanx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmar )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmar.6j",    0x3000, 0x1000, CRC(b208c7dc) SHA1(d2e199936ab0fc3de80f7c86def50c03da8c1684) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmar.5e",    0x0000, 0x1000, CRC(7ccdfa59) SHA1(ed8e7f2e7b496a6f3246e1cc3966382e748ab1c3) )
	ROM_LOAD( "pacmar.5f",    0x1000, 0x1000, CRC(51af9fc5) SHA1(0fe657bee3a811043663595f7f9e7e05cc71da84) )
	ROM_COPY( "gfx1",    0x1b00, 0x1c00, 0x40 )		/* show mario at start of level */

	PACMAN_PROMS
ROM_END

// extra keys: G = jump ; F = sound fx on/off ; R = music on/off
ROM_START( pacminer )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacminer.6e",  0x0000, 0x1000, CRC(11dee14e) SHA1(1aeb94299a33daa9b51fdab3c0abea98858cc7dd) )
	ROM_LOAD( "pacminer.6f",  0x1000, 0x1000, CRC(8c449bd7) SHA1(84233296683321f71fdef604ca19bfffd97e993c) )
	ROM_LOAD( "pacminer.6h",  0x2000, 0x1000, CRC(a0e62570) SHA1(654a71144eae3c78d7baaeb9119aad0ece500e60) )
	ROM_LOAD( "pacminer.6j",  0x3000, 0x1000, CRC(c71c0011) SHA1(1ceaf73df40e531df3bfb26b4fb7cd95fb7bff1d) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacminer.5e",  0x0000, 0x1000, CRC(9b3cc7cd) SHA1(9bfb71e71f7f61e26d23e6370d684cdc6c910899) )
	ROM_LOAD( "pacminer.5f",  0x1000, 0x1000, CRC(c1b2dc90) SHA1(3a089696bab9f3ff39e09c84117d22b861ddec40) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmini )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmini.6j",   0x3000, 0x1000, CRC(5e04f9c5) SHA1(1423a3698812fcfdb8537a528236278578864afb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmini.5e",   0x0000, 0x1000, CRC(5520b393) SHA1(5775a771b29efa5a8852310c36f1c8f68eec65a8) )
	ROM_LOAD( "pacmini.5f",   0x1000, 0x1000, CRC(30e6024c) SHA1(51c1f9074091c053437a25ef1c6e9ee63e9b5154) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmini2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmini2.6j",  0x3000, 0x1000, CRC(092dd5fa) SHA1(c016da8a22901fc4d441c94a3fecdd60d11f5c97) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmini2.5e",  0x0000, 0x1000, CRC(130eff7b) SHA1(ab29362b827591acb4204639c0e323dabf2bc767) )
	ROM_LOAD( "pacmini.5f",   0x1000, 0x1000, CRC(30e6024c) SHA1(51c1f9074091c053437a25ef1c6e9ee63e9b5154) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmn6m2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "pacmn6m2.map", 0x8000, 0x1000, CRC(aadee235) SHA1(0f8e4c26c2804033409dcdb5032d10fa650edc75) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmn6m2.5e",  0x0000, 0x1000, CRC(60ada878) SHA1(5340d1cb8cf5f7f5fc2bc5e2df4ca0fe209b77e9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmod )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacmanh.6e",   0x0000, 0x1000, CRC(3b2ec270) SHA1(48fc607ad8d86249948aa377c677ae44bb8ad3da) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacmanh.6h",   0x2000, 0x1000, CRC(18811780) SHA1(ab34acaa3dbcafe8b20c2197f36641e471984487) )
	ROM_LOAD( "pacmanh.6j",   0x3000, 0x1000, CRC(5c96a733) SHA1(22ae15a6f088e7296f77c7487a350c4bd102f00e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmanh.5e",   0x0000, 0x1000, CRC(299fb17a) SHA1(ad97adc2122482a9018bacd137df9d8f409ddf85) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmodx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacmanh.6e",   0x0000, 0x1000, CRC(3b2ec270) SHA1(48fc607ad8d86249948aa377c677ae44bb8ad3da) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacmanh.6h",   0x2000, 0x1000, CRC(18811780) SHA1(ab34acaa3dbcafe8b20c2197f36641e471984487) )
	ROM_LOAD( "pacmanh.6j",   0x3000, 0x1000, CRC(5c96a733) SHA1(22ae15a6f088e7296f77c7487a350c4bd102f00e) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmodx.5e",   0x0000, 0x4000, CRC(a05bc552) SHA1(5c2350aeac14cafebfcb7ff012427b1d3c6adf62) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( pacms1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms1.6j",    0x3000, 0x1000, CRC(c5da3887) SHA1(2a65b31891bc19fc0fbdace39e853fff6c4227eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacms2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms2.6j",    0x3000, 0x1000, CRC(b0a107ea) SHA1(86b8b7cb7e90b0de95bc9b8e3b98bcc46992578e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacms3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms3.6j",    0x3000, 0x1000, CRC(c7d8325a) SHA1(bf72a2d7b20d76e521cf45fbe1b0ab762daa8106) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacms4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms4.6j",    0x3000, 0x1000, CRC(7b63ff9b) SHA1(84c167878ca4a1a1d79c397438defc8dcc6326f7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmsa1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa1.6j",   0x3000, 0x1000, CRC(07163b97) SHA1(fdfbca327a2c3f69b4478b1e3ca9c8ed71605582) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmsa2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa2.6j",   0x3000, 0x1000, CRC(15e83c24) SHA1(ebe4d476104bbd037a1b5fe5783c0b77229ca707) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmsa3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa3.6j",   0x3000, 0x1000, CRC(5762f9cf) SHA1(b570468372a9520929513425523fdae734d4acfc) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmsa4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa4.6j",   0x3000, 0x1000, CRC(d9c2a669) SHA1(2f82405d297f35cc13199e1671cfa835dd598e9c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacms1.5e",    0x0000, 0x1000, CRC(7394868b) SHA1(832c44cf91251ebc1f7f6706ca71aa41cedc81e7) )
	ROM_LOAD( "pacms1.5f",    0x1000, 0x1000, CRC(ce1a3264) SHA1(04fb1954ad6d60ddcd515f9c30ece1abfe2455cc) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmulti )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacmulti.6e",  0x0000, 0x1000, CRC(cfb721a8) SHA1(028e508a9c11eab508452ffc644730894adf388c) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacmulti.6h",  0x2000, 0x1000, CRC(e55d5230) SHA1(b764e85871cf8bb96a4ee7d0f3df204adfd4da5e) )
	ROM_LOAD( "pacmulti.6j",  0x3000, 0x1000, CRC(29ba4b53) SHA1(f907641bb583342fa19f02de23639ba730123792) )
	ROM_LOAD( "pacmulti.8",   0x8000, 0x1000, CRC(5a4b59dc) SHA1(09b21036cf40decf4ee00898eb44673599c65b05) )
	ROM_LOAD( "pacmulti.9",   0x9000, 0x1000, CRC(3b939b12) SHA1(40333555af024f505064018a9b76dd284ecbf9a6) )
	ROM_FILL(0x3f83, 3, 0)		/* remove unneeeded subroutine call - this fixes a major bug */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmulti.5e",  0x0000, 0x1000, CRC(46cdda4a) SHA1(2c38050a4f5d0c9e1b4f3e054dcac25cde63ae2a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacplus )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacplus.5e",   0x0000, 0x1000, CRC(022c35da) SHA1(57d7d723c7b029e3415801f4ce83469ec97bb8a1) )
	ROM_LOAD( "pacplus.5f",   0x1000, 0x1000, CRC(4de65cdd) SHA1(9c0699204484be819b77f0b212c792fe9e9fae5d) )

	PACPLUS_PROMS
ROM_END

ROM_START( pacplusc )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplusc.cpu", 0x0000, 0x4000, CRC(a0d25591) SHA1(0c7e1eb93762f9bb487df95c14ea986855529ce3) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacplus.5e",   0x0000, 0x1000, CRC(022c35da) SHA1(57d7d723c7b029e3415801f4ce83469ec97bb8a1) )
	ROM_LOAD( "pacplus.5f",   0x1000, 0x1000, CRC(4de65cdd) SHA1(9c0699204484be819b77f0b212c792fe9e9fae5d) )

	PACPLUS_PROMS
ROM_END

ROM_START( pacplusx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacplusx.5e",  0x0000, 0x4000, CRC(06b08556) SHA1(982329d9568ee1a6edb8a1ed39686a33f65a1411) )
	ROM_LOAD( "pacplusx.5f",  0x4000, 0x4000, CRC(57e9f865) SHA1(e40f5a34b597e32bbc230ae3ad5f32cdbadf2438) )

	PACPLUS_PROMS
ROM_END

ROM_START( pacpopey )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacpopey.5f",  0x1000, 0x1000, CRC(af2b610b) SHA1(c787b1db588125f0770dc2bf4626ad22f0faee72) )

	PACMAN_PROMS
ROM_END

ROM_START( pacrab )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacrab.1",     0x0000, 0x1000, CRC(bb14430f) SHA1(80c632cc611e6cf774cc594a833916296bbbfccc) )
	ROM_LOAD( "pacrab.2",     0x1000, 0x1000, CRC(f3f5a91b) SHA1(38d912661cf95c6234b0d3650c2b6be8df3003f0) )
	ROM_LOAD( "pacrab.3",     0x2000, 0x1000, CRC(44caab2a) SHA1(10aaa38a720266739add8d2c54a4785ed133af0d) )
	ROM_LOAD( "pacrab.4",     0x3000, 0x1000, CRC(25523534) SHA1(e0272e5a8717ef95fd2d593597af4e5011081eca) )
	ROM_LOAD( "pacrab.5",     0x8000, 0x1000, CRC(af3985c4) SHA1(fb2e472b23cea56bf49ba2691a6cdeee4629a1b7) )
	ROM_LOAD( "pacrab.6",     0x9000, 0x1000, CRC(3389fe5a) SHA1(394df6145614083c16cfe606adb0faca8c6a7a09) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacrab.5e",    0x0000, 0x1000, CRC(f33761a4) SHA1(401d752eacb60f2c26aae5cf6079e2bf54a8260a) )
	ROM_LOAD( "pacrab.5f",    0x1000, 0x1000, CRC(60e1a3b7) SHA1(962b307f9fa3f3abde68507914fda78d5560f15b) )

	PACMAN_PROMS
ROM_END

ROM_START( pacshuf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacshuf.6j",   0x3000, 0x1000, CRC(55cc4d87) SHA1(1a05670c4fd09b4155941aa1ea90a5719a3e7ebb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacsnoop )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacsnoop.6e",  0x0000, 0x1000, CRC(FA536307) SHA1(55742123DE6EF87E1DEEF31DC75744B98129AA9E) )
	ROM_LOAD( "pacsnoop.6f",  0x1000, 0x1000, CRC(30EA615C) SHA1(AFB953167AA8060DC60173F3DE065C07FD933CC1) )
	ROM_LOAD( "pacsnoop.6h",  0x2000, 0x1000, CRC(25DA6440) SHA1(93414E82442044324AC5475A45FB124DA70E5D6F) )
	ROM_LOAD( "pacsnoop.6j",  0x3000, 0x1000, CRC(A9594951) SHA1(5C4FA744FDCC3B0EAD7900B079A13297D16A996F) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0C944964) SHA1(06EF227747A440831C9A3A613B76693D52A2F0A9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacspeed )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacspeed.6j",  0x3000, 0x1000, CRC(0e7946d1) SHA1(cea3440a305ec05dab197dba7ac8428db0d5e2f7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacweird )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "pacweird.map", 0x8000, 0x1000, CRC(92fac825) SHA1(05865bc5b84e0c100e358433c7d151c3918c5f19) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacweird.5e",  0x0000, 0x1000, CRC(11077f3e) SHA1(1f7a52a33abfc8650d5d6c685fd51039a467bf72) )
	ROM_LOAD( "pacweird.5f",  0x1000, 0x1000, CRC(7c213ab4) SHA1(fd76db379a32d5102cd2a6afff0bfe9a4afbc3b5) )

	PACMAN_PROMS
ROM_END

ROM_START( pacwf ) // Weird Fruit
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacwf.6j",     0x3000, 0x1000, CRC(a13ce3ea) SHA1(6562ca3e337bd95edb2ebeb6b7fb7e219799e429) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacwf.5e",     0x0000, 0x1000, CRC(f524b277) SHA1(91242ead3d1852241d030975228533086bc3539e) )
	ROM_LOAD( "pacwf.5f",     0x1000, 0x1000, CRC(66ec2c6c) SHA1(b22e2f3ba5a603cea938533dd05fced84d42f8d9) )

	PACMAN_PROMS
ROM_END

ROM_START( pcrunchy )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pcrunchy.6j",  0x3000, 0x1000, CRC(c21c6c52) SHA1(c63212f51c6bf49e76b3ca7553a29e3371c4fa83) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pengman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pengman.6j",   0x3000, 0x1000, CRC(37e7f3ba) SHA1(f9b476d9389fdd8171b443e177e05fae2052024b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pengman.5e",   0x0000, 0x1000, CRC(02f5f5e6) SHA1(029328f78f34330e90ad4407172b318a45017cac) )
	ROM_LOAD( "pengman.5f",   0x1000, 0x1000, CRC(5cd3db2e) SHA1(808655f543fa83762a68e13adcf4a7091d07173f) )

	PACMAN_PROMS
ROM_END

ROM_START( pheartb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuc2.6e",   0x0000, 0x0800, CRC(69496a98) SHA1(2934051d6305cc3654951bc1aacf2b8902f463fe) )
	ROM_LOAD( "newpuc2.6k",   0x0800, 0x0800, CRC(158fc01c) SHA1(2f7a1e24d259fdc716ef8e7354a87780595f3c4e) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "newpuc2.6m",   0x1800, 0x0800, CRC(70810ccf) SHA1(3941678606aab1e53356a6781e24d84e83cc88ce) )
	ROM_LOAD( "pheartb.6h",   0x2000, 0x0800, CRC(b42416d1) SHA1(3714efae24f35fe63dd89f0d7b82e4564c5405d3) )
	ROM_LOAD( "newpuc2.6n",   0x2800, 0x0800, CRC(3f250c58) SHA1(53bf2270c26f10f7e97960cd4c96e09e16b9bdf3) )
	ROM_LOAD( "pheartb.6j",   0x3000, 0x0800, CRC(01d13602) SHA1(dfec0784d005f82a6b2ee55923b510d2daeb9965) )
	ROM_LOAD( "newpuc2.6p",   0x3800, 0x0800, CRC(1f81e765) SHA1(442d8a82e79ae842f1ffb46369c632c1d0b83161) )
	ROM_FILL( 0x3fd5, 1, 0xc9 )		/* allow protection check to work - 0x36 or 0xc9 is ok */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacheart.5e",  0x0000, 0x0800, CRC(c62bbabf) SHA1(f6f28ae33c2ab274105283b22b49ad243780a95e) )
	ROM_LOAD( "newpuc2.5h",   0x0800, 0x0800, CRC(777c70d3) SHA1(ed5ccbeb1102ec9f837577de3aa51317c32520d6) )
	ROM_LOAD( "newpuc2.5f",   0x1000, 0x0800, CRC(ca8c184c) SHA1(833aa845824ed80777b62f03df36a920ad7c3656) )
	ROM_LOAD( "newpuc2.5j",   0x1800, 0x0800, CRC(7dc75a81) SHA1(d3fe1cad3b594052d8367685febb2335b0ad62f4) )

	PACMAN_PROMS
ROM_END

ROM_START( pheartx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacheart.6e",  0x0000, 0x0800, CRC(d844b679) SHA1(c4486198b3126bb8e05a308c53787e51065f77ae) )
	ROM_LOAD( "pacheart.6k",  0x0800, 0x0800, CRC(b9152a38) SHA1(b6be2cb6bc7dd123503eb6bf1165dd1c99456813) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "pacheart.6m",  0x1800, 0x0800, CRC(842d6574) SHA1(40e32d09cc8d701eb318716493a68cf3f95d3d6d) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "pacheart.6n",  0x2800, 0x0800, CRC(888f3c3e) SHA1(c2b5917bf13071131dd53ea76f0da86706db2d80) )
	ROM_LOAD( "pacheart.6j",  0x3000, 0x0800, CRC(f5265c10) SHA1(9a320790d7a03fd6192a92d30b3e9c754bbc6a9d) )
	ROM_LOAD( "pacheart.6p",  0x3800, 0x0800, CRC(1a21a381) SHA1(d5367a327d19fb57ba5e484bd4fda1b10953c040) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pheartx.5e",   0x0000, 0x4000, CRC(868825e6) SHA1(0cbe23ad47ef77f4ddfc0d476704a79701c621f1) )
	ROM_LOAD( "pheartx.5f",   0x4000, 0x4000, CRC(b7d222ba) SHA1(83a6bceff705bc2f7d7766f417ba5a9aa2a9a28e) )

	PACMAN_PROMS
ROM_END

ROM_START( pm1000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm1000.5e",    0x0000, 0x1000, CRC(16d9f7ce) SHA1(a9ac72aa5e3dcaaa2e4d03b6ae372e81d77817f8) )
	ROM_LOAD( "pm1000.5f",    0x1000, 0x1000, CRC(fd06d923) SHA1(a9d61bdcb6841bf07ce8ad08e1be2cf5ef349129) )

	PACMAN_PROMS
ROM_END

ROM_START( pm2000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm2000.5e",    0x0000, 0x1000, CRC(cba39624) SHA1(d815cb5be44b3d29cd1bdd4b8c68484e5330e4a4) )
	ROM_LOAD( "pm2000.5f",    0x1000, 0x1000, CRC(a2b31528) SHA1(69f1c4b34e581c0e6d75b9726cbefa08f8a97fdb) )

	PACMAN_PROMS
ROM_END

ROM_START( pm3000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm3000.5e",    0x0000, 0x1000, CRC(adb41be2) SHA1(7f2f4f3fc2d5f8d7bed9baddb1bb5787e4c5cd9e) )
	ROM_LOAD( "pm3000.5f",    0x1000, 0x1000, CRC(1fac6af1) SHA1(d9e62fb387ec1d02a78cc3b4ba2626f9044a1021) )

	PACMAN_PROMS
ROM_END

ROM_START( pm4000p )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm3000.5e",    0x0000, 0x1000, CRC(adb41be2) SHA1(7f2f4f3fc2d5f8d7bed9baddb1bb5787e4c5cd9e) )
	ROM_LOAD( "pm4000p.5f",   0x1000, 0x1000, CRC(74f6f4d3) SHA1(c65b295881bc531e165996aa365ff23126eab9d1) )

	PACMAN_PROMS
ROM_END

ROM_START( pm5000p )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm5000p.5e",   0x0000, 0x1000, CRC(5865ce98) SHA1(2ddaf119b1674a7c6af3e475ba2e43dcea09f1cd) )
	ROM_LOAD( "pm5000p.5f",   0x1000, 0x1000, CRC(81cf02e1) SHA1(cd1b3f1ac0265ed25e00eb1a2bfb831d5c7d7e8b) )

	PACPLUS_PROMS
ROM_END

ROM_START( pm6000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm6000.5e",    0x0000, 0x1000, CRC(89945690) SHA1(2cee6236da8d20555e02cdebb2fb81c9e7d806ca) )
	ROM_LOAD( "pm6000.5f",    0x1000, 0x1000, CRC(9268eb5a) SHA1(8db0365beeb89f5cb73de212b810938d5cad7971) )

	PACMAN_PROMS
ROM_END

ROM_START( pm7000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm7000.5e",    0x0000, 0x1000, CRC(c181ad5d) SHA1(74874eac512e445206303036e52d5e30e9ac57d0) )
	ROM_LOAD( "pm7000.5f",    0x1000, 0x1000, CRC(bd31be36) SHA1(fa879e1b0bc28d21478337fc799360d15541d763) )

	PACMAN_PROMS
ROM_END

ROM_START( pm7000p )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm7000.5e",    0x0000, 0x1000, CRC(c181ad5d) SHA1(74874eac512e445206303036e52d5e30e9ac57d0) )
	ROM_LOAD( "pm7000p.5f",   0x1000, 0x1000, CRC(3d5861a5) SHA1(43f86248b2980d62bcd3f3ee71625c2c73e21a5a) )

	PACMAN_PROMS
ROM_END
	
ROM_START( pmad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pmad.6h",      0x2000, 0x1000, CRC(dd2eb43d) SHA1(e80ca01ce0e2f5204dcfc95abc0abd180dd7eb58) )
	ROM_LOAD( "pmad.6j",      0x3000, 0x1000, CRC(ff3f4866) SHA1(1f55f6742ee08889ddc135cfdd3366c32c32510d) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmad.5e",      0x0000, 0x1000, CRC(5b794d59) SHA1(413488774cda263a477fe808bcacf76e9f60b542) )
	ROM_LOAD( "pmad.5f",      0x1000, 0x1000, CRC(59a9362c) SHA1(aa7dfa45cb529e7ca4493262090b149e8c523006) )

	PACMAN_PROMS
ROM_END

ROM_START( pmad00 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pmad.6h",      0x2000, 0x1000, CRC(dd2eb43d) SHA1(e80ca01ce0e2f5204dcfc95abc0abd180dd7eb58) )
	ROM_LOAD( "pmad00.6j",    0x3000, 0x1000, CRC(5f1aea94) SHA1(3122bd7e9e1c9123f6b1915b074491e9865d39a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmad00.5e",    0x0000, 0x1000, CRC(315f7131) SHA1(8182e3e177111aabad0058897b80cee8a3fd93c7) )
	ROM_LOAD( "pmad00.5f",    0x1000, 0x1000, CRC(157a6c0f) SHA1(93f66ad4595eb9fcc306398cb83abcacc60b1521) )

	PACMAN_PROMS
ROM_END

ROM_START( pmad6m )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "pmad6m.map",   0x8000, 0x1000, CRC(d6a0d7f2) SHA1(7f3de6e13dd834a1495fb7faf2743fcc71b84747) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "hanglyad.5e",  0x0000, 0x1000, CRC(93fd3682) SHA1(792ddb541aeab304b5facfcf929c3bba18150346) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( pmada )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pmad.6h",      0x2000, 0x1000, CRC(dd2eb43d) SHA1(e80ca01ce0e2f5204dcfc95abc0abd180dd7eb58) )
	ROM_LOAD( "pmada.6j",     0x3000, 0x1000, CRC(24e43519) SHA1(83fe90767bd73b1f72a5a86da79122fd2d8bc282) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacjr1.5e",    0x0000, 0x1000, CRC(9f3c32d4) SHA1(e7e40207f2a70768ce38cbb1e62c02ca4bed3a14) )
	ROM_LOAD( "pacjr1.5f",    0x1000, 0x1000, CRC(c2310808) SHA1(84c28a4fc327afcb441daee35acdab6f7613395e) )

	PACMAN_PROMS
ROM_END

ROM_START( pmba2000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pmba2000.6j",  0x3000, 0x1000, CRC(f7d647d8) SHA1(4edf34fc4a0da7367557392d55beac4a5eb5038a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba2000.5e",  0x0000, 0x1000, CRC(ff6308bb) SHA1(93bc32cfb43d521e77179597141587a9866c5319) )
	ROM_LOAD( "pmba2000.5f",  0x1000, 0x1000, CRC(c8b456b5) SHA1(a9e435300171779a6c0e145a50284045b3996d91) )

	PACPLUS_PROMS
ROM_END

ROM_START( pmba3000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pmba3000.6j",  0x3000, 0x1000, CRC(08f7a148) SHA1(288e04fb264e985fbb7ea2f57ab14edd72b97140) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba3000.5e",  0x0000, 0x1000, CRC(5ebb61c5) SHA1(cfd491170d1d3d0c0632d5d81629e98cfd100fb6) )
	ROM_LOAD( "pmba3000.5f",  0x1000, 0x1000, CRC(d9a055aa) SHA1(00c6867b70062672da3b8c254feff4354e35aa7a) )

	PACMAN_PROMS
ROM_END

ROM_START( pmba4000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba3000.5e",  0x0000, 0x1000, CRC(5ebb61c5) SHA1(cfd491170d1d3d0c0632d5d81629e98cfd100fb6) )
	ROM_LOAD( "pmba3000.5f",  0x1000, 0x1000, CRC(d9a055aa) SHA1(00c6867b70062672da3b8c254feff4354e35aa7a) )

	PACMAN_PROMS
ROM_END

ROM_START( pmba6000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pmba6000.6j",  0x3000, 0x1000, CRC(bdb3d11a) SHA1(d9920ad3b6c76ab9095956b960165477ed1e11f8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba6000.5e",  0x0000, 0x1000, CRC(7961ed4e) SHA1(595311b7a77d1379c9564d42a34581cc35a53875) )
	ROM_LOAD( "pmba6000.5f",  0x1000, 0x1000, CRC(417080fd) SHA1(13e8e812bca25b64e7ce072d86189fd15b1882b0) )

	PACPLUS_PROMS
ROM_END

ROM_START( pmba8000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmba6000.5e",  0x0000, 0x1000, CRC(7961ed4e) SHA1(595311b7a77d1379c9564d42a34581cc35a53875) )
	ROM_LOAD( "pmba6000.5f",  0x1000, 0x1000, CRC(417080fd) SHA1(13e8e812bca25b64e7ce072d86189fd15b1882b0) )

	PACPLUS_PROMS
ROM_END

ROM_START( pmbamaz )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pmbamaz.6j",   0x3000, 0x1000, CRC(bf5e4b89) SHA1(ad16845b92e56a6fe33c7561a231dd54ad558481) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmbamaz.5e",   0x0000, 0x1000, CRC(9a8cd8a1) SHA1(653d3c900b0be5e7ffe424207419bf86229b8c48) )
	ROM_LOAD( "pmbamaz.5f",   0x1000, 0x1000, CRC(13aeaac4) SHA1(2abcc95559bc57a209d7216ddb6488032c4e34d6) )

	PACMAN_PROMS
ROM_END

ROM_START( pmbaplus )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmbaplus.5e",  0x0000, 0x1000, CRC(3e5a31c2) SHA1(82716be9be3a7012a4283c6e9e6be01b655a6526) )
	ROM_LOAD( "pmbaplus.5f",  0x1000, 0x1000, CRC(3c099bbd) SHA1(f589e34330d78de173dc42929b17e4763ed4f1a3) )

	PACMAN_PROMS
ROM_END

ROM_START( pmbluep )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm1000.5e",    0x0000, 0x1000, CRC(16d9f7ce) SHA1(a9ac72aa5e3dcaaa2e4d03b6ae372e81d77817f8) )
	ROM_LOAD( "pmbluep.5f",   0x1000, 0x1000, CRC(2da4dec7) SHA1(1a5c441dad8b3365ef728fe318c63d4e10de4731) )

	PACMAN_PROMS
ROM_END

ROM_START( pmdeluxe )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm2000.5e",    0x0000, 0x1000, CRC(cba39624) SHA1(d815cb5be44b3d29cd1bdd4b8c68484e5330e4a4) )
	ROM_LOAD( "pmdeluxe.5f",  0x1000, 0x1000, CRC(ab172fc9) SHA1(8845c73b37bb117d222eeb01788ba5738aba755a) )

	PACMAN_PROMS
ROM_END

ROM_START( pmextra )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmextra.5e",   0x0000, 0x1000, CRC(3ee34b66) SHA1(2d7a7e9a59dd87be7bda4964471b0fa27e82a219) )
	ROM_LOAD( "pmextra.5f",   0x1000, 0x1000, CRC(edf92a4c) SHA1(63140c7100315edc8fd8a234822f0831e6d1799d) )

	PACMAN_PROMS
ROM_END

ROM_START( pmextrap )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmextra.5e",   0x0000, 0x1000, CRC(3ee34b66) SHA1(2d7a7e9a59dd87be7bda4964471b0fa27e82a219) )
	ROM_LOAD( "pmextrap.5f",  0x1000, 0x1000, CRC(34ff1749) SHA1(55eb0199fdc04a3331bcf1e31baf686310af2cc7) )

	PACMAN_PROMS
ROM_END

ROM_START( pmfever )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmfever.5e",   0x0000, 0x1000, CRC(f9b955e1) SHA1(d8565b6e0ccd37309fc7e1507603efb75eedb211) )
	ROM_LOAD( "pmfever.5f",   0x1000, 0x1000, CRC(8ddf8eb6) SHA1(72842687a8d957de22832b56cf1939864748cd6f) )

	PACMAN_PROMS
ROM_END

ROM_START( pmgrenp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmgrenp.5e",   0x0000, 0x1000, CRC(5807a438) SHA1(8f4160627948747e92c3c5dc386068e4561fd9aa) )
	ROM_LOAD( "pmgrenp.5f",   0x1000, 0x1000, CRC(1545b3c8) SHA1(a60fe521380af755b2f47bd526d523c7f9b9a923) )

	PACPLUS_PROMS
ROM_END

/* Only the program roms were supplied with this dump. As it was listed as a bootleg of pacheart, I've added
   the gfx roms and the proms from that game. */
ROM_START( pmheart )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "newpuck1.6e",  0x0000, 0x0800, CRC(2c0fa0ab) SHA1(37680e4502771ae69d51d07ce43f65b9b2dd2a49) )
	ROM_LOAD( "pmheart.6k",   0x0800, 0x0800, CRC(afeca2f1) SHA1(1e6d6c75eeb3a354ce2dc88da62caf9e7d53d0cb) )
	ROM_LOAD( "pmheart.6f",   0x1000, 0x0800, CRC(6b53ada9) SHA1(a905688b389bfbc6792965d8f3d5bb1b9f0f4ec6) )
	ROM_LOAD( "pmheart.6m",   0x1800, 0x0800, CRC(35f3ca84) SHA1(3da7336caa0742ea79f1e0e8f6b80f8560507a33) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "pmheart.6n",   0x2800, 0x0800, CRC(93f344c5) SHA1(987c7fa18a774a47c045fa1dc7dff37457cb8983) )
	ROM_LOAD( "pmheart.6j",   0x3000, 0x0800, CRC(bed4a077) SHA1(39ac1d4d2acf4752ff7f9839f8f0d1974e023fab) )
	ROM_LOAD( "pmheart.6p",   0x3800, 0x0800, CRC(800be41e) SHA1(6f40e741d95c2cfe1b217f1061da3497b4c2a153) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacheart.5e",  0x0000, 0x0800, CRC(c62bbabf) SHA1(f6f28ae33c2ab274105283b22b49ad243780a95e) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "newpuc2.5f",   0x1000, 0x0800, CRC(ca8c184c) SHA1(833aa845824ed80777b62f03df36a920ad7c3656) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( pplusad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pplusad.5e",   0x0000, 0x1000, CRC(ce684d85) SHA1(d7e40494631b3b1d73f9c07f15b6fa102286aa0d) )
	ROM_LOAD( "pplusad.5f",   0x1000, 0x1000, CRC(0b86d002) SHA1(77c74d8ff6e4bf3de8e87ee872b1a9b6cf10fece) )

	PACPLUS_PROMS
ROM_END

ROM_START( puck255 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "pacn255.6h",   0x2000, 0x1000, CRC(6dac9e8f) SHA1(bbebc8d059755f329ce6c0e4d39c1cd999c60dfe) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( puckman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckman.6e",   0x0000, 0x0800, CRC(f36e88ab) SHA1(813cecf44bf5464b1aed64b36f5047e4c79ba176) )
	ROM_LOAD( "puckman.6k",   0x0800, 0x0800, CRC(618bd9b3) SHA1(b9ca52b63a49ddece768378d331deebbe34fe177) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "puckman.6m",   0x1800, 0x0800, CRC(d3e8914c) SHA1(c2f00e1773c6864435f29c8b7f44f2ef85d227d3) )
	ROM_LOAD( "puckman.6h",   0x2000, 0x0800, CRC(6bf4f625) SHA1(afe72fdfec66c145b53ed865f98734686b26e921) )
	ROM_LOAD( "puckman.6n",   0x2800, 0x0800, CRC(a948ce83) SHA1(08759833f7e0690b2ccae573c929e2a48e5bde7f) )
	ROM_LOAD( "puckman.6j",   0x3000, 0x0800, CRC(b6289b26) SHA1(d249fa9cdde774d5fee7258147cd25fa3f4dc2b3) )
	ROM_LOAD( "puckman.6p",   0x3800, 0x0800, CRC(17a88c13) SHA1(eb462de79f49b7aa8adb0cc6d31535b10550c0ce) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmana )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "puckman.6j",   0x3000, 0x0800, CRC(b6289b26) SHA1(d249fa9cdde774d5fee7258147cd25fa3f4dc2b3) )
	ROM_LOAD( "puckman.6p",   0x3800, 0x0800, CRC(17a88c13) SHA1(eb462de79f49b7aa8adb0cc6d31535b10550c0ce) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmanb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmanf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanf.6f",  0x1000, 0x1000, CRC(51b38db9) SHA1(0a796f93462aec4758c2aa1c1f34cd05bb10a178) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmano )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckmano.5e",  0x0000, 0x1000, CRC(1f44f160) SHA1(d5ec84bf164eb549b80f8a6c25ec9246fc89d525) )
	ROM_LOAD( "puckmano.5f",  0x1000, 0x1000, CRC(c78395ca) SHA1(100507f230c348489b30386f8ff8130b956d2646) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmanx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmnax )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "puckman.6j",   0x3000, 0x0800, CRC(b6289b26) SHA1(d249fa9cdde774d5fee7258147cd25fa3f4dc2b3) )
	ROM_LOAD( "puckman.6p",   0x3800, 0x0800, CRC(17a88c13) SHA1(eb462de79f49b7aa8adb0cc6d31535b10550c0ce) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "puckmnax.5e",  0x0000, 0x4000, CRC(c8d9349e) SHA1(8ca4045fe888b78306017a5e17b180d13b37e436) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmnfx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanf.6f",  0x1000, 0x1000, CRC(51b38db9) SHA1(0a796f93462aec4758c2aa1c1f34cd05bb10a178) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmnhx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "puckmanh.6f",  0x1000, 0x1000, CRC(61d38c6c) SHA1(1406aacdc9c8a3776e5853d214380ad3124408f4) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "puckmanh.6j",  0x3000, 0x1000, CRC(8939ddd2) SHA1(cf769bb34f711cfd0ee75328cd5dc07442f88607) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmodx.5e",   0x0000, 0x4000, CRC(a05bc552) SHA1(5c2350aeac14cafebfcb7ff012427b1d3c6adf62) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmod )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmod.6j",   0x3000, 0x1000, CRC(7d98d5f5) SHA1(39939bcd6fb785d0d06fd29f0287158ab1267dfc) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmodx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmod.6j",   0x3000, 0x1000, CRC(7d98d5f5) SHA1(39939bcd6fb785d0d06fd29f0287158ab1267dfc) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "pacmanx.5e",   0x0000, 0x4000, CRC(a45138ce) SHA1(f18344980b4a58d0abf0cc2b7853184af3d5bfa0) )
	ROM_LOAD( "pacmanx.5f",   0x4000, 0x4000, CRC(92e1b10e) SHA1(bb04988502fa181365c08096b5fd47fa9541c291) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmot1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckmot1.5e",  0x0000, 0x1000, CRC(79ad3ca1) SHA1(a84330e1a7ef1b8c201e35fe66f4480065e0804a) )
	ROM_LOAD( "puckmot1.5f",  0x1000, 0x1000, CRC(bd898502) SHA1(3bf87f82cc2fcaab17dec15f4ad7e780df66b48f) )

	PACMAN_PROMS
ROM_END

ROM_START( puckmoti )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckmanb.6e",  0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "puckmanb.6f",  0x1000, 0x1000, CRC(39d1fc83) SHA1(326dbbf94c6fa2e96613dedb53702f8832b47d59) )
	ROM_LOAD( "puckmanb.6h",  0x2000, 0x1000, CRC(02083b03) SHA1(7e1945f6eb51f2e51806d0439f975f7a2889b9b8) )
	ROM_LOAD( "puckmanb.6j",  0x3000, 0x1000, CRC(7a36fe55) SHA1(01b4c38108d9dc4e48da4f8d685248e1e6821377) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckmoti.5e",  0x0000, 0x1000, CRC(e2a1f6b6) SHA1(798daee244fc4275f7ed0bd3a99b527531960807) )
	ROM_LOAD( "puckmoti.5f",  0x1000, 0x1000, CRC(31d24bb5) SHA1(ff26f2199e28f9bbbeca09acf3850b9e76c597da) )

	PACMAN_PROMS
ROM_END

ROM_START( puckren )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckren.6e",   0x0000, 0x1000, CRC(0f5ecf67) SHA1(1250eb19c721b007705c75cce9140365aeed3fad) )
	ROM_LOAD( "puckren.6f",   0x1000, 0x1000, CRC(70ea1149) SHA1(4c1f25e7e6fb7645b7a23419cd1c5b3e2cf90823) )
	ROM_LOAD( "puckren.6h",   0x2000, 0x1000, CRC(c6bb6ea6) SHA1(a33e23193c14d2418614ad7609fb3bc0856a4a1f) )
	ROM_LOAD( "puckren.6j",   0x3000, 0x1000, CRC(dc90f1dc) SHA1(af677ed5989abaebcbde5da9168e849135d5e96f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckren.5e",   0x0000, 0x1000, CRC(a502e25f) SHA1(9f02d3f1ec095910642fe0bb616d510be52b53d8) )
	ROM_LOAD( "puckren.5f",   0x1000, 0x1000, CRC(18d44098) SHA1(d07120b9ede3cfcbc69006381f4287bede04c6e3) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "puckren.7f",   0x0000, 0x0020, CRC(7b6aa3c9) SHA1(849911abdc26ccf2ad44e2c432e6219447b8bef3) )
	ROM_LOAD( "puckren.4a",   0x0020, 0x0100, CRC(64fe1dbf) SHA1(7f7541eb12148add79ad186a1da375c865d251e0) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( puckrenc )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckren.6e",   0x0000, 0x1000, CRC(0f5ecf67) SHA1(1250eb19c721b007705c75cce9140365aeed3fad) )
	ROM_LOAD( "puckrenc.6f",  0x1000, 0x1000, CRC(8a75a7e2) SHA1(79f787c8c55ce04c97baf465b64d89d2bdbf07c5) )
	ROM_LOAD( "puckren.6h",   0x2000, 0x1000, CRC(c6bb6ea6) SHA1(a33e23193c14d2418614ad7609fb3bc0856a4a1f) )
	ROM_LOAD( "puckren.6j",   0x3000, 0x1000, CRC(dc90f1dc) SHA1(af677ed5989abaebcbde5da9168e849135d5e96f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckren.5e",   0x0000, 0x1000, CRC(a502e25f) SHA1(9f02d3f1ec095910642fe0bb616d510be52b53d8) )
	ROM_LOAD( "puckren.5f",   0x1000, 0x1000, CRC(18d44098) SHA1(d07120b9ede3cfcbc69006381f4287bede04c6e3) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "puckren.7f",   0x0000, 0x0020, CRC(7b6aa3c9) SHA1(849911abdc26ccf2ad44e2c432e6219447b8bef3) )
	ROM_LOAD( "puckren.4a",   0x0020, 0x0100, CRC(64fe1dbf) SHA1(7f7541eb12148add79ad186a1da375c865d251e0) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( punleash )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "punleash.1",   0x0000, 0x1000, CRC(eddb20e7) SHA1(07f6cabac3c59c2e3f522da877e845b3ceb2cffb) )
	ROM_LOAD( "punleash.2",   0x1000, 0x1000, CRC(57fe8b4d) SHA1(629800fe793b53c27ee8c84a2e49caf7fe056f28) )
	ROM_LOAD( "punleash.3",   0x2000, 0x1000, CRC(94c63bb1) SHA1(872dbca70b45285dccfd0e24cb50225a5c3ac005) )
	ROM_LOAD( "punleash.4",   0x3000, 0x1000, CRC(fe7734d5) SHA1(f246baab82ea8e9b3cfd3dc5911445ab08223c1f) )
	ROM_LOAD( "punleash.5",   0x8000, 0x1000, CRC(ef155ffb) SHA1(c5bcd57c2a336fa8a018b192b8542954271f1bac) )
	ROM_LOAD( "punleash.6",   0x9000, 0x1000, CRC(70d15899) SHA1(9a7cff5811b591211eb23cee125fd77263f8d4fe) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "punleash.5e",  0x0000, 0x1000, CRC(713a4ba4) SHA1(f6eaadf92dae3a687fb317f2806b4d7a88f7c385) )
	ROM_LOAD( "punleash.5f",  0x1000, 0x1000, CRC(98d3d364) SHA1(f8023d1772d377d090a9bfebf194bbbce8643f1d) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "punleash.7f",  0x0000, 0x0020, CRC(357c2523) SHA1(f0c4cea06b17932cc15f6db2971406eaba3558b2) )
	ROM_LOAD( "punleash.4a",  0x0020, 0x0100, CRC(5ff3b85a) SHA1(6b01b9c9302ab41fe83c140c3ff217d1d66773dd) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( roboman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "roboman.6j",   0x3000, 0x1000, CRC(a909673e) SHA1(991d0c50087f5d47f85f6c16c667dea3b3973f4f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "roboman.5e",   0x0000, 0x1000, CRC(e386213e) SHA1(fbbc1d85cc61dc3a13cc5e26182d51b2bbdfcb63) )
	ROM_LOAD( "roboman.5f",   0x1000, 0x1000, CRC(d8db5788) SHA1(11698932aca1150ef00801fdcb35a5ee2cb3d3b8) )

	PACMAN_PROMS
ROM_END

ROM_START( snakeyes )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "snakeyes.map", 0x8000, 0x1000, CRC(7e506e78) SHA1(a198158a69d79d1c46149c9f8878ba220a6bf82e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmulti.5e",  0x0000, 0x1000, CRC(46cdda4a) SHA1(2c38050a4f5d0c9e1b4f3e054dcac25cde63ae2a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( snowpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "snowpac.6j",   0x3000, 0x1000, CRC(4f9e1d02) SHA1(90f7ed914e1291248f6b3ba0630a7d67d4f07374) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "snowpac.5e",   0x0000, 0x1000, CRC(2cc60a94) SHA1(05b8725cc71273e2ee61303bc00057dc60f88bb6) )
	ROM_LOAD( "snowpac.5f",   0x1000, 0x1000, CRC(41ba87bd) SHA1(3a1473bd3753fc3a3ab6ee5285e4832b0f92d173) )

	PACMAN_PROMS
ROM_END

/* This is a bootleg of Hangly set 3. The proms were missing and the gfx didn't match up (has hearts and said
   8000 points for eating the last monster when you only get 1600). Therefore, using the hangly3 gfx roms.
   Despite the name "Super Pacman", it is just another puckman clone. The first monster is named "Mario". */
ROM_START( spuckman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "hangly3.6k",	  0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "pheartb.6h",   0x2000, 0x0800, CRC(b42416d1) SHA1(3714efae24f35fe63dd89f0d7b82e4564c5405d3) )
	ROM_LOAD( "hangly3.6n",   0x2800, 0x0800, CRC(ab74b51f) SHA1(1bce8933ed7807eb7aca9670df8994f8d1a8b5b7) )
	ROM_LOAD( "spuckman.6j",  0x3000, 0x0800, CRC(bc33aa79) SHA1(b974755edebb19455efccd56412995e3fa9fbd8e) )
	ROM_LOAD( "spuckman.6p",  0x3800, 0x0800, CRC(e95eafca) SHA1(0a0080340a2beb6408fe79a19e7fa97f35de9619) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	/* gfx from hangly3 */
	ROM_LOAD( "hangly3.5e",   0x0000, 0x0800, CRC(5f4be3cc) SHA1(eeb0e1e44549b99eab481d9ac016b4359e19fe30) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( sumeltob )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "sumeltob.6j",  0x3000, 0x1000, CRC(03d59d0e) SHA1(354cfe283b34f40b913589df9bc415fff08bb796) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "sumelton.5e",  0x0000, 0x1000, CRC(10894f38) SHA1(83fac1d9e6d54697a93a0eadb4a66a813e4bd32a) )
	ROM_LOAD( "sumelton.5f",  0x1000, 0x1000, CRC(debef8a9) SHA1(503005bd563099094ad8815f8651cbdcd616571f) )

	PACMAN_PROMS
ROM_END

ROM_START( sumelton )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "sumelton.6j",  0x3000, 0x1000, CRC(c1fe2c7d) SHA1(8eabe4c30dfd1db9b38f8557f136894ee3865f15) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "sumelton.5e",  0x0000, 0x1000, CRC(10894f38) SHA1(83fac1d9e6d54697a93a0eadb4a66a813e4bd32a) )
	ROM_LOAD( "sumelton.5f",  0x1000, 0x1000, CRC(debef8a9) SHA1(503005bd563099094ad8815f8651cbdcd616571f) )

	PACMAN_PROMS
ROM_END

ROM_START( tbone )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "tbone.map",    0x8000, 0x1000, CRC(e7377f2d) SHA1(3cffb221655b1c7240a97ec82132a4a1a76d383e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "tbone.5e",     0x0000, 0x1000, CRC(afd44c48) SHA1(36750fbaeb8db4143fdd90aec83189fc552b110e) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( ultra2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "ultra2.6j",    0x3000, 0x1000, CRC(83da903e) SHA1(541cda1dad90cfbf5c9aa5dd3091337527236bc0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( ultrapac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "ultrapac.6j",  0x3000, 0x1000, CRC(84c6c23b) SHA1(f207ce54d7ee31d61397f1587292ea6add56cb04) )
	ROM_LOAD( "ultrapac.map", 0x8000, 0x1000, CRC(be212894) SHA1(eaf4d33d1e2ebd14d2c9dd3445666c1319ba7807) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ultrapac.5e",  0x0000, 0x1000, CRC(d46efbcf) SHA1(1d90cca40f5b0518cf39408e87d9faf42071e6f9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( vcrunchy )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pcrunchy.6j",  0x3000, 0x1000, CRC(c21c6c52) SHA1(c63212f51c6bf49e76b3ca7553a29e3371c4fa83) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vecbaby )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacbaby.6j",   0x3000, 0x1000, CRC(4e4ac798) SHA1(deab2d93a678abee576c891bdfb7ab93d5718792) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vecbaby2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacbaby2.6j",  0x3000, 0x1000, CRC(742453ba) SHA1(21707992ea3489c8bc95b14db33f62b6aaa24eb4) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vecbaby3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacbaby3.6j",  0x3000, 0x1000, CRC(b82f2b73) SHA1(63619220b9ed4dfeb4f462b41d9685870716590a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vecpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vectplus )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacplus.6e",   0x0000, 0x1000, CRC(d611ef68) SHA1(8531c54ca6b0de0ea4ccc34e0e801ba9847e75bc) )
	ROM_LOAD( "pacplus.6f",   0x1000, 0x1000, CRC(c7207556) SHA1(8ba97215bdb75f0e70eb8d3223847efe4dc4fb48) )
	ROM_LOAD( "pacplus.6h",   0x2000, 0x1000, CRC(ae379430) SHA1(4e8613d51a80cf106f883db79685e1e22541da45) )
	ROM_LOAD( "pacplus.6j",   0x3000, 0x1000, CRC(5a6dff7b) SHA1(b956ae5d66683aab74b90469ad36b5bb361d677e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vectplus.5e",  0x0000, 0x1000, CRC(330abcc6) SHA1(421eb2efe402572189621e2fac49df80bd891f2c) )
	ROM_LOAD( "vectplus.5f",  0x1000, 0x1000, CRC(61da12cf) SHA1(03bc75e3344b863e460fe74aa3b9be36e17d1149) )

	PACPLUS_PROMS
ROM_END

ROM_START( vectr6m )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "pacman6.map",  0x8000, 0x1000, CRC(de753fd9) SHA1(467cd6dd572a3e6f0b2e0120d520df919433655a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vectr6m.5e",   0x0000, 0x1000, CRC(f7be09ba) SHA1(1ac6f67637831a39979866b684898228fb7a9761) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vectr6tb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman6.6h",   0x2000, 0x1000, CRC(aee79ea1) SHA1(4f3214b56056c3ac00c0535ce5d6a367ccda6e31) )
	ROM_LOAD( "pacman6.6j",   0x3000, 0x1000, CRC(581ae70b) SHA1(a184c9d9b6ffbaba936867aef14e783234bf7af2) )
	ROM_LOAD( "vectr6tb.map", 0x8000, 0x1000, CRC(bdbbb207) SHA1(2beceac64d53f197b533375da255a4158e76d970) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vectr6m.5e",   0x0000, 0x1000, CRC(f7be09ba) SHA1(1ac6f67637831a39979866b684898228fb7a9761) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vectxens )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "xensrev.6j",   0x3000, 0x1000, CRC(64a10b6c) SHA1(689be23289fb32f2bfa1392edaba9c6bfa5a82e7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vhangly )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "hangly.6f",    0x1000, 0x1000, CRC(73726586) SHA1(cedddc5194589039dd8b64f07ab6320d7d4f55f9) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "hangly.6j",    0x3000, 0x1000, CRC(7f4147e6) SHA1(0a7ac0e59d4d26fe52a2f4196c9f19e5ab677c87) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

/* This set came from Gatinho in October 2008. */
ROM_START( vhangly3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly3.6e",   0x0000, 0x0800, CRC(9d027c4a) SHA1(88e094880057451a75cdc2ce9477403021813982) )
	ROM_LOAD( "hangly3.6k",	  0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
	ROM_LOAD( "hangly2.6f",   0x1000, 0x0800, CRC(5ba228bb) SHA1(b0e902cdf98bee72d6ec8069eec96adce3245074) )
	ROM_LOAD( "hangly2.6m",   0x1800, 0x0800, CRC(baf5461e) SHA1(754586a6449fd54a342f260e572c1cd60ab70815) )
	ROM_LOAD( "hangly3.6h",   0x2000, 0x0800, CRC(08419c4a) SHA1(7e5001adad401080c788737c1d2349f218750442) )
	ROM_LOAD( "hangly3.6n",   0x2800, 0x0800, CRC(ab74b51f) SHA1(1bce8933ed7807eb7aca9670df8994f8d1a8b5b7) )
	ROM_LOAD( "hangly3.6j",   0x3000, 0x0800, CRC(5039b082) SHA1(086a6ac4742734167d283b1121fce29d8ac4a6cd) )
	ROM_LOAD( "hangly3.6p",   0x3800, 0x0800, CRC(931770d7) SHA1(78fcf88e07ec5126c12c3297b62ca388809e947c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vhangly3.5e",  0x0000, 0x0800, CRC(6e0f31d5) SHA1(edf4674f511a61827d928cff3ea996f67814b281) )
	ROM_LOAD( "vhangly3.5h",  0x0800, 0x0800, CRC(d602128c) SHA1(cff082f5911688767d3d7cc828bb13714daeb19a) )
	ROM_LOAD( "vhangly3.5f",  0x1000, 0x0800, CRC(bff9b303) SHA1(863d155a54fb67864be3f2a0fe339815036bccf3) )
	ROM_LOAD( "vhangly3.5j",  0x1800, 0x0800, CRC(b9724ce8) SHA1(ef4320a572d69f09821d180a249bdd06af38ca63) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacbell )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacbell.6j",   0x3000, 0x1000, CRC(4b428215) SHA1(acdcc4d7ccfc58c6c0473e8ee548b5e02dcafcfd) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacelec )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacelec.6j",   0x3000, 0x1000, CRC(9e5c763d) SHA1(df56eb705c7dfaf00b084aa6744d7fd5fa4f47d3) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacjail )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacjail.6j",   0x3000, 0x1000, CRC(cc31f185) SHA1(cd184a55f775bc9a70d0bfa932d2993b8987135b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacms1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms1.6j",    0x3000, 0x1000, CRC(c5da3887) SHA1(2a65b31891bc19fc0fbdace39e853fff6c4227eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacms2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms2.6j",    0x3000, 0x1000, CRC(b0a107ea) SHA1(86b8b7cb7e90b0de95bc9b8e3b98bcc46992578e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacms3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms3.6j",    0x3000, 0x1000, CRC(c7d8325a) SHA1(bf72a2d7b20d76e521cf45fbe1b0ab762daa8106) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacms4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacms4.6j",    0x3000, 0x1000, CRC(7b63ff9b) SHA1(84c167878ca4a1a1d79c397438defc8dcc6326f7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacmsa1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa1.6j",   0x3000, 0x1000, CRC(07163b97) SHA1(fdfbca327a2c3f69b4478b1e3ca9c8ed71605582) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacmsa2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa2.6j",   0x3000, 0x1000, CRC(15e83c24) SHA1(ebe4d476104bbd037a1b5fe5783c0b77229ca707) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacmsa3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa3.6j",   0x3000, 0x1000, CRC(5762f9cf) SHA1(b570468372a9520929513425523fdae734d4acfc) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacmsa4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmsa4.6j",   0x3000, 0x1000, CRC(d9c2a669) SHA1(2f82405d297f35cc13199e1671cfa835dd598e9c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vpacms1.5e",   0x0000, 0x1000, CRC(035f0597) SHA1(50db2cd5dc99abdb90dd2ce8dc4691e7443ac2a5) )
	ROM_LOAD( "vpacms1.5f",   0x1000, 0x1000, CRC(306b24f0) SHA1(9fd92b05c8bf6749d78f68deaf370582dafcc92e) )

	PACMAN_PROMS
ROM_END

ROM_START( vpacshuf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacshuf.6j",   0x3000, 0x1000, CRC(55cc4d87) SHA1(1a05670c4fd09b4155941aa1ea90a5719a3e7ebb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",   0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",   0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( vpspeed )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacm255.6h",   0x2000, 0x1000, CRC(d3640977) SHA1(2cc05c16c0cf6f516c69ab0b6e1a83bdd3010798) )
	ROM_LOAD( "pacspeed.6j",  0x3000, 0x1000, CRC(0e7946d1) SHA1(cea3440a305ec05dab197dba7ac8428db0d5e2f7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "vecpac.5e",    0x0000, 0x1000, CRC(936d0475) SHA1(8db1fc56ed236deda01cb6a5169fb7d4cda53e7a) )
	ROM_LOAD( "vecpac.5f",    0x1000, 0x1000, CRC(022d0686) SHA1(ea64d134ececc6eb3fc373821318188489b0e7cc) )

	PACMAN_PROMS
ROM_END

ROM_START( xensad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "xensrev.6j",   0x3000, 0x1000, CRC(64a10b6c) SHA1(689be23289fb32f2bfa1392edaba9c6bfa5a82e7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pmad00.5e",    0x0000, 0x1000, CRC(315f7131) SHA1(8182e3e177111aabad0058897b80cee8a3fd93c7) )
	ROM_LOAD( "pmad00.5f",    0x1000, 0x1000, CRC(157a6c0f) SHA1(93f66ad4595eb9fcc306398cb83abcacc60b1521) )

	PACMAN_PROMS
ROM_END

ROM_START( xensrev )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "xensrev.6j",   0x3000, 0x1000, CRC(64a10b6c) SHA1(689be23289fb32f2bfa1392edaba9c6bfa5a82e7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "xensrev.5e",   0x0000, 0x1000, CRC(40e3522d) SHA1(461d1895f82502538bf120a61a3950970f9f1b09) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

/*************************************
 *
 *  Game drivers
 *
 *************************************/

/* in my opinion, the true original game */
GAME( 1980, puckman,  0,        pacman,   pacman,   driver_device, 0,        ROT90, "Namco", "Puckman (original)", GAME_SUPPORTS_SAVE )

/* Official MAME */

/*          rom       parent    machine   inp       init */
GAME( 1982, ctrpllrp, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Caterpillar Pacman Hack", GAME_SUPPORTS_SAVE )
GAME( 1981, hangly,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Hangly-Man (set 1)", GAME_SUPPORTS_SAVE )
GAME( 1981, hangly2,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Hangly-Man (set 2)", GAME_SUPPORTS_SAVE )
GAME( 1981, hangly3,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Hangly-Man (set 3)", GAME_SUPPORTS_SAVE )
GAME( 1982, joyman,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Joyman", GAME_SUPPORTS_SAVE )
GAME( 1980, newpuc2,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Newpuc2", GAME_SUPPORTS_SAVE )
GAME( 1980, newpuc2b, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Newpuc2 (set 2)", GAME_SUPPORTS_SAVE )
GAME( 1980, newpuckx, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "New Puck-X", GAME_SUPPORTS_SAVE )
GAME( 1981, pacheart, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "hack", "Pacman (Hearts)", GAME_SUPPORTS_SAVE )
GAME( 1980, pacman,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Namco] (Midway license)", "Pacman (Midway)", GAME_SUPPORTS_SAVE )
GAME( 1980, pacmanf,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Namco] (Midway license)", "Pacman (Midway, with speedup hack)", GAME_SUPPORTS_SAVE )
GAME( 1981, pacmod,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Namco] (Midway license)", "Pacman (Midway, harder)", GAME_SUPPORTS_SAVE )
GAME( 1982, pacplus,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "[Namco] (Midway license)", "Pacman Plus", GAME_SUPPORTS_SAVE )
GAME( 1981, puckmod,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 3)", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmanb, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 1, Probably Bootleg)", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmana, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 2)", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmanf, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 1 with speedup hack)", GAME_SUPPORTS_SAVE )

/* Blue Justice's Hacks */

GAME( 2001, hm1000,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man 1000", GAME_SUPPORTS_SAVE )
GAME( 2001, hm2000,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man 2000", GAME_SUPPORTS_SAVE )
GAME( 2001, hmba5000, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man Babies 5000", GAME_SUPPORTS_SAVE )
GAME( 2001, hmba7000, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man Babies 7000", GAME_SUPPORTS_SAVE )
GAME( 2001, hmbabymz, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man BabiesMaze", GAME_SUPPORTS_SAVE )
GAME( 2001, hmbluep,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man Blue Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, hmgrenp,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man Green Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, hmplus,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Hangly Man Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pm1000,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man 1000", GAME_SUPPORTS_SAVE )
GAME( 2001, pm2000,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man 2000", GAME_SUPPORTS_SAVE )
GAME( 2001, pm3000,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man 3000", GAME_SUPPORTS_SAVE )
GAME( 2001, pm4000p,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man 4000 Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pm5000p,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man 5000 Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pm6000,   puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man 6000", GAME_SUPPORTS_SAVE )
GAME( 2001, pm7000,   puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man 7000", GAME_SUPPORTS_SAVE )
GAME( 2001, pm7000p,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man 7000 Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pmba2000, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man Babies 2000", GAME_SUPPORTS_SAVE )
GAME( 2001, pmba3000, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man Babies 3000", GAME_SUPPORTS_SAVE )
GAME( 2001, pmba4000, puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Babies 4000", GAME_SUPPORTS_SAVE )
GAME( 2001, pmba6000, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man Babies 6000", GAME_SUPPORTS_SAVE )
GAME( 2001, pmba8000, puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Babies 8000", GAME_SUPPORTS_SAVE )
GAME( 2001, pmbamaz,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Blue Justice", "Pac Man Babies Maze", GAME_SUPPORTS_SAVE )
GAME( 2001, pmbaplus, puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Babies Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pmbluep,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Blue Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pmdeluxe, puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Deluxe", GAME_SUPPORTS_SAVE )
GAME( 2001, pmextra,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Extra", GAME_SUPPORTS_SAVE )
GAME( 2001, pmextrap, puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Extra Plus", GAME_SUPPORTS_SAVE )
GAME( 2001, pmfever,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Fever", GAME_SUPPORTS_SAVE )
GAME( 2001, pmgrenp,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "Blue Justice", "Pac Man Green Plus", GAME_SUPPORTS_SAVE )

/* T-Bone Hacks */

GAME( 2000, baby2,    puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "T-Bone", "Baby Pacman 2 (Alt)", GAME_SUPPORTS_SAVE )
GAME( 2000, baby3,    puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "T-Bone", "Baby Pacman 3 (Alt)", GAME_SUPPORTS_SAVE )
GAME( 2000, baby4,    puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "T-Bone", "Pacman (Baby Maze 4)", GAME_SUPPORTS_SAVE )
GAME( 2000, pmad6m,   puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "T-Bone", "Pacman 2000 After Dark", GAME_SUPPORTS_SAVE )
GAME( 2000, snakeyes, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "T-Bone", "Pacman 2000 (Snake Eyes Mazes)", GAME_SUPPORTS_SAVE )
GAME( 2000, snowpac,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "T-Bone", "Snowy Day Pacman", GAME_SUPPORTS_SAVE )
GAME( 2000, tbone,    puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "T-Bone", "Pacman 2000 (T-Bone Mazes)", GAME_SUPPORTS_SAVE )
GAME( 2000, vectr6m,  puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "T-Bone", "Vector Pacman 2000", GAME_SUPPORTS_SAVE )
GAME( 2000, vectr6tb, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "T-Bone", "Vector Pacman 2000 (T-Bone Mazes)", GAME_SUPPORTS_SAVE )

/* Jerronimo's Progs - www.umlautllama.com */

GAME( 2000, brakman,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Scott Lawrence", "Brakman", GAME_SUPPORTS_SAVE ) // http://www.csh.rit.edu/~jerry/arcade/age/
GAME( 2004, pacman25, puckman,  woodpek,  pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "25 MAP PAC [c]", GAME_SUPPORTS_SAVE )
GAME( 2004, pacsnoop, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "Pac Snoop v1.3 [f]", GAME_SUPPORTS_SAVE )
GAME( 2000, pengman,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Scott Lawrence", "Pengo Man", GAME_SUPPORTS_SAVE ) // http://www.csh.rit.edu/~jerry/arcade/age/

/* Medragon's Hacks */

GAME( 2000, crazypac, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Tim Appleton", "Crazy Pac", GAME_SUPPORTS_SAVE )
GAME( 2000, dizzy,    puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Tim Appleton", "Dizzy Ghost - A Reversal of Roles", GAME_SUPPORTS_SAVE )
GAME( 2000, pacfnt,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Tim Appleton", "Pacman with Pacman font", GAME_SUPPORTS_SAVE )
GAME( 2000, pacwf,    puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Tim Appleton", "Pacman (Weird Fruit)", GAME_SUPPORTS_SAVE )

/* Sil's Hacks */

GAME( 1998, chtpac3d, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "Marcel Silvius", "Pacman 3D [c]", GAME_SUPPORTS_SAVE )
GAME( 2000, eltonpac, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "Elton Pac", GAME_SUPPORTS_SAVE )
GAME( 2000, mazeman,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "Maze Man", GAME_SUPPORTS_SAVE )
GAME( 1998, namcosil, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "Marcel Silvius", "Pacman (Namco) (Sil hack) [c]", GAME_SUPPORTS_SAVE )
GAME( 1998, newpuck3, puckman,	pacman,   pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "New Puck-3", GAME_SUPPORTS_SAVE )
GAME( 1998, pacman3d, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "Pacman 3D", GAME_SUPPORTS_SAVE )
GAME( 2000, pacman6,  puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "Pacman 6", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmn6m2, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "Pacman 2000 (Set 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmulti, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "PacMulti (Pacman)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacweird, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "Marcel Silvius", "Pacman (Six Map Weird)", GAME_SUPPORTS_SAVE )

/* Bobby Tribble's Hacks */

GAME( 2000, pac2600,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Bobby Tribble", "Pac 2600", GAME_SUPPORTS_SAVE )
GAME( 19??, pac2600a, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Bobby Tribble?","Pac 2600 (Older?)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmini,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Bobby Tribble", "Mini Pacman", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmini2, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Bobby Tribble", "Mini Pacman 2", GAME_SUPPORTS_SAVE )

/* Hires hacks */

GAME( 1981, hanglyx,  puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Hangly-Man (set 1) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1981, hangly2x, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Hangly-Man (set 2) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1981, hangly3x, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Hangly-Man (set 3) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, newpuc2x, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Newpuc2 (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, nwpuc2bx, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Newpuc2 (set 2) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, nwpuckxx, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "New Puck-X (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, pacmanx,  puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "[Namco] (Midway license)", "Pac-Man (Midway) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, pacmanfx, puckmanx, pacmanx,  pacman0,  driver_device, 0,        ROT90, "[Namco] (Midway license)", "Pac-Man (Midway) (hires hack) [f]", GAME_SUPPORTS_SAVE )
GAME( 1981, pacmodx,  puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "[Namco] (Midway license)", "Pac-Man (Midway, harder) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1982, pacplusx, puckmanx, pacmanx,  pacman,   pacman_state,  pacplus,  ROT90, "[Namco] (Midway license)", "Pac-Man Plus (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1981, pheartx,  puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Pac-Man (Hearts) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmanx, 0,        pacmanx,  pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 1, Probably Bootleg) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmnax, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 2) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmnfx, puckmanx, pacmanx,  pacman0,  driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 1) (hires hack) [f]", GAME_SUPPORTS_SAVE )
GAME( 1980, puckmnhx, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "hack", "Puckman (Falcom?) (hires hack)", GAME_SUPPORTS_SAVE )
GAME( 1981, puckmodx, puckmanx, pacmanx,  pacman,   driver_device, 0,        ROT90, "Namco", "PuckMan (Japan set 3) (hires hack)", GAME_SUPPORTS_SAVE )

/* Misc Pacman Hacks */

GAME( 2001, abortman, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Paul Copeland",  "Abortman", GAME_SUPPORTS_SAVE )
GAME( 1981, bucaneer, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Bucco", "Bucaneer", GAME_SUPPORTS_SAVE )
GAME( 2000, caterpil, puckman,  pacman,   mspacman, driver_device, 0,        ROT90, "Phi", "Caterpillar", GAME_SUPPORTS_SAVE )
GAME( 2000, chtpac,   puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "[Midway]", "Pacman [c]", GAME_SUPPORTS_SAVE )
GAME( 2000, chtpman2, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "[Namco]", "New Puck2 [c]", GAME_SUPPORTS_SAVE )
GAME( 2000, chtpop,   puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "[Midway]", "Pac-man (Popeye) [c]", GAME_SUPPORTS_SAVE )
GAME( 2000, chtpuck,  puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "Deluxe", "New Puck-X [c]", GAME_SUPPORTS_SAVE )
GAME( 2000, europac,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Stefano Priore", "Euro Pac", GAME_SUPPORTS_SAVE )
GAME( 2000, fasthang, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "[Nittoh]", "Hangly Man [f]", GAME_SUPPORTS_SAVE )
GAME( 2000, fastpop,  puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "[Midway]", "Pacman (Popeye) [f]", GAME_SUPPORTS_SAVE )
GAME( 2000, fastpuck, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "Deluxe", "New Puck-X [f]", GAME_SUPPORTS_SAVE )
GAME( 2000, fpnleash, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "Peter Storey", "Pacman Unleashed (Flat)", GAME_SUPPORTS_SAVE )
GAME( 2000, fstpman2, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "[Namco]", "New Puck2 [f]", GAME_SUPPORTS_SAVE )
GAME( 2000, hanglyad, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Nittoh]", "Hangly-Man (set 1) After Dark", GAME_SUPPORTS_SAVE )
GAME( 2000, hearts,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Hearts)", GAME_SUPPORTS_SAVE )
GAME( 2014, hellowp,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "cmonkey", "Hello World (Pacman HW)", GAME_SUPPORTS_SAVE )
GAME( 1981, hmhearts, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Nittoh]", "Hangly-Man (Hearts)", GAME_SUPPORTS_SAVE )
GAME( 1981, hmpopey,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Nittoh]", "Hangly-Man (Popeye)", GAME_SUPPORTS_SAVE )
GAME( 2005, homercid, puckman,  pacman,   mspacpls, driver_device, 0,        ROT90, "David Widel", "Homercide", GAME_SUPPORTS_SAVE )
GAME( 2001, jacman,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Brent Cobb", "Jacman", GAME_SUPPORTS_SAVE )
GAME( 1980, jpmmunch, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "JPM", "Muncher", GAME_SUPPORTS_SAVE )
GAME( 1983, newpuc1,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "E.P.", "Newpuc1 (Italian dump)", GAME_SUPPORTS_SAVE )
GAME( 1998, newpuck2, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Sir Scotty", "New Puck-2", GAME_SUPPORTS_SAVE )
GAME( 1981, nwpuc2b,  puckman,  pacman,   newpuc2b, driver_device, 0,        ROT90, "Linear Elect", "New Puc2 (Set 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacbaby,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Baby Maze 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacbaby2, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Baby Maze 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacbaby3, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Baby Maze 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacbell,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Pacman (Bell)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacelec,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Pacman (Electric Cowboy)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacinvis, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Original Inviso)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacjail,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Pacman (Jail)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacjr1,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman Jr. (Maze 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacjr2,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman Jr. (Maze 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacjr3,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman Jr. (Maze 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacjr4,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman Jr. (Maze 4)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacm255,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Fixes 255th Maze)", GAME_SUPPORTS_SAVE )
GAME( 2007, pacmanp,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Don Hodges", "Pacman (Patched)", GAME_SUPPORTS_SAVE )
GAME( 2001, pacmar,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Mario Pacman", GAME_SUPPORTS_SAVE )
GAME( 2012, pacminer, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Jim Bagley", "Pac Manic Miner Man", GAME_SUPPORTS_SAVE )
GAME( 2000, pacms1,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacms2,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacms3,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacms4,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 4)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmsa1,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmsa2,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmsa3,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacmsa4,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 4)", GAME_SUPPORTS_SAVE )
GAME( 1982, pacplusc, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "Bally Midway", "Pacman Plus (Unencrypted) [c]", GAME_SUPPORTS_SAVE )
GAME( 1998, pacpopey, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman (Popeye)", GAME_SUPPORTS_SAVE )
GAME( 19??, pacrab,   puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "[Midway]", "Pac Rabbit", GAME_SUPPORTS_SAVE )
GAME( 2000, pacshuf,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Pacman (Shuffle)", GAME_SUPPORTS_SAVE )
GAME( 2000, pacspeed, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Pacman (Speedy)", GAME_SUPPORTS_SAVE )
GAME( 2000, pcrunchy, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Pacman (Crunchy)", GAME_SUPPORTS_SAVE )
GAME( 1981, pheartb,  puckman,  pacman,   newpuc2,  driver_device, 0,        ROT90, "Kamiya", "Pacman Hearts 2", GAME_SUPPORTS_SAVE )
GAME( 2000, pmad,     puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Sir Scotty", "Pacman After Dark", GAME_SUPPORTS_SAVE )
GAME( 2000, pmad00,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman After Dark 2000", GAME_SUPPORTS_SAVE )
GAME( 2000, pmada,    puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Pacman After Dark (Alternate)", GAME_SUPPORTS_SAVE )
GAME( 1980, pmheart,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90,  "bootleg", "Puckman Hearts", GAME_SUPPORTS_SAVE )
GAME( 2000, pplusad,  puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "[Bally Midway]", "Pacman Plus After Dark", GAME_SUPPORTS_SAVE )
GAME( 2000, puck255,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Namco]", "Puckman (Fixes 255th Maze)", GAME_SUPPORTS_SAVE )
GAME( 2007, puckmano, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Hebus", "Puckman Opposite", GAME_SUPPORTS_SAVE )
GAME( 2007, puckmot1, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Hebus", "Puckmotik (alt)", GAME_SUPPORTS_SAVE )
GAME( 2007, puckmoti, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Hebus", "Puckmotik", GAME_SUPPORTS_SAVE )
GAME( 2000, puckren,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "PacHack", "Puckman Renaissance", GAME_SUPPORTS_SAVE )
GAME( 2000, puckrenc, puckman,  pacman,   pacman0,  driver_device, 0,        ROT90, "PacHack", "Puckman Renaissance [c]", GAME_SUPPORTS_SAVE )
GAME( 2000, punleash, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "Peter Storey", "Pacman Unleashed", GAME_SUPPORTS_SAVE )
GAME( 1999, roboman,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Runge Editing", "Robo Man", GAME_SUPPORTS_SAVE )
GAME( 1981, spuckman, puckman,  pacman,   spuckman, driver_device, 0,        ROT90, "TV Tune / Miet!o!mat", "Super Puckman", GAME_SUPPORTS_SAVE )
GAME( 2000, sumelton, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Staizitto", "Summertime Elton", GAME_SUPPORTS_SAVE )
GAME( 2000, sumeltob, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Staizitto", "Summertime Elton (old)", GAME_SUPPORTS_SAVE )
GAME( 2000, ultra2,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "TwoBit Score", "Ultra Pacman", GAME_SUPPORTS_SAVE )
GAME( 2000, ultrapac, puckman,  woodpek,  pacman,   driver_device, 0,        ROT90, "PacFan", "UltraPac", GAME_SUPPORTS_SAVE )
GAME( 2000, vcrunchy, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Vector Pacman (Crunchy)", GAME_SUPPORTS_SAVE )
GAME( 2000, vecbaby,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Baby Maze 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, vecbaby2, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Baby Maze 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, vecbaby3, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Baby Maze 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, vecpac,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman", GAME_SUPPORTS_SAVE )
GAME( 2000, vectplus, puckman,  pacman,   pacman,   pacman_state,  pacplus,  ROT90, "[Bally Midway]", "Vector Pacman Plus", GAME_SUPPORTS_SAVE )
GAME( 2000, vectxens, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Jeff Morris", "Vector Xens Revenge", GAME_SUPPORTS_SAVE )
GAME( 2000, vhangly,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Nittoh]", "Vector Hangly-Man", GAME_SUPPORTS_SAVE )
GAME( 2008, vhangly3, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Gatinho [Nittoh]", "Vector Hangly-Man3", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacbell, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Vector Pacman (Bell)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacelec, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Vector Pacman (Electric Cowboy)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacjail, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Vector Pacman (Jail)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacms1,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacms2,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacms3,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacms4,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 4)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacmsa1, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 1)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacmsa2, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 2)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacmsa3, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 3)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacmsa4, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 4)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpacshuf, puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Vector Pacman (Shuffle)", GAME_SUPPORTS_SAVE )
GAME( 2000, vpspeed,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Snake Eyes", "Vector Pacman (Speedy)", GAME_SUPPORTS_SAVE )
GAME( 2000, xensad,   puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Jeff Morris", "Xens Revenge After Dark", GAME_SUPPORTS_SAVE )
GAME( 2000, xensrev,  puckman,  pacman,   pacman,   driver_device, 0,        ROT90, "Jeff Morris", "Xens Revenge", GAME_SUPPORTS_SAVE )


#include "mspacman.c"
#include "multipac.c"
#include "maketrax.c"

