// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

 /***************************************************************************

  mui_opts.cpp

  Stores global options and per-game options;

***************************************************************************/

// standard windows headers
#include <windows.h>
#include <windowsx.h>

// standard C headers
#include <tchar.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "ui/moptions.h"
#include "ui/info.h"
#include "drivenum.h"
#include "winui.h"
#include "mui_opts.h"
#include <fstream>      // for *_opts.h (below)
#include "game_opts.h"  // this must be under emu.h and drivenum.h
#include "ui_opts.h"
//#include "ini_opts.h"   // not ready yet
#include "mui_util.h"
#include "treeview.h"
#include "splitters.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

/***************************************************************************
    Internal function prototypes
 ***************************************************************************/

// static void LoadFolderFilter(int folder_index,int filters);

static void LoadSettingsFile(windows_options &opts, const char *filename);
static void SaveSettingsFile(windows_options &opts, const char *filename);
static void LoadSettingsFile(ui_options &opts, const char *filename); // mewui

static string CusColorEncodeString(const COLORREF *value);
static void CusColorDecodeString(string ss, COLORREF *value);

static string SplitterEncodeString(const int *value);
static void SplitterDecodeString(string ss, int *value);

static string FontEncodeString(const LOGFONT *f);
static void FontDecodeString(string ss, LOGFONT *f);

static string TabFlagsEncodeString(int data);
static void TabFlagsDecodeString(string ss, int *data);

static DWORD DecodeFolderFlags(string ss);
static const char * EncodeFolderFlags(DWORD value);

static string ColumnEncodeStringWithCount(const int *value, int count);
static void ColumnDecodeStringWithCount(string ss, int *value, int count);

static void ResetToDefaults(windows_options &opts, int priority);



/***************************************************************************
    Internal defines
 ***************************************************************************/

#define GAMEINFO_INI_FILENAME                    MAMENAME "_g.ini"
#define MEWUI_FILENAME                           "ini\\ui.ini"



/***************************************************************************
    Internal structures
 ***************************************************************************/

 /***************************************************************************
    Internal variables
 ***************************************************************************/

static emu_options mameopts; // core options
static ui_options mewui; // ui.ini
static winui_ui_options settings; // mameui.ini
static windows_options global; // Global 'default' options
static winui_game_options game_opts;    // game stats


#if 0
// no longer used, but keep in case we need to add more per-game options in the future
static const options_entry perGameOptions[] =
{
	// per game options in messui.ini - transferred to swpath
	{ "_extra_software",         "",         OPTION_STRING,  NULL },
	{ NULL }
};
#endif

static const options_entry filterOptions[] =
{
	// filters
	{ "_filters",                "0",        OPTION_INTEGER,                 NULL },
	{ NULL }
};



// Screen shot Page tab control text
// these must match the order of the options flags in options.h
// (TAB_...)
static const char *const image_tabs_long_name[MAX_TAB_TYPES] =
{
	"Snapshot",
	"Flyer",
	"Cabinet",
	"Marquee",
	"Title",
	"Control Panel",
	"PCB",
	"History",
};

static const char *const image_tabs_short_name[MAX_TAB_TYPES] =
{
	"snapshot",
	"flyer",
	"cabinet",
	"marquee",
	"title",
	"cpanel",
	"pcb",
	"history"
};


/***************************************************************************
    External functions
 ***************************************************************************/
void SetSystemName(windows_options &opts, OPTIONS_TYPE opt_type, int driver_index)
{
	if (driver_index >= 0)
		mameopts.set_system_name(driver_list::driver(driver_index).name);
}

BOOL OptionsInit()
{
	// set up global options
	settings.load_file(UI_INI_FILENAME);                    // parse MAMEUI.ini
	LoadSettingsFile(mewui, MEWUI_FILENAME);                // parse UI.INI
	game_opts.load_file(GAMEINFO_INI_FILENAME);             // parse MAME_g.ini
	load_options(global, OPTIONS_GLOBAL, GLOBAL_OPTIONS, 0);   // parse MAME.INI
	return TRUE;
}

windows_options & MameUIGlobal(void)
{
	return global;
}

// Restore ui settings to factory
void ResetGUI(void)
{
	settings.reset_and_save(UI_INI_FILENAME);
}

const char * GetImageTabLongName(int tab_index)
{
	assert(tab_index >= 0);
	assert(tab_index < ARRAY_LENGTH(image_tabs_long_name));
	return image_tabs_long_name[tab_index];
}

const char * GetImageTabShortName(int tab_index)
{
	assert(tab_index >= 0);
	assert(tab_index < ARRAY_LENGTH(image_tabs_short_name));
	return image_tabs_short_name[tab_index];
}

//============================================================
//  OPTIONS WRAPPERS
//============================================================

static COLORREF options_get_color(const char *name)
{
	const char *value_str;
	unsigned int r = 0, g = 0, b = 0;
	COLORREF value;

	value_str = settings.getter(name).c_str();

	if (sscanf(value_str, "%u,%u,%u", &r, &g, &b) == 3)
		value = RGB(r,g,b);
	else
		value = (COLORREF) - 1;
	return value;
}

static void options_set_color(const char *name, COLORREF value)
{
	char value_str[32];

	if (value == (COLORREF) -1)
		snprintf(value_str, ARRAY_LENGTH(value_str), "%d", (int) value);
	else
		snprintf(value_str, ARRAY_LENGTH(value_str), "%d,%d,%d", (((int) value) >>  0) & 0xFF, (((int) value) >>  8) & 0xFF, (((int) value) >> 16) & 0xFF);

	settings.setter(name, string(value_str));
}

static COLORREF options_get_color_default(const char *name, int default_color)
{
	COLORREF value = options_get_color(name);
	if (value == (COLORREF) -1)
		value = GetSysColor(default_color);

	return value;
}

static void options_set_color_default(const char *name, COLORREF value, int default_color)
{
	if (value == GetSysColor(default_color))
		options_set_color(name, (COLORREF) -1);
	else
		options_set_color(name, value);
}

static input_seq *options_get_input_seq(const char *name)
{
/*  static input_seq seq;
	const char *seq_string;

	seq_string = settings.getter(name).c_str();
	input_seq_from_tokens(NULL, seq_string, &seq);  // HACK
	return &seq;*/
	return NULL;
}



//============================================================
//  OPTIONS CALLS
//============================================================

void SetViewMode(int val)
{
	settings.setter(MUIOPTION_LIST_MODE, val);
}

int GetViewMode(void)
{
	return settings.int_value(MUIOPTION_LIST_MODE);
}

void SetGameCheck(BOOL game_check)
{
	settings.setter(MUIOPTION_CHECK_GAME, game_check);
}

BOOL GetGameCheck(void)
{
	return settings.bool_value(MUIOPTION_CHECK_GAME);
}

void SetJoyGUI(BOOL use_joygui)
{
	settings.setter(MUIOPTION_JOYSTICK_IN_INTERFACE, use_joygui);
}

BOOL GetJoyGUI(void)
{
	return settings.bool_value( MUIOPTION_JOYSTICK_IN_INTERFACE);
}

void SetKeyGUI(BOOL use_keygui)
{
	settings.setter(MUIOPTION_KEYBOARD_IN_INTERFACE, use_keygui);
}

