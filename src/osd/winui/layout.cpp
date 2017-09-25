// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

/***************************************************************************

  layout.cpp

  MAME specific TreeView definitions (and maybe more in the future)

***************************************************************************/
// standard windows headers
#include <windows.h>
#include <commctrl.h>

// MAME/MAMEUI headers
#include "bitmask.h"
#include "treeview.h"
#include "emu.h"
#include "mui_util.h"
#include "resource.h"
#include "mui_opts.h"
#include "splitters.h"
#include "help.h"
#include "mui_audit.h"
#include "properties.h"


static BOOL FilterAvailable(uint32_t driver_index)
{
	return !DriverUsesRoms(driver_index) || IsAuditResultYes(GetRomAuditResults(driver_index));
}

#ifdef MESS
extern const FOLDERDATA g_folderData[] =
{
	{"All Systems",     "allgames",          FOLDER_ALLGAMES,     IDI_FOLDER,               0,             0,            0, NULL,                       NULL,                    TRUE },
	{"Available",       "available",         FOLDER_AVAILABLE,    IDI_FOLDER_AVAILABLE,     F_AVAILABLE,   0,            0, NULL,                       FilterAvailable,         TRUE },
	{"BIOS",            "bios",              FOLDER_BIOS,         IDI_FOLDER,               0,             0,            1, CreateBIOSFolders,          DriverIsBios,            TRUE },
	{"CHD",             "harddisk",          FOLDER_HARDDISK,     IDI_HARDDISK,             0,             0,            0, NULL,                       DriverIsHarddisk,        TRUE },
	{"Clones",          "clones",            FOLDER_CLONES,       IDI_FOLDER,               F_CLONES,      F_ORIGINALS,  0, NULL,                       DriverIsClone,           TRUE },
	{"Computer",        "computer",          FOLDER_COMPUTER,     IDI_FOLDER,               F_COMPUTER,    F_CONSOLE,    0, NULL,                       DriverIsComputer,        TRUE },
	{"Console",         "console",           FOLDER_CONSOLE,      IDI_FOLDER,               F_CONSOLE,     F_COMPUTER,   0, NULL,                       DriverIsConsole,         TRUE },
	{"CPU",             "cpu",               FOLDER_CPU,          IDI_FOLDER,               0,             0,            1, CreateCPUFolders },
	{"Dumping Status",  "dumping",           FOLDER_DUMPING,      IDI_FOLDER,               0,             0,            1, CreateDumpingFolders },
	{"FPS",             "fps",               FOLDER_FPS,          IDI_FOLDER,               0,             0,            1, CreateFPSFolders },
	{"Horizontal",      "horizontal",        FOLDER_HORIZONTAL,   IDI_FOLDER_HORIZONTAL,    F_HORIZONTAL,  F_VERTICAL,   0, NULL,                       DriverIsVertical,        FALSE },
	{"Imperfect",       "imperfect",         FOLDER_DEFICIENCY,   IDI_FOLDER,               0,             0,            0, CreateDeficiencyFolders },
	{"Lightgun",        "Lightgun",          FOLDER_LIGHTGUN,     IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesLightGun,      TRUE },
	{"Manufacturer",    "manufacturer",      FOLDER_MANUFACTURER, IDI_FOLDER_MANUFACTURER,  0,             0,            1, CreateManufacturerFolders },
	{"Mechanical",      "mechanical",        FOLDER_MECHANICAL,   IDI_FOLDER,               0,             0,            0, NULL,                       DriverIsMechanical,      TRUE },
	{"Modified/Hacked", "modified",          FOLDER_MODIFIED,     IDI_FOLDER,               0,             0,            0, NULL,                       DriverIsModified,        TRUE },
	{"Mouse",           "mouse",             FOLDER_MOUSE,        IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesMouse,         TRUE },
	{"Non Mechanical",  "nonmechanical",     FOLDER_NONMECHANICAL,IDI_FOLDER,               0,             0,            0, NULL,                       DriverIsMechanical,      FALSE },
	{"Not Working",     "nonworking",        FOLDER_NONWORKING,   IDI_NONWORKING,           F_NONWORKING,  F_WORKING,    0, NULL,                       DriverIsBroken,          TRUE },
	{"Originals",       "originals",         FOLDER_ORIGINAL,     IDI_FOLDER,               F_ORIGINALS,   F_CLONES,     0, NULL,                       DriverIsClone,           FALSE },
	{"Raster",          "raster",            FOLDER_RASTER,       IDI_FOLDER,               F_RASTER,      F_VECTOR,     0, NULL,                       DriverIsVector,          FALSE },
	{"Resolution",      "resolution",        FOLDER_RESOLUTION,   IDI_FOLDER,               0,             0,            1, CreateResolutionFolders },
	{"Samples",         "samples",           FOLDER_SAMPLES,      IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesSamples,       TRUE },
	{"Save State",      "savestate",         FOLDER_SAVESTATE,    IDI_FOLDER,               0,             0,            0, NULL,                       DriverSupportsSaveState, TRUE },
	{"Screens",         "screens",           FOLDER_SCREENS,      IDI_FOLDER,               0,             0,            1, CreateScreenFolders },
	{"Sound",           "sound",             FOLDER_SND,          IDI_FOLDER,               0,             0,            1, CreateSoundFolders },
	{"Source",          "source",            FOLDER_SOURCE,       IDI_FOLDER_SOURCE,        0,             0,            1, CreateSourceFolders },
	{"Stereo",          "stereo",            FOLDER_STEREO,       IDI_SOUND,                0,             0,            0, NULL,                       DriverIsStereo,          TRUE },
	{"Trackball",       "trackball",         FOLDER_TRACKBALL,    IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesTrackball,     TRUE },
	{"Unavailable",     "unavailable",       FOLDER_UNAVAILABLE,  IDI_FOLDER_UNAVAILABLE,   0,             F_AVAILABLE,  0, NULL,                       FilterAvailable,         FALSE },
	{"Vector",          "vector",            FOLDER_VECTOR,       IDI_FOLDER,               F_VECTOR,      F_RASTER,     0, NULL,                       DriverIsVector,          TRUE },
	{"Vertical",        "vertical",          FOLDER_VERTICAL,     IDI_FOLDER_VERTICAL,      F_VERTICAL,    F_HORIZONTAL, 0, NULL,                       DriverIsVertical,        TRUE },
	{"Working",         "working",           FOLDER_WORKING,      IDI_WORKING,              F_WORKING,     F_NONWORKING, 0, NULL,                       DriverIsBroken,          FALSE },
	{"Year",            "year",              FOLDER_YEAR,         IDI_FOLDER_YEAR,          0,             0,            1, CreateYearFolders },
	{ NULL }
};
#else
extern const FOLDERDATA g_folderData[] =
{
	{"All Games",       "allgames",          FOLDER_ALLGAMES,     IDI_FOLDER,               0,             0,            0, NULL,                       NULL,                    TRUE },
	{"Available",       "available",         FOLDER_AVAILABLE,    IDI_FOLDER_AVAILABLE,     F_AVAILABLE,   0,            0, NULL,                       FilterAvailable,         TRUE },
	{"BIOS",            "bios",              FOLDER_BIOS,         IDI_FOLDER,               0,             0,            1, CreateBIOSFolders,          DriverIsBios,            TRUE },
	{"CHD",             "harddisk",          FOLDER_HARDDISK,     IDI_HARDDISK,             0,             0,            0, NULL,                       DriverIsHarddisk,        TRUE },
	{"Clones",          "clones",            FOLDER_CLONES,       IDI_FOLDER,               F_CLONES,      F_ORIGINALS,  0, NULL,                       DriverIsClone,           TRUE },
	{"CPU",             "cpu",               FOLDER_CPU,          IDI_CPU,                  0,             0,            1, CreateCPUFolders },
	{"Dumping Status",  "dumping",           FOLDER_DUMPING,      IDI_FOLDER,               0,             0,            1, CreateDumpingFolders },
	{"FPS",             "fps",               FOLDER_FPS,          IDI_FOLDER,               0,             0,            1, CreateFPSFolders },
	{"Horizontal",      "horizontal",        FOLDER_HORIZONTAL,   IDI_FOLDER_HORIZONTAL,    F_HORIZONTAL,  F_VERTICAL,   0, NULL,                       DriverIsVertical,        FALSE },
	{"Imperfect",       "imperfect",         FOLDER_DEFICIENCY,   IDI_FOLDER,               0,             0,            0, CreateDeficiencyFolders },
	{"Lightgun",        "Lightgun",          FOLDER_LIGHTGUN,     IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesLightGun,      TRUE },
	{"Manufacturer",    "manufacturer",      FOLDER_MANUFACTURER, IDI_FOLDER_MANUFACTURER,  0,             0,            1, CreateManufacturerFolders },
	{"Mechanical",      "mechanical",        FOLDER_MECHANICAL,   IDI_FOLDER,               0,             0,            0, NULL,                       DriverIsMechanical,      TRUE },
	{"Non Mechanical",  "nonmechanical",     FOLDER_NONMECHANICAL,IDI_FOLDER,               0,             0,            0, NULL,                       DriverIsMechanical,      FALSE },
	{"Not Working",     "nonworking",        FOLDER_NONWORKING,   IDI_NONWORKING,           F_NONWORKING,  F_WORKING,    0, NULL,                       DriverIsBroken,          TRUE },
	{"Originals",       "originals",         FOLDER_ORIGINAL,     IDI_FOLDER,               F_ORIGINALS,   F_CLONES,     0, NULL,                       DriverIsClone,           FALSE },
	{"Raster",          "raster",            FOLDER_RASTER,       IDI_FOLDER,               F_RASTER,      F_VECTOR,     0, NULL,                       DriverIsVector,          FALSE },
	{"Resolution",      "resolution",        FOLDER_RESOLUTION,   IDI_FOLDER,               0,             0,            1, CreateResolutionFolders },
	{"Samples",         "samples",           FOLDER_SAMPLES,      IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesSamples,       TRUE },
	{"Save State",      "savestate",         FOLDER_SAVESTATE,    IDI_FOLDER,               0,             0,            0, NULL,                       DriverSupportsSaveState, TRUE },
	{"Screens",         "screens",           FOLDER_SCREENS,      IDI_FOLDER,               0,             0,            1, CreateScreenFolders },
	{"Sound",           "sound",             FOLDER_SND,          IDI_SOUND,                0,             0,            1, CreateSoundFolders },
	{"Source",          "source",            FOLDER_SOURCE,       IDI_FOLDER_SOURCE,        0,             0,            1, CreateSourceFolders },
	{"Stereo",          "stereo",            FOLDER_STEREO,       IDI_FOLDER,               0,             0,            0, NULL,                       DriverIsStereo,          TRUE },
	{"Trackball",       "trackball",         FOLDER_TRACKBALL,    IDI_FOLDER,               0,             0,            0, NULL,                       DriverUsesTrackball,     TRUE },
	{"Unavailable",     "unavailable",       FOLDER_UNAVAILABLE,  IDI_FOLDER_UNAVAILABLE,   0,             F_AVAILABLE,  0, NULL,                       FilterAvailable,         FALSE },
	{"Vector",          "vector",            FOLDER_VECTOR,       IDI_FOLDER,               F_VECTOR,      F_RASTER,     0, NULL,                       DriverIsVector,          TRUE },
	{"Vertical",        "vertical",          FOLDER_VERTICAL,     IDI_FOLDER_VERTICAL,      F_VERTICAL,    F_HORIZONTAL, 0, NULL,                       DriverIsVertical,        TRUE },
	{"Working",         "working",           FOLDER_WORKING,      IDI_WORKING,              F_WORKING,     F_NONWORKING, 0, NULL,                       DriverIsBroken,          FALSE },
	{"Year",            "year",              FOLDER_YEAR,         IDI_FOLDER_YEAR,          0,             0,            1, CreateYearFolders },
	{ NULL }
};
#endif

/* list of filter/control Id pairs */
#ifdef MESS
extern const FILTER_ITEM g_filterList[] =
{
	{ F_VECTOR,       IDC_FILTER_VECTOR,      DriverIsVector, TRUE },
	{ F_RASTER,       IDC_FILTER_RASTER,      DriverIsVector, FALSE },
	{ F_CLONES,       IDC_FILTER_CLONES,      DriverIsClone, TRUE },
	{ F_ORIGINALS,    IDC_FILTER_ORIGINALS,   DriverIsClone, FALSE },
	{ F_NONWORKING,   IDC_FILTER_NONWORKING,  DriverIsBroken, TRUE },
	{ F_WORKING,      IDC_FILTER_WORKING,     DriverIsBroken, FALSE },
	{ F_HORIZONTAL,   IDC_FILTER_HORIZONTAL,  DriverIsVertical, FALSE },
	{ F_VERTICAL,     IDC_FILTER_VERTICAL,    DriverIsVertical, TRUE },
	{ F_COMPUTER,     IDC_FILTER_COMPUTER,    DriverIsComputer, TRUE },
	{ F_CONSOLE,      IDC_FILTER_CONSOLE,     DriverIsConsole, TRUE },
	{ F_UNAVAILABLE,  IDC_FILTER_UNAVAILABLE, FilterAvailable, FALSE },
	{ F_MECHANICAL,   IDC_FILTER_MECHANICAL,  DriverIsMechanical, TRUE },
	{ F_ARCADE,       IDC_FILTER_ARCADE,      DriverIsArcade, TRUE },
	{ F_MESS,         IDC_FILTER_MESS,        DriverIsArcade, FALSE },
//	{ F_MODIFIED,     IDC_FILTER_MODIFIED,    DriverIsModified, TRUE },
//	{ F_AVAILABLE,    IDC_FILTER_AVAILABLE,   FilterAvailable, TRUE },
	{ 0 }
};
#else
extern const FILTER_ITEM g_filterList[] =
{
	{ F_CLONES,       IDC_FILTER_CLONES,      DriverIsClone, TRUE },
	{ F_NONWORKING,   IDC_FILTER_NONWORKING,  DriverIsBroken, TRUE },
	{ F_UNAVAILABLE,  IDC_FILTER_UNAVAILABLE, FilterAvailable, FALSE },
	{ F_RASTER,       IDC_FILTER_RASTER,      DriverIsVector, FALSE },
	{ F_VECTOR,       IDC_FILTER_VECTOR,      DriverIsVector, TRUE },
	{ F_ORIGINALS,    IDC_FILTER_ORIGINALS,   DriverIsClone, FALSE },
	{ F_WORKING,      IDC_FILTER_WORKING,     DriverIsBroken, FALSE },
	{ F_AVAILABLE,    IDC_FILTER_AVAILABLE,   FilterAvailable, TRUE },
	{ F_HORIZONTAL,   IDC_FILTER_HORIZONTAL,  DriverIsVertical, FALSE },
	{ F_VERTICAL,     IDC_FILTER_VERTICAL,    DriverIsVertical, TRUE },
	{ F_MECHANICAL,   IDC_FILTER_MECHANICAL,  DriverIsMechanical, TRUE },
	{ F_ARCADE,       IDC_FILTER_ARCADE,      DriverIsArcade, TRUE },
	{ F_MESS,         IDC_FILTER_MESS,        DriverIsArcade, FALSE },
	{ 0 }
};
#endif

