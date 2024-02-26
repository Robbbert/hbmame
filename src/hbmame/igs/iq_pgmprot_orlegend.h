// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, iq_132
/* for machine/pgmprot_orlegend.c type games */

class iq_pgm_asic3 : public iq_pgm
{
public:
	iq_pgm_asic3(const machine_config &mconfig, device_type type, const char *tag)
		: iq_pgm(mconfig, type, tag) {
	}

	void init_orlegend();
	void pgm_asic3(machine_config &config);

private:
	// ASIC 3 (oriental legends protection)
	u8         m_asic3_reg = 0;
	u8         m_asic3_latch[3]{};
	u8         m_asic3_x = 0;
	u16        m_asic3_hilo = 0;
	u16        m_asic3_hold = 0;

	void asic3_compute_hold(int,int);
	u16 pgm_asic3_r();
	void pgm_asic3_w(offs_t offset, u16 data);
};

INPUT_PORTS_EXTERN( iq_orlegend );
INPUT_PORTS_EXTERN( iq_orlegendt );
INPUT_PORTS_EXTERN( iq_orlegendk );
