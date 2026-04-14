// license:GPL-2.0+
// copyright-holders:Robbbert
/***************************************************************************************
The test roms:
- Test VID-R only works if the cabinet is in Cocktail mode.
- Test LAU-H is when your ship blows up. On a colour board, the screen will flash red.
- The SHIFTERS test (when present) only happens on the 2nd round of audio tests.

The multigame roms:
- The colours and sounds are bad. This might occur on real hardware.
***************************************************************************************/
#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/eepromser.h"
#include "machine/mb14241.h"
#include "machine/timer.h"
#include "machine/watchdog.h"
#include "sound/samples.h"
#include "sound/sn76477.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"
#include "invaders.lh"

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
#define MW8080BW_60HZ                     (MW8080BW_PIXEL_CLOCK / MW8080BW_HTOTAL / MW8080BW_VTOTAL)

// +4 is added to HBSTART because the hardware displays that many pixels after setting HBLANK
#define MW8080BW_HPIXCOUNT                (MW8080BW_HBSTART + 4)
#define INVADERS_P1_CONTROL_PORT_TAG    ("CONTP1")
#define INVADERS_P2_CONTROL_PORT_TAG    ("CONTP2")
#define INVADERS_SW6_SW7_PORT_TAG       ("SW6SW7")
#define INVADERS_SW5_PORT_TAG           ("SW5")

#define INVADERS_CONTROL_PORT_P1 \
	PORT_START(INVADERS_P1_CONTROL_PORT_TAG) \
	INVADERS_CONTROL_PORT_PLAYER(1)

#define INVADERS_CONTROL_PORT_P2 \
	PORT_START(INVADERS_P2_CONTROL_PORT_TAG) \
	INVADERS_CONTROL_PORT_PLAYER(2)

#define INVADERS_CONTROL_PORT_PLAYER(player) \
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(player) \
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_2WAY PORT_PLAYER(player) \
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_2WAY PORT_PLAYER(player)

#define INVADERS_CAB_TYPE_PORT \
	PORT_START("CAB") \
	PORT_CONFNAME( 0x01, 0x00, DEF_STR( Cabinet ) ) \
	PORT_CONFSETTING(    0x00, DEF_STR( Upright ) ) \
	PORT_CONFSETTING(    0x01, DEF_STR( Cocktail ) )


class invaders_state : public driver_device
{
public:
	invaders_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_prom(*this, "proms")
		, m_mb14241(*this,"mb14241")
		, m_watchdog(*this, "watchdog")
		, m_main_ram(*this, "main_ram")
		, m_screen(*this, "screen")
		, m_player_controls(*this, "CONTP%u", 1U)
		, m_cabinet_type(*this, "CAB")
		, m_sn(*this, "snsnd")
		, m_samples(*this, "samples")
		, m_palette(*this, "palette")
	{ }

	void mw8080bw_root(machine_config &config);
	void invaders(machine_config &config);
	void invaders_samples_audio(machine_config &config);
	void invadpt2(machine_config &config);

	DECLARE_INPUT_CHANGED_MEMBER(direct_coin_count);
	DECLARE_CUSTOM_INPUT_MEMBER(invaders_sw6_sw7_r);
	DECLARE_CUSTOM_INPUT_MEMBER(invaders_sw5_r);
	DECLARE_CUSTOM_INPUT_MEMBER(invaders_in0_control_r);
	DECLARE_CUSTOM_INPUT_MEMBER(invaders_in1_control_r);
	DECLARE_CUSTOM_INPUT_MEMBER(invaders_in2_control_r);
	DECLARE_CUSTOM_INPUT_MEMBER(invadpt2_in1_control_r);
	DECLARE_CUSTOM_INPUT_MEMBER(invadpt2_in2_control_r);
	IRQ_CALLBACK_MEMBER(interrupt_vector);

protected:
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override { m_color_map = m_screen_red = 0; }

	void port03_w(uint8_t data);
	void port05_w(uint8_t data);

	DECLARE_WRITE_LINE_MEMBER(int_enable_w);

