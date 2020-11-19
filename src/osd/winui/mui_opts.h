// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_MUI_OPTS_H
#define WINUI_MUI_OPTS_H

#include "winmain.h"
#include "winui.h"

// List of columns in the main game list
enum
{
	COLUMN_GAMES = 0,
	COLUMN_SRCDRIVERS,
	COLUMN_DIRECTORY,
	COLUMN_TYPE,
	COLUMN_ORIENTATION,
	COLUMN_MANUFACTURER,
	COLUMN_YEAR,
	COLUMN_PLAYED,
	COLUMN_PLAYTIME,
	COLUMN_CLONE,
	COLUMN_TRACKBALL,
	COLUMN_SAMPLES,
	COLUMN_ROMS,
	COLUMN_MAX
};

#define LOCAL_OPTIONS   -10

typedef struct
{
	int x, y, width, height;
} AREA;

typedef struct
{
	char *screen;
	char *aspect;
	char *resolution;
	char *view;
} ScreenParams;

// List of artwork types to display in the screen shot area
enum
{
	// these must match array of strings image_tabs_long_name in mui_opts.cpp
	// if you add new Tabs, be sure to also add them to the ComboBox init in dialogs.cpp
	TAB_ARTWORK = 0,
	TAB_BOSSES,
	TAB_CABINET,
	TAB_CONTROL_PANEL,
	TAB_COVER,
	TAB_ENDS,
	TAB_FLYER,
	TAB_GAMEOVER,
	TAB_HOWTO,
	TAB_LOGO,
	TAB_MARQUEE,
	TAB_PCB,
	TAB_SCORES,
	TAB_SELECT,
	TAB_SCREENSHOT,
	TAB_TITLE,
	TAB_VERSUS,
	TAB_HISTORY,
	MAX_TAB_TYPES,
	BACKGROUND,
	TAB_ALL,
	TAB_NONE
};
// Because we have added the Options after MAX_TAB_TYPES, we have to subtract 3 here
// (that's how many options we have after MAX_TAB_TYPES)
#define TAB_SUBTRACT 3

void OptionsInit(void);

#define OPTIONS_TYPE_GLOBAL -1
#define OPTIONS_TYPE_FOLDER -2

void LoadFolderFlags(void);

// Start interface to directories.h

const string GetManualsDir(void);
void SetManualsDir(const char* path);

const string GetVideoDir(void);
void SetVideoDir(const char *path);

// End interface to directories.h

void mui_save_ini(void);
void SaveGameListOptions(void);

void ResetGUI(void);


const char * GetImageTabLongName(int tab_index);
const char * GetImageTabShortName(int tab_index);

void SetViewMode(int val);
int  GetViewMode(void);

void SetGameCheck(BOOL game_check);
BOOL GetGameCheck(void);

void SetJoyGUI(BOOL use_joygui);
BOOL GetJoyGUI(void);

void SetKeyGUI(BOOL use_keygui);
BOOL GetKeyGUI(void);

void SetCycleScreenshot(int cycle_screenshot);
int GetCycleScreenshot(void);

void SetStretchScreenShotLarger(BOOL stretch);
BOOL GetStretchScreenShotLarger(void);

void SetScreenshotBorderSize(int size);
int GetScreenshotBorderSize(void);

void SetScreenshotBorderColor(COLORREF uColor);
COLORREF GetScreenshotBorderColor(void);

void SetFilterInherit(BOOL inherit);
BOOL GetFilterInherit(void);

void SetOffsetClones(BOOL offset);
BOOL GetOffsetClones(void);

void SetSavedFolderID(UINT val);
UINT GetSavedFolderID(void);

void SetOverrideRedX(BOOL val);
BOOL GetOverrideRedX(void);

BOOL GetShowFolder(int folder);
void SetShowFolder(int folder,BOOL show);

void SetShowStatusBar(BOOL val);
BOOL GetShowStatusBar(void);

void SetShowToolBar(BOOL val);
BOOL GetShowToolBar(void);

void SetShowTabCtrl(BOOL val);
BOOL GetShowTabCtrl(void);

void SetCurrentTab(int val);
int GetCurrentTab(void);

void SetDefaultGame(int val);
uint32_t GetDefaultGame(void);

void SetWindowArea(const AREA *area);
void GetWindowArea(AREA *area);

void SetWindowState(UINT state);
UINT GetWindowState(void);

void SetWindowPanes(int val);
UINT GetWindowPanes(void);

void SetColumnWidths(int widths[]);
void GetColumnWidths(int widths[]);

void SetColumnOrder(int order[]);
void GetColumnOrder(int order[]);

void SetColumnShown(int shown[]);
void GetColumnShown(int shown[]);

void SetSplitterPos(int splitterId, int pos);
int  GetSplitterPos(int splitterId);

void SetCustomColor(int iIndex, COLORREF uColor);
COLORREF GetCustomColor(int iIndex);

void SetListFont(const LOGFONT *font);
void GetListFont(LOGFONT *font);

DWORD GetFolderFlags(int folder_index);

void SetListFontColor(COLORREF uColor);
COLORREF GetListFontColor(void);

void SetListCloneColor(COLORREF uColor);
COLORREF GetListCloneColor(void);

int GetHistoryTab(void);
void SetHistoryTab(int tab,BOOL show);

int GetShowTab(int tab);
void SetShowTab(int tab,BOOL show);
BOOL AllowedToSetShowTab(int tab,BOOL show);

void SetSortColumn(int column);
int  GetSortColumn(void);

void SetSortReverse(BOOL reverse);
BOOL GetSortReverse(void);

const string GetBgDir(void);
void SetBgDir(const char *path);

int GetRomAuditResults(uint32_t driver_index);
void SetRomAuditResults(uint32_t driver_index, int audit_results);

int GetSampleAuditResults(uint32_t driver_index);
void SetSampleAuditResults(uint32_t driver_index, int audit_results);

void IncrementPlayCount(uint32_t driver_index);
uint32_t GetPlayCount(uint32_t driver_index);
void ResetPlayCount(int driver_index);

void IncrementPlayTime(uint32_t driver_index, uint32_t playtime);
uint32_t GetPlayTime(uint32_t driver_index);
void GetTextPlayTime(uint32_t driver_index, char *buf);
void ResetPlayTime(int driver_index);

const char * GetVersionString(void);




// Keyboard control of ui
input_seq* Get_ui_key_up(void);
input_seq* Get_ui_key_down(void);
input_seq* Get_ui_key_left(void);
input_seq* Get_ui_key_right(void);
input_seq* Get_ui_key_start(void);
input_seq* Get_ui_key_pgup(void);
input_seq* Get_ui_key_pgdwn(void);
input_seq* Get_ui_key_home(void);
input_seq* Get_ui_key_end(void);
input_seq* Get_ui_key_ss_change(void);
input_seq* Get_ui_key_history_up(void);
input_seq* Get_ui_key_history_down(void);

input_seq* Get_ui_key_context_filters(void);
input_seq* Get_ui_key_select_random(void);
input_seq* Get_ui_key_game_audit(void);
input_seq* Get_ui_key_game_properties(void);
input_seq* Get_ui_key_help_contents(void);
input_seq* Get_ui_key_update_gamelist(void);
input_seq* Get_ui_key_view_folders(void);
input_seq* Get_ui_key_view_fullscreen(void);
input_seq* Get_ui_key_view_pagetab(void);
input_seq* Get_ui_key_view_picture_area(void);
input_seq* Get_ui_key_view_software_area(void);
input_seq* Get_ui_key_view_status(void);
input_seq* Get_ui_key_view_toolbars(void);

input_seq* Get_ui_key_view_tab_cabinet(void);
input_seq* Get_ui_key_view_tab_cpanel(void);
input_seq* Get_ui_key_view_tab_flyer(void);
input_seq* Get_ui_key_view_tab_history(void);
input_seq* Get_ui_key_view_tab_marquee(void);
input_seq* Get_ui_key_view_tab_screenshot(void);
input_seq* Get_ui_key_view_tab_title(void);
input_seq* Get_ui_key_view_tab_pcb(void);
input_seq* Get_ui_key_quit(void);


