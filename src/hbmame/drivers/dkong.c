// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/dkong.c"

INPUT_PORTS_START( dkongex )
	PORT_INCLUDE( dkong )
	PORT_MODIFY("DSW0")
	PORT_DIPNAME( 0x70, 0x00, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x70, "Freeplay" )
	PORT_DIPSETTING(    0x50, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x30, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x10, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x20, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x40, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x60, DEF_STR( 1C_4C ) )
INPUT_PORTS_END


/* same as mame 'dkongf' except rom cx_5at_g.bin is different */
ROM_START( dkongex )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "cx_5et_g.bin", 0x0000, 0x1000, CRC(00b7efaf) SHA1(97ed5930eb5d0cb98a9008b1d329ba7f3b8b8dbf) )
	ROM_LOAD( "cx_5ct_g.bin", 0x1000, 0x1000, CRC(88af9b69) SHA1(c5621dd8198c333b3fa32fdece60ee5a3d8b2276) )
	ROM_LOAD( "cx_5bt_g.bin", 0x2000, 0x1000, CRC(de74ad91) SHA1(c80227361bdbc565e9f5764e6364b52d40ca778a) )
	ROM_LOAD( "cx_5at_g.bin", 0x3000, 0x1000, CRC(9fa3e5b7) SHA1(5c34170f762a147d5285d17d6689fd64dab99ff2) )

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "v_5h_b.bin",   0x0000, 0x0800, CRC(12c8c95d) SHA1(a57ff5a231c45252a63b354137c920a1379b70a3) )
	ROM_LOAD( "v_3pt.bin",    0x0800, 0x0800, CRC(15e9c5e9) SHA1(976eb1e18c74018193a35aa86cff482ebfc5cc4e) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "l_4m_b.bin",   0x0000, 0x0800, CRC(59f8054d) SHA1(793dba9bf5a5fe76328acdfb90815c243d2a65f1) )
	ROM_LOAD( "l_4n_b.bin",   0x0800, 0x0800, CRC(672e4714) SHA1(92e5d379f4838ac1fa44d448ce7d142dae42102f) )
	ROM_LOAD( "l_4r_b.bin",   0x1000, 0x0800, CRC(feaa59ee) SHA1(ecf95db5a20098804fc8bd59232c66e2e0ed3db4) )
	ROM_LOAD( "l_4s_b.bin",   0x1800, 0x0800, CRC(20f2ef7e) SHA1(3bc482a38bf579033f50082748ee95205b0f673d) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "c-2k.bpr",     0x0000, 0x0100, CRC(e273ede5) SHA1(b50ec9e1837c00c20fb2a4369ec7dd0358321127) )
	ROM_LOAD( "c-2j.bpr",     0x0100, 0x0100, CRC(d6412358) SHA1(f9c872da2fe8e800574ae3bf483fb3ccacc92eb3) )
	ROM_LOAD( "v-5e.bpr",  	  0x0200, 0x0100, CRC(b869b8f5) SHA1(c2bdccbf2654b64ea55cd589fd21323a9178a660) )
ROM_END


/* This set has the fixes proposed by Don Hodges at the page:
	http://donhodges.com/how_high_can_you_get.htm */
ROM_START( dkongp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "c_5et_g.bin",  0x0000, 0x1000, CRC(ba70b88b) SHA1(d76ebecfea1af098d843ee7e578e480cd658ac1a) )
	ROM_LOAD( "c_5ct_g.bin",  0x1000, 0x1000, CRC(5ec461ec) SHA1(acb11a8fbdbb3ab46068385fe465f681e3c824bd) )
	ROM_LOAD( "c_5bt_g.bin",  0x2000, 0x1000, CRC(1c97d324) SHA1(c7966261f3a1d3296927e0b6ee1c58039fc53c1f) )
	ROM_LOAD( "c_5at_g.bin",  0x3000, 0x1000, CRC(b9005ac0) SHA1(3fe3599f6fa7c496f782053ddf7bacb453d197c4) )
	ROM_FILL(0x0f7d,1,0xfe)
	ROM_FILL(0x0f7e,1,0x04)
	ROM_FILL(0x0f7f,1,0x38)
	ROM_FILL(0x0f80,1,0x02)
	ROM_FILL(0x0f81,1,0x3e)
	ROM_FILL(0x0f82,1,0x04)
	ROM_FILL(0x0f83,1,0x47)
	ROM_FILL(0x0f84,1,0xa7)
	ROM_FILL(0x0f85,1,0x17)
	ROM_FILL(0x0f86,1,0xa7)
	ROM_FILL(0x0f87,1,0x17)
	ROM_FILL(0x0f88,1,0xa7)
	ROM_FILL(0x0f89,1,0x17)
	ROM_FILL(0x0f8a,1,0x80)
	ROM_FILL(0x0f8b,1,0x80)
	ROM_FILL(0x0f8c,1,0xc6)
	ROM_FILL(0x0f8d,1,0x28)

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "v_5h_b.bin",   0x0000, 0x0800, CRC(12c8c95d) SHA1(a57ff5a231c45252a63b354137c920a1379b70a3) )
	ROM_LOAD( "v_3pt.bin",    0x0800, 0x0800, CRC(15e9c5e9) SHA1(976eb1e18c74018193a35aa86cff482ebfc5cc4e) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "l_4m_b.bin",   0x0000, 0x0800, CRC(59f8054d) SHA1(793dba9bf5a5fe76328acdfb90815c243d2a65f1) )
	ROM_LOAD( "l_4n_b.bin",   0x0800, 0x0800, CRC(672e4714) SHA1(92e5d379f4838ac1fa44d448ce7d142dae42102f) )
	ROM_LOAD( "l_4r_b.bin",   0x1000, 0x0800, CRC(feaa59ee) SHA1(ecf95db5a20098804fc8bd59232c66e2e0ed3db4) )
	ROM_LOAD( "l_4s_b.bin",   0x1800, 0x0800, CRC(20f2ef7e) SHA1(3bc482a38bf579033f50082748ee95205b0f673d) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "c-2k.bpr",     0x0000, 0x0100, CRC(e273ede5) SHA1(b50ec9e1837c00c20fb2a4369ec7dd0358321127) )
	ROM_LOAD( "c-2j.bpr",     0x0100, 0x0100, CRC(d6412358) SHA1(f9c872da2fe8e800574ae3bf483fb3ccacc92eb3) )
	ROM_LOAD( "v-5e.bpr",     0x0200, 0x0100, CRC(b869b8f5) SHA1(c2bdccbf2654b64ea55cd589fd21323a9178a660) )
ROM_END

ROM_START( kong2600 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "c_5et_g.bin",  0x0000, 0x1000, CRC(ba70b88b) SHA1(d76ebecfea1af098d843ee7e578e480cd658ac1a) )
	ROM_LOAD( "c_5ct_g.bin",  0x1000, 0x1000, CRC(5ec461ec) SHA1(acb11a8fbdbb3ab46068385fe465f681e3c824bd) )
	ROM_LOAD( "c_5bt_g.bin",  0x2000, 0x1000, CRC(1c97d324) SHA1(c7966261f3a1d3296927e0b6ee1c58039fc53c1f) )
	ROM_LOAD( "c_5at_g.bin",  0x3000, 0x1000, CRC(b9005ac0) SHA1(3fe3599f6fa7c496f782053ddf7bacb453d197c4) )

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "k2600.3n",     0x0000, 0x0800, CRC(0e6a2a6d) SHA1(7ebbcb78b269184cf588b5ad0b90f449d6d4e299) )
	ROM_LOAD( "k2600.3p",     0x0800, 0x0800, CRC(ca57e0f4) SHA1(12c7821fc9f7fee276f7aa27dd0421f565b0f469) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "k2600.7c",     0x0000, 0x0800, CRC(cf450a43) SHA1(efa772f92b890181c0823b8113e069b7c4400ede) )
	ROM_LOAD( "k2600.7d",     0x0800, 0x0800, CRC(d5046907) SHA1(1eb08ed67f6de04c4f3cb8f449503bbedc6207bc) )
	ROM_LOAD( "k2600.7e",     0x1000, 0x0800, CRC(1539fe2a) SHA1(ea599987a2d65ffe2d09ca0951529bde2d31b1be) )
	ROM_LOAD( "k2600.7f",     0x1800, 0x0800, CRC(77cc00ab) SHA1(7560bf4fd65c03f28f015abf64f83fc4ad7ae512) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "k2600.2k",     0x0000, 0x0100, CRC(1e82d375) SHA1(8f2da5cedd1f62be08555dd0eb929eb41da5079d) )
	ROM_LOAD( "k2600.2j",     0x0100, 0x0100, CRC(2ab01dc8) SHA1(c25958d0706cabf997efe16fad71f454fc1ced0b) )
	ROM_LOAD( "k2600.5f",     0x0200, 0x0100, CRC(44988665) SHA1(68c474fc81aff46eae6c9a7ac6ab80288303e291) )
ROM_END

ROM_START( nadkong )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "c_5et_g.bin",  0x0000, 0x1000, CRC(ba70b88b) SHA1(d76ebecfea1af098d843ee7e578e480cd658ac1a) )
	ROM_LOAD( "c_5ct_g.bin",  0x1000, 0x1000, CRC(5ec461ec) SHA1(acb11a8fbdbb3ab46068385fe465f681e3c824bd) )
	ROM_LOAD( "c_5bt_g.bin",  0x2000, 0x1000, CRC(1c97d324) SHA1(c7966261f3a1d3296927e0b6ee1c58039fc53c1f) )
	ROM_LOAD( "c_5at_g.bin",  0x3000, 0x1000, CRC(b9005ac0) SHA1(3fe3599f6fa7c496f782053ddf7bacb453d197c4) )

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "nadkong.5h",   0x0000, 0x0800, CRC(88772f5b) SHA1(0fd6cbb593e87b863eb317b063151ef3b81565c2) )
	ROM_LOAD( "nadkong.3p",   0x0800, 0x0800, CRC(4c8aa728) SHA1(a277988d48f0d2f5d7cf9ef05eef222a36248cd3) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "nadkong.4m",   0x0000, 0x0800, CRC(2afbf9e6) SHA1(51e4f2f67903158b77574d55fcd17f302bc04a52) )
	ROM_LOAD( "nadkong.4n",   0x0800, 0x0800, CRC(4f077877) SHA1(4f2c2bc693b0e8d36114024b6eaeb37a3b5d853d) )
	ROM_LOAD( "nadkong.4r",   0x1000, 0x0800, CRC(d868da93) SHA1(c1c5d21f8c6a057fd5f54cab1eb4dbef06120405) )
	ROM_LOAD( "nadkong.4s",   0x1800, 0x0800, CRC(7ebfe9f3) SHA1(75bd16e9590ada93b061dee3ec7916e1e1f113c3) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "c-2k.bpr",     0x0000, 0x0100, CRC(e273ede5) SHA1(b50ec9e1837c00c20fb2a4369ec7dd0358321127) )
	ROM_LOAD( "c-2j.bpr",     0x0100, 0x0100, CRC(d6412358) SHA1(f9c872da2fe8e800574ae3bf483fb3ccacc92eb3) )
	ROM_LOAD( "v-5e.bpr",  	  0x0200, 0x0100, CRC(b869b8f5) SHA1(c2bdccbf2654b64ea55cd589fd21323a9178a660) )
ROM_END

