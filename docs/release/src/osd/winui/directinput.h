// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

#undef WINNT
#ifdef DIRECTINPUT_VERSION
#undef DIRECTINPUT_VERSION
#endif
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>

extern BOOL DirectInputInitialize(void);
extern void DirectInputClose(void);

extern BOOL CALLBACK inputEnumDeviceProc(LPCDIDEVICEINSTANCE pdidi, LPVOID pv);

extern HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE2 pdev, REFGUID guidProperty, DWORD dwObject, DWORD dwHow, DWORD dwValue);

LPDIRECTINPUT GetDirectInput(void);

#endif
