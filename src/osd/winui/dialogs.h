// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIALOGS_H
#define DIALOGS_H

const char * GetFilterText(void);

INT_PTR CALLBACK ResetDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InterfaceDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK FilterDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AddCustomFileDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DirectXDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif

