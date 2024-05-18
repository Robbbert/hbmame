// license:GPL_2.0
// copyright-holders:Robbbert
/***************************************************************************

    hbmame.c

    Specific (per target) constants

****************************************************************************/

#include "emu.h"

#define APPNAME             "HBMAME"
#define APPNAME_LOWER       "hbmame"
#define CONFIGNAME          "hbmame"
#define COPYRIGHT           "Copyright Robbbert\nand the MAME team\nhttps://mamedev.org"
#define COPYRIGHT_INFO      "\nMAME portions are copyright the MAME team and contributors\n" \
                            "HBMAME portions are copyright Robbbert and contributors."

const char * emulator_info::get_appname() { return APPNAME;}
const char * emulator_info::get_appname_lower() { return APPNAME_LOWER;}
const char * emulator_info::get_configname() { return CONFIGNAME;}
const char * emulator_info::get_copyright() { return COPYRIGHT;}
const char * emulator_info::get_copyright_info() { return COPYRIGHT_INFO;}
