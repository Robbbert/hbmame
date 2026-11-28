// license:BSD-3-Clause
// copyright-holders:strygo
#ifndef MAME_INCLUDES_CPS2PLUS_H
#define MAME_INCLUDES_CPS2PLUS_H

#pragma once

#include "cps2.h"
#include "machine/bankdev.h"

// qsound.h aliases the DSP type to HLE by default. Use the actual DSP type
// locally without changing the stock driver's finder or its configuration.
#pragma push_macro("qsound_device")
#undef qsound_device

class cps2plus_state : public cps2_state
{
public:
	cps2plus_state(machine_config const &mconfig, device_type type, char const *tag);

	void cps2plus(machine_config &config);
	void cps2plus_dsp(machine_config &config);
	void cps2plus_16(machine_config &config);
	void cps2plus_16_dsp(machine_config &config);
	void cps2plus_8(machine_config &config);
	void cps2plus_8_dsp(machine_config &config);
	void init_cps2plus();

private:
	static constexpr u32 PROGRAM_SIZE = 0x800000;
	static constexpr u32 ENCRYPTED_SIZE = 0x400000;
	static constexpr u32 GRAPHICS_SIZE = 0x4000000;
	static constexpr u32 OBJECT_TILE_BYTES = 128;
	static constexpr u32 SCROLL1_TILE_BYTES = 64; // two column-side views per 8x8 record
	static constexpr u32 SCROLL2_TILE_BYTES = OBJECT_TILE_BYTES;
	static constexpr u32 SCROLL3_TILE_BYTES = 4 * OBJECT_TILE_BYTES;
	static constexpr u32 OBJECT_ENTRIES = 0x2000 / 8;

	optional_device<qsound_device> m_qsound_dsp;
	required_device<address_map_bank_device> m_sample_rom;
	memory_share_creator<u8> m_objram1_ext;
	memory_share_creator<u8> m_objram2_ext;
	memory_share_creator<u8> m_objram_ext_latched;
	u32 m_sample_mask = 0x1ffffff; // immutable board configuration
	u8 m_sound_bank = 0;
	bool m_extended_sound_rom = false; // immutable ROM-board configuration
	bool m_use_dsp = false; // address maps are built before device finders resolve

	void configure(machine_config &config, u8 sample_bits, bool dsp);
	void program_map(address_map &map);
	void opcodes_map(address_map &map);
	void sample_map(address_map &map);
	void sound_map(address_map &map);
	void sound_banksw_w(u8 data);
	u8 sound_status_r();
	void object_w(int window, u8 extension, offs_t offset, u16 data, u16 mem_mask);
	void objram1_w(offs_t offset, u16 data, u16 mem_mask = 0xffff);
	void objram1_alias_w(offs_t offset, u16 data, u16 mem_mask = 0xffff);
	void objram2_w(offs_t offset, u16 data, u16 mem_mask = 0xffff);
	void objram2_alias_w(offs_t offset, u16 data, u16 mem_mask = 0xffff);
	int cps2_scroll_code(cps2_scroll_layer layer, int code, int attr) const override;
	int cps2_sprite_code(int code, int index, int column, int row) const override;
	void cps2_objram_latch() override;
	void cps2_sound_gain(double gain) override;
	DECLARE_MACHINE_START(cps2plus);
	DECLARE_MACHINE_RESET(cps2plus);
};

#pragma pop_macro("qsound_device")

#endif // MAME_INCLUDES_CPS2PLUS_H
