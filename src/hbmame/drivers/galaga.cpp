// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/namco/galaga.cpp"
#include "includes/galaga.h"

// Produce pacman death sound in galagost
void galaga_hbmame::galaga_sample_w(offs_t offset, u8 data)
{
	m_06xx->data_w ( offset, data & 0xcf );
	// When ship blows up, the game sends the sequence 10 10 20 20
	// 10 is a slightly softer bang, 20 is a loud bang. They essentially
	// occur at the same time. We only need one of them.
	if (data == 0x20)
		m_samples->start(0, 0);
}

static const char *const galagost_sample_names[] =
{
	"*galagost",
	"bang",
	nullptr   /* end of array */
};

void galaga_hbmame::galagost_map(address_map &map) {
	map(0x0000,0x3fff).rom().nopw();  /* the only area different for each CPU */
	map(0x6800,0x6807).r(FUNC(galaga_hbmame::bosco_dsw_r));
	map(0x6800, 0x681f).w(m_namco_sound, FUNC(namco_wsg_device::pacman_sound_w));
	map(0x6820,0x6827).w("misclatch",FUNC(ls259_device::write_d0));
	map(0x6830,0x6830).w("watchdog",FUNC(watchdog_timer_device::reset_w));
	map(0x7000,0x70ff).r("06xx",FUNC(namco_06xx_device::data_r)).w(FUNC(galaga_hbmame::galaga_sample_w));
	map(0x7100,0x7100).rw("06xx",FUNC(namco_06xx_device::ctrl_r),FUNC(namco_06xx_device::ctrl_w));
	map(0x8000,0x87ff).ram().w(FUNC(galaga_hbmame::galaga_videoram_w)).share("videoram");
	map(0x8800,0x8bff).ram().share("galaga_ram1");
	map(0x9000,0x93ff).ram().share("galaga_ram2");
	map(0x9800,0x9bff).ram().share("galaga_ram3");
	map(0xa000,0xa007).w("videolatch",FUNC(ls259_device::write_d0));
}

void galaga_hbmame::galagost(machine_config &config)
{
	galaga(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &galaga_hbmame::galagost_map);

	/* sound hardware */
	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(galagost_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 0.75);
}


