// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

 /***************************************************************************

  mui_opts.cpp

  Stores global options and per-game options;

***************************************************************************/

// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <winreg.h>
#include <commctrl.h>

// standard C headers
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <direct.h>
#include <emu.h>
#include <mameopts.h>
#include <ui/moptions.h>
#include <stddef.h>
#include <tchar.h>

// MAME/MAMEUI headers
#include "bitmask.h"
#include "winui.h"
#include "mui_util.h"
#include "treeview.h"
#include "splitters.h"
#include "mui_opts.h"
#include "winutf8.h"
#include "strconv.h"
#include "drivenum.h"
#include "game_opts.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

/***************************************************************************
    Internal function prototypes
 ***************************************************************************/

// static void LoadFolderFilter(int folder_index,int filters);

static void LoadSettingsFile(winui_options &opts, const char *filename);
static void SaveSettingsFile(winui_options &opts, const char *filename);
static void LoadSettingsFile(windows_options &opts, const char *filename);
static void SaveSettingsFile(windows_options &opts, const char *filename);

static void LoadOptionsAndSettings(void);

static void CusColorEncodeString(const COLORREF *value, char* str);
static void CusColorDecodeString(const char* str, COLORREF *value);

static void SplitterEncodeString(const int *value, char* str);
static void SplitterDecodeString(const char *str, int *value);

static void FontEncodeString(const LOGFONT *f, char *str);
static void FontDecodeString(const char* str, LOGFONT *f);

static void TabFlagsEncodeString(int data, char *str);
static void TabFlagsDecodeString(const char *str, int *data);

static DWORD DecodeFolderFlags(const char *buf);
static const char * EncodeFolderFlags(DWORD value);

static void ResetToDefaults(windows_options &opts, int priority);

//static void ui_parse_ini_file(windows_options &opts, const char *name);


#ifdef _MSC_VER
#define snprintf _snprintf
#endif

/***************************************************************************
    Internal defines
 ***************************************************************************/

#define GAMEINFO_INI_FILENAME                    MAMENAME "_g.ini"
#define MEWUI_FILENAME                           "ini\\ui.ini"

#define MUIOPTION_LIST_MODE                      "list_mode"
#define MUIOPTION_CHECK_GAME                     "check_game"
#define MUIOPTION_JOYSTICK_IN_INTERFACE          "joystick_in_interface"
#define MUIOPTION_KEYBOARD_IN_INTERFACE          "keyboard_in_interface"
#define MUIOPTION_CYCLE_SCREENSHOT               "cycle_screenshot"
#define MUIOPTION_STRETCH_SCREENSHOT_LARGER      "stretch_screenshot_larger"
#define MUIOPTION_SCREENSHOT_BORDER_SIZE         "screenshot_bordersize"
#define MUIOPTION_SCREENSHOT_BORDER_COLOR        "screenshot_bordercolor"
#define MUIOPTION_INHERIT_FILTER                 "inherit_filter"
#define MUIOPTION_OFFSET_CLONES                  "offset_clones"
#define MUIOPTION_DEFAULT_FOLDER_ID              "default_folder_id"
#define MUIOPTION_SHOW_IMAGE_SECTION             "show_image_section"
#define MUIOPTION_SHOW_FOLDER_SECTION            "show_folder_section"
#define MUIOPTION_HIDE_FOLDERS                   "hide_folders"
#define MUIOPTION_SHOW_STATUS_BAR                "show_status_bar"
#define MUIOPTION_SHOW_TABS                      "show_tabs"
#define MUIOPTION_SHOW_TOOLBAR                   "show_tool_bar"
#define MUIOPTION_CURRENT_TAB                    "current_tab"
#define MUIOPTION_WINDOW_X                       "window_x"
#define MUIOPTION_WINDOW_Y                       "window_y"
#define MUIOPTION_WINDOW_WIDTH                   "window_width"
#define MUIOPTION_WINDOW_HEIGHT                  "window_height"
#define MUIOPTION_WINDOW_STATE                   "window_state"
#define MUIOPTION_CUSTOM_COLOR                   "custom_color"
#define MUIOPTION_LIST_FONT                      "list_font"
#define MUIOPTION_TEXT_COLOR                     "text_color"
#define MUIOPTION_CLONE_COLOR                    "clone_color"
#define MUIOPTION_HIDE_TABS                      "hide_tabs"
#define MUIOPTION_HISTORY_TAB                    "history_tab"
#define MUIOPTION_COLUMN_WIDTHS                  "column_widths"
#define MUIOPTION_COLUMN_ORDER                   "column_order"
#define MUIOPTION_COLUMN_SHOWN                   "column_shown"
#define MUIOPTION_SPLITTERS                      "splitters"
#define MUIOPTION_SORT_COLUMN                    "sort_column"
#define MUIOPTION_SORT_REVERSED                  "sort_reversed"
#define MUIOPTION_ICONS_DIRECTORY                "icons_directory"
#define MUIOPTION_BACKGROUND_DIRECTORY           "background_directory"
#define MUIOPTION_DATS_DIRECTORY                 "dats_directory"
#define MUIOPTION_UI_KEY_UP                      "ui_key_up"
#define MUIOPTION_UI_KEY_DOWN                    "ui_key_down"
#define MUIOPTION_UI_KEY_LEFT                    "ui_key_left"
#define MUIOPTION_UI_KEY_RIGHT                   "ui_key_right"
#define MUIOPTION_UI_KEY_START                   "ui_key_start"
#define MUIOPTION_UI_KEY_PGUP                    "ui_key_pgup"
#define MUIOPTION_UI_KEY_PGDWN                   "ui_key_pgdwn"
#define MUIOPTION_UI_KEY_HOME                    "ui_key_home"
#define MUIOPTION_UI_KEY_END                     "ui_key_end"
#define MUIOPTION_UI_KEY_SS_CHANGE               "ui_key_ss_change"
#define MUIOPTION_UI_KEY_HISTORY_UP              "ui_key_history_up"
#define MUIOPTION_UI_KEY_HISTORY_DOWN            "ui_key_history_down"
#define MUIOPTION_UI_KEY_CONTEXT_FILTERS         "ui_key_context_filters"
#define MUIOPTION_UI_KEY_SELECT_RANDOM           "ui_key_select_random"
#define MUIOPTION_UI_KEY_GAME_AUDIT              "ui_key_game_audit"
#define MUIOPTION_UI_KEY_GAME_PROPERTIES         "ui_key_game_properties"
#define MUIOPTION_UI_KEY_HELP_CONTENTS           "ui_key_help_contents"
#define MUIOPTION_UI_KEY_UPDATE_GAMELIST         "ui_key_update_gamelist"
#define MUIOPTION_UI_KEY_VIEW_FOLDERS            "ui_key_view_folders"
#define MUIOPTION_UI_KEY_VIEW_FULLSCREEN         "ui_key_view_fullscreen"
#define MUIOPTION_UI_KEY_VIEW_PAGETAB            "ui_key_view_pagetab"
#define MUIOPTION_UI_KEY_VIEW_PICTURE_AREA       "ui_key_view_picture_area"
#define MUIOPTION_UI_KEY_VIEW_STATUS             "ui_key_view_status"
#define MUIOPTION_UI_KEY_VIEW_TOOLBARS           "ui_key_view_toolbars"
#define MUIOPTION_UI_KEY_VIEW_TAB_CABINET        "ui_key_view_tab_cabinet"
#define MUIOPTION_UI_KEY_VIEW_TAB_CPANEL         "ui_key_view_tab_cpanel"
#define MUIOPTION_UI_KEY_VIEW_TAB_FLYER          "ui_key_view_tab_flyer"
#define MUIOPTION_UI_KEY_VIEW_TAB_HISTORY        "ui_key_view_tab_history"
#define MUIOPTION_UI_KEY_VIEW_TAB_MARQUEE        "ui_key_view_tab_marquee"
#define MUIOPTION_UI_KEY_VIEW_TAB_SCREENSHOT     "ui_key_view_tab_screenshot"
#define MUIOPTION_UI_KEY_VIEW_TAB_TITLE          "ui_key_view_tab_title"
#define MUIOPTION_UI_KEY_VIEW_TAB_PCB            "ui_key_view_tab_pcb"
#define MUIOPTION_UI_KEY_QUIT                    "ui_key_quit"
#define MUIOPTION_UI_JOY_UP                      "ui_joy_up"
#define MUIOPTION_UI_JOY_DOWN                    "ui_joy_down"
#define MUIOPTION_UI_JOY_LEFT                    "ui_joy_left"
#define MUIOPTION_UI_JOY_RIGHT                   "ui_joy_right"
#define MUIOPTION_UI_JOY_START                   "ui_joy_start"
#define MUIOPTION_UI_JOY_PGUP                    "ui_joy_pgup"
#define MUIOPTION_UI_JOY_PGDWN                   "ui_joy_pgdwn"
#define MUIOPTION_UI_JOY_HOME                    "ui_joy_home"
#define MUIOPTION_UI_JOY_END                     "ui_joy_end"
#define MUIOPTION_UI_JOY_SS_CHANGE               "ui_joy_ss_change"
#define MUIOPTION_UI_JOY_HISTORY_UP              "ui_joy_history_up"
#define MUIOPTION_UI_JOY_HISTORY_DOWN            "ui_joy_history_down"
#define MUIOPTION_UI_JOY_EXEC                    "ui_joy_exec"
#define MUIOPTION_EXEC_COMMAND                   "exec_command"
#define MUIOPTION_EXEC_WAIT                      "exec_wait"
#define MUIOPTION_HIDE_MOUSE                     "hide_mouse"
#define MUIOPTION_FULL_SCREEN                    "full_screen"

