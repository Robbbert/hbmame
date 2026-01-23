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
#include "attackfc.lh"


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


class attackfc_state : public driver_device
{
public:
	attackfc_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_mb14241(*this,"mb14241")
		, m_watchdog(*this, "watchdog")
		, m_main_ram(*this, "main_ram")
		, m_samples(*this, "samples")
		, m_screen(*this, "screen")
	{ }

	void attackfc(machine_config &config);
	void init_attackfc();

private:
	required_device<i8085a_cpu_device> m_maincpu;
	required_device<mb14241_device> m_mb14241;
	optional_device<watchdog_timer_device> m_watchdog;
	required_shared_ptr<uint8_t> m_main_ram;
	required_device<samples_device> m_samples;
	required_device<screen_device> m_screen;

	virtual void machine_start() override;
	virtual void machine_reset() override;

	void sound_w(uint8_t data);
	TIMER_CALLBACK_MEMBER(interrupt_trigger);
	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	DECLARE_WRITE_LINE_MEMBER(int_enable_w);
	IRQ_CALLBACK_MEMBER(interrupt_vector);
	uint8_t vpos_to_vysnc_chain_counter(int vpos);
	int vysnc_chain_counter_to_vpos(uint8_t counter, int vblank);
	attotime m_interrupt_time;
	bool m_int_enable = true;
	emu_timer *m_interrupt_timer = nullptr;

	void main_map(address_map &map);
	void io_map(address_map &map);

	uint8_t m_sound_en = 0;
};


uint8_t attackfc_state::vpos_to_vysnc_chain_counter( int vpos )
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

int attackfc_state::vysnc_chain_counter_to_vpos( uint8_t counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}

TIMER_CALLBACK_MEMBER(attackfc_state::interrupt_trigger)
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


WRITE_LINE_MEMBER(attackfc_state::int_enable_w)
{
	m_int_enable = state;
}

IRQ_CALLBACK_MEMBER(attackfc_state::interrupt_vector)
{
	int vpos = m_screen->vpos();
	if (machine().time() < m_interrupt_time)
		vpos++;
	uint8_t counter = vpos_to_vysnc_chain_counter(vpos);
	uint8_t vector = 0xc7 | ((counter & 0x40) >> 2) | ((~counter & 0x40) >> 3);

	m_maincpu->set_input_line(0, CLEAR_LINE);
	return vector;
}

uint32_t attackfc_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
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

void attackfc_state::machine_start()
{
	m_interrupt_timer = timer_alloc(FUNC(attackfc_state::interrupt_trigger), this);
}

void attackfc_state::machine_reset()
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));

	m_interrupt_time = attotime::zero;
}

static const char *const attackfc_sample_names[] =
{
	"*invaders",
	"6",
	"5",
	"7",
	"2",
	"4",
	0
};

void attackfc_state::sound_w(uint8_t data)
{
	if (m_sound_en)
	{
		if (data == 1)
			m_samples->start(0,0);
		else
		if (data == 2)
			m_samples->start(1,1);
		else
		if (data == 3)
			m_samples->start(2,2);
		else
		if (data == 8)
			m_samples->start(3,3);
		else
		if (data == 0x10)
			m_samples->start(4,4);
	}
}

void attackfc_state::main_map(address_map &map)
{
	map.global_mask(0x7fff);
	map(0x0000, 0x1fff).rom();
	map(0x2000, 0x3fff).ram().share("main_ram");
	map(0x4000, 0x4fff).noprw();
}

void attackfc_state::io_map(address_map &map)
{
	map(0x00, 0x00).portr("IN0");
	map(0x02, 0x02).nopw(); // lamp?
	map(0x03, 0x03).rw(m_mb14241, FUNC(mb14241_device::shift_result_r), FUNC(mb14241_device::shift_data_w));
	map(0x04, 0x04).lw8(NAME([this] (u8 data) {m_sound_en = data; }));
	map(0x05, 0x05).w(m_watchdog, FUNC(watchdog_timer_device::reset_w));
	map(0x06, 0x06).w(FUNC(attackfc_state::sound_w));
	map(0x07, 0x07).w(m_mb14241, FUNC(mb14241_device::shift_count_w));
}

static INPUT_PORTS_START( attackfc )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_2WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_2WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN1 )
INPUT_PORTS_END

void attackfc_state::attackfc(machine_config &config)
{
	/* basic machine hardware */
	I8080(config, m_maincpu, MW8080BW_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &attackfc_state::main_map);
	m_maincpu->set_addrmap(AS_IO, &attackfc_state::io_map);
	m_maincpu->set_irq_acknowledge_callback(FUNC(attackfc_state::interrupt_vector));
	m_maincpu->out_inte_func().set(FUNC(attackfc_state::int_enable_w));

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(attackfc_state::screen_update));

	/* sound hardware */
	SPEAKER(config, "mono").front_center();
	SAMPLES(config, m_samples);
	m_samples->set_channels(5);
	m_samples->set_samples_names(attackfc_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 0.50);

	WATCHDOG_TIMER(config, m_watchdog).set_time(555 * attotime::from_hz(MW8080BW_60HZ));
	MB14241(config, m_mb14241);
}

void attackfc_state::init_attackfc()
{
	uint8_t *rom = memregion("maincpu")->base();
	uint32_t len = memregion("maincpu")->bytes();
	std::vector<uint8_t> buffer(len);

	// swap a8/a9
	for (int i = 0; i < len; i++)
		buffer[bitswap<16>(i, 15,14,13,12,11,10,8,9, 7,6,5,4,3,2,1,0)] = rom[i];

	memcpy(rom, &buffer[0], len);
}

ROM_START( attackfc )
	ROM_REGION( 0x2000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "30a.bin",       0x0000, 0x0400, CRC(c12e3386) SHA1(72b1d3d67a83edf0be0b0c37ef6dcffba450f16f) )
	ROM_LOAD( "36a.bin",       0x0400, 0x0400, CRC(6738dcb9) SHA1(e4c68553fc3f2d3db3d251b9cb325e2409d9c02a) )
	ROM_LOAD( "31a.bin",       0x0800, 0x0400, CRC(787a4658) SHA1(5be3143bdba6a32256603be94400034a8ea1fda6) )
	ROM_LOAD( "37a.bin",       0x0c00, 0x0400, CRC(ad6bfbbe) SHA1(5f5437b6c8e7dfe9649b25040862f8a51d8c43ed) )
	ROM_LOAD( "32a.bin",       0x1000, 0x0400, CRC(cbe0a711) SHA1(6e5f4214a4b48b70464005f4263c9b1ec3cbbeb1) )
	ROM_LOAD( "33a.bin",       0x1800, 0x0400, CRC(53147393) SHA1(57e078f1734e382e8a46be09c133daab30c75681) )
	ROM_LOAD( "39a.bin",       0x1c00, 0x0400, CRC(f538cf08) SHA1(4a375a41ab5d9f0d9f9a2ebef4c448038c139204) )
ROM_END

GAMEL(1979?,attackfc,  0, attackfc, attackfc,  attackfc_state, init_attackfc, ROT0, "Electronic Games Systems", "Attack Force (Extra Sounds)", MACHINE_SUPPORTS_SAVE, layout_attackfc )