int GetUIJoyUp(int joycodeIndex);
void SetUIJoyUp(int joycodeIndex, int val);

int GetUIJoyDown(int joycodeIndex);
void SetUIJoyDown(int joycodeIndex, int val);

int GetUIJoyLeft(int joycodeIndex);
void SetUIJoyLeft(int joycodeIndex, int val);

int GetUIJoyRight(int joycodeIndex);
void SetUIJoyRight(int joycodeIndex, int val);

int GetUIJoyStart(int joycodeIndex);
void SetUIJoyStart(int joycodeIndex, int val);

int GetUIJoyPageUp(int joycodeIndex);
void SetUIJoyPageUp(int joycodeIndex, int val);

int GetUIJoyPageDown(int joycodeIndex);
void SetUIJoyPageDown(int joycodeIndex, int val);

int GetUIJoyHome(int joycodeIndex);
void SetUIJoyHome(int joycodeIndex, int val);

int GetUIJoyEnd(int joycodeIndex);
void SetUIJoyEnd(int joycodeIndex, int val);

int GetUIJoySSChange(int joycodeIndex);
void SetUIJoySSChange(int joycodeIndex, int val);

int GetUIJoyHistoryUp(int joycodeIndex);
void SetUIJoyHistoryUp(int joycodeIndex, int val);

int GetUIJoyHistoryDown(int joycodeIndex);
void SetUIJoyHistoryDown(int joycodeIndex, int val);

int GetUIJoyExec(int joycodeIndex);
void SetUIJoyExec(int joycodeIndex, int val);

const string GetExecCommand(void);
void SetExecCommand(char *cmd);

int GetExecWait(void);
void SetExecWait(int wait);

BOOL GetHideMouseOnStartup(void);
void SetHideMouseOnStartup(BOOL hide);

BOOL GetRunFullScreen(void);
void SetRunFullScreen(BOOL fullScreen);

uint32_t GetDriverCacheLower(uint32_t driver_index);
uint32_t GetDriverCacheUpper(uint32_t driver_index);
void SetDriverCache(uint32_t driver_index, uint32_t val);
BOOL RequiredDriverCache(void);
void ForceRebuild(void);
BOOL DriverIsComputer(uint32_t driver_index);
BOOL DriverIsConsole(uint32_t driver_index);
BOOL DriverIsModified(uint32_t driver_index);
BOOL DriverIsImperfect(uint32_t driver_index);
string GetGameName(uint32_t driver_index);

// from optionsms.h (MESSUI)

enum
{
	SW_COLUMN_IMAGES,
	SW_COLUMN_MAX
};

enum
{
	SL_COLUMN_IMAGES,
	SL_COLUMN_GOODNAME,
	SL_COLUMN_MANUFACTURER,
	SL_COLUMN_YEAR,
	SL_COLUMN_PLAYABLE,
	SL_COLUMN_USAGE,
	SL_COLUMN_MAX
};

void SetSWColumnWidths(int widths[]);
void GetSWColumnWidths(int widths[]);
void SetSWColumnOrder(int order[]);
void GetSWColumnOrder(int order[]);
void SetSWColumnShown(int shown[]);
void GetSWColumnShown(int shown[]);
void SetSWSortColumn(int column);
int  GetSWSortColumn(void);
void SetSWSortReverse(BOOL reverse);
BOOL GetSWSortReverse(void);

void SetSLColumnWidths(int widths[]);
void GetSLColumnWidths(int widths[]);
void SetSLColumnOrder(int order[]);
void GetSLColumnOrder(int order[]);
void SetSLColumnShown(int shown[]);
void GetSLColumnShown(int shown[]);
void SetSLSortColumn(int column);
int  GetSLSortColumn(void);
void SetSLSortReverse(BOOL reverse);
BOOL GetSLSortReverse(void);


void SetCurrentSoftwareTab(int val);
int GetCurrentSoftwareTab(void);


#endif

