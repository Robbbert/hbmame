// license:BSD-3-Clause
// copyright-holders:Bryan McPhail,Ernesto Corvi,Andrew Prime,Zsolt Vasvari
// thanks-to:Fuzz
/*************************************************************************

    Neo-Geo hardware

*************************************************************************/

#include "emu.h"
#include "cpu/m68000/m68000.h"
#include "cpu/z80/z80.h"
#include "machine/nvram.h"
#include "machine/watchdog.h"
#include "sound/2610intf.h"
#include "machine/upd1990a.h"
#include "machine/ng_memcard.h"
#include "machine/gen_latch.h"
#include "video/neogeo_spr.h"
#include "bus/neogeo/prot.h"
#include "bus/neogeo/banked_cart.h"
#include "bus/neogeo_ctrl/ctrl.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"

// On scanline 224, /VBLANK goes low 56 mclks (14 pixels) from the rising edge of /HSYNC.
// Two mclks after /VBLANK goes low, the hardware sets a pending IRQ1 flip-flop.
#define NEOGEO_VBLANK_IRQ_HTIM (attotime::from_ticks(56+2, NEOGEO_MASTER_CLOCK))
	// macros allow code below to be copy+pasted into slot devices more easily
#define cpuregion memregion("maincpu")->base()
#define cpuregion_size memregion("maincpu")->bytes()
#define spr_region memregion("sprites")->base()
#define spr_region_size memregion("sprites")->bytes()
#define fix_region memregion("fixed")->base()
#define fix_region_size memregion("fixed")->bytes()
#define ym_region memregion("ymsnd")->base()
#define ym_region_size memregion("ymsnd")->bytes()
#define audiocpu_region memregion("audiocpu")->base()
#define audio_region_size memregion("audiocpu")->bytes()
#define audiocrypt_region memregion("audiocrypt")->base()
#define audiocrypt_region_size memregion("audiocrypt")->bytes()


class neogeo_state : public driver_device
{
public:
	neogeo_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_use_cart_vectors(0)
		, m_use_cart_audio(0)
		, m_banked_cart(*this, "banked_cart")
		, m_maincpu(*this, "maincpu")
		, m_audiocpu(*this, "audiocpu")
		, m_ym(*this, "ymsnd")
		, m_region_maincpu(*this, "maincpu")
		, m_region_sprites(*this, "sprites")
		, m_region_fixed(*this, "fixed")
		, m_region_fixedbios(*this, "fixedbios")
		, m_bank_audio_main(*this, "audio_main")
		, m_upd4990a(*this, "upd4990a")
		, m_save_ram(*this, "saveram")
		, m_screen(*this, "screen")
		, m_palette(*this, "palette")
		, m_memcard(*this, "memcard")
		, m_dsw(*this, "DSW")
		, m_trackx(*this, "TRACK_X")
		, m_tracky(*this, "TRACK_Y")
		, m_edge(*this, "edge")
		, m_ctrl1(*this, "ctrl1")
		, m_ctrl2(*this, "ctrl2")
		, m_sprgen(*this, "spritegen")
		, m_soundlatch(*this, "soundlatch")
		, m_soundlatch2(*this, "soundlatch2")
		, m_mslugx_prot(*this, "mslugx_prot")
		, m_sma_prot(*this, "sma_prot")
		, m_cmc_prot(*this, "cmc_prot")
		, m_pcm2_prot(*this, "pcm2_prot")
		, m_pvc_prot(*this, "pvc_prot")
		, m_bootleg_prot(*this, "bootleg_prot")
		, m_kof2002_prot(*this, "kof2002_prot")
		, m_fatfury2_prot(*this, "fatfury2_prot")
		, m_kof98_prot(*this, "kof98_prot")
		, m_sbp_prot(*this, "sbp_prot")
		, m_kog_prot(*this, "kog_prot")
		, m_out_digit(*this, "digit%u", 0U)
	{ }

	void hbmame_kog(machine_config &config);
	void mvs(machine_config &config);
	void neogeo_arcade(machine_config &config);
	void neogeo_base(machine_config &config);
	void neogeo_dial(machine_config &config);
	void neogeo_imaze(machine_config &config);
	void neogeo_kiz4p(machine_config &config);
	void neogeo_kog(machine_config &config);
	void neogeo_mj(machine_config &config);
	void neogeo_noctrl(machine_config &config);
	void neogeo_noslot(machine_config &config);
	void no_watchdog(machine_config &config);
	void gsc(machine_config &config);

