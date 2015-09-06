// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#pragma once

#ifndef DATAFILE_H
#define DATAFILE_H

int load_driver_history(const game_driver *drv, char *buffer, int bufsize, int filenum);
int load_driver_mameinfo(const game_driver *drv, char *buffer, int bufsize, int filenum);
int load_driver_driverinfo(const game_driver *drv, char *buffer, int bufsize, int filenum);
int load_driver_command(const game_driver *drv, char *buffer, int bufsize);
int load_driver_scoreinfo(const game_driver *drv, char *buffer, int bufsize);

#endif