BOOL GetKeyGUI(void)
{
	return settings.bool_value(MUIOPTION_KEYBOARD_IN_INTERFACE);
}

void SetCycleScreenshot(int cycle_screenshot)
{
	settings.setter(MUIOPTION_CYCLE_SCREENSHOT, cycle_screenshot);
}

int GetCycleScreenshot(void)
{
	return settings.int_value(MUIOPTION_CYCLE_SCREENSHOT);
}

void SetStretchScreenShotLarger(BOOL stretch)
{
	settings.setter(MUIOPTION_STRETCH_SCREENSHOT_LARGER, stretch);
}

BOOL GetStretchScreenShotLarger(void)
{
	return settings.bool_value(MUIOPTION_STRETCH_SCREENSHOT_LARGER);
}

void SetScreenshotBorderSize(int size)
{
	settings.setter(MUIOPTION_SCREENSHOT_BORDER_SIZE, size);
}

int GetScreenshotBorderSize(void)
{
	return settings.int_value(MUIOPTION_SCREENSHOT_BORDER_SIZE);
}

void SetScreenshotBorderColor(COLORREF uColor)
{
	options_set_color_default(MUIOPTION_SCREENSHOT_BORDER_COLOR, uColor, COLOR_3DFACE);
}

COLORREF GetScreenshotBorderColor(void)
{
	return options_get_color_default(MUIOPTION_SCREENSHOT_BORDER_COLOR, COLOR_3DFACE);
}

void SetFilterInherit(BOOL inherit)
{
	settings.setter(MUIOPTION_INHERIT_FILTER, inherit);
}

BOOL GetFilterInherit(void)
{
	return settings.bool_value( MUIOPTION_INHERIT_FILTER);
}

void SetOffsetClones(BOOL offset)
{
	settings.setter(MUIOPTION_OFFSET_CLONES, offset);
}

BOOL GetOffsetClones(void)
{
	return settings.bool_value( MUIOPTION_OFFSET_CLONES);
}

void SetSavedFolderID(UINT val)
{
	settings.setter(MUIOPTION_DEFAULT_FOLDER_ID, (int) val);
}

UINT GetSavedFolderID(void)
{
	return (UINT) settings.int_value(MUIOPTION_DEFAULT_FOLDER_ID);
}

void SetShowScreenShot(BOOL val)
{
	settings.setter(MUIOPTION_SHOW_IMAGE_SECTION, val);
}

BOOL GetShowScreenShot(void)
{
	return settings.bool_value(MUIOPTION_SHOW_IMAGE_SECTION);
}

void SetShowSoftware(BOOL val)
{
	settings.setter(MUIOPTION_SHOW_SOFTWARE_SECTION, val);
}

BOOL GetShowSoftware(void)
{
	return settings.bool_value(MUIOPTION_SHOW_SOFTWARE_SECTION);
}

void SetShowFolderList(BOOL val)
{
	settings.setter(MUIOPTION_SHOW_FOLDER_SECTION, val);
}

BOOL GetShowFolderList(void)
{
	return settings.bool_value(MUIOPTION_SHOW_FOLDER_SECTION);
}

void SetShowExtraFolders(BOOL val)
{
	settings.setter(MUIOPTION_EXTRA_FOLDERS, val);
}

BOOL GetShowExtraFolders(void)
{
	return settings.bool_value(MUIOPTION_EXTRA_FOLDERS);
}

static void GetsShowFolderFlags(LPBITS bits)
{
	char s[2000];
	extern const FOLDERDATA g_folderData[];
	char *token;

	snprintf(s, ARRAY_LENGTH(s), "%s", settings.getter(MUIOPTION_HIDE_FOLDERS).c_str());

	SetAllBits(bits, TRUE);

	token = strtok(s,",");
	int j;
	while (token)
	{
		for (j=0; g_folderData[j].m_lpTitle; j++)
		{
			if (strcmp(g_folderData[j].short_name,token) == 0)
			{
				ClearBit(bits, g_folderData[j].m_nFolderId);
				break;
			}
		}
		token = strtok(NULL,",");
	}
}

BOOL GetShowFolder(int folder)
{
	LPBITS show_folder_flags = NewBits(MAX_FOLDERS);
	GetsShowFolderFlags(show_folder_flags);
	BOOL result = TestBit(show_folder_flags, folder);
	DeleteBits(show_folder_flags);
	return result;
}

void SetShowFolder(int folder,BOOL show)
{
	LPBITS show_folder_flags = NewBits(MAX_FOLDERS);
	int i = 0, j = 0;
	int num_saved = 0;
	string str;
	extern const FOLDERDATA g_folderData[];

	GetsShowFolderFlags(show_folder_flags);

	if (show)
		SetBit(show_folder_flags, folder);
	else
		ClearBit(show_folder_flags, folder);

	// we save the ones that are NOT displayed, so we can add new ones
	// and upgraders will see them
	for (i=0; i<MAX_FOLDERS; i++)
	{
		if (TestBit(show_folder_flags, i) == FALSE)
		{
			if (num_saved != 0)
				str.append(",");

			for (j=0; g_folderData[j].m_lpTitle; j++)
			{
				if (g_folderData[j].m_nFolderId == i)
				{
					str.append(g_folderData[j].short_name);
					num_saved++;
					break;
				}
			}
		}
	}
	settings.setter(MUIOPTION_HIDE_FOLDERS, str);
	DeleteBits(show_folder_flags);
}

void SetShowStatusBar(BOOL val)
{
	settings.setter(MUIOPTION_SHOW_STATUS_BAR, val);
}

BOOL GetShowStatusBar(void)
{
	return settings.bool_value( MUIOPTION_SHOW_STATUS_BAR);
}

void SetShowTabCtrl (BOOL val)
{
	settings.setter(MUIOPTION_SHOW_TABS, val);
}

BOOL GetShowTabCtrl (void)
{
	return settings.bool_value( MUIOPTION_SHOW_TABS);
}

void SetShowToolBar(BOOL val)
{
	settings.setter(MUIOPTION_SHOW_TOOLBAR, val);
}

BOOL GetShowToolBar(void)
{
	return settings.bool_value( MUIOPTION_SHOW_TOOLBAR);
}

void SetCurrentTab(const char *shortname)
{
	settings.setter(MUIOPTION_CURRENT_TAB, shortname);
}

const char *GetCurrentTab(void)
{
	return settings.getter(MUIOPTION_CURRENT_TAB).c_str();
}

void SetDefaultGame(const char *name)
{
	settings.setter(MUIOPTION_DEFAULT_GAME, name);
}

const char *GetDefaultGame(void)
{
	return settings.getter(MUIOPTION_DEFAULT_GAME).c_str();
}

void SetWindowArea(const AREA *area)
{
	settings.setter(MUIOPTION_WINDOW_X, area->x);
	settings.setter(MUIOPTION_WINDOW_Y, area->y);
	settings.setter(MUIOPTION_WINDOW_WIDTH, area->width);
	settings.setter(MUIOPTION_WINDOW_HEIGHT, area->height);
}

void GetWindowArea(AREA *area)
{
	area->x = settings.int_value(MUIOPTION_WINDOW_X);
	area->y = settings.int_value(MUIOPTION_WINDOW_Y);
	area->width  = settings.int_value(MUIOPTION_WINDOW_WIDTH);
	area->height = settings.int_value(MUIOPTION_WINDOW_HEIGHT);
}

void SetWindowState(UINT state)
{
	settings.setter(MUIOPTION_WINDOW_STATE, (int)state);
}

UINT GetWindowState(void)
{
	return settings.int_value(MUIOPTION_WINDOW_STATE);
}

void SetCustomColor(int iIndex, COLORREF uColor)
{
	if ((iIndex < 0) || (iIndex > 15))
		return;

	COLORREF custom_color[16];
	CusColorDecodeString(settings.getter(MUIOPTION_CUSTOM_COLOR), custom_color);
	custom_color[iIndex] = uColor;
	settings.setter(MUIOPTION_CUSTOM_COLOR, CusColorEncodeString(custom_color));
}

COLORREF GetCustomColor(int iIndex)
{
	if ((iIndex < 0) || (iIndex > 15))
		return (COLORREF)RGB(0,0,0);

	COLORREF custom_color[16];

	CusColorDecodeString(settings.getter(MUIOPTION_CUSTOM_COLOR), custom_color);

	if (custom_color[iIndex] == (COLORREF)-1)
		return (COLORREF)RGB(0,0,0);

	return custom_color[iIndex];
}

void SetListFont(const LOGFONT *font)
{
	settings.setter(MUIOPTION_LIST_FONT, FontEncodeString(font));
}

void GetListFont(LOGFONT *font)
{
	FontDecodeString(settings.getter(MUIOPTION_LIST_FONT), font);
}

void SetListFontColor(COLORREF uColor)
{
	options_set_color_default(MUIOPTION_TEXT_COLOR, uColor, COLOR_WINDOWTEXT);
}

COLORREF GetListFontColor(void)
{
	return options_get_color_default(MUIOPTION_TEXT_COLOR, COLOR_WINDOWTEXT);
}

void SetListCloneColor(COLORREF uColor)
{
	options_set_color_default(MUIOPTION_CLONE_COLOR, uColor, COLOR_WINDOWTEXT);
}

COLORREF GetListCloneColor(void)
{
	return options_get_color_default(MUIOPTION_CLONE_COLOR, COLOR_WINDOWTEXT);
}

int GetShowTab(int tab)
{
	int show_tab_flags = 0;
	TabFlagsDecodeString(settings.getter(MUIOPTION_HIDE_TABS), &show_tab_flags);
	return (show_tab_flags & (1 << tab)) != 0;
}

void SetShowTab(int tab,BOOL show)
{
	int show_tab_flags = 0;
	TabFlagsDecodeString(settings.getter(MUIOPTION_HIDE_TABS), &show_tab_flags);

	if (show)
		show_tab_flags |= 1 << tab;
	else
		show_tab_flags &= ~(1 << tab);

	settings.setter(MUIOPTION_HIDE_TABS, TabFlagsEncodeString(show_tab_flags));
}

// don't delete the last one
BOOL AllowedToSetShowTab(int tab,BOOL show)
{
	int show_tab_flags = 0;

	if (show == TRUE)
		return TRUE;

	TabFlagsDecodeString(settings.getter(MUIOPTION_HIDE_TABS), &show_tab_flags);

	show_tab_flags &= ~(1 << tab);
	return show_tab_flags != 0;
}

int GetHistoryTab(void)
{
	return settings.int_value(MUIOPTION_HISTORY_TAB);
}

void SetHistoryTab(int tab, BOOL show)
{
	if (show)
		settings.setter(MUIOPTION_HISTORY_TAB, tab);
	else
		settings.setter(MUIOPTION_HISTORY_TAB, TAB_NONE);
}

void SetColumnWidths(int width[])
{
	settings.setter(MUIOPTION_COLUMN_WIDTHS, ColumnEncodeStringWithCount(width, COLUMN_MAX));
}

void GetColumnWidths(int width[])
{
	ColumnDecodeStringWithCount(settings.getter(MUIOPTION_COLUMN_WIDTHS), width, COLUMN_MAX);
}

void SetSplitterPos(int splitterId, int pos)
{
	int *splitter;

	if (splitterId < GetSplitterCount())
	{
		splitter = (int *) alloca(GetSplitterCount() * sizeof(*splitter));
		SplitterDecodeString(settings.getter(MUIOPTION_SPLITTERS), splitter);
		splitter[splitterId] = pos;
		settings.setter(MUIOPTION_SPLITTERS, SplitterEncodeString(splitter));
	}
}

int GetSplitterPos(int splitterId)
{
	int *splitter;
	splitter = (int *) alloca(GetSplitterCount() * sizeof(*splitter));
	SplitterDecodeString(settings.getter(MUIOPTION_SPLITTERS), splitter);

	if (splitterId < GetSplitterCount())
		return splitter[splitterId];

	return -1; /* Error */
}

void SetColumnOrder(int order[])
{
	settings.setter(MUIOPTION_COLUMN_ORDER, ColumnEncodeStringWithCount(order, COLUMN_MAX));
}

void GetColumnOrder(int order[])
{
	ColumnDecodeStringWithCount(settings.getter(MUIOPTION_COLUMN_ORDER), order, COLUMN_MAX);
}

void SetColumnShown(int shown[])
{
	settings.setter(MUIOPTION_COLUMN_SHOWN, ColumnEncodeStringWithCount(shown, COLUMN_MAX));
}

void GetColumnShown(int shown[])
{
	ColumnDecodeStringWithCount(settings.getter(MUIOPTION_COLUMN_SHOWN), shown, COLUMN_MAX);
}

void SetSortColumn(int column)
{
	settings.setter(MUIOPTION_SORT_COLUMN, column);
}

int GetSortColumn(void)
{
	return settings.int_value(MUIOPTION_SORT_COLUMN);
}

void SetSortReverse(BOOL reverse)
{
	settings.setter(MUIOPTION_SORT_REVERSED, reverse);
}

BOOL GetSortReverse(void)
{
	return settings.bool_value( MUIOPTION_SORT_REVERSED);
}

const char* GetLanguageUI(void)
{
	return global.value(OPTION_LANGUAGE);
}

bool GetEnablePlugins(void)
{
	return global.bool_value(OPTION_PLUGINS);
}

const char* GetPlugins(void)
{
	return global.value(OPTION_PLUGIN);
}

const string GetRomDirs(void)
{
	return string(global.media_path());
}

