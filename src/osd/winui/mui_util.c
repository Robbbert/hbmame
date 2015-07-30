// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

/***************************************************************************

  mui_util.c

 ***************************************************************************/

// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

// standard C headers
#include <stdio.h>
#include <tchar.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "unzip.h"
#include "sound/samples.h"
#include "winutf8.h"
#include "strconv.h"
#include "winui.h"
#include "mui_util.h"
#include "mui_opts.h"
#include "drivenum.h"

#include <shlwapi.h>
#include <vector>

/***************************************************************************
    function prototypes
 ***************************************************************************/

/***************************************************************************
    External variables
 ***************************************************************************/

/***************************************************************************
    Internal structures
 ***************************************************************************/
struct DriversInfo
{
	int screenCount;
	bool isClone;
	bool isBroken;
	bool isHarddisk;
	bool hasOptionalBIOS;
	bool isStereo;
	bool isVector;
	bool usesRoms;
	bool usesSamples;
	bool usesTrackball;
	bool usesLightGun;
	bool usesMouse;
	bool isVertical;
};

static std::vector<DriversInfo>	drivers_info;


enum
{
	DRIVER_CACHE_SCREEN		= 0x000F,
	DRIVER_CACHE_ROMS		= 0x0010,
	DRIVER_CACHE_CLONE		= 0x0020,
	DRIVER_CACHE_STEREO		= 0x0040,
	DRIVER_CACHE_BIOS		= 0x0080,
	DRIVER_CACHE_TRACKBALL		= 0x0100,
	DRIVER_CACHE_HARDDISK		= 0x0200,
	DRIVER_CACHE_SAMPLES		= 0x0400,
	DRIVER_CACHE_LIGHTGUN		= 0x0800,
	DRIVER_CACHE_VECTOR		= 0x1000,
	DRIVER_CACHE_MOUSE		= 0x2000,
};

/***************************************************************************
    External functions
 ***************************************************************************/

/*
    ErrorMsg
*/
void __cdecl ErrorMsg(const char* fmt, ...)
{
	static FILE* pFile = NULL;
	DWORD dwWritten;
	char buf[5000];
	char buf2[5000];
	va_list va;

	va_start(va, fmt);

	vsprintf(buf, fmt, va);

	win_message_box_utf8(GetActiveWindow(), buf, MAMEUINAME, MB_OK | MB_ICONERROR);

	strcpy(buf2, MAMEUINAME ": ");
	strcat(buf2,buf);
	strcat(buf2, "\n");

	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buf2, strlen(buf2), &dwWritten, NULL);

	if (pFile == NULL)
		pFile = fopen("debug.txt", "wt");

	if (pFile != NULL)
	{
		fprintf(pFile, "%s", buf2);
		fflush(pFile);
	}

	va_end(va);
}

void __cdecl dprintf(const char* fmt, ...)
{
	char	buf[5000];
	va_list va;

	va_start(va, fmt);

	_vsnprintf(buf,sizeof(buf),fmt,va);

	win_output_debug_string_utf8(buf);

	va_end(va);
}

UINT GetDepth(HWND hWnd)
{
	UINT nBPP;
	HDC hDC;

	hDC = GetDC(hWnd);

	nBPP = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);

	ReleaseDC(hWnd, hDC);

	return nBPP;
}

/*
 * Return TRUE if comctl32.dll is version 4.71 or greater
 * otherwise return FALSE.
 */
LONG GetCommonControlVersion()
{
	HMODULE hModule = GetModuleHandle(TEXT("comctl32"));

	if (hModule)
	{
		FARPROC lpfnICCE = GetProcAddress(hModule, "InitCommonControlsEx");

		if (NULL != lpfnICCE)
		{
			FARPROC lpfnDLLI = GetProcAddress(hModule, "DllInstall");

			if (NULL != lpfnDLLI)
			{
				/* comctl 4.71 or greater */

				// see if we can find out exactly

				DLLGETVERSIONPROC pDllGetVersion;
				pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hModule, "DllGetVersion");

				/* Because some DLLs might not implement this function, you
                   must test for it explicitly. Depending on the particular
                   DLL, the lack of a DllGetVersion function can be a useful
                   indicator of the version. */

				if(pDllGetVersion)
				{
					DLLVERSIONINFO dvi;
					HRESULT hr;

					ZeroMemory(&dvi, sizeof(dvi));
					dvi.cbSize = sizeof(dvi);

					hr = (*pDllGetVersion)(&dvi);

					if (SUCCEEDED(hr))
					{
						return PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
					}
				}
				return PACKVERSION(4,71);
			}
			return PACKVERSION(4,7);
		}
		return PACKVERSION(4,0);
	}
	/* DLL not found */
	return PACKVERSION(0,0);
}

void DisplayTextFile(HWND hWnd, const char *cName)
{
	HINSTANCE hErr;
	LPCTSTR msg = 0;
	LPTSTR tName;

	tName = tstring_from_utf8(cName);
	if( !tName )
		return;

	hErr = ShellExecute(hWnd, NULL, tName, NULL, NULL, SW_SHOWNORMAL);
	if ((FPTR)hErr > 32)
	{
		osd_free(tName);
		return;
	}

	switch((FPTR)hErr)
	{
	case 0:
		msg = TEXT("The operating system is out of memory or resources.");
		break;

	case ERROR_FILE_NOT_FOUND:
		msg = TEXT("The specified file was not found.");
		break;

	case SE_ERR_NOASSOC :
		msg = TEXT("There is no application associated with the given filename extension.");
		break;

	case SE_ERR_OOM :
		msg = TEXT("There was not enough memory to complete the operation.");
		break;

	case SE_ERR_PNF :
		msg = TEXT("The specified path was not found.");
		break;

	case SE_ERR_SHARE :
		msg = TEXT("A sharing violation occurred.");
		break;

	default:
		msg = TEXT("Unknown error.");
	}

	MessageBox(NULL, msg, tName, MB_OK);

	osd_free(tName);
}

char* MyStrStrI(const char* pFirst, const char* pSrch)
{
	char* cp = (char*)pFirst;
	char* s1;
	char* s2;

	while (*cp)
	{
		s1 = cp;
		s2 = (char*)pSrch;

		while (*s1 && *s2 && !core_strnicmp(s1, s2, 1))
			s1++, s2++;

		if (!*s2)
			return cp;

		cp++;
	}
	return NULL;
}

char * ConvertToWindowsNewlines(const char *source)
{
	static char buf[2048 * 2048];
	char *dest;

	dest = buf;
	while (*source != 0)
	{
		if (*source == '\n')
		{
			*dest++ = '\r';
			*dest++ = '\n';
		}
		else
			*dest++ = *source;
		source++;
	}
	*dest = 0;
	return buf;
}

/* Lop off path and extension from a source file name
 * This assumes there is a pathname passed to the function
 * like src\drivers\blah.c
 */
const char * GetDriverFilename(int nIndex)
{
	static char tmp[40];
	std::string driver;
	core_filename_extract_base(driver, driver_list::driver(nIndex).source_file, TRUE);
	sprintf(tmp, "%s.c", driver.c_str());
	return tmp;
}

BOOL isDriverVector(const machine_config *config)
{
	const screen_device *screen  = config->first_screen();

	if (screen)
	{
		if (SCREEN_TYPE_VECTOR == screen->screen_type())
			return TRUE;
	}
	return FALSE;
}

int numberOfScreens(const machine_config *config)
{
	const screen_device *screen  = config->first_screen();
	screen_device_iterator iter(config->root_device());
	UINT8 i = 0;
	for (screen = iter.first(); screen; screen = iter.next())
		i++;
	return i;
}


int numberOfSpeakers(const machine_config *config)
{
	speaker_device_iterator iter(config->root_device());
	return iter.count();
}

static void SetDriversInfo(void)
{
	int ndriver;
	int cache = -1;
	int total = driver_list::total();
	struct DriversInfo *gameinfo = NULL;

	for (ndriver = 0; ndriver < total; ndriver++)
	{
		gameinfo = &drivers_info[ndriver];
		cache    = (gameinfo->screenCount & DRIVER_CACHE_SCREEN);
		if (gameinfo->isClone)			cache += DRIVER_CACHE_CLONE;
		if (gameinfo->isHarddisk)		cache += DRIVER_CACHE_HARDDISK;
		if (gameinfo->hasOptionalBIOS)		cache += DRIVER_CACHE_BIOS;
		if (gameinfo->isStereo)			cache += DRIVER_CACHE_STEREO;
		if (gameinfo->isVector)			cache += DRIVER_CACHE_VECTOR;
		if (gameinfo->usesRoms)			cache += DRIVER_CACHE_ROMS;
		if (gameinfo->usesSamples)		cache += DRIVER_CACHE_SAMPLES;
		if (gameinfo->usesTrackball)		cache += DRIVER_CACHE_TRACKBALL;
		if (gameinfo->usesLightGun)		cache += DRIVER_CACHE_LIGHTGUN;
		if (gameinfo->usesMouse)		cache += DRIVER_CACHE_MOUSE;

		SetDriverCache(ndriver, cache);
	}
}

