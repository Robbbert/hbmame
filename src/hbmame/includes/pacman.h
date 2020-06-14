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
	optional_shared_ptr<uint8_t> m_spriteram;
	optional_shared_ptr<uint8_t> m_spriteram2;
	optional_shared_ptr<uint8_t> m_s2650_spriteram;
	required_shared_ptr<uint8_t> m_videoram;
	optional_shared_ptr<uint8_t> m_colorram;
	optional_shared_ptr<uint8_t> m_s2650games_tileram;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	optional_region_ptr<u8> m_p_maincpu;
	optional_ioport m_io_fake;

	tilemap_t *m_bg_tilemap;
	uint8_t m_charbank;
	uint8_t m_spritebank;
	uint8_t m_palettebank;
	uint8_t m_colortablebank;
	uint8_t m_flipscreen;
	uint8_t m_bgpriority;
	int m_xoffsethack;
	uint8_t m_inv_spr;
	uint8_t m_maketrax_counter;
	uint8_t m_maketrax_offset;
	int m_maketrax_disable_protection;
	uint8_t m_irq_mask;

	DECLARE_WRITE8_MEMBER(pacman_interrupt_vector_w);
	DECLARE_WRITE8_MEMBER(piranha_interrupt_vector_w);
	DECLARE_WRITE_LINE_MEMBER(coin_counter_w);
	DECLARE_WRITE_LINE_MEMBER(coin_lockout_global_w);
	DECLARE_WRITE8_MEMBER(maketrax_protection_w);
	DECLARE_READ8_MEMBER(maketrax_special_port2_r);
	DECLARE_READ8_MEMBER(maketrax_special_port3_r);
	DECLARE_READ8_MEMBER(pacman_read_nop);
	DECLARE_READ8_MEMBER(in1_r);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x0038);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x03b0);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x1600);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x2120);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x3ff0);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x8000);
	DECLARE_READ8_MEMBER(mspacman_disable_decode_r_0x97f0);
	DECLARE_WRITE8_MEMBER(mspacman_disable_decode_w);
	DECLARE_READ8_MEMBER(mspacman_enable_decode_r_0x3ff8);
	DECLARE_WRITE8_MEMBER(mspacman_enable_decode_w);
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
	uint32_t screen_update_pacman(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_s2650games(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	INTERRUPT_GEN_MEMBER(vblank_irq);
	void init_save_state();
	void jrpacman_mark_tile_dirty( int offset );
	void eyes_decode(uint8_t *data);
	void mspacman_install_patches(uint8_t *ROM);

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
	uint32_t screen_update_pacmanx(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_multipac(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_pengo(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void multipac_palette(palette_device &palette) const;
	void pengo_palette(palette_device &palette) const;
	void m96in1b_gfxbank_w(uint8_t gfxbank);
	void madpac_gfxbank_w(uint8_t gfxbank);
	DECLARE_WRITE_LINE_MEMBER(led1_w);
	DECLARE_WRITE_LINE_MEMBER(led2_w);
	DECLARE_WRITE8_MEMBER(multipac_gfxbank_w);	
	DECLARE_WRITE8_MEMBER(multipac_palbank_w);
	TILE_GET_INFO_MEMBER(multipac_get_tile_info);
	uint8_t m_speedcheat;
	void speedcheat();
	uint8_t m_timerthing;
	DECLARE_READ8_MEMBER(mspacii_prot_r);
	DECLARE_READ8_MEMBER(zolatimer_r);
	DECLARE_WRITE8_MEMBER(zolatimer_w);
	DECLARE_MACHINE_RESET(mspacman);
	DECLARE_WRITE8_MEMBER(m96in1_rombank_w);
	DECLARE_WRITE8_MEMBER(m96in1b_rombank_w);
	DECLARE_WRITE8_MEMBER(hackypac_rombank_w);
	DECLARE_WRITE8_MEMBER(madpac_rombank_w);
	DECLARE_WRITE8_MEMBER(multipac_rombank_w);
	DECLARE_WRITE8_MEMBER(pm4n1_rombank_w);
	DECLARE_WRITE8_MEMBER(pm4n1d_rombank_w);
	DECLARE_WRITE8_MEMBER(superabc_rombank_w);
	DECLARE_MACHINE_RESET(96in1);
	DECLARE_MACHINE_RESET(hackypac);
	DECLARE_MACHINE_RESET(madpac);
	DECLARE_MACHINE_RESET(multipac);
	DECLARE_MACHINE_RESET(mspaceur);
	DECLARE_MACHINE_RESET(pm4n1);
	uint8_t hbmame_pacplus_decode(offs_t addr, uint8_t e);
// END HBMAME
};

#endif // MAME_INCLUDES_PACMAN_H
