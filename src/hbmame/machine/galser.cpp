// license:GPL_2.0
// copyright-holders:Robbbert
/***************************************************************************

Unknown serial nvram device used in Scramble Multikit

***************************************************************************/

#include "emu.h"
#include "machine/galser.h"

//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

// device type definition
DEFINE_DEVICE_TYPE(GALSER, galser_device, "galser", "smulti nvram")

//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  galser_device - constructor
//-------------------------------------------------

galser_device::galser_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock )
	: device_t(mconfig, GALSER, tag, owner, clock)
	, device_nvram_interface(mconfig, *this)
	, m_region(*this, DEVICE_SELF)
	, m_data_size(0x100)
	, m_scl(0)
	, m_sdaw(0)
	, m_en(0)
	, m_sdar(1)
	, m_byteaddr(0)
	, m_data_in(0)
	, m_data_out(0)
	, m_count(0)
	, m_write(0)
	{}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void galser_device::device_start()
{
	m_data = std::make_unique<uint8_t []>(m_data_size);

	save_item( NAME(m_scl) );
	save_item( NAME(m_sdaw) );
	save_item( NAME(m_sdar) );
	save_item( NAME(m_byteaddr) );
	save_pointer( &m_data[0], "m_data", m_data_size );
	save_item( NAME(m_data_in) );
	save_item( NAME(m_data_out) );
	save_item( NAME(m_count) );
	save_item( NAME(m_write) );
}


//-------------------------------------------------
//  nvram_default - called to initialize NVRAM to
//  its default state
//-------------------------------------------------

void galser_device::nvram_default()
{
	memset(&m_data[0], 0xff, m_data_size);

	/* populate from a memory region if present */
	if (m_region.found())
	{
		if( m_region->bytes() != m_data_size )
		{
			fatalerror( "galser region '%s' wrong size (expected size = 0x%X)\n", tag(), m_data_size );
		}

		if( m_region->bytewidth() != 1 )
		{
			fatalerror( "galser region '%s' needs to be an 8-bit region\n", tag() );
		}

		memcpy(&m_data[0], m_region->base(), m_data_size);
	}
}


//-------------------------------------------------
//  nvram_read - called to read NVRAM from the
//  .nv file
//-------------------------------------------------

bool galser_device::nvram_read( util::read_stream &file )
{
	size_t actual;
	return !file.read( &m_data[0], m_data_size, actual ) && actual == m_data_size;
}

//-------------------------------------------------
//  nvram_write - called to write NVRAM to the
//  .nv file
//-------------------------------------------------

bool galser_device::nvram_write( util::write_stream &file )
{
	size_t actual;
	return !file.write( &m_data[0], m_data_size, actual ) && actual == m_data_size;
}



//**************************************************************************
//  READ/WRITE HANDLERS
//**************************************************************************

WRITE_LINE_MEMBER( galser_device::write_en )
{
	m_en = state ? 1 : 0;
	if (m_en == 0)
	{
		m_sdar = 0;
		//printf("ack set\n");
		if (m_count == 22)
		{
			uint8_t header = m_data_in >> 16;
			m_byteaddr = (m_data_in >> 8) & 0xff;
			uint8_t data = m_data_in;
			if (header == 0x28)
			{
				m_data[m_byteaddr] = data;
				//printf("wrote %X:%X\n",m_byteaddr,data);
			}
		}
	}
	else
	{
		m_count = 0;
		m_data_in = 0;
	}
}

WRITE_LINE_MEMBER( galser_device::write_sda )
{
	state &= 1;
	// no start or stop bits
	m_sdaw = state;
}

WRITE_LINE_MEMBER( galser_device::write_scl )
{
	m_scl = state & 1;
	if (m_scl)
	{
		m_count++;
		m_data_in = (m_data_in << 1) | m_sdaw;
		// reading a byte from nvram
		if (m_count == 14)
		{
			//printf("%X\n",m_data_in);
			if((m_data_in & 0xff00) == 0x3000)
			{
				m_write = false;
				m_byteaddr = m_data_in & 0xff;
				m_data_out = m_data[m_byteaddr];
				//printf("read = %X:%X\n",m_byteaddr,m_data_out);
			}
			else
			if((m_data_in & 0xff00) == 0x2800)
				m_write = true;
		}
	}
}


READ_LINE_MEMBER( galser_device::read_sda )
{
	int res = m_sdar;
	m_sdar = BIT(m_data_out, 7);
	m_data_out <<= 1;
	if (m_write == 0)
		res = m_sdar;
	//printf("sdar = %X\n",res);

	return res;
}

