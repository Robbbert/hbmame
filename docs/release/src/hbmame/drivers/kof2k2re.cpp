// license:GPL_2.0
// copyright-holders:Robbbert

#include "includes/neogeo.h"

// This uses 4 audio cpus, with sounds and M1 taken from other games.
// Everything is hooked up as far as I can tell, but there's no extra sounds. No idea which fighters need them.
// The 'result_r' functions are never called, and so 'soundlatch2' never gets read.

class kof2k2re_hbmame : public neogeo_state
{
public:
	kof2k2re_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo_state(mconfig, type, tag)
		, m2_audiocpu(*this, "audiocpu2")
		, m2_ym(*this, "ymsnd2")
		, m2_soundlatch(*this, "soundlatch_m2")
		, m2_soundlatch2(*this, "soundlatch2_m2")
		, m2_bank_audio_main(*this, "audio2_main")
		, m3_audiocpu(*this, "audiocpu3")
		, m3_ym(*this, "ymsnd3")
		, m3_soundlatch(*this, "soundlatch_m3")
		, m3_soundlatch2(*this, "soundlatch2_m3")
		, m3_bank_audio_main(*this, "audio3_main")
		, m4_audiocpu(*this, "audiocpu4")
		, m4_ym(*this, "ymsnd4")
		, m4_soundlatch(*this, "soundlatch_m4")
		, m4_soundlatch2(*this, "soundlatch2_m4")
		, m4_bank_audio_main(*this, "audio4_main")
	{ }

	void ps2(machine_config &config);

private:
	void ps2_map(address_map &map);

	void audio2_map(address_map &map);
	void audio2_io(address_map &map);
	void audio2_command_w(u8 data);
	u16 audio2_result_r();
	u8 audio2_command_r();
	u8 audio2_bank_select_r(offs_t offset);
	void audio2_check_nmi();
	void audio2_enable_nmi_w(offs_t offset, u8 data);

	void audio3_map(address_map &map);
	void audio3_io(address_map &map);
	void audio3_command_w(u8 data);
	u16 audio3_result_r();
	u8 audio3_command_r();
	u8 audio3_bank_select_r(offs_t offset);
	void audio3_check_nmi();
	void audio3_enable_nmi_w(offs_t offset, u8 data);

	void audio4_map(address_map &map);
	void audio4_io(address_map &map);
	void audio4_command_w(u8 data);
	u16 audio4_result_r();
	u8 audio4_command_r();
	u8 audio4_bank_select_r(offs_t offset);
	void audio4_check_nmi();
	void audio4_enable_nmi_w(offs_t offset, u8 data);

	void machine_start() override;

	required_device<cpu_device> m2_audiocpu;
	required_device<ym2610_device> m2_ym;
	required_device<generic_latch_8_device> m2_soundlatch;
	required_device<generic_latch_8_device> m2_soundlatch2;
	required_memory_bank   m2_bank_audio_main;
	memory_bank           *m2_bank_audio_cart[4]{};
	bool       m2_nmi_enabled = 0;
	bool       m2_nmi_pending = 0;
	required_device<cpu_device> m3_audiocpu;
	required_device<ym2610_device> m3_ym;
	required_device<generic_latch_8_device> m3_soundlatch;
	required_device<generic_latch_8_device> m3_soundlatch2;
	required_memory_bank   m3_bank_audio_main;
	memory_bank           *m3_bank_audio_cart[4]{};
	bool       m3_nmi_enabled = 0;
	bool       m3_nmi_pending = 0;
	required_device<cpu_device> m4_audiocpu;
	required_device<ym2610_device> m4_ym;
	required_device<generic_latch_8_device> m4_soundlatch;
	required_device<generic_latch_8_device> m4_soundlatch2;
	required_memory_bank   m4_bank_audio_main;
	memory_bank           *m4_bank_audio_cart[4]{};
	bool       m4_nmi_enabled = 0;
	bool       m4_nmi_pending = 0;
};

void kof2k2re_hbmame::machine_start()
{
	neogeo_state::machine_start();

	m2_ym->reset();
	m3_ym->reset();
	m4_ym->reset();

	/* set the initial audio CPU ROM banks */
	u8 *rgn = memregion("audiocpu2")->base();

	/* audio bios/cartridge selection */
	m2_bank_audio_main->configure_entry(1, memregion("audiocpu2")->base());
	if (memregion("audiobios"))
		m2_bank_audio_main->configure_entry(0, memregion("audiobios")->base());

	m2_bank_audio_main->set_entry(m_use_cart_audio);

	/* audio banking */
	m2_bank_audio_cart[0] = membank("audio2_f000");
	m2_bank_audio_cart[1] = membank("audio2_e000");
	m2_bank_audio_cart[2] = membank("audio2_c000");
	m2_bank_audio_cart[3] = membank("audio2_8000");

	u32 address_mask = (memregion("audiocpu2")->bytes() - 0x10000 - 1) & 0x3ffff;


	for (u8 region = 0; region < 4; region++)
	{
		for (int bank = 0xff; bank >= 0; bank--)
		{
			u32 bank_address = 0x10000 + ((bank << (11 + region)) & address_mask);
			m2_bank_audio_cart[region]->configure_entry(bank, &rgn[bank_address]);
		}
	}

	m2_bank_audio_cart[0]->set_entry(0x1e);
	m2_bank_audio_cart[1]->set_entry(0x0e);
	m2_bank_audio_cart[2]->set_entry(0x06);
	m2_bank_audio_cart[3]->set_entry(0x02);

	/* set the initial audio CPU ROM banks */
	rgn = memregion("audiocpu3")->base();

	/* audio bios/cartridge selection */
	m3_bank_audio_main->configure_entry(1, memregion("audiocpu3")->base());
	if (memregion("audiobios"))
		m3_bank_audio_main->configure_entry(0, memregion("audiobios")->base());

	m3_bank_audio_main->set_entry(m_use_cart_audio);

	/* audio banking */
	m3_bank_audio_cart[0] = membank("audio3_f000");
	m3_bank_audio_cart[1] = membank("audio3_e000");
	m3_bank_audio_cart[2] = membank("audio3_c000");
	m3_bank_audio_cart[3] = membank("audio3_8000");

	address_mask = (memregion("audiocpu3")->bytes() - 0x10000 - 1) & 0x3ffff;


	for (u8 region = 0; region < 4; region++)
	{
		for (int bank = 0xff; bank >= 0; bank--)
		{
			u32 bank_address = 0x10000 + ((bank << (11 + region)) & address_mask);
			m3_bank_audio_cart[region]->configure_entry(bank, &rgn[bank_address]);
		}
	}

	m3_bank_audio_cart[0]->set_entry(0x1e);
	m3_bank_audio_cart[1]->set_entry(0x0e);
	m3_bank_audio_cart[2]->set_entry(0x06);
	m3_bank_audio_cart[3]->set_entry(0x02);

	/* set the initial audio CPU ROM banks */
	rgn = memregion("audiocpu4")->base();

	/* audio bios/cartridge selection */
	m4_bank_audio_main->configure_entry(1, memregion("audiocpu4")->base());
	if (memregion("audiobios"))
		m4_bank_audio_main->configure_entry(0, memregion("audiobios")->base());

	m4_bank_audio_main->set_entry(m_use_cart_audio);

	/* audio banking */
	m4_bank_audio_cart[0] = membank("audio4_f000");
	m4_bank_audio_cart[1] = membank("audio4_e000");
	m4_bank_audio_cart[2] = membank("audio4_c000");
	m4_bank_audio_cart[3] = membank("audio4_8000");

	address_mask = (memregion("audiocpu4")->bytes() - 0x10000 - 1) & 0x3ffff;


	for (u8 region = 0; region < 4; region++)
	{
		for (int bank = 0xff; bank >= 0; bank--)
		{
			u32 bank_address = 0x10000 + ((bank << (11 + region)) & address_mask);
			m4_bank_audio_cart[region]->configure_entry(bank, &rgn[bank_address]);
		}
	}

	m4_bank_audio_cart[0]->set_entry(0x1e);
	m4_bank_audio_cart[1]->set_entry(0x0e);
	m4_bank_audio_cart[2]->set_entry(0x06);
	m4_bank_audio_cart[3]->set_entry(0x02);
}

void kof2k2re_hbmame::audio2_check_nmi()
{
	m2_audiocpu->set_input_line(INPUT_LINE_NMI, (m2_nmi_enabled && m2_nmi_pending) ? ASSERT_LINE : CLEAR_LINE);
}

void kof2k2re_hbmame::audio2_enable_nmi_w(offs_t offset, u8 data)
{
	// out ($08) enables the nmi, out ($18) disables it
	m2_nmi_enabled = BIT(offset, 4) ^ 1;
	audio2_check_nmi();
}

