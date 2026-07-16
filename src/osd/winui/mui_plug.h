// For licensing and usage information, read docs/release/winui_license.txt
//****************************************************************************

#ifndef WINUI_PLUG_H
#define WINUI_PLUG_H


#endif // WINUI_PLUG_H

#pragma once

#include "corefile.h"
#include "winopts.h"

#include <list>
#include <string>


// ======================> plugin_options

class mui_plugin_options
{
public:
	struct plugin
	{
		std::string m_name;
		std::string m_description;
		std::string m_type;
		std::string m_directory;
		bool        m_start;
	};

	mui_plugin_options();

	// public access
	void init_plug(windows_options& o);
	std::pair<std::string, std::string> get_lists(windows_options& o);
	std::pair<std::string, std::string> split_into_lists(windows_options& o, int nGame, std::string chosen);

	// accessors
	std::list<plugin> &plugins() { return m_plugins; }
	const std::list<plugin> &plugins() const { return m_plugins; }

private:
	// methods
	void scan_directory(const std::string &path, bool recursive);
	bool load_plugin(const std::string &path);
	plugin *find(const std::string &name);

	// INI functionality
	void parse_ini_file(util::core_file &inifile);
	std::string output_ini() const;

	std::list<plugin> m_plugins;
};

