// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_f2.c"

static MACHINE_CONFIG_START( f2demo, taitof2_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", M68000, 24000000/2) /* 12 MHz */
	MCFG_CPU_PROGRAM_MAP(liquidk_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", taitof2_state,  taitof2_interrupt)

	MCFG_CPU_ADD("audiocpu", Z80, 24000000/6)   /* 4 MHz */
	MCFG_CPU_PROGRAM_MAP(sound_map)

	MCFG_MACHINE_START_OVERRIDE(taitof2_state,f2)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0)  /* frames per second, vblank duration */)
	MCFG_SCREEN_SIZE(120*8, 32*8)
	MCFG_SCREEN_VISIBLE_AREA(40*8, 106*8-1, 2*8, 32*8-1)
	MCFG_SCREEN_UPDATE_DRIVER(taitof2_state, screen_update_taitof2_pri)
	MCFG_SCREEN_VBLANK_DRIVER(taitof2_state, screen_eof_taitof2_partial_buffer_delayed)
	MCFG_SCREEN_PALETTE("palette")
	MCFG_GFXDECODE_ADD("gfxdecode", "palette", taitof2)
	MCFG_PALETTE_ADD("palette", 4096)
	MCFG_PALETTE_FORMAT(RRRRGGGGBBBBxxxx)
	MCFG_VIDEO_START_OVERRIDE(taitof2_state,taitof2_megab)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_STEREO("lspeaker", "rspeaker")

	MCFG_SOUND_ADD("ymsnd", YM2610, 24000000/3) /* Was 16000000/2, but only a 24Mhz OSC */
	MCFG_YM2610_IRQ_HANDLER(WRITELINE(taitof2_state, irqhandler))
	MCFG_SOUND_ROUTE(0, "lspeaker",  0.25)
	MCFG_SOUND_ROUTE(0, "rspeaker", 0.25)
	MCFG_SOUND_ROUTE(1, "lspeaker",  1.0)
	MCFG_SOUND_ROUTE(2, "rspeaker", 1.0)

	MCFG_DEVICE_ADD("tc0140syt", TC0140SYT, 0)
	MCFG_TC0140SYT_MASTER_CPU("maincpu")
	MCFG_TC0140SYT_SLAVE_CPU("audiocpu")

	MCFG_DEVICE_ADD("tc0220ioc", TC0220IOC, 0)
	MCFG_TC0220IOC_READ_0_CB(IOPORT("DSWA"))
	MCFG_TC0220IOC_READ_1_CB(IOPORT("DSWB"))
	MCFG_TC0220IOC_READ_2_CB(IOPORT("IN0"))
	MCFG_TC0220IOC_READ_3_CB(IOPORT("IN1"))
	MCFG_TC0220IOC_READ_7_CB(IOPORT("IN2"))

	MCFG_DEVICE_ADD("tc0100scn", TC0100SCN, 0)
	MCFG_TC0100SCN_GFX_REGION(1)
	MCFG_TC0100SCN_TX_REGION(2)
	MCFG_TC0100SCN_OFFSETS(3, 0)
	MCFG_TC0100SCN_GFXDECODE("gfxdecode")
	MCFG_TC0100SCN_PALETTE("palette")

	MCFG_TC0360PRI_ADD("tc0360pri")
MACHINE_CONFIG_END

// Game has no sound
ROM_START( f2demo )
	ROM_REGION( 0x80000, "maincpu", 0 )     /* 512k for 68000 code */
	ROM_LOAD16_BYTE( "lq09.bin",  0x00000, 0x20000, CRC(809a968b) SHA1(06e8be120869371d33c61fa14c48fe4e5fd659ec) )
	ROM_LOAD16_BYTE( "lq11.bin",  0x00001, 0x20000, CRC(7ba3a5cb) SHA1(08f5392296cceadcae0cd34e9dfe76c835583231) )

	ROM_REGION( 0x080000, "gfx1", 0 )	/* SCR */
	ROM_LOAD( "f2d_scr.bin", 0x000000, 0x80000, CRC(b178fb05) SHA1(3d33dd822622579c75b55971926f6eb04d33f5a6) )	/* TC0100SCN #1 */

	ROM_REGION( 0x100000, "gfx2", 0 )	/* OBJ */
	ROM_LOAD( "f2d_obj0.bin", 0x00000, 0x80000, CRC(1bb8aa37) SHA1(9051270485a7a995779c80b4e692b8395503d6c3) )
	ROM_LOAD( "f2d_obj1.bin", 0x80000, 0x80000, CRC(75660aac) SHA1(6a521e1d2a632c26e53b83d2cc4b0edecfc1e68c) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )		/* sound cpu */
	ROM_LOAD( "c49-08.9",    0x00000, 0x04000, CRC(413c310c) SHA1(cecb1c0c9fe3c8b744f95ce29009650a289107ab) )
	ROM_CONTINUE(            0x10000, 0x0c000 )	/* banked stuff */

	ROM_REGION( 0x80000, "ymsnd", 0 )	/* ADPCM samples */
	ROM_LOAD( "lk_snd.bin",  0x00000, 0x80000, CRC(474d45a4) SHA1(20cb818d753a185973098007e645f1aa75c5528d) )

	/* no Delta-T samples */
ROM_END

GAME( 1990, f2demo, 0, f2demo, liquidk, driver_device, 0, ROT180, "Charles Doty", "Demo - Taito F2", MACHINE_SUPPORTS_SAVE )
