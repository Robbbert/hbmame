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
	ROM_LOAD( "000-lo.lo", 0x00000, 0x20000, CRC(5a86cff2) SHA1(5992277debadeb64d1c1c64b0a92d9293eaf7e4a) ) \
	DISK_REGION( "cdrom" )

ROM_START( cd_alice )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "819", 0, SHA1(a9dfafadc121be3ea991a0a85377622d9a3a535f) )
ROM_END

ROM_START( cd_chelnov )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "chelnov", 0, SHA1(45d1902421e97e642d02a485dcd06f9780d94fd1) )
ROM_END

ROM_START( cd_diff )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "820", 0, SHA1(04594dcd984f21203bb5489c4612d28a93d6c7cd) )
ROM_END

ROM_START( cd_eira )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "817", 0, SHA1(c441932e9566f0647888ceeff3229622ae9609a8) )
ROM_END

ROM_START( cd_ffury )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "ffuryae", 0, SHA1(c0e0e4180bddc3691ab59e5e2900ab3dcc9080f7) )
ROM_END

ROM_START( cd_fukkireta )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "822", 0, SHA1(57fd82eb079a4e3951b769bd6cc9afa84394aaaa) )
ROM_END

ROM_START( cd_gaxe )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "gaxe", 0, SHA1(3b7c9d7e1d3390a3610e436edf4425b9a40859b6) )
ROM_END

ROM_START( cd_gaxearr )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "gaxearr", 0, SHA1(39bf69b8dfd73c2033a09f3e518db45194d2d7b8) )
ROM_END

ROM_START( cd_natademo )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "818", 0, SHA1(22ac0c741defe9d792b9f38cd48dd0742140ec0a) )
ROM_END

ROM_START( cd_ngdark )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "805", 0, SHA1(940b8b162ddf98cafebbd0bd7a3e43fae455d0cf) )
ROM_END

ROM_START( cd_noneon )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "823", 0, SHA1(daa286dc1e9c651e3f54802fd632f03dce10f3e4) )
ROM_END

ROM_START( cd_ntris3d )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "825", 0, SHA1(a06e149504f62dc94ecfe46d0d094bb9ca9f79a7) )
ROM_END

ROM_START( cd_phoenix )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "815", 0, SHA1(109fa573867236750396fcbf1037d47a90027415) )
ROM_END

ROM_START( cd_pong )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "827", 0, SHA1(5f2a993bce41f18e6562e0c113cc08fd7bc8060f) )
ROM_END

ROM_START( cd_pow )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "pow", 0, SHA1(fb3f0f0383ff0f4ac99ad64b2be997e68fd7ffe0) )
ROM_END

ROM_START( cd_pow2 )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "pow2", 0, SHA1(9017ead20ecbfeb25c6c36282423c90375403b32) )
ROM_END

ROM_START( cd_shinobiarr )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "shinobiarr", 0, SHA1(3f93364dc54a91eadeddf52675a7043b191675ba) )
ROM_END

ROM_START( cd_speedball )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "829", 0, SHA1(550726ee523e6dc32a961e4cdbc63a6c2e2a4bc8) )
ROM_END

ROM_START( cd_ssrpg )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "828", 0, SHA1(2bc6a30d305c52e9189efe9840d34370ed260300) )
ROM_END

ROM_START( cd_subspecies )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "824", 0, SHA1(59a23f8a30d8b0422734f8609f3ebea251de94e1) )
ROM_END

ROM_START( cd_visualnovel )
	NEOCDZ_BIOS
	DISK_IMAGE_READONLY( "821", 0, SHA1(70cde7435cec39241ff5f5d704f9f58193c6f1ac) )
ROM_END

} // anonymous namespace

GAME( 2016, cd_alice,       neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Blastar", "We Love Alice demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2024, cd_chelnov,     neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "iq_132", "Atomic Runner Chelnov (Neo-Geo port)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2016, cd_diff,        neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Citavia", "DIFF demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2015, cd_eira,        neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Resistance", "Eira demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cd_ffury,       neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "[unk]", "Fatal Fury - Album Edition[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2017, cd_fukkireta,   neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Visy", "Neo Fukkireta demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2025, cd_gaxe,        neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "h0ffman", "Golden Axe (Neo-Geo port)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2025, cd_gaxearr,     neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "h0ffman", "Golden Axe (v1.0) (PS2 Sega Ages Arranged)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2016, cd_natademo,    neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "BEY", "NATADEMO Rel.05[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2011, cd_ngdark,      neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "NGD::Devgroup", "NGD::ARK(2012-05-04)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2019, cd_noneon,      neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Citavia", "No Neo - No Party demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2021, cd_ntris3d,     neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Blastar", "N*tris3D Techdemo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2016, cd_phoenix,     neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "BEY", "Phoenix(v013)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cd_pong,        neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "David Vandensteen", "Pong[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2024, cd_pow,         neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "iq_132", "P.O.W. (Neo-Geo port)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2025, cd_pow2,        neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "iq_132", "P.O.W. (update)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2026, cd_shinobiarr,  neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "H0ffman", "Shinobi Arranged v1.1 (Neo-Geo port)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2026, cd_speedball,   neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Z-team", "Speedball2 v1.0.0 demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2016, cd_ssrpg,       neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Apocalypse", "Samurai Spirits RPG (English)[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2020, cd_subspecies,  neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Blastar", "Subspecies_Unfinished demo[CD]", MACHINE_SUPPORTS_SAVE )
GAME( 2016, cd_visualnovel, neocdz,  neocd_hb, neocd,  ngcd_hb,  init_neocdz,  ROT0, "Trilobit", "Visual Novel demo (2016-03-31)[CD]", MACHINE_SUPPORTS_SAVE )