void SetRomDirs(const char* paths)
{
	global.set_value(OPTION_MEDIAPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetHashDirs(void)
{
	return string(global.hash_path());
}

void SetHashDirs(const char* paths)
{
	global.set_value(OPTION_HASHPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetSampleDirs(void)
{
	return string(global.value(OPTION_SAMPLEPATH));
}

void SetSampleDirs(const char* paths)
{
	global.set_value(OPTION_SAMPLEPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const char * GetIniDir(void)
{
	const char *ini_dir;
//	const char *s;

//	ini_dir = global.value(OPTION_INIPATH);
//	while((s = strchr(ini_dir, ';')) != NULL)
//	{
//		ini_dir = s + 1;
//	}
	ini_dir = "ini\0";
	return ini_dir;

}

void SetIniDir(const char *path)
{
	global.set_value(OPTION_INIPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCtrlrDir(void)
{
	return global.value(OPTION_CTRLRPATH);
}

void SetCtrlrDir(const char* path)
{
	global.set_value(OPTION_CTRLRPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetSWDir(void)
{
	const char* t = global.value(OPTION_SWPATH);
	if (t)
		return string(global.value(OPTION_SWPATH));
	else
		return "";
}

void SetSWDir(const char* path)
{
	global.set_value(OPTION_SWPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCfgDir(void)
{
	return string(global.value(OPTION_CFG_DIRECTORY));
}

void SetCfgDir(const char* path)
{
	global.set_value(OPTION_CFG_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetNvramDir(void)
{
	return string(global.value(OPTION_NVRAM_DIRECTORY));
}

void SetNvramDir(const char* path)
{
	global.set_value(OPTION_NVRAM_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetInpDir(void)
{
	return string(global.value(OPTION_INPUT_DIRECTORY));
}

void SetInpDir(const char* path)
{
	global.set_value(OPTION_INPUT_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetImgDir(void)
{
	return string(global.value(OPTION_SNAPSHOT_DIRECTORY));
}

void SetImgDir(const char* path)
{
	global.set_value(OPTION_SNAPSHOT_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetStateDir(void)
{
	return string(global.value(OPTION_STATE_DIRECTORY));
}

void SetStateDir(const char* path)
{
	global.set_value(OPTION_STATE_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetArtDir(void)
{
	return string(global.value(OPTION_ARTPATH));
}

void SetArtDir(const char* path)
{
	global.set_value(OPTION_ARTPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetFontDir(void)
{
	return string(global.value(OPTION_FONTPATH));
}

void SetFontDir(const char* paths)
{
	global.set_value(OPTION_FONTPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetCrosshairDir(void)
{
	return string(global.value(OPTION_CROSSHAIRPATH));
}

void SetCrosshairDir(const char* paths)
{
	global.set_value(OPTION_CROSSHAIRPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetFlyerDir(void)
{
	return string(mewui.value(OPTION_FLYERS_PATH));
}

void SetFlyerDir(const char* path)
{
	mewui.set_value(OPTION_FLYERS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCabinetDir(void)
{
	return string(mewui.value(OPTION_CABINETS_PATH));
}

void SetCabinetDir(const char* path)
{
	mewui.set_value(OPTION_CABINETS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetMarqueeDir(void)
{
	return string(mewui.value(OPTION_MARQUEES_PATH));
}

void SetMarqueeDir(const char* path)
{
	mewui.set_value(OPTION_MARQUEES_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetTitlesDir(void)
{
	return string(mewui.value(OPTION_TITLES_PATH));
}

void SetTitlesDir(const char* path)
{
	mewui.set_value(OPTION_TITLES_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetControlPanelDir(void)
{
	return string(mewui.value(OPTION_CPANELS_PATH));
}

void SetControlPanelDir(const char *path)
{
	mewui.set_value(OPTION_CPANELS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetPcbDir(void)
{
	return string(mewui.value(OPTION_PCBS_PATH));
}

void SetPcbDir(const char *path)
{
	mewui.set_value(OPTION_PCBS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetPluginsDir(void)
{
	return string(global.value(OPTION_PLUGINSPATH));
}

void SetPluginsDir(const char* path)
{
	global.set_value(OPTION_PLUGINSPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetLangDir(void)
{
	return string(global.value(OPTION_LANGUAGEPATH));
}

void SetLangDir(const char* path)
{
	global.set_value(OPTION_LANGUAGEPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetDiffDir(void)
{
	return string(global.value(OPTION_DIFF_DIRECTORY));
}

void SetDiffDir(const char* path)
{
	global.set_value(OPTION_DIFF_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetIconsDir(void)
{
	return settings.getter(MUIOPTION_ICONS_DIRECTORY);
}

void SetIconsDir(const char* path)
{
	settings.setter(MUIOPTION_ICONS_DIRECTORY, path);
}

const string GetBgDir (void)
{
	return settings.getter(MUIOPTION_BACKGROUND_DIRECTORY);
}

void SetBgDir (const char* path)
{
	settings.setter(MUIOPTION_BACKGROUND_DIRECTORY, path);
}

const string GetDatsDir(void)
{
	return settings.getter(MUIOPTION_DATS_DIRECTORY);
	//return mewui.value(OPTION_HISTORY_PATH);
}

void SetDatsDir(const char *path)
{
	char t1[strlen(path)+1];
	strcpy(t1, path);
	settings.setter(MUIOPTION_DATS_DIRECTORY, path);
	mewui.set_value(OPTION_HISTORY_PATH, t1, OPTION_PRIORITY_CMDLINE);
}

const string GetFolderDir(void)
{
	return string(mewui.value(OPTION_CATEGORYINI_PATH));
}

void SetFolderDir(const char* path)
{
	mewui.set_value(OPTION_CATEGORYINI_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCheatDir(void)
{
	return string(global.value(OPTION_CHEATPATH));
}

void SetCheatDir(const char* path)
{
	global.set_value(OPTION_CHEATPATH, path, OPTION_PRIORITY_CMDLINE);
}

const char* GetSnapName(void)
{
	return global.value(OPTION_SNAPNAME);
}

void SetSnapName(const char* pattern)
{
	global.set_value(OPTION_SNAPNAME, pattern, OPTION_PRIORITY_CMDLINE);
}

void ResetGameOptions(int driver_index)
{
	assert(0 <= driver_index && driver_index < driver_list::total());

	//save_options(NULL, OPTIONS_GAME, driver_index);
}

void ResetGameDefaults(void)
{
	// Walk the global settings and reset everything to defaults;
	ResetToDefaults(global, OPTION_PRIORITY_CMDLINE);
	save_options(global, OPTIONS_GLOBAL, GLOBAL_OPTIONS);
}

/*
 * Reset all game, vector and source options to defaults.
 * No reason to reboot if this is done.
 */
void ResetAllGameOptions(void)
{
	for (int i = 0; i < driver_list::total(); i++)
		ResetGameOptions(i);
}

int GetRomAuditResults(int driver_index)
{
	return game_opts.rom(driver_index);
}

void SetRomAuditResults(int driver_index, int audit_results)
{
	game_opts.rom(driver_index, audit_results);
}

int  GetSampleAuditResults(int driver_index)
{
	return game_opts.sample(driver_index);
}

void SetSampleAuditResults(int driver_index, int audit_results)
{
	game_opts.sample(driver_index, audit_results);
}

static void IncrementPlayVariable(int driver_index, const char *play_variable, int increment)
{
	int count = 0;

	if (strcmp(play_variable, "count") == 0)
	{
		count = game_opts.play_count(driver_index);
		game_opts.play_count(driver_index, count + increment);
	}
	else if (strcmp(play_variable, "time") == 0)
	{
		count = game_opts.play_time(driver_index);
		game_opts.play_time(driver_index, count + increment);
	}
}

void IncrementPlayCount(int driver_index)
{
	IncrementPlayVariable(driver_index, "count", 1);
}

int GetPlayCount(int driver_index)
{
	return game_opts.play_count(driver_index);
}

static void ResetPlayVariable(int driver_index, const char *play_variable)
{
	if (driver_index < 0)
		/* all games */
		for (int i = 0; i< driver_list::total(); i++)
			ResetPlayVariable(i, play_variable);
	else
	{
		if (strcmp(play_variable, "count") == 0)
			game_opts.play_count(driver_index, 0);
		else
		if (strcmp(play_variable, "time") == 0)
			game_opts.play_time(driver_index, 0);
	}
}

void ResetPlayCount(int driver_index)
{
	ResetPlayVariable(driver_index, "count");
}

void ResetPlayTime(int driver_index)
{
	ResetPlayVariable(driver_index, "time");
}

int GetPlayTime(int driver_index)
{
	return game_opts.play_time(driver_index);
}

void IncrementPlayTime(int driver_index,int playtime)
{
	IncrementPlayVariable(driver_index, "time", playtime);
}

void GetTextPlayTime(int driver_index, char *buf)
{

	if (0 <= driver_index && driver_index < driver_list::total())
	{
		int second = GetPlayTime(driver_index);
		int hour = second / 3600;
		second -= 3600*hour;
		int minute = second / 60; //Calc Minutes
		second -= 60*minute;

		if (hour == 0)
			sprintf(buf, "%d:%02d", minute, second );
		else
			sprintf(buf, "%d:%02d:%02d", hour, minute, second );
	}
}

input_seq* Get_ui_key_up(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_UP);
}

input_seq* Get_ui_key_down(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_DOWN);
}

input_seq* Get_ui_key_left(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_LEFT);
}

input_seq* Get_ui_key_right(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_RIGHT);
}

input_seq* Get_ui_key_start(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_START);
}

input_seq* Get_ui_key_pgup(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_PGUP);
}

input_seq* Get_ui_key_pgdwn(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_PGDWN);
}

input_seq* Get_ui_key_home(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_HOME);
}

input_seq* Get_ui_key_end(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_END);
}

input_seq* Get_ui_key_ss_change(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_SS_CHANGE);
}

input_seq* Get_ui_key_history_up(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_HISTORY_UP);
}

input_seq* Get_ui_key_history_down(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_HISTORY_DOWN);
}

input_seq* Get_ui_key_context_filters(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_CONTEXT_FILTERS);
}

input_seq* Get_ui_key_select_random(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_SELECT_RANDOM);
}

input_seq* Get_ui_key_game_audit(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_GAME_AUDIT);
}

input_seq* Get_ui_key_game_properties(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_GAME_PROPERTIES);
}

input_seq* Get_ui_key_help_contents(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_HELP_CONTENTS);
}

input_seq* Get_ui_key_update_gamelist(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_UPDATE_GAMELIST);
}

input_seq* Get_ui_key_view_folders(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_FOLDERS);
}

input_seq* Get_ui_key_view_fullscreen(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_FULLSCREEN);
}

input_seq* Get_ui_key_view_pagetab(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_PAGETAB);
}

input_seq* Get_ui_key_view_picture_area(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_PICTURE_AREA);
}

input_seq* Get_ui_key_view_software_area(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_SOFTWARE_AREA);
}

input_seq* Get_ui_key_view_status(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_STATUS);
}

input_seq* Get_ui_key_view_toolbars(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TOOLBARS);
}

input_seq* Get_ui_key_view_tab_cabinet(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_CABINET);
}

input_seq* Get_ui_key_view_tab_cpanel(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_CPANEL);
}

input_seq* Get_ui_key_view_tab_flyer(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_FLYER);
}

input_seq* Get_ui_key_view_tab_history(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_HISTORY);
}

input_seq* Get_ui_key_view_tab_marquee(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_MARQUEE);
}

input_seq* Get_ui_key_view_tab_screenshot(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_SCREENSHOT);
}

input_seq* Get_ui_key_view_tab_title(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_TITLE);
}

input_seq* Get_ui_key_view_tab_pcb(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_VIEW_TAB_PCB);
}

input_seq* Get_ui_key_quit(void)
{
	return options_get_input_seq(MUIOPTION_UI_KEY_QUIT);
}

static int GetUIJoy(const char *option_name, int joycodeIndex)
{
	int joycodes[4];

	if ((joycodeIndex < 0) || (joycodeIndex > 3))
		joycodeIndex = 0;
	ColumnDecodeStringWithCount(settings.getter(option_name), joycodes, ARRAY_LENGTH(joycodes));
	return joycodes[joycodeIndex];
}

static void SetUIJoy(const char *option_name, int joycodeIndex, int val)
{
	int joycodes[4];

	if ((joycodeIndex < 0) || (joycodeIndex > 3))
		joycodeIndex = 0;
	ColumnDecodeStringWithCount(settings.getter(option_name), joycodes, ARRAY_LENGTH(joycodes));
	joycodes[joycodeIndex] = val;
	settings.setter(option_name, ColumnEncodeStringWithCount(joycodes, ARRAY_LENGTH(joycodes)));
}

int GetUIJoyUp(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_UP, joycodeIndex);
}

void SetUIJoyUp(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_UP, joycodeIndex, val);
}

int GetUIJoyDown(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_DOWN, joycodeIndex);
}

void SetUIJoyDown(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_DOWN, joycodeIndex, val);
}

int GetUIJoyLeft(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_LEFT, joycodeIndex);
}

void SetUIJoyLeft(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_LEFT, joycodeIndex, val);
}

int GetUIJoyRight(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_RIGHT, joycodeIndex);
}

void SetUIJoyRight(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_RIGHT, joycodeIndex, val);
}

int GetUIJoyStart(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_START, joycodeIndex);
}

void SetUIJoyStart(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_START, joycodeIndex, val);
}

int GetUIJoyPageUp(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_PGUP, joycodeIndex);
}

void SetUIJoyPageUp(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_PGUP, joycodeIndex, val);
}

int GetUIJoyPageDown(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_PGDWN, joycodeIndex);
}

void SetUIJoyPageDown(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_PGDWN, joycodeIndex, val);
}

int GetUIJoyHome(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_HOME, joycodeIndex);
}

