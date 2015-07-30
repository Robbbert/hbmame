// license:BSD-3-Clause
// copyright-holders:Robbbert
/**********************************************
     Multi-game systems on pacman hardware
     by Robbbert 2007-2013

     Game choice is done via a menu
**********************************************/

WRITE8_MEMBER( pacman_state::m96in1_rombank_w )
{
	data &= 0x0f;
	membank("bank1")->set_entry(data);
	membank("bank2")->set_entry(data);
	multipac_gfxbank_w( space, 0, data );
}

WRITE8_MEMBER( pacman_state::m96in1b_rombank_w )
{
	data &= 0x1f;			/* config screen outputs some large values such as 197, 200 */
	membank("bank1")->set_entry(data);
	membank("bank2")->set_entry(data);
	m96in1b_gfxbank_w( data );
}

WRITE8_MEMBER( pacman_state::hackypac_rombank_w )
{
	UINT8 banks[] = { 0, 0, 0, 0, 0, 0, 4, 0, 7, 0, 0, 0, 8, 6, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 5, 3 };
	data &= 0x1f;			/* strip extra bits when option selected */
	membank("bank1")->set_entry(data);
	membank("bank2")->set_entry(data);
	multipac_palbank_w( space, 1, banks[data] );	/* convert to a palette bank and switch to it */
}

WRITE8_MEMBER( pacman_state::madpac_rombank_w )
{
	data &= 0x1f;			/* config screen outputs some large values such as 197, 200 */
	membank("bank1")->set_entry(data);
	membank("bank2")->set_entry(data);
	madpac_gfxbank_w( data );
}

WRITE8_MEMBER( pacman_state::multipac_rombank_w )
{
	UINT8 temp = ((data & 0x0f) << 1) + ((data & 0x10) >> 4);	/* rearrange bits */
	membank("bank1")->set_entry(temp);				/* select rom bank */
	if (!m_maincpu->space(AS_PROGRAM).read_byte(0xa007))
		membank("bank2")->set_entry(temp << 1);	/* Ms Pacman needs more roms */
	m_maincpu->set_pc(0);					/* Set PC <- 0000 needed for Lizard Wizard */
}

WRITE8_MEMBER( pacman_state::pm4n1_rombank_w )
{
	static UINT8 pm4n1_temp = 0;
	UINT8 banks[] = { 0, 4, 2, 0, 1, 0, 3, 0 };
	pm4n1_temp &= (7 - (1<<offset));
	pm4n1_temp |= (data&1)<<offset;
	membank("bank1")->set_entry(banks[pm4n1_temp]);
	membank("bank2")->set_entry(banks[pm4n1_temp]);
	multipac_gfxbank_w( space, 0, banks[pm4n1_temp] );
}

WRITE8_MEMBER( pacman_state::pm4n1d_rombank_w )
{
	if (offset==1)
	{
		membank("bank1")->set_entry(data);
		membank("bank2")->set_entry(data);
		multipac_gfxbank_w( space, 0, data<<1 );
	}
}


/*************************************
 *
 *  Machine init
 *
 *************************************/

MACHINE_RESET_MEMBER( pacman_state, 96in1 )
{
	address_space &space = m_maincpu->space(AS_PROGRAM);
	m96in1_rombank_w( space, 0,0 );
	m_namco_sound->pacman_sound_enable_w(space, 0, 0);
}

MACHINE_RESET_MEMBER( pacman_state, hackypac )
{
	address_space &space = m_maincpu->space(AS_PROGRAM);
	multipac_gfxbank_w( space, 0,0 );
	hackypac_rombank_w( space, 0,0 );
	m_namco_sound->pacman_sound_enable_w(space, 0, 0);
}

MACHINE_RESET_MEMBER( pacman_state, madpac )
{
	address_space &space = m_maincpu->space(AS_PROGRAM);
	madpac_rombank_w( space, 0,0 );
	m_namco_sound->pacman_sound_enable_w(space, 0, 0);
}

MACHINE_RESET_MEMBER( pacman_state, mschamp )
{
	UINT8 *rom = memregion("maincpu")->base() + 0x10000;
	UINT8 data = ioport("GAME")->read() & 1;

	membank("bank1")->configure_entries(0, 2, &rom[0x0000], 0x8000);
	membank("bank2")->configure_entries(0, 2, &rom[0x4000], 0x8000);

	membank("bank1")->set_entry(data);
	membank("bank2")->set_entry(data);
}

MACHINE_RESET_MEMBER( pacman_state, multipac )
{
	address_space &space = m_maincpu->space(AS_PROGRAM);
	multipac_rombank_w( space, 0, 0);
	multipac_gfxbank_w( space, 0, 0);
	multipac_palbank_w( space, 0, 0);
	m_namco_sound->pacman_sound_enable_w(space, 0, 0);
}

static UINT8 curr_bank = 0;

/* select next game when F3 pressed */
MACHINE_RESET_MEMBER( pacman_state, mspaceur )
{
	membank("bank2")->set_entry(curr_bank);
	curr_bank ^= 1;		/* toggle between 0 and 1 */
}

MACHINE_RESET_MEMBER( pacman_state, pm4n1 )
{
	address_space &space = m_maincpu->space(AS_PROGRAM);
	pm4n1_rombank_w(space, 0, 0);
	pm4n1_rombank_w(space, 1, 0);
	pm4n1_rombank_w(space, 2, 0);
	m_namco_sound->pacman_sound_enable_w(space, 0, 0);
}



static ADDRESS_MAP_START( 96in1_writeport, AS_IO, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_WRITE(pacman_interrupt_vector_w)
	AM_RANGE(0x01, 0x01) AM_WRITE(m96in1_rombank_w)
ADDRESS_MAP_END

static ADDRESS_MAP_START( 96in1b_writeport, AS_IO, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_WRITE(pacman_interrupt_vector_w)
	AM_RANGE(0x01, 0x01) AM_WRITE(m96in1b_rombank_w)
ADDRESS_MAP_END

static ADDRESS_MAP_START( hackypac_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop) AM_WRITENOP
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x8000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x8000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5008, 0x503f) AM_WRITENOP
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x507f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5080, 0x50bf) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50ff) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank2")
	AM_RANGE(0xfffc, 0xffff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( hackypac_writeport, AS_IO, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_WRITE(pacman_interrupt_vector_w)
	AM_RANGE(0x01, 0x01) AM_WRITE(hackypac_rombank_w)
	AM_RANGE(0x02, 0x02) AM_WRITE(multipac_gfxbank_w)
	AM_RANGE(0x04, 0x04) AM_WRITENOP			/* colorbank select, not used due to a bug */
ADDRESS_MAP_END

static ADDRESS_MAP_START( madpac_map, AS_PROGRAM, 8, pacman_state )
	/* Mirrors in the 50xx range are needed by Zigzag */
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x8000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x8000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0x8000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_MIRROR(0x8000) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5008, 0x500f) AM_WRITENOP			/* most pacman-type games write here */
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x507f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0x8000) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_MIRROR(0x8000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_MIRROR(0x8000) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_MIRROR(0x8000) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_MIRROR(0x8000) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0xb7ff) AM_ROMBANK("bank2")
	AM_RANGE(0xb800, 0xbfff) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0xf800, 0xffff) AM_RAM				/* various games use this */
ADDRESS_MAP_END

