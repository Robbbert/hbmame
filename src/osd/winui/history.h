// For licensing and usage information, read docs/release/winui_license.txt
//  MASTER
//****************************************************************************

#pragma once
 
#ifndef WINUI_HISTORY_H
#define WINUI_HISTORY_H

string GetGameHistory(int);  // Arcade-only builds (HBMAME, ARCADE)
string GetGameHistory(int, std::string); // Builds with software support (MESSUI, MAMEUI)

#endif

