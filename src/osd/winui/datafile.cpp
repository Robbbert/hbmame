// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************
/****************************************************************************
 *      datafile.c
 *      History database engine
 *
 *      Token parsing by Neil Bradley
 *      Modifications and higher-level functions by John Butler
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "datafile.h"
#include "mui_opts.h"
#include "mui_util.h"
#include "sound/samples.h"
#include "drivenum.h"  // MESSUI only

#define MAX_TOKEN_LENGTH        256
#define DATAFILE_TAG '$'

/****************************************************************************
 *      datafile constants
 ****************************************************************************/
static const char *DATAFILE_TAG_KEY = "$info";
static const char *DATAFILE_TAG_BIO = "$bio";
static const char *DATAFILE_TAG_MAME = "$mame";
static const char *DATAFILE_TAG_DRIV = "$drv";
static const char *DATAFILE_TAG_CMD = "$cmd";
static const char *DATAFILE_TAG_SCORE = "$story";
static const char *DATAFILE_TAG_END = "$end";

/****************************************************************************
 *      private data for parsing functions
 ****************************************************************************/
static FILE *fp = NULL; /* Our file pointer */
static UINT64 dwFilePos = 0; /* file position */
static char filename[MAX_PATH]; /* datafile name */

struct tDatafileIndex
{
	long offset;
	const game_driver *driver;
};

static struct tDatafileIndex *hist_idx = 0;
static struct tDatafileIndex *mame_idx = 0;
static struct tDatafileIndex *driv_idx = 0;
static struct tDatafileIndex *sysinfo_idx = 0;
static struct tDatafileIndex *mess_idx = 0;
static struct tDatafileIndex *messdriv_idx = 0;
static struct tDatafileIndex *cmd_idx = 0;
static struct tDatafileIndex *score_idx = 0;

/****************************************************************************
* Create an array with sorted sourcedrivers for the function
* index_datafile_drivinfo to speed up the datafile access
****************************************************************************/

typedef struct
{
	const char *srcdriver;
	int index;
} srcdriver_data_type;

static srcdriver_data_type *sorted_srcdrivers = NULL;

static int SrcDriverDataCompareFunc(const void *arg1, const void *arg2)
{
	return strcmp(((srcdriver_data_type *)arg1)->srcdriver, ((srcdriver_data_type *)arg2)->srcdriver);
}

static int GetSrcDriverIndex(const char *srcdriver)
{
	srcdriver_data_type *srcdriver_index_info;
	srcdriver_data_type key;
	key.srcdriver = srcdriver;
	int num_games = driver_list::total();

	if (sorted_srcdrivers == NULL)
	{
		/* initialize array of game names/indices */
		sorted_srcdrivers = (srcdriver_data_type *)malloc(sizeof(srcdriver_data_type) * num_games);
		for (int i = 0; i < num_games; i++)
		{
			sorted_srcdrivers[i].srcdriver = driver_list::driver(i).source_file+32;
			sorted_srcdrivers[i].index = i;
		}
		qsort(sorted_srcdrivers,num_games,sizeof(srcdriver_data_type),SrcDriverDataCompareFunc);
	}

	srcdriver_index_info = (srcdriver_data_type *)bsearch(&key, sorted_srcdrivers, num_games, sizeof(srcdriver_data_type), SrcDriverDataCompareFunc);

	if (srcdriver_index_info)
		return srcdriver_index_info->index;
	else
		return -1;
}

/****************************************************************************
 *      ParseClose - Closes the existing opened file (if any)
 ****************************************************************************/
static void ParseClose(void)
{
	/* If the file is open, time for fclose. */
	if (fp)
		fclose(fp);
}

/****************************************************************************
 *      ParseOpen - Open up file for reading
 ****************************************************************************/
static BOOL ParseOpen(const char *pszFilename)
{
	/* MAME core file parsing functions fail in recognizing UNICODE chars in UTF-8 without BOM,
	so it's better and faster use standard C fileio functions */
	fp = fopen(pszFilename, "r");
	
	if (fp == NULL)
		return FALSE;

	/* Otherwise, prepare! */
	dwFilePos = 0;
	/* identify text file type first */
	fgetc(fp);
	fseek(fp, dwFilePos, SEEK_SET);
	return TRUE;
}

/****************************************************************************
 *      ParseSeek - Move the file position indicator
 ****************************************************************************/
static UINT8 ParseSeek(UINT64 offset, int whence)
{
	int result = fseek(fp, offset, whence);

	if (result == 0)
		dwFilePos = ftell(fp);

	return (UINT8)result;
}

