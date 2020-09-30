// license:BSD-3-Clause
// copyright-holders:Paul Leaman
/*****************************************************************************************************************************


*****************************************************************************************************************************/
#include "includes/cps2.h"

/********************************************************************

            Configuration table:

********************************************************************/
/* Game specific data */

#define GFXTYPE_SPRITES   (1<<0)
#define GFXTYPE_SCROLL1   (1<<1)
#define GFXTYPE_SCROLL2   (1<<2)
#define GFXTYPE_SCROLL3   (1<<3)


/*                     CPSB ID    multiply protection      unknown      ctrl     priority masks   palctrl    layer enable masks  */
#define CPS_B_21_DEF 0x32,  -1,   0x00,0x02,0x04,0x06, 0x08, -1,  -1,   0x26,{0x28,0x2a,0x2c,0x2e},0x30, {0x02,0x04,0x08,0x30,0x30}

#define mapper_cps2 { 0x20000, 0x20000, 0, 0 }, mapper_cps2_table
static const struct gfx_range mapper_cps2_table[] =
{
	/* type                                                start    end      bank */
	{ GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 1 },   // 20000-3ffff physical
	{ 0 }
};

static const struct CPS1config cps1_config_table[]=
{
	/* name         CPSB          gfx mapper   in2  in3  out2 */
	/* CPS2 games */
	{"cps2",        CPS_B_21_DEF, mapper_cps2 },
	{nullptr}     /* End of table */
};




/* Offset of each palette entry */
#define cps1_palette_entries (32*6)  /* Number colour schemes in palette */


/* CPS-A registers */
#define CPS1_OBJ_BASE           (0x00/2)    /* Base address of objects */
#define CPS1_SCROLL1_BASE       (0x02/2)    /* Base address of scroll 1 */
#define CPS1_SCROLL2_BASE       (0x04/2)    /* Base address of scroll 2 */
#define CPS1_SCROLL3_BASE       (0x06/2)    /* Base address of scroll 3 */
#define CPS1_OTHER_BASE         (0x08/2)    /* Base address of other video */
#define CPS1_PALETTE_BASE       (0x0a/2)    /* Base address of palette */
#define CPS1_SCROLL1_SCROLLX    (0x0c/2)    /* Scroll 1 X */
#define CPS1_SCROLL1_SCROLLY    (0x0e/2)    /* Scroll 1 Y */
#define CPS1_SCROLL2_SCROLLX    (0x10/2)    /* Scroll 2 X */
#define CPS1_SCROLL2_SCROLLY    (0x12/2)    /* Scroll 2 Y */
#define CPS1_SCROLL3_SCROLLX    (0x14/2)    /* Scroll 3 X */
#define CPS1_SCROLL3_SCROLLY    (0x16/2)    /* Scroll 3 Y */
#define CPS1_STARS1_SCROLLX     (0x18/2)    /* Stars 1 X */
#define CPS1_STARS1_SCROLLY     (0x1a/2)    /* Stars 1 Y */
#define CPS1_STARS2_SCROLLX     (0x1c/2)    /* Stars 2 X */
#define CPS1_STARS2_SCROLLY     (0x1e/2)    /* Stars 2 Y */
#define CPS1_ROWSCROLL_OFFS     (0x20/2)    /* base of row scroll offsets in other RAM */
#define CPS1_VIDEOCONTROL       (0x22/2)    /* flip screen, rowscroll enable */


/*
CPS1 VIDEO RENDERER

*/
#define CPS2_OBJ_BASE   0x00    /* Unknown (not base address of objects). Could be bass address of bank used when object swap bit set? */
#define CPS2_OBJ_UK1    0x02    /* Unknown (nearly always 0x807d, or 0x808e when screen flipped) */
#define CPS2_OBJ_PRI    0x04    /* Layers priorities */
#define CPS2_OBJ_UK2    0x06    /* Unknown (usually 0x0000, 0x1101 in ssf2, 0x0001 in 19XX) */
#define CPS2_OBJ_XOFFS  0x08    /* X offset (usually 0x0040) */
#define CPS2_OBJ_YOFFS  0x0a    /* Y offset (always 0x0010) */


MACHINE_RESET_MEMBER(cps2_state,cps)
{
	const char *gamename = machine().system().name;
	{
		const struct CPS1config *pCFG = &cps1_config_table[0];
		while (pCFG->name)
		{
			if (strcmp(pCFG->name, gamename) == 0)
				break;

			pCFG++;
		}

		m_game_config = pCFG;

		if (!m_game_config->name)
		{
			gamename = "cps2";
			pCFG = &cps1_config_table[0];

			while(pCFG->name)
			{
				if (strcmp(pCFG->name, gamename) == 0)
					break;

				pCFG++;
			}

			m_game_config = pCFG;
		}

		// convert legacy to new
		m_cpsb_addr = m_game_config->cpsb_addr;
		m_cpsb_value = m_game_config->cpsb_value;
		m_mult_factor1 = m_game_config->mult_factor1;
		m_mult_factor2 = m_game_config->mult_factor2;
		m_mult_result_lo = m_game_config->mult_result_lo;
		m_mult_result_hi = m_game_config->mult_result_hi;
		m_layer_control = m_game_config->layer_control;
		m_palette_control = m_game_config->palette_control;
		m_in2_addr = m_game_config->in2_addr;
		m_in3_addr = m_game_config->in3_addr;
		m_out2_addr = m_game_config->out2_addr;

		for (u8 i = 0; i < 4; i++)
		{
			m_priority[i] = m_game_config->priority[i];
			m_layer_enable_mask[i] = m_game_config->layer_enable_mask[i];
			m_bank_sizes[i] = m_game_config->bank_sizes[i];
		}
		m_layer_enable_mask[4] = m_game_config->layer_enable_mask[4];

		for (u8 i = 0; i < 32; i++)
			m_bank_type[i] = 0;
		const struct gfx_range *range = m_game_config->bank_mapper;
		u8 tu = 0;
		while ((range->type) && (tu < 32)) 
		{
			m_bank_type[tu++] = range->type;
			m_bank_type[tu++] = range->start;
			m_bank_type[tu++] = range->end;
			m_bank_type[tu++] = range->bank;
			++range;
		}
	}
}


inline u16 *cps2_state::cps1_base( int offset, int boundary )
{
	int base = m_cps_a_regs[offset] * 256;

	/*
	The scroll RAM must start on a 0x4000 boundary.
	Some games do not do this.
	For example:
	   Captain commando     - continue screen will not display
	   Muscle bomber games  - will animate garbage during gameplay
	Mask out the irrelevant bits.
	*/
	base &= ~(boundary - 1);
	return &m_gfxram[(base & 0x3ffff) / 2];
}



void cps2_state::cps1_cps_a_w(offs_t offset, u16 data, u16 mem_mask)
{
	data = COMBINE_DATA(&m_cps_a_regs[offset]);

	/*
	The main CPU writes the palette to gfxram, and the CPS-B custom copies it
	to the real palette RAM, which is separated from gfxram.
	This is done ONLY after the palette base register is written to. It is not
	known what the exact timing should be, how long it should take and when it
	should happen. We are assuming that the copy happens immediately, since it
	fixes glitches in the ghouls intro, but it might happen at next vblank.
	*/
	if (offset == CPS1_PALETTE_BASE)
		cps1_build_palette(cps1_base(CPS1_PALETTE_BASE, m_palette_align));

	// pzloop2 write to register 24 on startup. This is probably just a bug.
	if (offset == 0x24 / 2 && m_cps_version == 2)
		return;

#ifdef MAME_DEBUG
	if (offset > CPS1_VIDEOCONTROL)
		popmessage("write to CPS-A register %02x contact MAMEDEV", offset * 2);
#endif
}


u16 cps2_state::cps1_cps_b_r(offs_t offset)
{
	/* Some games interrogate a couple of registers on bootup. */
	/* These are CPS1 board B self test checks. They wander from game to */
	/* game. */
	if (offset == m_cpsb_addr / 2)
		return m_cpsb_value;
	else
	/* some games use as a protection check the ability to do 16-bit multiplications */
	/* with a 32-bit result, by writing the factors to two ports and reading the */
	/* result from two other ports. */
	if (offset == m_mult_result_lo / 2)
	{
		//printf("Multiply protection lo at %s\n",machine().describe_context());
		return (m_cps_b_regs[m_mult_factor1 / 2] *
				m_cps_b_regs[m_mult_factor2 / 2]) & 0xffff;
	}
	else
	if (offset == m_mult_result_hi / 2)
	{
		//printf("Multiply protection hi\n");
		return (m_cps_b_regs[m_mult_factor1 / 2] *
				m_cps_b_regs[m_mult_factor2 / 2]) >> 16;
	}
	else
	if (offset == m_in2_addr / 2)  /* Extra input ports (on C-board) */
		return cps1_in2_r(); // HBMAME ioport("IN2")->read();
	else
	if (offset == m_in3_addr / 2)  /* Player 4 controls (on C-board) ("Captain Commando") */
		return cps1_in3_r(); // HBMAME ioport("IN3")->read();
	else
	if (m_cps_version == 2)
	{
		if (offset == 0x10/2)
		{
			// UNKNOWN--only mmatrix appears to read this, and I'm not sure if the result is actually used
			return m_cps_b_regs[0x10 / 2];
		}
		if (offset == 0x12/2)
			return m_cps_b_regs[0x12 / 2];
	}
#ifdef MAME_DEBUG
	popmessage("CPS-B read port %02x contact MAMEDEV", offset * 2);
#endif
	return 0xffff;
}


void cps2_state::cps1_cps_b_w(offs_t offset, u16 data, u16 mem_mask)
{
	data = COMBINE_DATA(&m_cps_b_regs[offset]);

	if (m_cps_version == 2)
	{
		/* To mark scanlines for raster effects */
		if (offset == 0x0e/2)
		{
			// UNKNOWN
			return;
		}
		if (offset == 0x10/2)
		{
			m_scanline1 = (data & 0x1ff);
			return;
		}
		if (offset == 0x12/2)
		{
			m_scanline2 = (data & 0x1ff);
			return;
		}
	}


	// additional outputs on C-board
	if (offset == m_out2_addr / 2)
	{
		if (ACCESSING_BITS_0_7)
		{
			if (m_cpsb_value == 0x0402)    // Mercs (CN2 connector)
			{
				machine().bookkeeping().coin_lockout_w(2, ~data & 0x01);
				//output().set_led_value(0, data & 0x02);
				//output().set_led_value(1, data & 0x04);
				//output().set_led_value(2, data & 0x08);
			}
			else    // kod, captcomm, knights
			{
				machine().bookkeeping().coin_lockout_w(2, ~data & 0x02);
				machine().bookkeeping().coin_lockout_w(3, ~data & 0x08);
			}
		}
	}
}


void cps2_state::unshuffle( u64 *buf, int len )
{
	int i;
	u64 t;

	if (len == 2)
		return;

	assert(len % 4 == 0);   /* must not happen */

	len /= 2;

	unshuffle(buf, len);
	unshuffle(buf + len, len);

	for (i = 0; i < len / 2; i++)
	{
		t = buf[len / 2 + i];
		buf[len / 2 + i] = buf[len + i];
		buf[len + i] = t;
	}
}


void cps2_state::cps2_gfx_decode()
{
	const int banksize = 0x200000;
	int size = memregion("gfx")->bytes();
	int i;

	for (i = 0; i < size; i += banksize)
		unshuffle((u64 *)(memregion("gfx")->base() + i), banksize / 8);
}


void cps2_state::init_cps1()
{
	m_scanline1 = 0;
	m_scanline2 = 0;
	m_scancalls = 0;
	m_last_sprite_offset = 0;
	m_pri_ctrl = 0;
	m_objram_bank = 0;
}



void cps2_state::init_cps2_video()
{
	cps2_gfx_decode();

	m_scanline1 = 262;
	m_scanline2 = 262;
	m_scancalls = 0;
	m_last_sprite_offset = 0;
	m_cps2_last_sprite_offset = 0;
	m_pri_ctrl = 0;
	m_objram_bank = 0;
}