#ifdef MESS
extern const MAMEHELPINFO g_helpInfo[] =
{
	//{ ID_HELP_CONTENTS,    TRUE,  TEXT(MAMEUIHELP"::/windows/main.htm") },
	{ ID_HELP_CONTENTS,    TRUE,  TEXT(MAMEUIHELP) }, // 0 - call up CHM file
	//{ ID_HELP_RELEASE,     TRUE,  TEXT(MAMEUIHELP) },
	//{ ID_HELP_WHATS_NEW,   TRUE,  TEXT(MAMEUIHELP"::/messnew.txt") },
	{ ID_HELP_WHATS_NEW,   TRUE,  TEXT("") }, // 1 - call up whatsnew at mamedev.org
	{ -1 }
};
#else
extern const MAMEHELPINFO g_helpInfo[] =
{
	{ ID_HELP_CONTENTS,    TRUE,  TEXT(MAMEUIHELP) },
	//{ ID_HELP_WHATS_NEWUI, TRUE,  TEXT(MAMEUIHELP"::/html/mameui_changes.txt") },
	//{ ID_HELP_TROUBLE,     TRUE,  TEXT(MAMEUIHELP"::/html/mameui_support.htm") },
	//{ ID_HELP_RELEASE,     FALSE, TEXT("windows.txt") },
	{ ID_HELP_WHATS_NEW,   TRUE,  TEXT(MAMEUIHELP"::/docs/whatsnew.txt") },
	{ -1 }
};
#endif