/**************************************************************************
 **************************************************************************
 *
 *              Datafile functions
 *
 **************************************************************************
 **************************************************************************/

 /**************************************************************************
 *      index_datafile
 *      Create an index for the records in the currently open datafile.
 *
 *      Returns 0 on error, or the number of index entries created.
 **************************************************************************/
static int index_datafile(struct tDatafileIndex **_index, int source)
{
	struct tDatafileIndex *idx;
	int count = 0;
	char readbuf[512];
	char name[40];
	int num_games = driver_list::total();

	/* rewind file */
	if (ParseSeek (0L, SEEK_SET)) 
		return 0;

	/* allocate index */
	idx = *_index = global_alloc_array(tDatafileIndex, (num_games + 1) * sizeof (struct tDatafileIndex));
 
	if (!idx) 
		return 0;

	while (fgets(readbuf, 512, fp))
	{
		/* DATAFILE_TAG_KEY identifies the driver */
		if (!core_strnicmp(DATAFILE_TAG_KEY, readbuf, strlen(DATAFILE_TAG_KEY)))
		{
			int game_index = 0;
			char *curpoint = &readbuf[strlen(DATAFILE_TAG_KEY) + 1];
			char *pch = NULL;
			char *ends = &readbuf[strlen(readbuf) - 1];

			while (curpoint < ends)
			{
				// search for comma
				pch = strpbrk(curpoint, ",");
				
				// found it 
				if (pch)
				{
					// copy data and validate driver
					int len = pch - curpoint;
					strncpy(name, curpoint, len);
					name[len] = '\0';

					if (!source)
						game_index = GetGameNameIndex(name);
					else
						game_index = GetSrcDriverIndex(name);

					if (game_index >= 0)
					{
						idx->driver = &driver_list::driver(game_index);
						idx->offset = ftell(fp);
						idx++;
						count++;
					}

					// update current point
					curpoint = pch + 1;
				}
				// if comma not found, copy data while until reach the end of string
				else if (!pch && curpoint < ends)
				{
					int len = ends - curpoint;
					strncpy(name, curpoint, len);
					name[len] = '\0';

					if (!source)
						game_index = GetGameNameIndex(name);
					else
						game_index = GetSrcDriverIndex(name);

					if (game_index >= 0)
					{
						idx->driver = &driver_list::driver(game_index);
						idx->offset = ftell(fp);
						idx++;
						count++;
					}

					// update current point
					curpoint = ends;
				}
			}
		}
	}

	/* mark end of index */
	idx->offset = 0L;
	idx->driver = 0;
	return count;
}

/**************************************************************************
 *      load_datafile_text
 *
 *      Loads text field for a driver into the buffer specified. Specify the
 *      driver, a pointer to the buffer, the buffer size, the index created by
 *      index_datafile(), and the desired text field (e.g., DATAFILE_TAG_BIO).
 *
 *      Returns 0 if successful.
 **************************************************************************/
static int load_datafile_text(const game_driver *drv, char *buffer, int bufsize, struct tDatafileIndex *idx, const char *tag, int source_file, int mameinfo)
{
	char readbuf[16384];

	*buffer = '\0';

	if (!source_file)
	{
		/* find driver in datafile index */
		while (idx->driver)
		{
			if (idx->driver == drv) 
				break;

		idx++;
		}
	}
	else
	{
		/* find source file in datafile index */
		while (idx->driver)
		{
			if (idx->driver->source_file == drv->source_file) 
				break;

			idx++;
		}
	}

	if (idx->driver == 0) 
		return 1; /* driver not found in index */

	/* seek to correct point in datafile */
	if (ParseSeek (idx->offset, SEEK_SET)) 
		return 1;

	/* read text until buffer is full or end of entry is encountered */
	while (fgets(readbuf, 16384, fp))
	{
		if (!core_strnicmp(DATAFILE_TAG_END, readbuf, strlen(DATAFILE_TAG_END))) 
			break;

		if (!core_strnicmp(tag, readbuf, strlen(tag))) 
			continue;

		if (strlen(buffer) + strlen(readbuf) > bufsize) 
			break;

		if (mameinfo)
		{
			if (strtok(readbuf, "\r\n\r\n") != NULL)
			{
				char *temp = strtok(readbuf, "\r\n\r\n");
				strcat(buffer, temp);
			}
			else
				strcat(buffer, readbuf);
		}
		else
			strcat(buffer, readbuf);
	}

	return 0;
}

/**************************************************************************
 *      load_driver_history
 *      Load history text for the specified driver into the specified buffer.
 *      Combines $bio field of HISTORY.DAT with $mame field of MAMEINFO.DAT.
 *
 *      Returns 0 if successful.
 *
 *      NOTE: For efficiency the indices are never freed (intentional leak).
 **************************************************************************/
int load_driver_history(const game_driver *drv, char *buffer, int bufsize, int filenum)
{
	int history = 0;

	*buffer = 0;

	if (filenum)
		snprintf(filename, ARRAY_LENGTH(filename), "%s\\sysinfo.dat", GetDatsDir());
	else
		snprintf(filename, ARRAY_LENGTH(filename), "%s\\history.dat", GetDatsDir());

	/* try to open history datafile */
	if (filenum)
	{
		if (ParseOpen(filename))
		{
			/* create index if necessary */
			if (sysinfo_idx)
				history = 1;
			else
				history = (index_datafile (&sysinfo_idx, 0) != 0);

			/* load history text (append)*/
			if (sysinfo_idx)
			{
				strcat(buffer, "\n**** SYSINFO: ****\n\n");
				int len = strlen(buffer);
				int err = 0;
				const game_driver *gdrv;
				gdrv = drv;

				do
				{
					err = load_datafile_text(gdrv, buffer + len, bufsize - len, sysinfo_idx, DATAFILE_TAG_BIO, 0, 0);
					int g = driver_list::clone(*gdrv);

					if (g!=-1)
						gdrv = &driver_list::driver(g);
					else
						gdrv = NULL;
				} while (err && gdrv);

				if (err) 
					history = 0;
			}

			ParseClose();
		}
	}
	else
	{
		if (ParseOpen(filename))
		{
			/* create index if necessary */
			if (hist_idx)
				history = 1;
			else
				history = (index_datafile (&hist_idx, 0) != 0);

			/* load history text (append)*/
			if (hist_idx)
			{
				strcat(buffer, "\n**** HISTORY: ****\n\n");
				int len = strlen(buffer);
				int err = 0;
				const game_driver *gdrv;
				gdrv = drv;

				do
				{
					err = load_datafile_text(gdrv, buffer + len, bufsize - len, hist_idx, DATAFILE_TAG_BIO, 0, 0);
					int g = driver_list::clone(*gdrv);

					if (g!=-1)
						gdrv = &driver_list::driver(g);
					else
						gdrv = NULL;
				} while (err && gdrv);

				if (err) 
					history = 0;
			}

			ParseClose();
		}
	}

	strcat(buffer, "\n");
	return (history == 0);
}

