// license:BSD-3-Clause
// copyright-holders:Robbbert
/* ===== MS PAC MAN ======

*/

#define mspacman_enable_decode_latch(m)  m.root_device().membank("bank1")->set_entry(1)
#define mspacman_disable_decode_latch(m) m.root_device().membank("bank1")->set_entry(0)

// any access to these ROM addresses disables the decoder, and all you see is the original Pac-Man code
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x0038){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x0038]; }
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x03b0){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x03b0]; }
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x1600){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x1600]; }
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x2120){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x2120]; }
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x3ff0){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x3ff0]; }
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x8000){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x8000]; }
READ8_MEMBER(pacman_state::mspacman_disable_decode_r_0x97f0){ mspacman_disable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x97f0]; }
WRITE8_MEMBER(pacman_state::mspacman_disable_decode_w){ mspacman_disable_decode_latch(machine()); }

// any access to these ROM addresses enables the decoder, and you'll see the Ms. Pac-Man code
READ8_MEMBER(pacman_state::mspacman_enable_decode_r_0x3ff8){ mspacman_enable_decode_latch(machine()); return memregion("maincpu")->base()[offset+0x3ff8+0x10000]; }
WRITE8_MEMBER(pacman_state::mspacman_enable_decode_w){ mspacman_enable_decode_latch(machine()); }

#define BITSWAP12(val,B11,B10,B9,B8,B7,B6,B5,B4,B3,B2,B1,B0) \
	BITSWAP16(val,15,14,13,12,B11,B10,B9,B8,B7,B6,B5,B4,B3,B2,B1,B0)

#define BITSWAP11(val,B10,B9,B8,B7,B6,B5,B4,B3,B2,B1,B0) \
	BITSWAP16(val,15,14,13,12,11,B10,B9,B8,B7,B6,B5,B4,B3,B2,B1,B0)

void pacman_state::mspacman_install_patches(UINT8 *ROM)
{
	int i;

	/* copy forty 8-byte patches into Pac-Man code */
	for (i = 0; i < 8; i++)
	{
		ROM[0x0410+i] = ROM[0x8008+i];
		ROM[0x08E0+i] = ROM[0x81D8+i];
		ROM[0x0A30+i] = ROM[0x8118+i];
		ROM[0x0BD0+i] = ROM[0x80D8+i];
		ROM[0x0C20+i] = ROM[0x8120+i];
		ROM[0x0E58+i] = ROM[0x8168+i];
		ROM[0x0EA8+i] = ROM[0x8198+i];

		ROM[0x1000+i] = ROM[0x8020+i];
		ROM[0x1008+i] = ROM[0x8010+i];
		ROM[0x1288+i] = ROM[0x8098+i];
		ROM[0x1348+i] = ROM[0x8048+i];
		ROM[0x1688+i] = ROM[0x8088+i];
		ROM[0x16B0+i] = ROM[0x8188+i];
		ROM[0x16D8+i] = ROM[0x80C8+i];
		ROM[0x16F8+i] = ROM[0x81C8+i];
		ROM[0x19A8+i] = ROM[0x80A8+i];
		ROM[0x19B8+i] = ROM[0x81A8+i];

		ROM[0x2060+i] = ROM[0x8148+i];
		ROM[0x2108+i] = ROM[0x8018+i];
		ROM[0x21A0+i] = ROM[0x81A0+i];
		ROM[0x2298+i] = ROM[0x80A0+i];
		ROM[0x23E0+i] = ROM[0x80E8+i];
		ROM[0x2418+i] = ROM[0x8000+i];
		ROM[0x2448+i] = ROM[0x8058+i];
		ROM[0x2470+i] = ROM[0x8140+i];
		ROM[0x2488+i] = ROM[0x8080+i];
		ROM[0x24B0+i] = ROM[0x8180+i];
		ROM[0x24D8+i] = ROM[0x80C0+i];
		ROM[0x24F8+i] = ROM[0x81C0+i];
		ROM[0x2748+i] = ROM[0x8050+i];
		ROM[0x2780+i] = ROM[0x8090+i];
		ROM[0x27B8+i] = ROM[0x8190+i];
		ROM[0x2800+i] = ROM[0x8028+i];
		ROM[0x2B20+i] = ROM[0x8100+i];
		ROM[0x2B30+i] = ROM[0x8110+i];
		ROM[0x2BF0+i] = ROM[0x81D0+i];
		ROM[0x2CC0+i] = ROM[0x80D0+i];
		ROM[0x2CD8+i] = ROM[0x80E0+i];
		ROM[0x2CF0+i] = ROM[0x81E0+i];
		ROM[0x2D60+i] = ROM[0x8160+i];
	}
}

DRIVER_INIT_MEMBER(pacman_state,mspacman)
{
	int i;
	UINT8 *ROM, *DROM;

	/* CPU ROMs */

	/* Pac-Man code is in low bank */
	ROM = machine().root_device().memregion("maincpu")->base();

	/* decrypted Ms. Pac-Man code is in high bank */
	DROM = &machine().root_device().memregion("maincpu")->base()[0x10000];

	/* copy ROMs into decrypted bank */
	for (i = 0; i < 0x1000; i++)
	{
		DROM[0x0000+i] = ROM[0x0000+i]; /* pacman.6e */
		DROM[0x1000+i] = ROM[0x1000+i]; /* pacman.6f */
		DROM[0x2000+i] = ROM[0x2000+i]; /* pacman.6h */
		DROM[0x3000+i] = BITSWAP8(ROM[0xb000+BITSWAP12(i,11,3,7,9,10,8,6,5,4,2,1,0)],0,4,5,7,6,3,2,1);  /* decrypt u7 */
	}
	for (i = 0; i < 0x800; i++)
	{
		DROM[0x8000+i] = BITSWAP8(ROM[0x8000+BITSWAP11(i,   8,7,5,9,10,6,3,4,2,1,0)],0,4,5,7,6,3,2,1);  /* decrypt u5 */
		DROM[0x8800+i] = BITSWAP8(ROM[0x9800+BITSWAP12(i,11,3,7,9,10,8,6,5,4,2,1,0)],0,4,5,7,6,3,2,1);  /* decrypt half of u6 */
		DROM[0x9000+i] = BITSWAP8(ROM[0x9000+BITSWAP12(i,11,3,7,9,10,8,6,5,4,2,1,0)],0,4,5,7,6,3,2,1);  /* decrypt half of u6 */
		DROM[0x9800+i] = ROM[0x1800+i];     /* mirror of pacman.6f high */
	}
	for (i = 0; i < 0x1000; i++)
	{
		DROM[0xa000+i] = ROM[0x2000+i];     /* mirror of pacman.6h */
		DROM[0xb000+i] = ROM[0x3000+i];     /* mirror of pacman.6j */
	}
	/* install patches into decrypted bank */
	mspacman_install_patches(DROM);

	/* mirror Pac-Man ROMs into upper addresses of normal bank */
	for (i = 0; i < 0x1000; i++)
	{
		ROM[0x8000+i] = ROM[0x0000+i];
		ROM[0x9000+i] = ROM[0x1000+i];
		ROM[0xa000+i] = ROM[0x2000+i];
		ROM[0xb000+i] = ROM[0x3000+i];
	}

	/* initialize the banks */
	machine().root_device().membank("bank1")->configure_entries(0, 2, &ROM[0x00000], 0x10000);
	machine().root_device().membank("bank1")->set_entry(1);
}



/* used by extra routine at $3FE, bit 4 of 504d needs to be low, and of 504e to be high */
READ8_MEMBER( pacman_state::mspacii_prot_r )
{
	return offset<<4;
}

/* For Zola's timer */

READ8_MEMBER( pacman_state::zolatimer_r )
{
	UINT8 timernow = m_timerthing;
	m_timerthing++;
	return timernow;
}

WRITE8_MEMBER( pacman_state::zolatimer_w )	/* to stop it hanging when game reset */
{
	m_timerthing = data;
}


/********************************
	Memory Maps
 ********************************/


/* Same as woodhack, plus it has nvram, and some extra ram at fffc */
static ADDRESS_MAP_START( mspachi_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0xa000) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0xa000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0xa000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x5080) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")
	AM_RANGE(0x7000, 0x70ff) AM_RAM
	AM_RANGE(0x8000, 0xbfff) AM_ROM
	AM_RANGE(0xfffc, 0xffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( mspacii_map, AS_PROGRAM, 8, pacman_state )
	/* do not use UNMAP_HIGH (protection issues) */
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0xa000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0xa000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0xa000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0x8000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_MIRROR(0x8000) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_MIRROR(0x8000) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_MIRROR(0x8000) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_MIRROR(0x8000) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_MIRROR(0x8000) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_MIRROR(0x8000) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_MIRROR(0x8000) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_MIRROR(0x8000) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x5080) AM_MIRROR(0x8000) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0x8000) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0x8000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_MIRROR(0x8000) AM_READ_PORT("IN1")
	AM_RANGE(0x504d, 0x506f) AM_READ(mspacii_prot_r)
	AM_RANGE(0x5080, 0x5080) AM_MIRROR(0x8000) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0x8000) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0x9fff) AM_ROM
	AM_RANGE(0xfffc, 0xffff) AM_WRITENOP
ADDRESS_MAP_END

static ADDRESS_MAP_START( mspacman_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0xa000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0xa000) AM_READ(pacman_read_nop) AM_WRITENOP
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0xa000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0xa000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0xaf38) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_MIRROR(0xaf38) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_MIRROR(0xaf38) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_MIRROR(0xaf38) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_MIRROR(0xaf38) AM_WRITENOP // AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_MIRROR(0xaf38) AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_MIRROR(0xaf38) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_MIRROR(0xaf00) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_MIRROR(0xaf00) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_MIRROR(0xaf00) AM_WRITENOP
	AM_RANGE(0x5080, 0x5080) AM_MIRROR(0xaf3f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0xaf3f) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0xaf3f) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_MIRROR(0xaf3f) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_MIRROR(0xaf3f) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0xaf3f) AM_READ_PORT("DSW2")

	/* overlay decode enable/disable on top */
	AM_RANGE(0x0038, 0x003f) AM_READWRITE(mspacman_disable_decode_r_0x0038,mspacman_disable_decode_w)
	AM_RANGE(0x03b0, 0x03b7) AM_READWRITE(mspacman_disable_decode_r_0x03b0,mspacman_disable_decode_w)
	AM_RANGE(0x1600, 0x1607) AM_READWRITE(mspacman_disable_decode_r_0x1600,mspacman_disable_decode_w)
	AM_RANGE(0x2120, 0x2127) AM_READWRITE(mspacman_disable_decode_r_0x2120,mspacman_disable_decode_w)
	AM_RANGE(0x3ff0, 0x3ff7) AM_READWRITE(mspacman_disable_decode_r_0x3ff0,mspacman_disable_decode_w)
	AM_RANGE(0x3ff8, 0x3fff) AM_READWRITE(mspacman_enable_decode_r_0x3ff8,mspacman_enable_decode_w)
	AM_RANGE(0x8000, 0x8007) AM_READWRITE(mspacman_disable_decode_r_0x8000,mspacman_disable_decode_w)
	AM_RANGE(0x97f0, 0x97f7) AM_READWRITE(mspacman_disable_decode_r_0x97f0,mspacman_disable_decode_w)

	/* start with 0000-3fff and 8000-bfff mapped to the ROMs */
	AM_RANGE(0x4000, 0x7fff) AM_MIRROR(0x8000) AM_UNMAP
	AM_RANGE(0x0000, 0xffff) AM_ROMBANK("bank1")
ADDRESS_MAP_END


/* For Dave Widel's hacks */
static ADDRESS_MAP_START( widel_map, AS_PROGRAM, 8, pacman_state )
	/* bace and dderby set 5000-5FFF to zero for no apparent reason */
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP // mrmrspac
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x5080) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")	/* DSW2 - not used */
	AM_RANGE(0x5103, 0x5103) AM_WRITENOP			/* aa */
	AM_RANGE(0x5c0e, 0x5c0e) AM_RAM				/* mrmrspac */
	AM_RANGE(0x5fff, 0x5fff) AM_READNOP			/* aa */
	AM_RANGE(0x8000, 0x9fff) AM_ROM				/* only really needed by mrmrspac */
	AM_RANGE(0xd000, 0xd000) AM_RAM				/* mrmrspac */
	AM_RANGE(0xf000, 0xffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( zolapac_io, AS_IO, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_READ(zolatimer_r)
	AM_RANGE(0x10, 0x10) AM_WRITENOP		/* writes 0 at reset */
	AM_RANGE(0x11, 0x11) AM_WRITE(zolatimer_w)
ADDRESS_MAP_END


/*************************************
 *
 *  Machine drivers
 *
 *************************************/

/* only for encrypted sets with 20000 in cpu1 */
static MACHINE_CONFIG_DERIVED( mspacman, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mspacman_map)
MACHINE_CONFIG_END

/* Hires mspacman */
static MACHINE_CONFIG_DERIVED( mspacmnx, pacmanx )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mspacman_map)
MACHINE_CONFIG_END

/* Hires hacks */
static MACHINE_CONFIG_DERIVED( woodpekx, pacmanx )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(woodpek_map)
MACHINE_CONFIG_END

/* MSPacman with modified memory maps */

static MACHINE_CONFIG_DERIVED( mspacii, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mspacii_map)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( zolapac, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(woodpek_map)
	MCFG_CPU_IO_MAP(zolapac_io)
MACHINE_CONFIG_END

/* These drivers need the watchdog to be removed in order to work */

static MACHINE_CONFIG_DERIVED( pachack, pacman )
	MCFG_WATCHDOG_VBLANK_INIT(0)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( mspachi, pachack )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mspachi_map)
	MCFG_NVRAM_ADD_0FILL("nvram")

MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( widel, pachack )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(widel_map)
MACHINE_CONFIG_END


/*************************************
 *
 *  Driver initialization
 *
 *************************************/


/* The encryption is provided by a 74298 sitting on top of the rom at 6f.
The select line is tied to a2; a0 and a1 of the eprom are are left out of
socket and run through the 74298.  Clock is tied to system clock.  */
DRIVER_INIT_MEMBER( pacman_state, mspacmbe )
{
	UINT8 temp;
	UINT8 *RAM = machine().root_device().memregion("maincpu")->base();
	int i;

	/* Address lines A1 and A0 swapped if A2=0 */
	for(i = 0x1000; i < 0x2000; i+=4)
	{
		if (!(i & 8))
		{
			temp = RAM[i+1];
			RAM[i+1] = RAM[i+2];
			RAM[i+2] = temp;
		}
	}
}


/*************************************
 *
 *  ROM definitions
 *
 *************************************/


ROM_START( chtmsatk )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( chtmspa )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( cookiem )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "cookiem.1",    0x0000, 0x1000, CRC(3c95b06b) SHA1(c123de2f3246af4074e6a2f3a5a29aec0c6cd0f9) )
	ROM_LOAD( "cookiem.2",    0x1000, 0x1000, CRC(527859fb) SHA1(993baa017732e247e4c318a5f105a13c281fbd8d) ) // faulty
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "cookiem.4",    0x3000, 0x1000, CRC(66e397fd) SHA1(4e3d03abfaaa94a7a159fd51348ca6a7e4427272) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "cookiem.6",    0x9000, 0x1000, CRC(a96761a6) SHA1(73072f753f5659712e3644194a0982ef14648f46) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "cookiem.5e",   0x0000, 0x1000, CRC(ffb4de27) SHA1(ff12a37158e08f4d9ddad4184dfbd44f83120506) )
	ROM_LOAD( "cookiem.5f",   0x1000, 0x1000, CRC(1b5875ac) SHA1(9b0f4b96de9d7a2c7abcb06efaf083ff8ab28abe) )

	PACMAN_PROMS
ROM_END

ROM_START( cottoh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "cottoh.4",     0x3000, 0x1000, CRC(f0892bb8) SHA1(680d2335ca99128ad29295e161bd0bd52f9de0b8) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "cottoh.5e",    0x0000, 0x1000, CRC(8e78665a) SHA1(3d9c357bdcc7862d329d4f38c6d43bc792eee572) )
	ROM_LOAD( "cottoh.5f",    0x1000, 0x1000, CRC(4f83ae26) SHA1(9c308c1b684812747d573058ef157db14f03d2d9) )

	PACMAN_PROMS
ROM_END

ROM_START( cottoh2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "cottoh2.4",    0x3000, 0x1000, CRC(e285564b) SHA1(7b00549d680146fa2a88ce8a6ea45ece6e185d67) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "cottoh2.5e",   0x0000, 0x1000, CRC(3bbea74d) SHA1(613dc0989670347fa2827806b8408fc407d5386d) )
	ROM_LOAD( "cottoh2.5f",   0x1000, 0x1000, CRC(9c3d7558) SHA1(803aef874d988f112dea54f442e12973c799ea2e) )

	PACMAN_PROMS
ROM_END

ROM_START( fasthear )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "fastplus.2",   0x1000, 0x1000, CRC(a8d6c227) SHA1(b5891f911bb054ca2547e663a3c27c93897f2075) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "heartbn2.5e",  0x0000, 0x1000, CRC(28eb876c) SHA1(6bd3b9efdab76b3bf7c7f08ba9f44f830290df43) )
	ROM_LOAD( "heartbn2.5f",  0x1000, 0x1000, CRC(50fc9966) SHA1(21f79739b9884307430a35b2901098f45bbf5b7d) )

	PACMAN_PROMS
ROM_END

ROM_START( fastmspa )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "fastplus.2",   0x1000, 0x1000, CRC(a8d6c227) SHA1(b5891f911bb054ca2547e663a3c27c93897f2075) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( faststrm )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "fastplus.2",   0x1000, 0x1000, CRC(a8d6c227) SHA1(b5891f911bb054ca2547e663a3c27c93897f2075) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msdstorm.5e",  0x0000, 0x1000, CRC(b2940b89) SHA1(bfe7066fa11468d8373b43f966e76d92b17c0f48) )
	ROM_LOAD( "msdstorm.5f",  0x1000, 0x1000, CRC(5c65865f) SHA1(988b6deccd985a32d4561164bfabd80ebb43f0a6) )

	PACMAN_PROMS
ROM_END

ROM_START( fstmsatk )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "fastplus.2",   0x1000, 0x1000, CRC(a8d6c227) SHA1(b5891f911bb054ca2547e663a3c27c93897f2075) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( heartbn2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "heartbn2.5e",  0x0000, 0x1000, CRC(28eb876c) SHA1(6bd3b9efdab76b3bf7c7f08ba9f44f830290df43) )
	ROM_LOAD( "heartbn2.5f",  0x1000, 0x1000, CRC(50fc9966) SHA1(21f79739b9884307430a35b2901098f45bbf5b7d) )

	PACMAN_PROMS
ROM_END

ROM_START( heartbrn )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "heartbrn.5e",  0x0000, 0x1000, CRC(5431d4c4) SHA1(34d45da44b4208e2774f5e2af08657a9086252e6) )
	ROM_LOAD( "heartbrn.5f",  0x1000, 0x1000, CRC(ceb50654) SHA1(70dbe3cc715d3d52ee3d4f8dadbf5c59f87166a3) )

	PACMAN_PROMS
ROM_END

ROM_START( horizpab )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "horizpac.1",   0x0000, 0x1000, CRC(7CC1979D) SHA1(557346859DCFCE806E83DCF0DA1FE60DA20FED81) )
	ROM_LOAD( "horizpac.2",   0x1000, 0x1000, CRC(14202B73) SHA1(136B860D6AC8EFB9C83B67F77FBB47C3014D4ADC) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821EE0B) SHA1(5EA4D907DBB2690698DB72C4E0B5BE4D3E9A7786) )
	ROM_LOAD( "horizpab.4",   0x3000, 0x1000, CRC(c9df7abe) SHA1(da21a182b81181260e2a1615115d22797bd1434a) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8C3E6DE6) SHA1(FED6E9A2B210B07E7189A18574F6B8C4EC5BB49B) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368CB165) SHA1(387010A0C76319A1EAB61B54C9BCB5C66C4B67A1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "horizpac.5e",  0x0000, 0x1000, CRC(0572335D) SHA1(F2D8BB3124970B298026E841E057FBCC7CDFC4B1) )
	ROM_LOAD( "horizpac.5f",  0x1000, 0x1000, CRC(4198511D) SHA1(B3808062835AA0CC05FA19A678A5C8AB8FB646D7) )

	PACMAN_PROMS
ROM_END

ROM_START( horizpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "horizpac.1",   0x0000, 0x1000, CRC(7CC1979D) SHA1(557346859DCFCE806E83DCF0DA1FE60DA20FED81) )
	ROM_LOAD( "horizpac.2",   0x1000, 0x1000, CRC(14202B73) SHA1(136B860D6AC8EFB9C83B67F77FBB47C3014D4ADC) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821EE0B) SHA1(5EA4D907DBB2690698DB72C4E0B5BE4D3E9A7786) )
	ROM_LOAD( "horizpac.4",   0x3000, 0x1000, CRC(9A17E8DA) SHA1(2696643705878207952C4559C176D486BD0027C2) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8C3E6DE6) SHA1(FED6E9A2B210B07E7189A18574F6B8C4EC5BB49B) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368CB165) SHA1(387010A0C76319A1EAB61B54C9BCB5C66C4B67A1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "horizpac.5e",  0x0000, 0x1000, CRC(0572335D) SHA1(F2D8BB3124970B298026E841E057FBCC7CDFC4B1) )
	ROM_LOAD( "horizpac.5f",  0x1000, 0x1000, CRC(4198511D) SHA1(B3808062835AA0CC05FA19A678A5C8AB8FB646D7) )

	PACMAN_PROMS
ROM_END

ROM_START( mrmrspac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mrmrspac.1",   0x0000, 0x1000, CRC(02a86998) SHA1(649cfe5140eb63e42069ae9ad6c426837179fcbf) )
	ROM_LOAD( "mrmrspac.2",   0x1000, 0x1000, CRC(8917172f) SHA1(7b09ab3b6f2a1b5d889a05295b42f70bafb5aa98) )
	ROM_LOAD( "mrmrspac.3",   0x2000, 0x1000, CRC(6fd1444a) SHA1(64a98336eec75bb6ed90e77da18db8b71af562c1) )
	ROM_LOAD( "mrmrspac.4",   0x3000, 0x1000, CRC(22f68a1f) SHA1(bd8d684f957ee33dace9eddcc923006036181787) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "mrmrspac.6",   0x9000, 0x1000, CRC(23580455) SHA1(836b815c7b1dd4deb1a1297556d0a5c126250c34) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mrpacman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mrpacman.4",   0x3000, 0x1000, CRC(65c0526c) SHA1(b90f05a335a5ec641318dea15a97c9abb96ded2e) )
	ROM_LOAD( "mrpacman.5",   0x8000, 0x1000, CRC(4078d9b2) SHA1(c294735341504787fa7f4bc44543e132d975c0a2) )
	ROM_LOAD( "mrpacman.6",   0x9000, 0x1000, CRC(c1637f1c) SHA1(15577900bbba060e6c1b8dd03fcde19e7f4193c5) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mrpacman.5e",  0x0000, 0x1000, CRC(83e6a3a9) SHA1(9e76e0ff114778183324c905c7600ef848bf4a76) )
	ROM_LOAD( "mrpacman.5f",  0x1000, 0x1000, CRC(045dadf0) SHA1(1a28cb97ad547bddd3cc8e6d0102f7e1d43c71ea) )

	PACMAN_PROMS
ROM_END

ROM_START( ms1000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms1000.4",     0x3000, 0x1000, CRC(22a10392) SHA1(7c17601428988dd996bbe64afee486fcf2333beb) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms1000.5e",    0x0000, 0x1000, CRC(adde7864) SHA1(6e21d412b11bbabf05c65ad3c0dccba73fe15eb5) )
	ROM_LOAD( "ms1000.5f",    0x1000, 0x1000, CRC(5cb008a9) SHA1(bb9049b2a48773dac775acd70cb0cb54dcd6a5b0) )

	PACMAN_PROMS
ROM_END

ROM_START( ms2000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms2000.4",     0x3000, 0x1000, CRC(a72e49df) SHA1(396bf713cddeb5521c614ff8483b0baef23af7d0) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms2000.5e",    0x0000, 0x1000, CRC(2bcce741) SHA1(fd9729e54b049723c0fb6c47effe39724577ba0d) )
	ROM_LOAD( "ms2000.5f",    0x1000, 0x1000, CRC(4b40032a) SHA1(8002af3dfcfea82d56f5f04505d432c02dc56185) )

	PACMAN_PROMS
ROM_END

ROM_START( ms2600 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms2600.4",     0x3000, 0x1000, CRC(c2a293df) SHA1(98e864d1993c97293acea0febc9cac8eb0fb582c) )
	ROM_LOAD( "ms2600.5",     0x8000, 0x1000, CRC(4852db05) SHA1(8471e1640164dc311e0180a7a8323208a40cf889) )
	ROM_LOAD( "ms2600.6",     0x9000, 0x1000, CRC(f32b076f) SHA1(f361233a06e0376fe83a6fd0a071593c7c260cb1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms2600.5e",    0x0000, 0x1000, CRC(6b07b97a) SHA1(981ce12d92c57ed6c908d4b5b9b0c0b079d325db) )
	ROM_LOAD( "ms2600.5f",    0x1000, 0x1000, CRC(f844608b) SHA1(80b4a51536ac4c83974726cbfb36232e06334329) )

	PACMAN_PROMS
ROM_END

ROM_START( ms3000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms3000.4",     0x3000, 0x1000, CRC(46790576) SHA1(097366bb8008122bb8814e5db6228d1e5756cb41) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms3000.5e",    0x0000, 0x1000, CRC(1708299b) SHA1(c47d6d37f7c064f5bb22fa74c20fb259e9c694bf) )
	ROM_LOAD( "ms3000.5f",    0x1000, 0x1000, CRC(cf4798e3) SHA1(046958565ba31a63b36b89ee2ddc599196af1ec3) )

	PACMAN_PROMS
ROM_END

ROM_START( ms4000p )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms4000p.4",    0x3000, 0x1000, CRC(ff418db8) SHA1(eec95b43e451af2ab3dbbee83c577548097a90ef) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms4000p.5e",   0x0000, 0x1000, CRC(05ed8a9f) SHA1(8654596af1de7ddcd1795d9a38e75deb7e6f96c6) )
	ROM_LOAD( "ms4000p.5f",   0x1000, 0x1000, CRC(c0b58e02) SHA1(6ad23edff1b4a9324ea67ab28f3ab5e8362a55d8) )

	PACPLUS_PROMS
ROM_END

ROM_START( ms5000 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms2000.4",     0x3000, 0x1000, CRC(a72e49df) SHA1(396bf713cddeb5521c614ff8483b0baef23af7d0) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )
	ROM_FILL(0x3810, 1, 0x35) // put 5000 in because we don't have the actual ms5000 rom

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms5000.5e",    0x0000, 0x1000, CRC(fd1bd093) SHA1(b1b20614d2a06fa032373fb01e79b4004215e2b7) )
	ROM_LOAD( "ms5000.5f",    0x1000, 0x1000, CRC(07d39d56) SHA1(2482477c8a2130dd5eb279b99e541bc5229c0778) )

	PACMAN_PROMS
ROM_END

ROM_START( ms5000p )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "ms5000p.4",    0x3000, 0x1000, CRC(d7ff3b21) SHA1(478557c734062fc30a8b0252ebe04e15ec81a29a) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms5000p.5e",   0x0000, 0x1000, CRC(4fd847aa) SHA1(39e1f184df81a61325a4d3cf2ef04eb3703f3944) )
	ROM_LOAD( "ms5000p.5f",   0x1000, 0x1000, CRC(50d5fa64) SHA1(777e041901da0cae324524d64fa67b04b2f33bcc) )

	PACMAN_PROMS
ROM_END

ROM_START( msatk2ad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",   	  0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",   	  0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacat2.5",   0x8000, 0x1000, CRC(5a72a7a6) SHA1(aa54bf865e024fe00a8aefce6bc5086e90d71a54) )
	ROM_LOAD( "mspacat2.6",   0x9000, 0x1000, CRC(bd0c3e94) SHA1(1b01ed6eab2c31bb874d1e51e773d0c130de7b75) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msatkad.5e",   0x0000, 0x1000, CRC(dc70ed2e) SHA1(b9fbb1d21085bc78eb82c429506b20e332b18e0e) )
	ROM_LOAD( "msatkad.5f",   0x1000, 0x1000, CRC(643d5523) SHA1(565f92294a4b6a11dc5e9a165f35284c2a526e6c) )

	PACMAN_PROMS
ROM_END

ROM_START( msatkad )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",   	  0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",   	  0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",   	  0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msatkad.5e",   0x0000, 0x1000, CRC(dc70ed2e) SHA1(b9fbb1d21085bc78eb82c429506b20e332b18e0e) )
	ROM_LOAD( "msatkad.5f",   0x1000, 0x1000, CRC(643d5523) SHA1(565f92294a4b6a11dc5e9a165f35284c2a526e6c) )

	PACMAN_PROMS
ROM_END

ROM_START( msbaby )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msbaby.4",     0x3000, 0x1000, CRC(17aea21d) SHA1(2baa4b3812611ae8a1ff6b1b21b0a3ffd0d30be2) )
	ROM_LOAD( "msbaby.5",     0x8000, 0x1000, CRC(0c08aa7f) SHA1(0d4079d68c86cd64e4d309fdc43d2022a50f172d) )
	ROM_LOAD( "msbaby.6",     0x9000, 0x1000, CRC(e28aaa1f) SHA1(e0c4c1e0ceb3dfd6e63d954d3d6ea7924efac08b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msbaby.5e",    0x0000, 0x1000, CRC(e764738d) SHA1(79a7b871ee292d8cc2c4c97962fd876b4b95c4df) )
	ROM_LOAD( "msbaby.5f",    0x1000, 0x1000, CRC(3d316ad0) SHA1(fd7d1a02930cc1c8a2f4aa8518d805601a8e0a80) )

	PACMAN_PROMS
ROM_END

ROM_START( msberzk )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msberzk.4",    0x3000, 0x1000, CRC(795b1d20) SHA1(eb14ac6d2142fcfcbccf52eb7e8c51c208554eef) )
	ROM_LOAD( "msberzk.5",    0x8000, 0x1000, CRC(9fc2a679) SHA1(2684ecc09e2cf055daee181371691fc09cbd585a) )
	ROM_LOAD( "msberzk.6",    0x9000, 0x1000, CRC(6749a32d) SHA1(41b5b952c4f7f14b6a3884291c87e9ee6e760231) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msberzk.5e",   0x0000, 0x1000, CRC(92692429) SHA1(af5e72c445c7ea35655730364072c269befbc66f) )
	ROM_LOAD( "msberzk.5f",   0x1000, 0x1000, CRC(7c32ed2e) SHA1(124ff35143b1fcaa25d810a3479ae363cf74d2c9) )

	PACMAN_PROMS
ROM_END

ROM_START( mscrzyma )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mscrzyma.4",   0x3000, 0x1000, CRC(f2b17834) SHA1(4d687e681d21a9a702ed5e8022c9098b8347811a) )
	ROM_LOAD( "mspacnes.5",   0x8000, 0x1000, CRC(43936999) SHA1(4261f0bbb9ece2370c6283763a396ad851f42c51) )
	ROM_LOAD( "mspacnes.6",   0x9000, 0x1000, CRC(a15b524e) SHA1(91f211e479261b344fc64517394abf7a8f9d6f4b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ms2000.5e",    0x0000, 0x1000, CRC(2bcce741) SHA1(fd9729e54b049723c0fb6c47effe39724577ba0d) )
	ROM_LOAD( "mscrzyma.5f",  0x1000, 0x1000, CRC(2a9d6f77) SHA1(bcfec717967e64891d9638037ba5331b22654229) )

	PACMAN_PROMS
ROM_END

ROM_START( mscrzymp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mscrzymp.4",   0x3000, 0x1000, CRC(db440183) SHA1(ec4e7a7324f77818e7357e6c7c9e8af257fcbc7d) )
	ROM_LOAD( "mspacnes.5",   0x8000, 0x1000, CRC(43936999) SHA1(4261f0bbb9ece2370c6283763a396ad851f42c51) )
	ROM_LOAD( "mspacnes.6",   0x9000, 0x1000, CRC(a15b524e) SHA1(91f211e479261b344fc64517394abf7a8f9d6f4b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mscrzymp.5e",  0x0000, 0x1000, CRC(d43a7194) SHA1(2508b14778f8ff2d76b7ec00ba9fc30a0f606dcf) )
	ROM_LOAD( "mscrzymp.5f",  0x1000, 0x1000, CRC(7af54907) SHA1(d930b90f0500c3be451996ced9578ce870a1d6d5) )

	PACPLUS_PROMS
ROM_END

ROM_START( msdroid )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msdroid.4",    0x3000, 0x1000, CRC(984b3e88) SHA1(fb7c29adf0c7434c639fd82abf7aa9c709b37054) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msdroid.5e",   0x0000, 0x1000, CRC(97f5dd55) SHA1(14a1a8dda66223392fbae51af654607c56ac266e) )
	ROM_LOAD( "msdroid.5f",   0x1000, 0x1000, CRC(c0e47314) SHA1(d8a51d280bba4ad93db8019111ac4393aea9f9a2) )

	PACMAN_PROMS
ROM_END

ROM_START( msdstorm )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msdstorm.5e",  0x0000, 0x1000, CRC(b2940b89) SHA1(bfe7066fa11468d8373b43f966e76d92b17c0f48) )
	ROM_LOAD( "msdstorm.5f",  0x1000, 0x1000, CRC(5c65865f) SHA1(988b6deccd985a32d4561164bfabd80ebb43f0a6) )

	PACMAN_PROMS
ROM_END

ROM_START( mselton )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "mselton.5f",   0x1000, 0x1000, CRC(30d0b19f) SHA1(9964a5a103ce1975d8606d5cfd2306c05a74aca3) )

	PACMAN_PROMS
ROM_END

ROM_START( msextra )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msextra.4",    0x3000, 0x1000, CRC(b13d0521) SHA1(67e51e5447e5376a76ba1de6c600cf8aba6103f9) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msextra.5e",   0x0000, 0x1000, CRC(b656c0c5) SHA1(efa8aed573b4258a6f6da96b04968a9197ad2d33) )
	ROM_LOAD( "msextra.5f",   0x1000, 0x1000, CRC(c5d7017b) SHA1(29fa44d925526ab7f6385a2d4c750492063131ab) )

	PACMAN_PROMS
ROM_END

ROM_START( msextrap )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msextrap.4",   0x3000, 0x1000, CRC(728f7465) SHA1(c3e089d2c7bace010e29f4cb861baa888007e8bf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msextrap.5e",  0x0000, 0x1000, CRC(1a2fa3e2) SHA1(dafccc8123093a9f5306c0c19367c3accd755ca1) )
	ROM_LOAD( "msextrap.5f",  0x1000, 0x1000, CRC(5a400cbc) SHA1(7351b9c7f196fc49fdab04a135a1519ca6d489ed) )

	PACMAN_PROMS
ROM_END

ROM_START( msf1pac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msf1pac.4",    0x3000, 0x1000, CRC(e255428b) SHA1(fd128743eac8521f1a65bdbb2cfd59398eddfac5) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msindy.5e",    0x0000, 0x1000, CRC(c7183fd2) SHA1(af7e3bb7c4751f5df12bd321d6f583045c5acb10) )
	ROM_LOAD( "msindy.5f",    0x1000, 0x1000, CRC(a355e061) SHA1(b9223e01a77027d2f6afe4502b4a2dce3b3bdc0e) )

	PACMAN_PROMS
ROM_END

ROM_START( msgrkatk )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msgrkb.4",     0x3000, 0x1000, CRC(7143729f) SHA1(87152df222364908852b1d242dff15889da5e95d) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msgrkb.5e",    0x0000, 0x1000, CRC(b6526896) SHA1(904dc168680cec660bee5a1a28a6a4b0dee9364e) )
	ROM_LOAD( "msgrkb.5f",    0x1000, 0x1000, CRC(043b1c9b) SHA1(6aa7971390272c3df4ceaf7718ad08086bb33b5c) )

	PACMAN_PROMS
ROM_END

ROM_START( msgrkb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msgrkb.4",     0x3000, 0x1000, CRC(7143729f) SHA1(87152df222364908852b1d242dff15889da5e95d) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msgrkb.5e",    0x0000, 0x1000, CRC(b6526896) SHA1(904dc168680cec660bee5a1a28a6a4b0dee9364e) )
	ROM_LOAD( "msgrkb.5f",    0x1000, 0x1000, CRC(043b1c9b) SHA1(6aa7971390272c3df4ceaf7718ad08086bb33b5c) )

	PACMAN_PROMS
ROM_END

ROM_START( mshangly )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mshangly.5",   0x8000, 0x1000, CRC(00c4d538) SHA1(cbfc9c10809631aae3fe9c454661ffa84a34138c) )
	ROM_LOAD( "mshangly.6",   0x9000, 0x1000, CRC(7a600536) SHA1(7172b4658c1b0ea4cdcc55b7f21225d82e116e5e) )
	/* The dot counts are wrong, they are at 8b2c, 8e17, 9109, 93f9 for each map 1-4 */
	ROM_FILL ( 0x8b2c, 1, 0xdc )	/* Change dot count for map 1 from E0 to DC */
	ROM_FILL ( 0x8e17, 1, 0xf2 )	/* Change dot count for map 2 from F4 to F2 */
	ROM_FILL ( 0x93f9, 1, 0xec )	/* Map 3 (=F2) is ok, change dot count for map 4 from EE to EC */

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( msheartb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msheart.5e",   0x0000, 0x1000, CRC(56f505f1) SHA1(5628874f332d348a61c1abcf116278ece534a7ba) )
	ROM_LOAD( "msheart.5f",   0x1000, 0x1000, CRC(662423ca) SHA1(e22a20e2ed51b4efe39665222669d86353d6c4c5) )

	PACMAN_PROMS
ROM_END

ROM_START( mshearts )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mshearts.5e",  0x0000, 0x1000, CRC(0b5cf081) SHA1(9333369dd282f725c01831314f2aafd1667c64f9) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( msindy )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msindy.4",     0x3000, 0x1000, CRC(8e234b3a) SHA1(f54f94aa80486a96178765378123a16a2637a47b) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msindy.5e",    0x0000, 0x1000, CRC(c7183fd2) SHA1(af7e3bb7c4751f5df12bd321d6f583045c5acb10) )
	ROM_LOAD( "msindy.5f",    0x1000, 0x1000, CRC(a355e061) SHA1(b9223e01a77027d2f6afe4502b4a2dce3b3bdc0e) )

	PACMAN_PROMS
ROM_END

ROM_START( msmini )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msmini.5e",    0x0000, 0x1000, CRC(7787a3f6) SHA1(8d89be6fcc1dfeb4470a721c9c89225ea8c7cfad) )
	ROM_LOAD( "msmini.5f",    0x1000, 0x1000, CRC(98ab3c44) SHA1(07dcfb7d6f2ca50e6e50ee1c14c229d26bb4b909) )

	PACMAN_PROMS
ROM_END

ROM_START( msminia )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msmini.5e",    0x0000, 0x1000, CRC(7787a3f6) SHA1(8d89be6fcc1dfeb4470a721c9c89225ea8c7cfad) )
	ROM_LOAD( "msmini.5f",    0x1000, 0x1000, CRC(98ab3c44) SHA1(07dcfb7d6f2ca50e6e50ee1c14c229d26bb4b909) )

	PACMAN_PROMS
ROM_END

ROM_START( msmspac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msmspac.4",    0x3000, 0x1000, CRC(1c11c60f) SHA1(d445a333f79ad8a6886e3b08fc39dee01733cf07) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "msmspac.5f",   0x1000, 0x1000, CRC(a0ffcd08) SHA1(0cd5028e97fafdd8b9b221a7a07bdc81826f4765) )

	PACMAN_PROMS
ROM_END

ROM_START( msnes4a )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "msnes4a.5",    0x8000, 0x1000, CRC(d16b9605) SHA1(b54691591794fd8d9c9097da49add34f49021b50) )
	ROM_LOAD( "msnes4a.6",    0x9000, 0x1000, CRC(e6b58707) SHA1(cf0c5a3fbb9ae9a99a9c5c74ba45ac86323a0396) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msnes4a.5e",   0x0000, 0x1000, CRC(22e5b64e) SHA1(56e81329bee499a798a01867dbf2ceae6af2e11a) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( msnes62 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "msnes62.5",    0x8000, 0x1000, CRC(52120bc9) SHA1(dc3608672e614a486306660bd367b325cdb51bda) )
	ROM_LOAD( "msnes62.6",    0x9000, 0x1000, CRC(8fd36e02) SHA1(6d0b04b7efd8a4a6773d60e5e748243cf1c56a1a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacnes.5e",  0x0000, 0x1000, CRC(d3a28769) SHA1(3fa7d9a235e0bf8aae03b15c5377401f029d6946) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( msnes63 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "msnes63.5",    0x8000, 0x1000, CRC(f4ea7d2c) SHA1(c490720d3b76615f46cae1f9bfbfc74da6f2854a) )
	ROM_LOAD( "msnes63.6",    0x9000, 0x1000, CRC(1ea43bba) SHA1(0c52d0d2ca866b750a03587c8c51c14f67327d43) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacnes.5e",  0x0000, 0x1000, CRC(d3a28769) SHA1(3fa7d9a235e0bf8aae03b15c5377401f029d6946) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( msnes6m )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "msnes6m.5",    0x8000, 0x1000, CRC(bc091971) SHA1(d5046f5dcffd027805a53be937e7389a135e980f) )
	ROM_LOAD( "msnes6m.6",    0x9000, 0x1000, CRC(23ebe9a1) SHA1(a51b6bd74edbea8e34ef35602c4aa8723d7f745a) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msnes6m.5e",   0x0000, 0x1000, CRC(3abb116b) SHA1(75abb47c9acfc4048d18265ff2a42db2d3591a48) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( msnes6m2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "msstrmaz.5",   0x8000, 0x1000, CRC(b8f8edda) SHA1(5af8c7908df0d45d456cfe68470ba2098c956cac) )
	ROM_LOAD( "msstrmaz.6",   0x9000, 0x1000, CRC(5d7888bc) SHA1(1f3ecd3bc787dd64ed41b1d664cf6794fa799ee8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msnes6m.5e",   0x0000, 0x1000, CRC(3abb116b) SHA1(75abb47c9acfc4048d18265ff2a42db2d3591a48) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( msnes6m3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "msnes6m3.5",   0x8000, 0x1000, CRC(20833779) SHA1(07826e21a14c15590cba7b9a08d2b91bf247cfe7) )
	ROM_LOAD( "msnes6m3.6",   0x9000, 0x1000, CRC(a047e68f) SHA1(8b99f471f39c6c383f84754996b644fe067527f7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msnes6m.5e",   0x0000, 0x1000, CRC(3abb116b) SHA1(75abb47c9acfc4048d18265ff2a42db2d3591a48) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( msnes6m4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "msnes6m4.5",   0x8000, 0x1000, CRC(23c0db46) SHA1(dea5a3eeb1fb480f275cc2729b7ad604aa0824d3) )
	ROM_LOAD( "msnes6m4.6",   0x9000, 0x1000, CRC(daafdb39) SHA1(4120bdd859f37b065279cf048937cd1a5ce526da) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msnes4a.5e",   0x0000, 0x1000, CRC(22e5b64e) SHA1(56e81329bee499a798a01867dbf2ceae6af2e11a) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( mspac1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mspac1.cpu",   0x0000, 0x4000, CRC(ba5953c8) SHA1(ee5a0b9594f88c60be93e13f70adf45c5939f4da) )
	ROM_CONTINUE( 		  0x8000, 0x2000 )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspac6m )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "mspac6m.5",    0x8000, 0x1000, CRC(d0bd89d9) SHA1(c47e96d68d7e88e906b416c24ce926b76689f22d) )
	ROM_LOAD( "mspac6m.6",    0x9000, 0x1000, CRC(c5662407) SHA1(98827bda653b6d1d6984082c706247392ab706d2) )

	ROM_REGION( 0x2000, "gfx1" , 0 )	
	ROM_LOAD( "mspac6m.5e",   0x0000, 0x0800, CRC(2850148a) SHA1(4257a5dd4ad27d4a6bfae71bbe2564026bdaebb2) )
	ROM_LOAD( "mspac6m.5h",   0x0800, 0x0800, CRC(6c3c6ebb) SHA1(3f19e9a20e202fda139324ef03991af99516aeef) )
	ROM_LOAD( "mspac6m.5f",   0x1000, 0x0800, CRC(5596b345) SHA1(0320eb1f7e39dbd438be2f488f43f5b11e13f438) )
	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	PACPLUS_PROMS
ROM_END

ROM_START( mspac6m2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "mspac6m2.5",   0x8000, 0x1000, CRC(48453707) SHA1(3faf536de2e56770a99379849409eeea01b2d031) )
	ROM_LOAD( "mspac6m2.6",   0x9000, 0x1000, CRC(2e961bc4) SHA1(e29814151edde8a22e638a4a8edb9935e48d985b) )

	ROM_REGION( 0x2000, "gfx1" , 0 )	
	ROM_LOAD( "mspac6m.5e",   0x0000, 0x0800, CRC(2850148a) SHA1(4257a5dd4ad27d4a6bfae71bbe2564026bdaebb2) )
	ROM_LOAD( "mspac6m.5h",   0x0800, 0x0800, CRC(6c3c6ebb) SHA1(3f19e9a20e202fda139324ef03991af99516aeef) )
	ROM_LOAD( "mspac6m.5f",   0x1000, 0x0800, CRC(5596b345) SHA1(0320eb1f7e39dbd438be2f488f43f5b11e13f438) )
	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	PACPLUS_PROMS
ROM_END

ROM_START( mspacad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacad.5e",   0x0000, 0x1000, CRC(c3dca510) SHA1(6e9c3e47f2f928ef3857dce8bc26352a80d4a52d) )
	ROM_LOAD( "msvectr.5f",   0x1000, 0x1000, CRC(57cb31e3) SHA1(dc9278cee7f7ce341087760654bf0a1bcdeb2f7c) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacat2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",   	  0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",   	  0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacat2.5",   0x8000, 0x1000, CRC(5a72a7a6) SHA1(aa54bf865e024fe00a8aefce6bc5086e90d71a54) )
	ROM_LOAD( "mspacat2.6",   0x9000, 0x1000, CRC(bd0c3e94) SHA1(1b01ed6eab2c31bb874d1e51e773d0c130de7b75) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacatb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacat2.5",   0x8000, 0x1000, CRC(5a72a7a6) SHA1(aa54bf865e024fe00a8aefce6bc5086e90d71a54) )
	ROM_LOAD( "mspacatb.6b",  0x9000, 0x1000, CRC(7a8c4dfe) SHA1(b450a3272255527838b806dc0318256e47593018) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacatx )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6pacatk",     0x9000, 0x1000, CRC(f6d83f4d) SHA1(6135b187d6b968554d08f2ac00d3a3313efb8638) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mspacmnx.5e",  0x0000, 0x4000, CRC(d9a2897d) SHA1(38cc83d1a854ae289da7904faff32114a9731bda) )
	ROM_LOAD( "mspacmnx.5f",  0x4000, 0x4000, CRC(6f625a9e) SHA1(d9d739ba412c9374b26006d99a345212ef97f868) )

	PACMAN_PROMS
ROM_END

/* This dump had a bad rom at position 5a, i have re-created it from one half of boot5. This seems reasonable
   since the rom at position 5b is identical to the other half of boot5.
   More: checking against E180 shows this is correct, and also supplies the missing 6b rom. */
ROM_START( mspacde )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacde.5a",   0x8000, 0x0800, CRC(cb8a001c) SHA1(398992352c51f82049a4ac981fd152c4c2863f2d) )
	ROM_LOAD( "mspacde.5b",   0x8800, 0x0800, CRC(3e866689) SHA1(fbcb8eaddf15fd988de2cb35f93003380ee6e2de) )
	ROM_LOAD( "mspacde.6a",   0x9000, 0x0800, CRC(286041cf) SHA1(5a5fc97ea66a59895b3403b2982940b755076667) )
	ROM_LOAD( "mspacde.6b",   0x9800, 0x0800, CRC(321784cf) SHA1(9b67741e035fe1528e971db9072eea8982235a63) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacit.5e",   0x0000, 0x0800, CRC(93933d1d) SHA1(fa38d2cb87e872bb9a3158a4df98f38360dc85ec) )
	ROM_LOAD( "mspacit.5h",   0x0800, 0x0800, CRC(7409fbec) SHA1(f440f08ba026ae6172666e1bdc0894ce33bba420) )
 	ROM_LOAD( "mspacit.5f",	  0x1000, 0x0800, CRC(22b0188a) SHA1(a9ed9ca8b36a60081fd364abc9bc23963932cc0b) )
 	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacdel )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacdel.4",   0x3000, 0x1000, CRC(af8c63ad) SHA1(e119eac748179fef53b8fd62f33885df53bf94fa) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacdel.5e",  0x0000, 0x1000, CRC(605becd7) SHA1(d16bd3e151273ab6652b4f105dd58ef95c18b387) )
	ROM_LOAD( "mspacdel.5f",  0x1000, 0x1000, CRC(435de4cb) SHA1(f3aa39e969f79b0e01378c870e874f8076def746) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacfnt )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacfnt.5e",  0x0000, 0x1000, CRC(6d6d7af4) SHA1(768dc3fa909f6fef77eb3d72c287c91f3da2ae70) )
	ROM_LOAD( "mspacfnt.5f",  0x1000, 0x1000, CRC(7537ee90) SHA1(9e861fca1706ecabaad15ba3fc817bdc3146ea1d) )

	PACMAN_PROMS
ROM_END

ROM_START( mspachi )
	ROM_REGION( 0x11000, "maincpu", 0 )
	ROM_LOAD( "mspachi.1",    0x0000, 0x1000, CRC(dca2ffc4) SHA1(746ebc8afd3172323f1c19e97e55e0f906d68ea4) )
	ROM_LOAD( "mspachi.2",    0x1000, 0x1000, CRC(fea2627b) SHA1(bc7249f0939dc003de509721e7d4f669defefcfe) )
	ROM_LOAD( "mspachi.3",    0x2000, 0x1000, CRC(a9a32749) SHA1(2d1036a0903a12f4f8b9e56c1599d1dccea8991a) )
	ROM_LOAD( "mspachi.4",    0x3000, 0x1000, CRC(9123cd53) SHA1(e6a3461c818b2bc779f02c23d59e54530ef76584) )
	ROM_LOAD( "mspachi.5",    0x8000, 0x1000, CRC(397f196d) SHA1(aab641b8053adc0a43ee5d2eba43aa6c12326dec) )
	ROM_LOAD( "mspachi.6",    0x9000, 0x2000, CRC(50de42d3) SHA1(66b2dd376ab69cab2593357489251f06bc621120) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacii )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mspacii.1",    0x0000, 0x1000, CRC(df673b57) SHA1(93ee4e8f9751db5f7d3c35988fcb8ae8037464ed) )
	ROM_LOAD( "mspacii.2",    0x1000, 0x1000, CRC(7591f606) SHA1(0aead0ac5af602269df8732c5763147cdb543b8d) )
	ROM_LOAD( "mspacii.3",    0x2000, 0x1000, CRC(c8ef1a7f) SHA1(0c9a28bbe63d7d44511a13316937a21b8846543e) )
	ROM_LOAD( "mspacii.4",	  0x3000, 0x1000, CRC(d498f435) SHA1(c041841b1349d8d5c2d560aed54c4aeb8adfddb8) )
	ROM_LOAD( "mspacii.5",    0x8000, 0x1000, CRC(fbbc3d2e) SHA1(dc4337d7f0961e048a433021f670da6d314bd663) )
	ROM_LOAD( "mspacii.6",    0x9000, 0x1000, CRC(aa3887c5) SHA1(0e20cb686383156a9883749568c8e57c15c3ae44) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacii.5e",   0x0000, 0x1000, CRC(04333722) SHA1(bb179d5302b26b815b5d7eff14865e7b4f8a6880) )
 	ROM_LOAD( "5f",		  0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacit )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacit.4",    0x3000, 0x1000, CRC(50dcba32) SHA1(cc454829cd71ca874029a20118027873df824c66) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "mspacit.6",    0x9000, 0x1000, CRC(3a1d5323) SHA1(16e7dddf40505706ece2d053d51e28bff8be8e77) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacit.5e",   0x0000, 0x0800, CRC(93933d1d) SHA1(fa38d2cb87e872bb9a3158a4df98f38360dc85ec) )
	ROM_LOAD( "mspacit.5h",   0x0800, 0x0800, CRC(7409fbec) SHA1(f440f08ba026ae6172666e1bdc0894ce33bba420) )
 	ROM_LOAD( "mspacit.5f",	  0x1000, 0x0800, CRC(22b0188a) SHA1(a9ed9ca8b36a60081fd364abc9bc23963932cc0b) )
 	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "mspacit.7f",   0x0000, 0x0020, CRC(2c3cc909) SHA1(32d68d4cfdf9f3e7351353428d268c763e809c63) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )
	/* The sound is extremely distorted, perhaps a bad dump? The patches below fix the changed bytes */
	ROM_LOAD( "mspacit.1m",   0x0000, 0x0100, CRC(3cb61034) SHA1(2f24b88839aee107a0ac1064f8bc4853933f5205) )
	ROM_FILL( 0x0a, 1, 13 )
	ROM_FILL( 0x2a, 1, 10 )
	ROM_FILL( 0x4a, 1, 2 )
	ROM_FILL( 0x8a, 1, 13 )
	ROM_FILL( 0xaa, 1, 2 )
	ROM_FILL( 0xca, 1, 10 )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	
ROM_END

/* This was listed as a pac-gal clone however it is just another MS Pacman (mspacit with slightly altered gfx) */
ROM_START( mspacit2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacit.4",    0x3000, 0x1000, CRC(50dcba32) SHA1(cc454829cd71ca874029a20118027873df824c66) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "mspacit.6",    0x9000, 0x1000, CRC(3a1d5323) SHA1(16e7dddf40505706ece2d053d51e28bff8be8e77) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacit.5e",   0x0000, 0x0800, CRC(93933d1d) SHA1(fa38d2cb87e872bb9a3158a4df98f38360dc85ec) )
	ROM_LOAD( "mspacit.5h",   0x0800, 0x0800, CRC(7409fbec) SHA1(f440f08ba026ae6172666e1bdc0894ce33bba420) )
 	ROM_LOAD( "mspacit2.5f",  0x1000, 0x0800, CRC(20135b9b) SHA1(ea85134cdeb4142d72cf81ead96990d211046f2e) )
 	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	PACMAN_PROMS
ROM_END

/* The cpu rom is 2 copies of mspacmbg.cpu joined */
ROM_START( mspacj )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mspacj.cpu",   0x0000, 0x4000, CRC(807f978a) SHA1(19fa46e41ff671b88fb7d5e0a6c8c989a96e3477) )
	ROM_CONTINUE(		  0x8000, 0x4000)
	ROM_IGNORE(		  0x8000 )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacj.gfx",   0x0000, 0x0800, CRC(8ee4a3b0) SHA1(01e3453c99f7a5d78ab083c49c650e898c0dd2ee) )
	ROM_CONTINUE(             0x1000, 0x0800 )
	ROM_CONTINUE(             0x0800, 0x0800 )
	ROM_CONTINUE(             0x1800, 0x0800 )
	ROM_IGNORE(                       0x2000 )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "mspacj.7f",    0x0000, 0x0020, CRC(3545e7e9) SHA1(b866b02579438afb11296e5c53a32c6425bd044d) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	ROM_REGION( 0x0200, "namco", 0 )	
	ROM_LOAD( "mspacj.1m",    0x0000, 0x0100, CRC(640e455c) SHA1(c3f7d132cf8f1ec3334366b6891fca74450a577e) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( mspacmab )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacman )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6",           0x9000, 0x1000, CRC(a90e7000) SHA1(e4df96f1db753533f7d770aa62ae1973349ea4cf) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

/* This set has the fixes proposed by Don Hodges at the pages:
	http://donhodges.com/how_high_can_you_get3.htm
	http://donhodges.com/ms_pacman_bugs.htm */
ROM_START( mspacmap )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )
	ROM_FILL(0x0677,1,0xc3)
	ROM_FILL(0x0678,1,0x41)
	ROM_FILL(0x0679,1,0x98)
	ROM_FILL(0x0c1d,1,0x00)
	ROM_FILL(0x2bf3,1,0xc3)
	ROM_FILL(0x2bf4,1,0x93)
	ROM_FILL(0x2bf5,1,0x87)
	ROM_FILL(0x2bf8,1,0x3c)
	ROM_FILL(0x8794,1,0x07)
	ROM_FILL(0x8796,1,0xf8)
	ROM_FILL(0x2ffc,1,0xfa)
	ROM_FILL(0x2ffd,1,0x5b)
	ROM_FILL(0x3f7c,1,0x85)
	ROM_FILL(0x94c3,1,0xd2)
	ROM_FILL(0x94d8,1,0xd2)
	ROM_FILL(0x9595,1,0xd2)
	ROM_FILL(0x95a7,1,0xd2)
	ROM_FILL(0x95c8,1,0xd2)
	ROM_FILL(0x9841,1,0xaf)
	ROM_FILL(0x9842,1,0x32)
	ROM_FILL(0x9843,1,0x02)
	ROM_FILL(0x9844,1,0x4e)
	ROM_FILL(0x9845,1,0xef)
	ROM_FILL(0x9846,1,0x01)
	ROM_FILL(0x9847,1,0x01)
	ROM_FILL(0x9848,1,0xc3)
	ROM_FILL(0x9849,1,0x7a)
	ROM_FILL(0x984a,1,0x06)

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmat )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6pacatk",     0x9000, 0x1000, CRC(f6d83f4d) SHA1(6135b187d6b968554d08f2ac00d3a3313efb8638) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmbe )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "2.bin",        0x1000, 0x1000, CRC(04e6c486) SHA1(63aa3e6c49d345cccfe87dd3fdcddc75ab4a570d) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "6.bin",        0x9000, 0x1000, CRC(206a9623) SHA1(20006f945c1b7b0e3c0415eecc0b148e5a6a1dfa) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmbg )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mspacmbg.cpu", 0x0000, 0x4000, CRC(97c64918) SHA1(a46ca4822e6cd7b9a5603d5d06a78fd489dc0b96) )
	ROM_CONTINUE(             0x8000, 0x4000 )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacmbg.gfx", 0x0000, 0x0800, CRC(f2c5da43) SHA1(6a6de2ecc313a11ad12d8d1712c05f923984f668) )
	ROM_CONTINUE(             0x1000, 0x0800 )
	ROM_CONTINUE(             0x0800, 0x0800 )
	ROM_CONTINUE(             0x1800, 0x0800 )
	ROM_IGNORE(               0x6000 )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmbx )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mspacmnx.5e",  0x0000, 0x4000, CRC(d9a2897d) SHA1(38cc83d1a854ae289da7904faff32114a9731bda) )
	ROM_LOAD( "mspacmnx.5f",  0x4000, 0x4000, CRC(6f625a9e) SHA1(d9d739ba412c9374b26006d99a345212ef97f868) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmfx )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacmanf.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6",           0x9000, 0x1000, CRC(a90e7000) SHA1(e4df96f1db753533f7d770aa62ae1973349ea4cf) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mspacmnx.5e",  0x0000, 0x4000, CRC(d9a2897d) SHA1(38cc83d1a854ae289da7904faff32114a9731bda) )
	ROM_LOAD( "mspacmnx.5f",  0x4000, 0x4000, CRC(6f625a9e) SHA1(d9d739ba412c9374b26006d99a345212ef97f868) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmnf )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacfast.6f",   0x1000, 0x1000, CRC(720dc3ee) SHA1(7224d7acfa0144b681c71d7734a7337189835361) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6",           0x9000, 0x1000, CRC(a90e7000) SHA1(e4df96f1db753533f7d770aa62ae1973349ea4cf) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmnn )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6",           0x9000, 0x1000, CRC(a90e7000) SHA1(e4df96f1db753533f7d770aa62ae1973349ea4cf) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacmnn.5e",  0x0000, 0x1000, CRC(f4cf1273) SHA1(9120de96984e96a0425b210d9fb8175b91b26284) )
	ROM_LOAD( "mspacmnn.5f",  0x1000, 0x1000, CRC(c4e7ec0b) SHA1(06940f81b228fd7b6389afe93a7355dc2afee821) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacmnx )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5",           0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "u6",           0x9000, 0x1000, CRC(a90e7000) SHA1(e4df96f1db753533f7d770aa62ae1973349ea4cf) )
	ROM_LOAD( "u7",           0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mspacmnx.5e",  0x0000, 0x4000, CRC(d9a2897d) SHA1(38cc83d1a854ae289da7904faff32114a9731bda) )
	ROM_LOAD( "mspacmnx.5f",  0x4000, 0x4000, CRC(6f625a9e) SHA1(d9d739ba412c9374b26006d99a345212ef97f868) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacnes )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacnes.5",   0x8000, 0x1000, CRC(43936999) SHA1(4261f0bbb9ece2370c6283763a396ad851f42c51) )
	ROM_LOAD( "mspacnes.6",   0x9000, 0x1000, CRC(a15b524e) SHA1(91f211e479261b344fc64517394abf7a8f9d6f4b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacnes.5e",  0x0000, 0x1000, CRC(d3a28769) SHA1(3fa7d9a235e0bf8aae03b15c5377401f029d6946) )
	ROM_LOAD( "mspacnes.5f",  0x1000, 0x1000, CRC(6a4db98d) SHA1(e0eeb83c1265f5c9881e1c752842d20dabf35783) )

	PACPLUS_PROMS
ROM_END

ROM_START( mspacp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacp.4",     0x3000, 0x1000, CRC(203dcba5) SHA1(c1e94a17bab5f25bfdc5836e0ba968a56b6f9e58) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacp.5e",    0x0000, 0x1000, CRC(cd4afa90) SHA1(e917c5cd4b0af33279dbde5a50fc3aedb7602425) )
	ROM_LOAD( "mspacp.5f",    0x1000, 0x1000, CRC(58ea207f) SHA1(65d07838bd2cb8bfcb6fcb0e99f88036af5753be) )

	PACPLUS_PROMS
ROM_END

ROM_START( mspacpbl )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "mspacpbl.1",   0x0000, 0x1000, CRC(34f60afa) SHA1(86813f3f0c3832451ee215d542cce7ad2aaa9785) )
	ROM_LOAD( "mspacpbl.2",   0x1000, 0x1000, CRC(5426636c) SHA1(35cdd13e917c2d1e11086745de6dc3b85bf1fdbb) )
	ROM_LOAD( "mspacpbl.3",   0x2000, 0x1000, CRC(60c890a1) SHA1(c1fbaedb212449803f5b506ac6f8e258644e4536) )
	ROM_LOAD( "mspacpbl.4",   0x3000, 0x1000, CRC(586f1bfd) SHA1(1cf77bb44ba153ca03e7e8b26174b33f192da071) )
	ROM_LOAD( "mspacpbl.5",   0x8000, 0x1000, CRC(85a44e94) SHA1(59b2249f6834ee4634e9fb8daa53739e80654c85) )
	ROM_LOAD( "mspacpbl.6",   0x9000, 0x1000, CRC(3babaeab) SHA1(abc7f705a16dfd53540dfe7e0b6723a5843682c2) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacpbl.5e",  0x0000, 0x1000, CRC(5ba9c39a) SHA1(50e49234b051d74ce21ec4b819cccbbb98c57c3c) )
	ROM_LOAD( "mspacpbl.5f",  0x1000, 0x1000, CRC(4aeefccb) SHA1(68810190397415be042c4787618feade7dcfe23a) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacpls )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacplx )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mspacmnx.5e",  0x0000, 0x4000, CRC(d9a2897d) SHA1(38cc83d1a854ae289da7904faff32114a9731bda) )
	ROM_LOAD( "mspacmnx.5f",  0x4000, 0x4000, CRC(6f625a9e) SHA1(d9d739ba412c9374b26006d99a345212ef97f868) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacren )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacren.4",   0x3000, 0x1000, CRC(efe80423) SHA1(bc78085a53123da0e0c9ab2c212c60c9dd2e6983) )
	ROM_LOAD( "mspacren.5",   0x8000, 0x1000, CRC(f513ae17) SHA1(87e1a3d31d04ed527cb89369c53a5a66403f73a5) )
	ROM_LOAD( "mspacren.6",   0x9000, 0x1000, CRC(ce3b842b) SHA1(e4bf3578ccfaa757534f83fb3842d1515fd95d71) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacren.5e",  0x0000, 0x1000, CRC(31b134a0) SHA1(f457df057963aa60492a98f12a6d258fe7fded03) )
	ROM_LOAD( "mspacren.5f",  0x1000, 0x1000, CRC(022f61d1) SHA1(16ed25a881b058efa963b938341ce4f04d914262) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "puckren.7f",   0x0000, 0x0020, CRC(7b6aa3c9) SHA1(849911abdc26ccf2ad44e2c432e6219447b8bef3) )
	ROM_LOAD( "puckren.4a",   0x0020, 0x0100, CRC(64fe1dbf) SHA1(7f7541eb12148add79ad186a1da375c865d251e0) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( mspacrip )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacrip.4",   0x3000, 0x1000, CRC(5fb1ee61) SHA1(1123ec7d3de24fd8ae2eb75c87b3d22f53421e21) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacrip.5e",  0x0000, 0x1000, CRC(5aa9eb83) SHA1(da2500ed81873c2d96501d136c7d9478155e0d33) )
	ROM_LOAD( "mspacrip.5f",  0x1000, 0x1000, CRC(ceb3785d) SHA1(c3cc1475ca0b1725497cd03ab116c18d57dba127) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacsup )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacsup.6f",  0x1000, 0x0800, CRC(ab07b7c6) SHA1(855ce1a948660014796de15a8ef155a2b7068056) )
	ROM_LOAD( "mspacsup.6m",  0x1800, 0x0800, CRC(2a724972) SHA1(f67f4280f928f182330b32853591eec32a22d5f6) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacsup.6j",  0x3000, 0x0800, CRC(6023a7a8) SHA1(3424b057dc70c4a76f55dc1b618f5a0ad5d5b6fe) )
	ROM_LOAD( "mspacsup.6p",  0x3800, 0x0800, CRC(9bdbccb0) SHA1(58dedf146178f08dc0e8c09aa0def75267e2efe4) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mspacwf ) // Ms.Pacman Weird Fruit
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspacwf.4",    0x3000, 0x1000, CRC(a078477e) SHA1(e4016bfcd581cbf0b180d7e3cf6b63713de45add) )
	ROM_LOAD( "mspacwf.5",    0x8000, 0x1000, CRC(5064e97a) SHA1(3cc0f7ce7874c28bf4d379922e9509fca6227c13) )
	ROM_LOAD( "mspacwf.6",    0x9000, 0x1000, CRC(b9f6fc01) SHA1(43648fc301e0b2639ac98452154fd74ccec1f6d7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacwf.5e",   0x0000, 0x1000, CRC(4f00a074) SHA1(c9f0319537d327a46612d9255cfeea58c4f96918) )
	ROM_LOAD( "mspacwf.5f",   0x1000, 0x1000, CRC(a13df02c) SHA1(072f5a3fe25cc29d57082eb0cfc7b78f5e330363) )

	PACMAN_PROMS
ROM_END

ROM_START( mspamsba )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "mspamsba.4",   0x3000, 0x1000, CRC(8b3f8e44) SHA1(d56e640e3454950c4f2c4d00c8f6e9e1cfb33494) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspamsba.5e",  0x0000, 0x1000, CRC(aa7115b8) SHA1(895f85d157239be4a6f40beeb739157954697386) )
	ROM_LOAD( "mspamsba.5f",  0x1000, 0x1000, CRC(f94b0961) SHA1(8b1376dd50388cbc29426cc2e339798d687fb845) )

	PACMAN_PROMS
ROM_END

ROM_START( mspc6mad )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "mspc6mad.5",   0x8000, 0x1000, CRC(b556fe78) SHA1(2e84f6e69b3852b791de22ac31bee2b2f76ded07) )
	ROM_LOAD( "mspc6mad.6",   0x9000, 0x1000, CRC(4dbe1939) SHA1(5203274ed1ceb06308f2778b7b5e8d41e8f2bd63) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspc6mad.5e",  0x0000, 0x1000, CRC(92ea9860) SHA1(1d03cd9dd411ed9284fe4ccae3cbe2ac53560139) )
	ROM_LOAD( "mspc6mad.5f",  0x1000, 0x1000, CRC(713cc17e) SHA1(dca7722b38c4eae701f48110276376c83505fc9b) )

	PACMAN_PROMS
ROM_END

ROM_START( msplus )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msplus.4",     0x3000, 0x1000, CRC(9d6fed06) SHA1(9927933f8df589e995f542d48a6ac196029a343f) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacp.5e",    0x0000, 0x1000, CRC(cd4afa90) SHA1(e917c5cd4b0af33279dbde5a50fc3aedb7602425) )
	ROM_LOAD( "msplus.5f",    0x1000, 0x1000, CRC(e68b2a85) SHA1(8c1b294505ae5202bce760cafea2437a5f71d0cf) )

	PACPLUS_PROMS
ROM_END

ROM_START( msrumble )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msrumble.4",   0x3000, 0x1000, CRC(15b02313) SHA1(b1400928ff7a5c3067f3d7be02b172f3054c0ce0) )
	ROM_LOAD( "msrumble.5",   0x8000, 0x1000, CRC(e2473a86) SHA1(b51cb0a98178f4c52345d6ecdf89e5f23080eb72) )
	ROM_LOAD( "msrumble.6",   0x9000, 0x1000, CRC(17abb097) SHA1(7eae139e71c7ea07e05f9b1c04194b1ae6c2f975) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( mssilad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mssilad.3",    0x2000, 0x1000, CRC(79d241dd) SHA1(98f95619d4212da9f23b83ad5c9c793576d51177) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mssilad.5",    0x8000, 0x1000, CRC(2e2fcde0) SHA1(718dcbf395eb92b956adf185b5b2cf46aa5b9694) )
	ROM_LOAD( "mssilad.6",    0x9000, 0x1000, CRC(22c6d19a) SHA1(92cb3f962b7dab5e92b780d60f448b9f57c03687) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msatkad.5e",   0x0000, 0x1000, CRC(dc70ed2e) SHA1(b9fbb1d21085bc78eb82c429506b20e332b18e0e) )
	ROM_LOAD( "msatkad.5f",   0x1000, 0x1000, CRC(643d5523) SHA1(565f92294a4b6a11dc5e9a165f35284c2a526e6c) )

	PACMAN_PROMS
ROM_END

ROM_START( msstrmaz )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "msstrmaz.4",   0x3000, 0x1000, CRC(ef9cce5a) SHA1(2a9bcdd672023129761a573d77f69b76ccfdc5c4) )
	ROM_LOAD( "msstrmaz.5",   0x8000, 0x1000, CRC(b8f8edda) SHA1(5af8c7908df0d45d456cfe68470ba2098c956cac) )
	ROM_LOAD( "msstrmaz.6",   0x9000, 0x1000, CRC(5d7888bc) SHA1(1f3ecd3bc787dd64ed41b1d664cf6794fa799ee8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msstrmaz.5e",  0x0000, 0x1000, CRC(14fd0ad7) SHA1(4fac020002905b28c7f054340aa0eacc7b2c701b) )
	ROM_LOAD( "msstrmaz.5f",  0x1000, 0x1000, CRC(67fb6254) SHA1(ce2134f6c8c6fc5770a835aa1eeebd27611806da) )

	PACPLUS_PROMS
ROM_END

ROM_START( msultra )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "msultra.4",    0x3000, 0x1000, CRC(36b5e9fd) SHA1(70373fbcae8d2f6dd9274cb825a561de15c4c2b9) )
	ROM_LOAD( "msultra.5",    0x8000, 0x1000, CRC(cf609615) SHA1(f410eec29433085bf9138582e6725c9326d9001f) )
	ROM_LOAD( "msultra.6",    0x9000, 0x1000, CRC(007cd365) SHA1(c5ce3f81063052b36a6f5516d391e6c5a63f2819) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msultra.5e",   0x0000, 0x1000, CRC(508af5b0) SHA1(5a92354f45af65d3343e32111f675998a5347557) )
	ROM_LOAD( "msultra.5f",   0x1000, 0x1000, CRC(db5a4630) SHA1(2cb6e8650ae861a2008dd2f185aa2c02827ab370) )

	PACMAN_PROMS
ROM_END

ROM_START( msvctr6m )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "mspac6m.4",    0x3000, 0x1000, CRC(e21c81ff) SHA1(5a6ce8ee3059a31afbb9b8c1d0a1e37606d14a0a) )
	ROM_LOAD( "mspac6m.5",    0x8000, 0x1000, CRC(d0bd89d9) SHA1(c47e96d68d7e88e906b416c24ce926b76689f22d) )
	ROM_LOAD( "mspac6m.6",    0x9000, 0x1000, CRC(c5662407) SHA1(98827bda653b6d1d6984082c706247392ab706d2) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msvctr6m.5e",  0x0000, 0x1000, CRC(64b35a5a) SHA1(5c8dec1ae192c8e717346618bc4f28d3abd0ca84) )
	ROM_LOAD( "msvectr.5f",   0x1000, 0x1000, CRC(57cb31e3) SHA1(dc9278cee7f7ce341087760654bf0a1bcdeb2f7c) )

	PACMAN_PROMS
ROM_END

ROM_START( msvectr )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msvectr.5e",   0x0000, 0x1000, CRC(56560aef) SHA1(1eac2b242328ec4d6ba9faac2fd972c8cb6c3e55) )
	ROM_LOAD( "msvectr.5f",   0x1000, 0x1000, CRC(57cb31e3) SHA1(dc9278cee7f7ce341087760654bf0a1bcdeb2f7c) )

	PACMAN_PROMS
ROM_END

ROM_START( msyakman )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msyakman.5e",  0x0000, 0x1000, CRC(827892bb) SHA1(5b111c4d6293e13d5fae9e77823acd8be5c08b34) )
	ROM_LOAD( "msyakman.5f",  0x1000, 0x1000, CRC(1108ba96) SHA1(b1ee699722b8a22e24ec18d1930e055387a45d96) )

	PACMAN_PROMS
ROM_END

ROM_START( ottop1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ottop1.1",     0x0000, 0x1000, CRC(00340fb8) SHA1(60337395c35085c32fc4450a649cf629e4ae32da) )
	ROM_LOAD( "ottop1.2",     0x1000, 0x1000, CRC(f6c05582) SHA1(3803f1a9546ff6293e3fad1984300eb1c2835ff6) )
	ROM_LOAD( "ottop1.3",     0x2000, 0x1000, CRC(08381b15) SHA1(8727ddb990668acd0288ee9ec9de4922a505c7d1) )
	ROM_LOAD( "ottop1.4",     0x3000, 0x1000, CRC(0d20edf6) SHA1(11ec227cb5ee030bfed9aaa0b624fd2c0e47b6f5) )
	ROM_LOAD( "ottop1.5",     0x8000, 0x1000, CRC(3bb6cea7) SHA1(cf629f478f14fb392bc71b6453364b58c3544b0a) )
	ROM_LOAD( "ottop1.6",     0x9000, 0x1000, CRC(1e1357bf) SHA1(4fd6074cc971bfe8d233714ae88ec295840c7d9b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ottop1.5e",    0x0000, 0x1000, CRC(2ba71471) SHA1(69f111cee7c1228636ee38b065b2d5486a18749d) )
	ROM_LOAD( "ottop1.5f",    0x1000, 0x1000, CRC(ad353000) SHA1(8ec0c31dc4e9da5bd8cc8087abb3a9936759b39d) )

	PACMAN_PROMS
ROM_END

ROM_START( ottop2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ottop2.1",     0x0000, 0x1000, CRC(a9c0c5fc) SHA1(4631b6260b88c5dc23503ea420a62f946f5bb978) )
	ROM_LOAD( "ottop1.2",     0x1000, 0x1000, CRC(f6c05582) SHA1(3803f1a9546ff6293e3fad1984300eb1c2835ff6) )
	ROM_LOAD( "ottop1.3",     0x2000, 0x1000, CRC(08381b15) SHA1(8727ddb990668acd0288ee9ec9de4922a505c7d1) )
	ROM_LOAD( "ottop2.4",     0x3000, 0x1000, CRC(c8517a08) SHA1(ab6ed1004e6a66dfce426ad5db0bfc0cd4e6eabc) )
	ROM_LOAD( "ottop2.5",     0x8000, 0x1000, CRC(d8d8ceec) SHA1(b3ea6f35118b04281c568806127fe1a97c22ed42) )
	ROM_LOAD( "ottop2.6",     0x9000, 0x1000, CRC(184f877a) SHA1(473b47566fce1b53d6b885b54562c4ae82ed9ae1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ottop2.5e",    0x0000, 0x1000, CRC(54a743f7) SHA1(9406b856edf3f6a7979926295663cabdf53bb0d4) )
	ROM_LOAD( "ottop1.5f",    0x1000, 0x1000, CRC(ad353000) SHA1(8ec0c31dc4e9da5bd8cc8087abb3a9936759b39d) )

	PACMAN_PROMS
ROM_END

ROM_START( ottop3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ottop3.1",     0x0000, 0x1000, CRC(2df1dc0d) SHA1(93846a2affedda0ab1047b2927198fc2c24c7f72) )
	ROM_LOAD( "ottop1.2",     0x1000, 0x1000, CRC(f6c05582) SHA1(3803f1a9546ff6293e3fad1984300eb1c2835ff6) )
	ROM_LOAD( "ottop1.3",     0x2000, 0x1000, CRC(08381b15) SHA1(8727ddb990668acd0288ee9ec9de4922a505c7d1) )
	ROM_LOAD( "ottop3.4",     0x3000, 0x1000, CRC(0eb1ad52) SHA1(d533d06d3aab14ab7726d449269782d663da1180) )
	ROM_LOAD( "ottop3.5",     0x8000, 0x1000, CRC(3e2e62bb) SHA1(8dcbc2170214e538fcf092c54a9d6994b5157278) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ottop2.5e",    0x0000, 0x1000, CRC(54a743f7) SHA1(9406b856edf3f6a7979926295663cabdf53bb0d4) )
	ROM_LOAD( "ottop1.5f",    0x1000, 0x1000, CRC(ad353000) SHA1(8ec0c31dc4e9da5bd8cc8087abb3a9936759b39d) )

	PACMAN_PROMS
ROM_END

ROM_START( ottopz )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ottopz.1",     0x0000, 0x1000, CRC(35b94585) SHA1(6a8350efbd6fab30b22365c6a2ed7e23f47af0e6) )
	ROM_LOAD( "ottopz.2",     0x1000, 0x1000, CRC(298dbd3d) SHA1(302c82562c66c267dedcc7e7d5e3ab8234b5e20c) )
	ROM_LOAD( "ottopz.3",     0x2000, 0x1000, CRC(6f0c25bd) SHA1(f4cabdb7462310c14ecde5db9f586360ac1829f4) )
	ROM_LOAD( "ottopz.4",     0x3000, 0x1000, CRC(3af7ef8c) SHA1(bf340f3fa275950c0eda4fbf3eb3115edd039bd6) )
	ROM_LOAD( "ottopz.5",     0x8000, 0x1000, CRC(098c2a09) SHA1(cf0e69c891dbc8b448c359abf3e869d0aaba02df) )
	ROM_LOAD( "ottopz.6",     0x9000, 0x1000, CRC(367e5c05) SHA1(15bed84c6480fab5e7bcbbd1dbb2b8919ac1dca5) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ottopz.5e",    0x0000, 0x1000, CRC(046d76fb) SHA1(44f607dbed727a0ae5152da67b6953ba9619566f) )
	ROM_LOAD( "ottopz.5f",    0x1000, 0x1000, CRC(402e194d) SHA1(01867241edd28f25aa7be1d4004bb2c73979d8d0) )

	PACMAN_PROMS
ROM_END

ROM_START( pacatk )
	ROM_REGION( 0x20000, "maincpu", 0 ) /* 64k for code+64k for decrypted code */
	ROM_LOAD( "pacman.6e",    0x0000, 0x1000, CRC(c1e6ab10) SHA1(e87e059c5be45753f7e9f33dff851f16d6751181) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pacman.6h",    0x2000, 0x1000, CRC(bcdd1beb) SHA1(8e47e8c2c4d6117d174cdac150392042d3e0a881) )
	ROM_LOAD( "pacman.6j",    0x3000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )
	ROM_LOAD( "u5", 	  0x8000, 0x0800, CRC(f45fbbcd) SHA1(b26cc1c8ee18e9b1daa97956d2159b954703a0ec) )
	ROM_LOAD( "pacatk.u6",    0x9000, 0x1000, CRC(a35b3788) SHA1(51a1806a0ae4adb5baa9e875c6d9f715198ce8ae) )
	ROM_LOAD( "u7", 	  0xb000, 0x1000, CRC(c82cd714) SHA1(1d8ac7ad03db2dc4c8c18ade466e12032673f874) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e", 	  0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f", 	  0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )
//	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
//	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacgal )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "pacman.7hj",   0x3000, 0x1000, CRC(70694c8e) SHA1(d0d02f0997b44e1ba5ea27fc3f7af1b956e2a687) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "pacman.5ef",   0x1000, 0x0800, CRC(65a3ee71) SHA1(cbbf700eefba2a5bf158983f2ca9688b7c6f5d2b) )
	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "pacgal.4a",    0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( pacgalu )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacgalu.bin",  0x0000, 0x4000, CRC(23a0bd03) SHA1(6ebf7985374a0b0d7402f54c3c9b488fdebeb65e) )
	ROM_CONTINUE(             0x8000, 0x4000 )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacgalu.gfx",  0x0000, 0x2000, CRC(d6e3a240) SHA1(e176bfc6c0d2ecee825dce18758639c6e1dbe70d) )

	PACMAN_PROMS
ROM_END

ROM_START( pacgalx )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "mscrzyma.3",   0x2000, 0x1000, CRC(513f4d5c) SHA1(ae011b89422bd8cbb80389814500bc1427f6ecb2) )
	ROM_LOAD( "pacman.7hj",   0x3000, 0x1000, CRC(70694c8e) SHA1(d0d02f0997b44e1ba5ea27fc3f7af1b956e2a687) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mspacmnx.5e",  0x0000, 0x4000, CRC(d9a2897d) SHA1(38cc83d1a854ae289da7904faff32114a9731bda) )
	ROM_LOAD( "pacgalx.5f",   0x4000, 0x4000, CRC(dfb2a9e9) SHA1(a7c297910fce9723054ce9a7abc0175e75e72d1c) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( pacgent )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacgent.1",    0x0000, 0x1000, CRC(de6f3b2b) SHA1(cf499e8ce3bb88e69ce5bb3bbf412d81bf6a0c0f) )
	ROM_LOAD( "fastplus.2",   0x1000, 0x1000, CRC(a8d6c227) SHA1(b5891f911bb054ca2547e663a3c27c93897f2075) )
	ROM_LOAD( "pacgent.3",    0x2000, 0x1000, CRC(e96d522e) SHA1(41b08f3e10e81fc373bedd5ba928461f05a8ccb4) )
	ROM_LOAD( "pacgent.4",    0x3000, 0x1000, CRC(154482fb) SHA1(98097e37f97310e93a42f7ae03072627c064f372) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "pacgent.6",    0x9000, 0x1000, CRC(dcc58e81) SHA1(5e005b46c5fef89c144f906e695eabad4a50919e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacgent.5e",   0x0000, 0x1000, CRC(e9992415) SHA1(3e8241e1b17d7f5243d348ad9d07f55266ed524e) )
	ROM_LOAD( "pacgent.5f",   0x1000, 0x1000, CRC(50171b3a) SHA1(e1fef48206ba5af1e729bfa09f938cbc47917dc2) )

	PACMAN_PROMS
ROM_END

ROM_START( petshop )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "petshop.4",    0x3000, 0x1000, CRC(75230eef) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "petshop.5e",   0x0000, 0x1000, CRC(ac1c180b) SHA1(dc012d2ba8e1ab19d915e0b1218154f5441aee99) )
	ROM_LOAD( "petshop.5f",   0x1000, 0x1000, CRC(ce314b46) SHA1(bc091cef128f82c7d53ad1b4af841f41906cf601) )

	PACMAN_PROMS
ROM_END

ROM_START( piranha2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "piranha2.4",   0x3000, 0x1000, CRC(fe8b786d) SHA1(e9fcd78076e4e7e6aa73781238722e077e40f945) )
	ROM_LOAD( "piranha2.5",   0x8000, 0x1000, CRC(bef08ea4) SHA1(04211f34d7eb9009b1d2cb1a3fbff0e4bdeb0127) )
	ROM_LOAD( "piranha2.6",   0x9000, 0x1000, CRC(fd3df829) SHA1(69e6c66aab3eb43f0a64350279e67351f1aceddd) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "piranha2.5e",  0x0000, 0x1000, CRC(8f0fa58f) SHA1(bf1ae6523cc9161c0db55b8ecaafe1783f3600f2) )
	ROM_LOAD( "piranha2.5f",  0x1000, 0x1000, CRC(42153903) SHA1(9b231f783b5c847b09819d554a3bb136028938e0) )

	PACMAN_PROMS
ROM_END

ROM_START( ramsnoop )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ramsnoop.1",   0x0000, 0x1000, CRC(A08529FA) SHA1(E448E3A2BED42FA9A2C6435849426E6A2CCF5290) )
	ROM_LOAD( "ramsnoop.2",   0x1000, 0x1000, CRC(27B70DD6) SHA1(E52C724C7909C2B3B8EA44C2D3540BE9D23C5E03) )
	ROM_LOAD( "ramsnoop.3",   0x2000, 0x1000, CRC(812691A0) SHA1(8FF6D1C2697732C0466BCB37A7F36392980B4A99) )
	ROM_LOAD( "ramsnoop.4",   0x3000, 0x1000, CRC(9E7849E3) SHA1(8C574E42BFFC6B88D5B5DF3C6655F792B2FF5C5C) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8C3E6DE6) SHA1(FED6E9A2B210B07E7189A18574F6B8C4EC5BB49B) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368CB165) SHA1(387010A0C76319A1EAB61B54C9BCB5C66C4B67A1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5C281D01) SHA1(5E8B472B615F12EFCA3FE792410C23619F067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615AF909) SHA1(FD6A1DDE780B39AEA76BF1C4BEFA5882573C2EF4) )

	PACMAN_PROMS
ROM_END

ROM_START( sueworlb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "sueworlb.4",   0x3000, 0x1000, CRC(edc5ad28) SHA1(b1b95ed715a935fe3bf68c71cb4ac66402c17e79) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "sueworld.5e",  0x0000, 0x1000, CRC(6c975345) SHA1(8c294a14c3a2b390c85d75d61453e73ee8721058) )
	ROM_LOAD( "sueworld.5f",  0x1000, 0x1000, CRC(2c809842) SHA1(4325af0b41b67ce3783b667506b60d0221c6ac45) )

	PACMAN_PROMS
ROM_END

ROM_START( sueworld )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "sueworld.4",   0x3000, 0x1000, CRC(677adea7) SHA1(b258e748e47cb8ae77594728f1ccc97a429a411e) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "sueworld.5e",  0x0000, 0x1000, CRC(6c975345) SHA1(8c294a14c3a2b390c85d75d61453e73ee8721058) )
	ROM_LOAD( "sueworld.5f",  0x1000, 0x1000, CRC(2c809842) SHA1(4325af0b41b67ce3783b667506b60d0221c6ac45) )

	PACMAN_PROMS
ROM_END

ROM_START( superp4g )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "superp4g.1",   0x0000, 0x1000, CRC(f88c992f) SHA1(b6e2ec74dcc87aa097b6360f9fd19cfb11227efb) )
	ROM_LOAD( "superp4g.2",   0x1000, 0x1000, CRC(e6886710) SHA1(63207a2487a334e8de51ccede03b6d50e84e071a) )
	ROM_LOAD( "superp4g.3",   0x2000, 0x1000, CRC(c76dd2e6) SHA1(3172733cab013945de5f1c79e0a829cacad224ab) )
	ROM_LOAD( "superp4g.4",   0x3000, 0x1000, CRC(ba5b054e) SHA1(b7bb14167167c59ab9035539d5cf6338a10f00e0) )
	ROM_LOAD( "superp4g.5",   0x8000, 0x1000, CRC(c701506b) SHA1(31c96d13f290248c63143e22b052adb698a45687) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "superp4g.5e",  0x0000, 0x1000, CRC(593576fe) SHA1(238006f0d4fe6d478521a8d116f8d0679b502f6d) )
	ROM_LOAD( "superp4g.5f",  0x1000, 0x1000, CRC(e64e0f6d) SHA1(b87405ef0f90fc0a3afda1a4265ba6b0a074a21d) )

	PACMAN_PROMS
ROM_END

ROM_START( superp4m )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "superp4m.1",   0x0000, 0x1000, CRC(9caaac56) SHA1(8858e4e9d7e3b9a1cd55f6bff1b9766b13fd9b8b) )
	ROM_LOAD( "superp4g.2",   0x1000, 0x1000, CRC(e6886710) SHA1(63207a2487a334e8de51ccede03b6d50e84e071a) )
	ROM_LOAD( "superp4g.3",   0x2000, 0x1000, CRC(c76dd2e6) SHA1(3172733cab013945de5f1c79e0a829cacad224ab) )
	ROM_LOAD( "superp4g.4",   0x3000, 0x1000, CRC(ba5b054e) SHA1(b7bb14167167c59ab9035539d5cf6338a10f00e0) )
	ROM_LOAD( "superp4m.5",   0x8000, 0x1000, CRC(c4b3240c) SHA1(7839463d69c24d15ac4c4de199f2ba6ce1cfaad7) )
	ROM_LOAD( "boot6",        0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "superp4g.5e",  0x0000, 0x1000, CRC(593576fe) SHA1(238006f0d4fe6d478521a8d116f8d0679b502f6d) )
	ROM_LOAD( "superp4m.5f",  0x1000, 0x1000, CRC(9142e698) SHA1(229cc0f45ef4dcc60f1e449d24881a0a13720972) )

	PACMAN_PROMS
ROM_END

ROM_START( supmspac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "supmspac.2",   0x1000, 0x1000, CRC(f2110c60) SHA1(bfe4ba9bad0b3da44c36fa6735fdb38ba15a29ed) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "supmspac.4",   0x3000, 0x1000, CRC(0fe16984) SHA1(da77c5f04691d2679a53eb012542890c78f76683) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "mspacde.6a",   0x9000, 0x0800, CRC(286041cf) SHA1(5a5fc97ea66a59895b3403b2982940b755076667) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacit.5e",   0x0000, 0x0800, CRC(93933d1d) SHA1(fa38d2cb87e872bb9a3158a4df98f38360dc85ec) )
	ROM_LOAD( "mspacit.5h",   0x0800, 0x0800, CRC(7409fbec) SHA1(f440f08ba026ae6172666e1bdc0894ce33bba420) )
 	ROM_LOAD( "mspacit.5f",	  0x1000, 0x0800, CRC(22b0188a) SHA1(a9ed9ca8b36a60081fd364abc9bc23963932cc0b) )
 	ROM_LOAD( "mspacit.5j",   0x1800, 0x0800, CRC(50c7477d) SHA1(c04ec282a8cb528df5e38ad750d12ee71612695d) )

	PACMAN_PROMS
ROM_END

ROM_START( vectratk )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "boot1",   	  0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "mspacatk.2",   0x1000, 0x1000, CRC(0af09d31) SHA1(6ff73e4da4910bcd2ca3aa299d8ffad23f8abf79) )
	ROM_LOAD( "boot3", 	  0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4", 	  0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "mspacatk.5",   0x8000, 0x1000, CRC(e6e06954) SHA1(ee5b266b1cc178df31fc1da5f66ef4911c653dda) )
	ROM_LOAD( "mspacatk.6",   0x9000, 0x1000, CRC(3b5db308) SHA1(c1ba630cb8fb665c4881a6cce9d3b0d4300bd0eb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "msvectr.5e",   0x0000, 0x1000, CRC(56560aef) SHA1(1eac2b242328ec4d6ba9faac2fd972c8cb6c3e55) )
	ROM_LOAD( "msvectr.5f",   0x1000, 0x1000, CRC(57cb31e3) SHA1(dc9278cee7f7ce341087760654bf0a1bcdeb2f7c) )

	PACMAN_PROMS
ROM_END

ROM_START( womanp5 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "womanp5.1",    0x0000, 0x1000, CRC(0afeef14) SHA1(39d7d8ce9ba3c166c0bef7f46e71520d50e00c32) )
	ROM_LOAD( "superp4g.2",   0x1000, 0x1000, CRC(e6886710) SHA1(63207a2487a334e8de51ccede03b6d50e84e071a) )
	ROM_LOAD( "womanp5.3",    0x2000, 0x1000, CRC(835ebcc2) SHA1(951ec2e88f4d47b0dcdcac28f964860ea21a5726) )
	ROM_LOAD( "womanp5.4",    0x3000, 0x1000, CRC(b85c5572) SHA1(9eb397b24a783292d9a5ce9b866b19e9fdf2d1f8) )
	ROM_LOAD( "womanp5.5",    0x8000, 0x1000, CRC(ce7459ac) SHA1(16df266e465d6673448f85e8834c4f0031ede671) )
	ROM_LOAD( "womanp5.6",    0x9000, 0x1000, CRC(368cb165) SHA1(387010a0c76319a1eab61b54c9bcb5c66c4b67a1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "womanp5.5e",   0x0000, 0x1000, CRC(83613632) SHA1(1a47ef4d336baa3ba63cf0aceededec4bea35aaa) )
	ROM_LOAD( "womanp5.5f",   0x1000, 0x1000, CRC(f2c4195e) SHA1(54446e6d1b74623410b67d303c3ab8eaee69f392) )

	PACMAN_PROMS
ROM_END

ROM_START( zolaad )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zolapac1.bin", 0x0000, 0x4000, CRC(1aa2f312) SHA1(7b9685355e398c97de6972ec947fb11ca627a4d3) )
	ROM_LOAD( "zolapac2.bin", 0x8000, 0x2000, CRC(420ff603) SHA1(a314c28ec5c526da91535fd47f51c95539a2e8e8) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "msatkad.5e",   0x0000, 0x1000, CRC(dc70ed2e) SHA1(b9fbb1d21085bc78eb82c429506b20e332b18e0e) )
	ROM_LOAD( "msatkad.5f",   0x1000, 0x1000, CRC(643d5523) SHA1(565f92294a4b6a11dc5e9a165f35284c2a526e6c) )

	PACMAN_PROMS
ROM_END

ROM_START( zolaatk )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zolaatk.1",    0x0000, 0x1000, CRC(2b9e9433) SHA1(5ab15ad04b8fe1711c0ace0471ac3594382a9fb3) )
	ROM_LOAD( "zolaatk.2",    0x1000, 0x1000, CRC(0c8631d1) SHA1(09739200c90979550ae4ef14d27583aac50fd724) )
	ROM_LOAD( "zolaatk.3",    0x2000, 0x1000, CRC(694450a3) SHA1(fcaa1e3ee993c2f563721336c59cac8af30bcc15) )
	ROM_LOAD( "zolaatk.4",    0x3000, 0x1000, CRC(b5b30242) SHA1(b0b2044bcc26a1f9aee41c81367995b67ccd5edb) )
	ROM_LOAD( "zolaatk.5",    0x8000, 0x1000, CRC(24cb5d9b) SHA1(1439ba665b00e840838e5f12df7d44a022541ef4) )
	ROM_LOAD( "zolaatk.6",    0x9000, 0x1000, CRC(e0c03b4e) SHA1(3ef3decf7725a3ddb996c8268d4a83cbf5edd6b4) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( zolamaze )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "boot1",        0x0000, 0x1000, CRC(d16b31b7) SHA1(bc2247ec946b639dd1f00bfc603fa157d0baaa97) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "boot3",        0x2000, 0x1000, CRC(1821ee0b) SHA1(5ea4d907dbb2690698db72c4e0b5be4d3e9a7786) )
	ROM_LOAD( "boot4",        0x3000, 0x1000, CRC(165a9dd8) SHA1(3022a408118fa7420060e32a760aeef15b8a96cf) )
	ROM_LOAD( "zolamaze.5",   0x8000, 0x1000, CRC(f8ed55bb) SHA1(14c5b4adbf301dcaea2c59488f39eec08177f63f) )
	ROM_LOAD( "zolamaze.6",   0x9000, 0x1000, CRC(17e35c00) SHA1(f9f0ea4f462de298b704c2ddd53fef1219dae145) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( zolapac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zolapac1.bin", 0x0000, 0x4000, CRC(1aa2f312) SHA1(7b9685355e398c97de6972ec947fb11ca627a4d3) )
	ROM_LOAD( "zolapac2.bin", 0x8000, 0x2000, CRC(420ff603) SHA1(a314c28ec5c526da91535fd47f51c95539a2e8e8) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( zolaslow )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "zolaatk.1",    0x0000, 0x1000, CRC(2b9e9433) SHA1(5ab15ad04b8fe1711c0ace0471ac3594382a9fb3) )
	ROM_LOAD( "boot2",        0x1000, 0x1000, CRC(0d32de5e) SHA1(13ea0c343de072508908be885e6a2a217bbb3047) )
	ROM_LOAD( "zolaatk.3",    0x2000, 0x1000, CRC(694450a3) SHA1(fcaa1e3ee993c2f563721336c59cac8af30bcc15) )
	ROM_LOAD( "zolaatk.4",    0x3000, 0x1000, CRC(b5b30242) SHA1(b0b2044bcc26a1f9aee41c81367995b67ccd5edb) )
	ROM_LOAD( "zolaslow.5",   0x8000, 0x1000, CRC(092292f1) SHA1(156ca674c09abe348691585fb14d16d32488d50d) )
	ROM_LOAD( "zolaslow.6",   0x9000, 0x1000, CRC(52209eee) SHA1(91967cac6b327af24891e953df1c6900764ea09a) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( zolatunl )
	ROM_REGION( 0x10000, "maincpu", 0 )	
	ROM_LOAD( "zolaatk.1",    0x0000, 0x1000, CRC(2b9e9433) SHA1(5ab15ad04b8fe1711c0ace0471ac3594382a9fb3) )
	ROM_LOAD( "zolaatk.2",    0x1000, 0x1000, CRC(0c8631d1) SHA1(09739200c90979550ae4ef14d27583aac50fd724) )
	ROM_LOAD( "zolaatk.3",    0x2000, 0x1000, CRC(694450a3) SHA1(fcaa1e3ee993c2f563721336c59cac8af30bcc15) )
	ROM_LOAD( "zolaatk.4",    0x3000, 0x1000, CRC(b5b30242) SHA1(b0b2044bcc26a1f9aee41c81367995b67ccd5edb) )
	ROM_LOAD( "boot5",        0x8000, 0x1000, CRC(8c3e6de6) SHA1(fed6e9a2b210b07e7189a18574f6b8c4ec5bb49b) )
	ROM_LOAD( "zolaslow.6",   0x9000, 0x1000, CRC(52209eee) SHA1(91967cac6b327af24891e953df1c6900764ea09a) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "5e",           0x0000, 0x1000, CRC(5c281d01) SHA1(5e8b472b615f12efca3fe792410c23619f067845) )
	ROM_LOAD( "5f",           0x1000, 0x1000, CRC(615af909) SHA1(fd6a1dde780b39aea76bf1c4befa5882573c2ef4) )

	PACMAN_PROMS
ROM_END

ROM_START( zolavect )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zolapac1.bin", 0x0000, 0x4000, CRC(1aa2f312) SHA1(7b9685355e398c97de6972ec947fb11ca627a4d3) )
	ROM_LOAD( "zolapac2.bin", 0x8000, 0x2000, CRC(420ff603) SHA1(a314c28ec5c526da91535fd47f51c95539a2e8e8) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "msvectr.5e",   0x0000, 0x1000, CRC(56560aef) SHA1(1eac2b242328ec4d6ba9faac2fd972c8cb6c3e55) )
	ROM_LOAD( "msvectr.5f",   0x1000, 0x1000, CRC(57cb31e3) SHA1(dc9278cee7f7ce341087760654bf0a1bcdeb2f7c) )

	PACMAN_PROMS
ROM_END

/*************************************
 *
 *  Game drivers
 *
 *************************************/

/* MAME */

/*          rom       parent    machine   inp       init */
GAME( 1981, mspacman, 0,        mspacman, mspacpls, pacman_state,  mspacman, ROT90, "Midway", "Ms. Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmat, mspacman, mspacman, mspacman, pacman_state,  mspacman, ROT90, "hack", "Ms. Pac Attack", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmab, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "bootleg", "Ms. Pacman (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmbe, mspacman, woodpek,  mspacman, pacman_state,  mspacmbe, ROT90, "bootleg", "Ms. Pacman (bootleg, (encrypted))", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmnf, mspacman, mspacman, mspacman, pacman_state,  mspacman, ROT90, "Midway", "Ms. Pacman (with speedup hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacpls, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "hack", "Ms. Pacman Plus", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pacgal,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "hack", "Pac-Gal", MACHINE_SUPPORTS_SAVE )

/* Dave Widel's Games - http://www.widel.com */

GAME( 2003, mrmrspac, mspacman, widel,    mspacpls, driver_device, 0,        ROT90, "David Widel", "Mr and Mrs Pacman (MR MRS PAC-MAN)[c]", MACHINE_SUPPORTS_SAVE )
GAME( 1998, mspachi,  mspacman, mspachi,  mspacpls, driver_device, 0,        ROT90, "David Widel", "Ms. Pacman[h]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, pacatk,   mspacman, mspacman, mspacpls, pacman_state,  mspacman, ROT90, "David Widel", "Ms. Pacman Plus (Fruit)", MACHINE_SUPPORTS_SAVE )

/* Blue Justice's Hacks */

GAME( 2001, ms1000,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman 1000 (MS PAC-MAN 1000)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, ms2000,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman 2000 (MS PAC-MAN 2000)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, ms3000,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman 3000 (MS PAC-MAN 3000)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, ms4000p,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman 4000 Plus (MSPAC-MAN 4000PLUS)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, ms5000,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman 5000 (MS PAC-MAN 5000)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, ms5000p,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman 5000 Plus (MSPAC-MAN 5000PLUS)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, mscrzyma, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Crazy Mazes (MS PAC CRAZY MAZES)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, mscrzymp, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Crazy Mazes Plus (MS PAC-MAN CRAZYPLUS)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msextra,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Extra (MS PAC-MAN EXTRA)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msextrap, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Extra Plus (MS PAC-MAN EXTRAPLUS)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msmspac,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Ms. Pac (MS PAC-MAN MS PAC)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, mspacdel, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Deluxe (MS PAC-MAN DELUXE)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, mspamsba, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Ms. Babies (MS PAC-MAN MS BABIES)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, mspacp,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Plus (MS PAC-MAN PLUS)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msplus,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Ms Plus (MS PAC-MAN MS PLUS)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msstrmaz, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Blue Justice", "Ms. Pacman Strange Mazes (MS PAC STRANGE MAZES)", MACHINE_SUPPORTS_SAVE )

/* T-Bone Hacks */

GAME( 2000, ms2600,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman 2600", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msbaby,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Baby Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msmini,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pac-Mini", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msminia,  mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "T-Bone", "Ms. Pac-Attack Mini [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes4a,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman SNES (Tall Alternate)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes62,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman NES (Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes63,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman NES (Set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes6m,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman NES (6 maps)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes6m2, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman SNES (New Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes6m3, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman SNES (Regular / Tall)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msnes6m4, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman SNES (Regular)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, mspacnes, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman NES", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacren, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman Renaissance", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspc6mad, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Ms. Pacman After Dark (6 Mazes)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msvctr6m, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "T-Bone", "Vector Ms. Pacman (6 Mazes)", MACHINE_SUPPORTS_SAVE )

/* Jerronimo's Progs - www.umlautllama.com */

GAME( 2004, horizpab, mspacman, woodpek,  mspacman, driver_device, 0,        ROT0,  "Scott Lawrence", "Horiz Pac (v1.0)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, horizpac, mspacman, woodpek,  mspacman, driver_device, 0,        ROT0,  "Scott Lawrence", "Horiz Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2007, pacgent,  mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Scott Lawrence", "Pac Gentleman [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2004, ramsnoop, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Scott Lawrence", "Miss Snoop v1.3 [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2014, ottop1,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project P1", MACHINE_SUPPORTS_SAVE )
GAME( 2014, ottop2,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project P2", MACHINE_SUPPORTS_SAVE )
GAME( 2014, ottop3,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project P3", MACHINE_SUPPORTS_SAVE )
GAME( 2014, ottopz,   mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project PZ", MACHINE_SUPPORTS_SAVE )
GAME( 2014, superp4g, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project P4G", MACHINE_SUPPORTS_SAVE )
GAME( 2014, superp4m, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project P4M", MACHINE_SUPPORTS_SAVE )
GAME( 2014, womanp5,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Otto Project P5", MACHINE_SUPPORTS_SAVE )

/* Grendal74's Hacks */

GAME( 2001, msdroid,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Grendal74", "Ms. Pacman Android", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msgrkatk, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Grendal74", "Ms. Pac Attack Greek [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2001, msgrkb,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Grendal74", "Ms. Pacman Greek", MACHINE_SUPPORTS_SAVE )

/* Medragon's Hacks */

GAME( 2000, mrpacman, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Tim Appleton", "Mr. Pacman - Another Kind of Role Reversal", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacfnt, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Tim Appleton", "Ms. Pacman with Pacman font", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacwf,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Tim Appleton", "Ms Weird Fruit Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msrumble, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Tim Appleton", "Ms. Pac Rumble", MACHINE_SUPPORTS_SAVE )

/* Sil's Hacks */

GAME( 2000, mselton,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. Elton", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msf1pac,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. F1 Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msindy,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. Indy", MACHINE_SUPPORTS_SAVE )
GAME( 1999, mspac6m,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. Pacman 6M (Six Maze)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, mspac6m2, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. Pacman 6M (Six Maze) (Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacad,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. Pacman After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacat2, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Marcel Silvius", "Ms. Pacman Plus / Attack [c]", MACHINE_SUPPORTS_SAVE )

/* Bobby Tribble's Hacks */

GAME( 2000, msberzk,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Bobby Tribble", "Ms. Pacman Berzerk", MACHINE_SUPPORTS_SAVE )

/* Hires hacks */

GAME( 1981, mspacatx, mspacmnx, mspacmnx, mspacpls, pacman_state, mspacman,  ROT90, "hack", "Ms. Pac Attack (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmbx, mspacmnx, woodpekx, mspacman, driver_device, 0,        ROT90, "bootleg", "Ms. Pac-Man (bootleg) (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmfx, mspacmnx, mspacmnx, mspacpls, pacman_state, mspacman,  ROT90, "Midway", "Ms. Pac-Man (hires hack) [f]", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmnx, 0,        mspacmnx, mspacpls, pacman_state, mspacman,  ROT90, "Midway", "Ms. Pac-Man (hires hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacplx, mspacmnx, woodpekx, mspacpls, driver_device, 0,        ROT90, "hack", "Ms. Pac-Man Plus (hires hack) [c]", MACHINE_SUPPORTS_SAVE )
GAME( 1981, pacgalx,  mspacmnx, woodpekx, mspacman, driver_device, 0,        ROT90, "hack", "Pac-Gal (hires hack)", MACHINE_SUPPORTS_SAVE )

/* Misc MsPacman Hacks */

GAME( 2000, chtmsatk, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Plus [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, chtmspa,  mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2001, cookiem,  mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Nic", "Cookie-Mon! [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2008, cottoh,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Gatinho", "Crazy Otto simulation", MACHINE_SUPPORTS_SAVE )
GAME( 2010, cottoh2,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Gatinho", "Crazy Otto simulation Ver 2", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fasthear, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "PacHack", "Ms. Heart Burn [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fastmspa, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, faststrm, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Desert Storm [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, fstmsatk, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Attack [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, heartbn2, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "PacHack", "Ms. Heart Burn", MACHINE_SUPPORTS_SAVE )
GAME( 2000, heartbrn, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "TwoBit Score", "Ms. Heart Burn (set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msatk2ad, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Plus / Attack After Dark [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msatkad,  mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Plus After Dark [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msdstorm, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Mike Naylor", "Ms. Desert Storm", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mshangly, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Hangly Man", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msheartb, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Hearts (set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mshearts, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Hearts", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspac1,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman (single rom set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacatb, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman Plus (Set B) [c]", MACHINE_SUPPORTS_SAVE )
GAME( 1980, mspacde,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman -1", MACHINE_SUPPORTS_SAVE )
GAME( 1998, mspacii,  mspacman, mspacii,  mspacman, driver_device, 0,        ROT90, "Orca", "Ms. Pacman II", MACHINE_SUPPORTS_SAVE )
GAME( 1983, mspacit,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman -2", MACHINE_SUPPORTS_SAVE )
GAME( 1983, mspacit2, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman -3", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacj,   mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman (Jamma? Bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, mspacmap, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90,  "Don Hodges", "Ms. Pacman (patched)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacmbg, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman (single rom)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, mspacpbl, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Dav/PMP", "Ms. Pac Plus (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacrip, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "R3mer", "Ms. Pac-Mortem", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mspacsup, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Super Ms. Pac-Man", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mssilad,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman After Dark (Sil)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msultra,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "PacFan", "Ms. Ultra Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msvectr,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Vector Ms. Pacman", MACHINE_SUPPORTS_SAVE )
GAME( 2000, msyakman, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Jeff Minter", "Ms. Yak Man", MACHINE_SUPPORTS_SAVE )
GAME( 1982, pacgalu,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Uchida", "Pac-Gal (Hearts)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, petshop,  mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "Weirdness Inc", "Pet Shop Freak-Out!", MACHINE_SUPPORTS_SAVE )
GAME( 2002, piranha2, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "US Billiards", "Piranha 2 Revenge!", MACHINE_SUPPORTS_SAVE )
GAME( 2000, sueworld, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "PacFay", "Sue's World", MACHINE_SUPPORTS_SAVE )
GAME( 2000, sueworlb, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "PacFay", "Sue's World (Old)", MACHINE_SUPPORTS_SAVE )
GAME( 19??, supmspac, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "Spedic Vending", "Super Ms Pac-man [f]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vectratk, mspacman, woodpek,  mspacpls, driver_device, 0,        ROT90, "[Midway]", "Vector Attack [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolaad,   mspacman, zolapac,  mspacman, driver_device, 0,        ROT90, "Tqwn Amusement", "Super Zola Pac Gal After Dark", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolaatk,  mspacman, zolapac,  mspacman, driver_device, 0,        ROT90, "Tqwn Amusement", "Super Zola Pac Gal Attack", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolamaze, mspacman, woodpek,  mspacman, driver_device, 0,        ROT90, "[Midway]", "Ms. Pacman No Exit", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolapac,  mspacman, zolapac,  mspacman, driver_device, 0,        ROT90, "Tqwn Amusement", "Super Zola Pac Gal", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolaslow, mspacman, zolapac,  mspacman, driver_device, 0,        ROT90, "Tqwn Amusement", "Super Zola Pac Gal (Slow)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolatunl, mspacman, zolapac,  mspacman, driver_device, 0,        ROT90, "Tqwn Amusement", "Super Zola Pac Gal (Tunnels)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, zolavect, mspacman, zolapac,  mspacman, driver_device, 0,        ROT90, "Tqwn Amusement", "Vector Super Zola Pac Gal", MACHINE_SUPPORTS_SAVE )

