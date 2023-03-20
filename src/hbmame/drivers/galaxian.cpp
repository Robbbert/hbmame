// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/galaxian.cpp"


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

void galaxian_hbmame::jumpbugx_map(address_map &map)
{
	jumpbug_map(map);
/* HBMAME - added next lines */
	map(0x6800,0x6807).mirror(0x07f8).w("cust",FUNC(galaxian_sound_device::sound_w));
	map(0x7800,0x7800).mirror(0x07ff).w("cust",FUNC(galaxian_sound_device::pitch_w));
}

void galaxian_hbmame::jumpbugx(machine_config &config)
{
	jumpbug(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &galaxian_hbmame::jumpbugx_map);
	GALAXIAN_SOUND(config, "cust", 0);
}


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

void galaxian_hbmame::init_trukker()
{
	uint16_t i;

	/* video extensions */
	common_init(NULL, &galaxian_hbmame::frogger_draw_background, &galaxian_hbmame::frogger_extend_tile_info, &galaxian_hbmame::frogger_extend_sprite_info);
	m_frogger_adjust = true;

	/* the first ROM of the second CPU has data lines D0 and D1 swapped. Decode it. */
	uint8_t *ROM = memregion("audiocpu")->base();
	for (i = 0;i < 0x800;i++) ROM[i] = bitswap<8>(ROM[i],7,6,5,4,3,2,0,1);

	/* fix gfx error in the alphabetic characters */
	ROM = memregion("gfx1")->base();
	for (i = 0;i < 0x160;i++) ROM[i] = bitswap<8>(ROM[i],7,6,5,4,3,2,0,1);

	/* remove extraneous dot from steam rollers */
	for (i = 0x1a0;i < 0x200;i++) ROM[i] = ROM[i] & 0xfc;

	/* fix gfx error in sinking turtle */
	for (i = 0x4c0;i < 0x4e0;i++) ROM[i] = bitswap<8>(ROM[i],7,6,5,4,3,2,0,1);

	/* fix gfx error on left side of home bases */
	ROM[0x234] = bitswap<8>(ROM[0x234],7,6,5,4,3,2,0,1);

	/* fix gfx error in (c) sign and in square symbol */
	for (i = 0x270;i < 0x27f;i++) ROM[i] = bitswap<8>(ROM[i],7,6,5,4,3,2,0,1);

	/* fix gfx error in 200-point sign */
	for (i = 0x330;i < 0x337;i++) ROM[i] = bitswap<8>(ROM[i],7,6,5,4,3,2,0,1);
}



/***************************************************************
 *
 *  Frogger Sound Test rom - watchdog has been disabled.
 *  You can lengthen the time, but holding down an arrow key
 *  will still eventually trigger it.
 *
 ***************************************************************/

void galaxian_hbmame::tst_frog_map(address_map &map) {
	map.unmap_value_high();
	map(0x0000,0x3fff).rom();
	map(0x8000,0x87ff).ram();
	map(0x8800,0x8800).nopr();  //r("watchdog", FUNC(watchdog_timer_device::reset_r));
	map(0xa800,0xabff).mirror(0x0400).ram().w(FUNC(galaxian_hbmame::galaxian_videoram_w)).share("videoram");
	map(0xb000,0xb0ff).mirror(0x0700).ram().w(FUNC(galaxian_hbmame::galaxian_objram_w)).share("spriteram");
	map(0xb808,0xb808).mirror(0x07e3).w(FUNC(galaxian_hbmame::irq_enable_w));
	map(0xb80c,0xb80c).mirror(0x07e3).w(FUNC(galaxian_hbmame::galaxian_flip_screen_y_w));
	map(0xb810,0xb810).mirror(0x07e3).w(FUNC(galaxian_hbmame::galaxian_flip_screen_x_w));
	map(0xb818,0xb818).mirror(0x07e3).w(FUNC(galaxian_hbmame::coin_count_0_w));  /* IOPC7 */
	map(0xb81c,0xb81c).mirror(0x07e3).w(FUNC(galaxian_hbmame::coin_count_1_w));  /* POUT1 */
	map(0xc000,0xffff).rw(FUNC(galaxian_hbmame::frogger_ppi8255_r),FUNC(galaxian_hbmame::frogger_ppi8255_w));
}

