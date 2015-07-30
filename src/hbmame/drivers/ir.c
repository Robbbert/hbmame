// license:BSD-3-Clause
// copyright-holders:Robbbert
/****************************************************************************************

	Invader's Revenge

	October, 2008 by Robbbert

	No diagram available, so I threw this together


	Invader's Revenge was made by the English company, Zenitone-Microsec Ltd.
	Distributed in Germany by Dutchford.
	Also distributed in America by Americade Amusement, Inc., of New Jersey.

	It's basically a hardware hack of the normal Taito Space Invaders machine.

	Changes (start with a Space Invaders with a colour CRT):

	1. The sound board is replaced with another that makes different sounds.

	2. A sardine-can-shaped potted black epoxy block containing a 4mhz crystal,
	   two large 40-pin ICs, at least 2 roms, and a number of TTL-shaped chips.
	   My assumption is that one of the big chips is a (slave sound) CPU.

	3. New main roms

	4. A small card contains a colour PROM. There's no RAM for this, so the colours
	   must be quite simplistic.

	The main CPU communicates to the slave by sending sound codes via port 3,
	while port 5 controls various aspects of the screen display.

	The method of enabling cocktail mode is the same as for Space Invaders, that is,
	changing the positions of SW5,6,7; cutting a track; and adding some jumpers.
	Obviously this is a once-off change for the table model.

	Known bugs in the original roms:

	1. If you turn on the machine, start a 1-player game and just let it run to the
	   end, it will congratulate player 2 for getting a high score. Of course, zero
	   points is not a high score, and there wasn't a player 2.

	2. If you die just after killing the last enemy in the 8th level, the docking
	   stage uses the wrong colour map. I've added a fix for this.


	Notes about colour proms:

	1. The proms from sets 'invrvnga' and 'invrvng3' are the correct ones. The prom
	   from 'invrvng3' is used for the other sets, as their proms haven't shown up
           yet.

	2. Real name for 'invrvnga' prom is '3r.bin' ; for 'invrvng3' is 'ir.clr'.

	Todo:

	1. Identify the slave CPU and make it work.
	2. Get schematic diagrams, and design a discrete sound board.

	Changes:

	1. Added above documentation (MAME doesn't have any)
	2. Identified all sound codes. (We only need good samples now)

*******************************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/mb14241.h"
#include "sound/samples.h"

#define MW8080BW_MASTER_CLOCK             (19968000.0)
#define MW8080BW_CPU_CLOCK                (MW8080BW_MASTER_CLOCK / 10)
#define MW8080BW_PIXEL_CLOCK              (MW8080BW_MASTER_CLOCK / 4)
#define MW8080BW_HTOTAL                   (0x140)
#define MW8080BW_HBEND                    (0x000)
#define MW8080BW_HBSTART                  (0x100)
#define MW8080BW_VTOTAL                   (0x106)
#define MW8080BW_VBEND                    (0x000)
#define MW8080BW_VBSTART                  (0x0e0)
#define MW8080BW_VCOUNTER_START_NO_VBLANK (0x020)
#define MW8080BW_VCOUNTER_START_VBLANK    (0x0da)
#define MW8080BW_INT_TRIGGER_COUNT_1      (0x080)
#define MW8080BW_INT_TRIGGER_VBLANK_1     (0)
#define MW8080BW_INT_TRIGGER_COUNT_2      MW8080BW_VCOUNTER_START_VBLANK
#define MW8080BW_INT_TRIGGER_VBLANK_2     (1)
#define MW8080BW_HPIXCOUNT                MW8080BW_HBSTART

class ir_state : public driver_device
{
public:
	ir_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_p_ram(*this, "ram")
		, m_samples(*this, "samples")
		, m_screen(*this, "screen")
	{ }

	/* device/memory pointers */
	required_device<cpu_device> m_maincpu;
	required_shared_ptr<UINT8> m_p_ram;
	required_device<samples_device> m_samples;
	required_device<screen_device> m_screen;

	bool       m_flip_screen;
	bool       m_screen_red;

	/* timer */
	emu_timer   *m_interrupt_timer;

	DECLARE_READ8_MEMBER(mw8080bw_shift_result_rev_r);
	DECLARE_READ8_MEMBER(mw8080bw_reversable_shift_result_r);
	DECLARE_WRITE8_MEMBER(mw8080bw_reversable_shift_count_w);
	DECLARE_READ8_MEMBER(invrvnge_02_r);
	DECLARE_WRITE8_MEMBER(invrvnge_03_w);
	DECLARE_WRITE8_MEMBER(invrvnge_05_w);
	DECLARE_MACHINE_START(ir);
	DECLARE_MACHINE_RESET(ir);
	UINT8 vpos_to_vysnc_chain_counter( int vpos );
	int vysnc_chain_counter_to_vpos( UINT8 counter, int vblank );
	UINT32 screen_update_ir(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	TIMER_CALLBACK_MEMBER(mw8080bw_interrupt_callback);
	void mw8080bw_create_interrupt_timer(  );
	void mw8080bw_start_interrupt_timer(  );
};


