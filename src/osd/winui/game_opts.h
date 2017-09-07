// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_GAMEOPTS_H
#define WINUI_GAMEOPTS_H

class winui_game_options
{
	int m_total;
	uint32_t m_cache;
	bool m_rebuild;

	struct driver_options
	{
		int rom;
		int sample;
		uint32_t cache_lower;
		uint32_t cache_upper;
		int play_count;
		int play_time;
	};

	std::vector<driver_options> m_list;

	// true = recache needed
	bool create_index(std::ifstream &fp)
	{
		// does file exist?
		if (!fp.good())
			return true;

		// check first line is ok
		string file_line;
		std::getline(fp, file_line);
		if (file_line.empty())
			return true;

		// check that totals match
		int total = std::atoll(file_line.c_str());
		if (total == m_total)
			m_rebuild = false;

		// all good, read file to memory. If rebuild needed, it will overwrite cache, but we need the rest.
		int index;
		bool is_ready = false;
		std::getline(fp, file_line);
		while (fp.good())
		{
			if (is_ready)
			{
				char s[file_line.length()];
				strcpy(s, file_line.c_str());

				const char* name = strtok(s, "\t");  // get driver name
				index = driver_list::find(name);
				if (index > -1)
				{
					char* data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].rom = std::atoll(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].sample = std::atoll(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].cache_lower = std::atoll(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].cache_upper = std::atoll(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].play_count = std::atoll(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].play_time = std::atoll(data);
				}
			}
			else
			{
				if (file_line == "$start")
					is_ready = true;
			}

			std::getline(fp, file_line);
		}

		fp.close();
		return m_rebuild;
	}

