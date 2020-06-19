// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria
#ifndef MAME_INCLUDES_PACMAN_H
#define MAME_INCLUDES_PACMAN_H

#pragma once

#include "machine/74259.h"
#include "machine/watchdog.h"
#include "sound/namco.h"
#include "tilemap.h"
#include "emupal.h"

/*************************************************************************

    Namco PuckMan

**************************************************************************/

class pacman_state : public driver_device
{
public:
	pacman_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_mainlatch(*this, "mainlatch")
		, m_namco_sound(*this, "namco")
		, m_watchdog(*this, "watchdog")
		, m_screen(*this, "screen")
		, m_spriteram(*this, "spriteram")
		, m_spriteram2(*this, "spriteram2")
		, m_s2650_spriteram(*this, "s2650_spriteram")
		, m_videoram(*this, "videoram")
		, m_colorram(*this, "colorram")
		, m_s2650games_tileram(*this, "s2650_tileram")
		, m_gfxdecode(*this, "gfxdecode")
		, m_palette(*this, "palette")
		, m_p_maincpu(*this, "maincpu")
		, m_io_fake(*this, "FAKE")
	{ }

	void mschamp_map(address_map &map);
	void mspacman_map(address_map &map);
	void pacman_map(address_map &map);
	void superabc_map(address_map &map);
	void woodpek_map(address_map &map);
	void otto1012_map(address_map &map);
	void init_maketrax();
	void init_drivfrcp();
	void init_mspacmbe();
	void init_ponpoko();
	void init_eyes();
	void init_woodpek();
	void init_cannonbp();
	void init_jumpshot();
	void init_mspacii();
	void init_pacplus();
	void init_rocktrv2();
	void init_superabc();
	void init_8bpm();
	void init_porky();
	void init_mspacman();
	void init_mschamp();
	void init_mbrush();
	void init_pengomc1();

protected:
	required_device<cpu_device> m_maincpu;
	optional_device<ls259_device> m_mainlatch;
	optional_device<namco_device> m_namco_sound;
	required_device<watchdog_timer_device> m_watchdog;
	required_device<screen_device> m_screen;
	optional_shared_ptr<u8> m_spriteram;
	optional_shared_ptr<u8> m_spriteram2;
	optional_shared_ptr<u8> m_s2650_spriteram;
	required_shared_ptr<u8> m_videoram;
	optional_shared_ptr<u8> m_colorram;
	optional_shared_ptr<u8> m_s2650games_tileram;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	optional_region_ptr<u8> m_p_maincpu;
	optional_ioport m_io_fake;

	tilemap_t *m_bg_tilemap;
	u8 m_charbank;
	u8 m_spritebank;
	u8 m_palettebank;
	u8 m_colortablebank;
	u8 m_flipscreen;
	u8 m_bgpriority;
	int m_xoffsethack;
	u8 m_inv_spr;
	u8 m_maketrax_counter;
	u8 m_maketrax_offset;
	int m_maketrax_disable_protection;
	u8 m_irq_mask;

	void pacman_interrupt_vector_w(u8 data);
	void piranha_interrupt_vector_w(u8 data);
	DECLARE_WRITE_LINE_MEMBER(coin_counter_w);
	DECLARE_WRITE_LINE_MEMBER(coin_lockout_global_w);
	void maketrax_protection_w(u8 data);
	u8 maketrax_special_port2_r(offs_t offset);
	u8 maketrax_special_port3_r(offs_t offset);
	u8 pacman_read_nop();
	u8 in1_r();
	u8 mspacman_disable_decode_r_0x0038(offs_t offset);
	u8 mspacman_disable_decode_r_0x03b0(offs_t offset);
	u8 mspacman_disable_decode_r_0x1600(offs_t offset);
	u8 mspacman_disable_decode_r_0x2120(offs_t offset);
	u8 mspacman_disable_decode_r_0x3ff0(offs_t offset);
	u8 mspacman_disable_decode_r_0x8000(offs_t offset);
	u8 mspacman_disable_decode_r_0x97f0(offs_t offset);
	void mspacman_disable_decode_w(u8 data);
	u8 mspacman_enable_decode_r_0x3ff8(offs_t offset);
	void mspacman_enable_decode_w(u8 data);
	DECLARE_WRITE_LINE_MEMBER(irq_mask_w);
	void pacman_videoram_w(offs_t, u8);
	void pacman_colorram_w(offs_t, u8);
	DECLARE_WRITE_LINE_MEMBER(flipscreen_w);
	DECLARE_WRITE_LINE_MEMBER(pengo_palettebank_w);
	DECLARE_WRITE_LINE_MEMBER(pengo_colortablebank_w);
	DECLARE_WRITE_LINE_MEMBER(pengo_gfxbank_w);
	void s2650games_videoram_w(offs_t, u8);
	void s2650games_colorram_w(offs_t, u8);
	void s2650games_scroll_w(offs_t, u8);
	void s2650games_tilesbank_w(offs_t, u8);
	void jrpacman_videoram_w(offs_t, u8);
	DECLARE_WRITE_LINE_MEMBER(jrpacman_charbank_w);
	DECLARE_WRITE_LINE_MEMBER(jrpacman_spritebank_w);
	void jrpacman_scroll_w(u8);
	DECLARE_WRITE_LINE_MEMBER(jrpacman_bgpriority_w);
	TILEMAP_MAPPER_MEMBER(pacman_scan_rows);
	TILE_GET_INFO_MEMBER(pacman_get_tile_info);
	TILE_GET_INFO_MEMBER(s2650_get_tile_info);
	TILEMAP_MAPPER_MEMBER(jrpacman_scan_rows);
	TILE_GET_INFO_MEMBER(jrpacman_get_tile_info);
	DECLARE_VIDEO_START(pacman);
	void pacman_palette(palette_device &palette) const;
	DECLARE_VIDEO_START(birdiy);
	DECLARE_VIDEO_START(s2650games);
	DECLARE_MACHINE_RESET(mschamp);
	DECLARE_MACHINE_RESET(maketrax);
	DECLARE_VIDEO_START(pengo);
	DECLARE_VIDEO_START(jrpacman);
	u32 screen_update_pacman(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	u32 screen_update_s2650games(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	INTERRUPT_GEN_MEMBER(vblank_irq);
	void init_save_state();
	void jrpacman_mark_tile_dirty( int offset );
	void eyes_decode(u8 *data);
	void mspacman_install_patches(u8 *ROM);

public:
	void mspacman(machine_config &config);
	void mschamp(machine_config &config);
	void woodpek(machine_config &config);
	void superabc(machine_config &config);
	void pacman(machine_config &config);
	void maketrax(machine_config &config);
	void korosuke(machine_config &config);
	void alibaba(machine_config &config);
	void drivfrcp(machine_config &config);
	void pengojpm(machine_config &config);
	void piranha(machine_config &config);

	// HBMAME extras
	void _96in1(machine_config &config);
	void _96in1b(machine_config &config);
	void hackypac(machine_config &config);
	void madpac(machine_config &config);
	void mspaceur(machine_config &config);
	void mschampx(machine_config &config);
	void multipac(machine_config &config);
	void pm4n1(machine_config &config);
	void pm4n1c(machine_config &config);
	void pm4n1d(machine_config &config);
	void mspacmnx(machine_config &config);
	void woodpekx(machine_config &config);
	void mspacii(machine_config &config);
	void zolapc(machine_config &config);
	void pachack(machine_config &config);
	void mspachi(machine_config &config);
	void widel(machine_config &config);
	void pacmanx(machine_config &config);
	void zolapac(machine_config &config);
	void otto1012(machine_config &config);
	void init_96in1();
	void init_madpac();
	void init_mspaceur();
	void init_multipac();
	void init_pm4n1();
	void init_pengo();
	void init_otto1029();
	DECLARE_INPUT_CHANGED_MEMBER(pacman_fake);

protected:
	void io_map(address_map &map);
	void mspachi_map(address_map &map);
	void mspacii_map(address_map &map);
	void widel_map(address_map &map);
	void zolapac_io(address_map &map);
	void _96in1_writeport(address_map &map);
	void _96in1b_writeport(address_map &map);
	void hackypac_map(address_map &map);
	void hackypac_writeport(address_map &map);
	void madpac_map(address_map &map);
	void madpac_writeport(address_map &map);
	void mspaceur_map(address_map &map);
	void multipac_map(address_map &map);
	void pm4n1_map(address_map &map);
	void pm4n1c_map(address_map &map);
	void pm4n1d_map(address_map &map);
	void piranha_writeport(address_map &map);
	//void woodpek_map(address_map &map);
	//void woodpek_map(address_map &map);
	//void woodpek_map(address_map &map);
	DECLARE_VIDEO_START(pacmanx);
	DECLARE_VIDEO_START(multipac);
	u32 screen_update_pacmanx(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	u32 screen_update_multipac(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	u32 screen_update_pengo(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void multipac_palette(palette_device &palette) const;
	void pengo_palette(palette_device &palette) const;
	void m96in1b_gfxbank_w(u8 gfxbank);
	void madpac_gfxbank_w(u8 gfxbank);
	DECLARE_WRITE_LINE_MEMBER(led1_w);
	DECLARE_WRITE_LINE_MEMBER(led2_w);
	void multipac_gfxbank_w(u8 data);
	void multipac_palbank_w(offs_t offset, u8 data);
	TILE_GET_INFO_MEMBER(multipac_get_tile_info);
	u8 m_speedcheat;
	void speedcheat();
	u8 m_timerthing;
	u8 mspacii_prot_r(offs_t offset);
	u8 zolatimer_r();
	void zolatimer_w(u8 data);
	DECLARE_MACHINE_RESET(mspacman);
	void m96in1_rombank_w(u8 data);
	void m96in1b_rombank_w(u8 data);
	void hackypac_rombank_w(u8 data);
	void madpac_rombank_w(u8 data);
	void multipac_rombank_w(u8 data);
	void pm4n1_rombank_w(offs_t offset, u8 data);
	void pm4n1d_rombank_w(offs_t offset, u8 data);
	void superabc_rombank_w(u8 data);
	DECLARE_MACHINE_RESET(96in1);
	DECLARE_MACHINE_RESET(hackypac);
	DECLARE_MACHINE_RESET(madpac);
	DECLARE_MACHINE_RESET(multipac);
	DECLARE_MACHINE_RESET(mspaceur);
	DECLARE_MACHINE_RESET(pm4n1);
	u8 hbmame_pacplus_decode(offs_t addr, u8 e);
// END HBMAME
};

#endif // MAME_INCLUDES_PACMAN_H