int load_driver_mameinfo(const game_driver *drv, char *buffer, int bufsize, int filenum)
{
	machine_config config(*drv, MameUIGlobal());
	const game_driver *parent = NULL;
	char name[512];
	int mameinfo = 0;
	int is_bios = 0;

	*buffer = 0;

	if (filenum)
		snprintf(filename, ARRAY_LENGTH(filename), "%s\\messinfo.dat", GetDatsDir());
	else
		snprintf(filename, ARRAY_LENGTH(filename), "%s\\mameinfo.dat", GetDatsDir());

	if (filenum)
		strcat(buffer, "\n**** MESSINFO: ****\n\n");
	else
		strcat(buffer, "\n**** MAMEINFO: ****\n\n");

	/* List the game info 'flags' */
	if (drv->flags & MACHINE_NOT_WORKING)
		strcat(buffer, "THIS GAME DOESN'T WORK PROPERLY\n");

	if (drv->flags & MACHINE_UNEMULATED_PROTECTION)
		strcat(buffer, "The game has protection which isn't fully emulated.\n");

	if (drv->flags & MACHINE_IMPERFECT_GRAPHICS)
		strcat(buffer, "The video emulation isn't 100% accurate.\n");

	if (drv->flags & MACHINE_WRONG_COLORS)
		strcat(buffer, "The colors are completely wrong.\n");

	if (drv->flags & MACHINE_IMPERFECT_COLORS)
		strcat(buffer, "The colors aren't 100% accurate.\n");

	if (drv->flags & MACHINE_NO_SOUND)
		strcat(buffer, "The game lacks sound.\n");

	if (drv->flags & MACHINE_IMPERFECT_SOUND)
		strcat(buffer, "The sound emulation isn't 100% accurate.\n");

	if (drv->flags & MACHINE_SUPPORTS_SAVE)
		strcat(buffer, "Save state support.\n");

	if (drv->flags & MACHINE_MECHANICAL)
		strcat(buffer, "The game contains mechanical parts.\n");

	strcat(buffer, "\n");

	if (drv->flags & MACHINE_IS_BIOS_ROOT)
		is_bios = 1;

	/* try to open mameinfo datafile */
	if (ParseOpen(filename))
	{
		if (filenum)
		{
			/* create index if necessary */
			if (mess_idx)
				mameinfo = 1;
			else
				mameinfo = (index_datafile (&mess_idx, 0) != 0);

			/* load informational text (append) */
			if (mess_idx)
			{
				int len = strlen(buffer);
				int err = 0;
				const game_driver *gdrv;
				gdrv = drv;

				do
				{
					err = load_datafile_text(gdrv, buffer + len, bufsize - len, mess_idx, DATAFILE_TAG_MAME, 0, 1);
					int g = driver_list::clone(*gdrv);

					if (g!=-1)
						gdrv = &driver_list::driver(g);
					else
						gdrv = NULL;
				} while (err && gdrv);

				if (err) 
					mameinfo = 0;
			}
		}
		else
		{
			/* create index if necessary */
			if (mame_idx)
				mameinfo = 1;
			else
				mameinfo = (index_datafile (&mame_idx, 0) != 0);

			/* load informational text (append) */
			if (mame_idx)
			{
				int len = strlen(buffer);
				int err = 0;
				const game_driver *gdrv;
				gdrv = drv;

				do
				{
					err = load_datafile_text(gdrv, buffer + len, bufsize - len, mame_idx, DATAFILE_TAG_MAME, 0, 1);
					int g = driver_list::clone(*gdrv);

					if (g!=-1)
						gdrv = &driver_list::driver(g);
					else
						gdrv = NULL;
				} while (err && gdrv);

				if (err) 
					mameinfo = 0;
			}
		}

		ParseClose();
	}

	/* GAME INFORMATIONS */
	snprintf(name, ARRAY_LENGTH(name), "\nGAME: %s\n", drv->name);
	strcat(buffer, name);
	snprintf(name, ARRAY_LENGTH(name), "%s", drv->description);
	strcat(buffer, name);
	snprintf(name, ARRAY_LENGTH(name), " (%s %s)\n\nCPU:\n", drv->manufacturer, drv->year);
	strcat(buffer, name);
	/* iterate over CPUs */
	execute_interface_iterator iter(config.root_device());
	device_execute_interface *cpu = iter.first();

	while (cpu)
	{
		if (cpu->device().clock() >= 1000000)
			snprintf(name, ARRAY_LENGTH(name), "%s %d.%06d MHz\n", cpu->device().name(), cpu->device().clock() / 1000000, cpu->device().clock() % 1000000);
		else
			snprintf(name, ARRAY_LENGTH(name), "%s %d.%03d kHz\n", cpu->device().name(), cpu->device().clock() / 1000, cpu->device().clock() % 1000);

		strcat(buffer, name);
		cpu = iter.next();
	}

	strcat(buffer, "\nSOUND:\n");
	int has_sound = 0;
	/* iterate over sound chips */
	sound_interface_iterator sounditer(config.root_device());
	const device_sound_interface *sound = sounditer.first();

	while(sound)
	{
		int clock = 0;
		int count = 0;
		device_type sound_type_;
		char tmpname[1024];

		snprintf(tmpname, ARRAY_LENGTH(tmpname), "%s", sound->device().name());
		sound_type_ = sound->device().type();
		clock = sound->device().clock();
		has_sound = 1;
		count = 1;
		sound = sounditer.next();

		/* Matching chips at the same clock are aggregated */
		while (sound && sound->device().type() == sound_type_ && sound->device().clock() == clock)
		{
			count++;
			sound = sounditer.next();
		}

		if (count > 1)
		{
			snprintf(name, ARRAY_LENGTH(name), "%dx ",count);
			strcat(buffer, name);
		}

		strcat(buffer, tmpname);

		if (clock)
		{
			if (clock >= 1000000)
				snprintf(name, ARRAY_LENGTH(name), " %d.%06d MHz", clock / 1000000, clock % 1000000);
			else
				snprintf(name, ARRAY_LENGTH(name), " %d.%03d kHz", clock / 1000, clock % 1000);

			strcat(buffer, name);
		}

		strcat(buffer, "\n");
	}

	if (has_sound)
	{
		speaker_device_iterator iter(config.root_device());
		int channels = iter.count();

		if(channels == 1)
			snprintf(name, ARRAY_LENGTH(name), "%d Channel\n",channels);
		else
			snprintf(name, ARRAY_LENGTH(name), "%dx Channels\n",channels);

		strcat(buffer, name);
	}

	strcat(buffer, "\nVIDEO:\n");
	screen_device_iterator screeniter(config.root_device());
	const screen_device *screen = screeniter.first();

	if (screen == NULL)
		strcat(buffer, "Screenless\n");
	else if (screen->screen_type() == SCREEN_TYPE_VECTOR)
		strcat(buffer,"Vector\n");
	else
	{
		for (; screen != NULL; screen = screeniter.next())
		{
			if (drv->flags & ORIENTATION_SWAP_XY)
				snprintf(name, ARRAY_LENGTH(name), "%d x %d (V)", screen->visible_area().height(), screen->visible_area().width());
			else
				snprintf(name, ARRAY_LENGTH(name), "%d x %d (H)", screen->visible_area().width(), screen->visible_area().height());

			strcat(buffer, name);
			snprintf(name, ARRAY_LENGTH(name), " %f Hz", ATTOSECONDS_TO_HZ(screen->refresh_attoseconds()));
			strcat(buffer, name);
			strcat(buffer, "\n");
		}
	}

	strcat(buffer, "\nROM REGION:\n");
	int g = driver_list::clone(*drv);

	if (g!=-1)
		parent = &driver_list::driver(g);

	device_iterator deviter(config.root_device());

	for (device_t *device = deviter.first(); device; device = deviter.next())
	{
		for (const rom_entry *region = rom_first_region(*device); region; region = rom_next_region(region))
		{
			for (const rom_entry *rom = rom_first_file(region); rom; rom = rom_next_file(rom))
			{
				hash_collection hashes(ROM_GETHASHDATA(rom));

				if (g!=-1)
				{
					machine_config pconfig(*parent, MameUIGlobal());
					device_iterator deviter(pconfig.root_device());

					for (device_t *device = deviter.first(); device != NULL; device = deviter.next())
						for (const rom_entry *pregion = rom_first_region(*device); pregion != NULL; pregion = rom_next_region(pregion))
							for (const rom_entry *prom = rom_first_file(pregion); prom != NULL; prom = rom_next_file(prom))
							{
								hash_collection phashes(ROM_GETHASHDATA(prom));

								if (hashes == phashes)
									break;
							}
				}

				snprintf(name, ARRAY_LENGTH(name), "%-16s \t", ROM_GETNAME(rom));
				strcat(buffer, name);
				snprintf(name, ARRAY_LENGTH(name), "%09d \t", rom_file_size(rom));
				strcat(buffer, name);
				snprintf(name, ARRAY_LENGTH(name), "%-10s", ROMREGION_GETTAG(region));
				strcat(buffer, name);
				strcat(buffer, "\n");
			}
		}
	}

	samples_device_iterator samplesiter(config.root_device());

	for (samples_device *device = samplesiter.first(); device != NULL; device = samplesiter.next())
	{
		samples_iterator sampiter(*device);

		if (sampiter.altbasename() != NULL)
		{
			snprintf(name, ARRAY_LENGTH(name), "\nSAMPLES (%s):\n", sampiter.altbasename());
			strcat(buffer, name);
		}

		tagmap_t<int> already_printed;

		for (const char *samplename = sampiter.first(); samplename != NULL; samplename = sampiter.next())
		{
			// filter out duplicates
			if (already_printed.add(samplename, 1) == TMERR_DUPLICATE)
				continue;

			// output the sample name
			snprintf(name, ARRAY_LENGTH(name), "%s.wav\n", samplename);
			strcat(buffer, name);
		}
	}

	if (!is_bios)
	{
		int g = driver_list::clone(*drv);

		if (g!=-1)
			drv = &driver_list::driver(g);

		strcat(buffer, "\nORIGINAL:\n");
		strcat(buffer, drv->description);
		strcat(buffer, "\n\nCLONES:\n");

		for (int i = 0; i < driver_list::total(); i++)
		{
			if (!strcmp (drv->name, driver_list::driver(i).parent)) 
			{
				strcat(buffer, driver_list::driver(i).description);
				strcat(buffer, "\n");
			}
		}
	}

	strcat(buffer, "\n");
	return (mameinfo == 0);
}

int load_driver_driverinfo(const game_driver *drv, char *buffer, int bufsize, int filenum)
{
	int drivinfo = 0;
	char source_file[40];
	char tmp[100];
	std::string temp;

	core_filename_extract_base(temp, drv->source_file, FALSE);
	strcpy(source_file, temp.c_str());

	*buffer = 0;

	if (filenum)
		snprintf(filename, ARRAY_LENGTH(filename), "%s\\messinfo.dat", GetDatsDir());
	else
		snprintf(filename, ARRAY_LENGTH(filename), "%s\\mameinfo.dat", GetDatsDir());

	/* Print source code file */
	snprintf(tmp, ARRAY_LENGTH(tmp), "\n\nSOURCE: %s\n\n", source_file);
	strcat(buffer, tmp);

	/* Try to open mameinfo datafile - driver section*/
	if (ParseOpen(filename))
	{
		if (filenum)
		{
			/* create index if necessary */
			if (messdriv_idx)
				drivinfo = 1;
			else
				drivinfo = (index_datafile (&messdriv_idx, 1) != 0);

			/* load informational text (append) */
			if (messdriv_idx)
			{
				int len = strlen(buffer);
				int err = load_datafile_text(drv, buffer + len, bufsize - len, messdriv_idx, DATAFILE_TAG_DRIV, 1, 1);

				if (err) 
					drivinfo = 0;
			}
		}
		else
		{
			/* create index if necessary */
			if (driv_idx)
				drivinfo = 1;
			else
				drivinfo = (index_datafile (&driv_idx, 1) != 0);

			/* load informational text (append) */
			if (driv_idx)
			{
				int len = strlen(buffer);
				int err = load_datafile_text(drv, buffer + len, bufsize - len, driv_idx, DATAFILE_TAG_DRIV, 1, 1);

				if (err) 
					drivinfo = 0;
			}
		}
		ParseClose();
	}

	strcat(buffer, "\nGAMES SUPPORTED:\n");

	for (int i = 0; i < driver_list::total(); i++)
	{
		if (!strcmp(source_file, GetDriverFilename(i)) && !(DriverIsBios(i)))
		{
			strcat(buffer, driver_list::driver(i).description);
			strcat(buffer,"\n");
		}
	}

	strcat(buffer, "\n\n");
	return (drivinfo == 0);
}

