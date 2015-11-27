// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************
//============================================================
//
//  newui.h - NEWUI
//
//============================================================

#ifndef NEWUI_H
#define NEWUI_H

///// INCLUDES /////

#include <windows.h>
#include <commdlg.h>
#include <tchar.h>
#include "emu.h"
#include "emuopts.h"
#include "ui/ui.h"
#include "newuires.h"
#include "strconv.h"
#include "debug/debugcpu.h"
#include "imagedev/cassette.h"
#include "windows/window.h"
#include "winutf8.h"



// These are called from src/osd/windows/windows.c and
//   provide the linkage between newui and the core. 

LRESULT CALLBACK winwindow_video_window_proc_ui(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam);

int win_create_menu(running_machine &machine, HMENU *menus);

#endif /* NEWUI_H */
