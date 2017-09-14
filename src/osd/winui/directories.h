// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef DIRECTORIES_H
#define DIRECTORIES_H

/* Dialog return codes - do these do anything??? */
#define DIRDLG_ROMS         0x0010  // this one does
#define DIRDLG_SAMPLES      0x0020  // this one does
#define DIRDLG_INI          0x0040
#define DIRDLG_CFG          0x0100
#define DIRDLG_IMG          0x0400
#define DIRDLG_INP          0x0800
#define DIRDLG_CTRLR        0x1000
#define DIRDLG_SW           0x4000  // this one does
#define DIRDLG_CHEAT        0x8000

#define DIRLIST_NEWENTRYTEXT "<               >"

#include "mui_opts.h"

typedef struct
{
	LPCSTR   lpName;                                 // name to display
	const std::string   (*pfnGetTheseDirs)(void);    // function to get existing setting
	void     (*pfnSetTheseDirs)(LPCSTR lpDirs);      // function to save new setting
	BOOL     bMulti;                                 // true = it supports multiple directories
	int      nDirDlgFlags;                           // if changed, a refresh needs to be done
}
DIRECTORYINFO;

const DIRECTORYINFO g_directoryInfo[] =
{
	{ "ROMs",                  GetRomDirs,         SetRomDirs,         true,  DIRDLG_ROMS },
	{ "Samples",               GetSampleDirs,      SetSampleDirs,      true,  DIRDLG_SAMPLES },
	{ "Software File Base",    GetSWDir,           SetSWDir,           false, DIRDLG_SW }, // core cannot handle multiple path, even though we can.
	{ "Artwork",               GetArtDir,          SetArtDir,          true, 0 },
	{ "Cabinets",              GetCabinetDir,      SetCabinetDir,      true, 0 },
	{ "Cheats",                GetCheatDir,        SetCheatDir,        true, DIRDLG_CHEAT },
	{ "Config",                GetCfgDir,          SetCfgDir,          false, DIRDLG_CFG },
	{ "Control Panels",        GetControlPanelDir, SetControlPanelDir, true, 0 },
	{ "Controller Files",      GetCtrlrDir,        SetCtrlrDir,        true, DIRDLG_CTRLR },
	{ "Crosshairs",            GetCrosshairDir,    SetCrosshairDir,    true, 0 },
	{ "DAT files",             GetDatsDir,         SetDatsDir,         false, 0 },
	{ "Flyers",                GetFlyerDir,        SetFlyerDir,        true, 0 },
	{ "Folders",               GetFolderDir,       SetFolderDir,       false, 0 },
	{ "Fonts",                 GetFontDir,         SetFontDir,         true, 0 },
	{ "Hash",                  GetHashDirs,        SetHashDirs,        true, 0 },
	{ "Hard Drive Difference", GetDiffDir,         SetDiffDir,         true, 0 },
	{ "Icons",                 GetIconsDir,        SetIconsDir,        false, 0 },
//	{ "Ini Files",             GetIniDir,          SetIniDir,          false, DIRDLG_INI },  // 2017-02-03 hardcoded to 'ini' now
	{ "Input files",           GetInpDir,          SetInpDir,          true, DIRDLG_INP },
	{ "Language",              GetLangDir,         SetLangDir,         false, 0 },
	{ "Manuals (PDF)",         GetManualsDir,      SetManualsDir,      false, 0 },
	{ "Marquees",              GetMarqueeDir,      SetMarqueeDir,      true, 0 },
	{ "NVRAM",                 GetNvramDir,        SetNvramDir,        true, 0 },
	{ "PCBs",                  GetPcbDir,          SetPcbDir,          true, 0 },
	{ "Plugins",               GetPluginsDir,      SetPluginsDir,      false, 0 },
	{ "Snapshots",             GetImgDir,          SetImgDir,          true, DIRDLG_IMG },
	{ "State",                 GetStateDir,        SetStateDir,        true, 0 },
	{ "Titles",                GetTitlesDir,       SetTitlesDir,       true, 0 },
	{ "Videos and Movies",     GetVideoDir,        SetVideoDir,        false, 0 },
	{ NULL }
};


INT_PTR CALLBACK DirectoriesDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif /* DIRECTORIES_H */