static ADDRESS_MAP_START( madpac_writeport, AS_IO, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_WRITE(pacman_interrupt_vector_w)
	AM_RANGE(0x01, 0x01) AM_WRITE(madpac_rombank_w)
ADDRESS_MAP_END


static ADDRESS_MAP_START( mspaceur_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x8000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x8000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0x9fff) AM_ROMBANK("bank2")
	AM_RANGE(0xfffc, 0xffff) AM_RAM
ADDRESS_MAP_END


static ADDRESS_MAP_START( mschamp_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop)
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x8000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x8000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP // AM_WRITE(pacman_coin_lockout_global_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank2")
	AM_RANGE(0xf270, 0xf270) AM_RAM
	AM_RANGE(0xfffc, 0xffff) AM_RAM
ADDRESS_MAP_END


static ADDRESS_MAP_START( multipac_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop) AM_WRITENOP
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x8000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x8000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5005) AM_WRITE(pacman_leds_w)
	AM_RANGE(0x5006, 0x5006) AM_WRITENOP /* AM_WRITE(pacman_coin_lockout_global_w) breaks eyes,lizwiz,crush */
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c0) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5000) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5040) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5080) AM_READ_PORT("DSW1")
	AM_RANGE(0x50c0, 0x50c0) AM_READ_PORT("DSW2")
	AM_RANGE(0x8000, 0x9fff) AM_ROMBANK("bank2")			/* Ms. Pacman */
	AM_RANGE(0xa000, 0xa000) AM_WRITE(multipac_rombank_w)
	AM_RANGE(0xa001, 0xa001) AM_WRITE(multipac_gfxbank_w)
	AM_RANGE(0xa002, 0xa003) AM_WRITE(multipac_palbank_w)
	AM_RANGE(0xa007, 0xa007) AM_RAM				/* 0 = get ready to activate rombank(2) */
	AM_RANGE(0xf000, 0xffff) AM_RAM
ADDRESS_MAP_END


static ADDRESS_MAP_START( pm4n1_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_MIRROR(0x8000) AM_READ(pacman_read_nop) AM_WRITENOP
	AM_RANGE(0x4c00, 0x4fef) AM_MIRROR(0x8000) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_MIRROR(0x8000) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5006) AM_WRITE(pm4n1_rombank_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c1) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5001) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5041) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5081) AM_READ_PORT("DSW1")
	AM_RANGE(0x8000, 0x9fff) AM_ROMBANK("bank2")			/* Ms. Pacman */
ADDRESS_MAP_END


static ADDRESS_MAP_START( pm4n1c_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0x4c00, 0x4fef) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5006) AM_WRITE(pm4n1_rombank_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c1) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5001) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5041) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5081) AM_READ_PORT("DSW1")
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank2")
	AM_RANGE(0xe004, 0xe006) AM_WRITENOP	// mirror of 5004-5006
ADDRESS_MAP_END


static ADDRESS_MAP_START( pm4n1d_map, AS_PROGRAM, 8, pacman_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROMBANK("bank1")
	AM_RANGE(0x4000, 0x43ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x4400, 0x47ff) AM_MIRROR(0x8000) AM_RAM_WRITE(pacman_colorram_w) AM_SHARE("colorram")
	AM_RANGE(0x4800, 0x4bff) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0x4c00, 0x4fef) AM_RAM
	AM_RANGE(0x4ff0, 0x4fff) AM_RAM AM_SHARE("spriteram")
	AM_RANGE(0x5000, 0x5000) AM_WRITE(irq_mask_w)
	AM_RANGE(0x5001, 0x5001) AM_DEVWRITE("namco", namco_device, pacman_sound_enable_w)
	AM_RANGE(0x5002, 0x5002) AM_WRITENOP
	AM_RANGE(0x5003, 0x5003) AM_WRITE(pacman_flipscreen_w)
	AM_RANGE(0x5004, 0x5006) AM_WRITE(pm4n1d_rombank_w)
	AM_RANGE(0x5007, 0x5007) AM_WRITE(pacman_coin_counter_w)
	AM_RANGE(0x5040, 0x505f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x5060, 0x506f) AM_WRITEONLY AM_SHARE("spriteram2")
	AM_RANGE(0x5070, 0x507f) AM_WRITENOP
	AM_RANGE(0x50c0, 0x50c1) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x5000, 0x5001) AM_READ_PORT("IN0")
	AM_RANGE(0x5040, 0x5041) AM_READ_PORT("IN1")
	AM_RANGE(0x5080, 0x5081) AM_READ_PORT("DSW1")
	AM_RANGE(0x8000, 0xbfff) AM_ROMBANK("bank2")
	AM_RANGE(0xe004, 0xe006) AM_WRITENOP	// mirror of 5004-5006
ADDRESS_MAP_END



/*************************************
 *
 *  Port definitions
 *
 *************************************/

INPUT_PORTS_START( 96in1 )
	PORT_START ("IN0")	/* IN0 */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	/* Press this while playing pacman to instantly finish the level */
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME("Finish Level (Cheat)") PORT_CODE(KEYCODE_8)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN3 )	/* Alien Armada only */

	PORT_START ("IN1")	/* IN1 */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  ) PORT_4WAY PORT_COCKTAIL
	/* Hold this down while starting pacman to get service settings */
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME("Service") PORT_CODE(KEYCODE_9)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )	/* Also used as the fire button */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_DIPNAME(0x80, 0x80, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(   0x80, DEF_STR( Upright ) )
	PORT_DIPSETTING(   0x00, DEF_STR( Cocktail ) )

	PORT_START ("DSW1")	/* DSW 1 */
	PORT_DIPNAME( 0x01, 0x01, "DIP 1:  Simple Menu" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, "DIP 2:  Initials only" )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "DIP 3:  Fast OK for TM" )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "DIP 4:  Tournament Mode" )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "DIP 5:  Guest Mode" )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "DIP 6:  Dip Menu" )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x80, 0x80, "Ghost Names" )
	PORT_DIPSETTING(    0x80, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Alternate ) )

	PORT_START ("DSW2")	/* DSW 2 */
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START ("FAKE")
	/* This fake input port is used to get the status of the fire button */
	/* and activate the speedup cheat if it is. */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME( "Speed (Cheat)" )
	PORT_DIPNAME( 0x06, 0x02, "Speed Cheat" )
	PORT_DIPSETTING(    0x00, "Disabled" )
	PORT_DIPSETTING(    0x02, "Enabled with Button" )
	PORT_DIPSETTING(    0x04, "Enabled Always" )
INPUT_PORTS_END

INPUT_PORTS_START( mschamp )
	PORT_INCLUDE( mspacpls )

	PORT_START ("GAME")
	PORT_DIPNAME( 0x01, 0x01, "Game" )
	PORT_DIPSETTING(    0x01, "Champion Edition" )
	PORT_DIPSETTING(    0x00, "Super Pac Gal" )
	PORT_DIPNAME( 0x02, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x02, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x04, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x08, DEF_STR( On ) )
INPUT_PORTS_END

INPUT_PORTS_START( multipac )
	PORT_START ("IN0")	/* IN0 */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_SERVICE( 0x10, IP_ACTIVE_LOW )	/* service for eyes and lizwiz */
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE1 )

	PORT_START ("IN1")	/* IN1 */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )	/* also used as the fire button (eyes, lizwiz) */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_DIPNAME(0x80, 0x80, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(   0x80, DEF_STR( Upright ) )
	PORT_DIPSETTING(   0x00, DEF_STR( Cocktail ) )

	PORT_START ("DSW1")	/* DSW 1 */
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x08, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "1" )
	PORT_DIPSETTING(    0x04, "2" )
	PORT_DIPSETTING(    0x08, "3" )
	PORT_DIPSETTING(    0x0c, "5" )
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x00, "10000" )
	PORT_DIPSETTING(    0x10, "15000" )
	PORT_DIPSETTING(    0x20, "20000" )
	PORT_DIPSETTING(    0x30, DEF_STR( None ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x80, 0x80, "Ghost Names" )
	PORT_DIPSETTING(    0x80, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Alternate ) )

	PORT_START ("DSW2")	/* DSW 2 */
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )

//	PORT_START ("FAKE")
	/* This fake input port is used to get the status of the fire button */
	/* and activate the speedup cheat if it is. */
//	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME( "Speed (Cheat)" )
//	PORT_DIPNAME( 0x06, 0x02, "Speed Cheat" )
//	PORT_DIPSETTING(    0x00, "Disabled" )
//	PORT_DIPSETTING(    0x02, "Enabled with Button" )
//	PORT_DIPSETTING(    0x04, "Enabled Always" )
INPUT_PORTS_END


/*************************************
 *
 *  Driver initialization
 *
 *************************************/

