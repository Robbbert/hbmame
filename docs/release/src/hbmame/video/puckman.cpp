// license:GPL_2.0
// copyright-holders:Robbbert

#include "emu.h"
#include "includes/puckman.h"
#include "video/resnet.h"


void puckman_state::pacman_palette(palette_device &palette) const
{
	const uint8_t *color_prom = memregion("proms")->base();
	static constexpr int resistances[3] = { 1000, 470, 220 };

	// compute the color output resistor weights
	double rweights[3], gweights[3], bweights[2];
	compute_resistor_weights(0, 255, -1.0,
			3, &resistances[0], rweights, 0, 0,
			3, &resistances[0], gweights, 0, 0,
			2, &resistances[1], bweights, 0, 0);

	// create a lookup table for the palette
	for (int i = 0; i < 32; i++)
	{
		int bit0, bit1, bit2;

		// red component
		bit0 = BIT(color_prom[i], 0);
		bit1 = BIT(color_prom[i], 1);
		bit2 = BIT(color_prom[i], 2);
		int const r = combine_weights(rweights, bit0, bit1, bit2);

		// green component
		bit0 = BIT(color_prom[i], 3);
		bit1 = BIT(color_prom[i], 4);
		bit2 = BIT(color_prom[i], 5);
		int const g = combine_weights(gweights, bit0, bit1, bit2);

		// blue component
		bit0 = BIT(color_prom[i], 6);
		bit1 = BIT(color_prom[i], 7);
		int const b = combine_weights(bweights, bit0, bit1);

		palette.set_indirect_color(i, rgb_t(r, g, b));
	}

	// color_prom now points to the beginning of the lookup table
	color_prom += 32;

	// allocate the colortable
	for (int i = 0; i < 64*4; i++)
	{
		uint8_t const ctabentry = color_prom[i] & 0x0f;

		// first palette bank
		palette.set_pen_indirect(i, ctabentry);

		// second palette bank
		palette.set_pen_indirect(i + 64*4, 0x10 | ctabentry);
	}
}

TILEMAP_MAPPER_MEMBER(puckman_state::pacman_scan_rows)
{
	int offs;

	row += 2;
	col -= 2;
	if (col & 0x20)
		offs = row + ((col & 0x1f) << 5);
	else
		offs = col + (row << 5);

	return offs;
}

TILE_GET_INFO_MEMBER(puckman_state::pacman_get_tile_info)
{
	int code = m_videoram[tile_index] | (m_charbank << 8);
	int attr = (m_colorram[tile_index] & 0x1f) | (m_colortablebank << 5) | (m_palettebank << 6 );

	tileinfo.set(0,code,attr,0);
}

/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

void puckman_state::init_save_state()
{
	save_item(NAME(m_charbank));
	save_item(NAME(m_spritebank));
	save_item(NAME(m_palettebank));
	save_item(NAME(m_colortablebank));
	save_item(NAME(m_flipscreen));
	save_item(NAME(m_bgpriority));
}


VIDEO_START_MEMBER(puckman_state,pacman)
{
	init_save_state();

	m_charbank = 0;
	m_spritebank = 0;
	m_palettebank = 0;
	m_colortablebank = 0;
	m_flipscreen = 0;
	m_bgpriority = 0;
	m_inv_spr = 0;

	/* In the Pac Man based games (NOT Pengo) the first two sprites must be offset */
	/* one pixel to the left to get a more correct placement */
	m_xoffsethack = 1;

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(puckman_state::pacman_get_tile_info)), tilemap_mapper_delegate(*this, FUNC(puckman_state::pacman_scan_rows)), 8, 8, 36, 28);
}

VIDEO_START_MEMBER(puckman_state,birdiy)
{
	VIDEO_START_CALL_MEMBER( pacman );
	m_xoffsethack = 0;
	m_inv_spr = 1; // sprites are mirrored in X-axis compared to normal behaviour
}

void puckman_state::pacman_videoram_w(offs_t offset, uint8_t data)
{
	m_videoram[offset] = data;
	m_bg_tilemap->mark_tile_dirty(offset );
}

void puckman_state::pacman_colorram_w(offs_t offset, uint8_t data)
{
	m_colorram[offset] = data;
	m_bg_tilemap->mark_tile_dirty(offset );
}

WRITE_LINE_MEMBER(puckman_state::flipscreen_w)
{
	m_flipscreen = state;
	m_bg_tilemap->set_flip(m_flipscreen * ( TILEMAP_FLIPX + TILEMAP_FLIPY ) );
}


uint32_t puckman_state::screen_update_pacman(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	if (m_bgpriority != 0)
		bitmap.fill(0, cliprect);
	else
		m_bg_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_OPAQUE,0);

	if( m_spriteram != nullptr )
	{
		uint8_t *spriteram = m_spriteram;
		uint8_t *spriteram_2 = m_spriteram2;
		int offs;

		rectangle spriteclip(2*8, 34*8-1, 0*8, 28*8-1);
		spriteclip &= cliprect;

		/* Draw the sprites. Note that it is important to draw them exactly in this */
		/* order, to have the correct priorities. */
		for (offs = m_spriteram.bytes() - 2;offs > 2*2;offs -= 2)
		{
			int color;
			int sx,sy;
			uint8_t fx,fy;

			if(m_inv_spr)
			{
				sx = spriteram_2[offs + 1];
				sy = 240 - (spriteram_2[offs]);
			}
			else
			{
				sx = 272 - spriteram_2[offs + 1];
				sy = spriteram_2[offs] - 31;
			}

			fx = (spriteram[offs] & 1) ^ m_inv_spr;
			fy = (spriteram[offs] & 2) ^ ((m_inv_spr) << 1);

			color = ( spriteram[offs + 1] & 0x1f ) | (m_colortablebank << 5) | (m_palettebank << 6 );

			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));

			/* also plot the sprite with wraparound (tunnel in Crush Roller) */
			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx - 256,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));
		}
		/* In the Pac Man based games (NOT Pengo) the first two sprites must be offset */
		/* one pixel to the left to get a more correct placement */
		for (offs = 2*2;offs >= 0;offs -= 2)
		{
			int color;
			int sx,sy;
			uint8_t fx,fy;

			if(m_inv_spr)
			{
				sx = spriteram_2[offs + 1];
				sy = 240 - (spriteram_2[offs]);
			}
			else
			{
				sx = 272 - spriteram_2[offs + 1];
				sy = spriteram_2[offs] - 31;
			}
			color = ( spriteram[offs + 1] & 0x1f ) | (m_colortablebank << 5) | (m_palettebank << 6 );

			fx = (spriteram[offs] & 1) ^ m_inv_spr;
			fy = (spriteram[offs] & 2) ^ ((m_inv_spr) << 1);

			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx,sy + m_xoffsethack,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));

			/* also plot the sprite with wraparound (tunnel in Crush Roller) */
			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx - 256,sy + m_xoffsethack,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));
		}
	}

	if (m_bgpriority != 0)
		m_bg_tilemap->draw(screen, bitmap, cliprect, 0,0);
	return 0;
}


/*************************************************************************

    Sega Pengo

**************************************************************************/

VIDEO_START_MEMBER(puckman_state,pengo)
{
	init_save_state();

	m_charbank = 0;
	m_spritebank = 0;
	m_palettebank = 0;
	m_colortablebank = 0;
	m_flipscreen = 0;
	m_bgpriority = 0;
	m_inv_spr = 0;
	m_xoffsethack = 0;

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(puckman_state::pacman_get_tile_info)), tilemap_mapper_delegate(*this, FUNC(puckman_state::pacman_scan_rows)), 8, 8, 36, 28);
}

WRITE_LINE_MEMBER(puckman_state::pengo_palettebank_w)
{
	m_palettebank = state;
	m_bg_tilemap->mark_all_dirty();
}

WRITE_LINE_MEMBER(puckman_state::pengo_colortablebank_w)
{
	m_colortablebank = state;
	m_bg_tilemap->mark_all_dirty();
}

WRITE_LINE_MEMBER(puckman_state::pengo_gfxbank_w)
{
	m_spritebank = state;
	m_charbank = state;
	m_bg_tilemap->mark_all_dirty();
}


/*************************************************************************

S2650 Games

**************************************************************************/

TILE_GET_INFO_MEMBER(puckman_state::s2650_get_tile_info)
{
	int colbank, code, attr;

	colbank = m_s2650games_tileram[tile_index & 0x1f] & 0x3;

	code = m_videoram[tile_index] + (colbank << 8);
	attr = m_colorram[tile_index & 0x1f];

	tileinfo.set(0,code,attr & 0x1f,0);
}

VIDEO_START_MEMBER(puckman_state,s2650games)
{
	init_save_state();

	m_charbank = 0;
	m_spritebank = 0;
	m_palettebank = 0;
	m_colortablebank = 0;
	m_flipscreen = 0;
	m_bgpriority = 0;
	m_inv_spr = 0;
	m_xoffsethack = 1;

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(puckman_state::s2650_get_tile_info)), TILEMAP_SCAN_ROWS, 8, 8, 32, 32);

	m_bg_tilemap->set_scroll_cols(32);
}

uint32_t puckman_state::screen_update_s2650games(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	uint8_t *spriteram = m_spriteram;
	uint8_t *spriteram_2 = m_spriteram2;
	int offs;

	m_bg_tilemap->draw(screen, bitmap, cliprect, 0,0);

	for (offs = m_spriteram.bytes() - 2;offs > 2*2;offs -= 2)
	{
		int color;
		int sx,sy;


		sx = 255 - spriteram_2[offs + 1];
		sy = spriteram_2[offs] - 15;
		color = spriteram[offs + 1] & 0x1f;

		/* TODO: ?? */
		m_gfxdecode->gfx(1)->transmask(bitmap,cliprect,
				(spriteram[offs] >> 2) | ((m_s2650_spriteram[offs] & 3) << 6),
				color,
				spriteram[offs] & 1,spriteram[offs] & 2,
				sx,sy,
				m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));
	}
	/* In the Pac Man based games (NOT Pengo) the first two sprites must be offset */
	/* one pixel to the left to get a more correct placement */
	for (offs = 2*2;offs >= 0;offs -= 2)
	{
		int color;
		int sx,sy;


		sx = 255 - spriteram_2[offs + 1];
		sy = spriteram_2[offs] - 15;
		color = spriteram[offs + 1] & 0x1f;

		/* TODO: ?? */
		m_gfxdecode->gfx(1)->transmask(bitmap,cliprect,
				(spriteram[offs] >> 2) | ((m_s2650_spriteram[offs] & 3)<<6),
				color,
				spriteram[offs] & 1,spriteram[offs] & 2,
				sx,sy + m_xoffsethack,
				m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));
	}
	return 0;
}

void puckman_state::s2650games_videoram_w(offs_t offset, uint8_t data)
{
	m_videoram[offset] = data;
	m_bg_tilemap->mark_tile_dirty(offset);
}

void puckman_state::s2650games_colorram_w(offs_t offset, uint8_t data)
{
	int i;
	m_colorram[offset & 0x1f] = data;
	for (i = offset; i < 0x0400; i += 32)
		m_bg_tilemap->mark_tile_dirty(i);
}

void puckman_state::s2650games_scroll_w(offs_t offset, uint8_t data)
{
	m_bg_tilemap->set_scrolly(offset, data);
}

void puckman_state::s2650games_tilesbank_w(offs_t offset, uint8_t data)
{
	m_s2650games_tileram[offset] = data;
	m_bg_tilemap->mark_all_dirty();
}


/*************************************************************************

Jr. Pac-Man

**************************************************************************/

/*
   0 -   31 = column 2 - 33 attr (used for all 54 rows)
  64 - 1791 = column 2 - 33 code (54 rows)
1794 - 1821 = column 34 code (28 rows)
1826 - 1853 = column 35 code (28 rows)
1858 - 1885 = column 0 code (28 rows)
1890 - 1917 = column 1 code (28 rows)
1922 - 1949 = column 34 attr (28 rows)
1954 - 1981 = column 35 attr (28 rows)
1986 - 2013 = column 0 attr (28 rows)
2018 - 2045 = column 1 attr (28 rows)
*/

TILEMAP_MAPPER_MEMBER(puckman_state::jrpacman_scan_rows)
{
	int offs;

	row += 2;
	col -= 2;
	if ((col & 0x20) && (row & 0x20))
		offs = 0;
	else if (col & 0x20)
		offs = row + (((col&0x3) | 0x38)<< 5);
	else
		offs = col + (row << 5);
	return offs;
}

TILE_GET_INFO_MEMBER(puckman_state::jrpacman_get_tile_info)
{
	int color_index, code, attr;
	if( tile_index < 1792 )
	{
		color_index = tile_index & 0x1f;
	}
	else
	{
		color_index = tile_index + 0x80;
	}

	code = m_videoram[tile_index] | (m_charbank << 8);
	attr = (m_videoram[color_index] & 0x1f) | (m_colortablebank << 5) | (m_palettebank << 6 );

	tileinfo.set(0,code,attr,0);
}

void puckman_state::jrpacman_mark_tile_dirty( int offset )
{
	if( offset < 0x20 )
	{
		/* line color - mark whole line as dirty */
		int i;
		for( i = 2 * 0x20; i < 56 * 0x20; i += 0x20 )
		{
			m_bg_tilemap->mark_tile_dirty(offset + i );
		}
	}
	else if (offset < 1792)
	{
		/* tiles for playfield */
		m_bg_tilemap->mark_tile_dirty(offset );
	}
	else
	{
		/* tiles & colors for top and bottom two rows */
		m_bg_tilemap->mark_tile_dirty(offset & ~0x80 );
	}
}

VIDEO_START_MEMBER(puckman_state,jrpacman)
{
	init_save_state();

	m_charbank = 0;
	m_spritebank = 0;
	m_palettebank = 0;
	m_colortablebank = 0;
	m_flipscreen = 0;
	m_bgpriority = 0;
	m_inv_spr = 0;
	m_xoffsethack = 1;

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(puckman_state::jrpacman_get_tile_info)), tilemap_mapper_delegate(*this, FUNC(puckman_state::jrpacman_scan_rows)), 8, 8, 36, 54);

	m_bg_tilemap->set_transparent_pen(0 );
	m_bg_tilemap->set_scroll_cols(36 );
}

void puckman_state::jrpacman_videoram_w(offs_t offset, uint8_t data)
{
	m_videoram[offset] = data;
	jrpacman_mark_tile_dirty(offset);
}

WRITE_LINE_MEMBER(puckman_state::jrpacman_charbank_w)
{
	m_charbank = state;
	m_bg_tilemap->mark_all_dirty();
}

WRITE_LINE_MEMBER(puckman_state::jrpacman_spritebank_w)
{
	m_spritebank = state;
}

void puckman_state::jrpacman_scroll_w(uint8_t data)
{
	int i;
	for( i = 2; i < 34; i++ )
	{
		m_bg_tilemap->set_scrolly(i, data );
	}
}

WRITE_LINE_MEMBER(puckman_state::jrpacman_bgpriority_w)
{
	m_bgpriority = state;
}

// HBMAME

VIDEO_START_MEMBER( puckman_state, pacmanx )
{
	init_save_state();
	m_charbank = 0;
	m_spritebank = 0;
	m_palettebank = 0;
	m_colortablebank = 0;
	m_flipscreen = 0;
	m_bgpriority = 0;
	m_inv_spr = 0;
	m_xoffsethack = 2;

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(puckman_state::pacman_get_tile_info)), tilemap_mapper_delegate(*this, FUNC(puckman_state::pacman_scan_rows)),  16, 16, 36, 28 );

	m_bg_tilemap->set_scrolldx(0, 384*2 - 288*2 );
	m_bg_tilemap->set_scrolldy(0, 264*2 - 224*2 );
}

u32 puckman_state::screen_update_pacmanx(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	if (m_bgpriority)
		bitmap.fill(0,cliprect);
	else
		m_bg_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_OPAQUE,0);

	if( m_spriteram )
	{
		u8 *spriteram = m_spriteram;
		u8 *spriteram_2 = m_spriteram2;
		int offs;

		rectangle spriteclip(2*2*8, 2*68*8-1, 0*8, 2*56*8-1);
		spriteclip &= cliprect;

		/* Draw the sprites. Note that it is important to draw them exactly in this */
		/* order, to have the correct priorities. */
		for (offs = m_spriteram.bytes() - 2;offs > 2*2;offs -= 2)
		{
			int color;
			int sx,sy;
			u8 fx,fy;

			if(m_inv_spr)
			{
				sx = spriteram_2[offs + 1]*2;
				sy = 480 - (spriteram_2[offs]*2);
			}
			else
			{
				sx = 544 - spriteram_2[offs + 1]*2;
				sy = (spriteram_2[offs] - 31)*2;
			}

			fx = (spriteram[offs] & 1) ^ m_inv_spr;
			fy = (spriteram[offs] & 2) ^ ((m_inv_spr) << 1);

			color = ( spriteram[offs + 1] & 0x1f ) | (m_colortablebank << 5) | (m_palettebank << 6 );

			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));

			/* also plot the sprite with wraparound (tunnel in Crush Roller) */
			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx - 512,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));
		}
		/* In the Pac Man based games (NOT Pengo) the first two sprites must be offset */
		/* one pixel to the left to get a more correct placement */
		for (offs = 2*2;offs >= 0;offs -= 2)
		{
			int color;
			int sx,sy;
			u8 fx,fy;

			if(m_inv_spr)
			{
				sx = spriteram_2[offs + 1]*2;
				sy = 480 - (spriteram_2[offs]*2);
			}
			else
			{
				sx = 544 - spriteram_2[offs + 1]*2;
				sy = (spriteram_2[offs] - 31)*2;
			}
			color = ( spriteram[offs + 1] & 0x1f ) | (m_colortablebank << 5) | (m_palettebank << 6 );

			fx = (spriteram[offs] & 1) ^ m_inv_spr;
			fy = (spriteram[offs] & 2) ^ ((m_inv_spr) << 1);

			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx,sy + m_xoffsethack,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));

			/* also plot the sprite with wraparound (tunnel in Crush Roller) */
			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fy,fx,          //FIXME: flipping bits are really supposed to be inverted here?
					sx - 512,sy + m_xoffsethack,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color & 0x3f, 0));
		}
	}

	if (m_bgpriority != 0)
		m_bg_tilemap->draw(screen, bitmap, cliprect, 0,0);
	return 0;
}





/************************************************************************
  MULTIPAC adapted for HBMAME by Robbbert
 ************************************************************************/

TILE_GET_INFO_MEMBER(puckman_state::multipac_get_tile_info )
{
	int code = m_videoram[tile_index] | (m_charbank << 8);
	int attr = (m_colorram[tile_index] & 0x1f) | (m_colortablebank << 5);

	tileinfo.set(0,code,attr,0);
}

VIDEO_START_MEMBER( puckman_state, multipac )
{
	save_item(NAME(m_charbank));
	save_item(NAME(m_spritebank));
	save_item(NAME(m_palettebank));
	save_item(NAME(m_colortablebank));
	save_item(NAME(m_flipscreen));
	save_item(NAME(m_bgpriority));

	m_charbank = 0;
	m_spritebank = 0;
	m_colortablebank = 0;
	m_flipscreen = 0;
	m_bgpriority = 0;
	m_inv_spr = 0;
	m_xoffsethack = 1;

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(puckman_state::multipac_get_tile_info)), tilemap_mapper_delegate(*this, FUNC(puckman_state::pacman_scan_rows)),  8, 8, 36, 28 );

	m_bg_tilemap->set_scrolldx(0, 384 - 288 );
	m_bg_tilemap->set_scrolldy(0, 264 - 224 );
}

void puckman_state::multipac_palette(palette_device &palette) const
{
	u16 i;
	static const int resistances[3] = { 1000, 470, 220 };
	double rweights[3], gweights[3], bweights[2];
	u8 bit0, bit1, bit2, r, g, b;
	u16 entries = palette.indirect_entries();
	u16 bank,banks = entries >> 5;
	const u8 *color_prom = machine().root_device().memregion("proms")->base();
	const u8 *m_p_prom = machine().root_device().memregion("proms")->base() + entries;

	/* compute the color output resistor weights */
	compute_resistor_weights(0, 255, -1.0,
			3, &resistances[0], rweights, 0, 0,
			3, &resistances[0], gweights, 0, 0,
			2, &resistances[1], bweights, 0, 0);

	for (bank = 0; bank < banks; bank++)
	{
		u16 obank = bank << 7;
		u16 ibank = bank << 5;

		/* create a lookup table for the palette */
		for (i = ibank; i < (ibank+32); i++)
		{
			/* red component */
			bit0 = BIT(color_prom[i], 0);
			bit1 = BIT(color_prom[i], 1);
			bit2 = BIT(color_prom[i], 2);
			r = combine_weights(rweights, bit0, bit1, bit2);

			/* green component */
			bit0 = BIT(color_prom[i], 3);
			bit1 = BIT(color_prom[i], 4);
			bit2 = BIT(color_prom[i], 5);
			g = combine_weights(gweights, bit0, bit1, bit2);

			/* blue component */
			bit0 = BIT(color_prom[i], 6);
			bit1 = BIT(color_prom[i], 7);
			b = combine_weights(bweights, bit0, bit1);

			palette.set_indirect_color(i, rgb_t(r, g, b));
		}

		for (i = obank;i < (obank+128);i++)
		{
			if (m_p_prom[i] & 15)
				palette.set_pen_indirect(i,(m_p_prom[i] & 15) | ibank);
			else
				palette.set_pen_indirect(i, 0);    /* preserve transparency */
		}
	}
}

u32 puckman_state::screen_update_multipac(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	if (m_bgpriority != 0)
		bitmap.fill(0, cliprect);
	else
		m_bg_tilemap->draw(screen, bitmap, cliprect, TILEMAP_DRAW_OPAQUE,0);

	if( m_spriteram != NULL )
	{
		u8 *spriteram = m_spriteram;
		u8 *spriteram_2 = m_spriteram2;
		int offs;

		rectangle spriteclip(2*8, 34*8-1, 0*8, 28*8-1);
		spriteclip &= cliprect;

		/* Draw the sprites. Note that it is important to draw them exactly in this */
		/* order, to have the correct priorities. */
		for (offs = m_spriteram.bytes() - 2;offs > 2*2;offs -= 2)
		{
			int color;
			int sx,sy;
			u8 fx,fy;

			if(m_inv_spr)
			{
				sx = spriteram_2[offs + 1];
				sy = 240 - (spriteram_2[offs]);
			}
			else
			{
				sx = 272 - spriteram_2[offs + 1];
				sy = spriteram_2[offs] - 31;
			}

			fx = (spriteram[offs] & 1) ^ m_inv_spr;
			fy = (spriteram[offs] & 2) ^ ((m_inv_spr) << 1);

			color = ( spriteram[offs + 1] & 0x1f ) | (m_colortablebank << 5) | (m_palettebank << 6 );

			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color, 0));

			/* also plot the sprite with wraparound (tunnel in Crush Roller) */
			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx - 256,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color, 0));
		}
		/* In the Pac Man based games (NOT Pengo) the first two sprites must be offset */
		/* one pixel to the left to get a more correct placement */
		for (offs = 2*2;offs >= 0;offs -= 2)
		{
			int color;
			int sx,sy;
			u8 fx,fy;

			if(m_inv_spr)
			{
				sx = spriteram_2[offs + 1];
				sy = 240 - (spriteram_2[offs]);
			}
			else
			{
				sx = 272 - spriteram_2[offs + 1];
				sy = spriteram_2[offs] - 31;
			}
			color = ( spriteram[offs + 1] & 0x1f ) | (m_colortablebank << 5) | (m_palettebank << 6 );

			fx = (spriteram[offs] & 1) ^ m_inv_spr;
			fy = (spriteram[offs] & 2) ^ ((m_inv_spr) << 1);

			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color, 0));

			/* also plot the sprite with wraparound (tunnel in Crush Roller) */
			m_gfxdecode->gfx(1)->transmask(bitmap,spriteclip,
					( spriteram[offs] >> 2 ) | (m_spritebank << 6),
					color,
					fx,fy,
					sx - 256,sy,
					m_palette->transpen_mask(*m_gfxdecode->gfx(1), color, 0));
		}
	}

	if (m_bgpriority != 0)
		m_bg_tilemap->draw(screen, bitmap, cliprect, 0,0);
	return 0;
}


void puckman_state::multipac_gfxbank_w(u8 data)
{
	m_charbank = data;
	m_spritebank = data;
}

/* multipac passes bankselect info for the 4a and 7f roms on separate bytes. MAME can only use one of them */
void puckman_state::multipac_palbank_w(offs_t offset, u8 data)
{
	if (offset) m_colortablebank = data;
}


/* =============================================
    96in1 / Mad Pac by Robbbert, July 2007
   ============================================= */


void puckman_state::m96in1b_gfxbank_w ( u8 gfxbank )
{
	u8 banks[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 7, 4, 2, 3, 5, 6, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 };
	m_charbank = gfxbank;
	m_spritebank = gfxbank;
	m_colortablebank = banks[gfxbank];
}

void puckman_state::madpac_gfxbank_w ( u8 gfxbank )
{
	m_charbank = gfxbank;
	m_spritebank = gfxbank;

	if (gfxbank > 15)
		m_colortablebank = gfxbank - 15;
	else
		m_colortablebank = 0;
}
