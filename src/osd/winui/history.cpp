// For licensing and usage information, read docs/winui_license.txt
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
#include "sound/samples.h"

#define WINUI_ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))


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
#define MAX_HFILES 8
// The order of these is the order they are displayed
const HGAMEINFO m_gameInfo[MAX_HFILES] =
{
	{ "history.dat",  "\n**** :HISTORY: ****\n\n",          "$bio",   1 },
	{ "sysinfo.dat",  "\n**** :SYSINFO: ****\n\n",          "$bio",   1 },
	{ "messinfo.dat", "\n**** :MESSINFO: ****\n\n",         "$mame",  1 },
	{ "mameinfo.dat", "\n**** :MAMEINFO: ****\n\n",         "$mame",  1 },
	{ "gameinit.dat", "\n**** :GAMEINIT: ****\n\n",         "$mame",  1 },
	{ "command.dat",  "\n**** :COMMANDS: ****\n\n",         "$cmd",   1 },
	{ "story.dat",    "\n**** :HIGH SCORES: ****\n\n",      "$story", 0 },
	{ "marp.dat",     "\n**** :MARP HIGH SCORES: ****\n\n", "$marp",  0 },
};

const HSOURCEINFO m_sourceInfo[MAX_HFILES] =
{
	{ NULL },
	{ NULL },
	{ "messinfo.dat", "\n***:MESSINFO DRIVER: ",  "$drv" },
	{ "mameinfo.dat", "\n***:MAMEINFO DRIVER: ",  "$drv" },
	{ NULL },
	{ NULL },
	{ NULL },
	{ NULL },
};

const HSOURCEINFO m_swInfo[MAX_HFILES] =
{
	{ "history.dat",  "\n**** :HISTORY item: ",     "$bio" },
	{ NULL },
	{ NULL },
	{ NULL },
	{ NULL },
	{ NULL },
	{ NULL },
	{ NULL },
};

/*************************** END CONFIGURABLE AREA *******************************/

int file_sizes[MAX_HFILES] = { 0, };
std::map<std::string, std::streampos> mymap[MAX_HFILES];

static bool create_index(std::ifstream &fp, int filenum)
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
	std::string file_line, first, second;
	std::getline(fp, file_line);
	int position = file_line.size() + 2; // tellg is buggy, this works and is faster
	while (fp.good())
	{
		char t1 = file_line[0];
		if ((std::count(file_line.begin(),file_line.end(),'=') == 1) && (t1 == '$')) // line must start with $ and contain one =
		{
			// now start by removing all spaces
			file_line.erase(remove_if(file_line.begin(), file_line.end(), ::isspace), file_line.end());
			char s[file_line.length()+1];
			strcpy(s, file_line.c_str());

			const char* first = strtok(s, "=");  // get first part of key
			char* second = strtok(NULL, ",");    // get second part
			while (second)
			{
				// store into index
				mymap[filenum][std::string(first) + std::string("=") + std::string(second)] = position;
				second = strtok(NULL, ",");
			}
		}
		std::getline(fp, file_line);
		position += (file_line.size() + 2);
	}
	// check contents
//	if (filenum == 0)
//	for (auto const &it : mymap[filenum])
//		printf("%s = %X\n", it.first.c_str(), int(it.second));
	return true;
}

static std::string load_datafile_text(std::ifstream &fp, std::string keycode, int filenum, const char *tag)
{
	std::string readbuf;

	auto search = mymap[filenum].find(keycode);
	if (search != mymap[filenum].end())
	{
		std::streampos offset = mymap[filenum].find(keycode)->second;
		fp.seekg(offset);
		std::string file_line;

		/* read text until buffer is full or end of entry is encountered */
		while (std::getline(fp, file_line))
		{
			//printf("%s\n",file_line.c_str());
			if (file_line.find("$end")==0)
				break;

			if (file_line.find(tag)==0)
				continue;

			readbuf.append(file_line).append("\n");
		}
	}

	return readbuf;
}

