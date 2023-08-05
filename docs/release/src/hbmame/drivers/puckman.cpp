// license:BSD-3-Clause
// copyright-holders:Robbbert
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


 Notes for the speedup cheat.
 - Only applies to games that run at normal speed
 - Only applies if the rom has the bytes in the usual place
 - If always enabled, the patch is applied when start pressed.
   This fixes the BAD ROM message at boot.
 - If you change the dipswitch during a game, you need to
   press Fire to activate the change.

********************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "puckman.h"
#include "sound/namco.h"
#include "machine/nvram.h"
#include "machine/74259.h"
#include "speaker.h"



/*************************************
 *
 *  Constants
 *
 *************************************/

#define MASTER_CLOCK        18432000

#define PIXEL_CLOCK     (MASTER_CLOCK/3)

/* H counts from 128->511, HBLANK starts at 144 and ends at 240 */
#define HTOTAL          (384)
#define HBEND           (0)     /*(96+16)*/
#define HBSTART         (288)   /*(16)*/

#define VTOTAL          (264)
#define VBEND           (0)     /*(16)*/
#define VBSTART         (224)   /*(224+16)*/



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

WRITE_LINE_MEMBER(puckman_state::irq_mask_w)
{
	m_irq_mask = state;
}

void puckman_state::pacman_interrupt_vector_w(u8 data)
{
	m_maincpu->set_input_line_vector(0, data);
	m_maincpu->set_input_line(0, CLEAR_LINE);
}

// Apply the speedup hack depending on dipswitch and fire button
void puckman_state::speedcheat()
{
	u8 fake_input = m_io_fake->read() & 7;

	u8 *RAM = machine().root_device().memregion("maincpu")->base();

	if (fake_input > 2) /* activate the cheat */
	{
		if (RAM[0x180b] == 0xbe) RAM[0x180b] = 1; /* pacman */
		else
		if (RAM[0x182d] == 0xbe) RAM[0x182d] = 1; /* pacplus */
	}
	else /* remove the cheat */
	{
		if (RAM[0x180b] == 1) RAM[0x180b] = 0xbe; /* pacman */
		else
		if (RAM[0x182d] == 1) RAM[0x182d] = 0xbe; /* pacplus */
	}
}

INTERRUPT_GEN_MEMBER( puckman_state::vblank_irq )
{
	if(m_irq_mask)
		device.execute().set_input_line(0, HOLD_LINE);
}

// When fire button pressed or released, see if cheat should turn on or off
INPUT_CHANGED_MEMBER(puckman_state::pacman_fake)
{
	speedcheat();
}

// When start button pressed for the first time, see if cheat should turn on
u8 puckman_state::in1_r()
{
	u8 data = ioport("IN1")->read();
	if ((data & 0x60) < 0x60)
		if (m_speedcheat != 120)
		{
			if (m_io_fake.read_safe(120) == 4)
				speedcheat();
			m_speedcheat = 120;
		}

	return data;
}

/*************************************
 *
 *  LEDs/coin counters
 *
 *************************************/

WRITE_LINE_MEMBER(puckman_state::led1_w)
{
	//output().set_led_value(0, state);     // fix later
}

WRITE_LINE_MEMBER(puckman_state::led2_w)
{
	//output().set_led_value(1, state);     // fix later
}


WRITE_LINE_MEMBER(puckman_state::coin_counter_w)
{
	machine().bookkeeping().coin_counter_w(0, state);
}


WRITE_LINE_MEMBER(puckman_state::coin_lockout_global_w)
{
	machine().bookkeeping().coin_lockout_global_w(!state);
}


u8 puckman_state::pacman_read_nop()
{
	return 0xbf;
}

/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

void puckman_state::pacman_map(address_map &map) {
	map.global_mask(0x7fff);   /* A15 not connected at the CPU */
	map(0x0000,0x3fff).rom();
	map(0x4000,0x43ff).mirror(0x2000).ram().w(FUNC(puckman_state::pacman_videoram_w)).share("videoram");
	map(0x4400,0x47ff).mirror(0x2000).ram().w(FUNC(puckman_state::pacman_colorram_w)).share("colorram");
	map(0x4800,0x4bff).mirror(0x2000).r(FUNC(puckman_state::pacman_read_nop));
	map(0x4c00,0x4fef).mirror(0x2000).ram();
	map(0x4ff0,0x4fff).mirror(0x2000).ram().share("spriteram");
	map(0x5000,0x5007).w("mainlatch",FUNC(addressable_latch_device::write_d0));
	map(0x5040,0x505f).w("namco",FUNC(namco_device::pacman_sound_w));
	map(0x5060,0x506f).writeonly().share("spriteram2");
	map(0x5070,0x5080).nopw();
	map(0x50c0,0x50c0).w("watchdog",FUNC(watchdog_timer_device::reset_w));
	map(0x5000,0x5000).portr("IN0");
	map(0x5040,0x5040).r(FUNC(puckman_state::in1_r));
	map(0x5080,0x5080).portr("DSW1");
	map(0x50c0,0x50c0).portr("DSW2");
}

void puckman_state::woodpek_map(address_map &map) {
	map(0x0000,0x3fff).rom();
	map(0x4000,0x43ff).mirror(0xa000).ram().w(FUNC(puckman_state::pacman_videoram_w)).share("videoram");
	map(0x4400,0x47ff).mirror(0xa000).ram().w(FUNC(puckman_state::pacman_colorram_w)).share("colorram");
	map(0x4800,0x4bff).mirror(0xa000).r(FUNC(puckman_state::pacman_read_nop));
	map(0x4c00,0x4fef).mirror(0xa000).ram();
	map(0x4ff0,0x4fff).mirror(0xa000).ram().share("spriteram");
	map(0x5000,0x5007).w("mainlatch",FUNC(ls259_device::write_d0));
	map(0x5040,0x505f).mirror(0x8000).w("namco",FUNC(namco_device::pacman_sound_w));
	map(0x5060,0x506f).mirror(0x8000).writeonly().share("spriteram2");
	map(0x5070,0x5080).mirror(0x8000).nopw();
	map(0x50c0,0x50c0).w("watchdog",FUNC(watchdog_timer_device::reset_w));
	map(0x5000,0x5000).mirror(0x8000).portr("IN0");
	map(0x5040,0x5040).mirror(0x8000).r(FUNC(puckman_state::in1_r));
	map(0x5080,0x5080).mirror(0x8000).portr("DSW1");
	map(0x50c0,0x50c0).mirror(0x8000).portr("DSW2");
	map(0x8000,0xbfff).rom();
	map(0xff00,0xffff).ram();
}

/*************************************
 *
 *  Main CPU port handlers
 *
 *************************************/

void puckman_state::io_map(address_map &map) {
	map(0x0000,0xffff).w(FUNC(puckman_state::pacman_interrupt_vector_w));
}

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
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 ) /* Also invincibility when playing */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 ) /* Also speed-up when playing */
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
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME( "Speed (Cheat)" ) PORT_CHANGED_MEMBER(DEVICE_SELF, puckman_state, pacman_fake, 0)
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
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME( "Speed (Cheat)" ) PORT_CHANGED_MEMBER(DEVICE_SELF, puckman_state, pacman_fake, 0)
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
	8,8,    /* 8*8 characters */
	RGN_FRAC(1,2),    /* 256 characters */
	2,  /* 2 bits per pixel */
	{ 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 }, /* bits are packed in groups of four */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	16*8    /* every char takes 16 bytes */
};


static const gfx_layout spritelayout =
{
	16,16,  /* 16*16 sprites */
	RGN_FRAC(1,2),  /* 64 sprites */
	2,  /* 2 bits per pixel */
	{ 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3, 24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 32*8, 33*8, 34*8, 35*8, 36*8, 37*8, 38*8, 39*8 },
	64*8    /* every sprite takes 64 bytes */
};


static gfx_layout xtilelayout =
{
	16,16,  /* 16*16 characters */
	RGN_FRAC(1,2),    /* 256 characters */
	2,  /* 2 bits per pixel */
	{ 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 0, 1, 2, 3, 128, 129, 130, 131, 256, 257, 258, 259, 384, 385, 386, 387 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8 },
	64*8    /* every char takes 64 bytes */
};


static gfx_layout xspritelayout =
{
	32,32,  /* 32*32 sprites */
	RGN_FRAC(1,2),  /* 64 sprites */
	2,  /* 2 bits per pixel */
	{ 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 0, 1, 2, 3, 256, 257, 258, 259, 512, 513, 514, 515, 768, 769, 770, 771,
			1024+0, 1024+1, 1024+2, 1024+3, 1024+256, 1024+257, 1024+258, 1024+259,
			1024+512, 1024+513, 1024+514, 1024+515, 1024+768, 1024+769, 1024+770, 1024+771 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8,
			16*8, 17*8, 18*8, 19*8, 20*8, 21*8, 22*8, 23*8, 24*8, 25*8, 26*8, 27*8, 28*8, 29*8, 30*8, 31*8 },
	256*8   /* every sprite takes 256 bytes */
};


static GFXDECODE_START( gfx_pacman )
	GFXDECODE_ENTRY( "gfx1", 0x0000, tilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x1000, spritelayout, 0, 32 )
GFXDECODE_END

static GFXDECODE_START( gfx_pacmanx )
	GFXDECODE_ENTRY( "gfx1", 0x0000, xtilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x4000, xspritelayout, 0, 32 )
GFXDECODE_END



/*************************************
 *
 *  Machine drivers
 *
 *************************************/

void puckman_state::pacman(machine_config &config)
{
	/* basic machine hardware */
	Z80(config, m_maincpu, MASTER_CLOCK/6);
	m_maincpu->set_addrmap(AS_PROGRAM, &puckman_state::pacman_map);
	m_maincpu->set_addrmap(AS_IO, &puckman_state::io_map);
	m_maincpu->set_vblank_int("screen", FUNC(puckman_state::vblank_irq));

	WATCHDOG_TIMER(config, m_watchdog).set_vblank_count("screen", 16);

	LS259(config, m_mainlatch); // 74LS259 at 8K or 4099 at 7K
	m_mainlatch->q_out_cb<0>().set(FUNC(puckman_state::irq_mask_w));
	m_mainlatch->q_out_cb<1>().set("namco", FUNC(namco_device::sound_enable_w));
	m_mainlatch->q_out_cb<3>().set(FUNC(puckman_state::flipscreen_w));
	//m_mainlatch->q_out_cb<4>().set_output("led0");
	//m_mainlatch->q_out_cb<5>().set_output("led1");
	//m_mainlatch->q_out_cb<6>().set(FUNC(puckman_state::coin_lockout_global_w));
	m_mainlatch->q_out_cb<7>().set(FUNC(puckman_state::coin_counter_w));

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(PIXEL_CLOCK, HTOTAL, HBEND, HBSTART, VTOTAL, VBEND, VBSTART);
	m_screen->set_screen_update(FUNC(puckman_state::screen_update_pacman));
	m_screen->set_palette("palette");

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_pacman);
	PALETTE(config, m_palette, FUNC(puckman_state::pacman_palette), 128*4, 32);

	MCFG_VIDEO_START_OVERRIDE(puckman_state,pacman)

	/* sound hardware */
	SPEAKER(config, "mono").front_center();
	NAMCO(config, m_namco_sound, MASTER_CLOCK/6/32);
	m_namco_sound->set_voices(3);
	m_namco_sound->add_route(ALL_OUTPUTS, "mono", 0.50);
}

void puckman_state::pacmanx(machine_config &config)
{
	pacman(config);
	//config.device_remove("screen");
	//screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER));
	//screen.set_palette("palette");
	m_screen->set_raw(PIXEL_CLOCK<<2, HTOTAL<<1, HBEND<<1, HBSTART<<1, VTOTAL<<1, VBEND<<1, VBSTART<<1);
	m_screen->set_screen_update(FUNC(puckman_state::screen_update_pacmanx));
	m_gfxdecode->set_info(gfx_pacmanx);
	MCFG_VIDEO_START_OVERRIDE(puckman_state, pacmanx)
}


void puckman_state::woodpek(machine_config &config)
{
	pacman(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &puckman_state::woodpek_map);
}


/*************************************
 *
 *  Driver initialization
 *
 *************************************/

u8 puckman_state::hbmame_pacplus_decode(offs_t addr, u8 e)
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
	return bitswap<8>(e,tbl[0],tbl[1],tbl[2],tbl[3],tbl[4],tbl[5],tbl[6],tbl[7]) ^ tbl[8];
}

void puckman_state::init_pacplus()
{
	/* CPU ROMs */
	u8 *RAM = machine().root_device().memregion("maincpu")->base();

	for (offs_t i = 0; i < 0x4000; i++)
		RAM[i] = hbmame_pacplus_decode(i,RAM[i]);
}

void puckman_state::eyes_decode(u8 *data)
{
	u16 j;
	u16 swapbuffer[8];

	for (j = 0; j < 8; j++)
		swapbuffer[j] = data[bitswap<16>(j,15,14,13,12,11,10,9,8,7,6,5,4,3,0,1,2)];

	for (j = 0; j < 8; j++)
		data[j] = bitswap<8>(swapbuffer[j],7,4,5,6,3,2,1,0);
}

