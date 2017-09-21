// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_GAMEOPTS_H
#define WINUI_GAMEOPTS_H

class winui_game_options
{
	uint32_t m_total;
	uint32_t m_cache;
	uint32_t m_version;
	bool m_rebuild;

	struct driver_options
	{
		uint32_t game_number;
		int rom;
		int sample;
		uint32_t cache_lower;
		uint32_t cache_upper;
		uint32_t play_count;
		uint32_t play_time;
	};

	std::vector<driver_options> m_list;

	// convert audit cache - normally only 1 digit, although we can do 2. If the input is -1, it is treated as invalid and -1 is returned.
	int convert_to_int(const char* inp)
	{
		if (!inp)
			return -1;
		int c = inp[0];
		if (c < 0x30 || c > 0x39)
			return -1;
		int oup = c - 0x30;
		c = inp[1];
		if (c < 0x30 || c > 0x39)
			return oup;
		else
			return oup * 10 + (c - 0x30);
	}

	// convert all other numbers, up to end-of-string/invalid-character. If number is too large, return 0.
	uint32_t convert_to_uint(const char* inp)
	{
		if (!inp)
			return 0;
		uint32_t oup = 0;
		for (int i = 0; i < 11; i++)
		{
			int c = inp[i];
			if (c >= 0x30 && c <= 0x39)
				oup = oup * 10 + (c - 0x30);
			else
				return oup;
		}
		return 0; // numeric overflow
	}

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

		char s1[file_line.length()+1];
		strcpy(s1, file_line.c_str());

		// check that totals match
		const char* s2 = strtok(s1, ",");
		if (!s2)
			return true;  // file is corrupt

		uint32_t total = convert_to_uint(s2);
		if (total == m_total)
			m_rebuild = false; // everything is fine so far

		// check that versions match
		s2 = strtok(NULL, ",");
		if (!s2)
			return true;  // file is corrupt or old version

		uint32_t version = convert_to_uint(s2);
		if (version != m_version)
			return true; // file is old version, complete rebuild necessary

		// all good, read file to memory. If rebuild needed, it will overwrite cache, but we need the rest.
		int index; // must be int for validity check below
		bool is_ready = false;
		std::getline(fp, file_line);
		while (fp.good())
		{
			if (is_ready)
			{
				char s[file_line.length()+1];
				strcpy(s, file_line.c_str());

				const char* name = strtok(s, "\t");  // get driver name
				index = driver_list::find(name);
				if (index > -1)
				{
					char* data = strtok(NULL, ",");    // get next part (old game number, ignore)
					m_list[index].game_number = index;  // get new game number
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].rom = convert_to_int(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].sample = convert_to_int(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].cache_lower = convert_to_uint(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].cache_upper = convert_to_uint(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].play_count = convert_to_uint(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].play_time = convert_to_uint(data);
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
		m_version = 1;
		m_rebuild = true;
		m_total = driver_list::total();
		m_list.resize(m_total);
		std::fill(m_list.begin(), m_list.end(), driver_options{0, -1, -1, 0, 0, 0, 0});
	}

	int  rom(uint32_t index)
	{
		if (index < m_total)
			return m_list[index].rom;
		else
			return -1;
	}

	void rom(uint32_t index, int val)
	{
		if (index < m_total)
			m_list[index].rom = val;
	}

	int  sample(uint32_t index)
	{
		if (index < m_total)
			return m_list[index].sample;
		else
			return -1;
	}

	void sample(uint32_t index, int val)
	{
		if (index < m_total)
			m_list[index].sample = val;
	}

	uint32_t cache_lower(uint32_t index)
	{
		if (index < m_total)
			return m_list[index].cache_lower;
		else
			return 0;
	}

	uint32_t cache_upper(uint32_t index)
	{
		if (index < m_total)
			return m_list[index].cache_upper;
		else
			return 0;
	}

	void cache_upper(uint32_t index, uint32_t val)
	{
		if (index < m_total)
			m_list[index].cache_upper = val;
	}

	uint32_t play_count(uint32_t index)
	{
		if (index < m_total)
			return m_list[index].play_count;
		else
			return 0;
	}

	void play_count(uint32_t index, int val)
	{
		if (index < m_total)
			m_list[index].play_count = val;
	}

	uint32_t play_time(uint32_t index)
	{
		if (index < m_total)
			return m_list[index].play_time;
		else
			return 0;
	}

	void play_time(uint32_t index, int val)
	{
		if (index < m_total)
			m_list[index].play_time = val;
	}

	bool rebuild()
	{
		return m_rebuild;
	}

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
			for (uint32_t i = 0; i < m_total; i++)
			{
				uint32_t t = 0;
				// BITS 0,1 = arcade, console, computer, other
				drv = &driver_list::driver(i);
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
				t = (info.machine_flags() & machine_flags::SUPPORTS_SAVE) ? 0: 0x0080;
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
				t = (ft & device_t::feature::SOUND) ? 0x10000 : 0;
				m_cache |= t;
				// BIT 18 = IMPERFECT_GRAPHICS
				t = (ft & device_t::feature::GRAPHICS) ? 0x40000 : 0;
				m_cache |= t;
				// BIT 20 = IMPERFECT_COLOUR
				t = (ft & device_t::feature::PALETTE) ? 0x100000 : 0;
				m_cache |= t;
				// BIT 22 = PROTECTION
				t = (ft & device_t::feature::PROTECTION) ? 0x400000 : 0;
				m_cache |= t;
				// BIT 23 = IMPERFECT_CONTROLS
				t = (ft & device_t::feature::CONTROLS) ? 0x800000 : 0;
				m_cache |= t;

				ft = info.unemulated_features();
				// BIT 17 = NO_SOUND
				t = (ft & device_t::feature::SOUND) ? 0x20000 : 0;
				m_cache |= t;
				// BIT 19 = NO_GRAPHICS
				t = (ft & device_t::feature::GRAPHICS) ? 0x80000 : 0;
				m_cache |= t;
				// BIT 21 = NO_COLOUR
				t = (ft & device_t::feature::PALETTE) ? 0x200000 : 0;
				m_cache |= t;

				m_list[i].cache_lower = m_cache;
				m_list[i].cache_upper = 0;
			}
			printf("game_opts.cpp::load_file : Finished Rebuilding cache\n");fflush(stdout);
		}
	}

	void save_file(const char *filename)
	{
		string inistring = std::to_string(m_total).append(",").append(std::to_string(m_version)).append("\n");
		inistring.append("YOU CAN SAFELY DELETE THIS FILE TO RESET THE GAME STATS.\n\n$start\n");

		for (uint32_t i = 0; i < m_total; i++)
		{
			// 1:Game number(for debugging only), 2:Rom, 3:Sample, 4:Cache(New), 5:Cache(Legacy) 6:Play Count, 7:Play Time
			inistring.append(driver_list::driver(i).name).append("\t");
			inistring.append(std::to_string(m_list[i].game_number)).append(",");
			inistring.append(std::to_string(m_list[i].rom)).append(",");
			inistring.append(std::to_string(m_list[i].sample)).append(",");
			inistring.append(std::to_string(m_list[i].cache_lower)).append(",");
			inistring.append(std::to_string(m_list[i].cache_upper)).append(",");
			inistring.append(std::to_string(m_list[i].play_count)).append(",");
			inistring.append(std::to_string(m_list[i].play_time)).append("\n");
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

