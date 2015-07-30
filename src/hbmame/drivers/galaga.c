// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/galaga.c"

// Produce pacman death sound in galagost
WRITE8_MEMBER( galaga_state::galaga_sample_w )
{
	switch ( data )
	{
		case 0x20:				/* loud bang */
			m_samples->start(0, 0);;
		case 0x10:				/* soft bang */
			break;
		default:
			m_06xx->data_w ( space, offset, data );
	}
}

static const char *const galagost_sample_names[] =
{
	"*galagost",
	"bang",
	0   /* end of array */
};

static ADDRESS_MAP_START( galagost_map, AS_PROGRAM, 8, galaga_state )
	AM_RANGE(0x0000, 0x3fff) AM_ROM AM_WRITENOP         /* the only area different for each CPU */
	AM_RANGE(0x6800, 0x6807) AM_READ(bosco_dsw_r)
	AM_RANGE(0x6800, 0x681f) AM_DEVWRITE("namco", namco_device, pacman_sound_w)
	AM_RANGE(0x6820, 0x6827) AM_WRITE(bosco_latch_w)                        /* misc latches */
	AM_RANGE(0x6830, 0x6830) AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x7000, 0x70ff) AM_DEVREAD("06xx", namco_06xx_device, data_r) AM_WRITE(galaga_sample_w)
	AM_RANGE(0x7100, 0x7100) AM_DEVREADWRITE("06xx", namco_06xx_device, ctrl_r, ctrl_w)
	AM_RANGE(0x8000, 0x87ff) AM_RAM_WRITE(galaga_videoram_w) AM_SHARE("videoram")
	AM_RANGE(0x8800, 0x8bff) AM_RAM AM_SHARE("galaga_ram1")
	AM_RANGE(0x9000, 0x93ff) AM_RAM AM_SHARE("galaga_ram2")
	AM_RANGE(0x9800, 0x9bff) AM_RAM AM_SHARE("galaga_ram3")
	AM_RANGE(0xa000, 0xa005) AM_WRITEONLY AM_SHARE("starcontrol")
	AM_RANGE(0xa007, 0xa007) AM_WRITE(galaga_flip_screen_w)
ADDRESS_MAP_END

static MACHINE_CONFIG_DERIVED( galagost, galaga )
	MCFG_CPU_MODIFY( "maincpu" )
	MCFG_CPU_PROGRAM_MAP(galagost_map)

	/* sound hardware */
	MCFG_SOUND_ADD("samples", SAMPLES, 0)
	MCFG_SAMPLES_CHANNELS(1)
	MCFG_SAMPLES_NAMES(galagost_sample_names)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.75)
MACHINE_CONFIG_END

// galaga hacks

ROM_START( galagabl )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "galagabl.1_1", 0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "galagabl.1_2", 0x1000, 0x1000, CRC(f1d87ec3) SHA1(44034b4f015d9e876253d2827e05f3d41133e097) )
	ROM_LOAD( "galagabl.1_3", 0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "galagabl.1_4", 0x3000, 0x1000, CRC(83874442) SHA1(366cb0dbd31b787e64f88d182108b670d03b393e) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "galagabl.1_5", 0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "galagabl.1_7", 0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x10000, "sub3", 0 )	/* 64k for a Z80 which emulates the custom I/O chip (not used) */
	ROM_LOAD( "galagabl.1_6", 0x0000, 0x1000, CRC(001b70bc) SHA1(b465eee91e75257b7b049d49c0064ab5fd66c576) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "galagabl.1_8", 0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "galagabl.1_a", 0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "galagabl.1_9", 0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )	/* char lookup table */
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )	/* sprite lookup table */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( galagab2 )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "g1",           0x0000, 0x1000, CRC(ab036c9f) SHA1(ca7f5da42d4e76fd89bb0b35198a23c01462fbfe) )
	ROM_LOAD( "g2",           0x1000, 0x1000, CRC(d9232240) SHA1(ab202aa259c3d332ef13dfb8fc8580ce2a5a253d) )
	ROM_LOAD( "04j_g03.bin",  0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "g4",           0x3000, 0x1000, CRC(499fcc76) SHA1(ddb8b121903646c320939c7d13f4aa4ebb130378) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "04e_g05.bin",  0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "04d_g06.bin",  0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x10000, "sub3", 0 )	/* 64k for a Z80 which emulates the custom I/O chip (not used) */
	ROM_LOAD( "10h_g07.bin",  0x0000, 0x1000, CRC(035e300c) SHA1(cfda2467e71c27381b7150ff8fc7b69d61df123a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gallag.8",     0x0000, 0x1000, CRC(169a98a4) SHA1(edbeb11076061e744ea88d9899dbdfe0964c7e78) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "07e_g10.bin",  0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "07h_g09.bin",  0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "5n.bin",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )	/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )	/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galaga84 )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "g1",           0x0000, 0x1000, CRC(ab036c9f) SHA1(ca7f5da42d4e76fd89bb0b35198a23c01462fbfe) )
	ROM_LOAD( "gal84_u2",     0x1000, 0x1000, CRC(4d832a30) SHA1(88ee11df88cf08005efccd6305f87fb3e2797db6) )
	ROM_LOAD( "04j_g03.bin",  0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "g4",           0x3000, 0x1000, CRC(499fcc76) SHA1(ddb8b121903646c320939c7d13f4aa4ebb130378) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "gal84_u5",     0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "04d_g06.bin",  0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x10000, "sub3", 0 )	/* 64k for a Z80 which emulates the custom I/O chip (not used) */
	ROM_LOAD( "10h_g07.bin",  0x0000, 0x1000, CRC(035e300c) SHA1(cfda2467e71c27381b7150ff8fc7b69d61df123a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "07m_g08.bin",  0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gal84u4d",     0x0000, 0x1000, CRC(22e339d5) SHA1(9ac2887ede802d28daa4ad0a0a54bcf7b1155a2e) )
	ROM_LOAD( "gal84u4e",     0x1000, 0x1000, CRC(60dcf940) SHA1(6530aa5b4afef4a8422ece76a93d0c5b1d93355e) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "5n.bin",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )	/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )	/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( nebulbee )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "nebulbee.01",  0x0000, 0x1000, CRC(f405f2c4) SHA1(9249afeffd8df0f24539ea9b4f88c23a6ad58d8c) )
	ROM_LOAD( "nebulbee.02",  0x1000, 0x1000, CRC(31022b60) SHA1(90e64afb4128c6dfeeee89635ea9f97a34f70f5f) )
	ROM_LOAD( "04j_g03.bin",  0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "nebulbee.04",  0x3000, 0x1000, CRC(d76788a5) SHA1(adcb83cf64951d86c701a99b410e9230912f8a48) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "04e_g05.bin",  0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "04d_g06.bin",  0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "07m_g08.bin",  0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "07e_g10.bin",  0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "07h_g09.bin",  0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "5n.bin",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )	/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )	/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galagads )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "07m_g08.bin",  0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "07e_g10.bin",  0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "07h_g09.bin",  0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "5n.bin",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )	/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )	/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galaga99 )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "gg1-1",            0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "g99-04k_g02.bin",  0x1000, 0x1000, CRC(ab0df3ac) SHA1(29c5bce17baa6778fad5faff1a241021e143e29f) )
	ROM_LOAD( "g99-04j_g03.bin",  0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "g99-04h_g04.bin",  0x3000, 0x1000, CRC(b9a315fb) SHA1(3422dbd1fe116f9193dcccb1dba691eb98eab3aa) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "g99-07m_g08.bin",  0x0000, 0x1000, CRC(cc091e8b) SHA1(b71b962d5cd71fa78f0709b5cccf549cb7f4e9f0) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "g99-07e_g10.bin",  0x0000, 0x1000, CRC(2b493359) SHA1(e006ee999277120f01d1669ffe28df0b96415af5) )
	ROM_LOAD( "g99-07h_g09.bin",  0x1000, 0x1000, CRC(cdbb428d) SHA1(6c8dfb5da2bee99f07585e23ecfa21c67218cddb) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galagaf )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "gg_5fast.bin", 0x0000, 0x1000, CRC(ae82145e) SHA1(146bd848e9783f34b1a006d793ee3f36e53baad0) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1-9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1-11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1-10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )	/* char lookup table */
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )	/* sprite lookup table */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

