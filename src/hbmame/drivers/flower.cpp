/*

Flower (c)1986 Komax (USA license)
       (c)1986 Sega/Alpha (Sega game number 834-5998)
*/

#include "emu.h"
#include "cpu/z80/z80.h"

class flower_state : public driver_device
{
public:
	flower_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_sn_nmi_enable(*this, "sn_nmi_enable")
		, m_spriteram(*this, "spriteram")
		, m_textram(*this, "textram")
		, m_bg0ram(*this, "bg0ram")
		, m_bg1ram(*this, "bg1ram")
		, m_bg0_scroll(*this, "bg0_scroll")
		, m_bg1_scroll(*this, "bg1_scroll")
		, m_maincpu(*this, "maincpu")
		, m_audiocpu(*this, "audiocpu")
		, m_subcpu(*this, "subcpu")
		, m_gfxdecode(*this, "gfxdecode")
		, m_palette(*this, "palette")
		{ }

	DECLARE_READ8_MEMBER(soundlatch_byte_r);
	DECLARE_WRITE8_MEMBER(flower_maincpu_irq_ack);
	DECLARE_WRITE8_MEMBER(flower_subcpu_irq_ack);
	DECLARE_WRITE8_MEMBER(flower_soundcpu_irq_ack);
	DECLARE_WRITE8_MEMBER(flower_coin_counter_w);
	DECLARE_WRITE8_MEMBER(flower_coin_lockout_w);
	DECLARE_WRITE8_MEMBER(sound_command_w);
	DECLARE_WRITE8_MEMBER(flower_textram_w);
	DECLARE_WRITE8_MEMBER(flower_bg0ram_w);
	DECLARE_WRITE8_MEMBER(flower_bg1ram_w);
	DECLARE_WRITE8_MEMBER(flower_flipscreen_w);
	DECLARE_INPUT_CHANGED_MEMBER(coin_inserted);
	TILE_GET_INFO_MEMBER(get_bg0_tile_info);
	TILE_GET_INFO_MEMBER(get_bg1_tile_info);
	TILE_GET_INFO_MEMBER(get_text_tile_info);
	UINT32 screen_update_flower(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

private:

	UINT8 m_soundlatch;
	tilemap_t *m_bg0_tilemap;
	tilemap_t *m_bg1_tilemap;
	tilemap_t *m_text_tilemap;
	tilemap_t *m_text_right_tilemap;
	virtual void video_start() override;
	void draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect );
	required_shared_ptr<UINT8> m_sn_nmi_enable;
	required_shared_ptr<UINT8> m_spriteram;
	required_shared_ptr<UINT8> m_textram;
	required_shared_ptr<UINT8> m_bg0ram;
	required_shared_ptr<UINT8> m_bg1ram;
	required_shared_ptr<UINT8> m_bg0_scroll;
	required_shared_ptr<UINT8> m_bg1_scroll;
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	required_device<cpu_device> m_subcpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
};


// ======================> flower_sound_device


/* this structure defines the parameters for a channel */
struct flower_sound_channel
{
	UINT32 start;
	UINT32 pos;
	UINT16 freq;
	UINT8 volume;
	UINT8 voltab;
	UINT8 oneshot;
	UINT8 active;
	UINT8 effect;
	UINT32 ecount;

};

class flower_sound_device : public device_t, public device_sound_interface
{
public:
	flower_sound_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
	~flower_sound_device() {}

	enum
	{
	TIMER_CLOCK_EFFECT
	};

	DECLARE_WRITE8_MEMBER( sound1_w );
	DECLARE_WRITE8_MEMBER( sound2_w );

protected:
	// device-level overrides
	virtual void device_config_complete() override;
	virtual void device_start() override;
	virtual void device_reset() override;

	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;

	// sound stream update overrides
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

private:
	void make_mixer_table(int voices, int gain);

	// internal state
	emu_timer *m_effect_timer;

	/* data about the sound system */
	flower_sound_channel m_channel_list[8];
	flower_sound_channel *m_last_channel;

	/* global sound parameters */
	const UINT8 *m_sample_rom;
	const UINT8 *m_volume_rom;
	sound_stream * m_stream;

	/* mixer tables and internal buffers */
	std::unique_ptr<INT16[]> m_mixer_table;
	INT16 *m_mixer_lookup;
	std::unique_ptr<short[]> m_mixer_buffer;

	UINT8 m_soundregs1[0x40];
	UINT8 m_soundregs2[0x40];

};

extern const device_type FLOWER;

READ8_MEMBER( flower_state::soundlatch_byte_r )
{
	return m_soundlatch;
}

WRITE8_MEMBER(flower_state::flower_maincpu_irq_ack)
{
	m_maincpu->set_input_line(0, CLEAR_LINE);
}

WRITE8_MEMBER(flower_state::flower_subcpu_irq_ack)
{
	m_subcpu->set_input_line(0, CLEAR_LINE);
}

WRITE8_MEMBER(flower_state::flower_soundcpu_irq_ack)
{
	m_audiocpu->set_input_line(0, CLEAR_LINE);
}

