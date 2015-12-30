// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/exerion.cpp"

ROM_START( exriongr )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "exriongr.07",  0x0000, 0x2000, CRC(0ade934a) SHA1(a949aa66f27876ca439b799a9918841f7d8d3115) )
	ROM_LOAD( "exriongr.08",  0x2000, 0x2000, CRC(a469f93e) SHA1(7e800e4c0dce2b9c9525dc1667ca13f1b2844d9b) )
	ROM_LOAD( "exriongr.09",  0x4000, 0x2000, CRC(f0a8d5db) SHA1(df0f18ebef27a70ef420903a02ec5ce3c43d1d8b) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "exerion.05",   0x0000, 0x2000, CRC(32f6bff5) SHA1(a4d0289f9d1d9eea7ca9a32a0616af48da74b401) )

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "exriongr.06",  0x00000, 0x2000, CRC(e18dc7c6) SHA1(1ec9bb9aa1552e4d7dbe16435f98209813590211) ) /* fg chars */

	ROM_REGION( 0x04000, "gfx2", 0 )
	ROM_LOAD( "exerion.11",   0x00000, 0x2000, CRC(f0633a09) SHA1(8989bcb12abadde34777f7c189cfa6e2dfe92d62) ) /* sprites */
	ROM_LOAD( "exerion.10",   0x02000, 0x2000, CRC(80312de0) SHA1(4fa3bb9d5c62e41a54e8909f8d3b47637137e913) )

	ROM_REGION( 0x08000, "gfx3", 0 )
	ROM_LOAD( "exerion.03",   0x00000, 0x2000, CRC(790595b8) SHA1(8016ac2394b25db38e962bcff4805380082f6683) ) /* bg data */
	ROM_LOAD( "exerion.04",   0x02000, 0x2000, CRC(d7abd0b9) SHA1(ca6413ecd324cf84e11b703a4eda2c1e6d28ff15) )
	ROM_LOAD( "exriongr.01",  0x04000, 0x2000, CRC(b18009ea) SHA1(6a7abbe60e493a125b5d0e4d7f114b15758e4c6e) )
	ROM_LOAD( "exerion.02",   0x06000, 0x2000, CRC(a7ecbb70) SHA1(3c359d5bb21290a45d3eb18fea2b1f9439b931be) )

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "exerion.e1",   0x0000, 0x0020, CRC(2befcc20) SHA1(a24d3f691413378fde545a6ddcef7e5118e74019) ) /* palette */
	ROM_LOAD( "exerion.i8",   0x0020, 0x0100, CRC(31db0e08) SHA1(1041a778e86d3fe6f057cf40a0a08b30760f3887) ) /* fg char lookup table */
	ROM_LOAD( "exerion.h10",  0x0120, 0x0100, CRC(63b4c555) SHA1(30243041be4fa77ada71e8b29d721cad51640c29) ) /* sprite lookup table */
	ROM_LOAD( "exerion.i3",   0x0220, 0x0100, CRC(fe72ab79) SHA1(048a72e6db4768df687df927acaa70ef906b3dc0) ) /* bg char lookup table */
	ROM_LOAD( "exerion.k4",   0x0320, 0x0100, CRC(ffc2ba43) SHA1(03be1c41d6ac3fc11439caef04ef5ffa60d6aec4) ) /* bg char mixer */
ROM_END

ROM_START( exerionj )
	ROM_REGION( 0x10000, "maincpu", 0 )     /* 64k for code */
	ROM_LOAD( "exerion.07",   0x0000, 0x2000, CRC(4c78d57d) SHA1(ac702e9ad2bc05493fb1355858667c31c36acfe4) )
	ROM_LOAD( "exerion.08",   0x2000, 0x2000, CRC(dcadc1df) SHA1(91388f617cfaa4289ca1c84c697fcfdd8834ae15) )
	ROM_LOAD( "exerion.09",   0x4000, 0x2000, CRC(34cc4d14) SHA1(511c9de038f7bcaf6f7c96f2cbbe50a80673fa72) )

	ROM_REGION( 0x10000, "sub", 0 )     /* 64k for the second CPU */
	ROM_LOAD( "exerion.05",   0x0000, 0x2000, CRC(32f6bff5) SHA1(a4d0289f9d1d9eea7ca9a32a0616af48da74b401) )

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "exerion.06",   0x00000, 0x2000, CRC(435a85a4) SHA1(f6846bfee11df754405d4d796e7d8ac0321b6eb6) ) /* fg chars */

	ROM_REGION( 0x04000, "gfx2", 0 )
	ROM_LOAD( "exerion.11",   0x00000, 0x2000, CRC(f0633a09) SHA1(8989bcb12abadde34777f7c189cfa6e2dfe92d62) ) /* sprites */
	ROM_LOAD( "exerion.10",   0x02000, 0x2000, CRC(80312de0) SHA1(4fa3bb9d5c62e41a54e8909f8d3b47637137e913) )

	ROM_REGION( 0x08000, "gfx3", 0 )
	ROM_LOAD( "exerion.03",   0x00000, 0x2000, CRC(790595b8) SHA1(8016ac2394b25db38e962bcff4805380082f6683) ) /* bg data */
	ROM_LOAD( "exerion.04",   0x02000, 0x2000, CRC(d7abd0b9) SHA1(ca6413ecd324cf84e11b703a4eda2c1e6d28ff15) )
	ROM_LOAD( "exerion.01",   0x04000, 0x2000, CRC(5bb755cb) SHA1(ec92c518c116a78dbb23381468cefb3f930212cc) )
	ROM_LOAD( "exerion.02",   0x06000, 0x2000, CRC(a7ecbb70) SHA1(3c359d5bb21290a45d3eb18fea2b1f9439b931be) )

	ROM_REGION( 0x0420, "proms", 0 )
	ROM_LOAD( "exerionj.e1",  0x0000, 0x0020, CRC(60cec362) SHA1(2422e894e2def2ab31f4b6c14fabbe443bd495e6) ) /* palette */
	ROM_LOAD( "exerion.i8",   0x0020, 0x0100, CRC(31db0e08) SHA1(1041a778e86d3fe6f057cf40a0a08b30760f3887) ) /* fg char lookup table */
	ROM_LOAD( "exerion.h10",  0x0120, 0x0100, CRC(63b4c555) SHA1(30243041be4fa77ada71e8b29d721cad51640c29) ) /* sprite lookup table */
	ROM_LOAD( "exerion.i3",   0x0220, 0x0100, CRC(fe72ab79) SHA1(048a72e6db4768df687df927acaa70ef906b3dc0) ) /* bg char lookup table */
	ROM_LOAD( "exerion.k4",   0x0320, 0x0100, CRC(ffc2ba43) SHA1(03be1c41d6ac3fc11439caef04ef5ffa60d6aec4) ) /* bg char mixer */
ROM_END

GAME( 2004, exriongr, exerionj, exerion, exerion, exerion_state, exerion,  ROT90, "GreekRoms", "Exerion (Greek)", MACHINE_SUPPORTS_SAVE )
GAME( 1983, exerionj, 0,        exerion, exerion, exerion_state, exerion,  ROT90, "Jaleco", "Exerion (Japan)", MACHINE_SUPPORTS_SAVE )
