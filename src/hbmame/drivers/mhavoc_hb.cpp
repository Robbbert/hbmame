// license:BSD-3-Clause
// copyright-holders:Mike Appolo
/***************************************************************************

Documentation for Major Havoc - look in MAME driver

***************************************************************************/

#include "emu.h"
#include "includes/mhavoc_hb.h"

#include "cpu/m6502/m6502.h"
#include "video/avgdvg.h"
#include "video/vector.h"
#include "machine/eeprompar.h"
#include "machine/rescap.h"
#include "machine/watchdog.h"
#include "screen.h"
#include "speaker.h"


/* Quad pokey hookup (based on schematics):
Address: 543210
         |||||\- pokey A0
         ||||\-- pokey A1
         |||\--- pokey A2
         ||\---- pokey chip number LSB
         |\----- pokey chip number MSB
         \------ pokey A3
*/
uint8_t mhavoc_hbmame::quad_pokeyn_r(offs_t offset)
{
	int pokey_num = (offset >> 3) & ~0x04;
	int control = (offset & 0x20) >> 2;
	int pokey_reg = (offset & 0x7) | control;

	return m_pokey[pokey_num]->read(pokey_reg);
}

void mhavoc_hbmame::quad_pokeyn_w(offs_t offset, uint8_t data)
{
	int pokey_num = (offset >> 3) & ~0x04;
	int control = (offset & 0x20) >> 2;
	int pokey_reg = (offset & 0x7) | control;

	m_pokey[pokey_num]->write(pokey_reg, data);
}


/*************************************
 *
 *  Alpha One: dual POKEY?
 *
 *************************************/
/* dual pokey hookup (presumably, based on the prototype code):
Address: 43210
         ||||\- pokey A0
         |||\-- pokey A1
         ||\--- pokey A2
         |\---- pokey chip number
         \----- pokey A3
*/
uint8_t mhavoc_hbmame::dual_pokey_r(offs_t offset)
{
	int pokey_num = (offset >> 3) & 0x01;
	int control = (offset & 0x10) >> 1;
	int pokey_reg = (offset & 0x7) | control;

	return m_pokey[pokey_num]->read(pokey_reg);
}


void mhavoc_hbmame::dual_pokey_w(offs_t offset, uint8_t data)
{
	int pokey_num = (offset >> 3) & 0x01;
	int control = (offset & 0x10) >> 1;
	int pokey_reg = (offset & 0x7) | control;

	m_pokey[pokey_num]->write(pokey_reg, data);
}


/*************************************
 *
 *  Alpha CPU memory handlers
 *
 *************************************/

void mhavoc_hbmame::alpha_map(address_map &map)
{
	map(0x0000, 0x01ff).ram();
	map(0x0200, 0x07ff).bankrw("bank1").share("zram0");
	map(0x0800, 0x09ff).ram();
	map(0x0a00, 0x0fff).bankrw("bank1").share("zram1");
	map(0x1000, 0x1000).r(FUNC(mhavoc_hbmame::mhavoc_gamma_r));                  // Gamma Read Port
	map(0x1200, 0x1200).portr("IN0").nopw();                                    // Alpha Input Port 0
	map(0x1400, 0x141f).ram().share("avg:colorram");                            // ColorRAM
	map(0x1600, 0x1600).w(FUNC(mhavoc_hbmame::mhavoc_out_0_w));                  // Control Signals
	map(0x1640, 0x1640).w("avg", FUNC(avg_device::go_w));                       // Vector Generator GO
	map(0x1680, 0x1680).w("watchdog", FUNC(watchdog_timer_device::reset_w));    // Watchdog Clear
	map(0x16c0, 0x16c0).w("avg", FUNC(avg_device::reset_w));                    // Vector Generator Reset
	map(0x1700, 0x1700).w(FUNC(mhavoc_hbmame::mhavoc_alpha_irq_ack_w));          // IRQ ack
	map(0x1740, 0x1740).w(FUNC(mhavoc_hbmame::mhavoc_rom_banksel_w));            // Program ROM Page Select
	map(0x1780, 0x1780).w(FUNC(mhavoc_hbmame::mhavoc_ram_banksel_w));            // Program RAM Page Select
	map(0x17c0, 0x17c0).w(FUNC(mhavoc_hbmame::mhavoc_gamma_w));                  // Gamma Communication Write Port
	map(0x1800, 0x1fff).ram();                                                  // Shared Beta RAM
	map(0x2000, 0x3fff).bankr("bank2");                                         // Paged Program ROM (32K)
	map(0x4000, 0x4fff).ram();                                                  // Vector Generator RAM
	map(0x5000, 0x5fff).rom().region("vectorrom", 0x0000);                      // Vector ROM
	map(0x6000, 0x6fff).rom().region("vectorrom", 0x1000).mirror(0x1000);
	map(0x8000, 0xffff).rom();                                                  // Program ROM (32K)
}



/*************************************
 *
 *  Gamma CPU memory handlers
 *
 *************************************/

