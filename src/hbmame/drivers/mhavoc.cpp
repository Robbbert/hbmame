// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mhavoc.cpp"



void mhavoc_state::gammape_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().mirror(0x1800);                   /* Program RAM (2K) */
	map(0x2000, 0x203f).rw(FUNC(mhavoc_state::quad_pokeyn_r), FUNC(mhavoc_state::quad_pokeyn_w)).mirror(0x07C0); /* Quad Pokey read/write  */
	map(0x2800, 0x2800).portr("IN1").mirror(0x07ff);      /* Gamma Input Port */
	map(0x3000, 0x3000).r(FUNC(mhavoc_state::mhavoc_alpha_r)).mirror(0x07ff);  /* Alpha Comm. Read Port */
	map(0x3800, 0x3803).portr("DIAL").mirror(0x07fc);     /* Roller Controller Input */
	map(0x4000, 0x4000).portr("DSW2").w(FUNC(mhavoc_state::mhavoc_gamma_irq_ack_w)).mirror(0x07ff); /* DSW at 8S, IRQ Acknowledge */
	map(0x4800, 0x4800).w(FUNC(mhavoc_state::mhavoc_out_1_w)).mirror(0x07ff); /* Coin Counters    */
	map(0x5000, 0x5000).w(FUNC(mhavoc_state::mhavoc_alpha_w)).mirror(0x07ff); /* Alpha Comm. Write Port */
	map(0x5800, 0x5800).w(FUNC(mhavoc_state::mhavocrv_speech_data_w)).mirror(0x06ff); /* TMS5220 data write */
	map(0x5900, 0x5900).w(FUNC(mhavoc_state::mhavocrv_speech_strobe_w)).mirror(0x06ff); /* TMS5220 /WS strobe write */
	map(0x6000, 0x61ff).rw("eeprom", FUNC(eeprom_parallel_28xx_device::read), FUNC(eeprom_parallel_28xx_device::write)).mirror(0x1e00); /* EEROM */
	map(0x8000, 0xffff).rom();                   /* Program ROM (32K) */
}

void mhavoc_state::alphape_map(address_map &map)
{
	map(0x0000, 0x01ff).ram();
	map(0x0200, 0x07ff).bankrw("bank1").share("zram0");
	map(0x0800, 0x09ff).ram();
	map(0x0a00, 0x0fff).bankrw("bank1").share("zram1");
	map(0x1000, 0x1000).r(FUNC(mhavoc_state::mhavoc_gamma_r));            /* Gamma Read Port */
	map(0x1200, 0x1200).portr("IN0").nopw();    /* Alpha Input Port 0 */
	map(0x1400, 0x141f).ram().share("colorram");    /* ColorRAM */
	map(0x1600, 0x1600).w(FUNC(mhavoc_state::mhavoc_out_0_w));           /* Control Signals */
	map(0x1640, 0x1640).w("avg", FUNC(avg_mhavoc_device::go_w));               /* Vector Generator GO */
	map(0x1680, 0x1680).w("watchdog", FUNC(watchdog_timer_device::reset_w));         /* Watchdog Clear */
	map(0x16c0, 0x16c0).w("avg", FUNC(avg_mhavoc_device::reset_w));            /* Vector Generator Reset */
	map(0x1700, 0x1700).w(FUNC(mhavoc_state::mhavoc_alpha_irq_ack_w));   /* IRQ ack */
	//map(0x1740, 0x1740).w(FUNC(mhavoc_state::mhavocpe_rom_banksel_w));     /* Program ROM Page Select */
	map(0x1740, 0x1740).lw8("bnk", [this] (u8 data) { membank("bank2")->set_entry((data & 1) | ((data & 2)<<1) | ((data & 4)>>1)); });
	map(0x1780, 0x1780).w(FUNC(mhavoc_state::mhavoc_ram_banksel_w));     /* Program RAM Page Select */
	map(0x17c0, 0x17c0).w(FUNC(mhavoc_state::mhavoc_gamma_w));           /* Gamma Communication Write Port */
	map(0x1800, 0x1fff).ram();                             /* Shared Beta Ram */
	map(0x2000, 0x3fff).bankr("bank2");                        /* Paged Program ROM (32K) */
	map(0x4000, 0x4fff).ram().share("vectorram").region("alpha", 0x4000);    /* Vector Generator RAM */
	map(0x5000, 0x7fff).rom();                             /* Vector ROM */
	map(0x8000, 0xffff).rom();                 /* Program ROM (32K) */
}

