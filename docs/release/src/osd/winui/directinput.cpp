// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

/***************************************************************************

  directinput.cpp

  Direct Input routines.

 ***************************************************************************/
// standard windows headers
#include <windows.h>

// MAME/MAMEUI headers
#include "mui_util.h" // For ErrorMsg
#include "directinput.h"

/***************************************************************************
    function prototypes
 ***************************************************************************/

/***************************************************************************
    External variables
 ***************************************************************************/

/***************************************************************************
    Internal structures
 ***************************************************************************/

/***************************************************************************
    Internal variables
 ***************************************************************************/

static LPDIRECTINPUT di = NULL;

static HANDLE hDLL = NULL;

/***************************************************************************
    External functions
 ***************************************************************************/

/****************************************************************************
 *      DirectInputInitialize
 *
 *      Initialize the DirectInput variables.
 *
 *      This entails the following functions:
 *
 *          DirectInputCreate
 *
 ****************************************************************************/

typedef HRESULT (WINAPI *dic_proc)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUT *ppDI, LPUNKNOWN punkOuter);

BOOL DirectInputInitialize()
{
	if (hDLL)
		return TRUE;

	/* Turn off error dialog for this call */
	UINT error_mode = SetErrorMode(0);
	hDLL = LoadLibrary(TEXT("dinput.dll"));
	SetErrorMode(error_mode);

	if (hDLL == NULL)
		return FALSE;

	dic_proc dic;
#ifdef UNICODE
	dic = (dic_proc)GetProcAddress((HINSTANCE)hDLL, "DirectInputCreateW");
#else
	dic = (dic_proc)GetProcAddress((HINSTANCE)hDLL, "DirectInputCreateA");
#endif
	if (dic == NULL)
		return false;

	HRESULT hr = dic(GetModuleHandle(NULL), 0x0700, &di, NULL);	// setup DIRECT INPUT 7 for the GUI

	if (FAILED(hr))
	{
		hr = dic(GetModuleHandle(NULL), 0x0500, &di, NULL);	// if failed, try with version 5

		if (FAILED(hr))
		{
			ErrorMsg("DirectInputCreate failed! error=%x\n", (unsigned int)hr);
			di = NULL;
			return false;
		}
	}
	return true;
}

/****************************************************************************
 *
 *      DirectInputClose
 *
 *      Terminate our usage of DirectInput.
 *
 ****************************************************************************/

void DirectInputClose()
{
	/* Release any lingering IDirectInput object. */
	if (di)
	{
		IDirectInput_Release(di);
		di = NULL;
	}
}

BOOL CALLBACK inputEnumDeviceProc(LPCDIDEVICEINSTANCE pdidi, LPVOID pv)
{
	GUID *pguidDevice;

	/* report back the instance guid of the device we enumerated */
	if (pv)
	{
		pguidDevice  = (GUID *)pv;
		*pguidDevice = pdidi->guidInstance;
	}

	/* BUGBUG for now, stop after the first device has been found */
	return DIENUM_STOP;
}

HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty, DWORD dwObject, DWORD dwHow, DWORD dwValue)
{
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize       = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwObj        = dwObject;
	dipdw.diph.dwHow        = dwHow;
	dipdw.dwData            = dwValue;

	return IDirectInputDevice2_SetProperty(pdev, guidProperty, &dipdw.diph);
}

LPDIRECTINPUT GetDirectInput(void)
{
	return di;
}
