// license:BSD-3-Clause
// copyright-holders:Robbbert
/****************************************************************************************

Space Missile

Galaxian running on Space Invaders hardware

2013-06-26

*******************************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/mb14241.h"
#include "sound/discrete.h"
#include "sound/samples.h"
#include "screen.h"
#include "speaker.h"
#include "spacmissx.lh"

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
#define MW8080BW_HPIXCOUNT                MW8080BW_HBSTART + 6

class sm_state : public driver_device
{
public:
	sm_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_p_ram(*this, "ram")
		, m_discrete(*this, "discrete")
		, m_samples(*this, "samples")
		, m_screen(*this, "screen")
	{ }

	void spacmissx(machine_config &config);

private:

	bool m_flip_screen;
	bool m_screen_red;
	bool m_sound_enabled;
	u8 m_port_1_last_extra;
	u8 m_port_2_last_extra;
	emu_timer   *m_interrupt_timer;
	u8 spacmissx_02_r();
	void spacmissx_03_w(u8 data);
	void spacmissx_05_w(u8 data);
	void spacmissx_07_w(u8 data);
	DECLARE_MACHINE_START(sm);
	DECLARE_MACHINE_RESET(sm);
	u8 vpos_to_vysnc_chain_counter( int vpos );
	int vysnc_chain_counter_to_vpos( u8 counter, int vblank );
	u32 screen_update_spacmissx(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	TIMER_CALLBACK_MEMBER(mw8080bw_interrupt_callback);
	void mw8080bw_create_interrupt_timer(  );
	void mw8080bw_start_interrupt_timer(  );
	void mem_map(address_map &map);
	void io_map(address_map &map);
	required_device<cpu_device> m_maincpu;
	required_shared_ptr<u8> m_p_ram;
	required_device<discrete_sound_device> m_discrete;
	required_device<samples_device> m_samples;
	required_device<screen_device> m_screen;
};

static const discrete_dac_r1_ladder spacmissx_music_dac =
	{3, {0, RES_K(47), RES_K(12)}, 0, 0, 0, CAP_U(0.1)};

static const discrete_comp_adder_table invaders_thump_resistors =
{
	DISC_COMP_P_RESISTOR,
	0,                          /* no cDefault */
	4,                          /* length */
	{ RES_K(20) + RES_K(20),    /* R126 + R127 */
		RES_K(68),              /* R128 */
		RES_K(82),              /* R129 */
		RES_K(100) }                /* R130 */
};


static const discrete_555_desc invaders_thump_555 =
{
	DISC_555_OUT_ENERGY | DISC_555_OUT_DC,
	5,
	5.0 - 0.6,              /* 5V - diode drop */
	DEFAULT_TTL_V_LOGIC_1   /* Output of F3 7411 buffer */
};

static const discrete_mixer_desc mix1 =
{
	DISC_MIXER_IS_RESISTOR,
	{RES_K(22), RES_K(22)},
	{0},
	{CAP_U(.1), CAP_U(.1)},
	0, RES_K(100), 0, CAP_U(.1), 0, 20
};

#define spacmissx_MUSIC_CLK      (150000)

/* Nodes - Inputs */
/* Nodes - Sounds */
/************************************************
 * Fleet movement
 ************************************************/

DISCRETE_SOUND_START(spacmissx_disc)
/******************************************************************************
 *
 * Background Hum
 *
 ******************************************************************************/
	DISCRETE_INPUT_DATA(NODE_02)                   \
	DISCRETE_COMP_ADDER(NODE_30,                                      \
					NODE_02,     /* DATA */          \
					&invaders_thump_resistors)                                          \
	DISCRETE_555_ASTABLE(NODE_31,                 /* IC F3, pin 6 */  \
					1,                                              /* RESET */         \
					NODE_30,                      /* R1 */            \
					RES_K(75),                                      /* R131 */          \
					CAP_U(0.1),                                     /* C29 */           \
					&invaders_thump_555)                                                \
	DISCRETE_RCFILTER(NODE_32,                                        \
					NODE_31,                      /* IN0 */           \
					100,                                            /* R132 */          \
					CAP_U(4.7) )                                    /* C31 */           \
	DISCRETE_RCFILTER(NODE_12,                        \
					NODE_32,                      /* IN0 */           \
					100 + 100,                                      /* R132 + R133 */   \
					CAP_U(10) )                                     /* C32 */


/******************************************************************************
 *
 * Music Generator
 *
 ******************************************************************************/
	DISCRETE_INPUT_DATA (NODE_01)

	DISCRETE_NOTE(NODE_20, 1, spacmissx_MUSIC_CLK, NODE_01, 255, 5, DISC_CLK_IS_FREQ)

	// Convert count to 7492 output
	DISCRETE_TRANSFORM2(NODE_21, NODE_20, 2, "01>0+")

	DISCRETE_DAC_R1(NODE_22, NODE_21, DEFAULT_TTL_V_LOGIC_1, &spacmissx_music_dac)