READ8_MEMBER(ir_state::invrvnge_02_r)
{
	UINT8 data = ioport("IN2")->read();
	if (m_flip_screen) return data;
	return (data & 0x8f) | (ioport("IN1")->read() & 0x70);
}

static ADDRESS_MAP_START( main_map, AS_PROGRAM, 8, ir_state )
	ADDRESS_MAP_GLOBAL_MASK(0x7fff)
	AM_RANGE(0x0000, 0x1fff) AM_ROM AM_WRITENOP
	AM_RANGE(0x2000, 0x3fff) AM_MIRROR(0x4000) AM_RAM AM_SHARE("ram")
	AM_RANGE(0x4000, 0x5fff) AM_ROM AM_WRITENOP
ADDRESS_MAP_END

static ADDRESS_MAP_START( invrvnge_io_map, AS_IO, 8, ir_state )
	AM_RANGE(0x00, 0x00) AM_READ_PORT("IN0")
	AM_RANGE(0x01, 0x01) AM_READ_PORT("IN1")
	AM_RANGE(0x02, 0x02) AM_READ(invrvnge_02_r) AM_DEVWRITE("mb14241", mb14241_device, shift_count_w)
	AM_RANGE(0x03, 0x03) AM_DEVREAD("mb14241", mb14241_device, shift_result_r) AM_WRITE(invrvnge_03_w)
	AM_RANGE(0x04, 0x04) AM_DEVWRITE("mb14241", mb14241_device, shift_data_w)
	AM_RANGE(0x05, 0x05) AM_WRITE(invrvnge_05_w)
	AM_RANGE(0x06, 0x06) AM_WRITE(watchdog_reset_w)
ADDRESS_MAP_END

//static ADDRESS_MAP_START( invrvnge_sound_map, AS_PROGRAM, 8, ir_state )
//	ADDRESS_MAP_UNMAP_HIGH
//	AM_RANGE(0x0000, 0xffff) AM_ROM // dummy prg map, TODO: decrypt ROM
//ADDRESS_MAP_END


static INPUT_PORTS_START( ir )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_DIPNAME( 0x06, 0x00, DEF_STR( Difficulty ) )       PORT_DIPLOCATION("SW1:5,6")
	PORT_DIPSETTING(    0x00, DEF_STR( Easy ) )	// [code: 0x3b1-3b5]
	PORT_DIPSETTING(    0x02, DEF_STR( Hard ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Harder ) )
	PORT_DIPSETTING(    0x06, DEF_STR( Hardest ) )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNKNOWN ) // must be low or game won't boot [code: 0x1a9-1af]
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_DIPNAME( 0x80, 0x80, "Fuel Destroyed by Comet" )   PORT_DIPLOCATION("SW1:7")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x80, "6" )
	//PORT_DIPNAME( 0x80, 0x00, "Fuel Usage" )	// [code: 0x1cb0-1cb6]
	//PORT_DIPSETTING(    0x80, "Fast" )
	//PORT_DIPSETTING(    0x00, "Slow" )

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  ) PORT_2WAY
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_2WAY
	PORT_BIT( 0x80, IP_ACTIVE_LOW,  IPT_UNUSED )	// [code: 0x750-75B] causes this switch to be ineffective

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )            PORT_DIPLOCATION("SW1:1,2")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPSETTING(    0x02, "5" )
	PORT_DIPSETTING(    0x03, "6" )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Bonus_Life ) )       PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(    0x00, "1500" )	// [code: 0x3b9-3c9]
	PORT_DIPSETTING(    0x08, "2000" )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_2WAY PORT_PLAYER(2)
	PORT_DIPNAME( 0x80, 0x00, DEF_STR( Coinage ) )          PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x80, DEF_STR( 2C_1C ) ) // 1 play 10p, 2 play 20p, 6 play 50p
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) ) // 1 play 20p, 2 play 40p, 3 play 50p

	PORT_START("CAB")
	PORT_CONFNAME( 0x01, 0x00, DEF_STR( Cabinet ) )
	PORT_CONFSETTING(    0x00, DEF_STR( Upright ) )
	PORT_CONFSETTING(    0x01, DEF_STR( Cocktail ) )
INPUT_PORTS_END

UINT8 ir_state::vpos_to_vysnc_chain_counter( int vpos )
{
	/* convert from a vertical position to the actual values on the vertical sync counters */
	UINT8 counter;
	int vblank = (vpos >= MW8080BW_VBSTART);

	if (vblank)
		counter = vpos - MW8080BW_VBSTART + MW8080BW_VCOUNTER_START_VBLANK;
	else
		counter = vpos + MW8080BW_VCOUNTER_START_NO_VBLANK;

	return counter;
}


int ir_state::vysnc_chain_counter_to_vpos( UINT8 counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}


TIMER_CALLBACK_MEMBER(ir_state::mw8080bw_interrupt_callback)
{
	UINT8 next_counter;
	int next_vpos;
	int next_vblank;

	/* compute vector and set the interrupt line */
	int vpos = m_screen->vpos();
	UINT8 counter = vpos_to_vysnc_chain_counter(vpos);
	UINT8 vector = 0xc7 | ((counter & 0x40) >> 2) | ((~counter & 0x40) >> 3);
	m_maincpu->set_input_line_and_vector(0, HOLD_LINE, vector);

	/* set up for next interrupt */
	if (counter == MW8080BW_INT_TRIGGER_COUNT_1)
	{
		next_counter = MW8080BW_INT_TRIGGER_COUNT_2;
		next_vblank = MW8080BW_INT_TRIGGER_VBLANK_2;
	}
	else
	{
		next_counter = MW8080BW_INT_TRIGGER_COUNT_1;
		next_vblank = MW8080BW_INT_TRIGGER_VBLANK_1;
	}

	next_vpos = vysnc_chain_counter_to_vpos(next_counter, next_vblank);
	m_interrupt_timer->adjust(m_screen->time_until_pos(next_vpos));
}


void ir_state::mw8080bw_create_interrupt_timer(  )
{
	m_interrupt_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(ir_state::mw8080bw_interrupt_callback),this));
}


void ir_state::mw8080bw_start_interrupt_timer(  )
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));
}



/*************************************
 *
 *  Machine setup
 *
 *************************************/

MACHINE_START_MEMBER( ir_state, ir )
{
	mw8080bw_create_interrupt_timer();
}


/*************************************
 *
 *  Machine reset
 *
 *************************************/

MACHINE_RESET_MEMBER( ir_state, ir )
{
	mw8080bw_start_interrupt_timer();
}

static const char *const ir_sample_names[] =
{
	"*invrvnge",
	"02",		/* insert coin */
	"06",		/* you shoot */
	"14",		/* hit alien */
	"16",		/* hit asteroid */
	"18",		/* low fuel */
	"1a",		/* successful docking */
	"1e",		/* die */
	"22",		/* start game */
	"24",		/* alien diving */
	"26",		/* alien steals fuel */
	"28",		/* background sound 1 */
	"2a",		/* background sound 2 */
	"2c",		/* background sound 3 */
	"2e",		/* background sound 4 */
	"30",		/* build up fuel */
	"32",		/* ufo fires */
	"34",		/* high score */
	"36",		/* docking stage */
	"38",		/* bonus ship */
	"3a",		/* thrust */
	0
};


/*************************************************************************************/
/* Notes:                                                                            */
/* Init sequence: 0x01 (20 times), 0x40 (20 times), 0x4c, 0x40, 0x44, 0x40 (9 times).*/
/* Player 1 start sequence: 0x0c, 0x20, 0x22.                                        */
/* Unknown codes: 0x0A, 0x0c                                                         */
/*************************************************************************************/
WRITE8_MEMBER(ir_state::invrvnge_03_w)
{
	switch (data)
	{
		case 0x02:						/* Coin */
			machine().sound().system_enable(1);		/* enable sound so we can hear coin drop */
			m_samples->start(0, 0);
			break;

		case 0x04:						/* Attract Mode */
			machine().sound().system_enable(0);		/* disable sound */
			break;

		case 0x06:
			m_samples->start(1, 1);				/* Shoot */
			break;

		case 0x14:
			m_samples->start(2, 2);				/* Hit Alien (0x0a happens at the same time) */
			break;

		case 0x16:
			m_samples->start(2, 3);				/* Hit Asteroid */
			break;

		case 0x18:						/* Fuel Low */
			m_samples->start(5, 4);
			break;

		case 0x1a:						/* Successful docking */
			m_samples->start(0, 5);
			break;

		case 0x1c:						/* Finished fuelling */
			m_samples->stop(0);
			break;

		case 0x1e:
			m_samples->start(0, 6);				/* Death */
			break;

		case 0x20:
			machine().sound().system_enable(1);		/* enable sound for game start */
		case 0x22:
			m_samples->start(0, 7);				/* Start Game */
			break;

		case 0x24:						/* Alien dropping to steal fuel */
			m_samples->start(3, 8);
			break;

		case 0x26:						/* Alien lifting with fuel */
			m_samples->start(4, 9);
			break;

		case 0x28:						/* background sound level 1 */
		case 0x2a:						/* background sound level 2 */
		case 0x2c:						/* background sound level 3 */
		case 0x2e:						/* background sound level 4 */
			m_samples->start(0, data/2-10, 1);
			break;

		case 0x30:						/* Fuel bar filling up */
			m_samples->start(0, 14);
			break;

		case 0x32:						/* UFO drops a bomb */
			m_samples->start(6, 15);
			break;

		case 0x34:						/* You got the highest score */
			m_samples->start(0, 16);
			break;

		case 0x36:						/* Docking stage */
			m_screen_red = 0;				/* fix bug (2) in documentation */
			m_samples->start(0, 17);
			break;

		case 0x38:						/* Bonus ship */
			m_samples->start(8, 18);
			break;

		case 0x3a:						/* Thrust */
			m_samples->start(7, 19);
			break;

		case 0x4c:
			m_samples->stop(0);	// using this to stop sound if game tilted
		case 0x01:						/* These 3 play at boot time */
		case 0x40:						/* no idea if they are actual sounds */
		case 0x44:

		case 0x0a:						/* with 0x14 */
		case 0x0c:						/* used a lot but for what? */
			break;

		default:
			if (data) printf("Undefined sound code: %X\n",data);
	}
}

WRITE8_MEMBER(ir_state::invrvnge_05_w)
{
	/*
	    00 - normal play
	    0A, 0E - alternate during play/attract at about once per second (invrvngegw only) purpose unknown
	    01 - briefly at boot time
	    10 - different colour map (or screen red) when you die
	    20 - flip screen */

	m_screen_red = BIT(data, 4);
	m_flip_screen = BIT(data, 5) & ioport("CAB")->read();
}

