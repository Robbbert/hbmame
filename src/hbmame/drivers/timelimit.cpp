// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/timelimt.cpp"

/* Changes to MAME's timelimt:
- Added a handcrafted clr.57 from Dink
- Added an alternate clr.48 from my collection
- Run the game at half speed, unplayable otherwise
*/

class timelimt_hbmame : public timelimt_state
{
public:
	timelimt_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: timelimt_state(mconfig, type, tag)
		, m_irq_state(false)
	{ }

	INTERRUPT_GEN_MEMBER(hb_irq);
	void timelimit(machine_config &config);
private:
	bool m_irq_state;
};

INTERRUPT_GEN_MEMBER(timelimt_hbmame::hb_irq)
{
	if ( m_nmi_enabled )
	{
		m_irq_state = (m_irq_state) ? false : true;
		if (m_irq_state)
			device.execute().pulse_input_line(INPUT_LINE_NMI, attotime::zero);
	}
}

void timelimt_hbmame::timelimit(machine_config &config)
{
	/* basic machine hardware */
	Z80(config, m_maincpu, 5000000);   /* 5.000 MHz */
	m_maincpu->set_addrmap(AS_PROGRAM, &timelimt_hbmame::main_map);
	m_maincpu->set_addrmap(AS_IO, &timelimt_hbmame::main_io_map);
	m_maincpu->set_vblank_int("screen", FUNC(timelimt_hbmame::hb_irq));

	Z80(config, m_audiocpu, 18432000/6);    /* 3.072 MHz */
	m_audiocpu->set_addrmap(AS_PROGRAM, &timelimt_hbmame::sound_map);
	m_audiocpu->set_addrmap(AS_IO, &timelimt_hbmame::sound_io_map);
	m_audiocpu->set_vblank_int("screen", FUNC(timelimt_hbmame::irq0_line_hold)); /* ? */

	config.set_maximum_quantum(attotime::from_hz(3000));

	ls259_device &mainlatch(LS259(config, "mainlatch")); // IC15
	mainlatch.q_out_cb<0>().set(FUNC(timelimt_hbmame::nmi_enable_w));
	mainlatch.q_out_cb<2>().set(FUNC(timelimt_hbmame::coin_lockout_w));
	mainlatch.q_out_cb<3>().set_inputline(m_audiocpu, INPUT_LINE_RESET).invert();
	mainlatch.q_out_cb<6>().set_nop(); // probably flip screen
	mainlatch.q_out_cb<7>().set_nop(); // probably flip screen

	WATCHDOG_TIMER(config, "watchdog");

	/* video hardware */
	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER));
	screen.set_refresh_hz(60);
	screen.set_vblank_time(ATTOSECONDS_IN_USEC(0));
	screen.set_size(32*8, 32*8);
	screen.set_visarea(0*8, 32*8-1, 2*8, 30*8-1);
	screen.set_screen_update(FUNC(timelimt_hbmame::screen_update));
	screen.set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_timelimt);
	PALETTE(config, m_palette, FUNC(timelimt_hbmame::timelimt_palette), 64+32);

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	GENERIC_LATCH_8(config, "soundlatch");

	AY8910(config, "ay1", 18432000/12).add_route(ALL_OUTPUTS, "mono", 0.25);

	ay8910_device &ay2(AY8910(config, "ay2", 18432000/12));
	ay2.port_a_read_callback().set("soundlatch", FUNC(generic_latch_8_device::read));
	ay2.add_route(ALL_OUTPUTS, "mono", 0.25);
}

ROM_START( timelimit )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "t8",     0x0000, 0x2000, CRC(006767ca) SHA1(a5d528c58cd73c0101ffa9ab783ec870668256db) )
	ROM_LOAD( "t7",     0x2000, 0x2000, CRC(cbe7cd86) SHA1(502a78c14c9717a466ea24cdc63da4c0f3bec1f9) )
	ROM_LOAD( "t6",     0x4000, 0x2000, CRC(f5f17e39) SHA1(7d78f551ce73276725c349703a790f2a63bb5503) )
	ROM_LOAD( "t9",     0x6000, 0x2000, CRC(2d72ab45) SHA1(01d4afacc01b9e7c49355123efd5f5ad4d79a9cd) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "tl5",    0x0000, 0x1000, CRC(5b782e4a) SHA1(2f4fe2beb8efa5a636fefc1ee172d0200d1c9497) )
	ROM_LOAD( "tl4",    0x1000, 0x1000, CRC(a32883a9) SHA1(26e1725b67be87db28855672facb1504b8ac84d6) )

	ROM_REGION( 0x2000, "tiles_1", 0 )
	ROM_LOAD( "tl11",   0x0000, 0x1000, CRC(46676307) SHA1(38fe80722972b6b3ba32705469a0dcb868fb76a9) )
	ROM_LOAD( "tl10",   0x1000, 0x1000, CRC(2336908a) SHA1(345fc209ce891cc6f8f111c6d3a9e0f65ee6d818) )

	ROM_REGION( 0x2000, "tiles_2", 0 )
	ROM_LOAD( "tl13",   0x0000, 0x1000, CRC(072e4053) SHA1(209edf7b371078e38d1c2812fa6a3d1a78193b3f) )
	ROM_LOAD( "tl12",   0x1000, 0x1000, CRC(ce960389) SHA1(57ee52cfa1b5a3832b362b38c8b7aa411dfc782b) )

	ROM_REGION( 0x6000, "sprites", 0 )
	ROM_LOAD( "tl3",    0x0000, 0x2000, CRC(01a9fd95) SHA1(cd1078700c97a3539c9d9447c55efbd27540a1b3) )
	ROM_LOAD( "tl2",    0x2000, 0x2000, CRC(4693b849) SHA1(fbebedde53599fb1eaedc648bd704b321ab096b5) )
	ROM_LOAD( "tl1",    0x4000, 0x2000, CRC(c4007caf) SHA1(ae05af3319545d5ca98a046bfc100138a5a3ed96) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "clr.35", 0x0000, 0x0020, CRC(9c9e6073) SHA1(98496175bf19a8cdb0018705bc1a2193b8a782e1) )
	ROM_LOAD( "clrt.48", 0x0020, 0x0020, CRC(813e17c7) SHA1(25a65d8a841538748348368ebcfce101976406a9) )
	ROM_LOAD( "clrt.57", 0x0040, 0x0020, CRC(aaa6f23e) SHA1(9fcb6af82f725517e8eff86d748701f836a05eba) )
ROM_END

GAME( 2017, timelimit, timelimt, timelimit, timelimt, timelimt_hbmame, empty_init, ROT90, "Dink", "Time Limit (colour hack)", MACHINE_SUPPORTS_SAVE )