u16 kof2k2re_hbmame::audio2_result_r()
{
	return m2_soundlatch2->read() << 8;
}

void kof2k2re_hbmame::audio2_command_w(u8 data)
{
	m2_soundlatch->write(data);
	m2_nmi_pending = true;
	audio2_check_nmi();
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}

u8 kof2k2re_hbmame::audio2_command_r()
{
	u8 ret = m2_soundlatch->read();
	m2_nmi_pending = false;
	audio2_check_nmi();
	return ret;
}

u8 kof2k2re_hbmame::audio2_bank_select_r(offs_t offset)
{printf("2=%X ",offset);
	m2_bank_audio_cart[offset & 3]->set_entry(offset >> 8);
	return 0;
}

void kof2k2re_hbmame::audio3_check_nmi()
{
	m3_audiocpu->set_input_line(INPUT_LINE_NMI, (m3_nmi_enabled && m3_nmi_pending) ? ASSERT_LINE : CLEAR_LINE);
}

void kof2k2re_hbmame::audio3_enable_nmi_w(offs_t offset, u8 data)
{
	// out ($08) enables the nmi, out ($18) disables it
	m3_nmi_enabled = BIT(offset, 4) ^ 1;
	audio3_check_nmi();
}

u16 kof2k2re_hbmame::audio3_result_r()
{
	return m3_soundlatch2->read() << 8;
}

void kof2k2re_hbmame::audio3_command_w(u8 data)
{
	m3_soundlatch->write(data);
	m3_nmi_pending = true;
	audio3_check_nmi();
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}

u8 kof2k2re_hbmame::audio3_command_r()
{
	u8 ret = m3_soundlatch->read();
	m3_nmi_pending = false;
	audio3_check_nmi();
	return ret;
}

u8 kof2k2re_hbmame::audio3_bank_select_r(offs_t offset)
{//printf("3=%X ",offset);
	m3_bank_audio_cart[offset & 3]->set_entry(offset >> 8);
	return 0;
}

void kof2k2re_hbmame::audio4_check_nmi()
{
	m4_audiocpu->set_input_line(INPUT_LINE_NMI, (m4_nmi_enabled && m4_nmi_pending) ? ASSERT_LINE : CLEAR_LINE);
}

void kof2k2re_hbmame::audio4_enable_nmi_w(offs_t offset, u8 data)
{
	// out ($08) enables the nmi, out ($18) disables it
	m4_nmi_enabled = BIT(offset, 4) ^ 1;
	audio4_check_nmi();
}

void kof2k2re_hbmame::audio4_command_w(u8 data)
{
	m4_soundlatch->write(data);
	m4_nmi_pending = true;
	audio4_check_nmi();
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}

u16 kof2k2re_hbmame::audio4_result_r()
{
	return m4_soundlatch2->read() << 8;
}

u8 kof2k2re_hbmame::audio4_command_r()
{
	u8 ret = m4_soundlatch->read();
	m4_nmi_pending = false;
	audio4_check_nmi();
	return ret;
}

u8 kof2k2re_hbmame::audio4_bank_select_r(offs_t offset)
{//printf("4=%X ",offset);
	m4_bank_audio_cart[offset & 3]->set_entry(offset >> 8);
	return 0;
}

void kof2k2re_hbmame::ps2_map(address_map &map)
{
	gsc_map(map);
	//map(0x300000, 0x300001).mirror(0x01ff7e).portr("IN0");
	//map(0x340000, 0x340001).mirror(0x01fffe).portr("IN1");
	//map(0x3a0000, 0x3a001f).mirror(0x01ffe0).w(FUNC(kof2k2re_hbmame::system_control_w_m4));
	map(0xba0000, 0xba0001).rw(FUNC(kof2k2re_hbmame::audio4_result_r),FUNC(kof2k2re_hbmame::audio4_command_w));
	map(0xbc0000, 0xbc0001).rw(FUNC(kof2k2re_hbmame::audio3_result_r),FUNC(kof2k2re_hbmame::audio3_command_w));
	map(0xbe0000, 0xbe0001).rw(FUNC(kof2k2re_hbmame::audio2_result_r),FUNC(kof2k2re_hbmame::audio2_command_w));
	//map(0xba0000,0xba0001).w(FUNC(kof2k2re_hbmame::audio4_command_w)).umask16(0xff00);
	//map(0xbc0000,0xbc0001).w(FUNC(kof2k2re_hbmame::audio3_command_w)).umask16(0xff00);
	//map(0xbe0000,0xbe0001).w(FUNC(kof2k2re_hbmame::audio2_command_w)).umask16(0xff00);
}