void galaxian_hbmame::tst_frog(machine_config &config)
{
	frogger(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &galaxian_hbmame::tst_frog_map);
	config.device_remove("watchdog");
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

ROM_START( galaxiabh )
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
	ROM_FILL( 31,1,7 ) /* patch so floating scores can be seen, and explosions look better */
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

ROM_START( galxtest1 )
	ROM_REGION( 0x4000, "maincpu", 0 )
	ROM_LOAD( "test.u",       0x0000, 0x0800, CRC(0614cd7f) SHA1(12440678be8a27a6c3032b6e43c45e27905ffa83) )   // Test ROM

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "1h.bin",       0x0000, 0x0800, CRC(39fb43a4) SHA1(4755609bd974976f04855d51e08ec0d62ab4bc07) )
	ROM_LOAD( "1k.bin",       0x0800, 0x0800, CRC(7e3f56a2) SHA1(a9795d8b7388f404f3b0e2c6ce15d713a4c5bafa) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

ROM_START( galxtest2 )
	ROM_REGION( 0x4000, "maincpu", 0 )
	ROM_LOAD( "galxtest2.u",  0x0000, 0x0400, CRC(bdf2a530) SHA1(a60ab7c232d3552427744a36ea93ef597f0383f8) )   // Test ROM

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "1h.bin",       0x0000, 0x0800, CRC(39fb43a4) SHA1(4755609bd974976f04855d51e08ec0d62ab4bc07) )
	ROM_LOAD( "1k.bin",       0x0800, 0x0800, CRC(7e3f56a2) SHA1(a9795d8b7388f404f3b0e2c6ce15d713a4c5bafa) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

#if 0
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
#endif

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
	ROM_LOAD( "mmi6331.6l",   0x0000, 0x0020, CRC(6a0c7d87) SHA1(140335d85c67c75b65689d4e76d29863c209cf32) )
ROM_END


/* E78 - The dump was missing one rom (position "fake" below), I found that the equivalent code from fantazia
        works perfectly well. The rom still fails the checksum test, which is why the test is patched. So
        take note: It is a FAKE rom!!!

        The colour prom supplied produced terrible colours, with most enemies barely visible. Substituting
        the normal mooncrst prom fixes all the problems. */

ROM_START( mooncrs5f )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mooncrs5.7k", 0x0000, 0x1000, CRC(d62875a5) SHA1(2e162bf559a72acb5d23434497a9377a302b0440) )
	ROM_LOAD( "mooncrs5.7j", 0x1000, 0x1000, CRC(1683e74c) SHA1(5887ddeed7d3399e93bc3146f54c23e9f8893866) )
	ROM_LOAD( "mooncrs5.7h", 0x2000, 0x1000, CRC(46921e92) SHA1(7a3cec261a9e0baf969bd356a0766b4b266eaa39) )
	ROM_LOAD( "mooncrs5.7a", 0x3000, 0x0400, CRC(5318be0c) SHA1(cca73501da8783e47f3f3c7d8a795011d5fd91a3) )
	ROM_LOAD( "mooncrs5.7b", 0x3400, 0x0400, CRC(aed8acb2) SHA1(a2429daba96e6c064f4bc65cab2df6b15a43bf44) )
	ROM_LOAD( "mooncrs5.5f", 0x3800, 0x0400, CRC(76db94a5) SHA1(f7d9c81d5e09f7e42258caf265606b1ba32439dd) )
	ROM_LOAD( "mooncrs5.7d", 0x3c00, 0x0400, CRC(db0528cf) SHA1(be06b78d890e07fcbbfd6931d1a59ab0fb8b7a53) )
	ROM_FILL(0x3FF4,1,0x2c ) /* patch unused byte so checksum test can be passed */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mooncrs5.1h", 0x0800, 0x0800, CRC(60d783bc) SHA1(584a0600ef1c47fb8bce86f738af7691fdf91c9c) )
	ROM_CONTINUE(            0x0000, 0x0800 )
	ROM_LOAD( "mooncrs5.1k", 0x1800, 0x0800, CRC(bc25be88) SHA1(0e698a1e573f28e8aad07e79c11f38519d93286c) )
	ROM_CONTINUE(            0x1000, 0x0800 )

	ROM_REGION( 0x0020, "proms", 0 )
//  ROM_LOAD( "6l.bpr",      0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
	ROM_LOAD( "mmi6331.6l",  0x0000, 0x0020, CRC(6a0c7d87) SHA1(140335d85c67c75b65689d4e76d29863c209cf32) )
ROM_END

ROM_START( moontest )
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "moontest.7k",  0x0000, 0x0400, CRC(caf5eb02) SHA1(3ba9df1e8f3fa548a68f1f596f744c188a1ea48a) )
	ROM_LOAD( "mc2",          0x0800, 0x0800, CRC(44bb7cfa) SHA1(349c2e23a9fce73f95bb8168d369082fa129fe3d) )
	ROM_LOAD( "mc3",          0x1000, 0x0800, CRC(9c412104) SHA1(1b40054ebb1ace965a8522119bb23f09797bc5f6) )
	ROM_LOAD( "mc4",          0x1800, 0x0800, CRC(7e9b1ab5) SHA1(435f603c0c3e788a509dd144a7916a34e791ae44) )
	ROM_LOAD( "mc5.7r",       0x2000, 0x0800, CRC(16c759af) SHA1(3b48050411f65f9d3fb41ff22901e22d82bf1cf6) )
	ROM_LOAD( "mc6.8d",       0x2800, 0x0800, CRC(69bcafdb) SHA1(939c8c6ed1cd4660a3d99b8f17ed99cbd7e1352a) )
	ROM_LOAD( "mc7.8e",       0x3000, 0x0800, CRC(b50dbc46) SHA1(4fa084fd1ba5f78e7703e684c57af15ca7a844e4) )
	ROM_LOAD( "mc8",          0x3800, 0x0800, CRC(18ca312b) SHA1(39219059003b949e38305553fea2d33071062c64) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mcs_b",        0x0000, 0x0800, CRC(fb0f1f81) SHA1(38a6679a8b69bc1870a0e67e692131c42f9535c8) )
	ROM_LOAD( "mcs_d",        0x0800, 0x0800, CRC(13932a15) SHA1(b8885c555c6ad7021be55c6925a0a0872c1b6abd) )
	ROM_LOAD( "mcs_a",        0x1000, 0x0800, CRC(631ebb5a) SHA1(5bc9493afa76c55858b8c8849524cbc77dc838fc) )
	ROM_LOAD( "mcs_c",        0x1800, 0x0800, CRC(24cfd145) SHA1(08c6599db170dd6ee364c44f70a0f5c0f881b6ef) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "mmi6331.6l",   0x0000, 0x0020, CRC(6a0c7d87) SHA1(140335d85c67c75b65689d4e76d29863c209cf32) )
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

ROM_START( offender )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "off-2d",       0x0000, 0x0800, CRC(bb09d91e) SHA1(f47cd3fd91fc569c7a9bb6e485a3281b92656b7f) )
	ROM_LOAD( "2e",           0x0800, 0x0800, CRC(e9b4b9eb) SHA1(a8ee9ddfadf5e9accedfaf81da757a88a2e55a0a) )
	ROM_LOAD( "2f",           0x1000, 0x0800, CRC(a1f14f4c) SHA1(3eae2b3e4596505a8afb5c5cfb108e823c2c4319) )
	ROM_LOAD( "2h",           0x1800, 0x0800, CRC(591bc0d9) SHA1(170f9e92f0a3bee04407be27210b4fa825367688) )
	ROM_LOAD( "2j",           0x2000, 0x0800, CRC(22f11b6b) SHA1(e426ef6a7444a39a34d59799973b07d11b89f372) )
	ROM_LOAD( "2l",           0x2800, 0x0800, CRC(705ffe49) SHA1(174df3f281068c767344f751daace646360e26d6) )
	ROM_LOAD( "2m",           0x3000, 0x0800, CRC(ea26c35c) SHA1(a2f3380982d93a022f46756f974fd16c4cd617de) )
	ROM_LOAD( "2p",           0x3800, 0x0800, CRC(94d8f5e3) SHA1(f3a9c4d1d91836476fcad87ea0d243dde7171e0a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
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
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "sdog-2d",      0x0000, 0x0800, CRC(99dc8bb5) SHA1(a7f533471df4482f67be0ed969e559f483fb147a) )
	ROM_LOAD( "2e",           0x0800, 0x0800, CRC(e9b4b9eb) SHA1(a8ee9ddfadf5e9accedfaf81da757a88a2e55a0a) )
	ROM_LOAD( "2f",           0x1000, 0x0800, CRC(a1f14f4c) SHA1(3eae2b3e4596505a8afb5c5cfb108e823c2c4319) )
	ROM_LOAD( "2h",           0x1800, 0x0800, CRC(591bc0d9) SHA1(170f9e92f0a3bee04407be27210b4fa825367688) )
	ROM_LOAD( "2j",           0x2000, 0x0800, CRC(22f11b6b) SHA1(e426ef6a7444a39a34d59799973b07d11b89f372) )
	ROM_LOAD( "2l",           0x2800, 0x0800, CRC(705ffe49) SHA1(174df3f281068c767344f751daace646360e26d6) )
	ROM_LOAD( "2m",           0x3000, 0x0800, CRC(ea26c35c) SHA1(a2f3380982d93a022f46756f974fd16c4cd617de) )
	ROM_LOAD( "2p",           0x3800, 0x0800, CRC(94d8f5e3) SHA1(f3a9c4d1d91836476fcad87ea0d243dde7171e0a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "ot1.5c",       0x0000, 0x0800, CRC(bcd297f0) SHA1(8ed78487d76fd0a917ab7b258937a46e2cd9800c) )
	ROM_LOAD( "ot2.5d",       0x0800, 0x0800, CRC(de7912da) SHA1(8558b4eff5d7e63029b325edef9914feda5834c3) )
	ROM_LOAD( "ot3.5e",       0x1000, 0x0800, CRC(ba2fa933) SHA1(1f976d8595706730e29f93027e7ab4620075c078) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "sdog-5f",      0x0000, 0x0800, CRC(7edbc59c) SHA1(53c9a7f46a968b2f804243f661479918870756b5) )
	ROM_LOAD( "sdog-5h",      0x0800, 0x0800, CRC(5b130bea) SHA1(269df1344414c0d517084a32a348a856c25d499c) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "c01s.6e",      0x0000, 0x0020, CRC(4e3caeab) SHA1(a25083c3e36d28afdefe4af6e6d4f3155e303625) )
ROM_END

ROM_START( scratest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "scratest.2d",  0x0000, 0x043d, CRC(a7f3cf0f) SHA1(4a934f6f660e536009ccb96b4914ed65f7a28855) )
	ROM_LOAD( "2e",           0x0800, 0x0800, CRC(e9b4b9eb) SHA1(a8ee9ddfadf5e9accedfaf81da757a88a2e55a0a) )
	ROM_LOAD( "2f",           0x1000, 0x0800, CRC(a1f14f4c) SHA1(3eae2b3e4596505a8afb5c5cfb108e823c2c4319) )
	ROM_LOAD( "2h",           0x1800, 0x0800, CRC(591bc0d9) SHA1(170f9e92f0a3bee04407be27210b4fa825367688) )
	ROM_LOAD( "2j",           0x2000, 0x0800, CRC(22f11b6b) SHA1(e426ef6a7444a39a34d59799973b07d11b89f372) )
	ROM_LOAD( "2l",           0x2800, 0x0800, CRC(705ffe49) SHA1(174df3f281068c767344f751daace646360e26d6) )
	ROM_LOAD( "2m",           0x3000, 0x0800, CRC(ea26c35c) SHA1(a2f3380982d93a022f46756f974fd16c4cd617de) )
	ROM_LOAD( "2p",           0x3800, 0x0800, CRC(94d8f5e3) SHA1(f3a9c4d1d91836476fcad87ea0d243dde7171e0a) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "ot1.5c",       0x0000, 0x0800, CRC(bcd297f0) SHA1(8ed78487d76fd0a917ab7b258937a46e2cd9800c) )
	ROM_LOAD( "ot2.5d",       0x0800, 0x0800, CRC(de7912da) SHA1(8558b4eff5d7e63029b325edef9914feda5834c3) )
	ROM_LOAD( "ot3.5e",       0x1000, 0x0800, CRC(ba2fa933) SHA1(1f976d8595706730e29f93027e7ab4620075c078) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "off-5f",       0x0000, 0x0800, CRC(7015a2e1) SHA1(19bb4cb26c8b91cd79368e4ed48065df6fd6577e) )
	ROM_LOAD( "off-5h",       0x0800, 0x0800, CRC(a36ac0fa) SHA1(b58412536833ab5ea705e919562b7b495ff3dff4) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "c01s.6e",      0x0000, 0x0020, CRC(4e3caeab) SHA1(a25083c3e36d28afdefe4af6e6d4f3155e303625) )
ROM_END

ROM_START( scobtest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "scobtest.2c",  0x0000, 0x02ff, CRC(5319182a) SHA1(18302ec0f943aaf1d335cef531700c5bfe4c9507) )
	ROM_LOAD( "2e",           0x1000, 0x1000, CRC(8e7245cd) SHA1(281504ff364c3ddbf901c92729b139afd93b9785) )
	ROM_LOAD( "epr1267.2f",   0x2000, 0x1000, CRC(47a4e6fb) SHA1(01775ad11dc23469649539ee8fb8a5800df031c6) )
	ROM_LOAD( "2h",           0x3000, 0x1000, CRC(7244f21c) SHA1(f5fff565ed3f6c5f277a4db53c9f569813fcec1d) )
	ROM_LOAD( "epr1269.2j",   0x4000, 0x1000, CRC(e1f8a801) SHA1(2add8270352d6596052d3ff22c891ceccaa92071) )
	ROM_LOAD( "2l",           0x5000, 0x1000, CRC(d52affde) SHA1(5681771ed51d504bdcc2999fcbf926a30b137828) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "5c",           0x0000, 0x0800, CRC(d4346959) SHA1(5eab4505beb69a5bdd88b23db60e1193371250cf) )
	ROM_LOAD( "5d",           0x0800, 0x0800, CRC(cc025d95) SHA1(2b0784c4d05c466e0b7648f16e14f34393d792c3) )
	ROM_LOAD( "5e",           0x1000, 0x0800, CRC(1628c53f) SHA1(ec79a73e4a2d7373454b227dd7eff255f1cc60cc) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "epr1274.5h",   0x0000, 0x0800, CRC(64d113b4) SHA1(7b439bb74d5ecc792e0ca8964bcca8c6b7a51262) )
	ROM_LOAD( "epr1273.5f",   0x0800, 0x0800, CRC(a96316d3) SHA1(9de0e94932e91dc34aea7c81880bde6a486d103b) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "82s123.6e",    0x0000, 0x0020, CRC(9b87f90d) SHA1(d11ac5e4a6057301ea2a9cbb404c2b978eb4c1dc) )
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

ROM_START( amidars01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "amidar.2c",    0x0000, 0x1000, CRC(c294bf27) SHA1(399325bf1559e8cdbddf7cfbf0dc739f9ed72ef0) )
	ROM_LOAD( "amidar.2e",    0x1000, 0x1000, CRC(e6e96826) SHA1(e9c4f8c594640424b456505e676352a98b758c03) )
	ROM_LOAD( "s01.2f",       0x2000, 0x1000, CRC(000da382) SHA1(383973843215e8510ce97e8b295049736dd21979) )
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


#define rom_jumpbugx rom_jumpbug


ROM_START( kazzy )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "frogger.26",   0x0000, 0x1000, CRC(597696d6) SHA1(e7e021776cad00f095a1ebbef407b7c0a8f5d835) )
	ROM_LOAD( "frogger.27",   0x1000, 0x1000, CRC(b6e6fcc3) SHA1(5e8692f2b0c7f4b3642b3ee6670e1c3b20029cdc) )
	ROM_LOAD( "frsm3.7",      0x2000, 0x1000, CRC(aca22ae0) SHA1(5a99060ea2506a3ac7d61ca5876ce5cb3e493565) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
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
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "frogger.ic5",  0x0000, 0x1000, CRC(efab0c79) SHA1(68c99b6cdcb9396bb473739a62ffc009b4bf57d5) )
	ROM_LOAD( "frogger.ic6",  0x1000, 0x1000, CRC(aeca9c13) SHA1(cdf560adbd7f2813e86e378da7781cccf7928a44) )
	ROM_LOAD( "frogger.ic7",  0x2000, 0x1000, CRC(dd251066) SHA1(4612e1fe1ab7182a277140b1a1976cc17e0746a5) )
	ROM_LOAD( "frogger.ic8",  0x3000, 0x1000, CRC(bf293a02) SHA1(be94e9f5caa74c3de6fd95bd20928f4a9c514227) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "frogger.608",  0x0000, 0x0800, CRC(e8ab0256) SHA1(f090afcfacf5f13cdfa0dfda8e3feb868c6ce8bc) )
	ROM_LOAD( "frogger.609",  0x0800, 0x0800, CRC(7380a48f) SHA1(75582a94b696062cbdb66a4c5cf0bc0bb94f81ee) )
	ROM_LOAD( "frogger.610",  0x1000, 0x0800, CRC(31d7eb27) SHA1(2e1d34ae4da385fd7cac94707d25eeddf4604e1a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "trukker.606",  0x0000, 0x0800, CRC(f0c49466) SHA1(f88d02ada32a9f1e10004785d17659279fd1d651) )
	ROM_LOAD( "trukker.607",  0x0800, 0x0800, CRC(6ab5c2eb) SHA1(d13fcd5669bd0f8f76505684ba5e788b395aeef1) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "pr-91.6l",     0x0000, 0x0020, CRC(413703bf) SHA1(66648b2b28d3dcbda5bdb2605d1977428939dd3c) )
ROM_END

ROM_START( frogtest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "frogtest.ic5", 0x0000, 0x0226, CRC(1f5a7f35) SHA1(608ad6ac8c34aea66b13aac7e45c298a2083d642) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "frogger.608",  0x0000, 0x0800, CRC(e8ab0256) SHA1(f090afcfacf5f13cdfa0dfda8e3feb868c6ce8bc) )
	ROM_LOAD( "frogger.609",  0x0800, 0x0800, CRC(7380a48f) SHA1(75582a94b696062cbdb66a4c5cf0bc0bb94f81ee) )
	ROM_LOAD( "frogger.610",  0x1000, 0x0800, CRC(31d7eb27) SHA1(2e1d34ae4da385fd7cac94707d25eeddf4604e1a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "frogger.607",  0x0000, 0x0800, CRC(05f7d883) SHA1(78831fd287da18928651a8adb7e578d291493eff) )
	ROM_LOAD( "frogger.606",  0x0800, 0x0800, CRC(f524ee30) SHA1(dd768967add61467baa08d5929001f157d6cd911) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "pr-91.6l",     0x0000, 0x0020, CRC(413703bf) SHA1(66648b2b28d3dcbda5bdb2605d1977428939dd3c) )
ROM_END

ROM_START( frogtest2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "frogtest2.ic5", 0x0000, 0x0440, CRC(103cbaf8) SHA1(f3c9ecfca918ea6fbb7936f4ad15c6a7fe46c4ab) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
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

ROM_START( kingball01 ) // bootleg, namco copyright removed
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "kb1",          0x0000, 0x0800, CRC(62631d01) SHA1(5cb2f80866649c8d0c047367a02242a409ba284d) )
	ROM_LOAD( "kb2",          0x0800, 0x0800, CRC(87b46a5d) SHA1(2c1fa68c50b64152399cf5b56ddbecafb937b139) )
	ROM_LOAD( "kb3",          0x1000, 0x0800, CRC(8174f0ee) SHA1(d5f9644bcb1ff691eacf73b42f640ca28754ad0e) )
	ROM_LOAD( "kb4",          0x1800, 0x0800, CRC(b06197fc) SHA1(9d2af3eccb73a74570d5220de13f5837955b2f5a) )
	ROM_LOAD( "kb5",          0x2000, 0x0800, CRC(3c63e5f0) SHA1(55c15e95391b58fac046f01a42d22adb93c3f705) )

	ROM_REGION( 0x10000, "audiocpu", 0 ) // these not supplied in the dump, using roms from kingball
	ROM_LOAD( "kbe1.ic4",     0x0000, 0x0800, CRC(5be2c80a) SHA1(f719a80357bed3d66bce40569690f419740148c5) )
	ROM_LOAD( "kbe2.ic5",     0x0800, 0x0800, CRC(bb59e965) SHA1(830e0c415f051e932d76df604025e4e33118a799) )
	ROM_LOAD( "kbe3.ic6",     0x1000, 0x0800, CRC(fbc7d286) SHA1(11511028920d30f65859b80379b5aab94240a741) )
	ROM_LOAD( "kbe2.ic7",     0x1800, 0x0800, CRC(bb59e965) SHA1(830e0c415f051e932d76df604025e4e33118a799) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "kb-hj",        0x0000, 0x0800, CRC(75a23849) SHA1(0c6c4f799fcd6bfd70ca702ca713db2fbd476cc0) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "kb-kl",        0x1000, 0x0800, CRC(4b711dd3) SHA1(c6294a5e4f0363c4e417e27f47b2d6c5526f7893) )
	ROM_RELOAD(               0x1800, 0x0800 )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "kb2-1",        0x0000, 0x0020, CRC(15dd5b16) SHA1(3d2ca2b42bf508a9e5198e970abcbbedf5729164) )
ROM_END

ROM_START( cosmicw )
	ROM_REGION( 0x4000, "maincpu", 0 ) // mostly Space Battle with a name change, and corrected colours
	ROM_LOAD( "sb1",    0x0000, 0x0800, CRC(d975af10) SHA1(a2e2a36a75db8fd09441308b08b6ae073c68b8cf) )
	ROM_LOAD( "sb2",    0x0800, 0x0800, CRC(b2ed14c3) SHA1(7668df11f64b8e296eedfee53437777dc53a56d5) )
	ROM_LOAD( "sb3",    0x1000, 0x0800, CRC(945f4160) SHA1(5fbe879f51e14c4c7ae551e5b3089f8e148770a4) )
	ROM_LOAD( "sb4",    0x1800, 0x0800, CRC(8229835c) SHA1(8cfd8f6cab6f80ca69645a184f7e841fc69f47f6) )
	ROM_LOAD( "cw5",    0x2000, 0x0800, CRC(74e70148) SHA1(e3fc1931bc52e831b953ba1bba6e484dffd19c69) )
	ROM_LOAD( "sb6",    0x2800, 0x0800, CRC(d915a389) SHA1(0e2ff6eec9453856a1276889946b463cfae58eba) )
	ROM_LOAD( "sb7",    0x3000, 0x0800, CRC(c9245346) SHA1(239bad3fe64eaab2dfc3febd06d1124103a10504) )
	ROM_LOAD( "sb8",    0x3800, 0x0800, CRC(e59ff1ae) SHA1(fef22885cbd3273882f8c7755dd04c28e843b9ea) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "sb12",   0x0000, 0x0800, CRC(8313c959) SHA1(b09157c6f824d6e94647728cbb329877fcb4e502) )
	ROM_LOAD( "sb14",   0x0800, 0x0800, CRC(c26132af) SHA1(7ae125a911dfd47aeca4f129f580762ce4d8d91a) )
	ROM_LOAD( "sb11",   0x1000, 0x0800, CRC(c9d4537e) SHA1(65d27066ffec04b755d2f5d3f36f5ec6792e8d6c) )
	ROM_LOAD( "k01_2.bin", 0x1800, 0x0800, CRC(dcc2b33b) SHA1(c3a5ac935c519400dfabb28909f7e460769d1837) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "l06_prom.bin", 0x0000, 0x0020, CRC(6a0c7d87) SHA1(140335d85c67c75b65689d4e76d29863c209cf32) )
ROM_END

ROM_START( pandajump )
	ROM_REGION( 0x4000, "maincpu", 0 )
	ROM_LOAD( "pj.u",         0x0000, 0x0800, CRC(b2658d7d) SHA1(d9e87bd4483f925861cfc4f571201d0d088e8565) )
	ROM_LOAD( "pj.v",         0x0800, 0x0800, CRC(d47fa6c5) SHA1(ddb5058c28feb18fb35c8bf88f362decfc9d6fb3) )
	ROM_LOAD( "pj.w",         0x1000, 0x0800, CRC(12e3e07e) SHA1(9544a0b5e5990d6ca302236e2f639f981a9132c2) )
	ROM_LOAD( "pj.y",         0x1800, 0x0800, CRC(ee2133d0) SHA1(ad64039ec280ec24b8ff921bedbf8719762b569f) )
	ROM_LOAD( "pj.7l",        0x2000, 0x0800, CRC(f1e8ba9e) SHA1(605db3fdbaff4ba13729371ad0c4fbab3889378e) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "pj.1h",        0x0000, 0x0800, CRC(6ac96510) SHA1(acb9897a4c5325b26eadd0300e49e43bbbf11462) )
	ROM_LOAD( "pj.1k",        0x0800, 0x0800, CRC(f2fb3d80) SHA1(fe421dcf4b9d2f75d9d98f0adb7630f054115b36) )

	ROM_REGION( 0x0020, "proms", 0 )
	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END


/***************************************************************************

  Game driver(s)

***************************************************************************/


//    Year  romname     parent    machine   inputs    class            init           rot

// Frogger
GAME( 1981, kazzy,      frogger,  frogger,  frogger,  galaxian_hbmame, init_frogger,  ROT90, "Kazzy", "K-azzy", MACHINE_SUPPORTS_SAVE )
GAME( 1999, trukker,    frogger,  frogger,  frogger,  galaxian_hbmame, init_trukker,  ROT90, "Grimlick","Trukker", MACHINE_SUPPORTS_SAVE )
GAME( 2004, frogtest,   frogger,  tst_frog, frogger,  galaxian_hbmame, init_frogger,  ROT90, "msmcdoug / JROK", "Frogger Sound Test", MACHINE_SUPPORTS_SAVE )
GAME( 2019, frogtest2,  frogger,  frogger,  frogger,  galaxian_hbmame, init_frogger,  ROT90, "Phil Murray", "Frogger Test Rom (2019-01-27)", MACHINE_SUPPORTS_SAVE )


// Amidar
GAME( 2007, amidarf,    amidar,   turtles,  amidar,   galaxian_hbmame, init_turtles,  ROT90, "S.Arkames", "Amidar (French, 2007-11-28)(Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, amidargr,   amidar,   turtles,  amidar,   galaxian_hbmame, init_turtles,  ROT90, "GreekRoms", "Amidar (Greek)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, amidars01,  amidar,   turtles,  amidar,   galaxian_hbmame, init_turtles,  ROT90, "Arkatrad", "Amidar (French, 2007-11-28)(Set 2)", MACHINE_SUPPORTS_SAVE )


// Scramble
GAME( 1998, offender,   scramble, scramble, scramble, galaxian_hbmame, init_scramble, ROT90, "Chris Henry (Battlpriest)", "Offender", MACHINE_SUPPORTS_SAVE )
GAME( 1998, superdog,   scramble, scramble, scramble, galaxian_hbmame, init_scramble, ROT90, "Jerky", "Superdog", MACHINE_SUPPORTS_SAVE )
GAME( 2019, scratest,   scramble, scramble, scramble, galaxian_hbmame, init_scramble, ROT90, "Phil Murray", "Scramble Test Rom (2019-01-27)", MACHINE_SUPPORTS_SAVE )


// Super Cobra
GAME( 2019, scobtest,   scobra,   scobra,   scobra,   galaxian_state,  init_scobra,   ROT90, "Phil Murray", "Super Cobra Test Rom (2019-01-27)", MACHINE_SUPPORTS_SAVE )


// Galaxian
GAME( 1979, andromd,    galnamco, galaxian, superg,   galaxian_hbmame, init_galaxian, ROT90, "hack", "Andromeda", MACHINE_SUPPORTS_SAVE)
GAME( 1998, buglaxn,    galnamco, galaxian, galaxian, galaxian_hbmame, init_galaxian, ROT90, "The Dog", "Galaxian (Bug sprites)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galaxiabh,  galnamco, galaxian, galaxiab, galaxian_hbmame, init_galaxian, ROT90, "bootleg", "Galaxian (Edition Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, galaxkyo,   galnamco, galaxian, superg,   galaxian_hbmame, init_galaxian, ROT90, "hack", "Galaxian (Kyoko)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, galaxni,    galnamco, galaxian, superg,   galaxian_hbmame, init_galaxian, ROT90, "Petaco S.A.", "Galaxian (New Invasion)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, galinvad,   galnamco, galaxian, galaxian, galaxian_hbmame, init_galaxian, ROT90, "T-Bone", "Galaxian (Space Invaders sprites)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, galnamco,   0,        galaxian, galaxian, galaxian_hbmame, init_galaxian, ROT90, "Namco", "Galaxians", MACHINE_SUPPORTS_SAVE )
GAME( 1998, gaylaxn,    galnamco, galaxian, superg,   galaxian_hbmame, init_galaxian, ROT90, "The Dog", "Gaylaxian", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mrdonm,     0,        galaxian, mrdonm,   galaxian_hbmame, init_galaxian, ROT90, "Krazy Ivan", "Mr. Do's Nightmare", MACHINE_SUPPORTS_SAVE )
GAME( 19??, galxtest1,  galnamco, galaxian, galaxian, galaxian_state,  init_galaxian, ROT90, "<unknown>", "Galaxian Test Rom", MACHINE_SUPPORTS_SAVE )
GAME( 2017, galxtest2,  galnamco, galaxian, galaxian, galaxian_state,  init_galaxian, ROT90, "Phil Murray", "Galaxian Test Rom (2017-12-07)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, vectrgal,   galnamco, galaxian, galaxian, galaxian_hbmame, init_galaxian, ROT90, "T-Bone", "Galaxian (Vector Sim)", MACHINE_SUPPORTS_SAVE )


// Moon Cresta
GAME( 2000, mooncmst,   mooncrst, mooncrst, mooncrst, galaxian_hbmame, init_mooncrst, ROT90, "Kazzy", "Moon Creamsta", MACHINE_SUPPORTS_SAVE )
GAME( 1980, mooncrs5f,  mooncrst, mooncrst, mooncrst, galaxian_hbmame, init_mooncrsu, ROT90, "bootleg", "Moon Cresta (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, moontest,   mooncrst, mooncrst, mooncrst, galaxian_hbmame, init_mooncrsu, ROT90, "Phil Murray", "Moon Cresta Test Rom (2017-12-07)", MACHINE_SUPPORTS_SAVE )


// Other
GAME( 1981, jumpbugx,   0,        jumpbugx, jumpbug,  galaxian_hbmame, init_jumpbug,  ROT90, "Rock-ola", "Jump Bug (Extra Sounds)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, kingball01, kingball, kingball, kingball, kingball_state,  init_galaxian, ROT90, "bootleg", "King & Balloon (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1979, cosmicw,    uniwars,  pisces,   spacbatt, pisces_state,    init_pisces,   ROT90, "B.G.Video Ltd", "Cosmic Wars", MACHINE_SUPPORTS_SAVE )
GAME( 19??, starfgh2,   pisces,   pisces,   piscesb,  pisces_state,    init_pisces,   ROT90, "bootleg", "Starfighter II", MACHINE_SUPPORTS_SAVE )
GAME( 1981, wbeast,     0,        galaxian, warofbug, galaxian_hbmame, init_nolock,   ROT90, "Compost", "Wriggly Beasties", MACHINE_SUPPORTS_SAVE )
GAME( 2022, pandajump,  0,        galaxian, galaxian, galaxian_hbmame, init_galaxian, ROT90, "Ben Larson", "Panda Jump", MACHINE_SUPPORTS_SAVE )


ROM_START( multigameb )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD( "code.bin",     0x00000, 0x80000, CRC(5635ed48) SHA1(703aa21c58f58902c56fa3710db2523ae84877e0) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "gfx1.bin",     0x00000, 0x10000, CRC(63b8420d) SHA1(3cbc101165b02c0a64b8406bf91af42f9142512c) )
	ROM_LOAD( "gfx2.bin",     0x10000, 0x10000, CRC(f962d3f7) SHA1(bd9aab0a511a8679280a83a2f8bbd1e82faf64d1) )

	ROM_REGION( 0x400, "proms", 0 )
	ROM_LOAD( "col.bin",      0x000000, 0x00400, CRC(c9320883) SHA1(814d42545a81f3316b564e52817c72b193f974ea) )
//	ROM_LOAD( "bigcol.bin",   0x000400, 0x10000, CRC(19f54955) SHA1(45f4361a1136ecb5e5297708bfe0a577812eab29) )

//	ROM_REGION( 0x1000, "gfx1", 0 )
//	ROM_LOAD( "1h.bin",       0x0000, 0x0800, CRC(39fb43a4) SHA1(4755609bd974976f04855d51e08ec0d62ab4bc07) )
//	ROM_LOAD( "1k.bin",       0x0800, 0x0800, CRC(7e3f56a2) SHA1(a9795d8b7388f404f3b0e2c6ce15d713a4c5bafa) )

//	ROM_REGION( 0x0020, "proms", 0 )
//	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

//        ROM_LOAD( "multi-prom.6e", 0x000000, 0x010000, CRC(5760a4f5) SHA1(539f56cae010488f0c6e4ff8de43e7dfe9b34375) )
//        ROM_LOAD( "multi-sndz80.bin", 0x000000, 0x010000, CRC(25865125) SHA1(5bbbc6f5a0ad6c6b86dea7893e4e18195c37192e) )
ROM_START( scramblemk )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD( "multi.main",     0x00000, 0x80000, CRC(26e8a444) SHA1(abf3b69076e9318f10c487a3bbe530fb74ee8290) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "multi.gfx1",     0x00000, 0x10000, CRC(7d420a14) SHA1(e603c3cf8fd88fa09017269d4f9ce8d027e20eaf) )
	ROM_LOAD( "multi.gfx2",     0x10000, 0x10000, CRC(a5e17a10) SHA1(9208a74f1b46c31c6d95e2b2fad325258f2301b7) )

	ROM_REGION( 0x400, "proms", 0 )
	ROM_LOAD( "multi.col",      0x000000, 0x00400, CRC(5b2feb51) SHA1(413fd60057cf3fcf6ad86463b2b814a4471d4882) )
	ROM_IGNORE(0x400)
//	ROM_LOAD( "bigcol.bin",   0x000400, 0x10000, CRC(19f54955) SHA1(45f4361a1136ecb5e5297708bfe0a577812eab29) )

//	ROM_REGION( 0x1000, "gfx1", 0 )
//	ROM_LOAD( "1h.bin",       0x0000, 0x0800, CRC(39fb43a4) SHA1(4755609bd974976f04855d51e08ec0d62ab4bc07) )
//	ROM_LOAD( "1k.bin",       0x0800, 0x0800, CRC(7e3f56a2) SHA1(a9795d8b7388f404f3b0e2c6ce15d713a4c5bafa) )

//	ROM_REGION( 0x0020, "proms", 0 )
//	ROM_LOAD( "6l.bpr",       0x0000, 0x0020, CRC(c3ac9467) SHA1(f382ad5a34d282056c78a5ec00c30ec43772bae2) )
ROM_END

class multib_state : public videight_state
{
public:
	multib_state(const machine_config &mconfig, device_type type, const char *tag)
		: videight_state(mconfig, type, tag)
		, m_rombank(*this, "rombank")
	{
	}

	void multib(machine_config &config);
	void init_multib();

private:
	//void multib_rombank_w(offs_t offset, uint8_t data);
	//void multib_gfxbank_w(offs_t offset, uint8_t data);
	void multib_extend_tile_info(uint16_t *code, uint8_t *color, uint8_t attrib, uint8_t x, uint8_t y);
	void multib_extend_sprite_info(const uint8_t *base, uint8_t *sx, uint8_t *sy, uint8_t *flipx, uint8_t *flipy, uint16_t *code, uint8_t *color);
	void mem_map(address_map &map);

	required_memory_bank m_rombank;
};

void multib_state::init_multib()
{
	m_rombank->configure_entries(0, 16, memregion("maincpu")->base(), 0x4000);
	m_rombank->set_entry(0);

	/* video extensions */
	common_init(nullptr, nullptr, nullptr, nullptr);
	m_extend_tile_info_ptr = extend_tile_info_delegate(&multib_state::videight_extend_tile_info, this);
	m_extend_sprite_info_ptr = extend_sprite_info_delegate(&multib_state::videight_extend_sprite_info, this);
}

static GFXDECODE_START(gfx_multib)
	GFXDECODE_SCALE("gfx1", 0x0000, galaxian_charlayout,   0, 32*32, GALAXIAN_XSCALE,1)
	GFXDECODE_SCALE("gfx1", 0x0000, galaxian_spritelayout, 0, 32*32, GALAXIAN_XSCALE,1)
GFXDECODE_END

void multib_state::mem_map(address_map &map)
{
	map(0x0000,0x3fff).bankr(m_rombank);
	map(0x4000,0x4fff).ram();
	map(0x5000,0x53ff).mirror(0x400).ram().w(FUNC(multib_state::galaxian_videoram_w)).share("videoram");
	map(0x5800,0x58ff).mirror(0x700).ram().w(FUNC(multib_state::galaxian_objram_w)).share("spriteram");
	map(0x6000,0x6000).portr("IN0");
	map(0x6800,0x6800).portr("IN1");
	map(0x7000,0x7000).portr("IN2");
	map(0x7800,0x7fff).r("watchdog",FUNC(watchdog_timer_device::reset_r));
	map(0x6000,0x6002).w(FUNC(multib_state::videight_gfxbank_w));
	map(0x6003,0x6003).w(FUNC(multib_state::coin_count_0_w));
	map(0x6004,0x6007).w("cust",FUNC(galaxian_sound_device::lfo_freq_w));
	map(0x6800,0x6807).w("cust",FUNC(galaxian_sound_device::sound_w));
	map(0x6808,0x68ff).nopw();
	map(0x7001,0x7001).w(FUNC(multib_state::irq_enable_w));
	map(0x7002,0x7005).w(FUNC(multib_state::videight_rombank_w));
	map(0x7006,0x7006).w(FUNC(multib_state::galaxian_flip_screen_x_w));
	map(0x7007,0x7007).w(FUNC(multib_state::galaxian_flip_screen_y_w));
	map(0x7008,0x7008).nopw();  /* bit 4 of rombank select - always 0 */
	map(0x7800,0x7800).w("cust",FUNC(galaxian_sound_device::pitch_w));
}

void multib_state::multib(machine_config &config)
{
	galaxian(config);

	// basic machine hardware
	m_maincpu->set_addrmap(AS_PROGRAM, &multib_state::mem_map);

	/* video hardware */
	m_gfxdecode->set_info(gfx_multib);
	m_palette->set_entries(32 * 32);
}

GAME( 2022, multigameb, galnamco, multib, warofbug, multib_state, init_multib, ROT90, "Macro", "MultigameB", MACHINE_SUPPORTS_SAVE )

class smk_state : public videight_state
{
public:
	smk_state(const machine_config &mconfig, device_type type, const char *tag)
		: videight_state(mconfig, type, tag)
		, m_rombank(*this, "rombank")
	{
	}

	void smk(machine_config &config);
	void init_smk();

private:
	//void multib_rombank_w(offs_t offset, uint8_t data);
	//void multib_gfxbank_w(offs_t offset, uint8_t data);
	void smk_extend_tile_info(uint16_t *code, uint8_t *color, uint8_t attrib, uint8_t x, uint8_t y);
	void smk_extend_sprite_info(const uint8_t *base, uint8_t *sx, uint8_t *sy, uint8_t *flipx, uint8_t *flipy, uint16_t *code, uint8_t *color);
	void mem_map(address_map &map);

	required_memory_bank m_rombank;
};

void smk_state::init_smk()
{
	m_rombank->configure_entries(0, 16, memregion("maincpu")->base(), 0x8000);
	m_rombank->set_entry(0);

	/* video extensions */
	common_init(nullptr, nullptr, nullptr, nullptr);
	m_extend_tile_info_ptr = extend_tile_info_delegate(&smk_state::videight_extend_tile_info, this);
	m_extend_sprite_info_ptr = extend_sprite_info_delegate(&smk_state::videight_extend_sprite_info, this);
}

static GFXDECODE_START(gfx_smk)
	GFXDECODE_SCALE("gfx1", 0x0000, galaxian_charlayout,   0, 32*32, GALAXIAN_XSCALE,1)
	GFXDECODE_SCALE("gfx1", 0x0000, galaxian_spritelayout, 0, 32*32, GALAXIAN_XSCALE,1)
GFXDECODE_END

void smk_state::mem_map(address_map &map)
{
	map.unmap_value_high();
	map(0x0000,0x3fff).bankr(m_rombank);
	map(0x4000,0x4fff).ram();
	map(0x5000,0x53ff).mirror(0x400).ram().w(FUNC(smk_state::galaxian_videoram_w)).share("videoram");
	map(0x5800,0x58ff).mirror(0x700).ram().w(FUNC(smk_state::galaxian_objram_w)).share("spriteram");
	map(0x6000,0x6000).portr("IN0");
	map(0x6800,0x6800).portr("IN1");
	map(0x7000,0x7000).portr("IN2");
	map(0x7800,0x7fff).r("watchdog",FUNC(watchdog_timer_device::reset_r));
	map(0x6000,0x6002).w(FUNC(smk_state::videight_gfxbank_w));
	map(0x6003,0x6003).w(FUNC(smk_state::coin_count_0_w));
	map(0x6004,0x6007).w("cust",FUNC(galaxian_sound_device::lfo_freq_w));
	map(0x6800,0x6807).w("cust",FUNC(galaxian_sound_device::sound_w));
	map(0x6808,0x68ff).nopw();
	map(0x7001,0x7001).w(FUNC(smk_state::irq_enable_w));
	map(0x7002,0x7005).w(FUNC(smk_state::videight_rombank_w));
	map(0x7006,0x7006).w(FUNC(smk_state::galaxian_flip_screen_x_w));
	map(0x7007,0x7007).w(FUNC(smk_state::galaxian_flip_screen_y_w));
	map(0x7008,0x7008).nopw();  /* bit 4 of rombank select - always 0 */
	map(0x7800,0x7800).w("cust",FUNC(galaxian_sound_device::pitch_w));
	map(0x7a00,0x7a00).nopw();   // unknown 0 and 1
	map(0x7c00,0x7c00).nopw();   // unknown random numbers
	map(0x7e00,0x7e00).nopw();   // unknown
	map(0x3600,0x3600).nopw();   // unknown
	map(0x8000,0xffff).rom();
}

void smk_state::smk(machine_config &config)
{
	galaxian(config);

	// basic machine hardware
	m_maincpu->set_addrmap(AS_PROGRAM, &smk_state::mem_map);

	/* video hardware */
	m_gfxdecode->set_info(gfx_smk);
	m_palette->set_entries(32 * 32);
}

GAME( 2022, scramblemk, 0, smk, warofbug, smk_state, init_smk, ROT90, "<unknown>", "Scramble MultiKit", MACHINE_SUPPORTS_SAVE )