void mhavoc_hbmame::gamma_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800);                                                                       // Program RAM (2K)
	map(0x2000, 0x203f).rw(FUNC(mhavoc_hbmame::quad_pokeyn_r), FUNC(mhavoc_hbmame::quad_pokeyn_w)).mirror(0x07c0);    // Quad Pokey read/write
	map(0x2800, 0x2800).portr("IN1").mirror(0x07ff);                                                                // Gamma Input Port
	map(0x3000, 0x3000).r(FUNC(mhavoc_hbmame::mhavoc_alpha_r)).mirror(0x07ff);                                       // Alpha Comm. Read Port
	map(0x3800, 0x3803).portr("DIAL").mirror(0x07fc);                                                               // Roller Controller Input
	map(0x4000, 0x4000).portr("DSW2").w(FUNC(mhavoc_hbmame::mhavoc_gamma_irq_ack_w)).mirror(0x07ff);                 // DSW at 8S, IRQ Acknowledge
	map(0x4800, 0x4800).w(FUNC(mhavoc_hbmame::mhavoc_out_1_w)).mirror(0x07ff);                                       // Coin Counters
	map(0x5000, 0x5000).w(FUNC(mhavoc_hbmame::mhavoc_alpha_w)).mirror(0x07ff);                                       // Alpha Comm. Write Port
	//map(0x5800, 0x5800).w(FUNC(mhavoc_hbmame::mhavocrv_speech_data_w)).mirror(0x06ff);                               // TMS5220 data write
	//map(0x5900, 0x5900).w(FUNC(mhavoc_hbmame::mhavocrv_speech_strobe_w)).mirror(0x06ff);                             // TMS5220 /WS strobe write
	map(0x6000, 0x61ff).rw("eeprom", FUNC(eeprom_parallel_28xx_device::read), FUNC(eeprom_parallel_28xx_device::write)).mirror(0x1e00); // EEROM
	map(0x8000, 0xbfff).rom().region("gamma", 0).mirror(0x4000);                                                    // Program ROM (16K)
}

READ_LINE_MEMBER(mhavoc_hbmame::clock_r)
{
	/* 2.4kHz (divide 2.5MHz by 1024) */
	return (m_alpha->total_cycles() & 0x400) ? 0 : 1;
}


/* 2008-08 FP:
   IN0 Bit 4 is tested in the second input test, but it's not clear its effect.
   According to the memory map at top it should be Diagnostic Step, but it's
   actually IN0 Bit 5 to have this function. I marked it as UNKNOWN for the moment */
static INPUT_PORTS_START( mhavoc )
	PORT_START("IN0")   /* alpha */
	/* Bits 7-6 = selected based on player_1 */
	/* Bits 5-4 = common */
	/* Bit 3 = Gamma rcvd flag */
	/* Bit 2 = Gamma xmtd flag */
	/* Bit 1 = 2.4kHz (divide 2.5MHz by 1024) */
	/* Bit 0 = Vector generator halt flag */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("avg", avg_device, done_r)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, clock_r)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, gamma_xmtd_r)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, gamma_rcvd_r)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Diag Step/Coin C") PORT_CODE(KEYCODE_F1)
	PORT_BIT( 0xc0, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(mhavoc_hbmame, coin_service_r)

	PORT_START("IN1")   /* gamma */
	/* Bits 7-2 = input switches */
	/* Bit 1 = Alpha rcvd flag */
	/* Bit 0 = Alpha xmtd flag */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, alpha_xmtd_r)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, alpha_rcvd_r)
	PORT_BIT( 0x0c, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 )

	PORT_START("DIAL")  /* gamma */
	PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_REVERSE

	PORT_START("DSW1")  /* DIP Switch at position 13/14S */
	PORT_DIPNAME( 0x01, 0x00, "Adaptive Difficulty" )   PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ))
	PORT_DIPSETTING(    0x00, DEF_STR( On ))
	PORT_DIPNAME( 0x02, 0x00, DEF_STR( Demo_Sounds ) )  PORT_DIPLOCATION("SW1:7")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ))
	PORT_DIPSETTING(    0x00, DEF_STR( On ))
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Bonus_Life ) )   PORT_DIPLOCATION("SW1:5,6")
	PORT_DIPSETTING(    0x0c, "50000")
	PORT_DIPSETTING(    0x00, "100000")
	PORT_DIPSETTING(    0x04, "200000")
	PORT_DIPSETTING(    0x08, DEF_STR( None ))
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Difficulty ) )   PORT_DIPLOCATION("SW1:3,4")
	PORT_DIPSETTING(    0x10, DEF_STR( Easy ))
	PORT_DIPSETTING(    0x00, DEF_STR( Medium ))
	PORT_DIPSETTING(    0x30, DEF_STR( Hard ))
	PORT_DIPSETTING(    0x20, "Demo")
	PORT_DIPNAME( 0xc0, 0x00, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:1,2")
	PORT_DIPSETTING(    0x00, "3 (2 in Free Play)")
	PORT_DIPSETTING(    0xc0, "4 (3 in Free Play)")
	PORT_DIPSETTING(    0x80, "5 (4 in Free Play)")
	PORT_DIPSETTING(    0x40, "6 (5 in Free Play)")

	PORT_START("DSW2")  /* DIP Switch at position 8S */
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Coinage ) )      PORT_DIPLOCATION("SW2:7,8")
	PORT_DIPSETTING(    0x02, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x0c, "Right Coin Mechanism" )  PORT_DIPLOCATION("SW2:5,6")
	PORT_DIPSETTING(    0x0c, "x1" )
	PORT_DIPSETTING(    0x08, "x4" )
	PORT_DIPSETTING(    0x04, "x5" )
	PORT_DIPSETTING(    0x00, "x6" )
	PORT_DIPNAME( 0x10, 0x10, "Left Coin Mechanism" )   PORT_DIPLOCATION("SW2:4")
	PORT_DIPSETTING(    0x10, "x1" )
	PORT_DIPSETTING(    0x00, "x2" )
	PORT_DIPNAME( 0xe0, 0xe0, "Bonus Credits" )         PORT_DIPLOCATION("SW2:1,2,3")
	PORT_DIPSETTING(    0x80, "2 each 4" )
	PORT_DIPSETTING(    0x40, "1 each 3" )
	PORT_DIPSETTING(    0xa0, "1 each 4" )
	PORT_DIPSETTING(    0x60, "1 each 5" )
	PORT_DIPSETTING(    0xe0, DEF_STR( None ) )

	PORT_START("COIN")      /* dummy for player_1 = 0 on alpha */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )      /* Left Coin Switch  */
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 )      /* Right Coin */

	PORT_START("SERVICE")   /* dummy for player_1 = 1 on alpha */
	PORT_DIPNAME( 0x01, 0x01, "Credit to start" )
	PORT_DIPSETTING(    0x01, "1" )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_SERVICE( 0x02, IP_ACTIVE_LOW )