void kof2k2re_hbmame::audio2_map(address_map &map)
{
	map(0x0000,0x7fff).bankr("audio2_main");
	map(0x8000,0xbfff).bankr("audio2_8000");
	map(0xc000,0xdfff).bankr("audio2_c000");
	map(0xe000,0xefff).bankr("audio2_e000");
	map(0xf000,0xf7ff).bankr("audio2_f000");
	map(0xf800,0xffff).ram();
}

void kof2k2re_hbmame::audio2_io(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(kof2k2re_hbmame::audio2_command_r)).w("soundlatch_m2",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd2",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(kof2k2re_hbmame::audio2_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(kof2k2re_hbmame::audio2_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2_m2",FUNC(generic_latch_8_device::write));
}

void kof2k2re_hbmame::audio3_map(address_map &map)
{
	map(0x0000,0x7fff).bankr("audio3_main");
	map(0x8000,0xbfff).bankr("audio3_8000");
	map(0xc000,0xdfff).bankr("audio3_c000");
	map(0xe000,0xefff).bankr("audio3_e000");
	map(0xf000,0xf7ff).bankr("audio3_f000");
	map(0xf800,0xffff).ram();
}

void kof2k2re_hbmame::audio3_io(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(kof2k2re_hbmame::audio3_command_r)).w("soundlatch_m3",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd3",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(kof2k2re_hbmame::audio3_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(kof2k2re_hbmame::audio3_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2_m3",FUNC(generic_latch_8_device::write));
}

void kof2k2re_hbmame::audio4_map(address_map &map)
{
	map(0x0000,0x7fff).bankr("audio4_main");
	map(0x8000,0xbfff).bankr("audio4_8000");
	map(0xc000,0xdfff).bankr("audio4_c000");
	map(0xe000,0xefff).bankr("audio4_e000");
	map(0xf000,0xf7ff).bankr("audio4_f000");
	map(0xf800,0xffff).ram();
}

void kof2k2re_hbmame::audio4_io(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(kof2k2re_hbmame::audio4_command_r)).w("soundlatch_m4",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd4",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(kof2k2re_hbmame::audio4_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(kof2k2re_hbmame::audio4_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2_m4",FUNC(generic_latch_8_device::write));
}

void kof2k2re_hbmame::ps2(machine_config &config)
{
	gsc(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &kof2k2re_hbmame::ps2_map);

	GENERIC_LATCH_8(config, m2_soundlatch);
	GENERIC_LATCH_8(config, m2_soundlatch2);
	GENERIC_LATCH_8(config, m3_soundlatch);
	GENERIC_LATCH_8(config, m3_soundlatch2);
	GENERIC_LATCH_8(config, m4_soundlatch);
	GENERIC_LATCH_8(config, m4_soundlatch2);

	Z80(config, m2_audiocpu, NEOGEO_AUDIO_CPU_CLOCK);
	m2_audiocpu->set_addrmap(AS_PROGRAM, &kof2k2re_hbmame::audio2_map);
	m2_audiocpu->set_addrmap(AS_IO, &kof2k2re_hbmame::audio2_io);
	Z80(config, m3_audiocpu, NEOGEO_AUDIO_CPU_CLOCK);
	m3_audiocpu->set_addrmap(AS_PROGRAM, &kof2k2re_hbmame::audio3_map);
	m3_audiocpu->set_addrmap(AS_IO, &kof2k2re_hbmame::audio3_io);
	Z80(config, m4_audiocpu, NEOGEO_AUDIO_CPU_CLOCK);
	m4_audiocpu->set_addrmap(AS_PROGRAM, &kof2k2re_hbmame::audio4_map);
	m4_audiocpu->set_addrmap(AS_IO, &kof2k2re_hbmame::audio4_io);

	YM2610(config, m2_ym, NEOGEO_YM2610_CLOCK);
	m2_ym->irq_handler().set_inputline(m2_audiocpu, 0);
	m2_ym->add_route(0, "lspeaker", 0.84);
	m2_ym->add_route(0, "rspeaker", 0.84);
	m2_ym->add_route(1, "lspeaker", 0.98);
	m2_ym->add_route(2, "rspeaker", 0.98);
	YM2610(config, m3_ym, NEOGEO_YM2610_CLOCK);
	m3_ym->irq_handler().set_inputline(m3_audiocpu, 0);
	m3_ym->add_route(0, "lspeaker", 0.84);
	m3_ym->add_route(0, "rspeaker", 0.84);
	m3_ym->add_route(1, "lspeaker", 0.98);
	m3_ym->add_route(2, "rspeaker", 0.98);
	YM2610(config, m4_ym, NEOGEO_YM2610_CLOCK);
	m4_ym->irq_handler().set_inputline(m4_audiocpu, 0);
	m4_ym->add_route(0, "lspeaker", 0.84);
	m4_ym->add_route(0, "rspeaker", 0.84);
	m4_ym->add_route(1, "lspeaker", 0.98);
	m4_ym->add_route(2, "rspeaker", 0.98);
}


ROM_START( kf2k2ps2re )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265ps2re.p1",  0x000000, 0x100000, CRC(e8fd148f) SHA1(d35892e2ac8d3c85ff57d4ca644b93e25aafddf0) )
	ROM_LOAD16_WORD_SWAP( "265ps2re.p2",  0x100000, 0x500000, CRC(1de9efcb) SHA1(7cf4cfc54f881281373f42c4dc48c9e8149c8164) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "265ps2re.p3",  0x000000, 0x020000, CRC(adf44b1d) SHA1(a7a56ce99a728940812fd0678c9d018023f5482c) )

	NEO_SFIX_128K( "265ps2.s1", CRC(714ade47) SHA1(a46115ed89454d8090fae59cfa4aea61a4a81ebf) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1C661A4B)  SHA1(4E5AA862A0A182A806D538996DDC68D9F2DFFAF7) )

	ROM_REGION( 0x30000, "audiocpu2", 0 )
	ROM_LOAD( "214.m1", 0x00000, 0x20000, CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )
	ROM_RELOAD(     0x10000, 0x20000 )

	ROM_REGION( 0x50000, "audiocpu3", 0 )
	ROM_LOAD( "262n.m1", 0x00000, 0x40000, CRC(4BCC537B) SHA1(9FCF1342BCD53D5EEC12C46EE41A51BF543256C2) )
	ROM_RELOAD(     0x10000, 0x40000 )

	ROM_REGION( 0x90000, "audiocpu4", 0 )
	ROM_LOAD( "269dd.m1", 0x00000, 0x80000, CRC(7B7BF462) SHA1(7466A6962DE5242F71B9C52D7BD21A9832115E11) )
	ROM_RELOAD(     0x10000, 0x80000 )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0FC9A58D) SHA1(9D79EF00E2C2ABD9F29AF5521C2FBE5798BF336F) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(B8C475A4) SHA1(10CAF9C69927A223445D2C4B147864C02CE520A8) )
	ROM_REGION( 0x1000000, "ymsnd2:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )
	ROM_REGION( 0x1000000, "ymsnd3:adpcma", 0 )
	ROM_LOAD( "262d.v1", 0x000000, 0x800000, CRC(AC2913BF) SHA1(1721EC3D19684AF702F6C93DA25BB787A5D9DBFF) )
	ROM_LOAD( "262d.v2", 0x800000, 0x800000, CRC(15042F30) SHA1(F92E49110BDE007104590BE1A0FDC8064C216C37) )
	ROM_REGION( 0x1000000, "ymsnd4:adpcma", 0 )
	ROM_LOAD( "269pcbd.v1", 0x000000, 0x800000, CRC(FF64CD56) SHA1(E2754C554ED5CA14C2020C5D931021D5AC82660C) )
	ROM_LOAD( "269pcbd.v2", 0x800000, 0x800000, CRC(A8DD6446) SHA1(8972AAB271C33F8AF344BFFE6359D9DDC4B8AF2E) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265ps2re.c7", 0x3000000, 0x800000, CRC(F0897B93) SHA1(F1C38737B148C459212B61066E8C279852F080B3) )
	ROM_LOAD16_BYTE( "265ps2re.c8", 0x3000001, 0x800000, CRC(8D27A4A6) SHA1(F05A1FEA1B2E542B70B11E58455812E9186D0D77) )
	ROM_LOAD16_BYTE( "265ps2re.c9", 0x4000000, 0x800000, CRC(9939C08A) SHA1(75033A18ECD9177EC6DA00E32FE1E391FDE0BC39) )
	ROM_LOAD16_BYTE( "265ps2re.c10",0x4000001, 0x800000, CRC(C724C069) SHA1(26974478ED31C68A3B987986849C8386F8C5AF8C) )
ROM_END

ROM_START( kf2k2ps2re1 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265ps2re1.p1",  0x000000, 0x100000, CRC(77a0044c) SHA1(57665dcc803d6bf406fb047823aaf71348996b2b) )
	ROM_LOAD16_WORD_SWAP( "265ps2re1.p2",  0x100000, 0x500000, CRC(f9e4456a) SHA1(95e8ebdb7fd0db8ce3116091d160ad1260c2a5e4) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "265ps2re1.p3",  0x000000, 0x020000, CRC(6e6beeba) SHA1(9ab1687c9d8aacaa2626d25b8177b6ae48828674) )

	NEO_SFIX_128K( "265ps2.s1", CRC(714ade47) SHA1(a46115ed89454d8090fae59cfa4aea61a4a81ebf) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1C661A4B)  SHA1(4E5AA862A0A182A806D538996DDC68D9F2DFFAF7) )

	ROM_REGION( 0x30000, "audiocpu2", 0 )
	ROM_LOAD( "214.m1", 0x00000, 0x20000, CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )
	ROM_RELOAD(     0x10000, 0x20000 )

	ROM_REGION( 0x50000, "audiocpu3", 0 )
	ROM_LOAD( "262n.m1", 0x00000, 0x40000, CRC(4BCC537B) SHA1(9FCF1342BCD53D5EEC12C46EE41A51BF543256C2) )
	ROM_RELOAD(     0x10000, 0x40000 )

	ROM_REGION( 0x90000, "audiocpu4", 0 )
	ROM_LOAD( "269dd.m1", 0x00000, 0x80000, CRC(7B7BF462) SHA1(7466A6962DE5242F71B9C52D7BD21A9832115E11) )
	ROM_RELOAD(     0x10000, 0x80000 )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0FC9A58D) SHA1(9D79EF00E2C2ABD9F29AF5521C2FBE5798BF336F) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(B8C475A4) SHA1(10CAF9C69927A223445D2C4B147864C02CE520A8) )
	ROM_REGION( 0x1000000, "ymsnd2:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )
	ROM_REGION( 0x1000000, "ymsnd3:adpcma", 0 )
	ROM_LOAD( "262d.v1", 0x000000, 0x800000, CRC(AC2913BF) SHA1(1721EC3D19684AF702F6C93DA25BB787A5D9DBFF) )
	ROM_LOAD( "262d.v2", 0x800000, 0x800000, CRC(15042F30) SHA1(F92E49110BDE007104590BE1A0FDC8064C216C37) )
	ROM_REGION( 0x1000000, "ymsnd4:adpcma", 0 )
	ROM_LOAD( "269pcbd.v1", 0x000000, 0x800000, CRC(FF64CD56) SHA1(E2754C554ED5CA14C2020C5D931021D5AC82660C) )
	ROM_LOAD( "269pcbd.v2", 0x800000, 0x800000, CRC(A8DD6446) SHA1(8972AAB271C33F8AF344BFFE6359D9DDC4B8AF2E) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265ps2re.c7", 0x3000000, 0x800000, CRC(F0897B93) SHA1(F1C38737B148C459212B61066E8C279852F080B3) )
	ROM_LOAD16_BYTE( "265ps2re.c8", 0x3000001, 0x800000, CRC(8D27A4A6) SHA1(F05A1FEA1B2E542B70B11E58455812E9186D0D77) )
	ROM_LOAD16_BYTE( "265ps2re.c9", 0x4000000, 0x800000, CRC(9939C08A) SHA1(75033A18ECD9177EC6DA00E32FE1E391FDE0BC39) )
	ROM_LOAD16_BYTE( "265ps2re.c10",0x4000001, 0x800000, CRC(C724C069) SHA1(26974478ED31C68A3B987986849C8386F8C5AF8C) )
ROM_END


/*    YEAR  NAME         PARENT    MACHINE        INPUT     CLASS         INIT         MONITOR COMPANY      FULLNAME FLAGS */
GAME( 2018, kf2k2ps2re,  kof2002,  ps2,           neogeo, kof2k2re_hbmame, init_neogeo,    ROT0, "EGCG", "Kof2002 (PlayStation 2 Hack Ver.1.0 Public Test, 2018-10-14)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, kf2k2ps2re1, kof2002,  ps2,           neogeo, kof2k2re_hbmame, init_neogeo,    ROT0, "EGCG", "Kof2002 (PlayStation 2 Hack Ver.1.0, 2018-12-17)", MACHINE_SUPPORTS_SAVE )

