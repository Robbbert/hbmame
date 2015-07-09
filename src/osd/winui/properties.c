// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

/***************************************************************************

  Properties.c

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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

// standard C headers
#include <tchar.h>

// MAME/MAMEUI headers
#include "emu.h"
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
#include "sound/vlm5030.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#ifdef MESS
#include "resourcems.h"
#include "propertiesms.h"
#endif
#include "directdraw.h"    /* has to be after samples.h */
#include "properties.h"
#include "drivenum.h"
#include "machine/ram.h"


#if defined(__GNUC__)
/* fix warning: cast does not match function type */
#undef  PropSheet_GetTabControl
#define PropSheet_GetTabControl(d) (HWND)(LRESULT)(int)SendMessage((d),PSM_GETTABCONTROL,0,0)
#endif /* defined(__GNUC__) */

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
static void InitializeD3DVersionUI(HWND hwnd);
static void InitializeVideoUI(HWND hwnd);
static void InitializeBIOSUI(HWND hwnd);
static void InitializeControllerMappingUI(HWND hwnd);
static void UpdateOptions(HWND hDlg, datamap *map, windows_options &opts);
static void UpdateProperties(HWND hDlg, datamap *map, windows_options &opts);
static void PropToOptions(HWND hWnd, windows_options &o);
static void OptionsToProp(HWND hWnd, windows_options &o);
static void SetPropEnabledControls(HWND hWnd);
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

/**************************************************************
 * Local private variables
 **************************************************************/

/* No longer used by the core, but we need it to predefine configurable screens for all games. */
#ifndef MAX_SCREENS
/* maximum number of screens for one game */
#define MAX_SCREENS					4
#endif

static windows_options pOrigOpts, pDefaultOpts, pCurrentOpts;
static datamap *properties_datamap;

static int  g_nGame            = 0;
static int  g_nFolder          = 0;
static int  g_nFolderGame      = 0;
static OPTIONS_TYPE g_nPropertyMode;
//static BOOL g_bUseDefaults     = FALSE;
//static BOOL g_bReset           = FALSE;
static BOOL  g_bAutoAspect[MAX_SCREENS] = {FALSE, FALSE, FALSE, FALSE};
static BOOL  g_bAutoSnapSize = FALSE;
static HICON g_hIcon = NULL;

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

static struct ComboBoxVideo
{
	const TCHAR*	m_pText;
	const char*		m_pData;
} g_ComboBoxVideo[] =
{
	{ TEXT("Auto"),                 "auto"    },
	{ TEXT("GDI"),                  "gdi"     },
	{ TEXT("DirectDraw"),           "ddraw"   },
	{ TEXT("Direct3D"),             "d3d"     },
//	{ TEXT("BGFX"),                 "bgfx"    },
	{ TEXT("OpenGL"),               "opengl"  },
};
#define NUMVIDEO (sizeof(g_ComboBoxVideo) / sizeof(g_ComboBoxVideo[0]))

static struct ComboBoxSound
{
	const TCHAR*	m_pText;
	const char*		m_pData;
} g_ComboBoxSound[] =
{
	{ TEXT("None"),                 "none"    },
	{ TEXT("Auto"),                 "auto"    },
	{ TEXT("DirectSound"),          "dsound"  },
};
#define NUMSOUND (sizeof(g_ComboBoxSound) / sizeof(g_ComboBoxSound[0]))

static struct ComboBoxD3DVersion
{
	const TCHAR*	m_pText;
	const int		m_pData;
} g_ComboBoxD3DVersion[] =
{
	{ TEXT("Version 9"),           9   },
//	{ TEXT("Version 8"),           8   },
};

#define NUMD3DVERSIONS (sizeof(g_ComboBoxD3DVersion) / sizeof(g_ComboBoxD3DVersion[0]))
#define WINOPTION_D3DVERSION "9"

static struct ComboBoxSelectScreen
{
	const TCHAR*	m_pText;
	const int		m_pData;
} g_ComboBoxSelectScreen[] =
{
	{ TEXT("Screen 0"),             0    },
	{ TEXT("Screen 1"),             1    },
	{ TEXT("Screen 2"),             2    },
	{ TEXT("Screen 3"),             3    },
};
#define NUMSELECTSCREEN (sizeof(g_ComboBoxSelectScreen) / sizeof(g_ComboBoxSelectScreen[0]))

static struct ComboBoxView
{
	const TCHAR*	m_pText;
	const char*		m_pData;
} g_ComboBoxView[] =
{
	{ TEXT("Auto"),		        "auto"        },
	{ TEXT("Standard"),         "standard"    },
	{ TEXT("Pixel Aspect"),     "pixel"       },
	{ TEXT("Cocktail"),         "cocktail"    },
};
#define NUMVIEW (sizeof(g_ComboBoxView) / sizeof(g_ComboBoxView[0]))



static struct ComboBoxDevices
{
	const TCHAR*	m_pText;
	const char* 	m_pData;
} g_ComboBoxDevice[] =
{
	{ TEXT("None"),                  "none"      },
	{ TEXT("Keyboard"),              "keyboard"  },
	{ TEXT("Mouse"),		 "mouse"     },
	{ TEXT("Joystick"),              "joystick"  },
	{ TEXT("Lightgun"),              "lightgun"  },
};

#define NUMDEVICES (sizeof(g_ComboBoxDevice) / sizeof(g_ComboBoxDevice[0]))

