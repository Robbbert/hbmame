// For licensing and usage information, read docs/release/winui_license.txt

/* All things to do with lua plugins.

Determine which plugins are valid, which ones are wanted and which ones are not wanted.

1. Valid plugins are those that do not say "library" in the associated json file.

2. Plugin.ini has a global list of plugins and is the starting point of what is wanted.
   mame.ini also has plugin (additional wanted entries), and noplugin (unwanted entries).
   So in the end it's (plugin.ini | plugin) - noplugin.

   For our purposes, the global settings do not use mame.ini, plugin and noplugin are set blank.
   However, for each game, that game's ini file will specify extras to include and what to remove.

3. Winui will display the final list once everything has been taken into account. When plugins
   are added and removed, winui will adjust the game's plugin and noplugin entries accordingly.

=================================================================================================== */
#include "emu.h"
#include <windows.h>

#include "fileio.h"
#include "osdcore.h"
#include "options.h"
#include "path.h"
#include "emuopts.h"

#include "mui_plug.h"
#include "emu_opts.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>


//**************************************************************************
//  PLUGIN OPTIONS
//**************************************************************************

//-------------------------------------------------
//  plugin_options - constructor
//-------------------------------------------------

mui_plugin_options::mui_plugin_options()
{
}

//-------------------------------------------------
//  find
//-------------------------------------------------

mui_plugin_options::plugin *mui_plugin_options::find(const std::string &name)
{
	auto iter = std::find_if(
		m_plugins.begin(),
		m_plugins.end(),
		[&name](const plugin &p) { return name == p.m_name; });

	return iter != m_plugins.end() ? &*iter : nullptr;
}


//---------------------------------------------------------
//  load_plugin - make sure plugin is ok and not a library
//---------------------------------------------------------

bool mui_plugin_options::load_plugin(const std::string &path)
{
	std::ifstream ifs(path);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream<0>(isw);

	if (document.HasParseError())
	{
		const std::string error(GetParseError_En(document.GetParseError()));
		osd_printf_warning("Unable to parse plugin definition file %s. Errors returned:\n%s", path, error);
		return false;
	}

	if (!document["plugin"].IsObject())
	{
		osd_printf_warning("Bad plugin definition file %s:\n", path);
		return false;
	}

	size_t last_path_sep = path.find_last_of(PATH_SEPARATOR[0]);
	std::string dir = (last_path_sep != std::string::npos) ? path.substr(0, last_path_sep) : ".";

	std::string name, type, desc;

	// "name" is a required key field
	if (document["plugin"].HasMember("name"))
	{
		name = document["plugin"]["name"].GetString();
		// Reject duplicates
		if (find(name))
			return false;
	}
	else
		return false;

	// If "type" is missing, reject plugin
	if (document["plugin"].HasMember("type"))
	{
		// Don't want type of "library"
		type = document["plugin"]["type"].GetString();
		if (type == "library")
			return false;
	}
	else
		return false;

	// "description" is optional
	if (document["plugin"].HasMember("description"))
		desc = document["plugin"]["description"].GetString();
	else
		desc = "No description provided";

	plugin p;
	p.m_start       = false;
	p.m_directory   = std::move(dir);
	p.m_name        = std::move(name);
	p.m_type        = std::move(type);
	p.m_description = std::move(desc);

	if (document["plugin"].HasMember("start"))
		if (std::string(document["plugin"]["start"].GetString()) == "true")
			p.m_start = true;

	m_plugins.push_back(std::move(p));
	return true;
}


//-------------------------------------------------
//  create_core_options
//-------------------------------------------------

static core_options create_core_options(const mui_plugin_options &plugin_opts)
{
	// we're sort of abusing core_options to just get INI file parsing, so we'll build a
	// core_options structure for the sole purpose of parsing an INI file, and then reflect
	// the data back
	static const options_entry s_option_entries[] =
	{
		{ nullptr, nullptr, core_options::option_type::HEADER, "PLUGINS OPTIONS" }, { nullptr }
	};

	core_options opts;
	opts.add_entries(s_option_entries);

	// create an entry for each option
	for (const mui_plugin_options::plugin &p : plugin_opts.plugins())
		opts.add_entry( { p.m_name }, nullptr, core_options::option_type::BOOLEAN, p.m_start ? "1" : "0");

	return opts;
}


//-------------------------------------------------
//  parse_ini_file
//-------------------------------------------------

void mui_plugin_options::parse_ini_file(util::core_file &inifile)
{
	core_options opts = create_core_options(*this);

	// parse the INI file
	opts.parse_ini_file(inifile, OPTION_PRIORITY_NORMAL, true, true);

	// and reflect these options back
	for (plugin &p : m_plugins)
		p.m_start = opts.bool_value(p.m_name);
}


//-------------------------------------------------
//  output_ini
//-------------------------------------------------

std::string mui_plugin_options::output_ini() const
{
	core_options opts = create_core_options(*this);
	return opts.output_ini();
}


//-------------------------------------------------
//  split - a fancy strtok
//-------------------------------------------------

static std::vector<std::string> split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos)
	{
		std::string temp = text.substr(start, end - start);
		if (temp != "") tokens.push_back(temp);
		start = end + 1;
	}
	std::string temp = text.substr(start);
	if (temp != "") tokens.push_back(temp);
	return tokens;
}


//-------------------------------------------------
//  scan_directory
//-------------------------------------------------

