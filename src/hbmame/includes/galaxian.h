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

	// test for tenn
	DECLARE_WRITE8_MEMBER(tenn_unk_6000_w);
	DECLARE_WRITE8_MEMBER(tenn_unk_8000_w);
	DECLARE_WRITE8_MEMBER(tenn_unk_e000_w);
	DECLARE_READ8_MEMBER(tenn_dsw_read);
	void tenn_set_game_bank(int bank, int from_game);
	DECLARE_INPUT_CHANGED_MEMBER(tenn_fake);
	void tenn_select_map(address_map &map);
	int m_tenn_current_game;
public:
	void tenn(machine_config &config);
	void init_tenn();
};