void puckman_state::init_eyes()
{
	/* CPU ROMs */
	/* Data lines D3 and D5 swapped */
	u8 *RAM = machine().root_device().memregion("maincpu")->base();
	for (u16 i = 0; i < 0x4000; i++)
		RAM[i] = bitswap<8>(RAM[i],7,6,3,4,5,2,1,0);


	/* Graphics ROMs */

	/* Data lines D4 and D6 and address lines A0 and A2 are swapped */
	RAM = machine().root_device().memregion("gfx1")->base();
	for (u32 i = 0;i < machine().root_device().memregion("gfx1")->bytes();i += 8)
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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

ROM_START( bacman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "bacman.6e",    0x0000, 0x1000, CRC(12f22a11) SHA1(212f72ce9e8b999f00a0a8a31683e723b82c626a) )
	ROM_LOAD( "bacman.6f",    0x1000, 0x1000, CRC(6ed094e0) SHA1(b8a9df2526275a993c9e5f67b2dc72a45c882f60) )
	ROM_LOAD( "bacman.6h",    0x2000, 0x1000, CRC(373f007b) SHA1(a2b5067db2a19129fed16511ea18d2e7e49092e4) )
	ROM_LOAD( "bacman.6j",    0x3000, 0x1000, CRC(e060d3e2) SHA1(43b30a23c3b9b4370038bdfff8fe7ab0686b6a28) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

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

ROM_START( bucaneerx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "hangly3.6k",   0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
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
	ROM_LOAD( "hangly3.6k",   0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
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

ROM_START( jackman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "jackman.6j",   0x3000, 0x1000, CRC(1f52ef8b) SHA1(0a803352a964de9567bc314f8ace1c612082e3d0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "jackman.5e",   0x0000, 0x1000, CRC(809bd73e) SHA1(81989a21d089eb76297926905729efcd69abeb87) )
	ROM_LOAD( "jackman.5f",   0x1000, 0x1000, CRC(0509d3e6) SHA1(e589543b9a2f1c81115c03b4b88096ac84826e62) )

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
	ROM_LOAD( "pmheart.6k",   0x0800, 0x0800, CRC(afeca2f1) SHA1(1e6d6c75eeb3a354ce2dc88da62caf9e7d53d0cb) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "puckman.6m",   0x1800, 0x0800, CRC(d3e8914c) SHA1(c2f00e1773c6864435f29c8b7f44f2ef85d227d3) )
	ROM_LOAD( "pacheart.6h",  0x2000, 0x0800, CRC(9045a44c) SHA1(a97d7016effbd2ace9a7d92ceb04a6ce18fb42f9) )
	ROM_LOAD( "pmheart.6n",   0x2800, 0x0800, CRC(93f344c5) SHA1(987c7fa18a774a47c045fa1dc7dff37457cb8983) )
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

ROM_START( muckpan ) // Gobbler Muckpan - wrong map, should be newpuck map (used puckman roms because most of the muckpan set was bad)
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "puckman.6e",   0x0000, 0x0800, CRC(f36e88ab) SHA1(813cecf44bf5464b1aed64b36f5047e4c79ba176) )
	ROM_LOAD( "puckman.6k",   0x0800, 0x0800, CRC(618bd9b3) SHA1(b9ca52b63a49ddece768378d331deebbe34fe177) )
	ROM_LOAD( "puckman.6f",   0x1000, 0x0800, CRC(7d177853) SHA1(9b5ddaaa8b564654f97af193dbcc29f81f230a25) )
	ROM_LOAD( "puckman.6m",   0x1800, 0x0800, CRC(d3e8914c) SHA1(c2f00e1773c6864435f29c8b7f44f2ef85d227d3) )
	ROM_LOAD( "puckman.6h",   0x2000, 0x0800, CRC(6bf4f625) SHA1(afe72fdfec66c145b53ed865f98734686b26e921) )
	ROM_LOAD( "puckman.6n",   0x2800, 0x0800, CRC(a948ce83) SHA1(08759833f7e0690b2ccae573c929e2a48e5bde7f) )
	ROM_LOAD( "muckpan.6j",   0x3000, 0x0800, CRC(e1f4f531) SHA1(df8f898df416445330c0ce473e165c2e437da79f) )
	ROM_LOAD( "muckpan.6p",   0x3800, 0x0800, CRC(1d5b18bf) SHA1(82e17571a8b740884f0a164572c8297896b19e66) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( namcosil )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
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

ROM_START( packetman ) // real dump, hanglyman with the manufacturer name hacked out
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "hangly.6e",    0x0000, 0x1000, CRC(5fe8610a) SHA1(d63eaebd85e10aa6c27bb7f47642dd403eeb6934) )
	ROM_LOAD( "puckmanh.6f",  0x1000, 0x1000, CRC(61d38c6c) SHA1(1406aacdc9c8a3776e5853d214380ad3124408f4) )
	ROM_LOAD( "hangly.6h",    0x2000, 0x1000, CRC(4e7ef99f) SHA1(bd42e68b29b4d654dc817782ba00db69b7d2dfe2) )
	ROM_LOAD( "packetman.6j", 0x3000, 0x1000, CRC(2693f8ac) SHA1(da240af15b41bf253df14a73b2078c679cd01cb3) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "pacman25.5",   0x8000, 0x1000, CRC(19a22304) SHA1(6cc0e4921148b7a7eedf5d807bd63062b745e1c6) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )
	/* This colour table is configurable to whatever you want */
	ROM_FILL(0x8fc0, 1, 0x01)   // level 1
	ROM_FILL(0x8fc1, 1, 0x11)   // level 2
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
	ROM_FILL(0x8fcf, 1, 0x07)   // level 16
	ROM_COPY( "maincpu", 0x8fc0, 0x8fd0, 0x10 ) /* copy the above to levels 17-32 */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pac25.5e",     0x0000, 0x1000, CRC(11d47ae1) SHA1(fbd47432f258e56f1323766d5b2844f4a2e6e7ac) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacman25s01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s01",    0x8000, 0x1000, CRC(a7a8120b) SHA1(350b1c348a0a3678bd182a93ac8d8a7225128501) )
	ROM_LOAD( "maps2.s01",    0x9000, 0x1000, CRC(985ed43b) SHA1(384b14b7777f98433ec49a4ac2ca571e96fb4634) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s01.5e",       0x0000, 0x1000, CRC(b260db5b) SHA1(7f54776e323c25312ec8641e9ba539bb864a338f) )
	ROM_LOAD( "pmad.5f",      0x1000, 0x1000, CRC(59a9362c) SHA1(aa7dfa45cb529e7ca4493262090b149e8c523006) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s01.7f",       0x0000, 0x0020, CRC(8bf26558) SHA1(986e2624ec3c01d45f0973872c5457133d452732) )
	ROM_LOAD( "s01.4a",       0x0020, 0x0100, CRC(db8c8390) SHA1(fcd4295671df2d9f678c0d9ad8f80650851f6ec3) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s02 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "s02.6e",       0x0000, 0x1000, CRC(e9df2a73) SHA1(b57f4b360fa7624319e30fb36834f4a1806252f9) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s02.6j",       0x3000, 0x1000, CRC(3aad8292) SHA1(5489c7b409cbbd1d73e6367fd60852388b691883) )
	ROM_LOAD( "maps1.s02",    0x8000, 0x1000, CRC(5652bc93) SHA1(bcb6cbb86a0d37b79d8124ff9153b50ccf19fb79) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s02.5e",       0x0000, 0x1000, CRC(d50bd122) SHA1(89d71c43a1229236df2417892dd7a5787bbdce2b) )
	ROM_LOAD( "s02.5f",       0x1000, 0x1000, CRC(44226756) SHA1(c783582af949972230126d89b9dcaf608d3c030b) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s03 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "s03.6e",       0x0000, 0x1000, CRC(7c641473) SHA1(b931f29df19aeb4d0f133ca6dced387a6fa5700c) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s03.6j",       0x3000, 0x1000, CRC(4f4559ca) SHA1(41ecec82cda6622d42455fedeed41318223370c1) )
	ROM_LOAD( "maps1.s02",    0x8000, 0x1000, CRC(5652bc93) SHA1(bcb6cbb86a0d37b79d8124ff9153b50ccf19fb79) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s03.5e",       0x0000, 0x1000, CRC(39bbeae9) SHA1(d54fac4ebc49b56a9cce1f9b5aedbdc4c75987e6) )
	ROM_LOAD( "s03.5f",       0x1000, 0x1000, CRC(29b2ea8b) SHA1(50a6d43864c25ebf6eeeccf1c0580685d913c071) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s03.7f",       0x0000, 0x0020, CRC(529edab1) SHA1(9993c00abf075b7148513af65d57703e523617f4) )
	ROM_LOAD( "s03.4a",       0x0020, 0x0100, CRC(68fdfbe5) SHA1(5b05cfdc6ac2f9b5b67427f280afeded857e2699) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s04 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s04.6j",       0x3000, 0x1000, CRC(e6ba63fb) SHA1(6d22a568151ac2ddac53d959f56a1a1fc75141f5) )
	ROM_LOAD( "maps1.s04",    0x8000, 0x1000, CRC(5a031fe9) SHA1(e46dc81a7258090d39428560406d995f76089a92) )
	ROM_LOAD( "maps2.s04",    0x9000, 0x1000, CRC(ca55c3ca) SHA1(e07f7845cf788d7fbe6dde765373ed3c57b8b7d8) )
	ROM_LOAD( "maps3.s04",    0xa000, 0x1000, CRC(c89f8526) SHA1(95283483cd87b76db82b6d9e1257e02243d076bc) )
	ROM_LOAD( "maps4.s04",    0xb000, 0x1000, CRC(e724ed75) SHA1(6bad6b188d93071951b1a79d310ce5ff5ef3f56e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s04.5e",       0x0000, 0x1000, CRC(271be184) SHA1(f37dd55523aa6ad3ce002284aba7149335d58926) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s04.7f",       0x0000, 0x0020, CRC(933f85e1) SHA1(d9ecab6272db185a14f488a8f88a6ff20d570cfd) )
	ROM_LOAD( "s04.4a",       0x0020, 0x0100, CRC(ee240b8f) SHA1(e21837a9e42c80175260b3f727d9b0d32b403402) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s05 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s04.6j",       0x3000, 0x1000, CRC(e6ba63fb) SHA1(6d22a568151ac2ddac53d959f56a1a1fc75141f5) )
	ROM_LOAD( "maps1.s05",    0x8000, 0x1000, CRC(537f2f36) SHA1(87dcae6fab0692534384e9d0a76a8bddeac0f045) )
	ROM_LOAD( "maps2.s05",    0x9000, 0x1000, CRC(53075f4c) SHA1(771464527b3f61a01410fdac3a5897a66b6a05c4) )
	ROM_LOAD( "maps3.s04",    0xa000, 0x1000, CRC(c89f8526) SHA1(95283483cd87b76db82b6d9e1257e02243d076bc) )
	ROM_LOAD( "maps4.s05",    0xb000, 0x1000, CRC(f23b482e) SHA1(394c1a092b638e9ce2fb5c4c321b1e3e42fb2418) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s04.5e",       0x0000, 0x1000, CRC(271be184) SHA1(f37dd55523aa6ad3ce002284aba7149335d58926) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s06 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s06.6j",       0x3000, 0x1000, CRC(5f531db3) SHA1(beceec3f0b4590760625145d2be1f8861b6f8559) )
	ROM_LOAD( "maps1.s06",    0x8000, 0x1000, CRC(c1e5f099) SHA1(388b85f82053c61ffa491c80262cd19907cf9fe3) )
	ROM_LOAD( "maps2.s06",    0x9000, 0x1000, CRC(e0f5720d) SHA1(9be19ba2153bae983eb912d6c93f7d82b285020a) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s06.5e",       0x0000, 0x1000, CRC(12c5fd3e) SHA1(9a580c362b04336a0999f0bc06d3bcea67352d39) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s07 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s04.6j",       0x3000, 0x1000, CRC(e6ba63fb) SHA1(6d22a568151ac2ddac53d959f56a1a1fc75141f5) )
	ROM_LOAD( "maps1.s07",    0x8000, 0x1000, CRC(b0008a08) SHA1(07c70cb0d4d7916f9d956e7de4b9589e9050e6c9) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pac25.5e",     0x0000, 0x1000, CRC(11d47ae1) SHA1(fbd47432f258e56f1323766d5b2844f4a2e6e7ac) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s08 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s08.6j",       0x3000, 0x1000, CRC(1d5fea00) SHA1(e3ef689e95aaf1f49f7943b837dcd3f49c6c1b92) )
	ROM_LOAD( "maps1.s08",    0x8000, 0x1000, CRC(1385bca4) SHA1(4596515d34b39e9abaa2da7e2a723a3aac481bf6) )
	ROM_LOAD( "maps2.s08",    0x9000, 0x1000, CRC(70f5312e) SHA1(b77da874f645e47a4d3ca5cf54948498cc73f3fc) )
	ROM_LOAD( "maps3.s08",    0xa000, 0x1000, CRC(0a3a7f41) SHA1(0ab7cd64905c327917a851de7b4dd32be4454362) )
	ROM_LOAD( "maps4.s08",    0xb000, 0x1000, CRC(97eca9d9) SHA1(f881a7c19dd1dd3c3e08281df1af13d2a47805d0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s08.5e",       0x0000, 0x1000, CRC(f7cbd6f3) SHA1(180d264f2fb594d3aedfe8f642d8919a59485042) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s09 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s09.6j",       0x3000, 0x1000, CRC(72363463) SHA1(9d12039b94b8fd68911bf27e1d610ddd03027be2) )
	ROM_LOAD( "maps1.s09",    0x8000, 0x1000, CRC(95e36ec3) SHA1(c1fe4278a6115b8c1f345fba09b8c6243d68265a) )
	ROM_LOAD( "maps2.s08",    0x9000, 0x1000, CRC(70f5312e) SHA1(b77da874f645e47a4d3ca5cf54948498cc73f3fc) )
	ROM_LOAD( "maps3.s08",    0xa000, 0x1000, CRC(0a3a7f41) SHA1(0ab7cd64905c327917a851de7b4dd32be4454362) )
	ROM_LOAD( "maps4.s08",    0xb000, 0x1000, CRC(97eca9d9) SHA1(f881a7c19dd1dd3c3e08281df1af13d2a47805d0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s09.5e",       0x0000, 0x1000, CRC(833822e4) SHA1(c2b4dfa2714e85c2e3937cf036b23beccc30a66a) )
	ROM_LOAD( "s09.5f",       0x1000, 0x1000, CRC(38ec5122) SHA1(e333ce7084be5dfdbb0cec8df3ec58d6ba1669c6) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s10 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s09.6j",       0x3000, 0x1000, CRC(72363463) SHA1(9d12039b94b8fd68911bf27e1d610ddd03027be2) )
	ROM_LOAD( "maps1.s10",    0x8000, 0x1000, CRC(5ddee88e) SHA1(19aedfb8ab3756da4ef726144e57c89cfd006bf6) )
	ROM_LOAD( "maps2.s08",    0x9000, 0x1000, CRC(70f5312e) SHA1(b77da874f645e47a4d3ca5cf54948498cc73f3fc) )
	ROM_LOAD( "maps3.s08",    0xa000, 0x1000, CRC(0a3a7f41) SHA1(0ab7cd64905c327917a851de7b4dd32be4454362) )
	ROM_LOAD( "maps4.s08",    0xb000, 0x1000, CRC(97eca9d9) SHA1(f881a7c19dd1dd3c3e08281df1af13d2a47805d0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s08.5e",       0x0000, 0x1000, CRC(f7cbd6f3) SHA1(180d264f2fb594d3aedfe8f642d8919a59485042) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s11 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s09.6j",       0x3000, 0x1000, CRC(72363463) SHA1(9d12039b94b8fd68911bf27e1d610ddd03027be2) )
	ROM_LOAD( "maps1.s09",    0x8000, 0x1000, CRC(95e36ec3) SHA1(c1fe4278a6115b8c1f345fba09b8c6243d68265a) )
	ROM_LOAD( "maps2.s08",    0x9000, 0x1000, CRC(70f5312e) SHA1(b77da874f645e47a4d3ca5cf54948498cc73f3fc) )
	ROM_LOAD( "maps3.s08",    0xa000, 0x1000, CRC(0a3a7f41) SHA1(0ab7cd64905c327917a851de7b4dd32be4454362) )
	ROM_LOAD( "maps4.s08",    0xb000, 0x1000, CRC(97eca9d9) SHA1(f881a7c19dd1dd3c3e08281df1af13d2a47805d0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s08.5e",       0x0000, 0x1000, CRC(f7cbd6f3) SHA1(180d264f2fb594d3aedfe8f642d8919a59485042) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s12 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "s02.6e",       0x0000, 0x1000, CRC(e9df2a73) SHA1(b57f4b360fa7624319e30fb36834f4a1806252f9) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s12.6j",       0x3000, 0x1000, CRC(cc6e2584) SHA1(caa82a7556201287cddf3aaadfc1dd41b24bde41) )
	ROM_LOAD( "maps1.s12",    0x8000, 0x1000, CRC(da3e319f) SHA1(e146e422d4c828ad0299bc234d2ce4921d7ce7e2) )
	ROM_LOAD( "maps2.s12",    0x9000, 0x1000, CRC(f31a519a) SHA1(1054462ee03317e0f3df08d539c357a6c6ba423b) )
	ROM_LOAD( "maps3.s12",    0xa000, 0x1000, CRC(f6879287) SHA1(6040c599f6bc8349c5b3dbdab8036904e19d7553) )
	ROM_LOAD( "maps4.s12",    0xb000, 0x1000, CRC(19f73019) SHA1(076c4e60b808eec467aae69564491cc9afc4ddb4) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s02.5e",       0x0000, 0x1000, CRC(d50bd122) SHA1(89d71c43a1229236df2417892dd7a5787bbdce2b) )
	ROM_LOAD( "s02.5f",       0x1000, 0x1000, CRC(44226756) SHA1(c783582af949972230126d89b9dcaf608d3c030b) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s12.7f",       0x0000, 0x0020, CRC(106f6ae6) SHA1(7b7f42afce177fe9cc65efdb7acf14add3ce6837) )
	ROM_LOAD( "s12.4a",       0x0020, 0x0100, CRC(b47466e7) SHA1(586f384f9c40559407bf9fcfe3cdef39308257ca) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s13 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s13.6j",       0x3000, 0x1000, CRC(a44ca164) SHA1(249b5f392b269b613bae765bef19cfd484552dc8) )
	ROM_LOAD( "maps1.s13",    0x8000, 0x1000, CRC(d0408136) SHA1(302d4ec9e77f26afdd34e6225581503f6d326e0a) )
	ROM_LOAD( "maps2.s13",    0x9000, 0x1000, CRC(d359bf35) SHA1(ecd38c53799aa19dd694e491adb9223f016032db) )
	ROM_LOAD( "maps3.s13",    0xa000, 0x1000, CRC(1f6aabe9) SHA1(6c22da37de6dcf6192a64d759a868ce4a5fe98b9) )
	ROM_LOAD( "maps4.s13",    0xb000, 0x1000, CRC(e2da3bc6) SHA1(67cb17d5e497d447225ad2cc4f969ee84d55b45d) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s14 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s14.6j",       0x3000, 0x1000, CRC(a89dec09) SHA1(09dabbad1ba392615d3b56aeeca279cfe86c2cb8) )
	ROM_LOAD( "maps1.s14",    0x8000, 0x1000, CRC(c5cecfff) SHA1(49ca1afc2dceb81fc6b125fa55dface5e4a8d729) )
	ROM_LOAD( "maps2.s14",    0x9000, 0x1000, CRC(448a0986) SHA1(663fb0f7877dcb9c62f331ef351209ddeec2ed96) )
	ROM_LOAD( "maps3.s14",    0xa000, 0x1000, CRC(f104cd96) SHA1(f5e7e5ba3b4aff072fcba60a76d76dd23b10c11d) )
	ROM_LOAD( "maps4.s05",    0xb000, 0x1000, CRC(f23b482e) SHA1(394c1a092b638e9ce2fb5c4c321b1e3e42fb2418) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s04.5e",       0x0000, 0x1000, CRC(271be184) SHA1(f37dd55523aa6ad3ce002284aba7149335d58926) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s15 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s14.6j",       0x3000, 0x1000, CRC(a89dec09) SHA1(09dabbad1ba392615d3b56aeeca279cfe86c2cb8) )
	ROM_LOAD( "maps1.s15",    0x8000, 0x1000, CRC(9ca458de) SHA1(6470981e18aaa276167c98e3f8a8747760aef8df) )
	ROM_LOAD( "maps2.s14",    0x9000, 0x1000, CRC(448a0986) SHA1(663fb0f7877dcb9c62f331ef351209ddeec2ed96) )
	ROM_LOAD( "maps3.s14",    0xa000, 0x1000, CRC(f104cd96) SHA1(f5e7e5ba3b4aff072fcba60a76d76dd23b10c11d) )
	ROM_LOAD( "maps4.s05",    0xb000, 0x1000, CRC(f23b482e) SHA1(394c1a092b638e9ce2fb5c4c321b1e3e42fb2418) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s04.5e",       0x0000, 0x1000, CRC(271be184) SHA1(f37dd55523aa6ad3ce002284aba7149335d58926) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s18 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s18.6j",       0x3000, 0x1000, CRC(96158800) SHA1(d93c12f52a683d591a95a8891ad22f55c513a0db) )
	ROM_LOAD( "maps1.s18",    0x8000, 0x1000, CRC(6e6e7ad5) SHA1(785f1315ac4b69c4fdb09e9a36648482337c7310) )
	ROM_LOAD( "maps2.s18",    0x9000, 0x1000, CRC(9c5b4b57) SHA1(3b4863bbd3ae2ebea58b09fae33dbcd7816088a6) )
	ROM_LOAD( "maps3.s18",    0xa000, 0x1000, CRC(cdb902dc) SHA1(6f711dbd4ca29bde4341c8030d61beefc564c719) )
	ROM_LOAD( "maps4.s18",    0xb000, 0x1000, CRC(0718ef8f) SHA1(6fac422485a374fb0b536ad1ad577afe2064718f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s19 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s18.6j",       0x3000, 0x1000, CRC(96158800) SHA1(d93c12f52a683d591a95a8891ad22f55c513a0db) )
	ROM_LOAD( "maps1.s19",    0x8000, 0x1000, CRC(86406f02) SHA1(ab7b3b3b088f5ae9ac5bdb02ca2b0e768f4cb00d) )
	ROM_LOAD( "maps2.s18",    0x9000, 0x1000, CRC(9c5b4b57) SHA1(3b4863bbd3ae2ebea58b09fae33dbcd7816088a6) )
	ROM_LOAD( "maps3.s18",    0xa000, 0x1000, CRC(cdb902dc) SHA1(6f711dbd4ca29bde4341c8030d61beefc564c719) )
	ROM_LOAD( "maps4.s18",    0xb000, 0x1000, CRC(0718ef8f) SHA1(6fac422485a374fb0b536ad1ad577afe2064718f) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s20 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s20",    0x8000, 0x1000, CRC(5e9e7661) SHA1(e1da102a74533baf5db2c483273ed616edcf8420) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s21 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s21.6j",       0x3000, 0x1000, CRC(cb38a821) SHA1(621491bdc61da325084819c49de79206fe00be6d) )
	ROM_LOAD( "maps1.s21",    0x8000, 0x1000, CRC(5755ab7c) SHA1(79c8871f825a8bbc10d86eadbd418361107cf2c6) )
	ROM_LOAD( "maps2.s21",    0x9000, 0x1000, CRC(8cb86222) SHA1(a954e3d94cc7bc342e25ea230ecff00112bf57b9) )
	ROM_LOAD( "maps3.s21",    0xa000, 0x1000, CRC(841a5951) SHA1(d8e8feb7cd382330bb21692469290761117c12aa) )
	ROM_LOAD( "maps4.s21",    0xb000, 0x1000, CRC(85ed3b82) SHA1(c7dec0f7d03ca8c1afdb42e56a0a59cd666ac181) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s22 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s22.6j",       0x3000, 0x1000, CRC(d24028ce) SHA1(afae8d808bd6e15fcfadc969a0f361876a62065a) )
	ROM_LOAD( "maps1.s22",    0x8000, 0x1000, CRC(aa578681) SHA1(fd3758e6e6526a49365415a37c75ea7d79f79b4b) )
	ROM_LOAD( "maps2.s22",    0x9000, 0x1000, CRC(e49cbfe7) SHA1(74c429a5388b2dc419fdecf4e2e10b4c434b3856) )
	ROM_LOAD( "maps3.s22",    0xa000, 0x1000, CRC(f42821b0) SHA1(7b1744a399b2105b69c13804d391a9a45a74b637) )
	ROM_LOAD( "maps4.s22",    0xb000, 0x1000, CRC(9e7b539d) SHA1(e295c424517bd343d17fff084bd2ab0473acd41b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s23 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s23.6j",       0x3000, 0x1000, CRC(5d4e2de8) SHA1(f3d6fbecc412ce8ead6c98f31154cabbf3e7ed05) )
	ROM_LOAD( "maps1.s23",    0x8000, 0x1000, CRC(1adb75f0) SHA1(8f43f8f8dff684b6d082fba73c2996e5e0dd4cbc) )
	ROM_LOAD( "maps2.s23",    0x9000, 0x1000, CRC(688c496c) SHA1(87ed9bc231904b0fedbcd55b3917d9b806b369db) )
	ROM_LOAD( "maps3.s23",    0xa000, 0x1000, CRC(07867444) SHA1(2bc870081a621cb931de730e4b88b30b2823fb9d) )
	ROM_LOAD( "maps4.s23",    0xb000, 0x1000, CRC(5b0609b7) SHA1(ef45c45e748616bbe3c9e64ab0d45651aff3238a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s24 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s24",    0x8000, 0x1000, CRC(53ed11fd) SHA1(1041173cfa5646e1eb80777aba2aa4deac4828ac) )
	ROM_LOAD( "maps2.s24",    0x9000, 0x1000, CRC(99824404) SHA1(4c587ed1a60cbcde7b3505aaf212e6745cad5bb0) )
	ROM_LOAD( "maps3.s24",    0xa000, 0x1000, CRC(241b37bc) SHA1(93d84983ec1201cd8b1d9b7568ba9ad1bdcf8ca8) )
	ROM_LOAD( "maps4.s24",    0xb000, 0x1000, CRC(6eb73bb1) SHA1(bffa744f49b8f538aa1efbdc403da35ea8b677c6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s25 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s25.6j",       0x3000, 0x1000, CRC(bb54b431) SHA1(09557e1098fb9762bf44d4b25acd3088130fecbd) )
	ROM_LOAD( "maps1.s25",    0x8000, 0x1000, CRC(d769498a) SHA1(025f49dae14dcfc626b71740ce2dff9b937ed2e2) )
	ROM_LOAD( "maps2.s25",    0x9000, 0x1000, CRC(82c8c3d4) SHA1(3198011c217ea7a669018a7bbb29bf909f870405) )
	ROM_LOAD( "maps3.s25",    0xa000, 0x1000, CRC(7617939f) SHA1(3dfd72d9f7021d2b811e5e3777b7e8e963191de6) )
	ROM_LOAD( "maps4.s25",    0xb000, 0x1000, CRC(49fa7824) SHA1(45287767e6ab7f3aa9d6a9d2a7832070a7d2d294) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s26 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s26",    0x8000, 0x1000, CRC(f2e50017) SHA1(a6a1cc26c34bf140b05e8ae6720688c272cb704e) )
	ROM_LOAD( "maps2.s26",    0x9000, 0x1000, CRC(534e9339) SHA1(399eea6827d8b78942683fb7af50539ff57e09eb) )
	ROM_LOAD( "maps3.s26",    0xa000, 0x1000, CRC(92805999) SHA1(f1df6576a587dcd663b39518ab647c9711130a95) )
	ROM_LOAD( "maps4.s26",    0xb000, 0x1000, CRC(b12300a9) SHA1(2ee84d567416b2ca16a4ee5e5a2c6c389c62c2e8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s27 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s27",    0x8000, 0x1000, CRC(f53f1ed9) SHA1(b270d94de2649fc6fdf6db88c7223597151cbfa2) )
	ROM_LOAD( "maps2.s27",    0x9000, 0x1000, CRC(134d64d4) SHA1(736c77e468db7883d5871ca8895c3fdc0b8d0686) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "s27.4a",       0x0020, 0x0100, CRC(4fc62815) SHA1(ccc7a5d081186af0efdf07f0ac80d81451ba6f2f) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s28 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s28",    0x8000, 0x1000, CRC(8bed1880) SHA1(b5bc2dfb3828caa5abcf74dd89337758523d3913) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s28.7f",       0x0000, 0x0020, CRC(e29efa9c) SHA1(a21f169e509a3ba5cde56650a4670d91967523a0) )
	ROM_LOAD( "s28.4a",       0x0020, 0x0100, CRC(8c242882) SHA1(7dd1484133c045c58ce00a84b7234c71dd29ee70) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s29 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s29",    0x8000, 0x1000, CRC(88dd69b9) SHA1(3425a1e7f25a0c9feea2b617109f9b4f9eda012b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s30 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s30",    0x8000, 0x1000, CRC(3584fd52) SHA1(74efe77b63e4c6e2dc60c0e28042da38b8bf16f6) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s30.7f",       0x0000, 0x0020, CRC(93a89e2e) SHA1(585ebf68746f13ef88b8af2f4d63d20ea9ccdad2) )
	ROM_LOAD( "s30.4a",       0x0020, 0x0100, CRC(88d9547a) SHA1(a9a391e42daa823d67ff0127fe01e21705e00af1) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s31 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s21.6j",       0x3000, 0x1000, CRC(cb38a821) SHA1(621491bdc61da325084819c49de79206fe00be6d) )
	ROM_LOAD( "maps1.s31",    0x8000, 0x1000, CRC(48581cc3) SHA1(156fc9360b4cdc68fef56a40c948ffea9bc56bc3) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s31.7f",       0x0000, 0x0020, CRC(82adb5b0) SHA1(880fc08de17e05c93726f77223f8d5f853dfe022) )
	ROM_LOAD( "s31.4a",       0x0020, 0x0100, CRC(613acf6f) SHA1(59ab8835ad3a0096f757ec879bd546037a309d4a) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s32 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s32",    0x8000, 0x1000, CRC(d8b3bfb4) SHA1(cb5981fa6b2294e35dcba7dc77057268a2d5b53b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s32.7f",       0x0000, 0x0020, CRC(e27d1a18) SHA1(ae63512cea3b46fc22c7507d34178501b3c5ab80) )
	ROM_LOAD( "s32.4a",       0x0020, 0x0100, CRC(ad627d98) SHA1(dab07579f4c798477bf9f9be6530fca26c63c370) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s33 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s33",    0x8000, 0x1000, CRC(cd813782) SHA1(03dfee86162be09b2eb97798a385afe42acdb4c6) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s33.7f",       0x0000, 0x0020, CRC(399600bf) SHA1(6d8a25616685c51342d3e8aee8337829d1c0fe17) )
	ROM_LOAD( "s33.4a",       0x0020, 0x0100, CRC(f3689416) SHA1(a401d2c854ecd5e830bc40cde7f14a2c9ccb0f0f) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s34 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s34",    0x8000, 0x1000, CRC(5ad2442d) SHA1(eacc995e9c9282e896659bf8250d7b7f0bc61381) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s34.7f",       0x0000, 0x0020, CRC(9978108f) SHA1(ca4500f56f808f11bf8e96892ffe08afb3bc0467) )
	ROM_LOAD( "s34.4a",       0x0020, 0x0100, CRC(190b4854) SHA1(76b208770f3c681e4bdd64c7089da09a2763c31e) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s35 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s35",    0x8000, 0x1000, CRC(5a91d478) SHA1(b28ab88397d0670857bfa65a8f10c503eba47b55) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.s22",    0xa000, 0x1000, CRC(f42821b0) SHA1(7b1744a399b2105b69c13804d391a9a45a74b637) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s35.7f",       0x0000, 0x0020, CRC(aef83add) SHA1(3991df650749a10189e6c10b33a4a8383c879072) )
	ROM_LOAD( "s35.4a",       0x0020, 0x0100, CRC(db377c41) SHA1(67f67b1808b101171ad71e59865b93b4c8705137) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s36 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s36",    0x8000, 0x1000, CRC(a092015e) SHA1(e936996a693a0693a88ca708f05ec2d7e561f29c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s36.7f",       0x0000, 0x0020, CRC(a7f74729) SHA1(e6f127468f94924eee3337c7c43ac2e2be24fb4b) )
	ROM_LOAD( "s36.4a",       0x0020, 0x0100, CRC(9a64b83b) SHA1(60ce3756924a1a448a4ad04b2eaa7113039b896b) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s37 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s22.6j",       0x3000, 0x1000, CRC(d24028ce) SHA1(afae8d808bd6e15fcfadc969a0f361876a62065a) )
	ROM_LOAD( "maps1.s37",    0x8000, 0x1000, CRC(1ae14081) SHA1(656ed7ae6a3fb4d68a845fca3236273c91618c33) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s37.7f",       0x0000, 0x0020, CRC(6066666f) SHA1(bacb8fb169b430cd9acba38c0012246e977bc3f4) )
	ROM_LOAD( "s37.4a",       0x0020, 0x0100, CRC(acdc393a) SHA1(5cbb1df9f9c2f9d0e666ee8fb27fffed84986242) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s38 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s38",    0x8000, 0x1000, CRC(4d2777d4) SHA1(a725a462a4fecdfce877716ab3fb07b81d674a87) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.s22",    0xa000, 0x1000, CRC(f42821b0) SHA1(7b1744a399b2105b69c13804d391a9a45a74b637) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s38.7f",       0x0000, 0x0020, CRC(674edc19) SHA1(0d1d5d17b5bc0c0833205637437bce28917d2e82) )
	ROM_LOAD( "s38.4a",       0x0020, 0x0100, CRC(0fefca43) SHA1(e431aea84300c9c70171c0b5e54182c36a3f8fb5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s39 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s39",    0x8000, 0x1000, CRC(02edd1da) SHA1(cd97f77b6998410f46d51b2f2195be13fbe0e7eb) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s39.7f",       0x0000, 0x0020, CRC(29cd6f11) SHA1(84f878cdc4df2285632cba22e3472e0eee43c896) )
	ROM_LOAD( "s39.4a",       0x0020, 0x0100, CRC(7b66fa45) SHA1(fb75769c22a7ee2abe44c33307bad2a82e1423dd) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s40 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s40",    0x8000, 0x1000, CRC(e3ff416c) SHA1(87490a284a4fbf3f597b9af4c028feff81584ae1) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s40.7f",       0x0000, 0x0020, CRC(87fa898e) SHA1(33782f5257d03a1e1b587111381f61f534026cd5) )
	ROM_LOAD( "s40.4a",       0x0020, 0x0100, CRC(ba4c0ca8) SHA1(ee4029e257c2b5254e1ace4ed58d2784eae1984d) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s41 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s41",    0x8000, 0x1000, CRC(d0c91516) SHA1(9cdce67c73b150d778f7325af4dffa04f271b969) )
	ROM_LOAD( "maps2.s41",    0x9000, 0x1000, CRC(d080fc6e) SHA1(1e4b03c2cfdff4d8d63ad3cea8ef504dcb50163c) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s41.7f",       0x0000, 0x0020, CRC(4560fc25) SHA1(8ceda21b6dbf4e18016ef168a342b5eba2ac0aa7) )
	ROM_LOAD( "s41.4a",       0x0020, 0x0100, CRC(89092690) SHA1(35c9c9f1946505229513df8a72e02e937bb75865) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s42 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s42",    0x8000, 0x1000, CRC(a4e2440b) SHA1(d5e43f822c63d69c9668eb08d134788ec805315c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s42.7f",       0x0000, 0x0020, CRC(d3e4ab03) SHA1(657bec63bc8c4c64c1d115125a53550f1e5f4515) )
	ROM_LOAD( "s42.4a",       0x0020, 0x0100, CRC(7f4c69e7) SHA1(9d28c895db6f0339f85cbb0fb6a8b529aa3214ec) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s43 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s43",    0x8000, 0x1000, CRC(20142fce) SHA1(64f0f2647babbe7fb432625565661d1f93415a94) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.s43",    0xa000, 0x1000, CRC(1485424a) SHA1(bc16606344d726e87f2108dc26b78ba42f93b668) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s43.7f",       0x0000, 0x0020, CRC(40f00b15) SHA1(f5ef4294e36b8522b3e39e6ef51861356e2f898a) )
	ROM_LOAD( "s43.4a",       0x0020, 0x0100, CRC(250d5ec2) SHA1(cfe0dd2634cba522d0e242d2e5a7581e45143803) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s44 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s44",    0x8000, 0x1000, CRC(6dfad142) SHA1(36f15ba33df562b6ebc40634de3266d90afd8837) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s44.7f",       0x0000, 0x0020, CRC(4a347fdc) SHA1(5d90fe082baccc48c14dc7a34aa096f20b3a809e) )
	ROM_LOAD( "s44.4a",       0x0020, 0x0100, CRC(e7d8f21c) SHA1(37eed42438e76399289f3fe83ee33de6bf604e98) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s45 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s45",    0x8000, 0x1000, CRC(79d5dd5a) SHA1(6f3d5f6a0a061977de2ab7ccae0f335395b176e3) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s45.7f",       0x0000, 0x0020, CRC(7cdeea85) SHA1(c37a63f0fc16321ba82473e74abf97f8742cc86d) )
	ROM_LOAD( "s45.4a",       0x0020, 0x0100, CRC(614deaf8) SHA1(8aded7618f65cf611c9657b1f6d65162add8aada) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s46 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s46",    0x8000, 0x1000, CRC(9356a14f) SHA1(bc17fa914596e0aee0ca5145638c827807fa129c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s46.7f",       0x0000, 0x0020, CRC(dbf2024d) SHA1(fc63afa28bc6219fe76e419b2723900979a5b47f) )
	ROM_LOAD( "s46.4a",       0x0020, 0x0100, CRC(328d01d3) SHA1(d100dfa426914a04a3d1e0dea2fd6b8fa9c2ef5e) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s47 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s47",    0x8000, 0x1000, CRC(437535b2) SHA1(60de7771a15a70b769dcdd30d410ef76d9cfa4e4) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s47.7f",       0x0000, 0x0020, CRC(e166fbb3) SHA1(182e8fa546ebdbc5ccb2d61c693a47b32e1949c5) )
	ROM_LOAD( "s47.4a",       0x0020, 0x0100, CRC(55956331) SHA1(e6b6410893fdbdb4215e89aa6edfa45d5ec69ba1) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s48 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s48",    0x8000, 0x1000, CRC(faf7915f) SHA1(959b59577266099ac0f9b007ccdb834cd3953abd) )
	ROM_LOAD( "maps2.s48",    0x9000, 0x1000, CRC(e3531b47) SHA1(1f0637e646b4ec06681cfb03c6dda9870e582634) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s48.7f",       0x0000, 0x0020, CRC(2cb015b3) SHA1(5fe0d77f539f19265b71eaa7fe9a7484aa78f37b) )
	ROM_LOAD( "s48.4a",       0x0020, 0x0100, CRC(cddf79ef) SHA1(de217f88c4a6d7079c0af2a728a3a98387345141) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s49 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s49",    0x8000, 0x1000, CRC(ec201919) SHA1(4f487dfdc4ef55eedc8b9d3efed02bb50f914aee) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s49.7f",       0x0000, 0x0020, CRC(97233e02) SHA1(6c9115cffd19c885242a4695f1b7ede5ef505133) )
	ROM_LOAD( "s49.4a",       0x0020, 0x0100, CRC(8288fab8) SHA1(508b17642809612a70b87a69661439f66ea17f3b) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s50 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s50",    0x8000, 0x1000, CRC(9bed9e7d) SHA1(d218cd82f1cae0000768b0c36c1502bcd4eb57ae) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s50.7f",       0x0000, 0x0020, CRC(1de71b81) SHA1(022274d76c4a90410d613c46175ba9ee0d4db62a) )
	ROM_LOAD( "s50.4a",       0x0020, 0x0100, CRC(48db91a5) SHA1(5dbcefcce46c90e50a914c2eb803a62ffa8cbec2) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s51 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s51",    0x8000, 0x1000, CRC(fc01b045) SHA1(094c35ad464bea6a8ed355fc7d56fd7b428853a8) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s51.7f",       0x0000, 0x0020, CRC(4791c5db) SHA1(3a44180c449c78be6efd72e56410e9f2b46cae98) )
	ROM_LOAD( "s51.4a",       0x0020, 0x0100, CRC(86f4683a) SHA1(80d7f44681fdec0aa9752a01d6d2bc1dc9a8ca91) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s52 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps2.s23",    0x8000, 0x1000, CRC(688c496c) SHA1(87ed9bc231904b0fedbcd55b3917d9b806b369db) )
	ROM_LOAD( "maps2.s48",    0x9000, 0x1000, CRC(e3531b47) SHA1(1f0637e646b4ec06681cfb03c6dda9870e582634) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s52.7f",       0x0000, 0x0020, CRC(52497aed) SHA1(2f4971d303179b3ff26eed9cd2a1e29153fa33e3) )
	ROM_LOAD( "s52.4a",       0x0020, 0x0100, CRC(04ed9db7) SHA1(c84304266141285f80e79f6459f0424e6592c567) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s53 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s53",    0x8000, 0x1000, CRC(6309745c) SHA1(80367345a35ae35a1829d0a28f2828c9b2cd7c3f) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.s53",    0xa000, 0x1000, CRC(b0340c24) SHA1(8c9bc6203b1239c6b21ebf45afe54e05949b6e7e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s53.7f",       0x0000, 0x0020, CRC(1979f31d) SHA1(57a23c7a998c7418e9d3e25c1ae95b8676471635) )
	ROM_LOAD( "s53.4a",       0x0020, 0x0100, CRC(0acefb22) SHA1(25d7db606c9bb5dfce04cd70cf7ef924dfe93437) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s54 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s54",    0x8000, 0x1000, CRC(831c8e4e) SHA1(773fb53063cc14b5dc010845a069f51a2b345ec6) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s54.7f",       0x0000, 0x0020, CRC(0b03add5) SHA1(4c39d6612df9b73bf7a3d18d84c63998b7ac838d) )
	ROM_LOAD( "s54.4a",       0x0020, 0x0100, CRC(c0785fe6) SHA1(fcac92ba4ba8ad777a86c197b840552f6ccfd116) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s55 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s55",    0x8000, 0x1000, CRC(a8167f3e) SHA1(60bdaffa1ef7b851fea39e8f4f7b3ff5b4d1745a) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s55.7f",       0x0000, 0x0020, CRC(c78e5695) SHA1(dac7430ac599f250b5d19bacdb607b9823d4914b) )
	ROM_LOAD( "s55.4a",       0x0020, 0x0100, CRC(8726b275) SHA1(a094bf980b96bc0a9dc3446d12795694eb12c5d1) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s56 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s56",    0x8000, 0x1000, CRC(94fa662f) SHA1(e0358c270e9bd59855c10b6e4fd474c11291d2b7) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s56.7f",       0x0000, 0x0020, CRC(d71ab5fb) SHA1(53e41abefb535043fa99242a54e16c8cf4854302) )
	ROM_LOAD( "s56.4a",       0x0020, 0x0100, CRC(18214488) SHA1(4e7b916e0747d2129a7c2872fccc2b448c6bebfb) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s57 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s57",    0x8000, 0x1000, CRC(87af8fb9) SHA1(a2232c3c904eddc16a3b241ac508ab7b24ec41c7) )
	ROM_LOAD( "maps2.s57",    0x9000, 0x1000, CRC(4360db73) SHA1(1aab3eac95a499d7946393f5a4941ce7946a42fb) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s57.7f",       0x0000, 0x0020, CRC(62ea96fa) SHA1(5540473c9a5f48b9b23221a77d138e14078e3884) )
	ROM_LOAD( "s57.4a",       0x0020, 0x0100, CRC(f53aca43) SHA1(ad14d7821a4f4d9a2d5de48272fc6e52c272f004) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s58 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s58",    0x8000, 0x1000, CRC(eb2ade9d) SHA1(0ae61a7172d687db349b148ff24dd515bb38326b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s58.7f",       0x0000, 0x0020, CRC(e6e605dc) SHA1(de2934bb0846fbc6293e739c089cccb29a9a2822) )
	ROM_LOAD( "s42.4a",       0x0020, 0x0100, CRC(7f4c69e7) SHA1(9d28c895db6f0339f85cbb0fb6a8b529aa3214ec) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s59 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps4.s22",    0x8000, 0x1000, CRC(9e7b539d) SHA1(e295c424517bd343d17fff084bd2ab0473acd41b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s59.7f",       0x0000, 0x0020, CRC(fc43438d) SHA1(ae056111b4324facdd5ddaf4a701e957b80a8300) )
	ROM_LOAD( "s59.4a",       0x0020, 0x0100, CRC(9eb8e345) SHA1(ca2ee309c151ff9f3affa09cf9ba3f3f9d27f9e1) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s60 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s60",    0x8000, 0x1000, CRC(38808253) SHA1(1830d72eadfa8954b0442a64188c16bb50ef247b) )
	ROM_LOAD( "maps2.s60",    0x9000, 0x1000, CRC(cf67308d) SHA1(321773d41d547071c3dd879c18b40c6ca8525102) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s60.7f",       0x0000, 0x0020, CRC(f4870634) SHA1(a9604dfbaa3c677829ec0274d8f38c78bf7badff) )
	ROM_LOAD( "s60.4a",       0x0020, 0x0100, CRC(e9631103) SHA1(0c565e0d9bd5229afd1ed0f0e47c363d13391fae) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s61 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "s61.6e",       0x0000, 0x1000, CRC(50a44885) SHA1(1e68345ea69134b0be08141a885645e7108138f2) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s61.6j",       0x3000, 0x1000, CRC(8c733c5b) SHA1(7bb007db821ef3a0a6997586093c86d3dbe93c29) )
	ROM_LOAD( "maps1.s61",    0x8000, 0x1000, CRC(0949455b) SHA1(f3233a3dc93e4ff216be53f5db47f9729b6bf974) )
	ROM_LOAD( "maps2.s61",    0x9000, 0x1000, CRC(c840132c) SHA1(e0586f9931bae47b770d4fa61046e70dbb06b2dd) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s61.5e",       0x0000, 0x1000, CRC(4fd1a79c) SHA1(cecee1e24d9d6145c3caff8fb09578e23b27cc45) )
	ROM_LOAD( "s61.5f",       0x1000, 0x1000, CRC(a8954742) SHA1(38d6878e9fcf8ea61ef79dac21b695ba61f48736) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s61.7f",       0x0000, 0x0020, CRC(e4e13e7b) SHA1(3fb83e27cef0f1ed3288ba56ad9d6bb78d3bf8ed) )
	ROM_LOAD( "s61.4a",       0x0020, 0x0100, CRC(e4df45f6) SHA1(63f56a28aed2feefdf70e2cac147b9799d658b93) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s62 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s62",    0x8000, 0x1000, CRC(6d8ceec9) SHA1(efe18b23b3ae17bf0f09a480e01280e8c700d890) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s62.7f",       0x0000, 0x0020, CRC(0a648efa) SHA1(f2bc229f8022f0df38c01397198529c2030238fc) )
	ROM_LOAD( "s62.4a",       0x0020, 0x0100, CRC(44177a56) SHA1(0ae6a82764a6a1f43e9ad4b61a64db8044caf487) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s63 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s63",    0x8000, 0x1000, CRC(8210f432) SHA1(39d64199be51b6cc90825cb3e95d0072f03b05f2) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s63.7f",       0x0000, 0x0020, CRC(af657033) SHA1(98606951f1055c9dc75e000c7563e89bce915c97) )
	ROM_LOAD( "s63.4a",       0x0020, 0x0100, CRC(a55ea0db) SHA1(fafab0cb853c37b561c8d0150da54b699911a504) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s64 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s64",    0x8000, 0x1000, CRC(8101f8ff) SHA1(5cc73fca5f27a53210de4d67f32e44747e488d0a) )
	ROM_LOAD( "maps2.s64",    0x9000, 0x1000, CRC(0d7dd47e) SHA1(af8d6ce2dfec6343629a017771eb25703f47945d) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s64.7f",       0x0000, 0x0020, CRC(fed7d96e) SHA1(84aaaf20df52390932682312b22e098cbcd21d3f) )
	ROM_LOAD( "s64.4a",       0x0020, 0x0100, CRC(ef06041b) SHA1(28636f16429c5d633db06906fd4abc0135b553af) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s65 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s65",    0x8000, 0x1000, CRC(90aba2c6) SHA1(dcc0bf60497d1e05b3409ca6c0d5e66293a6e0f5) )
	ROM_LOAD( "maps2.s27",    0x9000, 0x1000, CRC(134d64d4) SHA1(736c77e468db7883d5871ca8895c3fdc0b8d0686) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s65.7f",       0x0000, 0x0020, CRC(02ee19fd) SHA1(85c279341323726af91e0ebbe6f6f601261b2553) )
	ROM_LOAD( "s43.4a",       0x0020, 0x0100, CRC(250d5ec2) SHA1(cfe0dd2634cba522d0e242d2e5a7581e45143803) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s66 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s66",    0x8000, 0x1000, CRC(7bfe1f07) SHA1(7a50eb23a25faa959daaa174a9c03671562ca944) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s66.7f",       0x0000, 0x0020, CRC(9129da4c) SHA1(0c9eb69ade5b326b5c3aee08b824e7b5dea129c7) )
	ROM_LOAD( "s66.4a",       0x0020, 0x0100, CRC(27debe8e) SHA1(aed785e0d19ba98838021563fccf4bd85ace77af) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s67 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s67",    0x8000, 0x1000, CRC(029ebe11) SHA1(669365b245c2ae25121851b204e474786414adf7) )
	ROM_LOAD( "maps2.s67",    0x9000, 0x1000, CRC(ec08217e) SHA1(de9f2b8d0c120fb1c77fc641356df7088b959a1e) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s67.7f",       0x0000, 0x0020, CRC(8d0ccc9a) SHA1(bc5f38d3bc65c746abba193e6ab66d35e61ae2ab) )
	ROM_LOAD( "s67.4a",       0x0020, 0x0100, CRC(10a65eae) SHA1(4c129a0d2963563438a45d557c8e417cb657a84a) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s68 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s68",    0x8000, 0x1000, CRC(9f57aba1) SHA1(340cb93efd3a3d23c514ed78417b372e2de632ed) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s68.7f",       0x0000, 0x0020, CRC(e7c11456) SHA1(0893674a21d1e657546c38261840f33e87dfb023) )
	ROM_LOAD( "s68.4a",       0x0020, 0x0100, CRC(89e451fb) SHA1(4b4905cd6f04828deb6bfb3e8d8991000de7be69) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s69 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s69",    0x8000, 0x1000, CRC(a056f280) SHA1(593eff3188c53f67b9b8a9fb3dacdfe210585360) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s69.7f",       0x0000, 0x0020, CRC(3499816d) SHA1(7e55b3883cd5ff6a73c9dc4e9fa881e25bb10d26) )
	ROM_LOAD( "s69.4a",       0x0020, 0x0100, CRC(9f47989a) SHA1(88010b504d6875dda361b771e94ed717d97092d5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s70 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s25.6j",       0x3000, 0x1000, CRC(bb54b431) SHA1(09557e1098fb9762bf44d4b25acd3088130fecbd) )
	ROM_LOAD( "maps1.s70",    0x8000, 0x1000, CRC(91e0ffdd) SHA1(94fa25bf8305959a6871943e4f4fc55ae1fda23e) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s70.7f",       0x0000, 0x0020, CRC(c3859f5d) SHA1(1600e1db887932d22faf092d5036548221fdb1be) )
	ROM_LOAD( "s70.4a",       0x0020, 0x0100, CRC(b84be943) SHA1(789e083f127f05f2ded635a051380897785de514) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s71 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s71",    0x8000, 0x1000, CRC(e31b4ff2) SHA1(87c410176ca4e50382acff6c7acfb1c0b1b6e5e6) )
	ROM_LOAD( "maps2.s22",    0x9000, 0x1000, CRC(e49cbfe7) SHA1(74c429a5388b2dc419fdecf4e2e10b4c434b3856) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s71.7f",       0x0000, 0x0020, CRC(64fab468) SHA1(635c57e8fddd2bfa238567cdf4f9898d8fe900e0) )
	ROM_LOAD( "s71.4a",       0x0020, 0x0100, CRC(2c27cc32) SHA1(d0f01a16d092d0f82ec9130af5940fafb96155ee) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s72 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s72",    0x8000, 0x1000, CRC(04e97c0d) SHA1(bc08be8268973896c8447acbecbfb679be2cb4fe) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s72.7f",       0x0000, 0x0020, CRC(a5173d8e) SHA1(5710c8140d448dfaa68446425ee0d63fd12b8931) )
	ROM_LOAD( "s43.4a",       0x0020, 0x0100, CRC(250d5ec2) SHA1(cfe0dd2634cba522d0e242d2e5a7581e45143803) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s73 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps3.s25",    0x8000, 0x1000, CRC(7617939f) SHA1(3dfd72d9f7021d2b811e5e3777b7e8e963191de6) )
	ROM_LOAD( "maps2.s73",    0x9000, 0x1000, CRC(c559ad02) SHA1(67140cf089756e165dedf7cda97a905dea13527b) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s73.7f",       0x0000, 0x0020, CRC(c18e3e9a) SHA1(49f698aa6a20134521b61bfb30dc3a28a81408b5) )
	ROM_LOAD( "s73.4a",       0x0020, 0x0100, CRC(870642cf) SHA1(bcf73e4afe11d02d2b80547094be39ab2cd474e1) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s74 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s74",    0x8000, 0x1000, CRC(0e862c3a) SHA1(a1f9dadf80926f4bacc8f8ce4c8fd69595f7c0f8) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.s74",    0xa000, 0x1000, CRC(d00de18f) SHA1(51dc6f4fe4e0816f067a7be5be825047966f4f7f) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s74.7f",       0x0000, 0x0020, CRC(1a3db875) SHA1(ca41af490ed02f4b9bbb84bbd400797677486668) )
	ROM_LOAD( "s74.4a",       0x0020, 0x0100, CRC(103afc30) SHA1(a8b7aa9bb425026558ea0abc16405522d6ec6e5f) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s75 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s75",    0x8000, 0x1000, CRC(245ab147) SHA1(62b47a899471e4d3258b7192e8278712bcffa303) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s13.5e",       0x0000, 0x1000, CRC(f8cdece3) SHA1(163a244c457a6a42b5226226dca3956c929ad905) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s75.7f",       0x0000, 0x0020, CRC(97511b32) SHA1(5254615d448bdd9d2ae7cf4c768e2ecd4889fc47) )
	ROM_LOAD( "s75.4a",       0x0020, 0x0100, CRC(1865c732) SHA1(1a1d50d4b558bc7143ed333e8d7d7e7a917cd5e9) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s76 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s76",    0x8000, 0x1000, CRC(d2f88d9a) SHA1(b7d27cf4b6ca1e6840758cd3dd02c21bb8f72b24) )
	ROM_LOAD( "maps2.s76",    0x9000, 0x1000, CRC(d0686d16) SHA1(86655af10fa3a1e32f04fcf2f96dc6a6373b35ab) )
	ROM_LOAD( "maps3.s76",    0xa000, 0x1000, CRC(47f16fc1) SHA1(0e0ddc0b014bd32335c1ac7d2ce32e9783d7323e) )
	ROM_LOAD( "maps4.s76",    0xb000, 0x1000, CRC(0694b229) SHA1(c5939a084b0e413c87a446aa4ad78624b6f2e170) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s77 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s77.6j",       0x3000, 0x1000, CRC(352edbbc) SHA1(ea1dbc7d644507050da714b0034442316b62e070) )
	ROM_LOAD( "maps1.s77",    0x8000, 0x1000, CRC(bd3fa185) SHA1(7009cbc6e6a0abf5fadfc45b825d184b9961358c) )
	ROM_LOAD( "maps2.s77",    0x9000, 0x1000, CRC(076a284d) SHA1(55fcceed6293003d228c2919e640ca57668611ea) )
	ROM_LOAD( "maps3.s77",    0xa000, 0x1000, CRC(a2f5a743) SHA1(1f6d4ed479c7d3c89d851efe47b4ffc38307b6e8) )
	ROM_LOAD( "maps4.s77",    0xb000, 0x1000, CRC(7657044c) SHA1(b6460d7a00dbf2a7a1a67089b11f0b356b1767a2) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s78 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s78",    0x8000, 0x1000, CRC(270ba4b5) SHA1(265fd4149a0da78f2fd18d17c8fe1e7408938c73) )
	ROM_LOAD( "maps2.s78",    0x9000, 0x1000, CRC(fd06c2ff) SHA1(13cc32ce5da5ebb37565d410a4df9ddde54b6b42) )
	ROM_LOAD( "maps3.s78",    0xa000, 0x1000, CRC(09967caa) SHA1(f0fab2714ea7fce7b7b1292db1f486ca77402f4a) )
	ROM_LOAD( "maps4.s78",    0xb000, 0x1000, CRC(372fd89a) SHA1(8a717c38697f30a2618091e684f2ecd2b94005ed) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s79 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s79",    0x8000, 0x1000, CRC(86f2ce0c) SHA1(37046c490ee86ab4c62da4ca8bfb98b99e6ec7b9) )
	ROM_LOAD( "maps2.s79",    0x9000, 0x1000, CRC(3d34deb8) SHA1(2855cf1cf74a4af427bf74ee6219204aaf0c2ecd) )
	ROM_LOAD( "maps3.s79",    0xa000, 0x1000, CRC(af04546c) SHA1(ea592269a47c5b9b855ed40e075aae141c92491d) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s80 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps4.s76",    0x8000, 0x1000, CRC(0694b229) SHA1(c5939a084b0e413c87a446aa4ad78624b6f2e170) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s80.7f",       0x0000, 0x0020, CRC(11126fd6) SHA1(39dd2c551411774aded3a2134f6865a8ef15a9b4) )
	ROM_LOAD( "s80.4a",       0x0020, 0x0100, CRC(a721abe1) SHA1(4e7134f34a49dc191fd8f178243ee6e5037ddda5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s81 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s81",    0x8000, 0x1000, CRC(89bdc714) SHA1(7281681d1f1aa26734518dedcb3130bcc170556c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s81.7f",       0x0000, 0x0020, CRC(73bb4e43) SHA1(4f4e38411e1c856edb8a2677bbc97a0a7b27742b) )
	ROM_LOAD( "s81.4a",       0x0020, 0x0100, CRC(c7a7abdd) SHA1(ce9b6ae57d9b28f1cc020c3a5f88652634e2d318) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s82 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s82",    0x8000, 0x1000, CRC(f579c38d) SHA1(04350d18c9ddc0f393b9bbae159421914fd309f7) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s82.7f",       0x0000, 0x0020, CRC(f084d073) SHA1(707feba56218efb21973077fbd69242291582cdf) )
	ROM_LOAD( "s82.4a",       0x0020, 0x0100, CRC(e5dd52b1) SHA1(66140cd740e2a5ddb59dc94e631161fdd43a7d13) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s83 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s83",    0x8000, 0x1000, CRC(ec44cf70) SHA1(3f4ef0f08e8637abb3ba1d3d93fda1d7a01fc5a8) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s83.7f",       0x0000, 0x0020, CRC(6b5096de) SHA1(7c4bff65176baf8866caf3ceae292e822c785002) )
	ROM_LOAD( "s83.4a",       0x0020, 0x0100, CRC(0156f604) SHA1(8c7101e66c7ae8d82bf9596756471305bf6e8efc) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s84 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s84",    0x8000, 0x1000, CRC(04b1f4dd) SHA1(43ea2ce7ae9a8a5c63e22eaa93453cd8425be043) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "s84.4a",       0x0020, 0x0100, CRC(2d92831e) SHA1(824bc1ffec1d40c1e88a1921061c83877c1b6324) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s85 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s85",    0x8000, 0x1000, CRC(bc9bb1ea) SHA1(62029f00aeab051838c30a8b19bef8582d3820a0) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s34.7f",       0x0000, 0x0020, CRC(9978108f) SHA1(ca4500f56f808f11bf8e96892ffe08afb3bc0467) )
	ROM_LOAD( "s85.4a",       0x0020, 0x0100, CRC(133219b9) SHA1(560eca0a94e8ee551f09848e9084652612dffebb) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s86 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s86",    0x8000, 0x1000, CRC(64e8bd4f) SHA1(33acabe4d113909dfb67dea2440f9c89ed3d0c8f) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s86.7f",       0x0000, 0x0020, CRC(42917312) SHA1(d13964799a35f8bf03e10c63b70adbc7387f0c38) )
	ROM_LOAD( "s86.4a",       0x0020, 0x0100, CRC(913ade52) SHA1(c4d1ee3d9900fa16e76c04a17a2f4a7671db7c57) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s87 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s87",    0x8000, 0x1000, CRC(df70afb4) SHA1(ea5360a5d56e4649bc1e1fa0d64dbeae1e6f4d88) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s87.7f",       0x0000, 0x0020, CRC(55685860) SHA1(089e481b47d6624cc058698b27fb22371e9ab673) )
	ROM_LOAD( "s87.4a",       0x0020, 0x0100, CRC(3ffc2b75) SHA1(9d3a7fddcf2f8bd750da7e79e9ab9c5fe2656d34) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s88 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s88",    0x8000, 0x1000, CRC(934fbec0) SHA1(af2fe9ad7c64b8138d32fe768de68a789cc40fa7) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "s88.4a",       0x0020, 0x0100, CRC(5bb05640) SHA1(7058989b6d6ac1a2818f89d5a98b488c9afb0a53) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s89 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s89",    0x8000, 0x1000, CRC(9419d4a9) SHA1(c46139810177b1b6ac09ecfec89339dd7947654c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s89.7f",       0x0000, 0x0020, CRC(dcd15f1e) SHA1(0c715d861c3613555f6e31b2cf896f76122a291e) )
	ROM_LOAD( "s89.4a",       0x0020, 0x0100, CRC(823e1ec5) SHA1(07f747dfe0493bf01659225f76bd6da115709fa4) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s90 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s90",    0x8000, 0x1000, CRC(69ec89e1) SHA1(d5a439156b2cda41275a396519bb45b0f1d00e55) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s90.7f",       0x0000, 0x0020, CRC(335a74ff) SHA1(337baf8167b53781051398536deaa523e1de3cbb) )
	ROM_LOAD( "s43.4a",       0x0020, 0x0100, CRC(250d5ec2) SHA1(cfe0dd2634cba522d0e242d2e5a7581e45143803) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s91 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s91",    0x8000, 0x1000, CRC(5ea6cad7) SHA1(c31aa7382922b4a07613ff06c591ddd9d654d7e0) )
	ROM_LOAD( "maps2.s91",    0x9000, 0x1000, CRC(20099178) SHA1(51b69e040911259aa2525b2d5988f1f195b2b82b) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "s91.4a",       0x0020, 0x0100, CRC(44df230b) SHA1(37ad03af4be0f29ea6ffc438a8883d4affc27732) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s92 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s24.6j",       0x3000, 0x1000, CRC(d780169c) SHA1(4caa6f8eedb610dcaa6fce99899084469063c717) )
	ROM_LOAD( "maps1.s92",    0x8000, 0x1000, CRC(f4591ebe) SHA1(50ccb4cb404def76864e52928cb210fa8307a294) )
	ROM_LOAD( "maps2.s92",    0x9000, 0x1000, CRC(a3082df1) SHA1(26a68a485486ae559b0868a5387ad0fe7968396f) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s44.7f",       0x0000, 0x0020, CRC(4a347fdc) SHA1(5d90fe082baccc48c14dc7a34aa096f20b3a809e) )
	ROM_LOAD( "s44.4a",       0x0020, 0x0100, CRC(e7d8f21c) SHA1(37eed42438e76399289f3fe83ee33de6bf604e98) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s93 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s93",    0x8000, 0x1000, CRC(bdddee55) SHA1(3ef53c78ec26ddb1620876fc2fe580bdb5fccb63) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s93.7f",       0x0000, 0x0020, CRC(f771c2d5) SHA1(448c76bc284804c5b0905d106c196a8c0d499e22) )
	ROM_LOAD( "s93.4a",       0x0020, 0x0100, CRC(b5409c2e) SHA1(e28f577fa958057425758fc28b786a765d76bd6d) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s94 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s94",    0x8000, 0x1000, CRC(76aa6c63) SHA1(bc0a463ae6b4f2dd93579df696cf37b23786165c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s94.7f",       0x0000, 0x0020, CRC(15d061e5) SHA1(a574d5716caee8b7c646bd1fc8bf4f3798689c94) )
	ROM_LOAD( "s94.4a",       0x0020, 0x0100, CRC(11f19072) SHA1(bbcd20b4b745b4632c3901eb80762b7be9bfe176) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s95 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s95",    0x8000, 0x1000, CRC(48d41de6) SHA1(957e2e7c64513dd0e55141e07edef7a924f8ec2f) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s89.7f",       0x0000, 0x0020, CRC(dcd15f1e) SHA1(0c715d861c3613555f6e31b2cf896f76122a291e) )
	ROM_LOAD( "s95.4a",       0x0020, 0x0100, CRC(8a930222) SHA1(875e25d146b6ade3b5dcf0dcbb663ac6c31619e3) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s96 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s96",    0x8000, 0x1000, CRC(2caabc23) SHA1(e9d00caca3f27824dabcc3861a7d7c4d9b17442d) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s96.7f",       0x0000, 0x0020, CRC(b972afae) SHA1(f56b1f13d5da386751a542ce69e76bfe60dcb1b4) )
	ROM_LOAD( "s96.4a",       0x0020, 0x0100, CRC(722ab965) SHA1(0edd341e7cb05bd6a7e53873c314b4b2baf9fc83) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s97 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s97",    0x8000, 0x1000, CRC(40e5c32b) SHA1(35707e7c9ea096b9f2ab02a7e56105282c1a1de1) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s97.7f",       0x0000, 0x0020, CRC(08739ccd) SHA1(5fa9b33ec300858d8618c735695155239a05a016) )
	ROM_LOAD( "s97.4a",       0x0020, 0x0100, CRC(48a1f395) SHA1(ed5c64c0e42786db7eee188e234eba9ead86889b) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s98 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s98",    0x8000, 0x1000, CRC(dc3e0d3f) SHA1(23eda1f01687c3773bd4a833b0c06fb7fa2b5877) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s98.7f",       0x0000, 0x0020, CRC(3d3f9ad6) SHA1(9291815d100fd77665d947fbdfd95f6381c5d4e4) )
	ROM_LOAD( "s98.4a",       0x0020, 0x0100, CRC(71e17fb6) SHA1(9bfa5f6416ffcee4c48e0be98470ec644200d3d8) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s99 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.s99",    0x8000, 0x1000, CRC(78d7901a) SHA1(64bc4ef6b50f3dad00694a188717471554b44d97) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s99.7f",       0x0000, 0x0020, CRC(55fcd3d8) SHA1(7fddbc5e6a037fdc5cea0b09d1d135ada7ebf8dc) )
	ROM_LOAD( "s99.4a",       0x0020, 0x0100, CRC(fd7414a2) SHA1(267d7f6afd1d8bf28b665167610d8a9f92cac8e7) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s100 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.100",    0x8000, 0x1000, CRC(55943ee6) SHA1(aa101cf5817c8da1c5f45206537985480f882bf8) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s49.7f",       0x0000, 0x0020, CRC(97233e02) SHA1(6c9115cffd19c885242a4695f1b7ede5ef505133) )
	ROM_LOAD( "s49.4a",       0x0020, 0x0100, CRC(8288fab8) SHA1(508b17642809612a70b87a69661439f66ea17f3b) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s101 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.101",    0x8000, 0x1000, CRC(42ca4522) SHA1(12e56735a0a1cff6b685b3ce4c8748d75621c873) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s101.7f",      0x0000, 0x0020, CRC(c8ff6101) SHA1(9f0b588165148919ddb19d09557c0696d08a5272) )
	ROM_LOAD( "s101.4a",      0x0020, 0x0100, CRC(63f3a781) SHA1(80a2e1b6eed76a559624038fb1a5490d9fdc6fc7) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s102 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.102",    0x8000, 0x1000, CRC(aaa07099) SHA1(a70fe2f758c43cbc7492921c06d4bce73edadb1b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s04.7f",       0x0000, 0x0020, CRC(933f85e1) SHA1(d9ecab6272db185a14f488a8f88a6ff20d570cfd) )
	ROM_LOAD( "s102.4a",      0x0020, 0x0100, CRC(b8638893) SHA1(2dadcf3203a727a7d0f1c64e4d92cf14008b5dce) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s103 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.103",    0x8000, 0x1000, CRC(49302177) SHA1(01810355d181637ca0bf8eada7d514e4b7d96190) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s103.7f",      0x0000, 0x0020, CRC(f3ef2a49) SHA1(01d88d346b50be09f0de7ce31ad9ab927eed0619) )
	ROM_LOAD( "s103.4a",      0x0020, 0x0100, CRC(21f122b2) SHA1(142092b9f6bd67cf89748822dff85136ab86dc1d) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s104 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.104",    0x8000, 0x1000, CRC(bcfddb29) SHA1(1c58d6d07a8024c6687dc6d9116dfbc71e6b0e8b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s104.7f",      0x0000, 0x0020, CRC(49c10b61) SHA1(983263b311976a35d65eb7563740eec7e73af6ad) )
	ROM_LOAD( "s104.4a",      0x0020, 0x0100, CRC(9f7c506d) SHA1(eea32e7dafbde2b74155b475385d46817a4347ff) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s105 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.105",    0x8000, 0x1000, CRC(aaa2aa2f) SHA1(c764257fc25fa243748675ca04e0798cecb4d7db) )
	ROM_LOAD( "maps2.105",    0x9000, 0x1000, CRC(d1c75665) SHA1(7c7f28570fc7071566becaf376125822ef396352) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s56.7f",       0x0000, 0x0020, CRC(d71ab5fb) SHA1(53e41abefb535043fa99242a54e16c8cf4854302) )
	ROM_LOAD( "s56.4a",       0x0020, 0x0100, CRC(18214488) SHA1(4e7b916e0747d2129a7c2872fccc2b448c6bebfb) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s106 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.106",    0x8000, 0x1000, CRC(094007f2) SHA1(5e7423e07b81ea77a92f45b5b2cb493321ab315b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s106.7f",      0x0000, 0x0020, CRC(700f470e) SHA1(1259f70b4921365b17ad73438aee86da2472ffd5) )
	ROM_LOAD( "s31.4a",       0x0020, 0x0100, CRC(613acf6f) SHA1(59ab8835ad3a0096f757ec879bd546037a309d4a) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s107 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.107",    0x8000, 0x1000, CRC(6c5d4b51) SHA1(3a60c0778dbf3387cd19427166d791d48228c0cc) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s107.7f",      0x0000, 0x0020, CRC(2e10a9fe) SHA1(3676f5659ea32b18af199230c30c2bc6e50ba454) )
	ROM_LOAD( "s107.4a",      0x0020, 0x0100, CRC(aee3fcb6) SHA1(d2d11c3f1c93f7abe75c0f9a142f466b3d77b3f4) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s108 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.108",    0x8000, 0x1000, CRC(1fcb1b9d) SHA1(cb456e49cffeeb417cb6cef692e02f028b411155) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s108.7f",      0x0000, 0x0020, CRC(cb949b3b) SHA1(4523a81732e1b2095fa07c76d6da0c3d7c6ca556) )
	ROM_LOAD( "s108.4a",      0x0020, 0x0100, CRC(0b27b737) SHA1(a1887025766c8a06a7b1e1cb083b86ea0676407a) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s109 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.109",    0x8000, 0x1000, CRC(a04c2d07) SHA1(287cf3881aabd4cc5a34313b0521e9f7cfe194f0) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "s109.4a",      0x0020, 0x0100, CRC(f9227d35) SHA1(1d1bb54ab4c04c8e9a9572f16df7c8dcfbc6f3d4) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s110 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.110",    0x8000, 0x1000, CRC(5b984cc9) SHA1(2d898777086bae6228e9c1292c90fb8486d46123) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s59.7f",       0x0000, 0x0020, CRC(fc43438d) SHA1(ae056111b4324facdd5ddaf4a701e957b80a8300) )
	ROM_LOAD( "s59.4a",       0x0020, 0x0100, CRC(9eb8e345) SHA1(ca2ee309c151ff9f3affa09cf9ba3f3f9d27f9e1) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s111 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.111",    0x8000, 0x1000, CRC(05926679) SHA1(2a692a63fbfc9ea11db00017f46efaaaefcc77a6) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s111.7f",      0x0000, 0x0020, CRC(56fcea68) SHA1(6e0d60944a05bc46ac138ff2af36e23fd064d47a) )
	ROM_LOAD( "s111.4a",      0x0020, 0x0100, CRC(cf3f4123) SHA1(e25443740d0ad247a1667bac72bc995112c1d137) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s112 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.112",    0x8000, 0x1000, CRC(4a7ccc41) SHA1(710651e3c183d6daaa025d25a61772bf074d1ffe) )
	ROM_LOAD( "maps2.112",    0x9000, 0x1000, CRC(569d2b1b) SHA1(f1415d04e225e1ef3f962ae64164a84717da2d7a) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s112.7f",      0x0000, 0x0020, CRC(a08a575d) SHA1(1ba9176b543c8e5bd53bf6d2fd3da8fb999d3681) )
	ROM_LOAD( "s112.4a",      0x0020, 0x0100, CRC(81a6bba1) SHA1(495ba898b1c7889b90e2048247d6b84f42bea248) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s113 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.113",    0x8000, 0x1000, CRC(6da38d76) SHA1(3cbe9ce6ca9582a97eaeffed85ffbd7c96236821) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s22.5e",       0x0000, 0x1000, CRC(9be20ee1) SHA1(3fa600a9ba21683ed6ce950e02529274c981e3d7) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s66.7f",       0x0000, 0x0020, CRC(9129da4c) SHA1(0c9eb69ade5b326b5c3aee08b824e7b5dea129c7) )
	ROM_LOAD( "s66.4a",       0x0020, 0x0100, CRC(27debe8e) SHA1(aed785e0d19ba98838021563fccf4bd85ace77af) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s114 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.114",    0x8000, 0x1000, CRC(d5b7ff6c) SHA1(672b7855bb7ddfd4ea9bb74420a8a48bc4e6fe80) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s114.5e",      0x0000, 0x1000, CRC(e20c4c44) SHA1(d2b846be5389b655b1ba269dc4f98fcc942fc3f0) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s114.7f",      0x0000, 0x0020, CRC(3647864a) SHA1(7f9a8172906038ffaa48b18f76bccdd94594be31) )
	ROM_LOAD( "s114.4a",      0x0020, 0x0100, CRC(a8ed779e) SHA1(705557fdfff92988a918753933e2edf918b10e14) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s115 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.115",    0x8000, 0x1000, CRC(66e1d935) SHA1(ee54938cc85b95091adce1bce56e9d1da9497fee) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s115.7f",      0x0000, 0x0020, CRC(96838baf) SHA1(d065dbf2e9fa3d79f1a7177f92e9f4ca344e8c49) )
	ROM_LOAD( "s115.4a",      0x0020, 0x0100, CRC(c67030de) SHA1(d5e1a3ddd1378766eb4525dc5a076a02265dd79a) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s116 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.116",    0x8000, 0x1000, CRC(acade442) SHA1(f9f96220fa954c3971513758d07ae8b9fa5548f2) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s116.7f",      0x0000, 0x0020, CRC(c55089e9) SHA1(186bb0a9b92a5cf34f16aad12dd3e3b111f31b8f) )
	ROM_LOAD( "s116.4a",      0x0020, 0x0100, CRC(387eb6e6) SHA1(e534aa30493b413d9065190a523b5dfade62cf41) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s117 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s77.6j",       0x3000, 0x1000, CRC(352edbbc) SHA1(ea1dbc7d644507050da714b0034442316b62e070) )
	ROM_LOAD( "maps1.117",    0x8000, 0x1000, CRC(76056d7e) SHA1(23f92a197bf5bb365ba68e4cdb3df9d6f8497d87) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s117.7f",      0x0000, 0x0020, CRC(7b775b92) SHA1(daa1ee428d6d262017b62d2b1d7de8d1bc9740c1) )
	ROM_LOAD( "s117.4a",      0x0020, 0x0100, CRC(296c67e7) SHA1(5765911e27472967bdbcdf179372718fc497ea0f) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s118 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.118",    0x8000, 0x1000, CRC(a4967aa4) SHA1(b9ba19f27e9ce8ec220b788c21ef25f41a2fe319) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s118.7f",      0x0000, 0x0020, CRC(f056b868) SHA1(46c662393c2051f66e500e3f46e96f14018c03ee) )
	ROM_LOAD( "s118.4a",      0x0020, 0x0100, CRC(02f6fe72) SHA1(020555202bf5d7c537d3e900b778876e9724838f) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s119 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.119",    0x8000, 0x1000, CRC(0d336812) SHA1(22a4d839368508229d4698146ac8bbdd9847ba9c) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s119.7f",      0x0000, 0x0020, CRC(7ca2b5a0) SHA1(5ef684a5a8b72cecda23fa796ad75838c066be1f) )
	ROM_LOAD( "s119.4a",      0x0020, 0x0100, CRC(c0630339) SHA1(6ce0db5c8714b54226fe93f1a7a92c7dfe71f960) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s120 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "s120.6e",      0x0000, 0x1000, CRC(7a539652) SHA1(abe64097ae7a99e1b967a7602886ac20e0896f3e) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s120.6j",      0x3000, 0x1000, CRC(be912055) SHA1(74b3d9ccac22f90ca4a71e30656dfbc96182c79f) )
	ROM_LOAD( "maps1.120",    0x8000, 0x1000, CRC(f7684bd3) SHA1(58f3b3af4492c7b996fa01cbf2cc2ec6fedbb630) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s120.5e",     0x0000, 0x1000, CRC(aceb2c4d) SHA1(6c0b28463e48c2da4b9d4b62c32724f3c487537e) )
	ROM_LOAD( "s120.5f",     0x1000, 0x1000, CRC(c9944f76) SHA1(50505718064749de0b2c528af6c4dc6be228942d) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s120.7f",      0x0000, 0x0020, CRC(3bc611e0) SHA1(aa68ef5aa347d1169bc999c970b9bce12aedeed1) )
	ROM_LOAD( "s120.4a",      0x0020, 0x0100, CRC(950d0f86) SHA1(37e4f01e28d3a1823d49b60ab2e5614b6e26a3a6) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s121 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.121",    0x8000, 0x1000, CRC(43ecf2e0) SHA1(ddc8dc1d082ec628d01195fa61ad27f8bc5e1c91) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s121.7f",      0x0000, 0x0020, CRC(af6de1c0) SHA1(6361a1a5b037487d9a78ca92d21515a34edd1fd5) )
	ROM_LOAD( "s121.4a",      0x0020, 0x0100, CRC(4007af66) SHA1(c0d34337cd06c35f6470c76e54279eb46c14c585) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s122 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "s122.6e",      0x0000, 0x1000, CRC(1e174278) SHA1(01e4ed2fe6d1f48753a8782ff1aeef3a01bcc16a) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "s122.6j",      0x3000, 0x1000, CRC(a671865e) SHA1(8b38ec84a3ba16ad2775f52126351fe22b1b7f28) )
	ROM_LOAD( "maps1.122",    0x8000, 0x1000, CRC(40239835) SHA1(2b95e6b0e0d23ed26f38735593391a3ba7a76756) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s122.5e",      0x0000, 0x1000, CRC(1a5ee8f9) SHA1(bcd995f7324e9136733e57415ad2e59182db4bde) )
	ROM_LOAD( "s122.5f",      0x1000, 0x1000, CRC(ebf3905c) SHA1(ce20501684331bcd6e5fcf8bb774fde1bfd2e50d) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s122.7f",      0x0000, 0x0020, CRC(10dfe8a6) SHA1(a40abbe8e341afa616edb00b9364c39d0726cd7d) )
	ROM_LOAD( "s122.4a",      0x0020, 0x0100, CRC(455dfcb3) SHA1(0cff8b6383d5c33d24b2d45fcb723165628d2f4c) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s123 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.123",    0x8000, 0x1000, CRC(e286d194) SHA1(bfc0c6442364a78726dd72dcf1e94c990296c246) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s123.7f",      0x0000, 0x0020, CRC(1d70004e) SHA1(51bc27df6e121a45b41a28c372dd041b4892d22f) )
	ROM_LOAD( "s123.4a",      0x0020, 0x0100, CRC(3d77d36e) SHA1(50188531a8062e06a30ce2474aced25e66380ced) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s124 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.124",    0x8000, 0x1000, CRC(bb5cd96d) SHA1(5b6f84fce37bb13c43e6047825136a5c9346da0b) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s124.7f",      0x0000, 0x0020, CRC(cd4b4e1a) SHA1(d0b00eb39f8b5434dc668741282bd0094b49600b) )
	ROM_LOAD( "s124.4a",      0x0020, 0x0100, CRC(2589fd32) SHA1(59bda09f5c13e31a57bff548b3b71f46abe75130) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s125 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.125",    0x8000, 0x1000, CRC(30f13117) SHA1(7d9c9665837200c717323dc277b040aa3f8550c3) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s125.7f",      0x0000, 0x0020, CRC(4749acbf) SHA1(6981959a7b715f22be66858737ac775dcce13ac9) )
	ROM_LOAD( "s125.4a",      0x0020, 0x0100, CRC(14d3716f) SHA1(99d691e674e14152d655791b9673e1638e560729) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s126 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.126",    0x8000, 0x1000, CRC(9eca650e) SHA1(461aef616ac9cf49041a73844915af7f791d2351) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s126.7f",      0x0000, 0x0020, CRC(c2a59b4f) SHA1(5fe4214ffd2831573c74877ec89415eb5b181ad1) )
	ROM_LOAD( "s126.4a",      0x0020, 0x0100, CRC(f1234b64) SHA1(43d8bbce4e92ce7cb5d0b37cb47cc6062c05d40e) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( pacman25s127 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
	ROM_LOAD( "pac25.6f",     0x1000, 0x1000, CRC(c5ec2352) SHA1(86178b16dabb90005b132febc1a261651a73018d) )
	ROM_LOAD( "pac25.6h",     0x2000, 0x1000, CRC(92cd89b9) SHA1(c3a0141480341c550a889b58692c8c90b322ee67) )
	ROM_LOAD( "pac25.6j",     0x3000, 0x1000, CRC(f7193845) SHA1(6d190cf6e76520b8b8c97e6e3a817c4a142003ba) )
	ROM_LOAD( "maps1.127",    0x8000, 0x1000, CRC(999633b1) SHA1(d61a710b313f8b813305f12a352b8919da591393) )
	ROM_LOAD( "maps2.rom",    0x9000, 0x1000, CRC(7cdbd912) SHA1(b19868bd5da3163e03320e6de0428b22b8e02d80) )
	ROM_LOAD( "maps3.rom",    0xa000, 0x1000, CRC(c626ea9c) SHA1(3cd3f20e002845937a43f323b685425c95881d9e) )
	ROM_LOAD( "maps4.rom",    0xb000, 0x1000, CRC(f5ba954d) SHA1(eba18f9a7f6d50f167ed37bff0adec43f52b49a0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "s25.5e",       0x0000, 0x1000, CRC(b332a47d) SHA1(417fa119a4b62bc2e954fbf2ce4a2237be96f55a) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "s127.7f",      0x0000, 0x0020, CRC(ea6efcbe) SHA1(7fa7fd20eea864424078c9eb3b237de68d81f4ea) )
	ROM_LOAD( "s127.4a",      0x0020, 0x0100, CRC(49025193) SHA1(73021ca0590a4ddbb5218a56ed3a1846dff647a7) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
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
	ROM_COPY( "gfx1",    0x1b00, 0x1c00, 0x40 )     /* show mario at start of level */

	PACMAN_PROMS
ROM_END

// extra keys: G = jump ; F = sound fx on/off ; R = music on/off
ROM_START( pacminer )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacminer.6e",  0x0000, 0x1000, CRC(11dee14e) SHA1(1aeb94299a33daa9b51fdab3c0abea98858cc7dd) )
	ROM_LOAD( "pacminer.6f",  0x1000, 0x1000, CRC(8c449bd7) SHA1(84233296683321f71fdef604ca19bfffd97e993c) )
	ROM_LOAD( "pacminer.6h",  0x2000, 0x1000, CRC(a0e62570) SHA1(654a71144eae3c78d7baaeb9119aad0ece500e60) )

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
	ROM_FILL(0x3f83, 3, 0)      /* remove unneeeded subroutine call - this fixes a major bug */

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

ROM_START( pacyuyu )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacyuyu.6j",   0x3000, 0x1000, CRC(85bb3cba) SHA1(9c3897b773c6dba55d78a385b0edc8f49439fbd2) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacyuyu.5e",   0x0000, 0x1000, CRC(430a4708) SHA1(96026757c4de2e6a1bc5b6f91ca5a5499880f7da) )
	ROM_LOAD( "pacyuyu.5f",   0x1000, 0x1000, CRC(333e98df) SHA1(c9fa72b9aad48a4d99a0ab9767335619d408ddc9) )

	PACMAN_SOUND_PROMS

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "pacyuyu.7f",   0x0000, 0x0020, CRC(67141599) SHA1(dd99fc1329f237a7c56abe643de15b49c18abb64) )
	ROM_LOAD( "pacyuyu.4a",   0x0020, 0x0100, CRC(2c004790) SHA1(00031f2dc7bd64b894ee5b5a5113f41f36c58edb) )
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
	ROM_FILL( 0x3fd5, 1, 0xc9 )     /* allow protection check to work - 0x36 or 0xc9 is ok */

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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "pac25.6e",     0x0000, 0x1000, CRC(fee263b3) SHA1(87117ba5082cd7a615b4ec7c02dd819003fbd669) )
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
	ROM_LOAD( "hangly3.6k",   0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
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
	ROM_LOAD( "hangly3.6k",   0x0800, 0x0800, CRC(194c7189) SHA1(fd423bac2810015313841c7b935054565390fbd0) )
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

ROM_START( mcpacman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "mcpacman.6j",  0x3000, 0x1000, CRC(addd1c14) SHA1(ac7e7ebb62b44c435a1c722d2b982097ff59b538) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mcpacman.5e",  0x0000, 0x1000, CRC(ab5f40ba) SHA1(b5d6b1515fadf1d3b22092bb922b2c5a380830c3) )
	ROM_LOAD( "mcpacman.5f",  0x1000, 0x1000, CRC(43752d25) SHA1(8e8c0ae54288a58de2613bf98a9a96c6a7369647) )

	PACMAN_PROMS
ROM_END

ROM_START( invispac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "invispac.6j",  0x3000, 0x1000, CRC(e4ec2f1a) SHA1(e0df9165cf5c0458c42cdf77f6444bfc0de64d7a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "invispac.5e",  0x0000, 0x1000, CRC(c16725bc) SHA1(f6eae4c7e7b30c33b0bf1ddfc4a617a3b908747f) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmini3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacmini3.6j",  0x3000, 0x1000, CRC(e87be2a3) SHA1(630116735e53644de3fcc7e7a249cd61105cf9f2) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacmini3.5e",  0x0000, 0x1000, CRC(0c3e20d7) SHA1(ab32160b0d079c83dbd522ac74f11f802e18230e) )
	ROM_LOAD( "pacmini.5f",   0x1000, 0x1000, CRC(30e6024c) SHA1(51c1f9074091c053437a25ef1c6e9ee63e9b5154) )

	PACMAN_PROMS
ROM_END

ROM_START( upsidpac ) // supposedly the maze upsidedown, but it's actually a new maze
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "upsidpac.6j",  0x3000, 0x1000, CRC(c225c48b) SHA1(9ec80b13f6da8a4aa7ba5797fd0ee0e69f2d9479) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacsmall )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacsmall.6j",  0x3000, 0x1000, CRC(74c4ce68) SHA1(93b7db314969b53644b9fad64c29595ce286ca74) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( baby1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "baby1.6j",     0x3000, 0x1000, CRC(646e53a4) SHA1(8b5ba5c4f1ad6fc721fd90b5a50258099fff704c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "baby2.5e",     0x0000, 0x1000, CRC(6f7d8d57) SHA1(2e786c5a61168455d31b7c45c97891f65243a40b) )
	ROM_LOAD( "baby2.5f",     0x1000, 0x1000, CRC(b6d77a1e) SHA1(63ada2a7f154ce7bd756c5002a34e5f4002c2473) )

	PACMAN_PROMS
ROM_END

ROM_START( pacforks )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacforks.6j",  0x3000, 0x1000, CRC(ce79408e) SHA1(000fa833e94b939cece93b75e097477f87f48749) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( bevpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "bevpac.6j",    0x3000, 0x1000, CRC(a9460bc2) SHA1(4969e3eba36e6e17d2d0db9239dd22f97baddd50) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "bevpac.5e",    0x0000, 0x1000, CRC(997948fa) SHA1(61162569679fa0791151ec17c6fde509ddaedbed) )
	ROM_LOAD( "bevpac.5f",    0x1000, 0x1000, CRC(5be1e1fd) SHA1(0ef63767c16d40bd999419d4977a97d9034d85ea) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "bevpac.4a",    0x0020, 0x0100, CRC(17d9b991) SHA1(32d4b8d0b476fafcbf3409b15dcd6ef3b50b52b2) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


/*************************************
 *
 *  Game drivers
 *
 *************************************/

/* in my opinion, the true original game */
GAME( 1980, puckman,  0,        pacman,   pacman,   puckman_state, empty_init,        ROT90, "Namco", "Puckman (original)", MACHINE_SUPPORTS_SAVE )

/* Official MAME */

/*          rom       parent    machine   inp       init */
GAME( 1982, ctrpllrp, puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Caterpillar Pacman Hack", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hangly,   puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Hangly-Man (set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hangly2,  puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Hangly-Man (set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hangly3,  puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Hangly-Man (set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, joyman,   puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Joyman", MACHINE_SUPPORTS_SAVE )
GAME( 1980, newpuc2,  puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Newpuc2", MACHINE_SUPPORTS_SAVE )
GAME( 1980, newpuc2b, puckman,  pacman,   pacman,   puckman_state, empty_init,        ROT90, "hack", "Newpuc2 (set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, newpuckx, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "hack", "New Puck-X", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pacheart, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "hack", "Pacman ((Heart/Edition hack))", MACHINE_SUPPORTS_SAVE ) //The roms of the pacman system have the same name.
GAME( 1980, pacman,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "[Namco] (Midway license)", "Pacman (Midway)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, pacmanf,  puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "[Namco] (Midway license)", "Pacman (Midway, with speedup hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pacmod,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "[Namco] (Midway license)", "Pacman (Midway, harder)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, pacplus,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "[Namco] (Midway license)", "Pacman Plus", MACHINE_SUPPORTS_SAVE )
GAME( 1981, puckmod,  puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Namco", "PuckMan (Japan set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmanb, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Namco", "PuckMan (Japan set 1, Probably Bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmana, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Namco", "PuckMan (Japan set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmanf, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Namco", "PuckMan (Japan set 1 with speedup hack)", MACHINE_SUPPORTS_SAVE )

/* Blue Justice's Hacks */

GAME( 2001, hm1000,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man 1000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hm2000,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man 2000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hmba5000, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man Babies 5000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hmba7000, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man Babies 7000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hmbabymz, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man BabiesMaze", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hmbluep,  puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man Blue Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hmgrenp,  puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man Green Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hmplus,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Hangly Man Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm1000,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Pac Man 1000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm2000,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Pac Man 2000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm3000,   puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Pac Man 3000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm4000p,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man 4000 Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm5000p,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man 5000 Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm6000,   puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man 6000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm7000,   puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man 7000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pm7000p,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man 7000 Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmba2000, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Pac Man Babies 2000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmba3000, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Pac Man Babies 3000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmba4000, puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Babies 4000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmba6000, puckman,  pacman,   pacman,   puckman_state, empty_init,  ROT90, "Blue Justice", "Pac Man Babies 6000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmba8000, puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Babies 8000", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmbamaz,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Blue Justice", "Pac Man Babies Maze", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmbaplus, puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Babies Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmbluep,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Blue Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmdeluxe, puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Deluxe", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmextra,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Extra", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmextrap, puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Extra Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmfever,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Fever", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pmgrenp,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "Blue Justice", "Pac Man Green Plus", MACHINE_SUPPORTS_SAVE )

/* T-Bone Hacks */

GAME( 2000, baby1,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Baby Pacman 1 (Alt)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, baby2,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Baby Pacman 2 (Alt)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, baby3,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Baby Pacman 3 (Alt)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, baby4,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman (Baby Maze 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, bevpac,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Bevelled Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, invispac, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman (Invisible Maze)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, mcpacman, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "McPacman", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacforks, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Forks", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s01, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man - (4 New Maps - After Dark)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s02, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man - (Fruit 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s03, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "pac Man - (Fruit 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s04, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Chomp Ms Pac Em SJB Maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s05, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Chomp Ms Pac Em)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s06, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Cookie Man)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s07, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (FHMC Sil 13 Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s08, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Namco Mazes Alternate)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s09, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Namco Mazes Featuring Ms Pac-Man)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s10, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Namco Mazes)(PAC-MAN Classic Look)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s11, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Namco Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s12, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (SJB Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s13, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (T-Bone Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s14, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Tengen Mazes)(PAC-MAN Classic Look)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s15, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Tengen Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s18, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Tengen Tall Mazes)(PAC-MAN Classic Look)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s19, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Tengen Tall Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s20, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - (Ultra Pac-Man)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s21, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s22, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s23, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s24, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s25, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 5)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s26, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 6)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s27, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - 360 Escape", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s28, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Alien", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s29, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Arcade", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s30, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Beach", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s31, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Beetle", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s32, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Blue", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s33, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Boys", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s34, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Candyland", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s35, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Chase", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s36, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Cherries", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s37, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Chocolate", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s38, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Colors", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s39, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Cookie", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s40, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Cup", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s41, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Dice", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s42, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Dinos", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s43, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Dragon", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s44, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Fire", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s45, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Fountain", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s46, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Garden", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s47, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Girder", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s48, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Girl", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s49, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Infinate", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s50, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Invisible", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s51, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Jailhouse", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s52, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - kaleidoscope", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s53, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Minutes", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s54, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Night Flower", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s55, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Night", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s56, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - No Escape", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s57, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Opposite", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s58, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Pig", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s59, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Plotted", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s60, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Prismatic", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s61, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac man + Tournament - Red Bull", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s62, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Royal", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s63, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Shadow", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s64, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Ship", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s65, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Six Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s66, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Solar System", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s67, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Square Dance", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s68, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Sugar Rush", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s69, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Sushi", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s70, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Tilt", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s71, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Traffic", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s72, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Trap", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s73, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Tunnels", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s74, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Turtle", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s75, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Windy", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s76, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 7)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s77, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 8)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s78, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 9)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s79, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament (Set 10)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s80, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - 3 Minutes", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s81, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - 40th Anniversary Event", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s82, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Acceleration", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s83, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - April Fools", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s84, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Blossom", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s85, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Candyland (With Bonus Maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s86, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Capsule", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s87, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Carnival", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s88, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Cupid", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s89, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Detective", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s90, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Domino", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s91, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Drift", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s92, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Fire (With Bonus Maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s93, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Gift Wrapped", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s94, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Gobble", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s95, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Good Year", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s96, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Halloween", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s97, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Happy 39th", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s98, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Hide", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s99, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Holiday", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s100, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Infinate (With Bonus Map)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s101, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Line Dance", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s102, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Magic", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s103, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Monster", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s104, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Native", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s105, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - No Escape (With Bonus Maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s106, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Outfoxed", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s107, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Pac-Kids", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s108, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Pilgrim", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s109, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Plans", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s110, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Plotted (With Bonus Map)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s111, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Purple Maze", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s112, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Sick", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s113, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Solar System (With Bonus Maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s114, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Sonic The Hedgehog", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s115, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Spangle", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s116, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Spelunker", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s117, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Squared", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s118, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Squirmy", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s119, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Survive", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s120, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Sushi (With Bonus Maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s121, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Sweet", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s122, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Taco", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s123, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Tangled", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s124, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Thing", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s125, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Underground", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s126, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - X-Mas", MACHINE_SUPPORTS_SAVE )
GAME( 2021, pacman25s127, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "T-Bone", "Pac Man 25 - Pac Man + Tournament - Zombies", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmini3, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacmini (Maze 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacsmall, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman (Small Maze)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pmad6m,   puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman 2000 After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, snakeyes, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman 2000 (Snake Eyes Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, snowpac,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Snowy Day Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, tbone,    puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman 2000 (T-Bone Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, upsidpac, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Pacman (Upsidedown Maze)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vectr6m,  puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Vector Pacman 2000", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vectr6tb, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "T-Bone", "Vector Pacman 2000 (T-Bone Mazes)", MACHINE_SUPPORTS_SAVE )

/* Jerronimo's Progs - www.umlautllama.com */

GAME( 2000, brakman,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Scott Lawrence", "Brakman", MACHINE_SUPPORTS_SAVE ) // http://www.csh.rit.edu/~jerry/arcade/age/
GAME( 2004, pacman25, puckman,  woodpek,  pacman0,  puckman_state, empty_init,   ROT90, "Scott Lawrence", "25 MAP PAC [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2004, pacsnoop, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "Scott Lawrence", "Pac Snoop v1.3 [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pengman,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Scott Lawrence", "Pengo Man", MACHINE_SUPPORTS_SAVE ) // http://www.csh.rit.edu/~jerry/arcade/age/

/* Medragon's Hacks */

GAME( 2000, crazypac, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Tim Appleton", "Crazy Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2000, dizzy,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Tim Appleton", "Dizzy Ghost - A Reversal of Roles", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacfnt,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Tim Appleton", "Pacman with Pacman font", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacwf,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Tim Appleton", "Pacman (Weird Fruit)", MACHINE_SUPPORTS_SAVE )

/* Sil's Hacks */

GAME( 1998, chtpac3d, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "Marcel Silvius", "Pacman 3D [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, eltonpac, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "Elton Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mazeman,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "Maze Man", MACHINE_SUPPORTS_SAVE )
GAME( 1998, namcosil, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "Marcel Silvius", "Pacman (Namco) (Sil hack) [c]", MACHINE_SUPPORTS_SAVE )
GAME( 1998, newpuck3, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "New Puck-3", MACHINE_SUPPORTS_SAVE )
GAME( 1998, pacman3d, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "Pacman 3D", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacman6,  puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "Pacman 6", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmn6m2, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "Pacman 2000 (Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmulti, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "PacMulti (Pacman)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacweird, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "Marcel Silvius", "Pacman (Six Map Weird)", MACHINE_SUPPORTS_SAVE )

/* Bobby Tribble's Hacks */

GAME( 2000, pac2600,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Bobby Tribble", "Pac 2600", MACHINE_SUPPORTS_SAVE )
GAME( 19??, pac2600a, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Bobby Tribble?","Pac 2600 (Older?)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmini,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Bobby Tribble", "Mini Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmini2, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Bobby Tribble", "Mini Pacman 2", MACHINE_SUPPORTS_SAVE )

/* Hires hacks */

GAME( 1981, hanglyx,  puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Hangly-Man (set 1) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hangly2x, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Hangly-Man (set 2) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hangly3x, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Hangly-Man (set 3) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, newpuc2x, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Newpuc2 (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, nwpuc2bx, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Newpuc2 (set 2) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, nwpuckxx, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "New Puck-X (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, pacmanx,  puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "[Namco] (Midway license)", "Pac-Man (Midway) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, pacmanfx, puckmanx, pacmanx,  pacman0,  puckman_state, empty_init,   ROT90, "[Namco] (Midway license)", "Pac-Man (Midway) (hires hack) [f]", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pacmodx,  puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "[Namco] (Midway license)", "Pac-Man (Midway, harder) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, pacplusx, puckmanx, pacmanx,  pacman,   puckman_state, init_pacplus, ROT90, "[Namco] (Midway license)", "Pac-Man Plus (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pheartx,  puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Pac-Man (Hearts) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmanx, 0,        pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "Namco", "PuckMan (Japan set 1, Probably Bootleg) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmnax, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "Namco", "PuckMan (Japan set 2) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmnfx, puckmanx, pacmanx,  pacman0,  puckman_state, empty_init,   ROT90, "Namco", "PuckMan (Japan set 1) (hires hack) [f]", MACHINE_SUPPORTS_SAVE )
GAME( 1980, puckmnhx, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "hack", "Puckman (Falcom?) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, puckmodx, puckmanx, pacmanx,  pacman,   puckman_state, empty_init,   ROT90, "Namco", "PuckMan (Japan set 3) (hires hack)", MACHINE_SUPPORTS_SAVE )

/* Misc Pacman Hacks */

GAME( 2001, abortman, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Paul Copeland",  "Abortman", MACHINE_SUPPORTS_SAVE )
GAME( 2015, bacman,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "George Phillips",  "Bac-man (2015-08-22)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, bucaneerx,puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Bucco", "Buccaneer", MACHINE_SUPPORTS_SAVE )
GAME( 2000, caterpil, puckman,  pacman,   mspacman, puckman_state, empty_init,   ROT90, "Phi", "Caterpillar", MACHINE_SUPPORTS_SAVE )
GAME( 2000, chtpac,   puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "[Midway]", "Pacman [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, chtpman2, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "[Namco]", "New Puck2 [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, chtpop,   puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "[Midway]", "Pac-man (Popeye) [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, chtpuck,  puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "Deluxe", "New Puck-X [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, europac,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Stefano Priore", "Euro Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fasthang, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "[Nittoh]", "Hangly Man [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fastpop,  puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Popeye) [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fastpuck, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "Deluxe", "New Puck-X [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fpnleash, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "Peter Storey", "Pacman Unleashed (Flat)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fstpman2, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "[Namco]", "New Puck2 [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, hanglyad, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Nittoh]", "Hangly-Man (set 1) After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, hearts,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Hearts)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, hellowp,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "cmonkey", "Hello World (Pacman HW)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hmhearts, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Nittoh]", "Hangly-Man (Hearts)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, hmpopey,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Nittoh]", "Hangly-Man (Popeye)", MACHINE_SUPPORTS_SAVE )
GAME( 2005, homercid, puckman,  pacman,   mspacpls, puckman_state, empty_init,   ROT90, "David Widel", "Homercide", MACHINE_SUPPORTS_SAVE )
GAME( 2001, jackman,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Brent Cobb", "Jackman", MACHINE_SUPPORTS_SAVE )
GAME( 1980, jpmmunch, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "JPM", "Muncher", MACHINE_SUPPORTS_SAVE )
GAME( 1983, muckpan,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "E.P.", "Muckpan", MACHINE_SUPPORTS_SAVE )
GAME( 1983, newpuc1,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "E.P.", "Newpuc1 (Italian dump)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, newpuck2, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Sir Scotty", "New Puck-2", MACHINE_SUPPORTS_SAVE )
GAME( 1981, nwpuc2b,  puckman,  pacman,   newpuc2b, puckman_state, empty_init,   ROT90, "Linear Elect", "New Puc2 (Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacbaby,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Baby Maze 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacbaby2, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Baby Maze 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacbaby3, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Baby Maze 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacbell,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Pacman (Bell)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacelec,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Pacman (Electric Cowboy)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacinvis, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Original Inviso)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacjail,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Pacman (Jail)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacjr1,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman Jr. (Maze 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacjr2,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman Jr. (Maze 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacjr3,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman Jr. (Maze 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacjr4,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman Jr. (Maze 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, packetman,puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "unbranded", "Packetman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacm255,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Fixes 255th Maze)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, pacmanp,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Don Hodges", "Pacman (Patched)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pacmar,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Mario Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2012, pacminer, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Jim Bagley", "Pac Manic Miner Man", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacms1,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacms2,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacms3,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacms4,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Maze 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmsa1,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmsa2,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmsa3,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacmsa4,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Ms. Pacman Attack 4)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, pacplusc, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "Bally Midway", "Pacman Plus (Unencrypted) [c]", MACHINE_SUPPORTS_SAVE )
GAME( 1998, pacpopey, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman (Popeye)", MACHINE_SUPPORTS_SAVE )
GAME( 19??, pacrab,   puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pac Rabbit", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacshuf,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Pacman (Shuffle)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacspeed, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Pacman (Speedy)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, pacyuyu,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Kirai Shouen, 125scratch", "Pac-Yuyu", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pcrunchy, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Pacman (Crunchy)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pheartb,  puckman,  pacman,   newpuc2,  puckman_state, empty_init,   ROT90, "Kamiya", "Pacman Hearts 2", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pmad,     puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Sir Scotty", "Pacman After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pmad00,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman After Dark 2000", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pmada,    puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Pacman After Dark (Alternate)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, pmheart,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90,  "bootleg", "Puckman Hearts", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pplusad,  puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "[Bally Midway]", "Pacman Plus After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, puck255,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Namco]", "Puckman (Fixes 255th Maze)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, puckmano, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Hebus", "Puckman Opposite", MACHINE_SUPPORTS_SAVE )
GAME( 2007, puckmot1, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Hebus", "Puckmotik (alt)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, puckmoti, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Hebus", "Puckmotik", MACHINE_SUPPORTS_SAVE )
GAME( 2000, puckren,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "PacHack", "Puckman Renaissance", MACHINE_SUPPORTS_SAVE )
GAME( 2000, puckrenc, puckman,  pacman,   pacman0,  puckman_state, empty_init,   ROT90, "PacHack", "Puckman Renaissance [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, punleash, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "Peter Storey", "Pacman Unleashed", MACHINE_SUPPORTS_SAVE )
GAME( 1999, roboman,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Runge Editing", "Robo Man", MACHINE_SUPPORTS_SAVE )
GAME( 1981, spuckman, puckman,  pacman,   spuckman, puckman_state, empty_init,   ROT90, "TV Tune / Miet!o!mat", "Super Puckman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, sumelton, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Staizitto", "Summertime Elton", MACHINE_SUPPORTS_SAVE )
GAME( 2000, sumeltob, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Staizitto", "Summertime Elton (old)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, ultra2,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "TwoBit Score", "Ultra Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, ultrapac, puckman,  woodpek,  pacman,   puckman_state, empty_init,   ROT90, "PacFan", "UltraPac", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vcrunchy, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Vector Pacman (Crunchy)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vecbaby,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Baby Maze 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vecbaby2, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Baby Maze 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vecbaby3, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Baby Maze 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vecpac,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vectplus, puckman,  pacman,   pacman,   puckman_state, init_pacplus, ROT90, "[Bally Midway]", "Vector Pacman Plus", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vectxens, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Jeff Morris", "Vector Xens Revenge", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vhangly,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Nittoh]", "Vector Hangly-Man", MACHINE_SUPPORTS_SAVE )
GAME( 2008, vhangly3, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Gatinho [Nittoh]", "Vector Hangly-Man3", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacbell, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Vector Pacman (Bell)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacelec, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Vector Pacman (Electric Cowboy)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacjail, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Vector Pacman (Jail)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacms1,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacms2,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacms3,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacms4,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Maze 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacmsa1, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacmsa2, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacmsa3, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacmsa4, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "[Midway]", "Vector Pacman (Ms. Pacman Attack 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpacshuf, puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Vector Pacman (Shuffle)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vpspeed,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Snake Eyes", "Vector Pacman (Speedy)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, xensad,   puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Jeff Morris", "Xens Revenge After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, xensrev,  puckman,  pacman,   pacman,   puckman_state, empty_init,   ROT90, "Jeff Morris", "Xens Revenge", MACHINE_SUPPORTS_SAVE )


#include "mspacman.cpp"
#include "multipac.cpp"
#include "maketrax.cpp"