static void InitDriversInfo(void)
{
	int ndriver;
	int num_speakers = 0;
	int total = driver_list::total();
	const game_driver *gamedrv = NULL;
	struct DriversInfo *gameinfo = NULL;
	const rom_entry *region, *rom;

	for (ndriver = 0; ndriver < total; ndriver++)
	{
		gamedrv = &driver_list::driver(ndriver);
		gameinfo = &drivers_info[ndriver];
		machine_config config(*gamedrv, MameUIGlobal());

		gameinfo->isClone = (GetParentRomSetIndex(gamedrv) != -1);
		gameinfo->isBroken = (gamedrv->flags & MACHINE_NOT_WORKING) ? true : false;
		gameinfo->isHarddisk = FALSE;
		gameinfo->isVertical = (gamedrv->flags & ORIENTATION_SWAP_XY) ? TRUE : FALSE;
		device_iterator deviter(config.root_device());
		for (device_t *device = deviter.first(); device; device = deviter.next())
			for (region = rom_first_region(*device); region; region = rom_next_region(region))
				if (ROMREGION_ISDISKDATA(region))
					gameinfo->isHarddisk = TRUE;

		gameinfo->hasOptionalBIOS = FALSE;
		if (gamedrv->rom)
			for (rom = gamedrv->rom; !ROMENTRY_ISEND(rom); rom++)
				if (ROMENTRY_ISSYSTEM_BIOS(rom))
					gameinfo->hasOptionalBIOS = TRUE;

		num_speakers = numberOfSpeakers(&config);

		gameinfo->isStereo = (num_speakers > 1);
		gameinfo->screenCount = numberOfScreens(&config);
		gameinfo->isVector = isDriverVector(&config);
		gameinfo->usesRoms = FALSE;
		for (device_t *device = deviter.first(); device; device = deviter.next())
			for (region = rom_first_region(*device); region; region = rom_next_region(region))
				for (rom = rom_first_file(region); rom; rom = rom_next_file(rom))
					gameinfo->usesRoms = TRUE;

		gameinfo->usesSamples = FALSE;

		samples_device_iterator iter(config.root_device());
		if (iter.first())
			gameinfo->usesSamples = TRUE;

		gameinfo->usesTrackball = FALSE;
		gameinfo->usesLightGun = FALSE;
		if (gamedrv->ipt)
		{
			ioport_port *port;
			ioport_list portlist;
			std::string errors;
			device_iterator iter(config.root_device());
			for (device_t *cfg = iter.first(); cfg; cfg = iter.next())
				if (cfg->input_ports())
					portlist.append(*cfg, errors);

			for (port = portlist.first(); port; port = port->next())
			{
				ioport_field *field;
				for (field = port->first_field(); field; field = field->next())
				{
					UINT32 type;
					type = field->type();
					if (type == IPT_END)
						break;
					if (type == IPT_DIAL || type == IPT_PADDLE ||
						type == IPT_TRACKBALL_X || type == IPT_TRACKBALL_Y ||
						type == IPT_AD_STICK_X || type == IPT_AD_STICK_Y)
						gameinfo->usesTrackball = TRUE;
					if (type == IPT_LIGHTGUN_X || type == IPT_LIGHTGUN_Y)
						gameinfo->usesLightGun = TRUE;
					if (type == IPT_MOUSE_X || type == IPT_MOUSE_Y)
						gameinfo->usesMouse = TRUE;
				}
			}
		}
	}

	SetDriversInfo();
}

