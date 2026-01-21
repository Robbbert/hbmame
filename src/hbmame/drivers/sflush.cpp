// license:GPL_2.0
// copyright-holders:Robbbert

/*******************************************************/
/*                                                     */
/* Taito "Straight Flush"                              */
/*                                                     */
/*******************************************************/
// Note: various MT bugs with this game, involving player 2
// Controls are crap. Use -mouse to at least have a small chance. Keyboard is useless.
#include "emu.h"
#include "cpu/m6800/m6800.h"
#include "machine/mb14241.h"   // used for ball movement
#include "machine/timer.h"
#include "sound/beep.h"
#include "sound/spkrdev.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"


#define MW8080BW_MASTER_CLOCK             (19968000.0)
#define MW8080BW_CPU_CLOCK                (MW8080BW_MASTER_CLOCK / 10)
#define MW8080BW_PIXEL_CLOCK              (MW8080BW_MASTER_CLOCK / 4)
#define MW8080BW_HTOTAL                   (0x140)
#define MW8080BW_HBEND                    (0x000)
#define MW8080BW_HBSTART                  (0xf4) // was 0x100, but causes large space above scores
#define MW8080BW_VTOTAL                   (0x106)
#define MW8080BW_VBEND                    (0x000)
#define MW8080BW_VBSTART                  (0x0e0)  // 0xd0 looks better, but then game crashes randomly
#define MW8080BW_VCOUNTER_START_NO_VBLANK (0x020)
#define MW8080BW_VCOUNTER_START_VBLANK    (0x0da)
#define MW8080BW_INT_TRIGGER_COUNT_1      (0x080)
#define MW8080BW_INT_TRIGGER_VBLANK_1     (0)
#define MW8080BW_INT_TRIGGER_COUNT_2      MW8080BW_VCOUNTER_START_VBLANK
#define MW8080BW_INT_TRIGGER_VBLANK_2     (1)
#define MW8080BW_60HZ                     (MW8080BW_PIXEL_CLOCK / MW8080BW_HTOTAL / MW8080BW_VTOTAL)

// +4 is added to HBSTART because the hardware displays that many pixels after setting HBLANK
#define MW8080BW_HPIXCOUNT                (MW8080BW_HBSTART + 4)


class sflush_state : public driver_device
{
public:
	sflush_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_mb14241(*this,"mb14241")
		, m_main_ram(*this, "main_ram")
		, m_beep(*this, "beep_%u",0)
		, m_speaker(*this, "speaker")
		, m_screen(*this, "screen")
		, m_palette(*this, "palette")
	{ }

	void sflush(machine_config &config);
	READ_LINE_MEMBER(sflush_80_r);
private:

	IRQ_CALLBACK_MEMBER(interrupt_vector);
	TIMER_CALLBACK_MEMBER(interrupt_trigger);

	virtual void machine_start() override;
	virtual void machine_reset() override;

	DECLARE_WRITE_LINE_MEMBER(int_enable_w);
	void sflush_palette(palette_device &palette) const;
	void colorram_w(offs_t offset, uint8_t data);
	uint8_t colorram_r(offs_t offset);
	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	void sflush_map(address_map &map);

	// device/memory pointers
	required_device<cpu_device> m_maincpu;
	required_device<mb14241_device> m_mb14241;
	required_shared_ptr<uint8_t> m_main_ram;
	required_device_array<beep_device, 6> m_beep;
	required_device<speaker_sound_device> m_speaker;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;

	std::unique_ptr<uint8_t[]> m_colorram;

	uint8_t vpos_to_vysnc_chain_counter(int vpos);
	int vysnc_chain_counter_to_vpos(uint8_t counter, int vblank);
	attotime m_interrupt_time;
	bool m_int_enable = true;
	emu_timer *m_interrupt_timer = nullptr;
	void main_map(address_map &map);
	void set_pixel( bitmap_rgb32 &bitmap, uint8_t y, uint8_t x, int color );
	uint8_t sflush_in0_r();
	void sound_w(uint8_t data);
	uint8_t m_sound_en = 0;
	uint8_t m_flip_screen = 0;
};

