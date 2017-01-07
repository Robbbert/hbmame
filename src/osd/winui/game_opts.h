// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef __GAME_OPTS_H__
#define __GAME_OPTS_H__

class game_options
{
public:
	// construction/destruction
	game_options()
	{
		m_total = driver_list::total();
		m_list.reserve(m_total);

		driver_options option = { -1, -1, -1, 0, 0 };

		for (int i = 0; i < m_total; i++)
			m_list[i] = option;
	}

	int  rom(int index)                 { assert(0 <= index && index < driver_list::total()); return m_list[index].rom; }
	void rom(int index, int val)        { assert(0 <= index && index < driver_list::total()); m_list[index].rom = val; }

	int  sample(int index)              { assert(0 <= index && index < driver_list::total()); return m_list[index].sample; }
	void sample(int index, int val)     { assert(0 <= index && index < driver_list::total()); m_list[index].sample = val; }

	int  cache(int index)               { assert(0 <= index && index < driver_list::total()); return m_list[index].cache; }
	void cache(int index, int val)      { assert(0 <= index && index < driver_list::total()); m_list[index].cache = val; }

	int  play_count(int index)          { assert(0 <= index && index < driver_list::total()); return m_list[index].play_count; }
	void play_count(int index, int val) { assert(0 <= index && index < driver_list::total()); m_list[index].play_count = val; }

	int  play_time(int index)           { assert(0 <= index && index < driver_list::total()); return m_list[index].play_time; }
	void play_time(int index, int val)  { assert(0 <= index && index < driver_list::total()); m_list[index].play_time = val; }

	void add_entries()
	{
		options_entry entry[2] = { { 0 }, { 0 } };

		// 1:Rom, 2:Sample, 3:Cache, 4:Play Count, 5:Play Time
		entry[0].defvalue    = "-1;-1;-1";
		entry[0].flags       = OPTION_STRING;
		entry[0].description = NULL;

		for (int i = 0; i < m_total; i++)
		{
			entry[0].name = driver_list::driver(i).name;
			m_info.add_entries(entry);
		}
	}

	osd_file::error load_file(const char *filename)
	{
		emu_file file(OPEN_FLAG_READ);

		osd_file::error filerr = file.open(filename);
		if (filerr == osd_file::error::NONE)
		{
			std::string error_string;
			m_info.parse_ini_file(file, OPTION_PRIORITY_CMDLINE, OPTION_PRIORITY_CMDLINE, error_string);
		}

		load_settings();

		return filerr;
	}

	void output_ini(std::string &buffer, const char *header = NULL)
	{
		std::string inibuffer = m_info.output_ini();

		if (header && !inibuffer.empty())
		{
			buffer.append(string_format("#\n# %s\n#\n", header));
			buffer.append(inibuffer);
		}
	}

	void load_settings(void)
	{
		std::string value_str;

		for (int i = 0; i < m_total; i++)
		{
			value_str.assign(m_info.value(driver_list::driver(i).name));

			if (!value_str.empty())
				load_settings(value_str.c_str(), i);
		}
	}

	void load_settings(const char *str, int index)
	{
		path_iterator   path(str);
		std::string     curpath;
		int             value_int;

		for (int i = 0; i < 5; i++)
		{
			if ( path.next(curpath) )
			{
				if (!curpath.empty() && (sscanf(curpath.c_str(), "%d", &value_int) == 1))
				{
					switch (i)
					{
						case 0:  m_list[index].rom        = value_int;  break;
						case 1:  m_list[index].sample     = value_int;  break;
						case 2:  m_list[index].cache      = value_int;  break;
						case 3:  m_list[index].play_count = value_int;  break;
						case 4:  m_list[index].play_time  = value_int;  break;
					}
				}
			}
			else
				break;
		}
	}

	void save_settings(void)
	{
		std::string value_str;
		std::string error_string;

		for (int i = 0; i < m_total; i++)
		{
			value_str = string_format("%d;%d;%d;%d;%d", m_list[i].rom, m_list[i].sample, m_list[i].cache, m_list[i].play_count, m_list[i].play_time);
			m_info.set_value(driver_list::driver(i).name, value_str.c_str(), OPTION_PRIORITY_CMDLINE, error_string);
		}
	}

	osd_file::error save_file(const char *filename)
	{
		osd_file::error filerr;
		std::string inistring;

		save_settings();

		output_ini(inistring, "GAME STATISTICS");

		emu_file file(OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS);
		filerr = file.open(filename);
		if (filerr == osd_file::error::NONE)
			file.puts(inistring.c_str());

		return filerr;
	}

private:
	core_options m_info;
	int         m_total;

	struct driver_options
	{
		int rom;
		int sample;
		int cache;
		int play_count;
		int play_time;
	};

	std::vector<driver_options> m_list;
};

#endif //  __GAME_OPTS_H__

