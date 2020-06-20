// license:BSD-3-Clause
// copyright-holders:S. Smith,David Haywood


#pragma once

#ifndef __NEOGEO_BANKED_CART__
#define __NEOGEO_BANKED_CART__

DECLARE_DEVICE_TYPE(NEOGEO_BANKED_CART, neogeo_banked_cart_device)


class neogeo_banked_cart_device :  public device_t
{
public:
	// construction/destruction
	neogeo_banked_cart_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	memory_bank*   m_bank_cartridge;
	u32     m_main_cpu_bank_address;
	u8* m_region;
	u32 m_region_size;


	void install_banks(running_machine& machine, cpu_device* maincpu, u8* region, u32 region_size);
	void main_cpu_bank_select_w(u16 data);
	void neogeo_set_main_cpu_bank_address(u32 bank_address);
	void _set_main_cpu_bank_address();
	void init_banks(void);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
	void postload();
};

#endif