static int InitDriversCache(void)
{
	int cache = -1;
	int total = driver_list::total();
	const game_driver *gamedrv = NULL;
	struct DriversInfo *gameinfo = NULL;
	int ndriver;

	if (RequiredDriverCache())
	{
		InitDriversInfo();
		return 0;
	}

	for (ndriver = 0; ndriver < total; ndriver++)
	{
		gamedrv  = &driver_list::driver(ndriver);
		gameinfo = &drivers_info[ndriver];
		cache    = GetDriverCache(ndriver);

		if (cache == -1)
		{
			InitDriversInfo();
			break;
		}

		gameinfo->isBroken          = (gamedrv->flags & MACHINE_NOT_WORKING)    ? TRUE : FALSE;
		gameinfo->isVertical        = (gamedrv->flags & ORIENTATION_SWAP_XY) ? TRUE : FALSE;
		gameinfo->screenCount       =  cache & DRIVER_CACHE_SCREEN;
		gameinfo->isClone           = (cache & DRIVER_CACHE_CLONE)           ? TRUE : FALSE;
		gameinfo->isHarddisk        = (cache & DRIVER_CACHE_HARDDISK)        ? TRUE : FALSE;
		gameinfo->hasOptionalBIOS   = (cache & DRIVER_CACHE_BIOS)            ? TRUE : FALSE;
		gameinfo->isStereo          = (cache & DRIVER_CACHE_STEREO)          ? TRUE : FALSE;
		gameinfo->isVector          = (cache & DRIVER_CACHE_VECTOR)          ? TRUE : FALSE;
		gameinfo->usesRoms          = (cache & DRIVER_CACHE_ROMS)            ? TRUE : FALSE;
		gameinfo->usesSamples       = (cache & DRIVER_CACHE_SAMPLES)         ? TRUE : FALSE;
		gameinfo->usesTrackball     = (cache & DRIVER_CACHE_TRACKBALL)       ? TRUE : FALSE;
		gameinfo->usesLightGun      = (cache & DRIVER_CACHE_LIGHTGUN)        ? TRUE : FALSE;
		gameinfo->usesMouse         = (cache & DRIVER_CACHE_MOUSE)           ? TRUE : FALSE;
	}

	return 0;
}

static struct DriversInfo* GetDriversInfo(int driver_index)
{
	static bool bFirst = true;

	if (bFirst)
	{
		bFirst = false;

		drivers_info.clear();
		drivers_info.reserve(driver_list::total());

		InitDriversCache();
	}

	return &drivers_info[driver_index];
}

BOOL DriverIsClone(int driver_index)
{
	 return GetDriversInfo(driver_index)->isClone;
}

BOOL DriverIsBroken(int driver_index)
{
	return GetDriversInfo(driver_index)->isBroken;
}

BOOL DriverIsHarddisk(int driver_index)
{
	return GetDriversInfo(driver_index)->isHarddisk;
}

BOOL DriverIsBios(int driver_index)
{
	return ( driver_list::driver(driver_index).flags & MACHINE_IS_BIOS_ROOT ) ? TRUE : FALSE;
}

BOOL DriverIsMechanical(int driver_index)
{
	return ( driver_list::driver(driver_index).flags & MACHINE_MECHANICAL ) ? TRUE : FALSE;
}

BOOL DriverIsArcade(int driver_index)
{
	return ( driver_list::driver(driver_index).flags & MACHINE_TYPE_ARCADE ) ? TRUE : FALSE;
}

BOOL DriverHasOptionalBIOS(int driver_index)
{
	return GetDriversInfo(driver_index)->hasOptionalBIOS;
}

BOOL DriverIsStereo(int driver_index)
{
	return GetDriversInfo(driver_index)->isStereo;
}

int DriverNumScreens(int driver_index)
{
	return GetDriversInfo(driver_index)->screenCount;
}

BOOL DriverIsVector(int driver_index)
{
	return GetDriversInfo(driver_index)->isVector;
}

BOOL DriverUsesRoms(int driver_index)
{
	return GetDriversInfo(driver_index)->usesRoms;
}

BOOL DriverUsesSamples(int driver_index)
{
	return GetDriversInfo(driver_index)->usesSamples;
}

BOOL DriverUsesTrackball(int driver_index)
{
	return GetDriversInfo(driver_index)->usesTrackball;
}

BOOL DriverUsesLightGun(int driver_index)
{
	return GetDriversInfo(driver_index)->usesLightGun;
}

BOOL DriverUsesMouse(int driver_index)
{
	return GetDriversInfo(driver_index)->usesMouse;
}

BOOL DriverSupportsSaveState(int driver_index)
{
	return ( driver_list::driver(driver_index).flags & MACHINE_SUPPORTS_SAVE ) ? TRUE : FALSE;
}

