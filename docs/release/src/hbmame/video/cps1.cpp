// license:BSD-3-Clause
// copyright-holders:Paul Leaman
/*****************************************************************************************************************************

Format of the .key files:

All values are little-endian

Offset         Purpose
00              0x55 file header
01              cpsb_addr
02,03           cpsb_value
04              mult_factor1
05              mult_factor2
06              mult_result_lo
07              mult_result_hi
08              layer_control
09              priority[0]
0A              priority[1]
0B              priority[2]
0C              priority[3]
0D              palette_control
0E              layer_enable_mask[0]
0F              layer_enable_mask[1]
10              layer_enable_mask[2]
11              layer_enable_mask[3]
12              layer_enable_mask[4]
13              in2_addr
14              in3_addr
15              out2_addr
16,17           bootleg_kludge
18              scrollx1 offset for bootlegs (negative values are subtracted, e.g 0x81 = subtract 1)
19              scrollx2 offset for bootlegs (as above)
1A              scrollx3 offset for bootlegs (as above)
1B,1C,1D        bank_sizes[0]
1E,1F,20        bank_sizes[1]
21,22,23        bank_sizes[2]
24,25,26        bank_sizes[3]
27              bank_type.type(bits 0-4) and bank_number(bits 6-7)
28,29,2A        bank_type.start
2B,2C,2D        bank_type.end
The bank_type occurs 8 times for the 8 possible combinations
After that, the remainder of the file is binary zeroes.

*****************************************************************************************************************************/
#include "emu.h"
#include "includes/cps1.h"

/********************************************************************

            Configuration table:

********************************************************************/
#define WRITE_FILE 0

/* Game specific data */

#define GFXTYPE_SPRITES   (1<<0)
#define GFXTYPE_SCROLL1   (1<<1)
#define GFXTYPE_SCROLL2   (1<<2)
#define GFXTYPE_SCROLL3   (1<<3)
#define GFXTYPE_STARS     (1<<4)


#define __not_applicable__  -1,-1,-1,-1,-1,-1,-1

/*                     CPSB ID    multiply protection      unknown      ctrl     priority masks   palctrl    layer enable masks  */
#define CPS_B_01      -1, 0x0000,          __not_applicable__,          0x26,{0x28,0x2a,0x2c,0x2e},0x30, {0x02,0x04,0x08,0x30,0x30}
#define CPS_B_02     0x20,0x0002,          __not_applicable__,          0x2c,{0x2a,0x28,0x26,0x24},0x22, {0x02,0x04,0x08,0x00,0x00}
#define CPS_B_03      -1, 0x0000,          __not_applicable__,          0x30,{0x2e,0x2c,0x2a,0x28},0x26, {0x20,0x10,0x08,0x00,0x00}
#define CPS_B_04     0x20,0x0004,          __not_applicable__,          0x2e,{0x26,0x30,0x28,0x32},0x2a, {0x02,0x04,0x08,0x00,0x00}
#define CPS_B_05     0x20,0x0005,          __not_applicable__,          0x28,{0x2a,0x2c,0x2e,0x30},0x32, {0x02,0x08,0x20,0x14,0x14}
#define CPS_B_11     0x32,0x0401,          __not_applicable__,          0x26,{0x28,0x2a,0x2c,0x2e},0x30, {0x08,0x10,0x20,0x00,0x00}
#define CPS_B_12     0x20,0x0402,          __not_applicable__,          0x2c,{0x2a,0x28,0x26,0x24},0x22, {0x02,0x04,0x08,0x00,0x00}
#define CPS_B_13     0x2e,0x0403,          __not_applicable__,          0x22,{0x24,0x26,0x28,0x2a},0x2c, {0x20,0x02,0x04,0x00,0x00}
#define CPS_B_14     0x1e,0x0404,          __not_applicable__,          0x12,{0x14,0x16,0x18,0x1a},0x1c, {0x08,0x20,0x10,0x00,0x00}
#define CPS_B_15     0x0e,0x0405,          __not_applicable__,          0x02,{0x04,0x06,0x08,0x0a},0x0c, {0x04,0x02,0x20,0x00,0x00}
#define CPS_B_16     0x00,0x0406,          __not_applicable__,          0x0c,{0x0a,0x08,0x06,0x04},0x02, {0x10,0x0a,0x0a,0x00,0x00}
#define CPS_B_17     0x08,0x0407,          __not_applicable__,          0x14,{0x12,0x10,0x0e,0x0c},0x0a, {0x08,0x14,0x02,0x00,0x00}   // the sf2 -> strider conversion needs 0x04 for the 2nd layer enable on one level, gfx confirmed to appear on the PCB, register at the time is 0x8e, so 0x10 is not set.
#define CPS_B_18     0x10,0x0408,          __not_applicable__,          0x1c,{0x1a,0x18,0x16,0x14},0x12, {0x10,0x08,0x02,0x00,0x00}
#define CPS_B_21_DEF 0x32,  -1,   0x00,0x02,0x04,0x06, 0x08, -1,  -1,   0x26,{0x28,0x2a,0x2c,0x2e},0x30, {0x02,0x04,0x08,0x30,0x30} // pang3 sets layer enable to 0x26 on startup
#define CPS_B_21_BT1 0x32,0x0800, 0x0e,0x0c,0x0a,0x08, 0x06,0x04,0x02,  0x28,{0x26,0x24,0x22,0x20},0x30, {0x20,0x04,0x08,0x12,0x12}
#define CPS_B_21_BT2  -1,   -1,   0x1e,0x1c,0x1a,0x18,  -1, 0x0c,0x0a,  0x20,{0x2e,0x2c,0x2a,0x28},0x30, {0x30,0x08,0x30,0x00,0x00}
#define CPS_B_21_BT3  -1,   -1,   0x06,0x04,0x02,0x00, 0x0e,0x0c,0x0a,  0x20,{0x2e,0x2c,0x2a,0x28},0x30, {0x20,0x12,0x12,0x00,0x00}
#define CPS_B_21_BT4  -1,   -1,   0x06,0x04,0x02,0x00, 0x1e,0x1c,0x1a,  0x28,{0x26,0x24,0x22,0x20},0x30, {0x20,0x10,0x02,0x00,0x00}
#define CPS_B_21_BT5 0x32,  -1,   0x0e,0x0c,0x0a,0x08, 0x1e,0x1c,0x1a,  0x20,{0x2e,0x2c,0x2a,0x28},0x30, {0x20,0x04,0x02,0x00,0x00}
#define CPS_B_21_BT6  -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x20,{0x2e,0x2c,0x2a,0x28},0x30, {0x20,0x14,0x14,0x00,0x00}
#define CPS_B_21_BT7  -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x2c,{ -1,  -1,  -1,  -1 },0x12, {0x14,0x02,0x14,0x00,0x00}
#define CPS_B_21_QS1  -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x22,{0x24,0x26,0x28,0x2a},0x2c, {0x10,0x08,0x04,0x00,0x00}
#define CPS_B_21_QS2  -1,   -1,    -1,  -1,  -1,  -1,   -1, 0x2e,0x20,  0x0a,{0x0c,0x0e,0x00,0x02},0x04, {0x16,0x16,0x16,0x00,0x00}
#define CPS_B_21_QS3 0x0e,0x0c00,  -1,  -1,  -1,  -1,  0x2c, -1,  -1,   0x12,{0x14,0x16,0x08,0x0a},0x0c, {0x04,0x02,0x20,0x00,0x00}
#define CPS_B_21_QS4 0x2e,0x0c01,  -1,  -1,  -1,  -1,  0x1c,0x1e,0x08,  0x16,{0x00,0x02,0x28,0x2a},0x2c, {0x04,0x08,0x10,0x00,0x00}
#define CPS_B_21_QS5 0x1e,0x0c02,  -1,  -1,  -1,  -1,  0x0c, -1,  -1,   0x2a,{0x2c,0x2e,0x30,0x32},0x1c, {0x04,0x08,0x10,0x00,0x00}
#define HACK_B_1      -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x14,{0x12,0x10,0x0e,0x0c},0x0a, {0x0e,0x0e,0x0e,0x30,0x30}
#define HACK_B_2      -1,   -1,   0x0e,0x0c,0x0a,0x08, 0x06,0x04,0x02,  0x28,{0x26,0x24,0x22,0x20},0x22, {0x20,0x04,0x08,0x12,0x12}

// HBMAME extras

#define HACK_H_2      -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x20,{ -1,  -1,  -1,  -1 },0x2a, {0x02,0x02,0x02,0x00,0x00} // wofh etc
#define HACK_H_3      -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x04,{0x12,0x10,0x0e,0x0c},0x0a, {0x02,0x02,0x02,0x00,0x00} // not used
#define HACK_H_4      -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x30,{0x2e,0x2c,0x2a,0x28},0x26, {0x02,0x04,0x08,0x00,0x00} // not used
#define HACK_H_5      -1, 0x0000,          __not_applicable__,          0x26,{0x28,0x2a,0x2c,0x2e},0x30, {0x40,0x40,0x40,0x00,0x00} // daimakb
#define HACK_H_6      -1,   -1,   0x06,0x04,0x02,0x00, 0x1e,0x1c,0x1a,  0x28,{0x26,0x24,0x22,0x20},0x30, {0x40,0x10,0x02,0x00,0x00} // knightsb2
#define HACK_H_7      -1,   -1,   0x06,0x04,0x02,0x00, 0x1e,0x1c,0x1a,  0x26,{0x28,0x2a,0x2c,0x2e},0x30, {0x20,0x10,0x02,0x00,0x00} // knightsha
#define HACK_H_8      -1,   -1,    -1,  -1,  -1,  -1,   -1,  -1,  -1,   0x20,{0x00,0x00,0x00,0x00},0x00, {0x80,0x80,0x80,0x00,0x00}

/*
CPS_B_21_DEF is CPS-B-21 at default settings (no battery)
CPS_B_21_BTx are various battery configurations
CPS_B_21_QSx are various battery configurations in Q-Sound games
*/


// LWCHR and LW621 are equivalent as far as the game is concerned, though the
// equations are different

#define mapper_LWCHR    { 0x8000, 0x8000, 0, 0 }, mapper_LWCHR_table
static const struct gfx_range mapper_LWCHR_table[] =
{
	// verified from PAL dump (PAL16P8B @ 3A):
	// bank 0 = pin 19 (ROMs 1,5,8,12)
	// bank 1 = pin 16 (ROMs 2,6,9,13)
	// pin 12 and pin 14 are always enabled (except for stars)
	// note that allowed codes go up to 0x1ffff but physical ROM is half that size

	/* type            start    end      bank */
	{ GFXTYPE_SPRITES, 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SCROLL1, 0x00000, 0x1ffff, 0 },
	{ GFXTYPE_STARS,   0x00000, 0x1ffff, 1 },
	{ GFXTYPE_SCROLL2, 0x00000, 0x1ffff, 1 },
	{ GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 1 },
	{ 0 }
};

#define mapper_LW621    { 0x8000, 0x8000, 0, 0 }, mapper_LW621_table
static const struct gfx_range mapper_LW621_table[] =
{
	// verified from PAL dump (PAL @ 1A):
	// bank 0 = pin 18
	// bank 1 = pin 14
	// pins 19, 16, 17, and 12 give an alternate half-size mapping which would
	// allow to use smaller ROMs:
	// pin 19
	// 0 00000-03fff
	// pin 16
	// 0 04000-07fff
	// 1 00000-1ffff
	// pin 17
	// 2 00000-1ffff
	// 3 00000-1ffff
	// 4 00000-1ffff
	// pin 12
	// 3 00000-1ffff
	//
	// note that allowed codes go up to 0x1ffff but physical ROM is half that size

	/* type            start    end      bank */
	{ GFXTYPE_SPRITES, 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SCROLL1, 0x00000, 0x1ffff, 0 },
	{ GFXTYPE_STARS,   0x00000, 0x1ffff, 1 },
	{ GFXTYPE_SCROLL2, 0x00000, 0x1ffff, 1 },
	{ GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 1 },
	{ 0 }
};


// DM620, DM22A and DAM63B are equivalent as far as the game is concerned, though
// the equations are quite different

#define mapper_DM620    { 0x8000, 0x2000, 0x2000, 0 }, mapper_DM620_table
static const struct gfx_range mapper_DM620_table[] =
{
	// verified from PAL dump (PAL16P8B @ 2A):
	// bank 0 = pin 19 (ROMs  5,6,7,8)
	// bank 1 = pin 16 (ROMs  9,11,13,15,18,20,22,24)
	// bank 2 = pin 14 (ROMs 10,12,14,16,19,21,23,25)
	// pin 12 is never enabled
	// note that bank 0 is enabled whenever banks 1 or 2 are not enabled,
	// which would make it highly redundant, so I'm relying on the table
	// to be scanned top to bottom and using a catch-all clause at the end.

	/* type            start   end     bank */
	{ GFXTYPE_SCROLL3, 0x8000, 0xbfff, 1 },
	{ GFXTYPE_SPRITES, 0x2000, 0x3fff, 2 },
	{ GFXTYPE_STARS | GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 0 },
	{ 0 }
};

#define mapper_DM22A    { 0x4000, 0x4000, 0x2000, 0x2000 }, mapper_DM22A_table
static const struct gfx_range mapper_DM22A_table[] =
{
	// verified from PAL dump
	// bank 0 = pin 19
	// bank 1 = pin 16
	// bank 2 = pin 14
	// bank 3 = pin 12

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x00000, 0x01fff, 0 },
	{ GFXTYPE_SCROLL1, 0x02000, 0x03fff, 0 },
	{ GFXTYPE_SCROLL2, 0x04000, 0x07fff, 1 },
	{ GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 2 },
	{ GFXTYPE_SPRITES, 0x02000, 0x03fff, 3 },
	{ 0 }
};

#define mapper_DAM63B   { 0x8000, 0x8000, 0, 0 }, mapper_DAM63B_table
static const struct gfx_range mapper_DAM63B_table[] =
{
	// verified from PAL dump:
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// bank1 = pin 17 (ROMs 5,7) & pin 16 (ROMs 6,8)
	// pins 12,13,14,15 are always enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x00000, 0x01fff, 0 },
	{ GFXTYPE_SCROLL1, 0x02000, 0x02fff, 0 },
	{ GFXTYPE_SCROLL2, 0x04000, 0x07fff, 0 },
	{ GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 1 },
	{ GFXTYPE_SPRITES, 0x02000, 0x03fff, 1 },
	{ 0 }
};


// ST24M1 and ST22B are equivalent except for the stars range which is
// different. This has no practical effect.

#define mapper_ST24M1   { 0x8000, 0x8000, 0, 0 }, mapper_ST24M1_table
static const struct gfx_range mapper_ST24M1_table[] =
{
	// verified from PAL dump
	// bank 0 = pin 19 (ROMs 2,4,6,8)
	// bank 1 = pin 16 (ROMs 1,3,5,7)
	// pin 12 and pin 14 are never enabled

	/* type            start    end      bank */
	{ GFXTYPE_STARS,   0x00000, 0x003ff, 0 },
	{ GFXTYPE_SPRITES, 0x00000, 0x04fff, 0 },
	{ GFXTYPE_SCROLL2, 0x04000, 0x07fff, 0 },
	{ GFXTYPE_SCROLL3, 0x00000, 0x07fff, 1 },
	{ GFXTYPE_SCROLL1, 0x07000, 0x07fff, 1 },
	{ 0 }
};

#define mapper_ST22B    { 0x4000, 0x4000, 0x4000, 0x4000 }, mapper_ST22B_table
static const struct gfx_range mapper_ST22B_table[] =
{
	// verified from PAL dump
	// bank 0 = pin 19 (ROMs 1,5, 9,13,17,24,32,38)
	// bank 1 = pin 16 (ROMs 2,6,10,14,18,25,33,39)
	// bank 2 = pin 14 (ROMs 3,7,11,15,19,21,26,28)
	// bank 3 = pin 12 (ROMS 4,8,12,16,20,22,27,29)

	/* type            start    end      bank */
	{ GFXTYPE_STARS,   0x00000, 0x1ffff, 0 },
	{ GFXTYPE_SPRITES, 0x00000, 0x03fff, 0 },
	{ GFXTYPE_SPRITES, 0x04000, 0x04fff, 1 },
	{ GFXTYPE_SCROLL2, 0x04000, 0x07fff, 1 },
	{ GFXTYPE_SCROLL3, 0x00000, 0x03fff, 2 },
	{ GFXTYPE_SCROLL3, 0x04000, 0x07fff, 3 },
	{ GFXTYPE_SCROLL1, 0x07000, 0x07fff, 3 },
	{ 0 }
};


#define mapper_TK22B    { 0x4000, 0x4000, 0x4000, 0x4000 }, mapper_TK22B_table
static const struct gfx_range mapper_TK22B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 1,5, 9,13,17,24,32,38)
	// bank 1 = pin 16 (ROMs 2,6,10,14,18,25,33,39)
	// bank 2 = pin 14 (ROMs 3,7,11,15,19,21,26,28)
	// bank 3 = pin 12 (ROMS 4,8,12,16,20,22,27,29)

	/* type            start  end      bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SPRITES, 0x4000, 0x5fff, 1 },
	{ GFXTYPE_SCROLL1, 0x6000, 0x7fff, 1 },
	{ GFXTYPE_SCROLL3, 0x0000, 0x3fff, 2 },
	{ GFXTYPE_SCROLL2, 0x4000, 0x7fff, 3 },
	{ 0 }
};


#define mapper_WL24B    { 0x8000, 0x8000, 0, 0 }, mapper_WL24B_table
static const struct gfx_range mapper_WL24B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// bank 1 = pin 12 (ROMs 10,12,14,16,20,22,24,26)
	// pin 14 and pin 19 are never enabled

	/* type            start  end      bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x4fff, 0 },
	{ GFXTYPE_SCROLL3, 0x5000, 0x6fff, 0 },
	{ GFXTYPE_SCROLL1, 0x7000, 0x7fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x3fff, 1 },
	{ 0 }
};


#define mapper_S224B    { 0x8000, 0, 0, 0 }, mapper_S224B_table
static const struct gfx_range mapper_S224B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// pin 12 & pin 14 give an alternate half-size mapping which would allow to
	// populate the 8-bit ROM sockets instead of the 16-bit ones:
	// pin 12
	// 0 00000 - 03fff
	// pin 14
	// 0 04000 - 043ff
	// 1 04400 - 04bff
	// 2 06000 - 07fff
	// 3 04c00 - 05fff
	// pin 19 is never enabled

	/* type            start  end      bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x43ff, 0 },
	{ GFXTYPE_SCROLL1, 0x4400, 0x4bff, 0 },
	{ GFXTYPE_SCROLL3, 0x4c00, 0x5fff, 0 },
	{ GFXTYPE_SCROLL2, 0x6000, 0x7fff, 0 },
	{ 0 }
};


#define mapper_YI24B    { 0x8000, 0, 0, 0 }, mapper_YI24B_table
static const struct gfx_range mapper_YI24B_table[] =
{
	// verified from JED:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// pin 12 & pin 14 give an alternate half-size mapping which would allow to
	// populate the 8-bit ROM sockets instead of the 16-bit ones:
	// pin 12
	// 0 0000-1fff
	// 3 2000-3fff
	// pin 14
	// 1 4000-47ff
	// 2 4800-7fff
	// pin 19 is never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x1fff, 0 },
	{ GFXTYPE_SCROLL3, 0x2000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL1, 0x4000, 0x47ff, 0 },
	{ GFXTYPE_SCROLL2, 0x4800, 0x7fff, 0 },
	{ 0 }
};


// AR24B and AR22B are equivalent, but since we could dump both PALs we are
// documenting both.

#define mapper_AR24B    { 0x8000, 0, 0, 0 }, mapper_AR24B_table
static const struct gfx_range mapper_AR24B_table[] =
{
	// verified from JED:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// pin 12 & pin 14 give an alternate half-size mapping which would allow to
	// populate the 8-bit ROM sockets instead of the 16-bit ones:
	// pin 12
	// 0 0000-2fff
	// 1 3000-3fff
	// pin 14
	// 2 4000-5fff
	// 3 6000-7fff
	// pin 19 is never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL1, 0x3000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL2, 0x4000, 0x5fff, 0 },
	{ GFXTYPE_SCROLL3, 0x6000, 0x7fff, 0 },
	{ 0 }
};

#define mapper_AR22B    { 0x4000, 0x4000, 0, 0 }, mapper_AR22B_table
static const struct gfx_range mapper_AR22B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 1,5, 9,13,17,24,32,38)
	// bank 1 = pin 16 (ROMs 2,6,10,14,18,25,33,39)
	// pins 12 and 14 are tristated

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL1, 0x3000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL2, 0x4000, 0x5fff, 1 },
	{ GFXTYPE_SCROLL3, 0x6000, 0x7fff, 1 },
	{ 0 }
};


#define mapper_O224B    { 0x8000, 0x4000, 0, 0 }, mapper_O224B_table
static const struct gfx_range mapper_O224B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 2,4,6,8)
	// bank 1 = pin 12 (ROMs 10,12,14,16,20,22,24,26)
	// pin 16 & pin 14 appear to be an alternate half-size mapping for bank 0
	// but scroll1 is missing:
	// pin 16
	// 2 00c00 - 03bff
	// 3 03c00 - 03fff
	// pin 14
	// 3 04000 - 04bff
	// 0 04c00 - 07fff

	/* type            start   end     bank */
	{ GFXTYPE_SCROLL1, 0x0000, 0x0bff, 0 },
	{ GFXTYPE_SCROLL2, 0x0c00, 0x3bff, 0 },
	{ GFXTYPE_SCROLL3, 0x3c00, 0x4bff, 0 },
	{ GFXTYPE_SPRITES, 0x4c00, 0x7fff, 0 },
	{ GFXTYPE_SPRITES, 0x8000, 0xa7ff, 1 },
	{ GFXTYPE_SCROLL2, 0xa800, 0xb7ff, 1 },
	{ GFXTYPE_SCROLL3, 0xb800, 0xbfff, 1 },
	{ 0 }
};


