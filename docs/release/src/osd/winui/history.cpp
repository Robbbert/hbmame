// For licensing and usage information, read docs/release/winui_license.txt
//  MASTER
/***************************************************************************

  history.cpp

 *   history functions.
 *      History database engine
 *      Collect all information on the selected driver, and return it as
 *         a string. Called by winui.cpp

 *      Token parsing by Neil Bradley
 *      Modifications and higher-level functions by John Butler

 *      Further work by Mamesick and Robbbert

 *      Completely rewritten by Robbbert in July 2017
 *      Notes:
 *      - The order listed in m_gameInfo is the order the data is displayed.
 *      - The other tables must have the files in the same places so that
 *             the index numbers line up. Anything with NULL indicates an
 *             unsupported option (the file doesn't contain the info).
 *      - Each table must contain at least MAX_HFILES members (extra lines
 *             are ignored)
 *      - Software comes first, followed by Game then Source.
 
 *      2023-06: Robbbert
 *      - Added support for history.xml
 *      - Dropped support for history.dat
 *      - HBMAME: Dropped support for messinfo.dat, sysinfo.dat, story.dat, marp.dat
 
***************************************************************************/
// license:BSD-3-Clause
// copyright-holders:Chris Kirmse, Mike Haaland, René Single, Mamesick, Robbbert

#include <windows.h>
#include <fstream>

// MAME/MAMEUI headers
#include "emu.h"
#include "screen.h"
#include "speaker.h"
#include "drivenum.h"
#include "mui_util.h"
#include "mui_opts.h"
#include "emu_opts.h"
#include "sound/samples.h"
#include "path.h"


/****************************************************************************
 *      struct definitions
 ****************************************************************************/
typedef struct
{
	LPCSTR   filename;
	LPCSTR   header;
	LPCSTR   descriptor;
	bool     bClone;    // if nothing found for a clone, try the parent
}
HGAMEINFO;

typedef struct
{
	LPCSTR   filename;
	LPCSTR   header;
	LPCSTR   descriptor;
}
HSOURCEINFO;

/*************************** START CONFIGURABLE AREA *******************************/
// number of dats we support
#define MAX_HFILES 4
// The order of these is the order they are displayed
const HGAMEINFO m_gameInfo[MAX_HFILES] =
{
	{ "history.xml",  "\n**** :HISTORY: ****\n\n",          "<text>",   1 },
	{ "mameinfo.dat", "\n**** :MAMEINFO: ****\n\n",         "$mame",  1 },
	{ "gameinit.dat", "\n**** :GAMEINIT: ****\n\n",         "$mame",  1 },
	{ "command.dat",  "\n**** :COMMANDS: ****\n\n",         "$cmd",   1 },
};

const HSOURCEINFO m_sourceInfo[MAX_HFILES] =
{
	{ NULL },
	{ "mameinfo.dat", "\n***:MAMEINFO DRIVER: ",  "$drv" },
	{ NULL },
	{ NULL },
};

const HSOURCEINFO m_swInfo[MAX_HFILES] =
{
	{ "history.xml",  "\n**** :HISTORY item: ",     "<text>" },
	{ NULL },
	{ NULL },
	{ NULL },
};

/*************************** END CONFIGURABLE AREA *******************************/

int file_sizes[MAX_HFILES] = { 0, };
std::map<std::string, std::streampos> mymap[MAX_HFILES];
const size_t npos = std::string::npos;

static void create_index_history(const char* datsdir, std::ifstream &fp, std::string file_line, int filenum)
{
	int what_to_do = 0;  // 0 = xml not found; 1 = idx not found or version mismatch; 2 = both ok, need to read idx.
	const std::string text1 = "<system name=", text2 = "<item list=", text4 = "\"";
	std::string xml_ver, idx_ver;
	size_t quot1 = npos, quot2 = npos;

	if (!fp.good())
		return;  // xml not found
	// get version from line 2
	std::getline(fp, file_line);
	quot1 = file_line.find(text4);
	if (quot1 != npos)
	{
		quot1++;
		quot2 = file_line.find(text4, quot1);
		if (quot2 != npos)
		{
			xml_ver = file_line.substr(quot1, quot2-quot1);
			//printf("XML-VER = %s\n",xml_ver.c_str());fflush(stdout);
		}
	}

	std::string buf, filename = datsdir + std::string("\\history.idx");
	std::ifstream fi (filename);
	if (!fi.good())
		what_to_do = 1;  // idx not found
	else
	// get version from line 1
	{
		std::getline(fi, idx_ver);
		//printf("IDX-VER = %s\n",idx_ver.c_str());fflush(stdout);
		if (xml_ver == idx_ver)
			what_to_do = 2;
	}

	//printf("*******2 %d\n",what_to_do);fflush(stdout);
	// 2: just read idx into map
	if (what_to_do == 2)
	{
		std::getline(fi, file_line);
		while (fi.good())
		{
			quot1 = file_line.find("=");
			//std::string t4 = file_line.substr(0, quot1);printf("*** %s\n",t4.c_str());fflush(stdout);
			//u32 t5 = stoi(file_line.substr(quot1+1));printf("*** %d\n",t5);fflush(stdout);
			mymap[filenum][file_line.substr(0, quot1)] = stoi(file_line.substr(quot1+1));
			std::getline(fi, file_line);
		}
		fi.close();
		return;
	}

	// 1: create idx then save it
	fi.close();
	std::streampos key_position = file_line.size() + 2, text_position = 0U; // tellg is buggy, this works and is faster
	while (fp.good())
	{
		size_t find = file_line.find(text1);
		std::string final_key;
		if (find != npos)   // found a system
		{
			// Find position of the 2 double-quotes
			quot1 = file_line.find(text4);
			if (quot1 != npos)
			{
				quot1++;
				quot2 = file_line.find(text4, quot1);
				if (quot2 != npos)
					final_key = file_line.substr(quot1, quot2-quot1);
			}
		}
		else
		{
			find = file_line.find(text2);
			if (find != npos)   // found a sw-item
			{
				// Find position of the 4 double-quotes
				quot1 = file_line.find(text4), quot2 = npos;
				if (quot1 != npos)
				{
					quot1++;
					quot2 = file_line.find(text4, quot1);
					if (quot2 != npos)
					{
						std::string first = file_line.substr(quot1, quot2-quot1);
						quot1 = file_line.find(text4, quot2+1);
						if (quot1 != npos)
						{
							quot1++;
							quot2 = file_line.find(text4, quot1);
							if (quot2 != npos)
							{
								std::string second = file_line.substr(quot1, quot2-quot1);
								final_key = first + std::string(":") + second;
							}
						}
					}
				}
			}
		}
		// If we passed the tests we now have the key, find the next text
		if (!final_key.empty())
		{
			bool found = false;
			if (key_position > text_position) // else new key uses the same text as last time
			{
				// look for next text
				text_position = key_position;  // set to actual fp
				for (;found == false;)
				{
					std::getline(fp, file_line);
					find = file_line.find("<text>");
					if (find != npos)
					{
						found = true;
						text_position += (find+6);
					}
					else
						text_position += (file_line.size() + 2);
				}
			}
			// Save info
			mymap[filenum][final_key] = text_position;
		}
		fp.seekg(key_position);
		std::getline(fp, file_line);
		key_position += (file_line.size() + 2);
	}

	// Save idx
	FILE *f = fopen(filename.c_str(), "w");
	if (f == NULL)
	{
		printf("Unable to open history.idx for writing.\n");
		return;
	}

	fprintf(f, "%s\n",xml_ver.c_str());
	for (auto const &it : mymap[filenum])
		fprintf(f, "%s=%d\n", it.first.c_str(), int(it.second));
	fclose(f);

}

static bool create_index(const char* datsdir, std::ifstream &fp, int filenum)
{
	if (!fp.good())
		return false;
	// get file size
	fp.seekg(0, std::ios::end);
	size_t file_size = fp.tellg();
	// same file as before?
	if (file_size == file_sizes[filenum])
		return true;
	// new file, it needs to be indexed
	mymap[filenum].clear();
	file_sizes[filenum] = file_size;
	fp.seekg(0);
	std::string file_line;
	std::getline(fp, file_line);
	if (filenum == 0)
		create_index_history(datsdir, fp, file_line, filenum);
	else
	{
		std::streampos position = file_line.size() + 2; // tellg is buggy, this works and is faster
		while (fp.good())
		{
			if (file_line.find("$info=")==0)
			{
				// now start by removing all spaces
				file_line.erase(remove_if(file_line.begin(), file_line.end(), ::isspace), file_line.end());
				file_line.erase(0,6);
				size_t t1 = npos;
				while (file_line.length() > 0)
				{
					t1 = file_line.find(",");
					// store into index
					mymap[filenum][file_line.substr(0, t1)] = position;
					if (t1 == npos)
						file_line.erase();
					else
						file_line.erase(0, t1+1); // erase key and comma
				}
			}
			std::getline(fp, file_line);
			position += (file_line.size() + 2);
		}
	}
	// check contents
//	if (filenum == 6)
//		for (auto const &it : mymap[filenum])
//			printf("%s = %X\n", it.first.c_str(), int(it.second));
	return true;
}

static std::string convert_xml(std::string buf)
{
	// convert xml to real chars
	if (!buf.empty())
	{
		bool found = false;
		size_t find = 0;
		for (; found == false;)
		{
			find = buf.find("&amp;");
			if (find != npos)
				buf.replace(find,5,"&");
			else
				found = true;
		}
		found = false;
		for (; found == false;)
		{
			find = buf.find("&apos;");
			if (find != npos)
				buf.replace(find,6,"\'");
			else
				found = true;
		}
		found = false;
		for (; found == false;)
		{
			find = buf.find("&quot;");
			if (find != npos)
				buf.replace(find,6,"\"");
			else
				found = true;
		}
		found = false;
		for (; found == false;)
		{
			find = buf.find("&lt;");
			if (find != npos)
				buf.replace(find,4,"<");
			else
				found = true;
		}
		found = false;
		for (; found == false;)
		{
			find = buf.find("&gt;");
			if (find != npos)
				buf.replace(find,4,">");
			else
				found = true;
		}
	}
	return buf;
}

std::string readbuf;

static std::string load_datafile_text(std::ifstream &fp, std::string keycode, int filenum, const char *tag)
{
	readbuf.clear();

	auto search = mymap[filenum].find(keycode);
	if (search != mymap[filenum].end())
	{
		std::streampos offset = mymap[filenum].find(keycode)->second;
		fp.seekg(offset);
		std::string file_line;

		// read text until buffer is full or end of entry is encountered
		while (std::getline(fp, file_line))
		{
			//if (filenum == 6) ("*******2: %s\n",file_line.c_str());
			if (file_line == "- CONTRIBUTE -")
				break;

			if (file_line.find("$end")==0)
				break;

			if (file_line.find("</text>") != npos)
				break;

			if (file_line.find(tag)==0)
				continue;

			readbuf.append(file_line).append("\n");
		}
	}

	return readbuf;
}

std::string buffer;

std::string load_swinfo(const game_driver *drv, const char* datsdir, std::string software, int filenum)
{
	buffer.clear();
	// if it's a NULL record exit now
	if (!m_swInfo[filenum].filename)
		return buffer;

	// datafile name
	std::string buf, filename = datsdir + std::string("\\") + m_swInfo[filenum].filename;
	std::ifstream fp (filename);

	// try to open datafile
	if (create_index(datsdir, fp, filenum))
	{
		size_t i = software.find(":");
		std::string ssoft = software.substr(i+1);
		// get info on software
		buf = load_datafile_text(fp, software, filenum, m_swInfo[filenum].descriptor);

		if (!buf.empty())
			buffer.append(m_swInfo[filenum].header).append(ssoft).append("\n").append(buf).append("\n\n\n");

		fp.close();
	}

	return convert_xml(buffer);
}

std::string load_gameinfo(const game_driver *drv, const char* datsdir, int filenum)
{
	buffer.clear();
	// if it's a NULL record exit now
	if (!m_gameInfo[filenum].filename)
		return buffer;

	// datafile name
	std::string buf, filename = datsdir + std::string("\\") + m_gameInfo[filenum].filename;
	std::ifstream fp (filename);

	if (filenum == 0)
	{
		// try to open history.xml
		if (create_index(datsdir, fp, filenum))
		{
			// get info on game
			buf = load_datafile_text(fp, drv->name, filenum, m_gameInfo[filenum].descriptor);

			// if nothing, and it's a clone, and it's allowed, try the parent
			if (buf.empty() && m_gameInfo[filenum].bClone)
			{
				int g = driver_list::clone(*drv);
				if (g != -1)
				{
					drv = &driver_list::driver(g);
					buf = load_datafile_text(fp, drv->name, filenum, m_gameInfo[filenum].descriptor);
				}
			}
		}
	}
	else
	// try to open datafile
	if (create_index(datsdir, fp, filenum))
	{
		// get info on game
		buf = load_datafile_text(fp, drv->name, filenum, m_gameInfo[filenum].descriptor);
		// if nothing, and it's a clone, and it's allowed, try the parent
		if (buf.empty() && m_gameInfo[filenum].bClone)
		{
			int g = driver_list::clone(*drv);
			if (g != -1)
			{
				drv = &driver_list::driver(g);
				buf = load_datafile_text(fp, drv->name, filenum, m_gameInfo[filenum].descriptor);
			}
		}
	}

	if (!buf.empty())
		buffer.append(m_gameInfo[filenum].header).append(buf).append("\n\n\n");

	fp.close();

	return convert_xml(buffer);
}

std::string load_sourceinfo(const game_driver *drv, const char* datsdir, int filenum)
{
	buffer.clear();
	// if it's a NULL record exit now
	if (!m_sourceInfo[filenum].filename)
		return buffer;

	// datafile name
	std::string buf, filename = datsdir + std::string("\\") + m_sourceInfo[filenum].filename;
	std::ifstream fp (filename);

	std::string source = drv->type.source();
	size_t i = source.find_last_of("/");
	source.erase(0,i+1);

	if (create_index(datsdir, fp, filenum))
	{
		// get info on game driver source
		buf = load_datafile_text(fp, source, filenum, m_sourceInfo[filenum].descriptor);

		if (!buf.empty())
			buffer.append(m_sourceInfo[filenum].header).append(source).append("\n").append(buf).append("\n\n\n");

		fp.close();
	}

	return buffer;
}


// General hardware information
std::string load_driver_geninfo(const game_driver *drv, int drvindex)
{
	machine_config config(*drv, MameUIGlobal());
	const game_driver *parent = NULL;
	char name[512];
	bool is_bios = false;
	buffer = "\n**** :GENERAL MACHINE INFO: ****\n\n";

	// List the game info 'flags'
	uint32_t cache = GetDriverCacheLower(drvindex);
	if (BIT(cache, 6))
		buffer.append("This game doesn't work properly\n");

	if (BIT(cache, 22))
		buffer.append("This game has protection which isn't fully emulated.\n");

	if (BIT(cache, 18))
		buffer.append("The video emulation isn't 100% accurate.\n");

	if (BIT(cache, 21))
		buffer.append("The colors are completely wrong.\n");

	if (BIT(cache, 20))
		buffer.append("The colors aren't 100% accurate.\n");

	if (BIT(cache, 17))
		buffer.append("This game lacks sound.\n");

	if (BIT(cache, 16))
		buffer.append("The sound emulation isn't 100% accurate.\n");

	if (BIT(cache, 7))
		buffer.append("Save state not supported.\n");

	if (BIT(cache, 14))
		buffer.append("This game contains mechanical parts.\n");

	if (BIT(cache, 15))
		buffer.append("This game was never completed.\n");

	if (BIT(cache, 13))
		buffer.append("This game has no sound hardware.\n");

	buffer.append("\n");

	if (drv->flags & MACHINE_IS_BIOS_ROOT)
		is_bios = true;

	// GAME INFORMATIONS
	snprintf(name, std::size(name), "\nGAME: %s\n", drv->name);
	buffer.append(name);
	snprintf(name, std::size(name), "%s", drv->type.fullname());
	buffer.append(name);
	snprintf(name, std::size(name), " (%s %s)\n\nCPU:\n", drv->manufacturer, drv->year);
	buffer.append(name);
	// iterate over CPUs
	execute_interface_enumerator cpuiter(config.root_device());
	std::unordered_set<std::string> exectags;

	for (device_execute_interface &exec : cpuiter)
	{
		if (!exectags.insert(exec.device().tag()).second)
			continue;

		int count = 1;
		int clock = exec.device().clock();
		const char *cpu_name = exec.device().name();

		for (device_execute_interface &scan : cpuiter)
			if (exec.device().type() == scan.device().type() && strcmp(cpu_name, scan.device().name()) == 0 && clock == scan.device().clock())
				if (exectags.insert(scan.device().tag()).second)
					count++;

		if (count > 1)
		{
			snprintf(name, std::size(name), "%d x ", count);
			buffer.append(name);
		}

		if (clock >= 1000000)
			snprintf(name, std::size(name), "%s %d.%06d MHz\n", cpu_name, clock / 1000000, clock % 1000000);
		else
			snprintf(name, std::size(name), "%s %d.%03d kHz\n", cpu_name, clock / 1000, clock % 1000);

		buffer.append(name);
	}

	buffer.append("\nSOUND:\n");
	int has_sound = 0;
	// iterate over sound chips
	sound_interface_enumerator sounditer(config.root_device());
	std::unordered_set<std::string> soundtags;

	for (device_sound_interface &sound : sounditer)
	{
		if (!soundtags.insert(sound.device().tag()).second)
			continue;

		has_sound = 1;
		int count = 1;
		int clock = sound.device().clock();
		const char *sound_name = sound.device().name();

		for (device_sound_interface &scan : sounditer)
			if (sound.device().type() == scan.device().type() && strcmp(sound_name, scan.device().name()) == 0 && clock == scan.device().clock())
				if (soundtags.insert(scan.device().tag()).second)
					count++;

		if (count > 1)
		{
			snprintf(name, std::size(name), "%d x ", count);
			buffer.append(name);
		}

		buffer.append(sound_name);

		if (clock)
		{
			if (clock >= 1000000)
				snprintf(name, std::size(name), " %d.%06d MHz", clock / 1000000, clock % 1000000);
			else
				snprintf(name, std::size(name), " %d.%03d kHz", clock / 1000, clock % 1000);

			buffer.append(name);
		}

		buffer.append("\n");
	}

	if (has_sound)
	{
		speaker_device_enumerator audioiter(config.root_device());
		int channels = audioiter.count();

		if(channels == 1)
			snprintf(name, std::size(name), "%d Channel\n", channels);
		else
			snprintf(name, std::size(name), "%d Channels\n", channels);

		buffer.append(name);
	}

	buffer.append("\nVIDEO:\n");
	screen_device_enumerator screeniter(config.root_device());
	int scrcount = screeniter.count();

	if (scrcount == 0)
		buffer.append("Screenless");
	else
	{
		for (screen_device &screen : screeniter)
		{
			if (screen.screen_type() == SCREEN_TYPE_VECTOR)
				buffer.append("Vector");
			else
			{
				const rectangle &visarea = screen.visible_area();

				if (drv->flags & ORIENTATION_SWAP_XY)
					snprintf(name, std::size(name), "%d x %d (V) %f Hz", visarea.width(), visarea.height(), ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));
				else
					snprintf(name, std::size(name), "%d x %d (H) %f Hz", visarea.width(), visarea.height(), ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));

				buffer.append(name);
			}

			buffer.append("\n");
		}
	}

	buffer.append("\nROM REGION:\n");
	int g = driver_list::clone(*drv);

	if (g != -1)
		parent = &driver_list::driver(g);

	for (device_t &device : device_enumerator(config.root_device()))
	{
		for (const rom_entry *region = rom_first_region(device); region; region = rom_next_region(region))
		{
			for (const rom_entry *rom = rom_first_file(region); rom; rom = rom_next_file(rom))
			{
				util::hash_collection hashes(rom->hashdata());

				if (g != -1)
				{
					machine_config pconfig(*parent, MameUIGlobal());

					for (device_t &device : device_enumerator(pconfig.root_device()))
					{
						for (const rom_entry *pregion = rom_first_region(device); pregion; pregion = rom_next_region(pregion))
						{
							for (const rom_entry *prom = rom_first_file(pregion); prom; prom = rom_next_file(prom))
							{
								util::hash_collection phashes(prom->hashdata());

								if (hashes == phashes)
									break;
							}
						}
					}
				}

				snprintf(name, std::size(name), "%-16s \t", ROM_GETNAME(rom));
				buffer.append(name);
				snprintf(name, std::size(name), "%09d \t", rom_file_size(rom));
				buffer.append(name);
				snprintf(name, std::size(name), "%-10s", region->name().c_str());
				buffer.append(name).append("\n");
			}
		}
	}

	for (samples_device &device : samples_device_enumerator(config.root_device()))
	{
		samples_iterator sampiter(device);

		if (sampiter.altbasename())
		{
			snprintf(name, std::size(name), "\nSAMPLES (%s):\n", sampiter.altbasename());
			buffer.append(name);
		}

		std::unordered_set<std::string> already_printed;

		for (const char *samplename = sampiter.first(); samplename; samplename = sampiter.next())
		{
			// filter out duplicates
			if (!already_printed.insert(samplename).second)
				continue;

			// output the sample name
			snprintf(name, std::size(name), "%s.wav\n", samplename);
			buffer.append(name);
		}
	}

	if (!is_bios)
	{
		int g = driver_list::clone(*drv);

		if (g != -1)
			drv = &driver_list::driver(g);

		buffer.append("\nORIGINAL:\n");
		buffer.append(drv->type.fullname());
		buffer.append("\n\nCLONES:\n");

		for (int i = 0; i < driver_list::total(); i++)
		{
			if (!strcmp (drv->name, driver_list::driver(i).parent))
			{
				buffer.append(driver_list::driver(i).type.fullname());
				buffer.append("\n");
			}
		}
	}

	string temp = string(core_filename_extract_base(drv->type.source(), false));
	char source_file[temp.size()+1], tmp[2048];
	strcpy(source_file, temp.c_str());
	snprintf(tmp, std::size(tmp), "\nGENERAL SOURCE INFO: %s\n", temp.c_str());
	buffer.append(tmp);
	buffer.append("\nGAMES SUPPORTED:\n");

	for (int i = 0; i < driver_list::total(); i++)
	{
		std::string t1 = driver_list::driver(i).type.source();
		size_t j = t1.find_last_of("/");
		t1.erase(0, j+1);
		if ((strcmp(source_file, t1.c_str())==0) && !(DriverIsBios(i)))
			buffer.append(driver_list::driver(i).type.fullname()).append("\n");
	}

	return buffer;
}

