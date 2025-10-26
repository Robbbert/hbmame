// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/asteroid.cpp"

static INPUT_PORTS_START( astdelu4 )
	PORT_INCLUDE(astdelux)
	PORT_MODIFY("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 ) // Coin Left
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_COIN2 ) // Coin Center
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_COIN3 ) // Coin Right
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_BUTTON4 ) PORT_CODE(KEYCODE_LALT) PORT_CODE(JOYCODE_BUTTON2) // thrust
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_CODE(KEYCODE_RIGHT) PORT_CODE(JOYCODE_X_RIGHT_SWITCH) // right
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_CODE(KEYCODE_LEFT) PORT_CODE(JOYCODE_X_LEFT_SWITCH) // left
INPUT_PORTS_END



/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( starslayer ) // This name does not appear anywhere, but it does say Star Destroyer at the end.
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "035145-02.ef2",  0x6800, 0x0800, CRC(0cc75459) SHA1(2af85c9689b878155004da47fedbde5853a18723) )
	ROM_LOAD( "035144-02.h2",   0x7000, 0x0800, CRC(096ed35c) SHA1(064d680ded7f30c543f93ae5ca85f90d550f73e5) )
	ROM_LOAD( "035143-02.j2",   0x7800, 0x0800, CRC(312caa02) SHA1(1ce2eac1ab90b972e3f1fc3d250908f26328d6cb) )
	/* Vector ROM */
	ROM_LOAD( "035127-02.ss",   0x5000, 0x0800, CRC(edf610a2) SHA1(bbbf62820a3b06488201137c735c0d8c03cf535c) )

	/* DVG PROM */
	ROM_REGION( 0x100, "dvg:prom", 0 )
	ROM_LOAD( "034602-01.c8",   0x0000, 0x0100, CRC(97953db8) SHA1(8cbded64d1dd35b18c4d5cece00f77e7b2cab2ad) )
ROM_END

ROM_START( asteroid6 )
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "asteroid6.ef2",  0x6800, 0x0800, CRC(6cdf25a3) SHA1(96e7882e5e144757d3adc51de8bfd5f583c0302e) )
	ROM_LOAD( "asteroid6.h2",   0x7000, 0x0800, CRC(b4dcfbde) SHA1(447e888c4af704c9f30801b2417617b7caa73921) )
	ROM_LOAD( "035143-02.j2",   0x7800, 0x0800, CRC(312caa02) SHA1(1ce2eac1ab90b972e3f1fc3d250908f26328d6cb) )
	/* Vector ROM */
	ROM_LOAD( "035127-02.np3",  0x5000, 0x0800, CRC(8b71fd9e) SHA1(8cd5005e531eafa361d6b7e9eed159d164776c70) )

	ROM_REGION( 0x0100, "dvg:prom", 0 )
	ROM_LOAD( "034602-01.c8",   0x0000, 0x0100, CRC(97953db8) SHA1(8cbded64d1dd35b18c4d5cece00f77e7b2cab2ad) )
ROM_END

ROM_START( astdelux4 )
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "036430.02",      0x6000, 0x0800, CRC(cbcfba4f) SHA1(7e0ed90588e47c89182a18a8f5815ad052a42674) )
	ROM_LOAD( "036431.02",      0x6800, 0x0800, CRC(3249e623) SHA1(7dd7f4531680e4fe2efb14e22466458c2e2ea4e7) )
	ROM_LOAD( "036432.02",      0x7000, 0x0800, CRC(6d720c41) SHA1(198218cd2f43f8b83e4463b1f3a8aa49da5015e4) )
	ROM_LOAD( "036433.03",      0x7800, 0x0800, CRC(0dcc0be6) SHA1(bf10ffb0c4870e777d6b509cbede35db8bb6b0b8) )
	/* Vector ROM */
	ROM_LOAD( "036800.02",      0x4800, 0x0800, CRC(5c0ca7a6) SHA1(4abbac4a4974c89a0b44e1a92ea0c8b9095d9594) )
	ROM_LOAD( "036799.01",      0x5000, 0x0800, CRC(7d511572) SHA1(1956a12bccb5d3a84ce0c1cc10c6ad7f64e30b40) )
	/* DVG PROM */
	ROM_REGION( 0x100, "dvg:prom", 0 )
	ROM_LOAD( "034602-01.c8",   0x0000, 0x0100, CRC(97953db8) SHA1(8cbded64d1dd35b18c4d5cece00f77e7b2cab2ad) )
ROM_END