#define mapper_MS24B    { 0x8000, 0, 0, 0 }, mapper_MS24B_table
static const struct gfx_range mapper_MS24B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// pin 14 duplicates pin 16 allowing to populate the 8-bit ROM sockets
	// instead of the 16-bit ones.
	// pin 12 is enabled only for sprites:
	// 0 0000-3fff
	// pin 19 is never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL1, 0x4000, 0x4fff, 0 },
	{ GFXTYPE_SCROLL2, 0x5000, 0x6fff, 0 },
	{ GFXTYPE_SCROLL3, 0x7000, 0x7fff, 0 },
	{ 0 }
};


#define mapper_CK24B    { 0x8000, 0, 0, 0 }, mapper_CK24B_table
static const struct gfx_range mapper_CK24B_table[] =
{
	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL1, 0x3000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL2, 0x4000, 0x6fff, 0 },
	{ GFXTYPE_SCROLL3, 0x7000, 0x7fff, 0 },
	{ 0 }
};


#define mapper_NM24B    { 0x8000, 0, 0, 0 }, mapper_NM24B_table
static const struct gfx_range mapper_NM24B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// pin 12 & pin 14 give an alternate half-size mapping which would allow to
	// populate the 8-bit ROM sockets instead of the 16-bit ones:
	// pin 12
	// 0 00000 - 03fff
	// 2 00000 - 03fff
	// pin 14
	// 1 04000 - 047ff
	// 0 04800 - 067ff
	// 2 04800 - 067ff
	// 3 06800 - 07fff
	// pin 19 is never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL1, 0x4000, 0x47ff, 0 },
	{ GFXTYPE_SPRITES, 0x4800, 0x67ff, 0 },
	{ GFXTYPE_SCROLL2, 0x4800, 0x67ff, 0 },
	{ GFXTYPE_SCROLL3, 0x6800, 0x7fff, 0 },
	{ 0 }
};


// CA24B and CA22B are equivalent, but since we could dump both PALs we are
// documenting both.

#define mapper_CA24B    { 0x8000, 0, 0, 0 }, mapper_CA24B_table
static const struct gfx_range mapper_CA24B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// pin 12 & pin 14 give an alternate half-size mapping which would allow to
	// populate the 8-bit ROM sockets instead of the 16-bit ones:
	// pin 12
	// 0 0000-2fff
	// 2 0000-2fff
	// 3 3000-3fff
	// pin 14
	// 3 4000-4fff
	// 1 5000-57ff
	// 0 5800-7fff
	// 2 5800-7fff
	// pin 19 is never enabled (actually it is always enabled when PAL pin 1 is 1, purpose unknown)

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL3, 0x3000, 0x4fff, 0 },
	{ GFXTYPE_SCROLL1, 0x5000, 0x57ff, 0 },
	{ GFXTYPE_SPRITES, 0x5800, 0x7fff, 0 },
	{ GFXTYPE_SCROLL2, 0x5800, 0x7fff, 0 },
	{ 0 }
};

#define mapper_CA22B    { 0x4000, 0x4000, 0, 0 }, mapper_CA22B_table
static const struct gfx_range mapper_CA22B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 1,5, 9,13,17,24,32,38)
	// bank 1 = pin 16 (ROMs 2,6,10,14,18,25,33,39)
	// pin 12 and pin 14 are never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x2fff, 0 },
	{ GFXTYPE_SCROLL3, 0x3000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL3, 0x4000, 0x4fff, 1 },
	{ GFXTYPE_SCROLL1, 0x5000, 0x57ff, 1 },
	{ GFXTYPE_SPRITES, 0x5800, 0x7fff, 1 },
	{ GFXTYPE_SCROLL2, 0x5800, 0x7fff, 1 },
	{ 0 }
};


#define mapper_STF29    { 0x8000, 0x8000, 0x8000, 0 }, mapper_STF29_table
static const struct gfx_range mapper_STF29_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 5,6,7,8)
	// bank 1 = pin 14 (ROMs 14,15,16,17)
	// bank 2 = pin 12 (ROMS 24,25,26,27)

	/* type            start    end      bank */
	{ GFXTYPE_SPRITES, 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SPRITES, 0x08000, 0x0ffff, 1 },
	{ GFXTYPE_SPRITES, 0x10000, 0x11fff, 2 },
	{ GFXTYPE_SCROLL3, 0x02000, 0x03fff, 2 },
	{ GFXTYPE_SCROLL1, 0x04000, 0x04fff, 2 },
	{ GFXTYPE_SCROLL2, 0x05000, 0x07fff, 2 },
	{ 0 }
};


// RT24B and RT22B are equivalent, but since we could dump both PALs we are
// documenting both.

#define mapper_RT24B    { 0x8000, 0x8000, 0, 0 }, mapper_RT24B_table
static const struct gfx_range mapper_RT24B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 16 (ROMs 1,3,5,7)
	// bank 1 = pin 19 (ROMs 2,4,6,8)
	// pin 12 & pin 14 are never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x53ff, 0 },
	{ GFXTYPE_SCROLL1, 0x5400, 0x6fff, 0 },
	{ GFXTYPE_SCROLL3, 0x7000, 0x7fff, 0 },
	{ GFXTYPE_SCROLL3, 0x0000, 0x3fff, 1 },
	{ GFXTYPE_SCROLL2, 0x2800, 0x7fff, 1 },
	{ GFXTYPE_SPRITES, 0x5400, 0x7fff, 1 },
	{ 0 }
};

#define mapper_RT22B    { 0x4000, 0x4000, 0x4000, 0x4000 }, mapper_RT22B_table
static const struct gfx_range mapper_RT22B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 1,5, 9,13,17,24,32,38)
	// bank 1 = pin 16 (ROMs 2,6,10,14,18,25,33,39)
	// bank 2 = pin 14 (ROMs 3,7,11,15,19,21,26,28)
	// bank 3 = pin 12 (ROMS 4,8,12,16,20,22,27,29)

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SPRITES, 0x4000, 0x53ff, 1 },
	{ GFXTYPE_SCROLL1, 0x5400, 0x6fff, 1 },
	{ GFXTYPE_SCROLL3, 0x7000, 0x7fff, 1 },
	{ GFXTYPE_SCROLL3, 0x0000, 0x3fff, 2 },
	{ GFXTYPE_SCROLL2, 0x2800, 0x3fff, 2 },
	{ GFXTYPE_SCROLL2, 0x4000, 0x7fff, 3 },
	{ GFXTYPE_SPRITES, 0x5400, 0x7fff, 3 },
	{ 0 }
};


#define mapper_KD29B    { 0x8000, 0x8000, 0, 0 }, mapper_KD29B_table
static const struct gfx_range mapper_KD29B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 1,2,3,4)
	// bank 1 = pin 14 (ROMs 10,11,12,13)
	// pin 12 is never enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x7fff, 0 },
	{ GFXTYPE_SPRITES, 0x8000, 0x8fff, 1 },
	{ GFXTYPE_SCROLL2, 0x9000, 0xbfff, 1 },
	{ GFXTYPE_SCROLL1, 0xc000, 0xd7ff, 1 },
	{ GFXTYPE_SCROLL3, 0xd800, 0xffff, 1 },
	{ 0 }
};


#define mapper_CC63B    { 0x8000, 0x8000, 0, 0 }, mapper_CC63B_table
static const struct gfx_range mapper_CC63B_table[] =
{
	// verified from PAL dump:
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// bank1 = pin 17 (ROMs 5,7) & pin 16 (ROMs 6,8)
	// pins 12,13,14,15 are always enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x7fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x7fff, 0 },
	{ GFXTYPE_SPRITES, 0x8000, 0xffff, 1 },
	{ GFXTYPE_SCROLL1, 0x8000, 0xffff, 1 },
	{ GFXTYPE_SCROLL2, 0x8000, 0xffff, 1 },
	{ GFXTYPE_SCROLL3, 0x8000, 0xffff, 1 },
	{ 0 }
};


#define mapper_KR63B    { 0x8000, 0x8000, 0, 0 }, mapper_KR63B_table
static const struct gfx_range mapper_KR63B_table[] =
{
	// verified from PAL dump:
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// bank1 = pin 17 (ROMs 5,7) & pin 16 (ROMs 6,8)
	// pins 12,13,14,15 are always enabled

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x7fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x7fff, 0 },
	{ GFXTYPE_SCROLL1, 0x8000, 0x9fff, 1 },
	{ GFXTYPE_SPRITES, 0x8000, 0xcfff, 1 },
	{ GFXTYPE_SCROLL2, 0x8000, 0xcfff, 1 },
	{ GFXTYPE_SCROLL3, 0xd000, 0xffff, 1 },
	{ 0 }
};


#define mapper_S9263B   { 0x8000, 0x8000, 0x8000, 0 }, mapper_S9263B_table
static const struct gfx_range mapper_S9263B_table[] =
{
	// verified from PAL dump:
	// FIXME there is some problem with this dump since pin 14 is never enabled
	// instead of being the same as pin 15 as expected
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// bank1 = pin 17 (ROMs 5,7) & pin 16 (ROMs 6,8)
	// bank2 = pin 15 (ROMs 10,12) & pin 14 (ROMs 11,13)
	// pins 12 and 13 are the same as 14 and 15

	/* type            start    end      bank */
	{ GFXTYPE_SPRITES, 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SPRITES, 0x08000, 0x0ffff, 1 },
	{ GFXTYPE_SPRITES, 0x10000, 0x11fff, 2 },
	{ GFXTYPE_SCROLL3, 0x02000, 0x03fff, 2 },
	{ GFXTYPE_SCROLL1, 0x04000, 0x04fff, 2 },
	{ GFXTYPE_SCROLL2, 0x05000, 0x07fff, 2 },
	{ 0 }
};


// VA22B and VA63B are equivalent, but since we could dump both PALs we are
// documenting both.

#define mapper_VA22B    { 0x4000, 0x4000, 0, 0 }, mapper_VA22B_table
static const struct gfx_range mapper_VA22B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19 (ROMs 1,5, 9,13,17,24,32,38)
	// bank 1 = pin 16 (ROMs 2,6,10,14,18,25,33,39)
	// pin 12 and pin 14 are never enabled

	/* type                                                                  start    end      bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x03fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x04000, 0x07fff, 1 },
	{ 0 }
};

#define mapper_VA63B    { 0x8000, 0, 0, 0 }, mapper_VA63B_table
static const struct gfx_range mapper_VA63B_table[] =
{
	// verified from PAL dump (PAL # uncertain):
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// pins 12,13,14,15,16,17 are never enabled

	/* type                                                                  start    end      bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x07fff, 0 },
	{ 0 }
};


#define mapper_Q522B    { 0x8000, 0, 0, 0 }, mapper_Q522B_table
static const struct gfx_range mapper_Q522B_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x0000, 0x6fff, 0 },
	{ GFXTYPE_SCROLL3,                   0x7000, 0x77ff, 0 },
	{ GFXTYPE_SCROLL1,                   0x7800, 0x7fff, 0 },
	{ 0 }
};


#define mapper_TK263B   { 0x8000, 0x8000, 0, 0 }, mapper_TK263B_table
static const struct gfx_range mapper_TK263B_table[] =
{
	// verified from PAL dump:
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// bank1 = pin 17 (ROMs 5,7) & pin 16 (ROMs 6,8)
	// pins 12,13,14,15 are always enabled

	/* type                                                                  start    end      bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x08000, 0x0ffff, 1 },
	{ 0 }
};


#define mapper_CD63B    { 0x8000, 0x8000, 0, 0 }, mapper_CD63B_table
static const struct gfx_range mapper_CD63B_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SCROLL1,                   0x0000, 0x0fff, 0 },
	{ GFXTYPE_SPRITES,                   0x1000, 0x7fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x8000, 0xdfff, 1 },
	{ GFXTYPE_SCROLL3,                   0xe000, 0xffff, 1 },
	{ 0 }
};


#define mapper_PS63B    { 0x8000, 0x8000, 0, 0 }, mapper_PS63B_table
static const struct gfx_range mapper_PS63B_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SCROLL1,                   0x0000, 0x0fff, 0 },
	{ GFXTYPE_SPRITES,                   0x1000, 0x7fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x8000, 0xdbff, 1 },
	{ GFXTYPE_SCROLL3,                   0xdc00, 0xffff, 1 },
	{ 0 }
};


#define mapper_MB63B    { 0x8000, 0x8000, 0x8000, 0 }, mapper_MB63B_table
static const struct gfx_range mapper_MB63B_table[] =
{
	/* type                              start    end      bank */
	{ GFXTYPE_SCROLL1,                   0x00000, 0x00fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x01000, 0x07fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x08000, 0x0ffff, 1 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x10000, 0x167ff, 2 },
	{ GFXTYPE_SCROLL3,                   0x16800, 0x17fff, 2 },
	{ 0 }
};


#define mapper_QD22B    { 0x4000, 0, 0, 0 }, mapper_QD22B_table
static const struct gfx_range mapper_QD22B_table[] =
{
	// verified from PAL dump:
	// bank 0 = pin 19

	/* type            start   end     bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL1, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL2, 0x0000, 0x3fff, 0 },
	{ GFXTYPE_SCROLL3, 0x0000, 0x3fff, 0 },
	{ 0 }
};


#define mapper_QAD63B   { 0x8000, 0, 0, 0 }, mapper_QAD63B_table
static const struct gfx_range mapper_QAD63B_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SCROLL1,                   0x0000, 0x07ff, 0 },
	{ GFXTYPE_SCROLL3,                   0x0800, 0x1fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x2000, 0x7fff, 0 },
	{ 0 }
};


#define mapper_TN2292   { 0x8000, 0x8000, 0, 0 }, mapper_TN2292_table
static const struct gfx_range mapper_TN2292_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SCROLL1,                   0x0000, 0x0fff, 0 },
	{ GFXTYPE_SCROLL3,                   0x1000, 0x3fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x4000, 0x7fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x8000, 0xffff, 1 },
	{ 0 }
};


#define mapper_RCM63B   { 0x8000, 0x8000, 0x8000, 0x8000 }, mapper_RCM63B_table
static const struct gfx_range mapper_RCM63B_table[] =
{
	// verified from PAL dump:
	// bank0 = pin 19 (ROMs 1,3) & pin 18 (ROMs 2,4)
	// bank1 = pin 17 (ROMs 5,7) & pin 16 (ROMs 6,8)
	// bank0 = pin 15 (ROMs 10,12) & pin 14 (ROMs 11,13)
	// bank1 = pin 13 (ROMs 14,16) & pin 12 (ROMs 15,17)

	/* type                                                                  start    end      bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x08000, 0x0ffff, 1 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x10000, 0x17fff, 2 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x18000, 0x1ffff, 3 },
	{ 0 }
};


#define mapper_PKB10B   { 0x8000, 0, 0, 0 }, mapper_PKB10B_table
static const struct gfx_range mapper_PKB10B_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SCROLL1,                   0x0000, 0x0fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x1000, 0x5fff, 0 },
	{ GFXTYPE_SCROLL3,                   0x6000, 0x7fff, 0 },
	{ 0 }
};


#define mapper_pang3    { 0x8000, 0x8000, 0, 0 }, mapper_pang3_table
static const struct gfx_range mapper_pang3_table[] =
{
	/* type                              start   end     bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x0000, 0x7fff, 0 },
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL2, 0x8000, 0x9fff, 1 },
	{ GFXTYPE_SCROLL1,                   0xa000, 0xbfff, 1 },
	{ GFXTYPE_SCROLL3,                   0xc000, 0xffff, 1 },
	{ 0 }
};


#define mapper_sfzch    { 0x20000, 0, 0, 0 }, mapper_sfzch_table
static const struct gfx_range mapper_sfzch_table[] =
{
	/* type                                                                  start    end      bank */
	{ GFXTYPE_SPRITES | GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 0 },
	{ 0 }
};

/*
  I don't know if CPS2 ROM boards use PALs as well; since all games seem to be
  well behaved, I'll just assume that there is no strong checking of gfx type.
  (sprites are not listed here because they are addressed linearly by the CPS2
  sprite code)
 */
#define mapper_cps2 { 0x20000, 0x20000, 0, 0 }, mapper_cps2_table
static const struct gfx_range mapper_cps2_table[] =
{
	/* type                                                start    end      bank */
	{ GFXTYPE_SCROLL1 | GFXTYPE_SCROLL2 | GFXTYPE_SCROLL3, 0x00000, 0x1ffff, 1 },   // 20000-3ffff physical
	{ 0 }
};


#define mapper_KNM10B    { 0x8000, 0x8000, 0x8000, 0 }, mapper_KNM10B_table
static const struct gfx_range mapper_KNM10B_table[] =
{
	/* type             start    end      bank */

	{ GFXTYPE_SPRITES , 0x00000, 0x07fff, 0 },
	{ GFXTYPE_SPRITES , 0x08000, 0x0ffff, 1 },
	{ GFXTYPE_SPRITES , 0x10000, 0x17fff, 2 },
	{ GFXTYPE_SCROLL2 , 0x04000, 0x07fff, 2 },
	{ GFXTYPE_SCROLL1,  0x01000, 0x01fff, 2 },
	{ GFXTYPE_SCROLL3 , 0x02000, 0x03fff, 2 },
	{ 0 }
};

#if 0
#define mapper_frog     { 0x8000, 0, 0, 0 }, mapper_frog_table
static const struct gfx_range mapper_frog_table[] =
{
	/* type            start   end    bank */
	{ GFXTYPE_SPRITES, 0x04e0, 0x050f, 0 }, // frogs and bugs
	{ GFXTYPE_SCROLL1, 0x0000, 0x003f, 0 }, // text
	{ GFXTYPE_SCROLL2, 0x0140, 0x025f, 0 }, // frog feast title
	{ GFXTYPE_SCROLL3, 0x0040, 0x04df, 0 }, // logo and background
	{ 0 }
};

#define mapper_demo    { 0x8000, 0, 0, 0 }, mapper_demo_table
static const struct gfx_range mapper_demo_table[] =
{
	/* type            start  end      bank */
	{ GFXTYPE_SPRITES, 0x0000, 0x003f, 0 }, // moveable chaos
	{ GFXTYPE_SCROLL1, 0x4400, 0x445f, 0 }, // text
	{ GFXTYPE_SCROLL2, 0x0040, 0x00ff, 0 }, // logo, blue box, 10th
	{ GFXTYPE_SCROLL3, 0x0100, 0x011f, 0 }, // must not allow access to 0000, otherwise not used
	{ 0 }
};
#endif


static const struct CPS1config cps1_config_table[]=
{
	/* name         CPSB          gfx mapper   in2  in3  out2   kludge */
	{"forgottn",    CPS_B_01,     mapper_LW621 },
	{"forgottna",   CPS_B_01,     mapper_LW621 },
	{"forgottnu",   CPS_B_01,     mapper_LW621 },
	{"forgottnu1",  CPS_B_01,     mapper_LWCHR },
	{"forgottnua",  CPS_B_01,     mapper_LWCHR },
	{"forgottnuaa", CPS_B_01,     mapper_LWCHR },
	{"lostwrld",    CPS_B_01,     mapper_LWCHR },
	{"lostwrldo",   CPS_B_01,     mapper_LWCHR },
	{"ghouls",      CPS_B_01,     mapper_DM620 },
	{"ghoulsu",     CPS_B_01,     mapper_DM620 },
	{"daimakai",    CPS_B_01,     mapper_DM22A },   // equivalent to DM620
	{"daimakair",   CPS_B_21_DEF, mapper_DAM63B },  // equivalent to DM620, also CPS_B_21_DEF is equivalent to CPS_B_01
	{"strider",     CPS_B_01,     mapper_ST24M1 },
	{"striderua",   CPS_B_01,     mapper_ST24M1 },  // wrong, this set uses ST24B2, still not dumped
	{"strideruc",   CPS_B_17,     mapper_ST24M1 },  // wrong?
	{"striderj",    CPS_B_01,     mapper_ST22B },   // equivalent to ST24M1
	{"striderjr",   CPS_B_21_DEF, mapper_ST24M1 },  // wrong, this set uses STH63B, still not dumped
	{"dynwar",      CPS_B_02,     mapper_TK22B },   // wrong, this set uses TK24B1, dumped but equations still not added
	{"dynwara",     CPS_B_02,     mapper_TK22B },
	{"dynwarj",     CPS_B_02,     mapper_TK22B },
	{"dynwarjr",    CPS_B_21_DEF, mapper_TK22B },   // wrong, this set uses TK163B, still not dumped
	{"willow",      CPS_B_03,     mapper_WL24B },
	{"willowu",     CPS_B_03,     mapper_WL24B },
	{"willowuo",    CPS_B_03,     mapper_WL24B },
	{"willowj",     CPS_B_03,     mapper_WL24B },   // wrong, this set uses WL22B, still not dumped
	{"ffight",      CPS_B_04,     mapper_S224B },
	{"ffighta",     CPS_B_04,     mapper_S224B },
	{"ffightu",     CPS_B_04,     mapper_S224B },
	{"ffightu1",    CPS_B_04,     mapper_S224B },
	{"ffightua",    CPS_B_01,     mapper_S224B },
	{"ffightub",    CPS_B_03,     mapper_S224B },   // had 04 handwritten on the CPS_B chip, but clearly isn't.
	{"ffightuc",    CPS_B_05,     mapper_S224B },
	{"ffightj",     CPS_B_04,     mapper_S224B },   // wrong, this set uses S222B
	{"ffightj1",    CPS_B_01,     mapper_S224B },   // wrong, this set uses S222B
	{"ffightj2",    CPS_B_02,     mapper_S224B },   // wrong, this set uses S222B
	{"ffightj3",    CPS_B_05,     mapper_S224B },   // wrong, this set uses S222B
	{"ffightjh",    CPS_B_01,     mapper_S224B },   // wrong, ffightjh hack doesn't even use the S222B PAL, since replaced with a GAL.
	{"1941",        CPS_B_05,     mapper_YI24B },
	{"1941r1",      CPS_B_05,     mapper_YI24B },
	{"1941u",       CPS_B_05,     mapper_YI24B },
	{"1941j",       CPS_B_05,     mapper_YI24B },   // wrong, this set uses YI22B, still not dumped
	{"unsquad",     CPS_B_11,     mapper_AR24B },
	{"area88",      CPS_B_11,     mapper_AR22B },   // equivalent to AR24B
	{"area88r",     CPS_B_21_DEF, mapper_AR22B },   // wrong, this set uses ARA63B, still not dumped
	{"mercs",       CPS_B_12,     mapper_O224B,  0x36, 0, 0x34 },
	{"mercsu",      CPS_B_12,     mapper_O224B,  0x36, 0, 0x34 },
	{"mercsur1",    CPS_B_12,     mapper_O224B,  0x36, 0, 0x34 },
	{"mercsj",      CPS_B_12,     mapper_O224B,  0x36, 0, 0x34 },   // wrong, this set uses O222B, still not dumped
	{"msword",      CPS_B_13,     mapper_MS24B },
	{"mswordr1",    CPS_B_13,     mapper_MS24B },
	{"mswordu",     CPS_B_13,     mapper_MS24B },
	{"mswordj",     CPS_B_13,     mapper_MS24B },   // wrong, this set uses MS22B, dumped but equations still not added
	{"mtwins",      CPS_B_14,     mapper_CK24B },
	{"chikij",      CPS_B_14,     mapper_CK24B },   // wrong, this set uses CK22B, dumped but equations still not added
	{"nemo",        CPS_B_15,     mapper_NM24B },
	{"nemor1",      CPS_B_15,     mapper_NM24B },
	{"nemoj",       CPS_B_15,     mapper_NM24B },   // wrong, this set uses NM22B, still not dumped
	{"cawing",      CPS_B_16,     mapper_CA24B },
	{"cawingr1",    CPS_B_16,     mapper_CA24B },
	{"cawingu",     CPS_B_16,     mapper_CA24B },
	{"cawingj",     CPS_B_16,     mapper_CA22B },   // equivalent to CA24B
	{"cawingbl",    CPS_B_16,     mapper_CA22B },   // equivalent to CA24B
	{"sf2",         CPS_B_11,     mapper_STF29,  0x36 },
	{"sf2eb",       CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2ed",       CPS_B_05,     mapper_STF29,  0x36 },
	{"sf2ee",       CPS_B_18,     mapper_STF29,  0x3c },
	{"sf2ebbl",     CPS_B_17,     mapper_STF29,  0x36, 0, 0, 1  },
	{"sf2ebbl2",    CPS_B_17,     mapper_STF29,  0x36, 0, 0, 1  },
	{"sf2ebbl3",    CPS_B_17,     mapper_STF29,  0x36, 0, 0, 1  },
	{"sf2stt",      CPS_B_17,     mapper_STF29,  0x36, 0, 0, 1  },
	{"sf2rk",       CPS_B_17,     mapper_STF29,  0x36, 0, 0, 1  },
	{"sf2ua",       CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2ub",       CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2uc",       CPS_B_12,     mapper_STF29,  0x36 },
	{"sf2ud",       CPS_B_05,     mapper_STF29,  0x36 },
	{"sf2ue",       CPS_B_18,     mapper_STF29,  0x3c },
	{"sf2uf",       CPS_B_15,     mapper_STF29,  0x36 },
	{"sf2ug",       CPS_B_11,     mapper_STF29,  0x36 },
	{"sf2ui",       CPS_B_14,     mapper_STF29,  0x36 },
	{"sf2uk",       CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2j",        CPS_B_13,     mapper_STF29,  0x36 },
	{"sf2ja",       CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2jc",       CPS_B_12,     mapper_STF29,  0x36 },
	{"sf2jf",       CPS_B_15,     mapper_STF29,  0x36 },
	{"sf2jh",       CPS_B_13,     mapper_STF29,  0x36 },
	{"sf2jl",       CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2qp1",      CPS_B_17,     mapper_STF29,  0x36 },
	{"sf2thndr",    CPS_B_17,     mapper_STF29,  0x36 },

	/* from here onwards the CPS-B board has suicide battery and multiply protection */

	{"3wonders",    CPS_B_21_BT1, mapper_RT24B },
	{"3wondersr1",  CPS_B_21_BT1, mapper_RT24B },
	{"3wondersu",   CPS_B_21_BT1, mapper_RT24B },
	{"wonder3",     CPS_B_21_BT1, mapper_RT22B },   // equivalent to RT24B
	{"3wondersb",   CPS_B_21_BT1, mapper_RT24B,  0x36, 0, 0, 0x88 }, // same as 3wonders except some registers are hard wired rather than written to
	{"3wondersh",   HACK_B_2,     mapper_RT24B },  // one port is changed from 3wonders, and no protection
	{"kod",         CPS_B_21_BT2, mapper_KD29B,  0x36, 0, 0x34 },
	{"kodr1",       CPS_B_21_BT2, mapper_KD29B,  0x36, 0, 0x34 },
	{"kodu",        CPS_B_21_BT2, mapper_KD29B,  0x36, 0, 0x34 },
	{"kodj",        CPS_B_21_BT2, mapper_KD29B,  0x36, 0, 0x34 },
	{"kodja",       CPS_B_21_BT2, mapper_KD29B,  0x36, 0, 0x34 },   // wrong, this set uses KD22B, still not dumped
	{"kodb",        CPS_B_21_BT2, mapper_KD29B,  0x36, 0, 0x34 },   /* bootleg, doesn't use multiply protection */
	{"captcomm",    CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },
	{"captcommr1",  CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },
	{"captcommu",   CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },
	{"captcommj",   CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },
	{"captcommjr1", CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },
	{"captcommb",   CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34, 3 },
	{"knights",     CPS_B_21_BT4, mapper_KR63B,  0x36, 0, 0x34 },
	{"knightsu",    CPS_B_21_BT4, mapper_KR63B,  0x36, 0, 0x34 },
	{"knightsj",    CPS_B_21_BT4, mapper_KR63B,  0x36, 0, 0x34 },
	{"knightsja",   CPS_B_21_BT4, mapper_KR63B,  0x36, 0, 0x34 },   // wrong, this set uses KR22B, still not dumped
	//{"knightsb",    CPS_B_21_BT4, mapper_KR63B,  0x36, 0, 0x34 },   // wrong, knightsb bootleg doesn't use the KR63B PAL
	{"sf2ce",       CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2ceea",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2ceua",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2ceub",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2ceuc",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2ceja",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2cejb",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2cejc",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2bhh",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2rb",       CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2rb2",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2rb3",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2red",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2v004",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2acc",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2ceblp",    CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2cebltw",   CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2acca",     CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2accp2",    CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2amf",      CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 }, // probably wrong but this set is not completely dumped anyway
	{"sf2amf2",     CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2dkot2",    CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2m1",       CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2m2",       CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2m3",       HACK_B_1,     mapper_S9263B, 0,    0, 0, 2 },
	{"sf2m4",       HACK_B_1,     mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2m5",       CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2m6",       CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2m7",       CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2m8",       HACK_B_1,     mapper_S9263B, 0,    0, 0, 2 },
	{"sf2m9",       CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2m10",      HACK_B_1,     mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2dongb",    CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2yyc",      CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2koryu",    CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2mdt",      CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2mdta",     CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2mdtb",     CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 1 },
	{"sf2b",        CPS_B_17,     mapper_STF29,  0x36, 0, 0, 1  },
	{"varth",       CPS_B_04,     mapper_VA63B },   /* CPSB test has been patched out (60=0008) register is also written to, possibly leftover from development */  // wrong, this set uses VA24B, dumped but equations still not added
	{"varthb",      CPS_B_04,     mapper_VA63B, 0, 0, 0, 0x0F },
	{"varthr1",     CPS_B_04,     mapper_VA63B },   /* CPSB test has been patched out (60=0008) register is also written to, possibly leftover from development */  // wrong, this set uses VA24B, dumped but equations still not added
	{"varthu",      CPS_B_04,     mapper_VA63B },   /* CPSB test has been patched out (60=0008) register is also written to, possibly leftover from development */
	{"varthj",      CPS_B_21_BT5, mapper_VA22B },   /* CPSB test has been patched out (72=0001) register is also written to, possibly leftover from development */
	{"varthjr",     CPS_B_21_BT5, mapper_VA63B },   /* CPSB test has been patched out (72=0001) register is also written to, possibly leftover from development */
	{"cworld2j",    CPS_B_21_BT6, mapper_Q522B,  0x36, 0, 0x34 },   /* (ports 36, 34 probably leftover input code from another game) */
	{"cworld2ja",   CPS_B_21_DEF, mapper_Q522B }, // patched set, no battery, could be desuicided // wrong, this set uses Q529B, still not dumped
	{"cworld2jb",   CPS_B_21_BT6, mapper_Q522B,  0x36, 0, 0x34 }, // wrong, this set uses Q563B, still not dumped
	{"wof",         CPS_B_21_QS1, mapper_TK263B },
	{"wofr1",       CPS_B_21_DEF, mapper_TK263B },  // patched set coming from a desuicided board?
	{"wofa",        CPS_B_21_DEF, mapper_TK263B },  // patched set coming from a desuicided board?
	{"wofu",        CPS_B_21_QS1, mapper_TK263B },
	{"wofj",        CPS_B_21_QS1, mapper_TK263B },
	{"wofhfh",      CPS_B_21_DEF, mapper_TK263B, 0x36 },    /* Chinese bootleg */
	{"dino",        CPS_B_21_QS2, mapper_CD63B },   /* layer enable never used */
	{"dinou",       CPS_B_21_QS2, mapper_CD63B },   /* layer enable never used */
	{"dinoj",       CPS_B_21_QS2, mapper_CD63B },   /* layer enable never used */
	{"dinopic",     CPS_B_21_QS2, mapper_CD63B },   /* layer enable never used */
	{"dinopic2",    CPS_B_21_QS2, mapper_CD63B },   /* layer enable never used */
	{"dinohunt",    CPS_B_21_DEF, mapper_CD63B },   /* Chinese bootleg */
	{"punisher",    CPS_B_21_QS3, mapper_PS63B },
	{"punisheru",   CPS_B_21_QS3, mapper_PS63B },
	{"punisherh",   CPS_B_21_QS3, mapper_PS63B },
	{"punisherj",   CPS_B_21_QS3, mapper_PS63B },
	{"punipic",     CPS_B_21_QS3, mapper_PS63B },
	{"punipic2",    CPS_B_21_QS3, mapper_PS63B },
	{"punipic3",    CPS_B_21_QS3, mapper_PS63B },
	{"punisherbz",  CPS_B_21_DEF, mapper_PS63B },   /* Chinese bootleg */
	{"slammast",    CPS_B_21_QS4, mapper_MB63B },
	{"slammastu",   CPS_B_21_QS4, mapper_MB63B },
	{"slampic",     CPS_B_21_QS4, mapper_MB63B },
	{"mbomberj",    CPS_B_21_QS4, mapper_MB63B },
	{"mbombrd",     CPS_B_21_QS5, mapper_MB63B },
	{"mbombrdj",    CPS_B_21_QS5, mapper_MB63B },
	{"sf2hf",       CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2hfu",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"sf2hfj",      CPS_B_21_DEF, mapper_S9263B, 0x36 },
	{"qad",         CPS_B_21_BT7, mapper_QD22B,  0x36 },    /* TODO: layer enable (port 36 probably leftover input code from another game) */
	{"qadjr",       CPS_B_21_DEF, mapper_QAD63B, 0x36, 0x38, 0x34 },    /* (ports 36, 38, 34 probably leftover input code from another game) */
	{"qtono2j",     CPS_B_21_DEF, mapper_TN2292, 0x36, 0x38, 0x34 },    /* (ports 36, 38, 34 probably leftover input code from another game) */
	{"megaman",     CPS_B_21_DEF, mapper_RCM63B },
	{"megamana",    CPS_B_21_DEF, mapper_RCM63B },
	{"rockmanj",    CPS_B_21_DEF, mapper_RCM63B },
	{"pnickj",      CPS_B_21_DEF, mapper_PKB10B },
	{"pang3",       CPS_B_21_DEF, mapper_pang3 },   /* EEPROM port is among the CPS registers (handled by DRIVER_INIT) */   // should use one of these three CP1B1F,CP1B8K,CP1B9KA
	{"pang3r1",     CPS_B_21_DEF, mapper_pang3 },   /* EEPROM port is among the CPS registers (handled by DRIVER_INIT) */   // should use one of these three CP1B1F,CP1B8K,CP1B9K
	{"pang3j",      CPS_B_21_DEF, mapper_pang3 },   /* EEPROM port is among the CPS registers (handled by DRIVER_INIT) */   // should use one of these three CP1B1F,CP1B8K,CP1B9K
	{"pang3b",      CPS_B_21_DEF, mapper_pang3 },   /* EEPROM port is among the CPS registers (handled by DRIVER_INIT) */   // should use one of these three CP1B1F,CP1B8K,CP1B9K
	{"ganbare",     CPS_B_21_DEF, mapper_sfzch },   // wrong, this set uses GBPR2, dumped but equations still not added

	/* CPS Changer */

	{"sfach",       CPS_B_21_DEF, mapper_sfzch },   // wrong, this set uses an unknown PAL, still not dumped
	{"sfzbch",      CPS_B_21_DEF, mapper_sfzch },   // wrong, this set uses an unknown PAL, still not dumped
	{"sfzch",       CPS_B_21_DEF, mapper_sfzch },   // wrong, this set uses an unknown PAL, still not dumped
	{"wofch",       CPS_B_21_DEF, mapper_TK263B },

	/* CPS2 games */

	{"cps2",        CPS_B_21_DEF, mapper_cps2 },

	/* CPS1 board + extra support boards */

	{"kenseim",     CPS_B_21_DEF, mapper_KNM10B },  // wrong, need to convert equations from PAL

// HBMAME
	{"captcommb2",  CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },		//works
	{"dinosf2",     CPS_B_21_QS2, mapper_CD63B },							// nw
	{"kodd",        CPS_B_01, mapper_KD29B, 0x36, 0, 0x34 },			// nw
	{"kodud",       CPS_B_01, mapper_KD29B, 0x36, 0, 0x34 },			// nw
#if 0
	{"3wondrud",    CPS_B_21_DEF, mapper_RT24B },							//works
	{"captcoud",    CPS_B_21_DEF, mapper_CC63B,  0x36, 0x38, 0x34 },		//works
	{"captcomc",    CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },		//works
	{"captcomh",    CPS_B_21_BT3, mapper_CC63B,  0x36, 0x38, 0x34 },		//works
	//{"cawingb",     CPS_B_16,     mapper_CA24B, 0, 0, 0, 0x8F },			// no sprites here, ok in fcrash.cpp
	{"cps1demo",    CPS_B_04,     mapper_demo,  0, 0, 0, 0x80 },			//works
	{"cps1frog",    CPS_B_04,     mapper_frog,  0, 0, 0, 0x80 },			//works
	{"cps1test",    CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"cps1testa",   CPS_B_21_DEF, mapper_RCM63B },							//works
	{"daimakb",     HACK_H_5,     mapper_DM22A },							//works
	{"dino08h2",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dino08h3",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dino08hp",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinob",       CPS_B_21_QS2, mapper_CD63B, 0, 0, 0, 0x0F },			//works
	{"dinobt",      CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinobtpp",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinocgba",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinocgbb",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinocgbc",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinoeh",      CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinoex",      CPS_B_21_DEF, mapper_CD63B },							//works
	{"dinoh",       CPS_B_21_DEF, mapper_CD63B, 0x36 },						//works
	{"dinoha",      CPS_B_21_DEF, mapper_CD63B, 0x36 },						//works
	{"dinohb",      CPS_B_21_QS2, mapper_CD63B, 0, 0, 0, 0x0F },			//works
	{"dinohp",      CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinohx",      CPS_B_21_DEF, mapper_CD63B, 0x36 },						//works
	{"dinojp",      CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinorp",      CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinoslice",   CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinouphp",    CPS_B_21_QS2, mapper_CD63B },							//works
	{"dinoz",       CPS_B_21_QS2, mapper_CD63B },							//works
	{"ffightb",     CPS_B_01,     mapper_S224B },							//works
	{"knight21",    CPS_B_21_BT4, mapper_KR63B, 0x36, 0, 0x34 },			//works
	//{"knightsa",    CPS_B_21_BT4, mapper_KR63B, 0x36, 0, 0x34 },			// a rom is unobtainable
	{"knightsb2",   HACK_H_6,     mapper_KR63B, 0x36, 0, 0x34, 0x44 }, 		//works
	{"knightsh",    CPS_B_21_DEF, mapper_KR63B, 0x36, 0, 0x34 },			//works (intro screen is crap)
	{"knightsha",   HACK_H_7,     mapper_KR63B, 0x36, 0, 0x34 },			//works
	{"knightsjb",   CPS_B_21_DEF, mapper_KR63B, 0x36, 0, 0x34 },			//works
	{"knightsro",   CPS_B_21_BT4, mapper_KR63B, 0x36, 0, 0x34 },			//works
	{"knightud",    CPS_B_21_DEF, mapper_KR63B, 0x36, 0, 0x34 },			//works
	{"kodh",        CPS_B_21_DEF, mapper_KD29B, 0x36, 0, 0x34 },			//works
	{"kodsp",       CPS_B_21_BT2, mapper_KD29B, 0x36, 0, 0x34 },			//works
	{"pnicku",      CPS_B_21_DEF, mapper_PKB10B },							//works
	{"punisherb",   CPS_B_21_QS3, mapper_PS63B, 0, 0, 0, 0x0E },			//works
	{"punisherf",   CPS_B_21_QS3, mapper_PS63B },							//works
	{"punisherje1", CPS_B_21_QS3, mapper_PS63B },							//works
	{"punisherjh",  CPS_B_21_QS3, mapper_PS63B },							//works
	{"punisherjh1", CPS_B_21_QS3, mapper_PS63B },							//works
	{"punisherjha", CPS_B_21_QS3, mapper_PS63B },							//works
	{"punisherud1", CPS_B_21_DEF, mapper_PS63B },							//works
	{"punisherud2", CPS_B_21_DEF, mapper_PS63B },							//works
	{"sf2bhj",      CPS_B_12,     mapper_STF29,  0x36 },					//works
	{"sf2c",        CPS_B_13,     mapper_STF29,  0x36 },					//works
	{"sf2cebr",     CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2ced",      CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2ceda",     CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2ceh",      CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2cehk",     CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2cehp",     CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2cejem",    CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2koryuh",   CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2h9",       CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2h10",      CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2h11",      HACK_B_1,     mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2h12",      HACK_B_1,     mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2h13",      CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2pun",      CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2sl73a",    CPS_B_21_DEF, mapper_S9263B, 0x36 },					//works
	{"sf2th",       CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2tha",      CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2tlona",    CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2tlonb",    CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2tlonc",    CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2turyu",    CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2yyc3d5",   CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2yyc3g",    CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"sf2yyc6",     CPS_B_21_DEF, mapper_S9263B, 0x36, 0, 0, 0x41 },		//works
	{"slammastud",  CPS_B_21_DEF, mapper_MB63B },							//works
	{"stridergf",   CPS_B_01,     mapper_ST24M1 },							//works
	{"stridergh",   CPS_B_01,     mapper_ST24M1 },							//works
	{"tk2h5",       CPS_B_21_QS1, mapper_TK263B },							// problem with chinese language roms
	{"varthb",      CPS_B_04,     mapper_VA63B, 0, 0, 0, 0x0F },			//works
	{"wofb",        CPS_B_21_DEF, mapper_TK263B },							//works
	{"wofes",       CPS_B_21_DEF, mapper_TK263B },							//works
	{"woffr",       CPS_B_21_DEF, mapper_sfzch },							//works
	{"wofr1h",      CPS_B_21_DEF, mapper_TK263B },							//works
	{"wofsf2",      CPS_B_21_QS1, mapper_TK263B, 0x36 },					//works
	{"wofud",       CPS_B_21_DEF, mapper_TK263B },							//works
#endif
	// HBMAME end
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


MACHINE_RESET_MEMBER(cps_state,cps)
{
	const char *gamename = machine().system().name;
	u8 tt = 0;
	u8 *rom = 0;
	if (memregion("control"))
	{
		rom = memregion("control")->base();
		tt = rom[0x00];
	}
	if (tt == 0x55)
	{
		tt = 1;
		m_cpsb_addr            = rom[tt++];
		m_cpsb_value           = rom[tt] | (rom[tt+1] << 8); tt += 2;
		m_mult_factor1         = rom[tt++];
		m_mult_factor2         = rom[tt++];
		m_mult_result_lo       = rom[tt++];
		m_mult_result_hi       = rom[tt++];
		m_layer_control        = rom[tt++];
		m_priority[0]          = rom[tt++];
		m_priority[1]          = rom[tt++];
		m_priority[2]          = rom[tt++];
		m_priority[3]          = rom[tt++];
		m_palette_control      = rom[tt++];
		m_layer_enable_mask[0] = rom[tt++];
		m_layer_enable_mask[1] = rom[tt++];
		m_layer_enable_mask[2] = rom[tt++];
		m_layer_enable_mask[3] = rom[tt++];
		m_layer_enable_mask[4] = rom[tt++];
		m_in2_addr             = rom[tt++];
		m_in3_addr             = rom[tt++];
		m_out2_addr            = rom[tt++];
		m_bootleg_kludge       = rom[tt] | (rom[tt+1] << 8); tt += 2;
		m_scrollx1             = rom[tt++];
		m_scrollx2             = rom[tt++];
		m_scrollx3             = rom[tt++];
		m_bank_sizes[0]        = rom[tt] | (rom[tt+1] << 8) | (rom[tt+2] << 16); tt += 3;
		m_bank_sizes[1]        = rom[tt] | (rom[tt+1] << 8) | (rom[tt+2] << 16); tt += 3;
		m_bank_sizes[2]        = rom[tt] | (rom[tt+1] << 8) | (rom[tt+2] << 16); tt += 3;
		m_bank_sizes[3]        = rom[tt] | (rom[tt+1] << 8) | (rom[tt+2] << 16); tt += 3;

		for (u8 i = 0; i < 8; i++)
		{
			m_bank_type[i*4] = rom[tt] & 0x1f;
			m_bank_type[i*4+1] = rom[tt+1] | (rom[tt+2] << 8) | (rom[tt+3] << 16);
			m_bank_type[i*4+2] = rom[tt+4] | (rom[tt+5] << 8) | (rom[tt+6] << 16);
			m_bank_type[i*4+3] = rom[tt] >> 6;
			tt += 7;
		}
	}
	else
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
		m_bootleg_kludge = m_game_config->bootleg_kludge;

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
#if 0
	if (strcmp(gamename, "sf2accp2") == 0)
	{
		/* Patch out a odd branch which would be incorrectly interpreted
		   by the cpu core as a 32-bit branch. This branch would make the
		   game crash (address error, since it would branch to an odd address)
		   if location 180ca6 (outside ROM space) isn't 0. Protection check? */
		u16 *rom = (u16 *)memregion("maincpu")->base();
		rom[0x11756 / 2] = 0x4e71;
	}
	else if (strcmp(gamename, "ghouls") == 0)
	{
		/* Patch out self-test... it takes forever */
		u16 *rom = (u16 *)memregion("maincpu")->base();
		rom[0x61964 / 2] = 0x4ef9;
		rom[0x61966 / 2] = 0x0000;
		rom[0x61968 / 2] = 0x0400;
	}
#endif
#if WRITE_FILE
	// Write the config to a file
	FILE *fp;
	fp = fopen(gamename, "wb");
	fprintf(fp, "%s", "U");
	fprintf(fp, "%c", m_cpsb_addr);
	fprintf(fp, "%c%c", m_cpsb_value&0xff, (m_cpsb_value >> 8) & 0xff);
	fprintf(fp, "%c", m_mult_factor1);
	fprintf(fp, "%c", m_mult_factor2);
	fprintf(fp, "%c", m_mult_result_lo);
	fprintf(fp, "%c", m_mult_result_hi);
	fprintf(fp, "%c", m_layer_control);
	fprintf(fp, "%c", m_priority[0]);
	fprintf(fp, "%c", m_priority[1]);
	fprintf(fp, "%c", m_priority[2]);
	fprintf(fp, "%c", m_priority[3]);
	fprintf(fp, "%c", m_palette_control);
	fprintf(fp, "%c", m_layer_enable_mask[0]);
	fprintf(fp, "%c", m_layer_enable_mask[1]);
	fprintf(fp, "%c", m_layer_enable_mask[2]);
	fprintf(fp, "%c", m_layer_enable_mask[3]);
	fprintf(fp, "%c", m_layer_enable_mask[4]);
	fprintf(fp, "%c", m_in2_addr);
	fprintf(fp, "%c", m_in3_addr);
	fprintf(fp, "%c", m_out2_addr);
	fprintf(fp, "%c%c", m_bootleg_kludge&0xff, (m_bootleg_kludge >> 8) & 0xff);
	u8 scroll1x = 0, scroll2x = 0, scroll3x = 0;
	switch (m_bootleg_kludge&0x0f)
	{
		case 0x01:
			scroll1x = 0x8c;
			scroll2x = 0x8e;
			scroll3x = 0x90;
			break;
		case 0x02:
			scroll1x = 0x8c;
			scroll2x = 0x90;
			scroll3x = 0x90;
			break;
		case 0x03:
			scroll1x = 0x88;
			scroll2x = 0x8b;
			scroll3x = 0x8c;
			break;
		case 0x08:
			scroll1x = 0x04;
			scroll2x = 0x06;
			scroll3x = 0x0a;
			break;
		case 0x0E:
			scroll1x = 0xc6;
			scroll2x = 0xc0;
			scroll3x = 0xc6;
			break;
		case 0x0F:
			scroll1x = 0xc0;
			scroll2x = 0xc0;
			scroll3x = 0xc0;
	}
	fprintf(fp, "%c%c%c", scroll1x, scroll2x, scroll3x);
	fprintf(fp, "%c%c%c", m_bank_sizes[0]&0xff, (m_bank_sizes[0] >> 8)&0xff, m_bank_sizes[0] >> 16);
	fprintf(fp, "%c%c%c", m_bank_sizes[1]&0xff, (m_bank_sizes[1] >> 8)&0xff, m_bank_sizes[1] >> 16);
	fprintf(fp, "%c%c%c", m_bank_sizes[2]&0xff, (m_bank_sizes[2] >> 8)&0xff, m_bank_sizes[2] >> 16);
	fprintf(fp, "%c%c%c", m_bank_sizes[3]&0xff, (m_bank_sizes[3] >> 8)&0xff, m_bank_sizes[3] >> 16);

	for (u8 i = 0; i < 8; i++)
	{
		fprintf(fp, "%c", m_bank_type[i*4] | (m_bank_type[i*4+3] << 6));
		fprintf(fp, "%c%c%c", m_bank_type[i*4+1] & 0xff, (m_bank_type[i*4+1] >> 8)&0xff, m_bank_type[i*4+1] >> 16);
		fprintf(fp, "%c%c%c", m_bank_type[i*4+2] & 0xff, (m_bank_type[i*4+2] >> 8)&0xff, m_bank_type[i*4+2] >> 16);
		printf("%X,%X,%X,%X\n",m_bank_type[i*4],m_bank_type[i*4+1],m_bank_type[i*4+2],m_bank_type[i*4+3]);
	}
	for (u8 i = ftell(fp); i < 0x80; i++)
		fprintf(fp, "%c", 0);
	fclose(fp);
#endif
}


inline u16 *cps_state::cps1_base( int offset, int boundary )
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



void cps_state::cps1_cps_a_w(offs_t offset, u16 data, u16 mem_mask)
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


u16 cps_state::cps1_cps_b_r(offs_t offset)
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


void cps_state::cps1_cps_b_w(offs_t offset, u16 data, u16 mem_mask)
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
			}
			else    // kod, captcomm, knights
			{
				machine().bookkeeping().coin_lockout_w(2, ~data & 0x02);
				machine().bookkeeping().coin_lockout_w(3, ~data & 0x08);
			}
		}
	}

#ifdef MAME_DEBUG
	if (offset != m_cpsb_addr / 2 &&   // only varth writes here
			offset != m_mult_factor1 / 2 &&
			offset != m_mult_factor2 / 2 &&
			offset != m_layer_control / 2 &&
			offset != m_unknown1 / 2 &&
			offset != m_unknown2 / 2 &&
			offset != m_unknown3 / 2 &&
			offset != m_priority[0] / 2 &&
			offset != m_priority[1] / 2 &&
			offset != m_priority[2] / 2 &&
			offset != m_priority[3] / 2 &&
			offset != m_palette_control / 2 &&
			offset != m_out2_addr / 2 &&
			!m_bootleg_kludge)
		popmessage("CPS-B write %04x to port %02x contact MAMEDEV", data, offset * 2);
#endif
}


void cps_state::unshuffle( u64 *buf, int len )
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


void cps_state::cps2_gfx_decode()
{
	const int banksize = 0x200000;
	int size = memregion("gfx")->bytes();
	int i;

	for (i = 0; i < size; i += banksize)
		unshuffle((u64 *)(memregion("gfx")->base() + i), banksize / 8);
}


void cps_state::init_cps1()
{
	m_scanline1 = 0;
	m_scanline2 = 0;
	m_scancalls = 0;
	m_last_sprite_offset = 0;
	m_pri_ctrl = 0;
	m_objram_bank = 0;
}


void cps_state::cps1_get_video_base()
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

	// HBMAME start
	/* Some of the sf2 hacks use only sprite port 0x9100 and the scroll layers are offset */
	u8 kludge = m_bootleg_kludge & 15;
	if (kludge == 0x01 || kludge == 0x02)
		m_cps_a_regs[CPS1_OBJ_BASE] = 0x9100;
	else
	if (kludge == 0x08) // 3wondersb
	{
		m_cps_b_regs[0x30/2] = 0x3f;
		m_cps_a_regs[CPS1_VIDEOCONTROL] = 0x3e;
		m_cps_a_regs[CPS1_SCROLL2_BASE] = 0x90c0;
		m_cps_a_regs[CPS1_SCROLL3_BASE] = 0x9100;
		m_cps_a_regs[CPS1_PALETTE_BASE] = 0x9140;
	}
	else
	if (m_bootleg_kludge == 0x8F)
	{
		m_cps_b_regs[0x28 / 2] = m_mainram[0x639a / 2];
		m_cps_b_regs[0x2c / 2] = m_mainram[0x639c / 2];
		m_cps_b_regs[0x2e / 2] = m_mainram[0x639e / 2];
		//m_cps_b_regs[m_layer_enable_reg / 2] = m_mainram[0x6398 / 2];
	}
	// HBMAME end

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

#ifdef MAME_DEBUG
{
	int enablemask = 0;

	if (m_layer_enable_mask[0] == m_layer_enable_mask[1])
		enablemask = m_layer_enable_mask[0];

	if (m_layer_enable_mask[0] == m_layer_enable_mask[2])
		enablemask = m_layer_enable_mask[0];

	if (m_layer_enable_mask[1] == m_layer_enable_mask[2])
		enablemask = m_layer_enable_mask[1];

	if (enablemask)
	{
		if (((layercontrol & enablemask) && (layercontrol & enablemask) != enablemask))
			popmessage("layer %02x contact MAMEDEV", layercontrol & 0xc03f);
	}

	enablemask = m_layer_enable_mask[0] | m_layer_enable_mask[1]
			| m_layer_enable_mask[2]
			| m_layer_enable_mask[3] | m_layer_enable_mask[4];

	if (((layercontrol & ~enablemask) & 0x003e) != 0)
		popmessage("layer %02x contact MAMEDEV", layercontrol & 0xc03f);
}
#endif

}


void cps_state::cps1_gfxram_w(offs_t offset, u16 data, u16 mem_mask)
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



int cps_state::gfxrom_bank_mapper( int type, int code )
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

TILEMAP_MAPPER_MEMBER(cps_state::tilemap0_scan)
{
	/* logical (col,row) -> memory offset */
	return (row & 0x1f) + ((col & 0x3f) << 5) + ((row & 0x20) << 6);
}

TILEMAP_MAPPER_MEMBER(cps_state::tilemap1_scan)
{
	/* logical (col,row) -> memory offset */
	return (row & 0x0f) + ((col & 0x3f) << 4) + ((row & 0x30) << 6);
}

TILEMAP_MAPPER_MEMBER(cps_state::tilemap2_scan)
{
	/* logical (col,row) -> memory offset */
	return (row & 0x07) + ((col & 0x3f) << 3) + ((row & 0x38) << 6);
}

TILE_GET_INFO_MEMBER(cps_state::get_tile0_info)
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

TILE_GET_INFO_MEMBER(cps_state::get_tile1_info)
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

TILE_GET_INFO_MEMBER(cps_state::get_tile2_info)
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



void cps_state::cps1_update_transmasks()
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

VIDEO_START_MEMBER(cps_state,cps)
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
	m_bg_tilemap[0] = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(cps_state::get_tile0_info)), tilemap_mapper_delegate(*this, FUNC(cps_state::tilemap0_scan)),  8,  8, 64, 64);
	m_bg_tilemap[1] = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(cps_state::get_tile1_info)), tilemap_mapper_delegate(*this, FUNC(cps_state::tilemap1_scan)), 16, 16, 64, 64);
	m_bg_tilemap[2] = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(*this, FUNC(cps_state::get_tile2_info)), tilemap_mapper_delegate(*this, FUNC(cps_state::tilemap2_scan)), 32, 32, 64, 64);

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

	machine().save().register_postload(save_prepost_delegate(FUNC(cps_state::cps1_get_video_base), this));
}

VIDEO_START_MEMBER(cps_state,cps1)
{
	m_cps_version = 1;
	VIDEO_START_CALL_MEMBER(cps);
}


/***************************************************************************

  Build palette from palette RAM

  12 bit RGB with a 4 bit brightness value.

***************************************************************************/

void cps_state::cps1_build_palette( const u16* const palette_base )
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

void cps_state::cps1_find_last_sprite()    /* Find the offset of last sprite */
{
	int offset = 0;
	/* Locate the end of table marker */
	while (offset < m_obj_size / 2)
	{
		if (m_bootleg_kludge == 3)
		{
			/* captcommb - same end of sprite marker as CPS-2 */
			int colour = m_buffered_obj[offset + 1];
			if (colour >= 0x8000)
			{
				/* Marker found. This is the last sprite. */
				m_last_sprite_offset = offset - 4;
				return;
			}
		}
		else
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


void cps_state::cps1_render_sprites( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect )
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

	/* some sf2 hacks draw the sprites in reverse order */
	if (BIT(m_bootleg_kludge, 6)) // HBMAME
	{
		base += m_last_sprite_offset;
		baseadd = -4;
	}
	else
	{
		baseadd = 4;
	}

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




void cps_state::cps2_objram_bank_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (ACCESSING_BITS_0_7)
		m_objram_bank = data & 1;
}

u16 cps_state::cps2_objram1_r(offs_t offset)
{
	if (m_objram_bank & 1)
		return m_objram2[offset];
	else
		return m_objram1[offset];
}

u16 cps_state::cps2_objram2_r(offs_t offset)
{
	if (m_objram_bank & 1)
		return m_objram1[offset];
	else
		return m_objram2[offset];
}

void cps_state::cps2_objram1_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (m_objram_bank & 1)
		COMBINE_DATA(&m_objram2[offset]);
	else
		COMBINE_DATA(&m_objram1[offset]);
}

void cps_state::cps2_objram2_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (m_objram_bank & 1)
		COMBINE_DATA(&m_objram1[offset]);
	else
		COMBINE_DATA(&m_objram2[offset]);
}

u16 *cps_state::cps2_objbase()
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


void cps_state::cps2_find_last_sprite()    /* Find the offset of last sprite */
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

void cps_state::cps2_render_sprites( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int *primasks )
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




void cps_state::cps1_render_stars( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect )
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
					bitmap.pix16(sy, sx) = 0xa00 + col;
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
					bitmap.pix16(sy, sx) = 0x800 + col;
			}
		}
	}
}


void cps_state::cps1_render_layer( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int primask )
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

void cps_state::cps1_render_high_layer( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer )
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

u32 cps_state::screen_update_cps1(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
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
		if (BIT(m_bootleg_kludge, 7))
			cps1_build_palette(cps1_base(CPS1_PALETTE_BASE, m_palette_align));

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

WRITE_LINE_MEMBER(cps_state::screen_vblank_cps1)
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

void cps_state::cps2_set_sprite_priorities()
{
	m_pri_ctrl = m_output[CPS2_OBJ_PRI /2];
}

void cps_state::cps2_objram_latch()
{
	cps2_set_sprite_priorities();
	memcpy(m_cps2_buffered_obj.get(), cps2_objbase(), m_cps2_obj_size);
}