/* This set has the fix proposed by Don Hodges at the page:
	http://donhodges.com/galaga_stage_256_fix.htm */
ROM_START( galagap )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "gg1-1b.3p",    0x0000, 0x1000, CRC(ab036c9f) SHA1(ca7f5da42d4e76fd89bb0b35198a23c01462fbfe) )
	ROM_LOAD( "gg1-2b.3m",    0x1000, 0x1000, CRC(d9232240) SHA1(ab202aa259c3d332ef13dfb8fc8580ce2a5a253d) )
	ROM_LOAD( "gg1-3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1-4b.2l",    0x3000, 0x1000, CRC(499fcc76) SHA1(ddb8b121903646c320939c7d13f4aa4ebb130378) )
	ROM_FILL(0x25c4,1,0xa7)
	ROM_FILL(0x25d0,1,0x00)
	ROM_FILL(0x2fff,1,0xb2)

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "gg1-5b.3f",    0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "gg1-7b.2c",    0x0000, 0x1000, CRC(d016686b) SHA1(44c1a04fba3c7c826ff484185cb881b4b22e6657) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1-9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1-11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1-10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )	/* char lookup table */
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )	/* sprite lookup table */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( galagix )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "gg1-1",        0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "gg1-2",        0x1000, 0x1000, CRC(43bb0d5c) SHA1(666975aed5ce84f09794c54b550d64d95ab311f0) )
	ROM_LOAD( "gg1-3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1-4",        0x3000, 0x1000, CRC(83874442) SHA1(366cb0dbd31b787e64f88d182108b670d03b393e) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gx_07m_g08.bin",  0x0000, 0x1000, CRC(4b0a6b29) SHA1(1707bdfab7a17579b4171417d86ae76e065299c5) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gx_07e_g10.bin",  0x0000, 0x1000, CRC(b52a7dff) SHA1(1bf1cf9db72d16d1e6539d4fc05bbee071cc3420) )
	ROM_LOAD( "gx_07h_g09.bin",  0x1000, 0x1000, CRC(ffd5c70e) SHA1(5eac91087129ec8038feabdb18cb3199e52cae23) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galagost )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gost_07m_g08.bin",  0x0000, 0x1000, CRC(c7588555) SHA1(269362b7fe63007b317ee4b63ef7d09f24d9c362) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gost_07e_g10.bin",  0x0000, 0x1000, CRC(0758b90f) SHA1(89cc8db9457fa5f56b49930590f32aba1bc55323) )
	ROM_LOAD( "gost_07h_g09.bin",  0x1000, 0x1000, CRC(53f1332b) SHA1(04136e52008c4ad4813529d633fd9b8c71e6ccc0) )
	
	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galagosb )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gost_07m_g08.bin",  0x0000, 0x1000, CRC(c7588555) SHA1(269362b7fe63007b317ee4b63ef7d09f24d9c362) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gost_07e_g10.bin",  0x0000, 0x1000, CRC(0758b90f) SHA1(89cc8db9457fa5f56b49930590f32aba1bc55323) )
	ROM_LOAD( "gosb_07h_g09.bin",  0x1000, 0x1000, CRC(0339291c) SHA1(4958a78b6afb9f2b4f46d3c0f654843c077686a9) )
	
	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galaped )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )
	
	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "ped_07m_g08.bin",  0x0000, 0x1000, CRC(d4fda6a5) SHA1(c1d39bfa2ad866e726b77f15ca22ef97ac3f51a4) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "ped_07e_g10.bin",  0x0000, 0x1000, CRC(1d0a2438) SHA1(66a4d7120f926a67dac0203b73c60917e5c140cf) )
	ROM_LOAD( "ped_07h_g09.bin",  0x1000, 0x1000, CRC(11d9b093) SHA1(8f151f4813ca7a80a228f2123cc089de414bc3f9) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galgaxin )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "2600j_4l.bin",  0x0000, 0x1000, CRC(10fa1481) SHA1(09f56503251cc29cf2131d2a0c5bcca4ee276e70) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "2800l_4d.bin",  0x0000, 0x1000, CRC(bf803b06) SHA1(ab0051b3b08f03ad93d100f38b106e761d4c4607) )
	ROM_LOAD( "2700k.bin",     0x1000, 0x1000, CRC(020906a3) SHA1(72e63918556ae366ede3ffad64a9d3ad4df1e514) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( galagaef )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1-9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "terror.bin",   0x0000, 0x1000, CRC(1b629f80) SHA1(6db66601fd357400edf3dc1d689255c64a46a72d) )
	ROM_LOAD( "gg1-10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )	/* char lookup table */
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )	/* sprite lookup table */

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( terror )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "gg1-1",        0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "gg1-2",        0x1000, 0x1000, CRC(43bb0d5c) SHA1(666975aed5ce84f09794c54b550d64d95ab311f0) )
	ROM_LOAD( "gg1-3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1-4",        0x3000, 0x1000, CRC(83874442) SHA1(366cb0dbd31b787e64f88d182108b670d03b393e) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1-9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "terror.bin",   0x0000, 0x1000, CRC(1b629f80) SHA1(6db66601fd357400edf3dc1d689255c64a46a72d) )
	ROM_LOAD( "gg1-10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( vgalaga )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "v_07m_g08.bin",  0x0000, 0x1000, CRC(bf59992f) SHA1(78f1e01affca694693254ecfedebdb05cb8d3b82) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "v_07e_g10.bin",  0x0000, 0x1000, CRC(3cf64d44) SHA1(54035738c6b0eed3331f11c53477cf87f07a8422) )
	ROM_LOAD( "v_07h_g09.bin",  0x1000, 0x1000, CRC(206bf7fc) SHA1(7510ed4a6c084f555cd5a6cd6fbc862f4797656f) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( vgalagam )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vm_07m_g08.bin",  0x0000, 0x1000, CRC(c1bdeb1c) SHA1(7f07a9da708066401710b0b62b98e324ebbc2bca) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "vm_07e_g10.bin",  0x0000, 0x1000, CRC(99367e04) SHA1(7e140e2b9dd4c6b0da41b21ed2cf1bf33f088a7e) )
	ROM_LOAD( "vm_07h_g09.bin",  0x1000, 0x1000, CRC(683585c4) SHA1(fdc89b72bf64e44c80f4ae601280fca9b0d1c18c) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

ROM_START( vgalagaf )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code for the first CPU  */
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "3600fast.bin",    0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )     /* 64k for the third CPU  */
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vgalagaf.08",  0x0000, 0x1000, CRC(c1bdeb1c) SHA1(7f07a9da708066401710b0b62b98e324ebbc2bca) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "vgalagaf.10",  0x0000, 0x1000, CRC(99367e04) SHA1(7e140e2b9dd4c6b0da41b21ed2cf1bf33f088a7e) )
	ROM_LOAD( "vgalagaf.09",  0x1000, 0x1000, CRC(683585c4) SHA1(fdc89b72bf64e44c80f4ae601280fca9b0d1c18c) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	/* palette */
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )	/* char lookup table */
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )/* sprite lookup table */
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )/* unknown */

	ROM_REGION( 0x0100, "namco", 0 )	/* sound prom */
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END

// xevious hacks

ROM_START( battlesh )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "b_1.bin",      0x0000, 0x2000, CRC(b6e4f4f3) SHA1(ceaaa63b50e75dcb05aeb68574336dfe56a8434a) )
	ROM_LOAD( "b_2.bin",      0x2000, 0x2000, CRC(47017bc8) SHA1(0da73ae079fb6a64eed56197e2c88609ef34166c) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "b_3.bin",      0x0000, 0x2000, CRC(0ede5706) SHA1(65b235c5abe487612e11d0235410f1ca59b06e95) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x10000, "sub3", 0 )	/* 64k for the CUSTOM I/O Emulation CPU */
	ROM_LOAD( "b_5.bin",      0x0000, 0x1000, CRC(23107dfb) SHA1(74c49a5648faab632ae5ed8dd18a1d8b39837e2d) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "b_9.bin",      0x0000, 0x1000, CRC(5bd6e9ae) SHA1(f16c7eec39fce856c775b2b81ab55fb42376850e) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "b_10.bin",     0x0000, 0x1000, CRC(b43ea55d) SHA1(06f4c4e7fc71b9e173c3bdf91c40f47750051b5e) )	/* bg pattern B0 */
	ROM_LOAD( "b_11.bin",     0x1000, 0x1000, CRC(73603931) SHA1(1f7824b107a5a3d5c3434f02f17173a1f85fd29c) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "battlesh_15.4m", 0x0000, 0x2000, CRC(308c5bad) SHA1(583a6c2b9a3c9fac6ed33de6ae5ee11543fe6164) ) /* sprite set #1, planes 0/1 */
	ROM_LOAD( "battlesh_17.4p", 0x2000, 0x2000, CRC(b06b9ccb) SHA1(186442f0aa7bf0365adfb2da874110c01797b140) ) /* sprite set #2, planes 1/2 */
	ROM_LOAD( "battlesh_16.4n", 0x4000, 0x1000, CRC(1cbbddce) SHA1(5fcf20b3ab335cfac01f1c2bcfca76b90c92618b) ) /* sprite set #3, planes 0/1 */
	ROM_LOAD( "battlesh_18.4r", 0x5000, 0x2000, CRC(f56c3697) SHA1(4f5108a1bf6b828dbed4a34836c42fd359fb68c5) ) /* sprite set #1, plane 2, set #2, plane 0 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x1400, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "b_-bpr.bin",   0x0300, 0x0400, CRC(d2d208b1) SHA1(6c8d29912c03ee93759e24085bc66ab738768bcc) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "b_6bpr.bin",   0x0700, 0x0400, CRC(0260c041) SHA1(1a7516e8b18ffdd9789eec8b834c17b3ba312afe) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "b_4bpr.bin",   0x0b00, 0x0400, CRC(33764974) SHA1(567b048b8a93e30090ccee4f6aadc0353524d8d1) ) /* sprite lookup table low bits */
	ROM_LOAD( "b_5bpr.bin",   0x0f00, 0x0400, CRC(43674c7e) SHA1(94c19a9da81839cb1dfde3f11b2fd82ffe45efb9) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( gaous )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvi_1.3p",     0x0000, 0x1000, CRC(09964dda) SHA1(4882b25b0938a903f3a367455ba788a30759b5b0) )
	ROM_LOAD( "xvi_2.3m",     0x1000, 0x1000, CRC(60ecce84) SHA1(8adc60a5fcbca74092518dbc570ffff0f04c5b17) )
	ROM_LOAD( "xvi_3.2m",     0x2000, 0x1000, CRC(79754b7d) SHA1(c6a154858716e1f073b476824b183de20e06d093) )
	ROM_LOAD( "xvi_4.2l",     0x3000, 0x1000, CRC(c7d4bbf0) SHA1(4b846de204d08651253d3a141677c8a31626af07) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvi_5.3f",     0x0000, 0x1000, CRC(c85b703f) SHA1(15f1c005b9d806a384ab1f2240b9c580bfe83893) )
	ROM_LOAD( "xvi_6.3j",     0x1000, 0x1000, CRC(e18cdaad) SHA1(6b79efee1a9642edb9f752101737132401248aed) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gao_12.3b",    0x0000, 0x1000, CRC(e24d7e32) SHA1(c78798944f717d2b20ceb846e4f98966756b0964) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gao_13.3c",    0x0000, 0x1000, CRC(43ed5c90) SHA1(15f541fb7314c59fe56245f4dbfe05fa35ee2396) )	/* bg pattern B0 */
	ROM_LOAD( "gao_14.3d",    0x1000, 0x1000, CRC(f361bdd0) SHA1(0ebae760617e8ff29d3f33856094f511f7a06dfa) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "gao_15.4m",    0x0000, 0x2000, CRC(ea41df4e) SHA1(8e7215654db77c7f8782dc4b909d1248952c10f0) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "gao_17.4p",    0x2000, 0x2000, CRC(79d65929) SHA1(ca60435cd4a64c82e4d263a57fe899e9e18b3389) )	/* sprite set #2, planes 0/1 */
	ROM_LOAD( "gao_16.4n",    0x4000, 0x1000, CRC(ceb8a6f8) SHA1(5153cfe4aaeefd4f1faec133a120e8551c460d2f) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "gao_18.4r",    0x5000, 0x2000, CRC(2d95245d) SHA1(7e5409f1f787047d7825d59f1dfeefa26732d1cc) )	/* sprite set #1, plane 2, set #2, plane 2 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "gao_4bpr.3l",  0x0700, 0x0200, CRC(bb0f377f) SHA1(451569602c83172132db76ee7652d174867639c0) ) /* sprite lookup table low bits */
	ROM_LOAD( "gao_5bpr.3m",  0x0900, 0x0200, CRC(3fb201f3) SHA1(9e1c79c13bc285b7bff104c84baab9dd04fd42ab) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( sxvioush )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "cpu_3p.rom",   0x0000, 0x1000, CRC(1c8d27d5) SHA1(2c41303d8c74acb5840295a4b460a39a9a8e21bb) )
	ROM_LOAD( "cpu_3m.rom",   0x1000, 0x1000, CRC(fd04e615) SHA1(7169e7f3bd1e9cfae9671b89f2a45f56b968e1ff) )
	ROM_LOAD( "cpu_2m.rom",   0x2000, 0x1000, CRC(294d5404) SHA1(ecc39fb2c0065a36f20541747089b4e30dfb99b1) )
	ROM_LOAD( "cpu_2l.rom",   0x3000, 0x1000, CRC(6a44bf92) SHA1(0ca726f7f9528789f2a718df55e59406a283cdfa) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "cpu_3f.rom",   0x0000, 0x1000, CRC(d4bd3d81) SHA1(5831bb306bd650779207936bfd00f25864733abb) )
	ROM_LOAD( "cpu_3j.rom",   0x1000, 0x1000, CRC(af06be5f) SHA1(5a020822387ab8c69214db961180760fa9853e6e) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "xh_xvi_12.3b",    0x0000, 0x1000, CRC(9b8f4deb) SHA1(f6e1b557be3cbfc2d7755da8e48dc9a57f2cc2c0) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "xvi_13.3c",    0x0000, 0x1000, CRC(de60ba25) SHA1(32bc09be5ff8b52ee3a26e0ac3ebc2d4107badb7) )	/* bg pattern B0 */
	ROM_LOAD( "xvi_14.3d",    0x1000, 0x1000, CRC(535cdbbc) SHA1(fb9ffe5fc43e0213231267e98d605d43c15f61e8) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "xh_xvi_15.4m",    0x0000, 0x2000, CRC(308c5bad) SHA1(583a6c2b9a3c9fac6ed33de6ae5ee11543fe6164) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "xh_xvi_17.4p",    0x2000, 0x2000, CRC(b06b9ccb) SHA1(186442f0aa7bf0365adfb2da874110c01797b140) )	/* sprite set #2, planes 1/2 */
	ROM_LOAD( "xh_xvi_16.4n",    0x4000, 0x1000, CRC(1cbbddce) SHA1(5fcf20b3ab335cfac01f1c2bcfca76b90c92618b) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "xh_xvi_18.4r",    0x5000, 0x2000, CRC(f56c3697) SHA1(4f5108a1bf6b828dbed4a34836c42fd359fb68c5) )	/* sprite set #1, plane 2, set #2, plane 0 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvi_4bpr.3l",  0x0700, 0x0200, CRC(fd8b9d91) SHA1(87ddf0b9d723aabb422d6d416aa9ec6bc246bf34) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvi_5bpr.3m",  0x0900, 0x0200, CRC(bf906d82) SHA1(776168a73d3b9f0ce05610acc8a623deae0a572b) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xviousah )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xea-1m-a.bin", 0x0000, 0x2000, CRC(8c2b50ec) SHA1(f770873b711d838556dde67a8aac8a7f572fcc5b) )
	ROM_LOAD( "xea-1l-a.bin", 0x2000, 0x2000, CRC(0821642b) SHA1(c6c322c61d0985a2ac59f5e92d4e351107afb9eb) )
	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xea-4c-a.bin", 0x0000, 0x2000, CRC(14d8fa03) SHA1(e8114141394adda86184b146f2497cfeef7fc2eb) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "xh_xvi_12.3b",    0x0000, 0x1000, CRC(9b8f4deb) SHA1(f6e1b557be3cbfc2d7755da8e48dc9a57f2cc2c0) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "xvi_13.3c",    0x0000, 0x1000, CRC(de60ba25) SHA1(32bc09be5ff8b52ee3a26e0ac3ebc2d4107badb7) )	/* bg pattern B0 */
	ROM_LOAD( "xvi_14.3d",    0x1000, 0x1000, CRC(535cdbbc) SHA1(fb9ffe5fc43e0213231267e98d605d43c15f61e8) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "xh_xvi_15.4m",    0x0000, 0x2000, CRC(308c5bad) SHA1(583a6c2b9a3c9fac6ed33de6ae5ee11543fe6164) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "xh_xvi_17.4p",    0x2000, 0x2000, CRC(b06b9ccb) SHA1(186442f0aa7bf0365adfb2da874110c01797b140) )	/* sprite set #2, planes 1/2 */
	ROM_LOAD( "xh_xvi_16.4n",    0x4000, 0x1000, CRC(1cbbddce) SHA1(5fcf20b3ab335cfac01f1c2bcfca76b90c92618b) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "xh_xvi_18.4r",    0x5000, 0x2000, CRC(f56c3697) SHA1(4f5108a1bf6b828dbed4a34836c42fd359fb68c5) )	/* sprite set #1, plane 2, set #2, plane 0 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvi_4bpr.3l",  0x0700, 0x0200, CRC(fd8b9d91) SHA1(87ddf0b9d723aabb422d6d416aa9ec6bc246bf34) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvi_5bpr.3m",  0x0900, 0x0200, CRC(bf906d82) SHA1(776168a73d3b9f0ce05610acc8a623deae0a572b) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xevioush )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvi_1.3p",     0x0000, 0x1000, CRC(09964dda) SHA1(4882b25b0938a903f3a367455ba788a30759b5b0) )
	ROM_LOAD( "xvi_2.3m",     0x1000, 0x1000, CRC(60ecce84) SHA1(8adc60a5fcbca74092518dbc570ffff0f04c5b17) )
	ROM_LOAD( "xvi_3.2m",     0x2000, 0x1000, CRC(79754b7d) SHA1(c6a154858716e1f073b476824b183de20e06d093) )
	ROM_LOAD( "xvi_4.2l",     0x3000, 0x1000, CRC(c7d4bbf0) SHA1(4b846de204d08651253d3a141677c8a31626af07) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvi_5.3f",     0x0000, 0x1000, CRC(c85b703f) SHA1(15f1c005b9d806a384ab1f2240b9c580bfe83893) )
	ROM_LOAD( "xvi_6.3j",     0x1000, 0x1000, CRC(e18cdaad) SHA1(6b79efee1a9642edb9f752101737132401248aed) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "xh_xvi_12.3b",    0x0000, 0x1000, CRC(9b8f4deb) SHA1(f6e1b557be3cbfc2d7755da8e48dc9a57f2cc2c0) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "xvi_13.3c",    0x0000, 0x1000, CRC(de60ba25) SHA1(32bc09be5ff8b52ee3a26e0ac3ebc2d4107badb7) )	/* bg pattern B0 */
	ROM_LOAD( "xvi_14.3d",    0x1000, 0x1000, CRC(535cdbbc) SHA1(fb9ffe5fc43e0213231267e98d605d43c15f61e8) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "xh_xvi_15.4m",    0x0000, 0x2000, CRC(308c5bad) SHA1(583a6c2b9a3c9fac6ed33de6ae5ee11543fe6164) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "xh_xvi_17.4p",    0x2000, 0x2000, CRC(b06b9ccb) SHA1(186442f0aa7bf0365adfb2da874110c01797b140) )	/* sprite set #2, planes 1/2 */
	ROM_LOAD( "xh_xvi_16.4n",    0x4000, 0x1000, CRC(1cbbddce) SHA1(5fcf20b3ab335cfac01f1c2bcfca76b90c92618b) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "xh_xvi_18.4r",    0x5000, 0x2000, CRC(f56c3697) SHA1(4f5108a1bf6b828dbed4a34836c42fd359fb68c5) )	/* sprite set #1, plane 2, set #2, plane 0 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvi_4bpr.3l",  0x0700, 0x0200, CRC(fd8b9d91) SHA1(87ddf0b9d723aabb422d6d416aa9ec6bc246bf34) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvi_5bpr.3m",  0x0900, 0x0200, CRC(bf906d82) SHA1(776168a73d3b9f0ce05610acc8a623deae0a572b) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xeviousp )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvip_1.3p",    0x0000, 0x1000, CRC(09c5fef9) SHA1(1111912763c818afc13e4dfd088877cf78bc30b4) )
	ROM_LOAD( "xvip_2.3m",    0x1000, 0x1000, CRC(a2cf842d) SHA1(9bb24b3527f2cad8c9b916f9a815ea4f747dc337) )
	ROM_LOAD( "xvip_3.2m",    0x2000, 0x1000, CRC(7ab657cd) SHA1(a27ad33b2808417dff8e27d0e2b503092f505bb0) )
	ROM_LOAD( "xvip_4.2l",    0x3000, 0x1000, CRC(884837bd) SHA1(750b0b118a88ab09a0dcbdc10737910b2fc28a1a) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvip_5.3f",    0x0000, 0x1000, CRC(68c5268b) SHA1(265c86b2f547230d88276471864a45460198e129) )
	ROM_LOAD( "xvip_6.3j",    0x1000, 0x1000, CRC(cafc97e6) SHA1(a3d8130b49fb941f6f05204793bf3226ca8eac5c) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )
	
	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "xvip_12.3b",   0x0000, 0x1000, CRC(08f184f1) SHA1(bcbf89b92c311a78db516b557b982ed51907c49d) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "xvi_13.3c",    0x0000, 0x1000, CRC(de60ba25) SHA1(32bc09be5ff8b52ee3a26e0ac3ebc2d4107badb7) )	/* bg pattern B0 */
	ROM_LOAD( "xvi_14.3d",    0x1000, 0x1000, CRC(535cdbbc) SHA1(fb9ffe5fc43e0213231267e98d605d43c15f61e8) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "xvi_15.4m",    0x0000, 0x2000, CRC(dc2c0ecb) SHA1(19ddbd9805f77f38c9a9a1bb30dba6c720b8609f) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "xvi_17.4p",    0x2000, 0x2000, CRC(dfb587ce) SHA1(acff2bf5cde85a16cdc98a52cdea11f77fadf25a) )	/* sprite set #2, planes 0/1 */
	ROM_LOAD( "xvi_16.4n",    0x4000, 0x1000, CRC(605ca889) SHA1(3bf380ef76c03822a042ecc73b5edd4543c268ce) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "xvip_16.4n",   0x5000, 0x1000, CRC(00babaa1) SHA1(b946510a9c5b6c1dd6e6bab1ba3217a9d449a477) )	/* sprite set #3, planes 0/1 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvip_9.2a",    0x0000, 0x1000, CRC(529aecf0) SHA1(60c18f834933e01ecd22aa652d39a5d1a9a0fe0f) )
	ROM_LOAD( "xvip_10.2b",   0x1000, 0x2000, CRC(f09eb6f9) SHA1(b189b4c23a2f9f185c75b096f44a083f8dbca3ae) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvi_4bpr.3l",  0x0700, 0x0200, CRC(fd8b9d91) SHA1(87ddf0b9d723aabb422d6d416aa9ec6bc246bf34) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvi_5bpr.3m",  0x0900, 0x0200, CRC(bf906d82) SHA1(776168a73d3b9f0ce05610acc8a623deae0a572b) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xevious3 )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvi_1.3p",     0x0000, 0x1000, CRC(09964dda) SHA1(4882b25b0938a903f3a367455ba788a30759b5b0) )
	ROM_LOAD( "xvi_2.3m",     0x1000, 0x1000, CRC(60ecce84) SHA1(8adc60a5fcbca74092518dbc570ffff0f04c5b17) )
	ROM_LOAD( "xvi_3.2m",     0x2000, 0x1000, CRC(79754b7d) SHA1(c6a154858716e1f073b476824b183de20e06d093) )
	ROM_LOAD( "xvi_4.2l",     0x3000, 0x1000, CRC(c7d4bbf0) SHA1(4b846de204d08651253d3a141677c8a31626af07) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvi_5.3f",     0x0000, 0x1000, CRC(c85b703f) SHA1(15f1c005b9d806a384ab1f2240b9c580bfe83893) )
	ROM_LOAD( "xvi_6.3j",     0x1000, 0x1000, CRC(e18cdaad) SHA1(6b79efee1a9642edb9f752101737132401248aed) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "x3_xvi_12.3b", 0x0000, 0x1000, CRC(34f7940e) SHA1(6e289b36d14fea02f2fdf45caa4720ab05529fe2) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "x3_xvi_13.3c", 0x0000, 0x1000, CRC(6eab56b5) SHA1(00da56424e520c9de5967c1c233a20cb3ae037db) )	/* bg pattern B0 */
	ROM_LOAD( "x3_xvi_14.3d", 0x1000, 0x1000, CRC(790b2f6c) SHA1(bc8e52d0ca4e97f38256ec3da1b23d266767ea9b) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "x3_xvi_15.4m", 0x0000, 0x2000, CRC(4dfaf4e1) SHA1(26456dc6c75e1a02cfc9c07fedfb2b7ece8e1692) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "x3_xvi_17.4p", 0x2000, 0x2000, CRC(9a135fe7) SHA1(278ab8c2805d138a47dfc89db7016c7438142a4f) )	/* sprite set #2, planes 1/2 */
	ROM_LOAD( "x3_xvi_16.4n", 0x4000, 0x1000, CRC(dd6f34c8) SHA1(9fcc194ea2ee94f8bdf4c46ff8dead113a99bdce) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "x3_xvi_18.4r", 0x5000, 0x2000, CRC(d5378335) SHA1(892a762a050c8f3ae21544392444b289583b8ca0) )	/* sprite set #1, plane 2, set #2, plane 0 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvi_4bpr.3l",  0x0700, 0x0200, CRC(fd8b9d91) SHA1(87ddf0b9d723aabb422d6d416aa9ec6bc246bf34) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvi_5bpr.3m",  0x0900, 0x0200, CRC(bf906d82) SHA1(776168a73d3b9f0ce05610acc8a623deae0a572b) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xevious4 )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvi_1.3p",     0x0000, 0x1000, CRC(09964dda) SHA1(4882b25b0938a903f3a367455ba788a30759b5b0) )
	ROM_LOAD( "xvi_2.3m",     0x1000, 0x1000, CRC(60ecce84) SHA1(8adc60a5fcbca74092518dbc570ffff0f04c5b17) )
	ROM_LOAD( "xvi_3.2m",     0x2000, 0x1000, CRC(79754b7d) SHA1(c6a154858716e1f073b476824b183de20e06d093) )
	ROM_LOAD( "xvi_4.2l",     0x3000, 0x1000, CRC(c7d4bbf0) SHA1(4b846de204d08651253d3a141677c8a31626af07) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvi_5.3f",     0x0000, 0x1000, CRC(c85b703f) SHA1(15f1c005b9d806a384ab1f2240b9c580bfe83893) )
	ROM_LOAD( "xvi_6.3j",     0x1000, 0x1000, CRC(e18cdaad) SHA1(6b79efee1a9642edb9f752101737132401248aed) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "x4_xvi_12.3b",    0x0000, 0x1000, CRC(bdf15ccc) SHA1(0e6fba428cf56668e5c9218af3c6d58c8f261d32) )     /* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "x4_xvi_13.3c",    0x0000, 0x1000, CRC(b9832011) SHA1(ac7b266791fd39c17cf239405068e309478e6703) )	/* bg pattern B0 */
	ROM_LOAD( "x4_xvi_14.3d",    0x1000, 0x1000, CRC(04677061) SHA1(3bf47661ddf1a8e98fcf8f7e46610e353483da9b) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "x4_xvi_15.4m",    0x0000, 0x2000, CRC(6cdeae2d) SHA1(5d43b11655e2952cec3c4e5df5855805273b0cb8) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "x4_xvi_17.4p",    0x2000, 0x2000, CRC(28ac2e0a) SHA1(8f85656d60439abc507ec09e9dfda31dad3e4e04) )	/* sprite set #2, planes 0/1 */
	ROM_LOAD( "x4_xvi_16.4n",    0x4000, 0x1000, CRC(400e2c3e) SHA1(d46fc7a40ad2b8f822db21e8af9ec29e21f7f093) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "x4_xvi_18.4r",    0x5000, 0x2000, CRC(45b2daaf) SHA1(8bda7ee9900a64c74996e5790832f11983caf31b) )	/* sprite set #1, plane 2, set #2, plane 2 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "x4_xvi_8bpr.6a",  0x0000, 0x0100, CRC(0e572608) SHA1(580acf168d13e5ba4448c6dbd077fe035dd9df3a) ) /* palette red component */
	ROM_LOAD( "x4_xvi_9bpr.6d",  0x0100, 0x0100, CRC(0e12c2bb) SHA1(829d14781e67268ceba5f771466584025894fd96) ) /* palette green component */
	ROM_LOAD( "x4_xvi10bpr.6e",  0x0200, 0x0100, CRC(6e235d02) SHA1(08378c12dfe190d33caf7bb0916dcc336dca42cd) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "x4_xvi_4bpr.3l",  0x0700, 0x0200, CRC(cce27181) SHA1(312d2f776f282354615f3a33a58a2744aafbc5a9) ) /* sprite lookup table low bits */
	ROM_LOAD( "x4_xvi_5bpr.3m",  0x0900, 0x0200, CRC(e15ddec9) SHA1(531551ab5b0f0dc6c450e24ee973451ec0c21471) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xevious5 )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvi_1.3p",     0x0000, 0x1000, CRC(09964dda) SHA1(4882b25b0938a903f3a367455ba788a30759b5b0) )
	ROM_LOAD( "xvi_2.3m",     0x1000, 0x1000, CRC(60ecce84) SHA1(8adc60a5fcbca74092518dbc570ffff0f04c5b17) )
	ROM_LOAD( "xvi_3.2m",     0x2000, 0x1000, CRC(79754b7d) SHA1(c6a154858716e1f073b476824b183de20e06d093) )
	ROM_LOAD( "xvi_4.2l",     0x3000, 0x1000, CRC(c7d4bbf0) SHA1(4b846de204d08651253d3a141677c8a31626af07) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvi_5.3f",     0x0000, 0x1000, CRC(c85b703f) SHA1(15f1c005b9d806a384ab1f2240b9c580bfe83893) )
	ROM_LOAD( "xvi_6.3j",     0x1000, 0x1000, CRC(e18cdaad) SHA1(6b79efee1a9642edb9f752101737132401248aed) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "xvi24_12.3b",  0x0000, 0x1000, CRC(935cb02d) SHA1(0800e2a643b2bbeae0f337bfbdc88d2eab16ada4) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "xvi_13.3c",    0x0000, 0x1000, CRC(de60ba25) SHA1(32bc09be5ff8b52ee3a26e0ac3ebc2d4107badb7) )	/* bg pattern B0 */
	ROM_LOAD( "xvi24_14.3d",  0x1000, 0x1000, CRC(e0fbfb86) SHA1(6e25609f2f70490d8f70a48e93b1abf9673b1d38) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "xvi24_15.4m",  0x0000, 0x2000, CRC(51dcbd85) SHA1(a681000eb279d0f71da4ec4164f57e9877cb391f) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "xvi24_17.4p",  0x2000, 0x2000, CRC(b4753406) SHA1(404e911377614eda1e92093c5e3504216f16e143) )	/* sprite set #2, planes 0/1 */
	ROM_LOAD( "xvi24_16.4n",  0x4000, 0x1000, CRC(eb0d690d) SHA1(1cc3a0e3647e36a925eab8e5aee73febeb41d4cc) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "xvi24_18.4r",  0x5000, 0x2000, CRC(490c1189) SHA1(a478d390a4931c43778cf24e5b01225c71dcac67) )	/* sprite set #1, plane 2, set #2, plane 2 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvi_4bpr.3l",  0x0700, 0x0200, CRC(fd8b9d91) SHA1(87ddf0b9d723aabb422d6d416aa9ec6bc246bf34) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvi_5bpr.3m",  0x0900, 0x0200, CRC(bf906d82) SHA1(776168a73d3b9f0ce05610acc8a623deae0a572b) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( xeviblk )
	ROM_REGION( 0x10000, "maincpu", 0 )	/* 64k for the first CPU */
	ROM_LOAD( "xvi_1.3p",     0x0000, 0x1000, CRC(09964dda) SHA1(4882b25b0938a903f3a367455ba788a30759b5b0) )
	ROM_LOAD( "xvi_2.3m",     0x1000, 0x1000, CRC(60ecce84) SHA1(8adc60a5fcbca74092518dbc570ffff0f04c5b17) )
	ROM_LOAD( "xvi_3.2m",     0x2000, 0x1000, CRC(79754b7d) SHA1(c6a154858716e1f073b476824b183de20e06d093) )
	ROM_LOAD( "xvi_4.2l",     0x3000, 0x1000, CRC(c7d4bbf0) SHA1(4b846de204d08651253d3a141677c8a31626af07) )

	ROM_REGION( 0x10000, "sub", 0 )	/* 64k for the second CPU */
	ROM_LOAD( "xvi_5.3f",     0x0000, 0x1000, CRC(c85b703f) SHA1(15f1c005b9d806a384ab1f2240b9c580bfe83893) )
	ROM_LOAD( "xvi_6.3j",     0x1000, 0x1000, CRC(e18cdaad) SHA1(6b79efee1a9642edb9f752101737132401248aed) )

	ROM_REGION( 0x10000, "sub2", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "xvi_7.2c",     0x0000, 0x1000, CRC(dd35cf1c) SHA1(f8d1f8e019d8198308443c2e7e815d0d04b23d14) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "xvi_12.3b",    0x0000, 0x1000, CRC(088c8b26) SHA1(9c3b61dfca2f84673a78f7f66e363777a8f47a59) )	/* foreground characters */

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "xvi_13.3c",    0x0000, 0x1000, CRC(de60ba25) SHA1(32bc09be5ff8b52ee3a26e0ac3ebc2d4107badb7) )	/* bg pattern B0 */
	ROM_LOAD( "xvi_14.3d",    0x1000, 0x1000, CRC(535cdbbc) SHA1(fb9ffe5fc43e0213231267e98d605d43c15f61e8) )	/* bg pattern B1 */

	ROM_REGION( 0xa000, "gfx3", 0 )
	ROM_LOAD( "xvi_15.4m",    0x0000, 0x2000, CRC(dc2c0ecb) SHA1(19ddbd9805f77f38c9a9a1bb30dba6c720b8609f) )	/* sprite set #1, planes 0/1 */
	ROM_LOAD( "xvi_17.4p",    0x2000, 0x2000, CRC(dfb587ce) SHA1(acff2bf5cde85a16cdc98a52cdea11f77fadf25a) )	/* sprite set #2, planes 0/1 */
	ROM_LOAD( "xvi_16.4n",    0x4000, 0x1000, CRC(605ca889) SHA1(3bf380ef76c03822a042ecc73b5edd4543c268ce) )	/* sprite set #3, planes 0/1 */
	ROM_LOAD( "xvi_18.4r",    0x5000, 0x2000, CRC(02417d19) SHA1(b5f830dd2cf25cf154308d2e640f0ecdcda5d8cd) )	/* sprite set #1, plane 2, set #2, plane 2 */
	/* 0x7000-0x8fff  will be unpacked from 0x5000-0x6fff */
	ROM_FILL(                 0x9000, 0x1000, 0x00 )	// empty space to decode sprite set #3 as 3 bits per pixel

	ROM_REGION( 0x4000, "gfx4", 0 )	/* background tilemaps */
	ROM_LOAD( "xvi_9.2a",     0x0000, 0x1000, CRC(57ed9879) SHA1(3106d1aacff06cf78371bd19967141072b32b7d7) )
	ROM_LOAD( "xvi_10.2b",    0x1000, 0x2000, CRC(ae3ba9e5) SHA1(49064b25667ffcd81137cd5e800df4b78b182a46) )
	ROM_LOAD( "xvi_11.2c",    0x3000, 0x1000, CRC(31e244dd) SHA1(3f7eac12863697a98e1122111801606759e44b2a) )

	ROM_REGION( 0x0b00, "proms", 0 )
	ROM_LOAD( "xvi_8bpr.6a",  0x0000, 0x0100, CRC(5cc2727f) SHA1(0dc1e63a47a4cb0ba75f6f1e0c15e408bb0ee2a1) ) /* palette red component */
	ROM_LOAD( "xvi_9bpr.6d",  0x0100, 0x0100, CRC(5c8796cc) SHA1(63015e3c0874afc6b1ca032f1ffb8f90562c77c8) ) /* palette green component */
	ROM_LOAD( "xvi10bpr.6e",  0x0200, 0x0100, CRC(3cb60975) SHA1(c94d5a5dd4d8a08d6d39c051a4a722581b903f45) ) /* palette blue component */
	ROM_LOAD( "xvi_7bpr.4h",  0x0300, 0x0200, CRC(22d98032) SHA1(ec6626828c79350417d08b98e9631ad35edd4a41) ) /* bg tiles lookup table low bits */
	ROM_LOAD( "xvi_6bpr.4f",  0x0500, 0x0200, CRC(3a7599f0) SHA1(a4bdf58c190ca16fc7b976c97f41087a61fdb8b8) ) /* bg tiles lookup table high bits */
	ROM_LOAD( "xvibl_4bpr.3l",  0x0700, 0x0200, CRC(83277522) SHA1(7c5edce9758fdb5aa2132da9d8aeb0f64d69d52e) ) /* sprite lookup table low bits */
	ROM_LOAD( "xvibl_5bpr.3m",  0x0900, 0x0200, CRC(07b4bd0b) SHA1(36b467756c54120a20b82b4183a524b4867bc729) ) /* sprite lookup table high bits */

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "xvi_2bpr.7n",  0x0000, 0x0100, CRC(550f06bc) SHA1(816a0fafa0b084ac11ae1af70a5186539376fc2a) )
	ROM_LOAD( "xvi_1bpr.5n",  0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END


GAME( 1984, galaga84, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "hack", "Galaga '84", MACHINE_SUPPORTS_SAVE )
GAME( 1999, galaga99, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Jerky and Sil", "Galaga '99", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagab2, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "bootleg", "Galaga (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, galagabl, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "bootleg", "Galaga (Namco 1982)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagads, galaga,  galaga,   galagamw, galaga_state,  galaga,  ROT90, "hack", "Galaga (fast shoot)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, galagaef, galaga,  galaga,   galagamw, galaga_state,  galaga,  ROT90, "Hack", "Galaga (Enduring Freedom)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagaf,  galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Namco", "Galaga (Fast Shoot Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, galagap,  galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Don Hodges", "Galaga (patched)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagix,  galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Grand Master Peter", "Galagix", MACHINE_SUPPORTS_SAVE )
GAME( 2002, galagost, galaga,  galagost, galaga,   galaga_state,  galaga,  ROT90, "Twisty", "Galaga Ghost (Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, galagosb, galaga,  galagost, galaga,   galaga_state,  galaga,  ROT90, "Twisty", "Galaga Ghost (Set 2 - 20021128)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, galaped,  galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Cary Barnhard", "Galapede (Fast Shoot)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, galgaxin, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "T-Bone", "Galagalaxian", MACHINE_SUPPORTS_SAVE )
GAME( 1984, nebulbee, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "hack", "Nebulous Bee", MACHINE_SUPPORTS_SAVE )
GAME( 2000, terror,   galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Hack", "Terrorist", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vgalaga,  galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Hack", "Vector Galaga", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vgalagam, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Hack", "Vector Galaga Midway", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vgalagaf, galaga,  galaga,   galaga,   galaga_state,  galaga,  ROT90, "Hack", "Vector Galaga Fast Shoot", MACHINE_SUPPORTS_SAVE )
GAME( 2002, battlesh, xevious, battles,  xevious,  xevious_state, battles, ROT90, "bootleg", "Battles 2002", MACHINE_SUPPORTS_SAVE )
GAME( 2005, gaous,    xevious, xevious,  xevious,  xevious_state, xevious, ROT90, "Namco (Hack)", "Gaous", MACHINE_SUPPORTS_SAVE )
GAME( 2002, sxvioush, xevious, xevious,  sxevious, xevious_state, xevious, ROT90, "Namco", "Super Xevious 2002", MACHINE_SUPPORTS_SAVE )
GAME( 2004, xeviblk,  xevious, xevious,  xevious,  xevious_state, xevious, ROT90, "Namco", "Xevious Black", MACHINE_SUPPORTS_SAVE )
GAME( 2002, xevious3, xevious, xevious,  xevious,  xevious_state, xevious, ROT90, "Namco", "Xevious 2003", MACHINE_SUPPORTS_SAVE )
GAME( 2004, xevious4, xevious, xevious,  xevious,  xevious_state, xevious, ROT90, "Namco", "Xevious 2004", MACHINE_SUPPORTS_SAVE )
GAME( 2004, xevious5, xevious, xevious,  xevious,  xevious_state, xevious, ROT90, "Namco", "Xevious 2005", MACHINE_SUPPORTS_SAVE )
GAME( 2002, xevioush, xevious, xevious,  xevious,  xevious_state, xevious, ROT90, "Namco", "Xevious 2002", MACHINE_SUPPORTS_SAVE )
GAME( 2001, xeviousp, xevious, xevious,  sxevious, xevious_state, xevious, ROT90, "bootleg", "Xevious Plus!", MACHINE_SUPPORTS_SAVE )
GAME( 2002, xviousah, xevious, xevious,  xeviousa, xevious_state, xevious, ROT90, "Namco (Atari license)", "Xevious 2002 (Atari set 1)", MACHINE_SUPPORTS_SAVE )