void SetUIJoyHome(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_HOME, joycodeIndex, val);
}

int GetUIJoyEnd(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_END, joycodeIndex);
}

void SetUIJoyEnd(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_END, joycodeIndex, val);
}

int GetUIJoySSChange(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_SS_CHANGE, joycodeIndex);
}

void SetUIJoySSChange(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_SS_CHANGE, joycodeIndex, val);
}

int GetUIJoyHistoryUp(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_HISTORY_UP, joycodeIndex);
}

void SetUIJoyHistoryUp(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_HISTORY_UP, joycodeIndex, val);
}

int GetUIJoyHistoryDown(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_HISTORY_DOWN, joycodeIndex);
}

void SetUIJoyHistoryDown(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_HISTORY_DOWN, joycodeIndex, val);
}

void SetUIJoyExec(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_EXEC, joycodeIndex, val);
}

int GetUIJoyExec(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_EXEC, joycodeIndex);
}

const char * GetExecCommand(void)
{
	return settings.getter(MUIOPTION_EXEC_COMMAND).c_str();
}

void SetExecCommand(char *cmd)
{
	settings.setter(MUIOPTION_EXEC_COMMAND, cmd);
}

int GetExecWait(void)
{
	return settings.int_value(MUIOPTION_EXEC_WAIT);
}