#ifdef MESS
extern const PROPERTYSHEETINFO g_propSheets[] =
{
	{ FALSE, NULL,                   IDD_PROP_GAME,          GamePropertiesDialogProc },
	{ FALSE, NULL,                   IDD_PROP_AUDIT,         GameAuditDialogProc },
	{ TRUE,  NULL,                   IDD_PROP_DISPLAY,       GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_ADVANCED,      GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_SCREEN,        GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_SOUND,         GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_INPUT,         GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_CONTROLLER,    GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_DEBUG,         GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_MISC,          GameOptionsProc },
	{ FALSE, NULL,                   IDD_PROP_SOFTWARE,      GameMessOptionsProc },
	{ FALSE, DriverHasRam,           IDD_PROP_CONFIGURATION, GameMessOptionsProc }, // PropSheetFilter_Config not needed
	{ TRUE,  DriverIsVector,         IDD_PROP_VECTOR,        GameOptionsProc },     // PropSheetFilter_Vector not needed
	{ FALSE }
};
#else
extern const PROPERTYSHEETINFO g_propSheets[] =
{
	{ FALSE, NULL,                   IDD_PROP_GAME,          GamePropertiesDialogProc },
	{ FALSE, NULL,                   IDD_PROP_AUDIT,         GameAuditDialogProc },
	{ TRUE,  NULL,                   IDD_PROP_DISPLAY,       GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_ADVANCED,      GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_SCREEN,        GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_SOUND,         GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_INPUT,         GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_CONTROLLER,    GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_MISC,          GameOptionsProc },
	{ TRUE,  NULL,                   IDD_PROP_DEBUG,         GameOptionsProc },
	{ TRUE,  DriverIsVector,         IDD_PROP_VECTOR,        GameOptionsProc },
	{ FALSE }
};
#endif

