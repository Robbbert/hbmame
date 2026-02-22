// license:GPL_2.0
// copyright-holders:Robbbert
/****************************************************************************************

Space Chaser (all versions)

Original driver for CV version, August 2008 by Robbbert
Added all the other versions and fixed various bugs, February 2026.

General game bugs:
- If you have 6 spare ships, one of them is in the same spot as the F in FUEL.
- The so-called "Hard" setting is buggy. It should not be used.

Each game will award an extra ship at 10,000 points, or 20,000 points for
schaserc, schaserm and crashrd.

*******************************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/mb14241.h"
#include "machine/timer.h"
#include "machine/watchdog.h"
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


class schaser_state : public driver_device
{
public:
	schaser_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_palette(*this, "palette")
		, m_prom(*this, "proms")
		, m_main_ram(*this, "main_ram")
		, m_colorram(*this, "colorram")
		, m_mb14241(*this,"mb14241")
		, m_watchdog(*this, "watchdog")
		, m_sn(*this, "snsnd")
		, m_discrete(*this, "discrete")
		, m_555_timer(*this, "timer_555")
		, m_screen(*this, "screen")
	{ }

	void schasercv(machine_config &config);
	void schaser(machine_config &config);

protected:

	void machine_start() override;
	void machine_reset() override;
	void mem_map(address_map &map);
	void io_map(address_map &map);
	required_device<i8080_cpu_device> m_maincpu;
	required_device<palette_device> m_palette;
	optional_region_ptr<u8> m_prom;
	required_shared_ptr<u8> m_main_ram;
	required_shared_ptr<u8> m_colorram;
	optional_device<mb14241_device> m_mb14241;
	optional_device<watchdog_timer_device> m_watchdog;
	required_device<sn76477_device> m_sn;
	required_device<discrete_sound_device> m_discrete;
	required_device<timer_device> m_555_timer;
	required_device<screen_device> m_screen;
	// video
	u32 screen_update_schaser(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	bool m_flip_screen = false;
	void colour_w(offs_t, uint8_t);
	uint8_t m_green_disable = 0; // always enabled?
	uint8_t m_green_select = 0;
	// interrupts
	TIMER_CALLBACK_MEMBER(interrupt_callback);
	attotime m_interrupt_time;
	emu_timer *m_interrupt_timer = nullptr;
	int vpos_to_vysnc_chain_counter(int);
	int vysnc_chain_counter_to_vpos(uint8_t, int);
	void create_interrupt_timer();
	void start_interrupt_timer();
	// sound
	attotime m_555_time_remain{};
	TIMER_DEVICE_CALLBACK_MEMBER(effect_555_cb);
	void port03_w(uint8_t);
	void port05_w(uint8_t);
	void apply_effect(uint8_t, bool);
	uint8_t m_last_effect = 0;
	bool m_sound_en = false;
	bool m_explosion = false;
	bool m_555_is_low = false;
};

/*
 *  The dot sound is a square wave clocked by either the
 *  the 8V or 4V signals
 *
 *  The frequencies are (for the 8V signal):
 *
 *  19.968 MHz crystal / 2 (Qa of 74160 #10) -> 9.984MHz
 *                     / 2 (7474 #14) -> 4.992MHz
 *                     / 256+16 (74161 #5 and #8) -> 18352.94Hz
 *                     / 8 (8V) -> 2294.12 Hz
 *                     / 2 the final freq. is 2 toggles -> 1147.06Hz
 *
 *  for 4V, it's double at 2294.12Hz
 */
#define SCHASER_HSYNC   18352.94
#define SCHASER_4V      SCHASER_HSYNC /2 /4
#define SCHASER_8V      SCHASER_HSYNC /2 /8

/* Nodes - Inputs */
#define SCHASER_DOT_EN      NODE_01
#define SCHASER_DOT_SEL     NODE_02
#define SCHASER_EXP_STREAM  NODE_03
#define SCHASER_MUSIC_BIT   NODE_04
#define SCHASER_SND_EN      NODE_05
/* Nodes - Adjusters */
#define SCHASER_VR1         NODE_07
#define SCHASER_VR2         NODE_08
#define SCHASER_VR3         NODE_09
/* Nodes - Sounds */
#define SCHASER_DOT_SND     NODE_10
#define SCHASER_EXP_SND     NODE_11
#define SCHASER_MUSIC_SND   NODE_12

DISCRETE_SOUND_START(schaser_discrete)
	/************************************************/
	/* Input register mapping for schaser           */
	/************************************************/
	DISCRETE_INPUT_LOGIC  (SCHASER_DOT_EN)
	DISCRETE_INPUT_LOGIC  (SCHASER_DOT_SEL)
	// scale to 0-3.5V
	DISCRETE_INPUTX_STREAM(SCHASER_EXP_STREAM, 0, 1.0/14100,             2.323)
	DISCRETE_INPUTX_LOGIC (SCHASER_MUSIC_BIT,    DEFAULT_TTL_V_LOGIC_1,  0,      0.0)
	DISCRETE_INPUT_LOGIC  (SCHASER_SND_EN)

	/************************************************/
	/* Volume adjusters.                            */
	/* We will set them to adjust the relative      */
	/* gains.                                       */
	/************************************************/
	DISCRETE_ADJUSTMENT(SCHASER_VR1, 0, RES_K(50)/(RES_K(50) + RES_K(470)), DISC_LINADJ, "VR1")
	DISCRETE_ADJUSTMENT(SCHASER_VR2, 0, RES_K(50)/(RES_K(50) + 560 + RES_K(6.8) + RES_K(2)), DISC_LINADJ, "VR2")
	DISCRETE_ADJUSTMENT(SCHASER_VR3, 0, RES_K(50)/(RES_K(50) + 560 + RES_K(6.8) + RES_K(10)), DISC_LINADJ, "VR3")

	/************************************************/
	/* Dot selection just selects between 4V and 8V */
	/************************************************/
	DISCRETE_SQUAREWFIX(NODE_20, 1, SCHASER_4V, DEFAULT_TTL_V_LOGIC_1, 50, 0, 0)
	DISCRETE_SQUAREWFIX(NODE_21, 1, SCHASER_8V, DEFAULT_TTL_V_LOGIC_1, 50, 0, 0)
	DISCRETE_SWITCH(NODE_22, SCHASER_DOT_EN, SCHASER_DOT_SEL, NODE_20, NODE_21)
	DISCRETE_RCFILTER(NODE_23, NODE_22, 560, CAP_U(.1))
	DISCRETE_RCFILTER(NODE_24, NODE_23, RES_K(6.8) + 560, CAP_U(.1))
	DISCRETE_MULTIPLY(SCHASER_DOT_SND, NODE_24, SCHASER_VR3)

	/************************************************/
	/* Explosion/Effect filtering                   */
	/************************************************/
	DISCRETE_RCFILTER(NODE_30, SCHASER_EXP_STREAM, 560, CAP_U(.1))
	DISCRETE_RCFILTER(NODE_31, NODE_30, RES_K(6.8) + 560, CAP_U(.1))
	DISCRETE_CRFILTER(NODE_32, NODE_31, RES_K(6.8) + 560 + RES_K(2) + RES_K(50), CAP_U(1))
	DISCRETE_MULTIPLY(SCHASER_EXP_SND, NODE_32, SCHASER_VR2)

	/************************************************/
	/* Music is just a 1 bit DAC                    */
	/************************************************/
	DISCRETE_CRFILTER(NODE_40, SCHASER_MUSIC_BIT, RES_K(470) + RES_K(50), CAP_U(.01))
	DISCRETE_MULTIPLY(SCHASER_MUSIC_SND, NODE_40, SCHASER_VR1)

	/************************************************/
	/* Final mix with gain                          */
	/************************************************/
	DISCRETE_ADDER3(NODE_90, SCHASER_SND_EN, SCHASER_DOT_SND, SCHASER_EXP_SND, SCHASER_MUSIC_SND)

	DISCRETE_OUTPUT(NODE_90, 33080)
DISCRETE_SOUND_END

static const double schaser_effect_rc[8] =
{
	0,
	(RES_K(15) + RES_K(20)) * CAP_U(1),
	(RES_K(39) + RES_K(20)) * CAP_U(1),
	(1.0/ (1.0/RES_K(15) + 1.0/RES_K(39)) + RES_K(20)) * CAP_U(1),
	(RES_K(82) + RES_K(20)) * CAP_U(1),
	(1.0/ (1.0/RES_K(15) + 1.0/RES_K(82)) + RES_K(20)) * CAP_U(1),
	(1.0/ (1.0/RES_K(15) + 1.0/RES_K(82)) + RES_K(20)) * CAP_U(1),
	(1.0/ (1.0/RES_K(15) + 1.0/RES_K(39) + 1.0/RES_K(82)) + RES_K(20)) * CAP_U(1)
};

TIMER_DEVICE_CALLBACK_MEMBER(schaser_state::effect_555_cb)
{
	int effect = param;
	attotime new_time{};

	/* Toggle 555 output */
	m_555_is_low ^= 1;
	m_555_time_remain = attotime::zero;

	if (m_555_is_low)
		new_time = PERIOD_OF_555_ASTABLE(0, RES_K(20), CAP_U(1)) / 2;
	else
	{
		if (effect)
			new_time = attotime(0, ATTOSECONDS_PER_SECOND * .8873 * schaser_effect_rc[effect]);
		else
			new_time = attotime::never;
	}
	m_555_timer->adjust(new_time, m_last_effect);
	m_sn->enable_w(!(m_555_is_low | m_explosion));
	m_sn->one_shot_cap_voltage_w(!(m_555_is_low | m_explosion) ? 0 : sn76477_device::EXTERNAL_VOLTAGE_DISCONNECT);
}

void schaser_state::port03_w(uint8_t data)
{
	/* bit 0 - Dot Sound Enable (SX0)
	   bit 1 - Dot Sound Pitch (SX1)
	   bit 2 - Effect Sound A (SX2)
	   bit 3 - Effect Sound B (SX3)
	   bit 4 - Effect Sound C (SX4)
	   bit 5 - Explosion (SX5)

	    Note that the schematic has SX2 and SX4 the wrong way around.
	    See MT 2662 for video proof. */

	if (!m_sound_en)
		data = 0;

	m_explosion = BIT(data, 5);
	if (m_explosion)
		data = 0;

	m_discrete->write(SCHASER_DOT_EN, BIT(data, 0));
	m_discrete->write(SCHASER_DOT_SEL, BIT(data, 1));

	uint8_t effect = BIT(data, 2, 3);
	apply_effect(effect, true);
}

void schaser_state::apply_effect(uint8_t effect, bool remain)
{
	/* The effect is a variable rate 555 timer.  A diode/resistor array is used to
	 * select the frequency.  Because of the diode voltage drop, we can not use the
	 * standard 555 time formulas.  Also, when effect=0, the charge resistor
	 * is disconnected.  This causes the charge on the cap to slowly bleed off, but
	 * but the bleed time is so long, that we can just cheat and put the time on hold
	 * when effect = 0. */
	if (m_last_effect != effect)
	{
		if (effect)
		{
			if (m_555_time_remain != attotime::zero)
			{
				/* timer re-enabled, use up remaining 555 high time */
				m_555_timer->adjust(m_555_time_remain, effect);
			}
			else if (!m_555_is_low)
			{
				/* set 555 high time */
				attotime new_time = attotime(0, ATTOSECONDS_PER_SECOND * .8873 * schaser_effect_rc[effect]);
				m_555_timer->adjust(new_time, effect);
			}
		}
		else
		{
			/* disable effect - stops at end of low cycle */
			if (!m_555_is_low)
			{
				if (remain)
					m_555_time_remain = m_555_timer->time_left();
				else
					m_555_time_remain = attotime::zero;

				m_555_timer->adjust(attotime::never);
			}
		}
		m_last_effect = effect;
	}

	if (m_explosion)
		m_sn->amplitude_res_w(1.0 / (1.0/RES_K(200) + 1.0/RES_K(68)));
	else
		m_sn->amplitude_res_w(RES_K(200));

	m_sn->enable_w(!(m_555_is_low || m_explosion));
	m_sn->one_shot_cap_voltage_w(!(m_555_is_low || m_explosion) ? 0 : sn76477_device::EXTERNAL_VOLTAGE_DISCONNECT);
	m_sn->mixer_b_w(m_explosion);
}

void schaser_state::port05_w(uint8_t data)
{
	/* bit 0 - Music (DAC) (SX6)
	   bit 1 - Sound Enable (SX7)
	   bit 2 - Coin Lockout (SX8)
	   bit 3 - Field Control A (SX9)
	   bit 4 - Field Control B (SX10)
	   bit 5 - Flip Screen */

	m_sound_en = BIT(data, 1);
	m_discrete->write(SCHASER_SND_EN, m_sound_en);
	m_discrete->write(SCHASER_MUSIC_BIT, BIT(data, 0));

	machine().bookkeeping().coin_lockout_global_w(BIT(data, 2));

	m_green_disable = BIT(data, 3);
	m_green_select = BIT(data, 4);

	m_flip_screen = BIT(data, 5) & BIT(ioport("IN2")->read(), 6);
}

void schaser_state::machine_start()
{
	save_item(NAME(m_flip_screen));
	save_item(NAME(m_sound_en));
	save_item(NAME(m_explosion));
	save_item(NAME(m_555_is_low));
	create_interrupt_timer();
}


void schaser_state::machine_reset()
{
	m_flip_screen = 0;
	m_sound_en = 0;
	m_explosion = 0;
	m_555_is_low = 0;
	m_555_time_remain = attotime::zero;
	m_555_timer->adjust(attotime::never);

	port03_w(0);
	port05_w(0);
	start_interrupt_timer();
}

int schaser_state::vpos_to_vysnc_chain_counter( int vpos )
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


int schaser_state::vysnc_chain_counter_to_vpos( u8 counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}


TIMER_CALLBACK_MEMBER(schaser_state::interrupt_callback)
{
	int next_counter, next_vblank;

	/* compute vector and set the interrupt line */
	int vpos = m_screen->vpos();
	int counter = vpos_to_vysnc_chain_counter(vpos);
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

	int next_vpos = vysnc_chain_counter_to_vpos(next_counter, next_vblank);
	m_interrupt_timer->adjust(m_screen->time_until_pos(next_vpos));
}


void schaser_state::create_interrupt_timer()
{
	m_interrupt_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(schaser_state::interrupt_callback),this));
}


void schaser_state::start_interrupt_timer()
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));
}

void schaser_state::colour_w(offs_t offset, u8 data)
{
	m_colorram[offset & 0x1f9f] = data;
}

