// license:BSD-3-Clause
// copyright-holders:Robbbert
/****************************************************************************************

Space Chaser (CV version)

August, 2008 by Robbbert

No diagram, therefore this is a Hack Job.


These cheats exist in this game:
1. Hold down 2P DOWN (the F key) while it says INSERT COIN. Then insert a coin
   and play. You will have 2 extra ships.
2. In the Hard difficulty setting, you normally start at level 4. Hold down the
   1P START (the 1 key) while it says INSERT COIN. Then insert a coin and play.
   You will start at level 5.


Changes:
1. Used the existing Discrete System for all sounds
2. Removed IN0 as it is never referenced
3. Identified and documented input cheats
4. Added watchdog
5. Added coin lockout

*******************************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/mb14241.h"
#include "machine/timer.h"
#include "sound/sn76477.h"
#include "sound/discrete.h"
#include "screen.h"
#include "speaker.h"

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

class sc_state : public driver_device
{
public:
	sc_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_p_ram(*this, "ram")
		, m_p_colorram(*this, "colorram")
		, m_sn(*this, "snsnd")
		, m_discrete(*this, "discrete")
		, m_555_timer(*this, "schaser_sh_555")
		, m_screen(*this, "screen")
	{ }

	void schasercv(machine_config &config);

private:

	u8 port02_r();
	void port03_w(u8 data);
	void port05_w(u8 data);
	void colour_w(offs_t offset, u8 data);
	DECLARE_MACHINE_START(sc);
	DECLARE_MACHINE_RESET(sc);
	TIMER_DEVICE_CALLBACK_MEMBER(schaser_effect_555_cb);
	TIMER_CALLBACK_MEMBER(mw8080bw_interrupt_callback);
	void mem_map(address_map &map);
	void io_map(address_map &map);
	u32 screen_update_schasercv(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	bool m_flip_screen;
	bool m_sound_enabled;
	bool m_explosion;
	bool m_555_is_low;
	attotime m_555_time_remain;
	int32_t m_555_time_remain_savable;
	u8 m_last_effect;
	u8 m_sound_seq;
	emu_timer   *m_interrupt_timer;
	u8 vpos_to_vysnc_chain_counter( int vpos );
	int vysnc_chain_counter_to_vpos( u8 counter, int vblank );
	void schaser_reinit_555_time_remain();
	void mw8080bw_create_interrupt_timer(  );
	void mw8080bw_start_interrupt_timer(  );
	required_device<cpu_device> m_maincpu;
	required_shared_ptr<u8> m_p_ram;
	required_shared_ptr<u8> m_p_colorram;
	required_device<sn76477_device> m_sn;
	required_device<discrete_sound_device> m_discrete;
	required_device<timer_device> m_555_timer;
	required_device<screen_device> m_screen;
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

void sc_state::port03_w(u8 data)
{
	/* bit 1 - Dot Sound Enable
	   bit 2 - Effect Sound A (a stream of pulses)
	   bit 3 - Effect Sound B (a stream of pulses)
	   bit 4 - Explosion (a stream of ff's with some fe's thrown in)
	   bit 5 - Goes high when first dot hit */

	u8 effect = 0, byte = data & 0x2c;

	/* If you use fuel, the dot sound turns into a continuous beep.

	The effect sequence is really bizarre, however a pattern exists. */

	m_explosion = BIT(data, 4);
	if (!m_explosion) m_discrete->write(SCHASER_DOT_EN, BIT(data, 1));      /* prevents beep during explosion */

	if (!m_sound_enabled)				/* sound disabled */
		m_sound_seq=0;
	else
	if ((m_sound_seq == 0) && (m_sound_enabled))		/* normal play */
		m_sound_seq=1;
	else
	if ((m_sound_seq == 1) && (byte == 0x28))			/* going faster... */
		m_sound_seq=2;
	else
	if ((m_sound_seq == 2) && (byte == 0x24))			/* and faster... */
		m_sound_seq=3;
	else
	if ((m_sound_seq < 4) && (byte == 0x2c))			/* died */
		m_sound_seq=4;
	else
	if ((m_sound_seq > 2) && (byte == 0))				/* finished stage, and the pause before a stage starts */
		m_sound_seq=5;

	if (m_sound_seq == 1) effect=4;					/* normal speed */
	else
	if (m_sound_seq == 2) effect=2;					/* going faster... */
	else
	if (m_sound_seq == 3) effect=1;					/* and faster... */
				/* effect = 0 when no missile on screen, or in attract mode */


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
				m_555_time_remain = attotime::zero;
				m_555_time_remain_savable = m_555_time_remain.as_double();
				m_555_timer->adjust(attotime::never);
			}
		}
		m_last_effect = effect;
	}

	if (m_explosion)
		m_sn->amplitude_res_w(1.0 / (1.0/RES_K(200) + 1.0/RES_K(68)));
	else
		m_sn->amplitude_res_w(RES_K(200));

	m_sn->enable_w(!(m_555_is_low | m_explosion));
	m_sn->one_shot_cap_voltage_w(!(m_555_is_low | m_explosion) ? 0 : sn76477_device::EXTERNAL_VOLTAGE_DISCONNECT);
	m_sn->mixer_b_w(m_explosion);
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

	m_sound_enabled = BIT(data, 4);
	m_discrete->write(SCHASER_SND_EN, m_sound_enabled);
	machine().sound().system_enable(m_sound_enabled);

	machine().bookkeeping().coin_lockout_global_w(BIT(data, 2));

	m_flip_screen = BIT(data, 5) & ioport("CAB")->read();
}


TIMER_DEVICE_CALLBACK_MEMBER(sc_state::schaser_effect_555_cb)
{
	u8 effect = param;
	attotime new_time;

	/* Toggle 555 output */
	m_555_is_low ^= 1;
	m_555_time_remain = attotime::zero;
	m_555_time_remain_savable = m_555_time_remain.as_double();

	if (m_555_is_low)
		new_time = PERIOD_OF_555_ASTABLE(0, RES_K(20), CAP_U(1)) / 2;
	else
	{
		if (effect)
			new_time = attotime(0, ATTOSECONDS_PER_SECOND * .8873 * schaser_effect_rc[effect]);
		else
			new_time = attotime::never;
	}
	m_555_timer->adjust(new_time, effect);
	m_sn->enable_w(!(m_555_is_low | m_explosion));
	m_sn->one_shot_cap_voltage_w(!(m_555_is_low | m_explosion) ? 0 : sn76477_device::EXTERNAL_VOLTAGE_DISCONNECT);
}


void sc_state::schaser_reinit_555_time_remain()
{
	m_555_time_remain = attotime::from_double(m_555_time_remain_savable);
}


MACHINE_START_MEMBER( sc_state, sc )
{
	save_item(NAME(m_flip_screen));
	save_item(NAME(m_sound_enabled));
	save_item(NAME(m_explosion));
	save_item(NAME(m_last_effect));
	save_item(NAME(m_sound_seq));
	save_item(NAME(m_555_is_low));
	save_item(NAME(m_555_time_remain_savable));
	machine().save().register_postload(save_prepost_delegate(FUNC(sc_state::schaser_reinit_555_time_remain), this));
	mw8080bw_create_interrupt_timer();
}


MACHINE_RESET_MEMBER( sc_state, sc )
{
	m_flip_screen = 0;
	m_sound_enabled = 0;
	m_explosion = 0;
	m_last_effect = 0xff;
	m_sound_seq = 0;
	m_555_is_low = 0;
	m_555_time_remain = attotime::zero;
	m_555_time_remain_savable = m_555_time_remain.as_double();
	m_555_timer->adjust(attotime::never);

	port03_w(0);
	port05_w(0);
	mw8080bw_start_interrupt_timer();
}

u8 sc_state::vpos_to_vysnc_chain_counter( int vpos )
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


int sc_state::vysnc_chain_counter_to_vpos( u8 counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}


TIMER_CALLBACK_MEMBER(sc_state::mw8080bw_interrupt_callback)
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


void sc_state::mw8080bw_create_interrupt_timer(  )
{
	m_interrupt_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(sc_state::mw8080bw_interrupt_callback),this));
}


void sc_state::mw8080bw_start_interrupt_timer(  )
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));
}



u32 sc_state::screen_update_schasercv(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	pen_t pens[8];
	offs_t offs;
	u8 i, x, y, data, fg, color;

	for (i = 0; i < 8; i++)
		pens[i] = rgb_t(pal1bit(i >> 0), pal1bit(i >> 2), pal1bit(i >> 1));

	for (offs = 0; offs < m_p_ram.bytes(); offs++)
	{
		y = offs >> 5;
		x = offs << 3;

		data = m_p_ram[offs];
		fg = m_p_colorram[offs & 0x1f9f] & 0x07;

		for (i = 0; i < 8; i++)
		{
			color = BIT(data, i) ? fg : 2;

			if (y >= MW8080BW_VCOUNTER_START_NO_VBLANK)
			{
				if (m_flip_screen)
					bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - x) = pens[color];
				else
					bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = pens[color];
			}

			x++;
		}
	}

	return 0;
}

u8 sc_state::port02_r()
{
	u8 data = ioport("IN2")->read();
	if (m_flip_screen) return data;
	u8 in1 = ioport("IN1")->read();
	return (data & 0x89) | (in1 & 0x70) | (BIT(in1, 3) << 1) | (BIT(in1, 7) << 2);
}

void sc_state::colour_w(offs_t offset, u8 data)
{
	m_p_colorram[offset & 0x1f9f] = data;
}

void sc_state::mem_map(address_map &map) {
	map(0x0000,0x1fff).rom();
	map(0x2000,0x3fff).ram().share("ram");
	map(0x4000,0x5fff).rom();
	map(0xc000,0xdfff).share("colorram").w(FUNC(sc_state::colour_w));
}

void sc_state::io_map(address_map &map) {
	map(0x01,0x01).portr("IN1");
	map(0x02,0x02).r(FUNC(sc_state::port02_r)).w("mb14241",FUNC(mb14241_device::shift_count_w));
	map(0x03,0x03).r("mb14241",FUNC(mb14241_device::shift_result_r)).w(FUNC(sc_state::port03_w));
	map(0x04,0x04).w("mb14241",FUNC(mb14241_device::shift_data_w));
	map(0x05,0x05).w(FUNC(sc_state::port05_w));
	map(0x06,0x06).nopw();  //(watchdog_reset_w)
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
	PORT_DIPUNKNOWN_DIPLOC( 0x80, 0x00, "SW1:3" ) /* doesn't do anything */

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
	/* basic machine hardware */
	I8080(config, m_maincpu, MW8080BW_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &sc_state::mem_map);
	m_maincpu->set_addrmap(AS_IO, &sc_state::io_map);

	MCFG_MACHINE_START_OVERRIDE(sc_state, sc)
	MCFG_MACHINE_RESET_OVERRIDE(sc_state, sc)

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(sc_state::screen_update_schasercv));

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
	snsnd.add_route(0, "discrete", 1.0, 0);

	DISCRETE(config, m_discrete, schaser_discrete);
	m_discrete->add_route(ALL_OUTPUTS, "mono", 0.50);

	TIMER(config, "schaser_sh_555").configure_generic(FUNC(sc_state::schaser_effect_555_cb));
}

ROM_START( schasrcv )
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
ROM_END

GAME( 1979, schasrcv, 0, schasercv, schasercv, sc_state, empty_init, ROT270, "Taito", "Space Chaser (CV version)(Extra Sounds)", MACHINE_SUPPORTS_SAVE | MACHINE_IMPERFECT_SOUND | MACHINE_IMPERFECT_COLORS )
