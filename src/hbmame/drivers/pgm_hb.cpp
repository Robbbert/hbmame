// license:GPL_2.0
// copyright-holders:Robbbert, David Haywood, ElSemi

#include "emu.h"

#include "cpu/m68000/m68000.h"
#include "cpu/z80/z80.h"
#include "machine/gen_latch.h"
#include "machine/nvram.h"
#include "machine/timer.h"
#include "machine/v3021.h"
#include "sound/ics2115.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"
#include "tilemap.h"

class pgm_hb : public driver_device
{
public:
	pgm_hb(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_mainram(*this, "sram")
		, m_maincpu(*this, "maincpu")
		, m_videoregs(*this, "videoregs")
		, m_videoram(*this, "videoram")
		, m_z80_mainram(*this, "z80_mainram")
		, m_soundcpu(*this, "soundcpu")
		, m_gfxdecode(*this, "gfxdecode")
		, m_palette(*this, "palette")
		, m_soundlatch(*this, "soundlatch")
		, m_soundlatch3(*this, "soundlatch3")
		, m_ics(*this, "ics")
		, m_adata(*this, "sprcol")
		, m_bdata(*this, "sprmask")
	{
		m_irq4_disabled = 0;
	}

	void init_pgm();

	void pgm(machine_config &config);

protected:
	virtual void machine_reset() override;
	virtual void video_start() override;

	/* memory pointers */
	required_shared_ptr<u16> m_mainram;

	/* devices */
	required_device<cpu_device> m_maincpu;

	/* hack */
	int m_irq4_disabled = 0;

	void pgm_base_mem(address_map &map);
	void pgm_mem(address_map &map);

private:
	/* memory pointers */
	required_shared_ptr<u16> m_videoregs;
	required_shared_ptr<u16> m_videoram;
	required_shared_ptr<u8>  m_z80_mainram;
	u16 *                    m_bg_videoram = nullptr;
	u16 *                    m_tx_videoram = nullptr;
	u16 *                    m_rowscrollram = nullptr;

	/* video-related */
	struct sprite_t
	{
		int x = 0, y = 0;
		bool xgrow = false, ygrow = false;
		u32 xzoom = 0, yzoom = 0;
		u32 color = 0, offs = 0;
		u32 width = 0, height = 0;
		u8 flip = 0, pri = 0;
	};

	std::unique_ptr<sprite_t[]> m_spritelist;
	struct sprite_t *m_sprite_ptr_pre = nullptr;
	tilemap_t     *m_bg_tilemap = nullptr;
	tilemap_t     *m_tx_tilemap = nullptr;

	/* devices */
	required_device<cpu_device>             m_soundcpu;
	required_device<gfxdecode_device>       m_gfxdecode;
	required_device<palette_device>         m_palette;
	required_device<generic_latch_8_device> m_soundlatch;
	required_device<generic_latch_8_device> m_soundlatch3;
	required_device<ics2115_device>         m_ics;

	/* used by rendering */
	required_region_ptr<u16> m_adata;
	required_region_ptr<u16> m_bdata;
	u32 m_aoffset = 0;
	u8 m_abit = 0;
	u32 m_boffset = 0;

	u16 videoram_r(offs_t offset);
	void videoram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void coin_counter_w(u16 data);
	u8 z80_ram_r(offs_t offset);
	void z80_ram_w(offs_t offset, u8 data);
	void z80_reset_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void z80_ctrl_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void m68k_l1_w(u8 data);
	void z80_l3_w(u8 data);
	void tx_videoram_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void bg_videoram_w(offs_t offset, u16 data, u16 mem_mask = ~0);

	TILE_GET_INFO_MEMBER(get_tx_tile_info);
	TILE_GET_INFO_MEMBER(get_bg_tile_info);
	u32 screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void screen_vblank(int);
	TIMER_DEVICE_CALLBACK_MEMBER(interrupt);

	inline void pgm_draw_pix(int xdrawpos, int pri, u16* dest, u8* destpri, const rectangle &cliprect, u16 srcdat);
	inline void pgm_draw_pix_nopri(int xdrawpos, u16* dest, u8* destpri, const rectangle &cliprect, u16 srcdat);
	inline void pgm_draw_pix_pri(int xdrawpos, u16* dest, u8* destpri, const rectangle &cliprect, u16 srcdat);
	inline u8 get_sprite_pix();
	void draw_sprite_line(int wide, u16* dest, u8* destpri, const rectangle &cliprect, int xzoom, bool xgrow, int flip, int xpos, int pri, int realxsize, int palt, bool draw);
	void draw_sprite_new_zoomed(int wide, int high, int xpos, int ypos, int palt, int flip, bitmap_ind16 &bitmap, const rectangle &cliprect, bitmap_ind8 &priority_bitmap, u32 xzoom, bool xgrow, u32 yzoom, bool ygrow, int pri);
	void draw_sprite_line_basic(int wide, u16* dest, u8* destpri, const rectangle &cliprect, int flip, int xpos, int pri, int realxsize, int palt, bool draw);
	void draw_sprite_new_basic(int wide, int high, int xpos, int ypos, int palt, int flip, bitmap_ind16 &bitmap, const rectangle &cliprect, bitmap_ind8 &priority_bitmap, int pri);
	void draw_sprites(bitmap_ind16& spritebitmap, const rectangle &cliprect, bitmap_ind8& priority_bitmap);
	void get_sprites();
	void pgm_basic_mem(address_map &map);
	void pgm_z80_io(address_map &map);
	void pgm_z80_mem(address_map &map);
};

static constexpr bool get_flipy(u8 flip) { return BIT(flip, 1); }
static constexpr bool get_flipx(u8 flip) { return BIT(flip, 0); }

inline void pgm_hb::pgm_draw_pix(int xdrawpos, int pri, u16* dest, u8* destpri, const rectangle &cliprect, u16 srcdat)
{
	if ((xdrawpos >= cliprect.min_x) && (xdrawpos <= cliprect.max_x))
	{
		if (!(destpri[xdrawpos] & 1))
		{
			if (!pri)
			{
				dest[xdrawpos] = srcdat;
			}
			else
			{
				if (!(destpri[xdrawpos] & 2))
				{
					dest[xdrawpos] = srcdat;
				}
			}
		}

		destpri[xdrawpos] |= 1;
	}
}

inline void pgm_hb::pgm_draw_pix_nopri(int xdrawpos, u16* dest, u8* destpri, const rectangle &cliprect, u16 srcdat)
{
	if ((xdrawpos >= cliprect.min_x) && (xdrawpos <= cliprect.max_x))
	{
		if (!(destpri[xdrawpos] & 1))
		{
			dest[xdrawpos] = srcdat;
		}
		destpri[xdrawpos] |= 1;
	}
}

inline void pgm_hb::pgm_draw_pix_pri(int xdrawpos, u16* dest, u8* destpri, const rectangle &cliprect, u16 srcdat)
{
	if ((xdrawpos >= cliprect.min_x) && (xdrawpos <= cliprect.max_x))
	{
		if (!(destpri[xdrawpos] & 1))
		{
			if (!(destpri[xdrawpos] & 2))
			{
				dest[xdrawpos] = srcdat;
			}
		}
		destpri[xdrawpos] |= 1;
	}
}

inline u8 pgm_hb::get_sprite_pix()
{
	const u8 srcdat = ((m_adata[m_aoffset & (m_adata.length() - 1)] >> m_abit) & 0x1f);
	m_abit += 5; // 5 bit per pixel, 3 pixels in each word; 15 bit used
	if (m_abit >= 15)
	{
		m_aoffset++;
		m_abit = 0;
	}
	return srcdat;
}

/*************************************************************************
 Full Sprite Renderer
  for complex zoomed cases
*************************************************************************/

void pgm_hb::draw_sprite_line(int wide, u16* dest, u8* destpri, const rectangle &cliprect, int xzoom, bool xgrow, int flip, int xpos, int pri, int realxsize, int palt, bool draw)
{
	int xoffset = 0;
	int xdrawpos = 0;
	int xcntdraw = 0;

	for (int xcnt = 0; xcnt < wide; xcnt++)
	{
		u16 msk = m_bdata[m_boffset & (m_bdata.length() - 1)];

		for (int x = 0; x < 16; x++)
		{
			if (!(BIT(msk, 0)))
			{
				const u16 srcdat = get_sprite_pix() + palt * 32;

				if (draw)
				{
					const bool xzoombit = BIT(xzoom, xoffset & 0x1f);
					xoffset++;

					if (xzoombit && xgrow)
					{ // double this column

						if (!get_flipx(flip))
							xdrawpos = xpos + xcntdraw;
						else
							xdrawpos = xpos + realxsize - xcntdraw;

						pgm_draw_pix(xdrawpos, pri, dest, destpri, cliprect, srcdat);

						xcntdraw++;

						if (!get_flipx(flip))
							xdrawpos = xpos + xcntdraw;
						else
							xdrawpos = xpos + realxsize - xcntdraw;

						pgm_draw_pix(xdrawpos, pri, dest, destpri, cliprect, srcdat);

						xcntdraw++;
					}
					else if (xzoombit && (!xgrow))
					{
						/* skip this column */
					}
					else //normal column
					{
						if (!get_flipx(flip))
							xdrawpos = xpos + xcntdraw;
						else
							xdrawpos = xpos + realxsize - xcntdraw;

						pgm_draw_pix(xdrawpos, pri, dest, destpri, cliprect, srcdat);

						xcntdraw++;
					}
				}

			}
			else
			{
				const bool xzoombit = BIT(xzoom, xoffset & 0x1f);
				xoffset++;
				if (xzoombit && xgrow) { xcntdraw += 2; }
				else if (xzoombit && (!xgrow)) { }
				else { xcntdraw++; }
			}

			msk >>= 1;
		}

		m_boffset++;
	}
}

void pgm_hb::draw_sprite_new_zoomed(int wide, int high, int xpos, int ypos, int palt, int flip, bitmap_ind16 &bitmap, const rectangle &cliprect, bitmap_ind8 &priority_bitmap, u32 xzoom, bool xgrow, u32 yzoom, bool ygrow, int pri)
{
	int ydrawpos;
	int xcnt = 0;

	m_aoffset = (m_bdata[(m_boffset + 1) & (m_bdata.length() - 1)] << 16) | (m_bdata[(m_boffset + 0) & (m_bdata.length() - 1)] << 0);
	m_aoffset = m_aoffset >> 2;
	m_abit = 0;

	m_boffset += 2;

	int ycnt = 0;
	int ycntdraw = 0;
	int realysize = 0;

	while (ycnt < high)
	{
		const bool yzoombit = BIT(yzoom, ycnt & 0x1f);
		if (yzoombit && ygrow) { realysize += 2; }
		else if (yzoombit && (!ygrow)) { }
		else { realysize++; };

		ycnt++;
	}
	realysize--;

	int realxsize = 0;

	while (xcnt < wide * 16)
	{
		const bool xzoombit = BIT(xzoom, xcnt & 0x1f);
		if (xzoombit && xgrow) { realxsize += 2; }
		else if (xzoombit && (!xgrow)) { }
		else { realxsize++; };

		xcnt++;
	}
	realxsize--;

	/* now draw it */
	ycnt = 0;
	ycntdraw = 0;

	while (ycnt < high)
	{
		const bool yzoombit = BIT(yzoom, ycnt & 0x1f);

		if (yzoombit && ygrow) // double this line
		{
			const int temp_aoffset = m_aoffset;
			const int temp_abit = m_abit;
			const int temp_boffset = m_boffset;

			if (!get_flipy(flip))
				ydrawpos = ypos + ycntdraw;
			else
				ydrawpos = ypos + realysize - ycntdraw;

			if ((ydrawpos >= cliprect.min_y) && (ydrawpos <= cliprect.max_y))
			{
				u16 *dest = &bitmap.pix(ydrawpos);
				u8 *destpri = &priority_bitmap.pix(ydrawpos);
				draw_sprite_line(wide, dest, destpri, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, true);
			}
			else
			{
				draw_sprite_line(wide, nullptr, nullptr, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, false);
			}

			ycntdraw++;

			// we need to draw this line again, so restore our pointers to previous values
			m_aoffset = temp_aoffset;
			m_abit = temp_abit;
			m_boffset = temp_boffset;

			if (!get_flipy(flip))
				ydrawpos = ypos + ycntdraw;
			else
				ydrawpos = ypos + realysize - ycntdraw;

			if ((ydrawpos >= cliprect.min_y) && (ydrawpos <= cliprect.max_y))
			{
				u16 *dest = &bitmap.pix(ydrawpos);
				u8 *destpri = &priority_bitmap.pix(ydrawpos);
				draw_sprite_line(wide, dest, destpri, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, true);
			}
			else
			{
				draw_sprite_line(wide, nullptr, nullptr, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, false);

				if (!get_flipy(flip))
				{
					if (ydrawpos >= cliprect.max_y)
						return;
				}
				else
				{
					if (ydrawpos < cliprect.min_y)
						return;
				}
			}

			ycntdraw++;

		}
		else if (yzoombit && (!ygrow))
		{
			/* skip this line */
			draw_sprite_line(wide, nullptr, nullptr, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, false);
		}
		else /* normal line */
		{
			if (!get_flipy(flip))
				ydrawpos = ypos + ycntdraw;
			else
				ydrawpos = ypos + realysize - ycntdraw;

			if ((ydrawpos >= cliprect.min_y) && (ydrawpos <= cliprect.max_y))
			{
				u16 *dest = &bitmap.pix(ydrawpos);
				u8 *destpri = &priority_bitmap.pix(ydrawpos);
				draw_sprite_line(wide, dest, destpri, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, true);
			}
			else
			{
				draw_sprite_line(wide, nullptr, nullptr, cliprect, xzoom, xgrow, flip, xpos, pri, realxsize, palt, false);

				if (!get_flipy(flip))
				{
					if (ydrawpos >= cliprect.max_y)
						return;
				}
				else
				{
					if (ydrawpos < cliprect.min_y)
						return;
				}

			}

			ycntdraw++;
		}

		ycnt++;
	}
}


void pgm_hb::draw_sprite_line_basic(int wide, u16* dest, u8* destpri, const rectangle &cliprect, int flip, int xpos, int pri, int realxsize, int palt, bool draw)
{
	int xdrawpos = 0;
	int xcntdraw = 0;

	if (!pri)
	{
		for (int xcnt = 0; xcnt < wide; xcnt++)
		{
			u16 msk = m_bdata[m_boffset & (m_bdata.length() - 1)];

			for (int x = 0; x < 16; x++)
			{
				if (!(BIT(msk, 0)))
				{
					const u16 srcdat = get_sprite_pix() + palt * 32;

					if (draw)
					{
						if (!get_flipx(flip))
							xdrawpos = xpos + xcntdraw;
						else
							xdrawpos = xpos + realxsize - xcntdraw;

						pgm_draw_pix_nopri(xdrawpos, dest, destpri, cliprect, srcdat);

						xcntdraw++;
					}

				}
				else
				{
					xcntdraw++;
				}

				msk >>= 1;
			}

			m_boffset++;
		}
	}
	else
	{
		for (int xcnt = 0; xcnt < wide; xcnt++)
		{
			u16 msk = m_bdata[m_boffset & (m_bdata.length() - 1)];

			for (int x = 0; x < 16; x++)
			{
				if (!(BIT(msk, 0)))
				{
					const u16 srcdat = get_sprite_pix() + palt * 32;

					if (draw)
					{
						if (!get_flipx(flip))
							xdrawpos = xpos + xcntdraw;
						else
							xdrawpos = xpos + realxsize - xcntdraw;

						pgm_draw_pix_pri(xdrawpos, dest, destpri, cliprect, srcdat);

						xcntdraw++;
					}

				}
				else
				{
					xcntdraw++;
				}

				msk >>= 1;
			}

			m_boffset++;
		}
	}
}

/*************************************************************************
 Basic Sprite Renderer
  simplified version for non-zoomed cases, a bit faster
*************************************************************************/

void pgm_hb::draw_sprite_new_basic(int wide, int high, int xpos, int ypos, int palt, int flip, bitmap_ind16 &bitmap, const rectangle &cliprect, bitmap_ind8 &priority_bitmap, int pri)
{
	int ydrawpos;

	m_aoffset = (m_bdata[(m_boffset + 1) & (m_bdata.length() - 1)] << 16) | (m_bdata[(m_boffset + 0) & (m_bdata.length() - 1)] << 0);
	m_aoffset = m_aoffset >> 2;
	m_abit = 0;

	m_boffset += 2;

	const int realysize = high - 1;
	const int realxsize = (wide * 16) - 1;

	/* now draw it */
	int ycnt = 0;
	int ycntdraw = 0;

	while (ycnt < high)
	{
		if (!get_flipy(flip))
			ydrawpos = ypos + ycntdraw;
		else
			ydrawpos = ypos + realysize - ycntdraw;

		if ((ydrawpos >= cliprect.min_y) && (ydrawpos <= cliprect.max_y))
		{
			u16 *dest = &bitmap.pix(ydrawpos);
			u8 *destpri = &priority_bitmap.pix(ydrawpos);
			draw_sprite_line_basic(wide, dest, destpri, cliprect, flip, xpos, pri, realxsize, palt, true);
		}
		else
		{
			draw_sprite_line_basic(wide, nullptr, nullptr, cliprect, flip, xpos, pri, realxsize, palt, false);

			if (!get_flipy(flip))
			{
				if (ydrawpos >= cliprect.max_y)
					return;
			}
			else
			{
				if (ydrawpos < cliprect.min_y)
					return;
			}
		}

		ycntdraw++;
		ycnt++;
	}
}


void pgm_hb::draw_sprites(bitmap_ind16& spritebitmap, const rectangle &cliprect, bitmap_ind8& priority_bitmap)
{
	struct sprite_t *sprite_ptr = m_sprite_ptr_pre;
	while (sprite_ptr != m_spritelist.get())
	{
		sprite_ptr--;

		m_boffset = sprite_ptr->offs;
		if ((!sprite_ptr->xzoom) && (!sprite_ptr->yzoom))
		{
			draw_sprite_new_basic(sprite_ptr->width, sprite_ptr->height,
				sprite_ptr->x, sprite_ptr->y,
				sprite_ptr->color, sprite_ptr->flip,
				spritebitmap, cliprect, priority_bitmap,
				sprite_ptr->pri);
		}
		else
		{
			draw_sprite_new_zoomed(sprite_ptr->width, sprite_ptr->height,
				sprite_ptr->x, sprite_ptr->y,
				sprite_ptr->color, sprite_ptr->flip,
				spritebitmap, cliprect, priority_bitmap,
				sprite_ptr->xzoom, sprite_ptr->xgrow, sprite_ptr->yzoom, sprite_ptr->ygrow, sprite_ptr->pri);
		}
	}
}

/*
        Sprite list format (10 bytes per sprites, 256 entries)

    Offset Bits
           fedcba98 76543210
    00     x------- -------- Horizontal Zoom/Shrink mode select
           -xxxx--- -------- Horizontal Zoom/Shrink table select
           -----xxx xxxxxxxx X position (11 bit signed)

    02     x------- -------- Vertical Zoom/Shrink mode select
           -xxxx--- -------- Vertical Zoom/Shrink table select
           -----xxx xxxxxxxx Y position (10 bit signed)

    04     -x------ -------- Flip Y
           --x----- -------- Flip X
           ---xxxxx -------- Palette select (32 color each)
           -------- x------- Priority (Over(0) or Under(1) background)
           -------- -xxxxxxx Sprite mask ROM address MSB
    06     xxxxxxxx xxxxxxxx Sprite mask ROM address LSB

    08     x------- -------- Another sprite width bit?
           -xxxxxx- -------- Sprite width (16 pixel each)
           -------x xxxxxxxx Sprite height (1 pixel each)

*/
void pgm_hb::get_sprites()
{
	m_sprite_ptr_pre = m_spritelist.get();

	u16 *sprite_source = &m_mainram[0];
	const u16 *finish = &m_mainram[0xa00 / 2];
	const u16* sprite_zoomtable = &m_videoregs[0x1000 / 2];

	while (sprite_source < finish)
	{
		if (!sprite_source[4]) break; /* is this right? */

		int xzom =                 (sprite_source[0] & 0x7800) >> 11;
		const bool xgrow =         (sprite_source[0] & 0x8000) >> 15;
		m_sprite_ptr_pre->x =      (sprite_source[0] & 0x03ff) - (sprite_source[0] & 0x0400);

		int yzom =                 (sprite_source[1] & 0x7800) >> 11;
		const bool ygrow =         (sprite_source[1] & 0x8000) >> 15;
		m_sprite_ptr_pre->y =      (sprite_source[1] & 0x01ff) - (sprite_source[1] & 0x0200);

		m_sprite_ptr_pre->flip =   (sprite_source[2] & 0x6000) >> 13;
		m_sprite_ptr_pre->color =  (sprite_source[2] & 0x1f00) >> 8;
		m_sprite_ptr_pre->pri =    (sprite_source[2] & 0x0080) >>  7;
		m_sprite_ptr_pre->offs =  ((sprite_source[2] & 0x007f) << 16) | (sprite_source[3] & 0xffff);

		m_sprite_ptr_pre->width =  (sprite_source[4] & 0x7e00) >> 9;
		m_sprite_ptr_pre->height =  sprite_source[4] & 0x01ff;

		if (xgrow)
		{
		//  xzom = 0xf - xzom; // would make more sense but everything gets zoomed slightly in dragon world 2 ?!
			xzom = 0x10 - xzom; // this way it doesn't but there is a bad line when zooming after the level select?
		}

		if (ygrow)
		{
		//  yzom = 0xf - yzom; // see comment above
			yzom = 0x10 - yzom;
		}

		m_sprite_ptr_pre->xzoom = (sprite_zoomtable[xzom * 2] << 16) | sprite_zoomtable[xzom * 2 + 1];
		m_sprite_ptr_pre->yzoom = (sprite_zoomtable[yzom * 2] << 16) | sprite_zoomtable[yzom * 2 + 1];
		m_sprite_ptr_pre->xgrow = xgrow;
		m_sprite_ptr_pre->ygrow = ygrow;
		m_sprite_ptr_pre++;
		sprite_source += 5;
	}
}

/* TX Layer */
void pgm_hb::tx_videoram_w(offs_t offset, u16 data, u16 mem_mask)
{
	m_tx_videoram[offset] = data;
	m_tx_tilemap->mark_tile_dirty(offset / 2);
}

TILE_GET_INFO_MEMBER(pgm_hb::get_tx_tile_info)
{
/* 0x904000 - 0x90ffff is the Text Overlay Ram (pgm_tx_videoram)
    each tile uses 4 bytes, the tilemap is 64x128?

   the layer uses 4bpp 8x8 tiles from the 'T' roms
   colours from 0xA01000 - 0xA017FF

   scroll registers are at 0xB05000 (Y) and 0xB06000 (X)

    ---- ---- ffpp ppp- nnnn nnnn nnnn nnnn

    n = tile number
    p = palette
    f = flip
*/
	const u32 tileno = m_tx_videoram[tile_index * 2] & 0xffff;
	const u32 colour = (m_tx_videoram[tile_index * 2 + 1] & 0x3e) >> 1;
	const u8  flipyx = (m_tx_videoram[tile_index * 2 + 1] & 0xc0) >> 6;

	tileinfo.set(0,tileno,colour,TILE_FLIPYX(flipyx));
}

/* BG Layer */

void pgm_hb::bg_videoram_w(offs_t offset, u16 data, u16 mem_mask)
{
	m_bg_videoram[offset] = data;
	m_bg_tilemap->mark_tile_dirty(offset / 2);
}

TILE_GET_INFO_MEMBER(pgm_hb::get_bg_tile_info)
{
	/* pretty much the same as tx layer */

	const u32 tileno = m_bg_videoram[tile_index *2] & 0xffff;
	const u32 colour = (m_bg_videoram[tile_index * 2 + 1] & 0x3e) >> 1;
	const u8  flipyx = (m_bg_videoram[tile_index * 2 + 1] & 0xc0) >> 6;

	tileinfo.set(1,tileno,colour,TILE_FLIPYX(flipyx));
}



/*** Video - Start / Update ****************************************************/

void pgm_hb::video_start()
{
	// assumes it can make an address mask with .length() - 1 on these
	assert(!(m_adata.length() & (m_adata.length() - 1)));
	assert(!(m_bdata.length() & (m_bdata.length() - 1)));

	m_spritelist = std::make_unique<sprite_t[]>(0xa00/2/5);
	m_sprite_ptr_pre = m_spritelist.get();

	m_aoffset = 0;
	m_abit = 0;
	m_boffset = 0;

	m_tx_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(pgm_hb::get_tx_tile_info)), TILEMAP_SCAN_ROWS, 8, 8, 64, 32);
	m_tx_tilemap->set_transparent_pen(15);

	m_bg_tilemap = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(pgm_hb::get_bg_tile_info)), TILEMAP_SCAN_ROWS, 32, 32, 64, 16);
	m_bg_tilemap->set_transparent_pen(31);
	m_bg_tilemap->set_scroll_rows(16 * 32);
}

u32 pgm_hb::screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bitmap.fill(0x3ff, cliprect); // ddp2 igs logo needs 0x3ff

	screen.priority().fill(0, cliprect);

	m_bg_tilemap->set_scrolly(0, m_videoregs[0x2000/2]);

	for (int y = cliprect.min_y; y <= cliprect.max_y; y++)
		m_bg_tilemap->set_scrollx((y + m_videoregs[0x2000 / 2]) & 0x1ff, m_videoregs[0x3000 / 2] + m_rowscrollram[y]);

	m_bg_tilemap->draw(screen, bitmap, cliprect, 0, 2);

	draw_sprites(bitmap, cliprect, screen.priority());

	m_tx_tilemap->set_scrolly(0, m_videoregs[0x5000/2]);
	m_tx_tilemap->set_scrollx(0, m_videoregs[0x6000/2]);

	m_tx_tilemap->draw(screen, bitmap, cliprect, 0, 0);

	return 0;
}

void pgm_hb::screen_vblank(int state)
{
	// rising edge
	if (state)
	{
		/* first 0xa00 of main ram = sprites, seems to be buffered, DMA? */
		get_sprites();

		// vblank start interrupt
		m_maincpu->set_input_line(M68K_IRQ_6, HOLD_LINE);
	}
}