public:
	// construction - runs before main
	winui_game_options()
	{
		m_rebuild = true;
		m_total = driver_list::total();
		m_list.reserve(m_total);

		driver_options option = { -1, -1, 0, 0, 0, 0 };

		for (int i = 0; i < m_total; i++)
			m_list[i] = option;
	}

	int  rom(int index)                       { return m_list[index].rom; }
	void rom(int index, int val)              { m_list[index].rom = val; }

	int  sample(int index)                    { return m_list[index].sample; }
	void sample(int index, int val)           { m_list[index].sample = val; }

	uint32_t cache_lower(int index)           { return m_list[index].cache_lower; }
	uint32_t cache_upper(int index)           { return m_list[index].cache_upper; }
	void cache_upper(int index, uint32_t val) { m_list[index].cache_upper = val; }

	int  play_count(int index)                { return m_list[index].play_count; }
	void play_count(int index, int val)       { m_list[index].play_count = val; }

	int  play_time(int index)                 { return m_list[index].play_time; }
	void play_time(int index, int val)        { m_list[index].play_time = val; }

	bool rebuild()                            { return m_rebuild; }

	void load_file(const char *filename)
	{
		std::ifstream infile (filename);
		if (create_index(infile))
		{
			// rebuild the cache
			device_t::feature_type ft;
			const game_driver *drv;
			windows_options dummy;
			printf("game_opts.cpp::load_file : Rebuilding cache\n");fflush(stdout);
			for (int i = 0; i < m_total; i++)
			{
				int t = 0;
				// BITS 0,1 = arcade, console, computer, other
				drv  = &driver_list::driver(i);
				ui::machine_static_info const info(machine_config(*drv, dummy));
				if ((info.machine_flags() & machine_flags::MASK_TYPE) == machine_flags::TYPE_CONSOLE)
					t = 1;
				else
				if ((info.machine_flags() & machine_flags::MASK_TYPE) == machine_flags::TYPE_COMPUTER)
					t = 2;
				else
				if ((info.machine_flags() & machine_flags::MASK_TYPE) == machine_flags::TYPE_OTHER)
					t = 3;
				m_cache = t;
				// BIT 2 = SWAP_XY
				t = ((info.machine_flags() & machine_flags::MASK_ORIENTATION) == machine_flags::SWAP_XY) ? 0x0004 : 0;
				m_cache |= t;
				// BIT 6 = NOT_WORKING
				t = (info.machine_flags() & machine_flags::NOT_WORKING) ? 0x0040 : 0;
				m_cache |= t;
				// BIT 7 = SUPPORTS_SAVE
				t = (info.machine_flags() & machine_flags::SUPPORTS_SAVE) ? 0x0080 : 0;
				m_cache |= t;
				// BIT 8 = NO_COCKTAIL
				t = (info.machine_flags() & machine_flags::NO_COCKTAIL) ? 0x0100 : 0;
				m_cache |= t;
				// BIT 9 = IS_BIOS_ROOT
				t = (info.machine_flags() & machine_flags::IS_BIOS_ROOT) ? 0x0200 : 0;
				m_cache |= t;
				// BIT 10 = REQUIRES_ARTWORK
				t = (info.machine_flags() & machine_flags::REQUIRES_ARTWORK) ? 0x0400 : 0;
				m_cache |= t;
				// BIT 11 = CLICKABLE_ARTWORK
				t = (info.machine_flags() & machine_flags::CLICKABLE_ARTWORK) ? 0x0800 : 0;
				m_cache |= t;
				// BIT 12 = UNOFFICIAL
				t = (info.machine_flags() & machine_flags::UNOFFICIAL) ? 0x1000 : 0;
				m_cache |= t;
				// BIT 13 = NO_SOUND_HW
				t = (info.machine_flags() & machine_flags::NO_SOUND_HW) ? 0x2000 : 0;
				m_cache |= t;
				// BIT 14 = MECHANICAL
				t = (info.machine_flags() & machine_flags::MECHANICAL) ? 0x4000 : 0;
				m_cache |= t;
				// BIT 15 = IS_INCOMPLETE
				t = (info.machine_flags() & machine_flags::IS_INCOMPLETE) ? 0x8000 : 0;
				m_cache |= t;

				ft = info.imperfect_features();
				// BIT 16 = IMPERFECT_SOUND
				t = (ft & device_t::feature::SOUND) ? 1 : 0; // 0x10000
				m_cache |= (t << 16);
				// BIT 18 = IMPERFECT_GRAPHICS
				t = (ft & device_t::feature::GRAPHICS) ? 1 : 0; // 0x40000
				m_cache |= (t << 18);
				// BIT 20 = IMPERFECT_COLOUR
				t = (ft & device_t::feature::PALETTE) ? 1 : 0; // 0x100000
				m_cache |= (t << 20);
				// BIT 22 = PROTECTION
				t = (ft & device_t::feature::PROTECTION) ? 1 : 0; // 0x400000
				m_cache |= (t << 22);
				// BIT 23 = IMPERFECT_CONTROLS
				t = (ft & device_t::feature::CONTROLS) ? 1 : 0; // 0x800000
				m_cache |= (t << 23);

				ft = info.unemulated_features();
				// BIT 17 = NO_SOUND
				t = (ft & device_t::feature::SOUND) ? 1 : 0; // 0x20000
				m_cache |= (t << 17);
				// BIT 19 = NO_GRAPHICS
				t = (ft & device_t::feature::GRAPHICS) ? 1 : 0; // 0x80000
				m_cache |= (t << 19);
				// BIT 21 = NO_COLOUR
				t = (ft & device_t::feature::PALETTE) ? 1 : 0; // 0x200000
				m_cache |= (t << 21);

				m_list[i].cache_lower = m_cache;
				m_list[i].cache_upper = 0;
			}
		}
	}

	void save_file(const char *filename)
	{
		string inistring = std::to_string(m_total).append("\n").append("YOU CAN SAFELY DELETE THIS FILE TO RESET THE GAME STATS.\n\n$start\n");
		for (int i = 0; i < m_total; i++)
		{
			// 1:Rom, 2:Sample, 3:Cache, 4:Play Count, 5:Play Time
			inistring.append(driver_list::driver(i).name).append("\t");
			inistring.append(std::to_string(m_list[i].rom)).append(",");
			inistring.append(std::to_string(m_list[i].sample)).append(",");
			inistring.append(std::to_string(m_list[i].cache_lower)).append(",");
			inistring.append(std::to_string(m_list[i].cache_upper)).append(",");
			inistring.append(std::to_string(m_list[i].play_count)).append(",");
			inistring.append(std::to_string(m_list[i].play_time)).append(",\n");
		}

		std::ofstream outfile (filename, std::ios::out | std::ios::trunc);
		size_t size = inistring.size();
		char t1[size+1];
		strcpy(t1, inistring.c_str());
		outfile.write(t1, size);
		outfile.close();
		return;
	}
};

#endif //  __GAME_OPTS_H__

