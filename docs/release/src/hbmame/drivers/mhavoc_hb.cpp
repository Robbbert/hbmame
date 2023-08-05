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


void mhavoc_hbmame::mhavocpe_out_0_w(uint8_t data)
{
	/* Bit 7 = Invert Y -- for mhavocpe only */
	/* Bit 6 = Invert X -- for mhavocpe only */
	m_avg->set_flip_x(data & 0x40);
	m_avg->set_flip_y(data & 0x80);

	logerror("\tWrite Outputs: %02X\n", data);
	mhavoc_out_0_w(data);
}

/*************************************
 *
 *  Alpha CPU memory handlers
 *
 *************************************/

void mhavoc_hbmame::alphape_map(address_map &map)
{
	map(0x0000, 0x01ff).ram();
	map(0x0200, 0x07ff).bankrw("bank1").share("zram0");
	map(0x0800, 0x09ff).ram();
	map(0x0a00, 0x0fff).bankrw("bank1").share("zram1");
	map(0x1000, 0x1000).r(FUNC(mhavoc_hbmame::mhavoc_gamma_r));         /* Gamma Read Port */
	map(0x1200, 0x1200).portr("IN0").nopw();                            /* Alpha Input Port 0 */
	map(0x1400, 0x141f).ram().share("avg:colorram");                    /* ColorRAM */
	map(0x1600, 0x1600).w(FUNC(mhavoc_hbmame::mhavocpe_out_0_w));       /* Control Signals - with vector inverts*/
	map(0x1640, 0x1640).w(m_avg, FUNC(avg_device::go_w));               /* Vector Generator GO */
	map(0x1680, 0x1680).w("watchdog", FUNC(watchdog_timer_device::reset_w));         /* Watchdog Clear */
	map(0x16c0, 0x16c0).w(m_avg, FUNC(avg_device::reset_w));             /* Vector Generator Reset */
	map(0x1700, 0x1700).w(FUNC(mhavoc_hbmame::mhavoc_alpha_irq_ack_w));  /* IRQ ack */
	//map(0x1740, 0x1740).w(FUNC(mhavoc_hbmame::mhavocpe_rom_banksel_w));     /* Program ROM Page Select */
	map(0x1740, 0x1740).lw8(NAME([this] (u8 data) { membank("bank2")->set_entry((data & 1) | ((data & 2)<<1) | ((data & 4)>>1)); })); /* Program ROM Page Select */
	map(0x1780, 0x1780).w(FUNC(mhavoc_hbmame::mhavoc_ram_banksel_w));    /* Program RAM Page Select */
	map(0x17c0, 0x17c0).w(FUNC(mhavoc_hbmame::mhavoc_gamma_w));          /* Gamma Communication Write Port */
	map(0x1800, 0x1fff).ram().share("comram");    						 /* BETA COMRAM */
	map(0x2000, 0x3fff).bankr("bank2");                     			 /* Paged Program ROM (32K) */
	map(0x4000, 0x4fff).ram();    										 /* Vector Generator RAM */
	map(0x5000, 0x5fff).rom().region("vectorrom", 0x0000);               /* Vector ROM */
	map(0x6000, 0x6fff).rom().region("vectorrom", 0x1000).mirror(0x1000);	/* Vector ROM */
	//map(0x6000, 0x7fff).bankr("bank3");                     			 /* Paged Vector ROM */
	map(0x8000, 0xffff).rom();                              			 /* Program ROM (32K) */
}

/*************************************
 *
 *  Beta CPU memory handlers
 *
 *************************************/
void mhavoc_hbmame::betape_map(address_map &map)
{
	map(0x0000, 0x07ff).ram();
	map(0x4000, 0x47ff).ram().share("comram");
	map(0x8000, 0xffff).rom();                 /* Program ROM (32K) */
}

/*************************************
 *
 *  Gamma CPU memory handlers
 *
 *************************************/

void mhavoc_hbmame::gammape_lpc_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800);                   /* Program RAM (2K) */
	map(0x2000, 0x203f).rw(FUNC(mhavoc_hbmame::quad_pokeyn_r), FUNC(mhavoc_hbmame::quad_pokeyn_w)).mirror(0x07C0); /* Quad Pokey read/write  */
	map(0x2800, 0x2800).portr("IN1").mirror(0x07ff);      /* Gamma Input Port */
	map(0x3000, 0x3000).r(FUNC(mhavoc_hbmame::mhavoc_alpha_r)).mirror(0x07ff);  /* Alpha Comm. Read Port */
	map(0x3800, 0x3803).r(FUNC(mhavoc_hbmame::leta_r));   /* Roller Controller Input */
	map(0x4000, 0x4000).w(FUNC(mhavoc_hbmame::mhavoc_gamma_irq_ack_w)).mirror(0x07ff); /* DSW at 8S, IRQ Acknowledge */
	map(0x4800, 0x4800).w(FUNC(mhavoc_hbmame::mhavoc_out_1_w)).mirror(0x07ff); /* Coin Counters    */
	map(0x5000, 0x5000).w(FUNC(mhavoc_hbmame::mhavoc_alpha_w)).mirror(0x07ff); /* Alpha Comm. Write Port */
	map(0x5800, 0x5800).w(FUNC(mhavoc_hbmame::mhavocrv_speech_data_w)).mirror(0x06ff); /* TMS5220 data write */
	map(0x5900, 0x5900).w(FUNC(mhavoc_hbmame::mhavocrv_speech_strobe_w)).mirror(0x06ff); /* TMS5220 /WS strobe write */
	map(0x6000, 0x61ff).rw("eeprom", FUNC(eeprom_parallel_28xx_device::read), FUNC(eeprom_parallel_28xx_device::write)).mirror(0x1e00); /* EEROM */
	map(0x8000, 0xffff).rom();                   /* Program ROM (32K) */
}