void cps2_state::cps1_get_video_base()
{
	// HBMAME start
	int layercontrol=0, videocontrol=0, scroll1xoff=0, scroll2xoff=0, scroll3xoff=0;

	if (BIT(m_scrollx1, 7))
		scroll1xoff = 0 - (m_scrollx1 & 0x7f);
	else
		scroll1xoff = m_scrollx1;

	if (BIT(m_scrollx2, 7))
		scroll2xoff = 0 - (m_scrollx2 & 0x7f);
	else
		scroll2xoff = m_scrollx2;

	if (BIT(m_scrollx3, 7))
		scroll3xoff = 0 - (m_scrollx3 & 0x7f);
	else
		scroll3xoff = m_scrollx3;
	// HBMAME end

	/* Re-calculate the VIDEO RAM base */
	if (m_scroll1 != cps1_base(CPS1_SCROLL1_BASE, m_scroll_size))
	{
		m_scroll1 = cps1_base(CPS1_SCROLL1_BASE, m_scroll_size);
		m_bg_tilemap[0]->mark_all_dirty();
	}
	if (m_scroll2 != cps1_base(CPS1_SCROLL2_BASE, m_scroll_size))
	{
		m_scroll2 = cps1_base(CPS1_SCROLL2_BASE, m_scroll_size);
		m_bg_tilemap[1]->mark_all_dirty();
	}
	if (m_scroll3 != cps1_base(CPS1_SCROLL3_BASE, m_scroll_size))
	{
		m_scroll3 = cps1_base(CPS1_SCROLL3_BASE, m_scroll_size);
		m_bg_tilemap[2]->mark_all_dirty();
	}

	m_obj = cps1_base(CPS1_OBJ_BASE, m_obj_size);
	m_other = cps1_base(CPS1_OTHER_BASE, m_other_size);

	/* Get scroll values */
	m_scroll1x = m_cps_a_regs[CPS1_SCROLL1_SCROLLX] + scroll1xoff;
	m_scroll1y = m_cps_a_regs[CPS1_SCROLL1_SCROLLY];
	m_scroll2x = m_cps_a_regs[CPS1_SCROLL2_SCROLLX] + scroll2xoff;
	m_scroll2y = m_cps_a_regs[CPS1_SCROLL2_SCROLLY];
	m_scroll3x = m_cps_a_regs[CPS1_SCROLL3_SCROLLX] + scroll3xoff;
	m_scroll3y = m_cps_a_regs[CPS1_SCROLL3_SCROLLY];
	m_stars1x = m_cps_a_regs[CPS1_STARS1_SCROLLX];
	m_stars1y = m_cps_a_regs[CPS1_STARS1_SCROLLY];
	m_stars2x = m_cps_a_regs[CPS1_STARS2_SCROLLX];
	m_stars2y = m_cps_a_regs[CPS1_STARS2_SCROLLY];

	/* Get layer enable bits */
	layercontrol = m_cps_b_regs[m_layer_control / 2];
	videocontrol = m_cps_a_regs[CPS1_VIDEOCONTROL];
	m_bg_tilemap[0]->enable(layercontrol & m_layer_enable_mask[0]);
	m_bg_tilemap[1]->enable((layercontrol & m_layer_enable_mask[1]) && (videocontrol & 4));
	m_bg_tilemap[2]->enable((layercontrol & m_layer_enable_mask[2]) && (videocontrol & 8));
	m_stars_enabled[0] = layercontrol & m_layer_enable_mask[3];
	m_stars_enabled[1] = layercontrol & m_layer_enable_mask[4];
}


void cps2_state::cps1_gfxram_w(offs_t offset, u16 data, u16 mem_mask)
{
	int page = (offset >> 7) & 0x3c0;
	COMBINE_DATA(&m_gfxram[offset]);

	if (page == (m_cps_a_regs[CPS1_SCROLL1_BASE] & 0x3c0))
		m_bg_tilemap[0]->mark_tile_dirty(offset / 2 & 0x0fff);

	if (page == (m_cps_a_regs[CPS1_SCROLL2_BASE] & 0x3c0))
		m_bg_tilemap[1]->mark_tile_dirty(offset / 2 & 0x0fff);

	if (page == (m_cps_a_regs[CPS1_SCROLL3_BASE] & 0x3c0))
		m_bg_tilemap[2]->mark_tile_dirty(offset / 2 & 0x0fff);
}



int cps2_state::gfxrom_bank_mapper( int type, int code )
{
	int shift = 0;

	switch (type)
	{
		case GFXTYPE_SPRITES: shift = 1; break;
		case GFXTYPE_SCROLL1: shift = 0; break;
		case GFXTYPE_SCROLL2: shift = 1; break;
		case GFXTYPE_SCROLL3: shift = 3; break;
	}

	code <<= shift;

	for (u8 i = 0; i < 8; i++)
	{
		if (code >= m_bank_type[i*4+1] && code <= m_bank_type[i*4+2])
		{
			if (m_bank_type[i*4] & type)
			{
				int base = 0;
				for (int j = 0; j < m_bank_type[i*4+3]; ++j)
					base += m_bank_sizes[j];

				return (base + (code & (m_bank_sizes[m_bank_type[i*4+3]] - 1))) >> shift;
			}
		}
	}

#ifdef MAME_DEBUG
//  popmessage("tile %02x/%04x out of range", type, code >> shift);
#endif

	return -1;
}


/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

TILEMAP_MAPPER_MEMBER(cps2_state::tilemap0_scan)
{
	/* logical (col,row) -> memory offset */
	return (row & 0x1f) + ((col & 0x3f) << 5) + ((row & 0x20) << 6);
}

TILEMAP_MAPPER_MEMBER(cps2_state::tilemap1_scan)
{
	/* logical (col,row) -> memory offset */
	return (row & 0x0f) + ((col & 0x3f) << 4) + ((row & 0x30) << 6);
}

TILEMAP_MAPPER_MEMBER(cps2_state::tilemap2_scan)
{
	/* logical (col,row) -> memory offset */
	return (row & 0x07) + ((col & 0x3f) << 3) + ((row & 0x38) << 6);
}