INPUT_PORTS_END

static INPUT_PORTS_START( mhavocrv )
	PORT_INCLUDE( mhavoc )

	PORT_MODIFY("IN1")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("tms", tms5220_device, readyq_r)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
INPUT_PORTS_END


/*************************************
 *
 *  Machine drivers
 *
 *************************************/

void mhavoc_hbmame::mhavoc(machine_config &config)
{
	/* basic machine hardware */
	M6502(config, m_alpha, MHAVOC_CLOCK_2_5M);     /* 2.5 MHz */
	m_alpha->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::alpha_map);

	M6502(config, m_gamma, MHAVOC_CLOCK_1_25M);    /* 1.25 MHz */
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::gamma_map);

	EEPROM_2804(config, "eeprom");

	TIMER(config, "5k_timer").configure_periodic(FUNC(mhavoc_hbmame::mhavoc_cpu_irq_clock), attotime::from_hz(MHAVOC_CLOCK_5K));

	WATCHDOG_TIMER(config, "watchdog");

	/* video hardware */
	VECTOR(config, "vector");
	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_VECTOR));
	screen.set_refresh_hz(50);
	screen.set_size(400, 300);
	screen.set_visarea(0, 300, 0, 260);
	screen.set_screen_update("vector", FUNC(vector_device::screen_update));

	avg_device &avg(AVG_MHAVOC(config, "avg", 0));
	avg.set_vector("vector");
	avg.set_memory(m_alpha, AS_PROGRAM, 0x4000);

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	POKEY(config, m_pokey[0], MHAVOC_CLOCK_1_25M);
	m_pokey[0]->allpot_r().set_ioport("DSW1");
	m_pokey[0]->set_output_opamp(RES_K(1), CAP_U(0.001), 5.0);
	m_pokey[0]->add_route(ALL_OUTPUTS, "mono", 0.25);

	POKEY(config, m_pokey[1], MHAVOC_CLOCK_1_25M);
	m_pokey[1]->set_output_opamp(RES_K(1), CAP_U(0.001), 5.0);
	m_pokey[1]->add_route(ALL_OUTPUTS, "mono", 0.25);

	POKEY(config, m_pokey[2], MHAVOC_CLOCK_1_25M);
	m_pokey[2]->set_output_opamp(RES_K(1), CAP_U(0.001), 5.0);
	m_pokey[2]->add_route(ALL_OUTPUTS, "mono", 0.25);

	POKEY(config, m_pokey[3], MHAVOC_CLOCK_1_25M);
	m_pokey[3]->set_output_opamp(RES_K(1), CAP_U(0.001), 5.0);
	m_pokey[3]->add_route(ALL_OUTPUTS, "mono", 0.25);
}

void mhavoc_hbmame::mhavocrv(machine_config &config)
{
	mhavoc(config);

	TMS5220(config, m_tms, MHAVOC_CLOCK/2/9);
	m_tms->add_route(ALL_OUTPUTS, "mono", 1.0);
}


/*************************************
 *
 *  Major Havoc The Promised End
 *
 *************************************/

/*
 * Notes:
 * the R3 roms are supported as "mhavoc", the R2 roms (with a bug in gameplay)
 * are supported as "mhavoc2".
 * "Return to Vax" - Jess Askey's souped up version (errors on self test)
 * are supported as "mhavocrv".
 * Prototype is supported as "mhavocp"
 * Alpha one is a single-board prototype
 */

// license:BSD-3-Clause
// copyright-holders:Robbbert
// thanks-to: Jess M Askey (designer of mhavocrv and mhavocpex)


void mhavoc_hbmame::gammape_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800);                   /* Program RAM (2K) */
	map(0x2000, 0x203f).rw(FUNC(mhavoc_hbmame::quad_pokeyn_r), FUNC(mhavoc_hbmame::quad_pokeyn_w)).mirror(0x07C0); /* Quad Pokey read/write  */
	map(0x2800, 0x2800).portr("IN1").mirror(0x07ff);      /* Gamma Input Port */
	map(0x3000, 0x3000).r(FUNC(mhavoc_hbmame::mhavoc_alpha_r)).mirror(0x07ff);  /* Alpha Comm. Read Port */
	map(0x3800, 0x3803).portr("DIAL").mirror(0x07fc);     /* Roller Controller Input */
	map(0x4000, 0x4000).portr("DSW2").w(FUNC(mhavoc_hbmame::mhavoc_gamma_irq_ack_w)).mirror(0x07ff); /* DSW at 8S, IRQ Acknowledge */
	map(0x4800, 0x4800).w(FUNC(mhavoc_hbmame::mhavoc_out_1_w)).mirror(0x07ff); /* Coin Counters    */
	map(0x5000, 0x5000).w(FUNC(mhavoc_hbmame::mhavoc_alpha_w)).mirror(0x07ff); /* Alpha Comm. Write Port */
	map(0x5800, 0x5800).w(FUNC(mhavoc_hbmame::mhavocrv_speech_data_w)).mirror(0x06ff); /* TMS5220 data write */
	map(0x5900, 0x5900).w(FUNC(mhavoc_hbmame::mhavocrv_speech_strobe_w)).mirror(0x06ff); /* TMS5220 /WS strobe write */
	map(0x6000, 0x61ff).rw("eeprom", FUNC(eeprom_parallel_28xx_device::read), FUNC(eeprom_parallel_28xx_device::write)).mirror(0x1e00); /* EEROM */
	map(0x8000, 0xffff).rom();                   /* Program ROM (32K) */
}

