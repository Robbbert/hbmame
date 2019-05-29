// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

/***************************************************************************

  Properties.cpp

    Properties Popup and Misc UI support routines.

    Created 8/29/98 by Mike Haaland (mhaaland@hypertech.com)

***************************************************************************/

/***************************************************************************

MSH - 20070809
--
Notes on properties and ini files, reset and reset to default.
----------------------------------------------------------------------------
Each ini contains a complete option set.

Priority order for option sets (Lowest to Highest):

built-in defaults
program    ini (executable root filename ini)
debug      ini (if running a debug build)
driver     ini (source code root filename in which this driver is found)
game       ini (where game is the driver name for this game)

To determine which option set to use, start at the top level (lowest
priority), and overlay all higher priority ini's until the desired level
is reached.

The 'default' option set is the next priority higher up the list from
the desired level. For the default (program.ini) level, it is also the
default.

When MAME is run, the desired level is game ini.

Expected Code behavior:
----------------------------------------------------------------------------
This approach requires 3 option sets, 'current', 'original' and 'default'.

'current': used to populate the property pages, and to initialize the
'original' set.

'original': used to evaluate if the 'Reset' button is enabled.
If 'current' matches 'original', the 'Reset' button is disabled,
otherwise it is enabled.

'default': used to evaluate if the 'Restore to Defaults' button is enabled.
If 'current' matches 'default', the 'Restore to Defaults' button is disabled,
otherwise it is enabled.

When editing any option set, the desired level is set to the one being
edited, the default set for that level, is the next lower priority set found.

Upon entering the properties dialog:
a) 'current' is initialized
b) 'original' is initialized by 'current'
c) 'default' is initialized
d) Populate Property pages with 'current'
e) 'Reset' and 'Restore to Defaults' buttons are evaluated.

After any change:
a) 'current' is updated
b) 'Reset' and 'Restore to Defaults' buttons are re-evaluated.

Reset:
a) 'current' is reinitialized to 'original'
b) Re-populate Property pages with 'current'
c) 'Reset' and 'Restore to Defaults' buttons are re-evaluated.

Restore to Defaults:
a) 'current' is reinitialized to 'default'
b) Re-populate Property pages with 'current'
b) 'Reset' and 'Restore to Defaults' buttons are re-evaluated.

Apply:
a) 'original' is reinitialized to 'current'
b) 'Reset' and 'Restore to defaults' are re-evaluated.
c) If they 'current' matches 'default', remove the ini from disk.
   Otherwise, write the ini to disk.

Cancel:
a) Exit the dialog.

OK:
a) If they 'current' matches 'default', remove the ini from disk.
   Otherwise, write the ini to disk.
b) Exit the dialog.


***************************************************************************/

// standard windows headers
#include <windows.h>
#include <windowsx.h>

// standard C headers
#include <tchar.h>
#include <sys/stat.h> // for S_IFDIR

// MAME/MAMEUI headers
#include "emu.h"
#include "screen.h"
#include "mui_audit.h"
#include "mui_opts.h"
#include "resource.h"
#include "dijoystick.h"     /* For DIJoystick availability. */
#include "mui_util.h"
#include "datamap.h"
#include "help.h"
#include "winui.h"
#include "strconv.h"
#include "winutf8.h"
#include "directories.h"
#include "sound/samples.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include "newuires.h"
#ifdef UI_DIRECTDRAW
#include "directdraw.h"    /* has to be after samples.h */
#endif
#include "properties.h"
#include "drivenum.h"
#include "machine/ram.h"
#include <shlwapi.h>


#if defined(__GNUC__)
/* fix warning: cast does not match function type */
#undef  PropSheet_GetTabControl
#define PropSheet_GetTabControl(d) (HWND)(LRESULT)(int)SendMessage((d),PSM_GETTABCONTROL,0,0)
#endif /* defined(__GNUC__) */

/* Enable this if MAME supports multiple versions of D3D */
//#define D3DVERSION

/***************************************************************
 * Imported function prototypes
 ***************************************************************/

/**************************************************************
 * Local function prototypes
 **************************************************************/

static void SetSamplesEnabled(HWND hWnd, int nIndex, BOOL bSoundEnabled);
static void InitializeOptions(HWND hDlg);
static void InitializeMisc(HWND hDlg);
static void OptOnHScroll(HWND hWnd, HWND hwndCtl, UINT code, int pos);
static void NumScreensSelectionChange(HWND hwnd);
static void RefreshSelectionChange(HWND hWnd, HWND hWndCtrl);
static void InitializeSoundUI(HWND hwnd);
static void InitializeSkippingUI(HWND hwnd);
static void InitializeRotateUI(HWND hwnd);
static void UpdateSelectScreenUI(HWND hwnd);
static void InitializeSelectScreenUI(HWND hwnd);
#ifdef D3DVERSION
static void InitializeD3DVersionUI(HWND hwnd);
#endif
static void InitializeVideoUI(HWND hwnd);
static void InitializeBIOSUI(HWND hwnd);
static void InitializeControllerMappingUI(HWND hwnd);
static void InitializeLanguageUI(HWND hWnd);
static void InitializePluginsUI(HWND hWnd);
//static void InitializeGLSLFilterUI(HWND hWnd);
static void UpdateOptions(HWND hDlg, datamap *map, windows_options &opts);
static void UpdateProperties(HWND hDlg, datamap *map, windows_options &opts);
static void PropToOptions(HWND hWnd, windows_options &o);
static void OptionsToProp(HWND hWnd, windows_options &o);
static void SetPropEnabledControls(HWND hWnd);
static bool SelectLUAScript(HWND hWnd);
static bool ResetLUAScript(HWND hWnd);
static bool SelectPlugins(HWND hWnd);
static bool ResetPlugins(HWND hWnd);
//static bool SelectBGFXChains(HWND hWnd);
//static bool ResetBGFXChains(HWND hWnd);
static BOOL SelectEffect(HWND hWnd);
static BOOL ResetEffect(HWND hWnd);
static BOOL SelectJoystickMap(HWND hWnd);
static BOOL ResetJoystickMap(HWND hWnd);
static BOOL SelectDebugscript(HWND hWnd);
static BOOL ResetDebugscript(HWND hWnd);

static void BuildDataMap(void);
static void ResetDataMap(HWND hWnd);

static void UpdateBackgroundBrush(HWND hwndTab);
static HBRUSH hBkBrush;

#ifdef MESS
static BOOL DirListReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name);
static BOOL DirListPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name);
static BOOL RamPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name);
extern BOOL BrowseForDirectory(HWND hwnd, LPCTSTR pStartDir, TCHAR* pResult);
//static BOOL g_bModifiedSoftwarePaths = false;
#endif

/**************************************************************
 * Local private variables
 **************************************************************/

/* No longer used by the core, but we need it to predefine configurable screens for all games. */
#ifndef MAX_SCREENS
/* maximum number of screens for one game */
#define MAX_SCREENS 4
#endif

static windows_options pOrigOpts, pDefaultOpts, pCurrentOpts;
static datamap *properties_datamap;

static int  g_nGame            = 0;
static int  g_nFolder          = 0;
static int  g_nFolderGame      = 0;
static OPTIONS_TYPE g_nPropertyMode = OPTIONS_GAME;
static BOOL  g_bAutoAspect[MAX_SCREENS] = {false, false, false, false};
static BOOL  g_bAutoSnapSize = false;
static HICON g_hIcon = NULL;
std::vector<string> plugin_names(32);

/* Property sheets */

#define HIGHLIGHT_COLOR RGB(0,196,0)
static HBRUSH highlight_brush = NULL;
static HBRUSH background_brush = NULL;

#define ORIENTATION_COLOR RGB( 190, 128, 0) //LIGHT BROWN
#define VECTOR_COLOR RGB( 190, 0, 0) //DARK RED
#define FOLDER_COLOR RGB( 0, 128, 0 ) // DARK GREEN
#define PARENT_COLOR RGB( 190, 128, 192 ) // PURPLE
#define GAME_COLOR RGB( 0, 128, 192 ) // DARK BLUE


BOOL PropSheetFilter_Vector(const machine_config *drv, const game_driver *gamedrv)
{
	return isDriverVector(drv);
}

/* Help IDs - moved to auto-generated helpids.c */
extern const DWORD dwHelpIDs[];

typedef struct
{
	const TCHAR* m_pText;
	const char* m_pData;
}
DUALCOMBOSTR;

typedef struct
{
	const TCHAR* m_pText;
	const int m_pData;
}
DUALCOMBOINT;

const DUALCOMBOSTR g_ComboBoxVideo[] =
{
	{ TEXT("Auto"),             "auto"    },
	{ TEXT("GDI"),              "gdi"     },
#ifdef UI_DIRECTDRAW
	{ TEXT("DirectDraw"),       "ddraw"   }, // removed 20160217
#endif
	{ TEXT("Direct3D"),         "d3d"     },
	{ TEXT("BGFX"),             "bgfx"    },
	{ TEXT("OpenGL"),           "opengl"  },
};
#define NUMVIDEO (sizeof(g_ComboBoxVideo) / sizeof(g_ComboBoxVideo[0]))

const DUALCOMBOSTR g_ComboBoxSound[] =
{
	{ TEXT("None"),             "none"    },
	{ TEXT("Auto"),             "auto"    },
	{ TEXT("DirectSound"),      "dsound"  },
	{ TEXT("PortAudio"),        "portaudio" },
//	{ TEXT("XAudio2"),          "xaudio2" },     // invalid option
};
#define NUMSOUND (sizeof(g_ComboBoxSound) / sizeof(g_ComboBoxSound[0]))
#ifdef D3DVERSION
const DUALCOMBOINT g_ComboBoxD3DVersion[] =
{
	{ TEXT("Version 9"),        9   },
//	{ TEXT("Version 8"),        8   },
};

#define NUMD3DVERSIONS (sizeof(g_ComboBoxD3DVersion) / sizeof(g_ComboBoxD3DVersion[0]))
#define WINOPTION_D3DVERSION "9"
#endif
const DUALCOMBOINT g_ComboBoxSelectScreen[] =
{
	{ TEXT("Screen 0"),         0 },
	{ TEXT("Screen 1"),         1 },
	{ TEXT("Screen 2"),         2 },
	{ TEXT("Screen 3"),         3 },
};
#define NUMSELECTSCREEN (sizeof(g_ComboBoxSelectScreen) / sizeof(g_ComboBoxSelectScreen[0]))

const DUALCOMBOSTR g_ComboBoxView[] =
{
	{ TEXT("Auto"),             "auto"        },
	{ TEXT("Standard"),         "standard"    },
	{ TEXT("Pixel Aspect"),     "pixel"       },
	{ TEXT("Cocktail"),         "cocktail"    },
};
#define NUMVIEW (sizeof(g_ComboBoxView) / sizeof(g_ComboBoxView[0]))



const DUALCOMBOSTR g_ComboBoxDevice[] =
{
	{ TEXT("None"),             "none"      },
	{ TEXT("Keyboard"),         "keyboard"  },
	{ TEXT("Mouse"),            "mouse"     },
	{ TEXT("Joystick"),         "joystick"  },
	{ TEXT("Lightgun"),         "lightgun"  },
};

#define NUMDEVICES (sizeof(g_ComboBoxDevice) / sizeof(g_ComboBoxDevice[0]))

const DUALCOMBOSTR g_ComboBoxSnapView[] =
{
	{ TEXT("Internal"),         "internal"    },
	{ TEXT("Auto"),             "auto"        },
	{ TEXT("Standard"),         "standard"    },
	{ TEXT("Pixel Aspect"),     "pixel"       },
	{ TEXT("Cocktail"),         "cocktail"    },
};
#define NUMSNAPVIEW (sizeof(g_ComboBoxSnapView) / sizeof(g_ComboBoxSnapView[0]))


/***************************************************************
 * Public functions
 ***************************************************************/


int PropertiesCurrentGame(HWND hDlg)
{
	return g_nGame;
}

DWORD_PTR GetHelpIDs(void)
{
	return (DWORD_PTR)dwHelpIDs;
}

static int GetSelectedScreen(HWND hWnd)
{
	int nSelectedScreen = 0;
	HWND hCtrl = GetDlgItem(hWnd, IDC_SCREENSELECT);
	if (hCtrl)
		nSelectedScreen = ComboBox_GetCurSel(hCtrl);
	if ((nSelectedScreen < 0) || (nSelectedScreen >= NUMSELECTSCREEN))
		nSelectedScreen = 0;
	return nSelectedScreen;

}

static PROPSHEETPAGE *CreatePropSheetPages(HINSTANCE hInst, BOOL bOnlyDefault,
//	const game_driver *gamedrv, UINT *pnMaxPropSheets, BOOL isGame )
	int nGame, UINT *pnMaxPropSheets, BOOL isGame )
{
	PROPSHEETPAGE *pspages;
	int maxPropSheets = 0;
	int possiblePropSheets;
	int i = ( isGame ) ? 0 : 2;

	for (; g_propSheets[i].pfnDlgProc; i++)
		;

	possiblePropSheets = (isGame) ? i + 1 : i - 1;

	pspages = (PROPSHEETPAGE *)malloc(sizeof(PROPSHEETPAGE) * possiblePropSheets);
	if (!pspages)
		return NULL;

	memset(pspages, 0, sizeof(PROPSHEETPAGE) * possiblePropSheets);

	i = ( isGame ) ? 0 : 2;

	for (; g_propSheets[i].pfnDlgProc; i++)
	{
		if (nGame < 0)
		{
			if (g_propSheets[i].bOnDefaultPage)
			{
				pspages[maxPropSheets].dwSize      = sizeof(PROPSHEETPAGE);
				pspages[maxPropSheets].dwFlags     = 0;
				pspages[maxPropSheets].hInstance   = hInst;
				pspages[maxPropSheets].pszTemplate = MAKEINTRESOURCE(g_propSheets[i].dwDlgID);
				pspages[maxPropSheets].pfnCallback = NULL;
				pspages[maxPropSheets].lParam      = 0;
				pspages[maxPropSheets].pfnDlgProc  = g_propSheets[i].pfnDlgProc;
				maxPropSheets++;
			}
		}
		else
		if ((nGame >= 0) || g_propSheets[i].bOnDefaultPage)
		{
			//machine_config config(*gamedrv,pCurrentOpts);

//			if (!gamedrv || !g_propSheets[i].pfnFilterProc || g_propSheets[i].pfnFilterProc(&config, gamedrv))
			if (nGame < 0 || !g_propSheets[i].pfnFilterProc || g_propSheets[i].pfnFilterProc(nGame))
			{
				pspages[maxPropSheets].dwSize      = sizeof(PROPSHEETPAGE);
				pspages[maxPropSheets].dwFlags     = 0;
				pspages[maxPropSheets].hInstance   = hInst;
				pspages[maxPropSheets].pszTemplate = MAKEINTRESOURCE(g_propSheets[i].dwDlgID);
				pspages[maxPropSheets].pfnCallback = NULL;
				pspages[maxPropSheets].lParam      = 0;
				pspages[maxPropSheets].pfnDlgProc  = g_propSheets[i].pfnDlgProc;
				maxPropSheets++;
			}
		}
	}

	if (pnMaxPropSheets)
		*pnMaxPropSheets = maxPropSheets;

	return pspages;
}

// This is for the DEFAULT property-page options only
void InitDefaultPropertyPage(HINSTANCE hInst, HWND hWnd)
{
	// clear globals
	g_nGame = GLOBAL_OPTIONS;
	windows_options dummy;
	OptionsCopy(dummy,pDefaultOpts);
	OptionsCopy(dummy,pOrigOpts);
	OptionsCopy(dummy,pCurrentOpts);

	/* Get default options to populate property sheets */
	load_options(pCurrentOpts, OPTIONS_GLOBAL, g_nGame, 0);
	load_options(pOrigOpts, OPTIONS_GLOBAL, g_nGame, 0);
	load_options(pDefaultOpts, OPTIONS_GLOBAL, -2, 0);

	g_nPropertyMode = OPTIONS_GLOBAL;
	BuildDataMap();

	PROPSHEETHEADER pshead;
	ZeroMemory(&pshead, sizeof(pshead));

	PROPSHEETPAGE   *pspage;
	pspage = CreatePropSheetPages(hInst, true, -1, &pshead.nPages, false);
	if (!pspage)
		return;

	/* Fill in the property sheet header */
	pshead.hwndParent   = hWnd;
	pshead.dwSize       = sizeof(PROPSHEETHEADER);
	pshead.dwFlags      = PSH_PROPSHEETPAGE | PSH_USEICONID | PSH_PROPTITLE;
	pshead.hInstance    = hInst;
	pshead.pszCaption   = TEXT("Default Game");
	pshead.nStartPage   = 0;
	pshead.pszIcon      = MAKEINTRESOURCE(IDI_MAMEUI_ICON);
	pshead.ppsp         = pspage;

	/* Create the Property sheet and display it */
	if (PropertySheet(&pshead) == -1)
	{
		char temp[100];
		DWORD dwError = GetLastError();
		sprintf(temp, "Property Sheet Error %d %X", (int)dwError, (int)dwError);
		win_message_box_utf8(0, temp, "Error", IDOK);
	}

	free(pspage);
}

/* Initialize the property pages for anything but the Default option set */
void InitPropertyPage(HINSTANCE hInst, HWND hWnd, HICON hIcon, OPTIONS_TYPE opt_type, int folder_id, int game_num)
{
	InitPropertyPageToPage(hInst, hWnd, hIcon, opt_type, folder_id, game_num, PROPERTIES_PAGE);
}

void InitPropertyPageToPage(HINSTANCE hInst, HWND hWnd, HICON hIcon, OPTIONS_TYPE opt_type, int folder_id, int game_num, int start_page )
{
	if (highlight_brush == NULL)
		highlight_brush = CreateSolidBrush(HIGHLIGHT_COLOR);

	if (background_brush == NULL)
		background_brush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	// Initialize the options
	windows_options dummy;
	OptionsCopy(dummy,pDefaultOpts);
	OptionsCopy(dummy,pOrigOpts);
	OptionsCopy(dummy,pCurrentOpts);

	load_options(pCurrentOpts, opt_type, game_num, 1);
	load_options(pOrigOpts, opt_type, game_num, 1);
	if (game_num == GLOBAL_OPTIONS)
		load_options(pDefaultOpts, OPTIONS_GLOBAL, -2, 0); // base opts is the backup for global
	else
		load_options(pDefaultOpts, OPTIONS_GLOBAL, -1, 0); // global is the backup for games

	// Copy icon to use for the property pages
	g_hIcon = CopyIcon(hIcon);

	// These MUST be valid, they are used as indicies
	g_nGame = game_num;
	g_nFolder = folder_id;

	// Keep track of OPTIONS_TYPE that was passed in.
	g_nPropertyMode = opt_type;

	BuildDataMap();

	PROPSHEETHEADER pshead;
	ZeroMemory(&pshead, sizeof(PROPSHEETHEADER));

	// Set the game to audit to this game

	// Create the property sheets
	PROPSHEETPAGE *pspage;
	if( OPTIONS_GAME == opt_type )
	{
		InitGameAudit(game_num);
		pspage = CreatePropSheetPages(hInst, false, game_num, &pshead.nPages, true);
	}
	else
		pspage = CreatePropSheetPages(hInst, false, -1, &pshead.nPages, false);

	if (!pspage)
		return;

	// Get the description use as the dialog caption.
	TCHAR* t_description = 0;
	switch( opt_type )
	{
	case OPTIONS_GAME:
		t_description = ui_wstring_from_utf8(ModifyThe(driver_list::driver(g_nGame).type.fullname()));
		break;
	case OPTIONS_SOURCE:
		t_description = ui_wstring_from_utf8(GetDriverFilename(g_nGame));
		break;
	case OPTIONS_GLOBAL:
		t_description = ui_wstring_from_utf8("Default Settings");
		break;
	default:
		return;
	}
	// If we have no description, return.
	if( !t_description )
		return;

	/* Fill in the property sheet header */
	pshead.pszCaption = t_description;
	pshead.hwndParent = hWnd;
	pshead.dwSize     = sizeof(PROPSHEETHEADER);
	pshead.dwFlags    = PSH_PROPSHEETPAGE | PSH_USEICONID | PSH_PROPTITLE;
	pshead.hInstance  = hInst;
	pshead.nStartPage = start_page;
	pshead.pszIcon    = MAKEINTRESOURCE(IDI_MAMEUI_ICON);
	pshead.ppsp       = pspage;

	/* Create the Property sheet and display it */
	if (PropertySheet(&pshead) == -1)
	{
		char temp[100];
		DWORD dwError = GetLastError();
		sprintf(temp, "Property Sheet Error %d %X", (int)dwError, (int)dwError);
		win_message_box_utf8(0, temp, "Error", IDOK);
	}

	free(t_description);
	free(pspage);
}


/*********************************************************************
 * Local Functions
 *********************************************************************/
#define WINUI_ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

/* Build CPU info string */
static char *GameInfoCPU(int nIndex)
{
	machine_config config(driver_list::driver(nIndex), MameUIGlobal());
	execute_interface_iterator cpuiter(config.root_device());
	std::unordered_set<std::string> exectags;
	static char buffer[1024];

	memset(&buffer, 0, sizeof(buffer));

	for (device_execute_interface &exec : cpuiter)
	{
		if (!exectags.insert(exec.device().tag()).second)
			continue;

		char temp[300];
		int count = 1;
		int clock = exec.device().clock();
		const char *name = exec.device().name();

		for (device_execute_interface &scan : cpuiter)
		{
			if (exec.device().type() == scan.device().type() && strcmp(name, scan.device().name()) == 0 && clock == scan.device().clock())
				if (exectags.insert(scan.device().tag()).second)
					count++;
		}

		if (count > 1)
		{
			snprintf(temp, WINUI_ARRAY_LENGTH(temp), "%d x ", count);
			strcat(buffer, temp);
		}

		if (clock >= 1000000)
			snprintf(temp, WINUI_ARRAY_LENGTH(temp), "%s %d.%06d MHz\r\n", name, clock / 1000000, clock % 1000000);
		else
			snprintf(temp, WINUI_ARRAY_LENGTH(temp), "%s %d.%03d kHz\r\n", name, clock / 1000, clock % 1000);

		strcat(buffer, temp);
	}

	return buffer;
}

/* Build Sound system info string */
static char *GameInfoSound(int nIndex)
{
	machine_config config(driver_list::driver(nIndex), MameUIGlobal());
	sound_interface_iterator sounditer(config.root_device());
	std::unordered_set<std::string> soundtags;
	static char buffer[1024];

	memset(&buffer, 0, sizeof(buffer));

	for (device_sound_interface &sound : sounditer)
	{
		if (!soundtags.insert(sound.device().tag()).second)
				continue;

		char temp[300];
		int count = 1;
		int clock = sound.device().clock();
		const char *name = sound.device().name();

		for (device_sound_interface &scan : sounditer)
		{
			if (sound.device().type() == scan.device().type() && strcmp(name, scan.device().name()) == 0 && clock == scan.device().clock())
				if (soundtags.insert(scan.device().tag()).second)
					count++;
		}

		if (count > 1)
		{
			snprintf(temp, WINUI_ARRAY_LENGTH(temp), "%d x ", count);
			strcat(buffer, temp);
		}

		strcat(buffer, name);

		if (clock)
		{
			if (clock >= 1000000)
				snprintf(temp, WINUI_ARRAY_LENGTH(temp), " %d.%06d MHz", clock / 1000000, clock % 1000000);
			else
				snprintf(temp, WINUI_ARRAY_LENGTH(temp), " %d.%03d kHz", clock / 1000, clock % 1000);

			strcat(buffer, temp);
		}

		strcat(buffer, "\r\n");
	}

	return buffer;
}

/* Build Display info string */
static char *GameInfoScreen(UINT nIndex)
{
	static char buf[2048];
	machine_config config(driver_list::driver(nIndex),pCurrentOpts);
	memset(buf, '\0', 2048);

	if (isDriverVector(&config))
		strcpy(buf, "Vector Game");
	else
	{
		screen_device_iterator iter(config.root_device());
		const screen_device *screen = iter.first();
		if (screen == NULL)
			strcpy(buf, "Screenless Game");
		else
		{
			for (screen_device &screen : screen_device_iterator(config.root_device()))
			{
				char tmpbuf[2048];
				const rectangle &visarea = screen.visible_area();

				if (BIT(GetDriverCacheLower(nIndex), 2)) //ORIENTATION_SWAP_XY
				{
					sprintf(tmpbuf,"%d x %d (V) %f Hz\n",
							visarea.max_y - visarea.min_y + 1,
							visarea.max_x - visarea.min_x + 1,
							ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));
				}
				else
				{
					sprintf(tmpbuf,"%d x %d (H) %f Hz\n",
							visarea.max_x - visarea.min_x + 1,
							visarea.max_y - visarea.min_y + 1,
							ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));
				}
				strcat(buf, tmpbuf);
			}
		}
	}
	return buf;
}


/* Build game status string */
const char *GameInfoStatus(int driver_index, BOOL bRomStatus)
{
	static char buffer[2048];
	memset(buffer,0,sizeof(char)*2048);
	if (driver_index < 0)
		return buffer;

	int audit_result = GetRomAuditResults(driver_index);
	uint32_t cache = GetDriverCacheLower(driver_index);
	if ( bRomStatus )
	{
		if (IsAuditResultKnown(audit_result) == false)
			strcpy(buffer, "Unknown");
		else
		if (IsAuditResultYes(audit_result))
		{
			if (DriverIsBroken(driver_index))
			{
				strcpy(buffer, "Not working");

				if (BIT(cache, 22))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game protection isn't fully emulated");
				}
				if (BIT(cache, 21))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors are completely wrong");
				}
				if (BIT(cache, 20))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors aren't 100% accurate");
				}
				if (BIT(cache, 18))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Video emulation isn't 100% accurate");
				}
				if (BIT(cache, 17))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game lacks sound");
				}
				if (BIT(cache, 16))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Sound emulation isn't 100% accurate");
				}
				if (BIT(cache, 8))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Screen flipping is not supported");
				}
				if (BIT(cache, 10))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game requires artwork");
				}
			}
			else
			{
				strcpy(buffer, "Working");

				if (BIT(cache, 22))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game protection isn't fully emulated");
				}
				if (BIT(cache, 21))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors are completely wrong");
				}
				if (BIT(cache, 20))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors aren't 100% accurate");
				}
				if (BIT(cache, 18))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Video emulation isn't 100% accurate");
				}
				if (BIT(cache, 17))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game lacks sound");
				}
				if (BIT(cache, 16))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Sound emulation isn't 100% accurate");
				}
				if (BIT(cache, 8))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Screen flipping is not supported");
				}
				if (BIT(cache, 10))
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game requires artwork");
				}
			}
		}
		else
			// audit result is no
			strcpy(buffer, "BIOS missing");
	}
	else
	{
		//Just show the emulation flags
		if (DriverIsBroken(driver_index))
			strcpy(buffer, "Not working");
		else
			strcpy(buffer, "Working");

		if (BIT(cache, 22))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Game protection isn't fully emulated");
		}
		if (BIT(cache, 21))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Colors are completely wrong");
		}
		if (BIT(cache, 20))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Colors aren't 100% accurate");
		}
		if (BIT(cache, 18))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Video emulation isn't 100% accurate");
		}
		if (BIT(cache, 17))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Game lacks sound");
		}
		if (BIT(cache, 16))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Sound emulation isn't 100% accurate");
		}
		if (BIT(cache, 8))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Screen flipping is not supported");
		}
		if (BIT(cache, 10))
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Game requires artwork");
		}
	}
	return buffer;
}

/* Build game manufacturer string */
static char *GameInfoManufactured(UINT nIndex)
{
	static char buffer[1024];

	snprintf(buffer,sizeof(buffer),"%s %s",driver_list::driver(nIndex).year,driver_list::driver(nIndex).manufacturer);
	return buffer;
}

/* Build Game title string */
char *GameInfoTitle(OPTIONS_TYPE opt_type, UINT nIndex)
{
	static char buf[1024];

	if (OPTIONS_GLOBAL == opt_type)
		strcpy(buf, "Global game options\nDefault options used by all games");
	else
	if (OPTIONS_SOURCE == opt_type)
		strcpy(buf, "Options used by all games in the source");
	else
	if (OPTIONS_VECTOR == opt_type)
		strcpy(buf, "Global vector options");
	else
	if (OPTIONS_GAME == opt_type)
		sprintf(buf, "%s\n\"%s\"", ModifyThe(driver_list::driver(nIndex).type.fullname()), driver_list::driver(nIndex).name);
	return buf;
}

/* Build game clone information string */
static char *GameInfoCloneOf(UINT nIndex)
{
	static char buf[1024];
	int nParentIndex= -1;

	buf[0] = '\0';

	if (DriverIsClone(nIndex))
	{
		nParentIndex = GetParentIndex(&driver_list::driver(nIndex));
		sprintf(buf, "%s - \"%s\"",
			ConvertAmpersandString(ModifyThe(driver_list::driver(nParentIndex).type.fullname())),
			driver_list::driver(nParentIndex).name);
	}

	return buf;
}

static const char * GameInfoSource(UINT nIndex)
{
	return GetDriverFilename(nIndex);
}

/* Handle the information property page */
INT_PTR CALLBACK GamePropertiesDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
HWND hWnd;
	switch (Msg)
	{
	case WM_INITDIALOG:
		if (g_hIcon)
			SendDlgItemMessage(hDlg, IDC_GAME_ICON, STM_SETICON, (WPARAM) g_hIcon, 0);
#if defined(USE_SINGLELINE_TABCONTROL)
		{
			HWND hWnd = PropSheet_GetTabControl(GetParent(hDlg));
			DWORD tabStyle = (GetWindowLong(hWnd,GWL_STYLE) & ~TCS_MULTILINE);
			SetWindowLong(hWnd,GWL_STYLE,tabStyle | TCS_SINGLELINE);
		}
#endif

		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_TITLE),         GameInfoTitle(g_nPropertyMode, g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_MANUFACTURED),  GameInfoManufactured(g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_STATUS),        GameInfoStatus(g_nGame, false));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_CPU),           GameInfoCPU(g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_SOUND),         GameInfoSound(g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_SCREEN),        GameInfoScreen(g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_CLONEOF),       GameInfoCloneOf(g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_SOURCE),        GameInfoSource(g_nGame));

		if (DriverIsClone(g_nGame))
			ShowWindow(GetDlgItem(hDlg, IDC_PROP_CLONEOF_TEXT), SW_SHOW);
		else
			ShowWindow(GetDlgItem(hDlg, IDC_PROP_CLONEOF_TEXT), SW_HIDE);

		hWnd = PropSheet_GetTabControl(GetParent(hDlg));
		UpdateBackgroundBrush(hWnd);
		ShowWindow(hDlg, SW_SHOW);
		return 1;
	}
	return 0;
}

/* Handle all options property pages */
INT_PTR CALLBACK GameOptionsProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	BOOL g_bUseDefaults = false, g_bReset = false;

	switch (Msg)
	{
	case WM_INITDIALOG:
		/* Fill in the Game info at the top of the sheet */
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_TITLE), GameInfoTitle(g_nPropertyMode, g_nGame));
		InitializeOptions(hDlg);
		InitializeMisc(hDlg);

		UpdateProperties(hDlg, properties_datamap, pCurrentOpts);

		g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
		g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;

		// Default button doesn't exist on Default settings
