// license:GPL_2.0
// copyright-holders:Robbbert

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "sound/samples.h"
#include "sound/sn76477.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"
#include "shuttlei.lh"

namespace {

class skylove_state : public driver_device
{
public:
	skylove_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_main_ram(*this, "main_ram")
		, m_samples(*this, "samples")
		, m_sn(*this, "snsnd")
		, m_screen(*this, "screen")
		, m_inputs(*this, "INPUTS")
		, m_p2(*this, "P2")
		, m_palette(*this, "palette")
	{ }

	void skylove(machine_config &config);

private:
	required_device<i8085a_cpu_device> m_maincpu;
	required_shared_ptr<uint8_t> m_main_ram;
	required_device<samples_device> m_samples;
	optional_device<sn76477_device> m_sn;
	required_device<screen_device> m_screen;
	required_ioport m_inputs;
	required_ioport m_p2;
	required_device<palette_device> m_palette;

	virtual void machine_start() override;
	virtual void machine_reset() override;

	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint8_t m_port_1_last = 0;
	bool m_flip_screen = 0;
	uint8_t port_ff_r();
	void port_ff_w(uint8_t data);
	void port_fd_w(uint8_t data);
	void port_fe_w(uint8_t data);

	void main_map(address_map &map);
	void io_map(address_map &map);
};


uint32_t skylove_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	for (offs_t offs = 0; offs < m_main_ram.bytes(); offs++)
	{
		uint8_t const y = offs >> 5;
		uint8_t const x = offs << 3;

		uint8_t data = m_main_ram[offs];

		for (int i = 0; i < 8; i++)
		{
			if (m_flip_screen)
				bitmap.pix(191-y, 255-(x|i)) = m_palette->pen_color(BIT(data, 7));
			else
				bitmap.pix(y, x|i) = m_palette->pen_color(BIT(data, 7));
			data <<= 1;
		}
	}

	return 0;
}

void skylove_state::machine_start()
{
	save_item(NAME(m_port_1_last));
	save_item(NAME(m_flip_screen));
}

void skylove_state::machine_reset()
{
}

uint8_t skylove_state::port_ff_r()
{
	uint8_t data = m_inputs->read();
	if (!m_flip_screen)
		return data;
	else
		return (data & 0x3b) | m_p2->read();
}

void skylove_state::port_ff_w(uint8_t data)
{
	/* bit 0 goes high when first coin inserted
	   bit 1 also goes high when subsequent coins are inserted
	      These may be for indicator lamps under the start buttons.
	   bit 2 goes high while player 2 is playing
	*/
	m_flip_screen = BIT(data, 2) & BIT(ioport("CAB")->read(), 0);
}

static const char *const skylove_sample_names[] =
{
	"*invaders",
	"1",        /* Shoot */
	"2",        /* Death */
	"3",        /* invader hit */
	nullptr
};

// The game gives a bonus, but no sound for it.
void skylove_state::port_fd_w(uint8_t data)
{
	if (BIT(data, 5))
		m_samples->start(2, 2);  // Hit
}

void skylove_state::port_fe_w(uint8_t data)
{
	uint8_t c = data ^ m_port_1_last;
	if (c && (data ==0x0a))
		m_samples->start(1, 1);           // Death
	else
	if ((c == 0x04) && (BIT(data, 2)))
		m_samples->start(0, 0);           // Shoot

	m_sn->enable_w(BIT(data, 0) ? 0:1);   //Rocket
	m_port_1_last = data;
}

void skylove_state::main_map(address_map &map)
{
	map(0x0000, 0x1fff).rom();
	map(0x2000, 0x37ff).ram().share("main_ram");
	map(0x3800, 0x3fff).ram();
	map(0x6000, 0x63ff).ram();
}

void skylove_state::io_map(address_map &map)
{
	map(0xfc, 0xfc).nopw(); // screen red when you die, for just a moment via monostable
	map(0xfd, 0xfd).w(FUNC(skylove_state::port_fd_w));
	map(0xfe, 0xfe).portr("DSW").w(FUNC(skylove_state::port_fe_w));
	map(0xff, 0xff).rw(FUNC(skylove_state::port_ff_r), FUNC(skylove_state::port_ff_w));
}