//later mhavocpe versions ditched the old TMS5220 and used the OKIM6295
void mhavoc_hbmame::gammape2_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800);                   /* Program RAM (2K) */
	map(0x2000, 0x203f).rw(FUNC(mhavoc_hbmame::quad_pokeyn_r), FUNC(mhavoc_hbmame::quad_pokeyn_w)).mirror(0x07C0); /* Quad Pokey read/write  */
	map(0x2800, 0x2800).portr("IN1").mirror(0x07ff);      /* Gamma Input Port */
	map(0x3000, 0x3000).r(FUNC(mhavoc_hbmame::mhavoc_alpha_r)).mirror(0x07ff);  /* Alpha Comm. Read Port */
	map(0x3800, 0x3803).portr("DIAL").mirror(0x07fc);     /* Roller Controller Input */
	map(0x4000, 0x4000).portr("DSW2").w(FUNC(mhavoc_hbmame::mhavoc_gamma_irq_ack_w)).mirror(0x07ff); /* DSW at 8S, IRQ Acknowledge */
	map(0x4800, 0x4800).w(FUNC(mhavoc_hbmame::mhavoc_out_1_w)).mirror(0x07ff); /* Coin Counters    */
	map(0x5000, 0x5000).w(FUNC(mhavoc_hbmame::mhavoc_alpha_w)).mirror(0x07ff); /* Alpha Comm. Write Port */
	map(0x5900, 0x5900).w("oki", FUNC(okim6295_device::write));
	map(0x6000, 0x61ff).rw("eeprom", FUNC(eeprom_parallel_28xx_device::read), FUNC(eeprom_parallel_28xx_device::write)).mirror(0x1e00); /* EEROM */
	map(0x8000, 0xbfff).rom().mirror(0x4000); 
}

void mhavoc_hbmame::alphape_map(address_map &map)
{
	map(0x0000, 0x01ff).ram();
	map(0x0200, 0x07ff).bankrw("bank1").share("zram0");
	map(0x0800, 0x09ff).ram();
	map(0x0a00, 0x0fff).bankrw("bank1").share("zram1");
	map(0x1000, 0x1000).r(FUNC(mhavoc_hbmame::mhavoc_gamma_r));          /* Gamma Read Port */
	map(0x1200, 0x1200).portr("IN0").nopw();                            /* Alpha Input Port 0 */
	map(0x1400, 0x141f).ram().share("avg:colorram");                    /* ColorRAM */
	map(0x1600, 0x1600).w(FUNC(mhavoc_hbmame::mhavoc_out_0_w));          /* Control Signals */
	map(0x1640, 0x1640).w("avg", FUNC(avg_mhavoc_device::go_w));        /* Vector Generator GO */
	map(0x1680, 0x1680).w("watchdog", FUNC(watchdog_timer_device::reset_w));         /* Watchdog Clear */
	map(0x16c0, 0x16c0).w("avg", FUNC(avg_mhavoc_device::reset_w));     /* Vector Generator Reset */
	map(0x1700, 0x1700).w(FUNC(mhavoc_hbmame::mhavoc_alpha_irq_ack_w));  /* IRQ ack */
	//map(0x1740, 0x1740).w(FUNC(mhavoc_hbmame::mhavocpe_rom_banksel_w));     /* Program ROM Page Select */
	map(0x1740, 0x1740).lw8(NAME([this] (u8 data) { membank("bank2")->set_entry((data & 1) | ((data & 2)<<1) | ((data & 4)>>1)); })); /* Program ROM Page Select */
	map(0x1780, 0x1780).w(FUNC(mhavoc_hbmame::mhavoc_ram_banksel_w));    /* Program RAM Page Select */
	map(0x17c0, 0x17c0).w(FUNC(mhavoc_hbmame::mhavoc_gamma_w));          /* Gamma Communication Write Port */
	map(0x1800, 0x1fff).ram().share("comram");    						/* BETA COMRAM */
	map(0x2000, 0x3fff).bankr("bank2");                     			/* Paged Program ROM (32K) */
	map(0x4000, 0x4fff).ram();    										/* Vector Generator RAM */
	map(0x5000, 0x5fff).rom().region("vectorrom", 0x0000);              /* Vector ROM */
	map(0x6000, 0x6fff).rom().region("vectorrom", 0x1000).mirror(0x1000);	/* Vector ROM */
	//map(0x6000, 0x7fff).bankr("bank3");                     			/* Paged Vector ROM */
	map(0x8000, 0xffff).rom();                              			/* Program ROM (32K) */
}

void mhavoc_hbmame::betape_map(address_map &map)
{
	map(0x0000, 0x07ff).ram();
	map(0x4000, 0x47ff).ram().share("comram");
	map(0x8000, 0xffff).rom();                 				/* Program ROM (32K) */
}

//  membank("bank3")->set_entry(m_map);

