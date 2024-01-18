// license:BSD-3-Clause
// copyright-holders: same as cps2.cpp, plus Zero800
/*******************************************************************************/

#include "sound/samples.h" //Zero800

//************** AUDIO SAMPLES ******************

const char *const samples_id_l[] = //zero800
{
	"*sfz3mix_l",
	"empty", // 00
	"L01", // 01
	"L02", // 02
	"L03", // 03
	"L04", // 04
	"L05", // 05
	"L06", // 06
	"L07", // 07
	"L08", // 08
	"L09", // 09
	"L0A", // 10
	"L0B", // 11
	"L0C", // 12
	"L0D", // 13
	"L0E", // 14
	"L0F", // 15
	"L10", // 16
	"L11", // 17
	"L12", // 18
	"L13", // 19
	"L14", // 20
	"L15", // 21
	"L16", // 22
	"L17", // 23
	"L18", // 24
	"L19", // 25
	"L1A", // 26
	"L1B", // 27
	"L1C", // 28
	"L1D", // 29
	"L1E", // 30
	"L1F", // 31
	"L20", // 32
	"L21", // 33
	"L22", // 34
	"L23", // 35
	"L24", // 36
	"L25", // 37
	"L26", // 38
	"L27", // 39
	"L28", // 40
	"L29", // 41
	"L2A", // 42
	"L2B", // 43
	"L2C", // 44
	"L2D", // 45
	"L2E", // 46
	"L2F", // 47
	"L30", // 48
	"L31", // 49
	"L32", // 50
	"L33", // 51
	"L34", // 52
	"L35", // 53
	"L36", // 54
	"L37", // 55
	"L38", // 56
	"L39", // 57
	"L3A", // 58
	"L3B", // 59
	"L3C", // 60
	"L3D", // 61
	"L3E", // 62
	"L3F", // 63
	0
};


const char *const samples_id_r[] = //zero800
{
	"*sfz3mix_r",
	"empty", // 00
	"R01", // 01
	"R02", // 02
	"R03", // 03
	"R04", // 04
	"R05", // 05
	"R06", // 06
	"R07", // 07
	"R08", // 08
	"R09", // 09
	"R0A", // 10
	"R0B", // 11
	"R0C", // 12
	"R0D", // 13
	"R0E", // 14
	"R0F", // 15
	"R10", // 16
	"R11", // 17
	"R12", // 18
	"R13", // 19
	"R14", // 20
	"R15", // 21
	"R16", // 22
	"R17", // 23
	"R18", // 24
	"R19", // 25
	"R1A", // 26
	"R1B", // 27
	"R1C", // 28
	"R1D", // 29
	"R1E", // 30
	"R1F", // 31
	"R20", // 32
	"R21", // 33
	"R22", // 34
	"R23", // 35
	"R24", // 36
	"R25", // 37
	"R26", // 38
	"R27", // 39
	"R28", // 40
	"R29", // 41
	"R2A", // 42
	"R2B", // 43
	"R2C", // 44
	"R2D", // 45
	"R2E", // 46
	"R2F", // 47
	"R30", // 48
	"R31", // 49
	"R32", // 50
	"R33", // 51
	"R34", // 52
	"R35", // 53
	"R36", // 54
	"R37", // 55
	"R38", // 56
	"R39", // 57
	"R3A", // 58
	"R3B", // 59
	"R3C", // 60
	"R3D", // 61
	"R3E", // 62
	"R3F", // 63
	0
};

