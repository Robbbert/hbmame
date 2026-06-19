// license:BSD-3-Clause
// copyright-holders:David Haywood, ElSemi
#ifndef MAME_IGS_IGS022_H
#define MAME_IGS_IGS022_H

#pragma once

class iq_igs022 : public device_t
{
public:
	iq_igs022(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	void handle_command();

protected:
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	static constexpr u16 NUM_REGS = 0x300, STACK_SIZE = 0x100;
	u32 m_regs[NUM_REGS];
	u32 m_stack[STACK_SIZE];
	u8 m_stack_ptr;

	optional_shared_ptr<u16> m_sharedprotram;
	required_memory_region m_rom;

	u32 read_reg(u16 offset);
	void write_reg(u16 offset, u32 data);

	void push_stack(u32 data);
	u32 pop_stack();
	std::string stack_as_string() const;

	void do_dma(u16 src, u16 dst, u16 size, u16 mode);

	void handle_command_6d();
	void handle_incomplete_command(u16 cmd, u16 res);
};

DECLARE_DEVICE_TYPE(IQ_IGS022, iq_igs022)

#endif // MAME_IGS_IGS022_H
