// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIRECTDRAW_H
#define DIRECTDRAW_H

#include <ddraw.h>

#define MAXMODES    256 /* Maximum number of DirectDraw Display modes. */

/* Display mode node */
struct tDisplayMode
{
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	DWORD m_dwBPP;
	DWORD m_dwRefresh;
};

/* EnumDisplayMode Context */
struct tDisplayModes
{
	struct tDisplayMode m_Modes[MAXMODES];
	int                 m_nNumModes;
};

extern BOOL DirectDraw_Initialize(void);
extern void DirectDraw_Close(void);

extern struct tDisplayModes* DirectDraw_GetDisplayModes(void);
extern int            DirectDraw_GetNumDisplays(void);
extern BOOL           DirectDraw_HasHWStretch(void);
extern BOOL           DirectDraw_HasRefresh(void);
extern LPCTSTR        DirectDraw_GetDisplayName(int num_display);

#endif
