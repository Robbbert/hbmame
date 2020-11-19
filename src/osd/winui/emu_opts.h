// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_EMU_OPTS_H
#define WINUI_EMU_OPTS_H

#include "winmain.h"
#include "ui/moptions.h"

// These help categorise the folders on the left side
// This list is mainly for documentation, although a few are used in code
typedef enum {
	// Global types
	OPTIONS_GLOBAL = 0,
	OPTIONS_HORIZONTAL,
	OPTIONS_VERTICAL,
	OPTIONS_RASTER,
	OPTIONS_VECTOR,
	OPTIONS_LCD,
	OPTIONS_ARCADE,
	OPTIONS_CONSOLE,
	OPTIONS_COMPUTER,
	OPTIONS_OTHERSYS,
	// Local types
	OPTIONS_SOURCE,
	OPTIONS_GPARENT,
	OPTIONS_PARENT,
	OPTIONS_GAME,
	// EOF marker
	OPTIONS_MAX
} OPTIONS_TYPE;

#define GLOBAL_OPTIONS  -1

std::string emu_get_value(windows_options *o, std::string name);
std::string emu_get_value(windows_options &o, std::string name);
void emu_set_value(windows_options *o, const char* name, float value);
void emu_set_value(windows_options *o, const char* name, int value);
void emu_set_value(windows_options *o, const char* name, std::string value);
void emu_set_value(windows_options *o, std::string name, float value);
void emu_set_value(windows_options *o, std::string name, int value);
void emu_set_value(windows_options *o, std::string name, std::string value);
void emu_set_value(windows_options &o, const char* name, float value);
void emu_set_value(windows_options &o, const char* name, int value);
void emu_set_value(windows_options &o, const char* name, std::string value);
void emu_set_value(windows_options &o, std::string name, float value);
void emu_set_value(windows_options &o, std::string name, int value);
void emu_set_value(windows_options &o, std::string name, std::string value);
void ui_set_value(ui_options &o, std::string sname, std::string value);
void dir_set_value(int, std::string value);
std::string dir_get_value(int);
void emu_opts_init(bool);
void ui_save_ini();
std::string GetIniDir(void);
const char* GetSnapName();
void SetSnapName(const char*);
const std::string GetLanguageUI();
bool GetEnablePlugins();
bool GetSkipWarnings();
void SetSkipWarnings(BOOL);
const std::string GetPlugins();
void SetSelectedSoftware(int driver_index, std::string opt_name, const char *software);
void global_save_ini(void);
bool DriverHasSoftware(uint32_t drvindex);
void ResetGameDefaults(void);
void ResetAllGameOptions(void);
windows_options &MameUIGlobal(void);
void SetSystemName(windows_options &, OPTIONS_TYPE, int);
bool AreOptionsEqual(windows_options &opts1, windows_options &opts2);
void OptionsCopy(windows_options &source, windows_options &dest);
void SetDirectories(windows_options &opts);
void load_options(windows_options &, OPTIONS_TYPE, int, bool);
void save_options(windows_options &opts, OPTIONS_TYPE opt_type, int game_num);


#endif

