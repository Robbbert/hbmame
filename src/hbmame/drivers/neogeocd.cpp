// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/snk/neogeocd.cpp"

namespace {

class ngcd_hb : public ngcd_state
{
public:
	ngcd_hb(const machine_config &mconfig, device_type type, const char *tag)
		: ngcd_state(mconfig, type, tag)
		, m_cdrom(*this, "cdrom")
	{ }

	void neocd_hb(machine_config &config);

private:
	required_device<cdrom_image_device> m_cdrom;
};


void ngcd_hb::neocd_hb(machine_config &config)
{
	neogeo_base(config);
	neogeo_stereo(config);

	m_maincpu->set_addrmap(AS_PROGRAM, &ngcd_hb::neocd_main_map);
	m_maincpu->set_addrmap(m68000_base_device::AS_CPU_SPACE, &ngcd_hb::neocd_vector_map);

	m_audiocpu->set_addrmap(AS_PROGRAM, &ngcd_hb::neocd_audio_map);
	m_audiocpu->set_addrmap(AS_IO, &ngcd_hb::neocd_audio_io_map);

	m_screen->set_screen_update(FUNC(ngcd_hb::screen_update));

	// temporary until things are cleaned up
	LC89510_TEMP(config, m_tempcdc); // cd controller
	m_tempcdc->set_cdrom_tag("cdrom");
	m_tempcdc->set_is_neoCD(true);
	m_tempcdc->set_type1_interrupt_callback(FUNC(ngcd_hb::interrupt_callback_type1));
	m_tempcdc->set_type2_interrupt_callback(FUNC(ngcd_hb::interrupt_callback_type2));
	m_tempcdc->set_type3_interrupt_callback(FUNC(ngcd_hb::interrupt_callback_type3));

	NVRAM(config, "saveram", nvram_device::DEFAULT_ALL_0);

	NEOGEO_CONTROL_PORT(config, m_ctrl1, neogeo_controls, "joy", false);
	NEOGEO_CONTROL_PORT(config, m_ctrl2, neogeo_controls, "joy", false);

	CDROM(config, m_cdrom).set_interface("cdrom");
	m_cdrom->add_region("cdrom");

	m_ym->set_addrmap(0, &ngcd_hb::neocd_ym_map);
}


#define NEOCDZ_BIOS \
	ROM_REGION16_BE( 0x80000, "mainbios", 0 ) \
	ROM_SYSTEM_BIOS( 0, "official",   "Official BIOS" ) \
	ROMX_LOAD("neocd.bin",    0x00000, 0x80000, CRC(df9de490) SHA1(7bb26d1e5d1e930515219cb18bcde5b7b23e2eda), ROM_GROUPWORD | ROM_REVERSE | ROM_BIOS(0)) \
	ROM_SYSTEM_BIOS( 1, "unibios32", "Universe BIOS (Hack, Ver. 3.2)" ) \
	ROMX_LOAD("uni-bioscd32.rom",    0x00000, 0x80000, CRC(0ffb3127) SHA1(5158b728e62b391fb69493743dcf7abbc62abc82), ROM_GROUPWORD | ROM_REVERSE | ROM_BIOS(1)) \
	ROM_SYSTEM_BIOS( 2, "unibios33", "Universe BIOS (Hack, Ver. 3.3)" ) \
	ROMX_LOAD("uni-bioscd33.rom",    0x00000, 0x80000, CRC(ff3abc59) SHA1(5142f205912869b673a71480c5828b1eaed782a8), ROM_GROUPWORD | ROM_REVERSE | ROM_BIOS(2)) \
	ROM_REGION( 0x20000, "spritegen:zoomy", 0 ) \
	ROM_LOAD( "000-lo.lo", 0x00000, 0x20000, CRC(5a86cff2) SHA1(5992277debadeb64d1c1c64b0a92d9293eaf7e4a) )

ROM_START( cd_blueandr )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "blueandr", 0, SHA1(1e3ea5255c8df92dfdebe060b9a72d8d3fd140dd) )
ROM_END

ROM_START( cd_galx )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "galaxian", 0, SHA1(00edb916464653c14dba6920d2b96867dbb486d1) )
ROM_END

ROM_START( cd_galx2 )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "galaxian2", 0, SHA1(ecaf39c082393ab3274661c7b2386067cf297c01) )
ROM_END

ROM_START( cd_midnight )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "midnight", 0, SHA1(209a3717709d82a5e4fdab30457e0e1894bc24e6) )
ROM_END

ROM_START( cd_pong )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "pong", 0, SHA1(5f2a993bce41f18e6562e0c113cc08fd7bc8060f) )
ROM_END

ROM_START( cd_rickdx )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "rickdx", 0, SHA1(e55a576b7bfabe1b71aff8f0febd8a4963d3104f) )
ROM_END

ROM_START( cd_ssrpg )
	NEOCDZ_BIOS
	DISK_REGION( "cdrom" )
	DISK_IMAGE_READONLY( "ssrpg", 0, SHA1(2bc6a30d305c52e9189efe9840d34370ed260300) )
ROM_END

} // anonymous namespace

GAME( 2026, cd_blueandr, neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Kaiju Wasp", "Blue and Red Fight the Robots v1.7 (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cd_galx,     neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "tcdev", "Galaxians (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cd_galx2,    neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "tcdev", "Galaxians 2 (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2026, cd_midnight, neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Z-team", "Midnight Wanderers (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cd_pong,     neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "unk", "Pong (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2026, cd_rickdx,   neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Z-team", "Rick Dangerous DX (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, cd_ssrpg,    neocdz, neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Apocalypse", "Samurai Spirits RPG (English) (CD)", MACHINE_SUPPORTS_SAVE )