uint32_t schaser_state::screen_update_schaser(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	for (offs_t offs = 0; offs < m_main_ram.bytes(); offs++)
	{
		uint8_t x = offs << 3;
		uint8_t y = (offs >> 5) - MW8080BW_VCOUNTER_START_NO_VBLANK;
		if (cliprect.contains(x,y))
		{
			uint8_t data = m_main_ram[offs];
			uint8_t bg = 2;
			uint8_t fg = m_colorram[offs & 0x1f9f] & 0x07;

			if (!m_green_disable && m_prom)
			{
				offs_t prom_address = (offs >> 8 << 5) | (offs & 0x1f);

				uint8_t prom_data = m_prom[prom_address] & 0x0c;

				if ((prom_data == 0x0c) && m_green_select)
					bg = 4;
			}

			for (uint8_t i = 0; i < 8; i++)
			{
				uint8_t color = BIT(data, 0) ? fg : bg;
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

void schaser_state::mem_map(address_map &map)
{
	map(0x0000, 0x1fff).rom();
	map(0x2000, 0x3fff).ram().share("main_ram");
	map(0x4000, 0x5fff).rom();
	map(0xc000, 0xdfff).share("colorram").w(FUNC(schaser_state::colour_w));
}


void schaser_state::io_map(address_map &map)
{
	map(0x00, 0x00).portr("IN0");
	map(0x01, 0x01).portr("IN1");
	map(0x02, 0x02).portr("IN2").w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x03, 0x03).r(m_mb14241, FUNC(mb14241_device::shift_result_r)).w(FUNC(schaser_state::port03_w));
	map(0x04, 0x04).w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x05, 0x05).w(FUNC(schaser_state::port05_w));
	map(0x06, 0x06).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
}

static INPUT_PORTS_START( schaser )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_DIPNAME( 0x60, 0x00, "Hard Starting Level" )   PORT_DIPLOCATION("SW1:5,7")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x20, "4" )
	PORT_DIPSETTING(    0x40, "5" )
	PORT_DIPSETTING(    0x60, "6" )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_COIN1 )

	PORT_START("IN2")   // port 2
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:1,2")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPSETTING(    0x02, "5" )
	PORT_DIPSETTING(    0x03, "6" )
	PORT_DIPUNKNOWN_DIPLOC( 0x04, 0x00, "SW1:3" )
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Difficulty ) )   PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(    0x00, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Hard ) )
	// Name Reset - if name of high scorer was rude, owner can press this button
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_OTHER ) PORT_NAME("Name Reset") PORT_CODE(KEYCODE_F1)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Cabinet ) )      PORT_DIPLOCATION("SW1:6")
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Cocktail ) )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("VR1")
	PORT_ADJUSTER( 70, "VR1 - Music Volume" )

	PORT_START("VR2")
	PORT_ADJUSTER( 90, "VR2 - Explosion/Effect Volume" )

	PORT_START("VR3")
	PORT_ADJUSTER( 70, "VR3 - Dot Volume" )
INPUT_PORTS_END


static INPUT_PORTS_START( schaserm )
	PORT_INCLUDE( schaser )

	PORT_MODIFY("IN2")
	PORT_DIPNAME( 0x03, 0x02, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:1,2")
	PORT_DIPSETTING(    0x00, "1" )
	PORT_DIPSETTING(    0x01, "2" )
	PORT_DIPSETTING(    0x02, "3" )
	PORT_DIPSETTING(    0x03, "4" )
INPUT_PORTS_END

void schaser_state::schaser(machine_config &config)
{
	/* basic machine hardware */
	I8080(config, m_maincpu, MW8080BW_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &schaser_state::mem_map);
	m_maincpu->set_addrmap(AS_IO, &schaser_state::io_map);

	WATCHDOG_TIMER(config, m_watchdog).set_vblank_count("screen", 255);

	TIMER(config, "timer_555").configure_generic(FUNC(schaser_state::effect_555_cb));

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(schaser_state::screen_update_schaser));
	PALETTE(config, m_palette, palette_device::RBG_3BIT);

	/* add shifter */
	MB14241(config, "mb14241");

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	sn76477_device &snsnd(SN76477(config, "snsnd"));
	snsnd.set_noise_params(RES_K(47), RES_K(330), CAP_P(470));   // noise + filter
	snsnd.set_decay_res(RES_M(2.2));
	snsnd.set_attack_params(CAP_U(1.0), RES_K(4.7));             // attack_decay_cap; attack resistor
	snsnd.set_amp_res(0);                                        // amplitude res
	snsnd.set_feedback_res(RES_K(33));
	snsnd.set_vco_params(0, CAP_U(0.1), RES_K(39));              // VCO volt; cap; res
	snsnd.set_pitch_voltage(5.0);
	snsnd.set_slf_params(CAP_U(1.0), RES_K(120));
	snsnd.set_oneshot_params(CAP_U(0.1), RES_K(220));
	snsnd.set_vco_mode(1);
	snsnd.set_mixer_params(0, 0, 0);                             // mixer A;B;C
	snsnd.set_envelope_params(1, 0);                             // envelope 1;2
	snsnd.set_enable(1);
	snsnd.add_route(0, "discrete", 0.30, 0);

	DISCRETE(config, m_discrete, schaser_discrete);
	m_discrete->add_route(ALL_OUTPUTS, "mono", 2.0);
}