void cps2_state::qsound_sharedram1_samples_w(offs_t offset, uint16_t data, uint16_t mem_mask) //Zero800 allows you to write and signal when a new song is played
{
	if (ACCESSING_BITS_0_7)
	{
		if (offset == 0x0001 && (data & 0xff) == 0x00 && m_qsound_sharedram1[0x00] == 0xff) //stop samples sound
		{
			m_samples_l->start(0, 0, false);
			m_samples_r->start(0, 0, false);
			//printf("%s Stop: %02x to %x\n", machine().describe_context().c_str(), offset, offset);
		}

		if (offset == 0xffe)
		{
			if (m_qsound_sharedram1[0x80] == 0x77) //stop music condition in audiocpu = C080 (z80 hacked)
			{
				m_samples_l->start(0, 0, false); //init L samples
				m_samples_r->start(0, 0, false); //init R samples
				m_qsound_sharedram2[0x27] = 0xc9; //Enable Qsound Volume
				m_qsound_sharedram1[0x80] = 0xff; //Reset music id flag
				m_qsound_sharedram1[0x81] = 0xff; //Reset music id flag
				//printf("%s Clear: %02x to %x\n", machine().describe_context().c_str(), data, offset);
			}
			else
			if (m_qsound_sharedram1[0x81] > 0x00 && m_qsound_sharedram1[0x81] < 0x40) //play music condition in audiocpu = C081 (z80 hacked)
			{
				int data_byte = m_qsound_sharedram1[0x81];
				m_qsound_sharedram1[0x80] = 0xff; //Reset music id flag
				m_qsound_sharedram1[0x81] = 0xff; //Reset music id flag
				//printf("%s Music: %02x to %x\n", machine().describe_context().c_str(), data_byte, offset);
				m_samples_l->start(0, data_byte, true);
				if (m_samples_l->playing(0) == true)
				{
					m_qsound_sharedram2[0x27] = 0x00; //Qsound music volume = 0
				}
				else
					m_samples_l->start(0, 0, false); //init L samples

				m_samples_r->start(0, data_byte, true);
				if (m_samples_r->playing(0)  == true)
				{
					m_qsound_sharedram2[0x27] = 0x00; //Qsound music volume = 0
				}
				else
					m_samples_r->start(0, 0, false); //init R samples
			}
			else
			if (m_qsound_sharedram2[0x27] == 0x00 && m_samples_l->playing(0) != true) //if loadstate and qsound volume = 0 && samples stopped
			{
				m_samples_l->start(0, 0, false); //init L samples
				m_samples_r->start(0, 0, false); //init R samples
				m_qsound_sharedram2[0x27] = 0xc9; //Enable Qsound Volume
				m_qsound_sharedram1[0x80] = 0xff; //Reset music id flag
				m_qsound_sharedram1[0x81] = 0xff; //Reset music id flag
				//printf("%s Volume0: %02x to %x\n", machine().describe_context().c_str(), data, offset);
			}
			else
			if (m_qsound_sharedram2[0x27] != 0x00 && m_samples_l->playing(0) == true) //if loadstate and qsound volume != 0 && samples playing
			{
				m_samples_l->start(0, 0, false); //init L samples
				m_samples_r->start(0, 0, false); //init R samples
				m_qsound_sharedram2[0x27] = 0xc9; //Enable Qsound Volume
				m_qsound_sharedram1[0x80] = 0xff; //Reset music id flag
				m_qsound_sharedram1[0x81] = 0xff; //Reset music id flag
				//printf("%s Volume0: %02x to %x\n", machine().describe_context().c_str(), data, offset);
			}
			else 
			if ((data & 0xff) == 0x55) //init samples in rom start
			{
				m_samples_l->start(0, 0, false);
				m_samples_r->start(0, 0, false);
			}
		}

		m_qsound_sharedram1[offset] = data;	//Play data Qsound
	}
}


