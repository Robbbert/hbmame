// license:BSD-3-Clause
// copyright-holders:David Haywood, ElSemi

class iq_pgm_022_025 : public iq_pgm
{
public:
	iq_pgm_022_025(const machine_config &mconfig, device_type type, const char *tag)
		: iq_pgm(mconfig, type, tag),
			m_igs025(*this,"igs025"),
			m_igs022(*this,"igs022")

	{ }

	void pgm_dw3_decrypt();
	void pgm_killbld_decrypt();

	void init_killbld();
	void init_drgw3();
	DECLARE_MACHINE_RESET(killbld);
	DECLARE_MACHINE_RESET(dw3);

	void igs025_to_igs022_callback( void );

	required_device<iq_igs025> m_igs025;
	required_device<iq_igs022> m_igs022;
	void pgm_022_025(machine_config &config);
	void pgm_022_025_dw3(machine_config &config);
	void pgm_022_025_killbld(machine_config &config);
	void killbld_mem(address_map &map);
};

INPUT_PORTS_EXTERN( iq_killbld );
INPUT_PORTS_EXTERN( iq_dw3 );
INPUT_PORTS_EXTERN( iq_dw3j );
