// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef TABVIEW_H
#define TABVIEW_H

struct TabViewCallbacks
{
	// Options retrieval
	BOOL (*pfnGetShowTabCtrl)(void);
	void (*pfnSetCurrentTab)(LPCSTR pszShortName);
	LPCSTR (*pfnGetCurrentTab)(void);
	void (*pfnSetShowTab)(int nTab, BOOL show);
	int (*pfnGetShowTab)(int nTab);

	// Accessors
	LPCSTR (*pfnGetTabShortName)(int nTab);
	LPCSTR (*pfnGetTabLongName)(int nTab);

	// Callbacks
	void (*pfnOnSelectionChanged)(void);
	void (*pfnOnMoveSize)(void);
};

struct TabViewOptions
{
	const struct TabViewCallbacks *pCallbacks;
	int nTabCount;
};


BOOL SetupTabView(HWND hwndTabView, const struct TabViewOptions *pOptions);

void TabView_Reset(HWND hwndTabView);
void TabView_CalculateNextTab(HWND hwndTabView);
int TabView_GetCurrentTab(HWND hwndTabView);
void TabView_SetCurrentTab(HWND hwndTabView, int nTab);
void TabView_UpdateSelection(HWND hwndTabView);

// These are used to handle events received by the parent regarding
// tabview controls
BOOL TabView_HandleNotify(LPNMHDR lpNmHdr);

#endif // TABVIEW_H
