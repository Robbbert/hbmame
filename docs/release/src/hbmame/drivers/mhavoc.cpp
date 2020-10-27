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
	map(0x1200, 0x1200).portr("IN0").nopw();                            /* Alpha Input Port 0 */
	map(0x1400, 0x141f).ram().share("avg:colorram");                    /* ColorRAM */
	map(0x1600, 0x1600).w(FUNC(mhavoc_state::mhavoc_out_0_w));          /* Control Signals */
	map(0x1640, 0x1640).w("avg", FUNC(avg_mhavoc_device::go_w));        /* Vector Generator GO */
	map(0x1680, 0x1680).w("watchdog", FUNC(watchdog_timer_device::reset_w));         /* Watchdog Clear */
	map(0x16c0, 0x16c0).w("avg", FUNC(avg_mhavoc_device::reset_w));     /* Vector Generator Reset */
	map(0x1700, 0x1700).w(FUNC(mhavoc_state::mhavoc_alpha_irq_ack_w));  /* IRQ ack */
	//map(0x1740, 0x1740).w(FUNC(mhavoc_state::mhavocpe_rom_banksel_w));     /* Program ROM Page Select */
	map(0x1740, 0x1740).lw8(NAME([this] (u8 data) { membank("bank2")->set_entry((data & 1) | ((data & 2)<<1) | ((data & 4)>>1)); }));
	map(0x1780, 0x1780).w(FUNC(mhavoc_state::mhavoc_ram_banksel_w));    /* Program RAM Page Select */
	map(0x17c0, 0x17c0).w(FUNC(mhavoc_state::mhavoc_gamma_w));          /* Gamma Communication Write Port */
	map(0x1800, 0x1fff).ram();                              /* Shared Beta Ram */
	map(0x2000, 0x3fff).bankr("bank2");                     /* Paged Program ROM (32K) */
	map(0x4000, 0x4fff).ram().share("avg:vectorram").region("alpha", 0x4000);    /* Vector Generator RAM */
	map(0x5000, 0x5fff).rom();                              /* Vector ROM */
	map(0x6000, 0x7fff).bankr("bank3");                     /* Paged Vector ROM */
	map(0x8000, 0xffff).rom();                              /* Program ROM (32K) */
}
//  membank("bank3")->set_entry(m_map);

void mhavoc_state::mhavocpe(machine_config &config)
{
	mhavocrv(config);

	/* basic machine hardware */
	m_alpha->set_addrmap(AS_PROGRAM, &mhavoc_state::alphape_map);
	m_gamma->set_addrmap(AS_PROGRAM, &mhavoc_state::gammape_map);
}


