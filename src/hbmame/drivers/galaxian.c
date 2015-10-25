// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/galaxian.c"


/*************************************
 *
 *  Four Play
 *
 *************************************/

static ADDRESS_MAP_START( fourplay_map, AS_PROGRAM, 8, galaxian_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x47ff) AM_RAM
	AM_RANGE(0x5000, 0x53ff) AM_MIRROR(0x0400) AM_RAM_WRITE(galaxian_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x5800, 0x58ff) AM_MIRROR(0x0700) AM_RAM_WRITE(galaxian_objram_w) AM_SHARE("spriteram")
	AM_RANGE(0x6000, 0x6000) AM_READ_PORT("IN0")
	AM_RANGE(0x6800, 0x6800) AM_READ_PORT("IN1")
	AM_RANGE(0x7000, 0x7000) AM_READ_PORT("IN2")
	AM_RANGE(0x7800, 0x7fff) AM_READ(watchdog_reset_r)
	AM_RANGE(0x6000, 0x6001) AM_WRITE(start_lamp_w)
	AM_RANGE(0x6002, 0x6002) AM_WRITENOP // AM_WRITE(coin_lock_w)
	AM_RANGE(0x6003, 0x6003) AM_WRITE(coin_count_0_w)
	AM_RANGE(0x6004, 0x6007) AM_DEVWRITE("cust", galaxian_sound_device, lfo_freq_w)
	AM_RANGE(0x6800, 0x6807) AM_DEVWRITE("cust", galaxian_sound_device, sound_w)
	AM_RANGE(0x7001, 0x7001) AM_WRITE(irq_enable_w)
	AM_RANGE(0x7002, 0x7003) AM_WRITE(fourplay_rombank_w)
	AM_RANGE(0x7004, 0x7004) AM_WRITE(galaxian_stars_enable_w)
	AM_RANGE(0x7005, 0x7005) AM_WRITENOP			/* bit 3 of rombank select - always 0 */
	AM_RANGE(0x7006, 0x7006) AM_WRITE(galaxian_flip_screen_x_w)
	AM_RANGE(0x7007, 0x7007) AM_WRITE(galaxian_flip_screen_y_w)
	AM_RANGE(0x7008, 0x7008) AM_WRITENOP			/* bit 4 of rombank select - always 0 */
	AM_RANGE(0x7800, 0x7800) AM_DEVWRITE("cust", galaxian_sound_device, pitch_w)
ADDRESS_MAP_END

static MACHINE_CONFIG_DERIVED( fourplay, galaxian )
	/* info can be found at http://www.ionpool.net/arcade/galaxian_hacks/fourplay.htm */
	/* basic machine hardware */
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(fourplay_map)

	/* video hardware */
	MCFG_GFXDECODE_MODIFY("gfxdecode", gmgalax)
	MCFG_PALETTE_MODIFY("palette")
	MCFG_PALETTE_ENTRIES(64)
MACHINE_CONFIG_END

DRIVER_INIT_MEMBER( galaxian_state, fourplay )
{
	membank("bank1")->configure_entries(0, 4, memregion("maincpu")->base() + 0x10000, 0x4000);
	membank("bank1")->set_entry(0);

	/* video extensions */
	common_init(NULL, NULL, &galaxian_state::pisces_extend_tile_info, &galaxian_state::pisces_extend_sprite_info);
}



/*************************************
 *
 *  Galaxiab
 *
 *************************************/

static INPUT_PORTS_START( galaxiab )
	PORT_INCLUDE(galaxian)

	PORT_MODIFY("IN2")
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x00, DEF_STR( None ) )
	PORT_DIPSETTING(    0x01, "20000" )
	PORT_DIPSETTING(    0x02, "40000" )
	PORT_DIPSETTING(    0x03, "80000" )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPSETTING(    0x04, "3" )
	PORT_BIT( 0xf8, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END



/***********************************************************
 *
 *  Jump Bug (with extra sounds via Galaxian sound board)
 *
 ***********************************************************/

static ADDRESS_MAP_START( jumpbugx_map, AS_PROGRAM, 8, galaxian_state )
	AM_IMPORT_FROM(jumpbug_map)
/* HBMAME - added next lines */
	AM_RANGE(0x6800, 0x6807) AM_MIRROR(0x07f8) AM_DEVWRITE("cust", galaxian_sound_device, sound_w)
	AM_RANGE(0x7800, 0x7800) AM_MIRROR(0x07ff) AM_DEVWRITE("cust", galaxian_sound_device, pitch_w)
ADDRESS_MAP_END

static MACHINE_CONFIG_DERIVED( jumpbugx, jumpbug )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(jumpbugx_map)
	MCFG_FRAGMENT_ADD(galaxian_audio)
MACHINE_CONFIG_END


/*************************************
 *
 *  Mr. Do Nightmare
 *
 *************************************/

static INPUT_PORTS_START( mrdonm )

	PORT_START("IN0")

	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )

	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_2WAY

	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_2WAY

	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )


	PORT_START("IN1")

	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_START1 )

	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )

	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_8WAY

	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_8WAY

	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_DIPNAME( 0xc0, 0x00, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0xc0, DEF_STR( Free_Play ) )

	PORT_START("IN2")

	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )

INPUT_PORTS_END

/*************************************
 *
 *  Trukker
 *
 *************************************/

DRIVER_INIT_MEMBER( galaxian_state, trukker )
{
	UINT16 i;

	/* video extensions */
	common_init(NULL, &galaxian_state::frogger_draw_background, &galaxian_state::frogger_extend_tile_info, &galaxian_state::frogger_extend_sprite_info);
	m_frogger_adjust = TRUE;

	/* the first ROM of the second CPU has data lines D0 and D1 swapped. Decode it. */
	UINT8 *ROM = memregion("audiocpu")->base();
	for (i = 0;i < 0x800;i++) ROM[i] = BITSWAP8(ROM[i],7,6,5,4,3,2,0,1);

	/* fix gfx error in the alphabetic characters */
	ROM = memregion("gfx1")->base();
	for (i = 0;i < 0x160;i++) ROM[i] = BITSWAP8(ROM[i],7,6,5,4,3,2,0,1);

	/* remove extraneous dot from steam rollers */
	for (i = 0x1a0;i < 0x200;i++) ROM[i] = ROM[i] & 0xfc;

	/* fix gfx error in sinking turtle */
	for (i = 0x4c0;i < 0x4e0;i++) ROM[i] = BITSWAP8(ROM[i],7,6,5,4,3,2,0,1);

	/* fix gfx error on left side of home bases */
	ROM[0x234] = BITSWAP8(ROM[0x234],7,6,5,4,3,2,0,1);

	/* fix gfx error in (c) sign and in square symbol */
	for (i = 0x270;i < 0x27f;i++) ROM[i] = BITSWAP8(ROM[i],7,6,5,4,3,2,0,1);

	/* fix gfx error in 200-point sign */
	for (i = 0x330;i < 0x337;i++) ROM[i] = BITSWAP8(ROM[i],7,6,5,4,3,2,0,1);
}



/************************************************************
 *
 *  Frogger Sound Test rom - have to disable the watchdog
 *
 ************************************************************/

static MACHINE_CONFIG_DERIVED( tst_frog, frogger )
	MCFG_WATCHDOG_VBLANK_INIT(0)
MACHINE_CONFIG_END



/*************************************
 *
 *  Video Eight
 *
 *************************************/

