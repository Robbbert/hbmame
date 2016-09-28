/***************************************************************************

    Popper

    Omori Electric CAD (OEC) 1983

***************************************************************************/
#include "emu.h"
#include "cpu/z80/z80.h"
#include "sound/ay8910.h"
#include "video/resnet.h"

class popper_state : public driver_device
{
public:
	popper_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_ol_videoram(*this, "ol_videoram")
		, m_videoram(*this, "videoram")
		, m_ol_attribram(*this, "ol_attribram")
		, m_attribram(*this, "attribram")
		, m_spriteram(*this, "spriteram")
		, m_audiocpu(*this, "audiocpu")
		, m_maincpu(*this, "maincpu")
		, m_gfxdecode(*this, "gfxdecode")
		, m_screen(*this, "screen")
		, m_palette(*this, "palette")
		{ }

	DECLARE_READ8_MEMBER(popper_input_ports_r);
	DECLARE_READ8_MEMBER(popper_soundcpu_nmi_r);
	DECLARE_WRITE8_MEMBER(nmi_mask_w);
	DECLARE_WRITE8_MEMBER(popper_ol_videoram_w);
	DECLARE_WRITE8_MEMBER(popper_videoram_w);
	DECLARE_WRITE8_MEMBER(popper_ol_attribram_w);
	DECLARE_WRITE8_MEMBER(popper_attribram_w);
	DECLARE_WRITE8_MEMBER(popper_flipscreen_w);
	DECLARE_WRITE8_MEMBER(popper_e002_w);
	DECLARE_WRITE8_MEMBER(popper_gfx_bank_w);
	TILE_GET_INFO_MEMBER(get_popper_p123_tile_info);
	TILE_GET_INFO_MEMBER(get_popper_p0_tile_info);
	TILE_GET_INFO_MEMBER(get_popper_ol_p123_tile_info);
	TILE_GET_INFO_MEMBER(get_popper_ol_p0_tile_info);
	DECLARE_PALETTE_INIT(popper);
	INTERRUPT_GEN_MEMBER(vblank_irq);
	UINT32 screen_update_popper(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

private:

	tilemap_t *m_p123_tilemap;
	tilemap_t *m_p0_tilemap;
	tilemap_t *m_ol_p123_tilemap;
	tilemap_t *m_ol_p0_tilemap;
	INT32 m_flipscreen;
	INT32 m_e002;
	INT32 m_gfx_bank;
	rectangle m_tilemap_clip;
	UINT8 m_nmi_mask;
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	void draw_sprites( bitmap_ind16 &bitmap,const rectangle &cliprect );

	required_shared_ptr<UINT8> m_ol_videoram;
	required_shared_ptr<UINT8> m_videoram;
	required_shared_ptr<UINT8> m_ol_attribram;
	required_shared_ptr<UINT8> m_attribram;
	required_shared_ptr<UINT8> m_spriteram;
	required_device<cpu_device> m_audiocpu;
	required_device<cpu_device> m_maincpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;
};


/*************************************
 *
 *  Memory handlers
 *
 *************************************/

//e000                  e001                  e002                  e003
//76543210              76543210              76543210              76543210
//x-------  unused      x-------  unused      x-------  unused      x-------  unused
//-x------  dsw1:1      -x------  dsw1:2      -x------  dsw1:3      -x------  dsw1:4
//--x-----  unused      --x-----  unused      --x-----  unused      --x-----  unused
//---x----  dsw2:1      ---x----  dsw2:2      ---x----  dsw2:3      ---x----  dsw2:4
//----x---  service     ----xxxx  p1 udlr     ----x---  unused      ----xxxx  p2 udlr
//-----x--  coin a                            -----x--  coin b
//------x-  start 1                           ------x-  start 2
//-------x  p1 b1                             -------x  p2 b1
//
//e004                  e005                  e006                  e007
//x-------  dsw1:5      x-------  dsw1:6      x-------  dsw1:7      x-------  dsw1:8
//-x------  unused      -x------  unused      -x------  unused      -x------  unused
//--x-----  dsw2:5      --x-----  dsw2:6      --x-----  dsw2:7      --x-----  dsw2:8
//---xxxxx  unused      ---xxxxx  unused      ---xxxxx  unused      ---xxxxx  unused
//
//dsw1                  dsw2
//87654321              87654321
//xx------  extra       x-------  stop
//--xx----  poppers     -x------  clear (current level)
//----xx--  coin b      --x-----  upright (cabinet)
//------xx  coin a      ---x----  crt dir. (flip screen)
//                      ----x---  pass (unlimited lives)
//                      -----x--  free play
//                      ------x-  continue
//                      -------x  sound
READ8_MEMBER(popper_state::popper_input_ports_r)
{
	UINT8 data = 0;
	switch (offset)
	{
		//           player inputs        dsw1                           dsw2
		case 0: data = ioport("IN0")->read() | ((ioport("DSW1")->read() & 0x02) << 5) | ((ioport("DSW2")->read() & 0x01) << 4); break;
		case 1: data = ioport("IN1")->read() | ((ioport("DSW1")->read() & 0x01) << 6) | ((ioport("DSW2")->read() & 0x02) << 3); break;
		case 2: data = ioport("IN2")->read() | ((ioport("DSW1")->read() & 0x08) << 3) | ((ioport("DSW2")->read() & 0x04) << 2); break;
		case 3: data = ioport("IN3")->read() | ((ioport("DSW1")->read() & 0x04) << 4) | ((ioport("DSW2")->read() & 0x08) << 1); break;
		case 4: data = ((ioport("DSW1")->read() & 0x20) << 2) | ((ioport("DSW2")->read() & 0x10) << 1); break;
		case 5: data = ((ioport("DSW1")->read() & 0x10) << 3) | ((ioport("DSW2")->read() & 0x20) << 0); break;
		case 6: data = ((ioport("DSW1")->read() & 0x80) << 0) | ((ioport("DSW2")->read() & 0x40) >> 1); break;
		case 7: data = ((ioport("DSW1")->read() & 0x40) << 1) | ((ioport("DSW2")->read() & 0x80) >> 2); break;
	}
	return data;
}

READ8_MEMBER(popper_state::popper_soundcpu_nmi_r)
{
	m_audiocpu->set_input_line(INPUT_LINE_NMI, PULSE_LINE);
	return 0;
}

WRITE8_MEMBER(popper_state::nmi_mask_w)
{
	m_nmi_mask = data & 1;
}


/*************************************
 *
 *  Memory maps
 *
 *************************************/

static ADDRESS_MAP_START( popper_map, AS_PROGRAM, 8, popper_state )
	AM_RANGE(0x0000, 0x5fff) AM_ROM
	AM_RANGE(0xc000, 0xc1bf) AM_RAM
	AM_RANGE(0xc1c0, 0xc1ff) AM_RAM_WRITE(popper_ol_videoram_w) AM_SHARE("ol_videoram")
	AM_RANGE(0xc200, 0xc61f) AM_RAM_WRITE(popper_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0xc620, 0xc9bf) AM_RAM
	AM_RANGE(0xc9c0, 0xc9ff) AM_RAM_WRITE(popper_ol_attribram_w) AM_SHARE("ol_attribram")
	AM_RANGE(0xca00, 0xce1f) AM_RAM_WRITE(popper_attribram_w) AM_SHARE("attribram")
	AM_RANGE(0xce20, 0xcfff) AM_RAM
	AM_RANGE(0xd000, 0xd7ff) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0xd800, 0xdfff) AM_RAM AM_SHARE("share1")
	AM_RANGE(0xe000, 0xe007) AM_READ(popper_input_ports_r)
	AM_RANGE(0xe000, 0xe000) AM_WRITE(nmi_mask_w)
	AM_RANGE(0xe001, 0xe001) AM_WRITE(popper_flipscreen_w)
	AM_RANGE(0xe002, 0xe002) AM_WRITE(popper_e002_w)                //?? seems to be graphic related
	AM_RANGE(0xe003, 0xe003) AM_WRITE(popper_gfx_bank_w)
	AM_RANGE(0xe004, 0xe007) AM_WRITENOP                    //?? range cleared once when the SP is set
	AM_RANGE(0xe400, 0xe400) AM_READ(popper_soundcpu_nmi_r)
	AM_RANGE(0xf800, 0xf800) AM_READNOP                 //?? read once at startup
	AM_RANGE(0xfc00, 0xfc00) AM_READNOP                 //?? possibly watchdog
	AM_RANGE(0xffff, 0xffff) AM_READNOP
ADDRESS_MAP_END

static ADDRESS_MAP_START( popper_sound_map, AS_PROGRAM, 8, popper_state )
	AM_RANGE(0x0000, 0x0fff) AM_ROM
	AM_RANGE(0x8000, 0x8001) AM_DEVWRITE("ay1", ay8910_device, address_data_w)
	AM_RANGE(0x8002, 0x8002) AM_READNOP                 //?? all read once at startup and the
	AM_RANGE(0x8002, 0x8002) AM_WRITENOP                //?? same writes as 0x8000 (mostly)
	AM_RANGE(0x8003, 0x8003) AM_READNOP                 //?? result ignored, looks like part
	AM_RANGE(0xa000, 0xa001) AM_DEVWRITE("ay2", ay8910_device, address_data_w)
	AM_RANGE(0xa002, 0xa002) AM_READNOP                 //?? of AY8910 initialisation
	AM_RANGE(0xa002, 0xa002) AM_WRITENOP                //?? same writes as 0xa000
	AM_RANGE(0xd800, 0xdfff) AM_RAM AM_SHARE("share1")
ADDRESS_MAP_END

/*************************************
 *
 *  Input ports
 *
 *************************************/

static INPUT_PORTS_START( popper )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_COIN1 )         //ignored if held for 12 or more frames
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_SERVICE1 )

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_8WAY
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_8WAY
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_8WAY
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_8WAY

	PORT_START("IN2")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_COIN2 )         //ignored if held for 12 or more frames

	PORT_START("IN3")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_8WAY PORT_COCKTAIL

	PORT_START("DSW1")  /* FAKE DSW1 */
	PORT_DIPNAME( 0x03, 0x00, DEF_STR( Coin_A ) )       //SW1:1-2
	PORT_DIPSETTING(    0x02, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_2C ) )
	PORT_DIPNAME( 0x0c, 0x08, DEF_STR( Coin_B ) )       //SW1:3-4
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0x0c, DEF_STR( 1C_6C ) )
	PORT_DIPNAME( 0x30, 0x10, DEF_STR( Lives ) )        //SW1:5-6
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPSETTING(    0x10, "3" )
	PORT_DIPSETTING(    0x20, "4" )
	PORT_DIPSETTING(    0x30, "5" )
	PORT_DIPNAME( 0xc0, 0x00, DEF_STR( Bonus_Life ) )   //SW1:7-8
	PORT_DIPSETTING(    0x00, "20k, then every 70k" )
	PORT_DIPSETTING(    0x40, "30k, then every 70k" )
	PORT_DIPSETTING(    0x80, "40k, then every 70k" )
	PORT_DIPSETTING(    0xc0, "50k, then every 70k" )

	PORT_START("DSW2")  /* FAKE DSW2 */
	PORT_DIPNAME( 0x01, 0x01, DEF_STR( Demo_Sounds ) )  //SW2:1
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, DEF_STR( Allow_Continue ) )       //SW2:2 (stored in 0xd987, never read)
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x02, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, DEF_STR( Free_Play ) )    //SW2:3
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x04, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x00, "Pass (Unlimited Lives) (Cheat)") //SW2:4
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x08, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x00, DEF_STR( Flip_Screen ) )  //SW2:5
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x10, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Cabinet ) )      //SW2:6
	PORT_DIPSETTING(    0x20, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x40, 0x00, "Clear (Current Level) (Cheat)")  //SW2:7
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x00, "Stop" )                  //SW2:8
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x80, DEF_STR( On ) )
INPUT_PORTS_END

/*************************************
 *
 *  Graphics definitions
 *
 *************************************/

static const gfx_layout popper_charlayout =
{
	8,8,
	RGN_FRAC(2,2),
	2,
	{ 0, 4 },
	{ STEP4(8,1), STEP4(0,1) },
	{ STEP8(0,16) },
	16*8
};

static const gfx_layout popper_spritelayout =
{
	16,16,
	RGN_FRAC(1,2),
	2,
	{ 0, RGN_FRAC(1,2) },
	{ STEP8(8,1), STEP8(0,1) },
	{ STEP16(0, 16) },
	16*2*8
};

static GFXDECODE_START( popper )
	GFXDECODE_ENTRY( "gfx1", 0, popper_charlayout,   0, 16 )
	GFXDECODE_ENTRY( "gfx2", 0, popper_spritelayout, 0, 16 )
GFXDECODE_END

/*************************************
 *
 *  Machine driver
 *
 *************************************/

void popper_state::machine_start()
{
	save_item(NAME(m_flipscreen));
	save_item(NAME(m_e002));
	save_item(NAME(m_gfx_bank));
}

void popper_state::machine_reset()
{
	m_flipscreen = 0;
	m_e002 = 0;
	m_gfx_bank = 0;
}

INTERRUPT_GEN_MEMBER(popper_state::vblank_irq)
{
	if(m_nmi_mask)
		device.execute().set_input_line(INPUT_LINE_NMI, PULSE_LINE);
}


/***************************************************************************
 *
 * Color guns - from schematics
 *
 ***************************************************************************/

static const res_net_decode_info popper_decode_info =
{
	1,      // there may be two proms needed to construct color
	0,      // start at 0
	63, // end at 255
	//  R,   G,   B,
	{   0,   0,   0, },     // offsets
	{   0,   3,   6, },     // shifts
	{0x07,0x07,0x03, }      // masks
};

static const res_net_info popper_net_info =
{
	RES_NET_VCC_5V | RES_NET_VBIAS_5V | RES_NET_VIN_TTL_OUT,
	{
		{ RES_NET_AMP_NONE, 0, 0, 3, { 1000, 470, 220 } },
		{ RES_NET_AMP_NONE, 0, 0, 3, { 1000, 470, 220 } },
		{ RES_NET_AMP_NONE, 0, 0, 2, {  470, 220,   0 } }
	}
};

PALETTE_INIT_MEMBER(popper_state, popper)
{
	const UINT8 *color_prom = memregion("proms")->base();
	std::vector<rgb_t> rgb;

	compute_res_net_all(rgb, color_prom, popper_decode_info, popper_net_info);
	palette.set_pen_colors(0, rgb);
	palette.palette()->normalize_range(0, 63);
}

WRITE8_MEMBER(popper_state::popper_ol_videoram_w)
{
	m_ol_videoram[offset] = data;
	m_ol_p123_tilemap->mark_tile_dirty(offset);
	m_ol_p0_tilemap->mark_tile_dirty(offset);
}

WRITE8_MEMBER(popper_state::popper_videoram_w)
{
	m_videoram[offset] = data;
	m_p123_tilemap->mark_tile_dirty(offset);
	m_p0_tilemap->mark_tile_dirty(offset);
}

WRITE8_MEMBER(popper_state::popper_ol_attribram_w)
{
	m_ol_attribram[offset] = data;
	m_ol_p123_tilemap->mark_tile_dirty(offset);
	m_ol_p0_tilemap->mark_tile_dirty(offset);
}

WRITE8_MEMBER(popper_state::popper_attribram_w)
{
	m_attribram[offset] = data;
	m_p123_tilemap->mark_tile_dirty(offset);
	m_p0_tilemap->mark_tile_dirty(offset);
}

WRITE8_MEMBER(popper_state::popper_flipscreen_w)
{
	m_flipscreen = data;
	machine().tilemap().set_flip_all(m_flipscreen ? (TILEMAP_FLIPX | TILEMAP_FLIPY) : 0);

	if (m_flipscreen)
		m_tilemap_clip.min_x = m_tilemap_clip.max_x - 15;
	else
		m_tilemap_clip.max_x = 15;
}

WRITE8_MEMBER(popper_state::popper_e002_w)
{
	m_e002 = data;
}

WRITE8_MEMBER(popper_state::popper_gfx_bank_w)
{
	if (m_gfx_bank != data)
	{
		m_gfx_bank = data;
		machine().tilemap().mark_all_dirty();
	}
}

TILE_GET_INFO_MEMBER(popper_state::get_popper_p123_tile_info)
{
	UINT32 tile_number = m_videoram[tile_index];
	UINT8 attr = m_attribram[tile_index];
	tile_number += m_gfx_bank << 8;

	SET_TILE_INFO_MEMBER(0,
			tile_number,
			(attr & 0xf),
			0);
	tileinfo.group = (attr & 0x80) >> 7;
}

TILE_GET_INFO_MEMBER(popper_state::get_popper_p0_tile_info)
{
	UINT32 tile_number = m_videoram[tile_index];
	UINT8 attr = m_attribram[tile_index];
	tile_number += m_gfx_bank << 8;

	//pen 0 only in front if colour set as well
	tileinfo.group = (attr & 0x70) ? ((attr & 0x80) >> 7) : 0;

	SET_TILE_INFO_MEMBER(0,
			tile_number,
			((attr & 0x70) >> 4) + 8,
			0);
}

TILE_GET_INFO_MEMBER(popper_state::get_popper_ol_p123_tile_info)
{
	UINT32 tile_number = m_ol_videoram[tile_index];
	UINT8 attr  = m_ol_attribram[tile_index];
	tile_number += m_gfx_bank << 8;

	SET_TILE_INFO_MEMBER(0,
			tile_number,
			(attr & 0xf),
			0);
	tileinfo.group = (attr & 0x80) >> 7;
}

TILE_GET_INFO_MEMBER(popper_state::get_popper_ol_p0_tile_info)
{
	UINT32 tile_number = m_ol_videoram[tile_index];
	UINT8 attr = m_ol_attribram[tile_index];
	tile_number += m_gfx_bank << 8;

	//pen 0 only in front if colour set as well
	tileinfo.group = (attr & 0x70) ? ((attr & 0x80) >> 7) : 0;

	SET_TILE_INFO_MEMBER(0,
			tile_number,
			((attr & 0x70) >> 4) + 8,
			0);
}

void popper_state::video_start()
{
	m_p123_tilemap    = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(popper_state::get_popper_p123_tile_info),this),    TILEMAP_SCAN_COLS, 8, 8, 33, 32 );
	m_p0_tilemap      = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(popper_state::get_popper_p0_tile_info),this),      TILEMAP_SCAN_COLS, 8, 8, 33, 32);
	m_ol_p123_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(popper_state::get_popper_ol_p123_tile_info),this), TILEMAP_SCAN_COLS, 8, 8, 2, 32);
	m_ol_p0_tilemap   = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(popper_state::get_popper_ol_p0_tile_info),this),   TILEMAP_SCAN_COLS, 8, 8, 2, 32);

	m_p123_tilemap->set_transmask(0, 0x0f, 0x01);
	m_p123_tilemap->set_transmask(1, 0x01, 0x0f);
	m_p0_tilemap->set_transmask(0, 0x0f, 0x0e);
	m_p0_tilemap->set_transmask(1, 0x0e, 0x0f);
	m_ol_p123_tilemap->set_transmask(0, 0x0f, 0x01);
	m_ol_p123_tilemap->set_transmask(1, 0x01, 0x0f);
	m_ol_p0_tilemap->set_transmask(0, 0x0f, 0x0e);
	m_ol_p0_tilemap->set_transmask(1, 0x0e, 0x0f);

	m_tilemap_clip = m_screen->visible_area();
}

void popper_state::draw_sprites( bitmap_ind16 &bitmap,const rectangle &cliprect )
{
	int offs, sx, sy, flipx, flipy;

	for (offs = 0; offs < m_spriteram.bytes() - 4; offs += 4)
	{
		//if y position is in the current strip
		if (m_spriteram[offs + 1] && (((m_spriteram[offs] + (m_flipscreen ? 2 : 0)) & 0xf0) == (0x0f - offs / 0x80) << 4))
		{
			//offs     y pos
			//offs+1   sprite number
			//offs+2
			//76543210
			//x------- flipy
			//-x------ flipx
			//--xx---- unused
			//----xxxx colour
			//offs+3   x pos

			sx = m_spriteram[offs + 3];
			sy = 240 - m_spriteram[offs];
			flipx = (m_spriteram[offs + 2] & 0x40) >> 6;
			flipy = (m_spriteram[offs + 2] & 0x80) >> 7;

			if (m_flipscreen)
			{
				sx = 248 - sx;
				sy = 242 - sy;
				flipx = !flipx;
				flipy = !flipy;
			}

			m_gfxdecode->gfx(1)->transpen(bitmap,cliprect,
					m_spriteram[offs + 1],
					(m_spriteram[offs + 2] & 0x0f),
					flipx,flipy,
					sx,sy,0);
		}
	}
}

UINT32 popper_state::screen_update_popper(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	rectangle finalclip = m_tilemap_clip;
	finalclip &= cliprect;

	//attribram
	//76543210
	//x------- draw over sprites
	//-xxx---- colour for pen 0 (from second prom?)
	//----xxxx colour for pens 1,2,3

	m_p123_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_LAYER1, 0);
	m_p0_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_LAYER1, 0);
	m_ol_p123_tilemap->draw(screen, bitmap, finalclip, TILEMAP_DRAW_LAYER1, 0);
	m_ol_p0_tilemap->draw(screen, bitmap, finalclip, TILEMAP_DRAW_LAYER1, 0);

	draw_sprites(bitmap, cliprect);

	m_p123_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_LAYER0, 0);
	m_p0_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_LAYER0, 0);
	m_ol_p123_tilemap->draw(screen, bitmap, finalclip, TILEMAP_DRAW_LAYER0, 0);
	m_ol_p0_tilemap->draw(screen, bitmap, finalclip, TILEMAP_DRAW_LAYER0, 0);
	return 0;
}

static MACHINE_CONFIG_START( popper, popper_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80,18432000/6)
	MCFG_CPU_PROGRAM_MAP(popper_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", popper_state,  vblank_irq)

	MCFG_CPU_ADD("audiocpu", Z80,18432000/12)
	MCFG_CPU_PROGRAM_MAP(popper_sound_map)
	MCFG_CPU_PERIODIC_INT_DRIVER(popper_state, irq0_line_hold, 4*60)        //NMIs caused by the main CPU

	MCFG_QUANTUM_TIME(attotime::from_hz(1800))

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MCFG_SCREEN_SIZE(33*8, 32*8)
	MCFG_SCREEN_VISIBLE_AREA(0*8, 33*8-1, 2*8, 30*8-1)
	MCFG_SCREEN_UPDATE_DRIVER(popper_state, screen_update_popper)
	MCFG_SCREEN_PALETTE("palette")

	MCFG_GFXDECODE_ADD("gfxdecode", "palette", popper)
	MCFG_PALETTE_ADD("palette", 64)
	MCFG_PALETTE_INIT_OWNER(popper_state, popper)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")

	MCFG_SOUND_ADD("ay1", AY8910, 18432000/12)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)

	MCFG_SOUND_ADD("ay2", AY8910, 18432000/12)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_CONFIG_END


/*************************************
 *
 *  ROM definition
 *
 *************************************/

ROM_START( popper )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "p1",   0x0000, 0x2000, CRC(56881b70) SHA1(d3ade7a54a6cb8a0babf0d667a6b27f492a739dc) )
	ROM_LOAD( "p2",   0x2000, 0x2000, CRC(a054d9d2) SHA1(fcd86e7247b40cf07ea595a64c104b99b0e93ced) )
	ROM_LOAD( "p3",   0x4000, 0x2000, CRC(6201928a) SHA1(53b571b9f2c0568f10cd974641863c2e00777b46) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "p0",   0x0000, 0x1000, CRC(ef5f7c5b) SHA1(c63a3d9ef2868ad7eaacddec810d62d2e124dc15) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "p4",   0x0000, 0x2000, CRC(86203349) SHA1(cce2dd3fa786c2fb3ca80e7b93adf94db3b46b01) )

	ROM_REGION( 0x4000, "gfx2", 0 )
	ROM_LOAD( "p5",   0x0000, 0x2000, CRC(a21ac194) SHA1(2c0e3df8981a12d383b1c4619a0b95a7c2d176a7) )
	ROM_LOAD( "p6",   0x2000, 0x2000, CRC(d99fa790) SHA1(201271ee4fb812236a38cb5f9070ac29e8186097) )

	ROM_REGION( 0x0040, "proms", 0 )
	ROM_LOAD( "p.m3", 0x0000, 0x0020, CRC(713217aa) SHA1(6083c3432bf94c9e983fcc79171529f519c86105) )
	ROM_LOAD( "p.m4", 0x0020, 0x0020, CRC(384de5c1) SHA1(892c89a01c11671c5708113b4e4c27b84be37ea6) )
ROM_END

GAME( 1983, popper, 0, popper, popper, driver_device, 0, ROT90, "Omori Electric Co., Ltd.", "Popper", MACHINE_IMPERFECT_COLORS | MACHINE_SUPPORTS_SAVE )
