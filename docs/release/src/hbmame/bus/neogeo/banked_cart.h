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
	neogeo_banked_cart_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

	memory_bank*   m_bank_cartridge;
	uint32_t     m_main_cpu_bank_address;
	uint8_t* m_region;
	uint32_t m_region_size;


	void install_banks(running_machine& machine, cpu_device* maincpu, uint8_t* region, uint32_t region_size);
	WRITE16_MEMBER(main_cpu_bank_select_w);
	void neogeo_set_main_cpu_bank_address(uint32_t bank_address);
	void _set_main_cpu_bank_address();
	void init_banks(void);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
	void postload();
};

#endif
