// license:GPL_2.0
// copyright-holders:Robbbert
/****************************************************************************************

Yosaku, recreated by AI and Stuey's Repair Shop.

The maker shows as OLCA CORPORATION, assumed to actually be ORCA.

Game seems to be mostly fine, except when the wild boar appears. In the invulnerability
 mode you simply swing your axe at it and it dies. But in normal mode swinging the axe
 does nothing and the player loses a life.
If you pause for a bit, a supercharged snake may kill you from below.
Because of these things, a game generally lasts less than a minute, even with 6 lives.

*******************************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/mb14241.h"
#include "machine/watchdog.h"
#include "sound/samples.h"
#include "sound/sn76477.h"
#include "sound/discrete.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"

namespace {

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


class yosaku_class : public driver_device
{
public:
	yosaku_class(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_palette(*this, "palette")
		, m_main_ram(*this, "main_ram")
		, m_colorram(*this, "colorram")
		, m_mb14241(*this,"mb14241")
		, m_watchdog(*this, "watchdog")
		, m_samples(*this, "samples")
		, m_sn(*this, "snsnd")
		, m_discrete(*this, "discrete")
		, m_screen(*this, "screen")
	{ }

	void yosaku(machine_config &config);

protected:

	void machine_start() override;
	void machine_reset() override;
	void mem_map(address_map &map);
	void io_map(address_map &map);
	required_device<i8080_cpu_device> m_maincpu;
	required_device<palette_device> m_palette;
	required_shared_ptr<u8> m_main_ram;
	required_shared_ptr<u8> m_colorram;
	optional_device<mb14241_device> m_mb14241;
	optional_device<watchdog_timer_device> m_watchdog;
	optional_device<samples_device> m_samples;
	required_device<sn76477_device> m_sn;
	required_device<discrete_sound_device> m_discrete;
	required_device<screen_device> m_screen;
	// video
	u32 screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	bool m_flip_screen = false;
	void colour_w(offs_t, uint8_t);
	// interrupts
	bool m_int_enable = 0;
	void int_enable_w(int);
	TIMER_CALLBACK_MEMBER(interrupt_trigger);
	IRQ_CALLBACK_MEMBER(interrupt_vector);
	attotime m_interrupt_time;
	emu_timer *m_interrupt_timer = nullptr;
	int vpos_to_vysnc_chain_counter(int);
	int vysnc_chain_counter_to_vpos(uint8_t, int);
	// sound
	void port00_w(uint8_t);
	void port03_w(uint8_t);
	void port05_w(uint8_t);
	uint8_t m_port03 = 0;
	uint8_t m_port05 = 0;
};

static const discrete_dac_r1_ladder indianbt_music_dac =
	{3, {0, RES_K(47), RES_K(12)}, 0, 0, 0, CAP_U(0.1)};

#define INDIANBT_MUSIC_CLK      (7680.0*2*2*2)

/* Nodes - Inputs */
#define INDIANBT_MUSIC_DATA     NODE_01
/* Nodes - Sounds */
#define INDIANBT_MUSIC          NODE_11

DISCRETE_SOUND_START(indianbt_discrete)

	DISCRETE_INPUT_DATA (INDIANBT_MUSIC_DATA)

/******************************************************************************
 *
 * Music Generator
 *
 ******************************************************************************/
	DISCRETE_NOTE(NODE_20, 1, INDIANBT_MUSIC_CLK, INDIANBT_MUSIC_DATA, 255, 5, DISC_CLK_IS_FREQ)

	// Convert count to 7492 output
	DISCRETE_TRANSFORM2(NODE_21, NODE_20, 2, "01>0+")

	DISCRETE_DAC_R1(NODE_22, NODE_21, DEFAULT_TTL_V_LOGIC_1, &indianbt_music_dac)

/******************************************************************************
 *
 * Final Mixing and Output
 *
 ******************************************************************************/
	DISCRETE_CRFILTER(NODE_90, NODE_22, RES_K(10), CAP_U(0.1))

	DISCRETE_OUTPUT(NODE_90, 21000)

DISCRETE_SOUND_END

const char *const yosaku_sample_names[] =
{
	"*yosaku",
	"cap",      // boar
	"bark",     // beat thud, running on into the start tune
	"walk1",    // tree falls
	"walk2",    // boar hit (groan)
	"warp",     // axe chop
	"extend",   // bird
	"kick",     // death tune
	nullptr
};

void yosaku_class::port00_w (uint8_t data)
{
	m_discrete->write(INDIANBT_MUSIC_DATA, data);
}

void yosaku_class::port03_w(uint8_t data)
{
	uint8_t rising_bits = data & ~m_port03;
	static uint8_t lupin3_step = 2;

	if (BIT(rising_bits, 0))
	{
		m_samples->start(0, lupin3_step);
		lupin3_step ^= 1;
	}

	m_sn->enable_w(BIT(~data, 1));

	if (BIT(rising_bits, 2)) m_samples->start(1, 4);
	if (BIT(rising_bits, 3)) m_samples->start(0, 0);
	if (BIT(rising_bits, 4)) m_samples->start(2, 5);

	m_port03 = data;
}

void yosaku_class::port05_w(uint8_t data)
{
	uint8_t rising_bits = data & ~m_port05;

	if (rising_bits & 0x01) m_samples->start(0, 6);
	if (rising_bits & 0x10) m_samples->start(3, 1);

	m_flip_screen = BIT(data, 5) & BIT(ioport("IN2")->read(), 2);

	m_port05 = data;
}

void yosaku_class::machine_start()
{
	m_interrupt_timer = timer_alloc(FUNC(yosaku_class::interrupt_trigger), this);
	save_item(NAME(m_flip_screen));
	save_item(NAME(m_port03));
	save_item(NAME(m_port05));
}


void yosaku_class::machine_reset()
{
	m_flip_screen = 0;

	port03_w(0);
	port05_w(0);

	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));

	m_interrupt_time = attotime::zero;
}

int yosaku_class::vpos_to_vysnc_chain_counter( int vpos )
{
	/* convert from a vertical position to the actual values on the vertical sync counters */
	int counter;
	int vblank = (vpos >= MW8080BW_VBSTART);

	if (vblank)
		counter = vpos - MW8080BW_VBSTART + MW8080BW_VCOUNTER_START_VBLANK;
	else
		counter = vpos + MW8080BW_VCOUNTER_START_NO_VBLANK;

	return counter;
}


int yosaku_class::vysnc_chain_counter_to_vpos( u8 counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}

TIMER_CALLBACK_MEMBER(yosaku_class::interrupt_trigger)
{
	int const vpos = m_screen->vpos();
	uint8_t const counter = vpos_to_vysnc_chain_counter(vpos);

	if (m_int_enable)
	{
		m_maincpu->set_input_line(0, ASSERT_LINE);
		m_interrupt_time = machine().time();
	}
	else
		m_maincpu->set_input_line(0, CLEAR_LINE);

	// set up for next interrupt
	uint8_t next_counter;
	int next_vblank;
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

	int const next_vpos = vysnc_chain_counter_to_vpos(next_counter, next_vblank);
	m_interrupt_timer->adjust(m_screen->time_until_pos(next_vpos));
}

void yosaku_class::int_enable_w(int state)
{
	m_int_enable = state;
}

IRQ_CALLBACK_MEMBER(yosaku_class::interrupt_vector)
{
	int vpos = m_screen->vpos();
	// MAME scheduling quirks cause this to happen more often than you might think, in fact far too often
	if (machine().time() < m_interrupt_time)
		vpos++;
	uint8_t counter = vpos_to_vysnc_chain_counter(vpos);
	uint8_t vector = 0xc7 | ((counter & 0x40) >> 2) | ((~counter & 0x40) >> 3);

	m_maincpu->set_input_line(0, CLEAR_LINE);
	return vector;
}

void yosaku_class::colour_w(offs_t offset, u8 data)
{
	m_colorram[offset & 0x1f9f] = ~data;
}

uint32_t yosaku_class::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	for (offs_t offs = 0; offs < m_main_ram.bytes(); offs++)
	{
		uint8_t x = offs << 3;
		uint8_t y = (offs >> 5) - MW8080BW_VCOUNTER_START_NO_VBLANK;
		if (cliprect.contains(x,y))
		{
			uint8_t data = m_main_ram[offs];
			uint8_t fg = m_colorram[offs & 0x1f9f] & 7;

			for (uint8_t i = 0; i < 8; i++)
			{
				uint8_t color = BIT(data, 0) ? fg : 0;
				if (m_flip_screen)
					bitmap.pix(MW8080BW_VBSTART - 1 - y, MW8080BW_HPIXCOUNT - 1 - x) = m_palette->pen_color(color);
				else
					bitmap.pix(y, x) = m_palette->pen_color(color);

				x++;
				data >>= 1;
			}
		}
	}

	return 0;
}

void yosaku_class::mem_map(address_map &map)
{
	map(0x0000, 0x1fff).rom();
	map(0x2000, 0x3fff).ram().share("main_ram");
	map(0xc000, 0xdfff).share("colorram").w(FUNC(yosaku_class::colour_w));
}


void yosaku_class::io_map(address_map &map)
{
	map(0x00, 0x00).portr("IN0").w(FUNC(yosaku_class::port00_w));
	map(0x01, 0x01).portr("IN1");
	map(0x02, 0x02).portr("IN2").w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x03, 0x03).r(m_mb14241, FUNC(mb14241_device::shift_result_r)).w(FUNC(yosaku_class::port03_w));
	map(0x04, 0x04).w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x05, 0x05).w(FUNC(yosaku_class::port05_w));
	map(0x06, 0x06).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
}