UINT32 ir_state::screen_update_ir(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	pen_t pens[8];
	offs_t offs, color_address;
	UINT8 *prom;
	UINT8 *color_map_base;
	UINT8 i, x, y, data, fore_color, color;

	for (i = 0; i < 8; i++)
		pens[i] = rgb_t(pal1bit(i >> 0), pal1bit(i >> 2), pal1bit(i >> 1));

	prom = memregion("proms")->base();
	color_map_base = &prom[0x0000];

	for (offs = 0; offs < m_p_ram.bytes(); offs++)
	{
		y = offs >> 5;
		x = offs << 3;

		color_address = (offs >> 8 << 5) | (offs & 0x1f);

		data = m_p_ram[offs];
		fore_color = m_screen_red ? 1 : color_map_base[color_address] & 0x07;

		for (i = 0; i < 8; i++)
		{
			color = BIT(data, i) ? fore_color : 0;

			if (y >= MW8080BW_VCOUNTER_START_NO_VBLANK)
			{
				if (m_flip_screen)
					bitmap.pix32(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - x) = pens[color];
				else
					bitmap.pix32(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = pens[color];
			}

			x++;
		}
	}

	return 0;
}

static MACHINE_CONFIG_START( ir, ir_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu",I8080,MW8080BW_CPU_CLOCK)
	MCFG_CPU_PROGRAM_MAP(main_map)
	MCFG_CPU_IO_MAP(invrvnge_io_map)
	MCFG_MACHINE_START_OVERRIDE(ir_state, ir)
	MCFG_MACHINE_RESET_OVERRIDE(ir_state, ir)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_RAW_PARAMS(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART)
	MCFG_SCREEN_UPDATE_DRIVER(ir_state, screen_update_ir)

	//MCFG_CPU_ADD("audiocpu", M6808, XTAL_4MHz/2) // MC6808P
	//MCFG_CPU_PROGRAM_MAP(invrvnge_sound_map)

	/* add shifter */
	MCFG_MB14241_ADD("mb14241")

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")

	//MCFG_SOUND_ADD("ay1", AY8910, XTAL_4MHz/2)
	//MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.5)

	MCFG_SOUND_ADD("samples", SAMPLES, 0)
	MCFG_SAMPLES_CHANNELS(9)
	MCFG_SAMPLES_NAMES(ir_sample_names)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)
MACHINE_CONFIG_END

ROM_START( invrvnge )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "h.ic36",      0x0000, 0x0800, CRC(0e229b9f) SHA1(617197bf94e9700cbbb2f32487dc47b318d4f2af) )
	ROM_LOAD( "g.ic35",      0x0800, 0x0800, CRC(26b38aa4) SHA1(f281c7ec47ce6ab61bfda2e7aa6a5b8a01f2c11e) )
	ROM_LOAD( "f.ic34",      0x1000, 0x0800, CRC(b3b2749e) SHA1(4f854f981396e2d6a959dd48cff12234074fb69b) )
	ROM_LOAD( "e.ic33",      0x1800, 0x0800, CRC(d8e75102) SHA1(86d5618944265947e3ce60fdf048d8fff4a55744) )

	// not used
	//ROM_REGION( 0x10000, "audiocpu", 0 ) // encrypted
	//ROM_LOAD( "snd.2c",      0xf000, 0x0800, CRC(135f3b16) SHA1(d472a6ca32c4a16cc1faf09f4a4876d75cd4ba24) )
	//ROM_LOAD( "snd.1c",      0xf800, 0x0800, CRC(152fc85e) SHA1(df207d6e690287a56e4e330deaa5ee40a179f1fc) )

	ROM_REGION( 0x0800, "proms", 0 )
	ROM_LOAD( "colour.bin",  0x0000, 0x0800, CRC(7de74988) SHA1(0b8c94b2bfdbc3921d60aad765df8af611f3fdd7) )
ROM_END

GAME( 1980?,invrvnge, 0, ir, ir, driver_device, 0, ROT270, "Zenitone-Microsec Ltd.", "Invader's Revenge (set 1)", MACHINE_SUPPORTS_SAVE )