MACHINE_CONFIG_START(mhavoc_state::mhavocpe)
	/* basic machine hardware */
	MCFG_DEVICE_ADD("alpha", M6502, MHAVOC_CLOCK_2_5M)     /* 2.5 MHz */
	MCFG_DEVICE_PROGRAM_MAP(alphape_map)

	MCFG_DEVICE_ADD("gamma", M6502, MHAVOC_CLOCK_1_25M)    /* 1.25 MHz */
	MCFG_DEVICE_PROGRAM_MAP(gammape_map)

	EEPROM_2804(config, "eeprom");

	MCFG_TIMER_DRIVER_ADD_PERIODIC("5k_timer", mhavoc_state, mhavoc_cpu_irq_clock, attotime::from_hz(MHAVOC_CLOCK_5K))

	WATCHDOG_TIMER(config, "watchdog");

	/* video hardware */
	MCFG_VECTOR_ADD("vector")
	MCFG_SCREEN_ADD("screen", VECTOR)
	MCFG_SCREEN_REFRESH_RATE(50)
	MCFG_SCREEN_SIZE(400, 300)
	MCFG_SCREEN_VISIBLE_AREA(0, 300, 0, 260)
	MCFG_SCREEN_UPDATE_DEVICE("vector", vector_device, screen_update)

	avg_device &avg(AVG_MHAVOC(config, "avg", 0));
	avg.set_vector_tag("vector");

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	/* FIXME: Outputs 1,2,3 are tied together
	* This signal and Output 4 are processed separately.
	* Later they are mixed together again.
	* ==> DISCRETE emulation, below is just an approximation.
	*/

	MCFG_DEVICE_ADD("pokey1", POKEY, MHAVOC_CLOCK_1_25M)
	MCFG_POKEY_ALLPOT_R_CB(IOPORT("DSW1"))
	MCFG_POKEY_OUTPUT_OPAMP(RES_K(1), CAP_U(0.001), 5.0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)

	MCFG_DEVICE_ADD("pokey2", POKEY, MHAVOC_CLOCK_1_25M)
	MCFG_POKEY_OUTPUT_OPAMP(RES_K(1), CAP_U(0.001), 5.0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)

	MCFG_DEVICE_ADD("pokey3", POKEY, MHAVOC_CLOCK_1_25M)
	MCFG_POKEY_OUTPUT_OPAMP(RES_K(1), CAP_U(0.001), 5.0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)

	MCFG_DEVICE_ADD("pokey4", POKEY, MHAVOC_CLOCK_1_25M)
	MCFG_POKEY_OUTPUT_OPAMP(RES_K(1), CAP_U(0.001), 5.0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)

	MCFG_DEVICE_ADD("tms", TMS5220, MHAVOC_CLOCK / 2 / 9)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)
MACHINE_CONFIG_END


ROM_START( mhavocpe )
	/* Alpha Processor ROMs */
	ROM_REGION( 0x28000, "alpha", 0 )   /* 152KB for ROMs */
	/* Vector Generator ROM */
	ROM_LOAD( "mhpe.6kl",   0x05000, 0x2000, CRC(5605a866) SHA1(4518df3507b896c5ba32f70372e16bbb428f295d) )

	/* Program ROM */
	ROM_LOAD( "mhpe.1mn",   0x08000, 0x4000, CRC(69a709fe) SHA1(b44cd253a8af6c4c85c5a1e9e2f9c4956eb8c478) )
	ROM_LOAD( "mhpe.1l",    0x0c000, 0x4000, CRC(6d7b7b70) SHA1(1b1ba87d8f63504791b87b0475df146fc377b1cf) )
    
	/* Paged Program ROM */
	ROM_LOAD( "mhpe.1q",    0x10000, 0x8000, CRC(88edbfb5) SHA1(240bb1cf36640f8070ba53b3d6419b7305f272d6) ) /* page 0+1+4+5 */
	ROM_LOAD( "mhpe.1np",   0x18000, 0x8000, CRC(c4c06eec) SHA1(7c5b0c5f69bc7faa0292eebcb12db5904ebb3129) ) /* page 2+3+6+7 */

	/* Paged Vector Generator ROM */
	ROM_LOAD( "mhpe.6h",    0x20000, 0x4000, CRC(9a9b5e8f) SHA1(38991917b1eda6b9d4016fba28781d85b2f5f9fe) ) /* page 0+1 */
	ROM_LOAD( "mhpe.6jk",   0x24000, 0x4000, CRC(0b32fde1) SHA1(65b6ced0db9900cfea25a3edb7ef4749e7ce3c90) ) /* page 2+3 */

	/* Gamma Processor ROM */
	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe.9s",    0x08000, 0x8000, CRC(66b8190e) SHA1(d52c51b859c5125447a9b3e2f6e1de92312db358) ) /* 32K Here */

	/* AVG PROM */
	ROM_REGION( 0x100, "user1", 0 )
	ROM_LOAD( "036408-01.b1",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END


GAME( 2018, mhavocpe, mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc (The Promised End)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )

