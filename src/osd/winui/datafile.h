// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#pragma once

#ifndef DATAFILE_H
#define DATAFILE_H

struct tDatafileIndex
{
	long offset;
	const game_driver *driver;
};

extern int load_driver_history (const game_driver *drv, char *buffer, int bufsize);
extern int load_driver_mameinfo (const game_driver *drv, char *buffer, int bufsize);
extern int load_driver_drivinfo (const game_driver *drv, char *buffer, int bufsize);

#endif
