// license:BSD-3-Clause
// copyright-holders:Robbbert
/*********************************************************************

    ng_memcard.c

    NEOGEO Memory card functions.

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

*********************************************************************/

#include "emu.h"
#include "emuopts.h"
#include "nghb_memc.h"

// device type definition
const device_type XNG_MEMCARD = &device_creator<ng_memcard_class>;

//-------------------------------------------------
//  ng_memcard_class - constructor
//-------------------------------------------------

ng_memcard_class::ng_memcard_class(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock)
	: device_t(mconfig, XNG_MEMCARD, "NEOGEO Memory Card", tag, owner, clock, "xng_memcard", __FILE__),
	  device_image_interface(mconfig, *this)
{
}


//-------------------------------------------------
//  device_config_complete - perform any
//  operations now that the configuration is
//  complete
//-------------------------------------------------

void ng_memcard_class::device_config_complete()
{
	// set brief and instance name
	update_names();
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void ng_memcard_class::device_start()
{
	save_item(NAME(m_memcard_data));
}

/*-------------------------------------------------
    memcard_insert - insert an existing memory card
    with the given index
-------------------------------------------------*/

bool ng_memcard_class::call_load()
{
	if(length() != 0x800)
		return IMAGE_INIT_FAIL;

	fseek(0, SEEK_SET);
	size_t ret = fread(m_memcard_data, 0x800);
	if(ret != 0x800)
		return IMAGE_INIT_FAIL;
		
	return IMAGE_INIT_PASS;
}

void ng_memcard_class::call_unload()
{
	fseek(0, SEEK_SET);
	fwrite(m_memcard_data, 0x800);
}

bool ng_memcard_class::call_create(int format_type, option_resolution *format_options)
{	
	memset(m_memcard_data, 0, 0x800);
	
	size_t ret = fwrite(m_memcard_data, 0x800);
	if(ret != 0x800)
		return IMAGE_INIT_FAIL;
	
	return IMAGE_INIT_PASS;
}


READ8_MEMBER(ng_memcard_class::read)
{
	return m_memcard_data[offset];
}

WRITE8_MEMBER(ng_memcard_class::write)
{
	m_memcard_data[offset] = data;
}