// This is check that the tables are at least as big as they should be
bool validate_datfiles(void)
{
	bool result = true;
	if (std::size(m_gameInfo) < MAX_HFILES)
	{
		printf("m_gameInfo needs to have at least MAX_HFILES members\n");
		result = false;
	}

	if (std::size(m_sourceInfo) < MAX_HFILES)
	{
		printf("m_sourceInfo needs to have at least MAX_HFILES members\n");
		result = false;
	}

	if (std::size(m_swInfo) < MAX_HFILES)
	{
		printf("m_swInfo needs to have at least MAX_HFILES members\n");
		result = false;
	}

	return result;
}

std::string fullbuf;

// For all of MAME builds - called by winui.cpp
char * GetGameHistory(int driver_index, std::string software)
{
	fullbuf.clear();
	if (driver_index < 0)
		return ConvertToWindowsNewlines(fullbuf.c_str());

	if (validate_datfiles())
	{
		// Get the path to dat files
		std::string t = dir_get_value(23);
		char buf[t.size()+1];
		strcpy(buf, t.c_str());
		// only want first path
		const char* datsdir = strtok(buf, ";");
		// validate software
		BOOL sw_valid = false;
		if (!software.empty())
		{
			size_t i = software.find(':');
			sw_valid = (i != npos) ? true : false;
		}

		if (datsdir && osd::directory::open(datsdir))
		{
			for (int filenum = 0; filenum < MAX_HFILES; filenum++)
			{
				if (sw_valid)
					fullbuf.append(load_swinfo(&driver_list::driver(driver_index), datsdir, software, filenum));
				fullbuf.append(load_gameinfo(&driver_list::driver(driver_index), datsdir, filenum));
				fullbuf.append(load_sourceinfo(&driver_list::driver(driver_index), datsdir, filenum));
			}
		}
		else
			fullbuf = "\nThe path to your dat files is invalid.\n\n\n";
	}
	else
		fullbuf = "\nUnable to display info due to an internal error.\n\n\n";

	fullbuf.append(load_driver_geninfo(&driver_list::driver(driver_index), driver_index));

	return ConvertToWindowsNewlines(fullbuf.c_str());
}

// For Arcade-only builds
char * GetGameHistory(int driver_index)
{
	fullbuf.clear();
	if (driver_index < 0)
		return ConvertToWindowsNewlines(fullbuf.c_str());

	if (validate_datfiles())
	{
		std::string t = dir_get_value(23);
		char buf[t.size()+1];
		strcpy(buf, t.c_str());
		// only want first path
		const char* datsdir = strtok(buf, ";");

		if (datsdir && osd::directory::open(datsdir))
		{
			for (int filenum = 0; filenum < MAX_HFILES; filenum++)
			{
				fullbuf.append(load_gameinfo(&driver_list::driver(driver_index), datsdir, filenum));
				fullbuf.append(load_sourceinfo(&driver_list::driver(driver_index), datsdir, filenum));
			}
		}
		else
			fullbuf = "\nThe path to your dat files is invalid.\n\n\n";
	}
	else
		fullbuf = "\nUnable to display info due to an internal error.\n\n\n";

	fullbuf.append(load_driver_geninfo(&driver_list::driver(driver_index), driver_index));

	return ConvertToWindowsNewlines(fullbuf.c_str());
}