std::string load_swinfo(const game_driver *drv, const char* datsdir, std::string software, int filenum)
{
	std::string buffer;
	// if it's a NULL record exit now
	if (!m_swInfo[filenum].filename)
		return buffer;

	// datafile name
	std::string buf, filename = datsdir + std::string("\\") + m_swInfo[filenum].filename;
	std::ifstream fp (filename);

	/* try to open datafile */
	if (create_index(fp, filenum))
	{
		size_t i = software.find(":");
		std::string ssys = software.substr(0, i);
		std::string ssoft = software.substr(i+1);
		std::string first = std::string("$") + ssys + std::string("=") + ssoft;
		// get info on software
		buf = load_datafile_text(fp, first, filenum, m_swInfo[filenum].descriptor);

		if (!buf.empty())
			buffer.append(m_swInfo[filenum].header).append(ssoft).append("\n").append(buf).append("\n\n\n");

		fp.close();
	}

	return buffer;
}

std::string load_gameinfo(const game_driver *drv, const char* datsdir, int filenum)
{
	std::string buffer;
	// if it's a NULL record exit now
	if (!m_gameInfo[filenum].filename)
		return buffer;

	// datafile name
	std::string buf, filename = datsdir + std::string("\\") + m_gameInfo[filenum].filename;
	std::ifstream fp (filename);

	/* try to open datafile */
	if (create_index(fp, filenum))
	{
		std::string first = std::string("$info=")+drv->name;
		// get info on game
		buf = load_datafile_text(fp, first, filenum, m_gameInfo[filenum].descriptor);

		// if nothing, and it's a clone, and it's allowed, try the parent
		if (buf.empty() && m_gameInfo[filenum].bClone)
		{
			int g = driver_list::clone(*drv);
			if (g != -1)
			{
				drv = &driver_list::driver(g);
				first = std::string("$info=")+drv->name;
				buf = load_datafile_text(fp, first, filenum, m_gameInfo[filenum].descriptor);
			}
		}

		if (!buf.empty())
			buffer.append(m_gameInfo[filenum].header).append(buf).append("\n\n\n");

		fp.close();
	}

	return buffer;
}