//later mhavocpe versions support both the old TMS5220 and the OKIM6295
void mhavoc_hbmame::gammape_adpcm_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800);                   /* Program RAM (2K) */
	map(0x2000, 0x203f).rw(FUNC(mhavoc_hbmame::quad_pokeyn_r), FUNC(mhavoc_hbmame::quad_pokeyn_w)).mirror(0x07C0); /* Quad Pokey read/write  */
	map(0x2800, 0x2800).portr("IN1").mirror(0x07ff);      /* Gamma Input Port */
	map(0x3000, 0x3000).r(FUNC(mhavoc_hbmame::mhavoc_alpha_r)).mirror(0x07ff);  /* Alpha Comm. Read Port */
	map(0x3800, 0x3801).r(FUNC(mhavoc_hbmame::leta_r));   /* Roller Controller Input */
	map(0x4000, 0x4000).w(FUNC(mhavoc_hbmame::mhavoc_gamma_irq_ack_w)).mirror(0x07ff); /* DSW at 8S, IRQ Acknowledge */
	map(0x4800, 0x4800).w(FUNC(mhavoc_hbmame::mhavoc_out_1_w)).mirror(0x07ff); /* Coin Counters    */
	map(0x5000, 0x5000).w(FUNC(mhavoc_hbmame::mhavoc_alpha_w)).mirror(0x07ff); /* Alpha Comm. Write Port */
	map(0x5800, 0x5800).w("oki", FUNC(okim6295_device::write));
	map(0x5900, 0x5900).w("oki", FUNC(okim6295_device::write));
	map(0x6000, 0x61ff).rw("eeprom", FUNC(eeprom_parallel_28xx_device::read), FUNC(eeprom_parallel_28xx_device::write)).mirror(0x1e00); /* EEROM */
	map(0x8000, 0xbfff).rom().mirror(0x4000);  
}

READ_LINE_MEMBER(mhavoc_hbmame::clock_r)
{
	/* 2.4kHz (divide 2.5MHz by 1024) */
	return (m_alpha->total_cycles() & 0x400) ? 0 : 1;
}

uint8_t mhavoc_hbmame::leta_r(offs_t offset)
{
	static const char *const letanames[] = { "LETA0", "LETA1"};
	return ioport(letanames[offset])->read();
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
    PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(mhavoc_hbmame, coin_cabinet_l)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(mhavoc_hbmame, coin_service_r)

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

	//PORT_START("DIAL")  /* gamma */
	//PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_REVERSE
	PORT_START("LETA0")
	PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_REVERSE
	PORT_START("LETA1")
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

static INPUT_PORTS_START( mhavocpe_inputs )
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
    PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(mhavoc_hbmame, coin_cabinet_l)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(mhavoc_hbmame, coin_service_r)

	PORT_START("IN1")   /* gamma */
	/* Bits 7-2 = input switches */
	/* Bit 2 = TI Ready Flag */
	/* Bit 1 = Alpha rcvd flag */
	/* Bit 0 = Alpha xmtd flag */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, alpha_xmtd_r)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, alpha_rcvd_r)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(mhavoc_hbmame, gamma_tirdy_r)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 )

	PORT_START("LETA0")
	PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_REVERSE
	PORT_START("LETA1")
	PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_COCKTAIL PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_REVERSE

	// DIP SWITCHES ARE UNUSED IN PE, SETTINGS CHANGED IN SELF TEST, STILL DEFINE A CABINET PIN SWITCH THO
    PORT_START("CABINET")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_SERVICE1 ) 

	PORT_START("COIN")      /* dummy for player_1 = 0 on alpha */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_COIN1 )      /* Left Coin Switch  */
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_COIN2 )      /* Right Coin */

	PORT_START("SERVICE")   /* dummy for player_1 = 1 on alpha */
	PORT_SERVICE( 0x01, IP_ACTIVE_LOW )
	PORT_SERVICE( 0x02, IP_ACTIVE_LOW )
    
INPUT_PORTS_END

/*************************************
 *
 *  Machine drivers
 *
 *************************************/

void mhavoc_hbmame::mhavoc_common(machine_config &config)
{
	M6502(config, m_alpha, MHAVOC_CLOCK_2_5M);     /* 2.5 MHz */
	m_alpha->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::alphape_map);

	/* create BETA */
	//M6502(config, m_beta, MHAVOC_CLOCK_2_5M);     /* 2.5 MHz */
	//m_beta->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::betape_map);

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

	AVG_MHAVOC(config, m_avg, 0);
	m_avg->set_vector("vector");
	m_avg->set_memory(m_alpha, AS_PROGRAM, 0x4000);

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	POKEY(config, m_pokey[0], MHAVOC_CLOCK_1_25M);
	//m_pokey[0]->allpot_r().set_ioport("DSW1");
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

void mhavoc_hbmame::mhavocpe_lpc(machine_config &config)
{
	mhavoc_common(config);

	M6502(config, m_gamma, MHAVOC_CLOCK_1_25M);    /* 1.25 MHz */
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::gammape_lpc_map);

	TMS5220(config, m_tms, MHAVOC_CLOCK/2/9);
	m_tms->add_route(ALL_OUTPUTS, "mono", 1.0);
}

void mhavoc_hbmame::mhavocpe_adpcm(machine_config &config)
{
	mhavoc_common(config);

	M6502(config, m_gamma, MHAVOC_CLOCK_1_25M);    /* 1.25 MHz */
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_hbmame::gammape_adpcm_map);

	TMS5220(config, m_tms, MHAVOC_CLOCK/2/9);
	m_tms->add_route(ALL_OUTPUTS, "mono", 1.0);

	OKIM6295(config, m_oki, OKI_CLOCK, okim6295_device::PIN7_HIGH);
	m_oki->add_route(ALL_OUTPUTS, "mono", 1.0);	
}


/*************************************
 *
 *  ROM definitions
 *
 *************************************/

/*
 * Notes:
 * the R3 roms are supported as "mhavoc", the R2 roms (with a bug in gameplay) are supported as "mhavoc2".
 * "Return to Vax" - Jess Askey's souped up version (errors on self test) are supported as "mhavocrv".
 * Prototype is supported as "mhavocp"
 * Alpha one is a single-board prototype
 */

// license:BSD-3-Clause
// copyright-holders:Robbbert, Jess M. Askey

