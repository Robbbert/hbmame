// license:GPL_2.0
// copyright-holders:Robbbert
/***************************************************************************

NVRAM device used in the Scramble MultiKit

***************************************************************************/

#ifndef MAME_MACHINE_GALSER_H
#define MAME_MACHINE_GALSER_H

#pragma once

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> galser_device

class galser_device : public device_t, public device_nvram_interface
{
public:
	// I/O operations
	DECLARE_WRITE_LINE_MEMBER( write_sda );
	DECLARE_WRITE_LINE_MEMBER( write_scl );
	DECLARE_WRITE_LINE_MEMBER( write_en );
	DECLARE_READ_LINE_MEMBER( read_sda );
	galser_device( const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0 );

protected:
	// construction/destruction

	// device-level overrides
	virtual void device_start() override;

	// device_nvram_interface overrides
	virtual void nvram_default() override;
	virtual bool nvram_read( util::read_stream &file ) override;
	virtual bool nvram_write( util::write_stream &file ) override;

	// internal helpers
	optional_memory_region m_region;

	// internal state
	std::unique_ptr<uint8_t[]> m_data;
	int m_data_size;
	int m_scl;
	int m_sdaw;
	int m_en;
	int m_sdar;
	int m_byteaddr;
	uint32_t m_data_in;
	uint8_t m_data_out;
	uint8_t m_count;
	bool m_write;
};

#define DECLARE_GALSER_DEVICE(name) \
	class galser_device : public galser_device \
	{ \
	public: \
		galser_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0); \
	};

DECLARE_DEVICE_TYPE(GALSER, galser_device)

#endif // MAME_MACHINE_GALSER_H