static ADDRESS_MAP_START( videight_map, AS_PROGRAM, 8, galaxian_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x47ff) AM_RAM
	AM_RANGE(0x5000, 0x53ff) AM_MIRROR(0x400) AM_RAM_WRITE(galaxian_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x5800, 0x58ff) AM_MIRROR(0x700) AM_RAM_WRITE(galaxian_objram_w) AM_SHARE("spriteram")
	AM_RANGE(0x6000, 0x6000) AM_READ_PORT("IN0")
	AM_RANGE(0x6800, 0x6800) AM_READ_PORT("IN1")
	AM_RANGE(0x7000, 0x7000) AM_READ_PORT("IN2")
	AM_RANGE(0x7800, 0x7fff) AM_READ(watchdog_reset_r)
	AM_RANGE(0x6000, 0x6002) AM_WRITE(videight_gfxbank_w)
	AM_RANGE(0x6003, 0x6003) AM_WRITE(coin_count_0_w)
	AM_RANGE(0x6004, 0x6007) AM_DEVWRITE("cust", galaxian_sound_device, lfo_freq_w)
	AM_RANGE(0x6800, 0x6807) AM_DEVWRITE("cust", galaxian_sound_device, sound_w)
	AM_RANGE(0x6808, 0x68ff) AM_WRITENOP
	AM_RANGE(0x7001, 0x7001) AM_WRITE(irq_enable_w)
	AM_RANGE(0x7002, 0x7005) AM_WRITE(videight_rombank_w)
	AM_RANGE(0x7006, 0x7006) AM_WRITE(galaxian_flip_screen_x_w)
	AM_RANGE(0x7007, 0x7007) AM_WRITE(galaxian_flip_screen_y_w)
	AM_RANGE(0x7008, 0x7008) AM_WRITENOP			/* bit 4 of rombank select - always 0 */
	AM_RANGE(0x7800, 0x7800) AM_DEVWRITE("cust", galaxian_sound_device, pitch_w)
ADDRESS_MAP_END

static GFXDECODE_START( videight )
	GFXDECODE_SCALE( "gfx1", 0x0000, galaxian_charlayout,   0, 8*32, GALAXIAN_XSCALE,1)
	GFXDECODE_SCALE( "gfx1", 0x0000, galaxian_spritelayout, 0, 8*32, GALAXIAN_XSCALE,1)
GFXDECODE_END

static MACHINE_CONFIG_DERIVED( videight, galaxian )

	/* basic machine hardware */
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(videight_map)

	/* video hardware */
	MCFG_GFXDECODE_MODIFY("gfxdecode", videight)
	MCFG_PALETTE_MODIFY("palette")
	MCFG_PALETTE_ENTRIES(8*32)
MACHINE_CONFIG_END

DRIVER_INIT_MEMBER( galaxian_state, videight )
{
	membank("bank1")->configure_entries(0, 8, memregion("maincpu")->base() + 0x10000, 0x4000);
	membank("bank1")->set_entry(0);

	/* video extensions */
	common_init(NULL, NULL, &galaxian_state::videight_extend_tile_info, &galaxian_state::videight_extend_sprite_info);
}


/***************************************************************************

  Game driver(s)

***************************************************************************/




/* this looks exactly like Galaxian, the word "Andromeda" never appears. The dipswitches are different. */
ROM_START( andromd )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "and01",        0x0000, 0x0400, CRC(a0059ddd) SHA1(06bc3bd5eef9fea9e038ec08e5a228ab19f31a56) )
	ROM_LOAD( "and02",        0x0400, 0x0400, CRC(da6421b8) SHA1(d3099d94d722e3e12ade712842261aac8b45f9f4) )
	ROM_LOAD( "and03",        0x0800, 0x0400, CRC(30e28016) SHA1(07a621e5061d85a9559a920d76716ea4db61b674) )
	ROM_LOAD( "and04",        0x0c00, 0x0400, CRC(de7e7770) SHA1(b06043a1d898eb323ddabffd3d2a3b1f63df0e5e) )
	ROM_LOAD( "and05",        0x1000, 0x0400, CRC(a916c919) SHA1(b3e264ff92687022a0f2f551d5df36db848b48eb) )
	ROM_LOAD( "and06",        0x1400, 0x0400, CRC(9175882b) SHA1(d9943efcb9245af7f01aecc533a699bdefc7d283) )
	ROM_LOAD( "and07",        0x1800, 0x0400, CRC(1237b9da) SHA1(00e11532c599fca452a816683b361a24476b7100) )
	ROM_LOAD( "and08",        0x1c00, 0x0400, CRC(8b181929) SHA1(c7e789d1a76e8db0251ac4c75aa85f0a4259fc81) )
	ROM_LOAD( "and09",        0x2000, 0x0400, CRC(17fd5357) SHA1(abda92319f6851232a0dd26229fc55cf6d8361d8) )
	ROM_LOAD( "and10",        0x2400, 0x0400, CRC(3032821c) SHA1(7ff45a9ddd86e77a550b10148147f44c4c934467) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "1h.bin",       0x0000, 0x0800, CRC(39fb43a4) SHA1(4755609bd974976f04855d51e08ec0d62ab4bc07) )
	ROM_LOAD( "1k.bin",       0x0800, 0x0800, CRC(7e3f56a2) SHA1(a9795d8b7388f404f3b0e2c6ce15d713a4c5bafa) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END


#if 0
/* From CloneMAME 0.101 - this is galapx with different gfx roms */
ROM_START( galaxni )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galx.u",       0x0000, 0x0800, CRC(79e4007d) SHA1(d55050498a670d1c022ba3caad34f8fcaccf4a30) )
	ROM_LOAD( "galx.v",       0x0800, 0x0800, CRC(bc16064e) SHA1(4e3220fd63c8184bf9581a89dffb6944d8fae3bb) )
	ROM_LOAD( "galx.w",       0x1000, 0x0800, CRC(72d2d3ee) SHA1(96e0c5824e46d7398c7e58dd6b75a9f4ead6f3f5) )
	ROM_LOAD( "galx.y",       0x1800, 0x0800, CRC(afe397f3) SHA1(283c6f3b3f07581d88f7a6e11fc36947a9d90e2e) )
	ROM_LOAD( "galx.z",       0x2000, 0x0800, CRC(778c0d3c) SHA1(6a81875abfea515d379c6212cb57f8e54573e943) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galaxni.1h",   0x0000, 0x0800, CRC(1b13ca05) SHA1(6999068771dacc6bf6c17eb858af593a929d09af) )
	ROM_LOAD( "galaxni.1k",   0x0800, 0x0800, CRC(5cd7df03) SHA1(77873408c89546a17b1da3f64b7e96e314fadb17) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END
#endif

/* From CloneMAME 0.111 */
ROM_START( galaxni )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galaxni.u",    0x0000, 0x0800, CRC(0701fcff) SHA1(b4fbc864fb9d95af63c32427779aad05934d95d0) )
	ROM_LOAD( "galaxni.v",    0x0800, 0x0800, CRC(cb1136d8) SHA1(7be8c50bbd1b891ca7ded597e55b481efa6ac9d5) )
	ROM_LOAD( "galaxni.w",    0x1000, 0x0800, CRC(f45ef2c7) SHA1(6b50383d98360afee8137a94c4c91d4d10db9221) )
	ROM_LOAD( "galaxni.y",    0x1800, 0x0800, CRC(3f5c8a62) SHA1(7bde7f712c2cb97d40467062390097707b2896ae) )
	ROM_LOAD( "galaxni.z",    0x2000, 0x0800, CRC(2d897370) SHA1(21c108712e7c00bd0f30d1fdddfaa71eed84158c) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galaxni.1h",   0x0000, 0x0800, CRC(6c14fa93) SHA1(0ac55adee875fde0488b4376c4ca5e0047d09fbf) )
	ROM_LOAD( "galaxni.1k",   0x0800, 0x0800, CRC(2bd0ef95) SHA1(a17edda8940bca02820e270a3ec9e38666343b26) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "galaxni.clr",  0x0000, 0x0020, CRC(5aa5c5dd) SHA1(be74103889248f24123d1df553da02586681835e) )
ROM_END


ROM_START( buglaxn )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galmidw.u",    0x0000, 0x0800, CRC(745e2d61) SHA1(e65f74e35b1bfaccd407e168ea55678ae9b68edf) )
	ROM_LOAD( "galmidw.v",    0x0800, 0x0800, CRC(9c999a40) SHA1(02fdcd95d8511e64c0d2b007b874112d53e41045) )
	ROM_LOAD( "galmidw.w",    0x1000, 0x0800, CRC(b5894925) SHA1(0046b9ed697a34d088de1aead8bd7cbe526a2396) )
	ROM_LOAD( "galmidw.y",    0x1800, 0x0800, CRC(6b3ca10b) SHA1(18d8714e5ef52f63ba8888ecc5a25b17b3bf17d1) )
	ROM_LOAD( "galmidw.z",    0x2000, 0x0800, CRC(cb24f797) SHA1(e6bb977ded0654c2c7388aad188059e1e0647908) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "buglaxn.1j",   0x0000, 0x0800, CRC(b8d46e4b) SHA1(5fbf130912029fbe2aea5282b2201c6a17eae40b) )
	ROM_LOAD( "buglaxn.1k",   0x0800, 0x0800, CRC(a00bde50) SHA1(c6519af48d2cf0a641b2f5aa327c3591f3184dbc) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

/* This is a combination of 2 EMMA dumps. E48 contained all roms except for "y" - even though the web page said
   it was there. E423 contained all roms except the prom, because this board had been hacked to work without it.

   The prom is the same except that the last 2 bytes are zeroed. This causes the explosions to be mostly missing,
   and since the last byte also controls the colour of the floating scores, these are blanked. */

ROM_START( galaxiab )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galaxiab.u",  0x0000, 0x0800, CRC(63e6d8ab) SHA1(ec2ade4d93903ba99bc9565b68328bd7d0830eac) )
	ROM_LOAD( "galaxiab.v",  0x0800, 0x0800, CRC(f58283e3) SHA1(edc6e72516c50fd3402281d9936574d276581ce9) )
	ROM_LOAD( "galaxiab.w",  0x1000, 0x0800, CRC(4c7031c0) SHA1(97f7ab0cedcd8eba1c8f6f516d84d672a2108258) )
	ROM_LOAD( "galaxiab.y",  0x1800, 0x0800, CRC(faa0bd8c) SHA1(30dd130298609632212b34874a0819340b1c8037) )
	ROM_LOAD( "galaxiab.z",  0x2000, 0x0800, CRC(5766c95b) SHA1(cf6c226df0cc9d088b04ade43d6db87d278f8b09) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galaxiab.1h", 0x0000, 0x0800, CRC(977e37cf) SHA1(88ff1e4edadf5cfc83413a1fe999aecf4ba72232) )
	ROM_LOAD( "galaxiab.1k", 0x0800, 0x0800, CRC(bc7d0985) SHA1(550a44c9ddc8da66a29405591a590f91d2675cdf) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "galaxiab.6l", 0x0000, 0x0020, CRC(feea6525) SHA1(cdb207776f51411ed4e9536babff6050488d2808) )
	ROM_FILL( 31,1,7 )	/* patch so floating scores can be seen, and explosions look better */
ROM_END

ROM_START( galaxkyo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galxk.u",      0x0000, 0x0800, CRC(acfde501) SHA1(4b72c1ffecaccadc541da2367f3ef70a2a9aed64) )
	ROM_LOAD( "galxk.v",      0x0800, 0x0800, CRC(65cf3c77) SHA1(1c5249815816b395e1e04bf6a7dbb63e40faa0e3) )
	ROM_LOAD( "galxk.w",      0x1000, 0x0800, CRC(9eef9ae6) SHA1(b2282e4edb8911e6aabfa936c3526f90381e1320) )
	ROM_LOAD( "galxk.y",      0x1800, 0x0800, CRC(56a5ddd1) SHA1(1f87f647ebdffba28d5957f195448f6bce17f4d5) )
	ROM_LOAD( "galxk.z",      0x2000, 0x0800, CRC(f4bc7262) SHA1(c4b70e474d49f45cec96f7c250bd77e01e18601a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galxk.1h",     0x0000, 0x0800, CRC(66a4335d) SHA1(0527a9ea99e525b3ba25c172034a61cde9307a9b) )
	ROM_LOAD( "galxk.1k",     0x0800, 0x0800, CRC(2160265b) SHA1(97385e9c8012bc7fe85e29cfccefda72e4ca3320) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( galinvad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galnamco.7f",  0x0000, 0x1000, CRC(b10533fd) SHA1(ea8653e80000d607cd37a41f2de27ffe6e190f0f) )
	ROM_LOAD( "galnamco.7j",  0x1000, 0x1000, CRC(9163454d) SHA1(683ce43c0e24fd5110b860e424e5de30d59e5223) )
	ROM_LOAD( "galnamco.7l",  0x2000, 0x1000, CRC(952c6135) SHA1(e0989d5dce37ae421c34833d48b09e76d178680d) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galinv.1h",    0x0000, 0x0800, CRC(d3d42377) SHA1(4eae6b2b20cfb6841d679006f394c7081f8f426f) )
	ROM_LOAD( "galinv.1k",    0x0800, 0x0800, CRC(7e515c84) SHA1(e15ff199f1db55e9abb30c403fc667a4e7143b4f) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( galnamco )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galnamco.7f",  0x0000, 0x1000, CRC(b10533fd) SHA1(ea8653e80000d607cd37a41f2de27ffe6e190f0f) )
	ROM_LOAD( "galnamco.7j",  0x1000, 0x1000, CRC(9163454d) SHA1(683ce43c0e24fd5110b860e424e5de30d59e5223) )
	ROM_LOAD( "galnamco.7l",  0x2000, 0x1000, CRC(952c6135) SHA1(e0989d5dce37ae421c34833d48b09e76d178680d) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "1h.bin",       0x0000, 0x0800, CRC(39fb43a4) SHA1(4755609bd974976f04855d51e08ec0d62ab4bc07) )
	ROM_LOAD( "1k.bin",       0x0800, 0x0800, CRC(7e3f56a2) SHA1(a9795d8b7388f404f3b0e2c6ce15d713a4c5bafa) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( gaylaxn )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galx.u",       0x0000, 0x0800, CRC(79e4007d) SHA1(d55050498a670d1c022ba3caad34f8fcaccf4a30) )
	ROM_LOAD( "galx.v",       0x0800, 0x0800, CRC(bc16064e) SHA1(4e3220fd63c8184bf9581a89dffb6944d8fae3bb) )
	ROM_LOAD( "galx.w",       0x1000, 0x0800, CRC(72d2d3ee) SHA1(96e0c5824e46d7398c7e58dd6b75a9f4ead6f3f5) )
	ROM_LOAD( "galx.y",       0x1800, 0x0800, CRC(afe397f3) SHA1(283c6f3b3f07581d88f7a6e11fc36947a9d90e2e) )
	ROM_LOAD( "galx.z",       0x2000, 0x0800, CRC(778c0d3c) SHA1(6a81875abfea515d379c6212cb57f8e54573e943) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galxn.1h",     0x0000, 0x0800, CRC(ce77d89b) SHA1(17de678c91e96bf3dc414cc2ad8631a8f617da3d) )
	ROM_LOAD( "galxn.1k",     0x0800, 0x0800, CRC(08008d2c) SHA1(6600dbc42e5076f9ce3abe677134bf1677ae70ea) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( gmunch )
	/* There was gmunch and gmunch2, using same roms, different proms. But colours are completely wrong in either case
	   Both games have now been removed (replaced by ghostmun) */
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pac1.bin",     0x0000, 0x1000, CRC(19338c70) SHA1(cc2665b7d534d324627d12025ee099ff415d4214) )
	ROM_LOAD( "pac2.bin",     0x1000, 0x1000, CRC(18db074d) SHA1(a70ed18f632e947493e648e6fc057dfb7a2a3322) )
	ROM_LOAD( "pac3.bin",     0x2000, 0x1000, CRC(abb98b1d) SHA1(bb0109d353359bb192a3e6856a857c2f842838cb) )
	ROM_LOAD( "pac4.bin",     0x3000, 0x1000, CRC(2403c78e) SHA1(52d8c8a4efcf47871485080ab217098a019e6579) )
	/* this rom isn't loaded into rom space so what is it for? */
	ROM_LOAD( "pac7.bin",     0x5000, 0x1000, CRC(6566c07b) SHA1(8047350e5f6a39d34dfc9242fa0747db9819fe99) )

	ROM_REGION( 0x3000, "gfx1" , 0 )
	ROM_LOAD( "pac5.bin",     0x0000, 0x1000, CRC(a5965fa6) SHA1(c746f90b4c4620ef6190f9ae85d5036bda3b81ae) )
	ROM_LOAD( "pac8.bin",     0x1000, 0x1000, CRC(0491a967) SHA1(868f7c7efdbe40c8e0977557bc2cedefb9a3ea81) )
	/* this rom contains gfx from uniwars and mame doesn't process it */
	ROM_LOAD( "pac6.bin",     0x2000, 0x1000, CRC(2145e7ea) SHA1(a2e3a54fa25b030d72431f42e12783db08e95e2e) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "sk.bpr",       0x0000, 0x0020, CRC(bce79607) SHA1(49d60fde149240bcd025f721b0fbbbdbc549a42f) ) /* gmunch */
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) ) /* gmunch2 */
ROM_END

ROM_START( mooncmst )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mc1",          0x0000, 0x0800, CRC(7d954a7a) SHA1(a93ee403cfd7887538ad12d33f6dd6c71bea2a32) )
	ROM_LOAD( "mc2",          0x0800, 0x0800, CRC(44bb7cfa) SHA1(349c2e23a9fce73f95bb8168d369082fa129fe3d) )
	ROM_LOAD( "mc3",          0x1000, 0x0800, CRC(9c412104) SHA1(1b40054ebb1ace965a8522119bb23f09797bc5f6) )
	ROM_LOAD( "mc4",          0x1800, 0x0800, CRC(7e9b1ab5) SHA1(435f603c0c3e788a509dd144a7916a34e791ae44) )
	ROM_LOAD( "mc5.7r",       0x2000, 0x0800, CRC(16c759af) SHA1(3b48050411f65f9d3fb41ff22901e22d82bf1cf6) )
	ROM_LOAD( "mc6.8d",       0x2800, 0x0800, CRC(69bcafdb) SHA1(939c8c6ed1cd4660a3d99b8f17ed99cbd7e1352a) )
	ROM_LOAD( "mc7.8e",       0x3000, 0x0800, CRC(b50dbc46) SHA1(4fa084fd1ba5f78e7703e684c57af15ca7a844e4) )
	ROM_LOAD( "mc8",          0x3800, 0x0800, CRC(18ca312b) SHA1(39219059003b949e38305553fea2d33071062c64) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mmcs_b",       0x0000, 0x0800, CRC(94d69a79) SHA1(d984ce68046244589e94cb526dc2fa11a85077d9) )
	ROM_LOAD( "mmcs_d",       0x0800, 0x0800, CRC(44a6c84a) SHA1(464dc9a11fdf3015ce9d4735c27c1d6eab51cecb) )
	ROM_LOAD( "mmcs_a",       0x1000, 0x0800, CRC(d8c9713c) SHA1(13f514bed011bb7f420e99bd9caeca0f2a1012ff) )
	ROM_LOAD( "mmcs_c",       0x1800, 0x0800, CRC(50e37124) SHA1(ae07715733e864eadc3b4586c3e9f1852a4cf127) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "prom.6l",      0x0000, 0x0020, CRC(6a0c7d87) SHA1(140335d85c67c75b65689d4e76d29863c209cf32) )
ROM_END


/* E78 - The dump was missing one rom (position "fake" below), I found that the equivalent code from fantazia
		works perfectly well. The rom still fails the checksum test, which is why the test is patched. So
		take note: It is a FAKE rom!!!

		The colour prom supplied produced terrible colours, with most enemies barely visible. Substituting
		the normal mooncrst prom fixes all the problems. */

ROM_START( mooncrs5 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mooncrs5.7k", 0x0000, 0x1000, CRC(d62875a5) SHA1(2e162bf559a72acb5d23434497a9377a302b0440) )
	ROM_LOAD( "mooncrs5.7j", 0x1000, 0x1000, CRC(1683e74c) SHA1(5887ddeed7d3399e93bc3146f54c23e9f8893866) )
	ROM_LOAD( "mooncrs5.7h", 0x2000, 0x1000, CRC(46921e92) SHA1(7a3cec261a9e0baf969bd356a0766b4b266eaa39) )
	ROM_LOAD( "mooncrs5.7a", 0x3000, 0x0400, CRC(5318be0c) SHA1(cca73501da8783e47f3f3c7d8a795011d5fd91a3) )
	ROM_LOAD( "mooncrs5.7b", 0x3400, 0x0400, CRC(aed8acb2) SHA1(a2429daba96e6c064f4bc65cab2df6b15a43bf44) )
	ROM_LOAD( "mooncrs5.fake", 0x3800, 0x0400, CRC(76db94a5) SHA1(f7d9c81d5e09f7e42258caf265606b1ba32439dd) ) /* fake rom */
	ROM_LOAD( "mooncrs5.7d", 0x3c00, 0x0400, CRC(db0528cf) SHA1(be06b78d890e07fcbbfd6931d1a59ab0fb8b7a53) )
	ROM_FILL(0x3FF4,1,0x2c )	/* patch unused byte so checksum test can be passed */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mooncrs5.1h", 0x0800, 0x0800, CRC(60d783bc) SHA1(584a0600ef1c47fb8bce86f738af7691fdf91c9c) )
	ROM_CONTINUE(            0x0000, 0x0800 )
	ROM_LOAD( "mooncrs5.1k", 0x1800, 0x0800, CRC(bc25be88) SHA1(0e698a1e573f28e8aad07e79c11f38519d93286c) )
	ROM_CONTINUE(            0x1000, 0x0800 )

	ROM_REGION( 0x0020, "proms", 0 )
//	ROM_LOAD( "6l.bpr",      0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
	ROM_LOAD( "prom.6l",     0x0000, 0x0020, CRC(6a0c7d87) SHA1(140335d85c67c75b65689d4e76d29863c209cf32) )
ROM_END

// Mr. Do. Nightmare
ROM_START( mrdonm )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mrdonm.u",    0x0000, 0x0800, CRC(197493a6) SHA1(f939fd712985db24dced4f7a66f4a804ca34ce60) )
	ROM_LOAD( "mrdonm.v",    0x0800, 0x0800, CRC(b8ee84cf) SHA1(c5018f21da2f65fa573a2d5e9c8f96db1f0136e9) )
	ROM_LOAD( "mrdonm.w",    0x1000, 0x0800, CRC(76879d31) SHA1(7aae5ee1eeaa5dacf4a232b3336f9e3df74018ca) )
	ROM_LOAD( "mrdonm.y",    0x1800, 0x0800, CRC(d6d5e47e) SHA1(e5f41f90ca357f3e7ef1a6b055ca0cccc91ed391) )
	ROM_LOAD( "mrdonm.z",    0x2000, 0x0800, CRC(34913886) SHA1(c1936ef6dc6080d080715d1c064513d581180fea) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "mrdonm.1h",   0x0000, 0x0800, CRC(f880af4b) SHA1(67d24ac48a6bd68de5b914675dc4cd5982d8ffc4) )
	ROM_LOAD( "mrdonm.1k",   0x0800, 0x0800, CRC(40fd608a) SHA1(3a58d5ac17e98bea3d58d238d81bd6a5dd24bb81) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "mrdonm.6l",   0x0000, 0x0020, CRC(77f95861) SHA1(2acb3021e31b3c91bbf48763d2504dfad0d87f38) )
ROM_END


/* StarFighter II - This set came with a1,a2,b2,c1,e2 only. The rest I've used from Pisces */
ROM_START( starfgh2 )
	ROM_REGION( 0x4000, "maincpu", 0 )
	ROM_LOAD( "a1.bin",       0x0000, 0x0800, CRC(0d496c93) SHA1(c0f9a20d01a11045f6ad8a6c9c3980a52ba16495) )
	ROM_LOAD( "a2.bin",       0x0800, 0x0800, CRC(4f0676eb) SHA1(7b4a230c723959261cb240fce52d378a01740942) )
	ROM_LOAD( "b2.bin",       0x1000, 0x0800, CRC(1496a894) SHA1(d010cd02b1ffe351ef1abdd424cdefca44ea6b29) )
	ROM_LOAD( "c1.bin",       0x1800, 0x0800, CRC(59820900) SHA1(d42b99573a44f76620d75647c1c253a869e9a403) )
	ROM_LOAD( "p5.bin",       0x2000, 0x0800, CRC(fade512b) SHA1(ccef2650f1d9dc3fdde2d441774246d47febc2cc) )
	ROM_LOAD( "e2.bin",       0x2800, 0x0800, CRC(153316b8) SHA1(c7de0c0f65723d3699e68f3b8b76cd1d69dbd0f3) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "g09.bin",      0x0000, 0x0800, CRC(9503a23a) SHA1(23848de56841dd1de9ef74d5a9c981c784098175) )
	ROM_LOAD( "g11.bin",      0x0800, 0x0800, CRC(0adfc3fe) SHA1(a4da488632d9906066db45ae62747caf5ffbf2d8) )
	ROM_LOAD( "g10.bin",      0x1000, 0x0800, CRC(3e61f849) SHA1(efa0059bc843af0c3bb94f4bc0a8286ca5069179) )
	ROM_LOAD( "g12.bin",      0x1800, 0x0800, CRC(7130e9eb) SHA1(e6bb7a9b4f2fc001296e1060d0671b7a88599c8b) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "colour.bin",   0x0000, 0x0020, CRC(57a45057) SHA1(d4ec6a54f72640e5b485aa59e206c090e67ff640) )
ROM_END

ROM_START( supergs )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "7f.bin",       0x0000, 0x1000, CRC(4335b1de) SHA1(e41e3d90dac738cf71377f3b476ec67b14dee27a) )
	ROM_LOAD( "superg.w",     0x1000, 0x0800, CRC(ddeabdae) SHA1(daa5109a32c7c9a80bdb212dc3e4e3e3c104a731) )
	ROM_LOAD( "superg.y",     0x1800, 0x0800, CRC(9463f753) SHA1(d9cb35c19aafec43d08b048bbe2337a790f6ba9d) )
	ROM_LOAD( "supergs.z",    0x2000, 0x0800, CRC(95e9005c) SHA1(cb4c556f00ec2ae9876d235224a7eb7816c06f60) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galmidw.1j",   0x0000, 0x0800, CRC(84decf98) SHA1(2e565cb6057b1816a6b4541e6dfadd3c3762fa36) )
	ROM_LOAD( "galmidw.1k",   0x0800, 0x0800, CRC(c31ada9e) SHA1(237ebb48549b34ca59a13cc2706512d957413ec4) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( vectrgal )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "galnamco.7f",  0x0000, 0x1000, CRC(b10533fd) SHA1(ea8653e80000d607cd37a41f2de27ffe6e190f0f) )
	ROM_LOAD( "galnamco.7j",  0x1000, 0x1000, CRC(9163454d) SHA1(683ce43c0e24fd5110b860e424e5de30d59e5223) )
	ROM_LOAD( "galnamco.7l",  0x2000, 0x1000, CRC(952c6135) SHA1(e0989d5dce37ae421c34833d48b09e76d178680d) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vecgal.1h",    0x0000, 0x0800, CRC(02815aec) SHA1(94975ee5eff84a3ae483dd3d5766384859402477) )
	ROM_LOAD( "vecgal.1k",    0x0800, 0x0800, CRC(11c9898c) SHA1(fb6bac221b3984a70a775bccf06f6e2f17d447cf) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( fourplay )
	ROM_REGION( 0x20000, "maincpu", 0 )   /* 64k for code & 64k for banked code */
	ROM_LOAD( "fourplay.bin", 0x10000, 0x10000, CRC(b42b2c2c) SHA1(f3b6f136b39e7a6adadecb9acf906fcfe649f398) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "graph1.bin",   0x0000, 0x1000, CRC(359c0c1f) SHA1(9e39baae4ab5763db236d0a49a6665d2e69cba11) )
	ROM_LOAD( "graph2.bin",   0x1000, 0x1000, CRC(b22a51c9) SHA1(cccfbab0c92dba81e3451de72d1f1899226e30e2) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( videight )
	ROM_REGION( 0x30000, "maincpu", 0 )
	ROM_LOAD( "videight.bin", 0x10000, 0x20000, CRC(0601db09) SHA1(0c9cd1afb0034946261219bf42b2f9b1ed5bdb17) )

	ROM_REGION( 0x10000, "gfx1", 0 )
	ROM_LOAD( "v8g1.bin",     0x0000, 0x8000, CRC(af771e33) SHA1(cdc960c5f548e19da4eabdaf8b789ca8ffb6a29f) )
	ROM_LOAD( "v8g2.bin",     0x8000, 0x8000, CRC(776c34e6) SHA1(de70a29f73469a58ab15acb95aaad1efeb4de08d) )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "v8c.bin",      0x0000, 0x0100, CRC(b35a6ca8) SHA1(d9195215bf2482a1b02a019bb708a9981eb0bdf8) )
ROM_END

ROM_START( offender )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "off-2d",       0x0000, 0x0800, CRC(bb09d91e) SHA1(f47cd3fd91fc569c7a9bb6e485a3281b92656b7f) )
	ROM_LOAD( "2e",           0x0800, 0x0800, CRC(e9b4b9eb) SHA1(a8ee9ddfadf5e9accedfaf81da757a88a2e55a0a) )
	ROM_LOAD( "2f",           0x1000, 0x0800, CRC(a1f14f4c) SHA1(3eae2b3e4596505a8afb5c5cfb108e823c2c4319) )
	ROM_LOAD( "2h",           0x1800, 0x0800, CRC(591bc0d9) SHA1(170f9e92f0a3bee04407be27210b4fa825367688) )
	ROM_LOAD( "2j",           0x2000, 0x0800, CRC(22f11b6b) SHA1(e426ef6a7444a39a34d59799973b07d11b89f372) )
	ROM_LOAD( "2l",           0x2800, 0x0800, CRC(705ffe49) SHA1(174df3f281068c767344f751daace646360e26d6) )
	ROM_LOAD( "2m",           0x3000, 0x0800, CRC(ea26c35c) SHA1(a2f3380982d93a022f46756f974fd16c4cd617de) )
	ROM_LOAD( "2p",           0x3800, 0x0800, CRC(94d8f5e3) SHA1(f3a9c4d1d91836476fcad87ea0d243dde7171e0a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "ot1.5c",       0x0000, 0x0800, CRC(bcd297f0) SHA1(8ed78487d76fd0a917ab7b258937a46e2cd9800c) )
	ROM_LOAD( "ot2.5d",       0x0800, 0x0800, CRC(de7912da) SHA1(8558b4eff5d7e63029b325edef9914feda5834c3) )
	ROM_LOAD( "ot3.5e",       0x1000, 0x0800, CRC(ba2fa933) SHA1(1f976d8595706730e29f93027e7ab4620075c078) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "off-5f",       0x0000, 0x0800, CRC(7015a2e1) SHA1(19bb4cb26c8b91cd79368e4ed48065df6fd6577e) )
	ROM_LOAD( "off-5h",       0x0800, 0x0800, CRC(a36ac0fa) SHA1(b58412536833ab5ea705e919562b7b495ff3dff4) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "c01s.6e",      0x0000, 0x0020, CRC(4e3caeab) SHA1(a25083c3e36d28afdefe4af6e6d4f3155e303625) )
ROM_END

ROM_START( superdog )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "sdog-2d",      0x0000, 0x0800, CRC(99dc8bb5) SHA1(a7f533471df4482f67be0ed969e559f483fb147a) )
	ROM_LOAD( "2e",           0x0800, 0x0800, CRC(e9b4b9eb) SHA1(a8ee9ddfadf5e9accedfaf81da757a88a2e55a0a) )
	ROM_LOAD( "2f",           0x1000, 0x0800, CRC(a1f14f4c) SHA1(3eae2b3e4596505a8afb5c5cfb108e823c2c4319) )
	ROM_LOAD( "2h",           0x1800, 0x0800, CRC(591bc0d9) SHA1(170f9e92f0a3bee04407be27210b4fa825367688) )
	ROM_LOAD( "2j",           0x2000, 0x0800, CRC(22f11b6b) SHA1(e426ef6a7444a39a34d59799973b07d11b89f372) )
	ROM_LOAD( "2l",           0x2800, 0x0800, CRC(705ffe49) SHA1(174df3f281068c767344f751daace646360e26d6) )
	ROM_LOAD( "2m",           0x3000, 0x0800, CRC(ea26c35c) SHA1(a2f3380982d93a022f46756f974fd16c4cd617de) )
	ROM_LOAD( "2p",           0x3800, 0x0800, CRC(94d8f5e3) SHA1(f3a9c4d1d91836476fcad87ea0d243dde7171e0a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "ot1.5c",       0x0000, 0x0800, CRC(bcd297f0) SHA1(8ed78487d76fd0a917ab7b258937a46e2cd9800c) )
	ROM_LOAD( "ot2.5d",       0x0800, 0x0800, CRC(de7912da) SHA1(8558b4eff5d7e63029b325edef9914feda5834c3) )
	ROM_LOAD( "ot3.5e",       0x1000, 0x0800, CRC(ba2fa933) SHA1(1f976d8595706730e29f93027e7ab4620075c078) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "sdog-5f",      0x0000, 0x0800, CRC(7edbc59c) SHA1(53c9a7f46a968b2f804243f661479918870756b5) )
	ROM_LOAD( "sdog-5h",      0x0800, 0x0800, CRC(5b130bea) SHA1(269df1344414c0d517084a32a348a856c25d499c) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "c01s.6e",      0x0000, 0x0020, CRC(4e3caeab) SHA1(a25083c3e36d28afdefe4af6e6d4f3155e303625) )
ROM_END

ROM_START( amidarf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "amidar.2c",    0x0000, 0x1000, CRC(c294bf27) SHA1(399325bf1559e8cdbddf7cfbf0dc739f9ed72ef0) )
	ROM_LOAD( "amidar.2e",    0x1000, 0x1000, CRC(e6e96826) SHA1(e9c4f8c594640424b456505e676352a98b758c03) )
	ROM_LOAD( "amidarf.2f",   0x2000, 0x1000, CRC(45b24635) SHA1(0a6b004803e67ed536b3213cc58ae976e4dc24be) )
	ROM_LOAD( "amidar.2h",    0x3000, 0x1000, CRC(1be170bd) SHA1(c047bc393b297c0d47668a5f6f4870e3fac937ef) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "amidar.5c",    0x0000, 0x1000, CRC(c4b66ae4) SHA1(9d09dbde4019f7be3abe0815b0e06d542c01c255) )
	ROM_LOAD( "amidar.5d",    0x1000, 0x1000, CRC(806785af) SHA1(c8c85e3a6a204feccd7859b4527bd649e96134b4) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "amidar.5f",    0x0000, 0x0800, CRC(5e51e84d) SHA1(dfe84db7e2b1a45a1d484fcf37291f536bc5324c) )
	ROM_LOAD( "amidar.5h",    0x0800, 0x0800, CRC(2f7f1c30) SHA1(83c330eca20dfcc6a4099001943b9ed7a7c3db5b) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "amidar.clr",   0x0000, 0x0020, CRC(f940dcc3) SHA1(1015e56f37c244a850a8f4bf0e36668f047fd46d) )
ROM_END

ROM_START( amidargr )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "amidarg.2c",   0x0000, 0x1000, CRC(e9822709) SHA1(ce7c76607a512b9b092a8bf83bdf6744a92a519a) )
	ROM_LOAD( "amidar.2e",    0x1000, 0x1000, CRC(e6e96826) SHA1(e9c4f8c594640424b456505e676352a98b758c03) )
	ROM_LOAD( "amidarg.2f",   0x2000, 0x1000, CRC(fd81b040) SHA1(f710cb192a50514f55b4aec859fd9c298b0b90f8) )
	ROM_LOAD( "amidar.2h",    0x3000, 0x1000, CRC(1be170bd) SHA1(c047bc393b297c0d47668a5f6f4870e3fac937ef) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "amidar.5c",    0x0000, 0x1000, CRC(c4b66ae4) SHA1(9d09dbde4019f7be3abe0815b0e06d542c01c255) )
	ROM_LOAD( "amidar.5d",    0x1000, 0x1000, CRC(806785af) SHA1(c8c85e3a6a204feccd7859b4527bd649e96134b4) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "amidar.5f",    0x0000, 0x0800, CRC(5e51e84d) SHA1(dfe84db7e2b1a45a1d484fcf37291f536bc5324c) )
	ROM_LOAD( "amidarg.5h",   0x0800, 0x0800, CRC(141f8dbe) SHA1(3a627f5dd072909177f50920271f0d8ffb8a09c0) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "amidar.clr",   0x0000, 0x0020, CRC(f940dcc3) SHA1(1015e56f37c244a850a8f4bf0e36668f047fd46d) )
ROM_END

#define rom_jumpbugx rom_jumpbug


ROM_START( kazzy )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "frogger.26",   0x0000, 0x1000, CRC(597696d6) SHA1(e7e021776cad00f095a1ebbef407b7c0a8f5d835) )
	ROM_LOAD( "frogger.27",   0x1000, 0x1000, CRC(b6e6fcc3) SHA1(5e8692f2b0c7f4b3642b3ee6670e1c3b20029cdc) )
	ROM_LOAD( "frsm3.7",      0x2000, 0x1000, CRC(aca22ae0) SHA1(5a99060ea2506a3ac7d61ca5876ce5cb3e493565) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "frogger.608",  0x0000, 0x0800, CRC(e8ab0256) SHA1(f090afcfacf5f13cdfa0dfda8e3feb868c6ce8bc) )
	ROM_LOAD( "frogger.609",  0x0800, 0x0800, CRC(7380a48f) SHA1(75582a94b696062cbdb66a4c5cf0bc0bb94f81ee) )
	ROM_LOAD( "frogger.610",  0x1000, 0x0800, CRC(31d7eb27) SHA1(2e1d34ae4da385fd7cac94707d25eeddf4604e1a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "kazzy.607",    0x0000, 0x0800, CRC(86f041e6) SHA1(61dd02bb2dda15d2f47ecc878a9dd6aa3118687d) )
	ROM_LOAD( "kazzy.606",    0x0800, 0x0800, CRC(9ea43774) SHA1(d96eae151580f6ccef78049a73ec1fc5fa90aa63) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "pr-91.6l",     0x0000, 0x0020, CRC(413703bf) SHA1(66648b2b28d3dcbda5bdb2605d1977428939dd3c) )
ROM_END

ROM_START( trukker )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "frogger.ic5",  0x0000, 0x1000, CRC(efab0c79) SHA1(68c99b6cdcb9396bb473739a62ffc009b4bf57d5) )
	ROM_LOAD( "frogger.ic6",  0x1000, 0x1000, CRC(aeca9c13) SHA1(cdf560adbd7f2813e86e378da7781cccf7928a44) )
	ROM_LOAD( "frogger.ic7",  0x2000, 0x1000, CRC(dd251066) SHA1(4612e1fe1ab7182a277140b1a1976cc17e0746a5) )
	ROM_LOAD( "frogger.ic8",  0x3000, 0x1000, CRC(bf293a02) SHA1(be94e9f5caa74c3de6fd95bd20928f4a9c514227) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "frogger.608",  0x0000, 0x0800, CRC(e8ab0256) SHA1(f090afcfacf5f13cdfa0dfda8e3feb868c6ce8bc) )
	ROM_LOAD( "frogger.609",  0x0800, 0x0800, CRC(7380a48f) SHA1(75582a94b696062cbdb66a4c5cf0bc0bb94f81ee) )
	ROM_LOAD( "frogger.610",  0x1000, 0x0800, CRC(31d7eb27) SHA1(2e1d34ae4da385fd7cac94707d25eeddf4604e1a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "trukker.606",  0x0000, 0x0800, CRC(f0c49466) SHA1(f88d02ada32a9f1e10004785d17659279fd1d651) )
	ROM_LOAD( "trukker.607",  0x0800, 0x0800, CRC(6ab5c2eb) SHA1(d13fcd5669bd0f8f76505684ba5e788b395aeef1) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "pr-91.6l",     0x0000, 0x0020, CRC(413703bf) SHA1(66648b2b28d3dcbda5bdb2605d1977428939dd3c) )
ROM_END

ROM_START( tst_frog )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "frogtest.bin", 0x0000, 0x0226, CRC(1f5a7f35) SHA1(608ad6ac8c34aea66b13aac7e45c298a2083d642) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "frogger.608",  0x0000, 0x0800, CRC(e8ab0256) SHA1(f090afcfacf5f13cdfa0dfda8e3feb868c6ce8bc) )
	ROM_LOAD( "frogger.609",  0x0800, 0x0800, CRC(7380a48f) SHA1(75582a94b696062cbdb66a4c5cf0bc0bb94f81ee) )
	ROM_LOAD( "frogger.610",  0x1000, 0x0800, CRC(31d7eb27) SHA1(2e1d34ae4da385fd7cac94707d25eeddf4604e1a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "frogger.607",  0x0000, 0x0800, CRC(05f7d883) SHA1(78831fd287da18928651a8adb7e578d291493eff) )
	ROM_LOAD( "frogger.606",  0x0800, 0x0800, CRC(f524ee30) SHA1(dd768967add61467baa08d5929001f157d6cd911) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "pr-91.6l",     0x0000, 0x0020, CRC(413703bf) SHA1(66648b2b28d3dcbda5bdb2605d1977428939dd3c) )
ROM_END

/* Wriggly Beasties - bootleg of WarOfBug */
ROM_START( wbeast )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "wbeast.7f",    0x0000, 0x1000, CRC(967e92ee) SHA1(319953fda4bedd447e95f93cd800fc036ce6a8e4) )
	ROM_LOAD( "wbeast.7j",    0x1000, 0x1000, CRC(72c70fff) SHA1(e0e2cfafdce4d027f89a32542cdc2d724da9f6b0) )
	ROM_LOAD( "wbeast.7l",    0x2000, 0x1000, CRC(2486d3a6) SHA1(93ac6178303bf65175b8b0b889c1dae11da46d0e) )

	/* These 2 roms are overdumped */
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "wbeast.1h",    0x0000, 0x1000, CRC(623bb742) SHA1(081cc0cc2f3133338495e61a8e904448521c6c9a) )
	ROM_LOAD( "wbeast.1k",    0x1000, 0x1000, CRC(c1b1143f) SHA1(72436840ff72f7f6341b11c43e25107ecb3ebb73) )

	/* No prom supplied - using the one from warofbug */
	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "warofbug.clr", 0x0000, 0x0020, CRC(8688e64b) SHA1(ed13414257f580b98b50c9892a14159c55e7838d) )
ROM_END

/* This is sort of playable - no sound, wrong colours. Reboots after defeating the boss. */
ROM_START( monstrz )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "b-1e.a1",      0x0000, 0x1000, CRC(97886542) SHA1(01f4f9bd55f9eae28162cbb22a26f7cda22cd3f3) )
	ROM_LOAD( "b-2e.c1",      0x1000, 0x1000, CRC(184ffcb4) SHA1(829d6ca13773aba7c3a81e122171befbe3666110) )
	ROM_LOAD( "b-3e.d1",      0x2000, 0x1000, CRC(b7b10ac7) SHA1(51d544d4db456df756a95d7f1853fffed9259647) )
	ROM_LOAD( "b-4e.e1",      0x3000, 0x1000, CRC(fb02c736) SHA1(24466116dd07b856b1afff62b8312c67ff466b95) )
	ROM_LOAD( "b-5e.g1",      0xc000, 0x1000, CRC(b2788ab9) SHA1(eb1a6b41f4c7a243481bfccf2b068ce1bc292366) )
	ROM_LOAD( "b-6e.h1",      0xd000, 0x1000, CRC(77d7aa8d) SHA1(62aaf582ba55f7b21f6cf13b4fb6c2c54bb729f5) )
	// protection
	ROM_FILL(0xc5d8,1,0)
	ROM_FILL(0xc5fc,1,0)
	ROM_FILL(0xc624,1,0)
	ROM_FILL(0xc63c,1,0)
	// fill the protection ram with RET so we don't run into the weeds.
	ROM_FILL(0x3800,0x800,0xc9)
	// enable attract mode
	ROM_FILL(0x3918, 1, 0xC5)
	ROM_FILL(0x3919, 1, 0xE5)
	ROM_FILL(0x391A, 1, 0xF5)
	ROM_FILL(0x391B, 1, 0x21)
	ROM_FILL(0x391C, 1, 0x10)
	ROM_FILL(0x391D, 1, 0x41)
	ROM_FILL(0x391E, 1, 0xAF)
	ROM_FILL(0x391F, 1, 0x06)
	ROM_FILL(0x3920, 1, 0x08)
	ROM_FILL(0x3921, 1, 0xAE)
	ROM_FILL(0x3922, 1, 0x23)
	ROM_FILL(0x3923, 1, 0x10)
	ROM_FILL(0x3924, 1, 0xFC)
	ROM_FILL(0x3925, 1, 0xFE)
	ROM_FILL(0x3926, 1, 0x40)
	ROM_FILL(0x3927, 1, 0x20)
	ROM_FILL(0x3928, 1, 0x03)
	ROM_FILL(0x3929, 1, 0x32)
	ROM_FILL(0x392A, 1, 0x12)
	ROM_FILL(0x392B, 1, 0x41)
	ROM_FILL(0x392C, 1, 0xF1)
	ROM_FILL(0x392D, 1, 0xE1)
	ROM_FILL(0x392E, 1, 0xC1)

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "a-1e.k1",      0x0000, 0x1000, CRC(b88ba44e) SHA1(85c141fb411d541b1e20412f5fefd18395f635ae) )
	ROM_LOAD( "a-2.k2",       0x1000, 0x1000, CRC(8913c94e) SHA1(6c4fe065217a234d45761f8ad4d2c4e7078a0abd) )
	ROM_LOAD( "a-3e.k3",      0x2000, 0x1000, CRC(a8fa5095) SHA1(5cabe5497a79a0c43e78a84ae87c824af60a2a3f) )
	ROM_LOAD( "a-4.k4",       0x3000, 0x1000, CRC(93f81317) SHA1(167708be94cb9a47290067a20bc5ff6f018b93b6) )

	ROM_REGION( 0x10000, "audio2", 0 )
	ROM_LOAD( "a-5e.k5",      0x0000, 0x1000, CRC(b5bcdb4e) SHA1(db0965e5636e0f4e9cd4f4a7d808c413ecf733db) )
	ROM_LOAD( "a-6.k6",       0x1000, 0x1000, CRC(24832b2e) SHA1(2a67888e86ce1a3182303e841513ba2a07977359) )
	ROM_LOAD( "a-7e.k7",      0x2000, 0x1000, CRC(20ebea81) SHA1(473c688365b256d8593663ff95768f4a5bb1289d) )
	// 0x3000 empty ?
	ROM_LOAD( "a-8.k8",       0x4000, 0x1000, CRC(b833a15b) SHA1(0d21aaa0ca5ccba89118b205a6b3b36b15663c47) )
	ROM_LOAD( "a-9.k9",       0x5000, 0x1000, CRC(cbd76ec2) SHA1(9434350ee93ca71efe78018b69913386353306ff) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "b-7e.a5",      0x0000, 0x1000, CRC(ddd4158d) SHA1(9701e2d8a0226455dfbed650e58bb4be05918fe8) )
	ROM_LOAD( "b-8e.c5",      0x1000, 0x1000, CRC(b1331b4c) SHA1(fa1af406ecd6919b4846aea68d3edb70106f9273) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "prom.g9",      0x0000, 0x0020, CRC(b7ea00d7) SHA1(f658c6ac8123ae1e6b68ae513cc02c4d9d2b4e47) )
ROM_END


/***************************************************************************

  Game driver(s)

***************************************************************************/


//    Year  romname   parent    machine   inputs    init      rot

/* Frogger */
GAME( 1981, kazzy,    frogger,  frogger,  frogger,  galaxian_state, frogger,  ROT90, "Kazzy", "K-azzy", MACHINE_SUPPORTS_SAVE )
GAME( 1999, trukker,  frogger,  frogger,  frogger,  galaxian_state, trukker,  ROT90, "Grimlick","Trukker", MACHINE_SUPPORTS_SAVE )
GAME( 2004, tst_frog, frogger,  tst_frog, frogger,  galaxian_state, frogger,  ROT90, "msmcdoug", "Test Rom - Frogger Sound Test", MACHINE_SUPPORTS_SAVE )


/* Amidar */
GAME( 2007, amidarf,  amidar,   turtles,  amidar,   galaxian_state, turtles,  ROT90, "Arkatrad", "Amidar (French)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, amidargr, amidar,   turtles,  amidar,   galaxian_state, turtles,  ROT90, "GreekRoms", "Amidar (Greek)", MACHINE_SUPPORTS_SAVE )


/* Scramble */
GAME( 1998, offender, scramble, scramble, scramble, galaxian_state, scramble, ROT90, "Chris Henry", "Offender", MACHINE_SUPPORTS_SAVE )
GAME( 1998, superdog, scramble, scramble, scramble, galaxian_state, scramble, ROT90, "Jerky", "Superdog", MACHINE_SUPPORTS_SAVE )


/* Galaxian */
GAME( 1979, andromd,  galnamco, galaxian, superg,   galaxian_state, galaxian, ROT90, "hack",  "Andromeda", MACHINE_SUPPORTS_SAVE)
GAME( 1998, buglaxn,  galnamco, galaxian, galaxian, galaxian_state, galaxian, ROT90, "The Dog", "Galaxian (Bug sprites)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galaxiab, galnamco, galaxian, galaxiab, galaxian_state, galaxian, ROT90, "bootleg", "Galaxian", MACHINE_SUPPORTS_SAVE )
GAME( 1979, galaxkyo, galnamco, galaxian, superg,   galaxian_state, galaxian, ROT90, "hack", "Galaxian (Kyoko)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, galaxni,  galnamco, galaxian, superg,   galaxian_state, galaxian, ROT90, "Petaco S.A.", "Galaxian (New Invasion)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, galinvad, galnamco, galaxian, galaxian, galaxian_state, galaxian, ROT90, "T-Bone", "Galaxian (Space Invaders sprites)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, galnamco, 0,        galaxian, galaxian, galaxian_state, galaxian, ROT90, "Namco", "Galaxians", MACHINE_SUPPORTS_SAVE )
GAME( 1998, gaylaxn,  galnamco, galaxian, superg,   galaxian_state, galaxian, ROT90, "The Dog", "Gaylaxian", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mrdonm,   0,        galaxian, mrdonm,   galaxian_state, galaxian, ROT90, "Krazy Ivan", "Mr. Do Nightmare", MACHINE_SUPPORTS_SAVE )
GAME( 1979, supergs,  galnamco, galaxian, superg,   galaxian_state, galaxian, ROT90, "Midway", "Super Galaxians (Silver Systems)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, vectrgal, galnamco, galaxian, galaxian, galaxian_state, galaxian, ROT90, "T-Bone", "Galaxian (Vector Sim)", MACHINE_SUPPORTS_SAVE )


/* Moon Cresta */
GAME( 2000, mooncmst, mooncrst, mooncrst, mooncrst, galaxian_state, mooncrst, ROT90, "Kazzy", "Moon Creamsta", MACHINE_SUPPORTS_SAVE )
GAME( 1980, mooncrs5, mooncrst, mooncrst, mooncrst, galaxian_state, mooncrsu, ROT90, "bootleg", "Moon Cresta", MACHINE_SUPPORTS_SAVE )


/* Multigames */
GAME( 2002, fourplay, 0,        fourplay, galaxian, galaxian_state, fourplay, ROT90, "Macro", "Four Play", MACHINE_SUPPORTS_SAVE )
GAME( 2001, videight, 0,        videight, warofbug, galaxian_state, videight, ROT90, "Macro", "Video Eight", MACHINE_SUPPORTS_SAVE )


/* Other */
GAME( 1981, jumpbugx, 0,        jumpbugx, jumpbug,  galaxian_state, jumpbug,  ROT90, "Rock-ola", "Jump Bug (Extra Sounds)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, monstrz,  0,        sfx,      sfx,      galaxian_state, sfx,      ORIENTATION_FLIP_X, "Nihon Game Co", "Monster Zero", MACHINE_UNEMULATED_PROTECTION | MACHINE_NO_SOUND | MACHINE_WRONG_COLORS | MACHINE_SUPPORTS_SAVE )
GAME( 19??, starfgh2, pisces,   galaxian, piscesb,  galaxian_state, pisces,   ROT90, "bootleg", "Starfighter II", MACHINE_SUPPORTS_SAVE )
GAME( 1981, wbeast,   0,        galaxian, warofbug, galaxian_state, nolock,   ROT90, "Compost", "Wriggly Beasties", MACHINE_SUPPORTS_SAVE )