ROM_START( dkong2m )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "c_5et_g.bin",  0x0000, 0x1000, CRC(ba70b88b) SHA1(d76ebecfea1af098d843ee7e578e480cd658ac1a) )
	ROM_LOAD( "c_5ct_g.bin",  0x1000, 0x1000, CRC(5ec461ec) SHA1(acb11a8fbdbb3ab46068385fe465f681e3c824bd) )
	ROM_LOAD( "c_5bt_g.bin",  0x2000, 0x1000, CRC(1c97d324) SHA1(c7966261f3a1d3296927e0b6ee1c58039fc53c1f) )
	ROM_LOAD( "c_5at_g.bin",  0x3000, 0x1000, CRC(b9005ac0) SHA1(3fe3599f6fa7c496f782053ddf7bacb453d197c4) )

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "v_5h_b.bin",   0x0000, 0x0800, CRC(12c8c95d) SHA1(a57ff5a231c45252a63b354137c920a1379b70a3) )
	ROM_LOAD( "v_3pt.bin",    0x0800, 0x0800, CRC(15e9c5e9) SHA1(976eb1e18c74018193a35aa86cff482ebfc5cc4e) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "l_4m_bm.bin",  0x0000, 0x0800, CRC(4b67ccf9) SHA1(8206436a0e5d1f7d5569f918eb87102f03b6cea3) )
	ROM_LOAD( "l_4n_bm.bin",  0x0800, 0x0800, CRC(32a708cb) SHA1(2c8b4d5179f10fb2e8e9da2012419bdfb436a545) )
	ROM_LOAD( "l_4r_bm.bin",  0x1000, 0x0800, CRC(22b5d5c4) SHA1(6bf39a5766786023cc18620283f12dbb9194cbfd) )
	ROM_LOAD( "l_4s_bm.bin",  0x1800, 0x0800, CRC(b212b185) SHA1(03d3586e80cafd2440d03d60b7a4c8808b0c6caa) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "c-2k.bpr",     0x0000, 0x0100, CRC(e273ede5) SHA1(b50ec9e1837c00c20fb2a4369ec7dd0358321127) )
	ROM_LOAD( "c-2j.bpr",     0x0100, 0x0100, CRC(d6412358) SHA1(f9c872da2fe8e800574ae3bf483fb3ccacc92eb3) )
	ROM_LOAD( "v-5e.bpr",     0x0200, 0x0100, CRC(b869b8f5) SHA1(c2bdccbf2654b64ea55cd589fd21323a9178a660) )
ROM_END

ROM_START( pauline )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "c_5et_g.bin",  0x0000, 0x1000, CRC(ba70b88b) SHA1(d76ebecfea1af098d843ee7e578e480cd658ac1a) )
	ROM_LOAD( "c_5ct_gp.bin", 0x1000, 0x1000, CRC(45af403e) SHA1(6030a4af7df98bfdf5b35a9a42541566f7d12901) )
	ROM_LOAD( "c_5bt_gp.bin", 0x2000, 0x1000, CRC(3a9783b7) SHA1(e98d757c048f2180ba22c774e0e425ddc661ba8c) )
	ROM_LOAD( "c_5at_gp.bin", 0x3000, 0x1000, CRC(32bc20ff) SHA1(ef141f437912923625722b83a33ea182eaa31427) )

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "v_5h_bp.bin",  0x0000, 0x0800, CRC(007aa348) SHA1(ff2ae583fef6da9d260fda8f4a896dd0414c3388) )
	ROM_LOAD( "v_3ptp.bin",   0x0800, 0x0800, CRC(a967aff0) SHA1(7bcfdbeb0a5cdfec604eb8450664bc4b789526be) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "l_4m_bp.bin",  0x0000, 0x0800, CRC(766ae006) SHA1(0ec53798aa2c30b2c5c8b2f99b811a187faa2549) )
	ROM_LOAD( "l_4n_bp.bin",  0x0800, 0x0800, CRC(39e7ca4b) SHA1(b77ddd39608d08013fa8bb764c8e5aa4e03181dc) )
	ROM_LOAD( "l_4r_bp.bin",  0x1000, 0x0800, CRC(012f2f25) SHA1(836709192a249b00ded783be542ee844eb930c7a) )
	ROM_LOAD( "l_4s_bp.bin",  0x1800, 0x0800, CRC(84eb5bfb) SHA1(c1f38efb8670f1a489275eb8ff576a95d140cfb9) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "c-2k.bpr",     0x0000, 0x0100, CRC(e273ede5) SHA1(b50ec9e1837c00c20fb2a4369ec7dd0358321127) )
	ROM_LOAD( "c-2j.bpr",     0x0100, 0x0100, CRC(d6412358) SHA1(f9c872da2fe8e800574ae3bf483fb3ccacc92eb3) )
	ROM_LOAD( "v-5e.bpr",     0x0200, 0x0100, CRC(b869b8f5) SHA1(c2bdccbf2654b64ea55cd589fd21323a9178a660) )
ROM_END