	uint32_t screen_update_mw8080bw(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_invaders(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_invadpt2(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);

	/* misc game specific */
	bool m_color_map = 0;
	bool m_screen_red = 0;
	bool m_flip_screen = 0;
	bool is_cabinet_cocktail();
	void set_pixel( bitmap_rgb32 &bitmap, uint8_t y, uint8_t x, int color );
	void set_8_pixels(bitmap_rgb32 &bitmap, uint8_t y, uint8_t x, uint8_t data, int fore_color, int back_color);
	void clear_extra_columns( bitmap_rgb32 &bitmap, int color );

	// device/memory pointers
	required_device<cpu_device> m_maincpu;
	optional_region_ptr<u8> m_prom;
	optional_device<mb14241_device> m_mb14241;
	optional_device<watchdog_timer_device> m_watchdog;
	required_shared_ptr<uint8_t> m_main_ram;
	required_device<screen_device> m_screen;
	optional_ioport_array<2> m_player_controls;
	optional_ioport m_cabinet_type;
	optional_device<sn76477_device> m_sn;
	optional_device<samples_device> m_samples;
	optional_device<palette_device> m_palette;

private:
	// misc game specific
	void io_map(address_map &map);
	void invadpt2_io_map(address_map &map);

	attotime m_interrupt_time;
	emu_timer *m_interrupt_timer = nullptr;

	bool m_int_enable = 0;
	bool m_sound_en = 0;

	TIMER_CALLBACK_MEMBER(interrupt_trigger);
	uint8_t vpos_to_vysnc_chain_counter(int vpos);
	int vysnc_chain_counter_to_vpos(uint8_t counter, int vblank);
	void main_map(address_map &map);
	std::unique_ptr<uint8_t[]> m_scattered_colorram;

	/* sound-related */
	uint8_t m_port03 = 0;
	uint8_t m_port05 = 0;
};


INPUT_CHANGED_MEMBER(invaders_state::direct_coin_count)
{
	machine().bookkeeping().coin_counter_w(0, newval);
}

CUSTOM_INPUT_MEMBER(invaders_state::invaders_sw6_sw7_r)
{
	// upright PCB : switches visible
	// cocktail PCB: HI

	if (is_cabinet_cocktail())
		return 0x03;
	else
		return ioport(INVADERS_SW6_SW7_PORT_TAG)->read();
}


CUSTOM_INPUT_MEMBER(invaders_state::invaders_sw5_r)
{
	// upright PCB : switch visible
	// cocktail PCB: HI

	if (is_cabinet_cocktail())
		return 0x01;
	else
		return ioport(INVADERS_SW5_PORT_TAG)->read();
}


CUSTOM_INPUT_MEMBER(invaders_state::invaders_in0_control_r)
{
	// upright PCB : P1 controls
	// cocktail PCB: HI

	if (is_cabinet_cocktail())
		return 0x07;
	else
		return m_player_controls[0]->read();
}


CUSTOM_INPUT_MEMBER(invaders_state::invaders_in1_control_r)
{
	return m_player_controls[0]->read();
}


CUSTOM_INPUT_MEMBER(invaders_state::invaders_in2_control_r)
{
	// upright PCB : P1 controls
	// cocktail PCB: P2 controls

	return m_player_controls[is_cabinet_cocktail() ? 1 : 0]->read();
}


bool invaders_state::is_cabinet_cocktail()
{
	return BIT(m_cabinet_type->read(), 0);
}


/*************************************
 *
 *  Interrupt generation
 *
 *************************************/

uint8_t invaders_state::vpos_to_vysnc_chain_counter( int vpos )
{
	/* convert from a vertical position to the actual values on the vertical sync counters */
	uint8_t counter;
	int vblank = (vpos >= MW8080BW_VBSTART);

	if (vblank)
		counter = vpos - MW8080BW_VBSTART + MW8080BW_VCOUNTER_START_VBLANK;
	else
		counter = vpos + MW8080BW_VCOUNTER_START_NO_VBLANK;

	return counter;
}

int invaders_state::vysnc_chain_counter_to_vpos( uint8_t counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}

TIMER_CALLBACK_MEMBER(invaders_state::interrupt_trigger)
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

WRITE_LINE_MEMBER(invaders_state::int_enable_w)
{
	m_int_enable = state;
}

IRQ_CALLBACK_MEMBER(invaders_state::interrupt_vector)
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

/*************************************
 *
 *  Machine setup
 *
 *************************************/

void invaders_state::machine_start()
{
	m_interrupt_timer = timer_alloc(FUNC(invaders_state::interrupt_trigger), this);
	save_item(NAME(m_color_map));
	save_item(NAME(m_screen_red));
	save_item(NAME(m_port03));
	save_item(NAME(m_port05));
}


void invaders_state::machine_reset()
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));

	m_interrupt_time = attotime::zero;
}


/*************************************
 *
 *  Special shifter circuit
 *
 *************************************/

inline void invaders_state::set_pixel(bitmap_rgb32 &bitmap, uint8_t y, uint8_t x, int color)
{
	if (y >= MW8080BW_VCOUNTER_START_NO_VBLANK)
	{
		if (m_flip_screen)
			bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - x) = m_palette->pen_color(color);
		else
			bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = m_palette->pen_color(color);
	}
}


inline void invaders_state::set_8_pixels(bitmap_rgb32 &bitmap, uint8_t y, uint8_t x, uint8_t data, int fore_color, int back_color)
{
	for (int i = 0; i < 8; i++)
	{
		set_pixel(bitmap, y, x, BIT(data, 0) ? fore_color : back_color);

		x += 1;
		data >>= 1;
	}
}

void invaders_state::clear_extra_columns(bitmap_rgb32 &bitmap, int color)
{
	for (uint8_t x = 0; x < 4; x++)
	{
		for (uint8_t y = MW8080BW_VCOUNTER_START_NO_VBLANK; y != 0; y++)
		{
			if (m_flip_screen)
				bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - (256 + x)) =  m_palette->pen_color(color);
			else
				bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, 256 + x) = m_palette->pen_color(color);
		}
	}
}

uint32_t invaders_state::screen_update_mw8080bw(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	uint8_t x = 0;
	uint8_t y = MW8080BW_VCOUNTER_START_NO_VBLANK;
	uint8_t video_data = 0;

	while (1)
	{
		// plot the current pixel
		pen_t pen = (video_data & 0x01) ? rgb_t::white() : rgb_t::black();
		bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = pen;

		// next pixel
		video_data = video_data >> 1;
		x++;

		/* end of line? */
		if (x == 0)
		{
			// yes, flush out the shift register
			for (int i = 0; i < 4; i++)
			{
				pen = (video_data & 0x01) ? rgb_t::white() : rgb_t::black();
				bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, 256 + i) = pen;

				video_data = video_data >> 1;
			}

			// next row, video_data is now 0, so the next line will start with 4 blank pixels
			y++;

			// end of screen?
			if (y == 0)
				break;
		}
		/* the video RAM is read at every 8 pixels starting with pixel 4 */
		else if ((x & 0x07) == 0x04)
		{
			offs_t const offs = ((offs_t)y << 5) | (x >> 3);
			video_data = m_main_ram[offs];
		}
	}

	return 0;
}

uint32_t invaders_state::screen_update_invaders(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	uint8_t x = 0;
	uint8_t y = MW8080BW_VCOUNTER_START_NO_VBLANK;
	uint8_t video_data = 0;

	while (1)
	{
		// plot the current pixel
		pen_t pen = (video_data & 0x01) ? rgb_t::white() : rgb_t::black();

		if (m_flip_screen)
			bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - x) = pen;
		else
			bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = pen;

		// next pixel
		video_data = video_data >> 1;
		x++;

		// end of line?
		if (x == 0)
		{
			// yes, flush out the shift register
			for (int i = 0; i < 4; i++)
			{
				pen = (video_data & 0x01) ? rgb_t::white() : rgb_t::black();

				if (m_flip_screen)
					bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - (256 + i)) = pen;
				else
					bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, 256 + i) = pen;

				video_data = video_data >> 1;
			}

			// next row, video_data is now 0, so the next line will start with 4 blank pixels
			y++;

			// end of screen?
			if (y == 0)
				break;
		}
		else if ((x & 0x07) == 0x04) // the video RAM is read at every 8 pixels starting with pixel 4
		{
			offs_t const offs = (offs_t(y) << 5) | (x >> 3);
			video_data = m_main_ram[offs];
		}
	}

	return 0;
}

uint32_t invaders_state::screen_update_invadpt2(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	for (offs_t offs = 0; offs < m_main_ram.bytes(); offs++)
	{
		uint8_t const y = offs >> 5;
		uint8_t const x = offs << 3;

		offs_t const color_address = ((offs >> 8 << 5) | (offs & 0x1f)) | (m_color_map ? 0x400 : 0);

		uint8_t const data = m_main_ram[offs];
		uint8_t const fore_color = m_prom ? m_prom[color_address]: 7; // no prom, use white

		set_8_pixels(bitmap, y, x, data, fore_color & 7, m_screen_red);
	}

	clear_extra_columns(bitmap, 0);

	return 0;
}

static const char *const invaders_sample_names[] =
{
	"*invaders",
	"1",        /* shot/missile */
	"2",        /* base hit/explosion */
	"3",        /* invader hit */
	"4",        /* fleet move 1 */
	"5",        /* fleet move 2 */
	"6",        /* fleet move 3 */
	"7",        /* fleet move 4 */
	"8",        /* UFO/saucer hit */
	"9",        /* bonus base */
	nullptr
};


/* left in for all games that hack into invaders samples for audio */
void invaders_state::invaders_samples_audio(machine_config &config)
{
	SPEAKER(config, "mono").front_center();

	SN76477(config, m_sn);
	m_sn->set_noise_params(0, 0, 0);
	m_sn->set_decay_res(0);
	m_sn->set_attack_params(0, RES_K(100));
	m_sn->set_amp_res(RES_K(56));
	m_sn->set_feedback_res(RES_K(10));
	m_sn->set_vco_params(0, CAP_U(0.1), RES_K(8.2));
	m_sn->set_pitch_voltage(5.0);
	m_sn->set_slf_params(CAP_U(1.0), RES_K(120));
	m_sn->set_oneshot_params(0, 0);
	m_sn->set_vco_mode(1);
	m_sn->set_mixer_params(0, 0, 0);
	m_sn->set_envelope_params(1, 0);
	m_sn->set_enable(1);
	m_sn->add_route(ALL_OUTPUTS, "mono", 0.5);

	SAMPLES(config, m_samples);
	m_samples->set_channels(6);
	m_samples->set_samples_names(invaders_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 1.0);
}

static INPUT_PORTS_START( invaders )
	PORT_START("IN0")
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW:8")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On ) )
	PORT_BIT( 0x06, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_sw6_sw7_r)
	PORT_BIT( 0x08, IP_ACTIVE_LOW,  IPT_UNUSED )
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_in0_control_r)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_sw5_r)

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_COIN1 ) PORT_CHANGED_MEMBER(DEVICE_SELF, invaders_state, direct_coin_count, 0)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW,  IPT_UNUSED )
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_in1_control_r)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) ) PORT_DIPLOCATION("SW:3,4")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPSETTING(    0x02, "5" )
	PORT_DIPSETTING(    0x03, "6" )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_UNUSED ) // in the software, this is TILI, but not connected on the Midway PCB. Is this correct?
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW:2")
	PORT_DIPSETTING(    0x08, "1000" )
	PORT_DIPSETTING(    0x00, "1500" )
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_in2_control_r)
	PORT_DIPNAME( 0x80, 0x00, "Display Coinage" ) PORT_DIPLOCATION("SW:1")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	/* fake port for cabinet type */
	INVADERS_CAB_TYPE_PORT

	/* fake ports for handling the various input ports based on cabinet type */
	PORT_START(INVADERS_SW6_SW7_PORT_TAG)
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Unused ) ) PORT_DIPLOCATION("SW:7")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, DEF_STR( Unused ) ) PORT_DIPLOCATION("SW:6")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x02, DEF_STR( On ) )
	PORT_BIT( 0xfc, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START(INVADERS_SW5_PORT_TAG)
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Unused ) ) PORT_DIPLOCATION("SW:5")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On ) )
	PORT_BIT( 0xfe, IP_ACTIVE_HIGH, IPT_UNUSED )

	INVADERS_CONTROL_PORT_P1

	INVADERS_CONTROL_PORT_P2
INPUT_PORTS_END


void invaders_state::main_map(address_map &map)
{
	map.global_mask(0x7fff);
	map(0x0000, 0x1fff).rom().nopw();
	map(0x2000, 0x3fff).mirror(0x4000).ram().share("main_ram");
	map(0x4000, 0x5fff).rom().nopw();
}

void invaders_state::io_map(address_map &map)
{
	map.global_mask(0x7);
	map(0x00, 0x00).mirror(0x04).portr("IN0");
	map(0x01, 0x01).mirror(0x04).portr("IN1");
	map(0x02, 0x02).mirror(0x04).portr("IN2");
	map(0x03, 0x03).mirror(0x04).r(m_mb14241, FUNC(mb14241_device::shift_result_r));

	map(0x02, 0x02).w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x03, 0x03).w(FUNC(invaders_state::port03_w));
	map(0x04, 0x04).w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x05, 0x05).w(FUNC(invaders_state::port05_w));
	map(0x06, 0x06).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
}

void invaders_state::mw8080bw_root(machine_config &config)
{
	/* basic machine hardware */
	i8080_cpu_device &maincpu(I8080(config, m_maincpu, MW8080BW_CPU_CLOCK));
	maincpu.set_addrmap(AS_PROGRAM, &invaders_state::main_map);
	maincpu.set_irq_acknowledge_callback(FUNC(invaders_state::interrupt_vector));
	maincpu.out_inte_func().set(FUNC(invaders_state::int_enable_w));

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(invaders_state::screen_update_mw8080bw));
}

void invaders_state::invaders(machine_config &config)
{
	mw8080bw_root(config);

	// basic machine hardware
	m_maincpu->set_addrmap(AS_IO, &invaders_state::io_map);

	WATCHDOG_TIMER(config, m_watchdog).set_time(255 * attotime::from_hz(MW8080BW_60HZ));

	// video hardware
	m_screen->set_screen_update(FUNC(invaders_state::screen_update_invaders));

	// add shifter
	MB14241(config, m_mb14241);

	// audio hardware
	invaders_samples_audio(config);
}



/*******************************************************/
/*                                                     */
/* Midway "Space Invaders Part II"                     */
/*                                                     */
/*******************************************************/

void invaders_state::port03_w(uint8_t data)
{
	m_sound_en = BIT(data, 5);
	uint8_t c = m_sound_en ? data & ~m_port03 : 0;

	m_sn->enable_w(!BIT(data, 0));            // SAUCER SOUND

	if (BIT(c, 1)) m_samples->start(0, 0);    // Shoot SOUND
	if (BIT(c, 2)) m_samples->start(1, 1);    // EXPLOSION
	if (BIT(c, 3)) m_samples->start(2, 2);    // INVADER HIT
	if (BIT(c, 4)) m_samples->start(5, 8);    // BONUS BASE

	m_screen_red = BIT(data, 2);

	m_port03 = data;
}

void invaders_state::port05_w(uint8_t data)
{
	/* FLEET (movement)

	   DO = 20K + 20K
	   D1 = 68K
	   D2 = 82K
	   D3 = 100K */

	uint8_t c = m_sound_en ? data & ~m_port05 : 0;

	if (BIT(c, 0)) m_samples->start(4, 3);    // FLEET
	if (BIT(c, 1)) m_samples->start(4, 4);    // FLEET
	if (BIT(c, 2)) m_samples->start(4, 5);    // FLEET
	if (BIT(c, 3)) m_samples->start(4, 6);    // FLEET
	if (BIT(c, 4)) m_samples->start(3, 7);    // SAUCER HIT

	m_flip_screen = (BIT(data, 5) & ioport("CAB")->read()) ? 1 : 0;
	m_color_map = BIT(data, 5);

	m_port05 = data;
}


/*******************************************************/
/*                                                     */
/* Space Invaders CV Version (Taito)                   */
/*                                                     */
/*******************************************************/

static INPUT_PORTS_START( sicv_base )
	// common port definitions used by SICV and clones, based on sicv unless otherwise noted

	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_UNKNOWN )   // sicv has a DIP switch connected here
	PORT_BIT( 0x02, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic
	PORT_BIT( 0x04, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic
	PORT_BIT( 0x08, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic (shard with IN1 bit 3)
	PORT_BIT( 0x10, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic
	PORT_BIT( 0x20, IP_ACTIVE_LOW,  IPT_UNUSED )    // not connected (floating) on schematic)
	PORT_BIT( 0x40, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic
	PORT_BIT( 0x80, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW,  IPT_UNUSED )    // tied high via 1k resistor on schematic (shared with IN0 bit 3)
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_in1_control_r)
	PORT_BIT( 0x80, IP_ACTIVE_LOW,  IPT_UNUSED )    // not connected (floating) on schematic

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:1,2")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPSETTING(    0x02, "5" )
	PORT_DIPSETTING(    0x03, "6" )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Bonus_Life ) )   PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(    0x08, "1000" )
	PORT_DIPSETTING(    0x00, "1500" )
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invaders_in2_control_r) // P2 controls gated by DIP switches on sicv
	PORT_DIPNAME( 0x80, 0x00, "Coin Info" )             PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// P1 controls (read via IN1, and also via IN2 on upright cabinets)
	INVADERS_CONTROL_PORT_P1

	// P2 controls on cocktail cabinet (read via IN2)
	INVADERS_CONTROL_PORT_P2

	// Dummy port for cocktail mode
	INVADERS_CAB_TYPE_PORT
INPUT_PORTS_END

/*******************************************************/
/*                                                     */
/* Midway "Space Invaders Part II"                     */
/*                                                     */
/*******************************************************/

CUSTOM_INPUT_MEMBER(invaders_state::invadpt2_in1_control_r)
{
	return m_player_controls[0]->read() | (m_player_controls[1]->read() & ~m_cabinet_type->read());
}

CUSTOM_INPUT_MEMBER(invaders_state::invadpt2_in2_control_r)
{
	return m_player_controls[1]->read() | (m_player_controls[0]->read() & ~m_cabinet_type->read());
}

void invaders_state::invadpt2_io_map(address_map &map)
{
	map(0x00, 0x00).portr("IN0");
	map(0x01, 0x01).portr("IN1");
	map(0x02, 0x02).portr("IN2").w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x03, 0x03).r(m_mb14241, FUNC(mb14241_device::shift_result_r)).w(FUNC(invaders_state::port03_w));
	map(0x04, 0x04).w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x05, 0x05).w(FUNC(invaders_state::port05_w));
	map(0x06, 0x06).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
}


static INPUT_PORTS_START( invadpt2 )
	PORT_START("IN0")
	PORT_DIPUNUSED_DIPLOC( 0x01, 0x00, "SW1:8" ) // manual suggests this is for an unavailable test mode
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED ) // tied low on schematic
	PORT_BIT( 0x04, IP_ACTIVE_LOW,  IPT_UNUSED ) // tied high via 1k resistor on schematic (shared with IN0 bit 4, IN0 bit 5, IN0 bit 7, IN1 bit 7, IN2 bit 2)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNUSED ) // tied low on schematic
	PORT_BIT( 0x10, IP_ACTIVE_LOW,  IPT_UNUSED ) // tied high via 1k resistor on schematic (shared with IN0 bits 3/5/7, IN1 bit 7, IN2 bit 2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW,  IPT_UNUSED ) // tied high via 1k resistor on schematic (shared with IN0 bits 3/4/7, IN1 bit 7, IN2 bit 2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW,  IPT_MEMORY_RESET ) PORT_NAME("Name Reset") // if name of high scorer was rude, owner can press this button
	PORT_BIT( 0x80, IP_ACTIVE_LOW,  IPT_UNUSED ) // tied high via 1k resistor on schematic (shared with IN0 bits 3/4/5, IN1 bit 7, IN2 bit 2)

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW,  IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNUSED ) // tied low on schematic
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invadpt2_in1_control_r)
	PORT_BIT( 0x80, IP_ACTIVE_LOW,  IPT_UNUSED ) // tied high via 1k resistor on schematic (shared with IN0 bits 3/4/5/7, IN2 bit 2)

	PORT_START("IN2")
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Lives ) )    PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPUNUSED_DIPLOC( 0x02, 0x00, "SW1:3" ) // previously called "Image Rotation" - "When ON, the images on screen will be rotated. Default is ON."
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_UNUSED ) // tied high via 1k resistor on schematic (shared with IN0 bits 3/4/5/7, IN1 bit 7), but triggers tilt - must be pulled low somehow
	PORT_DIPNAME( 0x08, 0x00, "Preset Mode" )       PORT_DIPLOCATION("SW1:2") // in preset mode, 1P start increases score by 1000 to pre-set high score/name
	PORT_DIPSETTING(    0x00, "Game Mode" )
	PORT_DIPSETTING(    0x08, "Name Entry" )
	PORT_BIT( 0x70, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(invaders_state, invadpt2_in2_control_r)
	PORT_DIPNAME( 0x80, 0x00, "Coin Info" )         PORT_DIPLOCATION("SW1:1")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// P1 controls (read via IN1, and also via IN2 on upright cabinets)
	INVADERS_CONTROL_PORT_P1

	// P2 controls (read via IN2, and also via IN1 on upright cabinets)
	INVADERS_CONTROL_PORT_P2

	PORT_START("CAB")
	PORT_DIPNAME( 0x07, 0x00, DEF_STR( Cabinet ) )  PORT_DIPLOCATION("SW1:5,6,7") // couples player inputs in upright mode
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x07, DEF_STR( Cocktail ) )
INPUT_PORTS_END


/* same as regular invaders, but with a color board added */
void invaders_state::invadpt2(machine_config &config)
{
	mw8080bw_root(config);

	/* basic machine hardware */
	m_maincpu->set_addrmap(AS_IO, &invaders_state::invadpt2_io_map);

	/* 60 Hz signal clocks two LS161. Ripple carry will */
	/* reset circuit, if LS161 not cleared before.      */
	WATCHDOG_TIMER(config, m_watchdog).set_vblank_count("screen", 255);

	/* add shifter */
	MB14241(config, m_mb14241);

	/* video hardware */
	m_screen->set_screen_update(FUNC(invaders_state::screen_update_invadpt2));

	PALETTE(config, m_palette, palette_device::RBG_3BIT);

	/* sound hardware */
	invaders_samples_audio(config);
}



/*******************************************************/
/*                                                     */
/* bootleg "Super Invaders"                            */
/*                                                     */
/*******************************************************/

static INPUT_PORTS_START( sinvrdzm )
	PORT_INCLUDE( sicv_base )

	PORT_MODIFY("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_COIN2 )

	PORT_MODIFY("IN2")
	PORT_DIPNAME( 0x80, 0x00, DEF_STR( Coinage ) )      PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x80, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
INPUT_PORTS_END


// Roms
ROM_START( invaders )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "invaders.h", 0x0000, 0x0800, CRC(734f5ad8) SHA1(ff6200af4c9110d8181249cbcef1a8a40fa40b7f) )
	ROM_LOAD( "invaders.g", 0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f", 0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e", 0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

// Dumped by Andy Welburn, 2026-03-12.
// Says "Space Invaders Part II" on screen - dump didn't come with colour proms, so I added them.
ROM_START( invdelux )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "invdelux.h",   0x0000, 0x0800, CRC(e690818f) SHA1(0860fb03a64d34a9704a1459a5e96929eafd39c7) )
	ROM_LOAD( "invdelux.g",   0x0800, 0x0800, CRC(4268c12d) SHA1(df02419f01cf0874afd1f1aa16276751acd0604a) )
	ROM_LOAD( "sidm.f",       0x1000, 0x0800, CRC(17943e02) SHA1(16b055965fe8c758f3d477b10093cae7a61a4a2a) )
	ROM_LOAD( "sidm.e",       0x1800, 0x0800, CRC(3560e97b) SHA1(918d0df2860df8be7749f45b6e10465e2666469b) )
	ROM_LOAD( "invdelux.d",   0x4000, 0x0800, CRC(e8d5afcd) SHA1(91fde9a9e7c3dd53aac4770bd169721a79b41ed1) )

	ROM_REGION( 0x0800, "proms", 0 )        /* color maps player 1/player 2 */
	ROM_LOAD( "pv06.1",   0x0000, 0x0400, CRC(a732810b) SHA1(a5fabffa73ca740909e23b9530936f9274dff356) )
	ROM_LOAD( "pv07.2",   0x0400, 0x0400, CRC(2c5b91cb) SHA1(7fa4d4aef85473b1b4f18734230c164e72be44e7) )
ROM_END

ROM_START( tst_invd )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "test.h",       0x0000, 0x0800, CRC(f86a2eea) SHA1(4a72ff01f3e6d16bbe9bf7f123cd98895bfbed9a) ) // Test ROM
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvtest )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvtest.h",   0x0000, 0x0800, CRC(0a428359) SHA1(cfb961d4888c781ea52a7579ce73ab22cab58ab6) ) // Test rom
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvtest0 )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvtest0.h",  0x0000, 0x0800, CRC(f78d2682) SHA1(c3dd0e91ec40ca22e90370a77f3f5e08345ff92b) ) // Test rom
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvtest1 ) // This hangs in the 2nd ram test due to an internal bug. Fixed in sinvtest3.
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvtest1.h",  0x0000, 0x0800, CRC(9c2f913e) SHA1(2cc2ed2267a121034ef02d734f025aad91c5c8d1) ) // Test rom
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvtest2 ) // This hangs in the 2nd ram test due to an internal bug. Fixed in sinvtest3.
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvtest2.h",  0x0000, 0x0800, CRC(7c5e93f5) SHA1(b927a073f4a1ee54df62bdc7173b038232edfea4) ) // Test rom
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvtest3 )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvtest3.h",  0x0000, 0x0800, CRC(08d43dfe) SHA1(debe4e137e3cbc46c720ddd956960a313c43470e) ) // Test rom
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvtest4 ) // for flip, turn on Cocktail mode in dips. Then button is a toggle and press 2 times per direction.
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvtest4.h",  0x0000, 0x0800, CRC(8218a7c0) SHA1(91ebc7958c952852d929b97cc98e6b6b301dd8ca) ) // Test rom
	ROM_LOAD( "invaders.g",   0x0800, 0x0800, CRC(6bfaca4a) SHA1(16f48649b531bdef8c2d1446c429b5f414524350) )
	ROM_LOAD( "invaders.f",   0x1000, 0x0800, CRC(0ccead96) SHA1(537aef03468f63c5b9e11dd61e253f7ae17d9743) )
	ROM_LOAD( "invaders.e",   0x1800, 0x0800, CRC(14e538b0) SHA1(1d6ca0c99f9df71e2990b610deb9d7da0125e2d8) )
ROM_END

ROM_START( sinvrdzm )
	ROM_REGION( 0x6000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "sinvrdzm.1",  0x0000, 0x0400, CRC(f625f153) SHA1(2a19f7b4f5687e89eebf02cfdf3d1d23624879fb) )
	ROM_LOAD( "sinvrdzm.2",  0x0400, 0x0400, CRC(9d628753) SHA1(2b4468f64246da9263a384f2a940829e5a8cebde) )
	ROM_LOAD( "sinvrdzm.3",  0x0800, 0x0400, CRC(db342868) SHA1(f9ab63b3e89be9f22e453b59bf00732b7e265ee3) )
	ROM_LOAD( "sinvrdzm.4",  0x0c00, 0x0400, CRC(b635e8ac) SHA1(88a58e6ec550ec8f24443716b38f3d0aab57fc85) )
	ROM_LOAD( "sinvrdzm.5",  0x1000, 0x0400, CRC(014e761f) SHA1(1669c8f86e9d5522b009a517a75792b65902cee2) )
	ROM_LOAD( "sinvrdzm.6",  0x1400, 0x0400, CRC(599c6b78) SHA1(79e2d3a2d4e67bc13fea0d13301fffd92d5c11da) )
	ROM_LOAD( "sinvrdzm.7",  0x1800, 0x0400, CRC(0e8a75df) SHA1(e03c4a4d5aa6b7d97cc12f3bd3df84a15df4ad35) )
	ROM_LOAD( "sinvrdzm.8",  0x1c00, 0x0400, CRC(11392151) SHA1(ddeaf6678c898416e734d924f872496095ff294d) )
ROM_END

} // anonymous namespace

GAMEL(1978, invaders,    0,        invaders,  invaders,  invaders_state, empty_init,   ROT270, "Taito / Midway",        "Space Invaders", MACHINE_SUPPORTS_SAVE, layout_invaders )
GAME( 1979, invdelux,    0,        invadpt2,  invadpt2,  invaders_state, empty_init,   ROT270, "Midway",                "Space Invaders Part II (alt)", MACHINE_SUPPORTS_SAVE )
GAME( 1978, tst_invd,    invaders, invadpt2,  invadpt2,  invaders_state, empty_init,   ROT0,   "Taito",                 "Space Invaders Test ROM", MACHINE_SUPPORTS_SAVE )
GAME( 2022, sinvtest,    invaders, invadpt2,  invadpt2,  invaders_state, empty_init,   ROT0,   "Phil Murray",           "Space Invaders Test ROM (SMv1.0, 2022-01-14)", MACHINE_SUPPORTS_SAVE )
GAMEL(2013, sinvtest0,   invaders, invaders,  invaders,  invaders_state, empty_init,   ROT270, "Frederic Rodo",         "Space Invaders Test ROM (v1.0, 2013-10-12)", MACHINE_SUPPORTS_SAVE, layout_invaders )
GAMEL(2017, sinvtest1,   invaders, invaders,  invaders,  invaders_state, empty_init,   ROT270, "Fabrice Girardot",      "Space Invaders Test ROM (v1.1, 2017-09-17)", MACHINE_SUPPORTS_SAVE, layout_invaders )
GAMEL(2017, sinvtest2,   invaders, invaders,  invaders,  invaders_state, empty_init,   ROT270, "Frederic Rodo",         "Space Invaders Test ROM (v1.2, 2017-10-14)", MACHINE_SUPPORTS_SAVE, layout_invaders )
GAMEL(2019, sinvtest3,   invaders, invaders,  invaders,  invaders_state, empty_init,   ROT270, "Marc Deslauriers",      "Space Invaders Test ROM (v1.3, 2019-03-27)", MACHINE_SUPPORTS_SAVE, layout_invaders )
GAME (2025, sinvtest4,   invaders, invadpt2,  invadpt2,  invaders_state, empty_init,   ROT0,   "Phil Murray",           "Space Invaders Test ROM (SMv1.1, 2025-07-11)", MACHINE_SUPPORTS_SAVE )
GAMEL(1978, sinvrdzm,    invaders, invaders,  sinvrdzm,  invaders_state, empty_init,   ROT270, "Zenitone-Microsec Ltd", "Super Invaders (Ruffler & Deith)", MACHINE_SUPPORTS_SAVE, layout_invaders )

