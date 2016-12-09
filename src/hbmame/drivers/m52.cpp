// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/m52.cpp"

ROM_START( spatrol )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mpa-1.3m",      0x0000, 0x1000, CRC(5873a860) SHA1(8c03726d6e049c3edbc277440184e31679f78258) )
	ROM_LOAD( "mpa-2.3l",      0x1000, 0x1000, CRC(f4b85974) SHA1(dfb2efb57378a20af6f20569f4360cde95596f93) )
	ROM_LOAD( "mpa-3.3k",      0x2000, 0x1000, CRC(2e1a598c) SHA1(112c3c9678db8a8540a8df3708020c87fd10c91b) )
	ROM_LOAD( "mpa-4.3j",      0x3000, 0x1000, CRC(dd05b587) SHA1(727961b0dafa4a96b580d51013336db2a18aff1e) )

	ROM_REGION( 0x8000, "irem_audio:iremsound", 0 )
	ROM_LOAD( "mp-s1.1a",     0x7000, 0x1000, CRC(561d3108) SHA1(4998c68a9e9a8002251fa8f07aa1082444a9dc80) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "spatrol.3e",   0x0000, 0x1000, CRC(130709e8) SHA1(fa3d0aca3179073c92a39c50f535d945f9b58725) )
	ROM_LOAD( "spatrol.3f",   0x1000, 0x1000, CRC(1ac58abc) SHA1(1a945c4efa6ef4a7d68cb6cb7b91a76be956837e) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "spatrol.3m",   0x0000, 0x1000, CRC(09e18f9a) SHA1(9f41aacbc5352b8783422fdffef949426bada588) )
	ROM_LOAD( "spatrol.3n",   0x1000, 0x1000, CRC(36b1891e) SHA1(e204c259ca9c37d0843b5c877debf2171c40249a) )

	ROM_REGION( 0x1000, "gfx3", 0 )
	ROM_LOAD( "spatrol.3l",   0x0000, 0x1000, CRC(49877023) SHA1(a76b18afb20ab416318812f66bfdd57733441cb5) )

	ROM_REGION( 0x1000, "gfx4", 0 )
	ROM_LOAD( "mpe-2.3k",     0x0000, 0x1000, CRC(c7aa1fb0) SHA1(14c6c76e1d0db2c0745e5d6d33ea6945fac8e9ee) )

	ROM_REGION( 0x1000, "gfx5", 0 )
	ROM_LOAD( "spatrol.3h",   0x0000, 0x1000, CRC(091045ad) SHA1(715b5b964fe06ab015e30ce6e38d6737f0033510) )

	ROM_REGION( 0x0340, "proms", 0 )
	ROM_LOAD( "mpc-4.2a",     0x0000, 0x0200, CRC(07f99284) SHA1(dfc52958f2520e1ce4446dd4c84c91413bbacf76) )
	ROM_LOAD( "mpc-3.1m",     0x0200, 0x0020, CRC(6a57eff2) SHA1(2d1c12dab5915da2ccd466e39436c88be434d634) )
	ROM_LOAD( "mpc-1.1f",     0x0220, 0x0020, CRC(26979b13) SHA1(8c41a8cce4f3384c392a9f7a223a50d7be0e14a5) )
	ROM_LOAD( "mpc-2.2h",     0x0240, 0x0100, CRC(7ae4cd97) SHA1(bc0662fac82ffe65f02092d912b2c2b0c7a8ac2b) )
ROM_END

GAME( 2008, spatrol,  mpatrol, m52, mpatrol, driver_device, 0, ROT0, "Gatinho", "Sponge Patrol", MACHINE_SUPPORTS_SAVE )


// alpha1vx, here for playing around with
// Issues: sometimes you die for no reason; and the sprites seem to be missing solidarity and colours
// It's safer to stay near the bottom, you can get through a few levels that way. Keep firing or you will self-destruct within a few seconds.
// The POW bonus (which is made of 4 bytes) only shows the bottom half or the left half... probably a code bug

static ADDRESS_MAP_START( alpha1vx_map, AS_PROGRAM, 8, m52_state )
	AM_RANGE(0x0000, 0x6fff) AM_ROM
	AM_RANGE(0x8000, 0x83ff) AM_RAM_WRITE(m52_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x8400, 0x87ff) AM_RAM_WRITE(m52_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x8800, 0x8800) AM_READNOP // watchdog? HBMAME
	AM_RANGE(0xc800, 0xc9ff) AM_WRITEONLY AM_SHARE("spriteram") // bigger or mirrored?
	AM_RANGE(0xd000, 0xd000) AM_READ_PORT("IN0") AM_DEVWRITE("irem_audio", irem_audio_device, cmd_w)
	AM_RANGE(0xd001, 0xd001) AM_READ_PORT("IN1") AM_WRITE(alpha1v_flipscreen_w)
	AM_RANGE(0xd002, 0xd002) AM_READ_PORT("IN2")
	AM_RANGE(0xd003, 0xd003) AM_READ_PORT("DSW1")
	AM_RANGE(0xd004, 0xd004) AM_READ_PORT("DSW2")
	AM_RANGE(0xe000, 0xefff) AM_RAM // bigger or mirrored?
ADDRESS_MAP_END

static MACHINE_CONFIG_START( alpha1vx, m52_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, MASTER_CLOCK/6)
	MCFG_CPU_PROGRAM_MAP(alpha1vx_map)
	MCFG_CPU_IO_MAP(main_portmap)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", m52_state,  irq0_line_hold)

	/* video hardware */
	MCFG_GFXDECODE_ADD("gfxdecode", "palette", m52)
	MCFG_PALETTE_ADD("palette", 128*4+16*4+3*4)
	MCFG_PALETTE_INDIRECT_ENTRIES(512+32+32)
	MCFG_PALETTE_INIT_OWNER(m52_state, m52)

	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_RAW_PARAMS(MASTER_CLOCK/3, 384, 136, 376, 282, 16, 272) // HBMAME
	MCFG_SCREEN_UPDATE_DRIVER(m52_state, screen_update_m52)
	MCFG_SCREEN_PALETTE("palette")

	/* sound hardware */
	MCFG_DEVICE_ADD("irem_audio", IREM_M52_SOUNDC_AUDIO, 0)

MACHINE_CONFIG_END


ROM_START( alpha1vx )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "2-m3",      0x0000, 0x1000, CRC(3a679d34) SHA1(1a54a43070c56dc91d4d258f29e29613bb309f1c) )
	ROM_LOAD( "3-l3",      0x1000, 0x1000, CRC(2f09df64) SHA1(e91602e9e41ad24dd1d7f384ed81b9bdaadd03e1) )
	ROM_LOAD( "4-k3",      0x2000, 0x1000, CRC(64fb9c8a) SHA1(735fd00cc42193a417e6cde75f12b4cf2e804942) )
	ROM_LOAD( "5-j3",      0x3000, 0x1000, CRC(d1643d18) SHA1(7c794b82e17e2ba0a6237e3fc20d8314f6c2481c) )
	ROM_LOAD( "6-h3",      0x4000, 0x1000, CRC(cf34ab51) SHA1(3696da71e2bc7edd1ee7aeaac87be5386608c09e) )
	ROM_LOAD( "7-f3",      0x5000, 0x1000, CRC(99db9781) SHA1(a56a675cc4cbc9681bfe8052f51f19336eb2a0a6) )
	ROM_LOAD( "7a e3",     0x6000, 0x1000, CRC(3b0b4b0d) SHA1(0d8eea1e2db269943611289b3490a578ee347f85) )

	ROM_REGION( 0x8000, "irem_audio:iremsound", 0 )
	ROM_LOAD( "1-a1",      0x7000, 0x1000, CRC(9e07fdd5) SHA1(ed4f462fcfe91fa8e88bfeaaba0a0c11fa0b4601) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "14-e3",     0x0000, 0x1000, CRC(cf00c737) SHA1(415e90289039cac4d04cb1d559f1378ca6a32132) )
	ROM_LOAD( "13-f3",     0x1000, 0x1000, CRC(4b799229) SHA1(42cbdcf787b08b041d30504d699a12c378224933) )

	ROM_REGION( 0x3000, "gfx2", 0 ) // 3bpp? (mpatrol is 2bpp..)
	ROM_LOAD( "15-n3",     0x0000, 0x1000, CRC(dc26df76) SHA1(dd1cff7935f5559f9d1b440e02d5e5aa521b0054) )
	ROM_LOAD( "16-l3",     0x1000, 0x1000, CRC(39b9863b) SHA1(da9da9a1066188f050c422dfed1bbbd3ba612ccc) )
	ROM_LOAD( "17-k3",     0x2000, 0x1000, CRC(cfd90773) SHA1(052e126888b6de636db9c521a090699c282b620b) )

	ROM_REGION( 0x1000, "gfx3", 0 )
	ROM_LOAD( "11-k3",     0x0000, 0x1000, CRC(7659440a) SHA1(2efd27c82913513dd03e799f1ed3c10b0863677d) )

	ROM_REGION( 0x1000, "gfx4", 0 )
	ROM_LOAD( "9-n3",     0x0000, 0x1000, CRC(0fdb7d13) SHA1(e828254a4f94df633d338b5772719276d41c6b7f) )

	ROM_REGION( 0x1000, "gfx5", 0 )
	ROM_LOAD( "10-lm3",     0x0000, 0x1000, CRC(9dde3a75) SHA1(293d093485be19bfb20685d76a08ac78e24062bf) )

	ROM_REGION( 0x0340, "proms", 0 )
	ROM_LOAD( "63s481-a2",     0x0000, 0x0200, CRC(58678ea8) SHA1(b13a78a5bca8ad5bdec1293512b53654768a7a7a) )
	ROM_LOAD( "18s030-m1",     0x0200, 0x0020, CRC(6a57eff2) SHA1(2d1c12dab5915da2ccd466e39436c88be434d634) )
	ROM_LOAD( "mb7051-f1",     0x0220, 0x0020, CRC(d8bdd0df) SHA1(ca522428927911808214d319af314f601497ded4) )
	ROM_LOAD( "mb7052-h2",     0x0240, 0x0100, CRC(ce9f0ef9) SHA1(3afb94ed033f272983bbed22a59856df7824ef8a) )
ROM_END

GAME( 1988, alpha1vx,  alpha1v, alpha1vx, alpha1v, driver_device, 0, ROT0, "Vision Electronics", "Alpha One (test)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )

