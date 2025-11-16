// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/asteroid.cpp"
#include "machine/eepromser.h"

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

ROM_START( stardest ) // Says Star Destroyer where you enter your initials after game over.
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

	ROM_REGION( 0x40, "earom", ROMREGION_ERASE00 ) // default to zero fill to suppress invalid high score display
ROM_END


GAME( 1979, asteroid6,  asteroid, asteroid, asteroid, asteroid_state, empty_init, ROT0, "Clay Cowgill", "Asteroids (Six-Bullet Hack)", MACHINE_SUPPORTS_SAVE )
GAMEL(1980, astdelux4,  asteroid, astdelux, astdelu4, asteroid_state, empty_init, ROT0, "Atari", "Asteroids Deluxe (Unknown Revision) [h]", MACHINE_SUPPORTS_SAVE, layout_astdelux )
GAME( 1979, stardest,   asteroid, asteroid, asteroid, asteroid_state, empty_init, ROT0, "Nufekop", "Star Destroyer (hack of Asteroids (rev 2))", MACHINE_SUPPORTS_SAVE )


/*********************************************************************************************************************/

/* Asteroids Multigame

such a confusing pile of roms

Status: Selftest screen shows. Main rom unscrambled, passes the seflftest. NVram implemented.
        Vector rom not unscrambled yet.
        Numerous hacks to be removed. Proper rom banking to be done.
        On the selftest, VEC is supposed to show the vector rom version (ex 1.1A).

*/

class amulti_state : public asteroid_state
{
public:
	amulti_state(const machine_config &mconfig, device_type type, const char *tag)
		: asteroid_state(mconfig, type, tag)
		, m_eeprom(*this, "nvram")
	{ }

	void amulti(machine_config &config);
	void init_amulti();
	void mem_map(address_map &map);
	void gfxbank_w(offs_t, u8 data);
	void rombank_w(offs_t, u8 data);
	required_device<eeprom_serial_93cxx_device> m_eeprom;
};

ROM_START( amulti )
	ROM_REGION( 0x10000, "maincpu", 0 )
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
	//ROM_LOAD( "adlv-11a.256",   0x10000, 0x8000, CRC(a03b957c) SHA1(45819fafdb34b9da0c3100e5ff143ba8e164c9d0) )

	ROM_REGION( 0x18000, "user3", ROMREGION_ERASEFF )
	// asteroid
	ROM_LOAD( "035127-02.np3", 0x11000, 0x0800, CRC(8b71fd9e) SHA1(8cd5005e531eafa361d6b7e9eed159d164776c70) ) //5000
	ROM_FILL(0x10800,2,0xEA)
	ROM_FILL(0x10802,1,1)
	ROM_FILL(0x117fe,1,0)
	// astdelux
	ROM_LOAD( "036800-02.r2",  0x12800, 0x0800, CRC(bb8cabe1) SHA1(cebaa1b91b96e8b80f2b2c17c6fd31fa9f156386) ) //4800
	ROM_LOAD( "036799-01.np2", 0x13000, 0x0800, CRC(7d511572) SHA1(1956a12bccb5d3a84ce0c1cc10c6ad7f64e30b40) ) //5000
	ROM_FILL(0x12800,2,0xEA)
	ROM_FILL(0x12802,1,1)
	ROM_FILL(0x137fe,1,0)
	// llander
	ROM_LOAD( "034599-01.r3",  0x14800, 0x0800, CRC(355a9371) SHA1(6ecb40169b797d9eb623bcb17872f745b1bf20fa) ) //4800
	ROM_LOAD( "034598-01.np3", 0x15000, 0x0800, CRC(9c4ffa68) SHA1(eb4ffc289d254f699f821df3146aa2c6cd78597f) ) //5000
	ROM_LOAD( "034597-01.m3",  0x15800, 0x0800, CRC(ebb744f2) SHA1(e685b094c1261a351e4e82dfb487462163f136a4) ) //5800
	ROM_FILL(0x14800,2,0xEA)
	ROM_FILL(0x14802,1,1)
	ROM_FILL(0x157fe,1,0)
ROM_END

void amulti_state::mem_map(address_map &map)
{
	map(0x0000, 0x01ff).ram();
	map(0x0200, 0x02ff).bankrw("ram1");
	map(0x0300, 0x03ff).bankrw("ram2");
	map(0x2000, 0x2007).r(FUNC(asteroid_state::asteroid_IN0_r)).nopw(); // IN0
	map(0x2400, 0x2407).r(FUNC(asteroid_state::asteroid_IN1_r)).nopw(); // IN1
	map(0x2800, 0x2803).r(FUNC(asteroid_state::asteroid_DSW1_r));       // DSW1
	map(0x2c00, 0x2c0f).rw("pokey", FUNC(pokey_device::read), FUNC(pokey_device::write));
	map(0x2c40, 0x2c7f).r(FUNC(asteroid_state::earom_read));
	map(0x3000, 0x3000).w(m_dvg, FUNC(dvg_device::go_w));
	map(0x3200, 0x323f).w(FUNC(asteroid_state::earom_write)).nopr();
	map(0x3400, 0x3400).w("watchdog", FUNC(watchdog_timer_device::reset_w));
	map(0x3600, 0x3600).w(FUNC(asteroid_state::asteroid_explode_w));
	map(0x3a00, 0x3a00).w(FUNC(asteroid_state::earom_control_w));
	map(0x3c00, 0x3c07).w("audiolatch", FUNC(ls259_device::write_d7));
	map(0x3e00, 0x3e00).w(FUNC(asteroid_state::asteroid_noise_reset_w));
	map(0x4000, 0x47ff).ram();                     // vector RAM
	map(0x4800, 0x5fff).rom();                     // vector ROM
	map(0x6000, 0x7fff).rom().mirror(0x8000);      // game ROM
	map(0x8000, 0x9fff).rom();                     // boot rom
	map(0xd900, 0xd900).w(FUNC(amulti_state::gfxbank_w));
	map(0xda00, 0xda00).w(FUNC(amulti_state::rombank_w));
	// NVram interface
	map(0xd300, 0xd300).lw8(NAME([this] (u8 data) {m_eeprom->clk_write(BIT(data,0)); }));
	map(0xd500, 0xd500).lw8(NAME([this] (u8 data) {m_eeprom->di_write(BIT(data,7)); }));
	map(0xd800, 0xd800).lr8(NAME([this] () {return m_eeprom->do_read() ? 128 : 0; }));
	map(0xdc00, 0xdc00).lw8(NAME([this] (u8 data) {m_eeprom->cs_write(BIT(data,0)); }));
}

void amulti_state::amulti(machine_config &config)
{
	astdelux(config);

	m_maincpu->set_addrmap(AS_PROGRAM, &amulti_state::mem_map);

	EEPROM_93C46_8BIT(config, m_eeprom);
}

void amulti_state::init_amulti()
{
	u8 *src = memregion("user2")->base();
	u8 *dst = memregion("user3")->base();

	offs_t i,j;
	u8 t;

	// unscramble main rom
	for (i = 0; i < 0x10000; i++)
	{
		t = bitswap<8>(src[i], 3, 4, 5, 6, 7, 1, 2, 0);
		j = bitswap<16>(i, 15, 5, 4, 7, 0, 10, 1, 11, 6, 2, 12, 3, 9, 8, 13, 14);

		if (!BIT(i, 10))
			j ^= 0x100;

		dst[j] = t;
	}

	dst[0x5a9] = 0x80;
	dst[0x5c6] = 0xd0;
	dst[0x6f8] = 0xd0;
	dst[0x78f] = 0x1a;
	dst[0x790] = 0x1a;
	dst[0x791] = 0x1a;

	// unscramble vector rom (to do later - in the meantime we will just use the original roms)

	//for (i = 0x10000; i < 0x18000; i++)
		//src[i] = bitswap<8>(src[i], 3, 4, 5, 6, 7, 1, 2, 0);

//	for (i = 0x10000; i < 0x18000; i++)
//	{
//		j = bitswap<16>(i, 15, 6, 5, 12, 11, 10, 9, 8, 7, 14, 13, 4, 3, 2, 1, 0);
//		dst[j|0x10000] = src[i];
//	}

	rombank_w(0,0);
	gfxbank_w(0,0);
}

void amulti_state::rombank_w(offs_t, u8 data)
{
	uint8_t* srcregion = memregion("user3")->base();
	uint8_t* dstregion = memregion("maincpu")->base();
	printf("switch to rombank %X\n",data);
	switch (data)
	{
		case 0x00:
			memcpy(dstregion+0x8000, srcregion, 0x2000);
			memcpy(dstregion+0x6000, srcregion+0x2000, 0x2000);
			return;
		case 0x40: // asteroids?
			memcpy(dstregion+0x8000, srcregion+0x4000, 0x2000);
			memcpy(dstregion+0x6000, srcregion+0x2000, 0x2000);
			return;
		case 0x80: // astdelux?
			memcpy(dstregion+0x8000, srcregion+0x7f00, 0x2000);
			memcpy(dstregion+0x6000, srcregion+0x4000, 0x2000);
			return;
		case 0xc0: // llander?
			memcpy(dstregion+0x8000, srcregion+0xbf00, 0x2000);
			memcpy(dstregion+0x6000, srcregion+0x7f00, 0x2000);
			return;
		default:
			printf("Unknown rombank: %X\n",data);
			return;
	}
}

void amulti_state::gfxbank_w(offs_t, u8 data)
{
	uint8_t* srcregion = memregion("user3")->base()+0x10000;
	uint8_t* dstregion = memregion("maincpu")->base()+0x4800;
	printf("switch to gfxbank %X\n",data);
	switch (data)
	{
		case 0x00:
			memcpy(dstregion, srcregion+0x2800, 0x1800);
			dstregion[0xffe] = 0x80;
			return;
		case 0x80: 
			memcpy(dstregion, srcregion+0x2800, 0x1800);
			dstregion[0xffe] = 0x40;
			return;
		default:
			printf("Unknown gfxbank: %X\n",data);
			return;
	}
}

GAME( 2002, amulti, astdelux, amulti, astdelux, amulti_state, init_amulti, ROT0, "Braze", "Asteroids Multigame", MACHINE_NOT_WORKING )