TILE_GET_INFO_MEMBER(cps2_state::get_tile0_info)
{
	int code = m_scroll1[2 * tile_index];
	int attr = m_scroll1[2 * tile_index + 1];
	int gfxset;

	code = gfxrom_bank_mapper(GFXTYPE_SCROLL1, code);

	/* allows us to reproduce a problem seen with a ffight board where USA and Japanese
	     roms have been mixed to be reproduced (ffightub) -- it looks like each column
	     should alternate between the left and right side of the 16x16 tiles */
	gfxset = (tile_index & 0x20) >> 5;

	tileinfo.set(gfxset,
			code,
			(attr & 0x1f) + 0x20,
			TILE_FLIPYX((attr & 0x60) >> 5));
	tileinfo.group = (attr & 0x0180) >> 7;

	// for out of range tiles, switch to fully transparent data
	// (but still call tileinfo.set, otherwise problems might occur on boot e.g. unsquad)
	if (code == -1)
		tileinfo.pen_data = m_empty_tile;
}

TILE_GET_INFO_MEMBER(cps2_state::get_tile1_info)
{
	int code = m_scroll2[2 * tile_index];
	int attr = m_scroll2[2 * tile_index + 1];

	code = gfxrom_bank_mapper(GFXTYPE_SCROLL2, code);

	tileinfo.set(2,
			code,
			(attr & 0x1f) + 0x40,
			TILE_FLIPYX((attr & 0x60) >> 5));
	tileinfo.group = (attr & 0x0180) >> 7;

	// for out of range tiles, switch to fully transparent data
	if (code == -1)
		tileinfo.pen_data = m_empty_tile;
}

TILE_GET_INFO_MEMBER(cps2_state::get_tile2_info)
{
	int code = m_scroll3[2 * tile_index] & 0x3fff;
	int attr = m_scroll3[2 * tile_index + 1];

	code = gfxrom_bank_mapper(GFXTYPE_SCROLL3, code);

	tileinfo.set(3,
			code,
			(attr & 0x1f) + 0x60,
			TILE_FLIPYX((attr & 0x60) >> 5));
	tileinfo.group = (attr & 0x0180) >> 7;

	// for out of range tiles, switch to fully transparent data
	// (but still call tileinfo.set, otherwise problems might occur on boot e.g. unsquad)
	if (code == -1)
		tileinfo.pen_data = m_empty_tile;
}



void cps2_state::cps1_update_transmasks()
{
	int i;

	for (i = 0; i < 4; i++)
	{
		int mask;

		/* Get transparency registers */
		if (m_priority[i] != -1)
			mask = m_cps_b_regs[m_priority[i] / 2] ^ 0xffff;
		else
			mask = 0xffff;  /* completely transparent if priority masks not defined (qad) */

		m_bg_tilemap[0]->set_transmask(i, mask, 0x8000);
		m_bg_tilemap[1]->set_transmask(i, mask, 0x8000);
		m_bg_tilemap[2]->set_transmask(i, mask, 0x8000);
	}
}

VIDEO_START_MEMBER(cps2_state,cps)
{
	int i;

	MACHINE_RESET_CALL_MEMBER(cps);

	/* Put in some const */
	m_scroll_size    = 0x4000;  /* scroll1, scroll2, scroll3 */
	m_obj_size       = 0x0800;
	m_cps2_obj_size  = 0x2000;
	m_other_size     = 0x0800;
	m_palette_align  = 0x0400;  /* minimum alignment is a single palette page (512 colors). Verified on pcb. */
	m_palette_size   = cps1_palette_entries * 32; /* Size of palette RAM */
	m_stars_rom_size = 0x2000;  /* first 0x4000 of gfx ROM are used, but 0x0000-0x1fff is == 0x2000-0x3fff */

	/* create tilemaps */
	m_bg_tilemap[0] = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(cps2_state::get_tile0_info)), tilemap_mapper_delegate(*this, FUNC(cps2_state::tilemap0_scan)),  8,  8, 64, 64);
	m_bg_tilemap[1] = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(cps2_state::get_tile1_info)), tilemap_mapper_delegate(*this, FUNC(cps2_state::tilemap1_scan)), 16, 16, 64, 64);
	m_bg_tilemap[2] = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(cps2_state::get_tile2_info)), tilemap_mapper_delegate(*this, FUNC(cps2_state::tilemap2_scan)), 32, 32, 64, 64);

	/* create empty tiles */
	memset(m_empty_tile, 0x0f, sizeof(m_empty_tile));

	/* front masks will change at runtime to handle sprite occluding */
	cps1_update_transmasks();

	for (i = 0; i < cps1_palette_entries * 16; i++)
		m_palette->set_pen_color(i, rgb_t(0,0,0));

	m_buffered_obj = make_unique_clear<u16[]>(m_obj_size / 2);

	if (m_cps_version == 2)
		m_cps2_buffered_obj = make_unique_clear<u16[]>(m_cps2_obj_size / 2);

	/* clear RAM regions */
	memset(m_gfxram, 0, m_gfxram.bytes());   /* Clear GFX RAM */
	memset(m_cps_a_regs, 0, 0x40);   /* Clear CPS-A registers */
	memset(m_cps_b_regs, 0, 0x40);   /* Clear CPS-B registers */

	m_cps_b_regs[m_palette_control/2] = 0x3F; // HBMAME

	if (m_cps_version == 2)
	{
		memset(m_objram1, 0, m_cps2_obj_size);
		memset(m_objram2, 0, m_cps2_obj_size);
	}

	/* Put in some defaults */
	m_cps_a_regs[CPS1_OBJ_BASE]     = 0x9200;
	m_cps_a_regs[CPS1_SCROLL1_BASE] = 0x9000;
	m_cps_a_regs[CPS1_SCROLL2_BASE] = 0x9040;
	m_cps_a_regs[CPS1_SCROLL3_BASE] = 0x9080;
	m_cps_a_regs[CPS1_OTHER_BASE]   = 0x9100;

	/* Set up old base */
	m_scroll1 = nullptr;
	m_scroll2 = nullptr;
	m_scroll3 = nullptr;
	m_obj = nullptr;
	m_other = nullptr;
	cps1_get_video_base();   /* Calculate base pointers */
	cps1_get_video_base();   /* Calculate old base pointers */

	m_screen->register_screen_bitmap(m_dummy_bitmap);

	/* state save register */
	save_item(NAME(m_scanline1));
	save_item(NAME(m_scanline2));
	save_item(NAME(m_scancalls));
#if 0
	/* these do not need to be saved, because they are recovered from cps_a_regs in cps1_postload */
	save_item(NAME(m_scroll1x));
	save_item(NAME(m_scroll1y));
	save_item(NAME(m_scroll2x));
	save_item(NAME(m_scroll2y));
	save_item(NAME(m_scroll3x));
	save_item(NAME(m_scroll3y));
	save_item(NAME(m_stars1x));
	save_item(NAME(m_stars1y));
	save_item(NAME(m_stars2x));
	save_item(NAME(m_stars2y));
	save_item(NAME(m_stars_enabled));
#endif
	save_item(NAME(m_last_sprite_offset));
	save_item(NAME(m_pri_ctrl));
	save_item(NAME(m_objram_bank));

	save_pointer(NAME(m_buffered_obj.get()), m_obj_size / 2);
	if (m_cps_version == 2)
	{
		save_item(NAME(m_cps2_last_sprite_offset));
		save_pointer(NAME(m_cps2_buffered_obj.get()), m_cps2_obj_size / 2);
	}

	machine().save().register_postload(save_prepost_delegate(FUNC(cps2_state::cps1_get_video_base), this));
}

VIDEO_START_MEMBER(cps2_state,cps1)
{
	m_cps_version = 1;
	VIDEO_START_CALL_MEMBER(cps);
}

VIDEO_START_MEMBER(cps2_state,cps2)
{
	m_cps_version = 2;
	VIDEO_START_CALL_MEMBER(cps);
}

/***************************************************************************

  Build palette from palette RAM

  12 bit RGB with a 4 bit brightness value.

***************************************************************************/

void cps2_state::cps1_build_palette( const u16* const palette_base )
{
	int offset, page;
	const u16 *palette_ram = palette_base;
	int ctrl = m_cps_b_regs[m_palette_control/2];

	/*
	The palette is copied only for pages that are enabled in the ctrl
	register. Note that if the first palette pages are skipped, all
	the following pages are scaled down.
	*/
	for (page = 0; page < 6; ++page)
	{
		if (BIT(ctrl, page))
		{
			for (offset = 0; offset < 0x200; ++offset)
			{
				int palette = *(palette_ram++);
				int r, g, b, bright;

				// from my understanding of the schematics, when the 'brightness'
				// component is set to 0 it should reduce brightness to 1/3

				// HBMAME start
				u8 b_adj = 0x0f;
				u8 b_div = 0x1e + b_adj;
				bright = b_adj + ((palette >> 12) << 1);

				// New code to get rid of grey squares
				r = (palette >> 8) & 0x0f;
				g = (palette >> 4) & 0x0f;
				b = palette & 0x0f;
				r = (r > 1) ? r * 0x11 * bright / b_div : 0;
				g = (g > 1) ? g * 0x11 * bright / b_div : 0;
				b = (b > 1) ? b * 0x11 * bright / b_div : 0;
				// HBMAME end

				m_palette->set_pen_color (0x200 * page + offset, rgb_t(r, g, b));
			}
		}
		else
		{
			// skip page in gfxram, but only if we have already copied at least one page
			if (palette_ram != palette_base)
				palette_ram += 0x200;
		}
	}
}



/***************************************************************************

                                Sprites
                                =======

  Sprites are represented by a number of 8 byte values

  xx xx yy yy nn nn aa aa

  where xxxx = x position
        yyyy = y position
        nnnn = tile number
        aaaa = attribute word
                    0x0001  colour
                    0x0002  colour
                    0x0004  colour
                    0x0008  colour
                    0x0010  colour
                    0x0020  X Flip
                    0x0040  Y Flip
                    0x0080  X & Y offset toggle (used in Marvel vs. Capcom.)
                    0x0100  X block size (in sprites)
                    0x0200  X block size
                    0x0400  X block size
                    0x0800  X block size
                    0x1000  Y block size (in sprites)
                    0x2000  Y block size
                    0x4000  Y block size
                    0x8000  Y block size

  The end of the table (may) be marked by an attribute value of 0xff00.

***************************************************************************/

void cps2_state::cps1_find_last_sprite()    /* Find the offset of last sprite */
{
	int offset = 0;
	/* Locate the end of table marker */
	while (offset < m_obj_size / 2)
	{
		{
			int colour = m_buffered_obj[offset + 3];
			if ((colour & 0xff00) == 0xff00)
			{
				/* Marker found. This is the last sprite. */
				m_last_sprite_offset = offset - 4;
				return;
			}
		}

		offset += 4;
	}
	/* Sprites must use full sprite RAM */
	m_last_sprite_offset = m_obj_size / 2 - 4;
}


void cps2_state::cps1_render_sprites( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
#define DRAWSPRITE(CODE,COLOR,FLIPX,FLIPY,SX,SY)                    \
{                                                                   \
	if (flip_screen())                                           \
		m_gfxdecode->gfx(2)->prio_transpen(bitmap,\
				cliprect,                            \
				CODE,                                               \
				COLOR,                                              \
				!(FLIPX),!(FLIPY),                                  \
				512-16-(SX),256-16-(SY),    screen.priority(),0x02,15);                   \
	else                                                            \
		m_gfxdecode->gfx(2)->prio_transpen(bitmap,\
				cliprect,                            \
				CODE,                                               \
				COLOR,                                              \
				FLIPX,FLIPY,                                        \
				SX,SY, screen.priority(),0x02,15);          \
}


	int i, baseadd;
	u16 *base = m_buffered_obj.get();

	baseadd = 4;

	for (i = m_last_sprite_offset; i >= 0; i -= 4)
	{
		int x = *(base + 0);
		int y = *(base + 1);
		int code = *(base + 2);
		int colour = *(base + 3);
		int col = colour & 0x1f;

//      x -= 0x20;
//      y += 0x20;

		code = gfxrom_bank_mapper(GFXTYPE_SPRITES, code);

		if (code != -1)
		{
			if (colour & 0xff00 )
			{
				/* handle blocked sprites */
				int nx = (colour & 0x0f00) >> 8;
				int ny = (colour & 0xf000) >> 12;
				int nxs, nys, sx, sy;
				nx++;
				ny++;

				if (colour & 0x40)
				{
					/* Y flip */
					if (colour & 0x20)
					{
						for (nys = 0; nys < ny; nys++)
						{
							for (nxs = 0; nxs < nx; nxs++)
							{
								sx = (x + nxs * 16) & 0x1ff;
								sy = (y + nys * 16) & 0x1ff;

								DRAWSPRITE(
//                                      code + (nx - 1) - nxs + 0x10 * (ny - 1 - nys),
										(code & ~0xf) + ((code + (nx - 1) - nxs) & 0xf) + 0x10 * (ny - 1 - nys),
										(col & 0x1f),
										1,1,
										sx,sy);
							}
						}
					}
					else
					{
						for (nys = 0; nys < ny; nys++)
						{
							for (nxs = 0; nxs < nx; nxs++)
							{
								sx = (x + nxs * 16) & 0x1ff;
								sy = (y + nys * 16) & 0x1ff;

								DRAWSPRITE(
//                                      code + nxs + 0x10 * (ny - 1 - nys),
										(code & ~0xf) + ((code + nxs) & 0xf) + 0x10 * (ny - 1 - nys),
										(col & 0x1f),
										0,1,
										sx,sy);
							}
						}
					}
				}
				else
				{
					if (colour & 0x20)
					{
						for (nys = 0; nys < ny; nys++)
						{
							for (nxs = 0; nxs<nx; nxs++)
							{
								sx = (x + nxs * 16) & 0x1ff;
								sy = (y + nys * 16) & 0x1ff;

								DRAWSPRITE(
//                                      code + (nx - 1) - nxs + 0x10 * nys,
										(code & ~0xf) + ((code + (nx - 1) - nxs) & 0xf) + 0x10 * nys,
										(col & 0x1f),
										1,0,
										sx,sy);
							}
						}
					}
					else
					{
						for (nys = 0; nys < ny; nys++)
						{
							for (nxs = 0; nxs < nx; nxs++)
							{
								sx = (x + nxs * 16) & 0x1ff;
								sy = (y + nys * 16) & 0x1ff;

								DRAWSPRITE(
//                                      code + nxs + 0x10 * nys,
										(code & ~0xf) + ((code + nxs) & 0xf) + 0x10 * nys,  // fix 00406: qadjr: When playing as the ninja, there is one broken frame in his animation loop when walking.
										(col & 0x1f),
										0,0,
										sx,sy);
							}
						}
					}
				}
			}
			else
			{
				/* Simple case... 1 sprite */
						DRAWSPRITE(
						code,
						(col & 0x1f),
						colour&0x20,colour&0x40,
						x & 0x1ff,y & 0x1ff);
			}
		}
		base += baseadd;
	}
#undef DRAWSPRITE
}




void cps2_state::cps2_objram_bank_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (ACCESSING_BITS_0_7)
		m_objram_bank = data & 1;
}

u16 cps2_state::cps2_objram1_r(offs_t offset)
{
	if (m_objram_bank & 1)
		return m_objram2[offset];
	else
		return m_objram1[offset];
}

u16 cps2_state::cps2_objram2_r(offs_t offset)
{
	if (m_objram_bank & 1)
		return m_objram1[offset];
	else
		return m_objram2[offset];
}

void cps2_state::cps2_objram1_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (m_objram_bank & 1)
		COMBINE_DATA(&m_objram2[offset]);
	else
		COMBINE_DATA(&m_objram1[offset]);
}

void cps2_state::cps2_objram2_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (m_objram_bank & 1)
		COMBINE_DATA(&m_objram1[offset]);
	else
		COMBINE_DATA(&m_objram2[offset]);
}

u16 *cps2_state::cps2_objbase()
{
	int baseptr;
	baseptr = 0x7000;

	if (m_objram_bank & 1)
		baseptr ^= 0x0080;

//popmessage("%04x %d", cps2_port(machine, CPS2_OBJ_BASE), m_objram_bank & 1);

	if (baseptr == 0x7000)
		return m_objram1;
	else //if (baseptr == 0x7080)
		return m_objram2;
}


void cps2_state::cps2_find_last_sprite()    /* Find the offset of last sprite */
{
	int offset = 0;
	u16 *base = m_cps2_buffered_obj.get();

	/* Locate the end of table marker */
	while (offset < m_cps2_obj_size / 2)
	{
		if (base[offset + 1] >= 0x8000 || base[offset + 3] >= 0xff00)
		{
			/* Marker found. This is the last sprite. */
			m_cps2_last_sprite_offset = offset - 4;
			return;
		}

		offset += 4;
	}
	/* Sprites must use full sprite RAM */
	m_cps2_last_sprite_offset = m_cps2_obj_size / 2 - 4;
}

void cps2_state::cps2_render_sprites( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int *primasks )
{
#define DRAWSPRITE(CODE,COLOR,FLIPX,FLIPY,SX,SY)                                    \
{                                                                                   \
	if (flip_screen())                                                           \
		m_gfxdecode->gfx(2)->prio_transpen(bitmap,\
				cliprect,                                            \
				CODE,                                                               \
				COLOR,                                                              \
				!(FLIPX),!(FLIPY),                                                  \
				512-16-(SX),256-16-(SY), screen.priority(),primasks[priority],15);                 \
	else                                                                            \
		m_gfxdecode->gfx(2)->prio_transpen(bitmap,\
				cliprect,                                            \
				CODE,                                                               \
				COLOR,                                                              \
				FLIPX,FLIPY,                                                        \
				SX,SY, screen.priority(),primasks[priority],15);                 \
}

	int i;
	u16 *base = m_cps2_buffered_obj.get();
	int xoffs = 64 - m_output[CPS2_OBJ_XOFFS /2];
	int yoffs = 16 - m_output[CPS2_OBJ_YOFFS /2];

#ifdef MAME_DEBUG
	if (machine().input().code_pressed(KEYCODE_Z) && machine().input().code_pressed(KEYCODE_R))
	{
		return;
	}
#endif

	for (i = m_cps2_last_sprite_offset; i >= 0; i -= 4)
	{
		int x = base[i + 0];
		int y = base[i + 1];
		int priority = (x >> 13) & 0x07;
		int code = base[i + 2] + ((y & 0x6000) << 3);
		int colour = base[i + 3];
		int col = colour & 0x1f;

		if (colour & 0x80)
		{
			x += m_output[CPS2_OBJ_XOFFS /2];  /* fix the offset of some games */
			y += m_output[CPS2_OBJ_YOFFS /2];  /* like Marvel vs. Capcom ending credits */
		}

		if (colour & 0xff00)
		{
			/* handle blocked sprites */
			int nx = (colour & 0x0f00) >> 8;
			int ny = (colour & 0xf000) >> 12;
			int nxs, nys, sx, sy;
			nx++;
			ny++;

			if (colour & 0x40)
			{
				/* Y flip */
				if (colour & 0x20)
				{
					for (nys = 0; nys < ny; nys++)
					{
						for (nxs = 0; nxs < nx; nxs++)
						{
							sx = (x + nxs * 16 + xoffs) & 0x3ff;
							sy = (y + nys * 16 + yoffs) & 0x3ff;
							DRAWSPRITE(
									code + (nx - 1) - nxs + 0x10 * (ny - 1 - nys),
									(col & 0x1f),
									1,1,
									sx,sy);
						}
					}
				}
				else
				{
					for (nys = 0; nys < ny; nys++)
					{
						for (nxs = 0; nxs < nx; nxs++)
						{
							sx = (x + nxs * 16 + xoffs) & 0x3ff;
							sy = (y + nys * 16 + yoffs) & 0x3ff;

							DRAWSPRITE(
									code + nxs + 0x10 * (ny - 1 - nys),
									(col & 0x1f),
									0,1,
									sx,sy);
						}
					}
				}
			}
			else
			{
				if (colour & 0x20)
				{
					for (nys = 0; nys < ny; nys++)
					{
						for (nxs = 0; nxs < nx; nxs++)
						{
							sx = (x + nxs * 16 + xoffs) & 0x3ff;
							sy = (y + nys * 16 + yoffs) & 0x3ff;

							DRAWSPRITE(
									code + (nx - 1) - nxs + 0x10 * nys,
									(col & 0x1f),
									1,0,
									sx,sy);
						}
					}
				}
				else
				{
					for (nys = 0; nys < ny; nys++)
					{
						for (nxs = 0; nxs < nx; nxs++)
						{
							sx = (x + nxs * 16 + xoffs) & 0x3ff;
							sy = (y + nys * 16 + yoffs) & 0x3ff;

							DRAWSPRITE(
//                                      code + nxs + 0x10 * nys,
									(code & ~0xf) + ((code + nxs) & 0xf) + 0x10 * nys,  //  pgear fix
									(col & 0x1f),
									0,0,
									sx,sy);
						}
					}
				}
			}
		}
		else
		{
			/* Simple case... 1 sprite */
			DRAWSPRITE(
					code,
					(col & 0x1f),
					colour&0x20,colour&0x40,
					(x+xoffs) & 0x3ff,(y+yoffs) & 0x3ff);
		}
	}
}




void cps2_state::cps1_render_stars( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
	int offs;
	u8 *stars_rom = m_region_stars->base();

	if (!stars_rom && (m_stars_enabled[0] || m_stars_enabled[1]))
	{
#ifdef MAME_DEBUG
//      popmessage("stars enabled but no stars ROM");
#endif
		return;
	}

	if (m_stars_enabled[0])
	{
		for (offs = 0; offs < m_stars_rom_size / 2; offs++)
		{
			int col = stars_rom[8 * offs + 4];
			if (col != 0x0f)
			{
				int sx = (offs / 256) * 32;
				int sy = (offs % 256);
				sx = (sx - m_stars2x + (col & 0x1f)) & 0x1ff;
				sy = (sy - m_stars2y) & 0xff;
				if (flip_screen())
				{
					sx = 512 - sx;
					sy = 256 - sy;
				}

				col = ((col & 0xe0) >> 1) + (screen.frame_number() / 16 & 0x0f);

				if (cliprect.contains(sx, sy))
					bitmap.pix(sy, sx) = 0xa00 + col;
			}
		}
	}

	if (m_stars_enabled[1])
	{
		for (offs = 0; offs < m_stars_rom_size / 2; offs++)
		{
			int col = stars_rom[8*offs];
			if (col != 0x0f)
			{
				int sx = (offs / 256) * 32;
				int sy = (offs % 256);
				sx = (sx - m_stars1x + (col & 0x1f)) & 0x1ff;
				sy = (sy - m_stars1y) & 0xff;
				if (flip_screen())
				{
					sx = 512 - sx;
					sy = 256 - sy;
				}

				col = ((col & 0xe0) >> 1) + (screen.frame_number() / 16 & 0x0f);

				if (cliprect.contains(sx, sy))
					bitmap.pix(sy, sx) = 0x800 + col;
			}
		}
	}
}


void cps2_state::cps1_render_layer( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int primask )
{
	switch (layer)
	{
		case 0:
			cps1_render_sprites(screen, bitmap, cliprect);
			break;
		case 1:
		case 2:
		case 3:
			m_bg_tilemap[layer - 1]->draw(screen, bitmap, cliprect, TILEMAP_DRAW_LAYER1, primask);
			break;
	}
}

void cps2_state::cps1_render_high_layer( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer )
{
	switch (layer)
	{
		case 0:
			/* there are no high priority sprites */
			break;
		case 1:
		case 2:
		case 3:
			m_bg_tilemap[layer - 1]->draw(screen, m_dummy_bitmap, cliprect, TILEMAP_DRAW_LAYER0, 1);
			break;
	}
}


/***************************************************************************

    Refresh screen

***************************************************************************/