	void init_bangbead();
	void init_cmc42sfix();
	void init_ct2k3sp();
	void init_ct2k3sa();
	void init_cthd2003();
	void init_cthd2k3a();
	void init_fatfury2();
	void init_fr2ch();
	void init_ganryu();
	void init_garou();
	void init_garouh();
	void init_garoubl();
	void init_garoud();
	void init_irrmaze();
	void init_jckeygpd();
	void init_jockeygp();
	void init_kf10thep();
	void init_kf2k1pa();
	void init_kf2k2mp();
	void init_kf2k2mp2();
	void init_kf2k3pcb();
	void init_kf2k3pl();
	void init_kf2k3upl();
	void init_kf2k5uni();
	void init_kof10th();
	void init_kof10thu();
	void init_kof2k2bd();
	void init_kof2k2pl17();
	void init_kof2k3fd();
	void init_kof2k3hd();
	void init_kof2k3pcd();
	void init_kof2k4pls();
	void init_kof2k4se();
	void init_kof96ep();
	void init_kof97pla();
	void init_kof97oro();
	void init_kof98();
	void init_kof99();
	void init_kof2000();
	void init_kof2001();
	void init_kof2002();
	void init_kof2002b();
	void init_kof2003();
	void init_kof2003h();
	void init_kof2003b();
	void init_kog();
	void init_kogd();
	void init_lans2004();
	void init_matrim();
	void init_matrima();
	void init_matrimbl();
	void init_matrimd();
	void init_matrmehc();
	void init_mslug3();
	void init_mslug3b6();
	void init_mslug3d();
	void init_mslug3fr();
	void init_mslug3h();
	void init_mslug3n();
	void init_mslug4();
	void init_mslug4hb();
	void init_mslug5();
	void init_mslug5b();
	void init_mslug5hb();
	void init_mslugx();
	void init_ms4plus();
	void init_ms5pcb();
	void init_ms5pcbd();
	void init_ms5plus();
	void init_ms6s16();
	void init_mp2s39();
	void init_neogeo();
	void init_nitd();
	void init_pnyaa();
	void init_pnyaad();
	void init_preisle2();
	void init_rotd();
	void init_rotdb();
	void init_rotdd();
	void init_rotdnd();
	void init_s1945p();
	void init_sam5hb();
	void init_sam5sphb();
	void init_sam5sphb2();
	void init_samsh5sp();
	void init_samsho5();
	void init_samsho5b();
	void init_sbp();
	void init_sengo3d();
	void init_sengoku3();
	void init_shockt2w();
	void init_svc();
	void init_svchb();
	void init_svcboot();
	void init_svcpcb();
	void init_svcpcd();
	void init_svcplus();
	void init_svcplusa();
	void init_svcsplus();
	void init_vliner();
	void init_xs02();
	void init_zupapa();
	DECLARE_CUSTOM_INPUT_MEMBER(get_memcard_status);
	DECLARE_CUSTOM_INPUT_MEMBER(get_audio_result);
	DECLARE_CUSTOM_INPUT_MEMBER(kizuna4p_start_r);
	DECLARE_INPUT_CHANGED_MEMBER(select_bios);

private:

	void io_control_w(offs_t offset, u8 data);
	u16 memcard_r(offs_t offset);
	void memcard_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void audio_command_w(u8 data);
	u8 audio_command_r();
	u8 audio_cpu_bank_select_r(offs_t offset);
	void audio_cpu_enable_nmi_w(offs_t offset, u8 data);
	void system_control_w(offs_t offset, u8 data);
	u16 neogeo_unmapped_r(address_space &space);
	u16 neogeo_paletteram_r(offs_t offset);
	void neogeo_paletteram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 neogeo_video_register_r(address_space &space, offs_t offset, u16 mem_mask = ~0);
	void neogeo_video_register_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 banked_vectors_r(offs_t offset);
	u16 in0_r();
	u16 in1_r();
	void save_ram_w(offs_t offset, u16 data, u16 mem_mask = ~0);

	TIMER_CALLBACK_MEMBER(display_position_interrupt_callback);
	TIMER_CALLBACK_MEMBER(display_position_vblank_callback);
	TIMER_CALLBACK_MEMBER(vblank_interrupt_callback);

	u32 screen_update_neogeo(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);

	void neogeo_main_map(address_map &map);
	void main_map_slot(address_map &map);
	void audio_map(address_map &map);
	void audio_io_map(address_map &map);
	void main_map_noslot(address_map &map);
	void gsc_map(address_map &map);
	void main_map1(address_map &map);

	void neogeo_postload();
	void update_interrupts();
	void create_interrupt_timers();
	void start_interrupt_timers();
	void neogeo_acknowledge_interrupt(u16  data);

	void neogeo_main_cpu_banking_init();
	void neogeo_audio_cpu_banking_init(int set_entry);
	void adjust_display_position_interrupt_timer();
	void neogeo_set_display_position_interrupt_control(u16  data);
	void neogeo_set_display_counter_msb(u16  data);
	void neogeo_set_display_counter_lsb(u16  data);
	void set_video_control( u16  data );

	void create_rgb_lookups();
	void set_pens();
	void neogeo_set_screen_shadow( int data );
	void neogeo_set_palette_bank( int data );

	void audio_cpu_check_nmi();
	void set_save_ram_unlock( u8 data );
	void set_outputs(  );
	void set_output_latch( u8 data );
	void set_output_data( u8 data );

	virtual void machine_start() override;
	virtual void machine_reset() override;

	memory_bank           *m_bank_audio_cart[4];

	// configuration
	enum {NEOGEO_MVS, NEOGEO_AES, NEOGEO_CD} m_type;

	// internal state
	bool       m_recurse;
	bool       m_audio_cpu_nmi_enabled;
	bool       m_audio_cpu_nmi_pending;

	// MVS-specific state
	u8      m_save_ram_unlocked;
	u8      m_output_data;
	u8      m_output_latch;
	u8      m_el_value;
	u8      m_led1_value;
	u8      m_led2_value;

	virtual void video_start() override;

	emu_timer  *m_display_position_interrupt_timer;
	emu_timer  *m_display_position_vblank_timer;
	emu_timer  *m_vblank_interrupt_timer;
	u32     m_display_counter;
	u8      m_vblank_interrupt_pending;
	u8      m_display_position_interrupt_pending;
	u8      m_irq3_pending;
	u8      m_display_position_interrupt_control;
	u8      m_vblank_level;
	u8      m_raster_level;

	u16  get_video_control(  );

	// color/palette related
	std::vector<u16 > m_paletteram;
	u8        m_palette_lookup[32][4];
	const pen_t *m_bg_pen;
	int          m_screen_shadow;
	int          m_palette_bank;

	u16 neogeo_slot_rom_low_r();
	u16 neogeo_slot_rom_low_vectors_r(offs_t offset);

	void install_banked_bios();

	int m_use_cart_vectors;
	int m_use_cart_audio;
	optional_device<neogeo_banked_cart_device> m_banked_cart;
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	required_device<ym2610_device> m_ym;
	required_memory_region m_region_maincpu;
	required_memory_region m_region_sprites;
	required_memory_region m_region_fixed;
	optional_memory_region m_region_fixedbios;
	optional_memory_bank   m_bank_audio_main; // optional because of neocd
	optional_device<upd4990a_device> m_upd4990a;
	optional_shared_ptr<u16 > m_save_ram;
	required_device<screen_device> m_screen;
	optional_device<palette_device> m_palette;
	optional_device<ng_memcard_device> m_memcard;
	optional_ioport m_dsw;
	optional_ioport m_trackx;
	optional_ioport m_tracky;
	optional_device<neogeo_ctrl_edge_port_device> m_edge;
	optional_device<neogeo_control_port_device> m_ctrl1;
	optional_device<neogeo_control_port_device> m_ctrl2;
	required_device<neosprite_device> m_sprgen;
	optional_device<generic_latch_8_device> m_soundlatch;
	optional_device<generic_latch_8_device> m_soundlatch2;
	optional_device<mslugx_prot_device> m_mslugx_prot;
	optional_device<sma_prot_device> m_sma_prot;
	optional_device<cmc_prot_device> m_cmc_prot;
	optional_device<pcm2_prot_device> m_pcm2_prot;
	optional_device<pvc_prot_device> m_pvc_prot;
	optional_device<ngbootleg_prot_device> m_bootleg_prot;
	optional_device<kof2002_prot_device> m_kof2002_prot;
	optional_device<fatfury2_prot_device> m_fatfury2_prot;
	optional_device<kof98_prot_device> m_kof98_prot;
	optional_device<sbp_prot_device> m_sbp_prot;
	optional_device<kog_prot_device> m_kog_prot;
	output_finder<5> m_out_digit;
};

