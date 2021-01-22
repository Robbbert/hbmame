// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_DIRECTORIES_H
#define WINUI_DIRECTORIES_H

/* Dialog return codes - do these do anything??? */
#define DIRDLG_ROMS         0x0010  // this one does
#define DIRDLG_SAMPLES      0x0020  // this one does
//#define DIRDLG_INI          0x0040
//#define DIRDLG_CFG          0x0100
//#define DIRDLG_IMG          0x0400
//#define DIRDLG_INP          0x0800
//#define DIRDLG_CTRLR        0x1000
#define DIRDLG_SW           0x4000  // this one does
//#define DIRDLG_CHEAT        0x8000

#define DIRLIST_NEWENTRYTEXT "<               >"

#include "mui_opts.h"

typedef struct
{
	LPCSTR   lpName;                                 // name to display
	const std::string   (*pfnGetTheseDirs)(void);    // function to get existing setting
	void     (*pfnSetTheseDirs)(LPCSTR lpDirs);      // function to save new setting
	int      dir_index;                              // entry number in emu_opts:dir_map
	BOOL     bMulti;                                 // true = it supports multiple directories
	int      nDirDlgFlags;                           // if changed, a refresh needs to be done
}
DIRECTORYINFO;

const DIRECTORYINFO g_directoryInfo[] =
{
	{ "ROMs",                  nullptr,            nullptr,            2,  true,  DIRDLG_ROMS },
	{ "Samples",               nullptr,            nullptr,            4,  true,  DIRDLG_SAMPLES },
	{ "Software File Base",    nullptr,            nullptr,            13, false, DIRDLG_SW }, // core cannot handle multiple path, even though we can.
	{ "Artwork",               nullptr,            nullptr,            5,  true, 0 },
	{ "Artwork Previews",      nullptr,            nullptr,            32, true, 0 },
	{ "Bosses",                nullptr,            nullptr,            33, true, 0 },
	{ "Cabinets",              nullptr,            nullptr,            25, true, 0 },
	{ "Cheats",                nullptr,            nullptr,            9,  true, 0 }, //DIRDLG_CHEAT },  //not used anywhere
	{ "Config",                nullptr,            nullptr,            14, false, 0 }, //DIRDLG_CFG },  //not used anywhere
	{ "Control Panels",        nullptr,            nullptr,            26, true, 0 },
	{ "Controller Files",      nullptr,            nullptr,            6,  true, 0 }, //DIRDLG_CTRLR },  //not used anywhere
	{ "Covers",                nullptr,            nullptr,            41, true, 0 },
	{ "Crosshairs",            nullptr,            nullptr,            10, true, 0 },
	{ "DAT files",             nullptr,            nullptr,            23, false, 0 },
	{ "Ends",                  nullptr,            nullptr,            30, true, 0 },
	{ "Flyers",                nullptr,            nullptr,            28, true, 0 },
	{ "Folders",               nullptr,            nullptr,            24, false, 0 },
	{ "Fonts",                 nullptr,            nullptr,            8,  true, 0 },
	{ "Game Overs",            nullptr,            nullptr,            37, true, 0 },
	{ "Hash",                  nullptr,            nullptr,            3,  true, 0 },
	{ "Hard Drive Difference", nullptr,            nullptr,            19, true, 0 },
	{ "HLSL",                  nullptr,            nullptr,            22, false, 0 },
	{ "How To",                nullptr,            nullptr,            38, true, 0 },
	{ "Icons",                 nullptr,            nullptr,            40, true, 0 },
//	{ "Ini Files",             GetIniDir,          nullptr,            7,  false, DIRDLG_INI },  // 2017-02-03 hardcoded to 'ini' now
	{ "Input files",           nullptr,            nullptr,            16, true, 0 }, //DIRDLG_INP },  //not used anywhere
	{ "Language",              nullptr,            nullptr,            12, false, 0 },
	{ "Logos",                 nullptr,            nullptr,            34, true, 0 },
	{ "Manuals (PDF)",         GetManualsDir,      SetManualsDir,      0,  false, 0 },
	{ "Marquees",              nullptr,            nullptr,            31, true, 0 },
	{ "NVRAM",                 nullptr,            nullptr,            15, true, 0 },
	{ "PCBs",                  nullptr,            nullptr,            27, true, 0 },
	{ "Plugins",               nullptr,            nullptr,            11, false, 0 },
	{ "Scores",                nullptr,            nullptr,            35, true, 0 },
	{ "Selects",               nullptr,            nullptr,            39, true, 0 },
	{ "Snapshots",             nullptr,            nullptr,            18, true, 0 }, //DIRDLG_IMG },  //not used anywhere
	{ "State",                 nullptr,            nullptr,            17, true, 0 },
	{ "Titles",                nullptr,            nullptr,            29, true, 0 },
	{ "Versus",                nullptr,            nullptr,            36, true, 0 },
	{ "Videos and Movies",     GetVideoDir,        SetVideoDir,        0,  false, 0 },
	{ NULL }
};


INT_PTR CALLBACK DirectoriesDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif /* DIRECTORIES_H */


