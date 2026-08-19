// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/*********************************************************************

    ng_memcard.c

    NEOGEO Memory card functions.

*********************************************************************/

#include "emu.h"
#include "emuopts.h"
#include "hbng_memcard.h"

// device type definition
DEFINE_DEVICE_TYPE(HBNG_MEMCARD, hbng_memcard_device, "hbng_memcard", "HB NeoGeo Memory Card")

//-------------------------------------------------
//  ng_memcard_device - constructor
//-------------------------------------------------

hbng_memcard_device::hbng_memcard_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: device_t(mconfig, HBNG_MEMCARD, tag, owner, clock),
		device_memcard_image_interface(mconfig, *this)
{
}



//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void hbng_memcard_device::device_start()
{
	save_item(NAME(m_memcard_data));
}

/*-------------------------------------------------
    memcard_insert - insert an existing memory card
    with the given index
-------------------------------------------------*/

std::pair<std::error_condition, std::string> hbng_memcard_device::call_load()
{
	if(length() != 0x800)
		return std::make_pair(image_error::INVALIDLENGTH, "Unsupported memory card size (only 2K cards are supported)");

	fseek(0, SEEK_SET);
	size_t ret = fread(m_memcard_data, 0x800);
	if(ret != 0x800)
		return std::make_pair(image_error::UNSPECIFIED, "Error reading file");

	return std::make_pair(std::error_condition(), std::string());
}

void hbng_memcard_device::call_unload()
{
	fseek(0, SEEK_SET);
	fwrite(m_memcard_data, 0x800);
}

std::pair<std::error_condition, std::string> hbng_memcard_device::call_create(int format_type, util::option_resolution *format_options)
{
	memset(m_memcard_data, 0, 0x800);

	size_t ret = fwrite(m_memcard_data, 0x800);
	if(ret != 0x800)
		return std::make_pair(image_error::UNSPECIFIED, "Error writing file");

	return std::make_pair(std::error_condition(), std::string());
}


u8 hbng_memcard_device::read(offs_t offset)
{
	return m_memcard_data[offset];
}

void hbng_memcard_device::write(offs_t offset, u8 data)
{
	m_memcard_data[offset] = data;
}