//		if (g_nGame == GLOBAL_OPTIONS)
//			ShowWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), SW_HIDE);
//		else
			EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);

		// Setup Reset button
		EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
		ShowWindow(hDlg, SW_SHOW);
		return 1;

	case WM_HSCROLL:
		/* slider changed */
		HANDLE_WM_HSCROLL(hDlg, wParam, lParam, OptOnHScroll);
		EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), true);

		// Enable Apply button
		PropSheet_Changed(GetParent(hDlg), hDlg);

		// make sure everything's copied over, to determine what's changed
		UpdateOptions(hDlg, properties_datamap, pCurrentOpts);

		// redraw it, it might be a new color now
		InvalidateRect((HWND)lParam,NULL,true);

		break;

	case WM_COMMAND:
		{
			/* Below, 'changed' is used to signify the 'Apply' button should be enabled. */
			WORD wID         = GET_WM_COMMAND_ID(wParam, lParam);
			HWND hWndCtrl    = GET_WM_COMMAND_HWND(wParam, lParam);
			WORD wNotifyCode = GET_WM_COMMAND_CMD(wParam, lParam);
			BOOL changed     = false;
			int nCurSelection = 0;
			TCHAR szClass[256];

			switch (wID)
			{
			case IDC_REFRESH:
				if (wNotifyCode == LBN_SELCHANGE)
				{
					RefreshSelectionChange(hDlg, hWndCtrl);
					changed = true;
				}
				break;

			case IDC_ASPECT:
				nCurSelection = Button_GetCheck( GetDlgItem(hDlg, IDC_ASPECT));
				if( g_bAutoAspect[GetSelectedScreen(hDlg)] != nCurSelection )
				{
					changed = true;
					g_bAutoAspect[GetSelectedScreen(hDlg)] = nCurSelection;
				}
				break;

			case IDC_SNAPSIZE:
				nCurSelection = Button_GetCheck( GetDlgItem(hDlg, IDC_SNAPSIZE));
				if( g_bAutoSnapSize != nCurSelection )
				{
					changed = true;
					g_bAutoSnapSize = nCurSelection;
				}
				break;

			case IDC_SELECT_EFFECT:
				changed = SelectEffect(hDlg);
				break;

			case IDC_RESET_EFFECT:
				changed = ResetEffect(hDlg);
				break;

			case IDC_SELECT_JOYSTICKMAP:
				changed = SelectJoystickMap(hDlg);
				break;

			case IDC_RESET_JOYSTICKMAP:
				changed = ResetJoystickMap(hDlg);
				break;

			case IDC_SELECT_DEBUGSCRIPT:
				changed = SelectDebugscript(hDlg);
				break;

			case IDC_RESET_DEBUGSCRIPT:
				changed = ResetDebugscript(hDlg);
				break;

			case IDC_SELECT_LUASCRIPT:
				changed = SelectLUAScript(hDlg);
				break;

			case IDC_RESET_LUASCRIPT:
				changed = ResetLUAScript(hDlg);
				break;

			case IDC_SELECT_PLUGIN:
				changed = SelectPlugins(hDlg);
				break;

			case IDC_RESET_PLUGIN:
				changed = ResetPlugins(hDlg);
				break;

			case IDC_PROP_RESET:
				// RESET Button - Only do it if mouse-clicked
				if (wNotifyCode != BN_CLICKED)
					break;

				// Change settings in property sheets back to original
				UpdateProperties(hDlg, properties_datamap, pOrigOpts);
				// The original options become the current options.
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);

				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				// Turn off Apply
				PropSheet_UnChanged(GetParent(hDlg), hDlg);
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
				break;

			case IDC_USE_DEFAULT:
				// DEFAULT Button - Only do it if mouse-clicked
				if (wNotifyCode != BN_CLICKED)
					break;

				// Change settings to be the same as mame.ini
				UpdateProperties(hDlg, properties_datamap, pDefaultOpts);
				// The original options become the current options.
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);

				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				// Enable/Disable the Reset to Defaults button
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
				// Tell the dialog to enable/disable the apply button.
				if (g_nGame != GLOBAL_OPTIONS)
				{
					if (g_bReset)
						PropSheet_Changed(GetParent(hDlg), hDlg);
					else
						PropSheet_UnChanged(GetParent(hDlg), hDlg);
				}
				break;

				// MSH 20070813 - Update all related controls
			case IDC_SCREENSELECT:
			case IDC_SCREEN:
				// NPW 3-Apr-2007:  Ugh I'm only perpetuating the vile hacks in this code
				if ((wNotifyCode == CBN_SELCHANGE) || (wNotifyCode == CBN_SELENDOK))
				{
					changed = datamap_read_control(properties_datamap, hDlg, pCurrentOpts, wID);
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_SIZES);
					//MSH 20070814 - Hate to do this, but its either this, or update each individual
					// control on the SCREEN tab.
					UpdateProperties(hDlg, properties_datamap, pCurrentOpts);
					changed = true;
					/*   NOT USED *************
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_SCREENSELECT);
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_SCREEN);
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_REFRESH);
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_SIZES);
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_VIEW);
					datamap_populate_control(properties_datamap, hDlg, pCurrentOpts, IDC_SWITCHRES);

					if (strcmp(options_get_string(pCurrentOpts, "screen0"), options_get_string(pOrigOpts, "screen0")!=0) ||
					strcmp(options_get_string(pCurrentOpts, "screen1"), options_get_string(pOrigOpts, "screen1")!=0) ||
					strcmp(options_get_string(pCurrentOpts, "screen2"), options_get_string(pOrigOpts, "screen2")!=0) ||
					strcmp(options_get_string(pCurrentOpts, "screen3"), options_get_string(pOrigOpts, "screen3")!=0))
						changed = true;
				*************************************  */
				}
				break;
			default:
#ifdef MESS
			if (MessPropertiesCommand(hDlg, wNotifyCode, wID, &changed))
				// To Do: add a hook to MessReadMountedSoftware(drvindex); so the software will update itself when the folder is configured
					break;
#endif

				// use default behavior; try to get the result out of the datamap if
				// appropriate
				GetClassName(hWndCtrl, szClass, ARRAY_LENGTH(szClass));
				if (!_tcscmp(szClass, WC_COMBOBOX))
				{
					// combo box
					if ((wNotifyCode == CBN_SELCHANGE) || (wNotifyCode == CBN_SELENDOK))
						changed = datamap_read_control(properties_datamap, hDlg, pCurrentOpts, wID);
				}
				else if (!_tcscmp(szClass, WC_BUTTON) && (GetWindowLong(hWndCtrl, GWL_STYLE) & BS_CHECKBOX))
				{
					// check box
					changed = datamap_read_control(properties_datamap, hDlg, pCurrentOpts, wID);
				}
				break;
			}

			if (changed == true)
			{
				// make sure everything's copied over, to determine what's changed
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);
				// enable the apply button
				PropSheet_Changed(GetParent(hDlg), hDlg);
				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
			}
		}
		break;

	case WM_NOTIFY:
		{
			// Set to true if we are exiting the properties dialog
			//BOOL bClosing = ((LPPSHNOTIFY) lParam)->lParam; // indicates OK was clicked rather than APPLY

			switch (((NMHDR *) lParam)->code)
			{
				//We'll need to use a CheckState Table
				//Because this one gets called for all kinds of other things too, and not only if a check is set
			case PSN_SETACTIVE:
				/* Initialize the controls. */
				UpdateProperties(hDlg, properties_datamap, pCurrentOpts);
				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
				break;

			case PSN_APPLY:
				// Handle more than one PSN_APPLY, since this proc handles more than one
				// property sheet and can be called multiple times when it's time to exit,
				// and we may have already freed the windows_options.
				//if (bClosing)
				//{
					//if (NULL == pCurrentOpts)
					//return true;
				//}

				// Read the datamap
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);
				// The current options become the original options.
				UpdateOptions(hDlg, properties_datamap, pOrigOpts);

				// Repopulate the controls?  WTF?  We just read them, they should be fine.
				UpdateProperties(hDlg, properties_datamap, pCurrentOpts);

				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);

				// Save the current options
				save_options(pCurrentOpts, g_nPropertyMode, g_nGame);

				// Disable apply button
				PropSheet_UnChanged(GetParent(hDlg), hDlg);
				SetWindowLongPtr(hDlg, DWLP_MSGRESULT, PSNRET_NOERROR);

				return true;

			case PSN_KILLACTIVE:
				/* Save Changes to the options here. */
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);
				// Determine button states.
				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);

				ResetDataMap(hDlg);
				SetWindowLongPtr(hDlg, DWLP_MSGRESULT, false);
				return 1;

			case PSN_RESET:
				// Reset to the original values. Disregard changes
				//pCurrentOpts = pOrigOpts;
				g_bUseDefaults = AreOptionsEqual(pCurrentOpts, pDefaultOpts) ? false : true;
				g_bReset = AreOptionsEqual(pCurrentOpts, pOrigOpts) ? false : true;
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
				SetWindowLongPtr(hDlg, DWLP_MSGRESULT, false);
				break;

			case PSN_HELP:
				// User wants help for this property page
				break;
			}
		}
		break;

	case WM_HELP:
		/* User clicked the ? from the upper right on a control */
		HelpFunction((HWND)((LPHELPINFO)lParam)->hItemHandle, MAMEUICONTEXTHELP, HH_TP_HELP_WM_HELP, GetHelpIDs());
		break;

	case WM_CONTEXTMENU:
		HelpFunction((HWND)wParam, MAMEUICONTEXTHELP, HH_TP_HELP_CONTEXTMENU, GetHelpIDs());
		break;

	}

	return 0;
}

/* Read controls that are not handled in the DataMap */
static void PropToOptions(HWND hWnd, windows_options &o)
{
	HWND hCtrl;
	HWND hCtrl2;
	HWND hCtrl3;

	/* aspect ratio */
	hCtrl  = GetDlgItem(hWnd, IDC_ASPECTRATION);
	hCtrl2 = GetDlgItem(hWnd, IDC_ASPECTRATIOD);
	hCtrl3 = GetDlgItem(hWnd, IDC_ASPECT);
	if (hCtrl && hCtrl2 && hCtrl3)
	{
		char aspect_option[32];
		snprintf(aspect_option, ARRAY_LENGTH(aspect_option), "aspect%d", GetSelectedScreen(hWnd));

		if (Button_GetCheck(hCtrl3))
		{
			o.set_value(aspect_option, "auto", OPTION_PRIORITY_CMDLINE);
		}
		else
		{
			int n = 0;
			int d = 0;
			TCHAR buffer[200];
			char buffer2[200];

			Edit_GetText(hCtrl, buffer, ARRAY_LENGTH(buffer));
			_stscanf(buffer,TEXT("%d"),&n);

			Edit_GetText(hCtrl2, buffer, ARRAY_LENGTH(buffer));
			_stscanf(buffer,TEXT("%d"),&d);

			if (n == 0 || d == 0)
			{
				n = 4;
				d = 3;
			}

			snprintf(buffer2, sizeof(buffer2), "%d:%d", n, d);
			o.set_value(aspect_option, buffer2, OPTION_PRIORITY_CMDLINE);
		}
	}
	/* aspect ratio */
	hCtrl  = GetDlgItem(hWnd, IDC_SNAPSIZEWIDTH);
	hCtrl2 = GetDlgItem(hWnd, IDC_SNAPSIZEHEIGHT);
	hCtrl3 = GetDlgItem(hWnd, IDC_SNAPSIZE);
	if (hCtrl && hCtrl2 && hCtrl3)
	{
		if (Button_GetCheck(hCtrl3))
		{
			o.set_value(OPTION_SNAPSIZE, "auto", OPTION_PRIORITY_CMDLINE);
		}
		else
		{
			int width = 0;
			int height = 0;
			TCHAR buffer[200];
			char buffer2[200];

			Edit_GetText(hCtrl, buffer, ARRAY_LENGTH(buffer));
			_stscanf(buffer, TEXT("%d"), &width);

			Edit_GetText(hCtrl2, buffer, ARRAY_LENGTH(buffer));
			_stscanf(buffer, TEXT("%d"), &height);

			if (width == 0 || height == 0)
			{
				width = 640;
				height = 480;
			}

			snprintf(buffer2, sizeof(buffer2), "%dx%d", width, height);
			o.set_value(OPTION_SNAPSIZE, buffer2, OPTION_PRIORITY_CMDLINE);
		}
	}
}

/* Update options from the dialog */
static void UpdateOptions(HWND hDlg, datamap *map, windows_options &opts)
{
	/* These are always called together, so make one convenient function. */
	datamap_read_all_controls(map, hDlg, opts);
	PropToOptions(hDlg, opts);
}

/* Update the dialog from the options */
static void UpdateProperties(HWND hDlg, datamap *map, windows_options &opts)
{
	/* These are always called together, so make one convenient function. */
	datamap_populate_all_controls(map, hDlg, opts);
	OptionsToProp(hDlg, opts);
	SetPropEnabledControls(hDlg);
}

/* Populate controls that are not handled in the DataMap */
static void OptionsToProp(HWND hWnd, windows_options& o)
{
	HWND hCtrl2;
	TCHAR buf[1024];
	int  n = 0;
	int  d = 0;
	int  width = 0;
	int  height = 0;
	int res = 0;

	/* video */

	/* Setup refresh list based on depth. */
	datamap_update_control(properties_datamap, hWnd, pCurrentOpts, IDC_REFRESH);
	/* Setup Select screen*/
	UpdateSelectScreenUI(hWnd );

	HWND hCtrl = GetDlgItem(hWnd, IDC_ASPECT);
	if (hCtrl)
		Button_SetCheck(hCtrl, g_bAutoAspect[GetSelectedScreen(hWnd)] );

	hCtrl = GetDlgItem(hWnd, IDC_SNAPSIZE);
	if (hCtrl)
		Button_SetCheck(hCtrl, g_bAutoSnapSize );

	/* Bios select list */
	hCtrl = GetDlgItem(hWnd, IDC_BIOS);
	if (hCtrl)
	{
		const char* cBuffer;
		for( uint8_t i = 0; i < ComboBox_GetCount( hCtrl ); i++ )
		{
			cBuffer = (const char*)ComboBox_GetItemData( hCtrl, i );
			if (strcmp(cBuffer, pCurrentOpts.value(OPTION_BIOS) ) == 0)
			{
				res = ComboBox_SetCurSel(hCtrl, i);
				break;
			}

		}
	}


	hCtrl = GetDlgItem(hWnd, IDC_ASPECT);
	if (hCtrl)
	{
		char aspect_option[32];
		snprintf(aspect_option, ARRAY_LENGTH(aspect_option), "aspect%d", GetSelectedScreen(hWnd));
		if( strcmp(o.value(aspect_option), "auto") == 0)
		{
			Button_SetCheck(hCtrl, true);
			g_bAutoAspect[GetSelectedScreen(hWnd)] = true;
		}
		else
		{
			Button_SetCheck(hCtrl, false);
			g_bAutoAspect[GetSelectedScreen(hWnd)] = false;
		}
	}

	/* aspect ratio */
	hCtrl  = GetDlgItem(hWnd, IDC_ASPECTRATION);
	hCtrl2 = GetDlgItem(hWnd, IDC_ASPECTRATIOD);
	if (hCtrl && hCtrl2)
	{
		char aspect_option[32];
		snprintf(aspect_option, ARRAY_LENGTH(aspect_option), "aspect%d", GetSelectedScreen(hWnd));

		n = 0;
		d = 0;
		if (o.value(aspect_option))
		{
			if (sscanf(o.value(aspect_option), "%d:%d", &n, &d) == 2 && n != 0 && d != 0)
			{
				_stprintf(buf, TEXT("%d"), n);
				Edit_SetText(hCtrl, buf);
				_stprintf(buf, TEXT("%d"), d);
				Edit_SetText(hCtrl2, buf);
			}
			else
			{
				Edit_SetText(hCtrl,  TEXT("4"));
				Edit_SetText(hCtrl2, TEXT("3"));
			}
		}
		else
		{
			Edit_SetText(hCtrl,  TEXT("4"));
			Edit_SetText(hCtrl2, TEXT("3"));
		}
	}

	hCtrl = GetDlgItem(hWnd, IDC_EFFECT);
	if (hCtrl) {
		const char* effect = o.value(OPTION_EFFECT);
		if (effect == NULL) {
			effect = "none";
			o.set_value(OPTION_EFFECT, effect, OPTION_PRIORITY_CMDLINE);
		}
		win_set_window_text_utf8(hCtrl, effect);
	}

	hCtrl = GetDlgItem(hWnd, IDC_SNAPSIZE);
	if (hCtrl)
	{
		if( strcmp(o.value(OPTION_SNAPSIZE), "auto") == 0)
		{
			Button_SetCheck(hCtrl, true);
			g_bAutoSnapSize = true;
		}
		else
		{
			Button_SetCheck(hCtrl, false);
			g_bAutoSnapSize = false;
		}
	}
#if 0
	for (int i = 0; i < 5; i++)
	{
		UpdateMameShader(hWnd, i, opts);
		UpdateScreenShader(hWnd, i, opts);
	}

	hCtrl = GetDlgItem(hWnd, IDC_CHEATFILE);

	if (hCtrl)
	{
		const char* cheatfile = opts.value(OPTION_CHEATPATH);

		if (strcmp(cheatfile, "cheat") == 0)
			winui_set_window_text_utf8(hCtrl, "Default");
		else
		{
			char *cheatname = strrchr(cheatfile, '\\');

			if (cheatname != NULL)
			{
				strcpy(buffer, cheatname + 1);
				winui_set_window_text_utf8(hCtrl, buffer);
			}
			else
				winui_set_window_text_utf8(hCtrl, cheatfile);
		}
	}
#endif
	hCtrl = GetDlgItem(hWnd, IDC_JOYSTICKMAP);

	if (hCtrl)
	{
		const char* joymap = o.value(OPTION_JOYSTICK_MAP);

		win_set_window_text_utf8(hCtrl, joymap);
	}

	hCtrl = GetDlgItem(hWnd, IDC_LUASCRIPT);

	if (hCtrl)
	{
		const char* script = o.value(OPTION_AUTOBOOT_SCRIPT);

		if (strcmp(script, "") == 0)
			win_set_window_text_utf8(hCtrl, "None");
		else
		{
			char buffer[260];
			wchar_t *t_filename = ui_wstring_from_utf8(script);
			wchar_t *tempname = PathFindFileName(t_filename);
			PathRemoveExtension(tempname);
			char *optname = ui_utf8_from_wstring(tempname);
			strcpy(buffer, optname);
			free(t_filename);
			free(optname);
			win_set_window_text_utf8(hCtrl, buffer);
		}
	}

	hCtrl = GetDlgItem(hWnd, IDC_PLUGIN);

	if (hCtrl)
	{
		string t1 = GetPlugins();
		const char* plugin = t1.c_str();

		if (strlen(plugin) == 0)
		//if (strcmp(plugin, "") == 0)
			win_set_window_text_utf8(hCtrl, "None");
		else
			win_set_window_text_utf8(hCtrl, plugin);
	}
#if 0
	hCtrl = GetDlgItem(hWnd, IDC_BGFX_CHAINS);

	if (hCtrl)
	{
		const char* chains = o.value(OSDOPTION_BGFX_SCREEN_CHAINS);

		if (strcmp(chains, "default") == 0)
			winui_set_window_text_utf8(hCtrl, "Default");
		else
			winui_set_window_text_utf8(hCtrl, chains);
	}
#endif
	/* snapshot size */
	hCtrl  = GetDlgItem(hWnd, IDC_SNAPSIZEWIDTH);
	hCtrl2 = GetDlgItem(hWnd, IDC_SNAPSIZEHEIGHT);
	if (hCtrl && hCtrl2)
	{
		n = 0;
		d = 0;
		if (o.value(OPTION_SNAPSIZE))
		{
			if (sscanf(o.value(OPTION_SNAPSIZE), "%dx%d", &width, &height) == 2 && width && height)
			{
				_stprintf(buf, TEXT("%d"), width);
				Edit_SetText(hCtrl, buf);
				_stprintf(buf, TEXT("%d"), height);
				Edit_SetText(hCtrl2, buf);
			}
			else
			{
				Edit_SetText(hCtrl,  TEXT("640"));
				Edit_SetText(hCtrl2, TEXT("480"));
			}
		}
		else
		{
			Edit_SetText(hCtrl,  TEXT("640"));
			Edit_SetText(hCtrl2, TEXT("480"));
		}
	}
	res++;
}

/* Adjust controls - tune them to the currently selected game */
static void SetPropEnabledControls(HWND hWnd)
{
	bool useart = true;
	BOOL joystick_attached = false;
	bool in_window = false;
	int nIndex = g_nGame;

	// auto is a reserved word
	bool autov = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "auto")==0);
	bool d3d = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "d3d")==0) | autov;
#ifdef UI_DIRECTDRAW
	bool ddraw = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "ddraw")==0) | autov;
#else
	bool ddraw = false;
#endif

	in_window = pCurrentOpts.bool_value(OSDOPTION_WINDOW);
	Button_SetCheck(GetDlgItem(hWnd, IDC_ASPECT), g_bAutoAspect[GetSelectedScreen(hWnd)] );

	EnableWindow(GetDlgItem(hWnd, IDC_WAITVSYNC), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_TRIPLE_BUFFER), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_PRESCALE), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_PRESCALEDISP), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_PRESCALETEXT), d3d|ddraw);
#ifdef UI_DIRECTDRAW
	EnableWindow(GetDlgItem(hWnd, IDC_HWSTRETCH), ddraw && DirectDraw_HasHWStretch());
#else
	EnableWindow(GetDlgItem(hWnd, IDC_HWSTRETCH), false);
#endif
	EnableWindow(GetDlgItem(hWnd, IDC_SWITCHRES), true);
	EnableWindow(GetDlgItem(hWnd, IDC_SYNCREFRESH), true);
	EnableWindow(GetDlgItem(hWnd, IDC_REFRESH), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_REFRESHTEXT), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSGAMMA), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSGAMMATEXT), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSGAMMADISP), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSBRIGHTNESS), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSBRIGHTNESSTEXT), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSBRIGHTNESSDISP), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSCONTRAST), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSCONTRASTTEXT), !in_window);
	EnableWindow(GetDlgItem(hWnd, IDC_FSCONTRASTDISP), !in_window);

	EnableWindow(GetDlgItem(hWnd, IDC_ASPECTRATIOTEXT), !g_bAutoAspect[GetSelectedScreen(hWnd)]);
	EnableWindow(GetDlgItem(hWnd, IDC_ASPECTRATION), !g_bAutoAspect[GetSelectedScreen(hWnd)]);
	EnableWindow(GetDlgItem(hWnd, IDC_ASPECTRATIOD), !g_bAutoAspect[GetSelectedScreen(hWnd)]);

	EnableWindow(GetDlgItem(hWnd, IDC_SNAPSIZETEXT), !g_bAutoSnapSize);
	EnableWindow(GetDlgItem(hWnd, IDC_SNAPSIZEHEIGHT), !g_bAutoSnapSize);
	EnableWindow(GetDlgItem(hWnd, IDC_SNAPSIZEWIDTH), !g_bAutoSnapSize);
	EnableWindow(GetDlgItem(hWnd, IDC_SNAPSIZEX), !g_bAutoSnapSize);

	EnableWindow(GetDlgItem(hWnd, IDC_D3D_FILTER),d3d);
#ifdef D3DVERSION
	EnableWindow(GetDlgItem(hWnd, IDC_D3D_VERSION),(NUMD3DVERSIONS>1) & d3d);
#endif

	//Switchres and D3D or ddraw enable the per screen parameters
	EnableWindow(GetDlgItem(hWnd, IDC_NUMSCREENS), ddraw | d3d);
	EnableWindow(GetDlgItem(hWnd, IDC_NUMSCREENSDISP), ddraw | d3d);
	EnableWindow(GetDlgItem(hWnd, IDC_SCREENSELECT), ddraw | d3d);
	EnableWindow(GetDlgItem(hWnd, IDC_SCREENSELECTTEXT), ddraw | d3d);

	EnableWindow(GetDlgItem(hWnd, IDC_ARTWORK_CROP), useart);
	EnableWindow(GetDlgItem(hWnd, IDC_BACKDROPS), useart);
	EnableWindow(GetDlgItem(hWnd, IDC_BEZELS), useart);
	EnableWindow(GetDlgItem(hWnd, IDC_OVERLAYS), useart);
	EnableWindow(GetDlgItem(hWnd, IDC_CPANELS), useart);
	EnableWindow(GetDlgItem(hWnd, IDC_MARQUEES), useart);
	EnableWindow(GetDlgItem(hWnd, IDC_ARTMISCTEXT), useart);

	/* Joystick options */
	joystick_attached = DIJoystick.Available();

	Button_Enable(GetDlgItem(hWnd,IDC_JOYSTICK), joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JDZTEXT),  joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JDZDISP),  joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JDZ),      joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JSATTEXT), joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JSATDISP), joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JSAT),     joystick_attached);
	/* Trackball / Mouse options */
	if (nIndex <= -1 || DriverUsesTrackball(nIndex) || DriverUsesLightGun(nIndex))
		Button_Enable(GetDlgItem(hWnd,IDC_USE_MOUSE),true);
	else
		Button_Enable(GetDlgItem(hWnd,IDC_USE_MOUSE),false);

	if (!in_window && (nIndex <= -1 || DriverUsesLightGun(nIndex)))
	{
		// on WinXP the Lightgun and Dual Lightgun switches are no longer supported use mouse instead
		OSVERSIONINFOEX osvi;
		BOOL bOsVersionInfoEx;
		// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
		// If that fails, try using the OSVERSIONINFO structure.

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
			bOsVersionInfoEx = GetVersionEx ( (OSVERSIONINFO *) &osvi);
		}

//		if( bOsVersionInfoEx && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osvi.dwMajorVersion >= 5) )
//		{
			BOOL use_lightgun;
			//XP and above...
			Button_Enable(GetDlgItem(hWnd,IDC_LIGHTGUN),false);
			use_lightgun = Button_GetCheck(GetDlgItem(hWnd,IDC_USE_MOUSE));
			Button_Enable(GetDlgItem(hWnd,IDC_DUAL_LIGHTGUN),false);
			Button_Enable(GetDlgItem(hWnd,IDC_RELOAD),use_lightgun);
//		}
//		else
//		{
//			BOOL use_lightgun;
			// Older than XP
//			Button_Enable(GetDlgItem(hWnd,IDC_LIGHTGUN),true);
//			use_lightgun = Button_GetCheck(GetDlgItem(hWnd,IDC_LIGHTGUN));
//			Button_Enable(GetDlgItem(hWnd,IDC_DUAL_LIGHTGUN),use_lightgun);
//			Button_Enable(GetDlgItem(hWnd,IDC_RELOAD),use_lightgun);
//		}
	}
	else
	{
		Button_Enable(GetDlgItem(hWnd,IDC_LIGHTGUN),false);
		Button_Enable(GetDlgItem(hWnd,IDC_DUAL_LIGHTGUN),false);
		Button_Enable(GetDlgItem(hWnd,IDC_RELOAD),false);
	}


	/* Sound options */
	bool sound = (core_stricmp(pCurrentOpts.value(OSDOPTION_SOUND), "none")!=0);
	ComboBox_Enable(GetDlgItem(hWnd, IDC_SAMPLERATE), sound);
	EnableWindow(GetDlgItem(hWnd,IDC_VOLUME),sound);
	EnableWindow(GetDlgItem(hWnd,IDC_RATETEXT),sound);
	EnableWindow(GetDlgItem(hWnd,IDC_VOLUMEDISP),sound);
	EnableWindow(GetDlgItem(hWnd,IDC_VOLUMETEXT),sound);
	EnableWindow(GetDlgItem(hWnd,IDC_AUDIO_LATENCY),sound);
	EnableWindow(GetDlgItem(hWnd,IDC_AUDIO_LATENCY_DISP),sound);
	EnableWindow(GetDlgItem(hWnd,IDC_AUDIO_LATENCY_TEXT),sound);
	SetSamplesEnabled(hWnd, nIndex, sound);

	if (Button_GetCheck(GetDlgItem(hWnd, IDC_AUTOFRAMESKIP)))
		EnableWindow(GetDlgItem(hWnd, IDC_FRAMESKIP), false);
	else
		EnableWindow(GetDlgItem(hWnd, IDC_FRAMESKIP), 1);

	if (nIndex <= -1 || DriverHasOptionalBIOS(nIndex))
		EnableWindow(GetDlgItem(hWnd,IDC_BIOS),true);
	else
		EnableWindow(GetDlgItem(hWnd,IDC_BIOS),false);

}

//============================================================
//  CONTROL HELPER FUNCTIONS FOR DATA EXCHANGE
//============================================================

static BOOL RotateReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int selected_index = ComboBox_GetCurSel(control);
	int original_selection = 0;

	// Figure out what the original selection value is
	if (opts->bool_value(OPTION_ROR) && !opts->bool_value(OPTION_ROL))
		original_selection = 1;
	else if (!opts->bool_value(OPTION_ROR) && opts->bool_value(OPTION_ROL))
		original_selection = 2;
	else if (!opts->bool_value(OPTION_ROTATE))
		original_selection = 3;
	else if (opts->bool_value(OPTION_AUTOROR))
		original_selection = 4;
	else if (opts->bool_value(OPTION_AUTOROL))
		original_selection = 5;

	// Any work to do?  If so, make the changes and return true.
	if (selected_index != original_selection)
	{
		// Set the options based on the new selection.
		opts->set_value(OPTION_ROR, selected_index == 1, OPTION_PRIORITY_CMDLINE);
		opts->set_value(OPTION_ROL, selected_index == 2, OPTION_PRIORITY_CMDLINE);
		opts->set_value(OPTION_ROTATE, selected_index != 3, OPTION_PRIORITY_CMDLINE);
		opts->set_value(OPTION_AUTOROR, selected_index == 4, OPTION_PRIORITY_CMDLINE);
		opts->set_value(OPTION_AUTOROL, selected_index == 5, OPTION_PRIORITY_CMDLINE);
		return true;
	}

	// No changes
	return false;
}



static BOOL RotatePopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int res, selected_index = 0;
	if (opts->bool_value(OPTION_ROR) && !opts->bool_value(OPTION_ROL))
		selected_index = 1;
	else if (!opts->bool_value(OPTION_ROR) && opts->bool_value(OPTION_ROL))
		selected_index = 2;
	else if (!opts->bool_value(OPTION_ROTATE))
		selected_index = 3;
	else if (opts->bool_value(OPTION_AUTOROR))
		selected_index = 4;
	else if (opts->bool_value(OPTION_AUTOROL))
		selected_index = 5;

	res = ComboBox_SetCurSel(control, selected_index);
	res++;
	return false;
}



static BOOL ScreenReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	char screen_option_name[32];
	int selected_screen = GetSelectedScreen(dialog);
	int screen_option_index = ComboBox_GetCurSel(control);
	TCHAR *screen_option_value = (TCHAR*) ComboBox_GetItemData(control, screen_option_index);
	snprintf(screen_option_name, ARRAY_LENGTH(screen_option_name), "screen%d", selected_screen);
	char *op_val = ui_utf8_from_wstring(screen_option_value);
	opts->set_value(screen_option_name, op_val, OPTION_PRIORITY_CMDLINE);
	free(op_val);
	return false;
}



static BOOL ScreenPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	//int iMonitors;
	DISPLAY_DEVICE dd;
	int res, i = 0;
	int nSelection = 0;
	TCHAR* t_option = 0;

	/* Remove all items in the list. */
	res = ComboBox_ResetContent(control);
	res = ComboBox_InsertString(control, 0, TEXT("Auto"));
	res = ComboBox_SetItemData(control, 0, (void*)ui_wstring_from_utf8("auto"));

	//Dynamically populate it, by enumerating the Monitors
	//iMonitors = GetSystemMetrics(SM_CMONITORS); // this gets the count of monitors attached
	ZeroMemory(&dd, sizeof(dd));
	dd.cb = sizeof(dd);
	for(i=0; EnumDisplayDevices(NULL, i, &dd, 0); i++)
	{
		if( !(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) )
		{
			char screen_option[32];

			//we have to add 1 to account for the "auto" entry
			res = ComboBox_InsertString(control, i+1, win_tstring_strdup(dd.DeviceName));
			res = ComboBox_SetItemData(control, i+1, (void*)win_tstring_strdup(dd.DeviceName));

			snprintf(screen_option, ARRAY_LENGTH(screen_option), "screen%d", GetSelectedScreen(dialog));
			t_option = ui_wstring_from_utf8(opts->value(screen_option));
			if( !t_option )
				return false;
			if (_tcscmp(t_option, dd.DeviceName) == 0)
				nSelection = i+1;
			free(t_option);
		}
	}
	res = ComboBox_SetCurSel(control, nSelection);
	res++;
	return false;
}



static void ViewSetOptionName(datamap *map, HWND dialog, HWND control, char *buffer, size_t buffer_size)
{
	snprintf(buffer, buffer_size, "view%d", GetSelectedScreen(dialog));
}

static BOOL ViewPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int selected_index = 0;
	char view_option[32];

	// determine the view option value
	snprintf(view_option, ARRAY_LENGTH(view_option), "view%d", GetSelectedScreen(dialog));
	const char *view = opts->value(view_option);

	int res = ComboBox_ResetContent(control);
	for (int i = 0; i < NUMVIEW; i++)
	{
		res = ComboBox_InsertString(control, i, g_ComboBoxView[i].m_pText);
		res = ComboBox_SetItemData(control, i, g_ComboBoxView[i].m_pData);

		if (strcmp(view, g_ComboBoxView[i].m_pData)==0)
			selected_index = i;
	}
	res = ComboBox_SetCurSel(control, selected_index);
	res++;
	return false;
}

static BOOL SnapViewPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int selected_index = 0;

	// determine the snapview option value
	const char *snapview = opts->value(OPTION_SNAPVIEW);

	int res = ComboBox_ResetContent(control);
	for (int i = 0; i < NUMSNAPVIEW; i++)
	{
		res = ComboBox_InsertString(control, i, g_ComboBoxSnapView[i].m_pText);
		res = ComboBox_SetItemData(control, i, g_ComboBoxSnapView[i].m_pData);

		if (strcmp(snapview, g_ComboBoxSnapView[i].m_pData)==0)
			selected_index = i;
	}
	res = ComboBox_SetCurSel(control, selected_index);
	res++;
	return false;
}

static BOOL DefaultInputReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int input_option_index = ComboBox_GetCurSel(control);
	TCHAR *input_option_value = (TCHAR*) ComboBox_GetItemData(control, input_option_index);
	char *op_val = ui_utf8_from_wstring(input_option_value);
	opts->set_value(OPTION_CTRLR, input_option_index ? op_val : "", OPTION_PRIORITY_CMDLINE);
	free(op_val);
	return false;
}

wchar_t *win_wstring_from_utf8(const char *utf8string)
{
	// convert MAME string (UTF-8) to UTF-16
	int char_count = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, nullptr, 0);
	wchar_t *result = (wchar_t *)malloc(char_count * sizeof(*result));

	if (result != nullptr)
		MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, result, char_count);

	return result;
}

char *win_utf8_from_wstring(const wchar_t *wstring)
{
	// convert UTF-16 to MAME string (UTF-8)
	int char_count = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, nullptr, 0, nullptr, nullptr);
	char *result = (char *)malloc(char_count * sizeof(*result));

	if (result != nullptr)
		WideCharToMultiByte(CP_UTF8, 0, wstring, -1, result, char_count, nullptr, nullptr);

	return result;
}

HANDLE winui_find_first_file_utf8(const char* filename, WIN32_FIND_DATA *findfiledata)
{
	wchar_t *t_filename = win_wstring_from_utf8(filename);

	if (!t_filename)
		return NULL;

	HANDLE result = FindFirstFile(t_filename, findfiledata);
	free(t_filename);
	return result;
}

static BOOL DefaultInputPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	WIN32_FIND_DATA FindFileData;
	char path[MAX_PATH];
	int selected = 0;
	int index = 0;

	// determine the ctrlr option
	const char *ctrlr_option = opts->value(OPTION_CTRLR);

	// reset the controllers dropdown
	(void)ComboBox_ResetContent(control);
	(void)ComboBox_InsertString(control, index, TEXT("Default"));
	(void)ComboBox_SetItemData(control, index, "");
	index++;
	snprintf(path, WINUI_ARRAY_LENGTH(path), "%s\\*.*", GetCtrlrDir().c_str());
	HANDLE hFind = winui_find_first_file_utf8(path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile (hFind, &FindFileData) != 0)
		{
			// copy the filename
			const char *root = win_utf8_from_wstring(FindFileData.cFileName);
			// find the extension
			char *ext = strrchr(root, '.');

			if (ext)
			{
				// check if it's a cfg file
				if (strcmp (ext, ".cfg") == 0)
				{
					// and strip off the extension
					*ext = 0;

					// set the option?
					if (!strcmp(root, ctrlr_option))
						selected = index;

					// add it as an option
					wchar_t *t_root = win_wstring_from_utf8(root);
					(void)ComboBox_InsertString(control, index, t_root);
					(void)ComboBox_SetItemData(control, index, root);
					free(t_root);
					root = NULL;
					index++;
				}
			}
		}

		FindClose (hFind);
	}

	(void)ComboBox_SetCurSel(control, selected);

	return false;
}



static void ResolutionSetOptionName(datamap *map, HWND dialog, HWND control, char *buffer, size_t buffer_size)
{
	snprintf(buffer, buffer_size, "resolution%d", GetSelectedScreen(dialog));
}


static BOOL ResolutionReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	HWND refresh_control = GetDlgItem(dialog, IDC_REFRESH);
	HWND sizes_control = GetDlgItem(dialog, IDC_SIZES);
	int res = 0, width = 0, height = 0;
	char option_value[256];

	if (refresh_control && sizes_control)
	{
		TCHAR buffer[256];
		res = ComboBox_GetText(sizes_control, buffer, ARRAY_LENGTH(buffer) - 1);
		if (_stscanf(buffer, TEXT("%d x %d"), &width, &height) == 2)
		{
			int refresh_index = ComboBox_GetCurSel(refresh_control);
			int refresh_value = ComboBox_GetItemData(refresh_control, refresh_index);
			snprintf(option_value, ARRAY_LENGTH(option_value), "%dx%d@%d", width, height, refresh_value);
		}
		else
			snprintf(option_value, ARRAY_LENGTH(option_value), "auto");

		opts->set_value(option_name, option_value, OPTION_PRIORITY_CMDLINE);
	}
	res++;
	return false;
}



static BOOL ResolutionPopulateControl(datamap *map, HWND dialog, HWND control_, windows_options *opts, const char *option_name)
{
	HWND sizes_control = GetDlgItem(dialog, IDC_SIZES);
	HWND refresh_control = GetDlgItem(dialog, IDC_REFRESH);
	int width, height, refresh;
	const char *option_value;
	int sizes_index = 0;
	int refresh_index = 0;
	int sizes_selection = 0;
	int refresh_selection = 0;
	char screen_option[32];
	const char *screen;
	LPTSTR t_screen;
	TCHAR buf[16];
	int res = 0, i;
	DEVMODE devmode;

	if (sizes_control && refresh_control)
	{
		// determine the resolution
		option_value = opts->value(option_name);
		if (sscanf(option_value, "%dx%d@%d", &width, &height, &refresh) != 3)
		{
			width = 0;
			height = 0;
			refresh = 0;
		}

		// reset sizes control
		res = ComboBox_ResetContent(sizes_control);
		res = ComboBox_InsertString(sizes_control, sizes_index, TEXT("Auto"));
		res = ComboBox_SetItemData(sizes_control, sizes_index, 0);
		sizes_index++;

		// reset refresh control
		res = ComboBox_ResetContent(refresh_control);
		res = ComboBox_InsertString(refresh_control, refresh_index, TEXT("Auto"));
		res = ComboBox_SetItemData(refresh_control, refresh_index, 0);
		refresh_index++;

		// determine which screen we're using
		snprintf(screen_option, ARRAY_LENGTH(screen_option), "screen%d", GetSelectedScreen(dialog));
		screen = opts->value(screen_option);
		t_screen = ui_wstring_from_utf8(screen);

		// retrieve screen information
		devmode.dmSize = sizeof(devmode);
		for (i = 0; EnumDisplaySettings(t_screen, i, &devmode); i++)
		{
			if ((devmode.dmBitsPerPel == 32 ) // Only 32 bit depth supported by core
				&&(devmode.dmDisplayFrequency == refresh || refresh == 0))
			{
				_sntprintf(buf, ARRAY_LENGTH(buf), TEXT("%li x %li"),
					devmode.dmPelsWidth, devmode.dmPelsHeight);

				if (ComboBox_FindString(sizes_control, 0, buf) == CB_ERR)
				{
					res = ComboBox_InsertString(sizes_control, sizes_index, buf);

					if ((width == devmode.dmPelsWidth) && (height == devmode.dmPelsHeight))
						sizes_selection = sizes_index;
					sizes_index++;

				}
			}
			if (devmode.dmDisplayFrequency >= 10 )
			{
				// I have some devmode "vga" which specifes 1 Hz, which is probably bogus, so we filter it out

				_sntprintf(buf, ARRAY_LENGTH(buf), TEXT("%li Hz"), devmode.dmDisplayFrequency);

				if (ComboBox_FindString(refresh_control, 0, buf) == CB_ERR)
				{
					res = ComboBox_InsertString(refresh_control, refresh_index, buf);
					res = ComboBox_SetItemData(refresh_control, refresh_index, devmode.dmDisplayFrequency);

					if (refresh == devmode.dmDisplayFrequency)
						refresh_selection = refresh_index;

					refresh_index++;
				}
			}
		}
		free(t_screen);

		res = ComboBox_SetCurSel(sizes_control, sizes_selection);
		res = ComboBox_SetCurSel(refresh_control, refresh_selection);
	}
	res++;
	return false;
}


/************************************************************
 * DataMap initializers
 ************************************************************/

/* Initialize local helper variables */
static void ResetDataMap(HWND hWnd)
{
	char screen_option[32];

	snprintf(screen_option, ARRAY_LENGTH(screen_option), "screen%d", GetSelectedScreen(hWnd));

	if (pCurrentOpts.value(screen_option) == NULL
		|| (core_stricmp(pCurrentOpts.value(screen_option), "") == 0 )
		|| (core_stricmp(pCurrentOpts.value(screen_option), "auto") == 0 ) )
	{
		pCurrentOpts.set_value(screen_option, "auto", OPTION_PRIORITY_CMDLINE);
	}
}


/* Build the control mapping by adding all needed information to the DataMap */
static void BuildDataMap(void)
{
	properties_datamap = datamap_create();

	// core state options
	datamap_add(properties_datamap, IDC_ENABLE_AUTOSAVE,		DM_BOOL,	OPTION_AUTOSAVE);
	datamap_add(properties_datamap, IDC_SNAPVIEW,				DM_STRING,	OPTION_SNAPVIEW);
	datamap_add(properties_datamap, IDC_SNAPSIZEWIDTH,			DM_STRING,	NULL);
	datamap_add(properties_datamap, IDC_SNAPSIZEHEIGHT,			DM_STRING,	NULL);

	// core performance options
	datamap_add(properties_datamap, IDC_AUTOFRAMESKIP,			DM_BOOL,	OPTION_AUTOFRAMESKIP);
	datamap_add(properties_datamap, IDC_FRAMESKIP,				DM_INT,		OPTION_FRAMESKIP);
	datamap_add(properties_datamap, IDC_SECONDSTORUN,			DM_INT,		OPTION_SECONDS_TO_RUN);
	datamap_add(properties_datamap, IDC_SECONDSTORUNDISP,		DM_INT,		OPTION_SECONDS_TO_RUN);
	datamap_add(properties_datamap, IDC_THROTTLE,				DM_BOOL,	OPTION_THROTTLE);
	datamap_add(properties_datamap, IDC_SLEEP,					DM_BOOL,	OPTION_SLEEP);
	datamap_add(properties_datamap, IDC_SPEED,				    DM_FLOAT,	OPTION_SPEED);
	datamap_add(properties_datamap, IDC_SPEEDDISP,				DM_FLOAT,	OPTION_SPEED);
	datamap_add(properties_datamap, IDC_REFRESHSPEED,			DM_BOOL,	OPTION_REFRESHSPEED);

	// core retation options
	datamap_add(properties_datamap, IDC_ROTATE,					DM_INT,		NULL);
	// ror, rol, autoror, autorol handled by callback
	datamap_add(properties_datamap, IDC_FLIPX,					DM_BOOL,	OPTION_FLIPX);
	datamap_add(properties_datamap, IDC_FLIPY,					DM_BOOL,	OPTION_FLIPY);

	// core artwork options
	datamap_add(properties_datamap, IDC_ARTWORK_CROP,			DM_BOOL,	OPTION_ARTWORK_CROP);
	datamap_add(properties_datamap, IDC_BACKDROPS,				DM_BOOL,	OPTION_USE_BACKDROPS);
	datamap_add(properties_datamap, IDC_OVERLAYS,				DM_BOOL,	OPTION_USE_OVERLAYS);
	datamap_add(properties_datamap, IDC_BEZELS,					DM_BOOL,	OPTION_USE_BEZELS);
	datamap_add(properties_datamap, IDC_CPANELS,				DM_BOOL,	OPTION_USE_CPANELS);
	datamap_add(properties_datamap, IDC_MARQUEES,				DM_BOOL,	OPTION_USE_MARQUEES);

	// core screen options
	datamap_add(properties_datamap, IDC_BRIGHTCORRECT,			DM_FLOAT,	OPTION_BRIGHTNESS);
	datamap_add(properties_datamap, IDC_BRIGHTCORRECTDISP,		DM_FLOAT,	OPTION_BRIGHTNESS);
	datamap_add(properties_datamap, IDC_CONTRAST,				DM_FLOAT,	OPTION_CONTRAST);
	datamap_add(properties_datamap, IDC_CONTRASTDISP,			DM_FLOAT,	OPTION_CONTRAST);
	datamap_add(properties_datamap, IDC_GAMMA,					DM_FLOAT,	OPTION_GAMMA);
	datamap_add(properties_datamap, IDC_GAMMADISP,				DM_FLOAT,	OPTION_GAMMA);
	datamap_add(properties_datamap, IDC_PAUSEBRIGHT,			DM_FLOAT,	OPTION_PAUSE_BRIGHTNESS);
	datamap_add(properties_datamap, IDC_PAUSEBRIGHTDISP,		DM_FLOAT,	OPTION_PAUSE_BRIGHTNESS);
	datamap_add(properties_datamap, IDC_BURNIN,					DM_BOOL,	OPTION_BURNIN);
	datamap_add(properties_datamap, IDC_SNAPBILINEAR,			DM_BOOL,	OPTION_SNAPBILINEAR);

	// core vector options
	datamap_add(properties_datamap, IDC_BEAM_MIN,				DM_FLOAT,	OPTION_BEAM_WIDTH_MIN);
	datamap_add(properties_datamap, IDC_BEAM_MINDISP,			DM_FLOAT,	OPTION_BEAM_WIDTH_MIN);
	datamap_add(properties_datamap, IDC_BEAM_MAX,				DM_FLOAT,	OPTION_BEAM_WIDTH_MAX);
	datamap_add(properties_datamap, IDC_BEAM_MAXDISP,			DM_FLOAT,	OPTION_BEAM_WIDTH_MAX);
	datamap_add(properties_datamap, IDC_BEAM_INTEN,				DM_FLOAT,	OPTION_BEAM_INTENSITY_WEIGHT);
	datamap_add(properties_datamap, IDC_BEAM_INTENDISP,			DM_FLOAT,	OPTION_BEAM_INTENSITY_WEIGHT);
	datamap_add(properties_datamap, IDC_FLICKER,				DM_FLOAT,	OPTION_FLICKER);
	datamap_add(properties_datamap, IDC_FLICKERDISP,			DM_FLOAT,	OPTION_FLICKER);

	// core sound options
	datamap_add(properties_datamap, IDC_SAMPLERATE,				DM_INT,		OPTION_SAMPLERATE);
	datamap_add(properties_datamap, IDC_SAMPLES,				DM_BOOL,	OPTION_SAMPLES);
	datamap_add(properties_datamap, IDC_SOUND_MODE,				DM_STRING,	OSDOPTION_SOUND);
	datamap_add(properties_datamap, IDC_VOLUME,					DM_INT,		OPTION_VOLUME);
	datamap_add(properties_datamap, IDC_VOLUMEDISP,				DM_INT,		OPTION_VOLUME);

	// core input options
	datamap_add(properties_datamap, IDC_COINLOCKOUT,			DM_BOOL,	OPTION_COIN_LOCKOUT);
	datamap_add(properties_datamap, IDC_DEFAULT_INPUT,			DM_STRING,	OPTION_CTRLR);
	datamap_add(properties_datamap, IDC_USE_MOUSE,				DM_BOOL,	OPTION_MOUSE);
	datamap_add(properties_datamap, IDC_JOYSTICK,				DM_BOOL,	OPTION_JOYSTICK);
	datamap_add(properties_datamap, IDC_LIGHTGUN,				DM_BOOL,	OPTION_LIGHTGUN);
	datamap_add(properties_datamap, IDC_STEADYKEY,				DM_BOOL,	OPTION_STEADYKEY);
	datamap_add(properties_datamap, IDC_MULTIKEYBOARD,			DM_BOOL,	OPTION_MULTIKEYBOARD);
	datamap_add(properties_datamap, IDC_MULTIMOUSE,				DM_BOOL,	OPTION_MULTIMOUSE);
	datamap_add(properties_datamap, IDC_RELOAD,					DM_BOOL,	OPTION_OFFSCREEN_RELOAD);

	datamap_add(properties_datamap, IDC_JDZ,					DM_FLOAT,	OPTION_JOYSTICK_DEADZONE);
	datamap_add(properties_datamap, IDC_JDZDISP,				DM_FLOAT,	OPTION_JOYSTICK_DEADZONE);
	datamap_add(properties_datamap, IDC_JSAT,					DM_FLOAT,	OPTION_JOYSTICK_SATURATION);
	datamap_add(properties_datamap, IDC_JSATDISP,				DM_FLOAT,	OPTION_JOYSTICK_SATURATION);
	datamap_add(properties_datamap, IDC_JOYSTICKMAP,			DM_STRING,	OPTION_JOYSTICK_MAP);

	// core input automatic enable options
	datamap_add(properties_datamap, IDC_PADDLE,					DM_STRING,	OPTION_PADDLE_DEVICE);
	datamap_add(properties_datamap, IDC_ADSTICK,				DM_STRING,	OPTION_ADSTICK_DEVICE);
	datamap_add(properties_datamap, IDC_PEDAL,					DM_STRING,	OPTION_PEDAL_DEVICE);
	datamap_add(properties_datamap, IDC_DIAL,					DM_STRING,	OPTION_DIAL_DEVICE);
	datamap_add(properties_datamap, IDC_TRACKBALL,				DM_STRING,	OPTION_TRACKBALL_DEVICE);
	datamap_add(properties_datamap, IDC_LIGHTGUNDEVICE,			DM_STRING,	OPTION_LIGHTGUN_DEVICE);
	datamap_add(properties_datamap, IDC_POSITIONAL,				DM_STRING,	OPTION_POSITIONAL_DEVICE);
	datamap_add(properties_datamap, IDC_MOUSE,					DM_STRING,	OPTION_MOUSE_DEVICE);

	// core debugging options
	datamap_add(properties_datamap, IDC_LOG,					DM_BOOL,	OPTION_LOG);
	datamap_add(properties_datamap, IDC_DEBUG,					DM_BOOL,	OPTION_DEBUG);
	datamap_add(properties_datamap, IDC_VERBOSE,				DM_BOOL,	OPTION_VERBOSE);
	datamap_add(properties_datamap, IDC_UPDATEINPAUSE,			DM_BOOL,	OPTION_UPDATEINPAUSE);
	datamap_add(properties_datamap, IDC_DEBUGSCRIPT,			DM_STRING,	OPTION_DEBUGSCRIPT);

	// core misc options
	datamap_add(properties_datamap, IDC_BIOS,					DM_STRING,	OPTION_BIOS);
	datamap_add(properties_datamap, IDC_CHEAT,					DM_BOOL,	OPTION_CHEAT);
	datamap_add(properties_datamap, IDC_SKIP_GAME_INFO,			DM_BOOL,	OPTION_SKIP_GAMEINFO);

	datamap_add(properties_datamap, IDC_LANGUAGE,				DM_STRING,	OPTION_LANGUAGE);
	datamap_add(properties_datamap, IDC_LUASCRIPT,				DM_STRING,	OPTION_AUTOBOOT_SCRIPT);
	datamap_add(properties_datamap, IDC_BOOTDELAY,				DM_INT,		OPTION_AUTOBOOT_DELAY);
	datamap_add(properties_datamap, IDC_BOOTDELAYDISP,			DM_INT,		OPTION_AUTOBOOT_DELAY);
	datamap_add(properties_datamap, IDC_PLUGINS,				DM_BOOL,	OPTION_PLUGINS);
	datamap_add(properties_datamap, IDC_PLUGIN,					DM_STRING,	OPTION_PLUGIN);
	datamap_add(properties_datamap, IDC_NVRAM_SAVE,				DM_BOOL,	OPTION_NVRAM_SAVE);
	datamap_add(properties_datamap, IDC_REWIND,					DM_BOOL,	OPTION_REWIND);

	// windows debugging options
	datamap_add(properties_datamap, IDC_OSLOG,					DM_BOOL,	OPTION_OSLOG);

	// windows performance options
	datamap_add(properties_datamap, IDC_HIGH_PRIORITY,			DM_INT,		WINOPTION_PRIORITY);
	datamap_add(properties_datamap, IDC_HIGH_PRIORITYTXT,		DM_INT,		WINOPTION_PRIORITY);

	// windows video options
	datamap_add(properties_datamap, IDC_VIDEO_MODE,				DM_STRING,	OSDOPTION_VIDEO);
	datamap_add(properties_datamap, IDC_NUMSCREENS,				DM_INT,		OSDOPTION_NUMSCREENS);
	datamap_add(properties_datamap, IDC_NUMSCREENSDISP,			DM_INT,		OSDOPTION_NUMSCREENS);
	datamap_add(properties_datamap, IDC_WINDOWED,				DM_BOOL,	OSDOPTION_WINDOW);
	datamap_add(properties_datamap, IDC_MAXIMIZE,				DM_BOOL,	OSDOPTION_MAXIMIZE);
	datamap_add(properties_datamap, IDC_KEEPASPECT,				DM_BOOL,	OPTION_KEEPASPECT);
	datamap_add(properties_datamap, IDC_PRESCALE,				DM_INT,		OSDOPTION_PRESCALE);
	datamap_add(properties_datamap, IDC_PRESCALEDISP,			DM_INT,		OSDOPTION_PRESCALE);
	datamap_add(properties_datamap, IDC_EFFECT,					DM_STRING,	OPTION_EFFECT);
	datamap_add(properties_datamap, IDC_WAITVSYNC,				DM_BOOL,	OSDOPTION_WAITVSYNC);
	datamap_add(properties_datamap, IDC_SYNCREFRESH,			DM_BOOL,	OSDOPTION_SYNCREFRESH);

	// Direct3D specific options
#ifdef D3DVERSION
	datamap_add(properties_datamap, IDC_D3D_VERSION,			DM_INT,		WINOPTION_D3DVERSION);
#endif
	datamap_add(properties_datamap, IDC_D3D_FILTER,				DM_BOOL,	OSDOPTION_FILTER);

	// per window video options
	datamap_add(properties_datamap, IDC_SCREEN,					DM_STRING,	NULL);
	datamap_add(properties_datamap, IDC_SCREENSELECT,			DM_STRING,	NULL);
	datamap_add(properties_datamap, IDC_VIEW,					DM_STRING,	NULL);
	datamap_add(properties_datamap, IDC_ASPECTRATIOD,			DM_STRING,  NULL);
	datamap_add(properties_datamap, IDC_ASPECTRATION,			DM_STRING,  NULL);
	datamap_add(properties_datamap, IDC_REFRESH,				DM_STRING,  NULL);
	datamap_add(properties_datamap, IDC_SIZES,					DM_STRING,  NULL);

	// full screen options
	datamap_add(properties_datamap, IDC_TRIPLE_BUFFER,			DM_BOOL,	WINOPTION_TRIPLEBUFFER);
	datamap_add(properties_datamap, IDC_SWITCHRES,				DM_BOOL,	OSDOPTION_SWITCHRES);
	datamap_add(properties_datamap, IDC_FSBRIGHTNESS,			DM_FLOAT,	WINOPTION_FULLSCREENBRIGHTNESS);
	datamap_add(properties_datamap, IDC_FSBRIGHTNESSDISP,		DM_FLOAT,	WINOPTION_FULLSCREENBRIGHTNESS);
	datamap_add(properties_datamap, IDC_FSCONTRAST,				DM_FLOAT,	WINOPTION_FULLSCREENCONTRAST);
	datamap_add(properties_datamap, IDC_FSCONTRASTDISP,			DM_FLOAT,	WINOPTION_FULLSCREENCONTRAST);
	datamap_add(properties_datamap, IDC_FSGAMMA,				DM_FLOAT,	WINOPTION_FULLSCREENGAMMA);
	datamap_add(properties_datamap, IDC_FSGAMMADISP,			DM_FLOAT,	WINOPTION_FULLSCREENGAMMA);

	// windows sound options
	datamap_add(properties_datamap, IDC_AUDIO_LATENCY,			DM_INT,		OSDOPTION_AUDIO_LATENCY);
	datamap_add(properties_datamap, IDC_AUDIO_LATENCY_DISP,		DM_INT,		OSDOPTION_AUDIO_LATENCY);

	// input device options
	datamap_add(properties_datamap, IDC_DUAL_LIGHTGUN,			DM_BOOL,	WINOPTION_DUAL_LIGHTGUN);

	// show menu
	datamap_add(properties_datamap, IDC_SHOW_MENU,				DM_BOOL,	WINOPTION_MENU);

	// set up callbacks
	datamap_set_callback(properties_datamap, IDC_ROTATE,		DCT_READ_CONTROL,		RotateReadControl);
	datamap_set_callback(properties_datamap, IDC_ROTATE,		DCT_POPULATE_CONTROL,	RotatePopulateControl);
	datamap_set_callback(properties_datamap, IDC_SCREEN,		DCT_READ_CONTROL,		ScreenReadControl);
	datamap_set_callback(properties_datamap, IDC_SCREEN,		DCT_POPULATE_CONTROL,	ScreenPopulateControl);
	datamap_set_callback(properties_datamap, IDC_VIEW,			DCT_POPULATE_CONTROL,	ViewPopulateControl);
	datamap_set_callback(properties_datamap, IDC_REFRESH,		DCT_READ_CONTROL,		ResolutionReadControl);
	datamap_set_callback(properties_datamap, IDC_REFRESH,		DCT_POPULATE_CONTROL,	ResolutionPopulateControl);
	datamap_set_callback(properties_datamap, IDC_SIZES,			DCT_READ_CONTROL,		ResolutionReadControl);
	datamap_set_callback(properties_datamap, IDC_SIZES,			DCT_POPULATE_CONTROL,	ResolutionPopulateControl);
	datamap_set_callback(properties_datamap, IDC_DEFAULT_INPUT,	DCT_READ_CONTROL,		DefaultInputReadControl);
	datamap_set_callback(properties_datamap, IDC_DEFAULT_INPUT,	DCT_POPULATE_CONTROL,	DefaultInputPopulateControl);
	datamap_set_callback(properties_datamap, IDC_SNAPVIEW,		DCT_POPULATE_CONTROL,	SnapViewPopulateControl);

	datamap_set_option_name_callback(properties_datamap, IDC_VIEW,		ViewSetOptionName);
	//missing population of views with per game defined additional views
	datamap_set_option_name_callback(properties_datamap, IDC_REFRESH,	ResolutionSetOptionName);
	datamap_set_option_name_callback(properties_datamap, IDC_SIZES,		ResolutionSetOptionName);


	// formats
	datamap_set_int_format(properties_datamap, IDC_VOLUMEDISP,		"%ddB");
	datamap_set_int_format(properties_datamap, IDC_AUDIO_LATENCY_DISP,	"%d/5");
	datamap_set_float_format(properties_datamap, IDC_BEAM_MINDISP,		"%3.2f");
	datamap_set_float_format(properties_datamap, IDC_BEAM_MAXDISP,		"%3.2f");
	datamap_set_float_format(properties_datamap, IDC_BEAM_INTENDISP,	"%3.2f");
	datamap_set_float_format(properties_datamap, IDC_FLICKERDISP,		"%3.2f");
	datamap_set_float_format(properties_datamap, IDC_GAMMADISP,		"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_BRIGHTCORRECTDISP,	"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_CONTRASTDISP,		"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_PAUSEBRIGHTDISP,	"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_FSGAMMADISP,		"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_FSBRIGHTNESSDISP,	"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_FSCONTRASTDISP,	"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_JDZDISP,		"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_JSATDISP,		"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_SPEEDDISP,		"%03.2f");

	// trackbar ranges - slider-name,start,end,step
	datamap_set_trackbar_range(properties_datamap, IDC_JDZ,         0.00, 1.00,  (float)0.05);
	datamap_set_trackbar_range(properties_datamap, IDC_JSAT,        0.00, 1.00,  (float)0.05);
	datamap_set_trackbar_range(properties_datamap, IDC_SPEED,       0.00, 3.00,  (float)0.01);
	datamap_set_trackbar_range(properties_datamap, IDC_BEAM_MIN,        0.00, 1.00, (float)0.01);
	datamap_set_trackbar_range(properties_datamap, IDC_BEAM_MAX,        1.00, 10.00, (float)0.01);
	datamap_set_trackbar_range(properties_datamap, IDC_BEAM_INTEN,      -10.00, 10.00, (float)0.01);
	datamap_set_trackbar_range(properties_datamap, IDC_FLICKER,          0.00, 1.00, (float)0.01);
	datamap_set_trackbar_range(properties_datamap, IDC_AUDIO_LATENCY, 1,  5, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_VOLUME,      -32,  0, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_SECONDSTORUN, 0,  60, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_NUMSCREENS, 1,  4, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_PRESCALE, 1,  3, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_FSGAMMA, 0.0, 3.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_FSBRIGHTNESS, 0.00,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_FSCONTRAST, 0.0,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_GAMMA, 0.0, 3.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_BRIGHTCORRECT, 0.00,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_CONTRAST, 0.0,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_PAUSEBRIGHT, 0.00,  1.00, (float)0.05);
	datamap_set_trackbar_range(properties_datamap, IDC_BOOTDELAY, 0, 5, 1);

#ifdef MESS
	// MESS specific stuff
	datamap_add(properties_datamap, IDC_DIR_LIST,                    DM_STRING, NULL);
	datamap_add(properties_datamap, IDC_RAM_COMBOBOX,                DM_INT, OPTION_RAMSIZE);

	// set up callbacks
	datamap_set_callback(properties_datamap, IDC_DIR_LIST,           DCT_READ_CONTROL,      DirListReadControl);
	datamap_set_callback(properties_datamap, IDC_DIR_LIST,           DCT_POPULATE_CONTROL,  DirListPopulateControl);
	datamap_set_callback(properties_datamap, IDC_RAM_COMBOBOX,       DCT_POPULATE_CONTROL,  RamPopulateControl);
#endif
}


static void SetSamplesEnabled(HWND hWnd, int nIndex, BOOL bSoundEnabled)
{
	BOOL enabled = false;
	HWND hCtrl = GetDlgItem(hWnd, IDC_SAMPLES);


	if (hCtrl)
	{
		if ( nIndex > -1 )
		{
			machine_config config(driver_list::driver(nIndex),pCurrentOpts);

			for (device_sound_interface &sound : sound_interface_iterator(config.root_device()))
				if (sound.device().type() == SAMPLES)
					enabled = true;
		}
		enabled = enabled && bSoundEnabled;
		EnableWindow(hCtrl, enabled);
	}
}

/* Moved here cause it's called in a few places */
static void InitializeOptions(HWND hDlg)
{
// from FX
//	InitializeSampleRateUI(hDlg);
	InitializeSoundUI(hDlg);
//	InitializeSoundModeUI(hDlg);
	InitializeSkippingUI(hDlg);
	InitializeRotateUI(hDlg);
	InitializeSelectScreenUI(hDlg);
	InitializeBIOSUI(hDlg);
	InitializeControllerMappingUI(hDlg);
	InitializeVideoUI(hDlg);
//	InitializeSnapViewUI(hDlg);
//	InitializeSnapNameUI(hDlg);
	InitializeLanguageUI(hDlg);
	InitializePluginsUI(hDlg);
//	InitializeGLSLFilterUI(hDlg);
#ifdef D3DVERSION
	InitializeD3DVersionUI(hDlg);
#endif
}

/* Moved here because it is called in several places */
static void InitializeMisc(HWND hDlg)
{
	Button_Enable(GetDlgItem(hDlg, IDC_JOYSTICK), DIJoystick.Available());
}

static void OptOnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	if (hwndCtl == GetDlgItem(hwnd, IDC_NUMSCREENS))
		NumScreensSelectionChange(hwnd);
}

/* Handle changes to the Numscreens slider */
static void NumScreensSelectionChange(HWND hwnd)
{
	//Also Update the ScreenSelect Combo with the new number of screens
	UpdateSelectScreenUI(hwnd );
}

/* Handle changes to the Refresh drop down */
static void RefreshSelectionChange(HWND hWnd, HWND hWndCtrl)
{
	int nCurSelection = ComboBox_GetCurSel(hWndCtrl);

	if (nCurSelection != CB_ERR)
	{
		datamap_read_control(properties_datamap, hWnd, pCurrentOpts, IDC_SIZES);
		datamap_populate_control(properties_datamap, hWnd, pCurrentOpts, IDC_SIZES);
	}
}

/* Initialize the sound options */
static void InitializeSoundUI(HWND hwnd)
{
	int res = 0, i;

	HWND hCtrl = GetDlgItem(hwnd, IDC_SOUND_MODE);
	if (hCtrl)
	{
		for (i = 0; i < NUMSOUND; i++)
		{
			res = ComboBox_InsertString(hCtrl, i, g_ComboBoxSound[i].m_pText);
			res = ComboBox_SetItemData( hCtrl, i, g_ComboBoxSound[i].m_pData);
		}
	}

	i = 0;

	hCtrl = GetDlgItem(hwnd, IDC_SAMPLERATE);
	if (hCtrl)
	{
		res = ComboBox_AddString(hCtrl, TEXT("11025"));
		res = ComboBox_SetItemData(hCtrl, i++, 11025);
		res = ComboBox_AddString(hCtrl, TEXT("22050"));
		res = ComboBox_SetItemData(hCtrl, i++, 22050);
		res = ComboBox_AddString(hCtrl, TEXT("44100"));
		res = ComboBox_SetItemData(hCtrl, i++, 44100);
		res = ComboBox_AddString(hCtrl, TEXT("48000"));
		res = ComboBox_SetItemData(hCtrl, i++, 48000);
		res = ComboBox_SetCurSel(hCtrl, 1);
	}
	res++;
}

/* Populate the Frame Skipping drop down */
static void InitializeSkippingUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_FRAMESKIP);
	int res = 0, i = 0;

	if (hCtrl)
	{
		res = ComboBox_AddString(hCtrl, TEXT("Draw every frame"));
		res = ComboBox_SetItemData(hCtrl, i++, 0);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 1 frame"));
		res = ComboBox_SetItemData(hCtrl, i++, 1);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 2 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 2);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 3 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 3);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 4 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 4);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 5 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 5);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 6 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 6);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 7 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 7);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 8 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 8);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 9 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 9);
		res = ComboBox_AddString(hCtrl, TEXT("Skip 10 frames"));
		res = ComboBox_SetItemData(hCtrl, i++, 10);
	}
	res++;
}

/* Populate the Rotate drop down */
static void InitializeRotateUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_ROTATE);
	int res = 0;

	if (hCtrl)
	{
		res = ComboBox_AddString(hCtrl, TEXT("Default"));             // 0
		res = ComboBox_AddString(hCtrl, TEXT("Clockwise"));           // 1
		res = ComboBox_AddString(hCtrl, TEXT("Anti-clockwise"));      // 2
		res = ComboBox_AddString(hCtrl, TEXT("None"));                // 3
		res = ComboBox_AddString(hCtrl, TEXT("Auto clockwise"));      // 4
		res = ComboBox_AddString(hCtrl, TEXT("Auto anti-clockwise")); // 5
	}
	res++;
}

/* Populate the Video Mode drop down */
static void InitializeVideoUI(HWND hwnd)
{
	int res = 0;

	HWND hCtrl = GetDlgItem(hwnd, IDC_VIDEO_MODE);
	if (hCtrl)
	{
		for (int i = 0; i < NUMVIDEO; i++)
		{
			res = ComboBox_InsertString(hCtrl, i, g_ComboBoxVideo[i].m_pText);
			res = ComboBox_SetItemData( hCtrl, i, g_ComboBoxVideo[i].m_pData);
		}
	}
	res++;
}

#ifdef D3DVERSION
/* Populate the D3D Version drop down */
static void InitializeD3DVersionUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_D3D_VERSION);
	int res = 0;
	if (hCtrl)
	{
		for (int i = 0; i < NUMD3DVERSIONS; i++)
		{
			res = ComboBox_InsertString(hCtrl, i, g_ComboBoxD3DVersion[i].m_pText);
			res = ComboBox_SetItemData( hCtrl, i, g_ComboBoxD3DVersion[i].m_pData);
		}
	}
	res++;
}
#endif
static void UpdateSelectScreenUI(HWND hwnd)
{
	int res = 0, i, curSel;
	HWND hCtrl = GetDlgItem(hwnd, IDC_SCREENSELECT);
	if (hCtrl)
	{
		curSel = ComboBox_GetCurSel(hCtrl);
		if ((curSel < 0) || (curSel >= NUMSELECTSCREEN))
			curSel = 0;
		res = ComboBox_ResetContent(hCtrl);
		for (i = 0; i < NUMSELECTSCREEN && i < pCurrentOpts.int_value(OSDOPTION_NUMSCREENS) ; i++)
		{
			res = ComboBox_InsertString(hCtrl, i, g_ComboBoxSelectScreen[i].m_pText);
			res = ComboBox_SetItemData( hCtrl, i, g_ComboBoxSelectScreen[i].m_pData);
		}
		// Smaller Amount of screens was selected, so use 0
		if( i < curSel )
			res = ComboBox_SetCurSel(hCtrl, 0);
		else
			res = ComboBox_SetCurSel(hCtrl, curSel);
	}
	res++;
}

/* Populate the Select Screen drop down */
static void InitializeSelectScreenUI(HWND hwnd)
{
	UpdateSelectScreenUI(hwnd);
}

static void InitializeControllerMappingUI(HWND hwnd)
{
	int res = 0;
	HWND hCtrl  = GetDlgItem(hwnd,IDC_PADDLE);
	HWND hCtrl1 = GetDlgItem(hwnd,IDC_ADSTICK);
	HWND hCtrl2 = GetDlgItem(hwnd,IDC_PEDAL);
	HWND hCtrl3 = GetDlgItem(hwnd,IDC_MOUSE);
	HWND hCtrl4 = GetDlgItem(hwnd,IDC_DIAL);
	HWND hCtrl5 = GetDlgItem(hwnd,IDC_TRACKBALL);
	HWND hCtrl6 = GetDlgItem(hwnd,IDC_LIGHTGUNDEVICE);
	HWND hCtrl7 = GetDlgItem(hwnd,IDC_POSITIONAL);

	for (int i = 0; i < NUMDEVICES; i++)
	{
		if (hCtrl)
		{
			res = ComboBox_InsertString(hCtrl, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl1)
		{
			res = ComboBox_InsertString(hCtrl1, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl1, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl2)
		{
			res = ComboBox_InsertString(hCtrl2, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl2, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl3)
		{
			res = ComboBox_InsertString(hCtrl3, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl3, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl4)
		{
			res = ComboBox_InsertString(hCtrl4, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl4, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl5)
		{
			res = ComboBox_InsertString(hCtrl5, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl5, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl6)
		{
			res = ComboBox_InsertString(hCtrl6, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl6, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl7)
		{
			res = ComboBox_InsertString(hCtrl7, i, g_ComboBoxDevice[i].m_pText);
			res = ComboBox_SetItemData( hCtrl7, i, g_ComboBoxDevice[i].m_pData);
		}
	}
	res++;
}


static void InitializeBIOSUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd,IDC_BIOS);
	int res = 0, i = 0;
	TCHAR* t_s;
	if (hCtrl)
	{
		const game_driver *gamedrv = &driver_list::driver(g_nGame);
		const rom_entry *rom;

		if (g_nGame == GLOBAL_OPTIONS)
		{
			res = ComboBox_InsertString(hCtrl, i, TEXT("None"));
			res = ComboBox_SetItemData( hCtrl, i++, "");
			return;
		}
		if (g_nGame == LOCAL_OPTIONS) //Folder Options: This is the only place that LOCAL_OPTIONS is used, is this code ever executed?
		{
			gamedrv = &driver_list::driver(g_nFolderGame);
			if (DriverHasOptionalBIOS(g_nFolderGame) == false)
			{
				res = ComboBox_InsertString(hCtrl, i, TEXT("None"));
				res = ComboBox_SetItemData( hCtrl, i++, "");
				return;
			}
			res = ComboBox_InsertString(hCtrl, i, TEXT("Default"));
			res = ComboBox_SetItemData( hCtrl, i++, "");

			if (gamedrv->rom)
			{
				auto rom_entries = rom_build_entries(gamedrv->rom);
				for (rom = rom_entries.data(); !ROMENTRY_ISEND(rom); rom++)
				{
					if (ROMENTRY_ISSYSTEM_BIOS(rom))
					{
						const char *name = ROM_GETHASHDATA(rom);
						const char *biosname = ROM_GETNAME(rom);
						t_s = ui_wstring_from_utf8(name);
						if( !t_s )
							return;
						res = ComboBox_InsertString(hCtrl, i, win_tstring_strdup(t_s));
						res = ComboBox_SetItemData( hCtrl, i++, biosname);
						free(t_s);
					}
				}
			}
			return;
		}

		if (DriverHasOptionalBIOS(g_nGame) == false)
		{
			res = ComboBox_InsertString(hCtrl, i, TEXT("None"));
			res = ComboBox_SetItemData( hCtrl, i++, "");
			return;
		}
		res = ComboBox_InsertString(hCtrl, i, TEXT("Default"));
		res = ComboBox_SetItemData( hCtrl, i++, "");

		if (gamedrv->rom)
		{
			auto rom_entries = rom_build_entries(gamedrv->rom);
			for (rom = rom_entries.data(); !ROMENTRY_ISEND(rom); rom++)
			{
				if (ROMENTRY_ISSYSTEM_BIOS(rom))
				{
					const char *name = ROM_GETHASHDATA(rom);
					const char *biosname = ROM_GETNAME(rom);
					t_s = ui_wstring_from_utf8(name);
					if( !t_s )
						return;
					res = ComboBox_InsertString(hCtrl, i, win_tstring_strdup(t_s));
					res = ComboBox_SetItemData( hCtrl, i++, biosname);
					free(t_s);
				}
			}
		}
	}
	res++;
}

static void InitializeLanguageUI(HWND hWnd)
{
	HWND hCtrl = GetDlgItem(hWnd, IDC_LANGUAGE);

	if (hCtrl)
	{
		int count = 0;
		string t1 = GetLangDir();
		const char* t2 = t1.c_str();
		osd::directory::ptr directory = osd::directory::open(t2);

		if (directory == nullptr)
			return;

		for (const osd::directory::entry *entry = directory->read(); entry; entry = directory->read())
		{
			if (entry->type == osd::directory::entry::entry_type::DIR)
			{
				string name = entry->name;
				
				if (!(name == "." || name == ".."))
				{
					char *value = strdup(entry->name);
					wchar_t *text = ui_wstring_from_utf8(entry->name);
					(void)ComboBox_InsertString(hCtrl, count, text);
					(void)ComboBox_SetItemData(hCtrl, count, value);
					count++;
					free(text);
					free(value);
				}
			}
		}

		directory.reset();
	}
}

static void InitializePluginsUI(HWND hWnd)
{
	HWND hCtrl = GetDlgItem(hWnd, IDC_SELECT_PLUGIN);

	if (hCtrl)
	{
		string t1 = GetPluginsDir();
		const char* t2 = t1.c_str();
		osd::directory::ptr directory = osd::directory::open(t2);

		if (directory == nullptr)
			return;

		TCHAR* t_s;
		int count = 0;

		for (const osd::directory::entry *entry = directory->read(); entry; entry = directory->read())
		{
			if (entry->type == osd::directory::entry::entry_type::DIR)
			{
				string name = entry->name;

				if (!(name == "." || name == ".." || name == "json"))
				{
					plugin_names[count] = name;
					const char* label = name.c_str();
					t_s = ui_wstring_from_utf8(label);
					label = 0;
					if( !t_s )
						return;
					if (ComboBox_InsertString(hCtrl, count++, win_tstring_strdup(t_s)) == CB_ERR)
						return;
					free(t_s);
				}
			}
		}
		directory.reset();
	}

	(void)ComboBox_SetCurSel(hCtrl, -1);
	(void)ComboBox_SetCueBannerText(hCtrl, TEXT("Select a plugin"));
}
#if 0
static void InitializeGLSLFilterUI(HWND hWnd)
{
	HWND hCtrl = GetDlgItem(hWnd, IDC_GLSLFILTER);

	if (hCtrl)
	{
		for (int i = 0; i < NUMGLSLFILTER; i++)
		{
			(void)ComboBox_InsertString(hCtrl, i, g_ComboBoxGLSLFilter[i].m_pText);
			(void)ComboBox_SetItemData(hCtrl, i, g_ComboBoxGLSLFilter[i].m_pData);
		}
	}
}
#endif
static BOOL SelectEffect(HWND hWnd)
{
	char filename[MAX_PATH];
	BOOL changed = false;

	*filename = 0;
	if (CommonFileDialog(GetOpenFileName, filename, FILETYPE_EFFECT_FILES))
	{
		//strip Path and extension
		char buff[MAX_PATH];
		int i, j = 0, k = 0, l = 0;
		for(i=0; i<strlen(filename); i++ )
		{
			if( filename[i] == '\\' )
				j = i;
			if( filename[i] == '.' )
				k = i;
		}
		for(i=j+1; i<k; i++)
			buff[l++] = filename[i];

		buff[l] = '\0';

		if (strcmp(buff, pCurrentOpts.value(OPTION_EFFECT))!=0)
		{
			HWND control = GetDlgItem(hWnd, IDC_EFFECT);
			pCurrentOpts.set_value(OPTION_EFFECT, buff, OPTION_PRIORITY_CMDLINE);
			win_set_window_text_utf8(control, buff);
			// datamap_populate_control(properties_datamap, hWnd, pCurrentOpts, IDC_EFFECT);
			changed = true;
		}
	}
	return changed;
}

static BOOL ResetEffect(HWND hWnd)
{
	BOOL changed = false;
	const char *new_value = "none";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_EFFECT))!=0)
	{
		HWND control = GetDlgItem(hWnd, IDC_EFFECT);
		pCurrentOpts.set_value(OPTION_EFFECT, new_value, OPTION_PRIORITY_CMDLINE);
		win_set_window_text_utf8(control, new_value);
		// datamap_populate_control(properties_datamap, hWnd, pCurrentOpts, IDC_EFFECT);
		changed = true;
	}
	return changed;
}

static BOOL SelectJoystickMap(HWND hWnd)
{
	char filename[MAX_PATH];
	BOOL changed = false;

	*filename = 0;
	if (CommonFileDialog(GetOpenFileName, filename, FILETYPE_JOYMAP_FILES))
	{
		if (strcmp(filename, pCurrentOpts.value(OPTION_JOYSTICK_MAP))!=0)
		{
			HWND control = GetDlgItem(hWnd, IDC_JOYSTICKMAP);
			pCurrentOpts.set_value(OPTION_JOYSTICK_MAP, filename, OPTION_PRIORITY_CMDLINE);
			win_set_window_text_utf8(control, filename);
			changed = true;
		}
	}
	return changed;
}

static BOOL ResetJoystickMap(HWND hWnd)
{
	BOOL changed = false;
	const char *new_value = "auto";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_JOYSTICK_MAP))!=0)
	{
		HWND control = GetDlgItem(hWnd, IDC_JOYSTICKMAP);
		pCurrentOpts.set_value(OPTION_JOYSTICK_MAP, new_value, OPTION_PRIORITY_CMDLINE);
		win_set_window_text_utf8(control, new_value);
		changed = true;
	}
	return changed;
}

static bool SelectLUAScript(HWND hWnd)
{
	char filename[MAX_PATH];
	bool changed = false;

	*filename = 0;

	if (CommonFileDialog(GetOpenFileName, filename, FILETYPE_LUASCRIPT_FILES))
	{
		char option[MAX_PATH];
		char script[MAX_PATH];
		wchar_t *t_filename = ui_wstring_from_utf8(filename);
		wchar_t *tempname = PathFindFileName(t_filename);
		char *optvalue = ui_utf8_from_wstring(tempname);
		strcpy(script, optvalue);
		PathRemoveExtension(tempname);
		char *optname = ui_utf8_from_wstring(tempname);
		strcpy(option, optname);
		free(t_filename);
		free(optname);
		free(optvalue);

		if (strcmp(script, pCurrentOpts.value(OPTION_AUTOBOOT_SCRIPT)))
		{
			pCurrentOpts.set_value(OPTION_AUTOBOOT_SCRIPT, script, OPTION_PRIORITY_CMDLINE);
			win_set_window_text_utf8(GetDlgItem(hWnd, IDC_LUASCRIPT), option);
			changed = true;
		}
	}

	return changed;
}

static bool ResetLUAScript(HWND hWnd)
{
	bool changed = false;
	const char *new_value = "";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_AUTOBOOT_SCRIPT)))
	{
		pCurrentOpts.set_value(OPTION_AUTOBOOT_SCRIPT, new_value, OPTION_PRIORITY_CMDLINE);
		win_set_window_text_utf8(GetDlgItem(hWnd, IDC_LUASCRIPT), "None");
		changed = true;
	}

	return changed;
}

static bool SelectPlugins(HWND hWnd)
{
	bool changed = false;
	bool already_enabled = false;
	HWND hcontrol = GetDlgItem(hWnd, IDC_SELECT_PLUGIN);
	if (!hcontrol)
		return changed;

	int index = ComboBox_GetCurSel(hcontrol);
	if (index == CB_ERR)
		return changed;

	const char *new_value = plugin_names[index].c_str();
	string t1 = GetPlugins();
	const char* value = t1.c_str();

	char *token = NULL;
	char buffer[990];  // hold all plugins
	char plugins[24][32]; // number of possible plugins, max length of name
	int num_plugins = 0;

	strcpy(buffer, value);
	token = strtok(buffer, ",");

	if (token == NULL)
	{
		strcpy(plugins[num_plugins], buffer);
	}
	else
	{
		while (token != NULL)
		{
			strcpy(plugins[num_plugins], token);
			num_plugins++;
			token = strtok(NULL, ",");
		}
	}

	if (strcmp(value, "") == 0)
	{
		pCurrentOpts.set_value(OPTION_PLUGIN, new_value, OPTION_PRIORITY_CMDLINE);
		win_set_window_text_utf8(GetDlgItem(hWnd, IDC_PLUGIN), new_value);
		changed = true;
		(void)ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_SELECT_PLUGIN), -1);
		return changed;	
	}

	for (int i = 0; i < num_plugins; i++)
	{
		if (strcmp(new_value, plugins[i]) == 0)
		{
			already_enabled = true;
			break;
		}
	}

	if (!already_enabled)
	{
		char new_option[256];
		snprintf(new_option, WINUI_ARRAY_LENGTH(new_option), "%s,%s", value, new_value);
		pCurrentOpts.set_value(OPTION_PLUGIN, new_option, OPTION_PRIORITY_CMDLINE);
		win_set_window_text_utf8(GetDlgItem(hWnd, IDC_PLUGIN), new_option);
		changed = true;
	}

	(void)ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_SELECT_PLUGIN), -1);
	return changed;
}

static bool ResetPlugins(HWND hWnd)
{
	bool changed = false;
	pCurrentOpts.set_value(OPTION_PLUGIN, "", OPTION_PRIORITY_CMDLINE);
	win_set_window_text_utf8(GetDlgItem(hWnd, IDC_PLUGIN), "None");
	changed = true;
	(void)ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_SELECT_PLUGIN), -1);
	return changed;
}

static BOOL SelectDebugscript(HWND hWnd)
{
	char filename[MAX_PATH];
	BOOL changed = false;

	*filename = 0;
	if (CommonFileDialog(GetOpenFileName, filename, FILETYPE_DEBUGSCRIPT_FILES))
	{
		if (strcmp(filename, pCurrentOpts.value(OPTION_DEBUGSCRIPT))!=0)
		{
			HWND control = GetDlgItem(hWnd, IDC_DEBUGSCRIPT);
			pCurrentOpts.set_value(OPTION_DEBUGSCRIPT, filename, OPTION_PRIORITY_CMDLINE);
			win_set_window_text_utf8(control, filename);
			changed = true;
		}
	}
	return changed;
}

static BOOL ResetDebugscript(HWND hWnd)
{
	BOOL changed = false;
	const char *new_value = "";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_DEBUGSCRIPT))!=0)
	{
		HWND control = GetDlgItem(hWnd, IDC_DEBUGSCRIPT);
		pCurrentOpts.set_value(OPTION_DEBUGSCRIPT, new_value, OPTION_PRIORITY_CMDLINE);
		win_set_window_text_utf8(control, new_value);
		changed = true;
	}
	return changed;
}

void UpdateBackgroundBrush(HWND hwndTab)
{
	// Destroy old brush
	if (hBkBrush)
		DeleteBrush(hBkBrush);

	hBkBrush = NULL;

	// Only do this if the theme is active
	if (SafeIsAppThemed())
	{
		// Get tab control dimensions
		RECT rc;
		GetWindowRect( hwndTab, &rc);

		// Get the tab control DC
		HDC hDC = GetDC(hwndTab);

		// Create a compatible DC
		HDC hDCMem = CreateCompatibleDC(hDC);
		HBITMAP hBmp = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
		HBITMAP hBmpOld = (HBITMAP)(SelectObject(hDCMem, hBmp));

		// Tell the tab control to paint in our DC
		SendMessage(hwndTab, WM_PRINTCLIENT, (WPARAM)(hDCMem), (LPARAM)(PRF_ERASEBKGND | PRF_CLIENT | PRF_NONCLIENT));

		// Create a pattern brush from the bitmap selected in our DC
		hBkBrush = CreatePatternBrush(hBmp);

		// Restore the bitmap
		SelectObject(hDCMem, hBmpOld);

		// Cleanup
		DeleteBitmap(hBmp);
		DeleteDC(hDCMem);
		ReleaseDC(hwndTab, hDC);
	}
}


// from propertiesms.cpp (MESSUI)


//============================================================
#ifdef MESS
//============================================================
//  DATAMAP HANDLERS FOR MESS
//============================================================

static void AppendList(HWND hList, LPCTSTR lpItem, int nItem)
{
	LV_ITEM Item;
	memset(&Item, 0, sizeof(LV_ITEM));
	Item.mask = LVIF_TEXT;
	Item.pszText = (LPTSTR) lpItem;
	Item.iItem = nItem;
	HRESULT res = ListView_InsertItem(hList, &Item);
	res++;
}

static BOOL DirListReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int directory_count;
	LV_ITEM lvi;
	TCHAR buffer[2048];
	int pos = 0;
	BOOL res;

	// determine the directory count; note that one item is the "<    >" entry
	directory_count = ListView_GetItemCount(control);
	if (directory_count > 0)
		directory_count--;

	buffer[0] = '\0';

	for (int i = 0; i < directory_count; i++)
	{
		// append a semicolon, if we're past the first entry
		if (i > 0)
			pos += _sntprintf(&buffer[pos], ARRAY_LENGTH(buffer) - pos, TEXT(";"));

		// retrieve the next entry
		memset(&lvi, '\0', sizeof(lvi));
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.pszText = &buffer[pos];
		lvi.cchTextMax = ARRAY_LENGTH(buffer) - pos;
		res = ListView_GetItem(control, &lvi);

		// advance the position
		pos += _tcslen(&buffer[pos]);
	}

	char* paths = ui_utf8_from_wstring(buffer);
	if ((buffer[1] == 0x3A) || (buffer[0] == 0)) // must be a folder or null
		pCurrentOpts.set_value(OPTION_SWPATH, paths, OPTION_PRIORITY_CMDLINE);

	free (paths);
	res++;
	return true;
}


static BOOL DirListPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int new_pos = 0;

	// access the directory list, and convert to TCHARs
	int driver_index = PropertiesCurrentGame(dialog);
	windows_options o;
	load_options(o, OPTIONS_GAME, driver_index, 0);
	const char* paths = o.value(OPTION_SWPATH);
	TCHAR* t_dir_list = ui_wstring_from_utf8(paths);
	paths = 0;
	if (!t_dir_list)
		return false;

	// delete all items in the list control
	BOOL b_res = ListView_DeleteAllItems(control);

	// add the column
	RECT r;
	GetClientRect(control, &r);
	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(LVCOLUMN));
	lvc.mask = LVCF_WIDTH;
	lvc.cx = r.right - r.left - GetSystemMetrics(SM_CXHSCROLL);
	HRESULT res = ListView_InsertColumn(control, 0, &lvc);

	// add each of the directories
	int pos = 0;
	int current_item = 0;
	while(t_dir_list[pos] != '\0')
	{
		// parse off this item
		TCHAR *s = _tcschr(&t_dir_list[pos], ';');
		if (s)
		{
			*s = '\0';
			new_pos = s - t_dir_list + 1;
		}
		else
			new_pos = pos + _tcslen(&t_dir_list[pos]);

		// append this item
		AppendList(control, &t_dir_list[pos], current_item);

		// advance to next item
		pos = new_pos;
		current_item++;
	}

	// finish up
	AppendList(control, TEXT(DIRLIST_NEWENTRYTEXT), current_item);
	ListView_SetItemState(control, 0, LVIS_SELECTED, LVIS_SELECTED);
	free(t_dir_list);
	res++;
	b_res++;
	return true;
}



static const char *messram_string(char *buffer, UINT32 ram)
{
	const char *suffix;

	if ((ram % (1024*1024)) == 0)
	{
		ram /= 1024*1024;
		suffix = "MB";
	}
	else if ((ram % 1024) == 0)
	{
		ram /= 1024;
		suffix = "KB";
	}
	else
		suffix = "";

	sprintf(buffer, "%u%s", ram, suffix);
	return buffer;
}

//-------------------------------------------------
//  parse_string - convert a ram string to an
//  integer value
//-------------------------------------------------

static uint32_t parse_string(const char *s)
{
	static const struct
	{
		const char *suffix;
		unsigned multiple;
	} s_suffixes[] =
	{
		{ "",       1 },
		{ "k",      1024 },
		{ "kb",     1024 },
		{ "kib",    1024 },
		{ "m",      1024 * 1024 },
		{ "mb",     1024 * 1024 },
		{ "mib",    1024 * 1024 }
	};

	// parse the string
	unsigned ram = 0;
	char suffix[8] = { 0, };
	sscanf(s, "%u%7s", &ram, suffix);

	// perform the lookup
	auto iter = std::find_if(std::begin(s_suffixes), std::end(s_suffixes), [&suffix](const auto &potential_suffix)
	{ return !core_stricmp(suffix, potential_suffix.suffix); } );

	// identify the multiplier (or 0 if not recognized, signalling a parse failure)
	unsigned multiple = iter != std::end(s_suffixes) ? iter->multiple : 0;

	// return the result
	return ram * multiple;
}

static BOOL RamPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int i = 0, current_index = 0;

	// identify the driver
	int driver_index = PropertiesCurrentGame(dialog);
	const game_driver *gamedrv = &driver_list::driver(driver_index);

	// clear out the combo box
	int res = ComboBox_ResetContent(control);

	// allocate the machine config
	machine_config cfg(*gamedrv,*opts);

	// identify how many options that we have
	ram_device_iterator iter(cfg.root_device());
	ram_device *device = iter.first();

	EnableWindow(control, (device != NULL));

	// we can only do something meaningful if there is more than one option
	if (device)
	{
		const ram_device *ramdev = dynamic_cast<const ram_device *>(device);

		// identify the current amount of RAM
		const char *this_ram_string = opts->value(OPTION_RAMSIZE);
		uint32_t current_ram = (this_ram_string) ? parse_string(this_ram_string) : 0;
		uint32_t ram = ramdev->default_size();
		if (current_ram == 0)
			current_ram = ram;

		char ramtext[20];
		messram_string(ramtext, ram);
		TCHAR *t_ramstring = ui_wstring_from_utf8(ramtext);
		if( !t_ramstring )
			return false;

		res = ComboBox_InsertString(control, i, win_tstring_strdup(t_ramstring));
		res = ComboBox_SetItemData(control, i, ram);

		if (!ramdev->extra_options().empty())
		{
			/* try to parse each option */
			for (ram_device::extra_option const &option : ramdev->extra_options())
			{
				// identify this option
				t_ramstring = ui_wstring_from_utf8(option.first.c_str());
				if( t_ramstring )
				{
					i++;
					// add this option to the combo box
					res = ComboBox_InsertString(control, i, win_tstring_strdup(t_ramstring));
					res = ComboBox_SetItemData(control, i, option.second);

					// is this the current option?  record the index if so
					if (option.second == current_ram)
						current_index = i;
				}
			}
		}
		if (t_ramstring)
			free (t_ramstring);
		// set the combo box
		res = ComboBox_SetCurSel(control, current_index);
	}
	res++;
	return true;
}


static void MarkChanged(HWND hDlg)
{
	/* fake a CBN_SELCHANGE event from IDC_SIZES to force it to be changed */
	HWND hCtrl = GetDlgItem(hDlg, IDC_SIZES);
	PostMessage(hDlg, WM_COMMAND, (CBN_SELCHANGE << 16) | IDC_SIZES, (LPARAM) hCtrl);
}


static BOOL SoftwareDirectories_OnInsertBrowse(HWND hDlg, BOOL bBrowse, LPCTSTR lpItem)
{
	TCHAR inbuf[MAX_PATH];
	TCHAR outbuf[MAX_PATH];
	LPTSTR lpIn;
	BOOL res = false;

	g_bModifiedSoftwarePaths = true;

	HWND hList = GetDlgItem(hDlg, IDC_DIR_LIST);
	int nItem = ListView_GetNextItem(hList, -1, LVNI_SELECTED);

	if (nItem == -1)
		return false;

	/* Last item is placeholder for append */
	if (nItem == ListView_GetItemCount(hList) - 1)
		bBrowse = false;

	if (!lpItem)
	{
		if (bBrowse)
		{
			ListView_GetItemText(hList, nItem, 0, inbuf, ARRAY_LENGTH(inbuf));
			lpIn = inbuf;
		}
		else
			lpIn = NULL;

		if (!BrowseForDirectory(hDlg, lpIn, outbuf))
			return false;

		lpItem = outbuf;
	}

	AppendList(hList, lpItem, nItem);
	if (bBrowse)
		res = ListView_DeleteItem(hList, nItem+1);
	MarkChanged(hDlg);
	res++;
	return true;
}



static BOOL SoftwareDirectories_OnDelete(HWND hDlg)
{
	int nSelect = 0;
	HWND hList = GetDlgItem(hDlg, IDC_DIR_LIST);

	g_bModifiedSoftwarePaths = true;

	int nItem = ListView_GetNextItem(hList, -1, LVNI_SELECTED | LVNI_ALL);

	if (nItem == -1)
		return false;

	/* Don't delete "Append" placeholder. */
	if (nItem == ListView_GetItemCount(hList) - 1)
		return false;

	BOOL res = ListView_DeleteItem(hList, nItem);

	int nCount = ListView_GetItemCount(hList);
	if (nCount <= 1)
		return false;

	/* If the last item was removed, select the item above. */
	if (nItem == nCount - 1)
		nSelect = nCount - 2;
	else
		nSelect = nItem;

	ListView_SetItemState(hList, nSelect, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	MarkChanged(hDlg);
	res++;
	return true;
}



static BOOL SoftwareDirectories_OnBeginLabelEdit(HWND hDlg, NMHDR* pNMHDR)
{
	BOOL          bResult = false;
	NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*)pNMHDR;
	LVITEM*       pItem = &pDispInfo->item;
	HWND          hList = GetDlgItem(hDlg, IDC_DIR_LIST);

	/* Last item is placeholder for append */
	if (pItem->iItem == ListView_GetItemCount(hList) - 1)
	{
		HWND hEdit = (HWND) (uintptr_t) SendMessage(hList, LVM_GETEDITCONTROL, 0, 0);
		win_set_window_text_utf8(hEdit, "");
	}

	return bResult;
}



static BOOL SoftwareDirectories_OnEndLabelEdit(HWND hDlg, NMHDR* pNMHDR)
{
	BOOL bResult = false;
	NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*)pNMHDR;
	LVITEM* pItem = &pDispInfo->item;

	if (pItem->pszText)
	{
		struct _stat file_stat;

		/* Don't allow empty entries. */
		if (!_tcscmp(pItem->pszText, TEXT("")))
			return false;

		/* Check validity of edited directory. */
		if ((_tstat(pItem->pszText, &file_stat) == 0) && (file_stat.st_mode & S_IFDIR))
			bResult = true;
		else
		if (win_message_box_utf8(NULL, "Folder does not exist, continue anyway?", MAMEUINAME, MB_OKCANCEL) == IDOK)
			bResult = true;
	}

	if (bResult == true)
		SoftwareDirectories_OnInsertBrowse(hDlg, true, pItem->pszText);

	return bResult;
}


static BOOL DriverHasDevice(const game_driver *gamedrv, iodevice_t type)
{
	BOOL b = false;

	// allocate the machine config
	machine_config config(*gamedrv,MameUIGlobal());

	for (device_image_interface &dev : image_interface_iterator(config.root_device()))
	{
		if (!dev.user_loadable())
			continue;
		if (dev.image_type() == type)
		{
			b = true;
			break;
		}
	}
	return b;
}

BOOL PropSheetFilter_Config(const machine_config *drv, const game_driver *gamedrv)
{
	ram_device_iterator iter(drv->root_device());
	return (iter.first()) || DriverHasDevice(gamedrv, IO_PRINTER);
}

INT_PTR CALLBACK GameMessOptionsProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	INT_PTR rc = 0;
	BOOL bHandled = false;

	switch (Msg)
	{
	case WM_NOTIFY:
		switch (((NMHDR *) lParam)->code)
		{
		case LVN_ENDLABELEDIT:
			rc = SoftwareDirectories_OnEndLabelEdit(hDlg, (NMHDR *) lParam);
			bHandled = true;
			break;

		case LVN_BEGINLABELEDIT:
			rc = SoftwareDirectories_OnBeginLabelEdit(hDlg, (NMHDR *) lParam);
			bHandled = true;
			break;
		}
	}

	if (!bHandled)
		rc = GameOptionsProc(hDlg, Msg, wParam, lParam);

	return rc;
}



BOOL MessPropertiesCommand(HWND hWnd, WORD wNotifyCode, WORD wID, BOOL *changed)
{
	BOOL handled = true;

	switch(wID)
	{
		case IDC_DIR_BROWSE:
			if (wNotifyCode == BN_CLICKED)
				*changed = SoftwareDirectories_OnInsertBrowse(hWnd, true, NULL);
			break;

		case IDC_DIR_INSERT:
			if (wNotifyCode == BN_CLICKED)
				*changed = SoftwareDirectories_OnInsertBrowse(hWnd, false, NULL);
			break;

		case IDC_DIR_DELETE:
			if (wNotifyCode == BN_CLICKED)
				*changed = SoftwareDirectories_OnDelete(hWnd);
			break;

		default:
			handled = false;
			break;
	}
	return handled;
}

#endif