void cps2_state::cps2turbo_map(address_map &map) //Zero800
{
	map(0x000000, 0x3fffff).rom(); //+200000 extra                                                  												   // 68000 ROM
	map(0x618000, 0x619fff).rw(FUNC(cps2_state::qsound_sharedram1_r), FUNC(cps2_state::qsound_sharedram1_samples_w));                 // Q RAM + Play samples condition
	map(0x660000, 0x663fff).ram();                                                                                                    // When bit 14 of 0x804030 equals 0 this space is available. Many games store highscores and other info here if available.
	map(0x664000, 0x664001).ram();                                                                                                    // Unknown - Only used if 0x660000-0x663fff available (could be RAM enable?)
	map(0x665000, 0x66500b).ram().share("output"); //moved from 0x400000  																   // CPS2 object output			
	map(0x700000, 0x701fff).w(FUNC(cps2_state::cps2_objram1_w)).share("objram1");                                                     // Object RAM, no game seems to use it directly
	map(0x708000, 0x709fff).mirror(0x006000).rw(FUNC(cps2_state::cps2_objram2_r), FUNC(cps2_state::cps2_objram2_w)).share("objram2"); // Object RAM
	map(0x800100, 0x80013f).w(FUNC(cps2_state::cps1_cps_a_w)).share("cps_a_regs");                                                    // Mirror (sfa)
	map(0x800140, 0x80017f).rw(FUNC(cps2_state::cps1_cps_b_r), FUNC(cps2_state::cps1_cps_b_w)).share("cps_b_regs");                   // Mirror (sfa)
	map(0x804000, 0x804001).portr("IN0");                                                                                             // IN0
	map(0x804010, 0x804011).portr("IN1");                                                                                             // IN1
	map(0x804020, 0x804021).portr("IN2");                                                                                             // IN2 + EEPROM
	map(0x804030, 0x804031).r(FUNC(cps2_state::cps2_qsound_volume_r));                                                                // Master volume. Also when bit 14=0 addon memory is present, when bit 15=0 network adapter present.
	map(0x804040, 0x804041).w(FUNC(cps2_state::cps2_eeprom_port_w));                                                                  // EEPROM
	map(0x8040a0, 0x8040a1).nopw();                                                                                                   // Unknown (reset once on startup)
	map(0x8040b0, 0x8040b3).r(FUNC(cps2_state::kludge_r));                                                                            // Unknown (xmcotaj hangs if this is 0)
	map(0x8040e0, 0x8040e1).w(FUNC(cps2_state::cps2_objram_bank_w));                                                                  // bit 0 = Object ram bank swap
	map(0x804100, 0x80413f).w(FUNC(cps2_state::cps1_cps_a_w)).share("cps_a_regs");                                                    // CPS-A custom
	map(0x804140, 0x80417f).rw(FUNC(cps2_state::cps1_cps_b_r), FUNC(cps2_state::cps1_cps_b_w));                                       // CPS-B custom
	map(0x900000, 0x92ffff).ram().w(FUNC(cps2_state::cps1_gfxram_w)).share("gfxram");                                                 // Video RAM
	map(0xff0000, 0xffffef).ram();                                                                                                    // RAM
	map(0xfffff0, 0xfffffb).ram().share("output");                                                                                    // CPS2 output
	map(0xfffffc, 0xffffff).ram();
}

void cps2_state::init_sfz3mix()
{
	m_turbo = 1;
	init_cps2();
}

void cps2_state::cps2turbo(machine_config &config) //Zero800
{
	/* basic machine hardware */
	M68000(config, m_maincpu, XTAL(32'000'000));
	m_maincpu->set_addrmap(AS_PROGRAM, &cps2_state::cps2turbo_map);
	m_maincpu->set_addrmap(AS_OPCODES, &cps2_state::decrypted_opcodes_map);
	//m_maincpu->disable_interrupt_mixer();
	TIMER(config, "scantimer").configure_scanline(FUNC(cps2_state::cps2_interrupt), "screen", 0, 1);

	Z80(config, m_audiocpu, XTAL(16'000'000));
	m_audiocpu->set_addrmap(AS_PROGRAM, &cps2_state::qsound_sub_map);
	m_audiocpu->set_periodic_int(FUNC(cps2_state::irq0_line_hold), attotime::from_hz(250)); // measured

	MCFG_MACHINE_START_OVERRIDE(cps2_state, cps2)

	EEPROM_93C46_16BIT(config, "eeprom");

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_video_attributes(VIDEO_UPDATE_BEFORE_VBLANK);
	m_screen->set_raw(CPS_PIXEL_CLOCK, CPS_HTOTAL, 48, 464, CPS_VTOTAL, 12, 246);
	m_screen->set_screen_update(FUNC(cps2_state::screen_update_cps1));
	m_screen->screen_vblank().set(FUNC(cps2_state::screen_vblank_cps1));
	m_screen->set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_cps1);
	PALETTE(config, m_palette, palette_device::BLACK).set_entries(0xc00);

	MCFG_VIDEO_START_OVERRIDE(cps2_state, cps2)

	/* sound hardware */
	SPEAKER(config, "lspeaker").front_left();
	SPEAKER(config, "rspeaker").front_right();

	QSOUND(config, m_qsound);
	m_qsound->add_route(0, "lspeaker", 1.0);
	m_qsound->add_route(1, "rspeaker", 1.0);

	//Zero800
	SAMPLES(config, m_samples_l);
	m_samples_l->set_channels(1);
	m_samples_l->set_samples_names(samples_id_l);
	m_samples_l->add_route(0, "lspeaker", 0.1);

	SAMPLES(config, m_samples_r);
	m_samples_r->set_channels(1);
	m_samples_r->set_samples_names(samples_id_r);
	m_samples_r->add_route(0, "rspeaker", 0.1); 
}


