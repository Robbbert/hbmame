// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "emu.h"
#include "cpu/z80/z80.h"
#include "machine/segacrpt.h"
#include "includes/pacman.h"
#include "sound/namco.h"


#define MASTER_CLOCK		(18432000)

#define PIXEL_CLOCK			(MASTER_CLOCK/3)

/* H counts from 128->511, HBLANK starts at 128+16=144 and ends at 128+64+32+16=240 */
#define HTOTAL				(384)
#define HBEND				(0)		/*(96+16)*/
#define HBSTART				(288)	/*(16)*/

#define VTOTAL				(264)
#define VBEND				(0)		/*(16)*/
#define VBSTART				(224)	/*(224+16)*/

class pengo_state : public pacman_state
{
public:
	pengo_state(const machine_config &mconfig, device_type type, const char *tag)
		: pacman_state(mconfig, type, tag), m_decrypted_opcodes(*this, "decrypted_opcodes") { }
	//DECLARE_WRITE8_MEMBER(pengo_coin_counter_w);
	//DECLARE_WRITE8_MEMBER(irq_mask_w);
	//DECLARE_DRIVER_INIT(penta);
	DECLARE_DRIVER_INIT(pengo);
	//INTERRUPT_GEN_MEMBER(vblank_irq);

	optional_shared_ptr<UINT8> m_decrypted_opcodes;
};

/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

static ADDRESS_MAP_START( pengo_map, AS_PROGRAM, 8, pengo_state )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x8000, 0x83ff) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram") /* video and color RAM, scratchpad RAM, sprite codes */
	AM_RANGE(0x8400, 0x87ff) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x8800, 0x8fef) AM_RAM AM_SHARE("mainram")
	AM_RANGE(0x8ff0, 0x8fff) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x9000, 0x901f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x9020, 0x902f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x9000, 0x903f) AM_READ_PORT("DSW1")
	AM_RANGE(0x9040, 0x907f) AM_READ_PORT("DSW0")
	AM_RANGE(0x9040, 0x9040) AM_WRITE(irq_mask_w)
	AM_RANGE(0x9041, 0x9041) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x9042, 0x9042) AM_WRITE(pengo_palettebank_w)
	AM_RANGE(0x9043, 0x9043) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x9044, 0x9045) AM_WRITENOP			/* coin counters */
	AM_RANGE(0x9046, 0x9046) AM_WRITE(pengo_colortablebank_w)
	AM_RANGE(0x9047, 0x9047) AM_WRITE(pengo_gfxbank_w)
	AM_RANGE(0x9070, 0x9070) AM_WRITENOP
	AM_RANGE(0x9080, 0x90bf) AM_READ_PORT("IN1")
	AM_RANGE(0x90c0, 0x90ff) AM_READ_PORT("IN0")
ADDRESS_MAP_END

static ADDRESS_MAP_START( decrypted_opcodes_map, AS_DECRYPTED_OPCODES, 8, pengo_state )
	AM_RANGE(0x0000, 0x7fff) AM_ROM AM_SHARE("decrypted_opcodes")
	AM_RANGE(0x8800, 0x8fef) AM_RAM AM_SHARE("mainram")
	AM_RANGE(0x8ff0, 0x8fff) AM_RAM AM_SHARE("spriteram")
ADDRESS_MAP_END

/*************************************
 *
 *  Port definitions
 *
 *************************************/

