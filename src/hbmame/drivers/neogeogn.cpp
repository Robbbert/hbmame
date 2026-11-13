// license:GPL_2.0
// copyright-holders:Robbbert
/*****************************************************
This adds support for the GNGeo file format.

The system "nggno" uses the quickload facility
  to load and run loose files and software-list items.

Specific games could be set up to use downloaded
  GNGeo files, however it is not needed at this time.

This driver is not enabled because it doesn't yet work.
  Problems:
  - Protected sets either won't load, or get address error
  - Need to find out what "c-usage" and "s-usage" are for: (regions 10 and 11).
  - Gfx are broken; need to unscramble FIX and SPR

Documentation that's been figured out.

Region numbers:
0 : audio bios (not used)
1 : m1 rom
2 : Encrypted audio (not used)
3 : v1 adpcma
4 : v21 adpcmb
5 : fixed bios (not used)
6 : s1 rom (scrambled)
7 : 68k bios (not used)
8 : px rom
9 : cx rom (scrambled)
10: c-usage
11: s-usage

File header
8 bytes : "gnodmpv1"
8 bytes : name
4 bytes : flags
1 byte  : nb_sec

Each rom has a header
4 bytes : size in bytes
1 byte  : region number as listed above
1 byte  : type
size bytes : the data
******************************************************/

#include "includes/neogeo.h"
//#include "softlist_dev.h"

