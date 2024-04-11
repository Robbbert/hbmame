// license:BSD-3-Clause
// copyright-holders:David Haywood, ElSemi

class iq_pgm_012_025 : public iq_pgm
{
public:
	iq_pgm_012_025(const machine_config &mconfig, device_type type, const char *tag)
		: iq_pgm(mconfig, type, tag),
			m_igs025(*this,"igs025")
	{
	}

	void drgw2_common_init();

	void init_drgw2();
	void init_dw2v100x();
	void init_drgw2c();
	void init_drgw2c101();
	void init_drgw2j();
	void init_drgw2hk();

	void pgm_012_025_drgw2(machine_config &config);
private:
	required_device<iq_igs025> m_igs025;

	void pgm_drgw2_decrypt();
	void drgw2_mem(address_map &map);
};