void SetExecWait(int wait)
{
	settings.setter(MUIOPTION_EXEC_WAIT, wait);
}

BOOL GetHideMouseOnStartup(void)
{
	return settings.bool_value( MUIOPTION_HIDE_MOUSE);
}

void SetHideMouseOnStartup(BOOL hide)
{
	settings.setter(MUIOPTION_HIDE_MOUSE, hide);
}

BOOL GetRunFullScreen(void)
{
	return settings.bool_value( MUIOPTION_FULL_SCREEN);
}

void SetRunFullScreen(BOOL fullScreen)
{
	settings.setter(MUIOPTION_FULL_SCREEN, fullScreen);
}

/***************************************************************************
    Internal functions
 ***************************************************************************/

static string CusColorEncodeString(const COLORREF *value)
{
	string str = std::to_string(value[0]);

	for (int i = 1; i < 16; i++)
		str.append(",").append(std::to_string(value[i]));

	return str;
}

static void CusColorDecodeString(string ss, COLORREF *value)
{
	const char *str = ss.c_str();
	char *s, *p;
	char tmpStr[256];

	strcpy(tmpStr, str);
	p = tmpStr;

	for (int i = 0; p && i < 16; i++)
	{
		s = p;

		if ((p = strchr(s,',')) != NULL && *p == ',')
		{
			*p = '\0';
			p++;
		}
		value[i] = atoi(s);
	}
}


static string ColumnEncodeStringWithCount(const int *value, int count)
{
	string str = std::to_string(value[0]);

	for (int i = 1; i < count; i++)
		str.append(",").append(std::to_string(value[i]));

	return str;
}

static void ColumnDecodeStringWithCount(string ss, int *value, int count)
{
	const char *str = ss.c_str();
	char *s, *p;
	char tmpStr[256];

	if (str == NULL)
		return;

	strcpy(tmpStr, str);
	p = tmpStr;

	for (int i = 0; p && i < count; i++)
	{
		s = p;

		if ((p = strchr(s,',')) != NULL && *p == ',')
		{
			*p = '\0';
			p++;
		}
		value[i] = atoi(s);
	}
}

static string SplitterEncodeString(const int *value)
{
	string str = std::to_string(value[0]);

	for (int i = 1; i < GetSplitterCount(); i++)
		str.append(",").append(std::to_string(value[i]));

	return str;
}

static void SplitterDecodeString(string ss, int *value)
{
	const char *str = ss.c_str();
	char *s, *p;
	char tmpStr[256];

	strcpy(tmpStr, str);
	p = tmpStr;

	for (int i = 0; p && i < GetSplitterCount(); i++)
	{
		s = p;

		if ((p = strchr(s,',')) != NULL && *p == ',')
		{
			*p = '\0';
			p++;
		}
		value[i] = atoi(s);
	}
}

/* Parse the given comma-delimited string into a LOGFONT structure */
static void FontDecodeString(string ss, LOGFONT *f)
{
	const char* str = ss.c_str();
	sscanf(str, "%li,%li,%li,%li,%li,%i,%i,%i,%i,%i,%i,%i,%i",
		&f->lfHeight,
		&f->lfWidth,
		&f->lfEscapement,
		&f->lfOrientation,
		&f->lfWeight,
		(int*)&f->lfItalic,
		(int*)&f->lfUnderline,
		(int*)&f->lfStrikeOut,
		(int*)&f->lfCharSet,
		(int*)&f->lfOutPrecision,
		(int*)&f->lfClipPrecision,
		(int*)&f->lfQuality,
		(int*)&f->lfPitchAndFamily);
	const char *ptr = strrchr(str, ',');
	if (ptr)
	{
		TCHAR *t_s = ui_wstring_from_utf8(ptr + 1);
		if( !t_s )
			return;
		_tcscpy(f->lfFaceName, t_s);
		free(t_s);
	}
}

/* Encode the given LOGFONT structure into a comma-delimited string */
static string FontEncodeString(const LOGFONT *f)
{
	char* utf8_FaceName = ui_utf8_from_wstring(f->lfFaceName);
	if( !utf8_FaceName )
		return "";

	char s[200];
	sprintf(s, "%li,%li,%li,%li,%li,%i,%i,%i,%i,%i,%i,%i,%i,%s",
			f->lfHeight,
			f->lfWidth,
			f->lfEscapement,
			f->lfOrientation,
			f->lfWeight,
			f->lfItalic,
			f->lfUnderline,
			f->lfStrikeOut,
			f->lfCharSet,
			f->lfOutPrecision,
			f->lfClipPrecision,
			f->lfQuality,
			f->lfPitchAndFamily,
			utf8_FaceName);

	free(utf8_FaceName);
	return string(s);
}

static string TabFlagsEncodeString(int data)
{
	int num_saved = 0;
	string str;

	// we save the ones that are NOT displayed, so we can add new ones
	// and upgraders will see them
	for ( int i=0; i<MAX_TAB_TYPES; i++)
	{
		if (((data & (1 << i)) == 0) && GetImageTabShortName(i))
		{
			if (num_saved > 0)
				str.append(",");

			str.append(GetImageTabShortName(i));
			num_saved++;
		}
	}
	return str;
}

static void TabFlagsDecodeString(string ss, int *data)
{
	const char *str = ss.c_str();
	int j = 0;
	char s[2000];
	char *token;

	snprintf(s, ARRAY_LENGTH(s), "%s", str);

	// simple way to set all tab bits "on"
	*data = (1 << MAX_TAB_TYPES) - 1;

	token = strtok(s,", \t");
	while (token)
	{
		for (j=0; j<MAX_TAB_TYPES; j++)
		{
			if (!GetImageTabShortName(j) || (strcmp(GetImageTabShortName(j), token) == 0))
			{
				// turn off this bit
				*data &= ~(1 << j);
				break;
			}
		}
		token = strtok(NULL,", \t");
	}

	if (*data == 0)
	{
		// not allowed to hide all tabs, because then why even show the area?
		*data = (1 << TAB_SCREENSHOT);
	}
}

// load mewui settings
static void LoadSettingsFile(ui_options &opts, const char *filename)
{
	osd_file::error filerr;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_READ, file);
	if (filerr == osd_file::error::NONE)
	{
		opts.parse_ini_file(*file, OPTION_PRIORITY_CMDLINE, true, true);
		file.reset();
	}
}

// load a game ini
static void LoadSettingsFile(windows_options &opts, const char *filename)
{
	osd_file::error filerr;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_READ, file);
	if (filerr == osd_file::error::NONE)
	{
		opts.parse_ini_file(*file, OPTION_PRIORITY_CMDLINE, true, true);
		file.reset();
	}
}

// This saves changes to UI.INI only
static void SaveSettingsFile(ui_options &opts, const char *filename)
{
	osd_file::error filerr = osd_file::error::NONE;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS, file);

	if (filerr == osd_file::error::NONE)
	{
		string inistring = opts.output_ini();
		file->puts(inistring.c_str());
		file.reset();
	}
}



