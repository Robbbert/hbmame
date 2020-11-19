// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_SCREENSHOT_H
#define WINUI_SCREENSHOT_H

typedef struct _mybitmapinfo
{
	int bmWidth;
	int bmHeight;
	int bmColors;
} MYBITMAPINFO, *LPMYBITMAPINFO;

extern BOOL LoadScreenShot(int nGame, LPCSTR lpSoftwareName, int nType);
extern HANDLE GetScreenShotHandle(void);
extern int GetScreenShotWidth(void);
extern int GetScreenShotHeight(void);

extern void FreeScreenShot(void);
extern BOOL ScreenShotLoaded(void);

extern BOOL LoadDIBBG(HGLOBAL *phDIB, HPALETTE *pPal);
extern HBITMAP DIBToDDB(HDC hDC, HANDLE hDIB, LPMYBITMAPINFO desc);

#if 0
typedef struct
{
	TEXT            tabtext;
	const char*     zipname;
	int             tabenum;
	const string    (*pfnGetTheseDirs)(void);
}
SSINFO;

const SSINFO m_ssInfo[] =
{
	{ "Snapshot",       "snap",      TAB_SCREENSHOT,     GetImgDir          },
	{ "Flyer",          "flyers",    TAB_FLYER,          GetFlyerDir        },
	{ "Cabinet",        "cabinets",  TAB_CABINET,        GetCabinetDir      },
	{ "Marquee",        "marquees",  TAB_MARQUEE,        GetMarqueeDir      },
	{ "Title",          "titles",    TAB_TITLE,          GetTitleDir        },
	{ "Control Panel",  "cpanel",    TAB_CONTROL_PANEL,  GetControlPanelDir },
	{ "PCB",            "pcb",       TAB_PCB,            GetPcbDir          },
};

/* if adding a new tab, need to also update:
- dialogs.cpp (~50)                (history on tab or not)
- winui.cpp   MameCommand  (~4173) (action the mouse click in the menu)
- mameui.rc                        (show in menu)
- ui_opts.h                        (tab enabled or not)
- resource.h
- mui_opts.cpp/h                   (directory get function)
- mui_opts.h                       enum of names
- mui_opts.cpp                     image_tabs_long_name
- screenshot.cpp (~316)            (choose image to display)
*/

#endif
#endif
