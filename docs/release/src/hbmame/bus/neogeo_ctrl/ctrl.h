// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/**********************************************************************

   SNK Neo Geo controller port emulation

**********************************************************************/


#pragma once

#ifndef __NEOGEO_CONTROL_PORT__
#define __NEOGEO_CONTROL_PORT__

#include "emu.h"

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

class neogeo_control_port_device;
class neogeo_ctrl_edge_port_device;

// ======================> device_neogeo_control_port_interface

class device_neogeo_control_port_interface : public device_interface
{
public:
	// construction/destruction
	device_neogeo_control_port_interface(const machine_config &mconfig, device_t &device);
	virtual ~device_neogeo_control_port_interface();

	virtual uint8_t read_ctrl() { return 0xff; };
	virtual uint8_t read_start_sel() { return 0xff; };
	virtual void write_ctrlsel(uint8_t data) { };

protected:
	neogeo_control_port_device *m_port;
};

// ======================> neogeo_control_port_device

class neogeo_control_port_device : public device_t, public device_single_card_slot_interface<device_neogeo_control_port_interface>
{
public:
	// construction/destruction
	neogeo_control_port_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	virtual ~neogeo_control_port_device();

	uint8_t read_ctrl();
	uint8_t read_start_sel();
	void write_ctrlsel(uint8_t data);

protected:
	// device-level overrides
	virtual void device_start() override;

	device_neogeo_control_port_interface *m_device;
};


// ======================> device_neogeo_ctrl_edge_interface

class device_neogeo_ctrl_edge_interface : public device_interface
{
public:
	// construction/destruction
	device_neogeo_ctrl_edge_interface(const machine_config &mconfig, device_t &device);
	virtual ~device_neogeo_ctrl_edge_interface();

	virtual uint8_t read_start_sel() { return 0xff; }
	virtual u8 in0_r() { return 0xff; }
	virtual u8 in1_r() { return 0xff; }
	virtual void write_ctrlsel(uint8_t data) { }

protected:
	neogeo_ctrl_edge_port_device *m_port;
};

// ======================> neogeo_ctrl_edge_port_device

class neogeo_ctrl_edge_port_device : public device_t, public device_single_card_slot_interface<device_neogeo_ctrl_edge_interface>
{
public:
	// construction/destruction
	neogeo_ctrl_edge_port_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	virtual ~neogeo_ctrl_edge_port_device();

	uint8_t read_start_sel();
	u8 in0_r();
	u8 in1_r();
	void write_ctrlsel(uint8_t data);

protected:
	// device-level overrides
	virtual void device_start() override;

	device_neogeo_ctrl_edge_interface *m_device;
};


// device type definition
DECLARE_DEVICE_TYPE(NEOGEO_CONTROL_PORT, neogeo_control_port_device)
DECLARE_DEVICE_TYPE(NEOGEO_CTRL_EDGE_CONNECTOR, neogeo_ctrl_edge_port_device)


//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************


void neogeo_controls(device_slot_interface &device);
void neogeo_arc_edge(device_slot_interface &device);
void neogeo_arc_edge_fixed(device_slot_interface &device);
void neogeo_arc_pin15(device_slot_interface &device);


#endif