u32 cps2_state::screen_update_cps1(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	int layercontrol, l0, l1, l2, l3;
	int videocontrol = m_cps_a_regs[CPS1_VIDEOCONTROL];

	flip_screen_set(videocontrol & 0x8000);

	layercontrol = m_cps_b_regs[m_layer_control / 2];

	/* Get video memory base registers */
	cps1_get_video_base();

	/* Find the offset of the last sprite in the sprite table */
	cps1_find_last_sprite();

	if (m_cps_version == 2)
		cps2_find_last_sprite();

	cps1_update_transmasks();

	m_bg_tilemap[0]->set_scrollx(0, m_scroll1x);
	m_bg_tilemap[0]->set_scrolly(0, m_scroll1y);

	if (videocontrol & 0x01)    /* linescroll enable */
	{
		int scrly = -m_scroll2y;

		m_bg_tilemap[1]->set_scroll_rows(1024);

		int otheroffs = m_cps_a_regs[CPS1_ROWSCROLL_OFFS];

		for (int i = 0; i < 256; i++)
			m_bg_tilemap[1]->set_scrollx((i - scrly) & 0x3ff, m_scroll2x + m_other[(i + otheroffs) & 0x3ff]);
	}
	else
	{
		m_bg_tilemap[1]->set_scroll_rows(1);
		m_bg_tilemap[1]->set_scrollx(0, m_scroll2x);
	}
	m_bg_tilemap[1]->set_scrolly(0, m_scroll2y);
	m_bg_tilemap[2]->set_scrollx(0, m_scroll3x);
	m_bg_tilemap[2]->set_scrolly(0, m_scroll3y);


	/* Blank screen */
	if (m_cps_version == 1)
	{
		// CPS1 games use pen 0xbff as background color; this is used in 3wonders,
		// mtwins (explosion during attract), mercs (intermission).
		bitmap.fill(0xbff, cliprect);
	}
	else
	{
		// CPS2 apparently always force the background to black. Several games would
		// should a blue screen during boot if we used the same code as CPS1.
		// Maybe Capcom changed the background handling due to the problems that
		// it caused on several monitors (because the background extended into the
		// blanking area instead of going black, causing the monitor to clip).
		bitmap.fill(m_palette->black_pen(), cliprect);
	}

	if (m_region_stars)
	{
		cps1_render_stars(screen, bitmap, cliprect);
	}

	/* Draw layers (0 = sprites, 1-3 = tilemaps) */
	l0 = (layercontrol >> 0x06) & 03;
	l1 = (layercontrol >> 0x08) & 03;
	l2 = (layercontrol >> 0x0a) & 03;
	l3 = (layercontrol >> 0x0c) & 03;
	screen.priority().fill(0, cliprect);

	if (m_cps_version == 1)
	{
		cps1_render_layer(screen, bitmap, cliprect, l0, 0);

		if (l1 == 0)
			cps1_render_high_layer(screen, bitmap, cliprect, l0); /* prepare mask for sprites */

		cps1_render_layer(screen, bitmap, cliprect, l1, 0);

		if (l2 == 0)
			cps1_render_high_layer(screen, bitmap, cliprect, l1); /* prepare mask for sprites */

		cps1_render_layer(screen, bitmap, cliprect, l2, 0);

		if (l3 == 0)
			cps1_render_high_layer(screen, bitmap, cliprect, l2); /* prepare mask for sprites */

		cps1_render_layer(screen, bitmap, cliprect, l3, 0);
	}
	else
	{
		int l0pri, l1pri, l2pri, l3pri;
		int primasks[8], i;
		l0pri = (m_pri_ctrl >> 4 * l0) & 0x0f;
		l1pri = (m_pri_ctrl >> 4 * l1) & 0x0f;
		l2pri = (m_pri_ctrl >> 4 * l2) & 0x0f;
		l3pri = (m_pri_ctrl >> 4 * l3) & 0x0f;

#if 0
if (    (m_output[CPS2_OBJ_BASE /2] != 0x7080 && m_output[CPS2_OBJ_BASE /2] != 0x7000) ||
		m_output[CPS2_OBJ_UK1 /2] != 0x807d ||
		(m_output[CPS2_OBJ_UK2 /2] != 0x0000 && m_output[CPS2_OBJ_UK2 /2] != 0x1101 && m_output[CPS2_OBJ_UK2 /2] != 0x0001))
	popmessage("base %04x uk1 %04x uk2 %04x",
			m_output[CPS2_OBJ_BASE /2],
			m_output[CPS2_OBJ_UK1 /2],
			m_output[CPS2_OBJ_UK2 /2]);

if (0 && machine().input().code_pressed(KEYCODE_Z))
	popmessage("order: %d (%d) %d (%d) %d (%d) %d (%d)",l0,l0pri,l1,l1pri,l2,l2pri,l3,l3pri);
#endif

		/* take out the CPS1 sprites layer */
		if (l0 == 0) { l0 = l1; l1 = 0; l0pri = l1pri; }
		if (l1 == 0) { l1 = l2; l2 = 0; l1pri = l2pri; }
		if (l2 == 0) { l2 = l3; l3 = 0; l2pri = l3pri; }

		{
			int mask0 = 0xaa;
			int mask1 = 0xcc;
			if (l0pri > l1pri) mask0 &= ~0x88;
			if (l0pri > l2pri) mask0 &= ~0xa0;
			if (l1pri > l2pri) mask1 &= ~0xc0;

			primasks[0] = 0xff;
			for (i = 1; i < 8; i++)
			{
				if (i <= l0pri && i <= l1pri && i <= l2pri)
				{
					primasks[i] = 0xfe;
					continue;
				}
				primasks[i] = 0;
				if (i <= l0pri) primasks[i] |= mask0;
				if (i <= l1pri) primasks[i] |= mask1;
				if (i <= l2pri) primasks[i] |= 0xf0;
			}
		}

		cps1_render_layer(screen, bitmap, cliprect, l0, 1);
		cps1_render_layer(screen, bitmap, cliprect, l1, 2);
		cps1_render_layer(screen, bitmap, cliprect, l2, 4);
		cps2_render_sprites(screen, bitmap, cliprect, primasks);
	}

	return 0;
}

WRITE_LINE_MEMBER(cps2_state::screen_vblank_cps1)
{
	// rising edge
	if (state)
	{
		/* Get video memory base registers */
		cps1_get_video_base();

		if (m_cps_version == 1)
		{
			/* CPS1 sprites have to be delayed one frame */
			memcpy(m_buffered_obj.get(), m_obj, m_obj_size);
		}
	}
}

void cps2_state::cps2_set_sprite_priorities()
{
	m_pri_ctrl = m_output[CPS2_OBJ_PRI /2];
}

void cps2_state::cps2_objram_latch()
{
	cps2_set_sprite_priorities();
	memcpy(m_cps2_buffered_obj.get(), cps2_objbase(), m_cps2_obj_size);
}