#ifdef MESS
#define MUIOPTION_SHOW_SOFTWARE_SECTION          "show_software_section"
#define MUIOPTION_UI_KEY_VIEW_SOFTWARE_AREA      "ui_key_view_software_area"
#define MUIOPTION_DEFAULT_GAME                   "default_system"
#define MUIDEFAULT_SELECTION                     "3do"
#define MUIDEFAULT_SPLITTERS                     "152,310,468"
#else
#define MUIOPTION_DEFAULT_GAME                   "default_machine"
#define MUIDEFAULT_SELECTION                     "puckman"
#define MUIDEFAULT_SPLITTERS                     "152,362"
#endif

#define MUIOPTION_VERSION                        "version"



/***************************************************************************
    Internal structures
 ***************************************************************************/

 /***************************************************************************
    Internal variables
 ***************************************************************************/

static mame_options mameopts; // core options
static ui_options mewui; // ui.ini
static winui_options settings; // mameui.ini

static windows_options global; // Global 'default' options

static game_options game_opts;

// UI options in MAMEui.ini
const options_entry winui_options::s_option_entries[] =
{
	// UI options
	{ NULL,                                   NULL,       OPTION_HEADER,     "APPLICATION OPTIONS" },
	{ MUIOPTION_VERSION,                      "",         OPTION_STRING,                 NULL },
	{ NULL,                                   NULL,       OPTION_HEADER,     "DISPLAY STATE OPTIONS" },
	{ MUIOPTION_DEFAULT_GAME,                 MUIDEFAULT_SELECTION, OPTION_STRING,       NULL },
	{ MUIOPTION_DEFAULT_FOLDER_ID,            "0",        OPTION_INTEGER,                 NULL },
	{ MUIOPTION_SHOW_IMAGE_SECTION,           "1",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_FULL_SCREEN,                  "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_CURRENT_TAB,                  "0",        OPTION_STRING,                 NULL },
	{ MUIOPTION_SHOW_TOOLBAR,                 "1",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_SHOW_STATUS_BAR,              "1",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_HIDE_FOLDERS,                 "",         OPTION_STRING,                 NULL },
	{ MUIOPTION_SHOW_FOLDER_SECTION,          "1",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_SHOW_TABS,                    "1",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_HIDE_TABS,                    "marquee, title, cpanel, pcb, history", OPTION_STRING, NULL },
	{ MUIOPTION_HISTORY_TAB,                  "0",        OPTION_INTEGER,                 NULL },
#ifdef MESS
	{ MUIOPTION_SHOW_SOFTWARE_SECTION,        "1",        OPTION_BOOLEAN,    NULL },
#endif
	{ MUIOPTION_SORT_COLUMN,                  "0",        OPTION_INTEGER,                 NULL },
	{ MUIOPTION_SORT_REVERSED,                "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_WINDOW_X,                     "0",        OPTION_INTEGER,                 NULL },
	{ MUIOPTION_WINDOW_Y,                     "0",        OPTION_INTEGER,                 NULL },
	{ MUIOPTION_WINDOW_WIDTH,                 "800",      OPTION_INTEGER,                 NULL },
	{ MUIOPTION_WINDOW_HEIGHT,                "600",      OPTION_INTEGER,                 NULL },
	{ MUIOPTION_WINDOW_STATE,                 "1",        OPTION_INTEGER,                 NULL },
	{ MUIOPTION_TEXT_COLOR,                   "-1",       OPTION_INTEGER,                 NULL },
	{ MUIOPTION_CLONE_COLOR,                  "-1",       OPTION_INTEGER,                 NULL },
	{ MUIOPTION_CUSTOM_COLOR,                 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0", OPTION_STRING, NULL },
	/* ListMode needs to be before ColumnWidths settings */
	{ MUIOPTION_LIST_MODE,                    "5",       OPTION_INTEGER,                 NULL },
	{ MUIOPTION_SPLITTERS,                    MUIDEFAULT_SPLITTERS, OPTION_STRING,       NULL },
	{ MUIOPTION_LIST_FONT,                    "-8,0,0,0,400,0,0,0,0,0,0,0,MS Sans Serif", OPTION_STRING, NULL },
	{ MUIOPTION_COLUMN_WIDTHS,                "185,78,84,84,64,88,74,108,60,144,84,40,40", OPTION_STRING, NULL },
	{ MUIOPTION_COLUMN_ORDER,                 "0,1,2,3,4,5,6,7,8,9,10,11,12", OPTION_STRING, NULL },
	{ MUIOPTION_COLUMN_SHOWN,                 "1,1,1,1,1,1,1,1,1,1,1,1,0", OPTION_STRING,  NULL },
	{ NULL,                                   NULL,       OPTION_HEADER,     "INTERFACE OPTIONS" },
	{ MUIOPTION_CHECK_GAME,                   "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_JOYSTICK_IN_INTERFACE,        "1",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_KEYBOARD_IN_INTERFACE,        "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_HIDE_MOUSE,                   "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_INHERIT_FILTER,               "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_OFFSET_CLONES,                "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_STRETCH_SCREENSHOT_LARGER,    "0",        OPTION_BOOLEAN,    NULL },
	{ MUIOPTION_CYCLE_SCREENSHOT,             "0",        OPTION_INTEGER,                 NULL },
	{ MUIOPTION_SCREENSHOT_BORDER_SIZE,       "11",       OPTION_INTEGER,                 NULL },
	{ MUIOPTION_SCREENSHOT_BORDER_COLOR,      "-1",       OPTION_INTEGER,                 NULL },
	{ MUIOPTION_EXEC_COMMAND,                 "",         OPTION_STRING,                 NULL },
	{ MUIOPTION_EXEC_WAIT,                    "0",        OPTION_INTEGER,                 NULL },
	{ NULL,                                   NULL,       OPTION_HEADER,     "SEARCH PATH OPTIONS" },
	{ MUIOPTION_BACKGROUND_DIRECTORY,         "bkground", OPTION_STRING,                 NULL },
	{ MUIOPTION_ICONS_DIRECTORY,              "icons",    OPTION_STRING,                 NULL },
	{ MUIOPTION_DATS_DIRECTORY,               "dats",     OPTION_STRING,                 NULL },
	{ NULL,                                   NULL,       OPTION_HEADER,     "NAVIGATION KEY CODES" },
	{ MUIOPTION_UI_KEY_UP,                    "KEYCODE_UP",                        OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_DOWN,                  "KEYCODE_DOWN",                     OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_LEFT,                  "KEYCODE_LEFT",                     OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_RIGHT,                 "KEYCODE_RIGHT",                    OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_START,                 "KEYCODE_ENTER NOT KEYCODE_LALT",    OPTION_STRING,            NULL },
	{ MUIOPTION_UI_KEY_PGUP,                  "KEYCODE_PGUP",                     OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_PGDWN,                 "KEYCODE_PGDN",                     OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_HOME,                  "KEYCODE_HOME",                     OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_END,                   "KEYCODE_END",                        OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_SS_CHANGE,             "KEYCODE_INSERT",                    OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_HISTORY_UP,            "KEYCODE_DEL",                        OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_HISTORY_DOWN,          "KEYCODE_LALT KEYCODE_0",            OPTION_STRING,          NULL },
	{ MUIOPTION_UI_KEY_CONTEXT_FILTERS,       "KEYCODE_LCONTROL KEYCODE_F", OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_SELECT_RANDOM,         "KEYCODE_LCONTROL KEYCODE_R", OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_GAME_AUDIT,            "KEYCODE_LALT KEYCODE_A",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_GAME_PROPERTIES,       "KEYCODE_LALT KEYCODE_ENTER", OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_HELP_CONTENTS,         "KEYCODE_F1",                 OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_UPDATE_GAMELIST,       "KEYCODE_F5",                 OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_FOLDERS,          "KEYCODE_LALT KEYCODE_D",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_FULLSCREEN,       "KEYCODE_F11",                OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_PAGETAB,          "KEYCODE_LALT KEYCODE_B",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_PICTURE_AREA,     "KEYCODE_LALT KEYCODE_P",     OPTION_STRING, NULL },
#ifdef MESS
	{ MUIOPTION_UI_KEY_VIEW_SOFTWARE_AREA,    "KEYCODE_LALT KEYCODE_W",     OPTION_STRING, NULL },
#endif
	{ MUIOPTION_UI_KEY_VIEW_STATUS,           "KEYCODE_LALT KEYCODE_S",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TOOLBARS,         "KEYCODE_LALT KEYCODE_T",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_CABINET,      "KEYCODE_LALT KEYCODE_3",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_CPANEL,       "KEYCODE_LALT KEYCODE_6",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_FLYER,        "KEYCODE_LALT KEYCODE_2",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_HISTORY,      "KEYCODE_LALT KEYCODE_8",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_MARQUEE,      "KEYCODE_LALT KEYCODE_4",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_SCREENSHOT,   "KEYCODE_LALT KEYCODE_1",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_TITLE,        "KEYCODE_LALT KEYCODE_5",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_VIEW_TAB_PCB,          "KEYCODE_LALT KEYCODE_7",     OPTION_STRING, NULL },
	{ MUIOPTION_UI_KEY_QUIT,                  "KEYCODE_LALT KEYCODE_Q",     OPTION_STRING, NULL },
	{ NULL,                                   NULL,      OPTION_HEADER,     "NAVIGATION JOYSTICK CODES" },
	{ MUIOPTION_UI_JOY_UP,                    "1,1,1,1",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_DOWN,                  "1,1,1,2",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_LEFT,                  "1,1,2,1",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_RIGHT,                 "1,1,2,2",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_START,                 "1,0,1,0",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_PGUP,                  "2,1,2,1",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_PGDWN,                 "2,1,2,2",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_HOME,                  "0,0,0,0",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_END,                   "0,0,0,0",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_SS_CHANGE,             "2,0,3,0",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_HISTORY_UP,            "2,0,4,0",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_HISTORY_DOWN,          "2,0,1,0",  OPTION_STRING,                 NULL },
	{ MUIOPTION_UI_JOY_EXEC,                  "0,0,0,0",  OPTION_STRING,                 NULL },
	{ NULL }
};

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
winui_options::winui_options()
{
	add_entries(s_option_entries);
}

void CreateGameOptions(windows_options &opts, OPTIONS_TYPE opt_type, int driver_index)
{
#ifdef MESS
	MessSetupGameOptions(opts, opt_type, driver_index);
#endif
}



BOOL OptionsInit()
{
	MessSetupSettings(settings);

#if 0
// keep in case we need per-game options in the future
	// set up per game options
	{
		char buffer[128];
		int i, j;
		int game_option_count = 0;

		while(perGameOptions[game_option_count].name)
			game_option_count++;

		for (i = 0; i < driver_list::total(); i++)
		{
			for (j = 0; j < game_option_count; j++)
			{
				options_entry entry[2] = { { 0 }, { 0 } };
				snprintf(buffer, ARRAY_LENGTH(buffer), "%s%s", driver_list::driver(i).name, perGameOptions[j].name);

				entry[0].name = core_strdup(buffer);
				entry[0].defvalue = perGameOptions[j].defvalue;
				entry[0].flags = perGameOptions[j].flags;
				entry[0].description = perGameOptions[j].description;
				settings.add_entries(entry);
			}
		}
	}
#endif

	game_opts.add_entries();
	// set up global options
	CreateGameOptions(global, OPTIONS_GLOBAL, GLOBAL_OPTIONS);
	// now load the options and settings
	LoadOptionsAndSettings();

	return TRUE;

}

void OptionsExit(void)
{
}

winui_options & MameUISettings(void)
{
	return settings;
}

windows_options & MameUIGlobal(void)
{
	return global;
}

// Restore ui settings to factory
void ResetGUI(void)
{
	settings.revert(OPTION_PRIORITY_NORMAL);
	// Save the new MAMEui.ini
	SaveOptions();
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

static COLORREF options_get_color(winui_options &opts, const char *name)
{
	const char *value_str;
	unsigned int r = 0, g = 0, b = 0;
	COLORREF value;

	value_str = opts.value(name);

	if (sscanf(value_str, "%u,%u,%u", &r, &g, &b) == 3)
		value = RGB(r,g,b);
	else
		value = (COLORREF) - 1;
	return value;
}

static void options_set_color(winui_options &opts, const char *name, COLORREF value)
{
	char value_str[32];

	if (value == (COLORREF) -1)
	{
		snprintf(value_str, ARRAY_LENGTH(value_str), "%d", (int) value);
	}
	else
	{
		snprintf(value_str, ARRAY_LENGTH(value_str), "%d,%d,%d", (((int) value) >>  0) & 0xFF, (((int) value) >>  8) & 0xFF, (((int) value) >> 16) & 0xFF);
	}
	std::string error_string;
	opts.set_value(name, value_str, OPTION_PRIORITY_CMDLINE,error_string);
}

static COLORREF options_get_color_default(winui_options &opts, const char *name, int default_color)
{
	COLORREF value = options_get_color(opts, name);
	if (value == (COLORREF) -1)
		value = GetSysColor(default_color);
	return value;
}

static void options_set_color_default(winui_options &opts, const char *name, COLORREF value, int default_color)
{
	if (value == GetSysColor(default_color))
		options_set_color(settings, name, (COLORREF) -1);
	else
		options_set_color(settings, name, value);
}

static input_seq *options_get_input_seq(winui_options &opts, const char *name)
{
/*  static input_seq seq;
	const char *seq_string;

	seq_string = opts.value( name);
	input_seq_from_tokens(NULL, seq_string, &seq);  // HACK
	return &seq;*/
	return NULL;
}



//============================================================
//  OPTIONS CALLS
//============================================================

void SetViewMode(int val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_LIST_MODE, val, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetViewMode(void)
{
	return settings.int_value(MUIOPTION_LIST_MODE);
}

void SetGameCheck(BOOL game_check)
{
	std::string error_string;
	settings.set_value(MUIOPTION_CHECK_GAME, game_check, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetGameCheck(void)
{
	return settings.bool_value(MUIOPTION_CHECK_GAME);
}

void SetJoyGUI(BOOL use_joygui)
{
	std::string error_string;
	settings.set_value(MUIOPTION_JOYSTICK_IN_INTERFACE, use_joygui, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetJoyGUI(void)
{
	return settings.bool_value( MUIOPTION_JOYSTICK_IN_INTERFACE);
}

void SetKeyGUI(BOOL use_keygui)
{
	std::string error_string;
	settings.set_value(MUIOPTION_KEYBOARD_IN_INTERFACE, use_keygui, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetKeyGUI(void)
{
	return settings.bool_value( MUIOPTION_KEYBOARD_IN_INTERFACE);
}

void SetCycleScreenshot(int cycle_screenshot)
{
	std::string error_string;
	settings.set_value(MUIOPTION_CYCLE_SCREENSHOT, cycle_screenshot, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetCycleScreenshot(void)
{
	return settings.int_value(MUIOPTION_CYCLE_SCREENSHOT);
}

void SetStretchScreenShotLarger(BOOL stretch)
{
	std::string error_string;
	settings.set_value(MUIOPTION_STRETCH_SCREENSHOT_LARGER, stretch, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetStretchScreenShotLarger(void)
{
	return settings.bool_value( MUIOPTION_STRETCH_SCREENSHOT_LARGER);
}

void SetScreenshotBorderSize(int size)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SCREENSHOT_BORDER_SIZE, size, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetScreenshotBorderSize(void)
{
	return settings.int_value(MUIOPTION_SCREENSHOT_BORDER_SIZE);
}

void SetScreenshotBorderColor(COLORREF uColor)
{
	options_set_color_default(settings, MUIOPTION_SCREENSHOT_BORDER_COLOR, uColor, COLOR_3DFACE);
}

COLORREF GetScreenshotBorderColor(void)
{
	return options_get_color_default(settings, MUIOPTION_SCREENSHOT_BORDER_COLOR, COLOR_3DFACE);
}

void SetFilterInherit(BOOL inherit)
{
	std::string error_string;
	settings.set_value(MUIOPTION_INHERIT_FILTER, inherit, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetFilterInherit(void)
{
	return settings.bool_value( MUIOPTION_INHERIT_FILTER);
}

void SetOffsetClones(BOOL offset)
{
	std::string error_string;
	settings.set_value(MUIOPTION_OFFSET_CLONES, offset, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetOffsetClones(void)
{
	return settings.bool_value( MUIOPTION_OFFSET_CLONES);
}

void SetSavedFolderID(UINT val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_DEFAULT_FOLDER_ID, (int) val, OPTION_PRIORITY_CMDLINE,error_string);
}

UINT GetSavedFolderID(void)
{
	return (UINT) settings.int_value(MUIOPTION_DEFAULT_FOLDER_ID);
}

void SetShowScreenShot(BOOL val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SHOW_IMAGE_SECTION, val, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetShowScreenShot(void)
{
	return settings.bool_value(MUIOPTION_SHOW_IMAGE_SECTION);
}

#ifdef MESS
void SetShowSoftware(BOOL val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SHOW_SOFTWARE_SECTION, val, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetShowSoftware(void)
{
	return settings.bool_value(MUIOPTION_SHOW_SOFTWARE_SECTION);
}
#endif

void SetShowFolderList(BOOL val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SHOW_FOLDER_SECTION, val, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetShowFolderList(void)
{
	return settings.bool_value(MUIOPTION_SHOW_FOLDER_SECTION);
}

static void GetsShowFolderFlags(LPBITS bits)
{
	char s[2000];
	extern const FOLDERDATA g_folderData[];
	char *token;

	snprintf(s, ARRAY_LENGTH(s), "%s", settings.value( MUIOPTION_HIDE_FOLDERS));

	SetAllBits(bits, TRUE);

	token = strtok(s,", \t");
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
		token = strtok(NULL,", \t");
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
	char str[10000];
	extern const FOLDERDATA g_folderData[];

	GetsShowFolderFlags(show_folder_flags);

	if (show)
		SetBit(show_folder_flags, folder);
	else
		ClearBit(show_folder_flags, folder);

	strcpy(str, "");

	// we save the ones that are NOT displayed, so we can add new ones
	// and upgraders will see them
	for (i=0; i<MAX_FOLDERS; i++)
	{
		if (TestBit(show_folder_flags, i) == FALSE)
		{
			if (num_saved != 0)
				strcat(str,", ");

			for (j=0; g_folderData[j].m_lpTitle; j++)
			{
				if (g_folderData[j].m_nFolderId == i)
				{
					strcat(str,g_folderData[j].short_name);
					num_saved++;
					break;
				}
			}
		}
	}
	std::string error_string;
	settings.set_value(MUIOPTION_HIDE_FOLDERS, str, OPTION_PRIORITY_CMDLINE,error_string);
	DeleteBits(show_folder_flags);
}

void SetShowStatusBar(BOOL val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SHOW_STATUS_BAR, val, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetShowStatusBar(void)
{
	return settings.bool_value( MUIOPTION_SHOW_STATUS_BAR);
}

void SetShowTabCtrl (BOOL val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SHOW_TABS, val, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetShowTabCtrl (void)
{
	return settings.bool_value( MUIOPTION_SHOW_TABS);
}

void SetShowToolBar(BOOL val)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SHOW_TOOLBAR, val, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetShowToolBar(void)
{
	return settings.bool_value( MUIOPTION_SHOW_TOOLBAR);
}

void SetCurrentTab(const char *shortname)
{
	std::string error_string;
	settings.set_value(MUIOPTION_CURRENT_TAB, shortname, OPTION_PRIORITY_CMDLINE,error_string);
}

const char *GetCurrentTab(void)
{
	return settings.value( MUIOPTION_CURRENT_TAB);
}

void SetDefaultGame(const char *name)
{
	std::string error_string;
	settings.set_value(MUIOPTION_DEFAULT_GAME, name, OPTION_PRIORITY_CMDLINE,error_string);
}

const char *GetDefaultGame(void)
{
	return settings.value( MUIOPTION_DEFAULT_GAME);
}

void SetWindowArea(const AREA *area)
{
	std::string error_string;
	settings.set_value(MUIOPTION_WINDOW_X, area->x, OPTION_PRIORITY_CMDLINE,error_string);
	settings.set_value(MUIOPTION_WINDOW_Y, area->y, OPTION_PRIORITY_CMDLINE,error_string);
	settings.set_value(MUIOPTION_WINDOW_WIDTH, area->width, OPTION_PRIORITY_CMDLINE,error_string);
	settings.set_value(MUIOPTION_WINDOW_HEIGHT, area->height, OPTION_PRIORITY_CMDLINE,error_string);
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
	std::string error_string;
	settings.set_value(MUIOPTION_WINDOW_STATE, (int)state, OPTION_PRIORITY_CMDLINE,error_string);
}

UINT GetWindowState(void)
{
	return settings.int_value(MUIOPTION_WINDOW_STATE);
}

void SetCustomColor(int iIndex, COLORREF uColor)
{
	const char *custom_color_string;
	COLORREF custom_color[256];
	char buffer[80];

	custom_color_string = settings.value( MUIOPTION_CUSTOM_COLOR);
	CusColorDecodeString(custom_color_string, custom_color);

	custom_color[iIndex] = uColor;

	CusColorEncodeString(custom_color, buffer);
	std::string error_string;
	settings.set_value(MUIOPTION_CUSTOM_COLOR, buffer, OPTION_PRIORITY_CMDLINE,error_string);
}

COLORREF GetCustomColor(int iIndex)
{
	const char *custom_color_string;
	COLORREF custom_color[256];

	custom_color_string = settings.value( MUIOPTION_CUSTOM_COLOR);
	CusColorDecodeString(custom_color_string, custom_color);

	if (custom_color[iIndex] == (COLORREF)-1)
		return (COLORREF)RGB(0,0,0);

	return custom_color[iIndex];
}

void SetListFont(const LOGFONT *font)
{
	char font_string[10000];
	FontEncodeString(font, font_string);
	std::string error_string;
	settings.set_value(MUIOPTION_LIST_FONT, font_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetListFont(LOGFONT *font)
{
	const char *font_string = settings.value( MUIOPTION_LIST_FONT);
	FontDecodeString(font_string, font);
}

void SetListFontColor(COLORREF uColor)
{
	options_set_color_default(settings, MUIOPTION_TEXT_COLOR, uColor, COLOR_WINDOWTEXT);
}

COLORREF GetListFontColor(void)
{
	return options_get_color_default(settings, MUIOPTION_TEXT_COLOR, COLOR_WINDOWTEXT);
}

void SetListCloneColor(COLORREF uColor)
{
	options_set_color_default(settings, MUIOPTION_CLONE_COLOR, uColor, COLOR_WINDOWTEXT);
}

COLORREF GetListCloneColor(void)
{
	return options_get_color_default(settings, MUIOPTION_CLONE_COLOR, COLOR_WINDOWTEXT);
}

int GetShowTab(int tab)
{
	const char *show_tabs_string;
	int show_tab_flags = 0;

	show_tabs_string = settings.value( MUIOPTION_HIDE_TABS);
	TabFlagsDecodeString(show_tabs_string, &show_tab_flags);
	return (show_tab_flags & (1 << tab)) != 0;
}

void SetShowTab(int tab,BOOL show)
{
	const char *show_tabs_string;
	int show_tab_flags = 0;
	char buffer[10000];

	show_tabs_string = settings.value( MUIOPTION_HIDE_TABS);
	TabFlagsDecodeString(show_tabs_string, &show_tab_flags);

	if (show)
		show_tab_flags |= 1 << tab;
	else
		show_tab_flags &= ~(1 << tab);

	TabFlagsEncodeString(show_tab_flags, buffer);
	std::string error_string;
	settings.set_value(MUIOPTION_HIDE_TABS, buffer, OPTION_PRIORITY_CMDLINE,error_string);
}

// don't delete the last one
BOOL AllowedToSetShowTab(int tab,BOOL show)
{
	const char *show_tabs_string;
	int show_tab_flags = 0;

	if (show == TRUE)
		return TRUE;

	show_tabs_string = settings.value( MUIOPTION_HIDE_TABS);
	TabFlagsDecodeString(show_tabs_string, &show_tab_flags);

	show_tab_flags &= ~(1 << tab);
	return show_tab_flags != 0;
}

int GetHistoryTab(void)
{
	return settings.int_value(MUIOPTION_HISTORY_TAB);
}

void SetHistoryTab(int tab, BOOL show)
{
	std::string error_string;
	if (show)
		settings.set_value(MUIOPTION_HISTORY_TAB, tab, OPTION_PRIORITY_CMDLINE,error_string);
	else
		settings.set_value(MUIOPTION_HISTORY_TAB, TAB_NONE, OPTION_PRIORITY_CMDLINE,error_string);
}

void SetColumnWidths(int width[])
{
	char column_width_string[80];
	ColumnEncodeStringWithCount(width, column_width_string, COLUMN_MAX);
	std::string error_string;
	settings.set_value(MUIOPTION_COLUMN_WIDTHS, column_width_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetColumnWidths(int width[])
{
	const char *column_width_string;
	column_width_string = settings.value(MUIOPTION_COLUMN_WIDTHS);
	ColumnDecodeStringWithCount(column_width_string, width, COLUMN_MAX);
}

void SetSplitterPos(int splitterId, int pos)
{
	const char *splitter_string;
	int *splitter;
	char buffer[80];

	if (splitterId < GetSplitterCount())
	{
		splitter_string = settings.value(MUIOPTION_SPLITTERS);
		splitter = (int *) alloca(GetSplitterCount() * sizeof(*splitter));
		SplitterDecodeString(splitter_string, splitter);

		splitter[splitterId] = pos;

		SplitterEncodeString(splitter, buffer);
		std::string error_string;
		settings.set_value(MUIOPTION_SPLITTERS, buffer, OPTION_PRIORITY_CMDLINE,error_string);
	}
}

int  GetSplitterPos(int splitterId)
{
	const char *splitter_string;
	int *splitter;

	splitter_string = settings.value( MUIOPTION_SPLITTERS);
	splitter = (int *) alloca(GetSplitterCount() * sizeof(*splitter));
	SplitterDecodeString(splitter_string, splitter);

	if (splitterId < GetSplitterCount())
		return splitter[splitterId];

	return -1; /* Error */
}

void SetColumnOrder(int order[])
{
	char column_order_string[80];
	ColumnEncodeStringWithCount(order, column_order_string, COLUMN_MAX);
	std::string error_string;
	settings.set_value(MUIOPTION_COLUMN_ORDER, column_order_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetColumnOrder(int order[])
{
	const char *column_order_string;
	column_order_string = settings.value( MUIOPTION_COLUMN_ORDER);
	ColumnDecodeStringWithCount(column_order_string, order, COLUMN_MAX);
}

void SetColumnShown(int shown[])
{
	char column_shown_string[80];
	ColumnEncodeStringWithCount(shown, column_shown_string, COLUMN_MAX);
	std::string error_string;
	settings.set_value(MUIOPTION_COLUMN_SHOWN, column_shown_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetColumnShown(int shown[])
{
	const char *column_shown_string;
	column_shown_string = settings.value( MUIOPTION_COLUMN_SHOWN);
	ColumnDecodeStringWithCount(column_shown_string, shown, COLUMN_MAX);
}

void SetSortColumn(int column)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SORT_COLUMN, column, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetSortColumn(void)
{
	return settings.int_value(MUIOPTION_SORT_COLUMN);
}

void SetSortReverse(BOOL reverse)
{
	std::string error_string;
	settings.set_value(MUIOPTION_SORT_REVERSED, reverse, OPTION_PRIORITY_CMDLINE,error_string);
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

const char* GetRomDirs(void)
{
	return global.media_path();
}

void SetRomDirs(const char* paths)
{
	std::string error_string;
	global.set_value(OPTION_MEDIAPATH, paths, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetHashDirs(void)
{
	return global.hash_path();
}

void SetHashDirs(const char* paths)
{
	std::string error_string;
	global.set_value(OPTION_HASHPATH, paths, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetSampleDirs(void)
{
	return global.value(OPTION_SAMPLEPATH);
}

void SetSampleDirs(const char* paths)
{
	std::string error_string;
	global.set_value(OPTION_SAMPLEPATH, paths, OPTION_PRIORITY_CMDLINE,error_string);
}

const char * GetIniDir(void)
{
	const char *ini_dir;
	const char *s;

	ini_dir = global.value(OPTION_INIPATH);
	while((s = strchr(ini_dir, ';')) != NULL)
	{
		ini_dir = s + 1;
	}
	return ini_dir;

}

void SetIniDir(const char *path)
{
	std::string error_string;
	global.set_value(OPTION_INIPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetCtrlrDir(void)
{
	return global.value(OPTION_CTRLRPATH);
}

void SetCtrlrDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_CTRLRPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetSWDir(void)
{
	return global.value(OPTION_SWPATH);
}

void SetSWDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_SWPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetCfgDir(void)
{
	return global.value(OPTION_CFG_DIRECTORY);
}

void SetCfgDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_CFG_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetNvramDir(void)
{
	return global.value(OPTION_NVRAM_DIRECTORY);
}

void SetNvramDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_NVRAM_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetInpDir(void)
{
	return global.value(OPTION_INPUT_DIRECTORY);
}

void SetInpDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_INPUT_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetImgDir(void)
{
	return global.value(OPTION_SNAPSHOT_DIRECTORY);
}

void SetImgDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_SNAPSHOT_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetStateDir(void)
{
	return global.value(OPTION_STATE_DIRECTORY);
}

void SetStateDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_STATE_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetArtDir(void)
{
	return global.value(OPTION_ARTPATH);
}

void SetArtDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_ARTPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetFontDir(void)
{
	return global.value(OPTION_FONTPATH);
}

void SetFontDir(const char* paths)
{
	std::string error_string;
	global.set_value(OPTION_FONTPATH, paths, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetCrosshairDir(void)
{
	return global.value(OPTION_CROSSHAIRPATH);
}

void SetCrosshairDir(const char* paths)
{
	std::string error_string;
	global.set_value(OPTION_CROSSHAIRPATH, paths, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetFlyerDir(void)
{
	return mewui.value(OPTION_FLYERS_PATH);
}

void SetFlyerDir(const char* path)
{
	std::string error_string;
	mewui.set_value(OPTION_FLYERS_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetCabinetDir(void)
{
	return mewui.value(OPTION_CABINETS_PATH);
}

void SetCabinetDir(const char* path)
{
	std::string error_string;
	mewui.set_value(OPTION_CABINETS_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetMarqueeDir(void)
{
	return mewui.value(OPTION_MARQUEES_PATH);
}

void SetMarqueeDir(const char* path)
{
	std::string error_string;
	mewui.set_value(OPTION_MARQUEES_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetTitlesDir(void)
{
	return mewui.value(OPTION_TITLES_PATH);
}

void SetTitlesDir(const char* path)
{
	std::string error_string;
	mewui.set_value(OPTION_TITLES_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char * GetControlPanelDir(void)
{
	return mewui.value(OPTION_CPANELS_PATH);
}

void SetControlPanelDir(const char *path)
{
	std::string error_string;
	mewui.set_value(OPTION_CPANELS_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char * GetPcbDir(void)
{
	return mewui.value(OPTION_PCBS_PATH);
}

void SetPcbDir(const char *path)
{
	std::string error_string;
	mewui.set_value(OPTION_PCBS_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetPluginsDir(void)
{
	return global.value(OPTION_PLUGINSPATH);
}

void SetPluginsDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_PLUGINSPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetLangDir(void)
{
	return global.value(OPTION_LANGUAGEPATH);
}

void SetLangDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_LANGUAGEPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char * GetDiffDir(void)
{
	return global.value(OPTION_DIFF_DIRECTORY);
}

void SetDiffDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_DIFF_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetIconsDir(void)
{
	return settings.value( MUIOPTION_ICONS_DIRECTORY);
}

void SetIconsDir(const char* path)
{
	std::string error_string;
	settings.set_value(MUIOPTION_ICONS_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetBgDir (void)
{
	return settings.value( MUIOPTION_BACKGROUND_DIRECTORY);
}

void SetBgDir (const char* path)
{
	std::string error_string;
	settings.set_value(MUIOPTION_BACKGROUND_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char * GetDatsDir(void)
{
	return settings.value( MUIOPTION_DATS_DIRECTORY);
	//return mewui.value(OPTION_HISTORY_PATH);
}

void SetDatsDir(const char *path)
{
	std::string error_string;
	settings.set_value(MUIOPTION_DATS_DIRECTORY, path, OPTION_PRIORITY_CMDLINE,error_string);
	//mewui.set_value(OPTION_HISTORY_PATH, path, OPTION_PRIORITY_CMDLINE, error_string);
}

const char* GetFolderDir(void)
{
	return mewui.value(OPTION_EXTRAINI_PATH);
}

void SetFolderDir(const char* path)
{
	std::string error_string;
	mewui.set_value(OPTION_EXTRAINI_PATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetCheatDir(void)
{
	return global.value(OPTION_CHEATPATH);
}

void SetCheatDir(const char* path)
{
	std::string error_string;
	global.set_value(OPTION_CHEATPATH, path, OPTION_PRIORITY_CMDLINE,error_string);
}

const char* GetSnapName(void)
{
	return global.value(OPTION_SNAPNAME);
}

void SetSnapName(const char* pattern)
{
	std::string error_string;
	global.set_value(OPTION_SNAPNAME, pattern, OPTION_PRIORITY_CMDLINE,error_string);
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
	{
		/* all games */
		for (int i = 0; i< driver_list::total(); i++)
		{
			ResetPlayVariable(i, play_variable);
		}
	}
	else
	{
		if (strcmp(play_variable, "count") == 0)
			game_opts.play_count(driver_index, 0);
		else if (strcmp(play_variable, "time") == 0)
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

	assert(0 <= driver_index && driver_index < driver_list::total());
	int temp = GetPlayTime(driver_index);
	int hour = temp / 3600;
	temp = temp - 3600*hour;
	int minute = temp / 60; //Calc Minutes
	int second = temp - 60*minute;

	if (hour == 0)
		sprintf(buf, "%d:%02d", minute, second );
	else
		sprintf(buf, "%d:%02d:%02d", hour, minute, second );
}

input_seq* Get_ui_key_up(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_UP);
}

input_seq* Get_ui_key_down(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_DOWN);
}

input_seq* Get_ui_key_left(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_LEFT);
}

input_seq* Get_ui_key_right(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_RIGHT);
}

input_seq* Get_ui_key_start(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_START);
}

input_seq* Get_ui_key_pgup(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_PGUP);
}

input_seq* Get_ui_key_pgdwn(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_PGDWN);
}

input_seq* Get_ui_key_home(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_HOME);
}

input_seq* Get_ui_key_end(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_END);
}

input_seq* Get_ui_key_ss_change(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_SS_CHANGE);
}

input_seq* Get_ui_key_history_up(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_HISTORY_UP);
}

input_seq* Get_ui_key_history_down(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_HISTORY_DOWN);
}

input_seq* Get_ui_key_context_filters(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_CONTEXT_FILTERS);
}

input_seq* Get_ui_key_select_random(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_SELECT_RANDOM);
}

input_seq* Get_ui_key_game_audit(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_GAME_AUDIT);
}

input_seq* Get_ui_key_game_properties(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_GAME_PROPERTIES);
}

input_seq* Get_ui_key_help_contents(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_HELP_CONTENTS);
}

input_seq* Get_ui_key_update_gamelist(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_UPDATE_GAMELIST);
}

input_seq* Get_ui_key_view_folders(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_FOLDERS);
}

input_seq* Get_ui_key_view_fullscreen(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_FULLSCREEN);
}

input_seq* Get_ui_key_view_pagetab(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_PAGETAB);
}

input_seq* Get_ui_key_view_picture_area(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_PICTURE_AREA);
}

#ifdef MESS
input_seq* Get_ui_key_view_software_area(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_SOFTWARE_AREA);
}
#endif

input_seq* Get_ui_key_view_status(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_STATUS);
}

input_seq* Get_ui_key_view_toolbars(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TOOLBARS);
}

input_seq* Get_ui_key_view_tab_cabinet(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_CABINET);
}

input_seq* Get_ui_key_view_tab_cpanel(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_CPANEL);
}

input_seq* Get_ui_key_view_tab_flyer(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_FLYER);
}

input_seq* Get_ui_key_view_tab_history(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_HISTORY);
}

input_seq* Get_ui_key_view_tab_marquee(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_MARQUEE);
}

input_seq* Get_ui_key_view_tab_screenshot(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_SCREENSHOT);
}

input_seq* Get_ui_key_view_tab_title(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_TITLE);
}

input_seq* Get_ui_key_view_tab_pcb(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_VIEW_TAB_PCB);
}

input_seq* Get_ui_key_quit(void)
{
	return options_get_input_seq(settings, MUIOPTION_UI_KEY_QUIT);
}

static int GetUIJoy(const char *option_name, int joycodeIndex)
{
	const char *joycodes_string;
	int joycodes[4];

	assert(0 <= joycodeIndex && joycodeIndex < 4);
	joycodes_string = settings.value( option_name);
	ColumnDecodeStringWithCount(joycodes_string, joycodes, ARRAY_LENGTH(joycodes));
	return joycodes[joycodeIndex];
}

static void SetUIJoy(const char *option_name, int joycodeIndex, int val)
{
	const char *joycodes_string;
	int joycodes[4];
	char buffer[1024];

	assert(0 <= joycodeIndex && joycodeIndex < 4);
	joycodes_string = settings.value( option_name);
	ColumnDecodeStringWithCount(joycodes_string, joycodes, ARRAY_LENGTH(joycodes));

	joycodes[joycodeIndex] = val;
	ColumnEncodeStringWithCount(joycodes, buffer, ARRAY_LENGTH(joycodes));
	std::string error_string;
	settings.set_value(option_name, buffer, OPTION_PRIORITY_CMDLINE,error_string);
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
	return settings.value( MUIOPTION_EXEC_COMMAND);
}

void SetExecCommand(char *cmd)
{
	std::string error_string;
	settings.set_value(MUIOPTION_EXEC_COMMAND, cmd, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetExecWait(void)
{
	return settings.int_value(MUIOPTION_EXEC_WAIT);
}

void SetExecWait(int wait)
{
	std::string error_string;
	settings.set_value(MUIOPTION_EXEC_WAIT, wait, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetHideMouseOnStartup(void)
{
	return settings.bool_value( MUIOPTION_HIDE_MOUSE);
}

void SetHideMouseOnStartup(BOOL hide)
{
	std::string error_string;
	settings.set_value(MUIOPTION_HIDE_MOUSE, hide, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetRunFullScreen(void)
{
	return settings.bool_value( MUIOPTION_FULL_SCREEN);
}

void SetRunFullScreen(BOOL fullScreen)
{
	std::string error_string;
	settings.set_value(MUIOPTION_FULL_SCREEN, fullScreen, OPTION_PRIORITY_CMDLINE,error_string);
}

/***************************************************************************
    Internal functions
 ***************************************************************************/

static void  CusColorEncodeString(const COLORREF *value, char* str)
{
	char tmpStr[256];

	sprintf(tmpStr, "%u", (int) value[0]);

	strcpy(str, tmpStr);

	for (int i = 1; i < 16; i++)
	{
		sprintf(tmpStr, ",%u", (unsigned) value[i]);
		strcat(str, tmpStr);
	}
}

static void CusColorDecodeString(const char* str, COLORREF *value)
{
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


void ColumnEncodeStringWithCount(const int *value, char *str, int count)
{
	char buffer[256];

	snprintf(buffer,sizeof(buffer),"%d",value[0]);

	strcpy(str,buffer);

	for (int i = 1; i < count; i++)
	{
		snprintf(buffer,sizeof(buffer),",%d",value[i]);
		strcat(str,buffer);
	}
}

void ColumnDecodeStringWithCount(const char* str, int *value, int count)
{
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

static void SplitterEncodeString(const int *value, char* str)
{
	char tmpStr[256];

	sprintf(tmpStr, "%d", value[0]);

	strcpy(str, tmpStr);

	for (int i = 1; i < GetSplitterCount(); i++)
	{
		sprintf(tmpStr, ",%d", value[i]);
		strcat(str, tmpStr);
	}
}

static void SplitterDecodeString(const char *str, int *value)
{
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
static void FontDecodeString(const char* str, LOGFONT *f)
{
	const char* ptr;
	TCHAR* t_ptr;

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
	ptr = strrchr(str, ',');
	if (ptr)
	{
		t_ptr = ui_wstring_from_utf8(ptr + 1);
		if( !t_ptr )
			return;
		_tcscpy(f->lfFaceName, t_ptr);
		osd_free(t_ptr);
	}
}

/* Encode the given LOGFONT structure into a comma-delimited string */
static void FontEncodeString(const LOGFONT *f, char *str)
{
	char* utf8_FaceName = ui_utf8_from_wstring(f->lfFaceName);
	if( !utf8_FaceName )
		return;

	sprintf(str, "%li,%li,%li,%li,%li,%i,%i,%i,%i,%i,%i,%i,%i,%s",
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

	osd_free(utf8_FaceName);
}

static void TabFlagsEncodeString(int data, char *str)
{
	int num_saved = 0;

	strcpy(str,"");

	// we save the ones that are NOT displayed, so we can add new ones
	// and upgraders will see them
	for ( int i=0; i<MAX_TAB_TYPES; i++)
	{
		if (((data & (1 << i)) == 0) && GetImageTabShortName(i))
		{
			if (num_saved > 0)
				strcat(str, ", ");

			strcat(str,GetImageTabShortName(i));
			num_saved++;
		}
	}
}

static void TabFlagsDecodeString(const char *str, int *data)
{
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

static void LoadSettingsFile(ui_options &opts, const char *filename)
{
	osd_file::error filerr;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_READ, file);
	if (filerr == osd_file::error::NONE)
	{
		std::string error_string;
		opts.parse_ini_file(*file, OPTION_PRIORITY_CMDLINE, OPTION_PRIORITY_CMDLINE, error_string);
		file.reset();
	}
}

static void LoadSettingsFile(winui_options &opts, const char *filename)
{
	osd_file::error filerr;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_READ, file);
	if (filerr == osd_file::error::NONE)
	{
		std::string error_string;
		opts.parse_ini_file(*file, OPTION_PRIORITY_CMDLINE, OPTION_PRIORITY_CMDLINE, error_string);
		file.reset();
	}
}

static void LoadSettingsFile(windows_options &opts, const char *filename)
{
	osd_file::error filerr;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_READ, file);
	if (filerr == osd_file::error::NONE)
	{
		std::string error_string;
		opts.parse_ini_file(*file, OPTION_PRIORITY_CMDLINE, OPTION_PRIORITY_CMDLINE, error_string);
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
		std::string inistring = opts.output_ini();
		file->puts(inistring.c_str());
		file.reset();
	}
}



// This saves changes to MAMEUI.INI only
static void SaveSettingsFile(winui_options &opts, const char *filename)
{
	osd_file::error filerr = osd_file::error::NONE;
	util::core_file::ptr file;

	filerr = util::core_file::open(filename, OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS, file);

	if (filerr == osd_file::error::NONE)
	{
		std::string inistring = opts.output_ini();
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
		std::string inistring = opts.output_ini();
		// printf("=====%s=====\n%s\n",filename,inistring.c_str());  // for debugging
		file->puts(inistring.c_str());
		file.reset();
	}
}



/* Register access functions below */
static void LoadOptionsAndSettings(void)
{
//	char buffer[MAX_PATH];

	// parse MAMEui.ini - always in the current directory.
	LoadSettingsFile(settings, UI_INI_FILENAME);
	LoadSettingsFile(mewui, MEWUI_FILENAME);

	// parse GameInfo.ini - game options.
	game_opts.load_file(GAMEINFO_INI_FILENAME);

	// parse global options ini/mame32.ini
	load_options(global, OPTIONS_GLOBAL, GLOBAL_OPTIONS);
}

void SetDirectories(windows_options &opts)
{
	std::string error_string;
	opts.set_value(OPTION_MEDIAPATH, GetRomDirs(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_SAMPLEPATH, GetSampleDirs(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_INIPATH, GetIniDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_CFG_DIRECTORY, GetCfgDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_SNAPSHOT_DIRECTORY, GetImgDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_INPUT_DIRECTORY, GetInpDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_STATE_DIRECTORY, GetStateDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_ARTPATH, GetArtDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_NVRAM_DIRECTORY, GetNvramDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_CTRLRPATH, GetCtrlrDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_CHEATPATH, GetCheatDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_CROSSHAIRPATH, GetCrosshairDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_FONTPATH, GetFontDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_DIFF_DIRECTORY, GetDiffDir(), OPTION_PRIORITY_CMDLINE, error_string);
	opts.set_value(OPTION_SNAPNAME, GetSnapName(), OPTION_PRIORITY_CMDLINE, error_string);
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
static DWORD DecodeFolderFlags(const char *buf)
{
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
	winui_options opts;
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
				if (*ptr == ' ')
				{
					*ptr = '_';
				}
				ptr++;
			}

			std::string option_name = std::string(folder_name) + "_filters";
			// create entry
			entries[0].name = option_name.c_str();
			opts.add_entries(entries);
		}
	}

	// These are overlaid at the end of our UI ini
	// The normal read will skip them.
	LoadSettingsFile(opts, UI_INI_FILENAME);

	// retrieve the stored values
	for (i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);

		if (lpFolder)
		{
			char folder_name[256];
			char *ptr;
			const char *value;

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if (*ptr == ' ')
				{
					*ptr = '_';
				}
				ptr++;
			}
			std::string option_name = std::string(folder_name) + "_filters";
			// get entry and decode it
			value = opts.value(option_name.c_str());

			if (value)
				lpFolder->m_dwFlags |= DecodeFolderFlags(value) & F_MASK;
		}
	}
}



// Adds our folder flags to a temporary winui_options, for saving.
static void AddFolderFlags(winui_options &opts)
{
	int numFolders = 0, num_entries = 0;
	LPTREEFOLDER lpFolder;
	options_entry entries[2] = { { 0 }, { 0 } };

	entries[0].name = NULL;
	entries[0].defvalue = NULL;
	entries[0].flags = OPTION_HEADER;
	entries[0].description = "FOLDER FILTERS";
	opts.add_entries(entries);

	memcpy(entries, filterOptions, sizeof(filterOptions));

	numFolders = GetNumFolders();

	for (int i = 0; i < numFolders; i++)
	{
		lpFolder = GetFolder(i);
		if (lpFolder && (lpFolder->m_dwFlags & F_MASK) != 0)
		{
			char folder_name[256];
			char *ptr;

			// Convert spaces to underscores
			strcpy(folder_name, lpFolder->m_lpTitle);
			ptr = folder_name;
			while (*ptr && *ptr != '\0')
			{
				if (*ptr == ' ')
				{
					*ptr = '_';
				}
				ptr++;
			}

			std::string option_name = std::string(folder_name) + "_filters";

			// create entry
			entries[0].name = option_name.c_str();
			opts.add_entries(entries);

			// store entry
			std::string error_string;
			opts.set_value(option_name.c_str(), EncodeFolderFlags(lpFolder->m_dwFlags), OPTION_PRIORITY_CMDLINE,error_string);

			// increment counter
			num_entries++;
		}
	}
}

// Save MAMEUI.ini
void SaveOptions(void)
{
	// Add the folder flag to settings.
	AddFolderFlags(settings);
	// Save opts if it is non-null, else save settings.
	// It will be null if there are no filters set.
	SaveSettingsFile(settings, UI_INI_FILENAME);
	SaveSettingsFile(mewui, MEWUI_FILENAME);
}

void SaveGameListOptions(void)
{
	// Save GameInfo.ini - game options.
	game_opts.save_file(GAMEINFO_INI_FILENAME);
}

void SaveDefaultOptions(void)
{
	std::string fname = std::string(GetIniDir()) + PATH_SEPARATOR + std::string(emulator_info::get_configname()).append(".ini");
	SaveSettingsFile(global, fname.c_str());
}

const char * GetVersionString(void)
{
	return emulator_info::get_build_version();
}


/*  get options, based on passed in game number. */
void load_options(windows_options &opts, OPTIONS_TYPE opt_type, int game_num)
{
	const game_driver *driver = NULL;
	if (game_num > -1)
		driver = &driver_list::driver(game_num);
	CreateGameOptions(opts, opt_type, game_num);

	// Try base ini first
	std::string fname = std::string(emulator_info::get_configname()).append(".ini");
	LoadSettingsFile(opts, fname.c_str());

	if (opt_type == OPTIONS_SOURCE)
	{
		fname = std::string(GetIniDir()) + PATH_SEPARATOR + "source" + PATH_SEPARATOR + core_filename_extract_base(driver->source_file, true) + ".ini";
		LoadSettingsFile(opts, fname.c_str());
		return;
	}

	if (game_num > -2)
	{
		driver = &driver_list::driver(game_num);
		// Now try global ini
		fname = std::string(GetIniDir()) + PATH_SEPARATOR + std::string(emulator_info::get_configname()).append(".ini");
		LoadSettingsFile(opts, fname.c_str());

		if (game_num > -1)
		{
			// global swpath serves a different purpose than for games, so blank it out
			std::string error_string;
			opts.set_value(OPTION_SWPATH, "", OPTION_PRIORITY_CMDLINE,error_string);
			// Lastly, gamename.ini
			if (driver)
			{
				fname = std::string(GetIniDir()) + PATH_SEPARATOR + std::string(driver->name).append(".ini");
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
	std::string filename, filepath;

	if (game_num >= 0)
	{
		driver = &driver_list::driver(game_num);
		if (driver)
		{
			filename.assign(driver->name);
			if (opt_type == OPTIONS_SOURCE)
				filepath = std::string(GetIniDir()) + PATH_SEPARATOR + "source" + PATH_SEPARATOR + core_filename_extract_base(driver->source_file, true) + ".ini";
		}
	}
	else
	if (game_num == -1)
		filename = std::string(emulator_info::get_configname());

	if (!filename.empty() && filepath.empty())
		filepath = std::string(GetIniDir()).append(PATH_SEPARATOR).append(filename.c_str()).append(".ini");

	if (game_num == -2)
		filepath = std::string(emulator_info::get_configname()).append(".ini");

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
	opts.revert(priority);
}

int GetDriverCache(int driver_index)
{
	return game_opts.cache(driver_index);
}

void SetDriverCache(int driver_index, int val)
{
	game_opts.cache(driver_index, val);
}

BOOL RequiredDriverCache(void)
{
	bool ret = false;

	if ( strcmp(settings.value(MUIOPTION_VERSION), GetVersionString()) != 0 )
		ret = true;

	std::string error_string;
	settings.set_value(MUIOPTION_VERSION, GetVersionString(), OPTION_PRIORITY_CMDLINE,error_string);

	return ret;
}

// from optionsms.cpp (MESSUI)

#define MESSUI_SL_COLUMN_SHOWN        "sl_column_shown"
#define MESSUI_SL_COLUMN_WIDTHS       "sl_column_widths"
#define MESSUI_SL_COLUMN_ORDER        "sl_column_order"
#define MESSUI_SL_SORT_REVERSED       "sl_sort_reversed"
#define MESSUI_SL_SORT_COLUMN         "sl_sort_column"
#define MESSUI_SW_COLUMN_SHOWN        "sw_column_shown"
#define MESSUI_SW_COLUMN_WIDTHS       "sw_column_widths"
#define MESSUI_SW_COLUMN_ORDER        "sw_column_order"
#define MESSUI_SW_SORT_REVERSED       "sw_sort_reversed"
#define MESSUI_SW_SORT_COLUMN         "sw_sort_column"
#define MESSUI_SOFTWARE_TAB           "current_software_tab"
#define MESSUI_SLPATH                 "slpath"

#define LOG_SOFTWARE 0

static const options_entry mess_wingui_settings[] =
{
	{ MESSUI_SL_COLUMN_WIDTHS,        "100,75,223,46,120,120", OPTION_STRING, NULL },
	{ MESSUI_SL_COLUMN_ORDER,         "0,1,2,3,4,5", OPTION_STRING, NULL }, // order of columns
	{ MESSUI_SL_COLUMN_SHOWN,         "1,1,1,1,1,1", OPTION_STRING, NULL }, // 0=hide,1=show
	{ MESSUI_SL_SORT_COLUMN,          "0", OPTION_INTEGER, NULL },
	{ MESSUI_SL_SORT_REVERSED,        "0", OPTION_BOOLEAN, NULL },
	{ MESSUI_SW_COLUMN_WIDTHS,        "400", OPTION_STRING, NULL },
	{ MESSUI_SW_COLUMN_ORDER,         "0", OPTION_STRING, NULL }, // 1= dummy column
	{ MESSUI_SW_COLUMN_SHOWN,         "1", OPTION_STRING, NULL }, // 0=don't show it
	{ MESSUI_SW_SORT_COLUMN,          "0", OPTION_INTEGER, NULL },
	{ MESSUI_SW_SORT_REVERSED,        "0", OPTION_BOOLEAN, NULL },
	{ MESSUI_SOFTWARE_TAB,            "0", OPTION_STRING, NULL },
	{ MESSUI_SLPATH,                  "software", OPTION_STRING, NULL },
	{ NULL }
};

void MessSetupSettings(winui_options &settings)
{
	settings.add_entries(mess_wingui_settings);
}

void MessSetupGameOptions(windows_options &opts, OPTIONS_TYPE opt_type, int driver_index)
{
	if (driver_index >= 0)
		mameopts.set_system_name(opts, driver_list::driver(driver_index).name);
}

void SetSLColumnOrder(int order[])
{
	char column_order_string[80];
	ColumnEncodeStringWithCount(order, column_order_string, SL_COLUMN_MAX);
	std::string error_string;
	MameUISettings().set_value(MESSUI_SL_COLUMN_ORDER, column_order_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetSLColumnOrder(int order[])
{
	const char *column_order_string;
	column_order_string = MameUISettings().value(MESSUI_SL_COLUMN_ORDER);
	ColumnDecodeStringWithCount(column_order_string, order, SL_COLUMN_MAX);
}

void SetSLColumnShown(int shown[])
{
	char column_shown_string[80];
	ColumnEncodeStringWithCount(shown, column_shown_string, SL_COLUMN_MAX);
	std::string error_string;
	MameUISettings().set_value(MESSUI_SL_COLUMN_SHOWN, column_shown_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetSLColumnShown(int shown[])
{
	const char *column_shown_string;
	column_shown_string = MameUISettings().value(MESSUI_SL_COLUMN_SHOWN);
	ColumnDecodeStringWithCount(column_shown_string, shown, SL_COLUMN_MAX);
}

void SetSLColumnWidths(int width[])
{
	char column_width_string[80];
	ColumnEncodeStringWithCount(width, column_width_string, SL_COLUMN_MAX);
	std::string error_string;
	MameUISettings().set_value(MESSUI_SL_COLUMN_WIDTHS, column_width_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetSLColumnWidths(int width[])
{
	const char *column_width_string;
	column_width_string = MameUISettings().value(MESSUI_SL_COLUMN_WIDTHS);
	ColumnDecodeStringWithCount(column_width_string, width, SL_COLUMN_MAX);
}

void SetSLSortColumn(int column)
{
	std::string error_string;
	MameUISettings().set_value(MESSUI_SL_SORT_COLUMN, column, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetSLSortColumn(void)
{
	return MameUISettings().int_value(MESSUI_SL_SORT_COLUMN);
}

void SetSLSortReverse(BOOL reverse)
{
	std::string error_string;
	MameUISettings().set_value( MESSUI_SL_SORT_REVERSED, reverse, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetSLSortReverse(void)
{
	return MameUISettings().bool_value(MESSUI_SL_SORT_REVERSED);
}

void SetSWColumnOrder(int order[])
{
	char column_order_string[80];
	ColumnEncodeStringWithCount(order, column_order_string, SW_COLUMN_MAX);
	std::string error_string;
	MameUISettings().set_value(MESSUI_SW_COLUMN_ORDER, column_order_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetSWColumnOrder(int order[])
{
	const char *column_order_string;
	column_order_string = MameUISettings().value(MESSUI_SW_COLUMN_ORDER);
	ColumnDecodeStringWithCount(column_order_string, order, SW_COLUMN_MAX);
}

void SetSWColumnShown(int shown[])
{
	char column_shown_string[80];
	ColumnEncodeStringWithCount(shown, column_shown_string, SW_COLUMN_MAX);
	std::string error_string;
	MameUISettings().set_value(MESSUI_SW_COLUMN_SHOWN, column_shown_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetSWColumnShown(int shown[])
{
	const char *column_shown_string;
	column_shown_string = MameUISettings().value(MESSUI_SW_COLUMN_SHOWN);
	ColumnDecodeStringWithCount(column_shown_string, shown, SW_COLUMN_MAX);
}

void SetSWColumnWidths(int width[])
{
	char column_width_string[80];
	ColumnEncodeStringWithCount(width, column_width_string, SW_COLUMN_MAX);
	std::string error_string;
	MameUISettings().set_value(MESSUI_SW_COLUMN_WIDTHS, column_width_string, OPTION_PRIORITY_CMDLINE,error_string);
}

void GetSWColumnWidths(int width[])
{
	const char *column_width_string;
	column_width_string = MameUISettings().value(MESSUI_SW_COLUMN_WIDTHS);
	ColumnDecodeStringWithCount(column_width_string, width, SW_COLUMN_MAX);
}

void SetSWSortColumn(int column)
{
	std::string error_string;
	MameUISettings().set_value(MESSUI_SW_SORT_COLUMN, column, OPTION_PRIORITY_CMDLINE,error_string);
}

int GetSWSortColumn(void)
{
	return MameUISettings().int_value(MESSUI_SW_SORT_COLUMN);
}

void SetSWSortReverse(BOOL reverse)
{
	std::string error_string;
	MameUISettings().set_value( MESSUI_SW_SORT_REVERSED, reverse, OPTION_PRIORITY_CMDLINE,error_string);
}

BOOL GetSWSortReverse(void)
{
	return MameUISettings().bool_value(MESSUI_SW_SORT_REVERSED);
}

void SetSelectedSoftware(int driver_index, const machine_config *config, const device_image_interface *dev, const char *software)
{
	const char *opt_name = dev->instance_name();
	windows_options o;
	std::string error_string;

	if (LOG_SOFTWARE)
	{
		dprintf("SetSelectedSoftware(): dev=%p (\'%s\') software='%s'\n", dev, driver_list::driver(driver_index).name, software);
	}

	load_options(o, OPTIONS_GAME, driver_index);
	o.set_value(opt_name, software, OPTION_PRIORITY_CMDLINE,error_string);
	save_options(o, OPTIONS_GAME, driver_index);
}

// not used
const char *GetSelectedSoftware(int driver_index, const machine_config *config, const device_image_interface *dev)
{
	const char *opt_name = dev->instance_name();
	windows_options o;

	load_options(o, OPTIONS_GAME, driver_index);
	return o.value(opt_name);
}

void SetCurrentSoftwareTab(const char *shortname)
{
	std::string error_string;
	MameUISettings().set_value(MESSUI_SOFTWARE_TAB, shortname, OPTION_PRIORITY_CMDLINE,error_string);
}

const char *GetCurrentSoftwareTab(void)
{
	return MameUISettings().value(MESSUI_SOFTWARE_TAB);
}

const char* GetSLDir(void)
{
	return MameUISettings().value(MESSUI_SLPATH);
}

void SetSLDir(const char* paths)
{
	std::string error_string;
	MameUISettings().set_value(MESSUI_SLPATH, paths, OPTION_PRIORITY_CMDLINE,error_string);
}