ROM_START( mhavocpex )
	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe.6kl",   0x05000, 0x2000, CRC(4c05b1a8) SHA1(89b524182fcfd966d6a7e3188235c957c451b8a9) )

	ROM_LOAD( "mhpe.1mn",   0x08000, 0x4000, CRC(3b691eff) SHA1(e8227d1458e3ed4d0e8444ec23f2c2d45a0d93b8) )
	ROM_LOAD( "mhpe.1l",    0x0c000, 0x4000, CRC(fb53dae6) SHA1(08e9bd60e801778d3521d64817a10ba1ed74f4ff) )

	ROM_LOAD( "mhpe.1q",    0x10000, 0x8000, CRC(660e3d57) SHA1(6eddf1335c536406080eab73f5501a202fb0583d) )
	ROM_LOAD( "mhpe.1np",   0x18000, 0x8000, CRC(c1a70bad) SHA1(0b72b6817e2f00d2c001ac61ebd2cd42ff7785c9) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe.6h",    0x0000, 0x4000, CRC(79fc58c0) SHA1(7b40dfb89bc4078e2bd6f89a570f2be9cca15df9) )
	ROM_LOAD( "mhpe.6jk",   0x4000, 0x4000, CRC(dc78b802) SHA1(6b951982232de08d32d3a2d01814cc28f89d2120) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe.9s",    0x8000, 0x8000, CRC(d42ee58e) SHA1(667aec3c3e93df3f8dedddb0db1188291e37630b) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex2 )
	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe2.6kl",  0x05000, 0x2000, CRC(3804822b) SHA1(78ca5ad3ac9ce9f6ce6ca497f70c7233adc52502) )

	ROM_LOAD( "mhpe2.1mn",  0x08000, 0x4000, CRC(2b505cae) SHA1(1ca14c6c810a3dec964eae63e9941e411d3ec546) )
	ROM_LOAD( "mhpe2.1l",   0x0c000, 0x4000, CRC(8c078dcf) SHA1(b4681eb4930938a93e74c313ee1031ad1a71e198) )

	ROM_LOAD( "mhpe2.1q",   0x10000, 0x8000, CRC(7602fccd) SHA1(9073c5e2a70f23c7bab2927936ee5a22e2adb57a) )
	ROM_LOAD( "mhpe2.1np",  0x18000, 0x8000, CRC(bc6775e1) SHA1(a59a93d4414a0318e37c1e50ef139b6611e4233f) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe2.6h",   0x0000, 0x4000, CRC(ec19097f) SHA1(261c957e5311ac5fd700949e04e9d4a6ef12c043) )
	ROM_LOAD( "mhpe2.6jk",  0x4000, 0x4000, CRC(2ac3f07b) SHA1(27d06ed9edf8cd227c7db7c28a7f7357da1260c6) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe2.9s",   0x8000, 0x8000, CRC(57ca8c09) SHA1(84ae19a9ff3b61c75f1fd147540a1e5fb42e6915) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex3 )
	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe3.6kl",  0x05000, 0x2000, CRC(0bebf550) SHA1(043dd185a40208de8f7952026f1645799eb3445f) )

	ROM_LOAD( "mhpe3.1mn",  0x08000, 0x4000, CRC(b3a485e8) SHA1(47e20e02a69675d5df51e68d33bc407b441d621c) )
	ROM_LOAD( "mhpe3.1l",   0x0c000, 0x4000, CRC(baa2d31a) SHA1(d6337763e4d6cc86c95d2abf54955a7b413c9846) )

	ROM_LOAD( "mhpe3.1q",   0x10000, 0x8000, CRC(ebb47bb5) SHA1(e73ea9bde6c525b644824deb73a4caa3f9a7e28f) )
	ROM_LOAD( "mhpe3.1np",  0x18000, 0x8000, CRC(793b480d) SHA1(9a0398dad5b8634190d9432e1b94a786054b4e6e) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe3.6h",   0x0000, 0x4000, CRC(f1bcfaf2) SHA1(a70c3dd399c961f8a12dfc343c106d7e77e77a42) )
	ROM_LOAD( "mhpe3.6jk",  0x4000, 0x4000, CRC(8a8a67cf) SHA1(af5e64e0329015e3d2cc5292d671fe64cafd6463) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe3.9s",   0x8000, 0x8000, CRC(603ac7da) SHA1(8343cc8a388673436df9b5ad2894136943c676da) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex4 )
	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe4.6kl",  0x05000, 0x2000, CRC(75ab6d18) SHA1(874f4a15532f257480c8f6ca0cf9b3cd7ea39908) )

	ROM_LOAD( "mhpe4.1mn",  0x08000, 0x4000, CRC(120b00f0) SHA1(b8546c81ecc8e16f5e0afb7ce9b941623d862d31) )
	ROM_LOAD( "mhpe4.1l",   0x0c000, 0x4000, CRC(ffcb7a73) SHA1(ec284426c381da3c15ec50c3bbbdf38fb15efcb2) )

	ROM_LOAD( "mhpe4.1q",   0x10000, 0x8000, CRC(dc85e7b3) SHA1(c9cbe410915686f470c033cc5ccacf76f81c46ba) )
	ROM_LOAD( "mhpe4.1np",  0x18000, 0x8000, CRC(134d84f9) SHA1(4933dda173c9abc813edf6cd0bcef24977b6fe1a) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe4.6h",   0x0000, 0x4000, CRC(df87c581) SHA1(bc13cd3ddf4a1e8c93d452eafafd295c37e43057) )
	ROM_LOAD( "mhpe4.6jk",  0x4000, 0x4000, CRC(8b1f8b3d) SHA1(c0825a1d7b49eb2cf169361563cd0f8e6df5a159) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe4.9s",   0x8000, 0x8000, CRC(e91ba3e8) SHA1(6283e5365cda8fe9706c1cd15ac7bd3978e029bc) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END

ROM_START( mhavocpex5 )
	ROM_REGION( 0x20000, "alpha", 0 )
	ROM_LOAD( "mhpe5.6kl",  0x05000, 0x2000, CRC(025ae0de) SHA1(5390dee62e5499c8767b4fc34f38fef49579d05b) )

	ROM_LOAD( "mhpe5.1mn",  0x08000, 0x4000, CRC(b2b3067e) SHA1(209a5d6d53825164ba7c92cff67e69571c6a0b32) )
	ROM_LOAD( "mhpe5.1l",   0x0c000, 0x4000, CRC(367c0028) SHA1(e2f153d4c1b3ba19b48a34d1bd776318499513c9) )

	ROM_LOAD( "mhpe5.1q",   0x10000, 0x8000, CRC(04e4476b) SHA1(b9ac489e2b3ea8c04e6c907b3dbf6dfdbba976af) )
	ROM_LOAD( "mhpe5.1np",  0x18000, 0x8000, CRC(8c8ca853) SHA1(12d1dbd3f76e85021ba2226defd194de8543c9ac) )

	ROM_REGION( 0x8000, "avg", 0 )
	ROM_LOAD( "mhpe5.6h",   0x0000, 0x4000, CRC(ea89c415) SHA1(f91ae8b9d1942e4ee14eaf9c1d0e687f2039c89d) )
	ROM_LOAD( "mhpe5.6jk",  0x4000, 0x4000, CRC(c56e754a) SHA1(6f02782aa14e10f6a9a2f5065d3ec9e2d9e07d5e) )

	ROM_REGION( 0x10000, "gamma", 0 )
	ROM_LOAD( "mhpe5.9s",   0x8000, 0x8000, CRC(8abffddd) SHA1(4cea72b569de302feac53b4e4a861887904c6bbe) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "136002-125.6c",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END


GAME( 2018, mhavocpex,  mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc - The Promised End (v0.21)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mhavocpex2, mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc - The Promised End (v0.50)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex3, mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc - The Promised End (v0.52)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex4, mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc - The Promised End (v0.67)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mhavocpex5, mhavoc, mhavocpe, mhavocrv, mhavoc_state, init_mhavocrv, ROT0, "HaxRus", "Major Havoc - The Promised End (v0.72)", MACHINE_SUPPORTS_SAVE )