ROM_START( schaser )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "rt13.bin",     0x0000, 0x0400, CRC(0dfbde68) SHA1(7367b138ad8448aba9222fed632a892df65cecbd) )
	ROM_LOAD( "rt14.bin",     0x0400, 0x0400, CRC(5a508a25) SHA1(c681d0bbf49317e79b596fb094e66b8912f0e409) )
	ROM_LOAD( "rt15.bin",     0x0800, 0x0400, CRC(2ac43a93) SHA1(d364f0940681a888c0147e06bcb01f8a0d4a24c8) )
	ROM_LOAD( "rt16.bin",     0x0c00, 0x0400, CRC(f5583afc) SHA1(5e8edb43ccb138fd47ac8f3da1af79b4444a4a82) )
	ROM_LOAD( "rt17.bin",     0x1000, 0x0400, CRC(51cf1155) SHA1(fd8c82d951602fd7e0ada65fc7cdee9f277c70db) )
	ROM_LOAD( "rt18.bin",     0x1400, 0x0400, CRC(3f0fc73a) SHA1(b801c3f1e8e6e41c564432db7c5891f6b27293b2) )
	ROM_LOAD( "rt19.bin",     0x1800, 0x0400, CRC(b66ea369) SHA1(d277f572f9c7c4301518546cf60671a6539326ee) )
	ROM_LOAD( "rt20.bin",     0x1c00, 0x0400, CRC(e3a7466a) SHA1(2378970f38b0cec066ef853a6540500e468e4ab4) )
	ROM_LOAD( "rt21.bin",     0x4000, 0x0400, CRC(b368ac98) SHA1(6860efe0496955db67611183be0efecda92c9c98) )
	ROM_LOAD( "rt22.bin",     0x4400, 0x0400, CRC(6e060dfb) SHA1(614e2ecf676c3ea2f9ea869125cfffef2f713684) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

ROM_START( schasera )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "rt13.bin",     0x0000, 0x0800, CRC(7b0bfeed) SHA1(832fe90430653d03cd0e7ea1b046524a2ca292ea) )
	ROM_LOAD( "rt15.bin",     0x0800, 0x0800, CRC(825fc8ac) SHA1(176ff0f4d0cd55be30efb184bd5bef62b92d0333) )
	ROM_LOAD( "rt17.bin",     0x1000, 0x0800, CRC(de9d3f85) SHA1(13a71fdd889023cfc65ed2c0a65236884b79b1f0) )
	ROM_LOAD( "rt19.bin",     0x1800, 0x0800, CRC(c0adab87) SHA1(4bb8e4ccfb5eaa052584555bfa03fecf19ab8a29) )
	ROM_LOAD( "rt21.bin",     0x4000, 0x0800, CRC(a3b31070) SHA1(af0108e1446a2be66cfc00d0b837fa91ab882441) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

ROM_START( schaserb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "rt33.bin",     0x0000, 0x0800, CRC(eec6b032) SHA1(da14fcd862d6b80531cd3b858034bc5a120ed8ae) )
	ROM_LOAD( "rt34.bin",     0x0800, 0x0800, CRC(13a73701) SHA1(48ddbc10dec458070274c9fabbb0c420e2a07c96) )
	ROM_LOAD( "rt35.bin",     0x1000, 0x0800, CRC(de9d3f85) SHA1(13a71fdd889023cfc65ed2c0a65236884b79b1f0) )
	ROM_LOAD( "rt36.bin",     0x1800, 0x0800, CRC(521ec25e) SHA1(ce53e882c11a4c36f3edc3b389d3f5ad0e0ec151) )
	ROM_LOAD( "rt37.bin",     0x4000, 0x0800, CRC(44f65f19) SHA1(ee97d7987f54c9c26f5a20c72bdae04c46f94dc4) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

ROM_START( schaserc )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "45.ic30",      0x0000, 0x0400, CRC(ca90619c) SHA1(d2f9b29290d720c57f867d1dc193e877248e6afd) )
	ROM_LOAD( "46.ic36",      0x0400, 0x0400, CRC(6a016895) SHA1(6984d9d002e5d8fa14bdaf16f6ba9ca02136372c) )
	ROM_LOAD( "rt15.bin",     0x0800, 0x0400, CRC(2ac43a93) SHA1(d364f0940681a888c0147e06bcb01f8a0d4a24c8) )
	ROM_LOAD( "rt16.bin",     0x0c00, 0x0400, CRC(f5583afc) SHA1(5e8edb43ccb138fd47ac8f3da1af79b4444a4a82) )
	ROM_LOAD( "rt17.bin",     0x1000, 0x0400, CRC(51cf1155) SHA1(fd8c82d951602fd7e0ada65fc7cdee9f277c70db) )
	ROM_LOAD( "rt18.bin",     0x1400, 0x0400, CRC(3f0fc73a) SHA1(b801c3f1e8e6e41c564432db7c5891f6b27293b2) )
	ROM_LOAD( "rt19.bin",     0x1800, 0x0400, CRC(b66ea369) SHA1(d277f572f9c7c4301518546cf60671a6539326ee) )
	ROM_LOAD( "47.ic39",      0x1c00, 0x0400, CRC(d476e182) SHA1(87428bf0131f8bf39d506b8df424af94cd944d82) )
	ROM_LOAD( "rt21.bin",     0x4000, 0x0400, CRC(b368ac98) SHA1(6860efe0496955db67611183be0efecda92c9c98) )
	ROM_LOAD( "rt22.bin",     0x4400, 0x0400, CRC(6e060dfb) SHA1(614e2ecf676c3ea2f9ea869125cfffef2f713684) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

ROM_START( schaserm )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mr26.71",     0x0000, 0x0800, CRC(4e547879) SHA1(464fab35373d6bd6218474e7f5109425376f1db2) )
	ROM_LOAD( "rt08.70",     0x0800, 0x0800, CRC(825fc8ac) SHA1(176ff0f4d0cd55be30efb184bd5bef62b92d0333) )
	ROM_LOAD( "rt09.69",     0x1000, 0x0800, CRC(de9d3f85) SHA1(13a71fdd889023cfc65ed2c0a65236884b79b1f0) )
	ROM_LOAD( "mr27.62",     0x1800, 0x0800, CRC(069ec108) SHA1(b12cd288d7e42002d01290f0572f9074adf2cdca) )
	ROM_LOAD( "rt11.61",     0x4000, 0x0800, CRC(17a7ef7a) SHA1(1a7b3f9393dceddcd1e220cadbff7e619594f884) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

//***************************************** CRASH ROAD ************************************************
/* Issues:
 - No effect sound - there's nothing to turn it on/off
 - When tested, the faster effect sounds are silent - only effect 4 works.
 - When it speeds up toward the end, the dot sound disappears - the on time is too short
 - In cocktail mode, there's no positive indication of player 2's turn
 - In cocktail mode, player 2 has no controls
 - The green bar turns off too soon
 - During testing one time, at the start of level 3, the screen cleared and the watchdog fired.
*/ 
class cr_state : public schaser_state
{
public:
	cr_state(const machine_config &mconfig, device_type type, const char *tag)
		: schaser_state(mconfig, type, tag)
	{ }

	void crashrd(machine_config &config);

private:

	void port03_w(u8 data);
	void port05_w(u8 data);
	void io_map(address_map &map);
};

void cr_state::port03_w(uint8_t data)
{
	/* bit 0 - Dot Sound Pitch (SX1) - changes when fuel used
	   bit 2 - Explosion (SX5)
	   bit 4 - Dot Sound Enable (SX0) - when dot is run over
	   bit 5 - Sound Enable */

	m_sound_en = BIT(data, 5);
	m_discrete->write(SCHASER_SND_EN, m_sound_en);
	if (!m_sound_en)
		data = 0;

	m_explosion = BIT(data, 2);

	if (m_explosion)
		data = 0;

	m_discrete->write(SCHASER_DOT_EN, BIT(data, 4));
	m_discrete->write(SCHASER_DOT_SEL, BIT(data, 0));

	if (m_explosion)
		m_sn->amplitude_res_w(1.0 / (1.0/RES_K(200) + 1.0/RES_K(68)));
	else
		m_sn->amplitude_res_w(RES_K(200));

	m_sn->enable_w(!m_explosion);
	m_sn->one_shot_cap_voltage_w(!m_explosion ? 0 : sn76477_device::EXTERNAL_VOLTAGE_DISCONNECT);
	m_sn->mixer_b_w(m_explosion);

	m_green_select = (m_main_ram[0x6f] == 0xff); // switches off too early
}

void cr_state::port05_w(uint8_t data)
{
	// bit 0 = bitstream audio
	// bit 4 = indicates when enemy goes faster toward end of level
	// bit 5 = random? doesn't seem to mean anything
	m_discrete->write(SCHASER_MUSIC_BIT, BIT(data, 0));
}

void cr_state::io_map(address_map &map)
{
	map(0x01, 0x01).portr("IN1");
	map(0x02, 0x02).portr("IN2").w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x03, 0x03).r(m_mb14241, FUNC(mb14241_device::shift_result_r)).w(FUNC(cr_state::port03_w));
	map(0x04, 0x04).w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x05, 0x05).w(FUNC(cr_state::port05_w));
	map(0x06, 0x06).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
}

void cr_state::crashrd(machine_config &config)
{
	schaser(config);
	m_maincpu->set_addrmap(AS_IO, &cr_state::io_map);
}

ROM_START( crashrd )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "2716-5m.bin",  0x0000, 0x0800, CRC(53749427) SHA1(213828eea2d5baeae9c6553a531ec4127d795a67) )
	ROM_LOAD( "2716-5n.bin",  0x0800, 0x0800, CRC(e391d768) SHA1(22a52f4a01b586489ec79d53817152594cc3189d) )
	ROM_LOAD( "2716-5p.bin",  0x1000, 0x0800, CRC(fededc5d) SHA1(205079ddc5893884476672d378a457b5a603f5ae) )
	ROM_LOAD( "2716-5r.bin",  0x1800, 0x0800, CRC(30830779) SHA1(dff2fa9244cd3769a167673668acb53a17c395b4) )
	ROM_LOAD( "2716-5s.bin",  0x4000, 0x0800, CRC(6a974917) SHA1(4f1a4003652ef47de3d5c270f5f624d172970ec5) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

//***************************************** SCHASERCV ************************************************
class sc_state : public schaser_state
{
public:
	sc_state(const machine_config &mconfig, device_type type, const char *tag)
		: schaser_state(mconfig, type, tag)
	{ }

	void schasercv(machine_config &config);

private:

	u8 port02_r();
	void port03_w(u8 data);
	void port05_w(u8 data);
	void machine_start() override;
	void machine_reset() override;
	void mem_map(address_map &map);
	void io_map(address_map &map);
	// sound
	u8 m_sound_seq = 0U;
};

void sc_state::port03_w(u8 data)
{
	/* bit 1 - Dot Sound Enable
	   bit 2 - Effect Sound A (a stream of pulses)
	   bit 3 - Effect Sound B (a stream of pulses)
	   bit 4 - Explosion (a stream of ff's with some fe's thrown in)
	   bit 5 - Goes high when first dot hit */

	u8 effect = 0U, byte = data & 0x2c;

	/* If you use fuel, the dot sound turns into a continuous beep.

	The effect sequence is really bizarre, however a pattern exists. */

	m_explosion = BIT(data, 4);
	if (!m_explosion) m_discrete->write(SCHASER_DOT_EN, BIT(data, 1));      /* prevents beep during explosion */

	if (!m_sound_en)               /* sound disabled */
		m_sound_seq=0;
	else
	if ((m_sound_seq == 0) && (m_sound_en))        /* normal play */
		m_sound_seq=1;
	else
	if ((m_sound_seq == 1) && (byte == 0x28))           /* going faster... */
		m_sound_seq=2;
	else
	if ((m_sound_seq == 2) && (byte == 0x24))           /* and faster... */
		m_sound_seq=3;
	else
	if ((m_sound_seq < 4) && (byte == 0x2c))            /* died */
		m_sound_seq=4;
	else
	if ((m_sound_seq > 2) && (byte == 0))               /* finished stage, and the pause before a stage starts */
		m_sound_seq=5;

	if (m_sound_seq == 1) effect=4;                 /* normal speed */
	else
	if (m_sound_seq == 2) effect=2;                 /* going faster... */
	else
	if (m_sound_seq == 3) effect=1;                 /* and faster... */
	else
		effect = 0;
				/* effect = 0 when no missile on screen, or in attract mode */

	apply_effect(effect, false);

	m_green_select = (m_main_ram[0x6f] == 0xff);
}

void sc_state::port05_w(u8 data)
{
	/* bit 0 - Music (DAC)
	   bit 2 - Coin Lockout
	   bit 4 - Sound Enable
	   bit 5 - Flip Screen
	There are no field control bits. Therefore the green mask can never appear.
	    The sound is enabled early on the stages that should have the mask. */

	m_discrete->write(SCHASER_MUSIC_BIT, BIT(data, 0));

	m_sound_en = BIT(data, 4);
	m_discrete->write(SCHASER_SND_EN, m_sound_en);

	machine().bookkeeping().coin_lockout_global_w(BIT(data, 2));

	m_flip_screen = BIT(data, 5) & ioport("CAB")->read();
}

void sc_state::machine_start()
{
	save_item(NAME(m_flip_screen));
	save_item(NAME(m_sound_en));
	save_item(NAME(m_explosion));
	save_item(NAME(m_last_effect));
	save_item(NAME(m_sound_seq));
	save_item(NAME(m_555_is_low));
	create_interrupt_timer();
}


void sc_state::machine_reset()
{
	m_flip_screen = 0;
	m_sound_en = 0;
	m_explosion = 0;
	m_last_effect = 0xff;
	m_sound_seq = 0;
	m_555_is_low = 0;
	m_555_time_remain = attotime::zero;
	m_555_timer->adjust(attotime::never);

	port03_w(0);
	port05_w(0);
	start_interrupt_timer();
}

u8 sc_state::port02_r()
{
	u8 data = ioport("IN2")->read();
	if (m_flip_screen) return data;
	u8 in1 = ioport("IN1")->read();
	return (data & 0x89) | (in1 & 0x70) | (BIT(in1, 3) << 1) | (BIT(in1, 7) << 2);
}

void sc_state::io_map(address_map &map) {
	map(0x01, 0x01).portr("IN1");
	map(0x02, 0x02).r(FUNC(sc_state::port02_r)).w("mb14241",FUNC(mb14241_device::shift_count_w));
	map(0x03, 0x03).r("mb14241",FUNC(mb14241_device::shift_result_r)).w(FUNC(sc_state::port03_w));
	map(0x04, 0x04).w("mb14241",FUNC(mb14241_device::shift_data_w));
	map(0x05, 0x05).w(FUNC(sc_state::port05_w));
	map(0x06, 0x06).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
}


static INPUT_PORTS_START( schasercv )
	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY

	PORT_START("IN2")
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Lives ) )                                    PORT_DIPLOCATION("SW1:1")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN )  PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP )    PORT_4WAY PORT_PLAYER(2)
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Difficulty ) )                               PORT_DIPLOCATION("SW1:2")
	PORT_DIPSETTING(    0x00, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Hard ) )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )                  PORT_PLAYER(2)   PORT_DIPLOCATION("SW1:5")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT )  PORT_4WAY PORT_PLAYER(2)   PORT_DIPLOCATION("SW1:6")
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_PLAYER(2)   PORT_DIPLOCATION("SW1:7")
	PORT_DIPUNKNOWN_DIPLOC( 0x80, 0x00, "SW1:3" ) // doesn't do anything

	PORT_START("VR1")
	PORT_ADJUSTER( 70, "VR1 - Music Volume" )

	PORT_START("VR2")
	PORT_ADJUSTER( 90, "VR2 - Explosion/Effect Volume" )

	PORT_START("VR3")
	PORT_ADJUSTER( 70, "VR3 - Dot Volume" )

	PORT_START("CAB")
	PORT_CONFNAME( 0x01, 0x00, DEF_STR( Cabinet ) )
	PORT_CONFSETTING(    0x00, DEF_STR( Upright ) )
	PORT_CONFSETTING(    0x01, DEF_STR( Cocktail ) )
