// license:BSD-3-Clause
// copyright-holders:Bryan McPhail,Ernesto Corvi,Andrew Prime,Zsolt Vasvari
// thanks-to:Fuzz

// todo, move these back, currently the sprite code needs some of the values tho
#define NEOGEO_MASTER_CLOCK                     (24000000)
#define NEOGEO_MAIN_CPU_CLOCK                   (NEOGEO_MASTER_CLOCK / 2)
#define NEOGEO_AUDIO_CPU_CLOCK                  (NEOGEO_MASTER_CLOCK / 6)
#define NEOGEO_YM2610_CLOCK                     (NEOGEO_MASTER_CLOCK / 3)
#define NEOGEO_PIXEL_CLOCK                      (NEOGEO_MASTER_CLOCK / 4)
#define NEOGEO_HTOTAL                           (0x180)
#define NEOGEO_HBEND                            (0x01e) /* this should really be 29.5 */
#define NEOGEO_HBSTART                          (0x15e) /* this should really be 349.5 */
#define NEOGEO_VTOTAL                           (0x108)
#define NEOGEO_VBEND                            (0x010)
#define NEOGEO_VBSTART                          (0x0f0)
#define NEOGEO_VSSTART                          (0x100)


class neosprite_device : public device_t
{
public:
	neosprite_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	uint8_t      m_fixed_layer_bank_type = 0U;
	uint8_t      m_fixed_layer_source = 0U;
	uint16_t     get_videoram_data(  );
	uint16_t     get_videoram_modulo(  );
	uint8_t      neogeo_get_auto_animation_counter(  );
	void         set_videoram_data( uint16_t data);
	void         set_videoram_offset( uint16_t data );
	void         set_videoram_modulo( uint16_t data);
	void         set_auto_animation_speed( uint8_t data);
	void         set_auto_animation_disabled( uint8_t data);
	void         set_screen(screen_device* screen);
	void         set_pens(const pen_t* pens);
	virtual void set_sprite_region(uint8_t* region_sprites, uint32_t region_sprites_size);
	void         set_fixed_regions(uint8_t* fix_cart, uint32_t fix_cart_size, memory_region* fix_bios);
	void         draw_fixed_layer( bitmap_rgb32 &bitmap, int scanline );
	void         draw_sprites( bitmap_rgb32 &bitmap, int scanline );
	void         neogeo_set_fixed_layer_source( uint8_t data );
	virtual void optimize_sprite_data();

private:
	virtual void draw_fixed_layer_2pixels(uint32_t*&pixel_addr, int offset, uint8_t* gfx_base, const pen_t* char_pens);
	void         create_auto_animation_timer(  );
	void         start_auto_animation_timer(  );
	inline bool  sprite_on_scanline(int scanline, int y, int rows);
	void         parse_sprites( int scanline );
	void         create_sprite_line_timer(  );
	void         start_sprite_line_timer(  );
	virtual void draw_pixel(int romaddr, uint32_t* dst, const pen_t *line_pens);

	std::unique_ptr<uint16_t[]>     m_videoram{};
	std::vector<uint8_t> m_sprite_gfx{};

	uint16_t     *m_videoram_drawsource = nullptr;
	uint16_t     m_vram_offset = 0U;
	uint16_t     m_vram_read_buffer = 0U;
	uint16_t     m_vram_modulo = 0U;
	const uint8_t *m_region_zoomy;
	uint32_t     m_sprite_gfx_address_mask = 0U;
	uint8_t      m_auto_animation_speed = 0U;
	uint8_t      m_auto_animation_disabled = 0U;
	uint8_t      m_auto_animation_counter = 0U;
	uint8_t      m_auto_animation_frame_counter = 0U;

	emu_timer  *m_auto_animation_timer = nullptr;
	emu_timer  *m_sprite_line_timer = nullptr;

	TIMER_CALLBACK_MEMBER(auto_animation_timer_callback);
	TIMER_CALLBACK_MEMBER(sprite_line_timer_callback);

	uint8_t m_bppshift; // 4 for 4bpp gfx (NeoGeo) 8 for 8bpp gfx (Midas)

	virtual void device_start() override;
	virtual void device_reset() override;
	uint8_t* m_region_sprites = nullptr;
	uint32_t m_region_sprites_size = 0U;
	uint8_t* m_region_fixed = nullptr;
	uint32_t m_region_fixed_size = 0;
	memory_region* m_region_fixedbios;
	screen_device* m_screen;
	const pen_t   *m_pens;
	uint8_t* m_spritegfx8 = nullptr;
};

DECLARE_DEVICE_TYPE(NEOGEO_SPRITE, neosprite_device)
