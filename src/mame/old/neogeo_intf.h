// license:BSD-3-Clause
// copyright-holders:S. Smith,David Haywood

#ifndef __NEOGEO_INTF_H
#define __NEOGEO_INTF_H

// ======================> device_neogeo_cart_interface

#define DECRYPT_ALL_PARAMS \
	uint8_t* cpuregion, uint32_t cpuregion_size,uint8_t* spr_region, uint32_t spr_region_size,uint8_t* fix_region, uint32_t fix_region_size,uint8_t* ym_region, uint32_t ym_region_size,uint8_t* ymdelta_region, uint32_t ymdelta_region_size,uint8_t* audiocpu_region, uint32_t audio_region_size, uint8_t* audiocrypt_region, uint32_t audiocrypt_region_size

#define ACTIVATE_CART_PARAMS \
	running_machine& machine, cpu_device* maincpu, uint8_t* cpuregion, uint32_t cpuregion_size, uint8_t* fixedregion, uint32_t fixedregion_size

class device_neogeo_cart_interface : public device_slot_card_interface
{
public:
	// construction/destruction
	device_neogeo_cart_interface(const machine_config &mconfig, device_t &device);
	virtual ~device_neogeo_cart_interface();

	// reading from ROM
	virtual DECLARE_READ16_MEMBER(read_rom) { return 0xffff; }
	virtual void activate_cart(ACTIVATE_CART_PARAMS) { };
	virtual void decrypt_all(DECRYPT_ALL_PARAMS) { };
	virtual int get_fixed_bank_type(void) { return 0; }

	void rom_alloc(uint32_t size) { m_rom.resize(size/sizeof(uint16_t)); }
	uint16_t* get_rom_base() { return &m_rom[0]; }
	uint32_t  get_rom_size() { return m_rom.size()*sizeof(uint16_t); }

	void fixed_alloc(uint32_t size) { m_fixed.resize(size); }
	uint8_t* get_fixed_base() { return &m_fixed[0]; }
	uint32_t  get_fixed_size() { return m_fixed.size(); }

	void audio_alloc(uint32_t size) { m_audio.resize(size); }
	uint8_t* get_audio_base() { return &m_audio[0]; }
	uint32_t  get_audio_size() { return m_audio.size(); }

	void audiocrypt_alloc(uint32_t size) { m_audiocrypt.resize(size); }
	uint8_t* get_audiocrypt_base() { if (m_audiocrypt.size() == 0) return nullptr; else  return &m_audiocrypt[0]; }
	uint32_t  get_audiocrypt_size() { return m_audiocrypt.size(); }

	void sprites_alloc(uint32_t size) { m_sprites.resize(size); }
	uint8_t* get_sprites_base() { return &m_sprites[0]; }
	uint32_t  get_sprites_size() { return m_sprites.size(); }
	uint8_t* get_sprites_optimized() { return &m_sprites_optimized[0]; }
	uint32_t get_sprites_addrmask() { return m_sprite_gfx_address_mask; }
	std::vector<uint8_t>& get_sprites_optimized_arr() { return m_sprites_optimized; }

	void ym_alloc(uint32_t size) { m_ym.resize(size); }
	uint8_t* get_ym_base() { return &m_ym[0]; }
	uint32_t  get_ym_size() { return m_ym.size(); }

	void ymdelta_alloc(uint32_t size) { m_ymdelta.resize(size); }
	uint8_t* get_ymdelta_base() { return &m_ymdelta[0]; }
	uint32_t  get_ymdelta_size() { return m_ymdelta.size(); }

	std::vector<uint16_t> m_rom;
	std::vector<uint8_t> m_fixed;
	std::vector<uint8_t> m_sprites;
	std::vector<uint8_t> m_sprites_optimized;
	std::vector<uint8_t> m_audio;
	std::vector<uint8_t> m_ym;
	std::vector<uint8_t> m_ymdelta;

	uint32_t m_sprite_gfx_address_mask;



protected:
	// internal state
	std::vector<uint8_t> m_audiocrypt;


};

#endif
