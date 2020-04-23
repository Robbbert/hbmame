// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_l.cpp"

ROM_START( evilstonfr )
	ROM_REGION( 0x40000, "tc0090lvc", 0 )
	ROM_LOAD( "c67-03fr.ic2", 0x00000, 0x20000, CRC(3fa7e884) SHA1(94716256c199b5f347bbe122d04adbab1a54b1ce) )
	ROM_LOAD( "c67-04fr.ic6", 0x20000, 0x20000, CRC(8c43dc76) SHA1(ee395d55ee8e66c86e12f604774799ca7cfe1858) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "c67-05.ic22",  0x00000, 0x20000, CRC(94d3a642) SHA1(af20aa5bb60a45c05eb1deba23ba30e6640ca235) )

	ROM_REGION( 0x100000, "tc0090lvc:gfx", 0 )
	ROM_LOAD( "c67-01.ic1",  0x00000, 0x80000, CRC(2f351bf4) SHA1(0fb37abf3413cd11baece1c9bbca5a51b0f28938) )
	ROM_LOAD( "c67-02.ic5",  0x80000, 0x80000, CRC(eb4f895c) SHA1(2c902572fe5a5d4442e4dd29e8a85cb40c384140) )
ROM_END

ROM_START( sokoban )
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "sokoban.ic10",      0x00000, 0x10000, CRC(5959ddb4) SHA1(73f206e19cdfd151426092aa3eea6f72093f360b) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "sokoban.ic9", 0x00000, 0x10000, CRC(859e1ff4) SHA1(899b71bd0eb9923e88cf7142ed634b36b3e34c6c) )
	ROM_LOAD16_BYTE( "sokoban.ic8", 0x00001, 0x10000, CRC(439a6670) SHA1(79144e3ba5205f5356f28d3b23ef32d3d4fca51c) )

	// came with the zipfile but not used
	ROM_REGION( 0x0200, "plds", 0 )
	ROM_LOAD_OPTIONAL( "gal16v8-b86-04.bin", 0x0000, 0x0117, CRC(bf8c0ea0) SHA1(e0a00f1f6363fb79650202f90a56329990876d49) )
ROM_END

ROM_START( speccies )
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "speccies.ic10",     0x00000, 0x55e9, CRC(29e126ac) SHA1(1e3e1379f964e0967d6ea5ef6a84741319b5108f) )

	// came with the zipfile but not used
	ROM_REGION( 0x0800, "mcu:mcu", 0 )
	ROM_LOAD_OPTIONAL( "mc68705p3.ic4", 0x0000, 0x0800, CRC(085f68b4) SHA1(2dbc7e2c015220dc59ee1f1208540744e5b9b7cc) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "speccies.ic9", 0x00000, 0x10000, CRC(11ed1528) SHA1(55f0b63bcbbbe4ad4d1041dca980b55ccb7a7aea) )
	ROM_LOAD16_BYTE( "speccies.ic8", 0x00001, 0x10000, CRC(fcfeb89f) SHA1(fc0e811c72313b371d3a6530b27f9023ff201e31) )

	// came with the zipfile but not used
	ROM_REGION( 0x0200, "plds", 0 )
	ROM_LOAD_OPTIONAL( "gal16v8-b86-04.bin", 0x0000, 0x0117, CRC(bf8c0ea0) SHA1(e0a00f1f6363fb79650202f90a56329990876d49) )
ROM_END

ROM_START( sqij )
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "sqij.ic10",     0x00000, 0xf000, CRC(214ba802) SHA1(39c1d66178d409ccf41194f99c765d2a98727bf9) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "sqij.ic9", 0x00000, 0x10000, CRC(c4de729b) SHA1(58e4c8d8af121dd25eba737cc1917841bc3ec6f5) )
	ROM_LOAD16_BYTE( "sqij.ic8", 0x00001, 0x10000, CRC(978df42d) SHA1(51d7310591c68ec12178b606f0779915f15de8a0) )

	// came with the zipfile but not used
	ROM_REGION( 0x0200, "plds", 0 )
	ROM_LOAD_OPTIONAL( "gal16v8-b86-04.bin", 0x0000, 0x0117, CRC(bf8c0ea0) SHA1(e0a00f1f6363fb79650202f90a56329990876d49) )
