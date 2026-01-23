// license:GPL_2.0
// copyright-holders:Robbbert

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/mb14241.h"
#include "machine/timer.h"
#include "machine/watchdog.h"
#include "sound/samples.h"
#include "screen.h"
#include "speaker.h"
#include "gunchamp.lh"


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


class claybust_state : public driver_device
{
public:
	claybust_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_mb14241(*this,"mb14241")
		, m_watchdog(*this, "watchdog")
		, m_main_ram(*this, "main_ram")
		, m_samples(*this, "samples")
		, m_screen(*this, "screen")
		, m_gunx(*this, "GUNX")
		, m_guny(*this, "GUNY")
		, m_gun_on(*this, "claybust_gun")
	{ }

	void claybust(machine_config &config);
	DECLARE_INPUT_CHANGED_MEMBER(gun_trigger);
	DECLARE_READ_LINE_MEMBER(gun_on_r);

private:
	required_device<i8085a_cpu_device> m_maincpu;
	required_device<mb14241_device> m_mb14241;
	optional_device<watchdog_timer_device> m_watchdog;
	required_shared_ptr<uint8_t> m_main_ram;
	required_device<samples_device> m_samples;
	required_device<screen_device> m_screen;
	required_ioport m_gunx;
	required_ioport m_guny;
	required_device<timer_device> m_gun_on;

	virtual void machine_start() override;
	virtual void machine_reset() override;

	uint8_t gun_lo_r();
	uint8_t gun_hi_r();
	void sound_w(uint8_t data);
	TIMER_DEVICE_CALLBACK_MEMBER(gun_callback);
	TIMER_CALLBACK_MEMBER(interrupt_trigger);
	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	DECLARE_WRITE_LINE_MEMBER(int_enable_w);
	IRQ_CALLBACK_MEMBER(interrupt_vector);
	uint8_t vpos_to_vysnc_chain_counter(int vpos);
	int vysnc_chain_counter_to_vpos(uint8_t counter, int vblank);
	attotime m_interrupt_time;
	bool m_int_enable = true;
	emu_timer   *m_interrupt_timer = nullptr;

	void main_map(address_map &map);
	void io_map(address_map &map);

	uint16_t m_gun_pos = 0;
};


uint8_t claybust_state::vpos_to_vysnc_chain_counter( int vpos )
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

int claybust_state::vysnc_chain_counter_to_vpos( uint8_t counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}

TIMER_CALLBACK_MEMBER(claybust_state::interrupt_trigger)
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


WRITE_LINE_MEMBER(claybust_state::int_enable_w)
{
	m_int_enable = state;
}

IRQ_CALLBACK_MEMBER(claybust_state::interrupt_vector)
{
	int vpos = m_screen->vpos();
	if (machine().time() < m_interrupt_time)
		vpos++;
	uint8_t counter = vpos_to_vysnc_chain_counter(vpos);
	uint8_t vector = 0xc7 | ((counter & 0x40) >> 2) | ((~counter & 0x40) >> 3);

	m_maincpu->set_input_line(0, CLEAR_LINE);
	return vector;
}

uint32_t claybust_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
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
		x = x + 1;

		// end of line?
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
			y = y + 1;

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

void claybust_state::machine_start()
{
	m_interrupt_timer = timer_alloc(FUNC(claybust_state::interrupt_trigger), this);

	save_item(NAME(m_gun_pos));
}

void claybust_state::machine_reset()
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));

	m_interrupt_time = attotime::zero;
}

TIMER_DEVICE_CALLBACK_MEMBER(claybust_state::gun_callback)
{
	// reset gun latch
	m_gun_pos = 0;
}

READ_LINE_MEMBER(claybust_state::gun_on_r)
{
	return m_gun_pos ? 1 : 0;
}

INPUT_CHANGED_MEMBER(claybust_state::gun_trigger)
{
	if (newval)
	{
		uint8_t const gunx = m_gunx->read();
		uint8_t const guny = m_guny->read();
		m_gun_pos = ((gunx >> 3) | (guny << 5)) + 2;
		m_gun_on->adjust(attotime::from_msec(250)); // timing is a guess
	}
}

uint8_t claybust_state::gun_lo_r()
{
	return m_gun_pos;
}

uint8_t claybust_state::gun_hi_r()
{
	return BIT(m_gun_pos,8,8);
}

static const char *const claybust_sample_names[] =
{
	"*invaders",
	"1",        /* hit target */
	"2",        /* shoot gun */
	0
};

void claybust_state::sound_w(uint8_t data)
{
	if (data == 0xf1)
		m_samples->start(0,0);
	else
	if (data == 0xef)
		m_samples->start(1,1);
}

void claybust_state::main_map(address_map &map)
{
	map.global_mask(0x7fff);
	map(0x0000, 0x1fff).rom().nopw();
	map(0x2000, 0x3fff).ram().share("main_ram");
	map(0x4000, 0x4fff).noprw();
}

void claybust_state::io_map(address_map &map)
{
	map(0x00, 0x00).nopw(); // ?
	map(0x01, 0x01).portr("IN1").w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x02, 0x02).r(FUNC(claybust_state::gun_lo_r)).w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x03, 0x03).r(m_mb14241, FUNC(mb14241_device::shift_result_r)).w(FUNC(claybust_state::sound_w));
	map(0x04, 0x04).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
	map(0x05, 0x05).nopw(); // ?
	map(0x06, 0x06).r(FUNC(claybust_state::gun_hi_r));
}


static INPUT_PORTS_START( claybust )
	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(claybust_state, gun_on_r)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_IMPULSE(2) PORT_CHANGED_MEMBER(DEVICE_SELF, claybust_state, gun_trigger, 0)
	PORT_BIT( 0x04, IP_ACTIVE_LOW,  IPT_COIN1 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW,  IPT_START1 )

	// switch is 6-pos, but DNS06:5 and DNS06:6 are not connected
	PORT_DIPNAME( 0x10, 0x10, "Shots" )             PORT_DIPLOCATION("DNS06:1")
	PORT_DIPSETTING(    0x10, "4" )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPUNKNOWN_DIPLOC( 0x20, 0x20, "DNS06:2" )
	PORT_DIPNAME( 0x40, 0x00, "Number of Flings" )  PORT_DIPLOCATION("DNS06:3")
	PORT_DIPSETTING(    0x40, "8" )
	PORT_DIPSETTING(    0x00, "10" )
	PORT_DIPUNKNOWN_DIPLOC( 0x80, 0x80, "DNS06:4" )

	PORT_START( "GUNX" )
	PORT_BIT( 0xff, 0x80, IPT_LIGHTGUN_X ) PORT_MINMAX(0x00, 0xff) PORT_CROSSHAIR(X, 1.0 - (MW8080BW_HPIXCOUNT-256)/256.0, (MW8080BW_HPIXCOUNT-256)/256.0, 0) PORT_SENSITIVITY(56) PORT_KEYDELTA(5)
	PORT_START( "GUNY" )
	PORT_BIT( 0xff, 0xa0, IPT_LIGHTGUN_Y ) PORT_MINMAX(0x20, 0xff) PORT_CROSSHAIR(Y, 1.0, 0.0, 0) PORT_SENSITIVITY(64) PORT_KEYDELTA(5)
INPUT_PORTS_END

static INPUT_PORTS_START( gunchamp )
	PORT_INCLUDE( claybust )

	PORT_MODIFY("IN1")

	// switch is 6-pos, but DNS06:5 and DNS06:6 are not connected
	PORT_DIPNAME( 0x10, 0x10, "Enter Initials" )  PORT_DIPLOCATION("DNS06:1")
	PORT_DIPSETTING(    0x10, DEF_STR( Yes ) )
	PORT_DIPSETTING(    0x00, DEF_STR( No ) )
	PORT_DIPNAME( 0xe0, 0x40, DEF_STR( Coinage ) )  PORT_DIPLOCATION("DNS06:2,3,4")
	PORT_DIPSETTING(    0xa0, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0xc0, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0xe0, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x40, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x20, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(    0x60, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x80, DEF_STR( 1C_4C ) )
INPUT_PORTS_END

void claybust_state::claybust(machine_config &config)
{
	/* basic machine hardware */
	I8080(config, m_maincpu, MW8080BW_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &claybust_state::main_map);
	m_maincpu->set_addrmap(AS_IO, &claybust_state::io_map);
	m_maincpu->set_irq_acknowledge_callback(FUNC(claybust_state::interrupt_vector));
	m_maincpu->out_inte_func().set(FUNC(claybust_state::int_enable_w));

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(claybust_state::screen_update));

	/* sound hardware */
	SPEAKER(config, "mono").front_center();
	SAMPLES(config, m_samples);
	m_samples->set_channels(2);
	m_samples->set_samples_names(claybust_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 0.50);

	WATCHDOG_TIMER(config, m_watchdog).set_time(255 * attotime::from_hz(MW8080BW_60HZ));
	MB14241(config, m_mb14241);
	TIMER(config, "claybust_gun").configure_generic(FUNC(claybust_state::gun_callback));
}

ROM_START( claybust )
	ROM_REGION( 0x2000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "0.a1",         0x0000, 0x0400, CRC(90810582) SHA1(a5c3655bae6f92a3cd0eae3a5a3c25e414d4fdf0) )
	ROM_LOAD( "1.a2",         0x0400, 0x0400, CRC(5ce6fb0e) SHA1(19fa3fbc0dd7e0fa4fffc005ded5a814c3b48f2d) )
	ROM_LOAD( "2.a4",         0x0800, 0x0400, CRC(d4c1d523) SHA1(1a4785095caa8200d7e1d8d53a93c8e298f52c65) )
	ROM_LOAD( "3.a5",         0x0c00, 0x0400, CRC(1ca00825) SHA1(74633a4903a51f1eebdd09679597dbe86db2e001) )
	ROM_LOAD( "4.a6",         0x1000, 0x0400, CRC(09a21120) SHA1(e976d2c173c649e51b032bc5dad54f006864155c) )
	ROM_LOAD( "5.a8",         0x1400, 0x0400, CRC(92cd4da8) SHA1(217e00012a52c479bf0b0cf37ce556387755740d) )
ROM_END

ROM_START( gunchamp )
	ROM_REGION( 0x2000, "maincpu", 0 )
	ROM_LOAD( "251.a1",       0x0000, 0x0400, CRC(f27a8c1e) SHA1(510debd1ac2c0986f99c217e3078208a39d7837c) )
	ROM_LOAD( "252.a2",       0x0400, 0x0400, CRC(d53b8f91) SHA1(56919f4c88fb3b5c23b5365f0866698bfceb2762) )
	ROM_LOAD( "253.a4",       0x0800, 0x0400, CRC(9ef35c6c) SHA1(95bda3e2cdd50f7ac989c581481bad5f1ef2992f) )
	ROM_LOAD( "254.a5",       0x0c00, 0x0400, CRC(ba5b562d) SHA1(47819d7e5ef3700e700a5f2faa9537bc2199561c) )
	ROM_LOAD( "255.a6",       0x1000, 0x0400, CRC(00ea8293) SHA1(9c921fa4bafc36fc16a3f5f8588887342936d433) )
	ROM_LOAD( "256.a8",       0x1400, 0x0400, CRC(e271150c) SHA1(36d0c0c1335036b4a994e8a38904adcf74161c59) )
	ROM_LOAD( "257.a9",       0x1800, 0x0400, CRC(0da5d9ad) SHA1(c87c6ab248bfd2b75f070343a8f7fcbaed13f4e3) )
	ROM_LOAD( "258.a10",      0x1c00, 0x0400, CRC(471d4052) SHA1(c8ccda2eba44c2ab49f5fc2874fe70c2bdae35d3) )
ROM_END


GAME( 1978, claybust, 0, claybust, claybust, claybust_state, empty_init, ROT0, "Model Racing", "Claybuster (Extra Sounds)", MACHINE_SUPPORTS_SAVE )
GAMEL(1980, gunchamp, 0, claybust, gunchamp, claybust_state, empty_init, ROT0, "Model Racing", "Gun Champ (Extra Sounds)", MACHINE_SUPPORTS_SAVE, layout_gunchamp )