ROM_START( mhavocpex1 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex021.6kl",   0x00000, 0x2000, CRC(4c05b1a8) SHA1(89b524182fcfd966d6a7e3188235c957c451b8a9) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex021.1mn",   0x08000, 0x4000, CRC(3b691eff) SHA1(e8227d1458e3ed4d0e8444ec23f2c2d45a0d93b8) )
	ROM_LOAD( "mhpex021.1l",    0x0c000, 0x4000, CRC(fb53dae6) SHA1(08e9bd60e801778d3521d64817a10ba1ed74f4ff) )
	ROM_LOAD( "mhpex021.1q",    0x10000, 0x8000, CRC(660e3d57) SHA1(6eddf1335c536406080eab73f5501a202fb0583d) )
	ROM_LOAD( "mhpex021.1np",   0x18000, 0x8000, CRC(c1a70bad) SHA1(0b72b6817e2f00d2c001ac61ebd2cd42ff7785c9) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex021.6h",    0x0000, 0x4000, CRC(79fc58c0) SHA1(7b40dfb89bc4078e2bd6f89a570f2be9cca15df9) )
	ROM_LOAD( "mhpex021.6jk",   0x4000, 0x4000, CRC(dc78b802) SHA1(6b951982232de08d32d3a2d01814cc28f89d2120) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex021.9s",    0x8000, 0x8000, CRC(d42ee58e) SHA1(667aec3c3e93df3f8dedddb0db1188291e37630b) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex2 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex050.6kl",  0x00000, 0x2000, CRC(3804822b) SHA1(78ca5ad3ac9ce9f6ce6ca497f70c7233adc52502) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex050.1mn",  0x08000, 0x4000, CRC(2b505cae) SHA1(1ca14c6c810a3dec964eae63e9941e411d3ec546) )
	ROM_LOAD( "mhpex050.1l",   0x0c000, 0x4000, CRC(8c078dcf) SHA1(b4681eb4930938a93e74c313ee1031ad1a71e198) )
	ROM_LOAD( "mhpex050.1q",   0x10000, 0x8000, CRC(7602fccd) SHA1(9073c5e2a70f23c7bab2927936ee5a22e2adb57a) )
	ROM_LOAD( "mhpex050.1np",  0x18000, 0x8000, CRC(bc6775e1) SHA1(a59a93d4414a0318e37c1e50ef139b6611e4233f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex050.6h",   0x0000, 0x4000, CRC(ec19097f) SHA1(261c957e5311ac5fd700949e04e9d4a6ef12c043) )
	ROM_LOAD( "mhpex050.6jk",  0x4000, 0x4000, CRC(2ac3f07b) SHA1(27d06ed9edf8cd227c7db7c28a7f7357da1260c6) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex050.9s",   0x8000, 0x8000, CRC(57ca8c09) SHA1(84ae19a9ff3b61c75f1fd147540a1e5fb42e6915) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex3 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex052.6kl",  0x00000, 0x2000, CRC(0bebf550) SHA1(043dd185a40208de8f7952026f1645799eb3445f) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex052.1mn",  0x08000, 0x4000, CRC(b3a485e8) SHA1(47e20e02a69675d5df51e68d33bc407b441d621c) )
	ROM_LOAD( "mhpex052.1l",   0x0c000, 0x4000, CRC(baa2d31a) SHA1(d6337763e4d6cc86c95d2abf54955a7b413c9846) )
	ROM_LOAD( "mhpex052.1q",   0x10000, 0x8000, CRC(ebb47bb5) SHA1(e73ea9bde6c525b644824deb73a4caa3f9a7e28f) )
	ROM_LOAD( "mhpex052.1np",  0x18000, 0x8000, CRC(793b480d) SHA1(9a0398dad5b8634190d9432e1b94a786054b4e6e) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex052.6h",   0x0000, 0x4000, CRC(f1bcfaf2) SHA1(a70c3dd399c961f8a12dfc343c106d7e77e77a42) )
	ROM_LOAD( "mhpex052.6jk",  0x4000, 0x4000, CRC(8a8a67cf) SHA1(af5e64e0329015e3d2cc5292d671fe64cafd6463) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex052.9s",   0x8000, 0x8000, CRC(603ac7da) SHA1(8343cc8a388673436df9b5ad2894136943c676da) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex4 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex067.6kl",  0x00000, 0x2000, CRC(75ab6d18) SHA1(874f4a15532f257480c8f6ca0cf9b3cd7ea39908) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex067.1mn",  0x08000, 0x4000, CRC(120b00f0) SHA1(b8546c81ecc8e16f5e0afb7ce9b941623d862d31) )
	ROM_LOAD( "mhpex067.1l",   0x0c000, 0x4000, CRC(ffcb7a73) SHA1(ec284426c381da3c15ec50c3bbbdf38fb15efcb2) )
	ROM_LOAD( "mhpex067.1q",   0x10000, 0x8000, CRC(dc85e7b3) SHA1(c9cbe410915686f470c033cc5ccacf76f81c46ba) )
	ROM_LOAD( "mhpex067.1np",  0x18000, 0x8000, CRC(134d84f9) SHA1(4933dda173c9abc813edf6cd0bcef24977b6fe1a) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex067.6h",   0x0000, 0x4000, CRC(df87c581) SHA1(bc13cd3ddf4a1e8c93d452eafafd295c37e43057) )
	ROM_LOAD( "mhpex067.6jk",  0x4000, 0x4000, CRC(8b1f8b3d) SHA1(c0825a1d7b49eb2cf169361563cd0f8e6df5a159) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex067.9s",   0x8000, 0x8000, CRC(e91ba3e8) SHA1(6283e5365cda8fe9706c1cd15ac7bd3978e029bc) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex5 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex072.6kl",  0x00000, 0x2000, CRC(025ae0de) SHA1(5390dee62e5499c8767b4fc34f38fef49579d05b) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex072.1mn",  0x08000, 0x4000, CRC(b2b3067e) SHA1(209a5d6d53825164ba7c92cff67e69571c6a0b32) )
	ROM_LOAD( "mhpex072.1l",   0x0c000, 0x4000, CRC(367c0028) SHA1(e2f153d4c1b3ba19b48a34d1bd776318499513c9) )
	ROM_LOAD( "mhpex072.1q",   0x10000, 0x8000, CRC(04e4476b) SHA1(b9ac489e2b3ea8c04e6c907b3dbf6dfdbba976af) )
	ROM_LOAD( "mhpex072.1np",  0x18000, 0x8000, CRC(8c8ca853) SHA1(12d1dbd3f76e85021ba2226defd194de8543c9ac) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex072.6h",   0x0000, 0x4000, CRC(ea89c415) SHA1(f91ae8b9d1942e4ee14eaf9c1d0e687f2039c89d) )
	ROM_LOAD( "mhpex072.6jk",  0x4000, 0x4000, CRC(c56e754a) SHA1(6f02782aa14e10f6a9a2f5065d3ec9e2d9e07d5e) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex072.9s",   0x8000, 0x8000, CRC(8abffddd) SHA1(4cea72b569de302feac53b4e4a861887904c6bbe) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex6 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex075.6kl",  0x00000, 0x2000, CRC(93688346) SHA1(f17b44cb196216c01ac75b0472791fda6995bd18) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex075.1mn",  0x08000, 0x4000, CRC(dd079cdd) SHA1(c9a7d42f687b99859f95b397618a31461abc3aa6) )
	ROM_LOAD( "mhpex075.1l",   0x0c000, 0x4000, CRC(d0c7ed14) SHA1(a07fbcd2b90b34d9cb9bff25c656099a9eda501c) )
	ROM_LOAD( "mhpex075.1q",   0x10000, 0x8000, CRC(897fbf17) SHA1(e3e7f3ac2204d422568b7dfa4f5a577c79d50ed5) )
	ROM_LOAD( "mhpex075.1np",  0x18000, 0x8000, CRC(3a215adc) SHA1(67684860244ec50b368f17179b1eeeb7b92478b7) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex075.6h",   0x0000, 0x4000, CRC(ef9f8321) SHA1(3c590c78664d6e121c47e11046fb3874ef0e1e90) )
	ROM_LOAD( "mhpex075.6jk",  0x4000, 0x4000, CRC(3dd7bec9) SHA1(aaa5e48d45519624017a9ec04b3e4b721e01ff2a) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex075.9s",   0x8000, 0x8000, CRC(5e1b2ada) SHA1(8fc2334e090b5f9b2bd97ab792aa69f170643752) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex7 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex078.6kl",  0x00000, 0x2000, CRC(ef7843f8) SHA1(bfd096b0db69188f9032578839c9bd91367f7aae) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex078.1mn",  0x08000, 0x4000, CRC(f043dcaf) SHA1(3f20c20ea601190a52dac50d3281ed09a18a4948) )
	ROM_LOAD( "mhpex078.1l",   0x0c000, 0x4000, CRC(da963eab) SHA1(b10872898b12d79cd6cf0651ec61962315b5aff9) )
	ROM_LOAD( "mhpex078.1q",   0x10000, 0x8000, CRC(57499813) SHA1(ff53f65fc34c5f4b8d4af78776fb242ac59d536a) )
	ROM_LOAD( "mhpex078.1np",  0x18000, 0x8000, CRC(0d362096) SHA1(dccc0509aac7003fee11bf7eeabff81360c226a8) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex078.6h",   0x0000, 0x4000, CRC(4129d0e9) SHA1(6fd168de59d684f216a0a0ac45aed0782b18f900) )
	ROM_LOAD( "mhpex078.6jk",  0x4000, 0x4000, CRC(9361ed01) SHA1(ea07f8430f7e97e0c62d8df2963bb7b66a45b698) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex078.9s",   0x8000, 0x4000, CRC(9703c51f) SHA1(c010021e0dabc568af2eb1d516726ac21a1c9c98) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpex078.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

ROM_START( mhavocpex7de )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex078de.6kl",  0x00000, 0x2000, CRC(6bc3240f) SHA1(6152a5015a99142d77203fa70f96160ffecd460c) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex078de.1mn",  0x08000, 0x4000, CRC(30fc2132) SHA1(0d07fab70e9e2d606a825837436c242706fa690a) )
	ROM_LOAD( "mhpex078de.1l",   0x0c000, 0x4000, CRC(245100c8) SHA1(90333f9ad7b1c4422bc0b68a9a4d59edd9596d65) )
	ROM_LOAD( "mhpex078de.1q",   0x10000, 0x8000, CRC(8fb86a78) SHA1(53b3ec55962c7a6f615838005b56de7458147f46) )
	ROM_LOAD( "mhpex078de.1np",  0x18000, 0x8000, CRC(c8a86dfb) SHA1(8e3257068ae52095b8f15012a46cc99d9e28242f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex078de.6h",   0x0000, 0x4000, CRC(8df8c6e5) SHA1(b5fadd531f040ef01c9c39810bfe81b800a32f78) )
	ROM_LOAD( "mhpex078de.6jk",  0x4000, 0x4000, CRC(5fb0fb0d) SHA1(70f72856cb8ade503b00e43c12ed52bbabbd7ca6) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex078de.9s",   0x8000, 0x4000, CRC(722ade2e) SHA1(212cb5c9201e41e08eada3aec3c77ab8728135b1) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpex078.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

ROM_START( mhavocpex7fr )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex078fr.6kl",  0x00000, 0x2000, CRC(3d7f8a57) SHA1(cc8cd99003b56805c6eab9c4d26c17c485966d17) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex078fr.1mn",  0x08000, 0x4000, CRC(ad00b995) SHA1(f210a934be809b272bbbd7b3a459df6614e8a8f9) )
	ROM_LOAD( "mhpex078fr.1l",   0x0c000, 0x4000, CRC(c01b01fa) SHA1(9965da7863da933a1be1ce32e3db578ce4e54bb4) )
	ROM_LOAD( "mhpex078fr.1q",   0x10000, 0x8000, CRC(81ac4eda) SHA1(eeab82e1bad953a72300986c9dcdc02f3113cb06) )
	ROM_LOAD( "mhpex078fr.1np",  0x18000, 0x8000, CRC(6cf82736) SHA1(4a17ebd9aac863ce34ff5ef2d139cf319c373502) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex078fr.6h",   0x0000, 0x4000, CRC(03fafab0) SHA1(5a27c5cf745d51e76c8af63c7880b0337e1b97f5) )
	ROM_LOAD( "mhpex078fr.6jk",  0x4000, 0x4000, CRC(d1b2c758) SHA1(047c328855a6fa930e172b77798d0373027ac0ce) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex078fr.9s",   0x8000, 0x4000, CRC(8620f53c) SHA1(96a05e1410d2d5fa55a35bf88d057101aede6681) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpex078.x1", 0x000000, 0x040000, CRC(5cfa1865) SHA1(ab520b4af6a9ffc2593223798fee8026266a722e) )
ROM_END

ROM_START( mhavocpex8 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex089.6kl",  0x00000, 0x2000, CRC(016b6964) SHA1(32ca97d6b289f4719e994e46091d4694d37912df) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex089.1mn",  0x08000, 0x4000, CRC(153c0fa4) SHA1(70b793b432f51fab352b923208c5053addeb48db) )
	ROM_LOAD( "mhpex089.1l",   0x0c000, 0x4000, CRC(59e05d8f) SHA1(24728f51ae177c7e3d2e77e9896f973060ec94de) )
	ROM_LOAD( "mhpex089.1q",   0x10000, 0x8000, CRC(254a5fc7) SHA1(8241995378308c423d8e185a63290fcca77b8fcd) )
	ROM_LOAD( "mhpex089.1np",  0x18000, 0x8000, CRC(fe1e8da3) SHA1(43069a222553134f4ce537793656f0ee6b72a9e7) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex089.6h",   0x0000, 0x4000, CRC(48a68f90) SHA1(1552aed29241c09c0105e73bbe465634f201ce71) )
	ROM_LOAD( "mhpex089.6jk",  0x4000, 0x4000, CRC(35499881) SHA1(85b77393fda14ab8a7b6b439d96e9528e08f548d) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex089.9s",   0x8000, 0x4000, CRC(17f007bb) SHA1(163376f61b93e6e1dadfce2e586c9fbaa637e955) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpex089.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpex8de )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex089de.6kl",  0x00000, 0x2000, CRC(85d00e93) SHA1(d3859de5dfae4db7a6ad65df2c08243e5235bdaa) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex089de.1mn",  0x08000, 0x4000, CRC(155c3eb7) SHA1(4fbf6dca198bbbc22f2e527f8f5fcc47dcc3e5b0) )
	ROM_LOAD( "mhpex089de.1l",   0x0c000, 0x4000, CRC(b6f51911) SHA1(27d7ad3eaac124404afb705b8f5e2036b716f173) )
	ROM_LOAD( "mhpex089de.1q",   0x10000, 0x8000, CRC(fcc6a9bd) SHA1(d87aaeb3a0cca2c9f0e82d39a5624efce973ece4) )
	ROM_LOAD( "mhpex089de.1np",  0x18000, 0x8000, CRC(a472b443) SHA1(127c8c154ef30924afc30dc857c0d9af0be65fa4) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex089de.6h",   0x0000, 0x4000, CRC(8477999c) SHA1(af5bba82c7f6e9718cfd89e55eb4a10aaec7e901) )
	ROM_LOAD( "mhpex089de.6jk",  0x4000, 0x4000, CRC(f9988e8d) SHA1(43f43e5b6a5d05464ce3dc14a1a6e4c17f12e8ae) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex089de.9s",   0x8000, 0x4000, CRC(f2d91c8a) SHA1(143625874f17fa5c1cd4633c2bee22544c6e9ac9) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpex089.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpex8fr )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpex089fr.6kl",  0x00000, 0x2000, CRC(d36ca0cb) SHA1(6037773f555f75fb6002466931cd665218a29a91) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpex089fr.1mn",  0x08000, 0x4000, CRC(4b24b5d7) SHA1(4cb89248f16dd4264677068e3494efc85c895b0f) )
	ROM_LOAD( "mhpex089fr.1l",   0x0c000, 0x4000, CRC(ba77851e) SHA1(0d6b939ac1d1f2723125d142f371695537843e78) )
	ROM_LOAD( "mhpex089fr.1q",   0x10000, 0x8000, CRC(1a04a9ce) SHA1(12f225a2e5b9793b4ac3996f9c15b525fb3c236f) )
	ROM_LOAD( "mhpex089fr.1np",  0x18000, 0x8000, CRC(8f17b732) SHA1(a933b16907c00474eedd4701a6b78b197e3bf673) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpex089fr.6h",   0x0000, 0x4000, CRC(0a75a5c9) SHA1(4b6d6cbc798da565069a5751d12743ac5dce4d28) )
	ROM_LOAD( "mhpex089fr.6jk",  0x4000, 0x4000, CRC(779ab2d8) SHA1(e4bc1eda5d93dba3aaa92b1204599e384fea7d52) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpex089fr.9s",   0x8000, 0x4000, CRC(06d33798) SHA1(3299cbfa7b2faaec9f223f0183b17461a1cc8bac) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpex089.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

//Current Released Version ADPCM - English
ROM_START( mhavocpe )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpe101.6kl",  0x00000, 0x2000,  CRC(ddcaab40) SHA1(d70b91137204ebbf2685fe22a3498cb6014a7bef) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe101.1mn",  0x08000, 0x4000, CRC(8b4b1c7c) SHA1(9840532b78f5ca7e9785d35883f191b46d0e1436) )
	ROM_LOAD( "mhpe101.1l",   0x0c000, 0x4000, CRC(90348169) SHA1(58227bd56d123aa76c8f287f75b83a3a7cc6d1d2) )
	ROM_LOAD( "mhpe101.1q",   0x10000, 0x8000, CRC(d5d86868) SHA1(ddc6330a55106dadefa356b55f1562affb9ebc00) )
	ROM_LOAD( "mhpe101.1np",  0x18000, 0x8000, CRC(6f7b38a9) SHA1(0107cc88a54780c6bca97afbf0e99adf1f4ceba1) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe101.6h",   0x0000, 0x4000, CRC(6b380183) SHA1(2fad9dc301aa5622195e3acac2865339406ccc38) )
	ROM_LOAD( "mhpe101.6jk",  0x4000, 0x4000, CRC(7ecfd43b) SHA1(7f99fde09062bc4ca4bbbf06f1b11dc3f5203541) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe101.9s",   0x8000, 0x4000, CRC(46cdd0fa) SHA1(f6c75333311279b15cb42ce3ed8a40e54a508869) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpe101.x1", 0x000000, 0x040000, CRC(aade65d1) SHA1(7e379ee84ee36395095ce68d1c1da4fcf907a07e) )
ROM_END

ROM_START( mhavocpede )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpede101.6kl",  0x00000, 0x2000, CRC(5971ccb7) SHA1(4dc0a536727082d4e8d502bbaa9b655815ad517d) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpede101.1mn",  0x08000, 0x4000, CRC(a3ff05f2) SHA1(610a8c28d7a1f74fb0efacdaff5999dde85f335d) )
	ROM_LOAD( "mhpede101.1l",   0x0c000, 0x4000, CRC(214369f7) SHA1(fabf285e1dd03cfab847c7bb6c98a9ad9a645b4b) )
	ROM_LOAD( "mhpede101.1q",   0x10000, 0x8000, CRC(ad51ea70) SHA1(734bc3a574736efdb6cec43a29fac956d40c4954) )
	ROM_LOAD( "mhpede101.1np",  0x18000, 0x8000, CRC(7d3c4e41) SHA1(ed5e7415ec22526fb36f91ef12a8fefab554c248) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpede101.6h",   0x0000, 0x4000, CRC(a7e9178f) SHA1(8e98bbe8277c78629fe303678b589289fc05fff3) )
	ROM_LOAD( "mhpede101.6jk",  0x4000, 0x4000, CRC(b21ec237) SHA1(e974cef797f9e5568808201895708c00bcf20a8b) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpede101.9s",   0x8000, 0x4000, CRC(a3e4cbcb) SHA1(054ea96bd738b9ab704866d752b74b84975422a0) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpede101.x1", 0x000000, 0x040000, CRC(aade65d1) SHA1(7e379ee84ee36395095ce68d1c1da4fcf907a07e) )
ROM_END

ROM_START( mhavocpefr )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpefr101.6kl",  0x00000, 0x2000, CRC(0fcd62ef) SHA1(0f9f9f41a87f5ad839f96ee89be580d4f0531a99) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpefr101.1mn",  0x08000, 0x4000, CRC(64b59f45) SHA1(fc44f0a0d0364a610d1e06c4f08d19987d1b427a) )
	ROM_LOAD( "mhpefr101.1l",   0x0c000, 0x4000, CRC(bc0d539f) SHA1(3761c0118994154577466566f05378ae67a7f56e) )
	ROM_LOAD( "mhpefr101.1q",   0x10000, 0x8000, CRC(12ef0140) SHA1(0840485f3c47ad92a25d2d492beb835b324468e0) )
	ROM_LOAD( "mhpefr101.1np",  0x18000, 0x8000, CRC(14305109) SHA1(8b2ada62781fe28c40176195f3d39f331e76b575) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpefr101.6h",   0x0000, 0x4000, CRC(29eb2bda) SHA1(e843b39aa65c08e948fda60e9f9ba94d8f8a0643) )
	ROM_LOAD( "mhpefr101.6jk",  0x4000, 0x4000, CRC(3c1cfe62) SHA1(683eca811f87359129c66541d7f6601b87f1122c) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpefr101.9s",   0x8000, 0x4000, CRC(57eee0d9) SHA1(5ff2e0b49d5ddf07193fae7df708fb086e06202b) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpefr101.x1", 0x000000, 0x040000, CRC(aade65d1) SHA1(7e379ee84ee36395095ce68d1c1da4fcf907a07e) )
ROM_END

ROM_START( mhavocpete )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpete101.6kl",  0x00000, 0x2000, CRC(ddcaab40) SHA1(d70b91137204ebbf2685fe22a3498cb6014a7bef) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpete101.1mn",  0x08000, 0x4000, CRC(71fdffa0) SHA1(1a8694f73e81516580465f7dbc48a7ae21de5b23) )
	ROM_LOAD( "mhpete101.1l",   0x0c000, 0x4000, CRC(feba2023) SHA1(6384a57ed9df04adfa8f7080454f656e58c38aca) )
	ROM_LOAD( "mhpete101.1q",   0x10000, 0x8000, CRC(428b51fd) SHA1(6d09f94af81aa9cd417d692235127c212c03fa16) )
	ROM_LOAD( "mhpete101.1np",  0x18000, 0x8000, CRC(835c85fd) SHA1(952096742f5ff4194adfc1f4e4460522e23834e1) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpete101.6h",   0x0000, 0x4000, CRC(6b380183) SHA1(2fad9dc301aa5622195e3acac2865339406ccc38) )
	ROM_LOAD( "mhpete101.6jk",  0x4000, 0x4000, CRC(7ecfd43b) SHA1(7f99fde09062bc4ca4bbbf06f1b11dc3f5203541) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpete101.9s",   0x8000, 0x4000,  CRC(46cdd0fa) SHA1(f6c75333311279b15cb42ce3ed8a40e54a508869) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpete101.x1", 0x000000, 0x040000, CRC(aade65d1) SHA1(7e379ee84ee36395095ce68d1c1da4fcf907a07e) )
ROM_END

ROM_START( mhavocpedete )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpedete101.6kl",  0x00000, 0x2000, CRC(5971ccb7) SHA1(4dc0a536727082d4e8d502bbaa9b655815ad517d) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpedete101.1mn",  0x08000, 0x4000, CRC(5949e62e) SHA1(5b457709de3a70f1828e73cb3aed85356fa50059) )
	ROM_LOAD( "mhpedete101.1l",   0x0c000, 0x4000, CRC(daf0e86a) SHA1(1e96a534e0614cd628213b01822d19fbb19e97b8) )
	ROM_LOAD( "mhpedete101.1q",   0x10000, 0x8000, CRC(9401e3be) SHA1(37e31180b923b69c2448aa0e14315ca63026d63f) )
	ROM_LOAD( "mhpedete101.1np",  0x18000, 0x8000, CRC(9fe25082) SHA1(ba83e739d7bd5bf1742dcee4f4962cb962518251) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpedete101.6h",   0x0000, 0x4000, CRC(a7e9178f) SHA1(8e98bbe8277c78629fe303678b589289fc05fff3) )
	ROM_LOAD( "mhpedete101.6jk",  0x4000, 0x4000, CRC(b21ec237) SHA1(e974cef797f9e5568808201895708c00bcf20a8b) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpedete101.9s",   0x8000, 0x4000, CRC(a3e4cbcb) SHA1(054ea96bd738b9ab704866d752b74b84975422a0) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpedete101.x1", 0x000000, 0x040000, CRC(aade65d1) SHA1(7e379ee84ee36395095ce68d1c1da4fcf907a07e) )
ROM_END

ROM_START( mhavocpefrte )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpefrte101.6kl",  0x00000, 0x2000, CRC(0fcd62ef) SHA1(0f9f9f41a87f5ad839f96ee89be580d4f0531a99) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpefrte101.1mn",  0x08000, 0x4000, CRC(9e037c99) SHA1(95e72e73d0c73c530d4d38159eac776dcbdd4258) )
	ROM_LOAD( "mhpefrte101.1l",   0x0c000, 0x4000, CRC(67eac8ae) SHA1(b1bc18060321050dd0199c3e02c6a1d390d51e02) )
	ROM_LOAD( "mhpefrte101.1q",   0x10000, 0x8000, CRC(1db409a8) SHA1(b53897a7ef18b9bb73369b54b388543172064798) )
	ROM_LOAD( "mhpefrte101.1np",  0x18000, 0x8000, CRC(620015e9) SHA1(d588600362d6981520d86b4ac23f0d9c4d2be148) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpefrte101.6h",   0x0000, 0x4000, CRC(29eb2bda) SHA1(e843b39aa65c08e948fda60e9f9ba94d8f8a0643) )
	ROM_LOAD( "mhpefrte101.6jk",  0x4000, 0x4000, CRC(3c1cfe62) SHA1(683eca811f87359129c66541d7f6601b87f1122c) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpefrte101.9s",   0x8000, 0x4000, CRC(57eee0d9) SHA1(5ff2e0b49d5ddf07193fae7df708fb086e06202b) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpefrte101.x1", 0x000000, 0x040000, CRC(aade65d1) SHA1(7e379ee84ee36395095ce68d1c1da4fcf907a07e) )
ROM_END

//OLD RELEASE BUILDS
ROM_START( mhavocpe100 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpe100.6kl",  0x00000, 0x2000,  CRC(f195a326) SHA1(36681cdc9665ea55bb6a9f143ecc38083e28c379) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe100.1mn",  0x08000, 0x4000, CRC(8bdf1048) SHA1(f93d44ac80938dc6a3f22ab9768a7e2a4a9987f7) )
	ROM_LOAD( "mhpe100.1l",   0x0c000, 0x4000, CRC(b138e015) SHA1(385664936cdd9a65f1ea5c7b03b6369070fe9f0e) )
	ROM_LOAD( "mhpe100.1q",   0x10000, 0x8000, CRC(22e54ef3) SHA1(2b5e4e2b072c1a6fb2f7c34eebf59d0d03e8f564) )
	ROM_LOAD( "mhpe100.1np",  0x18000, 0x8000, CRC(6a67d68e) SHA1(1ffcb543db81db3c4d4591b3f4d0bf8a6eb4f47c) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe100.6h",   0x0000, 0x4000, CRC(88527b65) SHA1(f525e39b7cb32008c016a4e076195642cec5024d) )
	ROM_LOAD( "mhpe100.6jk",  0x4000, 0x4000, CRC(f5bd6c74) SHA1(a4dc4ad05469c125c842bf9e0d465ed3b9ee85c8) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe100.9s",   0x8000, 0x4000, CRC(99c12d95) SHA1(27d82f3bc73fbbb310f4e6cda27788e3f3fc1007) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpe100.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpede100 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpede100.6kl",  0x00000, 0x2000, CRC(752ec4d1) SHA1(012bda0d8af7ffa3beeebb31f4d8772bd6d4a9e0) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpede100.1mn",  0x08000, 0x4000, CRC(5972eb18) SHA1(9281756c65108e12a0155161c53a19d94654cc3d) )
	ROM_LOAD( "mhpede100.1l",   0x0c000, 0x4000, CRC(b926eccc) SHA1(960a337b674f37021b3b5de9fc9081c6c5d50da7) )
	ROM_LOAD( "mhpede100.1q",   0x10000, 0x8000, CRC(7fa593e9) SHA1(7e58b392f129ede857ed84385040e3fdb0054021) )
	ROM_LOAD( "mhpede100.1np",  0x18000, 0x8000, CRC(300bef6e) SHA1(fa89b42eea215997d09a864fe7ec4446052b735c) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpede100.6h",   0x0000, 0x4000, CRC(44836d69) SHA1(7f7fa905c659abd591fdc47c587fc770a8e5fada) )
	ROM_LOAD( "mhpede100.6jk",  0x4000, 0x4000, CRC(396c7a78) SHA1(671d50ab39f951d9401682a212f793103a1e56fb) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpede100.9s",   0x8000, 0x4000, CRC(7ce836a4) SHA1(9284fefb256d8e20812eb71216e3bc7ba76af39a) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpede100.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpefr100 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpefr100.6kl",  0x00000, 0x2000, CRC(23926a89) SHA1(9f056219e29b22d61c6549e1f6d6e8fe51ed98a8) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpefr100.1mn",  0x08000, 0x4000, CRC(b490caa2) SHA1(14e81015d9b9dc4b39102d4c09718d219d2874d4) )
	ROM_LOAD( "mhpefr100.1l",   0x0c000, 0x4000, CRC(625adcb2) SHA1(5e73398fd4a01a12fa9d99d85cc0381d9104ccbe) )
	ROM_LOAD( "mhpefr100.1q",   0x10000, 0x8000, CRC(6c8d6a09) SHA1(9a8a399a1667e3e61993a8053abbec1a9d3ce4d9) )
	ROM_LOAD( "mhpefr100.1np",  0x18000, 0x8000, CRC(3e91a0ff) SHA1(b633323672064073612298de22588e04544c762f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpefr100.6h",   0x0000, 0x4000, CRC(ca81513c) SHA1(2ddfcc708be7d70c2aae0d9b66b823aa70a45cf0) )
	ROM_LOAD( "mhpefr100.6jk",  0x4000, 0x4000, CRC(b76e462d) SHA1(4b598112bdcdf330c239b58f5a1a034f7b9ab07d) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpefr100.9s",   0x8000, 0x4000, CRC(88e21db6) SHA1(ee010fc63f90e9538b49e3ef3a78220d2f9e9ce9) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpefr100.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpete100 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpete100.6kl",  0x00000, 0x2000, CRC(f195a326) SHA1(36681cdc9665ea55bb6a9f143ecc38083e28c379) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpete100.1mn",  0x08000, 0x4000, CRC(ca751867) SHA1(3fac5e31c624ebf6273c912604482d65584895ae) )
	ROM_LOAD( "mhpete100.1l",   0x0c000, 0x4000, CRC(3dac3795) SHA1(a4e647f8ce418519b44dbd178d131f0bc4ba35f1) )
	ROM_LOAD( "mhpete100.1q",   0x10000, 0x8000, CRC(6b35a969) SHA1(b02f7f0d84af6389f0191ac6dcea911385b7cc4a) )
	ROM_LOAD( "mhpete100.1np",  0x18000, 0x8000, CRC(20310a4d) SHA1(016e46128722a550ff105f2b8adbf744250c79ca) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpete100.6h",   0x0000, 0x4000, CRC(88527b65) SHA1(f525e39b7cb32008c016a4e076195642cec5024d) )
	ROM_LOAD( "mhpete100.6jk",  0x4000, 0x4000, CRC(f5bd6c74) SHA1(a4dc4ad05469c125c842bf9e0d465ed3b9ee85c8) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpete100.9s",   0x8000, 0x4000, CRC(99c12d95) SHA1(27d82f3bc73fbbb310f4e6cda27788e3f3fc1007) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpete100.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpedete100 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpedete100.6kl",  0x00000, 0x2000, CRC(752ec4d1) SHA1(012bda0d8af7ffa3beeebb31f4d8772bd6d4a9e0) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpedete100.1mn",  0x08000, 0x4000, CRC(18d8e337) SHA1(853c59b8bfe46b09852c936b9860e2269b3065b2) )
	ROM_LOAD( "mhpedete100.1l",   0x0c000, 0x4000, CRC(94123d5c) SHA1(656b86422ef6344ec03d5e028d6c716aa1728baa) )
	ROM_LOAD( "mhpedete100.1q",   0x10000, 0x8000, CRC(8cbef67d) SHA1(ac51906e1a5ed192d72834594b7b72ffbc442562) )
	ROM_LOAD( "mhpedete100.1np",  0x18000, 0x8000, CRC(be075794) SHA1(559fd8ab093d80da1882627c3a0e795c71965d9a) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpedete100.6h",   0x0000, 0x4000, CRC(44836d69) SHA1(7f7fa905c659abd591fdc47c587fc770a8e5fada) )
	ROM_LOAD( "mhpedete100.6jk",  0x4000, 0x4000, CRC(396c7a78) SHA1(671d50ab39f951d9401682a212f793103a1e56fb) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpedete100.9s",   0x8000, 0x4000, CRC(7ce836a4) SHA1(9284fefb256d8e20812eb71216e3bc7ba76af39a) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpedete100.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

ROM_START( mhavocpefrte100 )
	ROM_REGION( 0x2000, "vectorrom", 0 )
	ROM_LOAD( "mhpefrte100.6kl",  0x00000, 0x2000, CRC(23926a89) SHA1(9f056219e29b22d61c6549e1f6d6e8fe51ed98a8) )

	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpefrte100.1mn",  0x08000, 0x4000, CRC(f53ac28d) SHA1(e25cf04327b0e1a9c0398cc1f1c23633e8879171) )
	ROM_LOAD( "mhpefrte100.1l",   0x0c000, 0x4000, CRC(c1f15780) SHA1(b788d138327270c65b93d9ea9e0b645ff4d76803) )
	ROM_LOAD( "mhpefrte100.1q",   0x10000, 0x8000, CRC(0d44804e) SHA1(718254f153ede02b6b9417dc583a63f2e2a65fc5) )
	ROM_LOAD( "mhpefrte100.1np",  0x18000, 0x8000, CRC(cff2909d) SHA1(d98c7a52cba7455dc9c6ed865c4919240dedd11f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpefrte100.6h",   0x0000, 0x4000, CRC(ca81513c) SHA1(2ddfcc708be7d70c2aae0d9b66b823aa70a45cf0) )
	ROM_LOAD( "mhpefrte100.6jk",  0x4000, 0x4000, CRC(b76e462d) SHA1(4b598112bdcdf330c239b58f5a1a034f7b9ab07d) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpefrte100.9s",   0x8000, 0x4000, CRC(88e21db6) SHA1(ee010fc63f90e9538b49e3ef3a78220d2f9e9ce9) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c", 0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )

	ROM_REGION( 0x40000, "oki", 0 )
	ROM_LOAD( "mhpefrte100.x1", 0x000000, 0x040000, CRC(c18cdcfb) SHA1(8b09f5539c90161a237ed3c74f1cf5a0864e04d4) )
ROM_END

//BETA BUILDS
GAME( 2018, mhavocpex1,   mhavoc, mhavocpe_lpc, mhavocrv, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (ALPHA v0.21)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mhavocpex2,   mhavoc, mhavocpe_lpc, mhavocrv, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (ALPHA v0.50)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex3,   mhavoc, mhavocpe_lpc, mhavocrv, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (ALPHA v0.52)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex4,   mhavoc, mhavocpe_lpc, mhavocrv, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.67)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex5,   mhavoc, mhavocpe_lpc, mhavocrv, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.72)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex6,   mhavoc, mhavocpe_lpc, mhavocrv, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.75)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex7,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.78) ENGLISH", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex7fr, mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.78) FRENCH", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mhavocpex7de, mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.78) GERMAN", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpex8,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.89) ENGLISH", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpex8fr, mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.89) FRENCH", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpex8de, mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (BETA v0.89) GERMAN", MACHINE_SUPPORTS_SAVE )

//RELEASE BUILDS
GAME( 2022, mhavocpe,     mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (v1.01) ENGLISH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpefr,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (v1.01) FRENCH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpede,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (v1.01) GERMAN - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpete,     mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End - Tournament (v1.01) ENGLISH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpefrte,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End - Tournament (v1.01) FRENCH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpedete,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End - Tournament (v1.01) GERMAN - ADPCM Speech", MACHINE_SUPPORTS_SAVE )

//OLD RELEASE BUILDS
GAME( 2022, mhavocpe100,     mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (v1.00) ENGLISH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpefr100,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (v1.00) FRENCH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpede100,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End (v1.00) GERMAN - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpete100,     mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End - Tournament (v1.00) ENGLISH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpefrte100,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End - Tournament (v1.00) FRENCH - ADPCM Speech", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mhavocpedete100,   mhavoc, mhavocpe_adpcm, mhavocpe_inputs, mhavoc_hbmame, empty_init, ROT0, "JMA", "Major Havoc - The Promised End - Tournament (v1.00) GERMAN - ADPCM Speech", MACHINE_SUPPORTS_SAVE )