ROM_START( dkongpac )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "c_5et_g.bin",  0x0000, 0x1000, CRC(ba70b88b) SHA1(d76ebecfea1af098d843ee7e578e480cd658ac1a) )
	ROM_LOAD( "c_5ct_g.bin",  0x1000, 0x1000, CRC(5ec461ec) SHA1(acb11a8fbdbb3ab46068385fe465f681e3c824bd) )
	ROM_LOAD( "c_5bt_g.bin",  0x2000, 0x1000, CRC(1c97d324) SHA1(c7966261f3a1d3296927e0b6ee1c58039fc53c1f) )
	ROM_LOAD( "c_5at_g.bin",  0x3000, 0x1000, CRC(b9005ac0) SHA1(3fe3599f6fa7c496f782053ddf7bacb453d197c4) )

	ROM_REGION( 0x1800, "soundcpu", 0 )
	ROM_LOAD( "s_3i_b.bin",   0x0000, 0x0800, CRC(45a4ed06) SHA1(144d24464c1f9f01894eb12f846952290e6e32ef) )
	ROM_RELOAD(               0x0800, 0x0800 )
	ROM_LOAD( "s_3j_b.bin",   0x1000, 0x0800, CRC(4743fe92) SHA1(6c82b57637c0212a580591397e6a5a1718f19fd2) )

	ROM_REGION( 0x1000, "gfx1", 0 )
	ROM_LOAD( "dkongpac.3n",  0x0000, 0x0800, CRC(1beba830) SHA1(04dce2464d0627b78d185924706f8ca38892d6d3) )
	ROM_LOAD( "dkongpac.3p",  0x0800, 0x0800, CRC(94d61766) SHA1(f45f10d523fefe0581dde42450b713cb94ce2072) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "dkongpac.7c",  0x0000, 0x0800, CRC(065e2713) SHA1(a32d506a247a692a10962c9e47024d51bbb52d22) )
	ROM_LOAD( "dkongpac.7d",  0x0800, 0x0800, CRC(a84b347d) SHA1(5a3e6fa3d520aef3c529e515d1e9fa464cbc6a78) )
	ROM_LOAD( "dkongpac.7e",  0x1000, 0x0800, CRC(6ae6f476) SHA1(a79927a1c7511bf510107d9ba5fcc8f284541259) )
	ROM_LOAD( "dkongpac.7f",  0x1800, 0x0800, CRC(9d293922) SHA1(14af1cd7f9854dec820c899d999be4623d0048b0) )

	ROM_REGION( 0x0300, "proms", 0 )
	ROM_LOAD( "k2600.2k",     0x0000, 0x0100, CRC(1e82d375) SHA1(8f2da5cedd1f62be08555dd0eb929eb41da5079d) )
	ROM_LOAD( "k2600.2j",     0x0100, 0x0100, CRC(2ab01dc8) SHA1(c25958d0706cabf997efe16fad71f454fc1ced0b) )
	ROM_LOAD( "k2600.5f",     0x0200, 0x0100, CRC(44988665) SHA1(68c474fc81aff46eae6c9a7ac6ab80288303e291) )
ROM_END

GAME( 2005, dkongex,  dkong, dkong2b, dkongex, driver_device, 0, ROT90, "Jeff Kulczycki", "Donkey Kong Foundry", MACHINE_SUPPORTS_SAVE )
GAME( 2007, dkongp,   dkong, dkong2b, dkong,   driver_device, 0, ROT90, "Don Hodges", "Donkey Kong (patched)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kong2600, dkong, dkong2b, dkong,   driver_device, 0, ROT90, "Vic Twenty George", "Donkey Kong (2600 graphics)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, nadkong,  dkong, dkong2b, dkong,   driver_device, 0, ROT90, "Hack", "Naked Donkey Kong", MACHINE_SUPPORTS_SAVE )
GAME( 2013, dkong2m,  dkong, dkong2b, dkong,   driver_device, 0, ROT90, "Hack", "Donkey Kong (2 marios)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, pauline,  dkong, dkong2b, dkong,   driver_device, 0, ROT90, "Clay Cowgill and Mike Mika", "Donkey Kong: Pauline Edition Rev 5 (2013-04-22)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, dkongpac, dkong, dkong2b, dkong,   driver_device, 0, ROT90, "Tim Appleton", "Donkey Kong (Pacman graphics)", MACHINE_SUPPORTS_SAVE )