int load_driver_command(const game_driver *drv, char *buffer, int bufsize)
{
	int command = 0;

	*buffer = 0;
	snprintf(filename, ARRAY_LENGTH(filename), "%s\\command.dat", GetDatsDir());
	strcat(buffer, "\n**** COMMAND: ****\n\n");

	/* try to open command datafile */
	if (ParseOpen(filename))
	{
		/* create index if necessary */
		if (cmd_idx)
			command = 1;
		else
			command = (index_datafile (&cmd_idx, 0) != 0);

		/* load informational text (append) */
		if (cmd_idx)
		{
			int len = strlen(buffer);
			int err = 0;
			const game_driver *gdrv;
			gdrv = drv;
			
			do
			{
				err = load_datafile_text(gdrv, buffer + len, bufsize - len, cmd_idx, DATAFILE_TAG_CMD, 0, 0);
				int g = driver_list::clone(*gdrv);

				if (g!=-1)
					gdrv = &driver_list::driver(g);
				else
					gdrv = NULL;
			} while (err && gdrv);

			if (err) 
				command = 0;
		}

		ParseClose();
	}

	strcat(buffer, "\n\n");
	return (command == 0);
}

int load_driver_scoreinfo(const game_driver *drv, char *buffer, int bufsize)
{
	int scoreinfo = 0;

	*buffer = 0;
	snprintf(filename, ARRAY_LENGTH(filename), "%s\\story.dat", GetDatsDir());

	/* try to open story datafile */
	if (ParseOpen(filename))
	{
		/* create index if necessary */
		if (score_idx)
			scoreinfo = 1;
		else
			scoreinfo = (index_datafile (&score_idx, 0) != 0);

		/* load informational text (append) */
		if (score_idx)
		{
			int len = strlen(buffer);
			int err = 0;
			const game_driver *gdrv;
			gdrv = drv;
			
			do
			{
				err = load_datafile_text(gdrv, buffer + len, bufsize - len, score_idx, DATAFILE_TAG_SCORE, 0, 0);
				int g = driver_list::clone(*gdrv);

				if (g!=-1)
					gdrv = &driver_list::driver(g);
				else 
					gdrv = NULL;
			} while (err && gdrv);

			if (err) 
				scoreinfo = 0;
		}

		ParseClose();
	}

	return (scoreinfo == 0);
}
