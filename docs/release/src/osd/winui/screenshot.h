// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

typedef struct _mybitmapinfo
{
	int bmWidth;
	int bmHeight;
	int bmColors;
} MYBITMAPINFO, *LPMYBITMAPINFO;

extern BOOL LoadScreenShotEx(int nGame, LPCSTR lpSoftwareName, int nType);
extern BOOL LoadScreenShot(int nGame, int nType);
extern HANDLE GetScreenShotHandle(void);
extern int GetScreenShotWidth(void);
extern int GetScreenShotHeight(void);

extern void FreeScreenShot(void);
extern BOOL ScreenShotLoaded(void);

extern BOOL LoadDIBBG(HGLOBAL *phDIB, HPALETTE *pPal);
extern HBITMAP DIBToDDB(HDC hDC, HANDLE hDIB, LPMYBITMAPINFO desc);

#endif