/******************************************************************************
 *
 * Final Mixing and Output
 *
 ******************************************************************************/
	DISCRETE_CRFILTER(NODE_80, NODE_22, RES_K(10), CAP_U(0.1))

	DISCRETE_MIXER2(NODE_90,       \
					1, /* ENAB */  \
					NODE_80,  \
					NODE_12,  \
					&mix1)

	DISCRETE_OUTPUT(NODE_90, 2500)

DISCRETE_SOUND_END

void sm_state::spacmissx_07_w(u8 data)
{
	m_discrete->write(NODE_01, data | 0xc0);
}

void sm_state::spacmissx_03_w(u8 data)
{
	u8 rising_bits = data & ~m_port_1_last_extra;

	if (BIT(rising_bits, 1)) m_samples->start(2, 2);     /* Killed an enemy */
	if (BIT(rising_bits, 2)) m_samples->start(1, 1);     /* Lost a life */

	m_port_1_last_extra = data;
}

// bits 0-3 make a variable background tone
void sm_state::spacmissx_05_w(u8 data)
{
	if (BIT(m_port_1_last_extra, 5))
		m_discrete->write(NODE_02, data & 0x0f);
	else
		m_discrete->write(NODE_02, 0);

	u8 rising_bits = data & ~m_port_2_last_extra;

	if (BIT(rising_bits, 4)) m_samples->start(0, 0);     /* Shoot */

	m_flip_screen = BIT(data, 5) & ioport("CAB")->read();

	m_port_2_last_extra = data;
}

u8 sm_state::vpos_to_vysnc_chain_counter( int vpos )
{
	/* convert from a vertical position to the actual values on the vertical sync counters */
	u8 counter;
	int vblank = (vpos >= MW8080BW_VBSTART);

	if (vblank)
		counter = vpos - MW8080BW_VBSTART + MW8080BW_VCOUNTER_START_VBLANK;
	else
		counter = vpos + MW8080BW_VCOUNTER_START_NO_VBLANK;

	return counter;
}


int sm_state::vysnc_chain_counter_to_vpos( u8 counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}


TIMER_CALLBACK_MEMBER(sm_state::mw8080bw_interrupt_callback)
{
	u8 next_counter;
	int next_vpos;
	int next_vblank;

	/* compute vector and set the interrupt line */
	int vpos = m_screen->vpos();
	u8 counter = vpos_to_vysnc_chain_counter(vpos);
	u8 vector = 0xc7 | ((counter & 0x40) >> 2) | ((~counter & 0x40) >> 3);
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


void sm_state::mw8080bw_create_interrupt_timer(  )
{
	m_interrupt_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(sm_state::mw8080bw_interrupt_callback),this));
}


void sm_state::mw8080bw_start_interrupt_timer(  )
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));
}

/*************************************
 *
 *  Machine setup
 *
 *************************************/

MACHINE_START_MEMBER( sm_state, sm )
{
	mw8080bw_create_interrupt_timer();
}


/*************************************
 *
 *  Machine reset
 *
 *************************************/

MACHINE_RESET_MEMBER( sm_state, sm )
{
	mw8080bw_start_interrupt_timer();
}



u32 sm_state::screen_update_spacmissx(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	u8 x = 0;
	u8 y = MW8080BW_VCOUNTER_START_NO_VBLANK;
	u8 video_data = 0;
	u8 flip = m_flip_screen;

	while (1)
	{
		/* plot the current pixel */
		pen_t pen = (video_data & 0x01) ? rgb_t(255,255,255) : rgb_t(0,0,0);

		if (flip)
			bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - x) = pen;
		else
			bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = pen;

		/* next pixel */
		video_data = video_data >> 1;
		x = x + 1;

		/* end of line? */
		if (x == 0)
		{
			/* yes, flush out the shift register */
			int i;

			for (i = 0; i < 4; i++)
			{
				pen = (video_data & 0x01) ? rgb_t(255,255,255) : rgb_t(0,0,0);

				if (flip)
					bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - (256 + i)) = pen;
				else
					bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, 256 + i) = pen;

				video_data = video_data >> 1;
			}

			/* next row, video_data is now 0, so the next line will start
			   with 4 blank pixels */
			y = y + 1;

			/* end of screen? */
			if (y == 0)
				break;
		}
		/* the video RAM is read at every 8 pixels starting with pixel 4 */
		else if ((x & 0x07) == 0x04)
		{
			offs_t offs = ((offs_t)y << 5) | (x >> 3);
			video_data = m_p_ram[offs];
		}
	}

	return 0;
}

u8 sm_state::spacmissx_02_r()
{
	u8 data = ioport("IN2")->read();
	if (m_flip_screen) return data;
	return (data & 0x8f) | (ioport("IN1")->read() & 0x70);
}

void sm_state::mem_map(address_map &map) {
	map.global_mask(0x7fff);
	map(0x0000,0x1fff).rom().nopw();
	map(0x2000,0x3fff).mirror(0x4000).ram().share("ram");
	map(0x4000,0x5fff).rom().nopw();
}

void sm_state::io_map(address_map &map) {
	map(0x00,0x00).portr("IN0");
	map(0x01,0x01).portr("IN1");
	map(0x02,0x02).r(FUNC(sm_state::spacmissx_02_r)).w("mb14241",FUNC(mb14241_device::shift_count_w));
	map(0x03,0x03).r("mb14241",FUNC(mb14241_device::shift_result_r)).w(FUNC(sm_state::spacmissx_03_w));
	map(0x04,0x04).w("mb14241",FUNC(mb14241_device::shift_data_w));
	map(0x05,0x05).w(FUNC(sm_state::spacmissx_05_w));
	map(0x06,0x06).nopw();  //(watchdog_reset_w)
	map(0x07,0x07).w(FUNC(sm_state::spacmissx_07_w));
}

static const char *const invaders_sample_names[] =
{
	"*invaders",
	"1",        /* shot/missle */
	"2",        /* base hit/explosion */
	"3",        /* invader hit */
	"4",        /* fleet move 1 */
	"5",        /* fleet move 2 */
	"6",        /* fleet move 3 */
	"7",        /* fleet move 4 */
	"8",        /* UFO/saucer hit */
	"9",        /* bonus base */
	0
};


void sm_state::spacmissx(machine_config &config)
{
	/* basic machine hardware */
	I8080(config, m_maincpu, MW8080BW_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &sm_state::mem_map);
	m_maincpu->set_addrmap(AS_IO, &sm_state::io_map);

	MCFG_MACHINE_START_OVERRIDE(sm_state, sm)
	MCFG_MACHINE_RESET_OVERRIDE(sm_state, sm)

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(sm_state::screen_update_spacmissx));

	/* add shifter */
	MB14241(config, "mb14241");

	/* sound hardware */
	SPEAKER(config, "mono").front_center();
	SAMPLES(config, m_samples);
	m_samples->set_channels(6);
	m_samples->set_samples_names(invaders_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 0.50);

	DISCRETE(config, m_discrete, spacmissx_disc);
	m_discrete->add_route(ALL_OUTPUTS, "mono", 0.50);
}


static INPUT_PORTS_START( spacmissx )
	PORT_START("IN0")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNKNOWN ) // gets read into memory (0x2012) then never used

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  ) PORT_2WAY
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_2WAY
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNKNOWN )

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x02, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, "4000" )
	PORT_DIPSETTING(    0x02, "5000" )
	PORT_DIPSETTING(    0x03, "7000" )
	PORT_DIPNAME( 0x04, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x04, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x08, "6" )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNKNOWN )

	/* Dummy port for cocktail mode */
	PORT_START("CAB")
	PORT_CONFNAME( 0x01, 0x00, DEF_STR( Cabinet ) )
	PORT_CONFSETTING(    0x00, DEF_STR( Upright ) )
	PORT_CONFSETTING(    0x01, DEF_STR( Cocktail ) )
INPUT_PORTS_END

ROM_START( spacmissx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "1",       0x0000, 0x0800, CRC(e212dc88) SHA1(bc56052bf43d18081f777b936b2be792e91ba842) )
	ROM_LOAD( "2",       0x0800, 0x0800, CRC(f97410ee) SHA1(47f1f296c905fa13f6c521edc12c10f1f0e42400) )
	ROM_LOAD( "3",       0x1000, 0x0800, CRC(c1175feb) SHA1(83bf955ed3a52e1ce8c688d89725d8dee1bcc866) )
	ROM_LOAD( "4",       0x1800, 0x0800, CRC(b4451d7c) SHA1(62a18e8e927ef00a7f6cb933cdc5aeae9f074dc0) )
	ROM_LOAD( "5",       0x4000, 0x0800, CRC(74c9da61) SHA1(cb98105729f0fa4343b71af3c658b378ade1ed46) )
	ROM_LOAD( "6",       0x4800, 0x0800, CRC(5e7c6c44) SHA1(be7eeef10462377909018cf40503766f38466022) )
	ROM_LOAD( "7",       0x5000, 0x0800, CRC(02619e18) SHA1(4c59f17fbc96ca08090f08c41ca9fc72c074e9c0) )

	ROM_REGION( 0x0800, "user1", 0 )
	ROM_LOAD( "8",       0x0000, 0x0800, CRC(942e5261) SHA1(e8af51d644eab4e7b31c14dc66bb036ad8940c42) ) // ?
ROM_END

GAMEL(1980?,spacmissx, 0, spacmissx, spacmissx, sm_state, empty_init, ROT270, "bootleg?", "Space Missile - Space Fighting Game (Extra Sounds)", MACHINE_SUPPORTS_SAVE, layout_spacmissx )
