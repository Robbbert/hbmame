// license:BSD-3-Clause
// copyright-holders:strygo
/***************************************************************************

    CPS+ capacity extension to CPS-2.

    8 MiB program: the original encrypted 4 MiB window plus a plaintext
    4 MiB window at a00000..dfffff, for both data reads and opcode fetches.
    64 MiB graphics: object-write alias A14 selects the extra 32 MiB;
    scroll attribute bit 9 selects that half after native bank mapping.
    256 or 512 KiB Z80 program/sequence ROM; up to 32 MiB QSound samples, with the original DSP and Z80 protocol.

    CPU/video clocks, palettes, priorities, object-list format, DSP program,
    sound voices and digital volume remain those of CPS-2. HLE and DSP are
    configurations of the same board, not separate capacity implementations.
    No external music-pack device is part of this implementation.

    Included by cps2.cpp, like HBMAME's other CPS-2 extension drivers.

***************************************************************************/

#include "includes/cps2plus.h"

#pragma push_macro("qsound_device")
#pragma push_macro("QSOUND")
#undef qsound_device
#undef QSOUND

cps2plus_state::cps2plus_state(machine_config const &mconfig, device_type type, char const *tag)
	: cps2_state(mconfig, type, tag)
	, m_qsound_dsp(*this, "qsound_dsp")
	, m_sample_rom(*this, "sample_rom")
	, m_objram1_ext(*this, "objram1_ext", OBJECT_ENTRIES, ENDIANNESS_LITTLE)
	, m_objram2_ext(*this, "objram2_ext", OBJECT_ENTRIES, ENDIANNESS_LITTLE)
	, m_objram_ext_latched(*this, "objram_ext_latched", OBJECT_ENTRIES, ENDIANNESS_LITTLE)
{
}

void cps2plus_state::configure(machine_config &config, u8 sample_bits, bool dsp)
{
	cps2(config);
	m_sample_mask = (u32(1) << sample_bits) - 1;
	m_use_dsp = dsp;
	m_audiocpu->set_addrmap(AS_PROGRAM, &cps2plus_state::sound_map);
	m_maincpu->set_addrmap(AS_PROGRAM, &cps2plus_state::program_map);
	m_maincpu->set_addrmap(AS_OPCODES, &cps2plus_state::opcodes_map);
	MCFG_MACHINE_START_OVERRIDE(cps2plus_state, cps2plus)
	MCFG_MACHINE_RESET_OVERRIDE(cps2plus_state, cps2plus)

	// This is the board's sample-ROM address decoder. device_rom_interface
	// supports an external space; no change to the shared QSound devices is
	// needed. Explicit mirroring models unconnected address lines on controls.
	ADDRESS_MAP_BANK(config, m_sample_rom).set_options(ENDIANNESS_LITTLE, 8, 25);
	m_sample_rom->set_map(&cps2plus_state::sample_map);
	if (dsp)
	{
		config.device_remove("qsound");
		QSOUND(config, m_qsound_dsp);
		m_qsound_dsp->set_space(m_sample_rom, AS_PROGRAM);
		m_qsound_dsp->add_route(0, "lspeaker", 1.0);
		m_qsound_dsp->add_route(1, "rspeaker", 1.0);
	}
	else
		m_qsound->set_space(m_sample_rom, AS_PROGRAM);
}

void cps2plus_state::cps2plus(machine_config &config) { configure(config, 25, false); }
void cps2plus_state::cps2plus_dsp(machine_config &config) { configure(config, 25, true); }
void cps2plus_state::cps2plus_16(machine_config &config) { configure(config, 24, false); }
void cps2plus_state::cps2plus_16_dsp(machine_config &config) { configure(config, 24, true); }
void cps2plus_state::cps2plus_8(machine_config &config) { configure(config, 23, false); }
void cps2plus_state::cps2plus_8_dsp(machine_config &config) { configure(config, 23, true); }

