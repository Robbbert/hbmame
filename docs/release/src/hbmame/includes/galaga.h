// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/includes/galaga.h"
#include "sound/samples.h"
#include "machine/namco06.h"

class galaga_hbmame : public galaga_state
{
public:
	galaga_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: galaga_state(mconfig, type, tag)
		, m_samples(*this, "samples")
		, m_06xx(*this, "06xx")
		{ }

	void galaga_sample_w(offs_t offset, u8 data);
	void galagost(machine_config &config);
	void galagost_map(address_map &map);

private:
	optional_device<samples_device> m_samples;
	optional_device<namco_06xx_device> m_06xx;
};
