/***************************************************************************

  M.A.M.E.UI  -  Multiple Arcade Machine Emulator with User Interface
  Win32 Portions Copyright (C) 1997-2003 Michael Soderstrom and Chris Kirmse,
  Copyright (C) 2003-2007 Chris Kirmse and the MAME32/MAMEUI team.

  This file is part of MAMEUI, and may only be used, modified and
  distributed under the terms of the MAME license, in "readme.txt".
  By continuing to use, modify or distribute this file you indicate
  that you have read the license and understand and accept it fully.

 ***************************************************************************/

/****************************************************************************
 *      datafile.c
 *      History database engine
 *
 *      Token parsing by Neil Bradley
 *      Modifications and higher-level functions by John Butler
 ****************************************************************************/
// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// standard C headers
#include <ctype.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "osdcomm.h"
#include "datafile.h"
#include "mui_opts.h" // For MameUIGlobal()
#include "sound/samples.h"
#include "drivenum.h"

#define MAX_TOKEN_LENGTH        256
#define DATAFILE_TAG '$'

/****************************************************************************
 *      datafile constants
 ****************************************************************************/
static const char *DATAFILE_TAG_KEY	  = "$info";
static const char *DATAFILE_TAG_BIO   = "$bio";
static const char *DATAFILE_TAG_MAME  = "$mame";
static const char *DATAFILE_TAG_DRIV  = "$drv";
static const char *DATAFILE_TAG_END   = "$end";

char *g_history_filename = NULL;
char *g_mameinfo_filename = NULL;


/****************************************************************************
 *      private data for parsing functions
 ****************************************************************************/
static emu_file *fp = NULL;                    	/* Our file pointer */
static UINT64 dwFilePos;                     	/* file position */

/* an array of driver name/drivers array index sorted by driver name
   for fast look up by name */
typedef struct
{
    const char *name;
    int index;
} driver_data_type;
static driver_data_type *sorted_drivers = NULL;
static int num_games;

/**************************************************************************
 **************************************************************************
 *
 *              Parsing functions
 *
 **************************************************************************
 **************************************************************************/

/*
 * DriverDataCompareFunc -- compare function for GetGameNameIndex
 */
static int CLIB_DECL DriverDataCompareFunc(const void *arg1,const void *arg2)
{
    return strcmp( ((driver_data_type *)arg1)->name, ((driver_data_type *)arg2)->name );
}

/*
 * GetGameNameIndex -- given a driver name (in lowercase), return
 * its index in the main drivers[] array, or -1 if it's not found.
 */
static int GetGameNameIndex(const char *name)
{
    driver_data_type *driver_index_info;
	driver_data_type key;
	key.name = name;

	if (sorted_drivers == NULL)
	{
		/* initialize array of game names/indices */
		int i;

		sorted_drivers = (driver_data_type *)malloc(sizeof(driver_data_type) * num_games);
		for (i=0;i<num_games;i++)
		{
			sorted_drivers[i].name = driver_list::driver(i).name;
			sorted_drivers[i].index = i;
		}
		qsort(sorted_drivers,num_games,sizeof(driver_data_type),DriverDataCompareFunc);
	}

	/* uses our sorted array of driver names to get the index in log time */
	driver_index_info = (driver_data_type *)bsearch(&key,sorted_drivers,num_games,sizeof(driver_data_type),
								DriverDataCompareFunc);

	if (driver_index_info == NULL)
		return -1;

	return driver_index_info->index;

}


/****************************************************************************
 *      Create an array with sorted sourcedrivers for the function
 *      index_datafile_drivinfo to speed up the datafile access
 ****************************************************************************/

typedef struct
{
    const char *srcdriver;
    int index;
} srcdriver_data_type;
static srcdriver_data_type *sorted_srcdrivers = NULL;


static int SrcDriverDataCompareFunc(const void *arg1,const void *arg2)
{
	return strcmp( ((srcdriver_data_type *)arg1)->srcdriver, ((srcdriver_data_type *)arg2)->srcdriver );
}


static int GetSrcDriverIndex(const char *srcdriver)
{
	srcdriver_data_type *srcdriver_index_info;
	srcdriver_data_type key;
	key.srcdriver = srcdriver;

	if (sorted_srcdrivers == NULL)
	{
		/* initialize array of game names/indices */
		int i;
		num_games = driver_list::total();

		sorted_srcdrivers = (srcdriver_data_type *)malloc(sizeof(srcdriver_data_type) * num_games);
		for (i=0;i<num_games;i++)
		{
			sorted_srcdrivers[i].srcdriver = driver_list::driver(i).source_file+32;
			sorted_srcdrivers[i].index = i;
		}
		qsort(sorted_srcdrivers,num_games,sizeof(srcdriver_data_type),SrcDriverDataCompareFunc);
	}

	srcdriver_index_info = (srcdriver_data_type *)bsearch(&key,sorted_srcdrivers,num_games,sizeof(srcdriver_data_type),
								SrcDriverDataCompareFunc);

	if (srcdriver_index_info == NULL)
		return -1;

	return srcdriver_index_info->index;

}


/****************************************************************************
 *      ParseClose - Closes the existing opened file (if any)
 ****************************************************************************/
static void ParseClose(void)
{
	/* If the file is open, time for fclose. */
	if (fp)
	{
		fp->close();
		global_free(fp);
	}

	fp = NULL;
}

/****************************************************************************
 *      ParseOpen - Open up file for reading
 ****************************************************************************/
static BOOL ParseOpen(const char *pszFilename)
{
	file_error filerr;

	ParseClose();

	/* Open file up in binary mode */
	fp = global_alloc(emu_file(OPEN_FLAG_READ));
	filerr = fp->open(pszFilename);
	if (filerr != FILERR_NONE)
		return FALSE;

	/* Otherwise, prepare! */
	dwFilePos = 0;

	/* identify text file type first */
	fp->getc();
	fp->seek(dwFilePos, SEEK_SET);

	return TRUE;
}

/****************************************************************************
 *      ParseSeek - Move the file position indicator
 ****************************************************************************/
static UINT8 ParseSeek(UINT64 offset, int whence)
{
	int result = fp->seek(offset, whence);

	if (0 == result)
	{
		dwFilePos = fp->tell();
	}
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
static int index_datafile (struct tDatafileIndex **_index)
{
	struct tDatafileIndex *idx;
    int count = 0;
  	char readbuf[512];
	char name[40];
	num_games = driver_list::total();
	
    /* rewind file */
    if (ParseSeek (0L, SEEK_SET)) 
		return 0;

    /* allocate index */
	idx = *_index = global_alloc_array(tDatafileIndex, (num_games + 1) * sizeof (struct tDatafileIndex));
    if (!idx) 
		return 0;

	while (fp->gets(readbuf, 512))
	{
		/* DATAFILE_TAG_KEY identifies the driver */
		if (!core_strnicmp(DATAFILE_TAG_KEY, readbuf, strlen(DATAFILE_TAG_KEY)))
		{
			int game_index;
			char *curpoint = &readbuf[strlen(DATAFILE_TAG_KEY) + 1];
			char *pch;
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

					game_index = GetGameNameIndex(name);

					if (game_index >= 0)
					{
						idx->driver = &driver_list::driver(game_index);
						idx->offset = fp->tell();
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

					game_index = GetGameNameIndex(name);

					if (game_index >= 0)
					{
						idx->driver = &driver_list::driver(game_index);
						idx->offset = fp->tell();
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

static int index_datafile_drivinfo (struct tDatafileIndex **_index)
{
	struct tDatafileIndex *idx;
    int count = 0;
  	char readbuf[512];
	char name[40];
	num_games = driver_list::total();
	
    /* rewind file */
    if (ParseSeek (0L, SEEK_SET)) 
		return 0;

    /* allocate index */
	idx = *_index = global_alloc_array(tDatafileIndex, (num_games + 1) * sizeof (struct tDatafileIndex));
    if (!idx) 
		return 0;

	while (fp->gets(readbuf, 512))
	{
 		/* DATAFILE_TAG_KEY identifies the driver */
		if (!core_strnicmp(DATAFILE_TAG_KEY, readbuf, strlen(DATAFILE_TAG_KEY)))
		{
			int game_index;
			char *curpoint = &readbuf[strlen(DATAFILE_TAG_KEY) + 1];
			char *pch;
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

					game_index = GetSrcDriverIndex(name);

					if (game_index >= 0)
					{
						idx->driver = &driver_list::driver(game_index);
						idx->offset = fp->tell();
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

					game_index = GetSrcDriverIndex(name);

					if (game_index >= 0)
					{
						idx->driver = &driver_list::driver(game_index);
						idx->offset = fp->tell();
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
static int load_datafile_text (const game_driver *drv, char *buffer, int bufsize, struct tDatafileIndex *idx, const char *tag, int source_file, int carriage)
{
	char readbuf[4096];

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
		return 1; 	/* driver not found in index */

    /* seek to correct point in datafile */
    if (ParseSeek (idx->offset, SEEK_SET)) 
		return 1;

    /* read text until buffer is full or end of entry is encountered */
	while (fp->gets(readbuf, 4096))
	{
		if (!core_strnicmp(DATAFILE_TAG_END, readbuf, strlen(DATAFILE_TAG_END))) 
			break;
		if (!core_strnicmp(tag, readbuf, strlen(tag))) 
			continue;
		if (strlen(buffer) + strlen(readbuf) > bufsize) 
			break;
		
		if (carriage)
		{
			strcat(buffer, readbuf);
			strcat(buffer, "\n");
		}
		else
		{
			if (strtok(readbuf, "\r\n\r\n") != NULL)
				strcat(buffer, readbuf);
			else
			{
				strcat(buffer, readbuf);
				strcat(buffer, "\n");
			}
		}
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
int load_driver_history (const game_driver *drv, char *buffer, int bufsize)
{
	static struct tDatafileIndex *hist_idx = 0;
    int history = 0;
    int err = 0;

    *buffer = 0;
	
	if (!g_history_filename || !*g_history_filename)
		g_history_filename = core_strdup("history.dat");
	
    /* try to open history datafile */
    if (ParseOpen (g_history_filename))
    {
		/* create index if necessary */
        if (hist_idx)
			history = 1;
        else
            history = (index_datafile (&hist_idx) != 0);

        /* load history text */
        if (hist_idx)
        {
            const game_driver *gdrv;

            gdrv = drv;
            do
            {
                err = load_datafile_text (gdrv, buffer, bufsize, hist_idx, DATAFILE_TAG_BIO, 0, 1);
				int g = driver_list::clone(*gdrv);

				if (g!=-1) 
					gdrv = &driver_list::driver(g); 
				else 
					gdrv = NULL;
            } while (err && gdrv);

			if (err) 
				history = 0;
		}
        ParseClose ();
   	}

	strcat(buffer, "\n");
	return (history == 0);
}

int load_driver_mameinfo (const game_driver *drv, char *buffer, int bufsize)
{
	static struct tDatafileIndex *mame_idx = 0;
	machine_config config(*drv,MameUIGlobal());
	const game_driver *parent = NULL;

	char name[300];
	int mameinfo = 0;
	int err = 0;
	int i;
	int has_sound;
	int is_bios = 0;

	*buffer = 0;

	strcat(buffer, "\nMAMEINFO:\n");

	/* List the game info 'flags' */
	if (drv->flags & GAME_NOT_WORKING)
		strcat(buffer, "THIS GAME DOESN'T WORK PROPERLY\n");
	if (drv->flags & GAME_UNEMULATED_PROTECTION)
		strcat(buffer, "The game has protection which isn't fully emulated.\n");
	if (drv->flags & GAME_IMPERFECT_GRAPHICS)
		strcat(buffer, "The video emulation isn't 100% accurate.\n");
	if (drv->flags & GAME_WRONG_COLORS)
		strcat(buffer, "The colors are completely wrong.\n");
	if (drv->flags & GAME_IMPERFECT_COLORS)
		strcat(buffer, "The colors aren't 100% accurate.\n");
	if (drv->flags & GAME_NO_SOUND)
		strcat(buffer, "The game lacks sound.\n");
	if (drv->flags & GAME_IMPERFECT_SOUND)
		strcat(buffer, "The sound emulation isn't 100% accurate.\n");
	if (drv->flags & GAME_SUPPORTS_SAVE)
		strcat(buffer, "Save state support.\n");
//	if (drv->flags & GAME_NO_COCKTAIL)
//		strcat(buffer, "Screen flipping in cocktail mode is not supported.\n");
	if (drv->flags & GAME_MECHANICAL)
		strcat(buffer, "The game contains mechanical parts.\n");
		strcat(buffer, "\n");
	if (drv->flags & GAME_IS_BIOS_ROOT)
		is_bios = 1;

	if (!g_mameinfo_filename || !*g_mameinfo_filename)
		g_mameinfo_filename = core_strdup("mameinfo.dat");

	/* try to open mameinfo datafile */
	if (ParseOpen (g_mameinfo_filename))
	{
		/* create index if necessary */
		if (mame_idx)
			mameinfo = 1;
		else
			mameinfo = (index_datafile (&mame_idx) != 0);

		/* load informational text (append) */
		if (mame_idx)
		{
			int len = strlen (buffer);
			const game_driver *gdrv;

			gdrv = drv;
			do
			{
				err = load_datafile_text (gdrv, buffer+len, bufsize-len, mame_idx, DATAFILE_TAG_MAME, 0, 0);
				int g = driver_list::clone(*gdrv);

				if (g!=-1) 
					gdrv = &driver_list::driver(g); 
				else 
					gdrv = NULL;
			} while (err && gdrv);

			if (err) 
				mameinfo = 0;
		}
		ParseClose ();
	}

	/* GAME INFORMATIONS */
	sprintf(name,"\nGAME: %s\n", drv->name);
	strcat(buffer, name);
	sprintf(name,"%s", drv->description);
	strcat(buffer, name);
	sprintf(name," (%s %s)\n\nCPU:\n", drv->manufacturer, drv->year);
	strcat(buffer, name);

	execute_interface_iterator iter(config.root_device());
	device_execute_interface *cpu = iter.first();
	while (cpu)
	{
		if (cpu->device().clock()-((cpu->device().clock()/1000000)*1000000) > 0)
			sprintf(name,"%s (%d Hz)\n", cpu->device().name(), cpu->device().clock());
		else
			sprintf(name,"%s (%d MHz)\n", cpu->device().name(), cpu->device().clock()/1000000);

		strcat(buffer, name);

		cpu = iter.next();
	}

	strcat(buffer, "\nSOUND:\n");
	i = 0; 
	has_sound = 0;
	
	/* iterate over sound chips */
	sound_interface_iterator sounditer(config.root_device());
	const device_sound_interface *sound = sounditer.first();
	while(sound)
	{
		int clock, count;
		device_type sound_type_;
		char tmpname[1024];

		sprintf(tmpname,"%s",sound->device().name());

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
			sprintf(name,"%dx ",count);
			strcat(buffer, name);
		}
		strcat(buffer, tmpname);

		if (clock)
		{
			if (clock-((clock/1000000)*1000000) > 0)
				sprintf(name," (%d Hz)", clock);
			else
				sprintf(name," (%d MHz)", clock/1000000);
			strcat(buffer, name);
		}
		strcat(buffer,"\n");
	}

	if (has_sound)
	{
		speaker_device_iterator iter(config.root_device());
		int channels = iter.count();
		if(channels == 1)
			sprintf(name,"%d Channel\n",channels);
		else
			sprintf(name,"%dx Channels\n",channels);
		strcat(buffer, name);
	}

	strcat(buffer, "\nVIDEO:\n");
	screen_device_iterator screeniter(config.root_device());
	const screen_device *screen = screeniter.first();
	if (screen == NULL)
		strcat(buffer, "None\n");
	else
	{
		for (; screen != NULL; screen = screeniter.next())
		{
			if (drv->flags & ORIENTATION_SWAP_XY)
				sprintf(name,"%dx%d (V)", screen->visible_area().height(), screen->visible_area().width());
			else
				sprintf(name,"%dx%d (H)", screen->visible_area().width(), screen->visible_area().height());

			strcat(buffer, name);

			i = ATTOSECONDS_TO_HZ(screen->refresh_attoseconds());
			if ((ATTOSECONDS_TO_HZ(screen->refresh_attoseconds()) * 1000000 - i * 1000000) > 0)
				sprintf(name," (%f Hz, ", ATTOSECONDS_TO_HZ(screen->refresh_attoseconds()));
			else
				sprintf(name," (%d Hz, ", i);

			strcat(buffer, name);

			//if (screen->screen_type() == SCREEN_TYPE_VECTOR)
			//	sprintf(name,"palette %d, vector)\n", config.m_total_colors);
			//else
			//	sprintf(name,"palette %d, raster)\n", config.m_total_colors);
			//strcat(buffer, name);
		}
	}

	strcat(buffer, "\nROM REGION:\n");
	int g = driver_list::clone(*drv);
	if (g!=-1) 
		parent = &driver_list::driver(g);

	device_iterator deviter(config.root_device());
	for (device_t *device = deviter.first(); device != NULL; device = deviter.next())
		for (const rom_entry *region = rom_first_region(*device); region != NULL; region = rom_next_region(region))
			for (const rom_entry *rom = rom_first_file(region); rom != NULL; rom = rom_next_file(rom))
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
								{
									break;
								}
							}
				}

				sprintf(name,"%-16s \t", ROM_GETNAME(rom));
				strcat(buffer, name);
				sprintf(name,"%07d \t", rom_file_size(rom));
				strcat(buffer, name);
				sprintf(name,"%-10s", ROMREGION_GETTAG(region));
				strcat(buffer, name);
				strcat(buffer, "\n");
			}

	samples_device_iterator samplesiter(config.root_device());
	for (samples_device *device = samplesiter.first(); device != NULL; device = samplesiter.next())
	{
		samples_iterator sampiter(*device);
		if (sampiter.altbasename() != NULL)
		{
			sprintf(name,"\nSAMPLES (%s):\n", sampiter.altbasename());
			strcat(buffer, name);
		}

		tagmap_t<int> already_printed;
		for (const char *samplename = sampiter.first(); samplename != NULL; samplename = sampiter.next())
		{
			// filter out duplicates
			if (already_printed.add(samplename, 1) == TMERR_DUPLICATE)
				continue;

			// output the sample name
			sprintf(name,"%s.wav\n", samplename);
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
		for (i = 0; i < driver_list::total(); i++)
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

int load_driver_drivinfo (const game_driver *drv, char *buffer, int bufsize)
{
	static struct tDatafileIndex *driv_idx = 0;
	int drivinfo = 0;
	int err;
	int i;

	*buffer = 0;

	/* Print source code file */
	sprintf (buffer, "\n\nSOURCE: %s\n", drv->source_file+32);

	if (!g_mameinfo_filename || !*g_mameinfo_filename)
		g_mameinfo_filename = core_strdup("mameinfo.dat");

	/* Try to open mameinfo datafile - driver section*/
	if (ParseOpen (g_mameinfo_filename))
	{
		/* create index if necessary */
		if (driv_idx)
			drivinfo = 1;
		else
			drivinfo = (index_datafile_drivinfo (&driv_idx) != 0);

		/* load informational text (append) */
		if (driv_idx)
		{
			int len = strlen (buffer);
			err = load_datafile_text (drv, buffer+len, bufsize-len, driv_idx, DATAFILE_TAG_DRIV, 1, 0);
			if (err) 
				drivinfo = 0;
		}
		ParseClose ();
	}

	strcat(buffer,"\nGAMES SUPPORTED:\n");
	for (i = 0; i < driver_list::total(); i++)
	{
		if (!strcmp (drv->source_file+32, driver_list::driver(i).source_file+32) && !(driver_list::driver(i).flags & GAME_IS_BIOS_ROOT)) 
		{
			strcat(buffer, driver_list::driver(i).description);
			strcat(buffer,"\n");
		}
	}

	strcat(buffer, "\n");
	return (drivinfo == 0);

}