INPUT_PORTS_START( pengo )
	PORT_START ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	/* the coin input must stay low for no less than 2 frames and no more */
	/* than 9 frames to pass the self test check. */
	/* Moreover, this way we avoid the game freezing until the user releases */
	/* the "coin" key. */
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN1 ) PORT_IMPULSE(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN2 ) PORT_IMPULSE(2)
	/* Coin Aux doesn't need IMPULSE to pass the test, but it still needs it */
	/* to avoid the freeze. */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN3 ) PORT_IMPULSE(2)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 )

	PORT_START ("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_SERVICE_NO_TOGGLE(0x10, IP_ACTIVE_LOW)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_COCKTAIL

	PORT_START ("DSW0")
	PORT_DIPNAME( 0x01, 0x00, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x00, "30000" )
	PORT_DIPSETTING(    0x01, "50000" )
	PORT_DIPNAME( 0x02, 0x00, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x18, 0x10, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x18, "2" )
	PORT_DIPSETTING(    0x10, "3" )
	PORT_DIPSETTING(    0x08, "4" )
	PORT_DIPSETTING(    0x00, "5" )
	PORT_DIPNAME( 0x20, 0x20, "Rack Test")	/* HBMAME - removed F1 because it doesn't work */
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0xc0, 0x80, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(    0xc0, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Medium ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Hard ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hardest ) )

	PORT_START ("DSW1")
	PORT_DIPNAME( 0x0f, 0x0c, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x04, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x09, "2 Coins/1 Credit 5/3" )
	PORT_DIPSETTING(    0x05, "2 Coins/1 Credit 4/3" )
	PORT_DIPSETTING(    0x0c, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x0d, "1 Coin/1 Credit 5/6" )
	PORT_DIPSETTING(    0x03, "1 Coin/1 Credit 4/5" )
	PORT_DIPSETTING(    0x0b, "1 Coin/1 Credit 2/3" )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x07, "1 Coin/2 Credits 5/11" )
	PORT_DIPSETTING(    0x0f, "1 Coin/2 Credits 4/9" )
	PORT_DIPSETTING(    0x0a, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0x0e, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_6C ) )
	PORT_DIPNAME( 0xf0, 0xc0, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x80, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x40, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x90, "2 Coins/1 Credit 5/3" )
	PORT_DIPSETTING(    0x50, "2 Coins/1 Credit 4/3" )
	PORT_DIPSETTING(    0xc0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0xd0, "1 Coin/1 Credit 5/6" )
	PORT_DIPSETTING(    0x30, "1 Coin/1 Credit 4/5" )
	PORT_DIPSETTING(    0xb0, "1 Coin/1 Credit 2/3" )
	PORT_DIPSETTING(    0x20, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x70, "1 Coin/2 Credits 5/11" )
	PORT_DIPSETTING(    0xf0, "1 Coin/2 Credits 4/9" )
	PORT_DIPSETTING(    0xa0, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x60, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0xe0, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0x10, DEF_STR( 1C_6C ) )
INPUT_PORTS_END


/*************************************
 *
 *  Graphics layouts
 *
 *************************************/

static const gfx_layout tilelayout =
{
	8,8,	/* 8*8 characters */
    RGN_FRAC(1,2),    /* 256 characters */
    2,  /* 2 bits per pixel */
    { 0, 4 },   /* the two bitplanes for 4 pixels are packed into one byte */
    { 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 }, /* bits are packed in groups of four */
    { 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
    16*8    /* every char takes 16 bytes */
};


static const gfx_layout spritelayout =
{
	16,16,	/* 16*16 sprites */
	RGN_FRAC(1,2),	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8,
			32*8, 33*8, 34*8, 35*8, 36*8, 37*8, 38*8, 39*8 },
	64*8	/* every sprite takes 64 bytes */
};


static GFXDECODE_START( pengo )
	GFXDECODE_ENTRY( "gfx1", 0x0000, tilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x2000, spritelayout, 0, 32 )
GFXDECODE_END




/*************************************
 *
 *  Machine drivers
 *
 *************************************/

static MACHINE_CONFIG_START( pengo, pengo_state )

	/* basic machine hardware */
	MCFG_CPU_ADD ("maincpu", Z80, MASTER_CLOCK/6)
	MCFG_CPU_PROGRAM_MAP(pengo_map)
	MCFG_CPU_DECRYPTED_OPCODES_MAP(decrypted_opcodes_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", pacman_state, irq0_line_hold)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_RAW_PARAMS(PIXEL_CLOCK, HTOTAL, HBEND, HBSTART, VTOTAL, VBEND, VBSTART)
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_pacman)
	MCFG_VIDEO_START_OVERRIDE(pacman_state,pengo)
	MCFG_SCREEN_PALETTE("palette")
	MCFG_GFXDECODE_ADD("gfxdecode", "palette", pengo)
	MCFG_PALETTE_ADD("palette", 128*4)
	MCFG_PALETTE_INDIRECT_ENTRIES(32)
	MCFG_PALETTE_INIT_OWNER(pacman_state,pacman)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("namco", NAMCO, MASTER_CLOCK/6/32)
	MCFG_NAMCO_AUDIO_VOICES(3)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( pengou, pengo )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_DEVICE_REMOVE_ADDRESS_MAP(AS_DECRYPTED_OPCODES)
MACHINE_CONFIG_END

/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( pengo )
	ROM_REGION( 0x10000, "maincpu", 0 ) /* 64k for code */
	ROM_LOAD( "ep1689c.8",    0x0000, 0x1000, CRC(f37066a8) SHA1(0930de17a763a527057f60783a92662b09554426) )
	ROM_LOAD( "ep1690b.7",    0x1000, 0x1000, CRC(baf48143) SHA1(4c97529e61eeca5d94938b1dfbeac41bf8cbaf7d) )
	ROM_LOAD( "ep1691b.15",   0x2000, 0x1000, CRC(adf0eba0) SHA1(c8949fbdbfe5023ee17a789ef60205e834a76c81) )
	ROM_LOAD( "ep1692b.14",   0x3000, 0x1000, CRC(a086d60f) SHA1(7079769d14dfe3873ffe29623ba0a93413706c6d) )
	ROM_LOAD( "ep1693b.21",   0x4000, 0x1000, CRC(b72084ec) SHA1(c0508951c2ad8dc31481be8b3bfee2063e3fb0d7) )
	ROM_LOAD( "ep1694b.20",   0x5000, 0x1000, CRC(94194a89) SHA1(7b47aec61593efd758e2a031f72a854bb0ba8af1) )
	ROM_LOAD( "ep5118b.32",   0x6000, 0x1000, CRC(af7b12c4) SHA1(207ed466546f40ca60a38031b83aef61446902e2) )
	ROM_LOAD( "ep5119c.31",   0x7000, 0x1000, CRC(933950fe) SHA1(fec7236b3dee2ea6e39c68440a6d2d9e3f72675a) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "ep1640.92",    0x0000, 0x1000, CRC(d7eec6cd) SHA1(e542bcc28f292be9a0a29d949de726e0b55e654a) ) /* tiles (bank 1) */
	ROM_CONTINUE(             0x2000, 0x1000 ) /* sprites (bank 1)*/
	ROM_LOAD( "ep1695.105",   0x1000, 0x1000, CRC(5bfd26e9) SHA1(bdec535e486b43a8f5550334beff423eeace10b2) ) /* tiles (bank 2) */
	ROM_CONTINUE(             0x3000, 0x1000 ) /* sprites (bank 2) */

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "pr1633.78",    0x0000, 0x0020, CRC(3a5844ec) SHA1(680eab0e1204c9b74adc11588461651b474021bb) ) /* color palette */
	ROM_LOAD( "pr1634.88",    0x0020, 0x0400, CRC(766b139b) SHA1(3fcd66610fcaee814953a115bf5e04788923181f) ) /* color lookup */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "pr1635.51",    0x0000, 0x0100, CRC(c29dea27) SHA1(563c9770028fe39188e62630711589d6ed242a66) ) /* waveform */
	ROM_LOAD( "pr1636.70",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) /* timing - not used */
ROM_END



/*************************************
 *
 *  Driver initialization
 *
 *************************************/

