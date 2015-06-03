// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

/***************************************************************************

  directinput.c

  Direct Input routines.

 ***************************************************************************/
// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// MAME/MAMEUI headers
#include "emu.h"
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

typedef HRESULT (WINAPI *dic_proc)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUT *ppDI,
									LPUNKNOWN punkOuter);

BOOL DirectInputInitialize()
{
	HRESULT   hr;
	UINT	  error_mode;
	dic_proc  dic;

	if (hDLL != NULL)
		return TRUE;

	hDLL = NULL;

	/* Turn off error dialog for this call */
	error_mode = SetErrorMode(0);
	hDLL = LoadLibrary(TEXT("dinput.dll"));
	SetErrorMode(error_mode);

	if (hDLL == NULL)
		return FALSE;

#ifdef UNICODE
	dic = (dic_proc)GetProcAddress((HINSTANCE)hDLL, "DirectInputCreateW");
#else
	dic = (dic_proc)GetProcAddress((HINSTANCE)hDLL, "DirectInputCreateA");
#endif
	if (dic == NULL)
		return FALSE;

	hr = dic(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &di, NULL);

	if (FAILED(hr))
	{
		hr = dic(GetModuleHandle(NULL), 0x0300, &di, NULL);

		if (FAILED(hr))
		{
			ErrorMsg("DirectInputCreate failed! error=%x\n", (unsigned int)hr);
			di = NULL;
			return FALSE;
		}
	}
	return TRUE;
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
	/*
        Release any lingering IDirectInput object.
    */
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

HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty,
						   DWORD dwObject, DWORD dwHow, DWORD dwValue)
{
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize		= sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwObj		= dwObject;
	dipdw.diph.dwHow		= dwHow;
	dipdw.dwData			= dwValue;

	return IDirectInputDevice2_SetProperty(pdev, guidProperty, &dipdw.diph);
}

LPDIRECTINPUT GetDirectInput(void)
{
	return di;
}
/***************************************************************************
    Internal functions
 ***************************************************************************/