static INPUT_PORTS_START( yosaku )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNKNOWN ) // selects color mode (dynamic vs. static)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNKNOWN ) // something has to do with sound
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_BUTTON1) PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:1,2")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPSETTING(    0x02, "5" )
	PORT_DIPSETTING(    0x03, "6" )
	PORT_DIPNAME( 0x04, 0x00, DEF_STR( Cabinet ) )      PORT_DIPLOCATION("SW1:3")
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(    0x08, "1500" )
	PORT_DIPSETTING(    0x00, "3000" )
	PORT_DIPUNUSED_DIPLOC( 0x10, 0x00, "SW1:5" )
	PORT_DIPUNUSED_DIPLOC( 0x20, 0x00, "SW1:6" )
	PORT_DIPUNUSED_DIPLOC( 0x40, 0x00, "SW1:7" )
	PORT_DIPNAME(0x80,  0x00, "Invulnerability (Cheat)") PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x80, DEF_STR( On ) )
INPUT_PORTS_END


void yosaku_class::yosaku(machine_config &config)
{
	/* basic machine hardware */
	I8080(config, m_maincpu, MW8080BW_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &yosaku_class::mem_map);
	m_maincpu->set_addrmap(AS_IO, &yosaku_class::io_map);
	m_maincpu->set_irq_acknowledge_callback(FUNC(yosaku_class::interrupt_vector));
	m_maincpu->out_inte_func().set(FUNC(yosaku_class::int_enable_w));

	/* video hardware */
	SCREEN(config, m_screen);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(yosaku_class::screen_update));
	PALETTE(config, m_palette, palette_device::RBG_3BIT);

	WATCHDOG_TIMER(config, m_watchdog);
	//MCFG_MACHINE_START_OVERRIDE(yosaku_class,sflush)

	// add shifter
	MB14241(config, m_mb14241);

	// sound hardware
	SPEAKER(config, "mono").front_center();

	sn76477_device &snsnd(SN76477(config, "snsnd"));
	snsnd.set_noise_params(0, 0, 0);
	snsnd.set_decay_res(0);
	snsnd.set_attack_params(0, RES_K(100));
	snsnd.set_amp_res(RES_K(56));
	snsnd.set_feedback_res(RES_K(10));
	snsnd.set_vco_params(0, CAP_U(0.1), RES_K(8.2));
	snsnd.set_pitch_voltage(5.0);
	snsnd.set_slf_params(CAP_U(1.0), RES_K(120));
	snsnd.set_oneshot_params(0, 0);
	snsnd.set_vco_mode(1);
	snsnd.set_mixer_params(0, 0, 0);
	snsnd.set_envelope_params(1, 0);
	snsnd.set_enable(1);
	snsnd.add_route(ALL_OUTPUTS, "mono", 0.5);

	SAMPLES(config, m_samples);
	m_samples->set_channels(4);
	m_samples->set_samples_names(yosaku_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 1.0);

	DISCRETE(config, m_discrete, indianbt_discrete);
	m_discrete->add_route(ALL_OUTPUTS, "mono", 1.0);
}

ROM_START( yosaku )
	ROM_REGION( 0x10000, "maincpu", 0 )
#if 0
	// v1
	ROM_LOAD( "yosaku.1", 0x0000, 0x0800, CRC(37fecee4) SHA1(e1ca44dcf42d7c9ed67044cef8d23bb5fd044f97) )
	ROM_LOAD( "yosaku.2", 0x0800, 0x0800, CRC(ceb6c3b4) SHA1(603392413124eec1acf8bd3a07590e270d41bfb7) )
	ROM_LOAD( "yosaku.3", 0x1000, 0x0800, CRC(c433a113) SHA1(ae793e9e550f88d875854fa585d56c089971bbb8) )
	ROM_LOAD( "yosaku.4", 0x1800, 0x0800, CRC(6cac14c1) SHA1(db3e37771ed8b440fdd7f8b6b4fd74c8da131977) )
#endif
	// v2
	ROM_LOAD( "yosaku.1", 0x0000, 0x0800, CRC(19bc1db4) SHA1(fbdb8f611c7c767379402ebf35c25588eef447a3) )
	ROM_LOAD( "yosaku.2", 0x0800, 0x0800, CRC(1fb43a59) SHA1(a9e8f4002922be5a3b3b0813b478acb017adb219) )
	ROM_LOAD( "yosaku.3", 0x1000, 0x0800, CRC(c418d5a3) SHA1(a8cbdbaa3a19644b217f784b7fe78d748389bd43) )
	ROM_LOAD( "yosaku.4", 0x1800, 0x0800, CRC(b76d9e6f) SHA1(479494503d22bb718c09a111427c59bc60e9e3fd) )
ROM_END

} // anonymous namespace

GAME( 1979, yosaku, 0, yosaku, yosaku, yosaku_class, empty_init, ROT270, "Orca", "Yosaku", MACHINE_SUPPORTS_SAVE )