std::string load_sourceinfo(const game_driver *drv, const char* datsdir, int filenum)
{
	std::string buffer;
	// if it's a NULL record exit now
	if (!m_sourceInfo[filenum].filename)
		return buffer;

	// datafile name
	std::string buf, filename = datsdir + std::string("\\") + m_sourceInfo[filenum].filename;
	std::ifstream fp (filename);

	std::string source = drv->type.source();
	size_t i = source.find_last_of("/");
	source.erase(0,i+1);

	if (create_index(fp, filenum))
	{
		std::string first = std::string("$info=")+source;
		// get info on game driver source
		buf = load_datafile_text(fp, first, filenum, m_sourceInfo[filenum].descriptor);

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
	std::string buffer = "\n**** :GENERAL MACHINE INFO: ****\n\n";

	/* List the game info 'flags' */
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

	/* GAME INFORMATIONS */
	snprintf(name, WINUI_ARRAY_LENGTH(name), "\nGAME: %s\n", drv->name);
	buffer.append(name);
	snprintf(name, WINUI_ARRAY_LENGTH(name), "%s", drv->type.fullname());
	buffer.append(name);
	snprintf(name, WINUI_ARRAY_LENGTH(name), " (%s %s)\n\nCPU:\n", drv->manufacturer, drv->year);
	buffer.append(name);
	/* iterate over CPUs */
	execute_interface_iterator cpuiter(config.root_device());
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
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%d x ", count);
			buffer.append(name);
		}

		if (clock >= 1000000)
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%s %d.%06d MHz\n", cpu_name, clock / 1000000, clock % 1000000);
		else
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%s %d.%03d kHz\n", cpu_name, clock / 1000, clock % 1000);

		buffer.append(name);
	}

	buffer.append("\nSOUND:\n");
	int has_sound = 0;
	/* iterate over sound chips */
	sound_interface_iterator sounditer(config.root_device());
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
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%d x ", count);
			buffer.append(name);
		}

		buffer.append(sound_name);

		if (clock)
		{
			if (clock >= 1000000)
				snprintf(name, WINUI_ARRAY_LENGTH(name), " %d.%06d MHz", clock / 1000000, clock % 1000000);
			else
				snprintf(name, WINUI_ARRAY_LENGTH(name), " %d.%03d kHz", clock / 1000, clock % 1000);

			buffer.append(name);
		}

		buffer.append("\n");
	}

	if (has_sound)
	{
		speaker_device_iterator audioiter(config.root_device());
		int channels = audioiter.count();

		if(channels == 1)
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%d Channel\n", channels);
		else
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%d Channels\n", channels);

		buffer.append(name);
	}

	buffer.append("\nVIDEO:\n");
	screen_device_iterator screeniter(config.root_device());
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
					snprintf(name, WINUI_ARRAY_LENGTH(name), "%d x %d (V) %f Hz", visarea.width(), visarea.height(), ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));
				else
					snprintf(name, WINUI_ARRAY_LENGTH(name), "%d x %d (H) %f Hz", visarea.width(), visarea.height(), ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));

				buffer.append(name);
			}

			buffer.append("\n");
		}
	}

	buffer.append("\nROM REGION:\n");
	int g = driver_list::clone(*drv);

	if (g != -1)
		parent = &driver_list::driver(g);

	for (device_t &device : device_iterator(config.root_device()))
	{
		for (const rom_entry *region = rom_first_region(device); region; region = rom_next_region(region))
		{
			for (const rom_entry *rom = rom_first_file(region); rom; rom = rom_next_file(rom))
			{
				util::hash_collection hashes(ROM_GETHASHDATA(rom));

				if (g != -1)
				{
					machine_config pconfig(*parent, MameUIGlobal());

					for (device_t &device : device_iterator(pconfig.root_device()))
					{
						for (const rom_entry *pregion = rom_first_region(device); pregion; pregion = rom_next_region(pregion))
						{
							for (const rom_entry *prom = rom_first_file(pregion); prom; prom = rom_next_file(prom))
							{
								util::hash_collection phashes(ROM_GETHASHDATA(prom));

								if (hashes == phashes)
									break;
							}
						}
					}
				}

				snprintf(name, WINUI_ARRAY_LENGTH(name), "%-16s \t", ROM_GETNAME(rom));
				buffer.append(name);
				snprintf(name, WINUI_ARRAY_LENGTH(name), "%09d \t", rom_file_size(rom));
				buffer.append(name);
				snprintf(name, WINUI_ARRAY_LENGTH(name), "%-10s", ROMREGION_GETTAG(region));
				buffer.append(name).append("\n");
			}
		}
	}

	for (samples_device &device : samples_device_iterator(config.root_device()))
	{
		samples_iterator sampiter(device);

		if (sampiter.altbasename())
		{
			snprintf(name, WINUI_ARRAY_LENGTH(name), "\nSAMPLES (%s):\n", sampiter.altbasename());
			buffer.append(name);
		}

		std::unordered_set<std::string> already_printed;

		for (const char *samplename = sampiter.first(); samplename; samplename = sampiter.next())
		{
			// filter out duplicates
			if (!already_printed.insert(samplename).second)
				continue;

			// output the sample name
			snprintf(name, WINUI_ARRAY_LENGTH(name), "%s.wav\n", samplename);
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

	char source_file[40], tmp[100];
	std::string temp = core_filename_extract_base(drv->type.source(), false);
	strcpy(source_file, temp.c_str());
	snprintf(tmp, WINUI_ARRAY_LENGTH(tmp), "\nGENERAL SOURCE INFO: %s\n", temp.c_str());
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
	if (WINUI_ARRAY_LENGTH(m_gameInfo) < MAX_HFILES)
	{
		printf("m_gameInfo needs to have at least MAX_HFILES members\n");
		result = false;
	}

	if (WINUI_ARRAY_LENGTH(m_sourceInfo) < MAX_HFILES)
	{
		printf("m_sourceInfo needs to have at least MAX_HFILES members\n");
		result = false;
	}

	if (WINUI_ARRAY_LENGTH(m_swInfo) < MAX_HFILES)
	{
		printf("m_swInfo needs to have at least MAX_HFILES members\n");
		result = false;
	}

	return result;
}


// For all of MAME builds - called by winui.cpp
char * GetGameHistory(int driver_index, std::string software)
{
	std::string fullbuf;
	if (driver_index < 0)
			return ConvertToWindowsNewlines(fullbuf.c_str());

	if (validate_datfiles())
	{
		// Get the path to dat files
		char buf[400];
		std::string t = GetDatsDir();
		strcpy(buf, t.c_str());
		// only want first path
		const char* datsdir = strtok(buf, ";");
		// validate software
		BOOL sw_valid = false;
		if (!software.empty())
		{
			size_t i = software.find(':');
			sw_valid = (i != std::string::npos) ? true : false;
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
	std::string fullbuf;
	if (driver_index < 0)
			return ConvertToWindowsNewlines(fullbuf.c_str());

	if (validate_datfiles())
	{
		char buf[400];
		std::string t = GetDatsDir();
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

