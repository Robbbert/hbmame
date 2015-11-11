// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************
/***************************************************************************

  history.c

    history functions.

***************************************************************************/

// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "drivenum.h"
#include "mui_util.h"
#include "datafile.h"
#include "history.h"

/**************************************************************
 * functions
 **************************************************************/

// Load indexes from history.dat if found
char * GetGameHistory(int driver_index)
{
	static char dataBuf[2048 * 2048];
	static char buffer[2048 * 2048];
	
	memset(&buffer, 0, sizeof(buffer));
	memset(&dataBuf, 0, sizeof(dataBuf));

	if (load_driver_history(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer), 0) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_history(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer), 1) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_mameinfo(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer), 0) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_driverinfo(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer), 0) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_mameinfo(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer), 1) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_driverinfo(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer), 1) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_command(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer)) == 0)
		strcat(dataBuf, buffer);

	if (load_driver_scoreinfo(&driver_list::driver(driver_index), buffer, ARRAY_LENGTH(buffer)) == 0)
		strcat(dataBuf, buffer);

	return ConvertToWindowsNewlines(dataBuf);
}