DRIVER_INIT_MEMBER( pacman_state, 96in1 )
{
	UINT8 *RAM = memregion("maincpu")->base();
	membank("bank1")->configure_entries(0, 16, &RAM[0x10000], 0x8000);
	membank("bank2")->configure_entries(0, 16, &RAM[0x14000], 0x8000);
}

DRIVER_INIT_MEMBER( pacman_state, madpac )
{
	UINT8 *RAM = memregion("maincpu")->base();
	membank("bank1")->configure_entries(0, 32, &RAM[0x10000], 0x8000);
	membank("bank2")->configure_entries(0, 32, &RAM[0x14000], 0x8000);
}

DRIVER_INIT_MEMBER( pacman_state, mspaceur )
{
	UINT8 *RAM = memregion("maincpu")->base();
	membank("bank2")->configure_entries(0, 2, &RAM[0x10000], 0x2000);
}

DRIVER_INIT_MEMBER( pacman_state, multipac )
{
	UINT8 *RAM = memregion("maincpu")->base();
	RAM[0x10000] = 0xED;	/* It seems that IM0 is not working properly in MAME */
	RAM[0x10001] = 0x56;	/* and, the interrupt mode is not being reset when a */
	RAM[0x10002] = 0xC3;	/* machine reset is done. So, inserting some code so */
	RAM[0x10003] = 0x00;	/* that the game can fix the problem */
	RAM[0x10004] = 0x01;
	membank("bank1")->configure_entries(0, 32, &RAM[0x10000], 0x4000);
	membank("bank2")->configure_entries(0, 64, &RAM[0x14000], 0x2000);
	membank("bank1")->set_entry(0);
}

DRIVER_INIT_MEMBER( pacman_state, pm4n1 )
{
	UINT8 *RAM = memregion("maincpu")->base();
	membank("bank1")->configure_entries(0, 5, &RAM[0x10000], 0x8000);
	membank("bank2")->configure_entries(0, 5, &RAM[0x14000], 0x8000);
}


/********************************
 Graphics layouts
 ********************************/

static GFXDECODE_START( 96in1 )
	GFXDECODE_ENTRY( "gfx1", 0x00000, tilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x10000, spritelayout, 0, 32 )
GFXDECODE_END

static GFXDECODE_START( 96in1b )
	GFXDECODE_ENTRY( "gfx1", 0x00000, tilelayout,   0, 8*32 )
	GFXDECODE_ENTRY( "gfx1", 0x20000, spritelayout, 0, 8*32 )
GFXDECODE_END

static GFXDECODE_START( hackypac )
	GFXDECODE_ENTRY( "gfx1", 0x00000, tilelayout,   0, 9*32 )
	GFXDECODE_ENTRY( "gfx1", 0x40000, spritelayout, 0, 9*32 )
GFXDECODE_END

static GFXDECODE_START( madpac )
	GFXDECODE_ENTRY( "gfx1", 0x00000, tilelayout,   0, 16*32 )
	GFXDECODE_ENTRY( "gfx1", 0x20000, spritelayout, 0, 16*32 )
GFXDECODE_END

static GFXDECODE_START( multipac )
	GFXDECODE_ENTRY( "gfx1", 0x00000, tilelayout,   0, 4*32 )
	GFXDECODE_ENTRY( "gfx1", 0x10000, spritelayout, 0, 4*32 )
GFXDECODE_END

static GFXDECODE_START( pm4n1 )
	GFXDECODE_ENTRY( "gfx1", 0x00000, tilelayout,   0, 32 )
	GFXDECODE_ENTRY( "gfx1", 0x05000, spritelayout, 0, 32 )
GFXDECODE_END


/*************************************
 *
 *  Machine drivers
 *
 *************************************/

/* These drivers are for multiple games in one package */