uint8_t sflush_state::vpos_to_vysnc_chain_counter( int vpos )
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

int sflush_state::vysnc_chain_counter_to_vpos( uint8_t counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}

TIMER_CALLBACK_MEMBER(sflush_state::interrupt_trigger)
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

READ_LINE_MEMBER(sflush_state::sflush_80_r)
{
	return (m_screen->vpos() & 0x80) ? 1 : 0;
}

uint8_t sflush_state::sflush_in0_r()
{
	// guess at interrupt acknowledgement
	if (!machine().side_effects_disabled())
		m_maincpu->set_input_line(0, CLEAR_LINE);

	uint8_t t = ioport("IN0")->read();
	m_flip_screen = !BIT(t,7) && BIT(m_sound_en,5);
	return t;
}

void sflush_state::sflush_palette(palette_device &palette) const
{
	// standard 3-bit rbg palette, but background color is bright blue
	palette.set_pen_color(0, 0x80, 0x80, 0xff);
	for (int i = 1; i < 8; i++)
		palette.set_pen_color(i, rgb_t(pal1bit(i >> 0), pal1bit(i >> 2), pal1bit(i >> 1)));
}

uint8_t sflush_state::colorram_r(offs_t offset)
{
	return m_colorram[(offset & 0x1f) | ((offset & 0x1f80) >> 2)];
}

void sflush_state::colorram_w(offs_t offset, uint8_t data)
{
	m_colorram[(offset & 0x1f) | ((offset & 0x1f80) >> 2)] = data;
}

inline void sflush_state::set_pixel(bitmap_rgb32 &bitmap, uint8_t y, uint8_t x, int color)
{
	if (y >= MW8080BW_VCOUNTER_START_NO_VBLANK)
	{
		if (m_flip_screen)
			bitmap.pix(MW8080BW_VBSTART - 1 - (y - MW8080BW_VCOUNTER_START_NO_VBLANK), MW8080BW_HPIXCOUNT - 1 - x) = m_palette->pen_color(color);
		else
			bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = m_palette->pen_color(color);
	}
}


uint32_t sflush_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	for (offs_t offs = 0; offs < m_main_ram.bytes(); offs++)
	{
		uint8_t y = offs >> 5;
		uint8_t x = offs << 3;

		uint8_t data = m_main_ram[offs];
		uint8_t fore_color = m_colorram[(offs & 0x1f) | ((offs & 0x1f80) >> 2)] & 0x07;

		for (uint8_t i = 0; i < 8; i++)
		{
			set_pixel(bitmap, y, x, BIT(data, 0) ? fore_color : 0);
			x++;
			data >>= 1;
		}
	}


	return 0;
}

void sflush_state::sound_w(uint8_t data)
{
	if (!BIT(m_sound_en, 2))
		data = 0;

	//if (data) printf("%X ",data);

	data = bitswap<8>(data, 5, 7, 6, 4, 3, 2, 1, 0);   // now, speaker on bit 7, beeper on bits 5-0

	for (uint8_t i = 0; i < 6; i++)
		m_beep[i]->set_state(BIT(data, i));

	m_speaker->level_w(BIT(data, 7));  // tune when completing a level
}

void sflush_state::sflush_map(address_map &map)
{
	map(0x0000, 0x3fff).ram();
	map(0x4000, 0x5fff).ram().share("main_ram");
	map(0x8008, 0x8008).portr("PADDLE");
	map(0x8009, 0x8009).r(m_mb14241, FUNC(mb14241_device::shift_result_r));
	map(0x800a, 0x800a).portr("IN2");
	map(0x800b, 0x800b).r(FUNC(sflush_state::sflush_in0_r));
	map(0x8018, 0x8018).nopr().w(m_mb14241, FUNC(mb14241_device::shift_data_w));
	map(0x8019, 0x8019).w(m_mb14241, FUNC(mb14241_device::shift_count_w));
	map(0x801a, 0x801a).nopw();  // watchdog?
	map(0x801c, 0x801c).w(FUNC(sflush_state::sound_w));
	map(0x801d, 0x801d).lw8(NAME([this] (u8 data) {m_sound_en = data;}));
	map(0xa000, 0xbfff).rw(FUNC(sflush_state::colorram_r), FUNC(sflush_state::colorram_w));
	map(0xd800, 0xffff).rom();
}


