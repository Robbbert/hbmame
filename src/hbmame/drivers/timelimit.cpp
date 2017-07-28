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
private:
	bool m_irq_state;
};

INTERRUPT_GEN_MEMBER(timelimt_hbmame::hb_irq)
{
	if ( m_nmi_enabled )
	{
		m_irq_state = (m_irq_state) ? false : true;
		if (m_irq_state)
			device.execute().set_input_line(INPUT_LINE_NMI, PULSE_LINE);
	}
}

static MACHINE_CONFIG_START( timelimit )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, 5000000)   /* 5.000 MHz */
	MCFG_CPU_PROGRAM_MAP(main_map)
	MCFG_CPU_IO_MAP(main_io_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", timelimt_hbmame, hb_irq)

	MCFG_CPU_ADD("audiocpu", Z80,18432000/6)    /* 3.072 MHz */
	MCFG_CPU_PROGRAM_MAP(sound_map)
	MCFG_CPU_IO_MAP(sound_io_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", timelimt_state,  irq0_line_hold) /* ? */

	MCFG_QUANTUM_TIME(attotime::from_hz(3000))

	MCFG_DEVICE_ADD("mainlatch", LS259, 0) // IC15
	MCFG_ADDRESSABLE_LATCH_Q0_OUT_CB(WRITELINE(timelimt_state, nmi_enable_w))
	MCFG_ADDRESSABLE_LATCH_Q2_OUT_CB(WRITELINE(timelimt_state, coin_lockout_w))
	MCFG_ADDRESSABLE_LATCH_Q3_OUT_CB(INPUTLINE("audiocpu", INPUT_LINE_RESET)) MCFG_DEVCB_INVERT
	MCFG_ADDRESSABLE_LATCH_Q6_OUT_CB(NOOP) // probably flip screen
	MCFG_ADDRESSABLE_LATCH_Q7_OUT_CB(NOOP) // probably flip screen

	MCFG_WATCHDOG_ADD("watchdog")

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MCFG_SCREEN_SIZE(32*8, 32*8)
	MCFG_SCREEN_VISIBLE_AREA(0*8, 32*8-1, 2*8, 30*8-1)
	MCFG_SCREEN_UPDATE_DRIVER(timelimt_state, screen_update)
	MCFG_SCREEN_PALETTE("palette")

	MCFG_GFXDECODE_ADD("gfxdecode", "palette", timelimt)
	MCFG_PALETTE_ADD("palette", 64+32)
	MCFG_PALETTE_INIT_OWNER(timelimt_state, timelimt)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")

	MCFG_GENERIC_LATCH_8_ADD("soundlatch")

	MCFG_SOUND_ADD("ay1", AY8910, 18432000/12)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)

	MCFG_SOUND_ADD("ay2", AY8910, 18432000/12)
	MCFG_AY8910_PORT_A_READ_CB(DEVREAD8("soundlatch", generic_latch_8_device, read))
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_CONFIG_END

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
	ROM_LOAD( "clr.48", 0x0020, 0x0020, CRC(813e17c7) SHA1(25a65d8a841538748348368ebcfce101976406a9) )
	ROM_LOAD( "clr.57", 0x0040, 0x0020, CRC(aaa6f23e) SHA1(9fcb6af82f725517e8eff86d748701f836a05eba) )
ROM_END

GAME( 2017, timelimit, timelimt, timelimit, timelimt, timelimt_hbmame, 0, ROT90, "Dink", "Time Limit (colour hack)", MACHINE_SUPPORTS_SAVE )
