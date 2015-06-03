// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIRECTORIES_H
#define DIRECTORIES_H

/* Dialog return codes */
#define DIRDLG_ROMS         0x0010
#define DIRDLG_SAMPLES      0x0020
#define DIRDLG_INI		    0x0040
#define DIRDLG_CFG          0x0100
#define DIRDLG_IMG          0x0400
#define DIRDLG_INP          0x0800
#define DIRDLG_CTRLR        0x1000
#define DIRDLG_SOFTWARE		0x2000
#define DIRDLG_COMMENT      0x4000
#define DIRDLG_CHEAT        0x8000

#define DIRLIST_NEWENTRYTEXT "<               >"

typedef struct
{
	LPCSTR   lpName;
	LPCSTR   (*pfnGetTheseDirs)(void);
	void     (*pfnSetTheseDirs)(LPCSTR lpDirs);
	BOOL     bMulti;
	int      nDirDlgFlags;
}
DIRECTORYINFO;

/* in layout[ms].c */
extern const DIRECTORYINFO g_directoryInfo[];

INT_PTR CALLBACK DirectoriesDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif /* DIRECTORIES_H */