INPUT_PORTS_END

void sc_state::schasercv(machine_config &config)
{
	schaser(config);

	/* basic machine hardware */
	m_maincpu->set_addrmap(AS_IO, &sc_state::io_map);
}

ROM_START( schasercv )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "1",            0x0000, 0x0400, CRC(bec2b16b) SHA1(c62210ecb64d7c38e5b63481d7fe04eb59bb1068) )
	ROM_LOAD( "2",            0x0400, 0x0400, CRC(9d25e608) SHA1(4cc52a93a3ab96a0ec1d07593e17832fa59b30a1) )
	ROM_LOAD( "3",            0x0800, 0x0400, CRC(113d0635) SHA1(ab5e98d0b5fc37d7d69bb5c541681a0f66460440) )
	ROM_LOAD( "4",            0x0c00, 0x0400, CRC(f3a43c8d) SHA1(29a7a8b7d1de763a255cfec79157fd95e7bff551) )
	ROM_LOAD( "5",            0x1000, 0x0400, CRC(47c84f23) SHA1(61b475fa92b8335f8edd3a128d8ac8561658e464) )
	ROM_LOAD( "6",            0x1400, 0x0400, CRC(02ff2199) SHA1(e12c235b2064cb4bb426145172e523256e3c6358) )
	ROM_LOAD( "7",            0x1800, 0x0400, CRC(87d06b88) SHA1(2d743161f85e47cb8ee2a600cbee790b1ad7ad99) )
	ROM_LOAD( "8",            0x1c00, 0x0400, CRC(6dfaad08) SHA1(2184c4e2f4b6bffdc4fe13e178134331fcd43253) )
	ROM_LOAD( "9",            0x4000, 0x0400, CRC(3d1a2ae3) SHA1(672ad6590aebdfebc2748455fa638107f3934c41) )
	ROM_LOAD( "10",           0x4400, 0x0400, CRC(037edb99) SHA1(f2fc5e61f962666e7f6bb81753ac24ea0b97e581) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "rt06.ic2",     0x0000, 0x0400, CRC(950cf973) SHA1(d22df09b325835a0057ccd0d54f827b374254ac6) )
