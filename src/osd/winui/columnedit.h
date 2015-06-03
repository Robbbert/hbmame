// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef COLUMNEDIT_H
#define COLUMNEDIT_H

INT_PTR InternalColumnDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam,
	int nColumnMax, int *shown, int *order,
	const LPCTSTR *names, void (*pfnGetRealColumnOrder)(int *),
	void (*pfnGetColumnInfo)(int *pnOrder, int *pnShown),
	void (*pfnSetColumnInfo)(int *pnOrder, int *pnShown));

INT_PTR CALLBACK ColumnDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif
