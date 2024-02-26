// license:BSD-3-Clause
// copyright-holders:David Haywood, ElSemi
/* IGS 028 */
#ifndef MAME_IGS_IGS028_H
#define MAME_IGS_IGS028_H

#pragma once


class iq_igs028 : public device_t
{
public:
	iq_igs028(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	uint16_t* m_sharedprotram = nullptr;

	void IGS028_handle(void);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;

	uint32_t olds_prot_addr(uint16_t addr);
	uint32_t olds_read_reg(uint16_t addr);
	void olds_write_reg( uint16_t addr, uint32_t val );
	void IGS028_do_dma(uint16_t src, uint16_t dst, uint16_t size, uint16_t mode);
};


DECLARE_DEVICE_TYPE(IQ_IGS028, iq_igs028)

#endif // MAME_IGS_IGS028_H