// This saves changes to <game>.INI or MAME.INI only
static void SaveSettingsFile(windows_options &opts, const char *filename)
{
	osd_file::error filerr = osd_file::error::NONE;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS, file);

	if (filerr == osd_file::error::NONE)
	{
		string inistring = opts.output_ini();
		// printf("=====%s=====\n%s\n",filename,inistring.c_str());  // for debugging
		file->puts(inistring.c_str());
		file.reset();
	}
}



void SetDirectories(windows_options &opts)
{
	opts.set_value(OPTION_MEDIAPATH, GetRomDirs(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_SAMPLEPATH, GetSampleDirs(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_INIPATH, GetIniDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_CFG_DIRECTORY, GetCfgDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_SNAPSHOT_DIRECTORY, GetImgDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_INPUT_DIRECTORY, GetInpDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_STATE_DIRECTORY, GetStateDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_ARTPATH, GetArtDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_NVRAM_DIRECTORY, GetNvramDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_CTRLRPATH, GetCtrlrDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_CHEATPATH, GetCheatDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_CROSSHAIRPATH, GetCrosshairDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_FONTPATH, GetFontDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_DIFF_DIRECTORY, GetDiffDir(), OPTION_PRIORITY_CMDLINE);
	opts.set_value(OPTION_SNAPNAME, GetSnapName(), OPTION_PRIORITY_CMDLINE);
}

// not used
#if 0
const char * GetFolderNameByID(UINT nID)
{
	UINT i;
	extern const FOLDERDATA g_folderData[];
	extern LPEXFOLDERDATA ExtraFolderData[];

	for (i = 0; i < MAX_EXTRA_FOLDERS * MAX_EXTRA_SUBFOLDERS; i++)
		if( ExtraFolderData[i] )
			if (ExtraFolderData[i]->m_nFolderId == nID)
				return ExtraFolderData[i]->m_szTitle;

	for( i = 0; i < MAX_FOLDERS; i++)
		if (g_folderData[i].m_nFolderId == nID)
			return g_folderData[i].m_lpTitle;

	return NULL;
}
#endif

DWORD GetFolderFlags(int folder_index)
{
	LPTREEFOLDER lpFolder = GetFolder(folder_index);

	if (lpFolder)
		return lpFolder->m_dwFlags & F_MASK;

	return 0;
}


/* Decode the flags into a DWORD */
static DWORD DecodeFolderFlags(string ss)
{
	const char *buf = ss.c_str();
	DWORD flags = 0;
	int shift = 0;
	const char *ptr = buf;

	while (*ptr && (1 << shift) & F_MASK)
	{
		if (*ptr++ == '1')
		{
			flags |= (1 << shift);
		}
		shift++;
	}
	return flags;
}

/* Encode the flags into a string */
static const char * EncodeFolderFlags(DWORD value)
{
	static char buf[40];
	int shift = 0;

	memset(buf,'\0', sizeof(buf));

	while ((1 << shift) & F_MASK)
	{
		buf[shift] = (value & (1 << shift)) ? '1' : '0';
		shift++;
	}

	return buf;
}

/* MSH 20080813
 * Read the folder filters from MAMEui.ini.  This must only
 * be called AFTER the folders have all been created.
 */
void LoadFolderFlags(void)
{
	int i, numFolders = 0;
	LPTREEFOLDER lpFolder;
	options_entry entries[2] = { { 0 }, { 0 } };

	memcpy(entries, filterOptions, sizeof(filterOptions));

	numFolders = GetNumFolders();

	for (i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);

		if (lpFolder)
		{
			char folder_name[256];
			char *ptr;

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if ((*ptr == ' ') || (*ptr == '-'))
				{
					*ptr = '_';
				}
				ptr++;
			}

			string option_name = string(folder_name) + "_filters";
		}
	}

	// These are overlaid at the end of our UI ini
	// The normal read will skip them.

	// retrieve the stored values
	for (i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);

		if (lpFolder)
		{
			char folder_name[256];

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			char *ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if ((*ptr == ' ') || (*ptr == '-'))
				{
					*ptr = '_';
				}
				ptr++;
			}
			string option_name = string(folder_name) + "_filters";

			// get entry and decode it
			lpFolder->m_dwFlags |= DecodeFolderFlags(settings.getter(option_name.c_str())) & F_MASK;
		}
	}
}



// Adds our folder flags to winui_options, for saving.
static void AddFolderFlags()
{
	int numFolders = 0, num_entries = 0;
	LPTREEFOLDER lpFolder;
	//options_entry entries[2] = { { 0 }, { 0 } };

	//memcpy(entries, filterOptions, sizeof(filterOptions));

	numFolders = GetNumFolders();

	for (int i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);
		if (lpFolder && (lpFolder->m_dwFlags & F_MASK) != 0)
		{
			char folder_name[256];

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			char *ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if ((*ptr == ' ') || (*ptr == '-'))
				{
					*ptr = '_';
				}
				ptr++;
			}

			string option_name = string(folder_name) + "_filters";

			// store entry
			settings.setter(option_name.c_str(), EncodeFolderFlags(lpFolder->m_dwFlags));

			// increment counter
			num_entries++;
		}
	}
}

// Save MAMEUI.ini
void SaveOptions(void)
{
	// Add the folder flag to settings.
	AddFolderFlags();
	settings.save_file(UI_INI_FILENAME);
	SaveSettingsFile(mewui, MEWUI_FILENAME);
}

void SaveGameListOptions(void)
{
	// Save GameInfo.ini - game options.
	game_opts.save_file(GAMEINFO_INI_FILENAME);
}

void SaveDefaultOptions(void)
{
	string fname = string(GetIniDir()) + PATH_SEPARATOR + string(emulator_info::get_configname()).append(".ini");
	SaveSettingsFile(global, fname.c_str());
}

const char * GetVersionString(void)
{
	return emulator_info::get_build_version();
}


/*  get options, based on passed in game number. */
void load_options(windows_options &opts, OPTIONS_TYPE opt_type, int game_num, bool set_system_name)
{
	const game_driver *driver = NULL;
	if (game_num > -1)
		driver = &driver_list::driver(game_num);

	// Try base ini first
	string fname = string(emulator_info::get_configname()).append(".ini");
	LoadSettingsFile(opts, fname.c_str());

	if (opt_type == OPTIONS_SOURCE)
	{
		fname = string(GetIniDir()) + PATH_SEPARATOR + "source" + PATH_SEPARATOR + core_filename_extract_base(driver->type.source(), true) + ".ini";
		LoadSettingsFile(opts, fname.c_str());
		return;
	}

	if (game_num > -2)
	{
		// Now try global ini
		fname = string(GetIniDir()) + PATH_SEPARATOR + string(emulator_info::get_configname()).append(".ini");
		LoadSettingsFile(opts, fname.c_str());

		if (game_num > -1)
		{
			// Lastly, gamename.ini
			if (driver)
			{
				fname = string(GetIniDir()) + PATH_SEPARATOR + string(driver->name).append(".ini");
				if (set_system_name)
					opts.set_value(OPTION_SYSTEMNAME, driver->name, OPTION_PRIORITY_CMDLINE);
				LoadSettingsFile(opts, fname.c_str());
			}
		}
	}
	SetDirectories(opts);
}