namespace {

class neogn_class : public neogeo_state
{
public:
	neogn_class(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo_state(mconfig, type, tag)
//		, m_user(*this, "user")
	{ }

	void nggno(machine_config &config);
//	[[maybe_unused]]void init_neogn();

private:
	DECLARE_QUICKLOAD_LOAD_MEMBER(gno_q_cb);
//	optional_memory_region m_user;
};

void neogn_class::nggno(machine_config &config)
{
	mvs(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogn_class::main_map_noslot);

	// quickload
	quickload_image_device &quickload(QUICKLOAD(config, "quickload", "gno", attotime::from_seconds(1)));
	quickload.set_load_callback(FUNC(neogn_class::gno_q_cb));
	quickload.set_interface("gno_quik");
//	SOFTWARE_LIST(config, "quik_list").set_original("gno_quik");
}


QUICKLOAD_LOAD_MEMBER(neogn_class::gno_q_cb)
{
	if (image.length() < 0x60000)
	{
		printf("File too short\n");
		return std::make_pair(image_error::INVALIDLENGTH, "File too short");
	}

	// main header
	// 8 bytes = "gnodmpv1"
	// 8 bytes = game name (we don't use)
	// 4 bytes = flags (we don't use)
	// 1 byte  = number of regions
	uint8_t header[21];
	image.fread( &header, 21);

	if ((header[0] == 'g') && (header[1] == 'n') && (header[2] == 'o') && (header[3] == 'd')
		&& (header[4] == 'm') && (header[5] == 'p') && (header[6] == 'v') && (header[7] == '1'))
	{
	}
	else
	{
		printf("GNO header missing\n");
		return std::make_pair(image_error::UNSPECIFIED, "GNO header missing");
	}

	uint32_t region_size = 0, csize = 0, ssize = 0, ym2_region_size = 0, offset = 21;

	for (uint8_t regions = 0; regions < header[20]; regions++)
	{
		// Get a region header
		// 4 bytes = size
		// 1 byte  = region code
		// 1 byte  = type (we don't use, 00 assumed)
		// size bytes = data
		uint8_t region_code = 0, region_type = 0;
		image.fread( &region_size, 4);
		image.fread( &region_code, 1);
		image.fread( &region_type, 1);
		offset += 6;
		// choose region, check size, and write to it
		printf ("region code = %d with size %08X at offset %08X\n",region_code,region_size,offset);
		offset += region_size;
		switch (region_code)
		{
			case 1:
				if (region_size > (audio_region_size - 0x10000))
				{
					printf("AUDIO region (%08X) in GNO file is larger than supported\n",region_size);
					return std::make_pair(image_error::UNSPECIFIED, "AUDIO region in GNO file is larger than supported");
				}
				else
				{
					image.fread(&audiocpu_region[0x10000], region_size);
					std::copy(&audiocpu_region[0x10000], &audiocpu_region[0x1ffff], &audiocpu_region[0]);
				}
				break;

			case 3:
				if (region_size > ym_region_size)
				{
					printf("ADPCMA size requested (%08X) is greater than available (%08X)\n",region_size,ym_region_size);
					return std::make_pair(image_error::UNSPECIFIED, "ADPCMA region in GNO file is larger than supported");
				}
				else
				{
					image.fread(&ym_region[0], region_size);
					std::copy(&ym_region[0], &ym_region[region_size-1], &memregion("ymsnd:adpcmb")->base()[0]); // fix totc,rotd
				}
				break;

			case 4:
				ym2_region_size = memregion("ymsnd:adpcmb")->bytes();
				if ((region_size > ym2_region_size) || (ym_region_size > ym2_region_size))
				{
					printf("ADPCMB size requested (%08X) is greater than available (%08X)\n",region_size,ym2_region_size);
					return std::make_pair(image_error::UNSPECIFIED, "ADPCMB region in GNO file is larger than supported");
				}
				else
				{
					image.fread(&memregion("ymsnd:adpcmb")->base()[0],region_size);
				}
				break;

			case 6:
				if (region_size > fix_region_size)
				{
					printf("FIX size requested (%08X) is greater than available (%08X)\n",region_size,fix_region_size);
					return std::make_pair(image_error::UNSPECIFIED, "FIX region in GNO file is larger than supported");
				}
				else
				{
					image.fread(&fix_region[0],region_size);
					ssize = region_size;
					// unscramble
					for (uint32_t h = 0; h < ssize; h += 0x20)
					{
						uint8_t t = fix_region[h|0x00];
						// circle 1
						fix_region[h|0x00] = fix_region[h|0x02];
						fix_region[h|0x02] = fix_region[h|0x0a];
						fix_region[h|0x0a] = fix_region[h|0x0b];
						fix_region[h|0x0b] = fix_region[h|0x0f];
						fix_region[h|0x0f] = fix_region[h|0x1f];
						fix_region[h|0x1f] = fix_region[h|0x1d];
						fix_region[h|0x1d] = fix_region[h|0x15];
						fix_region[h|0x15] = fix_region[h|0x14];
						fix_region[h|0x14] = fix_region[h|0x10];
						fix_region[h|0x10] = t;
						// circle 2
						t = fix_region[h|0x01];
						fix_region[h|0x01] = fix_region[h|0x06];
						fix_region[h|0x06] = fix_region[h|0x1a];
						fix_region[h|0x1a] = fix_region[h|0x09];
						fix_region[h|0x09] = fix_region[h|0x07];
						fix_region[h|0x07] = fix_region[h|0x1e];
						fix_region[h|0x1e] = fix_region[h|0x19];
						fix_region[h|0x19] = fix_region[h|0x05];
						fix_region[h|0x05] = fix_region[h|0x16];
						fix_region[h|0x16] = fix_region[h|0x18];
						fix_region[h|0x18] = t;
						// circle 3
						t = fix_region[h|0x03];
						fix_region[h|0x03] = fix_region[h|0x0e];
						fix_region[h|0x0e] = fix_region[h|0x1b];
						fix_region[h|0x1b] = fix_region[h|0x0d];
						fix_region[h|0x0d] = fix_region[h|0x17];
						fix_region[h|0x17] = fix_region[h|0x1c];
						fix_region[h|0x1c] = fix_region[h|0x11];
						fix_region[h|0x11] = fix_region[h|0x04];
						fix_region[h|0x04] = fix_region[h|0x12];
						fix_region[h|0x12] = fix_region[h|0x08];
						fix_region[h|0x08] = t;
						// circle 4
						t = fix_region[h|0x13];
						fix_region[h|0x13] = fix_region[h|0x0c];
						fix_region[h|0x0c] = t;
					}
				}
				break;

			case 8:
				if (region_size > cpuregion_size)
				{
					printf("CPU size requested (%08X) is greater than available (%08X)\n",region_size,cpuregion_size);
					return std::make_pair(image_error::UNSPECIFIED, "CPU region in GNO file is larger than supported");
				}
				else
				{
					image.fread(&cpuregion[0],region_size);
				}
				break;

			case 9:
				if (region_size > spr_region_size)
				{
					printf("SPR size requested (%08X) is greater than available (%08X)\n",region_size,spr_region_size);
					return std::make_pair(image_error::UNSPECIFIED, "SPR region in GNO file is larger than supported");
				}
				else
				{
					image.fread(&spr_region[0],region_size);
					csize = region_size;
					// unscramble
				}
				break;

			default:
				// Unsupported regions:
				// 0  - audio bios (not used)
				// 2  - audio encrypted (not used)
				// 5  - fix bios (not used)
				// 7  - 68k bios (not used)
				// 10 - C(SPR) usage
				// 11 - S(FIX) usage
				printf("...ignored\n");
				// jump past this region, point to next one
				image.fseek(region_size, SEEK_CUR);
		}
	}

	// Prepare the system
	printf("Ready to start\n");fflush(stdout);
	init_neogeo();
	m_sprgen->set_sprite_region(m_region_sprites->base(), csize); // fix wh2
	m_sprgen->set_fixed_regions(m_region_fixed->base(), ssize, m_region_fixedbios);
	m_sprgen->optimize_sprite_data(); // fix sprites

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

	m_audiocpu->reset();
	machine_reset();

	return std::make_pair(std::error_condition(), std::string());
}

ROM_START( nggno )
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
GAME( 1990, nggno,  neogeo,      nggno,     neogeo,         neogn_class, empty_init,  ROT0, "SNK", "Neo-Geo .gno support", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )

