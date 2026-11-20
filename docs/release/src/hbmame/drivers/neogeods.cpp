// license:GPL_2.0
// copyright-holders:Robbbert
/*****************************************************
  This adds support for the darksoft file format.

  The system "ngmvs" uses the quickload facility
  to load and run loose files and software-list items.

  Specific games could be set up to use downloaded
  darksoft composite files, however it is not needed
  at this time.
******************************************************/

#include "includes/neogeo.h"
#include "unzip.h"
//#include "softlist_dev.h"

namespace {

class neods_class : public neogeo_state
{
public:
	neods_class(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo_state(mconfig, type, tag)
//		, m_user(*this, "user")
	{ }

	void ngmvs(machine_config &config);
//	[[maybe_unused]]void init_neods();

private:
	DECLARE_QUICKLOAD_LOAD_MEMBER(mvs_q_cb);
	uint32_t open7z(std::string, std::string, uint8_t*, uint32_t);
//	optional_memory_region m_user;
};

uint32_t neods_class::open7z(std::string zip_name, std::string filename, uint8_t *region_name, uint32_t region_size)
{
	uint32_t file_size = 0U;
	util::archive_file::ptr zip;
	std::error_condition ziperr{};

	// look into 7z file
	ziperr = util::archive_file::open_7z(zip_name, zip);

	if (!ziperr)
	{
		int found = zip->search(filename, false);

		if (found >= 0)
		{
			ziperr = zip->decompress(&region_name[0], region_size);

			if (!ziperr)
				file_size = zip->current_uncompressed_length();
		}

		zip.reset();
	}

	return file_size;
}

QUICKLOAD_LOAD_MEMBER(neods_class::mvs_q_cb)
{
	uint32_t psize = 0U, ssize = 0U, msize = 0U, vsize = 0U, csize = 0U, fsize = 0U;
	std::string fname = "prom", sstr = ".7z", zipname = image.filename();
	// assuming that first .7z is the only occurence
	std::size_t found = zipname.find(sstr);
	if (found != std::string::npos)
	{
		zipname.erase(found+3);
		//printf("%s\n",zipname.c_str());fflush(stdout);

		// The protected sets also have "prom1", but we don't support it.
		psize = open7z(zipname, fname, &cpuregion[0], cpuregion_size);
	}
	if (!psize)
	{
		printf("File is missing or unusable\n");
		return std::make_pair(image_error::UNSPECIFIED, "File is missing or unusable");
	}

	fname = "srom";
	ssize = open7z(zipname, fname, &fix_region[0], fix_region_size);

	fname = "m1rom";
	msize = open7z(zipname, fname, &audiocpu_region[0x10000], audio_region_size - 0x10000);
	if (msize)
		std::copy(&audiocpu_region[0x10000], &audiocpu_region[0x1ffff], &audiocpu_region[0]);

	fname = "vroma0";
	vsize = open7z(zipname, fname, &ym_region[0], ym_region_size);
	if (vsize)
		std::copy(&ym_region[0], &ym_region[ym_region_size-1], &memregion("ymsnd:adpcmb")->base()[0]);
	// Assume that the ADPCMB rom would be called "vromb0", but no current sets use it.

	fname = "crom0";
	csize = open7z(zipname, fname, &spr_region[0], spr_region_size);
	if (csize)
		for (uint32_t i = 0; i < csize; i+=4)
			std::swap(spr_region[i+1], spr_region[i+2]);

	init_neogeo();
	m_sprgen->set_sprite_region(m_region_sprites->base(), csize);
	m_sprgen->set_fixed_regions(m_region_fixed->base(), ssize, m_region_fixedbios);
	m_sprgen->optimize_sprite_data();

	// Fix the 512k text with horrible game-specific stuff
	if (ssize > 0x20000)
	{
		uint16_t game = cpuregion[0x109] * 256 + cpuregion[0x108];
		// identify kof2000, matrim, svc, kof2003
		if ((game == 0x257) || (game == 0x266) || (game == 0x269) || (game == 0x271))
			m_sprgen->m_fixed_layer_bank_type = 2;
		else
			m_sprgen->m_fixed_layer_bank_type = 1;
	}

	// This file is optionally included, perhaps to signify how to handle protection?
	// The contents are an ascii number, so for now just print it out.
	fname = "fpga";
	unsigned char buffer[8] = {};
	fsize = open7z(zipname, fname, &buffer[0], 4);
	if (fsize)
		printf("FPGA code = %s\n",buffer);

	m_audiocpu->reset();
	machine_reset();

	return std::make_pair(std::error_condition(), std::string());
}

void neods_class::ngmvs(machine_config &config)
{
	mvs(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neods_class::main_map_noslot);

	// quickload
	quickload_image_device &quickload(QUICKLOAD(config, "quickload", "7z", attotime::from_seconds(1)));
	quickload.set_load_callback(FUNC(neods_class::mvs_q_cb));
	quickload.set_interface("mvs_quik");
	//SOFTWARE_LIST(config, "quik_list").set_original("mvs_quik");    // not working
}

ROM_START( ngmvs )
	NEOGEO_BIOS

	ROM_REGION( 0x900000, "maincpu", ROMREGION_ERASEFF )

	ROM_REGION( 0x20000, "audiobios", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_REGION( 0x90000, "audiocpu", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_Y_ZOOM

	ROM_REGION( 0x80000, "fixed", ROMREGION_ERASEFF )

	ROM_REGION( 0x20000, "fixedbios", 0 )
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", ROMREGION_ERASEFF )

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", ROMREGION_ERASEFF )

	ROM_REGION( 0x4000000, "sprites", ROMREGION_ERASEFF )
ROM_END

} // anonymous namespace

//    YEAR  NAME    PARENT       MACHINE    INPUT           CLASS         INIT
GAME( 1990, ngmvs,  neogeo,      ngmvs,     neogeo,         neods_class, empty_init,  ROT0, "SNK", "Neo-Geo MultiMVS support", MACHINE_SUPPORTS_SAVE )

