// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/includes/galaxian.h"

class galaxian_hbmame : public galaxian_state
{
public:
	galaxian_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: galaxian_state(mconfig, type, tag)
		{ }

	void jumpbugx(machine_config &config);
	void tst_frog(machine_config &config);
	void init_trukker();

private:
	void jumpbugx_map(address_map &map);
	void tst_frog_map(address_map &map);
};