ROM_END

ROM_START( sqij15 ) // this version works on real hardware
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "sqij15.ic10",     0x00000, 0xf000, CRC(793c8b97) SHA1(66b1a6b6c9cd95f06ca5c23843bf61be0474b659) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "sqij.ic9", 0x00000, 0x10000, CRC(c4de729b) SHA1(58e4c8d8af121dd25eba737cc1917841bc3ec6f5) )
	ROM_LOAD16_BYTE( "sqij.ic8", 0x00001, 0x10000, CRC(978df42d) SHA1(51d7310591c68ec12178b606f0779915f15de8a0) )

	// came with the zipfile but not used
	ROM_REGION( 0x0200, "plds", 0 )
	ROM_LOAD_OPTIONAL( "gal16v8-b86-04.bin", 0x0000, 0x0117, CRC(bf8c0ea0) SHA1(e0a00f1f6363fb79650202f90a56329990876d49) )
ROM_END

ROM_START( tldemo )
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "tldemo.ic10",   0x00000, 0x56a4, CRC(fda3d418) SHA1(e0171cc1c035d1f800702c3766825b63f6acd291) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "tldemo.ic9", 0x00000, 0x10000, CRC(792b8c11) SHA1(0592481b586bfca0f8f8eb096bee46690943596b) )
	ROM_LOAD16_BYTE( "tldemo.ic8", 0x00001, 0x10000, CRC(59cfc5ab) SHA1(133084eeacdf978ed13db565f64d155c08ecaade) )
ROM_END

ROM_START( tldemo2 )
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "tldemo2.ic10",   0x00000, 0x7142, CRC(1292a22c) SHA1(e351ac5381f7d73de0087586c7fb28d9eccd72cb) )
	// This one below is the old version, works fine in MAME but is buggy in real hardware.
	//ROM_LOAD( "tldemo2.ic10",   0x00000, 0x6514, CRC(efd21b6b) SHA1(1c3bfb64dacfb5a1da55201cd4d4202694e06d37) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "tldemo2.ic9", 0x00000, 0x10000, CRC(fcc75ddb) SHA1(e702480b84379e3106cb6d575d237a8f9701f49e) )
	ROM_LOAD16_BYTE( "tldemo2.ic8", 0x00001, 0x10000, CRC(33e70f71) SHA1(5a0caa0912dd7d76dd7e788e21b4cc7f320156ad) )
ROM_END

GAME( 2007, evilstonfr,  evilston, evilston,  evilston, taitol_2cpu_state, empty_init, ROT270, "Arkatrad", "Evil Stone (Translation French V1)", MACHINE_IMPERFECT_SOUND )
GAME( 2015, sokoban,     0,        plotting,  plotting, taitol_1cpu_state, empty_init, ROT0,   "cmonkey", "Sokoban LE", 0 )
GAME( 2015, speccies,    0,        plotting,  plotting, taitol_1cpu_state, empty_init, ROT0,   "Sokurah", "Speccies 2", 0 )
GAME( 2018, sqij,        0,        plotting,  plotting, taitol_1cpu_state, empty_init, ROT0,   "Sokurah", "SQIJ!", 0 )
GAME( 2018, sqij15,      sqij,     plotting,  plotting, taitol_1cpu_state, empty_init, ROT0,   "Sokurah", "SQIJ! v1.5", 0 )
GAME( 2014, tldemo,      0,        plotting,  plotting, taitol_1cpu_state, empty_init, ROT0,   "cmonkey", "Demo - System Timed Cheat Tool", 0 )
GAME( 2015, tldemo2,     0,        plotting,  plotting, taitol_1cpu_state, empty_init, ROT0,   "cmonkey", "Demo - 256 colours", 0 )


// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***********
  Evil Stone
**************/

ROM_START( evilstons01 )
	ROM_REGION( 0x40000, "tc0090lvc", 0 )
	ROM_LOAD( "c67-03fr.ic2", 0x00000, 0x20000, CRC(3fa7e884) SHA1(94716256c199b5f347bbe122d04adbab1a54b1ce) )
	ROM_LOAD( "c67-04hc01.ic6",  0x20000, 0x20000, CRC(50c46efa) SHA1(42671d4d1290b84cc32b5e0b21708c8a148d44b2) )

	ROM_REGION( 0x80000, "audiocpu", 0 )
	ROM_LOAD( "c67-05.ic22", 0x00000, 0x20000, CRC(94d3a642) SHA1(af20aa5bb60a45c05eb1deba23ba30e6640ca235) )

	ROM_REGION( 0x100000, "tc0090lvc:gfx", 0 )
	ROM_LOAD( "c67-01.ic1",  0x00000, 0x80000, CRC(2f351bf4) SHA1(0fb37abf3413cd11baece1c9bbca5a51b0f28938) )
	ROM_LOAD( "c67-02.ic5",  0x80000, 0x80000, CRC(eb4f895c) SHA1(2c902572fe5a5d4442e4dd29e8a85cb40c384140) )
ROM_END

 /***********
 Kuri Kinton
**************/

ROM_START( kurikintjs01 )
	ROM_REGION( 0x40000, "tc0090lvc", 0 )
	ROM_LOAD( "b42-05hc01.ic2",  0x00000, 0x20000, CRC(b0656c45) SHA1(777a9206ac8bc3a519000c35cbab58dafec461f5) )
	ROM_LOAD( "b42-06hc01.ic6",  0x20000, 0x20000, CRC(677547d9) SHA1(56c2befe20182d5241580fae3787abf506df2b6f) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "b42-07.ic22", 0x00000, 0x10000, CRC(0f2719c0) SHA1(f870335a75f236f0059522f9a577dee7ca3acb2f) )

	ROM_REGION( 0x100000, "tc0090lvc:gfx", 0 )
	ROM_LOAD( "b42-01hc01.ic1",  0x00000, 0x80000, CRC(592c6d20) SHA1(4c5d58e23855a905064049a7758706778a81f129) )
	ROM_LOAD( "b42-02hc01.ic5",  0x80000, 0x80000, CRC(cd5fc143) SHA1(5a5a6e674a47786ed9c843cefacf307429df9933) )
ROM_END

 /********
 Plotting
***********/

ROM_START( plottingas01 )
	ROM_REGION( 0x10000, "tc0090lvc", 0 )
	ROM_LOAD( "plot01hc01.ic10", 0x00000, 0x10000, CRC(134e39a3) SHA1(c3ec3ed35b3fb39d780369a4c50beb6e4ccaaff2) )

	ROM_REGION( 0x20000, "tc0090lvc:gfx", 0 )
	ROM_LOAD16_BYTE( "b96-02hc01.ic9", 0x00000, 0x10000, CRC(78b05c85) SHA1(384626d0ef246702cdf6869bedee42fa18c2f438) )
	ROM_LOAD16_BYTE( "b96-03hc01.ic8", 0x00001, 0x10000, CRC(ef56d689) SHA1(16391444b2e50accfd81e5bf35bf2b5f74e3d138) )

	ROM_REGION( 0x0200, "plds", 0 )
	ROM_LOAD( "gal16v8-b86-04.bin", 0x0000, 0x0117, CRC(bf8c0ea0) SHA1(e0a00f1f6363fb79650202f90a56329990876d49) ) 
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Evil Stone
GAME( 2007, evilstons01,  evilston, evilston,  evilston,  taitol_2cpu_state, empty_init, ROT270, "S.Arkames.", "Evil Stone (Translation French V2 2007-11-28)", 0 )
// Kuri Kinton
GAME( 1988, kurikintjs01, kurikint, kurikint,  kurikintj, taitol_2cpu_state, empty_init, ROT0,   "hack", "Kuri Kinton (Translation Chinese)", 0 )
// Plotting
GAME( 1989, plottingas01, plotting, plotting,  plotting,  taitol_1cpu_state, init_plottinga, ROT0,   "hack", "Plotting (Translation Chinese)", 0 )

