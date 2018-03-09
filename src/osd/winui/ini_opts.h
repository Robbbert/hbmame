// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_INIOPTS_H
#define WINUI_INIOPTS_H

typedef struct
{
	std::string   name;
	std::string   value;
	const core_options::option_type         unused1;
	char*          unused2;
}
INIOPTS;
// INI options
const INIOPTS option_entries[] =
{
	{ OPTION_READCONFIG ";rc",                           "1",         OPTION_BOOLEAN,    "enable loading of configuration files" },
	{ OPTION_WRITECONFIG ";wc",                          "1",         OPTION_BOOLEAN,    "writes configuration to (driver).ini on exit" },
	{ OPTION_MEDIAPATH ";rp;biospath;bp",                "roms",      OPTION_STRING,     "path to ROMsets and hard disk images" },
	{ OPTION_HASHPATH ";hash_directory;hash",            "hash",      OPTION_STRING,     "path to hash files" },
	{ OPTION_SAMPLEPATH ";sp",                           "samples",   OPTION_STRING,     "path to samplesets" },
	{ OPTION_ARTPATH,                                    "artwork",   OPTION_STRING,     "path to artwork files" },
	{ OPTION_CTRLRPATH,                                  "ctrlr",     OPTION_STRING,     "path to controller definitions" },
	{ OPTION_INIPATH,                                    ".",         OPTION_STRING,     "path to ini files" },
	{ OPTION_FONTPATH,                                   ".",         OPTION_STRING,     "path to font files" },
	{ OPTION_CHEATPATH,                                  "cheat",     OPTION_STRING,     "path to cheat files" },
	{ OPTION_CROSSHAIRPATH,                              "crosshair", OPTION_STRING,     "path to crosshair files" },
	{ OPTION_PLUGINSPATH,                                "plugins",   OPTION_STRING,     "path to plugin files" },
	{ OPTION_LANGUAGEPATH,                               "language",  OPTION_STRING,     "path to language files" },
	{ OPTION_SWPATH,                                     "software",  OPTION_STRING,     "path to loose software" },
	{ OPTION_CFG_DIRECTORY,                              "cfg",       OPTION_STRING,     "directory to save configurations" },
	{ OPTION_NVRAM_DIRECTORY,                            "nvram",     OPTION_STRING,     "directory to save nvram contents" },
	{ OPTION_INPUT_DIRECTORY,                            "inp",       OPTION_STRING,     "directory to save input device logs" },
	{ OPTION_STATE_DIRECTORY,                            "sta",       OPTION_STRING,     "directory to save states" },
	{ OPTION_SNAPSHOT_DIRECTORY,                         "snap",      OPTION_STRING,     "directory to save/load screenshots" },
	{ OPTION_DIFF_DIRECTORY,                             "diff",      OPTION_STRING,     "directory to save hard drive image difference files" },
	{ OPTION_COMMENT_DIRECTORY,                          "comments",  OPTION_STRING,     "directory to save debugger comments" },
	{ OPTION_STATE,                                      "",     OPTION_STRING,     "saved state to load" },
	{ OPTION_AUTOSAVE,                                   "0",         OPTION_BOOLEAN,    "enable automatic restore at startup, and automatic save at exit time" },
	{ OPTION_PLAYBACK ";pb",                             "",     OPTION_STRING,     "playback an input file" },
	{ OPTION_RECORD ";rec",                              "",     OPTION_STRING,     "record an input file" },
	{ OPTION_RECORD_TIMECODE,                            "0",         OPTION_BOOLEAN,    "record an input timecode file (requires -record option)" },
	{ OPTION_EXIT_AFTER_PLAYBACK,                        "0",         OPTION_BOOLEAN,    "close the program at the end of playback" },
	{ OPTION_MNGWRITE,                                   "",     OPTION_STRING,     "optional filename to write a MNG movie of the current session" },
	{ OPTION_AVIWRITE,                                   "",     OPTION_STRING,     "optional filename to write an AVI movie of the current session" },
	{ OPTION_WAVWRITE,                                   "",     OPTION_STRING,     "optional filename to write a WAV file of the current session" },
	{ OPTION_SNAPNAME,                                   "%g/%i",     OPTION_STRING,     "override of the default snapshot/movie naming; %g == gamename, %i == index" },
	{ OPTION_SNAPSIZE,                                   "auto",      OPTION_STRING,     "specify snapshot/movie resolution (<width>x<height>) or 'auto' to use minimal size " },
	{ OPTION_SNAPVIEW,                                   "internal",  OPTION_STRING,     "specify snapshot/movie view or 'internal' to use internal pixel-aspect views" },
	{ OPTION_SNAPBILINEAR,                               "1",         OPTION_BOOLEAN,    "specify if the snapshot/movie should have bilinear filtering applied" },
	{ OPTION_STATENAME,                                  "%g",        OPTION_STRING,     "override of the default state subfolder naming; %g == gamename" },
	{ OPTION_BURNIN,                                     "0",         OPTION_BOOLEAN,    "create burn-in snapshots for each screen" },
	{ OPTION_AUTOFRAMESKIP ";afs",                       "0",         OPTION_BOOLEAN,    "enable automatic frameskip selection" },
	{ OPTION_FRAMESKIP ";fs(0-10)",                      "0",         OPTION_INTEGER,    "set frameskip to fixed value, 0-10 (autoframeskip must be disabled)" },
	{ OPTION_SECONDS_TO_RUN ";str",                      "0",         OPTION_INTEGER,    "number of emulated seconds to run before automatically exiting" },
	{ OPTION_THROTTLE,                                   "1",         OPTION_BOOLEAN,    "enable throttling to keep game running in sync with real time" },
	{ OPTION_SLEEP,                                      "1",         OPTION_BOOLEAN,    "enable sleeping, which gives time back to other applications when idle" },
	{ OPTION_SPEED "(0.01-100)",                         "1.0",       OPTION_FLOAT,      "controls the speed of gameplay, relative to realtime; smaller numbers are slower" },
	{ OPTION_REFRESHSPEED ";rs",                         "0",         OPTION_BOOLEAN,    "automatically adjusts the speed of gameplay to keep the refresh rate lower than the screen" },
	{ OPTION_KEEPASPECT ";ka",                           "1",         OPTION_BOOLEAN,    "constrain to the proper aspect ratio" },
	{ OPTION_UNEVENSTRETCH ";ues",                       "1",         OPTION_BOOLEAN,    "allow non-integer stretch factors" },
	{ OPTION_UNEVENSTRETCHX ";uesx",                     "0",         OPTION_BOOLEAN,    "allow non-integer stretch factors only on horizontal axis"},
	{ OPTION_UNEVENSTRETCHY ";uesy",                     "0",         OPTION_BOOLEAN,    "allow non-integer stretch factors only on vertical axis"},
	{ OPTION_AUTOSTRETCHXY ";asxy",                      "0",         OPTION_BOOLEAN,    "automatically apply -unevenstretchx/y based on source native orientation"},
	{ OPTION_INTOVERSCAN ";ios",                         "0",         OPTION_BOOLEAN,    "allow overscan on integer scaled targets"},
	{ OPTION_INTSCALEX ";sx",                            "0",         OPTION_INTEGER,    "set horizontal integer scale factor."},
	{ OPTION_INTSCALEY ";sy",                            "0",         OPTION_INTEGER,    "set vertical integer scale."},
	{ OPTION_ROTATE,                                     "1",         OPTION_BOOLEAN,    "rotate the game screen according to the game's orientation needs it" },
	{ OPTION_ROR,                                        "0",         OPTION_BOOLEAN,    "rotate screen clockwise 90 degrees" },
	{ OPTION_ROL,                                        "0",         OPTION_BOOLEAN,    "rotate screen counterclockwise 90 degrees" },
	{ OPTION_AUTOROR,                                    "0",         OPTION_BOOLEAN,    "automatically rotate screen clockwise 90 degrees if vertical" },
	{ OPTION_AUTOROL,                                    "0",         OPTION_BOOLEAN,    "automatically rotate screen counterclockwise 90 degrees if vertical" },
	{ OPTION_FLIPX,                                      "0",         OPTION_BOOLEAN,    "flip screen left-right" },
	{ OPTION_FLIPY,                                      "0",         OPTION_BOOLEAN,    "flip screen upside-down" },
	{ OPTION_ARTWORK_CROP ";artcrop",                    "0",         OPTION_BOOLEAN,    "crop artwork to game screen size" },
	{ OPTION_USE_BACKDROPS ";backdrop",                  "1",         OPTION_BOOLEAN,    "enable backdrops if artwork is enabled and available" },
	{ OPTION_USE_OVERLAYS ";overlay",                    "1",         OPTION_BOOLEAN,    "enable overlays if artwork is enabled and available" },
	{ OPTION_USE_BEZELS ";bezel",                        "1",         OPTION_BOOLEAN,    "enable bezels if artwork is enabled and available" },
	{ OPTION_USE_CPANELS ";cpanel",                      "1",         OPTION_BOOLEAN,    "enable cpanels if artwork is enabled and available" },
	{ OPTION_USE_MARQUEES ";marquee",                    "1",         OPTION_BOOLEAN,    "enable marquees if artwork is enabled and available" },
	{ OPTION_BRIGHTNESS "(0.1-2.0)",                     "1.0",       OPTION_FLOAT,      "default game screen brightness correction" },
	{ OPTION_CONTRAST "(0.1-2.0)",                       "1.0",       OPTION_FLOAT,      "default game screen contrast correction" },
	{ OPTION_GAMMA "(0.1-3.0)",                          "1.0",       OPTION_FLOAT,      "default game screen gamma correction" },
	{ OPTION_PAUSE_BRIGHTNESS "(0.0-1.0)",               "0.65",      OPTION_FLOAT,      "amount to scale the screen brightness when paused" },
	{ OPTION_EFFECT,                                     "none",      OPTION_STRING,     "name of a PNG file to use for visual effects, or 'none'" },
	{ OPTION_BEAM_WIDTH_MIN,                             "1.0",       OPTION_FLOAT,      "set vector beam width minimum" },
	{ OPTION_BEAM_WIDTH_MAX,                             "1.0",       OPTION_FLOAT,      "set vector beam width maximum" },
	{ OPTION_BEAM_INTENSITY_WEIGHT,                      "0",         OPTION_FLOAT,      "set vector beam intensity weight " },
	{ OPTION_FLICKER,                                    "0",         OPTION_FLOAT,      "set vector flicker effect" },
	{ OPTION_SAMPLERATE ";sr(1000-1000000)",             "48000",     OPTION_INTEGER,    "set sound output sample rate" },
	{ OPTION_SAMPLES,                                    "1",         OPTION_BOOLEAN,    "enable the use of external samples if available" },
	{ OPTION_VOLUME ";vol",                              "0",         OPTION_INTEGER,    "sound volume in decibels (-32 min, 0 max)" },
	{ OPTION_COIN_LOCKOUT ";coinlock",                   "1",         OPTION_BOOLEAN,    "enable coin lockouts to actually lock out coins" },
	{ OPTION_CTRLR,                                      "",     OPTION_STRING,     "preconfigure for specified controller" },
	{ OPTION_MOUSE,                                      "0",         OPTION_BOOLEAN,    "enable mouse input" },
	{ OPTION_JOYSTICK ";joy",                            "1",         OPTION_BOOLEAN,    "enable joystick input" },
	{ OPTION_LIGHTGUN ";gun",                            "0",         OPTION_BOOLEAN,    "enable lightgun input" },
	{ OPTION_MULTIKEYBOARD ";multikey",                  "0",         OPTION_BOOLEAN,    "enable separate input from each keyboard device (if present)" },
	{ OPTION_MULTIMOUSE,                                 "0",         OPTION_BOOLEAN,    "enable separate input from each mouse device (if present)" },
	{ OPTION_STEADYKEY ";steady",                        "0",         OPTION_BOOLEAN,    "enable steadykey support" },
	{ OPTION_UI_ACTIVE,                                  "0",         OPTION_BOOLEAN,    "enable user interface on top of emulated keyboard (if present)" },
	{ OPTION_OFFSCREEN_RELOAD ";reload",                 "0",         OPTION_BOOLEAN,    "convert lightgun button 2 into offscreen reload" },
	{ OPTION_JOYSTICK_MAP ";joymap",                     "auto",      OPTION_STRING,     "explicit joystick map, or auto to auto-select" },
	{ OPTION_JOYSTICK_DEADZONE ";joy_deadzone;jdz(0.00-1)",      "0.3",       OPTION_FLOAT,      "center deadzone range for joystick where change is ignored (0.0 center, 1.0 end)" },
	{ OPTION_JOYSTICK_SATURATION ";joy_saturation;jsat(0.00-1)", "0.85",      OPTION_FLOAT,      "end of axis saturation range for joystick where change is ignored (0.0 center, 1.0 end)" },
	{ OPTION_NATURAL_KEYBOARD ";nat",                    "0",         OPTION_BOOLEAN,    "specifies whether to use a natural keyboard or not" },
	{ OPTION_JOYSTICK_CONTRADICTORY ";joy_contradictory","0",         OPTION_BOOLEAN,    "enable contradictory direction digital joystick input at the same time" },
	{ OPTION_COIN_IMPULSE,                               "0",         OPTION_INTEGER,    "set coin impulse time (n<0 disable impulse, n==0 obey driver, 0<n set time n)" },
	{ OPTION_PADDLE_DEVICE ";paddle",                    "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a paddle control is present" },
	{ OPTION_ADSTICK_DEVICE ";adstick",                  "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if an analog joystick control is present" },
	{ OPTION_PEDAL_DEVICE ";pedal",                      "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a pedal control is present" },
	{ OPTION_DIAL_DEVICE ";dial",                        "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a dial control is present" },
	{ OPTION_TRACKBALL_DEVICE ";trackball",              "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a trackball control is present" },
	{ OPTION_LIGHTGUN_DEVICE,                            "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a lightgun control is present" },
	{ OPTION_POSITIONAL_DEVICE,                          "keyboard",  OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a positional control is present" },
	{ OPTION_MOUSE_DEVICE,                               "mouse",     OPTION_STRING,     "enable (none|keyboard|mouse|lightgun|joystick) if a mouse control is present" },
	{ OPTION_VERBOSE ";v",                               "0",         OPTION_BOOLEAN,    "display additional diagnostic information" },
	{ OPTION_LOG,                                        "0",         OPTION_BOOLEAN,    "generate an error.log file" },
	{ OPTION_OSLOG,                                      "0",         OPTION_BOOLEAN,    "output error.log data to the system debugger" },
	{ OPTION_DEBUG ";d",                                 "0",         OPTION_BOOLEAN,    "enable/disable debugger" },
	{ OPTION_UPDATEINPAUSE,                              "0",         OPTION_BOOLEAN,    "keep calling video updates while in pause" },
	{ OPTION_DEBUGSCRIPT,                                "",     OPTION_STRING,     "script for debugger" },
	{ OPTION_COMM_LOCAL_HOST,                            "0.0.0.0",   OPTION_STRING,     "local address to bind to" },
	{ OPTION_COMM_LOCAL_PORT,                            "15112",     OPTION_STRING,     "local port to bind to" },
	{ OPTION_COMM_REMOTE_HOST,                           "127.0.0.1", OPTION_STRING,     "remote address to connect to" },
	{ OPTION_COMM_REMOTE_PORT,                           "15112",     OPTION_STRING,     "remote port to connect to" },
	{ OPTION_DRC,                                        "1",         OPTION_BOOLEAN,    "enable DRC cpu core if available" },
	{ OPTION_DRC_USE_C,                                  "0",         OPTION_BOOLEAN,    "force DRC use C backend" },
	{ OPTION_DRC_LOG_UML,                                "0",         OPTION_BOOLEAN,    "write DRC UML disassembly log" },
	{ OPTION_DRC_LOG_NATIVE,                             "0",         OPTION_BOOLEAN,    "write DRC native disassembly log" },
	{ OPTION_BIOS,                                       "",     OPTION_STRING,     "select the system BIOS to use" },
	{ OPTION_CHEAT ";c",                                 "0",         OPTION_BOOLEAN,    "enable cheat subsystem" },
	{ OPTION_SKIP_GAMEINFO,                              "0",         OPTION_BOOLEAN,    "skip displaying the information screen at startup" },
	{ OPTION_UI_FONT,                                    "default",   OPTION_STRING,     "specify a font to use" },
	{ OPTION_UI,                                         "cabinet",   OPTION_STRING,     "type of UI (simple|cabinet)" },
	{ OPTION_RAMSIZE ";ram",                             "",     OPTION_STRING,     "size of RAM (if supported by driver)" },
	{ OPTION_CONFIRM_QUIT,                               "0",         OPTION_BOOLEAN,    "display confirm quit screen on exit" },
	{ OPTION_UI_MOUSE,                                   "1",         OPTION_BOOLEAN,    "display ui mouse cursor" },
	{ OPTION_AUTOBOOT_COMMAND ";ab",                     "",     OPTION_STRING,     "command to execute after machine boot" },
	{ OPTION_AUTOBOOT_DELAY,                             "0",         OPTION_INTEGER,    "timer delay in sec to trigger command execution on autoboot" },
	{ OPTION_AUTOBOOT_SCRIPT ";script",                  "",     OPTION_STRING,     "lua script to execute after machine boot" },
	{ OPTION_CONSOLE,                                    "0",         OPTION_BOOLEAN,    "enable emulator LUA console" },
	{ OPTION_PLUGINS,                                    "1",         OPTION_BOOLEAN,    "enable LUA plugin support" },
	{ OPTION_PLUGIN,                                     "",     OPTION_STRING,     "list of plugins to enable" },
	{ OPTION_NO_PLUGIN,                                  "",     OPTION_STRING,     "list of plugins to disable" },
	{ OPTION_LANGUAGE ";lang",                           "English",   OPTION_STRING,     "display language" },
	{ OPTION_HTTP,                                       "0",         OPTION_BOOLEAN,    "HTTP server enable" },
	{ OPTION_HTTP_PORT,                                  "8080",      OPTION_INTEGER,    "HTTP server port" },
	{ OPTION_HTTP_ROOT,                                  "web",       OPTION_STRING,     "HTTP server document root" },
	{ OSDOPTION_UIMODEKEY,                   "SCRLOCK",         OPTION_STRING,    "Key to toggle keyboard mode" },
	{ OSD_FONT_PROVIDER,                      "auto",   OPTION_STRING,    "provider for ui font: " },
	{ OSD_OUTPUT_PROVIDER,                    "auto",   OPTION_STRING,    "provider for output: " },
	{ OSD_KEYBOARDINPUT_PROVIDER,             "auto",   OPTION_STRING,    "provider for keyboard input: " },
	{ OSD_MOUSEINPUT_PROVIDER,                "auto",   OPTION_STRING,    "provider for mouse input: " },
	{ OSD_LIGHTGUNINPUT_PROVIDER,             "auto",   OPTION_STRING,    "provider for lightgun input: " },
	{ OSD_JOYSTICKINPUT_PROVIDER,             "auto",   OPTION_STRING,    "provider for joystick input: " },
	{ OSDCOMMAND_LIST_MIDI_DEVICES ";mlist",  "0",              OPTION_COMMAND,   "list available MIDI I/O devices" },
	{ OSDCOMMAND_LIST_NETWORK_ADAPTERS ";nlist", "0",           OPTION_COMMAND,   "list available network adapters" },
	{ OSDOPTION_DEBUGGER,                     "auto",   OPTION_STRING,    "debugger used: " },
	{ OSDOPTION_DEBUGGER_FONT ";dfont",       "auto",   OPTION_STRING,    "specifies the font to use for debugging" },
	{ OSDOPTION_DEBUGGER_FONT_SIZE ";dfontsize", "0",           OPTION_FLOAT,     "specifies the font size to use for debugging" },
	{ OSDOPTION_WATCHDOG ";wdog",             "0",              OPTION_INTEGER,   "force the program to terminate if no updates within specified number of seconds" },
	{ OSDOPTION_NUMPROCESSORS ";np",          "auto",   OPTION_STRING,    "number of processors; this overrides the number the system reports" },
	{ OSDOPTION_BENCH,                        "0",              OPTION_INTEGER,   "benchmark for the given number of emulated seconds; implies -video none -sound none -nothrottle" },
	{ OSDOPTION_VIDEO,                        "auto",   OPTION_STRING,    "video output method: " },
	{ OSDOPTION_NUMSCREENS "(1-4)",           "1",              OPTION_INTEGER,   "number of screens to create; usually, you want just one" },
	{ OSDOPTION_WINDOW ";w",                  "0",              OPTION_BOOLEAN,   "enable window mode; otherwise, full screen mode is assumed" },
	{ OSDOPTION_MAXIMIZE ";max",              "1",              OPTION_BOOLEAN,   "default to maximized windows; otherwise, windows will be minimized" },
	{ OSDOPTION_WAITVSYNC ";vs",              "0",              OPTION_BOOLEAN,   "enable waiting for the start of VBLANK before flipping screens; reduces tearing effects" },
	{ OSDOPTION_SYNCREFRESH ";srf",           "0",              OPTION_BOOLEAN,   "enable using the start of VBLANK for throttling instead of the game time" },
	{ OSD_MONITOR_PROVIDER,                   "auto",   OPTION_STRING,    "monitor discovery method" },
	{ OSDOPTION_SCREEN,                       "auto",   OPTION_STRING,    "explicit name of the first screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_ASPECT ";screen_aspect",      "auto",   OPTION_STRING,    "aspect ratio for all screens; 'auto' here will try to make a best guess" },
	{ OSDOPTION_RESOLUTION ";r",              "auto",   OPTION_STRING,    "preferred resolution for all screens; format is <width>x<height>[@<refreshrate>] or 'auto'" },
	{ OSDOPTION_VIEW,                         "auto",   OPTION_STRING,    "preferred view for all screens" },
	{ OSDOPTION_SCREEN "0",                   "auto",   OPTION_STRING,    "explicit name of the first screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_ASPECT "0",                   "auto",   OPTION_STRING,    "aspect ratio of the first screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_RESOLUTION "0;r0",            "auto",   OPTION_STRING,    "preferred resolution of the first screen; format is <width>x<height>[@<refreshrate>] or 'auto'" },
	{ OSDOPTION_VIEW "0",                     "auto",   OPTION_STRING,    "preferred view for the first screen" },
	{ OSDOPTION_SCREEN "1",                   "auto",   OPTION_STRING,    "explicit name of the second screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_ASPECT "1",                   "auto",   OPTION_STRING,    "aspect ratio of the second screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_RESOLUTION "1;r1",            "auto",   OPTION_STRING,    "preferred resolution of the second screen; format is <width>x<height>[@<refreshrate>] or 'auto'" },
	{ OSDOPTION_VIEW "1",                     "auto",   OPTION_STRING,    "preferred view for the second screen" },
	{ OSDOPTION_SCREEN "2",                   "auto",   OPTION_STRING,    "explicit name of the third screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_ASPECT "2",                   "auto",   OPTION_STRING,    "aspect ratio of the third screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_RESOLUTION "2;r2",            "auto",   OPTION_STRING,    "preferred resolution of the third screen; format is <width>x<height>[@<refreshrate>] or 'auto'" },
	{ OSDOPTION_VIEW "2",                     "auto",   OPTION_STRING,    "preferred view for the third screen" },
	{ OSDOPTION_SCREEN "3",                   "auto",   OPTION_STRING,    "explicit name of the fourth screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_ASPECT "3",                   "auto",   OPTION_STRING,    "aspect ratio of the fourth screen; 'auto' here will try to make a best guess" },
	{ OSDOPTION_RESOLUTION "3;r3",            "auto",   OPTION_STRING,    "preferred resolution of the fourth screen; format is <width>x<height>[@<refreshrate>] or 'auto'" },
	{ OSDOPTION_VIEW "3",                     "auto",   OPTION_STRING,    "preferred view for the fourth screen" },
	{ OSDOPTION_SWITCHRES,                    "0",              OPTION_BOOLEAN,   "enable resolution switching" },
	{ OSDOPTION_FILTER ";glfilter;flt",       "1",              OPTION_BOOLEAN,   "enable bilinear filtering on screen output" },
	{ OSDOPTION_PRESCALE,                     "1",              OPTION_INTEGER,   "scale screen rendering by this amount in software" },
	{ OSDOPTION_GL_FORCEPOW2TEXTURE,          "0",              OPTION_BOOLEAN,   "force power of two textures  (default no)" },
	{ OSDOPTION_GL_NOTEXTURERECT,             "0",              OPTION_BOOLEAN,   "don't use OpenGL GL_ARB_texture_rectangle (default on)" },
	{ OSDOPTION_GL_VBO,                       "1",              OPTION_BOOLEAN,   "enable OpenGL VBO,  if available (default on)" },
	{ OSDOPTION_GL_PBO,                       "1",              OPTION_BOOLEAN,   "enable OpenGL PBO,  if available (default on)" },
	{ OSDOPTION_GL_GLSL,                      "0",              OPTION_BOOLEAN,   "enable OpenGL GLSL, if available (default off)" },
	{ OSDOPTION_GLSL_FILTER,                  "1",              OPTION_STRING,    "enable OpenGL GLSL filtering instead of FF filtering 0-plain, 1-bilinear (default)" },
	{ OSDOPTION_SHADER_MAME "0",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 0" },
	{ OSDOPTION_SHADER_MAME "1",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 1" },
	{ OSDOPTION_SHADER_MAME "2",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 2" },
	{ OSDOPTION_SHADER_MAME "3",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 3" },
	{ OSDOPTION_SHADER_MAME "4",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 4" },
	{ OSDOPTION_SHADER_MAME "5",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 5" },
	{ OSDOPTION_SHADER_MAME "6",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 6" },
	{ OSDOPTION_SHADER_MAME "7",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 7" },
	{ OSDOPTION_SHADER_MAME "8",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 8" },
	{ OSDOPTION_SHADER_MAME "9",              "none",   OPTION_STRING,    "custom OpenGL GLSL shader set mame bitmap 9" },
	{ OSDOPTION_SHADER_SCREEN "0",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 0" },
	{ OSDOPTION_SHADER_SCREEN "1",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 1" },
	{ OSDOPTION_SHADER_SCREEN "2",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 2" },
	{ OSDOPTION_SHADER_SCREEN "3",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 3" },
	{ OSDOPTION_SHADER_SCREEN "4",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 4" },
	{ OSDOPTION_SHADER_SCREEN "5",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 5" },
	{ OSDOPTION_SHADER_SCREEN "6",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 6" },
	{ OSDOPTION_SHADER_SCREEN "7",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 7" },
	{ OSDOPTION_SHADER_SCREEN "8",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 8" },
	{ OSDOPTION_SHADER_SCREEN "9",            "none",   OPTION_STRING,    "custom OpenGL GLSL shader screen bitmap 9" },
	{ OSDOPTION_SOUND,                        "auto",   OPTION_STRING,    "sound output method: " },
	{ OSDOPTION_AUDIO_LATENCY "(1-5)",        "2",              OPTION_INTEGER,   "set audio latency (increase to reduce glitches, decrease for responsiveness)" },
	{ OSDOPTION_PA_API,                       "none",   OPTION_STRING,    "PortAudio API" },
	{ OSDOPTION_PA_DEVICE,                    "none",   OPTION_STRING,    "PortAudio device" },
	{ OSDOPTION_PA_LATENCY "(0-0.25)",        "0",              OPTION_FLOAT,     "suggested latency in seconds, 0 for default" },
	{ OSDOPTION_BGFX_PATH,                    "bgfx",            OPTION_STRING, "path to BGFX-related files" },
	{ OSDOPTION_BGFX_BACKEND,                 "auto",            OPTION_STRING, "BGFX backend to use (d3d9, d3d11, metal, opengl, gles)" },
	{ OSDOPTION_BGFX_DEBUG,                   "0",               OPTION_BOOLEAN, "enable BGFX debugging statistics" },
	{ OSDOPTION_BGFX_SCREEN_CHAINS,           "default",         OPTION_STRING, "comma-delimited list of screen chain JSON names, colon-delimited per-window" },
	{ OSDOPTION_BGFX_SHADOW_MASK,             "slot-mask.png",   OPTION_STRING, "shadow mask texture name" },
	{ OSDOPTION_BGFX_AVI_NAME,                "auto",    OPTION_STRING, "filename for BGFX output logging" },
	{ WINOPTION_PRIORITY "(-15-1)",                   "0",        OPTION_INTEGER,    "thread priority for the main game thread; range from -15 to 1" },
	{ WINOPTION_PROFILE,                              "0",        OPTION_INTEGER,    "enables profiling, specifying the stack depth to track" },
	{ WINOPTION_MENU,                                 "0",        OPTION_BOOLEAN,    "enables menu bar if available by UI implementation" },
	{ WINOPTION_HLSLPATH,                                       "hlsl",              OPTION_STRING,     "path to hlsl files" },
	{ WINOPTION_HLSL_ENABLE";hlsl",                             "0",                 OPTION_BOOLEAN,    "enables HLSL post-processing (PS3.0 required)" },
	{ WINOPTION_HLSL_OVERSAMPLING,                              "0",                 OPTION_BOOLEAN,    "enables HLSL oversampling" },
	{ WINOPTION_HLSL_WRITE,                                     "auto",      OPTION_STRING,     "enables HLSL AVI writing (huge disk bandwidth suggested)" },
	{ WINOPTION_HLSL_SNAP_WIDTH,                                "2048",              OPTION_STRING,     "HLSL upscaled-snapshot width" },
	{ WINOPTION_HLSL_SNAP_HEIGHT,                               "1536",              OPTION_STRING,     "HLSL upscaled-snapshot height" },
	{ WINOPTION_SHADOW_MASK_TILE_MODE,                          "0",                 OPTION_INTEGER,    "shadow mask tile mode (0 for screen based, 1 for source based)" },
	{ WINOPTION_SHADOW_MASK_ALPHA";fs_shadwa(0.0-1.0)",         "0.0",               OPTION_FLOAT,      "shadow mask alpha-blend value (1.0 is fully blended, 0.0 is no mask)" },
	{ WINOPTION_SHADOW_MASK_TEXTURE";fs_shadwt(0.0-1.0)",       "shadow-mask.png",   OPTION_STRING,     "shadow mask texture name" },
	{ WINOPTION_SHADOW_MASK_COUNT_X";fs_shadww",                "6",                 OPTION_INTEGER,    "shadow mask tile width, in screen dimensions" },
	{ WINOPTION_SHADOW_MASK_COUNT_Y";fs_shadwh",                "4",                 OPTION_INTEGER,    "shadow mask tile height, in screen dimensions" },
	{ WINOPTION_SHADOW_MASK_USIZE";fs_shadwu(0.0-1.0)",         "0.1875",            OPTION_FLOAT,      "shadow mask texture width, in U/V dimensions" },
	{ WINOPTION_SHADOW_MASK_VSIZE";fs_shadwv(0.0-1.0)",         "0.25",              OPTION_FLOAT,      "shadow mask texture height, in U/V dimensions" },
	{ WINOPTION_SHADOW_MASK_UOFFSET";fs_shadwou(-1.0-1.0)",     "0.0",               OPTION_FLOAT,      "shadow mask texture offset, in U direction" },
	{ WINOPTION_SHADOW_MASK_VOFFSET";fs_shadwov(-1.0-1.0)",     "0.0",               OPTION_FLOAT,      "shadow mask texture offset, in V direction" },
	{ WINOPTION_DISTORTION";fs_dist(-1.0-1.0)",                 "0.0",               OPTION_FLOAT,      "screen distortion amount" },
	{ WINOPTION_CUBIC_DISTORTION";fs_cubedist(-1.0-1.0)",       "0.0",               OPTION_FLOAT,      "screen cubic distortion amount" },
	{ WINOPTION_DISTORT_CORNER";fs_distc(0.0-1.0)",             "0.0",               OPTION_FLOAT,      "screen distort corner amount" },
	{ WINOPTION_ROUND_CORNER";fs_rndc(0.0-1.0)",                "0.0",               OPTION_FLOAT,      "screen round corner amount" },
	{ WINOPTION_SMOOTH_BORDER";fs_smob(0.0-1.0)",               "0.0",               OPTION_FLOAT,      "screen smooth border amount" },
	{ WINOPTION_REFLECTION";fs_ref(0.0-1.0)",                   "0.0",               OPTION_FLOAT,      "screen reflection amount" },
	{ WINOPTION_VIGNETTING";fs_vig(0.0-1.0)",                   "0.0",               OPTION_FLOAT,      "image vignetting amount" },
	{ WINOPTION_SCANLINE_AMOUNT";fs_scanam(0.0-4.0)",           "0.0",               OPTION_FLOAT,      "overall alpha scaling value for scanlines" },
	{ WINOPTION_SCANLINE_SCALE";fs_scansc(0.0-4.0)",            "1.0",               OPTION_FLOAT,      "overall height scaling value for scanlines" },
	{ WINOPTION_SCANLINE_HEIGHT";fs_scanh(0.0-4.0)",            "1.0",               OPTION_FLOAT,      "individual height scaling value for scanlines" },
	{ WINOPTION_SCANLINE_VARIATION";fs_scanv(0.0-4.0)",         "1.0",               OPTION_FLOAT,      "individual height varying value for scanlines" },
	{ WINOPTION_SCANLINE_BRIGHT_SCALE";fs_scanbs(0.0-2.0)",     "1.0",               OPTION_FLOAT,      "overall brightness scaling value for scanlines (multiplicative)" },
	{ WINOPTION_SCANLINE_BRIGHT_OFFSET";fs_scanbo(0.0-1.0)",    "0.0",               OPTION_FLOAT,      "overall brightness offset value for scanlines (additive)" },
	{ WINOPTION_SCANLINE_JITTER";fs_scanjt(0.0-4.0)",           "0.0",               OPTION_FLOAT,      "overall interlace jitter scaling value for scanlines" },
	{ WINOPTION_HUM_BAR_ALPHA";fs_humba(0.0-1.0)",              "0.0",               OPTION_FLOAT,      "overall alpha scaling value for hum bar" },
	{ WINOPTION_DEFOCUS";fs_focus",                             "0.0,0.0",           OPTION_STRING,     "overall defocus value in screen-relative coords" },
	{ WINOPTION_CONVERGE_X";fs_convx",                          "0.0,0.0,0.0",       OPTION_STRING,     "convergence in screen-relative X direction" },
	{ WINOPTION_CONVERGE_Y";fs_convy",                          "0.0,0.0,0.0",       OPTION_STRING,     "convergence in screen-relative Y direction" },
	{ WINOPTION_RADIAL_CONVERGE_X";fs_rconvx",                  "0.0,0.0,0.0",       OPTION_STRING,     "radial convergence in screen-relative X direction" },
	{ WINOPTION_RADIAL_CONVERGE_Y";fs_rconvy",                  "0.0,0.0,0.0",       OPTION_STRING,     "radial convergence in screen-relative Y direction" },
	{ WINOPTION_RED_RATIO";fs_redratio",                        "1.0,0.0,0.0",       OPTION_STRING,     "red output signal generated by input signal" },
	{ WINOPTION_GRN_RATIO";fs_grnratio",                        "0.0,1.0,0.0",       OPTION_STRING,     "green output signal generated by input signal" },
	{ WINOPTION_BLU_RATIO";fs_bluratio",                        "0.0,0.0,1.0",       OPTION_STRING,     "blue output signal generated by input signal" },
	{ WINOPTION_SATURATION";fs_sat(0.0-4.0)",                   "1.0",               OPTION_FLOAT,      "saturation scaling value" },
	{ WINOPTION_OFFSET";fs_offset",                             "0.0,0.0,0.0",       OPTION_STRING,     "signal offset value (additive)" },
	{ WINOPTION_SCALE";fs_scale",                               "1.0,1.0,1.0",       OPTION_STRING,     "signal scaling value (multiplicative)" },
	{ WINOPTION_POWER";fs_power",                               "1.0,1.0,1.0",       OPTION_STRING,     "signal power value (exponential)" },
	{ WINOPTION_FLOOR";fs_floor",                               "0.0,0.0,0.0",       OPTION_STRING,     "signal floor level" },
	{ WINOPTION_PHOSPHOR";fs_phosphor",                         "0.0,0.0,0.0",       OPTION_STRING,     "phosphorescence decay rate (0.0 is instant, 1.0 is forever)" },
	{ WINOPTION_YIQ_ENABLE";yiq",                               "0",                 OPTION_BOOLEAN,    "enables YIQ-space HLSL post-processing" },
	{ WINOPTION_YIQ_JITTER";yiqj",                              "0.0",               OPTION_FLOAT,      "Jitter for the NTSC signal processing" },
	{ WINOPTION_YIQ_CCVALUE";yiqcc",                            "3.57954545",        OPTION_FLOAT,      "Color Carrier frequency for NTSC signal processing" },
	{ WINOPTION_YIQ_AVALUE";yiqa",                              "0.5",               OPTION_FLOAT,      "A value for NTSC signal processing" },
	{ WINOPTION_YIQ_BVALUE";yiqb",                              "0.5",               OPTION_FLOAT,      "B value for NTSC signal processing" },
	{ WINOPTION_YIQ_OVALUE";yiqo",                              "0.0",               OPTION_FLOAT,      "Outgoing Color Carrier phase offset for NTSC signal processing" },
	{ WINOPTION_YIQ_PVALUE";yiqp",                              "1.0",               OPTION_FLOAT,      "Incoming Pixel Clock scaling value for NTSC signal processing" },
	{ WINOPTION_YIQ_NVALUE";yiqn",                              "1.0",               OPTION_FLOAT,      "Y filter notch width for NTSC signal processing" },
	{ WINOPTION_YIQ_YVALUE";yiqy",                              "6.0",               OPTION_FLOAT,      "Y filter cutoff frequency for NTSC signal processing" },
	{ WINOPTION_YIQ_IVALUE";yiqi",                              "1.2",               OPTION_FLOAT,      "I filter cutoff frequency for NTSC signal processing" },
	{ WINOPTION_YIQ_QVALUE";yiqq",                              "0.6",               OPTION_FLOAT,      "Q filter cutoff frequency for NTSC signal processing" },
	{ WINOPTION_YIQ_SCAN_TIME";yiqsc",                          "52.6",              OPTION_FLOAT,      "Horizontal scanline duration for NTSC signal processing (in usec)" },
	{ WINOPTION_YIQ_PHASE_COUNT";yiqpc",                        "2",                 OPTION_INTEGER,    "Phase Count value for NTSC signal processing" },
	{ WINOPTION_VECTOR_BEAM_SMOOTH";vecsmooth",                 "0.0",               OPTION_FLOAT,      "The vector beam smoothness" },
	{ WINOPTION_VECTOR_LENGTH_SCALE";vecscale",                 "0.5",               OPTION_FLOAT,      "The maximum vector attenuation" },
	{ WINOPTION_VECTOR_LENGTH_RATIO";vecratio",                 "0.5",               OPTION_FLOAT,      "The minimum vector length (vector length to screen size ratio) that is affected by the attenuation" },
	{ WINOPTION_BLOOM_BLEND_MODE,                               "0",                 OPTION_INTEGER,    "bloom blend mode (0 for brighten, 1 for darken)" },
	{ WINOPTION_BLOOM_SCALE,                                    "0.0",               OPTION_FLOAT,      "Intensity factor for bloom" },
	{ WINOPTION_BLOOM_OVERDRIVE,                                "1.0,1.0,1.0",       OPTION_STRING,     "Overdrive factor for bloom" },
	{ WINOPTION_BLOOM_LEVEL0_WEIGHT,                            "1.0",               OPTION_FLOAT,      "Bloom level 0 weight (full-size target)" },
	{ WINOPTION_BLOOM_LEVEL1_WEIGHT,                            "0.64",              OPTION_FLOAT,      "Bloom level 1 weight (1/4 smaller that level 0 target)" },
	{ WINOPTION_BLOOM_LEVEL2_WEIGHT,                            "0.32",              OPTION_FLOAT,      "Bloom level 2 weight (1/4 smaller that level 1 target)" },
	{ WINOPTION_BLOOM_LEVEL3_WEIGHT,                            "0.16",              OPTION_FLOAT,      "Bloom level 3 weight (1/4 smaller that level 2 target)" },
	{ WINOPTION_BLOOM_LEVEL4_WEIGHT,                            "0.08",              OPTION_FLOAT,      "Bloom level 4 weight (1/4 smaller that level 3 target)" },
	{ WINOPTION_BLOOM_LEVEL5_WEIGHT,                            "0.06",              OPTION_FLOAT,      "Bloom level 5 weight (1/4 smaller that level 4 target)" },
	{ WINOPTION_BLOOM_LEVEL6_WEIGHT,                            "0.04",              OPTION_FLOAT,      "Bloom level 6 weight (1/4 smaller that level 5 target)" },
	{ WINOPTION_BLOOM_LEVEL7_WEIGHT,                            "0.02",              OPTION_FLOAT,      "Bloom level 7 weight (1/4 smaller that level 6 target)" },
	{ WINOPTION_BLOOM_LEVEL8_WEIGHT,                            "0.01",              OPTION_FLOAT,      "Bloom level 8 weight (1/4 smaller that level 7 target)" },
	{ WINOPTION_TRIPLEBUFFER ";tb",                   "0",        OPTION_BOOLEAN,    "enables triple buffering" },
	{ WINOPTION_FULLSCREENBRIGHTNESS ";fsb(0.1-2.0)", "1.0",      OPTION_FLOAT,      "brightness value in full screen mode" },
	{ WINOPTION_FULLSCREENCONTRAST ";fsc(0.1-2.0)",   "1.0",      OPTION_FLOAT,      "contrast value in full screen mode" },
	{ WINOPTION_FULLSCREENGAMMA ";fsg(0.1-3.0)",      "1.0",      OPTION_FLOAT,      "gamma value in full screen mode" },
	{ WINOPTION_GLOBAL_INPUTS,                        "0",        OPTION_BOOLEAN,    "enables global inputs" },
	{ WINOPTION_DUAL_LIGHTGUN ";dual",                "0",        OPTION_BOOLEAN,    "enables dual lightgun input" },
	{ "$end" }
};

class winui_ini_options
{
	std::map<std::string, std::string> m_list;
	const char *m_filename;

	void create_index(std::ifstream &fp)
	{
		if (!fp.good())
			return;
		bool is_ready = false;
		std::string file_line;
		std::getline(fp, file_line);
		while (fp.good())
		{
			if (is_ready)
			{
				char s[file_line.length()+1];
				strcpy(s, file_line.c_str());

				const char* name = strtok(s, "\t");  // get adjustment name
				char* data = strtok(NULL, "\t");    // get next part (there's no next tab, so just return whatever is left)
				if (name)
					m_list[name] = data ? data : "";
			}
			else
			{
				if (file_line == "$start")
					is_ready = true;
			}

			std::getline(fp, file_line);
		}

		fp.close();
		return;
	}

public:
	// construction/destruction
	winui_ini_options()
	{
		// set up default values
		for (int i = 0; option_entries[i].name != "$end"; i++)
			m_list[option_entries[i].name] = option_entries[i].value;
		//printf("*** START DUMP OF DEFAULT ***\n");
		//for (auto const &it : m_list)
		//	printf("%s = %s\n", it.first.c_str(), it.second.c_str());
		//printf("*** END DUMP OF DEFAULT ***\n");
	}

	void load_file(const char *filename)
	{
		if (!m_filename)
			m_filename = filename;
		std::ifstream infile (filename);
		create_index(infile);
	}

	void save_file(const char *filename)
	{
		if (!filename)
			return;
		std::string inistring = std::string("\n -- message for the user --.\n\n$start\n");
		for (auto const &it : m_list)
			inistring.append(it.first).append("\t").append(it.second).append("\n");

		std::ofstream outfile (filename, std::ios::out | std::ios::trunc);
		size_t size = inistring.size();
		char t1[size+1];
		strcpy(t1, inistring.c_str());
		outfile.write(t1, size);
		outfile.close();
		return;
	}

	void reset_and_save(const char *filename)
	{
		// set up default values
		for (int i = 0; option_entries[i].name != "$end"; i++)
			m_list[option_entries[i].name] = option_entries[i].value;

		save_file(filename);
	}

	void setter(const char* name, std::string value)
	{
		m_list[name] = value;
		save_file(m_filename);
	}

	// cannot be "setter" otherwise most strings use it, causing "1" to be saved.
	//void bool_setter(const char* name, bool value)
	//{
	//	m_list[name] = value ? "1" : "0";
	//	save_file(m_filename);
	//}

	void setter(const char* name, int value)
	{
		m_list[name] = std::to_string(value);
		save_file(m_filename);
	}

	std::string getter(const char* name)
	{
		if (m_list.count(name))
			return m_list.find(name)->second;
		else
			return "";
	}

	int int_value(const char* name)
	{
		return std::stoi(getter(name));
	}

	bool bool_value(const char* name)
	{
		return int_value(name) ? 1 : 0;
	}
};

#endif //  INI_OPTS_H