GAME( 1979, asteroid6,  asteroid, asteroid, asteroid, asteroid_state, empty_init, ROT0, "Clay Cowgill", "Asteroids (Six-Bullet Hack)", MACHINE_SUPPORTS_SAVE )
GAMEL(1980, astdelux4,  asteroid, astdelux, astdelu4, asteroid_state, empty_init, ROT0, "Atari", "Asteroids Deluxe (Unknown Revision) [h]", MACHINE_SUPPORTS_SAVE, layout_astdelux )
GAME( 1979, starslayer, asteroid, asteroid, asteroid, asteroid_state, empty_init, ROT0, "Nufekop", "Starslayer (hack of Asteroids (rev 2))", MACHINE_SUPPORTS_SAVE )


/*********************************************************************************************************************/

/* Asteroids Multigame

such a confusing pile of roms

*/

class amulti_state : public asteroid_state
{
public:
	amulti_state(const machine_config &mconfig, device_type type, const char *tag)
		: asteroid_state(mconfig, type, tag)
//		, m_maincpu(*this, "maincpu")
//		, m_p_ram(*this, "ram")
	{ }

	void init_amulti();
};

ROM_START( amulti )
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "036430-02.d1",  0x6000, 0x0800, CRC(a4d7a525) SHA1(abe262193ec8e1981be36928e9a89a8ac95cd0ad) )
	ROM_LOAD( "036431-02.ef1", 0x6800, 0x0800, CRC(d4004aae) SHA1(aa2099b8fc62a79879efeea70ea1e9ed77e3e6f0) )
	ROM_LOAD( "036432-02.fh1", 0x7000, 0x0800, CRC(6d720c41) SHA1(198218cd2f43f8b83e4463b1f3a8aa49da5015e4) )
	ROM_LOAD( "036433-03.j1",  0x7800, 0x0800, CRC(0dcc0be6) SHA1(bf10ffb0c4870e777d6b509cbede35db8bb6b0b8) )
	// Vector ROM
	ROM_LOAD( "036800-02.r2",  0x4800, 0x0800, CRC(bb8cabe1) SHA1(cebaa1b91b96e8b80f2b2c17c6fd31fa9f156386) )
	ROM_LOAD( "036799-01.np2", 0x5000, 0x0800, CRC(7d511572) SHA1(1956a12bccb5d3a84ce0c1cc10c6ad7f64e30b40) )

	// DVG PROM
	ROM_REGION( 0x100, "dvg:prom", 0 )
	ROM_LOAD( "034602-01.c8",  0x0000, 0x0100, CRC(97953db8) SHA1(8cbded64d1dd35b18c4d5cece00f77e7b2cab2ad) )

	ROM_REGION( 0x40, "earom", ROMREGION_ERASE00 ) // default to zero fill to suppress invalid high score display

	ROM_REGION( 0x18000, "user2", 0 )
	//ROM_LOAD( "adla-11a.512",   0x0000, 0x10000, CRC(43ad0f95) SHA1(6105926b4d734ec12e23579faa4453b4d5a981f9) )
	ROM_LOAD( "adld-11a.512",   0x0000, 0x10000, CRC(44e8583b) SHA1(945074d1efff3af83805bd3bcc403362038a1858) )
	ROM_LOAD( "adlv-11a.256",   0x10000, 0x8000, CRC(a03b957c) SHA1(45819fafdb34b9da0c3100e5ff143ba8e164c9d0) )

	ROM_REGION( 0x18000, "user3", ROMREGION_ERASEFF )
ROM_END

void amulti_state::init_amulti()
{
	u8 *src = memregion("user2")->base();
	u8 *dest = memregion("user3")->base();

	offs_t i,j;
	u8 t;

	for (i = 0; i < 0x10000; i++)
	{
		t = bitswap<8>(src[i], 3, 4, 5, 6, 7, 1, 2, 0);
		j = bitswap<16>(i, 15, 5, 4, 7, 0, 10, 1, 11, 6, 2, 12, 3, 9, 8, 13, 14);

		if (!BIT(i, 10))
			j ^= 0x100;

		dest[j] = t;
	}
	//uint8_t* srcregion = memregion("user3")->base()+0x6000;
	//uint8_t* dstregion = memregion("maincpu")->base()+0x6000;
	//memcpy(dstregion, srcregion, 0x2000);


//	for (i = 0x10000; i < 0x18000; i++)
//		src[i] = bitswap<8>(src[i], 3, 4, 5, 6, 7, 1, 2, 0);

//	for (i = 0x10000; i < 0x18000; i++)
//	{
//		j = bitswap<16>(i, 15, 6, 5, 12, 11, 10, 9, 8, 7, 14, 13, 4, 3, 2, 1, 0);
//		dest[j|0x10000] = src[i];
//	}

//	src = memregion("maincpu")->base()+0x6000;
//	for (i = 0; i < 0x2000; i++)
//		src[i] = dest[i];
}


GAME( 2002, amulti, astdelux, astdelux, astdelux, amulti_state, init_amulti, ROT0, "Braze", "Asteroids Multigame", MACHINE_NOT_WORKING )