/* Save ini file based on game_number. */
void save_options(windows_options &opts, OPTIONS_TYPE opt_type, int game_num)
{
	const game_driver *driver = NULL;
	string filename, filepath;

	if (game_num >= 0)
	{
		driver = &driver_list::driver(game_num);
		if (driver)
		{
			filename.assign(driver->name);
			if (opt_type == OPTIONS_SOURCE)
				filepath = string(GetIniDir()) + PATH_SEPARATOR + "source" + PATH_SEPARATOR + core_filename_extract_base(driver->type.source(), true) + ".ini";
		}
	}
	else
	if (game_num == -1)
		filename = string(emulator_info::get_configname());

	if (!filename.empty() && filepath.empty())
		filepath = string(GetIniDir()).append(PATH_SEPARATOR).append(filename.c_str()).append(".ini");

	if (game_num == -2)
		filepath = string(emulator_info::get_configname()).append(".ini");

	if (!filepath.empty())
	{
		SetDirectories(opts);
		SaveSettingsFile(opts, filepath.c_str());
//		printf("Settings saved to %s\n",filepath.c_str());
	}
//	else
//		printf("Unable to save settings\n");
}


// Reset the given windows_options to their default settings.
static void ResetToDefaults(windows_options &opts, int priority)
{
	// iterate through the options setting each one back to the default value.
	windows_options dummy;
	OptionsCopy(dummy, opts);
}

uint32_t GetDriverCacheLower(int driver_index)
{
	if (driver_index >= 0)
		return game_opts.cache_lower(driver_index);
	else
		return 0;
}

uint32_t GetDriverCacheUpper(int driver_index)
{
	if (driver_index >= 0)
		return game_opts.cache_upper(driver_index);
	else
		return 0;
}

void SetDriverCache(int driver_index, uint32_t val)
{
	if (driver_index >= 0)
		game_opts.cache_upper(driver_index, val);
}

BOOL RequiredDriverCache(void)
{
	return game_opts.rebuild();
}

// from optionsms.cpp (MESSUI)


#define LOG_SOFTWARE 1

void SetSLColumnOrder(int order[])
{
	settings.setter(MESSUI_SL_COLUMN_ORDER, ColumnEncodeStringWithCount(order, SL_COLUMN_MAX));
}

void GetSLColumnOrder(int order[])
{
	ColumnDecodeStringWithCount(settings.getter(MESSUI_SL_COLUMN_ORDER), order, SL_COLUMN_MAX);
}

void SetSLColumnShown(int shown[])
{
	settings.setter(MESSUI_SL_COLUMN_SHOWN, ColumnEncodeStringWithCount(shown, SL_COLUMN_MAX));
}

void GetSLColumnShown(int shown[])
{
	ColumnDecodeStringWithCount(settings.getter(MESSUI_SL_COLUMN_SHOWN), shown, SL_COLUMN_MAX);
}

void SetSLColumnWidths(int width[])
{
	settings.setter(MESSUI_SL_COLUMN_WIDTHS, ColumnEncodeStringWithCount(width, SL_COLUMN_MAX));
}

void GetSLColumnWidths(int width[])
{
	ColumnDecodeStringWithCount(settings.getter(MESSUI_SL_COLUMN_WIDTHS), width, SL_COLUMN_MAX);
}

void SetSLSortColumn(int column)
{
	settings.setter(MESSUI_SL_SORT_COLUMN, column);
}

int GetSLSortColumn(void)
{
	return settings.int_value(MESSUI_SL_SORT_COLUMN);
}

void SetSLSortReverse(BOOL reverse)
{
	settings.setter(MESSUI_SL_SORT_REVERSED, reverse);
}

BOOL GetSLSortReverse(void)
{
	return settings.bool_value(MESSUI_SL_SORT_REVERSED);
}

void SetSWColumnOrder(int order[])
{
	settings.setter(MESSUI_SW_COLUMN_ORDER, ColumnEncodeStringWithCount(order, SW_COLUMN_MAX));
}

void GetSWColumnOrder(int order[])
{
	ColumnDecodeStringWithCount(settings.getter(MESSUI_SW_COLUMN_ORDER), order, SW_COLUMN_MAX);
}

void SetSWColumnShown(int shown[])
{
	settings.setter(MESSUI_SW_COLUMN_SHOWN, ColumnEncodeStringWithCount(shown, SW_COLUMN_MAX));
}

void GetSWColumnShown(int shown[])
{
	ColumnDecodeStringWithCount(settings.getter(MESSUI_SW_COLUMN_SHOWN), shown, SW_COLUMN_MAX);
}

void SetSWColumnWidths(int width[])
{
	settings.setter(MESSUI_SW_COLUMN_WIDTHS, ColumnEncodeStringWithCount(width, SW_COLUMN_MAX));
}

void GetSWColumnWidths(int width[])
{
	ColumnDecodeStringWithCount(settings.getter(MESSUI_SW_COLUMN_WIDTHS), width, SW_COLUMN_MAX);
}

void SetSWSortColumn(int column)
{
	settings.setter(MESSUI_SW_SORT_COLUMN, column);
}

int GetSWSortColumn(void)
{
	return settings.int_value(MESSUI_SW_SORT_COLUMN);
}

void SetSWSortReverse(BOOL reverse)
{
	settings.setter( MESSUI_SW_SORT_REVERSED, reverse);
}

BOOL GetSWSortReverse(void)
{
	return settings.bool_value(MESSUI_SW_SORT_REVERSED);
}


void SetSelectedSoftware(int driver_index, string opt_name, const char *software)
{
	const char *s = opt_name.c_str();

	if (LOG_SOFTWARE)
	{
		printf("SetSelectedSoftware(): slot=%s driver=%s software='%s'\n", s, driver_list::driver(driver_index).name, software);
	}

	if (s)
	{
		printf("About to load %s into slot %s\n",software,s);
		windows_options o;
		//o.set_value(OPTION_SYSTEMNAME, driver_list::driver(driver_index).name, OPTION_PRIORITY_CMDLINE);
		load_options(o, OPTIONS_GAME, driver_index, 1);
		o.set_value(s, software, OPTION_PRIORITY_CMDLINE);
		//o.image_option(opt_name).specify(software);
		printf("Done\n");
		save_options(o, OPTIONS_GAME, driver_index);
	}
}

void SetCurrentSoftwareTab(const char *shortname)
{
	settings.setter(MESSUI_SOFTWARE_TAB, shortname);
}

const char *GetCurrentSoftwareTab(void)
{
	return settings.getter(MESSUI_SOFTWARE_TAB).c_str();
}

bool AreOptionsEqual(windows_options &opts1, windows_options &opts2)
{
	for (auto &curentry : opts1.entries())
	{
		if (curentry->type() != OPTION_HEADER)
		{
			const char *value = curentry->value();
			const char *comp = opts2.value(curentry->name().c_str());
			if (value && strcmp(value, comp) != 0)
				return false;
		}
	}
	return true;
}

void OptionsCopy(windows_options &source, windows_options &dest)
{
	for (auto &dest_entry : source.entries())
	{
		if (dest_entry->names().size() > 0)
		{
			// identify the source entry
			const core_options::entry::shared_ptr source_entry = source.get_entry(dest_entry->name());
			if (source_entry)
			{
				const char *value = source_entry->value();
				if (value)
					dest_entry->set_value(value, source_entry->priority(), true);
			}
		}
	}
}