// 'no 1' which is displayed before each player plays actually refers to the wave number, not the player number!
static INPUT_PORTS_START( skylove )
	PORT_START("DSW")
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPNAME( 0x06, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x02, "4" )
	PORT_DIPSETTING(    0x04, "5" )
	PORT_DIPSETTING(    0x06, "6" )
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x60, 0x00, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x00, "10k" )
	PORT_DIPSETTING(    0x20, "20k" )
	PORT_DIPSETTING(    0x40, "40k" )
	PORT_DIPSETTING(    0x60, "60k" )
	PORT_DIPNAME( 0x80, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("INPUTS")
	PORT_DIPNAME( 0x01, 0x01, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) ) // must be off to boot
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_CUSTOM ) PORT_VBLANK("screen")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_2WAY PORT_PLAYER(1)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_2WAY PORT_PLAYER(1)

	PORT_START("P2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_2WAY PORT_PLAYER(2)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_2WAY PORT_PLAYER(2)

	/* Dummy port for cocktail mode */
	PORT_START("CAB")
	PORT_CONFNAME( 0x01, 0x00, DEF_STR( Cabinet ) )
	PORT_CONFSETTING(    0x00, DEF_STR( Upright ) )
	PORT_CONFSETTING(    0x01, DEF_STR( Cocktail ) )
INPUT_PORTS_END

void skylove_state::skylove(machine_config &config)
{
	/* basic machine hardware */
	i8080_cpu_device &maincpu(I8080(config, m_maincpu, XTAL(18'000'000) / 9));
	maincpu.set_addrmap(AS_PROGRAM, &skylove_state::main_map);
	maincpu.set_addrmap(AS_IO, &skylove_state::io_map);

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_refresh_hz(60);
	m_screen->set_vblank_time(ATTOSECONDS_IN_USEC(2500));
	m_screen->set_size(32*8, 32*8);
	m_screen->set_visarea(0*8, 32*8-1, 0*8, 24*8-1);
	m_screen->set_screen_update(FUNC(skylove_state::screen_update));

	PALETTE(config, m_palette, palette_device::MONOCHROME);

	/* sound hardware */
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
	m_samples->set_samples_names(skylove_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 1.0);
}

ROM_START( skylove )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "01",   0x0000, 0x0400, CRC(391ad7d0) SHA1(73358fff44da5fffd4e08fbb615ccc0245e3365b) )
	ROM_LOAD( "02",   0x0400, 0x0400, CRC(365ba070) SHA1(8493bde493ea0d04b3563f9bc752a6ec57022524) )
	ROM_LOAD( "03",   0x0800, 0x0400, CRC(47364dad) SHA1(b49704f8d49a0866cb9cd8bb867f30246e3dabc9) )
	ROM_LOAD( "04",   0x0c00, 0x0400, CRC(9d76f33d) SHA1(5aa6a081a3609e6c036843049d58cc763a86fedb) )
	ROM_LOAD( "05",   0x1000, 0x0400, CRC(09084954) SHA1(f5c826188ffb7a572c45aad94e794f31bebfebe5) )
	ROM_LOAD( "06",   0x1400, 0x0400, CRC(6d494e82) SHA1(8e5ee1b842621cd088e80124b92b8a517e8dfbb9) )
	ROM_LOAD( "07",   0x1800, 0x0400, CRC(1a9aa4b8) SHA1(0da553c6343a2740312ebafc2b936ffbbf24af04) )
	ROM_LOAD( "08",   0x1c00, 0x0400, CRC(ecaacacc) SHA1(b815366d3aaa8ef311cd54a5be9fb4d60324e5a7) )
ROM_END

} // anonymous namespace

GAMEL(1979, skylove, 0, skylove, skylove, skylove_state, empty_init, ROT270, "Omori Electric Co., Ltd.", "Sky Love (Extra Sounds)", MACHINE_SUPPORTS_SAVE | MACHINE_IMPERFECT_SOUND, layout_shuttlei )