DRIVER_INIT_MEMBER(pengo_state,pengo)
{
	static const UINT8 convtable[32][4] =
	{
		/*       opcode                   data                     address      */
		/*  A    B    C    D         A    B    C    D                           */
		{ 0xa0,0x80,0xa8,0x88 }, { 0x28,0xa8,0x08,0x88 },   /* ...0...0...0...0 */
		{ 0x28,0xa8,0x08,0x88 }, { 0xa0,0x80,0xa8,0x88 },   /* ...0...0...0...1 */
		{ 0xa0,0x80,0x20,0x00 }, { 0xa0,0x80,0x20,0x00 },   /* ...0...0...1...0 */
		{ 0x08,0x28,0x88,0xa8 }, { 0xa0,0x80,0xa8,0x88 },   /* ...0...0...1...1 */
		{ 0x08,0x00,0x88,0x80 }, { 0x28,0xa8,0x08,0x88 },   /* ...0...1...0...0 */
		{ 0xa0,0x80,0x20,0x00 }, { 0x08,0x00,0x88,0x80 },   /* ...0...1...0...1 */
		{ 0xa0,0x80,0x20,0x00 }, { 0xa0,0x80,0x20,0x00 },   /* ...0...1...1...0 */
		{ 0xa0,0x80,0x20,0x00 }, { 0x00,0x08,0x20,0x28 },   /* ...0...1...1...1 */
		{ 0x88,0x80,0x08,0x00 }, { 0xa0,0x80,0x20,0x00 },   /* ...1...0...0...0 */
		{ 0x88,0x80,0x08,0x00 }, { 0x00,0x08,0x20,0x28 },   /* ...1...0...0...1 */
		{ 0x08,0x28,0x88,0xa8 }, { 0x08,0x28,0x88,0xa8 },   /* ...1...0...1...0 */
		{ 0xa0,0x80,0xa8,0x88 }, { 0xa0,0x80,0x20,0x00 },   /* ...1...0...1...1 */
		{ 0x08,0x00,0x88,0x80 }, { 0x88,0x80,0x08,0x00 },   /* ...1...1...0...0 */
		{ 0x00,0x08,0x20,0x28 }, { 0x88,0x80,0x08,0x00 },   /* ...1...1...0...1 */
		{ 0x08,0x28,0x88,0xa8 }, { 0x08,0x28,0x88,0xa8 },   /* ...1...1...1...0 */
		{ 0x08,0x00,0x88,0x80 }, { 0xa0,0x80,0x20,0x00 }    /* ...1...1...1...1 */
	};

	sega_decode(memregion("maincpu")->base(), m_decrypted_opcodes, 0x8000, convtable);
}



/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 1982, pengo, 0, pengo, pengo, pengo_state, pengo, ROT90, "Sega", "Pengo (set 1 rev c)", 0 )


/* The colours for alpacap7 and alpacap8 are completely wrong, but we are using the roms specified by
	the original programmer. See http://umlautllama.com/projects/alpaca */
ROM_START( alpacap7 )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "alp7.u8",		0x0000, 0x1000, CRC(E70923E4) SHA1(86A96DC0DEE5F5B532121892528ED2B456D51174) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "alp7_ic92",		0x0000, 0x1000, CRC(693B1A96) SHA1(DED8AC0A4186458FDA6A241C027718742BA87973) )
	ROM_CONTINUE(             0x2000, 0x1000 ) /* sprites (bank 1) */
	ROM_LOAD( "alp_ic105",		0x1000, 0x1000, CRC(BDA193F4) SHA1(A61B5E86809FEFF025C73DE78FC751EAD646125B) )
	ROM_CONTINUE(             0x3000, 0x1000 ) /* sprites (bank 1) */

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "pr1633.78",    0x0000, 0x0020, CRC(3a5844ec) SHA1(680eab0e1204c9b74adc11588461651b474021bb) ) /* color palette */
	ROM_LOAD( "pr1634.88",    0x0020, 0x0400, CRC(766b139b) SHA1(3fcd66610fcaee814953a115bf5e04788923181f) ) /* color lookup */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "pr1635.51",    0x0000, 0x0100, CRC(c29dea27) SHA1(563c9770028fe39188e62630711589d6ed242a66) ) /* waveform */
	ROM_LOAD( "pr1636.70",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) /* timing - not used */
ROM_END

ROM_START( alpacap8 )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "alp8.u8",		0x0000, 0x1000, CRC(ABD45FD7) SHA1(120276D1E9A3DA707DB4263A93418CA37F4F4C9F) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "alp8_ic92",		0x0000, 0x1000, CRC(4A45717F) SHA1(159A74C31FC5D4E5405BCD8D8843A1B381EA01A4) )
	ROM_CONTINUE(             0x2000, 0x1000 ) /* sprites (bank 1) */
	ROM_LOAD( "alp_ic105",		0x1000, 0x1000, CRC(BDA193F4) SHA1(A61B5E86809FEFF025C73DE78FC751EAD646125B) )
	ROM_CONTINUE(             0x3000, 0x1000 ) /* sprites (bank 1) */

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "pr1633.78",    0x0000, 0x0020, CRC(3a5844ec) SHA1(680eab0e1204c9b74adc11588461651b474021bb) ) /* color palette */
	ROM_LOAD( "pr1634.88",    0x0020, 0x0400, CRC(766b139b) SHA1(3fcd66610fcaee814953a115bf5e04788923181f) ) /* color lookup */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "pr1635.51",    0x0000, 0x0100, CRC(c29dea27) SHA1(563c9770028fe39188e62630711589d6ed242a66) ) /* waveform */
	ROM_LOAD( "pr1636.70",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) /* timing - not used */
ROM_END

ROM_START( pengopop )
	ROM_REGION( 0x10000, "maincpu", 0 ) /* 64k for code */
	ROM_LOAD( "pengo.u8",     0x0000, 0x1000, CRC(3dfeb20e) SHA1(a387b72501da77bf38b58619d2099083a0463e1f) )
	ROM_LOAD( "pengo.u7",     0x1000, 0x1000, CRC(1db341bd) SHA1(d1c66bb9cf479e6960dbcd35c820097a81eaa555) )
	ROM_LOAD( "pengo.u15",    0x2000, 0x1000, CRC(7c2842d5) SHA1(a8a568da68babd0ccb9f2cee4182fc01c3138494) )
	ROM_LOAD( "pengo.u14",    0x3000, 0x1000, CRC(6e3c1f2f) SHA1(2ee821b0f6e0f3cfeae7f5ff25a6e9bd977efce0) )
	ROM_LOAD( "ep5124.21",    0x4000, 0x1000, CRC(95f354ff) SHA1(fdebc68a6d87f8ecdf52a57a34ae5ae844a13510) )
	ROM_LOAD( "pengo.u20",    0x5000, 0x1000, CRC(0fdb04b8) SHA1(ed814d58318c1055e475ff678609d189727bf9b4) )
	ROM_LOAD( "ep5126.32",    0x6000, 0x1000, CRC(e5920728) SHA1(0ac5ffdad7bdcb32e630b9582e1b1aaece5198c9) )
	ROM_LOAD( "pengopc.u31",  0x7000, 0x1000, CRC(1ede8569) SHA1(0d10a0896847a06185a91eb83c0ccb88c4307b33) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "ep1640.92",    0x0000, 0x1000, CRC(d7eec6cd) SHA1(e542bcc28f292be9a0a29d949de726e0b55e654a) ) /* tiles (bank 1) */
	ROM_CONTINUE(             0x2000, 0x1000 ) /* sprites (bank 1) */
	ROM_LOAD( "ep1695.105",   0x1000, 0x1000, CRC(5bfd26e9) SHA1(bdec535e486b43a8f5550334beff423eeace10b2) ) /* tiles (bank 2) */
	ROM_CONTINUE(             0x3000, 0x1000 ) /* sprites (bank 2) */

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "pr1633.78",    0x0000, 0x0020, CRC(3a5844ec) SHA1(680eab0e1204c9b74adc11588461651b474021bb) ) /* color palette */
	ROM_LOAD( "pr1634.88",    0x0020, 0x0400, CRC(766b139b) SHA1(3fcd66610fcaee814953a115bf5e04788923181f) ) /* color lookup */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "pr1635.51",    0x0000, 0x0100, CRC(c29dea27) SHA1(563c9770028fe39188e62630711589d6ed242a66) ) /* waveform */
	ROM_LOAD( "pr1636.70",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) /* timing - not used */