static struct ComboBoxSnapView
{
	const TCHAR*	m_pText;
	const char*		m_pData;
} g_ComboBoxSnapView[] =
{
	{ TEXT("Internal"),	        "internal"    },
	{ TEXT("Auto"),		        "auto"        },
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

// This function (and the code that use it) is a gross hack - but at least the vile
// and disgusting global variables are gone, making it less gross than what came before
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
	PROPSHEETHEADER pshead;
	PROPSHEETPAGE   *pspage;

	// clear globals
	//pCurrentOpts = NULL;

	g_nGame = GLOBAL_OPTIONS;
	windows_options dummy;
	pCurrentOpts = dummy;
	pOrigOpts = dummy;
	pDefaultOpts = dummy;

	/* Get default options to populate property sheets */
	load_options(pCurrentOpts, g_nGame);
	load_options(pOrigOpts, g_nGame);
	load_options(pDefaultOpts, -2);

	/* Stash the result for comparing later */
	//CreateGameOptions(pOrigOpts, OPTIONS_TYPE_GLOBAL);

	g_nPropertyMode = OPTIONS_GLOBAL;
	BuildDataMap();

	ZeroMemory(&pshead, sizeof(pshead));

	pspage = CreatePropSheetPages(hInst, TRUE, -1, &pshead.nPages, FALSE);
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
	PROPSHEETHEADER pshead;
	PROPSHEETPAGE   *pspage;
	TCHAR*          t_description = 0;
///	OPTIONS_TYPE    default_type = opt_type;

	if (highlight_brush == NULL)
		highlight_brush = CreateSolidBrush(HIGHLIGHT_COLOR);

	if (background_brush == NULL)
		background_brush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	// Initialize the options
	load_options(pCurrentOpts, game_num);
	load_options(pOrigOpts, game_num);
	if (game_num == GLOBAL_OPTIONS)
		load_options(pDefaultOpts, -2); // base opts is the backup for global
	else
		load_options(pDefaultOpts, -1); // global is the backup for games

	// Copy current_options to original options
///	CreateGameOptions(pOrigOpts, OPTIONS_TYPE_GLOBAL);

	// Copy icon to use for the property pages
	g_hIcon = CopyIcon(hIcon);

	// These MUST be valid, they are used as indicies
	g_nGame = game_num;
	g_nFolder = folder_id;

	// Keep track of OPTIONS_TYPE that was passed in.
	g_nPropertyMode = opt_type;

	// Evaluate if the current set uses the Default set
	//g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
	//g_bReset = FALSE;
	BuildDataMap();

	ZeroMemory(&pshead, sizeof(PROPSHEETHEADER));

	// Set the game to audit to this game

	// Create the property sheets
	if( OPTIONS_GAME == opt_type )
	{
		InitGameAudit(game_num);
//		pspage = CreatePropSheetPages(hInst, FALSE, &driver_list::driver(game_num), &pshead.nPages, TRUE);
		pspage = CreatePropSheetPages(hInst, FALSE, game_num, &pshead.nPages, TRUE);
	}
	else
		pspage = CreatePropSheetPages(hInst, FALSE, -1, &pshead.nPages, FALSE);

	if (!pspage)
		return;


	// Get the description use as the dialog caption.
	switch( opt_type )
	{
	case OPTIONS_GAME:
		t_description = tstring_from_utf8(ModifyThe(driver_list::driver(g_nGame).description));
		break;
	case OPTIONS_SOURCE:
		t_description = tstring_from_utf8(GetFolderNameByID(g_nFolder));
		break;
	case OPTIONS_GLOBAL:
		t_description = tstring_from_utf8("Default Settings");
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

	osd_free(t_description);
	free(pspage);
}


/*********************************************************************
 * Local Functions
 *********************************************************************/

/* Build CPU info string */
static char *GameInfoCPU(UINT nIndex)
{
	static char buf[1024];
	machine_config config(driver_list::driver(nIndex),pCurrentOpts);
	ZeroMemory(buf, sizeof(buf));

	execute_interface_iterator iter(config.root_device());
	device_execute_interface *cpu = iter.first();
	while (cpu)
	{
		if (cpu->device().clock() >= 1000000)
		{
			sprintf(&buf[strlen(buf)], "%s %d.%06d MHz",
				cpu->device().name(),
				cpu->device().clock() / 1000000,
				cpu->device().clock() % 1000000);
		}
		else
		{
			sprintf(&buf[strlen(buf)], "%s %d.%03d kHz",
				cpu->device().name(),
				cpu->device().clock() / 1000,
				cpu->device().clock() % 1000);
		}

		strcat(buf, "\n");

		cpu = iter.next();
	}

	return buf;
}

/* Build Sound system info string */
static char *GameInfoSound(UINT nIndex)
{
	static char buf[1024];

	buf[0] = 0;

	machine_config config(driver_list::driver(nIndex),pCurrentOpts);

	/* iterate over sound chips */
	sound_interface_iterator iter(config.root_device());
	const device_sound_interface *sound = iter.first();
	while(sound)
	{
		int clock,count;
		device_type sound_type_;
		char tmpname[1024];

		sprintf(tmpname,"%s",sound->device().name());

		sound_type_ = sound->device().type();
		clock = sound->device().clock();

		count = 1;
		sound = iter.next();
		/* Matching chips at the same clock are aggregated */
		while (sound
			&& sound->device().type() == sound_type_
			&& sound->device().clock() == clock)
		{
			count++;
			sound = iter.next();
		}

		if (count > 1)
		{
			sprintf(&buf[strlen(buf)],"%dx",count);
		}

		sprintf(&buf[strlen(buf)],"%s",tmpname);

		if (clock)
		{
			if (clock >= 1000000)
			{
				sprintf(&buf[strlen(buf)]," %d.%06d MHz",
					clock / 1000000,
					clock % 1000000);
			}
			else
			{
				sprintf(&buf[strlen(buf)]," %d.%03d kHz",
						clock / 1000,
						clock % 1000);
			}
		}

		strcat(buf,"\n");
	}
	return buf;
}

/* Build Display info string */
static char *GameInfoScreen(UINT nIndex)
{
	static char buf[1024];
	machine_config config(driver_list::driver(nIndex),pCurrentOpts);
	memset(buf, '\0', 1024);

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
			for (; screen != NULL; screen = iter.next())
			{
				char tmpbuf[256];
				const rectangle &visarea = screen->visible_area();

				if (driver_list::driver(nIndex).flags & ORIENTATION_SWAP_XY)
				{
					sprintf(tmpbuf,"%d x %d (V) %f Hz\n",
							visarea.max_y - visarea.min_y + 1,
							visarea.max_x - visarea.min_x + 1,
							ATTOSECONDS_TO_HZ(screen->refresh_attoseconds()));
				}
				else
				{
					sprintf(tmpbuf,"%d x %d (H) %f Hz\n",
							visarea.max_x - visarea.min_x + 1,
							visarea.max_y - visarea.min_y + 1,
							ATTOSECONDS_TO_HZ(screen->refresh_attoseconds()));
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
	static char buffer[1024];
	int audit_result = GetRomAuditResults(driver_index);
	memset(buffer,0,sizeof(char)*1024);
	if ( bRomStatus )
	{
		if (IsAuditResultKnown(audit_result) == FALSE)
			strcpy(buffer, "Unknown");
		else if (IsAuditResultYes(audit_result))
		{
			if (DriverIsBroken(driver_index))
			{
				strcpy(buffer, "Not working");

				if (driver_list::driver(driver_index).flags & GAME_UNEMULATED_PROTECTION)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game protection isn't fully emulated");
				}
				if (driver_list::driver(driver_index).flags & GAME_WRONG_COLORS)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors are completely wrong");
				}
				if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_COLORS)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors aren't 100% accurate");
				}
				if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_GRAPHICS)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Video emulation isn't 100% accurate");
				}
				if (driver_list::driver(driver_index).flags & GAME_NO_SOUND)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game lacks sound");
				}
				if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_SOUND)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Sound emulation isn't 100% accurate");
				}
				if (driver_list::driver(driver_index).flags & GAME_NO_COCKTAIL)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Screen flipping is not supported");
				}
				if (driver_list::driver(driver_index).flags & GAME_REQUIRES_ARTWORK)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game requires artwork");
				}
			}
			else
			{
				strcpy(buffer, "Working");

				if (driver_list::driver(driver_index).flags & GAME_UNEMULATED_PROTECTION)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game protection isn't fully emulated");
				}
				if (driver_list::driver(driver_index).flags & GAME_WRONG_COLORS)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors are completely wrong");
				}
				if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_COLORS)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Colors aren't 100% accurate");
				}
				if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_GRAPHICS)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Video emulation isn't 100% accurate");
				}
				if (driver_list::driver(driver_index).flags & GAME_NO_SOUND)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game lacks sound");
				}
				if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_SOUND)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Sound emulation isn't 100% accurate");
				}
				if (driver_list::driver(driver_index).flags & GAME_NO_COCKTAIL)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Screen flipping is not supported");
				}
				if (driver_list::driver(driver_index).flags & GAME_REQUIRES_ARTWORK)
				{
					if (*buffer != '\0')
						strcat(buffer, "\r\n");
					strcat(buffer, "Game requires artwork");
				}
			}
		}
		else
		{
			// audit result is no
			strcpy(buffer, "BIOS missing");
		}
	}
	else
	{
		//Just show the emulation flags
		if (DriverIsBroken(driver_index))
		{
			strcpy(buffer, "Not working");
		}
		else
		{
			strcpy(buffer, "Working");
		}
		if (driver_list::driver(driver_index).flags & GAME_UNEMULATED_PROTECTION)
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Game protection isn't fully emulated");
		}
		if (driver_list::driver(driver_index).flags & GAME_WRONG_COLORS)
		{
		if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Colors are completely wrong");
		}
		if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_COLORS)
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Colors aren't 100% accurate");
		}
		if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_GRAPHICS)
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Video emulation isn't 100% accurate");
		}
		if (driver_list::driver(driver_index).flags & GAME_NO_SOUND)
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Game lacks sound");
		}
		if (driver_list::driver(driver_index).flags & GAME_IMPERFECT_SOUND)
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Sound emulation isn't 100% accurate");
		}
		if (driver_list::driver(driver_index).flags & GAME_NO_COCKTAIL)
		{
			if (*buffer != '\0')
				strcat(buffer, "\r\n");
			strcat(buffer, "Screen flipping is not supported");
		}
		if (driver_list::driver(driver_index).flags & GAME_REQUIRES_ARTWORK)
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
	else if ((OPTIONS_SOURCE == opt_type) || (OPTIONS_VECTOR == opt_type) )
		strcpy(buf, "Global folder options\nDefault options used by all games in the folder");
	else
	if (OPTIONS_GAME == opt_type)
		sprintf(buf, "%s\n\"%s\"", ModifyThe(driver_list::driver(nIndex).description), driver_list::driver(nIndex).name);
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
			ConvertAmpersandString(ModifyThe(driver_list::driver(nParentIndex).description)),
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

		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_TITLE), GameInfoTitle(g_nPropertyMode, g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_MANUFACTURED),  GameInfoManufactured(g_nGame));
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_STATUS),        GameInfoStatus(g_nGame, FALSE));
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
// NOTE you cannot just say opts1=opts2, although in theory it should work, in practice opts1 becomes null.
INT_PTR CALLBACK GameOptionsProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	BOOL g_bUseDefaults = 0, g_bReset = 0;
	//d3d_version = pCurrentOpts.int_value(WINOPTION_D3DVERSION);


	switch (Msg)
	{
	case WM_INITDIALOG:
		/* Fill in the Game info at the top of the sheet */
		win_set_window_text_utf8(GetDlgItem(hDlg, IDC_PROP_TITLE), GameInfoTitle(g_nPropertyMode, g_nGame));
		InitializeOptions(hDlg);
		InitializeMisc(hDlg);

		UpdateProperties(hDlg, properties_datamap, pCurrentOpts);

		//load_options(pCurrentOpts, g_nGame);
		//load_options(pOrigOpts, g_nGame);
		//load_options(pDefaultOpts, -1);
		g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
		g_bReset = (pCurrentOpts != pOrigOpts);

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
		EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), TRUE);

		// Enable Apply button
		PropSheet_Changed(GetParent(hDlg), hDlg);

		// make sure everything's copied over, to determine what's changed
		UpdateOptions(hDlg, properties_datamap, pCurrentOpts);

		// redraw it, it might be a new color now
		InvalidateRect((HWND)lParam,NULL,TRUE);

		break;

	case WM_COMMAND:
		{
			/* Below, 'changed' is used to signify the 'Apply' button should be enabled. */
			WORD wID         = GET_WM_COMMAND_ID(wParam, lParam);
			HWND hWndCtrl    = GET_WM_COMMAND_HWND(wParam, lParam);
			WORD wNotifyCode = GET_WM_COMMAND_CMD(wParam, lParam);
			BOOL changed     = FALSE;
			int nCurSelection = 0;
			TCHAR szClass[256];

			switch (wID)
			{
			case IDC_REFRESH:
				if (wNotifyCode == LBN_SELCHANGE)
				{
					RefreshSelectionChange(hDlg, hWndCtrl);
					changed = TRUE;
				}
				break;

			case IDC_ASPECT:
				nCurSelection = Button_GetCheck( GetDlgItem(hDlg, IDC_ASPECT));
				if( g_bAutoAspect[GetSelectedScreen(hDlg)] != nCurSelection )
				{
					changed = TRUE;
					g_bAutoAspect[GetSelectedScreen(hDlg)] = nCurSelection;
				}
				break;

			case IDC_SNAPSIZE:
				nCurSelection = Button_GetCheck( GetDlgItem(hDlg, IDC_SNAPSIZE));
				if( g_bAutoSnapSize != nCurSelection )
				{
					changed = TRUE;
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

			case IDC_PROP_RESET:
				// RESET Button - Only do it if mouse-clicked
				if (wNotifyCode != BN_CLICKED)
					break;

				// Change settings in property sheets back to original
				UpdateProperties(hDlg, properties_datamap, pOrigOpts);
				// The original options become the current options.
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);

				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
				// Turn off Apply
				PropSheet_UnChanged(GetParent(hDlg), hDlg);
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
				break;

			case IDC_USE_DEFAULT:
				// DEFAULT Button - Only do it if mouse-clicked
				if (wNotifyCode != BN_CLICKED)
					break;

				// Change settings to be the same as mess.ini
				UpdateProperties(hDlg, properties_datamap, pDefaultOpts);
				// The original options become the current options.
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);

				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
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
					changed = TRUE;
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
						changed = TRUE;
				*************************************  */
				}
				break;
			default:
#ifdef MESS
			if (MessPropertiesCommand(hDlg, wNotifyCode, wID, &changed))
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

			if (changed == TRUE)
			{
				// make sure everything's copied over, to determine what's changed
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);
				// enable the apply button
				PropSheet_Changed(GetParent(hDlg), hDlg);
				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
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
				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
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
					//return TRUE;
				//}

				// Read the datamap
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);
				// The current options become the original options.
				UpdateOptions(hDlg, properties_datamap, pOrigOpts);

				// Repopulate the controls?  WTF?  We just read them, they should be fine.
				UpdateProperties(hDlg, properties_datamap, pCurrentOpts);

				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);

				// Save the current options
				save_options(pCurrentOpts, g_nGame);

				// Disable apply button
				PropSheet_UnChanged(GetParent(hDlg), hDlg);
				SetWindowLongPtr(hDlg, DWLP_MSGRESULT, PSNRET_NOERROR);

				return TRUE;

			case PSN_KILLACTIVE:
				/* Save Changes to the options here. */
				UpdateOptions(hDlg, properties_datamap, pCurrentOpts);
				// Determine button states.
				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);

				ResetDataMap(hDlg);
				SetWindowLongPtr(hDlg, DWLP_MSGRESULT, FALSE);
				return 1;

			case PSN_RESET:
				// Reset to the original values. Disregard changes
				//pCurrentOpts = pOrigOpts;
				g_bUseDefaults = (pCurrentOpts != pDefaultOpts);
				g_bReset = (pCurrentOpts != pOrigOpts);
				EnableWindow(GetDlgItem(hDlg, IDC_USE_DEFAULT), g_bUseDefaults);
				EnableWindow(GetDlgItem(hDlg, IDC_PROP_RESET), g_bReset);
				SetWindowLongPtr(hDlg, DWLP_MSGRESULT, FALSE);
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
	std::string error_string;

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
			o.set_value(aspect_option, "auto", OPTION_PRIORITY_CMDLINE,error_string);
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
			o.set_value(aspect_option, buffer2, OPTION_PRIORITY_CMDLINE,error_string);
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
			o.set_value(OPTION_SNAPSIZE, "auto", OPTION_PRIORITY_CMDLINE,error_string);
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
			o.set_value(OPTION_SNAPSIZE, buffer2, OPTION_PRIORITY_CMDLINE,error_string);
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
	HWND hCtrl;
	HWND hCtrl2;
	TCHAR buf[100];
	int  n = 0;
	int  d = 0;
	int  width = 0;
	int  height = 0;

	/* video */

	/* Setup refresh list based on depth. */
	datamap_update_control(properties_datamap, hWnd, pCurrentOpts, IDC_REFRESH);
	/* Setup Select screen*/
	UpdateSelectScreenUI(hWnd );

	hCtrl = GetDlgItem(hWnd, IDC_ASPECT);
	if (hCtrl)
	{
		Button_SetCheck(hCtrl, g_bAutoAspect[GetSelectedScreen(hWnd)] )	;
	}

	hCtrl = GetDlgItem(hWnd, IDC_SNAPSIZE);
	if (hCtrl)
	{
		Button_SetCheck(hCtrl, g_bAutoSnapSize )	;
	}

	/* Bios select list */
	hCtrl = GetDlgItem(hWnd, IDC_BIOS);
	if (hCtrl)
	{
		const char* cBuffer;
		int i = 0;
		int iCount = ComboBox_GetCount( hCtrl );
		for( i=0; i< iCount; i++ )
		{
			cBuffer = (const char*)ComboBox_GetItemData( hCtrl, i );
			if (strcmp(cBuffer, pCurrentOpts.value(OPTION_BIOS) ) == 0)
			{
				(void)ComboBox_SetCurSel(hCtrl, i);
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
			Button_SetCheck(hCtrl, TRUE);
			g_bAutoAspect[GetSelectedScreen(hWnd)] = TRUE;
		}
		else
		{
			Button_SetCheck(hCtrl, FALSE);
			g_bAutoAspect[GetSelectedScreen(hWnd)] = FALSE;
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
			std::string error_string;
			o.set_value(OPTION_EFFECT, effect, OPTION_PRIORITY_CMDLINE,error_string);
		}
		win_set_window_text_utf8(hCtrl, effect);
	}
	hCtrl = GetDlgItem(hWnd, IDC_SNAPSIZE);
	if (hCtrl)
	{
		if( strcmp(o.value(OPTION_SNAPSIZE), "auto") == 0)
		{
			Button_SetCheck(hCtrl, TRUE);
			g_bAutoSnapSize = TRUE;
		}
		else
		{
			Button_SetCheck(hCtrl, FALSE);
			g_bAutoSnapSize = FALSE;
		}
	}
	/* snapshot size */
	hCtrl  = GetDlgItem(hWnd, IDC_SNAPSIZEWIDTH);
	hCtrl2 = GetDlgItem(hWnd, IDC_SNAPSIZEHEIGHT);
	if (hCtrl && hCtrl2)
	{
		n = 0;
		d = 0;
		if (o.value(OPTION_SNAPSIZE))
		{
			if (sscanf(o.value(OPTION_SNAPSIZE), "%dx%d", &width, &height) == 2 && width != 0 && height != 0)
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
}

/* Adjust controls - tune them to the currently selected game */
static void SetPropEnabledControls(HWND hWnd)
{
	//HWND hCtrl;
	bool useart = TRUE;
	BOOL joystick_attached = FALSE;
	bool in_window = FALSE;
	bool hlsl_on = FALSE;
	int nIndex = g_nGame;

	// auto is a reserved word
	bool autov = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "auto")==0);
	bool d3d = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "d3d")==0) | autov;
	bool ddraw = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "ddraw")==0) | autov;
	//bool gdi = (core_stricmp(pCurrentOpts.value(OSDOPTION_VIDEO), "gdi")==0) | autov;

	in_window = pCurrentOpts.bool_value(OSDOPTION_WINDOW);
	Button_SetCheck(GetDlgItem(hWnd, IDC_ASPECT), g_bAutoAspect[GetSelectedScreen(hWnd)] );

	EnableWindow(GetDlgItem(hWnd, IDC_WAITVSYNC), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_TRIPLE_BUFFER), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_PRESCALE), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_PRESCALEDISP), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_PRESCALETEXT), d3d|ddraw);
	EnableWindow(GetDlgItem(hWnd, IDC_HWSTRETCH), ddraw && DirectDraw_HasHWStretch());
	EnableWindow(GetDlgItem(hWnd, IDC_SWITCHRES), TRUE);
	EnableWindow(GetDlgItem(hWnd, IDC_SYNCREFRESH), TRUE);
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

	EnableWindow(GetDlgItem(hWnd,IDC_D3D_FILTER),d3d);
	EnableWindow(GetDlgItem(hWnd,IDC_D3D_VERSION),(NUMD3DVERSIONS>1) & d3d);

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

	// HLSL - only enable if D3D selected
	if (d3d)
	{
		//d3d_version = pCurrentOpts.int_value(WINOPTION_D3DVERSION);

		//if (d3d_version >= 9)
			hlsl_on = TRUE;//pCurrentOpts.bool_value(WINOPTION_HLSL_ENABLE);
	}

	EnableWindow(GetDlgItem(hWnd, IDC_HLSL_ON), hlsl_on);

	/* Joystick options */
	joystick_attached = DIJoystick.Available();

	Button_Enable(GetDlgItem(hWnd,IDC_JOYSTICK),		joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JDZTEXT),			joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JDZDISP),			joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JDZ),				joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JSATTEXT),			joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JSATDISP),			joystick_attached);
	EnableWindow(GetDlgItem(hWnd, IDC_JSAT),				joystick_attached);
	/* Trackball / Mouse options */
	if (nIndex <= -1 || DriverUsesTrackball(nIndex) || DriverUsesLightGun(nIndex))
		Button_Enable(GetDlgItem(hWnd,IDC_USE_MOUSE),TRUE);
	else
		Button_Enable(GetDlgItem(hWnd,IDC_USE_MOUSE),FALSE);

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

		if( bOsVersionInfoEx && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osvi.dwMajorVersion >= 5) )
		{
			BOOL use_lightgun;
			//XP and above...
			Button_Enable(GetDlgItem(hWnd,IDC_LIGHTGUN),FALSE);
			use_lightgun = Button_GetCheck(GetDlgItem(hWnd,IDC_USE_MOUSE));
			Button_Enable(GetDlgItem(hWnd,IDC_DUAL_LIGHTGUN),FALSE);
			Button_Enable(GetDlgItem(hWnd,IDC_RELOAD),use_lightgun);
		}
		else
		{
			BOOL use_lightgun;
			// Older than XP
			Button_Enable(GetDlgItem(hWnd,IDC_LIGHTGUN),TRUE);
			use_lightgun = Button_GetCheck(GetDlgItem(hWnd,IDC_LIGHTGUN));
			Button_Enable(GetDlgItem(hWnd,IDC_DUAL_LIGHTGUN),use_lightgun);
			Button_Enable(GetDlgItem(hWnd,IDC_RELOAD),use_lightgun);
		}
	}
	else
	{
		Button_Enable(GetDlgItem(hWnd,IDC_LIGHTGUN),FALSE);
		Button_Enable(GetDlgItem(hWnd,IDC_DUAL_LIGHTGUN),FALSE);
		Button_Enable(GetDlgItem(hWnd,IDC_RELOAD),FALSE);
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
		EnableWindow(GetDlgItem(hWnd, IDC_FRAMESKIP), FALSE);
	else
		EnableWindow(GetDlgItem(hWnd, IDC_FRAMESKIP), 1);

	if (nIndex <= -1 || DriverHasOptionalBIOS(nIndex))
		EnableWindow(GetDlgItem(hWnd,IDC_BIOS),TRUE);
	else
		EnableWindow(GetDlgItem(hWnd,IDC_BIOS),FALSE);

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

	// Any work to do?  If so, make the changes and return TRUE.
	if (selected_index != original_selection)
	{
		// Set the options based on the new selection.
		std::string error_string;
		opts->set_value(OPTION_ROR,	selected_index == 1, OPTION_PRIORITY_CMDLINE,error_string);
		opts->set_value(OPTION_ROL,	selected_index == 2, OPTION_PRIORITY_CMDLINE,error_string);
		opts->set_value(OPTION_ROTATE,	selected_index != 3, OPTION_PRIORITY_CMDLINE,error_string);
		opts->set_value(OPTION_AUTOROR,	selected_index == 4, OPTION_PRIORITY_CMDLINE,error_string);
		opts->set_value(OPTION_AUTOROL,	selected_index == 5, OPTION_PRIORITY_CMDLINE,error_string);
		return TRUE;
	}

	// No changes
	return FALSE;
}



static BOOL RotatePopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int selected_index = 0;
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

	(void)ComboBox_SetCurSel(control, selected_index);
	return FALSE;
}



static BOOL ScreenReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	char screen_option_name[32];
	TCHAR *screen_option_value;
	int selected_screen;
	int screen_option_index;
	char *op_val;
	std::string error_string;

	selected_screen = GetSelectedScreen(dialog);
	screen_option_index = ComboBox_GetCurSel(control);
	screen_option_value = (TCHAR*) ComboBox_GetItemData(control, screen_option_index);
	snprintf(screen_option_name, ARRAY_LENGTH(screen_option_name), "screen%d", selected_screen);
	op_val = utf8_from_tstring(screen_option_value);
	opts->set_value(screen_option_name, op_val, OPTION_PRIORITY_CMDLINE,error_string);
	osd_free(op_val);
	return FALSE;
}



static BOOL ScreenPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	//int iMonitors;
	DISPLAY_DEVICE dd;
	int i = 0;
	int nSelection = 0;
	TCHAR* t_option = 0;

	/* Remove all items in the list. */
	(void)ComboBox_ResetContent(control);
	(void)ComboBox_InsertString(control, 0, TEXT("Auto"));
	(void)ComboBox_SetItemData(control, 0, (void*)tstring_from_utf8("auto"));

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
			(void)ComboBox_InsertString(control, i+1, win_tstring_strdup(dd.DeviceName));
			(void)ComboBox_SetItemData(control, i+1, (void*)win_tstring_strdup(dd.DeviceName));

			snprintf(screen_option, ARRAY_LENGTH(screen_option), "screen%d", GetSelectedScreen(dialog));
			t_option = tstring_from_utf8(opts->value(screen_option));
			if( !t_option )
				return FALSE;
			if (_tcscmp(t_option, dd.DeviceName) == 0)
				nSelection = i+1;
			osd_free(t_option);
		}
	}
	(void)ComboBox_SetCurSel(control, nSelection);
	return FALSE;
}



static void ViewSetOptionName(datamap *map, HWND dialog, HWND control, char *buffer, size_t buffer_size)
{
	snprintf(buffer, buffer_size, "view%d", GetSelectedScreen(dialog));
}

static BOOL ViewPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int i;
	int selected_index = 0;
	char view_option[32];
	const char *view;

	// determine the view option value
	snprintf(view_option, ARRAY_LENGTH(view_option), "view%d", GetSelectedScreen(dialog));
	view = opts->value(view_option);

	(void)ComboBox_ResetContent(control);
	for (i = 0; i < NUMVIEW; i++)
	{
		(void)ComboBox_InsertString(control, i, g_ComboBoxView[i].m_pText);
		(void)ComboBox_SetItemData(control, i, g_ComboBoxView[i].m_pData);

		if (strcmp(view, g_ComboBoxView[i].m_pData)==0)
			selected_index = i;
	}
	(void)ComboBox_SetCurSel(control, selected_index);
	return FALSE;
}

static BOOL SnapViewPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int i;
	int selected_index = 0;
	const char *snapview;

	// determine the snapview option value
	snapview = opts->value(OPTION_SNAPVIEW);

	(void)ComboBox_ResetContent(control);
	for (i = 0; i < NUMSNAPVIEW; i++)
	{
		(void)ComboBox_InsertString(control, i, g_ComboBoxSnapView[i].m_pText);
		(void)ComboBox_SetItemData(control, i, g_ComboBoxSnapView[i].m_pData);

		if (strcmp(snapview, g_ComboBoxSnapView[i].m_pData)==0)
			selected_index = i;
	}
	(void)ComboBox_SetCurSel(control, selected_index);
	return FALSE;
}

static BOOL DefaultInputReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	TCHAR *input_option_value;
	int input_option_index;
	char *op_val = NULL;
	std::string error_string;

	input_option_index = ComboBox_GetCurSel(control);
	input_option_value = (TCHAR*) ComboBox_GetItemData(control, input_option_index);
	op_val = utf8_from_tstring(input_option_value);
	opts->set_value(OPTION_CTRLR, input_option_index ? op_val : "", OPTION_PRIORITY_CMDLINE,error_string);
	osd_free(op_val);
	return FALSE;
}

static BOOL DefaultInputPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	TCHAR *ext;
	TCHAR root[256];
	TCHAR path[256];
	int selected = 0;
	int index = 0;
	LPCTSTR t_ctrlr_option = 0;
	LPTSTR t_buf = 0;
	const char *ctrlr_option;
	TCHAR* t_ctrldir;

	// determine the ctrlr option
	ctrlr_option = opts->value(OPTION_CTRLR);
	if( ctrlr_option != NULL )
	{
		t_buf = tstring_from_utf8(ctrlr_option);
		if( !t_buf )
			return FALSE;
		t_ctrlr_option = t_buf;
	}
	else
	{
		t_ctrlr_option = TEXT("");
	}

	// reset the controllers dropdown
	(void)ComboBox_ResetContent(control);
	(void)ComboBox_InsertString(control, index, TEXT("Default"));
	(void)ComboBox_SetItemData(control, index, "");
	index++;

	t_ctrldir = tstring_from_utf8(GetCtrlrDir());
	if( !t_ctrldir )
	{
		if( t_buf )
			osd_free(t_buf);
		return FALSE;
	}

	_stprintf (path, TEXT("%s\\*.*"), t_ctrldir);

	osd_free(t_ctrldir);

	hFind = FindFirstFile(path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// copy the filename
			_tcscpy (root,FindFileData.cFileName);

			// find the extension
			ext = _tcsrchr (root,'.');
			if (ext)
			{
				// check if it's a cfg file
				if (_tcscmp (ext, TEXT(".cfg")) == 0)
				{
					// and strip off the extension
					*ext = 0;

					// set the option?
					if (!_tcscmp(root, t_ctrlr_option))
						selected = index;

					// add it as an option
					(void)ComboBox_InsertString(control, index, root);
					(void)ComboBox_SetItemData(control, index, (void*)win_tstring_strdup(root));	// FIXME - leaks memory!
					index++;
				}
			}
		}
		while (FindNextFile (hFind, &FindFileData) != 0);

		FindClose (hFind);
	}

	(void)ComboBox_SetCurSel(control, selected);

	if( t_buf )
		osd_free(t_buf);

	return FALSE;
}



static void ResolutionSetOptionName(datamap *map, HWND dialog, HWND control, char *buffer, size_t buffer_size)
{
	snprintf(buffer, buffer_size, "resolution%d", GetSelectedScreen(dialog));
}


static BOOL ResolutionReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	HWND refresh_control = GetDlgItem(dialog, IDC_REFRESH);
	HWND sizes_control = GetDlgItem(dialog, IDC_SIZES);
	int refresh_index, refresh_value, width, height;
	char option_value[256];
	TCHAR buffer[256];
	std::string error_string;

	if (refresh_control && sizes_control)
	{
		(void)ComboBox_GetText(sizes_control, buffer, ARRAY_LENGTH(buffer) - 1);
		if (_stscanf(buffer, TEXT("%d x %d"), &width, &height) == 2)
		{
			refresh_index = ComboBox_GetCurSel(refresh_control);
			refresh_value = ComboBox_GetItemData(refresh_control, refresh_index);
			snprintf(option_value, ARRAY_LENGTH(option_value), "%dx%d@%d", width, height, refresh_value);
		}
		else
		{
			snprintf(option_value, ARRAY_LENGTH(option_value), "auto");
		}
		opts->set_value(option_name, option_value, OPTION_PRIORITY_CMDLINE,error_string);
	}
	return FALSE;
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
	int i;
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
		(void)ComboBox_ResetContent(sizes_control);
		(void)ComboBox_InsertString(sizes_control, sizes_index, TEXT("Auto"));
		(void)ComboBox_SetItemData(sizes_control, sizes_index, 0);
		sizes_index++;

		// reset refresh control
		(void)ComboBox_ResetContent(refresh_control);
		(void)ComboBox_InsertString(refresh_control, refresh_index, TEXT("Auto"));
		(void)ComboBox_SetItemData(refresh_control, refresh_index, 0);
		refresh_index++;

		// determine which screen we're using
		snprintf(screen_option, ARRAY_LENGTH(screen_option), "screen%d", GetSelectedScreen(dialog));
		screen = opts->value(screen_option);
		t_screen = tstring_from_utf8(screen);

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
					(void)ComboBox_InsertString(sizes_control, sizes_index, buf);

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
					(void)ComboBox_InsertString(refresh_control, refresh_index, buf);
					(void)ComboBox_SetItemData(refresh_control, refresh_index, devmode.dmDisplayFrequency);

					if (refresh == devmode.dmDisplayFrequency)
						refresh_selection = refresh_index;

					refresh_index++;
				}
			}
		}
		osd_free(t_screen);

		(void)ComboBox_SetCurSel(sizes_control, sizes_selection);
		(void)ComboBox_SetCurSel(refresh_control, refresh_selection);
	}
	return FALSE;
}



//============================================================
#ifdef MESS
//============================================================
//  DATAMAP HANDLERS FOR MESS
//============================================================

static void AppendList(HWND hList, LPCTSTR lpItem, int nItem)
{
	LV_ITEM Item;
	HRESULT res;
	memset(&Item, 0, sizeof(LV_ITEM));
	Item.mask = LVIF_TEXT;
	Item.pszText = (LPTSTR) lpItem;
	Item.iItem = nItem;
	res = ListView_InsertItem(hList, &Item);
	res++;
}

static BOOL DirListReadControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int directory_count;
	LV_ITEM lvi;
	TCHAR buffer[2048];
	char *utf8_dir_list;
	int i, pos, driver_index;
	BOOL res;

	// determine the directory count; note that one item is the "<    >" entry
	directory_count = ListView_GetItemCount(control);
	if (directory_count > 0)
		directory_count--;

	buffer[0] = '\0';
	pos = 0;

	for (i = 0; i < directory_count; i++)
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

	utf8_dir_list = utf8_from_tstring(buffer);
	if (utf8_dir_list != NULL)
	{
		driver_index = PropertiesCurrentGame(dialog);
		SetExtraSoftwarePaths(driver_index, utf8_dir_list);
		std::string error_string;
		pCurrentOpts.set_value(OPTION_COMMENT_DIRECTORY, utf8_dir_list, OPTION_PRIORITY_CMDLINE,error_string);
		osd_free(utf8_dir_list);
	}
	res++;
	return TRUE;
}


static BOOL DirListPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int driver_index, pos, new_pos, current_item;
	const char *dir_list;
	TCHAR *t_dir_list;
	TCHAR *s;
	LV_COLUMN lvc;
	RECT r;
	HRESULT res;
	BOOL b_res;

	// access the directory list, and convert to TCHARs
	driver_index = PropertiesCurrentGame(dialog);
	dir_list = GetExtraSoftwarePaths(driver_index, 0);
	t_dir_list = tstring_from_utf8(dir_list);
	if (!t_dir_list)
		return FALSE;

	// delete all items in the list control
	b_res = ListView_DeleteAllItems(control);

	// add the column
	GetClientRect(control, &r);
	memset(&lvc, 0, sizeof(LVCOLUMN));
	lvc.mask = LVCF_WIDTH;
	lvc.cx = r.right - r.left - GetSystemMetrics(SM_CXHSCROLL);
	res = ListView_InsertColumn(control, 0, &lvc);

	// add each of the directories
	pos = 0;
	current_item = 0;
	while(t_dir_list[pos] != '\0')
	{
		// parse off this item
		s = _tcschr(&t_dir_list[pos], ';');
		if (s != NULL)
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
	osd_free(t_dir_list);
	res++;
	b_res++;
	return TRUE;
}



static const char *messram_string(char *buffer, UINT32 ram)
{
	const char *suffix;

	if ((ram % (1024*1024)) == 0)
	{
		ram /= 1024*1024;
		suffix = "m";
	}
	else if ((ram % 1024) == 0)
	{
		ram /= 1024;
		suffix = "k";
	}
	else
		suffix = "";

	sprintf(buffer, "%u%s", ram, suffix);
	return buffer;
}

static BOOL RamPopulateControl(datamap *map, HWND dialog, HWND control, windows_options *opts, const char *option_name)
{
	int i, current_index, driver_index;
	const game_driver *gamedrv;
	UINT32 ram, current_ram;
	const char *this_ram_string;
	TCHAR* t_ramstring;

	// identify the driver
	driver_index = PropertiesCurrentGame(dialog);
	gamedrv = &driver_list::driver(driver_index);

	// clear out the combo box
	(void)ComboBox_ResetContent(control);

	// allocate the machine config
	machine_config cfg(*gamedrv,*opts);

	// identify how many options that we have
	ram_device_iterator iter(cfg.root_device());
	ram_device *device = iter.first();

	EnableWindow(control, (device != NULL));
	i = 0;

	// we can only do something meaningful if there is more than one option
	if (device != NULL)
	{
		const ram_device *ramdev = dynamic_cast<const ram_device *>(device);

		// identify the current amount of RAM
		this_ram_string = opts->value(OPTION_RAMSIZE);
		current_ram = (this_ram_string != NULL) ? ramdev->parse_string(this_ram_string) : 0;
		if (current_ram == 0)
			current_ram = ramdev->default_size();

		// by default, assume index 0
		current_index = 0;

		{
			char tmpval[20] ;
			ram = ramdev->default_size();
			messram_string(tmpval,ram);
			t_ramstring = tstring_from_utf8(tmpval);
			if( !t_ramstring )
				return FALSE;

			(void)ComboBox_InsertString(control, i, win_tstring_strdup(t_ramstring));
			(void)ComboBox_SetItemData(control, i, ram);

			osd_free(t_ramstring);
		}
		if (ramdev->extra_options() != NULL)
		{
			int j;
			int size = strlen(ramdev->extra_options());
			char * const s = core_strdup(ramdev->extra_options());
			char * const e = s + size;
			char *p = s;
			for (j=0;j<size;j++)
				if (p[j]==',')
					p[j]=0;

			/* try to parse each option */
			while(p <= e)
			{
				i++;
				// identify this option
				ram = ramdev->parse_string(p);

				this_ram_string = p;

				t_ramstring = tstring_from_utf8(this_ram_string);
				if( !t_ramstring )
					return FALSE;

				// add this option to the combo box
				(void)ComboBox_InsertString(control, i, win_tstring_strdup(t_ramstring));
				(void)ComboBox_SetItemData(control, i, ram);

				osd_free(t_ramstring);

				// is this the current option?  record the index if so
				if (ram == current_ram)
					current_index = i;

				p+= strlen(p);
				if (p == e)
					break;

				p++;
			}

			osd_free(s);
		}

		// set the combo box
		(void)ComboBox_SetCurSel(control, current_index);
	}
	return TRUE;
}
#endif

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
		std::string error_string;
		pCurrentOpts.set_value(screen_option, "auto", OPTION_PRIORITY_CMDLINE,error_string);
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
	datamap_add(properties_datamap, IDC_BEZELS,				DM_BOOL,	OPTION_USE_BEZELS);
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

	// core vector options
	datamap_add(properties_datamap, IDC_ANTIALIAS,				DM_BOOL,	OPTION_ANTIALIAS);
	datamap_add(properties_datamap, IDC_BEAM,					DM_FLOAT,	OPTION_BEAM);
	datamap_add(properties_datamap, IDC_BEAMDISP,				DM_FLOAT,	OPTION_BEAM);
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

	// windows debugging options
	datamap_add(properties_datamap, IDC_OSLOG,					DM_BOOL,	OPTION_OSLOG);

	// windows performance options
	datamap_add(properties_datamap, IDC_HIGH_PRIORITY,			DM_INT,		WINOPTION_PRIORITY);
	datamap_add(properties_datamap, IDC_HIGH_PRIORITYTXT,		DM_INT,		WINOPTION_PRIORITY);
	datamap_add(properties_datamap, IDC_MULTITHREAD_RENDERING,	DM_BOOL,	OSDOPTION_MULTITHREADING);

	// windows video options
	datamap_add(properties_datamap, IDC_VIDEO_MODE,				DM_STRING,	OSDOPTION_VIDEO);
	datamap_add(properties_datamap, IDC_NUMSCREENS,				DM_INT,		OSDOPTION_NUMSCREENS);
	datamap_add(properties_datamap, IDC_NUMSCREENSDISP,			DM_INT,		OSDOPTION_NUMSCREENS);
	datamap_add(properties_datamap, IDC_WINDOWED,				DM_BOOL,	OSDOPTION_WINDOW);
	datamap_add(properties_datamap, IDC_MAXIMIZE,				DM_BOOL,	OSDOPTION_MAXIMIZE);
	datamap_add(properties_datamap, IDC_KEEPASPECT,				DM_BOOL,	OSDOPTION_KEEPASPECT);
	datamap_add(properties_datamap, IDC_PRESCALE,				DM_INT,		OSDOPTION_PRESCALE);
	datamap_add(properties_datamap, IDC_PRESCALEDISP,			DM_INT,		OSDOPTION_PRESCALE);
	datamap_add(properties_datamap, IDC_EFFECT,				DM_STRING,	OPTION_EFFECT);
	datamap_add(properties_datamap, IDC_WAITVSYNC,				DM_BOOL,	OSDOPTION_WAITVSYNC);
	datamap_add(properties_datamap, IDC_SYNCREFRESH,			DM_BOOL,	OSDOPTION_SYNCREFRESH);

	// DirectDraw specific options
	datamap_add(properties_datamap, IDC_HWSTRETCH,				DM_BOOL,	WINOPTION_HWSTRETCH);

	// Direct3D specific options
	datamap_add(properties_datamap, IDC_D3D_VERSION,			DM_INT,		WINOPTION_D3DVERSION);
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

#ifdef MESS
	// show menu
	datamap_add(properties_datamap, IDC_SHOW_MENU,				DM_BOOL,	WINOPTION_MENU);
#endif

	// hlsl
	datamap_add(properties_datamap, IDC_HLSL_ON,				DM_BOOL,	WINOPTION_HLSL_ENABLE);

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
	datamap_set_float_format(properties_datamap, IDC_BEAMDISP,		"%03.2f");
	datamap_set_float_format(properties_datamap, IDC_FLICKERDISP,		"%03.2f");
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
	datamap_set_trackbar_range(properties_datamap, IDC_BEAM,        (float)0.10, 10.00, (float)0.10);
	datamap_set_trackbar_range(properties_datamap, IDC_AUDIO_LATENCY, 1,  5, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_VOLUME,      -32,  0, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_SECONDSTORUN, 0,  60, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_NUMSCREENS, 1,  4, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_PRESCALE, 1,  10, 1);
	datamap_set_trackbar_range(properties_datamap, IDC_FSGAMMA, 0.0,  3.0, 0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_FSBRIGHTNESS, 0.00,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_FSCONTRAST, 0.0,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_GAMMA, 0.0,  3.0, 0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_BRIGHTCORRECT, 0.00,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_CONTRAST, 0.0,  2.0, (float)0.1);
	datamap_set_trackbar_range(properties_datamap, IDC_PAUSEBRIGHT, 0.00,  1.00, (float)0.05);

#ifdef MESS
	// MESS specific stuff
	datamap_add(properties_datamap, IDC_DIR_LIST,			DM_STRING,	NULL);
	datamap_add(properties_datamap, IDC_RAM_COMBOBOX,		DM_INT,		OPTION_RAMSIZE);

	// set up callbacks
	datamap_set_callback(properties_datamap, IDC_DIR_LIST,		DCT_READ_CONTROL,	DirListReadControl);
	datamap_set_callback(properties_datamap, IDC_DIR_LIST,		DCT_POPULATE_CONTROL,	DirListPopulateControl);
	datamap_set_callback(properties_datamap, IDC_RAM_COMBOBOX,	DCT_POPULATE_CONTROL,	RamPopulateControl);
#endif
}


static void SetSamplesEnabled(HWND hWnd, int nIndex, BOOL bSoundEnabled)
{
	BOOL enabled = FALSE;
	HWND hCtrl;

	hCtrl = GetDlgItem(hWnd, IDC_SAMPLES);


	if (hCtrl)
	{
		if ( nIndex > -1 )
		{
			machine_config config(driver_list::driver(nIndex),pCurrentOpts);

			sound_interface_iterator iter(config.root_device());
			for (device_sound_interface *sound = iter.first(); sound != NULL; sound = iter.next())
			{
				if ((sound->device().type() == SAMPLES) || (sound->device().type() == VLM5030))
					enabled = TRUE;
			}
		}
		enabled = enabled && bSoundEnabled;
		EnableWindow(hCtrl, enabled);
	}
}

/* Moved here cause it's called in a few places */
static void InitializeOptions(HWND hDlg)
{
	InitializeSoundUI(hDlg);
	InitializeSkippingUI(hDlg);
	InitializeRotateUI(hDlg);
	InitializeSelectScreenUI(hDlg);
	InitializeBIOSUI(hDlg);
	InitializeControllerMappingUI(hDlg);
	InitializeD3DVersionUI(hDlg);
	InitializeVideoUI(hDlg);
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
	HWND    hCtrl;
	int i;

	hCtrl = GetDlgItem(hwnd, IDC_SOUND_MODE);
	if (hCtrl)
	{
		for (i = 0; i < NUMSOUND; i++)
		{
			(void)ComboBox_InsertString(hCtrl, i, g_ComboBoxSound[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl, i, g_ComboBoxSound[i].m_pData);
		}
	}

	i = 0;

	hCtrl = GetDlgItem(hwnd, IDC_SAMPLERATE);
	if (hCtrl)
	{
		(void)ComboBox_AddString(hCtrl, TEXT("11025"));
		(void)ComboBox_SetItemData(hCtrl, i++, 11025);
		(void)ComboBox_AddString(hCtrl, TEXT("22050"));
		(void)ComboBox_SetItemData(hCtrl, i++, 22050);
		(void)ComboBox_AddString(hCtrl, TEXT("44100"));
		(void)ComboBox_SetItemData(hCtrl, i++, 44100);
		(void)ComboBox_AddString(hCtrl, TEXT("48000"));
		(void)ComboBox_SetItemData(hCtrl, i++, 48000);
		(void)ComboBox_SetCurSel(hCtrl, 1);
	}
}

/* Populate the Frame Skipping drop down */
static void InitializeSkippingUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_FRAMESKIP);
	int i = 0;

	if (hCtrl)
	{
		(void)ComboBox_AddString(hCtrl, TEXT("Draw every frame"));
		(void)ComboBox_SetItemData(hCtrl, i++, 0);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 1 frame"));
		(void)ComboBox_SetItemData(hCtrl, i++, 1);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 2 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 2);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 3 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 3);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 4 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 4);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 5 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 5);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 6 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 6);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 7 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 7);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 8 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 8);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 9 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 9);
		(void)ComboBox_AddString(hCtrl, TEXT("Skip 10 frames"));
		(void)ComboBox_SetItemData(hCtrl, i++, 10);
	}
}

/* Populate the Rotate drop down */
static void InitializeRotateUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_ROTATE);

	if (hCtrl)
	{
		(void)ComboBox_AddString(hCtrl, TEXT("Default"));             // 0
		(void)ComboBox_AddString(hCtrl, TEXT("Clockwise"));           // 1
		(void)ComboBox_AddString(hCtrl, TEXT("Anti-clockwise"));      // 2
		(void)ComboBox_AddString(hCtrl, TEXT("None"));                // 3
		(void)ComboBox_AddString(hCtrl, TEXT("Auto clockwise"));      // 4
		(void)ComboBox_AddString(hCtrl, TEXT("Auto anti-clockwise")); // 5
	}
}

/* Populate the Video Mode drop down */
static void InitializeVideoUI(HWND hwnd)
{
	HWND    hCtrl;

	hCtrl = GetDlgItem(hwnd, IDC_VIDEO_MODE);
	if (hCtrl)
	{
		int i;
		for (i = 0; i < NUMVIDEO; i++)
		{
			(void)ComboBox_InsertString(hCtrl, i, g_ComboBoxVideo[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl, i, g_ComboBoxVideo[i].m_pData);
		}
	}
}

/* Populate the D3D Version drop down */
static void InitializeD3DVersionUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_D3D_VERSION);
	if (hCtrl)
	{
		int i;
		for (i = 0; i < NUMD3DVERSIONS; i++)
		{
			(void)ComboBox_InsertString(hCtrl, i, g_ComboBoxD3DVersion[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl, i, g_ComboBoxD3DVersion[i].m_pData);
		}
	}
}

static void UpdateSelectScreenUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd, IDC_SCREENSELECT);
	if (hCtrl)
	{
		int i, curSel;
		curSel = ComboBox_GetCurSel(hCtrl);
		if ((curSel < 0) || (curSel >= NUMSELECTSCREEN))
			curSel = 0;
		(void)ComboBox_ResetContent(hCtrl);
		for (i = 0; i < NUMSELECTSCREEN && i < pCurrentOpts.int_value(OSDOPTION_NUMSCREENS) ; i++)
		{
			(void)ComboBox_InsertString(hCtrl, i, g_ComboBoxSelectScreen[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl, i, g_ComboBoxSelectScreen[i].m_pData);
		}
		// Smaller Amount of screens was selected, so use 0
		if( i < curSel )
			(void)ComboBox_SetCurSel(hCtrl, 0);
		else
			(void)ComboBox_SetCurSel(hCtrl, curSel);
	}
}

/* Populate the Select Screen drop down */
static void InitializeSelectScreenUI(HWND hwnd)
{
	UpdateSelectScreenUI(hwnd);
}

static void InitializeControllerMappingUI(HWND hwnd)
{
	int i;
	HWND hCtrl  = GetDlgItem(hwnd,IDC_PADDLE);
	HWND hCtrl1 = GetDlgItem(hwnd,IDC_ADSTICK);
	HWND hCtrl2 = GetDlgItem(hwnd,IDC_PEDAL);
	HWND hCtrl3 = GetDlgItem(hwnd,IDC_MOUSE);
	HWND hCtrl4 = GetDlgItem(hwnd,IDC_DIAL);
	HWND hCtrl5 = GetDlgItem(hwnd,IDC_TRACKBALL);
	HWND hCtrl6 = GetDlgItem(hwnd,IDC_LIGHTGUNDEVICE);
	HWND hCtrl7 = GetDlgItem(hwnd,IDC_POSITIONAL);

	for (i = 0; i < NUMDEVICES; i++)
	{
		if (hCtrl)
		{
			(void)ComboBox_InsertString(hCtrl, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl1)
		{
			(void)ComboBox_InsertString(hCtrl1, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl1, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl2)
		{
			(void)ComboBox_InsertString(hCtrl2, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl2, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl3)
		{
			(void)ComboBox_InsertString(hCtrl3, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl3, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl4)
		{
			(void)ComboBox_InsertString(hCtrl4, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl4, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl5)
		{
			(void)ComboBox_InsertString(hCtrl5, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl5, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl6)
		{
			(void)ComboBox_InsertString(hCtrl6, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl6, i, g_ComboBoxDevice[i].m_pData);
		}

		if (hCtrl7)
		{
			(void)ComboBox_InsertString(hCtrl7, i, g_ComboBoxDevice[i].m_pText);
			(void)ComboBox_SetItemData( hCtrl7, i, g_ComboBoxDevice[i].m_pData);
		}
	}
}


static void InitializeBIOSUI(HWND hwnd)
{
	HWND hCtrl = GetDlgItem(hwnd,IDC_BIOS);
	int i = 0;
	TCHAR* t_s;
	if (hCtrl)
	{
		const game_driver *gamedrv = &driver_list::driver(g_nGame);
		const rom_entry *rom;

		if (g_nGame == GLOBAL_OPTIONS)
		{
			(void)ComboBox_InsertString(hCtrl, i, TEXT("None"));
			(void)ComboBox_SetItemData( hCtrl, i++, "");
			return;
		}
		if (g_nGame == FOLDER_OPTIONS) //Folder Options
		{
			gamedrv = &driver_list::driver(g_nFolderGame);
			if (DriverHasOptionalBIOS(g_nFolderGame) == FALSE)
			{
				(void)ComboBox_InsertString(hCtrl, i, TEXT("None"));
				(void)ComboBox_SetItemData( hCtrl, i++, "");
				return;
			}
			(void)ComboBox_InsertString(hCtrl, i, TEXT("Default"));
			(void)ComboBox_SetItemData( hCtrl, i++, "");

			if (gamedrv->rom != NULL)
			{
				for (rom = gamedrv->rom; !ROMENTRY_ISEND(rom); rom++)
				{
					if (ROMENTRY_ISSYSTEM_BIOS(rom))
					{
						const char *name = ROM_GETHASHDATA(rom);
						const char *biosname = ROM_GETNAME(rom);
						t_s = tstring_from_utf8(name);
						if( !t_s )
							return;
						(void)ComboBox_InsertString(hCtrl, i, win_tstring_strdup(t_s));
						(void)ComboBox_SetItemData( hCtrl, i++, biosname);
						osd_free(t_s);
					}
				}
			}
			return;
		}

		if (DriverHasOptionalBIOS(g_nGame) == FALSE)
		{
			(void)ComboBox_InsertString(hCtrl, i, TEXT("None"));
			(void)ComboBox_SetItemData( hCtrl, i++, "");
			return;
		}
		(void)ComboBox_InsertString(hCtrl, i, TEXT("Default"));
		(void)ComboBox_SetItemData( hCtrl, i++, "");

		if (gamedrv->rom != NULL)
		{
			for (rom = gamedrv->rom; !ROMENTRY_ISEND(rom); rom++)
			{
				if (ROMENTRY_ISSYSTEM_BIOS(rom))
				{
					const char *name = ROM_GETHASHDATA(rom);
					const char *biosname = ROM_GETNAME(rom);
					t_s = tstring_from_utf8(name);
					if( !t_s )
						return;
					(void)ComboBox_InsertString(hCtrl, i, win_tstring_strdup(t_s));
					(void)ComboBox_SetItemData( hCtrl, i++, biosname);
					osd_free(t_s);
				}
			}
		}
	}
}

static BOOL SelectEffect(HWND hWnd)
{
	char filename[MAX_PATH];
	BOOL changed = FALSE;

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
		for(i=j+1;i<k;i++)
		{
			buff[l++] = filename[i];
		}
		buff[l] = '\0';

		if (strcmp(buff, pCurrentOpts.value(OPTION_EFFECT))!=0)
		{
			HWND control = GetDlgItem(hWnd, IDC_EFFECT);
			std::string error_string;
			pCurrentOpts.set_value(OPTION_EFFECT, buff, OPTION_PRIORITY_CMDLINE,error_string);
			win_set_window_text_utf8(control, buff);
			// datamap_populate_control(properties_datamap, hWnd, pCurrentOpts, IDC_EFFECT);
			changed = TRUE;
		}
	}
	return changed;
}

static BOOL ResetEffect(HWND hWnd)
{
	BOOL changed = FALSE;
	const char *new_value = "none";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_EFFECT))!=0)
	{
		HWND control = GetDlgItem(hWnd, IDC_EFFECT);
		std::string error_string;
		pCurrentOpts.set_value(OPTION_EFFECT, new_value, OPTION_PRIORITY_CMDLINE,error_string);
		win_set_window_text_utf8(control, new_value);
		// datamap_populate_control(properties_datamap, hWnd, pCurrentOpts, IDC_EFFECT);
		changed = TRUE;
	}
	return changed;
}

static BOOL SelectJoystickMap(HWND hWnd)
{
	char filename[MAX_PATH];
	BOOL changed = FALSE;

	*filename = 0;
	if (CommonFileDialog(GetOpenFileName, filename, FILETYPE_JOYMAP_FILES))
	{
		if (strcmp(filename, pCurrentOpts.value(OPTION_JOYSTICK_MAP))!=0)
		{
			HWND control = GetDlgItem(hWnd, IDC_JOYSTICKMAP);
			std::string error_string;
			pCurrentOpts.set_value(OPTION_JOYSTICK_MAP, filename, OPTION_PRIORITY_CMDLINE,error_string);
			win_set_window_text_utf8(control, filename);
			changed = TRUE;
		}
	}
	return changed;
}

static BOOL ResetJoystickMap(HWND hWnd)
{
	BOOL changed = FALSE;
	const char *new_value = "auto";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_JOYSTICK_MAP))!=0)
	{
		HWND control = GetDlgItem(hWnd, IDC_JOYSTICKMAP);
		std::string error_string;
		pCurrentOpts.set_value(OPTION_JOYSTICK_MAP, new_value, OPTION_PRIORITY_CMDLINE,error_string);
		win_set_window_text_utf8(control, new_value);
		changed = TRUE;
	}
	return changed;
}

static BOOL SelectDebugscript(HWND hWnd)
{
	char filename[MAX_PATH];
	BOOL changed = FALSE;

	*filename = 0;
	if (CommonFileDialog(GetOpenFileName, filename, FILETYPE_DEBUGSCRIPT_FILES))
	{
		if (strcmp(filename, pCurrentOpts.value(OPTION_DEBUGSCRIPT))!=0)
		{
			HWND control = GetDlgItem(hWnd, IDC_DEBUGSCRIPT);
			std::string error_string;
			pCurrentOpts.set_value(OPTION_DEBUGSCRIPT, filename, OPTION_PRIORITY_CMDLINE,error_string);
			win_set_window_text_utf8(control, filename);
			changed = TRUE;
		}
	}
	return changed;
}

static BOOL ResetDebugscript(HWND hWnd)
{
	BOOL changed = FALSE;
	const char *new_value = "";

	if (strcmp(new_value, pCurrentOpts.value(OPTION_DEBUGSCRIPT))!=0)
	{
		HWND control = GetDlgItem(hWnd, IDC_DEBUGSCRIPT);
		std::string error_string;
		pCurrentOpts.set_value(OPTION_DEBUGSCRIPT, new_value, OPTION_PRIORITY_CMDLINE,error_string);
		win_set_window_text_utf8(control, new_value);
		changed = TRUE;
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
		RECT rc;
		HDC hDC, hDCMem;
		HBITMAP hBmp, hBmpOld;
		// Get tab control dimensions
		GetWindowRect( hwndTab, &rc);

		// Get the tab control DC
		hDC = GetDC(hwndTab);

		// Create a compatible DC
		hDCMem = CreateCompatibleDC(hDC);
		hBmp = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
		hBmpOld = (HBITMAP)(SelectObject(hDCMem, hBmp));

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


/* End of source file */