BOOL DriverIsVertical(int driver_index)
{
	return GetDriversInfo(driver_index)->isVertical;
}

void FlushFileCaches(void)
{
	zip_file_cache_clear();
}

BOOL StringIsSuffixedBy(const char *s, const char *suffix)
{
	return (strlen(s) > strlen(suffix)) && (strcmp(s + strlen(s) - strlen(suffix), suffix) == 0);
}

/***************************************************************************
    Win32 wrappers
 ***************************************************************************/

BOOL SafeIsAppThemed(void)
{
	BOOL bResult = FALSE;
	HMODULE hThemes;
	BOOL (WINAPI *pfnIsAppThemed)(void);

	hThemes = LoadLibrary(TEXT("uxtheme.dll"));
	if (hThemes != NULL)
	{
		pfnIsAppThemed = (BOOL (WINAPI *)(void)) GetProcAddress(hThemes, "IsAppThemed");
		if (pfnIsAppThemed != NULL)
			bResult = pfnIsAppThemed();
		FreeLibrary(hThemes);
	}
	return bResult;

}


void GetSystemErrorMessage(DWORD dwErrorId, TCHAR **tErrorMessage)
{
	if( FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorId, 0, (LPTSTR)tErrorMessage, 0, NULL) == 0 )
	{
		*tErrorMessage = (LPTSTR)LocalAlloc(LPTR, MAX_PATH * sizeof(TCHAR));
		_tcscpy(*tErrorMessage, TEXT("Unknown Error"));
	}
}


//============================================================
//  win_extract_icon_utf8
//============================================================

HICON win_extract_icon_utf8(HINSTANCE inst, const char* exefilename, UINT iconindex)
{
	HICON icon = 0;
	TCHAR* t_exefilename = tstring_from_utf8(exefilename);
	if( !t_exefilename )
		return icon;

	icon = ExtractIcon(inst, t_exefilename, iconindex);

	osd_free(t_exefilename);

	return icon;
}



//============================================================
//  win_tstring_strdup
//============================================================

TCHAR* win_tstring_strdup(LPCTSTR str)
{
	TCHAR *cpy = NULL;
	if (str != NULL)
	{
		cpy = (TCHAR*)osd_malloc((_tcslen(str) + 1) * sizeof(TCHAR));
		if (cpy != NULL)
			_tcscpy(cpy, str);
	}
	return cpy;
}

//============================================================
//  win_create_file_utf8
//============================================================

HANDLE win_create_file_utf8(const char* filename, DWORD desiredmode, DWORD sharemode,
		LPSECURITY_ATTRIBUTES securityattributes, DWORD creationdisposition,
		DWORD flagsandattributes, HANDLE templatehandle)
{
	HANDLE result = 0;
	TCHAR* t_filename = tstring_from_utf8(filename);
	if( !t_filename )
		return result;

	result = CreateFile(t_filename, desiredmode, sharemode, securityattributes, creationdisposition,
			flagsandattributes, templatehandle);

	osd_free(t_filename);

	return result;
}

//============================================================
//  win_get_current_directory_utf8
//============================================================

DWORD win_get_current_directory_utf8(DWORD bufferlength, char* buffer)
{
	DWORD result = 0;
	TCHAR* t_buffer = NULL;
	char* utf8_buffer = NULL;

	if( bufferlength > 0 )
	{
		t_buffer = (TCHAR*)malloc((bufferlength * sizeof(TCHAR)) + 1);
		if( !t_buffer )
			return result;
	}

	result = GetCurrentDirectory(bufferlength, t_buffer);

	if( bufferlength > 0 )
	{
		utf8_buffer = utf8_from_tstring(t_buffer);
		if( !utf8_buffer )
		{
			osd_free(t_buffer);
			return result;
		}
	}

	strncpy(buffer, utf8_buffer, bufferlength);

	if( utf8_buffer )
		osd_free(utf8_buffer);

	if( t_buffer )
		free(t_buffer);

	return result;
}

//============================================================
//  win_find_first_file_utf8
//============================================================

HANDLE win_find_first_file_utf8(const char* filename, LPWIN32_FIND_DATA findfiledata)
{
	HANDLE result = 0;
	TCHAR* t_filename = tstring_from_utf8(filename);
	if( !t_filename )
		return result;

	result = FindFirstFile(t_filename, findfiledata);

	osd_free(t_filename);

	return result;
}