void mui_plugin_options::scan_directory(const std::string &path, bool recursive)
{
	// first try to open as a directory
	osd::directory::ptr directory = osd::directory::open(path);
	if (directory)
	{
		// iterate over all files in the directory
		for (const osd::directory::entry *entry = directory->read(); entry != nullptr; entry = directory->read())
		{
			if (entry->type == osd::directory::entry::entry_type::FILE && !strcmp(entry->name, "plugin.json"))
			{
				load_plugin(util::path_concat(path, entry->name));
			}
			else
			if (entry->type == osd::directory::entry::entry_type::DIR)
			{
				if (recursive && strcmp(entry->name, ".") && strcmp(entry->name, ".."))
					scan_directory(util::path_concat(path, entry->name), recursive);
			}
		}
	}
}


//----------------------------------------------------
//  get_lists -
//       ret1 = enabled plugins for string list
//       ret2 = all valid plugins for dropdown list
//----------------------------------------------------

std::pair<std::string, std::string>mui_plugin_options::get_lists(windows_options& o)
{
	m_plugins.clear();
	std::string path, ret1, ret2;

	// scan all plugin directories - discard duplicates
	path_iterator iter(o.plugins_path());
	while (iter.next(path))
	{
		// scan the directory recursively
		scan_directory(path, true);
	}

	// parse plugin.ini
	emu_file file(GetIniDir(), OPEN_FLAG_READ);
	if (!file.open("plugin.ini"))
	{
		try
		{
			parse_ini_file((util::core_file&)file);
		}
		catch (options_exception &)
		{
			osd_printf_error("**Error loading plugin.ini**\n");
		}
	}

	// process "plugin" - ignore unknowns
	for (const std::string &plug : split(o.plugin(), ','))
	{
		plugin *p = find(plug);
		if (p)
			p->m_start = true;
	}

	// process "noplugin" - ignore unknowns
	for (const std::string &plug : split(o.no_plugin(), ','))
	{
		plugin *p = find(plug);
		if (p)
			p->m_start = false;
	}

	// At this point we have the final list of what is in and what is out
	// Print it to make sure
	//printf("FINAL LIST OF PLUGINS\n");
	//for (plugin &p : m_plugins)
		//printf("%s = %d\n",p.m_name.c_str(),p.m_start);

	for (plugin &p : m_plugins)
	{
		// List of valid plugins
		if (ret2.empty())
			ret2 = p.m_name;
		else
			ret2 = ret2 + "," + p.m_name;

		// List of enabled plugins
		if (p.m_start == true)
		{
			if (ret1.empty())
				ret1 = p.m_name;
			else
				ret1 = ret1 + "," + p.m_name;
		}
	}

	return std::make_pair(ret1, ret2);
}


//-----------------------------------------------------
//  split_into_lists -
//       ret1 = "plugin"
//       ret2 = "noplugin"
//  if global, the above lists are made empty,
//   and plugin.ini gets updated instead
//-----------------------------------------------------

std::pair<std::string, std::string>mui_plugin_options::split_into_lists(windows_options& o, int nGame, std::string chosen)
{
	m_plugins.clear();
	std::string path, ret1, ret2;

	// scan all plugin directories - discard duplicates
	path_iterator iter(o.plugins_path());
	while (iter.next(path))
	{
		// scan the directory recursively
		scan_directory(path, true);
	}

	// To begin, set all false
	for (plugin &p : m_plugins)
		p.m_start = false;

	if (nGame == -1)
	{
		// Global - update plugin.ini, return empty lists

		// The ones in the incoming string are validated and set true
		for (const std::string &plug : split(chosen, ','))
		{
			plugin *p = find(plug);
			if (p)
				p->m_start = true;
		}

		// convert to ini format
		std::string pluglist(output_ini());

		// write new plugin.ini
		emu_file file(GetIniDir(), OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS);
		if (file.open("plugin.ini"))
			osd_printf_error("Unable to create plugin.ini, changes lost\n");
		else
			file.puts(pluglist);
	}
	else
	if (nGame >= 0)
	{
		// per-game - compare string against plugin.ini, return delta

		// parse plugin.ini
		emu_file file(GetIniDir(), OPEN_FLAG_READ);
		if (!file.open("plugin.ini"))
		{
			try
			{
				parse_ini_file((util::core_file&)file);
			}
			catch (options_exception &)
			{
				osd_printf_error("Plugin.ini not usable, ignored\n");
			}
		}

		// "plugin" entries - ignore ones not exist
		for (const std::string &plug : split(chosen, ','))
		{
			plugin *p = find(plug);
			if (p)
			{
				if (p->m_start == false)
				{
					if (ret1.empty())
						ret1 = plug;
					else
						ret1 = ret1 + "," + plug;
				}
			}
		}

		// "noplugin" entries - ignore ones not exist
		chosen = "," + chosen + ",";
		for (plugin &p : m_plugins)
		{
			if (p.m_start == true)
			{
				std::size_t found = chosen.find("," + p.m_name + ",");
				if (found == std::string::npos)
				{
					if (ret2.empty())
						ret2 = p.m_name;
					else
						ret2 = ret2 + "," + p.m_name;
				}
			}
		}
	}

	return std::make_pair(ret1, ret2);
}


//-----------------------------------------------------
//   If plugin.ini not exist, make a default one
//-----------------------------------------------------

void mui_plugin_options::init_plug(windows_options& o)
{
	m_plugins.clear();
	std::string path;

	// see if plugin.ini exist
	emu_file file(GetIniDir(), OPEN_FLAG_READ | OPEN_FLAG_NO_PRELOAD);
	if (file.open("plugin.ini"))
	{
		// find out what the defaults are
		path_iterator iter(o.plugins_path());
		while (iter.next(path))
			scan_directory(path, true);

		// convert results to ini format
		std::string pluglist(output_ini());

		// write new plugin.ini
		emu_file file(GetIniDir(), OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS);
		if (file.open("plugin.ini"))
			osd_printf_error("Unable to create new plugin.ini\n");
		else
			file.puts(pluglist);
	}
}


