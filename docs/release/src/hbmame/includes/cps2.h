// license:BSD-3-Clause
// copyright-holders:Robbbert
/***************************************************************************

   Capcom CPS2 hardware

***************************************************************************/

#ifndef _CPS2_H_
#define _CPS2_H_

#include "emu.h"
#include "sound/msm5205.h"
#include "sound/qsound.h"
#include "sound/okim6295.h"
#include "machine/gen_latch.h"
#include "machine/timekpr.h"
#include "machine/timer.h"
#include "cpu/m68000/m68000.h"
#include "tilemap.h"
#include "emupal.h"
#include "screen.h"

// Video raw params
// measured clocks:
// CPS2(Guru): V = 59.6376Hz, H = 15,4445kHz *H is probably measured too low!
// CPS1 GNG: V = 59.61Hz
/* CPS1(Charles MacDonald):
    Pixel clock: 8.00 MHz
    Total pixel clocks per scanline: 512 clocks
    Horizontal sync pulse width : 36 clocks
    Horizontal display and blanking period: 476 clocks
    Frame size: 262 scanlines
    Refresh rate: 59.63 MHz.
*/
#define CPS_PIXEL_CLOCK  (XTAL(16'000'000)/2)

#define CPS_HTOTAL       (512)
#define CPS_HBEND        (64)
#define CPS_HBSTART      (448)

#define CPS_VTOTAL       (262)
#define CPS_VBEND        (16)
#define CPS_VBSTART      (240)


struct gfx_range
{
	// start and end are as passed by the game (shift adjusted to be all
	// in the same scale a 8x8 tiles): they don't necessarily match the
	// position in ROM.
	int type;
	int start;
	int end;
	int bank;
};

struct CPS1config
{
	const char *name;             /* game driver name */

	/* Some games interrogate a couple of registers on bootup. */
	/* These are CPS1 board B self test checks. They wander from game to */
	/* game. */
	int cpsb_addr;        /* CPS board B test register address */
	int cpsb_value;       /* CPS board B test register expected value */

	/* some games use as a protection check the ability to do 16-bit multiplies */
	/* with a 32-bit result, by writing the factors to two ports and reading the */
	/* result from two other ports. */
	/* It looks like this feature was introduced with 3wonders (CPSB ID = 08xx) */
	int mult_factor1;
	int mult_factor2;
	int mult_result_lo;
	int mult_result_hi;

	/* unknown registers which might be related to the multiply protection */
	int unknown1;
	int unknown2;
	int unknown3;

	int layer_control;
	int priority[4];
	int palette_control;

	/* ideally, the layer enable masks should consist of only one bit, */
	/* but in many cases it is unknown which bit is which. */
	int layer_enable_mask[5];

	/* these depend on the B-board model and PAL */
	int bank_sizes[4];
	const struct gfx_range *bank_mapper;

	/* some C-boards have additional I/O for extra buttons/extra players */
	int in2_addr;
	int in3_addr;
	int out2_addr;
};


class cps2_state : public driver_device
{
public:
	cps2_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_mainram(*this, "mainram"),
		m_gfxram(*this, "gfxram"),
		m_cps_a_regs(*this, "cps_a_regs"),
		m_cps_b_regs(*this, "cps_b_regs"),
		m_qsound_sharedram1(*this, "qsound_ram1"),
		m_qsound_sharedram2(*this, "qsound_ram2"),
		m_objram1(*this, "objram1"),
		m_objram2(*this, "objram2"),
		m_output(*this, "output"),
		m_io_in0(*this, "IN0"),
		m_io_in1(*this, "IN1"),
		m_cps2_dial_type(0),
		m_ecofghtr_dial_direction0(0),
		m_ecofghtr_dial_direction1(0),
		m_ecofghtr_dial_last0(0),
		m_ecofghtr_dial_last1(0),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_oki(*this, "oki"),
		m_m48t35(*this,"m48t35"),
		m_gfxdecode(*this, "gfxdecode"),
		m_screen(*this, "screen"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch"),
		m_soundlatch2(*this, "soundlatch2"),
		m_decrypted_opcodes(*this, "decrypted_opcodes"),
		m_region_key(*this, "key"),
		m_region_stars(*this, "stars")
		, m_qsound(*this, "qsound")
	{ }

	//HBMAME start
	// cps config for new
	int m_cpsb_addr;
	int m_cpsb_value;
	int m_mult_factor1;
	int m_mult_factor2;
	int m_mult_result_lo;
	int m_mult_result_hi;
	int m_layer_control;
	int m_priority[4];
	int m_palette_control;
	int m_layer_enable_mask[5];
	int m_bank_sizes[4];
	int m_in2_addr;
	int m_in3_addr;
	int m_out2_addr;
	u8 m_scrollx1;
	u8 m_scrollx2;
	u8 m_scrollx3;
	u32 m_bank_type[32];
	//HBMAME end

	/* memory pointers */
	// cps1
	bitmap_ind16 m_dummy_bitmap;
	optional_shared_ptr<u16> m_mainram;
	required_shared_ptr<u16> m_gfxram;
	required_shared_ptr<u16> m_cps_a_regs;
	required_shared_ptr<u16> m_cps_b_regs;
	u16 *     m_scroll1;
	u16 *     m_scroll2;
	u16 *     m_scroll3;
	u16 *     m_obj;
	u16 *     m_other;
	std::unique_ptr<u16[]>     m_buffered_obj;
	optional_shared_ptr<u8> m_qsound_sharedram1;
	optional_shared_ptr<u8> m_qsound_sharedram2;
	std::unique_ptr<u8[]> m_decrypt_kabuki;
	// cps2
	optional_shared_ptr<u16> m_objram1;
	optional_shared_ptr<u16> m_objram2;
	optional_shared_ptr<u16> m_output;

	optional_ioport m_io_in0;
	optional_ioport m_io_in1;
	std::unique_ptr<u16[]>     m_cps2_buffered_obj;
	// game-specific
	std::unique_ptr<u16[]>    m_gigaman2_dummyqsound_ram;
	u16  sf2ceblp_prot;

	/* video-related */
	tilemap_t      *m_bg_tilemap[3];
	int          m_scanline1;
	int          m_scanline2;
	int          m_scancalls;

	int          m_scroll1x;
	int          m_scroll1y;
	int          m_scroll2x;
	int          m_scroll2y;
	int          m_scroll3x;
	int          m_scroll3y;

	int          m_stars_enabled[2];        /* Layer enabled [Y/N] */
	int          m_stars1x;
	int          m_stars1y;
	int          m_stars2x;
	int          m_stars2y;
	int          m_last_sprite_offset;      /* Offset of the last sprite */
	int          m_cps2_last_sprite_offset; /* Offset of the last sprite */
	int          m_pri_ctrl;                /* Sprite layer priorities */
	int          m_objram_bank;

	/* misc */
	int          m_readpaddle;  // pzloop2
	int          m_cps2networkpresent;
	int          m_cps2digitalvolumelevel;
	int          m_cps2disabledigitalvolume;
	emu_timer    *m_digital_volume_timer;
	int          m_cps2_dial_type;
	int          m_ecofghtr_dial_direction0;
	int          m_ecofghtr_dial_direction1;
	int          m_ecofghtr_dial_last0;
	int          m_ecofghtr_dial_last1;

	/* video config (never changed after video_start) */
	const struct CPS1config *m_game_config;
	int          m_scroll_size;
	int          m_obj_size;
	int          m_cps2_obj_size;
	int          m_other_size;
	int          m_palette_align;
	int          m_palette_size;
	int          m_stars_rom_size;
	u8     m_empty_tile[32*32];
	int          m_cps_version;

	/* devices */
	required_device<m68000_base_device> m_maincpu;
	optional_device<cpu_device> m_audiocpu;
	optional_device<okim6295_device> m_oki;
	optional_device<m48t35_device> m_m48t35;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;
	optional_device<generic_latch_8_device> m_soundlatch;
	optional_device<generic_latch_8_device> m_soundlatch2;
	optional_shared_ptr<u16> m_decrypted_opcodes;
	optional_memory_region m_region_key;
	optional_memory_region m_region_stars;
	optional_device<qsound_device> m_qsound;

	u16 cps1_in2_r();
	u16 cps1_in3_r();
	u16 qsound_sharedram2_r(offs_t offset);
	void qsound_sharedram2_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void qsound_banksw_w(u8 data);
	u16 cps1_dsw_r(offs_t offset);
	u16 qsound_sharedram1_r(offs_t offset);
	void qsound_sharedram1_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void cps1_cps_a_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 cps1_cps_b_r(offs_t offset);
	void cps1_cps_b_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void cps1_gfxram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void cps2_objram_bank_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 cps2_objram1_r(offs_t offset);
	u16 cps2_objram2_r(offs_t offset);
	void cps2_objram1_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void cps2_objram2_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void init_cps2_video();
	void init_cps1();
	void init_cps2();
	void init_cps2nc();
	void init_cps2crypt();
	void init_ssf2tb();
	void init_pzloop2();
	void init_singbrd();
	void init_gigaman2();
	void init_ecofghtr();
	void init_sf2dongb();
	void init_sf2ceblp();
	TILEMAP_MAPPER_MEMBER(tilemap0_scan);
	TILEMAP_MAPPER_MEMBER(tilemap1_scan);
	TILEMAP_MAPPER_MEMBER(tilemap2_scan);
	TILE_GET_INFO_MEMBER(get_tile0_info);
	TILE_GET_INFO_MEMBER(get_tile1_info);
	TILE_GET_INFO_MEMBER(get_tile2_info);
	DECLARE_MACHINE_START(common);
	DECLARE_MACHINE_START(cps2);
	DECLARE_VIDEO_START(cps);
	DECLARE_VIDEO_START(cps1);
	DECLARE_VIDEO_START(cps2);
	DECLARE_MACHINE_START(qsound);
	DECLARE_MACHINE_START(ganbare);
	DECLARE_MACHINE_RESET(cps);
	u32 screen_update_cps1(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	DECLARE_WRITE_LINE_MEMBER(screen_vblank_cps1);
	INTERRUPT_GEN_MEMBER(cps1_interrupt);
	TIMER_DEVICE_CALLBACK_MEMBER(ganbare_interrupt);
	IRQ_CALLBACK_MEMBER(cps1_int_ack);
	TIMER_DEVICE_CALLBACK_MEMBER(cps2_interrupt);
	TIMER_CALLBACK_MEMBER(cps2_update_digital_volume);

	void kabuki_setup(void (*decode)(u8 *src, u8 *dst));

	/* cps video */
	inline u16 *cps1_base( int offset, int boundary );
	void cps1_get_video_base();
	void unshuffle(u64 *buf, int len);
	void cps2_gfx_decode();
	int gfxrom_bank_mapper(int type, int code);
	void cps1_update_transmasks();
	void cps1_build_palette(const u16* const palette_base);
	void cps1_find_last_sprite();
	void cps1_render_sprites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void cps2_find_last_sprite();
	void cps2_render_sprites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int *primasks);
	void cps1_render_stars(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void cps1_render_layer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int primask);
	void cps1_render_high_layer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer);
	void cps2_set_sprite_priorities();
	void cps2_objram_latch();
	u16 *cps2_objbase();

	/* cps2 driver */
	void init_digital_volume();
	u16 gigaman2_dummyqsound_r(offs_t offset);
	void gigaman2_dummyqsound_w(offs_t offset, u16 data);
	void gigaman2_gfx_reorder();
	void cps2_eeprom_port_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 cps2_qsound_volume_r();
	u16 kludge_r();
	u16 joy_or_paddle_r();
	u16 joy_or_paddle_ecofghtr_r();
	DECLARE_WRITE_LINE_MEMBER(m5205_int1);
	DECLARE_WRITE_LINE_MEMBER(m5205_int2);
	void cps2(machine_config &config);
	void gigaman2(machine_config &config);
	void dead_cps2(machine_config &config);
	void cawingbl(machine_config &config);
	void sf2mdt(machine_config &config);
	void sf2m1(machine_config &config);
	void kodb(machine_config &config);
	void varthb(machine_config &config);
	void sgyxz(machine_config &config);
	void punipic(machine_config &config);
	void dinopic(machine_config &config);
	void slampic(machine_config &config);
	void sf2b(machine_config &config);
	void knightsb(machine_config &config);
	void fcrash(machine_config &config);
	void sf2m10(machine_config &config);
	void sf2m3(machine_config &config);
	void forgottn(machine_config &config);
	void ganbare(machine_config &config);
	void qsound(machine_config &config);
	void cps1_12MHz(machine_config &config);
	void wofhfh(machine_config &config);
	void cps1_10MHz(machine_config &config);
	void pang3(machine_config &config);
	void cps2_map(address_map &map);
	void dead_cps2_map(address_map &map);
	void decrypted_opcodes_map(address_map &map);
	void dinopic_map(address_map &map);
	void fcrash_map(address_map &map);
	void forgottn_map(address_map &map);
	void knightsb_map(address_map &map);
	void knightsb_z80map(address_map &map);
	void kodb_sound_map(address_map &map);
	void main_map(address_map &map);
	void punipic_map(address_map &map);
	void qsound_decrypted_opcodes_map(address_map &map);
	void qsound_main_map(address_map &map);
	void qsound_sub_map(address_map &map);
	void sf2b_map(address_map &map);
	void sf2m10_map(address_map &map);
	void sf2m1_map(address_map &map);
	void sf2m3_map(address_map &map);
	void sf2mdt_map(address_map &map);
	void sf2mdt_z80map(address_map &map);
	void sgyxz_map(address_map &map);
	void sgyxz_sound_map(address_map &map);
	void wofabl_map(address_map &map);
	void slampic_map(address_map &map);
	void sound_map(address_map &map);
	void sub_map(address_map &map);
	void varthb_map(address_map &map);
};

/*----------- defined in drivers/cps1.c -----------*/

extern gfx_decode_entry const gfx_cps1[];

#endif
