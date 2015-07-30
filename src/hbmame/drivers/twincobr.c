// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/twincobr.c"

ROM_START( twincbu2 )
	ROM_REGION( 0x30000, "maincpu", 0 )	/* Main 68K code */
	ROM_LOAD16_BYTE( "tc16",	0x00000, 0x10000, CRC(07f64d13) SHA1(864ce0f9369c40c3ae792fc4ab2444a168214749) )
	ROM_LOAD16_BYTE( "tc14",	0x00001, 0x10000, CRC(41be6978) SHA1(4784804b738a332c7f24a43bcbb7a1e607365735) )
	ROM_LOAD16_BYTE( "tcbra26.bin",	0x20000, 0x08000, CRC(bdd00ba4) SHA1(b76b22f03eb4b821a8c555edd9fcee814f2e66a7) )
	ROM_LOAD16_BYTE( "tcbra27.bin",	0x20001, 0x08000, CRC(ed600907) SHA1(e5964db9eab2c334940795d71cb90f6679490227) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* Sound Z80 code */
	ROM_LOAD( "b30-05",		0x00000, 0x08000, CRC(1a8f1e10) SHA1(0c37a7a50b2523506ad77ac03ae752eb94092ff6) )

	ROM_REGION( 0x2000, "dsp", 0 )	/* Co-Processor TMS320C10 MCU code */
	ROM_LOAD16_BYTE( "dsp_22.bin",	0x0001, 0x0800, CRC(79389a71) SHA1(14ec4c1c9b06702319e89a7a250d0038393437f4) )
	ROM_LOAD16_BYTE( "dsp_21.bin",	0x0000, 0x0800, CRC(2d135376) SHA1(67a2cc774d272ee1cd6e6bc1c5fc33fc6968837e) )

	ROM_REGION( 0x0c000, "gfx1", 0 )	/* chars */
	ROM_LOAD( "tc11",		0x00000, 0x04000, CRC(0a254133) SHA1(17e9cc5e36fb4696012d0f9229fa172034cd843a) )
	ROM_LOAD( "b30_07.128",		0x04000, 0x04000, CRC(3160894d) SHA1(234951e2aa4002e23781f8ed3be64b71b0400e9f) )
	ROM_LOAD( "tc04",		0x08000, 0x04000, CRC(a599d845) SHA1(732001f2d378d890f148e6b616c287d71fae832a) )

	ROM_REGION( 0x40000, "gfx2", 0 )	/* fg tiles */
	ROM_LOAD( "b30_15.512",		0x00000, 0x10000, CRC(88795b18) SHA1(73bd28ef12576cf54630304c3c0f8a08141a321f) )
	ROM_LOAD( "b30_16.512",		0x10000, 0x10000, CRC(e6f421d2) SHA1(82da99d017780d0e52988dc8ebfcd16b54d34b6b) )
	ROM_LOAD( "tc06",		0x20000, 0x10000, CRC(13daeac8) SHA1(1cb103f434e2ecf193fa936ca7ea9194064c5b39) )
	ROM_LOAD( "tc05",		0x30000, 0x10000, CRC(8cc79357) SHA1(31064df2b796ca85ad3caccf626b684dff1104a1) )

	ROM_REGION( 0x20000, "gfx3", 0 )	/* bg tiles */
	ROM_LOAD( "tc07",		0x00000, 0x08000, CRC(b5d48389) SHA1(a00c5b9c231d3d580fa20c7ad3f8b6fd990e6594) )
	ROM_LOAD( "tc08",		0x08000, 0x08000, CRC(97f20fdc) SHA1(7cb3cd0637b0db889a3d552fd7c1a916eee5ca27) )
	ROM_LOAD( "tc09",		0x10000, 0x08000, CRC(170c01db) SHA1(f4c5a1600f6cbb48abbace66c6f7514f79138e8b) )
	ROM_LOAD( "tc10",		0x18000, 0x08000, CRC(44f5accd) SHA1(2f9bdebe71c8be195332356df68992fd38d86994) )

	ROM_REGION( 0x40000, "scu", 0 )	/* sprites */
	ROM_LOAD( "tc20",		0x00000, 0x10000, CRC(cb4092b8) SHA1(35b1d1e04af760fa106124bd5a94174d63ff9705) )
	ROM_LOAD( "b30_19.512",		0x10000, 0x10000, CRC(4043ad5a) SHA1(bf14829ec800dab9ac6e385bddcab303301df530) )
	ROM_LOAD( "b30_18.512",		0x20000, 0x10000, CRC(cdc16155) SHA1(9f41695409dbf72c2a66911aaa94b8fe5f6acc01) )
	ROM_LOAD( "tc17",		0x30000, 0x10000, CRC(4264bff8) SHA1(3271b8b23f51346d1928ae01f8b547fed49181e6) )

	ROM_REGION( 0x260, "proms", 0 )	/* nibble bproms, lo/hi order to be determined */
	ROM_LOAD( "82s129.d3",	0x000, 0x100, CRC(24e7d62f) SHA1(1c06a1ef1b6a722794ca1d5ee2c476ecaa5178a3) )	/* sprite priority control ?? */
	ROM_LOAD( "82s129.d4",	0x100, 0x100, CRC(a50cef09) SHA1(55cafb5b2551b80ae708e9b966cf37c70a16d310) )	/* sprite priority control ?? */
	ROM_LOAD( "82s123.d2",	0x200, 0x020, CRC(f72482db) SHA1(b0cb911f9c81f6088a5aa8760916ddae1f8534d7) )	/* sprite control ?? */
	ROM_LOAD( "82s123.e18",	0x220, 0x020, CRC(bc88cced) SHA1(5055362710c0f58823c05fb4c0e0eec638b91e3d) )	/* sprite attribute (flip/position) ?? */
	ROM_LOAD( "82s123.b24",	0x240, 0x020, CRC(4fb5df2a) SHA1(506ef2c8e4cf45c256d6831a0a5760732f2de422) )	/* tile to sprite priority ?? */
ROM_END

ROM_START( twintwst )
	ROM_REGION( 0x30000, "maincpu", 0 )	/* Main 68K code */
	ROM_LOAD16_BYTE( "tc16",	0x00000, 0x10000, CRC(07f64d13) SHA1(864ce0f9369c40c3ae792fc4ab2444a168214749) )
	ROM_LOAD16_BYTE( "tc14",	0x00001, 0x10000, CRC(41be6978) SHA1(4784804b738a332c7f24a43bcbb7a1e607365735) )
	ROM_LOAD16_BYTE( "tc15",	0x20000, 0x08000, CRC(3a646618) SHA1(fc1ed8f3c491f5cf16a17e5ce08c5d8f3ce03683) )
	ROM_LOAD16_BYTE( "tc13",	0x20001, 0x08000, CRC(d7d1e317) SHA1(57b8433b1677a390a7c7e00a1464bb8ed9cbfc73) )

	ROM_REGION( 0x10000, "audiocpu", 0 )	/* Sound Z80 code */
	ROM_LOAD( "tc12",		0x00000, 0x08000, CRC(e37b3c44) SHA1(5fed10b29c14e27aee0cd92ecde5c5cb422273b1) )	/* slightly different from the other two sets */

	ROM_REGION( 0x2000, "dsp", 0 )	/* Co-Processor TMS320C10 MCU code */
	ROM_LOAD16_BYTE( "dsp_22.bin",	0x0001, 0x0800, CRC(79389a71) SHA1(14ec4c1c9b06702319e89a7a250d0038393437f4) )
	ROM_LOAD16_BYTE( "dsp_21.bin",	0x0000, 0x0800, CRC(2d135376) SHA1(67a2cc774d272ee1cd6e6bc1c5fc33fc6968837e) )

	ROM_REGION( 0x0c000, "gfx1", 0 )	/* chars */
	ROM_LOAD( "tc11",		0x00000, 0x04000, CRC(0a254133) SHA1(17e9cc5e36fb4696012d0f9229fa172034cd843a) )
	ROM_LOAD( "tc03",		0x04000, 0x04000, CRC(e9e2d4b1) SHA1(e0a19dd46a9ba85d95bba7fbf81d8dc36dbfeabd) )
	ROM_LOAD( "tc04",		0x08000, 0x04000, CRC(a599d845) SHA1(732001f2d378d890f148e6b616c287d71fae832a) )

	ROM_REGION( 0x40000, "gfx2", 0 )	/* fg tiles */
	ROM_LOAD( "tc01",		0x00000, 0x10000, CRC(15b3991d) SHA1(f5e7ed7a7721ed7e6dfd440634160390b7a294e4) )
	ROM_LOAD( "tc02",		0x10000, 0x10000, CRC(d9e2e55d) SHA1(0409e6df836d1d5198b64b21b42192631aa6d096) )
	ROM_LOAD( "tc06",		0x20000, 0x10000, CRC(13daeac8) SHA1(1cb103f434e2ecf193fa936ca7ea9194064c5b39) )
	ROM_LOAD( "tc05",		0x30000, 0x10000, CRC(8cc79357) SHA1(31064df2b796ca85ad3caccf626b684dff1104a1) )

	ROM_REGION( 0x20000, "gfx3", 0 )	/* bg tiles */
	ROM_LOAD( "tc07",		0x00000, 0x08000, CRC(b5d48389) SHA1(a00c5b9c231d3d580fa20c7ad3f8b6fd990e6594) )
	ROM_LOAD( "tc08",		0x08000, 0x08000, CRC(97f20fdc) SHA1(7cb3cd0637b0db889a3d552fd7c1a916eee5ca27) )
	ROM_LOAD( "tc09",		0x10000, 0x08000, CRC(170c01db) SHA1(f4c5a1600f6cbb48abbace66c6f7514f79138e8b) )
	ROM_LOAD( "tc10",		0x18000, 0x08000, CRC(44f5accd) SHA1(2f9bdebe71c8be195332356df68992fd38d86994) )

	ROM_REGION( 0x40000, "scu", 0 )	/* sprites */
	ROM_LOAD( "tcw20",		0x00000, 0x10000, CRC(41c8f4ea) SHA1(9da6911b89b5cf49b64a8bcb12ab191cc98316f2) )
	ROM_LOAD( "tcw19",		0x10000, 0x10000, CRC(8d5db99e) SHA1(33a1a51396fd289ee23ea1c645bc85bfed37848d) )
	ROM_LOAD( "tcw18",		0x20000, 0x10000, CRC(eade1ca3) SHA1(af62d4b3a4072d6b0970a47ab6ef68987bac9c93) )
	ROM_LOAD( "tcw17",		0x30000, 0x10000, CRC(6a559622) SHA1(8a07f792efea9d521a5fd0169a5d6c15fe615d5d) )

	ROM_REGION( 0x260, "proms", 0 )	/* nibble bproms, lo/hi order to be determined */
	ROM_LOAD( "82s129.d3",	0x000, 0x100, CRC(24e7d62f) SHA1(1c06a1ef1b6a722794ca1d5ee2c476ecaa5178a3) )	/* sprite priority control ?? */
	ROM_LOAD( "82s129.d4",	0x100, 0x100, CRC(a50cef09) SHA1(55cafb5b2551b80ae708e9b966cf37c70a16d310) )	/* sprite priority control ?? */
	ROM_LOAD( "82s123.d2",	0x200, 0x020, CRC(f72482db) SHA1(b0cb911f9c81f6088a5aa8760916ddae1f8534d7) )	/* sprite control ?? */
	ROM_LOAD( "82s123.e18",	0x220, 0x020, CRC(bc88cced) SHA1(5055362710c0f58823c05fb4c0e0eec638b91e3d) )	/* sprite attribute (flip/position) ?? */
	ROM_LOAD( "82s123.b24",	0x240, 0x020, CRC(4fb5df2a) SHA1(506ef2c8e4cf45c256d6831a0a5760732f2de422) )	/* tile to sprite priority ?? */
ROM_END

GAME( 2002, twincbu2, twincobr, twincobr, twincobru, twincobr_state, twincobr, ROT270, "[Hack]", "Twin Cobra (Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, twintwst, twincobr, twincobr, twincobr,  twincobr_state, twincobr, ROT270, "[Hack]", "Twin Twist (Twin Cobra Hack)", MACHINE_SUPPORTS_SAVE )
