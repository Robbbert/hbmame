// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIRECTORIES_H
#define DIRECTORIES_H

/* Dialog return codes */
#define DIRDLG_ROMS         0x0010
#define DIRDLG_SAMPLES      0x0020
#define DIRDLG_INI          0x0040
#define DIRDLG_CFG          0x0100
#define DIRDLG_IMG          0x0400
#define DIRDLG_INP          0x0800
#define DIRDLG_CTRLR        0x1000
#define DIRDLG_SOFTWARE     0x2000
#define DIRDLG_COMMENT      0x4000
#define DIRDLG_CHEAT        0x8000

#define DIRLIST_NEWENTRYTEXT "<               >"

#include "mui_opts.h"
#ifdef MESS
#include "optionsms.h"
#endif

typedef struct
{
	LPCSTR   lpName;
	LPCSTR   (*pfnGetTheseDirs)(void);
	void     (*pfnSetTheseDirs)(LPCSTR lpDirs);
	BOOL     bMulti;
	int      nDirDlgFlags;
}
DIRECTORYINFO;

const DIRECTORYINFO g_directoryInfo[] =
{
	{ "ROMs",                  GetRomDirs,         SetRomDirs,         TRUE,  DIRDLG_ROMS },
	{ "Samples",               GetSampleDirs,      SetSampleDirs,      TRUE,  DIRDLG_SAMPLES },
#ifdef MESS
	{ "Software File Base",    GetCommentDir,      SetCommentDir,      TRUE,  DIRDLG_COMMENT },
	{ "Software List Base",    GetSoftwareDirs,    SetSoftwareDirs,    FALSE, DIRDLG_SOFTWARE },
#endif
	{ "Artwork",               GetArtDir,          SetArtDir,          TRUE, 0 },
	{ "Cabinets",              GetCabinetDir,      SetCabinetDir,      TRUE, 0 },
	{ "Cheats",                GetCheatDir,        SetCheatDir,        TRUE, DIRDLG_CHEAT },
	{ "Config",                GetCfgDir,          SetCfgDir,          FALSE, DIRDLG_CFG },
	{ "Control Panels",        GetControlPanelDir, SetControlPanelDir, TRUE, 0 },
	{ "Controller Files",      GetCtrlrDir,        SetCtrlrDir,        TRUE, DIRDLG_CTRLR },
	{ "Crosshairs",            GetCrosshairDir,    SetCrosshairDir,    TRUE, 0 },
	{ "DAT files",             GetDatsDir,         SetDatsDir,         FALSE, 0 },
	{ "Folders",               GetFolderDir,       SetFolderDir,       FALSE, 0 },
	{ "Fonts",                 GetFontDir,         SetFontDir,         TRUE, 0 },
	{ "Flyers",                GetFlyerDir,        SetFlyerDir,        TRUE, 0 },
	{ "Hash",                  GetHashDirs,        SetHashDirs,        TRUE, 0 },
	{ "Hard Drive Difference", GetDiffDir,         SetDiffDir,         TRUE, 0 },
	{ "Icons",                 GetIconsDir,        SetIconsDir,        FALSE, 0 },
	{ "Ini Files",             GetIniDir,          SetIniDir,          FALSE, DIRDLG_INI },
	{ "Input files",           GetInpDir,          SetInpDir,          TRUE, DIRDLG_INP },
	{ "Marquees",              GetMarqueeDir,      SetMarqueeDir,      TRUE, 0 },
	{ "NVRAM",                 GetNvramDir,        SetNvramDir,        TRUE, 0 },
	{ "PCBs",                  GetPcbDir,          SetPcbDir,          TRUE, 0 },
	{ "Snapshots",             GetImgDir,          SetImgDir,          TRUE, DIRDLG_IMG },
	{ "State",                 GetStateDir,        SetStateDir,        TRUE, 0 },
	{ "Titles",                GetTitlesDir,       SetTitlesDir,       TRUE, 0 },
	{ NULL }
};


INT_PTR CALLBACK DirectoriesDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif /* DIRECTORIES_H */


