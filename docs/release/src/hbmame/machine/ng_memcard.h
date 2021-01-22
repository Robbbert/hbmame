// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/*********************************************************************

    ng_memcard.h

    NEOGEO Memory card functions.

*********************************************************************/

#pragma once

#ifndef __NG_MEMCARD_H__
#define __NG_MEMCARD_H__

//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_NEOGEO_MEMCARD_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, NG_MEMCARD, 0)

/***************************************************************************
    FUNCTION PROTOTYPES
***************************************************************************/

// ======================> ng_memcard_device

class ng_memcard_device :  public device_t, public device_image_interface
{
public:
	// construction/destruction
	ng_memcard_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	virtual iodevice_t image_type() const noexcept override { return IO_MEMCARD; }

	virtual bool is_readable()  const noexcept override { return true; }
	virtual bool is_writeable() const noexcept override { return true; }
	virtual bool is_creatable() const noexcept override { return true; }
	virtual bool must_be_loaded() const noexcept override { return false; }
	virtual bool is_reset_on_load() const noexcept override { return false; }
	virtual const char *file_extensions() const noexcept override { return "neo"; }

	virtual image_init_result call_load() override;
	virtual void call_unload() override;
	virtual image_init_result call_create(int format_type, util::option_resolution *format_options) override;

	// device-level overrides
	virtual void device_start() override;

	u8 read(offs_t offset);
	void write(offs_t offset, u8 data);

	/* returns the index of the current memory card, or -1 if none */
	int present() { return is_loaded() ? 0 : -1; }
private:
	u8 m_memcard_data[0x800];
};


// device type definition
DECLARE_DEVICE_TYPE(NG_MEMCARD, ng_memcard_device)


#endif  /* __NG_MEMCARD_H__ */