ROM_END

ROM_START( vecpengo )
	ROM_REGION( 2*0x10000, "maincpu", 0 )     /* 64k for code + 64k for decrypted opcodes */
	ROM_LOAD( "ep1689c.8",    0x0000, 0x1000, CRC(f37066a8) SHA1(0930de17a763a527057f60783a92662b09554426) )
	ROM_LOAD( "ep1690b.7",    0x1000, 0x1000, CRC(baf48143) SHA1(4c97529e61eeca5d94938b1dfbeac41bf8cbaf7d) )
	ROM_LOAD( "ep1691b.15",   0x2000, 0x1000, CRC(adf0eba0) SHA1(c8949fbdbfe5023ee17a789ef60205e834a76c81) )
	ROM_LOAD( "ep1692b.14",   0x3000, 0x1000, CRC(a086d60f) SHA1(7079769d14dfe3873ffe29623ba0a93413706c6d) )
	ROM_LOAD( "ep1693b.21",   0x4000, 0x1000, CRC(b72084ec) SHA1(c0508951c2ad8dc31481be8b3bfee2063e3fb0d7) )
	ROM_LOAD( "ep1694b.20",   0x5000, 0x1000, CRC(94194a89) SHA1(7b47aec61593efd758e2a031f72a854bb0ba8af1) )
	ROM_LOAD( "ep5118b.32",   0x6000, 0x1000, CRC(af7b12c4) SHA1(207ed466546f40ca60a38031b83aef61446902e2) )
	ROM_LOAD( "ep5119c.31",   0x7000, 0x1000, CRC(933950fe) SHA1(fec7236b3dee2ea6e39c68440a6d2d9e3f72675a) )

	ROM_REGION( 0x4000, "gfx1", 0 )
	ROM_LOAD( "vecp_ic92",    0x0000, 0x1000, CRC(57c5e53c) SHA1(4d1d4cdc352cb2fd14ebbd6678211093be73fb69) ) /* tiles (bank 1) */
	ROM_CONTINUE(         	  0x2000, 0x1000 ) /* sprites (bank 1) */
	ROM_LOAD( "vecp_ic105",   0x1000, 0x1000, CRC(b93588b0) SHA1(bbb779e538bdf7ebfcb0e12e11b57cabd5ddd29d) ) /* tiles (bank 2) */
	ROM_CONTINUE(             0x3000, 0x1000 ) /* sprites (bank 2) */

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "pr1633.78",	  0x0000, 0x0020, CRC(3a5844ec) SHA1(680eab0e1204c9b74adc11588461651b474021bb) ) /* color palette */
	ROM_LOAD( "pr1634.88",    0x0020, 0x0400, CRC(766b139b) SHA1(3fcd66610fcaee814953a115bf5e04788923181f) ) /* color lookup */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "pr1635.51",    0x0000, 0x0100, CRC(c29dea27) SHA1(563c9770028fe39188e62630711589d6ed242a66) ) /* waveform */
	ROM_LOAD( "pr1636.70",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) /* timing - not used */
ROM_END


GAME( 2003, alpacap7, alpaca8, pengou, pengo, driver_device, 0,     ROT90, "Scott Lawrence", "Alpaca v0.7 (Pengo Hardware)", MACHINE_IMPERFECT_COLORS )
GAME( 2003, alpacap8, alpaca8, pengou, pengo, driver_device, 0,     ROT90, "Scott Lawrence", "Alpaca v0.8 (Pengo Hardware)", MACHINE_IMPERFECT_COLORS )
GAME( 1997, pengopop, pengo,   pengou, pengo, driver_device, 0,     ROT90, "Sega", "Pengo (Popcorn Music)", 0 )
GAME( 2000, vecpengo, pengo,   pengo,  pengo, pengo_state,   pengo, ROT90, "T-Bone", "Pengo (Vector sim)", 0 )
