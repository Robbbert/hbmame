// For licensing and usage information, read docs/winui_license.txt
//  MASTER
//****************************************************************************

#pragma once
 
#ifndef WINUI_HISTORY_H
#define WINUI_HISTORY_H

char * GetGameHistory(int driver_index);  // Arcade-only builds (HBMAME, ARCADE)
char * GetGameHistory(int driver_index, std::string software); // Builds with software support (MESSUI, MAMEUI)

#endif