static INPUT_PORTS_START( sflush )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Upright ) )
	//PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )   // freezes on 2nd player

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x01, "4" )
	PORT_DIPSETTING(    0x02, "5" )
	PORT_DIPSETTING(    0x03, "6" )
	PORT_DIPNAME( 0x08, 0x00, "Hiscore" )
	PORT_DIPSETTING(    0x00, "0" )
	PORT_DIPSETTING(    0x08, "30000" )
	PORT_DIPNAME( 0x40, 0x00, "Coinage Display" )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On ) )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(sflush_state, sflush_80_r) // 128V?

	PORT_START("PADDLE")
	PORT_BIT( 0xff, 0x6a, IPT_PADDLE ) PORT_MINMAX(0x16,0xbf) PORT_SENSITIVITY(30) PORT_KEYDELTA(30) PORT_CENTERDELTA(0)
INPUT_PORTS_END


void sflush_state::machine_start()
{
	m_interrupt_timer = timer_alloc(FUNC(sflush_state::interrupt_trigger), this);
	m_colorram = std::make_unique<uint8_t []>(0x800);
	save_pointer(&m_colorram[0], "m_colorram", 0x800);
}

void sflush_state::machine_reset()
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));
	m_interrupt_time = attotime::zero;

	for (uint8_t i = 0; i < 6; i++)
		m_beep[i]->set_state(0);
}

void sflush_state::sflush(machine_config &config)
{
	/* basic machine hardware */
	M6800(config, m_maincpu, 1500000); // ?
	m_maincpu->set_addrmap(AS_PROGRAM, &sflush_state::sflush_map);

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(sflush_state::screen_update));

	/* sound hardware */
	SPEAKER(config, "mono").front_center();
	BEEP(config, m_beep[0], 250).add_route(ALL_OUTPUTS, "mono", 0.30);
	BEEP(config, m_beep[1], 700).add_route(ALL_OUTPUTS, "mono", 0.30);
	BEEP(config, m_beep[2], 800).add_route(ALL_OUTPUTS, "mono", 0.30);
	BEEP(config, m_beep[3], 400).add_route(ALL_OUTPUTS, "mono", 0.30);
	BEEP(config, m_beep[4], 900).add_route(ALL_OUTPUTS, "mono", 0.30);
	BEEP(config, m_beep[5], 1100).add_route(ALL_OUTPUTS, "mono", 0.30);
	SPEAKER_SOUND(config, m_speaker).add_route(ALL_OUTPUTS, "mono", 0.50);

	/* add shifter */
	MB14241(config, m_mb14241);

	/* video hardware */
	PALETTE(config, m_palette, FUNC(sflush_state::sflush_palette), 8);
}

ROM_START( sflush )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "fr05.sc2",     0xd800, 0x800, CRC(c4f08f9f) SHA1(997f216f5244942fc1a19f5c1988adbfadc301fc) )
	ROM_LOAD( "fr04.sc3",     0xe000, 0x800, CRC(87a754a5) SHA1(07c0e2c3cb7aa0086d8f4dd202a452bc6c20d4ee) )
	ROM_LOAD( "fr03.sc4",     0xe800, 0x800, CRC(5b12847f) SHA1(4b62342723dd49a387fae6637c331d7c853712a3) )
	ROM_LOAD( "fr02.sc5",     0xf000, 0x800, CRC(291c9b1f) SHA1(7e5b3e1605581abf3d8165f4de9d4e32a5ee3bb0) )
	ROM_LOAD( "fr01.sc6",     0xf800, 0x800, CRC(55d688c6) SHA1(574a3a2ca73cabb4b8f3444aa4464e6d64daa3ad) )
ROM_END

GAME( 1979, sflush, 0, sflush, sflush, sflush_state, empty_init, ROT270, "Taito", "Straight Flush (Extra Sounds)", MACHINE_SUPPORTS_SAVE | MACHINE_NO_COCKTAIL | MACHINE_NOT_WORKING )