/******
 Galaga
*********/
ROM_START( galagabl )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1-1",        0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "galagabl.1_2", 0x1000, 0x1000, CRC(f1d87ec3) SHA1(44034b4f015d9e876253d2827e05f3d41133e097) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1-4",        0x3000, 0x1000, CRC(83874442) SHA1(366cb0dbd31b787e64f88d182108b670d03b393e) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x10000, "sub3", 0 )
	ROM_LOAD( "galagabl.1_6", 0x0000, 0x1000, CRC(001b70bc) SHA1(b465eee91e75257b7b049d49c0064ab5fd66c576) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


ROM_START( galagab2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1_1b.3p",    0x0000, 0x1000, CRC(ab036c9f) SHA1(ca7f5da42d4e76fd89bb0b35198a23c01462fbfe) )
	ROM_LOAD( "gg1_2b.3m",    0x1000, 0x1000, CRC(d9232240) SHA1(ab202aa259c3d332ef13dfb8fc8580ce2a5a253d) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1_4b.2l",    0x3000, 0x1000, CRC(499fcc76) SHA1(ddb8b121903646c320939c7d13f4aa4ebb130378) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x10000, "sub3", 0 )
	ROM_LOAD( "10h_g07.bin",  0x0000, 0x1000, CRC(035e300c) SHA1(cfda2467e71c27381b7150ff8fc7b69d61df123a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gallag.8",     0x0000, 0x1000, CRC(169a98a4) SHA1(edbeb11076061e744ea88d9899dbdfe0964c7e78) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galaga84 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1_1b.3p",    0x0000, 0x1000, CRC(ab036c9f) SHA1(ca7f5da42d4e76fd89bb0b35198a23c01462fbfe) )
	ROM_LOAD( "gal84_u2",     0x1000, 0x1000, CRC(4d832a30) SHA1(88ee11df88cf08005efccd6305f87fb3e2797db6) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1_4b.2l",    0x3000, 0x1000, CRC(499fcc76) SHA1(ddb8b121903646c320939c7d13f4aa4ebb130378) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1_5b.3f",    0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x10000, "sub3", 0 )
	ROM_LOAD( "10h_g07.bin",  0x0000, 0x1000, CRC(035e300c) SHA1(cfda2467e71c27381b7150ff8fc7b69d61df123a) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gal84u4d",     0x0000, 0x1000, CRC(22e339d5) SHA1(9ac2887ede802d28daa4ad0a0a54bcf7b1155a2e) )
	ROM_LOAD( "gal84u4e",     0x1000, 0x1000, CRC(60dcf940) SHA1(6530aa5b4afef4a8422ece76a93d0c5b1d93355e) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galagads )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",       0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galaga99 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1-1",            0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "g99-04k_g02.bin",  0x1000, 0x1000, CRC(ab0df3ac) SHA1(29c5bce17baa6778fad5faff1a241021e143e29f) )
	ROM_LOAD( "gg1_3.2m",         0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "g99-04h_g04.bin",  0x3000, 0x1000, CRC(b9a315fb) SHA1(3422dbd1fe116f9193dcccb1dba691eb98eab3aa) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "g99-07m_g08.bin",  0x0000, 0x1000, CRC(cc091e8b) SHA1(b71b962d5cd71fa78f0709b5cccf549cb7f4e9f0) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "g99-07e_g10.bin",  0x0000, 0x1000, CRC(2b493359) SHA1(e006ee999277120f01d1669ffe28df0b96415af5) )
	ROM_LOAD( "g99-07h_g09.bin",  0x1000, 0x1000, CRC(cdbb428d) SHA1(6c8dfb5da2bee99f07585e23ecfa21c67218cddb) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galagaf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg_5fast.bin", 0x0000, 0x1000, CRC(ae82145e) SHA1(146bd848e9783f34b1a006d793ee3f36e53baad0) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


/* This set has the fix proposed by Don Hodges at the page:
    http://donhodges.com/galaga_stage_256_fix.htm */
ROM_START( galagap )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1_1b.3p",    0x0000, 0x1000, CRC(ab036c9f) SHA1(ca7f5da42d4e76fd89bb0b35198a23c01462fbfe) )
	ROM_LOAD( "gg1_2b.3m",    0x1000, 0x1000, CRC(d9232240) SHA1(ab202aa259c3d332ef13dfb8fc8580ce2a5a253d) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1_4b.2l",    0x3000, 0x1000, CRC(499fcc76) SHA1(ddb8b121903646c320939c7d13f4aa4ebb130378) )
	ROM_FILL(0x25c4,1,0xa7)
	ROM_FILL(0x25d0,1,0x00)
	ROM_FILL(0x2fff,1,0xb2)

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1_5b.3f",    0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1_7b.2c",    0x0000, 0x1000, CRC(d016686b) SHA1(44c1a04fba3c7c826ff484185cb881b4b22e6657) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


ROM_START( galagix )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1-1",        0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "gg1-2",        0x1000, 0x1000, CRC(43bb0d5c) SHA1(666975aed5ce84f09794c54b550d64d95ab311f0) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1-4",        0x3000, 0x1000, CRC(83874442) SHA1(366cb0dbd31b787e64f88d182108b670d03b393e) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gx_07m_g08.bin",  0x0000, 0x1000, CRC(4b0a6b29) SHA1(1707bdfab7a17579b4171417d86ae76e065299c5) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gx_07e_g10.bin",  0x0000, 0x1000, CRC(b52a7dff) SHA1(1bf1cf9db72d16d1e6539d4fc05bbee071cc3420) )
	ROM_LOAD( "gx_07h_g09.bin",  0x1000, 0x1000, CRC(ffd5c70e) SHA1(5eac91087129ec8038feabdb18cb3199e52cae23) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galagost )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gost_07m_g08.bin",  0x0000, 0x1000, CRC(c7588555) SHA1(269362b7fe63007b317ee4b63ef7d09f24d9c362) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gost_07e_g10.bin",  0x0000, 0x1000, CRC(0758b90f) SHA1(89cc8db9457fa5f56b49930590f32aba1bc55323) )
	ROM_LOAD( "gost_07h_g09.bin",  0x1000, 0x1000, CRC(53f1332b) SHA1(04136e52008c4ad4813529d633fd9b8c71e6ccc0) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galagosb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gost_07m_g08.bin",  0x0000, 0x1000, CRC(c7588555) SHA1(269362b7fe63007b317ee4b63ef7d09f24d9c362) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gost_07e_g10.bin",  0x0000, 0x1000, CRC(0758b90f) SHA1(89cc8db9457fa5f56b49930590f32aba1bc55323) )
	ROM_LOAD( "gosb_07h_g09.bin",  0x1000, 0x1000, CRC(0339291c) SHA1(4958a78b6afb9f2b4f46d3c0f654843c077686a9) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galaped )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600fast.bin", 0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "ped_07m_g08.bin",  0x0000, 0x1000, CRC(d4fda6a5) SHA1(c1d39bfa2ad866e726b77f15ca22ef97ac3f51a4) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "ped_07e_g10.bin",  0x0000, 0x1000, CRC(1d0a2438) SHA1(66a4d7120f926a67dac0203b73c60917e5c140cf) )
	ROM_LOAD( "ped_07h_g09.bin",  0x1000, 0x1000, CRC(11d9b093) SHA1(8f151f4813ca7a80a228f2123cc089de414bc3f9) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galgaxin )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "2600j_4l.bin",  0x0000, 0x1000, CRC(10fa1481) SHA1(09f56503251cc29cf2131d2a0c5bcca4ee276e70) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "2800l_4d.bin",  0x0000, 0x1000, CRC(bf803b06) SHA1(ab0051b3b08f03ad93d100f38b106e761d4c4607) )
	ROM_LOAD( "2700k.bin",     0x1000, 0x1000, CRC(020906a3) SHA1(72e63918556ae366ede3ffad64a9d3ad4df1e514) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galagaef )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "terror.bin",   0x0000, 0x1000, CRC(1b629f80) SHA1(6db66601fd357400edf3dc1d689255c64a46a72d) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


ROM_START( terror )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "gg1-1",        0x0000, 0x1000, CRC(a3a0f743) SHA1(6907773db7c002ecde5e41853603d53387c5c7cd) )
	ROM_LOAD( "gg1-2",        0x1000, 0x1000, CRC(43bb0d5c) SHA1(666975aed5ce84f09794c54b550d64d95ab311f0) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "gg1-4",        0x3000, 0x1000, CRC(83874442) SHA1(366cb0dbd31b787e64f88d182108b670d03b393e) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1-5",        0x0000, 0x1000, CRC(3102fccd) SHA1(d29b68d6aab3217fa2106b3507b9273ff3f927bf) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1-7",        0x0000, 0x1000, CRC(8995088d) SHA1(d6cb439de0718826d1a0363c9d77de8740b18ecf) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "terror.bin",   0x0000, 0x1000, CRC(1b629f80) SHA1(6db66601fd357400edf3dc1d689255c64a46a72d) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( vgalaga )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "v_07m_g08.bin",  0x0000, 0x1000, CRC(bf59992f) SHA1(78f1e01affca694693254ecfedebdb05cb8d3b82) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "v_07e_g10.bin",  0x0000, 0x1000, CRC(3cf64d44) SHA1(54035738c6b0eed3331f11c53477cf87f07a8422) )
	ROM_LOAD( "v_07h_g09.bin",  0x1000, 0x1000, CRC(206bf7fc) SHA1(7510ed4a6c084f555cd5a6cd6fbc862f4797656f) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( vgalagam )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vgalagaf.08",  0x0000, 0x1000, CRC(c1bdeb1c) SHA1(7f07a9da708066401710b0b62b98e324ebbc2bca) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "vgalagaf.10",  0x0000, 0x1000, CRC(99367e04) SHA1(7e140e2b9dd4c6b0da41b21ed2cf1bf33f088a7e) )
	ROM_LOAD( "vgalagaf.09",  0x1000, 0x1000, CRC(683585c4) SHA1(fdc89b72bf64e44c80f4ae601280fca9b0d1c18c) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( vgalagaf )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "3200a.bin",    0x0000, 0x1000, CRC(3ef0b053) SHA1(0c04a362b737998c0952a753fb3fd8c8a17e9b46) )
	ROM_LOAD( "3300b.bin",    0x1000, 0x1000, CRC(1b280831) SHA1(f7ea12e61929717ebe43a4198a97f109845a2c62) )
	ROM_LOAD( "3400c.bin",    0x2000, 0x1000, CRC(16233d33) SHA1(a7eb799be5e23058754a92b15e6527bfbb47a354) )
	ROM_LOAD( "3500d.bin",    0x3000, 0x1000, CRC(0aaf5c23) SHA1(3f4b0bb960bf002261e9c1278c88f594c6aa8ab6) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "3600fast.bin",    0x0000, 0x1000, CRC(23d586e5) SHA1(43346c69385e9091e64cff6c027ac2689cafcbb9) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "3700g.bin",    0x0000, 0x1000, CRC(b07f0aa4) SHA1(7528644a8480d0be2d0d37069515ed319e94778f) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "vgalagaf.08",  0x0000, 0x1000, CRC(c1bdeb1c) SHA1(7f07a9da708066401710b0b62b98e324ebbc2bca) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "vgalagaf.10",  0x0000, 0x1000, CRC(99367e04) SHA1(7e140e2b9dd4c6b0da41b21ed2cf1bf33f088a7e) )
	ROM_LOAD( "vgalagaf.09",  0x1000, 0x1000, CRC(683585c4) SHA1(fdc89b72bf64e44c80f4ae601280fca9b0d1c18c) )

	ROM_REGION( 0x0320, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "2n.bin",       0x0020, 0x0100, CRC(a547d33b) SHA1(7323084320bb61ae1530d916f5edd8835d4d2461) )
	ROM_LOAD( "1c.bin",       0x0120, 0x0100, CRC(b6f585fb) SHA1(dd10147c4f05fede7ae6e7a760681700a660e87e) )
	ROM_LOAD( "5c.bin",       0x0220, 0x0100, CRC(8bd565f6) SHA1(bedba65816abfc2ebeacac6ee335ca6f136e3e3d) )

	ROM_REGION( 0x0100, "namco", 0 )
	ROM_LOAD( "1d.bin",       0x0000, 0x0100, CRC(86d92b24) SHA1(6bef9102b97c83025a2cf84e89d95f2d44c3d2ed) )
ROM_END


ROM_START( galaga01 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "01_1b.3p",     0x0000, 0x1000, CRC(d7dffd9c) SHA1(2c7090b0f45647c8b1b97bf2fbc140063b46f6e8) )
	ROM_LOAD( "01_2b.3m",     0x1000, 0x1000, CRC(ab7cbd28) SHA1(c7cca7d01c720511588125b52b1c6a527747a3f5) )
	ROM_LOAD( "01_3.2m",      0x2000, 0x1000, CRC(75bcd999) SHA1(cfb6137545665f48afa1f7db3433199f9b3b5fb8) )
	ROM_LOAD( "01_4b.2l",     0x3000, 0x1000, CRC(114f2ae5) SHA1(80f3273908aa485d7397a7b86d7cf05bcac8286c) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1_5b.3f",    0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1_7b.2c",    0x0000, 0x1000, CRC(d016686b) SHA1(44c1a04fba3c7c826ff484185cb881b4b22e6657) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


ROM_START( galaga02 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "02_1b.3p",     0x0000, 0x1000, CRC(b6f16ca7) SHA1(69c98eb9e4ecb268e9a20cc3a566b6b83a5b9d3d) )
	ROM_LOAD( "02_2b.3m",     0x1000, 0x1000, CRC(e0faff40) SHA1(98c28fadfc8098acef6781ed399de6f7ae8582f1) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "02_4b.2l",     0x3000, 0x1000, CRC(16f469ef) SHA1(ab493ab138df2bce42eb951032ed704dd8755897) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1_5b.3f",    0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1_7b.2c",    0x0000, 0x1000, CRC(d016686b) SHA1(44c1a04fba3c7c826ff484185cb881b4b22e6657) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( galaga03 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "03_1b.3p",     0x0000, 0x1000, CRC(35768124) SHA1(41f3bf262eae71e0ca7f0cbf459565d2d6c2b5ec) )
	ROM_LOAD( "03_2b.3m",     0x1000, 0x1000, CRC(873b6c48) SHA1(ce67e08d3249f4dd27a1e70625ea2262dfc41c1a) )
	ROM_LOAD( "gg1_3.2m",     0x2000, 0x1000, CRC(753ce503) SHA1(481f443aea3ed3504ec2f3a6bfcf3cd47e2f8f81) )
	ROM_LOAD( "02_4b.2l",     0x3000, 0x1000, CRC(16f469ef) SHA1(ab493ab138df2bce42eb951032ed704dd8755897) )

	ROM_REGION( 0x10000, "sub", 0 )
	ROM_LOAD( "gg1_5b.3f",    0x0000, 0x1000, CRC(bb5caae3) SHA1(e957a581463caac27bc37ca2e2a90f27e4f62b6f) )

	ROM_REGION( 0x10000, "sub2", 0 )
	ROM_LOAD( "gg1_7b.2c",    0x0000, 0x1000, CRC(d016686b) SHA1(44c1a04fba3c7c826ff484185cb881b4b22e6657) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "gg1_9.4l",     0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "gg1_11.4d",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
	ROM_LOAD( "gg1_10.4f",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )

	ROM_REGION( 0x0220, "proms", 0 )
	ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )
	ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )
	ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "prom-1.1d",    0x0000, 0x0100, CRC(7a2815b4) SHA1(085ada18c498fdb18ecedef0ea8fe9217edb7b46) )
	ROM_LOAD( "prom-2.5c",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END


/*    YEAR  NAME          PARENT   MACHINE   INPUT     CLASS          INIT          MONITOR COMPANY                 FULLNAME FLAGS */
GAME( 1984, galaga84,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "hack", "Galaga '84", MACHINE_SUPPORTS_SAVE )
GAME( 1999, galaga99,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Jerky and Sil", "Galaga '99", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagab2,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "bootleg", "Galaga (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, galagabl,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "bootleg", "Galaga (Namco 1982)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagads,     galaga,  galaga,   galagamw, galaga_state,  init_galaga,  ROT90, "hack", "Galaga (fast shoot)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, galagaef,     galaga,  galaga,   galagamw, galaga_state,  init_galaga,  ROT90, "hack", "Galaga (Enduring Freedom)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagaf,      galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Namco", "Galaga (Fast Shoot Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, galagap,      galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Don Hodges", "Galaga (patched)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, galagix,      galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Grand Master Peter", "Galagix", MACHINE_SUPPORTS_SAVE )
GAME( 2002, galagost,     galaga,  galagost, galaga,   galaga_hbmame, init_galaga,  ROT90, "Twisty", "Galaga Ghost (Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, galagosb,     galaga,  galagost, galaga,   galaga_hbmame, init_galaga,  ROT90, "Twisty", "Galaga Ghost (Set 2, 2002-11-28)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, galaped,      galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Cary Barnhard", "Galapede (Fast Shoot)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, galgaxin,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "T-Bone", "Galagalaxian", MACHINE_SUPPORTS_SAVE )
GAME( 2000, terror,       galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Hack", "Terrorist", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vgalaga,      galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Hack", "Vector Galaga", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vgalagam,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Hack", "Vector Galaga Midway", MACHINE_SUPPORTS_SAVE )
GAME( 2000, vgalagaf,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Hack", "Vector Galaga Fast Shoot", MACHINE_SUPPORTS_SAVE )
GAME( 2024, galaga01,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Paul Goes", "Galaga Wave Mixer (2024-04-02)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, galaga02,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Paul Goes", "Galaga Challenging Only", MACHINE_SUPPORTS_SAVE )
GAME( 2024, galaga03,     galaga,  galaga,   galaga,   galaga_state,  init_galaga,  ROT90, "Paul Goes", "Galaga Challenging Only (2024-03-26)", MACHINE_SUPPORTS_SAVE )