static MACHINE_CONFIG_DERIVED( 96in1, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(madpac_map)
	MCFG_CPU_IO_MAP(96in1_writeport)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, 96in1)
	MCFG_GFXDECODE_MODIFY("gfxdecode", 96in1)
	MCFG_NVRAM_ADD_0FILL("nvram")
	MCFG_VIDEO_START_OVERRIDE(pacman_state, multipac)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( 96in1b, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(madpac_map)
	MCFG_CPU_IO_MAP(96in1b_writeport)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, madpac)
	MCFG_GFXDECODE_MODIFY("gfxdecode", 96in1b)
	MCFG_PALETTE_MODIFY("palette")
	MCFG_PALETTE_ENTRIES(128*8)
	MCFG_PALETTE_INDIRECT_ENTRIES(32*8)
	MCFG_PALETTE_INIT_OWNER(pacman_state,multipac)
	MCFG_VIDEO_START_OVERRIDE(pacman_state, multipac)
	MCFG_NVRAM_ADD_0FILL("nvram")
	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_multipac)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( hackypac, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(hackypac_map)
	MCFG_CPU_IO_MAP(hackypac_writeport)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, hackypac)
	MCFG_GFXDECODE_MODIFY("gfxdecode", hackypac)
	MCFG_PALETTE_MODIFY("palette")
	MCFG_PALETTE_ENTRIES(128*9)
	MCFG_PALETTE_INDIRECT_ENTRIES(32*9)		// colour banks * 128 colour lookup codes (4a rom)
	MCFG_PALETTE_INIT_OWNER(pacman_state,multipac)
	MCFG_VIDEO_START_OVERRIDE(pacman_state, multipac)
	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_multipac)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( madpac, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(madpac_map)
	MCFG_CPU_IO_MAP(madpac_writeport)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, madpac)
	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_multipac)
	MCFG_VIDEO_START_OVERRIDE(pacman_state, multipac)
	MCFG_PALETTE_MODIFY("palette")
	MCFG_PALETTE_ENTRIES(128*16)
	MCFG_PALETTE_INDIRECT_ENTRIES(32*16)
	MCFG_PALETTE_INIT_OWNER(pacman_state, multipac)
	MCFG_GFXDECODE_MODIFY("gfxdecode", madpac)
	MCFG_NVRAM_ADD_0FILL("nvram")
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( mspaceur, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mspaceur_map)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, mspaceur)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( mschamp, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mschamp_map)
	MCFG_CPU_IO_MAP(zolapac_io)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, mschamp)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( mschampx, pacmanx )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(mschamp_map)
	MCFG_CPU_IO_MAP(zolapac_io)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, mschamp)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( multipac, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(multipac_map)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, multipac)
	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_UPDATE_DRIVER(pacman_state, screen_update_multipac)
	MCFG_VIDEO_START_OVERRIDE(pacman_state, multipac)
	MCFG_PALETTE_MODIFY("palette")
	MCFG_PALETTE_ENTRIES(128*4)
	MCFG_PALETTE_INDIRECT_ENTRIES(32*4)
	MCFG_PALETTE_INIT_OWNER(pacman_state,multipac)
	MCFG_GFXDECODE_MODIFY("gfxdecode", multipac)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( pm4n1, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(pm4n1_map)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, pm4n1)
	MCFG_GFXDECODE_MODIFY("gfxdecode", pm4n1)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( pm4n1c, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(pm4n1c_map)
	MCFG_MACHINE_RESET_OVERRIDE(pacman_state, pm4n1)
	MCFG_NVRAM_ADD_0FILL("nvram")
	MCFG_GFXDECODE_MODIFY("gfxdecode", pm4n1)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( pm4n1d, pm4n1c )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(pm4n1d_map)
MACHINE_CONFIG_END


/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( 96in1 )
	ROM_REGION( 0x90000, "maincpu", 0 )
	ROM_LOAD( "96in1.cpu",    0x10000, 0x80000, CRC(4a25b763) SHA1(7e650fb7b60b08f3ab8307035a393d6b2941b24b) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "96in1.gfx",    0x00000, 0x1000, CRC(fb43e056) SHA1(aceacef836e4dd1bfe4229ea090ff713c66d8931) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	PACMAN_PROMS
ROM_END

ROM_START( 96in1a )
	ROM_REGION( 0x90000, "maincpu", 0 )
	ROM_LOAD( "96in1a.cpu",   0x10000, 0x80000, CRC(bd89f3de) SHA1(3bd2f51ce97509c7f3dc165ebfbf856039be375e) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "96in1a.gfx",   0x00000, 0x1000, CRC(f4ce2dca) SHA1(b0515ee98766225d4d4a1368949942324703e865) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	PACMAN_PROMS
ROM_END

ROM_START( 96in1b )
	ROM_REGION( 0x110000, "maincpu", 0 )
	ROM_LOAD( "96in1b.cpu",   0x10000, 0x80000, CRC(1c880d0b) SHA1(374bcefb047e48598706b2df27fd6715b11afd45) )
	ROM_LOAD( "96in1b.cp2",   0x90000, 0x80000, CRC(54ee41bd) SHA1(84fc81e2cd584c0fc94782c008990d66c599e435) )

	ROM_REGION( 0x40000, "gfx1", 0 )
	ROM_LOAD( "96in1b.gfx",   0x00000, 0x1000, CRC(ac6e00ac) SHA1(3c011cc76aebba6cfa17c042d0835055c2b42735) )
	ROM_CONTINUE(             0x20000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x21000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x22000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x23000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x24000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x25000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x26000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x27000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x28000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x29000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x2a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x2b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x2c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x2d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x2e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x2f000, 0x1000 )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x30000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x31000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x32000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x33000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x34000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x35000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x36000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x37000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x38000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x39000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x3a000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x3b000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x3c000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x3d000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x3e000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )
	ROM_CONTINUE(             0x3f000, 0x1000 )
	ROM_IGNORE  (             0x40000 )

	ROM_REGION( 0x1b00, "proms", 0 )
	ROM_LOAD( "96in1b.7f",    0x1000, 0x0160, CRC(58deb197) SHA1(2b10b9f21b4d498b9284e1308793dc773e9db874) )
	ROM_IGNORE(               0x3fea0 )
	ROM_COPY( "proms",        0x1000, 0x0000, 0x10 )	/* pacman and mspacman */
	ROM_COPY( "proms",        0x1000, 0x0020, 0x10 )	/* 08. pacplus */
	ROM_COPY( "proms",        0x1100, 0x00e0, 0x10 )	/* 10. naughty mouse */
	ROM_COPY( "proms",        0x1110, 0x0080, 0x10 )	/* 11. beastie feastie */
	ROM_COPY( "proms",        0x1120, 0x0040, 0x10 )	/* 12. make trax */
	ROM_COPY( "proms",        0x1130, 0x0060, 0x10 )	/* 13. lizard wizard */
	ROM_COPY( "proms",        0x1140, 0x00a0, 0x10 )	/* 14. atlantic city action */
	ROM_COPY( "proms",        0x1150, 0x00c0, 0x10 )	/* 15. jump shot */

	ROM_LOAD( "96in1b.4a",    0x1000, 0x0b00, CRC(fb1acbea) SHA1(bf344c46adea405125d67b3bf8a14d51316d3e12) )
	ROM_IGNORE(               0x3f500 )
	ROM_FILL(                 0x1068, 0x0001, 0x00 )	/* pacman - remove red bit outside monster lair */
	ROM_FILL(                 0x106c, 0x0001, 0x00 )	/* pacman - remove pink bits at top and bottom */
	ROM_COPY( "proms",        0x1000, 0x0100, 0x80 )	/* pacman and mspacman */
	ROM_COPY( "proms",        0x1000, 0x0180, 0x80 )	/* pacplus */
	ROM_COPY( "proms",        0x1800, 0x0480, 0x80 )	/* naughty mouse */
	ROM_COPY( "proms",        0x1880, 0x0300, 0x80 )	/* beastie feastie */
	ROM_COPY( "proms",        0x1900, 0x0200, 0x80 )	/* make trax */
	ROM_COPY( "proms",        0x1980, 0x0280, 0x80 )	/* lizard wizard */
	ROM_COPY( "proms",        0x1a00, 0x0380, 0x80 )	/* atlantic city action */
	ROM_COPY( "proms",        0x1a80, 0x0400, 0x80 )	/* jump shot */
	ROM_FILL(                 0x01c3, 0x0001, 0x0d )	/* pacplus - walls - make green */
	ROM_FILL(                 0x01eb, 0x0001, 0x0d )	/* pacplus - walls outside monster house - make green */
	ROM_FILL(                 0x01ef, 0x0001, 0x0d )	/* pacplus - corner bits at bottom - make green */

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( 96in1c )
	ROM_REGION( 0x90000, "maincpu", 0 )
	ROM_LOAD( "96in1c.cpu",   0x10000, 0x80000, CRC(72d5bf9a) SHA1(7d576ab2642fde8569ea58f6eea4390c309c6885) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "96in1c.gfx",   0x00000, 0x1000, CRC(ebbbad14) SHA1(cb7b538f52c7254ed2507db30c199e4b91a309d5) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	PACMAN_PROMS
ROM_END

ROM_START( hackypac )
	ROM_REGION( 0x110000, "maincpu", 0 )
	ROM_LOAD( "hackypac.cpu", 0x10000, 0x100000, CRC(9cc452a1) SHA1(7b0483e359f87c5e3035db7380436f176f2356b5) )

	ROM_REGION( 0x80000, "gfx1", 0 )
	ROM_LOAD( "hackypac.gfx", 0x00000, 0x1000, CRC(42a61854) SHA1(0968721abccf7d04d205d13d150d014e4e658d16) )
	ROM_CONTINUE(             0x40000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )	/* rearrange so that all gfx then all sprites */
	ROM_CONTINUE(             0x41000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x42000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x43000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x44000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x45000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x46000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x47000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x48000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x49000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x4a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x4b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x4c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x4d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x4e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x4f000, 0x1000 )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x50000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x51000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x52000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x53000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x54000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x55000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x56000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x57000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x58000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x59000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x5a000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x5b000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x5c000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x5d000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x5e000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )
	ROM_CONTINUE(             0x5f000, 0x1000 )
	ROM_CONTINUE(             0x20000, 0x1000 )
	ROM_CONTINUE(             0x60000, 0x1000 )
	ROM_CONTINUE(             0x21000, 0x1000 )
	ROM_CONTINUE(             0x61000, 0x1000 )
	ROM_CONTINUE(             0x22000, 0x1000 )
	ROM_CONTINUE(             0x62000, 0x1000 )
	ROM_CONTINUE(             0x23000, 0x1000 )
	ROM_CONTINUE(             0x63000, 0x1000 )
	ROM_CONTINUE(             0x24000, 0x1000 )
	ROM_CONTINUE(             0x64000, 0x1000 )
	ROM_CONTINUE(             0x25000, 0x1000 )
	ROM_CONTINUE(             0x65000, 0x1000 )
	ROM_CONTINUE(             0x26000, 0x1000 )
	ROM_CONTINUE(             0x66000, 0x1000 )
	ROM_CONTINUE(             0x27000, 0x1000 )
	ROM_CONTINUE(             0x67000, 0x1000 )
	ROM_CONTINUE(             0x28000, 0x1000 )
	ROM_CONTINUE(             0x68000, 0x1000 )
	ROM_CONTINUE(             0x29000, 0x1000 )
	ROM_CONTINUE(             0x69000, 0x1000 )
	ROM_CONTINUE(             0x2a000, 0x1000 )
	ROM_CONTINUE(             0x6a000, 0x1000 )
	ROM_CONTINUE(             0x2b000, 0x1000 )
	ROM_CONTINUE(             0x6b000, 0x1000 )
	ROM_CONTINUE(             0x2c000, 0x1000 )
	ROM_CONTINUE(             0x6c000, 0x1000 )
	ROM_CONTINUE(             0x2d000, 0x1000 )
	ROM_CONTINUE(             0x6d000, 0x1000 )
	ROM_CONTINUE(             0x2e000, 0x1000 )
	ROM_CONTINUE(             0x6e000, 0x1000 )
	ROM_CONTINUE(             0x2f000, 0x1000 )
	ROM_CONTINUE(             0x6f000, 0x1000 )
	ROM_CONTINUE(             0x30000, 0x1000 )
	ROM_CONTINUE(             0x70000, 0x1000 )
	ROM_CONTINUE(             0x31000, 0x1000 )
	ROM_CONTINUE(             0x71000, 0x1000 )
	ROM_CONTINUE(             0x32000, 0x1000 )
	ROM_CONTINUE(             0x72000, 0x1000 )
	ROM_CONTINUE(             0x33000, 0x1000 )
	ROM_CONTINUE(             0x73000, 0x1000 )
	ROM_CONTINUE(             0x34000, 0x1000 )
	ROM_CONTINUE(             0x74000, 0x1000 )
	ROM_CONTINUE(             0x35000, 0x1000 )
	ROM_CONTINUE(             0x75000, 0x1000 )
	ROM_CONTINUE(             0x36000, 0x1000 )
	ROM_CONTINUE(             0x76000, 0x1000 )
	ROM_CONTINUE(             0x37000, 0x1000 )
	ROM_CONTINUE(             0x77000, 0x1000 )
	ROM_CONTINUE(             0x38000, 0x1000 )
	ROM_CONTINUE(             0x78000, 0x1000 )
	ROM_CONTINUE(             0x39000, 0x1000 )
	ROM_CONTINUE(             0x79000, 0x1000 )
	ROM_CONTINUE(             0x3a000, 0x1000 )
	ROM_CONTINUE(             0x7a000, 0x1000 )
	ROM_CONTINUE(             0x3b000, 0x1000 )
	ROM_CONTINUE(             0x7b000, 0x1000 )
	ROM_CONTINUE(             0x3c000, 0x1000 )
	ROM_CONTINUE(             0x7c000, 0x1000 )
	ROM_CONTINUE(             0x3d000, 0x1000 )
	ROM_CONTINUE(             0x7d000, 0x1000 )
	ROM_CONTINUE(             0x3e000, 0x1000 )
	ROM_CONTINUE(             0x7e000, 0x1000 )
	ROM_CONTINUE(             0x3f000, 0x1000 )
	ROM_CONTINUE(             0x7f000, 0x1000 )

	/* no proms supplied - using ones from original games */
	ROM_REGION( 0x05a0, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_RELOAD(               0x0020, 0x0020 )
	ROM_RELOAD(               0x0040, 0x0020 )
	ROM_LOAD( "prom.7f",      0x0060, 0x0020, CRC(872b42f3) SHA1(bbcd392ba3d2a5715e92fa0f7a7cf1e7e6e655a2) )
	ROM_LOAD( "puckren.7f",   0x0080, 0x0020, CRC(7b6aa3c9) SHA1(849911abdc26ccf2ad44e2c432e6219447b8bef3) )
	ROM_LOAD( "glob.7f",      0x00a0, 0x0020, CRC(1f617527) SHA1(448845cab63800a05fcb106897503d994377f78f) )
	ROM_LOAD( "7f.cpu",       0x00c0, 0x0020, CRC(7549a947) SHA1(4f2c3e7d6c38f0b9a90317f91feb3f86c9a0d0a5) )
	ROM_LOAD( "pacplus.7f",   0x00e0, 0x0020, CRC(063dd53a) SHA1(2e43b46ec3b101d1babab87cdaddfa944116ec06) )
	ROM_LOAD( "7f.rom",       0x0100, 0x0020, CRC(ec578b98) SHA1(196da49cc260f967ec5f01bc3c75b11077c85998) )
	ROM_LOAD( "82s126.4a",    0x0120, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) ) /* pacman */
	ROM_LOAD( "82s129.4a",    0x01a0, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) ) /* eyes */
	ROM_LOAD( "2s140.4a",     0x0220, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) ) /* maketrax, chicken */
	ROM_LOAD( "prom.4a",      0x02a0, 0x0100, CRC(0399f39f) SHA1(e98f08da4666cab44e01acb760a1bd2fc858bc0d) ) /* jumpshot */
	ROM_LOAD( "puckren.4a",   0x0320, 0x0100, CRC(64fe1dbf) SHA1(7f7541eb12148add79ad186a1da375c865d251e0) ) /* puckren */
	ROM_LOAD( "glob.4a",      0x03a0, 0x0100, CRC(28faa769) SHA1(7588889f3102d4e0ca7918f536556209b2490ea1) ) /* beastie */
	ROM_LOAD( "4a.cpu",       0x0420, 0x0100, CRC(5fdca536) SHA1(3a09b29374031aaa3722932aff974a467b3bb201) ) /* lizwiz */
	ROM_LOAD( "pacplus.4a",   0x04a0, 0x0100, CRC(e271a166) SHA1(cf006536215a7a1d488eebc1d8a2e2a8134ce1a6) ) /* pacplus */
	ROM_LOAD( "4a.rom",       0x0520, 0x0080, CRC(81a6b30f) SHA1(60c767fd536c325151a2b759fdbce4ba41e0c78f) ) /* shoot bull */
	ROM_IGNORE(                       0x0080 )
	/* There are a number of minor colour errors in Lizard Wizard, 2 of which are fixed below */
	ROM_FILL( 0x43d, 1, 4 )		/* dragon wing = red */
	ROM_FILL( 0x43e, 1, 6 )		/* dragon body = green */

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( madpac )
	ROM_REGION( 0x110000, "maincpu", 0 )
	ROM_LOAD( "madpac.cpu",   0x10000, 0x80000, CRC(1a769aea) SHA1(ea5b3b077f34485246643cfb8c74d073287f4359) )
	ROM_LOAD( "madpac.cp2",   0x90000, 0x80000, CRC(84889f93) SHA1(83aee22bc543af230f5c7f652756b189d6cf5399) )

	/* reorganise gfx so that all chars come first, followed by all sprites */
	ROM_REGION( 0x40000, "gfx1", 0 )
	ROM_LOAD( "madpac.gfx",   0x00000, 0x1000, CRC(6a1b231a) SHA1(67046022a179043f9c017b12ecadd9d5772705a9) )
	ROM_CONTINUE(             0x20000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x21000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x22000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x23000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x24000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x25000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x26000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x27000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x28000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x29000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x2a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x2b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x2c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x2d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x2e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x2f000, 0x1000 )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x30000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x31000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x32000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x33000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x34000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x35000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x36000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x37000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x38000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x39000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x3a000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x3b000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x3c000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x3d000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x3e000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )
	ROM_CONTINUE(             0x3f000, 0x1000 )
	/* Death Derby gfx are corrupt - using this rom to fix the problem */
	ROM_LOAD( "dderby.5e",    0x8000, 0x1000, CRC(7e2c0a53) SHA1(c3c62b32927ac3c5948faeff28d4c2148fe18d0b) )

	ROM_REGION( 0x41000, "proms", 0 )
	/* palette comes as banks of 16 - we change it to banks of 32, and remove banks 1 to 0xf (they are same as bank 0) */
	ROM_LOAD( "madpac.7f",    0x1000, 0x40000, CRC(63e60ac6) SHA1(03b7035eb0a7e62f21793661c09b449113562ebb) )
	ROM_COPY( "proms",        0x1000, 0x0000, 0x10 )	/* pacman and mspacman */
	ROM_COPY( "proms",        0x1100, 0x0020, 0x10 )	/* 10. balloon ace */
	ROM_COPY( "proms",        0x1110, 0x0040, 0x10 )	/* 11. beastie feastie */
	ROM_COPY( "proms",        0x1120, 0x0060, 0x10 )	/* 12. make trax */
	ROM_COPY( "proms",        0x1130, 0x0080, 0x10 )	/* 13. lizard wizard */
	ROM_COPY( "proms",        0x1140, 0x00a0, 0x10 )	/* 14. atlantic city action */
	ROM_COPY( "proms",        0x1150, 0x00c0, 0x10 )	/* 15. jump shot */
	ROM_COPY( "proms",        0x1160, 0x00e0, 0x10 )	/* 16. ladybug */
	ROM_COPY( "proms",        0x1170, 0x0100, 0x10 )	/* 17. zig zag */
	ROM_COPY( "proms",        0x1180, 0x0120, 0x10 )	/* 18. chicken */
	ROM_COPY( "proms",        0x1190, 0x0140, 0x10 )	/* 19. pacplus */
	ROM_COPY( "proms",        0x11a0, 0x0160, 0x10 )	/* 1A. naughty mouse */
	ROM_COPY( "proms",        0x11b0, 0x0180, 0x10 )	/* 1B. cycle battle */
	ROM_COPY( "proms",        0x11c0, 0x01a0, 0x10 )	/* 1C. eggor */
	ROM_COPY( "proms",        0x11d0, 0x01c0, 0x10 )	/* 1D. gorkans */
	ROM_COPY( "proms",        0x11e0, 0x01e0, 0x10 )	/* 1E. homercide */
	/* lookup table is already at banks of 128 */
	ROM_LOAD( "madpac.4a",    0x0280, 0x0800, CRC(16057dac) SHA1(8dfa69f3847140dd5fa8514e9bf49962f5f9f0c0) )
	ROM_COPY( "proms",        0x0280, 0x0200, 0x80 )	/* pacman and mspacman */

	PACMAN_SOUND_PROMS
ROM_END

/* E253 - no proms were supplied - using standard pacman ones.
	  While the roms of the 2 games are different, I cannot discern any difference in gameplay */
ROM_START( mspaceur )
	ROM_REGION( 0x18000, "maincpu", 0 )	
	ROM_LOAD( "mspaceur.cpu", 0x00000, 0x4000, CRC(03905a76) SHA1(1780ef598c6150ffa44bf467479670f7ca50d512) )
	ROM_CONTINUE(             0x10000, 0x4000 )
	/* fix some corrupt bytes that stop the game from working. This makes it identical to mspacmbg. */
	ROM_FILL(                 0x01a61, 1, 0x21 )
	ROM_FILL(                 0x018b5, 1, 0x4e )
	ROM_FILL(                 0x0197d, 1, 0xda )
	ROM_FILL(                 0x12c0d, 1, 0xeb )

	/* reorganise gfx so that all chars come first, followed by all sprites */
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "mspacj.gfx",   0x0000, 0x0800, CRC(8ee4a3b0) SHA1(01e3453c99f7a5d78ab083c49c650e898c0dd2ee) )
	ROM_CONTINUE(             0x1000, 0x0800 )
	ROM_CONTINUE(             0x0800, 0x0800 )
	ROM_CONTINUE(             0x1800, 0x0800 )
	ROM_IGNORE(		  0x2000 )

	PACMAN_PROMS
ROM_END

ROM_START( mschamp )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "pm4.bin",      0x10000, 0x10000, CRC(7d6b6303) SHA1(65ad72a9188422653c02a48c07ed2661e1e36961) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pm5.bin",      0x0000, 0x0800, CRC(7fe6b9e2) SHA1(bfd0d84c7ef909ae078d8f60340682b3ff230aa6) )
	ROM_CONTINUE(             0x1000, 0x0800 )
	ROM_CONTINUE(             0x0800, 0x0800 )
	ROM_CONTINUE(             0x1800, 0x0800 )

	PACMAN_PROMS
ROM_END

ROM_START( mschampx )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "pm4.bin",      0x10000, 0x10000, CRC(7d6b6303) SHA1(65ad72a9188422653c02a48c07ed2661e1e36961) )

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "mschampx.5e",  0x0000, 0x4000, CRC(b3b248f8) SHA1(5e8271b28c6a98e36b4685224bbc99154ae6ba5b) )
	ROM_LOAD( "mspacmnx.5f",  0x4000, 0x4000, CRC(6f625a9e) SHA1(d9d739ba412c9374b26006d99a345212ef97f868) )

	PACMAN_PROMS
ROM_END

ROM_START( multi10 )
	ROM_REGION( 0x90000, "maincpu", 0 )
	ROM_LOAD( "multi10.bin",  0x10000, 0x80000, CRC(9abb7d01) SHA1(44eb136f2ffe6475585b7558c4eddcf4ffaafe9e) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "multi10.gfx",  0x00000, 0x1000, CRC(072b216f) SHA1(cc3cef4f9804c1120611ed750c50593eac9b10d4) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	ROM_REGION( 0x0480, "proms", 0)
	ROM_LOAD( "multipac.7f",  0x0000, 0x0100, CRC(40a5c3d9) SHA1(c159fe5ab64b9eb51a28ccd051fae219d2125577) )
	ROM_COPY( "proms",        0x0000, 0x0040, 0x0020 )
	ROM_LOAD( "multipac.4a",  0x0080, 0x0400, CRC(562a66de) SHA1(1281a7585f833b9f93269b14369e1c8df09f2292) )
	ROM_COPY( "proms",        0x0180, 0x0100, 0x0080 )
	ROM_COPY( "proms",        0x0280, 0x0180, 0x0080 )
	ROM_COPY( "proms",        0x0380, 0x0200, 0x0080 )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( multi11 )
	ROM_REGION( 0x90000, "maincpu", 0 )   /* 8 banks of 64k for code */
	ROM_LOAD( "multi11.bin",  0x10000, 0x80000, CRC(0fa787fe) SHA1(400592360b2930c1fbbbe1adc0198be007b2dd63) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "multipac.gfx", 0x00000, 0x1000, CRC(012fb9ec) SHA1(e4b5647f27f5ddad0bb5580736d6acd3a86cdcf7) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	ROM_REGION( 0x0480, "proms", 0)
	ROM_LOAD( "multipac.7f",  0x0000, 0x0100, CRC(40a5c3d9) SHA1(c159fe5ab64b9eb51a28ccd051fae219d2125577) )
	ROM_COPY( "proms",        0x0000, 0x0040, 0x0020 )
	ROM_LOAD( "multipac.4a",  0x0080, 0x0400, CRC(562a66de) SHA1(1281a7585f833b9f93269b14369e1c8df09f2292) )
	ROM_COPY( "proms",        0x0180, 0x0100, 0x0080 )
	ROM_COPY( "proms",        0x0280, 0x0180, 0x0080 )
	ROM_COPY( "proms",        0x0380, 0x0200, 0x0080 )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( multi13 )
	ROM_REGION( 0x90000, "maincpu", 0 )   /* 8 banks of 64k for code */
	ROM_LOAD( "multi13.bin",  0x10000, 0x80000, CRC(0aca3ce9) SHA1(81dd22fd2e8137645fc6cdeb534b4e5c113704fe) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "multipac.gfx", 0x00000, 0x1000, CRC(012fb9ec) SHA1(e4b5647f27f5ddad0bb5580736d6acd3a86cdcf7) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	ROM_REGION( 0x0480, "proms", 0)
	ROM_LOAD( "multipac.7f",  0x0000, 0x0100, CRC(40a5c3d9) SHA1(c159fe5ab64b9eb51a28ccd051fae219d2125577) )
	ROM_COPY( "proms",        0x0000, 0x0040, 0x0020 )
	ROM_LOAD( "multipac.4a",  0x0080, 0x0400, CRC(562a66de) SHA1(1281a7585f833b9f93269b14369e1c8df09f2292) )
	ROM_COPY( "proms",        0x0180, 0x0100, 0x0080 )
	ROM_COPY( "proms",        0x0280, 0x0180, 0x0080 )
	ROM_COPY( "proms",        0x0380, 0x0200, 0x0080 )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( multi14 )
	ROM_REGION( 0x90000, "maincpu", 0 )   /* 8 banks of 64k for code */
	ROM_LOAD( "multi14.bin",  0x10000, 0x80000, CRC(23de184d) SHA1(e466a1c46091a33a3f50747455abb8618a23f824) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "multipac.gfx", 0x00000, 0x1000, CRC(012fb9ec) SHA1(e4b5647f27f5ddad0bb5580736d6acd3a86cdcf7) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	ROM_REGION( 0x0480, "proms", 0)
	ROM_LOAD( "multipac.7f",  0x0000, 0x0100, CRC(40a5c3d9) SHA1(c159fe5ab64b9eb51a28ccd051fae219d2125577) )
	ROM_COPY( "proms",        0x0000, 0x0040, 0x0020 )
	ROM_LOAD( "multipac.4a",  0x0080, 0x0400, CRC(562a66de) SHA1(1281a7585f833b9f93269b14369e1c8df09f2292) )
	ROM_COPY( "proms",        0x0180, 0x0100, 0x0080 )
	ROM_COPY( "proms",        0x0280, 0x0180, 0x0080 )
	ROM_COPY( "proms",        0x0380, 0x0200, 0x0080 )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( multi15 )
	ROM_REGION( 0x90000, "maincpu", 0 )   /* 8 banks of 64k for code */
	ROM_LOAD( "multi15.bin",  0x10000, 0x80000, CRC(eb181a29) SHA1(14a44730d36c25c76ae68cbfd51febda9d8d1dfa) )

	/* reorganise gfx so that all chars come first, followed by all sprites */
	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "multipac.gfx", 0x00000, 0x1000, CRC(012fb9ec) SHA1(e4b5647f27f5ddad0bb5580736d6acd3a86cdcf7) )
	ROM_CONTINUE(             0x10000, 0x1000 )
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x11000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x12000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x13000, 0x1000 )
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x14000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )
	ROM_CONTINUE(             0x15000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )
	ROM_CONTINUE(             0x16000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x17000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x18000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )
	ROM_CONTINUE(             0x19000, 0x1000 )
	ROM_CONTINUE(             0x0a000, 0x1000 )
	ROM_CONTINUE(             0x1a000, 0x1000 )
	ROM_CONTINUE(             0x0b000, 0x1000 )
	ROM_CONTINUE(             0x1b000, 0x1000 )
	ROM_CONTINUE(             0x0c000, 0x1000 )
	ROM_CONTINUE(             0x1c000, 0x1000 )
	ROM_CONTINUE(             0x0d000, 0x1000 )
	ROM_CONTINUE(             0x1d000, 0x1000 )
	ROM_CONTINUE(             0x0e000, 0x1000 )
	ROM_CONTINUE(             0x1e000, 0x1000 )
	ROM_CONTINUE(             0x0f000, 0x1000 )
	ROM_CONTINUE(             0x1f000, 0x1000 )

	ROM_REGION( 0x0480, "proms", 0)
	/* this rom has 4 banks of (the 16 colours in the palette + 16 blanks) */
	ROM_LOAD( "multipac.7f",  0x0000, 0x0100, CRC(40a5c3d9) SHA1(c159fe5ab64b9eb51a28ccd051fae219d2125577) )
	ROM_COPY( "proms",        0x0000, 0x0040, 0x0020 )	/* crush roller */
	/* this rom has 4 banks of (the 128 lookup colour codes + 128 blanks) */
	ROM_LOAD( "multipac.4a",  0x0080, 0x0400, CRC(562a66de) SHA1(1281a7585f833b9f93269b14369e1c8df09f2292) )
	/* we move the tables down, to remove the blanks */
	ROM_COPY( "proms",        0x0180, 0x0100, 0x0080 )
	ROM_COPY( "proms",        0x0280, 0x0180, 0x0080 )
	ROM_COPY( "proms",        0x0380, 0x0200, 0x0080 )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( pm4n1 )
	ROM_REGION( 0x34000, "maincpu", 0 )	
	ROM_LOAD( "pm4n1.cpu",    0x10000, 0x24000, CRC(248f3153) SHA1(c36fb95c38619ec4947017a686f7d5feb643d269) )
	ROM_FILL(0x303fe, 1, 0xcd)
	ROM_FILL(0x303ff, 1, 0xa1)	/* These bytes allow Pacman to start up */
	ROM_FILL(0x30400, 1, 0x2b)

	ROM_REGION( 0x0a000, "gfx1", 0 )
	ROM_LOAD( "96in1a.gfx",   0x00000, 0x1000, CRC(f4ce2dca) SHA1(b0515ee98766225d4d4a1368949942324703e865) )
	ROM_CONTINUE(             0x05000, 0x1000 )	/* piranha, don't want */
	ROM_CONTINUE(             0x00000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )	/* menu */
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )	/* fast pacman */
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )	/* pacman */
	/* 4 dummy reads because rom_ignore doesn't work properly */
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )	/* mspacman */
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )	/* fast mspacman */
	ROM_IGNORE( 0x10000)

	PACMAN_PROMS
