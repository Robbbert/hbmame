// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

/***************************************************************************

  help.c

    Help wrapper code.

***************************************************************************/

// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// MAME/MAMEUI headers
#include "help.h"

typedef HWND (WINAPI *HtmlHelpProc)(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData);

/***************************************************************************
 Internal structures
***************************************************************************/

/***************************************************************************
 function prototypes
***************************************************************************/

static void Help_Load(void);

/***************************************************************************
 External function prototypes
***************************************************************************/

/***************************************************************************
 External variables
***************************************************************************/

/***************************************************************************
 Internal variables
***************************************************************************/

static HtmlHelpProc g_pHtmlHelp;
static HMODULE      g_hHelpLib;
static DWORD        g_dwCookie = 0;

/**************************************************************************
 External functions
***************************************************************************/

int HelpInit(void)
{
	g_pHtmlHelp = NULL;
	g_hHelpLib  = NULL;

	g_dwCookie = 0;
	HelpFunction(NULL, NULL, HH_INITIALIZE, (DWORD_PTR)&g_dwCookie);
	return 0;
}

void HelpExit(void)
{
	HelpFunction(NULL, NULL, HH_CLOSE_ALL, 0);
	HelpFunction(NULL, NULL, HH_UNINITIALIZE, (DWORD_PTR)&g_dwCookie);

	g_dwCookie  = 0;
	g_pHtmlHelp = NULL;

	if (g_hHelpLib)
	{
		FreeLibrary(g_hHelpLib);
		g_hHelpLib = NULL;
	}
}

HWND HelpFunction(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData)
{
	if (g_pHtmlHelp == NULL)
		Help_Load();

	if (g_pHtmlHelp)
		return g_pHtmlHelp(hwndCaller, pszFile, uCommand, dwData);
	else
		return NULL;
}

/***************************************************************************
 Internal functions
***************************************************************************/

static void Help_Load(void)
{
	g_hHelpLib = LoadLibrary(TEXT("hhctrl.ocx"));
	if (g_hHelpLib)
	{
		FARPROC pProc = NULL;
		pProc = GetProcAddress(g_hHelpLib, "HtmlHelpA");
		if (pProc)
		{
			g_pHtmlHelp = (HtmlHelpProc)pProc;
		}
		else
		{
			FreeLibrary(g_hHelpLib);
			g_hHelpLib = NULL;
		}
	}
}