#ifdef MESS
extern const ICONDATA g_iconData[] =
{
	{ IDI_WIN_NOROMS,        "noroms" },
	{ IDI_WIN_ROMS,          "roms" },
	{ IDI_WIN_UNKNOWN,       "unknown" },
	{ IDI_WIN_CLONE,         "clone" },
	{ IDI_WIN_REDX,          "warning" },
	{ IDI_WIN_IMPERFECT,     "imperfect" },
	{ IDI_WIN_NOROMSNEEDED,  "noromsneeded" },
	{ IDI_WIN_MISSINGOPTROM, "missingoptrom" },
	{ IDI_WIN_FLOP,          "floppy" },
	{ IDI_WIN_CASS,          "cassette" },
	{ IDI_WIN_SERL,          "serial" },
	{ IDI_WIN_SNAP,          "snapshot" },
	{ IDI_WIN_PRIN,          "printer" },
	{ IDI_WIN_HARD,          "hard" },
	{ IDI_WIN_MIDI,          "midi" },
	{ 0 }
};
#else
extern const ICONDATA g_iconData[] =
{
	{ IDI_WIN_NOROMS,        "noroms" },
	{ IDI_WIN_ROMS,          "roms" },
	{ IDI_WIN_UNKNOWN,       "unknown" },
	{ IDI_WIN_CLONE,         "clone" },
	{ IDI_WIN_REDX,          "warning" },
	{ IDI_WIN_IMPERFECT,     "imperfect" },
	{ 0 }
};
#endif

#ifdef MESS
extern const TCHAR g_szPlayGameString[] = TEXT("&Run %s");
extern const char g_szGameCountString[] = "%d machines";
#else
extern const TCHAR g_szPlayGameString[] = TEXT("&Play %s");
extern const char g_szGameCountString[] = "%d games";
#endif