ROM_END

ROM_START( pm4n1a )
	ROM_REGION( 0x34000, "maincpu", 0 )	
	ROM_LOAD( "pm4n1a.cpu",   0x10000, 0x24000, CRC(cdc319bb) SHA1(0f7b9956a67e46d5f3cb0bd84c350662668fd02d) )

	ROM_REGION( 0x0a000, "gfx1", 0 )
	ROM_LOAD( "96in1a.gfx",   0x00000, 0x1000, CRC(f4ce2dca) SHA1(b0515ee98766225d4d4a1368949942324703e865) )
	ROM_CONTINUE(             0x05000, 0x1000 )	/* piranha, don't want */
	ROM_CONTINUE(             0x00000, 0x1000 )
	ROM_CONTINUE(             0x05000, 0x1000 )	/* menu */
	ROM_CONTINUE(             0x01000, 0x1000 )
	ROM_CONTINUE(             0x06000, 0x1000 )	/* fast pacman */
	ROM_CONTINUE(             0x04000, 0x1000 )
	ROM_CONTINUE(             0x09000, 0x1000 )	/* pacman */
	/* 4 dummy reads because rom_ignore doesn't work properly */
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )
	ROM_CONTINUE(             0x02000, 0x1000 )
	ROM_CONTINUE(             0x07000, 0x1000 )	/* mspacman */
	ROM_CONTINUE(             0x03000, 0x1000 )
	ROM_CONTINUE(             0x08000, 0x1000 )	/* fast mspacman */
	ROM_IGNORE( 0x10000)

	PACMAN_PROMS
ROM_END

ROM_START( pm4n1b )
	ROM_REGION( 0x40000, "maincpu", 0 )	
	ROM_LOAD( "pm4n1b.cpu",   0x10000, 0x30000, CRC(31ea8134) SHA1(61bd5589ed1d304cfef90ccf9af95f2a6743782e) )
	/* Alternative working rom */
	//ROM_LOAD( "pm4n1ba.cpu",   0x10000, 0x24000, CRC(2709642c) SHA1(5e2984a9ff5db416ee065651765bf05c070ca052) )

	ROM_REGION( 0xa000, "gfx1", 0 )
	ROM_LOAD( "pm4n1b.gfx",   0x0000, 0x1000, CRC(949a50a2) SHA1(2b661e584b35740ce8b530f51485865a620354e6) )
	ROM_CONTINUE(             0x5000, 0x1000 )	/* menu */
	ROM_CONTINUE(             0x2000, 0x1000 )
	ROM_CONTINUE(             0x7000, 0x1000 )	/* mspacman */
	ROM_COPY( "gfx1", 0x0000, 0x1000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x6000, 0x1000 )	/* copy menu to fast pacman */
	ROM_COPY( "gfx1", 0x0000, 0x4000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x9000, 0x1000 )	/* copy menu to pacman */
	ROM_COPY( "gfx1", 0x2000, 0x3000, 0x1000 )
	ROM_COPY( "gfx1", 0x7000, 0x8000, 0x1000 )	/* copy mspacman to fast mspacman */

	PACMAN_PROMS
ROM_END

ROM_START( pm4n1c )
	ROM_REGION( 0x38000, "maincpu", 0 )	
	ROM_LOAD( "pm4n1c.cpu",   0x10000, 0x28000, CRC(48cafb4e) SHA1(8ff42c3524020f5f599f8f3c76f279b1af9983a9) )

	ROM_REGION( 0xa000, "gfx1", 0 )
	ROM_LOAD( "pm4n1b.gfx",   0x0000, 0x1000, CRC(949a50a2) SHA1(2b661e584b35740ce8b530f51485865a620354e6) )
	ROM_CONTINUE(             0x5000, 0x1000 )	/* menu */
	ROM_CONTINUE(             0x2000, 0x1000 )
	ROM_CONTINUE(             0x7000, 0x1000 )	/* mspacman */
	ROM_COPY( "gfx1", 0x0000, 0x1000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x6000, 0x1000 )	/* copy menu to fast pacman */
	ROM_COPY( "gfx1", 0x0000, 0x4000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x9000, 0x1000 )	/* copy menu to pacman */
	ROM_COPY( "gfx1", 0x2000, 0x3000, 0x1000 )
	ROM_COPY( "gfx1", 0x7000, 0x8000, 0x1000 )	/* copy mspacman to fast mspacman */

	PACMAN_PROMS
ROM_END

ROM_START( pm4n1d )
	ROM_REGION( 0x30000, "maincpu", 0 )	
//	ROM_LOAD( "pm4n1d.cpu",   0x10000, 0x20000, CRC(28f95683) SHA1(297ff0fb2d12293830cef9a4cc77629d658a96f9) )
	ROM_LOAD( "pm4n1d.cpu",   0x10000, 0x10000, CRC(9bab691e) SHA1(d90ee5e5b345c9feabe5f629132bd5f2b83c9c4f) )

	ROM_REGION( 0xa000, "gfx1", 0 )
	ROM_LOAD( "pm4n1b.gfx",   0x0000, 0x1000, CRC(949a50a2) SHA1(2b661e584b35740ce8b530f51485865a620354e6) )
	ROM_CONTINUE(             0x5000, 0x1000 )	/* menu */
	ROM_CONTINUE(             0x2000, 0x1000 )
	ROM_CONTINUE(             0x7000, 0x1000 )	/* mspacman */
	ROM_COPY( "gfx1", 0x0000, 0x1000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x6000, 0x1000 )	/* copy menu to fast pacman */
	ROM_COPY( "gfx1", 0x0000, 0x4000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x9000, 0x1000 )	/* copy menu to pacman */
	ROM_COPY( "gfx1", 0x2000, 0x3000, 0x1000 )
	ROM_COPY( "gfx1", 0x7000, 0x8000, 0x1000 )	/* copy mspacman to fast mspacman */

	PACMAN_PROMS
ROM_END

ROM_START( pm4n1e )
	ROM_REGION( 0x30000, "maincpu", 0 )	
//	ROM_LOAD( "pm4n1e.cpu",   0x10000, 0x20000, CRC(82035513) SHA1(9d05e5a07b47a2668e497a66cd6ceb29bbcab659) )
	ROM_LOAD( "pm4n1e.cpu",   0x10000, 0x10000, CRC(5bbf374a) SHA1(426451a9cd4e8680ab19c2ce635435c0b43cba95) )

	ROM_REGION( 0xa000, "gfx1", 0 )
	ROM_LOAD( "pm4n1b.gfx",   0x0000, 0x1000, CRC(949a50a2) SHA1(2b661e584b35740ce8b530f51485865a620354e6) )
	ROM_CONTINUE(             0x5000, 0x1000 )	/* menu */
	ROM_CONTINUE(             0x2000, 0x1000 )
	ROM_CONTINUE(             0x7000, 0x1000 )	/* mspacman */
	ROM_COPY( "gfx1", 0x0000, 0x1000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x6000, 0x1000 )	/* copy menu to fast pacman */
	ROM_COPY( "gfx1", 0x0000, 0x4000, 0x1000 )
	ROM_COPY( "gfx1", 0x5000, 0x9000, 0x1000 )	/* copy menu to pacman */
	ROM_COPY( "gfx1", 0x2000, 0x3000, 0x1000 )
	ROM_COPY( "gfx1", 0x7000, 0x8000, 0x1000 )	/* copy mspacman to fast mspacman */

	PACMAN_PROMS
ROM_END


/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 1995, mschamp,  mspacman, mschamp,  mschamp,  driver_device, 0,        ROT90, "hack", "Ms. Pacman Champion Edition / Super Zola Pac Gal", MACHINE_SUPPORTS_SAVE )
GAME( 1995, mschampx, mspacmnx, mschampx, mschamp,  driver_device, 0,        ROT90, "hack", "Ms. Pacman Champion Edition / Super Zola Pac Gal (hires hack)", MACHINE_SUPPORTS_SAVE )

/* Dave Widel's Games - http://www.widel.com */

GAME( 2005, 96in1,    madpac,	96in1,    96in1,    pacman_state,  96in1,    ROT90, "David Widel", "96 in 1 v3 [h]", MACHINE_SUPPORTS_SAVE )
GAME( 2005, 96in1c,   madpac,	96in1,    96in1,    pacman_state,  96in1,    ROT90, "David Widel", "96 in 1 v1 [h]", MACHINE_SUPPORTS_SAVE )
GAME( 2005, 96in1a,   madpac,	96in1,    96in1,    pacman_state,  96in1,    ROT90, "David Widel", "96 in 1 v2 [h]", MACHINE_SUPPORTS_SAVE )
GAME( 2005, 96in1b,   madpac,   96in1b,   96in1,    pacman_state,  madpac,   ROT90, "David Widel", "96 in 1 v4 [h]", MACHINE_SUPPORTS_SAVE )
GAME( 2001, hackypac, madpac,   hackypac, pacman0,  pacman_state,  madpac,   ROT90, "David Widel", "Hacky Pac", MACHINE_SUPPORTS_SAVE )
GAME( 2005, madpac,   0,        madpac,   96in1,    pacman_state,  madpac,   ROT90, "David Widel", "Mad Pac [h]", MACHINE_SUPPORTS_SAVE )

/* Other Misc Hacks */

GAME( 1993, mspaceur, mspacman, mspaceur, mspacman, pacman_state,  mspaceur, ROT90, "ImpEuropeX Corp", "Ms. Pac-man", MACHINE_SUPPORTS_SAVE )
GAME( 1998, multi10,  multi15,	multipac, multipac, pacman_state,  multipac, ROT90, "Clay Cowgill", "Multipac 1.0", MACHINE_SUPPORTS_SAVE )
GAME( 1998, multi11,  multi15,	multipac, multipac, pacman_state,  multipac, ROT90, "Clay Cowgill", "Multipac 1.1", MACHINE_SUPPORTS_SAVE )
GAME( 1998, multi13,  multi15,	multipac, multipac, pacman_state,  multipac, ROT90, "Clay Cowgill", "Multipac 1.3", MACHINE_SUPPORTS_SAVE )
GAME( 1998, multi14,  multi15,	multipac, multipac, pacman_state,  multipac, ROT90, "Clay Cowgill", "Multipac 1.4", MACHINE_SUPPORTS_SAVE )
GAME( 1998, multi15,  0,	multipac, multipac, pacman_state,  multipac, ROT90, "Clay Cowgill", "Multipac 1.5", MACHINE_SUPPORTS_SAVE )
GAME( 2007, pm4n1,    puckman,  pm4n1,    pacman0,  pacman_state,  pm4n1,    ROT90, "Jason Souza", "Pacman 4in1 v1.0 [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2007, pm4n1a,   puckman,  pm4n1,    pacman0,  pacman_state,  pm4n1,    ROT90, "Jason Souza", "Pacman 4in1 v1.1 [c]", MACHINE_SUPPORTS_SAVE )
GAME( 2007, pm4n1b,   puckman,  pm4n1,    pacman0,  pacman_state,  pm4n1,    ROT90, "Jason Souza", "Pacman 4in1 v1.2 [c]", MACHINE_SUPPORTS_SAVE )	// www.souzaonline.com/Games/Hacks/4n1hack.htm
GAME( 2008, pm4n1c,   puckman,  pm4n1c,   pacman0,  pacman_state,  pm4n1,    ROT90, "Jason Souza", "Pacman 4in1 v2.3 [c][h]", MACHINE_SUPPORTS_SAVE )
GAME( 2009, pm4n1d,   puckman,  pm4n1d,   pacman0,  pacman_state,  pm4n1,    ROT90, "Jason Souza", "Pacman 4in1 v3.0 [c][h]", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 2010, pm4n1e,   puckman,  pm4n1d,   pacman0,  pacman_state,  pm4n1,    ROT90, "Jason Souza", "Pacman 4in1 v3.3 [c][h]", MACHINE_SUPPORTS_SAVE )
