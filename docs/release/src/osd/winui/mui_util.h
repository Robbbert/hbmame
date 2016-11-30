// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef MUI_UTIL_H
#define MUI_UTIL_H

#include "emucore.h"

extern void __cdecl ErrorMsg(const char* fmt, ...);
extern void __cdecl dprintf(const char* fmt, ...);


extern UINT GetDepth(HWND hWnd);

/* Open a text file */
extern void DisplayTextFile(HWND hWnd, const char *cName);

#define PACKVERSION(major,minor) MAKELONG(minor,major)

/* Check for old version of comctl32.dll */
extern LONG GetCommonControlVersion(void);

extern char * MyStrStrI(const char* pFirst, const char* pSrch);
extern char * ConvertToWindowsNewlines(const char *source);

extern const char * GetDriverFilename(int nIndex);

BOOL DriverIsClone(int driver_index);
BOOL DriverIsBroken(int driver_index);
BOOL DriverIsHarddisk(int driver_index);
BOOL DriverHasOptionalBIOS(int driver_index);
BOOL DriverIsStereo(int driver_index);
BOOL DriverIsVector(int driver_index);
int  DriverNumScreens(int driver_index);
BOOL DriverIsBios(int driver_index);
BOOL DriverUsesRoms(int driver_index);
BOOL DriverUsesSamples(int driver_index);
BOOL DriverUsesTrackball(int driver_index);
BOOL DriverUsesLightGun(int driver_index);
BOOL DriverUsesMouse(int driver_index);
BOOL DriverSupportsSaveState(int driver_index);
BOOL DriverIsVertical(int driver_index);
BOOL DriverIsMechanical(int driver_index);
BOOL DriverIsArcade(int driver_index);
BOOL DriverHasRam(int driver_index);

int isDriverVector(const machine_config *config);
int numberOfSpeakers(const machine_config *config);
int numberOfScreens(const machine_config *config);

void FlushFileCaches(void);

BOOL StringIsSuffixedBy(const char *s, const char *suffix);

BOOL SafeIsAppThemed(void);

// provides result of FormatMessage()
// resulting buffer must be free'd with LocalFree()
void GetSystemErrorMessage(DWORD dwErrorId, TCHAR **tErrorMessage);

HICON win_extract_icon_utf8(HINSTANCE inst, const char* exefilename, UINT iconindex);
TCHAR* win_tstring_strdup(LPCTSTR str);
HANDLE win_create_file_utf8(const char* filename, DWORD desiredmode, DWORD sharemode,
							LPSECURITY_ATTRIBUTES securityattributes, DWORD creationdisposition,
							DWORD flagsandattributes, HANDLE templatehandle);
DWORD win_get_current_directory_utf8(DWORD bufferlength, char* buffer);
HANDLE win_find_first_file_utf8(const char* filename, LPWIN32_FIND_DATA findfiledata);

 //  wstring_from_utf8
 //============================================================
 
WCHAR *ui_wstring_from_utf8(const char *utf8string)
{
	int char_count;
	WCHAR *result;

	// convert MAME string (UTF-8) to UTF-16
	char_count = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, nullptr, 0);
	result = (WCHAR *)malloc(char_count * sizeof(*result));
	if (result != nullptr)
		MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, result, char_count);
 
	return result;
}

char *ui_utf8_from_wstring(const WCHAR *wstring)
{
	int char_count;
	char *result;

	// convert UTF-16 to MAME string (UTF-8)
	char_count = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, nullptr, 0, nullptr, nullptr);
	result = (char *)malloc(char_count * sizeof(*result));
	if (result != nullptr)
		WideCharToMultiByte(CP_UTF8, 0, wstring, -1, result, char_count, nullptr, nullptr);
	return result;
}


#endif /* MUI_UTIL_H */

