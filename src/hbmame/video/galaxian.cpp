// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/video/galaxian.cpp"


/***************************************************************************

  HBMAME Video support for extra games

***************************************************************************/





/*************************************
 *
 *  Four Play extensions
 *
 *************************************/

/* gfxbank[4] is used as a cpu bank number, and gfxbank[0] for graphics banking */ 
WRITE8_MEMBER( galaxian_state::fourplay_rombank_w )
{
	m_gfxbank[4] = (m_gfxbank[4] & (2 - offset)) | (data << offset);

	m_gfxbank[0] = (m_gfxbank[4] == 3);	// 1 = true, 0 = false

	membank("bank1")->set_entry( m_gfxbank[4] );
}



/*************************************
 *
 *  Video Eight extensions
 *
 *************************************/

void galaxian_state::videight_extend_tile_info(UINT16 *code, UINT8 *color, UINT8 attrib, UINT8 x)
{
	*code |= (m_gfxbank[0] << 8);
	*color |= (m_gfxbank[4] << 3);
}

void galaxian_state::videight_extend_sprite_info(const UINT8 *base, UINT8 *sx, UINT8 *sy, UINT8 *flipx, UINT8 *flipy, UINT16 *code, UINT8 *color)
{
	*code |= (m_gfxbank[0] << 6);
	*color |= (m_gfxbank[4] << 3);
}


/* This handles the main bankswitching for code and one-bank gfx games */
WRITE8_MEMBER( galaxian_state::videight_rombank_w )
{
	UINT8 gfxbanks[] = { 0, 10, 2, 8, 1, 9, 4, 11 };
	if (offset == 2)
		galaxian_stars_enable_w( space, 0, data );
	else
	{
		m_gfxbank[4] = (m_gfxbank[4] & (6 - offset)) | (data << ((offset + 1) >> 1));
		galaxian_gfxbank_w (space, 0, gfxbanks[m_gfxbank[4]]);
		membank("bank1")->set_entry( m_gfxbank[4] );
	}
}

/* This handles those games with multiple gfx banks */
WRITE8_MEMBER( galaxian_state::videight_gfxbank_w )
{
	/* Moon Cresta (mooncrgx) */
	if (( data < 2 ) && (m_gfxbank[4] == 3))
	{
		UINT8 gfxbanks[] = { 8, 12, 8, 14, 8, 13, 8, 15 };
		if (!offset) m_gfxbank[3] = (m_gfxbank[3] & 6) | data;
		if (offset == 1) m_gfxbank[3] = (m_gfxbank[3] & 5) | (data << 1);
		if (offset == 2) m_gfxbank[3] = (m_gfxbank[3] & 3) | (data << 2);
		galaxian_gfxbank_w(space, 0, gfxbanks[m_gfxbank[3]]);
	}

	/* Uniwar S */
	if (( data < 2 ) && (m_gfxbank[4] == 2) && (offset == 2))
		galaxian_gfxbank_w( space, 0, data+2 );

	/* Pisces (piscesb) */
	if (( data < 2 ) && (m_gfxbank[4] == 6) && (offset == 2))
		galaxian_gfxbank_w( space, 0, data+4 );
}