ROM_START( sfz3mix20 ) // 0.20
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix20.p1", 0x000000, 0x400000, CRC(dccaa96e) SHA1(2cb2645997de0de1c5d35b6af45cb203c826a8c1) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix20.c1", 0x0000000, 0x2000000, CRC(8f39c9b5) SHA1(d6c3dc2c83467238a078cba8691f2d67a4d92a77) )
	ROM_LOAD( "sfz3mix20.c2", 0x2000000, 0x2000000, CRC(ef993c43) SHA1(5fa66f483e888e4fc5442e79c00fd2d06d7a46a9) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix20.m1", 0x000000, 0x080000, CRC(09cc5048) SHA1(a302821d0ea2d04175ca11ffaa26422e0b9f8432) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix20.q1",   0x000000, 0x1000000, CRC(3e35e4ca) SHA1(2df04666808af25c749b00c8fef945d739c44f17) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3mix21 ) // 0.21
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix21.p1", 0x000000, 0x400000, CRC(ecb679c5) SHA1(e632ca9a37b50b7e4346b0b34c8c99b25e6c850e) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix20.c1", 0x0000000, 0x2000000, CRC(8f39c9b5) SHA1(d6c3dc2c83467238a078cba8691f2d67a4d92a77) )
	ROM_LOAD( "sfz3mix20.c2", 0x2000000, 0x2000000, CRC(ef993c43) SHA1(5fa66f483e888e4fc5442e79c00fd2d06d7a46a9) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix20.m1", 0x000000, 0x080000, CRC(09cc5048) SHA1(a302821d0ea2d04175ca11ffaa26422e0b9f8432) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix20.q1",   0x000000, 0x1000000, CRC(3e35e4ca) SHA1(2df04666808af25c749b00c8fef945d739c44f17) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3mix22 ) // 0.22
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix22.p1", 0x000000, 0x400000, CRC(dd05b62e) SHA1(33dc71b1766000aa4df4fd085ed280248cff1289) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix22.c1", 0x0000000, 0x2000000, CRC(820c9445) SHA1(addb1c9f0970fb8f4335147e48a07ecce10497a3) )
	ROM_LOAD( "sfz3mix22.c2", 0x2000000, 0x2000000, CRC(3922e859) SHA1(88ffddba1044f97479475ea178c44c0d5df78db9) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix22.m1", 0x000000, 0x080000, CRC(0b29471e) SHA1(34af4660798b243aaaaf3c5b70d09f948d9fb11b) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix20.q1",   0x000000, 0x1000000, CRC(3e35e4ca) SHA1(2df04666808af25c749b00c8fef945d739c44f17) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3mix23 ) // 0.23
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix23.p1", 0x000000, 0x400000, CRC(7287d3ae) SHA1(a82750bc9f586fa656b99d83123c27ca81b9d252) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix23.c1", 0x0000000, 0x2000000, CRC(09f422ed) SHA1(6e85264c8871f3f12360c82baf9e939137cbb919) )
	ROM_LOAD( "sfz3mix.c2", 0x2000000, 0x2000000, CRC(cbc02909) SHA1(a3bcf2f7a2d4139024790d55e4ab5a156b36e6e3) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix.m1", 0x000000, 0x080000, CRC(c6322d7b) SHA1(0530f0d7c248180478ff588397263d961b8c5c8b) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix.q1",   0x000000, 0x1000000, CRC(180cbe91) SHA1(182b56517606f8a9bde0f643fcc3b164f18136f7) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3mix24 ) // 0.24
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix24.p1", 0x000000, 0x400000, CRC(6b3cfa0c) SHA1(f9ddef33c3ef5a75859e907f12952f8e9cd9f7c7) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix.c1", 0x0000000, 0x2000000, CRC(ef7f2d3f) SHA1(0ac73c75c75210f36ed1dba11f0be1e513f88378) )
	ROM_LOAD( "sfz3mix.c2", 0x2000000, 0x2000000, CRC(cbc02909) SHA1(a3bcf2f7a2d4139024790d55e4ab5a156b36e6e3) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix.m1", 0x000000, 0x080000, CRC(c6322d7b) SHA1(0530f0d7c248180478ff588397263d961b8c5c8b) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix.q1",   0x000000, 0x1000000, CRC(180cbe91) SHA1(182b56517606f8a9bde0f643fcc3b164f18136f7) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3mix25 ) // 0.25
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix25.p1", 0x000000, 0x400000, CRC(2ef2a33b) SHA1(8b785b91851893f3a620b92e93f1c27aa3912459) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix.c1", 0x0000000, 0x2000000, CRC(ef7f2d3f) SHA1(0ac73c75c75210f36ed1dba11f0be1e513f88378) )
	ROM_LOAD( "sfz3mix.c2", 0x2000000, 0x2000000, CRC(cbc02909) SHA1(a3bcf2f7a2d4139024790d55e4ab5a156b36e6e3) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix.m1", 0x000000, 0x080000, CRC(c6322d7b) SHA1(0530f0d7c248180478ff588397263d961b8c5c8b) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix.q1",   0x000000, 0x1000000, CRC(180cbe91) SHA1(182b56517606f8a9bde0f643fcc3b164f18136f7) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3mix ) // 0.26
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "sfz3mix.p1", 0x000000, 0x400000, CRC(d97dcc03) SHA1(a79b1b272bd5cb510189434a3b8c6df474f95d40) )
	ROM_IGNORE(0x200000)  // empty

	ROM_REGION( 0x4000000, "gfx", 0 )
	ROM_LOAD( "sfz3mix.c1", 0x0000000, 0x2000000, CRC(94a13726) SHA1(9c940dfe4e6caf7d24d6740444abc7e44355c5fe) )
	ROM_LOAD( "sfz3mix.c2", 0x2000000, 0x2000000, CRC(cbc02909) SHA1(a3bcf2f7a2d4139024790d55e4ab5a156b36e6e3) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "sfz3mix.m1", 0x000000, 0x080000, CRC(c6322d7b) SHA1(0530f0d7c248180478ff588397263d961b8c5c8b) )

	ROM_REGION( 0x1000000, "qsound", 0 )
	ROM_LOAD( "sfz3mix.q1",   0x000000, 0x1000000, CRC(180cbe91) SHA1(182b56517606f8a9bde0f643fcc3b164f18136f7) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END


GAME( 2023, sfz3mix20,  sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.20)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, sfz3mix21,  sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.21)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, sfz3mix22,  sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.22)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, sfz3mix23,  sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.23)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, sfz3mix24,  sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.24)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, sfz3mix25,  sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.25)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, sfz3mix,    sfa3, cps2turbo, cps2_2p6b, cps2_state, init_sfz3mix, ROT0, "Zero800", "Street Fighter Zero 3 (Turbo Mix 0.26)", MACHINE_SUPPORTS_SAVE )