void mhavoc_hbmame::mhavocpe(machine_config &config)
{
	mhavocrv(config);

	/* set updated maps */
	m_alpha->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::alphape_map);
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::gammape_map);
	/* create BETA */
	M6502(config, m_beta, MHAVOC_CLOCK_2_5M);     /* 2.5 MHz */
	m_beta->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::betape_map);
}

void mhavoc_hbmame::mhavocpe2(machine_config &config)
{
	mhavoc(config);

	/* set updated maps */
	m_alpha->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::alphape_map);
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::gammape2_map);
	/* create BETA */
	//M6502(config, m_beta, MHAVOC_CLOCK_2_5M);     /* 2.5 MHz */
	//m_beta->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::betape_map);
	/* Put OKI on GAMMA */
	OKIM6295(config, m_oki, OKI_CLOCK, okim6295_device::PIN7_HIGH);
	m_oki->add_route(ALL_OUTPUTS, "mono", 1.0);	
}


ROM_START( mhavocpex1 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex1.6kl",   0x00000, 0x2000, CRC(4c05b1a8) SHA1(89b524182fcfd966d6a7e3188235c957c451b8a9) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex1.1mn",   0x08000, 0x4000, CRC(3b691eff) SHA1(e8227d1458e3ed4d0e8444ec23f2c2d45a0d93b8) )
	ROM_LOAD( "mhavocpex1.1l",    0x0c000, 0x4000, CRC(fb53dae6) SHA1(08e9bd60e801778d3521d64817a10ba1ed74f4ff) )
	ROM_LOAD( "mhavocpex1.1q",    0x10000, 0x8000, CRC(660e3d57) SHA1(6eddf1335c536406080eab73f5501a202fb0583d) )
	ROM_LOAD( "mhavocpex1.1np",   0x18000, 0x8000, CRC(c1a70bad) SHA1(0b72b6817e2f00d2c001ac61ebd2cd42ff7785c9) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex1.6h",    0x0000, 0x4000, CRC(79fc58c0) SHA1(7b40dfb89bc4078e2bd6f89a570f2be9cca15df9) )
	ROM_LOAD( "mhavocpex1.6jk",   0x4000, 0x4000, CRC(dc78b802) SHA1(6b951982232de08d32d3a2d01814cc28f89d2120) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex1.9s",    0x8000, 0x8000, CRC(d42ee58e) SHA1(667aec3c3e93df3f8dedddb0db1188291e37630b) )

	ROM_REGION( 0x10000, "beta", ROMREGION_ERASEFF )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex2 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex2.6kl",  0x00000, 0x2000, CRC(3804822b) SHA1(78ca5ad3ac9ce9f6ce6ca497f70c7233adc52502) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex2.1mn",  0x08000, 0x4000, CRC(2b505cae) SHA1(1ca14c6c810a3dec964eae63e9941e411d3ec546) )
	ROM_LOAD( "mhavocpex2.1l",   0x0c000, 0x4000, CRC(8c078dcf) SHA1(b4681eb4930938a93e74c313ee1031ad1a71e198) )
	ROM_LOAD( "mhavocpex2.1q",   0x10000, 0x8000, CRC(7602fccd) SHA1(9073c5e2a70f23c7bab2927936ee5a22e2adb57a) )
	ROM_LOAD( "mhavocpex2.1np",  0x18000, 0x8000, CRC(bc6775e1) SHA1(a59a93d4414a0318e37c1e50ef139b6611e4233f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex2.6h",   0x0000, 0x4000, CRC(ec19097f) SHA1(261c957e5311ac5fd700949e04e9d4a6ef12c043) )
	ROM_LOAD( "mhavocpex2.6jk",  0x4000, 0x4000, CRC(2ac3f07b) SHA1(27d06ed9edf8cd227c7db7c28a7f7357da1260c6) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex2.9s",   0x8000, 0x8000, CRC(57ca8c09) SHA1(84ae19a9ff3b61c75f1fd147540a1e5fb42e6915) )

	ROM_REGION( 0x10000, "beta", ROMREGION_ERASEFF )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex3 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex3.6kl",  0x00000, 0x2000, CRC(0bebf550) SHA1(043dd185a40208de8f7952026f1645799eb3445f) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex3.1mn",  0x08000, 0x4000, CRC(b3a485e8) SHA1(47e20e02a69675d5df51e68d33bc407b441d621c) )
	ROM_LOAD( "mhavocpex3.1l",   0x0c000, 0x4000, CRC(baa2d31a) SHA1(d6337763e4d6cc86c95d2abf54955a7b413c9846) )
	ROM_LOAD( "mhavocpex3.1q",   0x10000, 0x8000, CRC(ebb47bb5) SHA1(e73ea9bde6c525b644824deb73a4caa3f9a7e28f) )
	ROM_LOAD( "mhavocpex3.1np",  0x18000, 0x8000, CRC(793b480d) SHA1(9a0398dad5b8634190d9432e1b94a786054b4e6e) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex3.6h",   0x0000, 0x4000, CRC(f1bcfaf2) SHA1(a70c3dd399c961f8a12dfc343c106d7e77e77a42) )
	ROM_LOAD( "mhavocpex3.6jk",  0x4000, 0x4000, CRC(8a8a67cf) SHA1(af5e64e0329015e3d2cc5292d671fe64cafd6463) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex3.9s",   0x8000, 0x8000, CRC(603ac7da) SHA1(8343cc8a388673436df9b5ad2894136943c676da) )

	ROM_REGION( 0x10000, "beta", ROMREGION_ERASEFF )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex4 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex4.6kl",  0x00000, 0x2000, CRC(75ab6d18) SHA1(874f4a15532f257480c8f6ca0cf9b3cd7ea39908) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex4.1mn",  0x08000, 0x4000, CRC(120b00f0) SHA1(b8546c81ecc8e16f5e0afb7ce9b941623d862d31) )
	ROM_LOAD( "mhavocpex4.1l",   0x0c000, 0x4000, CRC(ffcb7a73) SHA1(ec284426c381da3c15ec50c3bbbdf38fb15efcb2) )
	ROM_LOAD( "mhavocpex4.1q",   0x10000, 0x8000, CRC(dc85e7b3) SHA1(c9cbe410915686f470c033cc5ccacf76f81c46ba) )
	ROM_LOAD( "mhavocpex4.1np",  0x18000, 0x8000, CRC(134d84f9) SHA1(4933dda173c9abc813edf6cd0bcef24977b6fe1a) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex4.6h",   0x0000, 0x4000, CRC(df87c581) SHA1(bc13cd3ddf4a1e8c93d452eafafd295c37e43057) )
	ROM_LOAD( "mhavocpex4.6jk",  0x4000, 0x4000, CRC(8b1f8b3d) SHA1(c0825a1d7b49eb2cf169361563cd0f8e6df5a159) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex4.9s",   0x8000, 0x8000, CRC(e91ba3e8) SHA1(6283e5365cda8fe9706c1cd15ac7bd3978e029bc) )

	ROM_REGION( 0x10000, "beta", ROMREGION_ERASEFF )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex5 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex5.6kl",  0x00000, 0x2000, CRC(025ae0de) SHA1(5390dee62e5499c8767b4fc34f38fef49579d05b) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex5.1mn",  0x08000, 0x4000, CRC(b2b3067e) SHA1(209a5d6d53825164ba7c92cff67e69571c6a0b32) )
	ROM_LOAD( "mhavocpex5.1l",   0x0c000, 0x4000, CRC(367c0028) SHA1(e2f153d4c1b3ba19b48a34d1bd776318499513c9) )
	ROM_LOAD( "mhavocpex5.1q",   0x10000, 0x8000, CRC(04e4476b) SHA1(b9ac489e2b3ea8c04e6c907b3dbf6dfdbba976af) )
	ROM_LOAD( "mhavocpex5.1np",  0x18000, 0x8000, CRC(8c8ca853) SHA1(12d1dbd3f76e85021ba2226defd194de8543c9ac) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex5.6h",   0x0000, 0x4000, CRC(ea89c415) SHA1(f91ae8b9d1942e4ee14eaf9c1d0e687f2039c89d) )
	ROM_LOAD( "mhavocpex5.6jk",  0x4000, 0x4000, CRC(c56e754a) SHA1(6f02782aa14e10f6a9a2f5065d3ec9e2d9e07d5e) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex5.9s",   0x8000, 0x8000, CRC(8abffddd) SHA1(4cea72b569de302feac53b4e4a861887904c6bbe) )

	ROM_REGION( 0x10000, "beta", ROMREGION_ERASEFF )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex6 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex6.6kl",  0x00000, 0x2000, CRC(93688346) SHA1(f17b44cb196216c01ac75b0472791fda6995bd18) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex6.1mn",  0x08000, 0x4000, CRC(dd079cdd) SHA1(c9a7d42f687b99859f95b397618a31461abc3aa6) )
	ROM_LOAD( "mhavocpex6.1l",   0x0c000, 0x4000, CRC(d0c7ed14) SHA1(a07fbcd2b90b34d9cb9bff25c656099a9eda501c) )
	ROM_LOAD( "mhavocpex6.1q",   0x10000, 0x8000, CRC(897fbf17) SHA1(e3e7f3ac2204d422568b7dfa4f5a577c79d50ed5) )
	ROM_LOAD( "mhavocpex6.1np",  0x18000, 0x8000, CRC(3a215adc) SHA1(67684860244ec50b368f17179b1eeeb7b92478b7) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex6.6h",   0x0000, 0x4000, CRC(ef9f8321) SHA1(3c590c78664d6e121c47e11046fb3874ef0e1e90) )
	ROM_LOAD( "mhavocpex6.6jk",  0x4000, 0x4000, CRC(3dd7bec9) SHA1(aaa5e48d45519624017a9ec04b3e4b721e01ff2a) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex6.9s",   0x8000, 0x8000, CRC(5e1b2ada) SHA1(8fc2334e090b5f9b2bd97ab792aa69f170643752) )

	ROM_REGION( 0x10000, "beta", 0 )
	ROM_LOAD( "mhavocpex6.1bc",  0x8000, 0x4000, CRC(54618f8d) SHA1(f20b1fca139eb1b0fd882c5a25b7c5b31c48d9b4) )
	ROM_LOAD( "mhavocpex6.1d",   0xc000, 0x4000, CRC(2f397fbd) SHA1(c714f0cb4cc8958e5bf0e1533f1790785e235299) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex7 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex7.6kl", 0x00000, 0x2000, CRC(ef7843f8) SHA1(bfd096b0db69188f9032578839c9bd91367f7aae) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex7.1mn",  0x08000, 0x4000, CRC(f043dcaf) SHA1(3f20c20ea601190a52dac50d3281ed09a18a4948) )
	ROM_LOAD( "mhavocpex7.1l",   0x0c000, 0x4000, CRC(da963eab) SHA1(b10872898b12d79cd6cf0651ec61962315b5aff9) )
	//ROM_LOAD( "mhavocpex7.1q",   0x10000, 0x8000, CRC(39c5b2d0) SHA1(9cd68ad1b4a7f68c1c6bdcbc60b3cf5a7a289ea7) )
	ROM_LOAD( "mhavocpex7.1q",   0x10000, 0x8000, CRC(57499813) SHA1(ff53f65fc34c5f4b8d4af78776fb242ac59d536a) )
	ROM_LOAD( "mhavocpex7.1np",  0x18000, 0x8000, CRC(0d362096) SHA1(dccc0509aac7003fee11bf7eeabff81360c226a8) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex7.6h",   0x0000, 0x4000, CRC(4129d0e9) SHA1(6fd168de59d684f216a0a0ac45aed0782b18f900) )
	ROM_LOAD( "mhavocpex7.6jk",  0x4000, 0x4000, CRC(9361ed01) SHA1(ea07f8430f7e97e0c62d8df2963bb7b66a45b698) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex7.9s",   0x8000, 0x4000, CRC(9703c51f) SHA1(c010021e0dabc568af2eb1d516726ac21a1c9c98) )

	//ROM_REGION( 0x10000, "beta", 0 )
	//ROM_LOAD( "mhavocpe.1bc",   0x8000, 0x4000, CRC(1b31ae10) SHA1(247726f9ebe4892edc7c9577a7fffc93057fafe5) )
	//ROM_LOAD( "mhavocpe.1d",   0xc000, 0x4000, CRC(e86d6226) SHA1(45e00d62c424cb60c30b63ee2de57a465989d8ee) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )   /* 256k for ADPCM samples */
	ROM_LOAD( "mhavocpex7.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

ROM_START( mhavocpex7de )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex7de.6kl", 0x00000, 0x2000, CRC(6bc3240f) SHA1(6152a5015a99142d77203fa70f96160ffecd460c) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex7de.1mn",  0x08000, 0x4000, CRC(30fc2132) SHA1(0d07fab70e9e2d606a825837436c242706fa690a) )
	ROM_LOAD( "mhavocpex7de.1l",   0x0c000, 0x4000, CRC(245100c8) SHA1(90333f9ad7b1c4422bc0b68a9a4d59edd9596d65) )
	ROM_LOAD( "mhavocpex7de.1q",   0x10000, 0x8000, CRC(8fb86a78) SHA1(53b3ec55962c7a6f615838005b56de7458147f46) )
	ROM_LOAD( "mhavocpex7de.1np",  0x18000, 0x8000, CRC(c8a86dfb) SHA1(8e3257068ae52095b8f15012a46cc99d9e28242f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex7de.6h",   0x0000, 0x4000, CRC(8df8c6e5) SHA1(b5fadd531f040ef01c9c39810bfe81b800a32f78) )
	ROM_LOAD( "mhavocpex7de.6jk",  0x4000, 0x4000, CRC(5fb0fb0d) SHA1(70f72856cb8ade503b00e43c12ed52bbabbd7ca6) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex7de.9s",   0x8000, 0x4000, CRC(722ade2e) SHA1(212cb5c9201e41e08eada3aec3c77ab8728135b1) )

	//ROM_REGION( 0x10000, "beta", 0 )
	//ROM_LOAD( "mhavocpe.1bc",   0x8000, 0x4000, CRC(1b31ae10) SHA1(247726f9ebe4892edc7c9577a7fffc93057fafe5) )
	//ROM_LOAD( "mhavocpe.1d",   0xc000, 0x4000, CRC(e86d6226) SHA1(45e00d62c424cb60c30b63ee2de57a465989d8ee) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )   /* 256k for ADPCM samples */
	ROM_LOAD( "mhavocpex7.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

ROM_START( mhavocpex7fr )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex7fr.6kl", 0x00000, 0x2000, CRC(3d7f8a57) SHA1(cc8cd99003b56805c6eab9c4d26c17c485966d17) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex7fr.1mn",  0x08000, 0x4000, CRC(ad00b995) SHA1(f210a934be809b272bbbd7b3a459df6614e8a8f9) )
	ROM_LOAD( "mhavocpex7fr.1l",   0x0c000, 0x4000, CRC(c01b01fa) SHA1(9965da7863da933a1be1ce32e3db578ce4e54bb4) )
	ROM_LOAD( "mhavocpex7fr.1q",   0x10000, 0x8000, CRC(81ac4eda) SHA1(eeab82e1bad953a72300986c9dcdc02f3113cb06) )
	ROM_LOAD( "mhavocpex7fr.1np",  0x18000, 0x8000, CRC(6cf82736) SHA1(4a17ebd9aac863ce34ff5ef2d139cf319c373502) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex7fr.6h",   0x0000, 0x4000, CRC(03fafab0) SHA1(5a27c5cf745d51e76c8af63c7880b0337e1b97f5) )
	ROM_LOAD( "mhavocpex7fr.6jk",  0x4000, 0x4000, CRC(d1b2c758) SHA1(047c328855a6fa930e172b77798d0373027ac0ce) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex7fr.9s",   0x8000, 0x4000, CRC(8620f53c) SHA1(96a05e1410d2d5fa55a35bf88d057101aede6681) )

	//ROM_REGION( 0x10000, "beta", 0 )
	//ROM_LOAD( "mhavocpe.1bc",   0x8000, 0x4000, CRC(1b31ae10) SHA1(247726f9ebe4892edc7c9577a7fffc93057fafe5) )
	//ROM_LOAD( "mhavocpe.1d",   0xc000, 0x4000, CRC(e86d6226) SHA1(45e00d62c424cb60c30b63ee2de57a465989d8ee) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )   /* 256k for ADPCM samples */
	ROM_LOAD( "mhavocpex7.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

ROM_START( mhavocpex81 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpex81.6kl", 0x00000, 0x2000, CRC(fb8a6db9) SHA1(1404f403e19d1cdfebee04f2363698b87884dda1) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpex81.1mn",  0x08000, 0x4000, CRC(d9bcf35c) SHA1(623a4f1f8675f9871c6c0afdab54112dfa052d92) )
	ROM_LOAD( "mhavocpex81.1l",   0x0c000, 0x4000, CRC(8d02e0a3) SHA1(b3db473918f7ec1eb2e3e1c30861ab75405cbb96) )
	ROM_LOAD( "mhavocpex81.1q",   0x10000, 0x8000, CRC(c26e9f60) SHA1(b6bd5eb67199009e09e21c667efbf2e0a3ec4d09) )
	ROM_LOAD( "mhavocpex81.1np",  0x18000, 0x8000, CRC(974da802) SHA1(9dcb9be35b13c135ebf92008ddfa3dbaa2271aef) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpex81.6h",   0x0000, 0x4000, CRC(3e3141a4) SHA1(6ac03f22bb5947f4254cfc33e0f53e12d0ada913) )
	ROM_LOAD( "mhavocpex81.6jk",  0x4000, 0x4000, CRC(ec797c4c) SHA1(0cfc375ba9325bbcaf1602df5f89076152a47fc3) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpex81.9s",   0x8000, 0x4000, CRC(a7da11ba) SHA1(faa8b1bbcaca885498106e0288cd35d8883eea95) )

	//ROM_REGION( 0x10000, "beta", 0 )
	//ROM_LOAD( "mhavocpe.1bc",   0x8000, 0x4000, CRC(767311a5) SHA1(a44ffdca094633eed26426c801c91a754ad0859c) )
	//ROM_LOAD( "mhavocpe.1d",   0xc000, 0x4000, CRC(3e251843) SHA1(232ab0f19d7ef2c32fad34ec379bb80e5a8427b2) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )   /* 256k for ADPCM samples */
	ROM_LOAD( "mhavocpex7.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

#if 0
//Placeholder Driver for upcoming final release
ROM_START( mhavocpe )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhavocpe.6kl",  0x00000, 0x2000, CRC(93688346) SHA1(f17b44cb196216c01ac75b0472791fda6995bd18) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhavocpe.1mn",  0x08000, 0x4000, CRC(dd079cdd) SHA1(c9a7d42f687b99859f95b397618a31461abc3aa6) )
	ROM_LOAD( "mhavocpe.1l",   0x0c000, 0x4000, CRC(d0c7ed14) SHA1(a07fbcd2b90b34d9cb9bff25c656099a9eda501c) )
	ROM_LOAD( "mhavocpe.1q",   0x10000, 0x8000, CRC(897fbf17) SHA1(e3e7f3ac2204d422568b7dfa4f5a577c79d50ed5) )
	ROM_LOAD( "mhavocpe.1np",  0x18000, 0x8000, CRC(3a215adc) SHA1(67684860244ec50b368f17179b1eeeb7b92478b7) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhavocpe.6h",   0x0000, 0x4000, CRC(ef9f8321) SHA1(3c590c78664d6e121c47e11046fb3874ef0e1e90) )
	ROM_LOAD( "mhavocpe.6jk",  0x4000, 0x4000, CRC(3dd7bec9) SHA1(aaa5e48d45519624017a9ec04b3e4b721e01ff2a) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhavocpe.9s",   0x8000, 0x4000, CRC(5e1b2ada) SHA1(8fc2334e090b5f9b2bd97ab792aa69f170643752) )

	//ROM_REGION( 0x10000, "beta", 0 )
	//ROM_LOAD( "mhavocpe.1bc",   0x8000, 0x4000, CRC(54618f8d) SHA1(f20b1fca139eb1b0fd882c5a25b7c5b31c48d9b4) )
	//ROM_LOAD( "mhavocpe.1d",   0xc000, 0x4000, CRC(2f397fbd) SHA1(c714f0cb4cc8958e5bf0e1533f1790785e235299) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )   /* 256k for ADPCM samples */
	ROM_LOAD( "mhavocpe.x1", 0x000000, 0x040000, CRC(4dc14fe8) SHA1(c7cc00715f6687ced9d69ec793d6e9d4bc1b5287) )
ROM_END
#endif


GAME( 2018, mhavocpex1,   mhavoc, mhavocpe, mhavocrv, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (ALPHA v0.21)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mhavocpex2,   mhavoc, mhavocpe, mhavocrv, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (ALPHA v0.50)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex3,   mhavoc, mhavocpe, mhavocrv, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (ALPHA v0.52)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex4,   mhavoc, mhavocpe, mhavocrv, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.67)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex5,   mhavoc, mhavocpe, mhavocrv, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.72)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex6,   mhavoc, mhavocpe, mhavocrv, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.75)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex7,   mhavoc, mhavocpe2, mhavoc, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.78)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex7fr, mhavoc, mhavocpe2, mhavoc, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.78) FRENCH", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex7de, mhavoc, mhavocpe2, mhavoc, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.78) GERMAN", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpex81,  mhavoc, mhavocpe2, mhavoc, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.81)", MACHINE_SUPPORTS_SAVE )
//GAME( 2021, mhavocpe, mhavoc, mhavocpe, mhavoc, mhavoc_hbmame, init_mhavocrv, ROT0, "JMA", "Major Havoc - The Promised End (NOT YET RELEASED)", MACHINE_SUPPORTS_SAVE )