void cps2plus_state::program_map(address_map &map)
{
	cps2_map(map);
	map(0xa00000, 0xdfffff).rom().region("maincpu", ENCRYPTED_SIZE);
	// A13 remains a mirror. A14 selects the entry's extension bit on writes.
	map(0x700000, 0x701fff).mirror(0x002000).rw(FUNC(cps2plus_state::cps2_objram1_r), FUNC(cps2plus_state::objram1_w));
	map(0x704000, 0x705fff).mirror(0x002000).rw(FUNC(cps2plus_state::cps2_objram1_r), FUNC(cps2plus_state::objram1_alias_w));
	map(0x708000, 0x709fff).mirror(0x002000).rw(FUNC(cps2plus_state::cps2_objram2_r), FUNC(cps2plus_state::objram2_w));
	map(0x70c000, 0x70dfff).mirror(0x002000).rw(FUNC(cps2plus_state::cps2_objram2_r), FUNC(cps2plus_state::objram2_alias_w));
}

void cps2plus_state::opcodes_map(address_map &map)
{
	decrypted_opcodes_map(map);
	map(0xa00000, 0xdfffff).rom().region("maincpu", ENCRYPTED_SIZE);
}

void cps2plus_state::sample_map(address_map &map)
{
	map(0, m_sample_mask).mirror(0x1ffffff ^ m_sample_mask).rom().region(":qsound", 0);
}

void cps2plus_state::sound_map(address_map &map)
{
	qsound_sub_map_common(map);
	map(0xd003, 0xd003).w(FUNC(cps2plus_state::sound_banksw_w));
	map(0xd007, 0xd007).r(FUNC(cps2plus_state::sound_status_r));
	if (m_use_dsp)
		map(0xd000, 0xd002).w(m_qsound_dsp, FUNC(qsound_device::qsound_w));
	else
		map(0xd000, 0xd002).w(m_qsound, FUNC(qsound_hle_device::qsound_w));
}

void cps2plus_state::sound_banksw_w(u8 data)
{
	if (!m_extended_sound_rom)
	{
		qsound_banksw_w(data);
		return;
	}
	// Bank 0 begins at linear ROM offset 0x8000. Banks 30/31 are
	// unpopulated: decode them to bank 0, never wrap into fixed code.
	u8 const selected = data & 0x1f;
	m_sound_bank = selected < 30 ? selected : 0;
	membank("bank1")->set_entry(m_sound_bank);
}

u8 cps2plus_state::sound_status_r()
{
	u8 const status = m_use_dsp ? m_qsound_dsp->qsound_r() : m_qsound->qsound_r();
	return m_extended_sound_rom ? (status & 0x80) | 0x60 | m_sound_bank : status;
}

void cps2plus_state::object_w(int window, u8 extension, offs_t offset, u16 data, u16 mem_mask)
{
	int const bank = (m_objram_bank & 1) ^ window;
	u16 *const ram = bank ? m_objram2.target() : m_objram1.target();
	COMBINE_DATA(&ram[offset]);
	(bank ? m_objram2_ext : m_objram1_ext)[offset >> 2] = extension;
}

void cps2plus_state::objram1_w(offs_t offset, u16 data, u16 mem_mask) { object_w(0, 0, offset, data, mem_mask); }
void cps2plus_state::objram1_alias_w(offs_t offset, u16 data, u16 mem_mask) { object_w(0, 1, offset, data, mem_mask); }
void cps2plus_state::objram2_w(offs_t offset, u16 data, u16 mem_mask) { object_w(1, 0, offset, data, mem_mask); }
void cps2plus_state::objram2_alias_w(offs_t offset, u16 data, u16 mem_mask) { object_w(1, 1, offset, data, mem_mask); }

void cps2plus_state::cps2_objram_latch()
{
	cps2_state::cps2_objram_latch();
	std::copy_n((cps2_objbase() == m_objram2.target() ? m_objram2_ext : m_objram1_ext).target(),
		OBJECT_ENTRIES, m_objram_ext_latched.target());
}

int cps2plus_state::cps2_scroll_code(cps2_scroll_layer layer, int code, int attr) const
{
	if (code < 0 || !BIT(attr, 9))
		return code;
	switch (layer)
	{
	case cps2_scroll_layer::SCROLL1: return code + GRAPHICS_SIZE / 2 / SCROLL1_TILE_BYTES;
	case cps2_scroll_layer::SCROLL2: return code + GRAPHICS_SIZE / 2 / SCROLL2_TILE_BYTES;
	case cps2_scroll_layer::SCROLL3: return code + GRAPHICS_SIZE / 2 / SCROLL3_TILE_BYTES;
	}
	return code;
}

int cps2plus_state::cps2_sprite_code(int, int index, int column, int row) const
{
	u16 const *const object = &m_cps2_buffered_obj[index * 4];
	// The native scanner expands the 16-bit row code, then wraps columns
	// within its low nibble. Bank wires remain those of the latched entry.
	u16 const row_code = object[2] + (row << 4);
	int const tile_code = (row_code & 0xfff0) | ((row_code + column) & 0x000f);
	return tile_code | ((object[1] & 0x6000) << 3) | (int(m_objram_ext_latched[index]) << 18);
}

void cps2plus_state::cps2_sound_gain(double gain)
{
	if (m_use_dsp)
	{
		m_qsound_dsp->set_output_gain(0, gain);
		m_qsound_dsp->set_output_gain(1, gain);
	}
	else
		cps2_state::cps2_sound_gain(gain);
}

MACHINE_START_MEMBER(cps2plus_state, cps2plus)
{
	MACHINE_START_CALL_MEMBER(cps2);
	u32 const sound_bytes = memregion("audiocpu")->bytes();
	// MAME's standard 0x50000 region includes the 0x8000 loading gap.
	// The expanded board has exactly 0x80000 linear bytes plus that gap.
	if (sound_bytes != QSOUND_SIZE && sound_bytes != 0x88000)
		fatalerror("cps2plus: expected standard or 512 KiB sound ROM layout\n");
	m_extended_sound_rom = sound_bytes == 0x88000;
	if (m_extended_sound_rom)
		membank("bank1")->configure_entries(0, 30, memregion("audiocpu")->base() + 0x10000, 0x4000);
	save_item(NAME(m_sound_bank));
	if (m_gfxdecode->gfx(2)->elements() != GRAPHICS_SIZE / OBJECT_TILE_BYTES)
		fatalerror("cps2plus: graphics decode must expose 64 MiB\n");
}

MACHINE_RESET_MEMBER(cps2plus_state, cps2plus)
{
	MACHINE_RESET_CALL_MEMBER(cps);
	m_sound_bank = 0;
	membank("bank1")->set_entry(0);
	std::fill_n(m_objram1_ext.target(), OBJECT_ENTRIES, 0);
	std::fill_n(m_objram2_ext.target(), OBJECT_ENTRIES, 0);
	std::fill_n(m_objram_ext_latched.target(), OBJECT_ENTRIES, 0);
}

void cps2plus_state::init_cps2plus()
{
	if (memregion("maincpu")->bytes() != PROGRAM_SIZE || memregion("gfx")->bytes() != GRAPHICS_SIZE)
		fatalerror("cps2plus: expected 8 MiB program and 64 MiB graphics\n");
	u32 const samples = memregion("qsound")->bytes();
	if ((samples != 0x800000 && samples != 0x1000000 && samples != 0x2000000) || samples <= m_sample_mask)
		fatalerror("cps2plus: sample region does not fit the configured address decoder\n");
	init_cps2crypt(ENCRYPTED_SIZE);
	init_cps2nc();
}

#pragma pop_macro("QSOUND")
#pragma pop_macro("qsound_device")