/*----------- defined in drivers/neogeo.c -----------*/

INPUT_PORTS_EXTERN(neogeo);
INPUT_PORTS_EXTERN(jockeygp);
INPUT_PORTS_EXTERN(dualbios);

/*************************************
 *
 *  Neo-Geo bios
 *
 *************************************

    These are the known Bios Roms, Set options.bios to the one you want.

    The Universe bios roms are supported because they're now used on enough PCBs
    to be considered 'in active arcade use' rather than just homebrew hacks.
    Some may be missing, there have been multiple CRCs reported for the same
    revision in some cases (the Universe bios has an option for entering / displaying
    a serial number; these should be noted as such if they're added).
    Universe bios prior to version 1.3 was incompatible with AES.

    The 'japan-hotel' BIOS is a dump of an MVS which could be found in some japanese
    hotels. it is a custom MVS mobo which uses MVS carts but it hasn't jamma
    connector and it's similar to a console with a coin mechanism, so it's a sort
    of little coin op console installed in hotels.

    The sp-45.sp1 bios is the latest 'ASIA' revision. Japan-j3.bin is the latest 'JAPAN'
    revision. Both of them are also used in the sp-4x.sp1 bios of the Jamma PCB boards.

    The current Neo-Geo MVS system set (SFIX/SM1/000-LO) used is from a NEO-MVH MV1FS board.
    Other boards (MV1xx / MV2x / MV4x /MV6x) other system sets?

    Zoom ROM (LO)    128K   TC531000CP      1x 128Kx8   Zoom look-up table ROM
    Fix ROM (SFIX)   128K   27C1000         1x 128Kx8   Text layer graphics ROM
    Sound ROM (SM1)  128K   27C1000/23C1000 1x 128Kx8   Z80 program ROM

*/

#define ROM_LOAD16_WORD_SWAP_BIOS(bios,name,offset,length,hash) \
		ROMX_LOAD(name, offset, length, hash, ROM_GROUPWORD | ROM_REVERSE | ROM_BIOS(bios))

#define NEOGEO_UNIBIOS(x) \
	ROM_SYSTEM_BIOS( x+ 0, "unibios40", "Universe Bios (Hack, Ver. 4.0)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 0, "uni-bios_4_0.rom",  0x00000, 0x020000, CRC(a7aab458) SHA1(938a0bda7d9a357240718c2cec319878d36b8f72) ) /* Universe Bios v4.0 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 1, "unibios33", "Universe Bios (Hack, Ver. 3.3)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 1, "uni-bios_3_3.rom",  0x00000, 0x020000, CRC(24858466) SHA1(0ad92efb0c2338426635e0159d1f60b4473d0785) ) /* Universe Bios v3.3 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 2, "unibios32", "Universe Bios (Hack, Ver. 3.2)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 2, "uni-bios_3_2.rom",  0x00000, 0x020000, CRC(a4e8b9b3) SHA1(c92f18c3f1edda543d264ecd0ea915240e7c8258) ) /* Universe Bios v3.2 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 3, "unibios31", "Universe Bios (Hack, Ver. 3.1)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 3, "uni-bios_3_1.rom",  0x00000, 0x020000, CRC(0c58093f) SHA1(29329a3448c2505e1ff45ffa75e61e9693165153) ) /* Universe Bios v3.1 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 4, "unibios30", "Universe Bios (Hack, Ver. 3.0)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 4, "uni-bios_3_0.rom",  0x00000, 0x020000, CRC(a97c89a9) SHA1(97a5eff3b119062f10e31ad6f04fe4b90d366e7f) ) /* Universe Bios v3.0 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 5, "unibios23", "Universe Bios (Hack, Ver. 2.3)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 5, "uni-bios_2_3.rom",  0x00000, 0x020000, CRC(27664eb5) SHA1(5b02900a3ccf3df168bdcfc98458136fd2b92ac0) ) /* Universe Bios v2.3 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 6, "unibios23o", "Universe Bios (Hack, Ver. 2.3, older?)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 6, "uni-bios_2_3o.rom", 0x00000, 0x020000, CRC(601720ae) SHA1(1b8a72c720cdb5ee3f1d735bbcf447b09204b8d9) ) /* Universe Bios v2.3 (hack) alt version, withdrawn? */ \
	ROM_SYSTEM_BIOS( x+ 7, "unibios22", "Universe Bios (Hack, Ver. 2.2)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 7, "uni-bios_2_2.rom",  0x00000, 0x020000, CRC(2d50996a) SHA1(5241a4fb0c63b1a23fd1da8efa9c9a9bd3b4279c) ) /* Universe Bios v2.2 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 8, "unibios21", "Universe Bios (Hack, Ver. 2.1)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 8, "uni-bios_2_1.rom",  0x00000, 0x020000, CRC(8dabf76b) SHA1(c23732c4491d966cf0373c65c83c7a4e88f0082c) ) /* Universe Bios v2.1 (hack) */ \
	ROM_SYSTEM_BIOS( x+ 9, "unibios20", "Universe Bios (Hack, Ver. 2.0)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+ 9, "uni-bios_2_0.rom",  0x00000, 0x020000, CRC(0c12c2ad) SHA1(37bcd4d30f3892078b46841d895a6eff16dc921e) ) /* Universe Bios v2.0 (hack) */ \
	ROM_SYSTEM_BIOS( x+10, "unibios13", "Universe Bios (Hack, Ver. 1.3)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+10, "uni-bios_1_3.rom",  0x00000, 0x020000, CRC(b24b44a0) SHA1(eca8851d30557b97c309a0d9f4a9d20e5b14af4e) ) /* Universe Bios v1.3 (hack) */
#define NEOGEO_UNIBIOS_1_2_AND_OLDER(x) \
	ROM_SYSTEM_BIOS( x+11, "unibios12", "Universe Bios (Hack, Ver. 1.2)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+11, "uni-bios_1_2.rom",  0x00000, 0x020000, CRC(4fa698e9) SHA1(682e13ec1c42beaa2d04473967840c88fd52c75a) ) /* Universe Bios v1.2 (hack) */ \
	ROM_SYSTEM_BIOS( x+12, "unibios12o", "Universe Bios (Hack, Ver. 1.2, older)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+12, "uni-bios_1_2o.rom", 0x00000, 0x020000, CRC(e19d3ce9) SHA1(af88ef837f44a3af2d7144bb46a37c8512b67770) ) /* Universe Bios v1.2 (hack) alt version */ \
	ROM_SYSTEM_BIOS( x+13, "unibios11", "Universe Bios (Hack, Ver. 1.1)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+13, "uni-bios_1_1.rom",  0x00000, 0x020000, CRC(5dda0d84) SHA1(4153d533c02926a2577e49c32657214781ff29b7) ) /* Universe Bios v1.1 (hack) */ \
	ROM_SYSTEM_BIOS( x+14, "unibios10", "Universe Bios (Hack, Ver. 1.0)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( x+14, "uni-bios_1_0.rom",  0x00000, 0x020000, CRC(0ce453a0) SHA1(3b4c0cd26c176fc6b26c3a2f95143dd478f6abf9) ) /* Universe Bios v1.0 (hack) */

#define NEOGEO_BIOS \
	ROM_REGION16_BE( 0x80000, "mainbios", 0 ) \
	ROMX_LOAD( "sp-s2.sp1", 0x00000, 0x020000, CRC(9036d879) SHA1(4f5ed7105b7128794654ce82b51723e16e389543), ROM_GROUPWORD | ROM_REVERSE ) \
	ROM_SYSTEM_BIOS( 0, "euro", "Europe MVS (Ver. 2)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0, "sp-s2.sp1",         0x00000, 0x020000, CRC(9036d879) SHA1(4f5ed7105b7128794654ce82b51723e16e389543) ) /* 5 Dark Blue - Europe, 1 Slot, has also been found on 2 Slot and 4 Slot (the old hacks were designed for this one) */ \
	ROM_SYSTEM_BIOS( 1, "euro-s1", "Europe MVS (Ver. 1)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1, "sp-s.sp1",          0x00000, 0x020000, CRC(c7f2fa45) SHA1(09576ff20b4d6b365e78e6a5698ea450262697cd) ) /* 3 Dark Blue - Europe, 4 Slot */ \
	ROM_SYSTEM_BIOS( 2, "asia-mv1c", "Asia NEO-MVH MV1C" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 2, "sp-45.sp1",         0x00000, 0x080000, CRC(03cc9f6a) SHA1(cdf1f49e3ff2bac528c21ed28449cf35b7957dc1) ) /* 6 Dark Blue - Latest Asia bios (MV1C - mask ROM) */ \
	ROM_SYSTEM_BIOS( 3, "asia-mv1b", "Asia MV1B" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 3, "sp-s3.sp1",         0x00000, 0x020000, CRC(91b64be3) SHA1(720a3e20d26818632aedf2c2fd16c54f213543e1) ) /* 6 Dark Blue - Asia (MV1B) */ \
	\
	ROM_SYSTEM_BIOS( 4, "us", "US MVS (Ver. 2?)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 4, "sp-u2.sp1",         0x00000, 0x020000, CRC(e72943de) SHA1(5c6bba07d2ec8ac95776aa3511109f5e1e2e92eb) ) /* 5 Cyan - US, 2 Slot */ \
	ROM_SYSTEM_BIOS( 5, "us-e", "US MVS (Ver. 1)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 5, "sp-e.sp1",          0x00000, 0x020000, CRC(2723a5b5) SHA1(5dbff7531cf04886cde3ef022fb5ca687573dcb8) ) /* 5 Yellow - US, 6 Slot (V5?) */ \
	ROM_SYSTEM_BIOS( 6, "us-v2", "US MVS (4 slot, Ver 2)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 6, "sp1-u2",            0x00000, 0x020000, CRC(62f021f4) SHA1(62d372269e1b3161c64ae21123655a0a22ffd1bb) ) /* 3 Cyan - US, 4 slot - also seen with "v2" label*/ \
	ROM_SYSTEM_BIOS( 7, "us-u4", "US MVS (U4)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 7, "sp1-u4.bin",        0x00000, 0x020000, CRC(1179a30f) SHA1(866817f47aa84d903d0b819d61f6ef356893d16a) ) /* 3 Green - 4 Slot (MV-4F) */ \
	ROM_SYSTEM_BIOS( 8, "us-u3", "US MVS (U3)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 8, "sp1-u3.bin",        0x00000, 0x020000, CRC(2025b7a2) SHA1(73d774746196f377111cd7aa051cc8bb5dd948b3) ) /* 2 Green - 6 Slot */ \
	\
	ROM_SYSTEM_BIOS( 9, "japan", "Japan MVS (Ver. 3)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 9, "vs-bios.rom",       0x00000, 0x020000, CRC(f0e8f27d) SHA1(ecf01eda815909f1facec62abf3594eaa8d11075) ) /* 6 Red - Japan, Ver 6 VS Bios */ \
	ROM_SYSTEM_BIOS( 10, "japan-s2", "Japan MVS (Ver. 2)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 10, "sp-j2.sp1",        0x00000, 0x020000, CRC(acede59c) SHA1(b6f97acd282fd7e94d9426078a90f059b5e9dd91) ) /* 5 Red - Japan, Older */ \
	ROM_SYSTEM_BIOS( 11, "japan-s1", "Japan MVS (Ver. 1)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 11, "sp1.jipan.1024",   0x00000, 0x020000, CRC(9fb0abe4) SHA1(18a987ce2229df79a8cf6a84f968f0e42ce4e59d) ) /* 3 Red - Japan, Older */ \
	ROM_SYSTEM_BIOS( 12, "japan-mv1b", "Japan MV1B" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 12, "japan-j3.bin",     0x00000, 0x020000, CRC(dff6d41f) SHA1(e92910e20092577a4523a6b39d578a71d4de7085) ) /* 6 Red - Latest Japan bios (MV1B) */ \
	ROM_SYSTEM_BIOS( 13, "japan-j3a", "Japan MVS (J3, alt)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 13, "sp1-j3.bin",       0x00000, 0x020000, CRC(fbc6d469) SHA1(46b2b409b5b68869e367b40c846373623edb632a) ) /* 2 Red - 6 Slot */ \
	ROM_SYSTEM_BIOS( 14, "japan-mv1c", "Japan NEO-MVH MV1C" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 14, "sp-j3.sp1",        0x00000, 0x080000, CRC(486cb450) SHA1(52c21ea817928904b80745a8c8d15cbad61e1dc1) ) /* 6 Red - Latest Japan bios (MV1C - mask ROM) */ \
	\
	ROM_SYSTEM_BIOS( 15, "japan-hotel", "Custom Japanese Hotel" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 15, "sp-1v1_3db8c.bin", 0x00000, 0x020000, CRC(162f0ebe) SHA1(fe1c6dd3dfcf97d960065b1bb46c1e11cb7bf271) ) /* 6 Red - 'rare MVS found in japanese hotels' shows v1.3 in test mode */ \
	\
	NEOGEO_UNIBIOS(16) \
	NEOGEO_UNIBIOS_1_2_AND_OLDER(16) \
	ROM_DEFAULT_BIOS("euro")


#define NEO_BIOS_AUDIO_64K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x20000, "audiocpu", 0 ) \
	ROM_LOAD( name, 0x00000, 0x10000, hash ) \
	ROM_RELOAD(     0x10000, 0x10000 )

#define NEO_BIOS_AUDIO_128K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x30000, "audiocpu", 0 ) \
	ROM_LOAD( name, 0x00000, 0x20000, hash ) \
	ROM_RELOAD(     0x10000, 0x20000 )

#define NEO_BIOS_AUDIO_256K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x50000, "audiocpu", 0 ) \
	ROM_LOAD( name, 0x00000, 0x40000, hash ) \
	ROM_RELOAD(     0x10000, 0x40000 )

#define NEO_BIOS_AUDIO_512K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x90000, "audiocpu", 0 ) \
	ROM_LOAD( name, 0x00000, 0x80000, hash ) \
	ROM_RELOAD(     0x10000, 0x80000 )


#define NEO_BIOS_AUDIO_ENCRYPTED_128K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x80000, "audiocrypt", 0 ) \
	ROM_LOAD( name, 0x00000, 0x20000, hash )
#define NEO_BIOS_AUDIO_ENCRYPTED_256K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x80000, "audiocrypt", 0 ) \
	ROM_LOAD( name, 0x00000, 0x40000, hash )
#define NEO_BIOS_AUDIO_ENCRYPTED_512K(name, hash) \
	NEOGEO_BIOS \
	ROM_REGION( 0x20000, "audiobios", 0 ) \
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) ) \
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF ) \
	ROM_REGION( 0x80000, "audiocrypt", 0 ) \
	ROM_LOAD( name,      0x00000, 0x80000, hash )


#define ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "zoomy", 0 ) \
	ROM_LOAD( "000-lo.lo", 0x00000, 0x20000, CRC(5a86cff2) SHA1(5992277debadeb64d1c1c64b0a92d9293eaf7e4a) )

#define NEO_SFIX_MT(bytes) \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix",  0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( bytes, "fixed", ROMREGION_ERASE00 )

#define NEO_SFIX_MT_128K \
	NEO_SFIX_MT( 0x20000 )

#define NEO_SFIX_MT_512K \
	NEO_SFIX_MT( 0x80000 )

#define NEO_SFIX_MSLUG(name, hash) \
	NEO_SFIX_MT( 0x80000 ) \
	ROM_LOAD( name, 0x00000, 0x20000, hash ) \
	ROM_RELOAD( 0x20000, 0x20000 ) \
	ROM_RELOAD( 0x40000, 0x20000 ) \
	ROM_RELOAD( 0x60000, 0x20000 )

#define NEO_SFIX(bytes, name, hash) \
	NEO_SFIX_MT( bytes ) \
	ROM_LOAD( name, 0x00000, bytes, hash )

#define NEO_SFIX_64K(name, hash) \
	NEO_SFIX( 0x10000, name, hash )

#define NEO_SFIX_128K(name, hash) \
	NEO_SFIX( 0x20000, name, hash )

#define NEO_SFIX_512K(name, hash) \
	NEO_SFIX( 0x80000, name, hash )
