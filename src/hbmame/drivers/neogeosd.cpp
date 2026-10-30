// license:GPL_2.0
// copyright-holders:Robbbert
/*****************************************************
  This adds support for .neo files, as used in the
  NeoSD Pro. This format is becoming increasingly
  popular in the homebrew scene.

  The system "ngneo" uses the quickload facility
  to load and run loose files and software-list items.

  Specific games can be set up to use downloaded
  neo files, so it is no longer necessary to convert
  them to roms.

  If you want to convert neo to roms, uncomment the
  define just below, compile, and load your neo rom
  with "ngneo". On exit, you'll find the extracted
  roms in your hbmame folder. The files names start
  with 555.
******************************************************/
#include "includes/neogeo.h"
#include "softlist_dev.h"

//#define DUMP_NEO_ROMS

namespace {

class neosd_class : public neogeo_state
{
public:
	neosd_class(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo_state(mconfig, type, tag)
		, m_user(*this, "user")
	{ }

	void ngneo(machine_config &config);
	[[maybe_unused]]void init_neosd();

private:
	DECLARE_QUICKLOAD_LOAD_MEMBER(neo_q_cb);
	optional_memory_region m_user;
};

void neosd_class::ngneo(machine_config &config)
{
	mvs(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neosd_class::main_map_noslot);

	// quickload
	quickload_image_device &quickload(QUICKLOAD(config, "quickload", "neo", attotime::from_seconds(1)));
	quickload.set_load_callback(FUNC(neosd_class::neo_q_cb));
	quickload.set_interface("neo_quik");
	SOFTWARE_LIST(config, "quik_list").set_original("neo_quik");
}

QUICKLOAD_LOAD_MEMBER(neosd_class::neo_q_cb)
{
	if (image.length() < 0x60000)
	{
		printf("File too short\n");
		return std::make_pair(image_error::INVALIDLENGTH, "File too short");
	}

	// check header
	u8 header[0x1000];
	image.fread( &header, 0x1000);

	if ((header[0] == 'N') && (header[1] == 'E') && (header[2] == 'O'))
	{
	}
	else
	{
		printf("NEO header missing\n");
		return std::make_pair(image_error::UNSPECIFIED, "NEO header missing");
	}

	// Get file sizes
	u32 psize = header[4] + header[5]*0x100 + header[6]*0x10000 + header[7]*0x1000000;
	u32 ssize = header[8] + header[9]*0x100 + header[10]*0x10000 + header[11]*0x1000000;
	u32 msize = header[12] + header[13]*0x100 + header[14]*0x10000 + header[15]*0x1000000;
	u32 vsize = header[16] + header[17]*0x100 + header[18]*0x10000 + header[19]*0x1000000;
	u32 v2size = header[20] + header[21]*0x100 + header[22]*0x10000 + header[23]*0x1000000;
	u32 csize = header[24] + header[25]*0x100 + header[26]*0x10000 + header[27]*0x1000000;

	// Make sure file is big enough
	u64 total = 0x1000 + psize + ssize + msize + vsize + v2size + csize;
	if (total > image.length())
	{
		printf("File is corrupt.\n");
		return std::make_pair(image_error::UNSPECIFIED, "File is corrupt");
	}

	// Make sure regions are big enough
	if (psize > cpuregion_size)
	{
		printf("CPU size requested (%08X) is greater than available (%08X)\n",psize,cpuregion_size);
		return std::make_pair(image_error::UNSPECIFIED, "CPU region in NEO file is larger than supported");
	}

	if (ssize > fix_region_size)
	{
		printf("FIX size requested (%08X) is greater than available (%08X)\n",ssize,fix_region_size);
		return std::make_pair(image_error::UNSPECIFIED, "FIX region in NEO file is larger than supported");
	}

	if (vsize > ym_region_size)
	{
		printf("ADPCMA size requested (%08X) is greater than available (%08X)\n",vsize,ym_region_size);
		return std::make_pair(image_error::UNSPECIFIED, "ADPCMA region in NEO file is larger than supported");
	}

	u32 ym2_region_size = memregion("ymsnd:adpcmb")->bytes();
	if ((v2size > ym2_region_size) || (ym_region_size > ym2_region_size))
	{
		printf("ADPCMB size requested (%08X) is greater than available (%08X)\n",v2size,ym2_region_size);
		return std::make_pair(image_error::UNSPECIFIED, "ADPCMB region in NEO file is larger than supported");
	}

	if (msize > (audio_region_size - 0x10000))
	{
		printf("AUDIO region (%08X) in NEO file is larger than supported\n",msize);
		return std::make_pair(image_error::UNSPECIFIED, "AUDIO region in NEO file is larger than supported");
	}

	if (csize > spr_region_size)
	{
		printf("SPR size requested (%08X) is greater than available (%08X)\n",csize,spr_region_size);
		return std::make_pair(image_error::UNSPECIFIED, "SPR region in NEO file is larger than supported");
	}

	// copy the data from the NEO file to the regions
	printf("psize=%X\n",psize);fflush(stdout);
	if (psize)
		image.fread(&cpuregion[0],psize);

	printf("ssize=%X\n",ssize);fflush(stdout);
	if (ssize)
		image.fread(&fix_region[0],ssize);

	printf("msize=%X\n",msize);fflush(stdout);
	if (msize)
	{
		image.fread(&audiocpu_region[0x10000],msize);
		std::copy(&audiocpu_region[0x10000], &audiocpu_region[0x1ffff], &audiocpu_region[0]);
	}

	printf("vsize=%X\n",vsize);fflush(stdout);
	if (vsize)
	{
		image.fread(&ym_region[0],vsize);
		std::copy(&ym_region[0], &ym_region[vsize-1], &memregion("ymsnd:adpcmb")->base()[0]); // fix totc,rotd
	}

	printf("v2size=%X\n",v2size);fflush(stdout);
	if (v2size)
		image.fread(&memregion("ymsnd:adpcmb")->base()[0],v2size);

	printf("csize=%X\n",csize);fflush(stdout);
	if (csize)
		image.fread(&spr_region[0],csize);

// Save the file parts if needed
#ifdef DUMP_NEO_ROMS
	FILE *f;
	if (psize)
	{
		f = fopen("555.p1", "wb");
		if (!f)
		{
			printf("Error opening file '555.p1'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		fwrite(&cpuregion[0], 1, psize, f);
		fclose(f);
	}

	if (ssize)
	{
		f = fopen("555.s1", "wb");
		if (!f)
		{
			printf("Error opening file '555.s1'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		fwrite(&fix_region[0], 1, ssize, f);
		fclose(f);
	}

	if (msize)
	{
		f = fopen("555.m1", "wb");
		if (!f)
		{
			printf("Error opening file '555.m1'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		fwrite(&audiocpu_region[0x10000], 1, msize, f);
		fclose(f);
	}

	if (vsize)
	{
		f = fopen("555.v1", "wb");
		if (!f)
		{
			printf("Error opening file '555.v1'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		fwrite(&ym_region[0], 1, vsize, f);
		fclose(f);
	}

	if (v2size)
	{
		f = fopen("555.v21", "wb");
		if (!f)
		{
			printf("Error opening file '555.v21'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		fwrite(&memregion("ymsnd:adpcmb")->base()[0], 1, v2size, f);
		fclose(f);
	}

	if (csize)
	{
		uint32_t i;
		f = fopen("555.c1", "wb");
		if (!f)
		{
			printf("Error opening file '555.c1'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		for (i = 0; i < csize; i+=2)
			fwrite(&spr_region[i], 1, 1, f);
		fclose(f);

		f = fopen("555.c2", "wb");
		if (!f)
		{
			printf("Error opening file '555.c2'\n");
			return std::make_pair(image_error::UNSPECIFIED, " ");
		}
		for (i = 1; i < csize; i+=2)
			fwrite(&spr_region[i], 1, 1, f);
		if (csize & 1)
			fwrite(&spr_region[i], 1, 1, f); // extra byte to keep both files the same size
		fclose(f);
	}
#endif

	// Prepare the system
	printf("Ready to start\n");fflush(stdout);
	init_neogeo();
	m_sprgen->set_sprite_region(m_region_sprites->base(), csize); // fix wh2
	m_sprgen->set_fixed_regions(m_region_fixed->base(), ssize, m_region_fixedbios);
	m_sprgen->optimize_sprite_data(); // fix sprites

	// Fix the 512k text with horrible game-specific stuff
	if (ssize > 0x20000)
	{
		u16 game = cpuregion[0x109] * 256 + cpuregion[0x108];
		// identify kof2000, matrim, svc, kof2003
		if ((game == 0x257) || (game == 0x266) || (game == 0x269) || (game == 0x271))
			m_sprgen->m_fixed_layer_bank_type = 2;
		else
			m_sprgen->m_fixed_layer_bank_type = 1;
	}

	m_audiocpu->reset();
	machine_reset();

	return std::make_pair(std::error_condition(), std::string());
}

void neosd_class::init_neosd()
{
	if (!m_user)
	{
		printf("NEOSD region not found\n");
		return;
	}

	if (m_user->bytes() < 0x60000)
	{
		printf("File too short\n");
		return;
	}

	// check header
	u8 header[0x1000];
	memcpy(header, m_user->base(), 0x1000);

	if ((header[0] == 'N') && (header[1] == 'E') && (header[2] == 'O'))
	{
	}
	else
	{
		printf("NEO header missing\n");
		return;
	}

	// Get file sizes
	u32 psize = header[4] + header[5]*0x100 + header[6]*0x10000 + header[7]*0x1000000;
	u32 ssize = header[8] + header[9]*0x100 + header[10]*0x10000 + header[11]*0x1000000;
	u32 msize = header[12] + header[13]*0x100 + header[14]*0x10000 + header[15]*0x1000000;
	u32 vsize = header[16] + header[17]*0x100 + header[18]*0x10000 + header[19]*0x1000000;
	u32 v2size = header[20] + header[21]*0x100 + header[22]*0x10000 + header[23]*0x1000000;
	u32 csize = header[24] + header[25]*0x100 + header[26]*0x10000 + header[27]*0x1000000;

	// Make sure file is big enough
	u64 total = 0x1000 + psize + ssize + msize + vsize + v2size + csize;
	if (total > m_user->bytes())
	{
		printf("File is corrupt.\n");
		return;
	}

	// Make sure regions are big enough
	if (psize > cpuregion_size)
	{
		printf("CPU size requested (%08X) is greater than available (%08X)\n",psize,cpuregion_size);
		return;
	}

	if (ssize > fix_region_size)
	{
		printf("FIX size requested (%08X) is greater than available (%08X)\n",ssize,fix_region_size);
		return;
	}

	if (vsize > ym_region_size)
	{
		printf("ADPCMA size requested (%08X) is greater than available (%08X)\n",vsize,ym_region_size);
		return;
	}

	u32 ym2_region_size = memregion("ymsnd:adpcmb")->bytes();
	if ((v2size > ym2_region_size) || (ym_region_size > ym2_region_size))
	{
		printf("ADPCMB size requested (%08X) is greater than available (%08X)\n",v2size,ym2_region_size);
		return;
	}

	if (msize > (audio_region_size - 0x10000))
	{
		printf("AUDIO region (%08X) in NEO file is larger than supported\n",msize);
		return;
	}

	if (csize > spr_region_size)
	{
		printf("SPR size requested (%08X) is greater than available (%08X)\n",csize,spr_region_size);
		return;
	}

	uint32_t user_ptr = 0x1000;

	// copy the data from the NEO file to the regions
	printf("psize=%X\n",psize);fflush(stdout);
	if (psize)
	{
		memcpy(&cpuregion[0], &m_user->base()[user_ptr], psize);
		user_ptr += psize;
	}

	printf("ssize=%X\n",ssize);fflush(stdout);
	if (ssize)
	{
		memcpy(&fix_region[0], &m_user->base()[user_ptr], ssize);
		user_ptr += ssize;
	}

	printf("msize=%X\n",msize);fflush(stdout);
	if (msize)
	{
		memcpy(&audiocpu_region[0x10000], &m_user->base()[user_ptr], msize);
		std::copy(&audiocpu_region[0x10000], &audiocpu_region[0x1ffff], &audiocpu_region[0]);
		user_ptr += msize;
	}

	printf("vsize=%X\n",vsize);fflush(stdout);
	if (vsize)
	{
		memcpy(&ym_region[0], &m_user->base()[user_ptr], vsize);
		std::copy(&ym_region[0], &ym_region[vsize-1], &memregion("ymsnd:adpcmb")->base()[0]); // fix totc,rotd
		user_ptr += vsize;
	}

	printf("v2size=%X\n",v2size);fflush(stdout);
	if (v2size)
	{
		memcpy(&memregion("ymsnd:adpcmb")->base()[0], &m_user->base()[user_ptr], v2size);
		user_ptr += v2size;
	}

	printf("csize=%X\n",csize);fflush(stdout);
	if (csize)
		memcpy(&spr_region[0], &m_user->base()[user_ptr], csize);

	// Prepare the system
	printf("Ready to start\n");fflush(stdout);
	init_neogeo();
	m_sprgen->set_sprite_region(m_region_sprites->base(), csize); // fix wh2
	m_sprgen->set_fixed_regions(m_region_fixed->base(), ssize, m_region_fixedbios);
	m_sprgen->optimize_sprite_data(); // fix sprites

	// Fix the 512k text with horrible game-specific stuff
	if (ssize > 0x20000)
	{
		u16 game = cpuregion[0x109] * 256 + cpuregion[0x108];
		// identify kof2000, matrim, svc, kof2003
		if ((game == 0x257) || (game == 0x266) || (game == 0x269) || (game == 0x271))
			m_sprgen->m_fixed_layer_bank_type = 2;
		else
			m_sprgen->m_fixed_layer_bank_type = 1;
	}

	return;
}

#define NEOSD_BASE \
	NEOGEO_BIOS \
	ROM_REGION( 0x900000, "maincpu", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x90000, "audiocpu", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_Y_ZOOM \
	ROM_REGION( 0x80000, "fixed", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x1000000, "ymsnd:adpcma", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x1000000, "ymsnd:adpcmb", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x8000000, "sprites", ROMREGION_ERASEFF )

ROM_START( ngneo )
	NEOSD_BASE
ROM_END

ROM_START( sd_test )
	NEOSD_BASE
	ROM_REGION( 0x18c1000, "user", 0 )
	ROM_LOAD( "jimpower.neo", 0x000000, 0x18c1000, CRC(223ddf7a) SHA1(70c4868f5df9250c5f2e65bf77cb717fd778226e) )
ROM_END


} // anonymous namespace

/*    YEAR  NAME        PARENT    MACHINE   INPUT    CLASS         INIT    */
GAME( 1990, ngneo,      neogeo,   ngneo,    neogeo,  neosd_class,  empty_init,  ROT0, "SNK", "Neo-Geo .neo support", MACHINE_SUPPORTS_SAVE )
GAME( 2026, sd_test,    neogeo,   ngneo,    neogeo,  neosd_class,  init_neosd,  ROT0, "SNK", "Jim Power test", MACHINE_SUPPORTS_SAVE )