ROM_END

} // anonymous namespace

GAME( 1979, schaser,     0,        schaser,   schaser,   schaser_state,  empty_init,    ROT270, "Taito", "Space Chaser (set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, schasera,    schaser,  schaser,   schaser,   schaser_state,  empty_init,    ROT270, "Taito", "Space Chaser (set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, schaserb,    schaser,  schaser,   schaser,   schaser_state,  empty_init,    ROT270, "Taito", "Space Chaser (set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, schaserc,    schaser,  schaser,   schaser,   schaser_state,  empty_init,    ROT270, "Taito", "Space Chaser (set 4)", MACHINE_SUPPORTS_SAVE | MACHINE_IMPERFECT_SOUND | MACHINE_IMPERFECT_COLORS )
GAME( 1979, schaserm,    schaser,  schaser,   schaserm,  schaser_state,  empty_init,    ROT270, "Model Racing", "Space Chaser (Model Racing bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, crashrd,     schaser,  crashrd,   schaserm,  cr_state,       empty_init,    ROT270, "Centromatic", "Crash Road", MACHINE_SUPPORTS_SAVE | MACHINE_IMPERFECT_SOUND  | MACHINE_NO_COCKTAIL )
GAME( 1979, schasercv,   schaser,  schasercv, schasercv, sc_state,       empty_init,    ROT270, "Taito", "Space Chaser (CV version)(Extra Sounds)", MACHINE_SUPPORTS_SAVE )

