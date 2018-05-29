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
#include "game_opts.h"
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


// Screen shot Page tab control text
// these must match the order of the options flags in options.h
// (TAB_...)
static const char *const image_tabs_long_name[MAX_TAB_TYPES] =
{
	"Artwork",
	"Boss",
	"Cabinet",
	"Control Panel",
	"Cover",
	"End",
	"Flyer",
	"Game Over",
	"How To",
	"Logo",
	"Marquee",
	"PCB",
	"Scores",
	"Select",
	"Snapshot",
	"Title",
	"Versus",
	"History"
};

static const char *const image_tabs_short_name[MAX_TAB_TYPES] =
{
	"artpreview",
	"boss",
	"cabinet",
	"cpanel",
	"cover",
	"end",
	"flyer",
	"gameover",
	"howto",
	"logo",
	"marquee",
	"pcb",
	"scores",
	"select",
	"snap",
	"title",
	"versus",
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

string GetGameName(uint32_t driver_index)
{
	if (driver_index < driver_list::total())
		return driver_list::driver(driver_index).name;
	else
		return "0";
}

BOOL OptionsInit()
{
	// set up global options
	printf("OptionsInit: About to load %s\n",UI_INI_FILENAME);fflush(stdout);
	settings.load_file(UI_INI_FILENAME);                    // parse MAMEUI.ini
	printf("OptionsInit: About to load %s\n",MEWUI_FILENAME);fflush(stdout);
	LoadSettingsFile(mewui, MEWUI_FILENAME);                // parse UI.INI
	printf("OptionsInit: About to load %s\n",GAMEINFO_INI_FILENAME);fflush(stdout);
	game_opts.load_file(GAMEINFO_INI_FILENAME);             // parse MAME_g.ini
	printf("OptionsInit: About to load Global Options\n");fflush(stdout);
	load_options(global, OPTIONS_GLOBAL, GLOBAL_OPTIONS, 0);   // parse MAME.INI
	printf("OptionsInit: Finished\n");fflush(stdout);
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
	return image_tabs_long_name[tab_index];
}

const char * GetImageTabShortName(int tab_index)
{
	return image_tabs_short_name[tab_index];
}

//============================================================
//  OPTIONS WRAPPERS
//============================================================

static COLORREF options_get_color(const char *name)
{
	unsigned int r = 0, g = 0, b = 0;
	COLORREF value;
	const string val = settings.getter(name);

	if (sscanf(val.c_str(), "%u,%u,%u", &r, &g, &b) == 3)
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
/*
	static input_seq seq;
	string val = settings.getter(name);
	input_seq_from_tokens(NULL, seq_string.c_str(), &seq);  // HACK
	return &seq;*/
	return NULL;
}



//============================================================
//  OPTIONS CALLS
//============================================================

// ***************************************************************** MAMEUI.INI settings **************************************************************************
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

void SetOverrideRedX(BOOL val)
{
	settings.setter(MUIOPTION_OVERRIDE_REDX, val);
}

BOOL GetOverrideRedX(void)
{
	return settings.bool_value(MUIOPTION_OVERRIDE_REDX);
}

static LPBITS GetShowFolderFlags(LPBITS bits)
{
	SetAllBits(bits, TRUE);

	string val = settings.getter(MUIOPTION_HIDE_FOLDERS);
	if (val.empty())
		return bits;

	extern const FOLDERDATA g_folderData[];
	char s[val.size()+1];
	snprintf(s, val.size()+1, "%s", val.c_str());
	char *token = strtok(s, ",");
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
	return bits;
}

BOOL GetShowFolder(int folder)
{
	LPBITS show_folder_flags = NewBits(MAX_FOLDERS);
	show_folder_flags = GetShowFolderFlags(show_folder_flags);
	BOOL result = TestBit(show_folder_flags, folder);
	DeleteBits(show_folder_flags);
	return result;
}

void SetShowFolder(int folder, BOOL show)
{
	LPBITS show_folder_flags = NewBits(MAX_FOLDERS);
	int i = 0, j = 0;
	int num_saved = 0;
	string str;
	extern const FOLDERDATA g_folderData[];

	show_folder_flags = GetShowFolderFlags(show_folder_flags);

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

void SetCurrentTab(int val)
{
	settings.setter(MUIOPTION_CURRENT_TAB, val);
}

int GetCurrentTab(void)
{
	return settings.int_value(MUIOPTION_CURRENT_TAB);
}

// Need int here in case no games were in the list at exit
void SetDefaultGame(uint32_t val)
{
	settings.setter(MUIOPTION_DEFAULT_GAME, driver_list::driver(val).name);
}

uint32_t GetDefaultGame(void)
{
	string t = settings.getter(MUIOPTION_DEFAULT_GAME);
	if (t.empty())
		return 0;
	int val = driver_list::find(t.c_str());
	if (val < 0)
		val = 0;
	return val;
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

void SetWindowPanes(int val)
{
	settings.setter(MUIOPTION_WINDOW_PANES, val & 15);
}

UINT GetWindowPanes(void)
{
	return settings.int_value(MUIOPTION_WINDOW_PANES) & 15;
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

const string GetIconsDir(void)
{
	string t = settings.getter(MUIOPTION_ICONS_DIRECTORY);
	if (t.empty())
		return "icons";
	else
		return settings.getter(MUIOPTION_ICONS_DIRECTORY);
}

void SetIconsDir(const char* path)
{
	settings.setter(MUIOPTION_ICONS_DIRECTORY, path);
}

const string GetBgDir (void)
{
	string t = settings.getter(MUIOPTION_BACKGROUND_DIRECTORY);
	if (t.empty())
		return "bkground\\bkground.png";
	else
		return settings.getter(MUIOPTION_BACKGROUND_DIRECTORY);
}

void SetBgDir (const char* path)
{
	settings.setter(MUIOPTION_BACKGROUND_DIRECTORY, path);
}

const string GetDatsDir(void)
{
	string t = settings.getter(MUIOPTION_DATS_DIRECTORY);
	if (t.empty())
		return "dats";
	else
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

const string GetVideoDir(void)
{
	string t = settings.getter(MUIOPTION_VIDEO_DIRECTORY);
	if (t.empty())
		return "video";
	else
		return settings.getter(MUIOPTION_VIDEO_DIRECTORY);
}

void SetVideoDir(const char *path)
{
	settings.setter(MUIOPTION_VIDEO_DIRECTORY, path);
}

const string GetManualsDir(void)
{
	string t = settings.getter(MUIOPTION_MANUALS_DIRECTORY);
	if (t.empty())
		return "manuals";
	else
		return settings.getter(MUIOPTION_MANUALS_DIRECTORY);
}

void SetManualsDir(const char *path)
{
	settings.setter(MUIOPTION_MANUALS_DIRECTORY, path);
}

// ***************************************************************** MAME.INI settings **************************************************************************
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
	const char* t = global.value(OPTION_MEDIAPATH);
	if (t)
		return string(global.value(OPTION_MEDIAPATH));
	else
		return "roms";
}

void SetRomDirs(const char* paths)
{
	global.set_value(OPTION_MEDIAPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetHashDirs(void)
{
	const char* t = global.value(OPTION_HASHPATH);
	if (t)
		return string(global.value(OPTION_HASHPATH));
	else
		return "hash";
}

void SetHashDirs(const char* paths)
{
	global.set_value(OPTION_HASHPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetSampleDirs(void)
{
	const char* t = global.value(OPTION_SAMPLEPATH);
	if (t)
		return string(global.value(OPTION_SAMPLEPATH));
	else
		return "samples";
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
	const char* t = global.value(OPTION_CTRLRPATH);
	if (t)
		return string(global.value(OPTION_CTRLRPATH));
	else
		return "ctrlr";
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
	const char* t = global.value(OPTION_CFG_DIRECTORY);
	if (t)
		return string(global.value(OPTION_CFG_DIRECTORY));
	else
		return "cfg";
}

void SetCfgDir(const char* path)
{
	global.set_value(OPTION_CFG_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetNvramDir(void)
{
	const char* t = global.value(OPTION_NVRAM_DIRECTORY);
	if (t)
		return string(global.value(OPTION_NVRAM_DIRECTORY));
	else
		return "nvram";
}

void SetNvramDir(const char* path)
{
	global.set_value(OPTION_NVRAM_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetInpDir(void)
{
	const char* t = global.value(OPTION_INPUT_DIRECTORY);
	if (t)
		return string(global.value(OPTION_INPUT_DIRECTORY));
	else
		return "inp";
}

void SetInpDir(const char* path)
{
	global.set_value(OPTION_INPUT_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetImgDir(void)
{
	const char* t = global.value(OPTION_SNAPSHOT_DIRECTORY);
	if (t)
		return string(global.value(OPTION_SNAPSHOT_DIRECTORY));
	else
		return "snap";
}

void SetImgDir(const char* path)
{
	global.set_value(OPTION_SNAPSHOT_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetStateDir(void)
{
	const char* t = global.value(OPTION_STATE_DIRECTORY);
	if (t)
		return string(global.value(OPTION_STATE_DIRECTORY));
	else
		return "sta";
}

void SetStateDir(const char* path)
{
	global.set_value(OPTION_STATE_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetArtDir(void)
{
	const char* t = global.value(OPTION_ARTPATH);
	if (t)
		return string(global.value(OPTION_ARTPATH));
	else
		return "artwork";
}

void SetArtDir(const char* path)
{
	global.set_value(OPTION_ARTPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetFontDir(void)
{
	const char* t = global.value(OPTION_FONTPATH);
	if (t)
		return string(global.value(OPTION_FONTPATH));
	else
		return ".";
}

void SetFontDir(const char* paths)
{
	global.set_value(OPTION_FONTPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetCrosshairDir(void)
{
	const char* t = global.value(OPTION_CROSSHAIRPATH);
	if (t)
		return string(global.value(OPTION_CROSSHAIRPATH));
	else
		return "crosshair";
}

void SetCrosshairDir(const char* paths)
{
	global.set_value(OPTION_CROSSHAIRPATH, paths, OPTION_PRIORITY_CMDLINE);
}

const string GetPluginsDir(void)
{
	const char* t = global.value(OPTION_PLUGINSPATH);
	if (t)
		return string(global.value(OPTION_PLUGINSPATH));
	else
		return "plugins";
}

void SetPluginsDir(const char* path)
{
	global.set_value(OPTION_PLUGINSPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetLangDir(void)
{
	const char* t = global.value(OPTION_LANGUAGEPATH);
	if (t)
		return string(global.value(OPTION_LANGUAGEPATH));
	else
		return "language";
}

void SetLangDir(const char* path)
{
	global.set_value(OPTION_LANGUAGEPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetDiffDir(void)
{
	const char* t = global.value(OPTION_DIFF_DIRECTORY);
	if (t)
		return string(global.value(OPTION_DIFF_DIRECTORY));
	else
		return "diff";
}

void SetDiffDir(const char* path)
{
	global.set_value(OPTION_DIFF_DIRECTORY, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCheatDir(void)
{
	const char* t = global.value(OPTION_CHEATPATH);
	if (t)
		return string(global.value(OPTION_CHEATPATH));
	else
		return "cheat";
}

void SetCheatDir(const char* path)
{
	global.set_value(OPTION_CHEATPATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetHLSLDir(void)
{
	const char* t = global.value(WINOPTION_HLSLPATH);
	if (t)
		return string(global.value(WINOPTION_HLSLPATH));
	else
		return "hlsl";
}

void SetHLSLDir(const char* path)
{
	global.set_value(WINOPTION_HLSLPATH, path, OPTION_PRIORITY_CMDLINE);
}

const char* GetSnapName(void)
{
	return global.value(OPTION_SNAPNAME);
}

void SetSnapName(const char* pattern)
{
	global.set_value(OPTION_SNAPNAME, pattern, OPTION_PRIORITY_CMDLINE);
}

// ***************************************************************** UI.INI settings **************************************************************************
const string GetScoresDir(void)
{
	const char* t = mewui.value(OPTION_SCORES_PATH);
	if (t)
		return string(mewui.value(OPTION_SCORES_PATH));
	else
		return "scores";
}

void SetScoresDir(const char *path)
{
	mewui.set_value(OPTION_SCORES_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetBossesDir(void)
{
	const char* t = mewui.value(OPTION_BOSSES_PATH);
	if (t)
		return string(mewui.value(OPTION_BOSSES_PATH));
	else
		return "bosses";
}

void SetBossesDir(const char *path)
{
	mewui.set_value(OPTION_BOSSES_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCoversDir(void)
{
	const char* t = mewui.value(OPTION_COVER_PATH);
	if (t)
		return string(mewui.value(OPTION_COVER_PATH));
	else
		return "covers";
}

void SetCoversDir(const char *path)
{
	mewui.set_value(OPTION_COVER_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetVersusDir(void)
{
	const char* t = mewui.value(OPTION_VERSUS_PATH);
	if (t)
		return string(mewui.value(OPTION_VERSUS_PATH));
	else
		return "versus";
}

void SetVersusDir(const char *path)
{
	mewui.set_value(OPTION_VERSUS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetEndsDir(void)
{
	const char* t = mewui.value(OPTION_ENDS_PATH);
	if (t)
		return string(mewui.value(OPTION_ENDS_PATH));
	else
		return "ends";
}

void SetEndsDir(const char *path)
{
	mewui.set_value(OPTION_ENDS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetGameOverDir(void)
{
	const char* t = mewui.value(OPTION_GAMEOVER_PATH);
	if (t)
		return string(mewui.value(OPTION_GAMEOVER_PATH));
	else
		return "gameover";
}

void SetGameOverDir(const char *path)
{
	mewui.set_value(OPTION_GAMEOVER_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetHowToDir(void)
{
	const char* t = mewui.value(OPTION_HOWTO_PATH);
	if (t)
		return string(mewui.value(OPTION_HOWTO_PATH));
	else
		return "howto";
}

void SetHowToDir(const char *path)
{
	mewui.set_value(OPTION_HOWTO_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetSelectDir(void)
{
	const char* t = mewui.value(OPTION_SELECT_PATH);
	if (t)
		return string(mewui.value(OPTION_SELECT_PATH));
	else
		return "select";
}

void SetSelectDir(const char *path)
{
	mewui.set_value(OPTION_SELECT_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetLogoDir(void)
{
	const char* t = mewui.value(OPTION_LOGOS_PATH);
	if (t)
		return string(mewui.value(OPTION_LOGOS_PATH));
	else
		return "logo";
}

void SetLogoDir(const char *path)
{
	mewui.set_value(OPTION_LOGOS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetArtworkDir(void)
{
	const char* t = mewui.value(OPTION_ARTPREV_PATH);
	if (t)
		return string(mewui.value(OPTION_ARTPREV_PATH));
	else
		return "artpreview";
}

void SetArtworkDir(const char *path)
{
	mewui.set_value(OPTION_ARTPREV_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetFlyerDir(void)
{
	const char* t = mewui.value(OPTION_FLYERS_PATH);
	if (t)
		return string(mewui.value(OPTION_FLYERS_PATH));
	else
		return "flyers";
}

void SetFlyerDir(const char* path)
{
	mewui.set_value(OPTION_FLYERS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetCabinetDir(void)
{
	const char* t = mewui.value(OPTION_CABINETS_PATH);
	if (t)
		return string(mewui.value(OPTION_CABINETS_PATH));
	else
		return "cabinets";
}

void SetCabinetDir(const char* path)
{
	mewui.set_value(OPTION_CABINETS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetMarqueeDir(void)
{
	const char* t = mewui.value(OPTION_MARQUEES_PATH);
	if (t)
		return string(mewui.value(OPTION_MARQUEES_PATH));
	else
		return "marquees";
}

void SetMarqueeDir(const char* path)
{
	mewui.set_value(OPTION_MARQUEES_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetTitlesDir(void)
{
	const char* t = mewui.value(OPTION_TITLES_PATH);
	if (t)
		return string(mewui.value(OPTION_TITLES_PATH));
	else
		return "titles";
}

void SetTitlesDir(const char* path)
{
	mewui.set_value(OPTION_TITLES_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetControlPanelDir(void)
{
	const char* t = mewui.value(OPTION_CPANELS_PATH);
	if (t)
		return string(mewui.value(OPTION_CPANELS_PATH));
	else
		return "cpanel";
}

void SetControlPanelDir(const char *path)
{
	mewui.set_value(OPTION_CPANELS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetPcbDir(void)
{
	const char* t = mewui.value(OPTION_PCBS_PATH);
	if (t)
		return string(mewui.value(OPTION_PCBS_PATH));
	else
		return "pcb";
}

void SetPcbDir(const char *path)
{
	mewui.set_value(OPTION_PCBS_PATH, path, OPTION_PRIORITY_CMDLINE);
}

const string GetFolderDir(void)
{
	const char* t = mewui.value(OPTION_CATEGORYINI_PATH);
	if (t)
		return string(mewui.value(OPTION_CATEGORYINI_PATH));
	else
		return "folders";
}

void SetFolderDir(const char* path)
{
	mewui.set_value(OPTION_CATEGORYINI_PATH, path, OPTION_PRIORITY_CMDLINE);
}

// ***************************************************************** MAME_g.INI settings **************************************************************************
int GetRomAuditResults(uint32_t driver_index)
{
	return game_opts.rom(driver_index);
}

void SetRomAuditResults(uint32_t driver_index, int audit_results)
{
	game_opts.rom(driver_index, audit_results);
}

int GetSampleAuditResults(uint32_t driver_index)
{
	return game_opts.sample(driver_index);
}

void SetSampleAuditResults(uint32_t driver_index, int audit_results)
{
	game_opts.sample(driver_index, audit_results);
}

static void IncrementPlayVariable(uint32_t driver_index, const char *play_variable, uint32_t increment)
{
	if (strcmp(play_variable, "count") == 0)
		game_opts.play_count(driver_index, game_opts.play_count(driver_index) + increment);
	else
	if (strcmp(play_variable, "time") == 0)
		game_opts.play_time(driver_index, game_opts.play_time(driver_index) + increment);
}

void IncrementPlayCount(uint32_t driver_index)
{
	IncrementPlayVariable(driver_index, "count", 1);
}

uint32_t GetPlayCount(uint32_t driver_index)
{
	return game_opts.play_count(driver_index);
}

// int needed here so we can reset all games
static void ResetPlayVariable(int driver_index, const char *play_variable)
{
	if (driver_index < 0)
		/* all games */
		for (uint32_t i = 0; i < driver_list::total(); i++)
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

// int needed here so we can reset all games
void ResetPlayCount(int driver_index)
{
	ResetPlayVariable(driver_index, "count");
}

// int needed here so we can reset all games
void ResetPlayTime(int driver_index)
{
	ResetPlayVariable(driver_index, "time");
}

uint32_t GetPlayTime(uint32_t driver_index)
{
	return game_opts.play_time(driver_index);
}

void IncrementPlayTime(uint32_t driver_index, uint32_t playtime)
{
	IncrementPlayVariable(driver_index, "time", playtime);
}

void GetTextPlayTime(uint32_t driver_index, char *buf)
{

	if (driver_index < driver_list::total())
	{
		uint32_t second = GetPlayTime(driver_index);
		uint32_t hour = second / 3600;
		second -= 3600*hour;
		uint8_t minute = second / 60; //Calc Minutes
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

// exec functions start: these are unsupported
void SetUIJoyExec(int joycodeIndex, int val)
{
	SetUIJoy(MUIOPTION_UI_JOY_EXEC, joycodeIndex, val);
}

int GetUIJoyExec(int joycodeIndex)
{
	return GetUIJoy(MUIOPTION_UI_JOY_EXEC, joycodeIndex);
}

const string GetExecCommand(void)
{
	return settings.getter(MUIOPTION_EXEC_COMMAND);
}

// not used
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
// exec functions end

BOOL GetHideMouseOnStartup(void)
{
	return settings.bool_value(MUIOPTION_HIDE_MOUSE);
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



void ResetGameOptions(int driver_index)
{
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

/* MSH 20080813
 * Read the folder filters from MAMEui.ini.  This must only
 * be called AFTER the folders have all been created.
 */
void LoadFolderFlags(void)
{
	LPTREEFOLDER lpFolder;
	int i, numFolders = GetNumFolders();

	for (i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);

		if (lpFolder)
		{
			char folder_name[400];
			char *ptr;

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if ((*ptr == ' ') || (*ptr == '-'))
					*ptr = '_';

				ptr++;
			}

			string option_name = string(folder_name) + "_filters";
		}
	}

	// These are added to our UI ini
	// The normal read will skip them.

	// retrieve the stored values
	for (i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);

		if (lpFolder)
		{
			char folder_name[400];

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			char *ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if ((*ptr == ' ') || (*ptr == '-'))
					*ptr = '_';

				ptr++;
			}
			string option_name = string(folder_name) + "_filters";

			// get entry and decode it
			lpFolder->m_dwFlags |= (settings.int_value(option_name.c_str()) & F_MASK);
		}
	}
}



// Adds our folder flags to winui_options, for saving.
static void AddFolderFlags()
{
	LPTREEFOLDER lpFolder;
	int num_entries = 0, numFolders = GetNumFolders();

	for (int i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);
		if (lpFolder)
		{
			char folder_name[400];

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			char *ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if ((*ptr == ' ') || (*ptr == '-'))
					*ptr = '_';

				ptr++;
			}

			string option_name = string(folder_name) + "_filters";

			// store entry
			settings.setter(option_name.c_str(), lpFolder->m_dwFlags & F_MASK);

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


// See if this driver has software support
bool DriverHasSoftware(uint32_t drvindex)
{
	if (drvindex < driver_list::total())
	{
		windows_options o;
		load_options(o, OPTIONS_GAME, drvindex, 1);
		machine_config config(driver_list::driver(drvindex), o);

		for (device_image_interface &img : image_interface_iterator(config.root_device()))
			if (img.user_loadable())
				return 1;
	}

	return 0;
}


// Reset the given windows_options to their default settings.
static void ResetToDefaults(windows_options &opts, int priority)
{
	// iterate through the options setting each one back to the default value.
	windows_options dummy;
	OptionsCopy(dummy, opts);
}

uint32_t GetDriverCacheLower(uint32_t driver_index)
{
	return game_opts.cache_lower(driver_index);
}

uint32_t GetDriverCacheUpper(uint32_t driver_index)
{
	return game_opts.cache_upper(driver_index);
}

void SetDriverCache(uint32_t driver_index, uint32_t val)
{
	game_opts.cache_upper(driver_index, val);
}

BOOL RequiredDriverCache(void)
{
	return game_opts.rebuild();
}

void ForceRebuild(void)
{
	game_opts.force_rebuild();
}

BOOL DriverIsComputer(uint32_t driver_index)
{
	uint32_t cache = game_opts.cache_lower(driver_index) & 3;
	return (cache == 2) ? true : false;
}

BOOL DriverIsConsole(uint32_t driver_index)
{
	uint32_t cache = game_opts.cache_lower(driver_index) & 3;
	return (cache == 1) ? true : false;
}

BOOL DriverIsModified(uint32_t driver_index)
{
	return BIT(game_opts.cache_lower(driver_index), 12);
}

BOOL DriverIsImperfect(uint32_t driver_index)
{
	return (game_opts.cache_lower(driver_index) & 0xff0000) ? true : false; // (NO|IMPERFECT) (CONTROLS|PALETTE|SOUND|GRAPHICS)
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

void SetCurrentSoftwareTab(int val)
{
	settings.setter(MESSUI_SOFTWARE_TAB, val);
}

int GetCurrentSoftwareTab(void)
{
	return settings.int_value(MESSUI_SOFTWARE_TAB);
}

bool AreOptionsEqual(windows_options &opts1, windows_options &opts2)
{
	for (auto &curentry : opts1.entries())
	{
		if (curentry->type() != OPTION_HEADER)
		{
			const char *value = curentry->value();
			const char *comp = opts2.value(curentry->name().c_str());
			if (!value && !comp) // both empty, they are the same
			{}
			else
			if (!value || !comp) // only one empty, they are different
				return false;
			else
			if (strcmp(value, comp) != 0) // both not empty, do proper compare
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
