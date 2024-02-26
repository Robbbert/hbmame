// license:BSD-3-Clause
// copyright-holders:David Haywood, ElSemi

class iq_pgm_028_025 : public iq_pgm
{
public:
	iq_pgm_028_025(const machine_config &mconfig, device_type type, const char *tag)
		: iq_pgm(mconfig, type, tag),
			m_sharedprotram(*this, "sharedprotram"),
			m_igs025(*this,"igs025"),
			m_igs028(*this,"igs028")

	{
	}

	void init_olds();

	void pgm_028_025_ol(machine_config &config);

protected:
	virtual void machine_reset() override;

private:
	required_shared_ptr<u16> m_sharedprotram;
	required_device<iq_igs025> m_igs025;
	required_device<iq_igs028> m_igs028;

	void igs025_to_igs028_callback( void );

	void olds_mem(address_map &map);
};

INPUT_PORTS_EXTERN( iq_olds );