WRITE8_MEMBER(flower_state::flower_coin_counter_w)
{
	//coin_counter_w(machine(), 0, data & 1);
}

WRITE8_MEMBER(flower_state::flower_coin_lockout_w)
{
	//coin_lockout_global_w(machine(), ~data & 1);
}

WRITE8_MEMBER(flower_state::sound_command_w)
{
	m_soundlatch = data;
	//soundlatch_byte_w(space, 0, data);

	if (*m_sn_nmi_enable & 1)
		m_audiocpu->set_input_line(INPUT_LINE_NMI, PULSE_LINE);
}

static ADDRESS_MAP_START( flower_cpu1_2, AS_PROGRAM, 8, flower_state )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0xa000, 0xa000) AM_WRITE(flower_coin_lockout_w)
	AM_RANGE(0xa001, 0xa001) AM_WRITE(flower_flipscreen_w)
	AM_RANGE(0xa002, 0xa002) AM_WRITE(flower_maincpu_irq_ack)
	AM_RANGE(0xa003, 0xa003) AM_WRITE(flower_subcpu_irq_ack)
	AM_RANGE(0xa004, 0xa004) AM_WRITE(flower_coin_counter_w)
	AM_RANGE(0xa005, 0xa005) AM_WRITENOP // subcpu nmi (unused)
	AM_RANGE(0xa100, 0xa100) AM_READ_PORT("IN0CPU1")
	AM_RANGE(0xa101, 0xa101) AM_READ_PORT("IN1CPU1")
	AM_RANGE(0xa102, 0xa102) AM_READ_PORT("IN0CPU0")
	AM_RANGE(0xa103, 0xa103) AM_READ_PORT("IN1CPU0")
	AM_RANGE(0xa400, 0xa400) AM_WRITE(sound_command_w)
	AM_RANGE(0xc000, 0xddff) AM_RAM AM_SHARE("mainram1")
	AM_RANGE(0xde00, 0xdfff) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0xe000, 0xe7ff) AM_RAM_WRITE(flower_textram_w) AM_SHARE("textram")
	AM_RANGE(0xe000, 0xefff) AM_RAM AM_SHARE("mainram2") // only cleared?
	AM_RANGE(0xf000, 0xf1ff) AM_RAM_WRITE(flower_bg0ram_w) AM_SHARE("bg0ram")
	AM_RANGE(0xf200, 0xf200) AM_RAM AM_SHARE("bg0_scroll")
	AM_RANGE(0xf800, 0xf9ff) AM_RAM_WRITE(flower_bg1ram_w) AM_SHARE("bg1ram")
	AM_RANGE(0xfa00, 0xfa00) AM_RAM AM_SHARE("bg1_scroll")
ADDRESS_MAP_END

static ADDRESS_MAP_START( flower_sound_cpu, AS_PROGRAM, 8, flower_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x4000) AM_WRITE(flower_soundcpu_irq_ack)
	AM_RANGE(0x4001, 0x4001) AM_WRITEONLY AM_SHARE("sn_nmi_enable")
	AM_RANGE(0x6000, 0x6000) AM_READ(soundlatch_byte_r)
	AM_RANGE(0x8000, 0x803f) AM_DEVWRITE("flower", flower_sound_device, sound1_w)
	AM_RANGE(0xa000, 0xa03f) AM_DEVWRITE("flower", flower_sound_device, sound2_w)
	AM_RANGE(0xc000, 0xc7ff) AM_RAM
ADDRESS_MAP_END


INPUT_CHANGED_MEMBER(flower_state::coin_inserted)
{
	m_maincpu->set_input_line(INPUT_LINE_NMI, newval ? CLEAR_LINE : ASSERT_LINE);
}

static INPUT_PORTS_START( flower )
	PORT_START("IN0CPU0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 ) PORT_CHANGED_MEMBER(DEVICE_SELF, flower_state,coin_inserted, 0)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_START1  )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_START2 )
	PORT_DIPNAME( 0x08, 0x08, "Energy Decrease" )       PORT_DIPLOCATION("SW2:4")
	PORT_DIPSETTING(    0x08, "Slow" )
	PORT_DIPSETTING(    0x00, "Fast" )
	PORT_DIPNAME( 0x10, 0x10, "Invulnerability (Cheat)") PORT_DIPLOCATION("SW2:5")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "Keep Weapons When Destroyed" ) PORT_DIPLOCATION("SW2:6") // check code at 0x74a2
	PORT_DIPSETTING(    0x20, DEF_STR( No ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Yes ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Difficulty ) )   PORT_DIPLOCATION("SW2:7")       // "Enemy Bullets"
	PORT_DIPSETTING(    0x40, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x80, 0x80, "Shot Range" )            PORT_DIPLOCATION("SW2:8")       // check code at 0x75f9
	PORT_DIPSETTING(    0x80, "Short" )
	PORT_DIPSETTING(    0x00, "Long" )

	PORT_START("IN1CPU0")
	PORT_DIPNAME( 0x07, 0x05, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:1,2,3")
	PORT_DIPSETTING(    0x07, "1" )
	PORT_DIPSETTING(    0x06, "2" )
	PORT_DIPSETTING(    0x05, "3" )
	PORT_DIPSETTING(    0x04, "4" )
	PORT_DIPSETTING(    0x03, "5" )
	PORT_DIPSETTING(    0x02, "6" )
	PORT_DIPSETTING(    0x01, "7" )
	PORT_DIPSETTING(    0x00, "Infinite (Cheat)")
	PORT_DIPNAME( 0x18, 0x18, DEF_STR( Coinage ) )      PORT_DIPLOCATION("SW1:4,5")
	PORT_DIPSETTING(    0x00, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x18, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x10, DEF_STR( 1C_2C ) )
	PORT_DIPNAME( 0x20, 0x00, DEF_STR( Cabinet ) )      PORT_DIPLOCATION("SW1:6")       // check code at 0x759f
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Demo_Sounds ) )  PORT_DIPLOCATION("SW1:7")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Bonus_Life ) )   PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x80, "30k, then every 50k" )
	PORT_DIPSETTING(    0x00, "50k, then every 80k" )

	PORT_START("IN0CPU1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("P1 Laser")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("P1 Missile")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("P1 Cutter")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("IN1CPU1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_COCKTAIL PORT_NAME("P2 Laser")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_COCKTAIL PORT_NAME("P2 Missile")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_COCKTAIL PORT_NAME("P2 Cutter")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )
INPUT_PORTS_END

static const gfx_layout flower_charlayout =
{
	8,8,
	RGN_FRAC(1,1),
	2,
	{ 0, 4 },
	{ STEP4(0,1), STEP4(8,1) },
	{ STEP8(0,16) },
	8*8*2
};

static const gfx_layout flower_tilelayout =
{
	16,16,
	RGN_FRAC(1,2),
	4,
	{ 0, 4, RGN_FRAC(1,2), RGN_FRAC(1,2)+4 },
	{ STEP4(0,1), STEP4(8,1), STEP4(8*8*2,1), STEP4(8*8*2+8,1) },
	{ STEP8(0,16), STEP8(8*8*4,16) },
	16*16*2
};

static GFXDECODE_START( flower )
	GFXDECODE_ENTRY( "gfx1", 0, flower_charlayout, 0,  64 )
	GFXDECODE_ENTRY( "gfx2", 0, flower_tilelayout, 0,  16 )
	GFXDECODE_ENTRY( "gfx3", 0, flower_tilelayout, 0,  16 )
GFXDECODE_END

void flower_state::draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect )
{
	gfx_element *gfx = m_gfxdecode->gfx(1);
	UINT8 *source = m_spriteram + 0x200;
	UINT8 *finish = source - 0x200;

	source -= 8;

	while( source>=finish )
	{
		int xblock,yblock;
		int sy = 256-32-source[0]+1;
		int sx = (source[4]|(source[5]<<8))-55;
		int code = source[1] & 0x3f;
		int color = (source[6]>>4);

		/*
		    Byte 0: Y
		    Byte 1:
		        0x80 - FlipY
		        0x40 - FlipX
		        0x3f - Tile
		    Byte 2:
		        0x08 - Tile MSB
		        0x01 - Tile MSB
		    Byte 3:
		        0x07 - X Zoom
		        0x08 - X Size
		        0x70 - Y Zoom
		        0x80 - Y Size
		    Byte 4: X LSB
		    Byte 5: X MSB
		    Byte 6:
		        0xf0 - Colour
		*/

		int flipy = source[1] & 0x80;
		int flipx = source[1] & 0x40;

		int size = source[3];

		int xsize = ((size & 0x08)>>3);
		int ysize = ((size & 0x80)>>7);

		xsize++;
		ysize++;

		if (ysize==2) sy -= 16;

		code |= ((source[2] & 0x01) << 6);
		code |= ((source[2] & 0x08) << 4);

		if(flip_screen())
		{
			flipx = !flipx;
			flipy = !flipy;
			sx = sx+16;
			sy = 250-sy;

			if (ysize==2) sy += 16;
		}

		for (xblock = 0; xblock<xsize; xblock++)
		{
			int xoffs=!flipx ? (xblock*8) : ((xsize-xblock-1)*8);
			int zoomx=((size&7)+1)<<13;
			int zoomy=((size&0x70)+0x10)<<9;
			int xblocksizeinpixels=(zoomx*16)>>16;
			int yblocksizeinpixels=(zoomy*16)>>16;

			for (yblock = 0; yblock<ysize; yblock++)
			{
				int yoffs=!flipy ? yblock : (ysize-yblock-1);
				int sxoffs=(16-xblocksizeinpixels)/2;
				int syoffs=(16-yblocksizeinpixels)/2;
				if (xblock) sxoffs+=xblocksizeinpixels;
				if (yblock) syoffs+=yblocksizeinpixels;

				gfx->zoom_transpen(bitmap,cliprect,
						code+yoffs+xoffs,
						color,
						flipx,flipy,
						sx+sxoffs,sy+syoffs,
						zoomx,zoomy,15);
			}
		}
		source -= 8;
	}

}

TILE_GET_INFO_MEMBER(flower_state::get_bg0_tile_info)
{
	int code = m_bg0ram[tile_index];
	int color = m_bg0ram[tile_index+0x100];
	/* Todo - may be tile flip bits? */

	SET_TILE_INFO_MEMBER(2, code, color>>4, 0);
}

TILE_GET_INFO_MEMBER(flower_state::get_bg1_tile_info)
{
	int code = m_bg1ram[tile_index];
	int color = m_bg1ram[tile_index+0x100];
	/* Todo - may be tile flip bits? */

	SET_TILE_INFO_MEMBER(2, code, color>>4, 0);
}

TILE_GET_INFO_MEMBER(flower_state::get_text_tile_info)
{
	int code = m_textram[tile_index];
	int color = m_textram[tile_index+0x400];
	/* Todo - may be tile flip bits? */

	SET_TILE_INFO_MEMBER(0, code, color>>2, 0);
}

void flower_state::video_start()
{
	m_bg0_tilemap        = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(flower_state::get_bg0_tile_info),this), TILEMAP_SCAN_ROWS,16,16,16,16);
	m_bg1_tilemap        = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(flower_state::get_bg1_tile_info),this), TILEMAP_SCAN_ROWS,16,16,16,16);
	m_text_tilemap       = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(flower_state::get_text_tile_info),this),TILEMAP_SCAN_ROWS, 8, 8,32,32);
	m_text_right_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(flower_state::get_text_tile_info),this),TILEMAP_SCAN_COLS, 8, 8, 2,32);

	m_bg1_tilemap->set_transparent_pen(15);
	m_text_tilemap->set_transparent_pen(3);
	m_text_right_tilemap->set_transparent_pen(3);

	m_text_tilemap->set_scrolly(0, 16);
	m_text_right_tilemap->set_scrolly(0, 16);
}

UINT32 flower_state::screen_update_flower(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	rectangle myclip = cliprect;

	m_bg0_tilemap->set_scrolly(0, m_bg0_scroll[0]+16);
	m_bg1_tilemap->set_scrolly(0, m_bg1_scroll[0]+16);

	m_bg0_tilemap->draw(screen, bitmap, cliprect, 0,0);
	m_bg1_tilemap->draw(screen, bitmap, cliprect, 0,0);

	draw_sprites(bitmap,cliprect);

	if(flip_screen())
	{
		myclip.min_x = cliprect.min_x;
		myclip.max_x = cliprect.min_x + 15;
	}
	else
	{
		myclip.min_x = cliprect.max_x - 15;
		myclip.max_x = cliprect.max_x;
	}

	m_text_tilemap->draw(screen, bitmap, cliprect, 0,0);
	m_text_right_tilemap->draw(screen, bitmap, myclip, 0,0);
	return 0;
}

WRITE8_MEMBER(flower_state::flower_textram_w)
{
	m_textram[offset] = data;
	m_text_tilemap->mark_tile_dirty(offset);
	m_text_right_tilemap->mark_all_dirty();
}

WRITE8_MEMBER(flower_state::flower_bg0ram_w)
{
	m_bg0ram[offset] = data;
	m_bg0_tilemap->mark_tile_dirty(offset & 0x1ff);
}

WRITE8_MEMBER(flower_state::flower_bg1ram_w)
{
	m_bg1ram[offset] = data;
	m_bg1_tilemap->mark_tile_dirty(offset & 0x1ff);
}

WRITE8_MEMBER(flower_state::flower_flipscreen_w)
{
	flip_screen_set(data);
}


static MACHINE_CONFIG_START( flower, flower_state )

	/* basic machine hardware */
	// clock divider (of all cpus) is unknown. /6 (3.072 MHz) is too slow
	// cpus are Z80 "A" type, official maximum speed of 4 MHz, but 4.6 MHz has been proven to work in practice
	MCFG_CPU_ADD("maincpu", Z80,XTAL_18_432MHz/4)
	MCFG_CPU_PROGRAM_MAP(flower_cpu1_2)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", flower_state, irq0_line_hold)

	MCFG_CPU_ADD("subcpu", Z80,XTAL_18_432MHz/4)
	MCFG_CPU_PROGRAM_MAP(flower_cpu1_2)
	MCFG_CPU_PERIODIC_INT_DRIVER(flower_state, irq0_line_hold, 120) // controls game speed? irqsource and frequency unknown

	MCFG_CPU_ADD("audiocpu", Z80,XTAL_18_432MHz/4)
	MCFG_CPU_PROGRAM_MAP(flower_sound_cpu)
	MCFG_CPU_PERIODIC_INT_DRIVER(flower_state, irq0_line_hold, 90) // controls music speed. irqsource and frequency unknown, same as subcpu perhaps?

	// tight sync, slowdowns otherwise
//  MCFG_QUANTUM_PERFECT_CPU("maincpu")
	MCFG_QUANTUM_TIME(attotime::from_hz(6000))

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60) // ?
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MCFG_SCREEN_SIZE(34*8, 33*8)
	MCFG_SCREEN_VISIBLE_AREA(0*8, 34*8-1, 0*8, 28*8-1)
	MCFG_SCREEN_UPDATE_DRIVER(flower_state, screen_update_flower)
	MCFG_SCREEN_PALETTE("palette")

	MCFG_GFXDECODE_ADD("gfxdecode", "palette", flower)
	MCFG_PALETTE_ADD_RRRRGGGGBBBB_PROMS("palette", 256)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")

	MCFG_SOUND_ADD("flower", FLOWER, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)
MACHINE_CONFIG_END


ROM_START( flower ) /* Komax version */
	ROM_REGION( 0x10000, "maincpu", 0 ) /* main cpu */
	ROM_LOAD( "1.5j",   0x0000, 0x8000, CRC(a4c3af78) SHA1(d149b0e0d82318273dd9cc5a143b175cdc818d0d) )

	ROM_REGION( 0x10000, "subcpu", 0 ) /* sub cpu */
	ROM_LOAD( "2.5f",   0x0000, 0x8000, CRC(7c7ee2d8) SHA1(1e67bfe0f3585be5a6e6719ccf9db764bafbcb01) )

	ROM_REGION( 0x10000, "audiocpu", 0 ) /* sound cpu */
	ROM_LOAD( "3.d9",   0x0000, 0x4000, CRC(8866c2b0) SHA1(d00f31994673e8087a1406f98e8832d07cedeb66) ) // 1xxxxxxxxxxxxx = 0xFF

	ROM_REGION( 0x2000, "gfx1", ROMREGION_INVERT ) /* tx layer */
	ROM_LOAD( "10.13e", 0x0000, 0x2000, CRC(62f9b28c) SHA1(d57d06b99e72a4f68f197a5b6c042c926cc70ca0) ) // FIRST AND SECOND HALF IDENTICAL

	ROM_REGION( 0x8000, "gfx2", ROMREGION_INVERT ) /* sprites */
	ROM_LOAD( "14.19e", 0x0000, 0x2000, CRC(11b491c5) SHA1(be1c4a0fbe8fd4e124c21e0f700efa0428376691) )
	ROM_LOAD( "13.17e", 0x2000, 0x2000, CRC(ea743986) SHA1(bbef4fd0f7d21cc89a52061fa50d7c2ea37287bd) )
	ROM_LOAD( "12.16e", 0x4000, 0x2000, CRC(e3779f7f) SHA1(8e12d06b3cdc2fcb7b77cc35f8eca45544cc4873) )
	ROM_LOAD( "11.14e", 0x6000, 0x2000, CRC(8801b34f) SHA1(256059fcd16b21e076db1c18fd9669128df1d658) )

	ROM_REGION( 0x8000, "gfx3", ROMREGION_INVERT ) /* bg layers */
	ROM_LOAD( "8.10e",  0x0000, 0x2000, CRC(f85eb20f) SHA1(699edc970c359143dee6de2a97cc2a552454785b) )
	ROM_LOAD( "6.7e",   0x2000, 0x2000, CRC(3e97843f) SHA1(4e4e5625dbf78eca97536b1428b2e49ad58c618f) )
	ROM_LOAD( "9.12e",  0x4000, 0x2000, CRC(f1d9915e) SHA1(158e1cc8c402f9ae3906363d99f2b25c94c64212) )
	ROM_LOAD( "15.9e",  0x6000, 0x2000, CRC(1cad9f72) SHA1(c38dbea266246ed4d47d12bdd8f9fae22a5f8bb8) )

	ROM_REGION( 0x8000, "sound1", 0 )
	ROM_LOAD( "4.12a",  0x0000, 0x8000, CRC(851ed9fd) SHA1(5dc048b612e45da529502bf33d968737a7b0a646) )  /* 8-bit samples */

	ROM_REGION( 0x4000, "sound2", 0 )
	ROM_LOAD( "5.16a",  0x0000, 0x4000, CRC(42fa2853) SHA1(cc1e8b8231d6f27f48b05d59390e93ea1c1c0e4c) )  /* volume tables? */

	ROM_REGION( 0x300, "proms", 0 ) /* RGB proms */
	ROM_LOAD( "82s129.k1",  0x0200, 0x0100, CRC(d311ed0d) SHA1(1d530c874aecf93133d610ab3ce668548712913a) ) // r
	ROM_LOAD( "82s129.k2",  0x0100, 0x0100, CRC(ababb072) SHA1(a9d46d12534c8662c6b54df94e96907f3a156968) ) // g
	ROM_LOAD( "82s129.k3",  0x0000, 0x0100, CRC(5aab7b41) SHA1(8d44639c7c9f1ba34fe9c4e74c8a38b6453f7ac0) ) // b

	ROM_REGION( 0x0520, "user1", 0 ) /* Other proms, (zoom table?) */
	ROM_LOAD( "82s147.d7",  0x0000, 0x0200, CRC(f0dbb2a7) SHA1(03cd8fd41d6406894c6931e883a9ac6a4a4effc9) )
	ROM_LOAD( "82s147.j18", 0x0200, 0x0200, CRC(d7de0860) SHA1(5d3d8c5476b1edffdacde09d592c64e78d2b90c0) )
	ROM_LOAD( "82s123.k7",  0x0400, 0x0020, CRC(ea9c65e4) SHA1(1bdd77a7f3ef5f8ec4dbb9524498c0c4a356f089) )
	ROM_LOAD( "82s129.a1",  0x0420, 0x0100, CRC(c8dad3fc) SHA1(8e852efac70223d02e45b20ed8a12e38c5010a78) )
ROM_END

ROM_START( flowerj ) /* Sega/Alpha version.  Sega game number 834-5998 */
	ROM_REGION( 0x10000, "maincpu", 0 ) /* main cpu */
	ROM_LOAD( "1",   0x0000, 0x8000, CRC(63a2ef04) SHA1(0770f5a18d58b780abcda7e000c2a5e46f96d319) ) // hacked? "AKINA.N" changed to "JUKYUNG"

	ROM_REGION( 0x10000, "subcpu", 0 ) /* sub cpu */
	ROM_LOAD( "2.5f",   0x0000, 0x8000, CRC(7c7ee2d8) SHA1(1e67bfe0f3585be5a6e6719ccf9db764bafbcb01) )

	ROM_REGION( 0x10000, "audiocpu", 0 ) /* sound cpu */
	ROM_LOAD( "3.d9",   0x0000, 0x4000, CRC(8866c2b0) SHA1(d00f31994673e8087a1406f98e8832d07cedeb66) ) // 1xxxxxxxxxxxxx = 0xFF

	ROM_REGION( 0x2000, "gfx1", ROMREGION_INVERT ) /* tx layer */
	ROM_LOAD( "10.13e", 0x0000, 0x2000, CRC(62f9b28c) SHA1(d57d06b99e72a4f68f197a5b6c042c926cc70ca0) ) // FIRST AND SECOND HALF IDENTICAL

	ROM_REGION( 0x8000, "gfx2", ROMREGION_INVERT ) /* sprites */
	ROM_LOAD( "14.19e", 0x0000, 0x2000, CRC(11b491c5) SHA1(be1c4a0fbe8fd4e124c21e0f700efa0428376691) )
	ROM_LOAD( "13.17e", 0x2000, 0x2000, CRC(ea743986) SHA1(bbef4fd0f7d21cc89a52061fa50d7c2ea37287bd) )
	ROM_LOAD( "12.16e", 0x4000, 0x2000, CRC(e3779f7f) SHA1(8e12d06b3cdc2fcb7b77cc35f8eca45544cc4873) )
	ROM_LOAD( "11.14e", 0x6000, 0x2000, CRC(8801b34f) SHA1(256059fcd16b21e076db1c18fd9669128df1d658) )

	ROM_REGION( 0x8000, "gfx3", ROMREGION_INVERT ) /* bg layers */
	ROM_LOAD( "8.10e",  0x0000, 0x2000, CRC(f85eb20f) SHA1(699edc970c359143dee6de2a97cc2a552454785b) )
	ROM_LOAD( "6.7e",   0x2000, 0x2000, CRC(3e97843f) SHA1(4e4e5625dbf78eca97536b1428b2e49ad58c618f) )
	ROM_LOAD( "9.12e",  0x4000, 0x2000, CRC(f1d9915e) SHA1(158e1cc8c402f9ae3906363d99f2b25c94c64212) )
	ROM_LOAD( "7.9e",   0x6000, 0x2000, CRC(e350f36c) SHA1(f97204dc95b4000c268afc053a2333c1629e07d8) )

	ROM_REGION( 0x8000, "sound1", 0 )
	ROM_LOAD( "4.12a",  0x0000, 0x8000, CRC(851ed9fd) SHA1(5dc048b612e45da529502bf33d968737a7b0a646) )  /* 8-bit samples */

	ROM_REGION( 0x4000, "sound2", 0 )
	ROM_LOAD( "5.16a",  0x0000, 0x4000, CRC(42fa2853) SHA1(cc1e8b8231d6f27f48b05d59390e93ea1c1c0e4c) )  /* volume tables? */

	ROM_REGION( 0x300, "proms", 0 ) /* RGB proms */
	ROM_LOAD( "82s129.k1",  0x0200, 0x0100, CRC(d311ed0d) SHA1(1d530c874aecf93133d610ab3ce668548712913a) ) // r
	ROM_LOAD( "82s129.k2",  0x0100, 0x0100, CRC(ababb072) SHA1(a9d46d12534c8662c6b54df94e96907f3a156968) ) // g
	ROM_LOAD( "82s129.k3",  0x0000, 0x0100, CRC(5aab7b41) SHA1(8d44639c7c9f1ba34fe9c4e74c8a38b6453f7ac0) ) // b

	ROM_REGION( 0x0520, "user1", 0 ) /* Other proms, (zoom table?) */
	ROM_LOAD( "82s147.d7",  0x0000, 0x0200, CRC(f0dbb2a7) SHA1(03cd8fd41d6406894c6931e883a9ac6a4a4effc9) )
	ROM_LOAD( "82s147.j18", 0x0200, 0x0200, CRC(d7de0860) SHA1(5d3d8c5476b1edffdacde09d592c64e78d2b90c0) )
	ROM_LOAD( "82s123.k7",  0x0400, 0x0020, CRC(ea9c65e4) SHA1(1bdd77a7f3ef5f8ec4dbb9524498c0c4a356f089) )
	ROM_LOAD( "82s129.a1",  0x0420, 0x0100, CRC(c8dad3fc) SHA1(8e852efac70223d02e45b20ed8a12e38c5010a78) )
ROM_END


GAME( 1986, flower,  0,      flower, flower, driver_device, 0, ROT0, "Clarue (Komax license)", "Flower (US)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 1986, flowerj, flower, flower, flower, driver_device, 0, ROT0, "Clarue (Sega / Alpha Denshi Co. license)", "Flower (Japan)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE)


/* Clarue Flower sound driver.
Initial version was based on the Wiping sound driver, which was based on the old namco.c sound driver.

TODO:
- timing (see main driver file), but also of samplerate and effects counter
- what do the unknown bits in soundregs do?
- Are channel effects correct? It's currently mostly guesswork, the pitch effects sound pretty convincing though.
  Considering that the game sound hardware isn't complicated (no dedicated soundchip) these bits are possibly
  for something way simpler, such as a length counter. PCB sound recordings would be useful!

*/

#define FLOWER_VERBOSE      0       // show register writes

#define MIXER_SAMPLERATE    48000   /* ? (native freq is probably in the MHz range) */
#define MIXER_DEFGAIN       48


const device_type FLOWER = &device_creator<flower_sound_device>;

flower_sound_device::flower_sound_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock)
	: device_t(mconfig, FLOWER, "Flower Audio Custom", tag, owner, clock, "flower_sound", __FILE__),
		device_sound_interface(mconfig, *this)
{
}

//-------------------------------------------------
//  device_config_complete - perform any
//  operations now that the configuration is
//  complete
//-------------------------------------------------

void flower_sound_device::device_config_complete()
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void flower_sound_device::device_start()
{
	flower_sound_channel *voice;

	m_effect_timer = timer_alloc(TIMER_CLOCK_EFFECT);
	m_stream = machine().sound().stream_alloc(*this, 0, 1, MIXER_SAMPLERATE);
	m_mixer_buffer = std::make_unique<short[]>(MIXER_SAMPLERATE);
	make_mixer_table(8, MIXER_DEFGAIN);

	/* extract globals from the interface */
	m_last_channel = m_channel_list + 8;

	m_sample_rom = machine().root_device().memregion("sound1")->base();
	m_volume_rom = machine().root_device().memregion("sound2")->base();

	/* register for savestates */
	for (int i = 0; i < 8; i++)
	{
		voice = &m_channel_list[i];

		save_item(NAME(voice->freq), i+1);
		save_item(NAME(voice->pos), i+1);
		save_item(NAME(voice->volume), i+1);
		save_item(NAME(voice->voltab), i+1);
		save_item(NAME(voice->effect), i+1);
		save_item(NAME(voice->ecount), i+1);
		save_item(NAME(voice->oneshot), i+1);
		save_item(NAME(voice->active), i+1);
		save_item(NAME(voice->start), i+1);
	}
}

//-------------------------------------------------
//  device_reset - device-specific reset
//-------------------------------------------------

void flower_sound_device::device_reset()
{
	flower_sound_channel *voice;
	attotime period;

	/* reset effect timer, period is unknown/guessed */
	period = attotime::from_hz(MIXER_SAMPLERATE / 256);
	m_effect_timer->adjust(period, 0, period);

	/* reset all the voices */
	for (auto & elem : m_channel_list)
	{
		voice = &elem;

		voice->freq = 0;
		voice->pos = 0;
		voice->volume = 0;
		voice->voltab = 0;
		voice->effect = 0;
		voice->ecount = 0;
		voice->oneshot = 1;
		voice->active = 0;
		voice->start = 0;
	}

	memset(m_soundregs1, 0, 0x40);
	memset(m_soundregs2, 0, 0x40);
}

/* build a table to divide by the number of voices; gain is specified as gain*16 */
void flower_sound_device::make_mixer_table(int voices, int gain)
{
	int count = voices * 128;

	/* allocate memory */
	m_mixer_table = std::make_unique<INT16[]>(256 * voices);

	/* find the middle of the table */
	m_mixer_lookup = m_mixer_table.get() + (128 * voices);

	/* fill in the table - 16 bit case */
	for (int i = 0; i < count; i++)
	{
		int val = i * gain * 16 / voices;
		if (val > 32767) val = 32767;
		m_mixer_lookup[ i] = val;
		m_mixer_lookup[-i] =-val;
	}
}


/* clock sound channel effect counters */
void flower_sound_device::device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr)
{
	switch (id)
	{
		case TIMER_CLOCK_EFFECT:
		flower_sound_channel *voice;
		m_stream->update();

		for (voice = m_channel_list; voice < m_last_channel; voice++)
			voice->ecount += (voice->ecount < (1<<22));
		break;

		default:
			assert_always(FALSE, "Unknown id in flower_sound_device::device_timer");
	}
}

/********************************************************************************/

#if FLOWER_VERBOSE
static int r_numwrites[2][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
void flower_sound_device::show_soundregs()
{
	int set,reg,chan;
	char text[0x100];
	char message[0x1000] = {0};
	UINT8 *base = m_soundregs1;

	for (set=0;set<2;set++)
	{
		for (reg=0;reg<8;reg++)
		{
			sprintf(text,"R%d%d:",set+1,reg);
			strcat(message,text);

			for (chan=0;chan<8;chan++)
			{
				sprintf(text," %02X",base[reg + 8*chan]);
				strcat(message,text);
			}
			sprintf(text," - %07d\n",r_numwrites[set][reg]);
			strcat(message,text);
		}
		strcat(message,"\n");
		base = m_soundregs2;
	}
	popmessage("%s",message);
}
#endif // FLOWER_VERBOSE


/* register functions (preliminary):
offset: cccrrr      c=channel, r=register

set 1:
R  76543210
0  xxxxxxxx         frequency (which nibble?)
1  xxxxxxxx         *
2  xxxxxxxx         *
3  xxxxxxxx         *
4  ...x....         one-shot sample
5  ...x....         ??? same as R4?
6  ........         unused
7  xxxx....         volume

set 2:
R  76543210
0  ....xxxx         start address
1  ....xxxx         *
2  ....xxxx         *
3  ....xxxx         *
4  xxxx             assume it's channel pitch/volume effects
       xxxx         start address
5  x...             ???
       xxxx         start address
6  ........         unused
7  ......xx         volume table + start trigger

*/

WRITE8_MEMBER( flower_sound_device::sound1_w )
{
	flower_sound_channel *voice = &m_channel_list[offset >> 3 & 7];
	int c = offset & 0xf8;
	UINT8 *base1 = m_soundregs1;
//  UINT8 *base2 = m_soundregs2;

	m_stream->update();
	base1[offset] = data;
#if FLOWER_VERBOSE
	r_numwrites[0][offset & 7]++;
	show_soundregs();
#endif

	// recompute voice parameters
	voice->freq = (base1[c+2] & 0xf) << 12 | (base1[c+3] & 0xf) << 8 | (base1[c+0] & 0xf) << 4 | (base1[c+1] & 0xf);
	voice->volume = base1[c+7] >> 4;
}

WRITE8_MEMBER( flower_sound_device::sound2_w )
{
	flower_sound_channel *voice = &m_channel_list[offset >> 3 & 7];
	int i, c = offset & 0xf8;
	UINT8 *base1 = m_soundregs1;
	UINT8 *base2 = m_soundregs2;

	m_stream->update();
	base2[offset] = data;
#if FLOWER_VERBOSE
	r_numwrites[1][offset & 7]++;
	show_soundregs();
#endif

	// reg 7 is start trigger!
	if ((offset & 7) != 7)
		return;

	voice->voltab = (base2[c+7] & 3) << 4;
	voice->oneshot = (~base1[c+4] & 0x10) >> 4;
	voice->effect = base2[c+4] >> 4;
	voice->ecount = 0;
	voice->pos = 0;
	voice->active = 1;

	// full start address is 6 nibbles
	voice->start = 0;
	for (i = 5; i >= 0; i--)
		voice->start = (voice->start << 4) | (base2[c+i] & 0xf);
}


//-------------------------------------------------
//  sound_stream_update - handle a stream update
//-------------------------------------------------

void flower_sound_device::sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples)
{
	stream_sample_t *buffer = outputs[0];
	flower_sound_channel *voice;
	short *mix;
	int i;

	/* zap the contents of the mixer buffer */
	memset(m_mixer_buffer.get(), 0, samples * sizeof(short));

	/* loop over each voice and add its contribution */
	for (voice = m_channel_list; voice < m_last_channel; voice++)
	{
		int f = voice->freq;
		int v = voice->volume;

		if (!voice->active)
			continue;

		// effects
		// bit 0: volume slide down?
		if (voice->effect & 1 && !voice->oneshot)
		{
			// note: one-shot samples are fixed volume
			v -= (voice->ecount >> 4);
			if (v < 0) v = 0;
		}
		// bit 1: used often, but hard to figure out what for
		// bit 2: probably pitch slide
		if (voice->effect & 4)
		{
			f -= (voice->ecount << 7);
			if (f < 0) f = 0;
		}
		// bit 3: not used much, maybe pitch slide the other way?

		v |= voice->voltab;
		mix = m_mixer_buffer.get();

		for (i = 0; i < samples; i++)
		{
			// add sample
			if (voice->oneshot)
			{
				UINT8 sample = m_sample_rom[(voice->start + voice->pos) >> 7 & 0x7fff];
				if (sample == 0xff)
				{
					voice->active = 0;
					break;
				}
				else
					*mix++ += m_volume_rom[v << 8 | sample] - 0x80;
			}
			else
			{
				UINT8 sample = m_sample_rom[(voice->start >> 7 & 0x7e00) | (voice->pos >> 7 & 0x1ff)];
				*mix++ += m_volume_rom[v << 8 | sample] - 0x80;
			}

			// update counter
			voice->pos += f;
		}
	}

	/* mix it down */
	mix = m_mixer_buffer.get();
	for (i = 0; i < samples; i++)
		*buffer++ = m_mixer_lookup[*mix++];
}
