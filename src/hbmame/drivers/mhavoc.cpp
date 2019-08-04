// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/mhavoc.cpp"

//**********************************************************************
//* Drivers included
//*
//* Major Havoc - The Promised End - CAX 2018 BETA v0.21 - mhavocpex
//* Major Havoc - The Promised End - CAX 2019 BETA v0.50 - mhavocpex2
//*
//**********************************************************************

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
	map(0x1000, 0x1000).r(FUNC(mhavoc_state::mhavoc_gamma_r));          /* Gamma Read Port */
	map(0x1200, 0x1200).portr("IN0").nopw();    						/* Alpha Input Port 0 */
	map(0x1400, 0x141f).ram().share("avg:colorram");    					/* ColorRAM */
	map(0x1600, 0x1600).w(FUNC(mhavoc_state::mhavoc_out_0_w));         	/* Control Signals */
	map(0x1640, 0x1640).w("avg", FUNC(avg_mhavoc_device::go_w));        /* Vector Generator GO */
	map(0x1680, 0x1680).w("watchdog", FUNC(watchdog_timer_device::reset_w));         /* Watchdog Clear */
	map(0x16c0, 0x16c0).w("avg", FUNC(avg_mhavoc_device::reset_w));     /* Vector Generator Reset */
	map(0x1700, 0x1700).w(FUNC(mhavoc_state::mhavoc_alpha_irq_ack_w));  /* IRQ ack */
	//map(0x1740, 0x1740).w(FUNC(mhavoc_state::mhavocpe_rom_banksel_w));     /* Program ROM Page Select */
	map(0x1740, 0x1740).lw8("bnk", [this] (u8 data) { membank("bank2")->set_entry((data & 1) | ((data & 2)<<1) | ((data & 4)>>1)); });
	map(0x1780, 0x1780).w(FUNC(mhavoc_state::mhavoc_ram_banksel_w));  	/* Program RAM Page Select */
	map(0x17c0, 0x17c0).w(FUNC(mhavoc_state::mhavoc_gamma_w));          /* Gamma Communication Write Port */
	map(0x1800, 0x1fff).ram();                             	/* Shared Beta Ram */
	map(0x2000, 0x3fff).bankr("bank2");                    	/* Paged Program ROM (32K) */
	map(0x4000, 0x4fff).ram().share("avg:vectorram").region("alpha", 0x4000);    /* Vector Generator RAM */
	map(0x5000, 0x5fff).rom();                             	/* Vector ROM */
	map(0x6000, 0x7fff).bankr("bank3");                    	/* Paged Vector ROM */
	map(0x8000, 0xffff).rom();                 				/* Program ROM (32K) */
}
//		membank("bank3")->set_entry(m_map);

void mhavoc_state::mhavocpe(machine_config &config)
{
	mhavocrv(config);

	/* basic machine hardware */
	m_alpha->set_addrmap(AS_PROGRAM, &mhavoc_state::alphape_map);
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_state::gammape_map);
}


ROM_START( mhavocpex )
	/* Alpha Processor ROMs */
	ROM_REGION( 0x20000, "alpha", 0 )   /* 152KB for ROMs */
	/* Vector Generator ROM */
	ROM_LOAD( "mhpe.6kl",   0x05000, 0x2000, CRC(4c05b1a8) SHA1(89b524182fcfd966d6a7e3188235c957c451b8a9) )

	/* Program ROM */
	ROM_LOAD( "mhpe.1mn",   0x08000, 0x4000, CRC(3b691eff) SHA1(e8227d1458e3ed4d0e8444ec23f2c2d45a0d93b8) )
	ROM_LOAD( "mhpe.1l",    0x0c000, 0x4000, CRC(fb53dae6) SHA1(08e9bd60e801778d3521d64817a10ba1ed74f4ff) )
    
	/* Paged Program ROM */
	ROM_LOAD( "mhpe.1q",    0x10000, 0x8000, CRC(660e3d57) SHA1(6eddf1335c536406080eab73f5501a202fb0583d) ) /* page 0+1+4+5 */
	ROM_LOAD( "mhpe.1np",   0x18000, 0x8000, CRC(c1a70bad) SHA1(0b72b6817e2f00d2c001ac61ebd2cd42ff7785c9) ) /* page 2+3+6+7 */

	/* Paged Vector Generator ROM */
	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe.6h",    0x0000, 0x4000, CRC(79fc58c0) SHA1(7b40dfb89bc4078e2bd6f89a570f2be9cca15df9) ) /* page 0+1 */
	ROM_LOAD( "mhpe.6jk",   0x4000, 0x4000, CRC(dc78b802) SHA1(6b951982232de08d32d3a2d01814cc28f89d2120) ) /* page 2+3 */

	/* Gamma Processor ROM */
	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe.9s",    0x08000, 0x8000, CRC(d42ee58e) SHA1(667aec3c3e93df3f8dedddb0db1188291e37630b) ) /* 32K Here */

	/* AVG PROM */
	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "036408-01.b1",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex2 )
	/* Alpha Processor ROMs */
	ROM_REGION( 0x20000, "alpha", 0 )   /* 152KB for ROMs */
	/* Vector Generator ROM */
	ROM_LOAD( "mhpe.6kl",   0x05000, 0x2000, CRC(3804822b) SHA1(78ca5ad3ac9ce9f6ce6ca497f70c7233adc52502) )

	/* Program ROM */
	ROM_LOAD( "mhpe.1mn",   0x08000, 0x4000, CRC(f3adf942) SHA1(9bf1e2c473cb04a73d587e6a4859caf92f78add9) )
	ROM_LOAD( "mhpe.1l",    0x0c000, 0x4000, CRC(d738d189) SHA1(44c31640b972476e6539b67f972a75c831a764d3) )
    
	/* Paged Program ROM */
	ROM_LOAD( "mhpe.1q",    0x10000, 0x8000, CRC(63d1e5bd) SHA1(d66a95b7fff33a76970beba8c73a063dcf25f5dc) ) /* page 0+1+4+5 */
	ROM_LOAD( "mhpe.1np",   0x18000, 0x8000, CRC(57c067c6) SHA1(ec5a86c7e887a89c65716762ba14d02dae77c60a) ) /* page 2+3+6+7 */

	/* Paged Vector Generator ROM */
	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe.6h",    0x0000, 0x4000, CRC(ec19097f) SHA1(261c957e5311ac5fd700949e04e9d4a6ef12c043) ) /* page 0+1 */
	ROM_LOAD( "mhpe.6jk",   0x4000, 0x4000, CRC(2ac3f07b) SHA1(27d06ed9edf8cd227c7db7c28a7f7357da1260c6) ) /* page 2+3 */

	/* Gamma Processor ROM */
	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe.9s",    0x08000, 0x8000, CRC(57ca8c09) SHA1(84ae19a9ff3b61c75f1fd147540a1e5fb42e6915) ) /* 32K Here */

	/* AVG PROM */
	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "036408-01.b1",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END


GAME( 2019, mhavocpex2, mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc - The Promised End (v0.50)", MACHINE_SUPPORTS_SAVE )