u16 pgm_hb::videoram_r(offs_t offset)
{
	if (offset < 0x4000 / 2)
		return m_bg_videoram[offset & 0x7ff];
	else if (offset < 0x7000 / 2)
		return m_tx_videoram[offset & 0xfff];
	else
		return m_videoram[offset];
}

void pgm_hb::videoram_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (offset < 0x4000 / 2)
		bg_videoram_w(offset & 0x7ff, data, mem_mask);
	else if (offset < 0x7000 / 2)
		tx_videoram_w(offset & 0xfff, data, mem_mask);
	else
		COMBINE_DATA(&m_videoram[offset]);
}

void pgm_hb::coin_counter_w(u16 data)
{
	machine().bookkeeping().coin_counter_w(0, data & 0x0001);
	machine().bookkeeping().coin_counter_w(1, data & 0x0002);
	machine().bookkeeping().coin_counter_w(2, data & 0x0004);
	machine().bookkeeping().coin_counter_w(3, data & 0x0008);
}

u8 pgm_hb::z80_ram_r(offs_t offset)
{
	return m_z80_mainram[offset];
}

void pgm_hb::z80_ram_w(offs_t offset, u8 data)
{
	m_z80_mainram[offset] = data;
}

void pgm_hb::z80_reset_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (data == 0x5050)
	{
		m_ics->reset();
		m_soundcpu->set_input_line(INPUT_LINE_HALT, CLEAR_LINE);
		m_soundcpu->pulse_input_line(INPUT_LINE_RESET, attotime::zero);
	}
	else
	{
		m_soundcpu->set_input_line(INPUT_LINE_HALT, ASSERT_LINE);
	}
}

void pgm_hb::z80_ctrl_w(offs_t offset, u16 data, u16 mem_mask)
{
}

void pgm_hb::m68k_l1_w(u8 data)
{
	m_soundlatch->write(data);
	m_soundcpu->pulse_input_line(INPUT_LINE_NMI, attotime::zero);
}

void pgm_hb::z80_l3_w(u8 data)
{
	m_soundlatch3->write(data);
}


/*** Memory Maps *************************************************************/

/*** Z80 (sound CPU)**********************************************************/

void pgm_hb::pgm_z80_mem(address_map &map)
{
	map(0x0000, 0xffff).ram().share("z80_mainram");
}

void pgm_hb::pgm_z80_io(address_map &map)
{
	map(0x8000, 0x8003).rw("ics", FUNC(ics2115_device::read), FUNC(ics2115_device::write));
	map(0x8100, 0x81ff).r(m_soundlatch3, FUNC(generic_latch_8_device::read)).w(FUNC(pgm_hb::z80_l3_w));
	map(0x8200, 0x82ff).rw(m_soundlatch, FUNC(generic_latch_8_device::read), FUNC(generic_latch_8_device::write));
	map(0x8400, 0x84ff).rw("soundlatch2", FUNC(generic_latch_8_device::read), FUNC(generic_latch_8_device::write));
}

/*** 68000 (main CPU) + variants for protection devices **********************/

void pgm_hb::pgm_base_mem(address_map &map)
{
	map(0x700006, 0x700007).nopw(); // Watchdog?

	map(0x800000, 0x81ffff).ram().mirror(0x0e0000).share("sram"); /* Main Ram */

	map(0x900000, 0x907fff).mirror(0x0f8000).rw(FUNC(pgm_hb::videoram_r), FUNC(pgm_hb::videoram_w)).share("videoram"); /* IGS023 VIDEO CHIP */
	map(0xa00000, 0xa011ff).ram().w(m_palette, FUNC(palette_device::write16)).share("palette");
	map(0xb00000, 0xb0ffff).ram().share("videoregs"); /* Video Regs inc. Zoom Table */

	map(0xc00003, 0xc00003).r(m_soundlatch, FUNC(generic_latch_8_device::read)).w(FUNC(pgm_hb::m68k_l1_w));
	map(0xc00005, 0xc00005).rw("soundlatch2", FUNC(generic_latch_8_device::read), FUNC(generic_latch_8_device::write));
	map(0xc00007, 0xc00007).rw("rtc", FUNC(v3021_device::read), FUNC(v3021_device::write));
	map(0xc00008, 0xc00009).w(FUNC(pgm_hb::z80_reset_w));
	map(0xc0000a, 0xc0000b).w(FUNC(pgm_hb::z80_ctrl_w));
	map(0xc0000d, 0xc0000d).rw(m_soundlatch3, FUNC(generic_latch_8_device::read), FUNC(generic_latch_8_device::write));

	map(0xc08000, 0xc08001).portr("P1P2");
	map(0xc08002, 0xc08003).portr("P3P4");
	map(0xc08004, 0xc08005).portr("Service");
	map(0xc08006, 0xc08007).portr("DSW").w(FUNC(pgm_hb::coin_counter_w));

	map(0xc10000, 0xc1ffff).rw(FUNC(pgm_hb::z80_ram_r), FUNC(pgm_hb::z80_ram_w)); /* Z80 Program */
}

void pgm_hb::pgm_mem(address_map &map)
{
	pgm_base_mem(map);
	map(0x000000, 0x0fffff).rom();   /* BIOS ROM */
}

void pgm_hb::pgm_basic_mem(address_map &map)
{
	pgm_mem(map);
	map(0x100000, 0x3fffff).bankr("bank1"); /* Game ROM */
}


/*** Input Ports *************************************************************/

/* enough for 4 players, the basic dips mapped are listed in the test mode */

INPUT_PORTS_START( pgm_hb )
	PORT_START("P1P2")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1)
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2)

	PORT_START("P3P4")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_START3 )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_PLAYER(3)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_PLAYER(3)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_PLAYER(3)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(3)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(3)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(3)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(3)
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_START4 )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_PLAYER(4)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_PLAYER(4)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_PLAYER(4)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(4)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(4)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(4)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(4)

	PORT_START("Service")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_COIN3 )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_COIN4 )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_SERVICE3 ) PORT_NAME("Test-Key 1P+2P")
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Service 1P+2P")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_SERVICE4 ) PORT_NAME("Test-Key 3P+4P")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Service 3P+4P")
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(3)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(4)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_UNKNOWN )  // unused?
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_UNKNOWN )  // unused?
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )  // unused?
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )  // unused?

	PORT_START("DSW")
	PORT_SERVICE( 0x0001, IP_ACTIVE_LOW )
	PORT_DIPNAME( 0x0002, 0x0002, "Music" )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( On ) )
	PORT_DIPNAME( 0x0004, 0x0004, "Voice" )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( On ) )
	PORT_DIPNAME( 0x0008, 0x0008, "Free" )
	PORT_DIPSETTING(      0x0008, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0010, 0x0010, "Stop" )
	PORT_DIPSETTING(      0x0010, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0020, 0x0020, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0040, 0x0040, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0080, 0x0080, DEF_STR( Unused ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )

	PORT_START("Region")
	PORT_BIT( 0xffff, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END


/*** GFX Decodes *************************************************************/

/* We can't decode the sprite data like this because it isn't tile based.
   Note that the bit indexes are reversed compared to usual gfx layouts
   (0-7 = LSB to MSB) */

static const gfx_layout pgm32_charlayout =
{
	32,32,
	RGN_FRAC(1,1),
	5,
	{ 4, 3, 2, 1, 0 },
	{ STEP32(0,5) },
	{ STEP32(0,5*32) },
	32*32*5
};

GFXDECODE_START( gfx_pgm )
	GFXDECODE_ENTRY(       "tiles", 0, gfx_8x8x4_packed_lsb, 0x800, 32 ) /* 8x8x4 Tiles */
	GFXDECODE_REVERSEBITS( "tiles", 0, pgm32_charlayout,     0x400, 32 ) /* 32x32x5 Tiles */
GFXDECODE_END

/*** Machine Driver **********************************************************/

TIMER_DEVICE_CALLBACK_MEMBER(pgm_hb::interrupt)
{
	int scanline = param;

//  vblank end interrupt
	if (scanline == 0)
		if (!m_irq4_disabled) m_maincpu->set_input_line(4, HOLD_LINE);
}

void pgm_hb::machine_reset()
{
	m_soundcpu->set_input_line(INPUT_LINE_HALT, ASSERT_LINE);
}

void pgm_hb::pgm(machine_config &config)
{
	/* basic machine hardware */
	M68000(config, m_maincpu, 20_MHz_XTAL); /* 20 mhz! verified on real board */
	m_maincpu->set_addrmap(AS_PROGRAM, &pgm_hb::pgm_basic_mem);
	TIMER(config, "scantimer").configure_scanline(FUNC(pgm_hb::interrupt), "screen", 0, 1);

	Z80(config, m_soundcpu, 33.8688_MHz_XTAL/4);
	m_soundcpu->set_addrmap(AS_PROGRAM, &pgm_hb::pgm_z80_mem);
	m_soundcpu->set_addrmap(AS_IO, &pgm_hb::pgm_z80_io);

	NVRAM(config, "sram", nvram_device::DEFAULT_ALL_0);

	V3021(config, "rtc");

	/* video hardware */
	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER));
	screen.set_raw(50_MHz_XTAL/5, 640, 0, 448, 264, 0, 224); // or 20MHz / 2? framerate verified
	screen.set_screen_update(FUNC(pgm_hb::screen_update));
	screen.screen_vblank().set(FUNC(pgm_hb::screen_vblank));
	screen.set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_pgm);
	PALETTE(config, m_palette, palette_device::BLACK).set_format(palette_device::xRGB_555, 0x1200/2);

	/*sound hardware */
	SPEAKER(config, "mono").front_center();

	GENERIC_LATCH_8(config, m_soundlatch);
	GENERIC_LATCH_8(config, "soundlatch2");
	GENERIC_LATCH_8(config, m_soundlatch3);

	ICS2115(config, m_ics, 33.8688_MHz_XTAL);
	m_ics->irq().set_inputline("soundcpu", 0);
	m_ics->add_route(ALL_OUTPUTS, "mono", 2.0); // HBMAME - wind the volume up to 11
}

void pgm_hb::init_pgm()
{
	u8 *ROM = memregion("maincpu")->base();
	membank("bank1")->set_base(&ROM[0x100000]);

	m_bg_videoram = &m_videoram[0];
	m_tx_videoram = &m_videoram[0x4000/2];
	m_rowscrollram = &m_videoram[0x7000/2];
}

/*** Rom Loading *************************************************************/

/* take note of "sprmask" needed for expanding the Sprite Colour Data */

#define ROM_LOAD16_WORD_SWAP_BIOS(bios,name,offset,length,hash) \
		ROMX_LOAD(name, offset, length, hash, ROM_GROUPWORD | ROM_REVERSE | ROM_BIOS(bios))

#define PGM_68K_BIOS \
	ROM_SYSTEM_BIOS( 0, "v2",     "PGM Bios V2" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0, "pgm_p02s.u20",    0x00000, 0x020000, CRC(78c15fa2) SHA1(885a6558e022602cc6f482ac9667ba9f61e75092) ) /* Version 2 (Label: IGS | PGM P02S | 1P0792D1 | J992438 )*/ \
	ROM_SYSTEM_BIOS( 1, "v1",     "PGM Bios V1" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1, "pgm_p01s.u20",    0x00000, 0x020000, CRC(e42b166e) SHA1(2a9df9ec746b14b74fae48b1a438da14973702ea) ) /* Version 1 */
#define PGM_AUDIO_BIOS \
	ROM_LOAD( "pgm_m01s.rom", 0x000000, 0x200000, CRC(45ae7159) SHA1(d3ed3ff3464557fd0df6b069b2e431528b0ebfa8) )
#define PGM_VIDEO_BIOS \
	ROM_LOAD( "pgm_t01s.rom", 0x000000, 0x200000, CRC(1a7123a0) SHA1(cc567f577bfbf45427b54d6695b11b74f2578af3) )

/**********
 Homebrew
***********/
ROM_START( pgemeni )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgemeni.p1",  0x100000, 0x200000, CRC(6cafa56b) SHA1(0b3447357cf9dc10e9473102a109a2c9f63be66b) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgemeni.t1",          0x180000, 0x400000, CRC(42b979dd) SHA1(38ae8a7e07f1169b876bd41e0a9bfe19ecb0256f) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgemeni.a1",          0x0000000, 0x400000, CRC(105d7cee) SHA1(43b19032e9e3a1aeabd7ce11438c6658ac4b5dd0) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgemeni.b1",          0x0000000, 0x400000, CRC(b4127373) SHA1(fa731bedec8ff4c19ec68bd44e8ce0b80ec0c716) )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmdemo.m1",          0x400000, 0x200000, CRC(8d89877e) SHA1(7d76d48d64d7ac5411d714a4bb83f37e3e5b8df6) )
ROM_END

ROM_START( pgmdemo )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmdemo.p1", 0x100000, 0x200000, CRC(d3f6ec45) SHA1(4c171ffff6a04b362f71c42de7bc1d7b57da2c0e) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgmdemo.t1",          0x180000, 0x400000, CRC(0596a59a) SHA1(e48cb703e886a5b71ecc839136c642117aa529ef) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgmdemo.a1",          0x0000000, 0x400000, CRC(5d0e8fa1) SHA1(95380456c8a6c9c48d4987951f689d5b9a7a397c) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgmdemo.b1",          0x0000000, 0x400000, CRC(15dd191f) SHA1(a90f63138c9a9bf9215b17a14635518aed7da8a9) )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmdemo.m1",          0x400000, 0x200000, CRC(8d89877e) SHA1(7d76d48d64d7ac5411d714a4bb83f37e3e5b8df6) )
ROM_END

ROM_START( pgmfrog )
	ROM_REGION( 0x600000, "maincpu", 0 )
	PGM_68K_BIOS
	ROM_LOAD16_WORD_SWAP( "pgmfrog.p1",  0x100000, 0x200000, CRC(cdec9e8d) SHA1(183753b3f19e9b06c6059206aac85d865bf1dd66) )

	ROM_REGION( 0x800000, "tiles",  0 )
	PGM_VIDEO_BIOS
	ROM_LOAD( "pgmfrog.t1",          0x180000, 0x400000, CRC(8f58b6d8) SHA1(79b3b503f29593170a7faa93d75707682d800c66) )

	ROM_REGION16_LE( 0x2000000, "sprcol", 0 )
	ROM_LOAD( "pgmfrog.a1",          0x0000000, 0x400000, CRC(dc1eafe6) SHA1(11eb0d02c38df828ae0361bc7624c4658047df67) )

	ROM_REGION16_LE( 0x1000000, "sprmask", 0 )
	ROM_LOAD( "pgmfrog.b1",          0x0000000, 0x400000, CRC(3d44b66f) SHA1(04347002bd942d320c6b8240e4485aec626f2aaa) )

	ROM_REGION( 0x600000, "ics", 0 )
	PGM_AUDIO_BIOS
	ROM_LOAD( "pgmfrog.m1",          0x400000, 0x200000, CRC(05e2f761) SHA1(c93d94a8f11c41b019fcf9b6a90645416fd2c75b) )
ROM_END

/*    YEAR  NAME     PARENT   MACHINE  INPUT  CLASS     INIT     MONITOR COMPANY                 FULLNAME FLAGS */
// Homebrew
GAME( 2006, pgemeni, pgm,  pgm,   pgm_hb,   pgm_hb,   init_pgm,   ROT0,   "Blastar", "P-Gemeni (2006-01-23)", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2005, pgmdemo, pgm,  pgm,   pgm_hb,   pgm_hb,   init_pgm,   ROT0,   "Charles Doty", "Demo - PGM", MACHINE_SUPPORTS_SAVE ) // has no sound
GAME( 2006, pgmfrog, pgm,  pgm,   pgm_hb,   pgm_hb,   init_pgm,   ROT0,   "Rastersoft", "Frog Feast (PGM)", MACHINE_SUPPORTS_SAVE )

