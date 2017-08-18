// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

#ifndef WINUI_GAMEOPTS_H
#define WINUI_GAMEOPTS_H

class winui_game_options
{
	int m_total;

	struct driver_options
	{
		int rom;
		int sample;
		int cache;
		int play_count;
		int play_time;
	};

	std::vector<driver_options> m_list;

	void create_index(std::ifstream &fp)
	{
		if (!fp.good())
			return;
		int index;
		bool is_ready = false;
		std::string file_line;
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
						m_list[index].rom = std::stoi(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].sample = std::stoi(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].cache = std::stoi(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].play_count = std::stoi(data);
					data = strtok(NULL, ",");    // get next part
					if (data)
						m_list[index].play_time = std::stoi(data);
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
		return;
	}

public:
	// construction/destruction
	winui_game_options()
	{
		m_total = driver_list::total();
		m_list.reserve(m_total);

		driver_options option = { -1, -1, -1, 0, 0 };

		for (int i = 0; i < m_total; i++)
			m_list[i] = option;
	}

	int  rom(int index)                 { return m_list[index].rom; }
	void rom(int index, int val)        { m_list[index].rom = val; }

	int  sample(int index)              { return m_list[index].sample; }
	void sample(int index, int val)     { m_list[index].sample = val; }

	int  cache(int index)               { return m_list[index].cache; }
	void cache(int index, int val)      { m_list[index].cache = val; }

	int  play_count(int index)          { return m_list[index].play_count; }
	void play_count(int index, int val) { m_list[index].play_count = val; }

	int  play_time(int index)           { return m_list[index].play_time; }
	void play_time(int index, int val)  { m_list[index].play_time = val; }

	void load_file(const char *filename)
	{
		std::ifstream infile (filename);
		create_index(infile);
	}

	void save_file(const char *filename)
	{
		std::string inistring = std::string("\nYOU CAN SAFELY DELETE THIS FILE TO RESET THE GAME STATS.\n\n$start\n");
		for (int i = 0; i < m_total; i++)
		{
			// 1:Rom, 2:Sample, 3:Cache, 4:Play Count, 5:Play Time
			if ((m_list[i].rom != -1) || (m_list[i].play_count > 0))
			{
				inistring.append(driver_list::driver(i).name).append("\t");
				inistring.append(std::to_string(m_list[i].rom)).append(",");
				inistring.append(std::to_string(m_list[i].sample)).append(",");
				inistring.append(std::to_string(m_list[i].cache)).append(",");
				inistring.append(std::to_string(m_list[i].play_count)).append(",");
				inistring.append(std::to_string(m_list[i].play_time)).append(",\n");
			}
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

