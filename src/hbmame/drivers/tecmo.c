// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/tecmo.c"

ROM_START( geminit )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "gw04-5s.rom",   0x00000, 0x10000, CRC(ff9de855) SHA1(34167af8456a081f68b338f10d4319ce1e703fd4) )	/* c000-ffff is not used */
	ROM_LOAD( "gw05-6s.rom",   0x10000, 0x10000, CRC(5a6947a9) SHA1(18b7aeb0f0e2c396bc759118dd7c45fd6070b804) )	/* banked at f000-f7ff */

	ROM_REGION( 0x10000, "soundcpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "gw03-5h.rom",   0x0000,  0x8000,  CRC(9bc79596) SHA1(61de9ddd45140e8ed88173294bd26147e2abfa21) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "gw02-3h.rom",   0x00000, 0x08000, CRC(7acc8d35) SHA1(05056e9f077e7571b314390b508c72d56ad0f43b) )	/* characters */

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "t-gw06-1c.rom", 0x00000, 0x10000, CRC(c47b1c97) SHA1(7d8680afe4bc76ac712ef2e86bb43bbe02c77199) )	/* sprites */
	ROM_LOAD( "gw07-1d.rom",   0x10000, 0x10000, CRC(da42637e) SHA1(9885c52823279f26871092c77bdbe027df08268f) )	/* sprites */
	ROM_LOAD( "gw08-1f.rom",   0x20000, 0x10000, CRC(0b4e8d70) SHA1(55069f3df1c8db83f306d46b8262fd23585e6013) )	/* sprites */
	ROM_LOAD( "gw09-1h.rom",   0x30000, 0x10000, CRC(b65c5e4c) SHA1(699e1a9e72b8d94edae7382ba119fe5da113514d) )	/* sprites */

	ROM_REGION( 0x40000, "gfx3", 0 )
	ROM_LOAD( "gw10-1n.rom",   0x00000, 0x10000, CRC(5e84cd4f) SHA1(e85320291027a16619c87fc2365448367bda454a) )	/* tiles #1 */
	ROM_LOAD( "gw11-2na.rom",  0x10000, 0x10000, CRC(08b458e1) SHA1(b3426faa57dca51dc053db44fa4968425d8bf3ee) )	/* tiles #1 */
	ROM_LOAD( "gw12-2nb.rom",  0x20000, 0x10000, CRC(229c9714) SHA1(f4f47d6b379c973c22f9ae7d7bec7041cdf3f737) )	/* tiles #1 */
	ROM_LOAD( "gw13-3n.rom",   0x30000, 0x10000, CRC(c5dfaf47) SHA1(c3202ca8c7f3c5c7dc9acdc09c1c894e168ef9fe) )	/* tiles #1 */

	ROM_REGION( 0x40000, "gfx4", 0 )
	ROM_LOAD( "gw14-1r.rom",   0x00000, 0x10000, CRC(9c10e5b5) SHA1(a81399b85d8f3ddca26883ec3535cb9044c35ada) )	/* tiles #2 */
	ROM_LOAD( "gw15-2ra.rom",  0x10000, 0x10000, CRC(4cd18cfa) SHA1(c197a098a7c1e5220aad039383a40702fe7c4f21) )	/* tiles #2 */
	ROM_LOAD( "gw16-2rb.rom",  0x20000, 0x10000, CRC(f911c7be) SHA1(3f49f6c4734f2b644d93c4a54249aae6ff080e1d) )	/* tiles #2 */
	ROM_LOAD( "gw17-3r.rom",   0x30000, 0x10000, CRC(79a9ce25) SHA1(74e3917b8e7a920ceb2135d7ef8fb2f2c5176b21) )	/* tiles #2 */

	ROM_REGION( 0x8000, "adpcm", 0 )	/* ADPCM samples */
	ROM_LOAD( "gw01-6a.rom",   0x0000, 0x8000, CRC(d78afa05) SHA1(b02a739b045f5cddf943ce59226ef234463eeebe) )
ROM_END

ROM_START( rygark )
	ROM_REGION( 0x18000, "maincpu", 0 )	/* 64k for code */
	ROM_LOAD( "cpuj_5p.bin",  0x00000, 0x08000, CRC(b39698ba) SHA1(01a5a12a71973ad117b0bbd763e470f89c439e45) ) /* code */
	ROM_LOAD( "cpuj_5m.bin",  0x08000, 0x04000, CRC(3f180979) SHA1(c4c2e9f83b06b8677978800bfcc39f4ba3b344ab) ) /* code */
	ROM_LOAD( "cpuj_5j.bin",  0x10000, 0x08000, CRC(69e44e8f) SHA1(e979760a3582e64788c043adf7e475f0e1b75033) ) /* banked at f000-f7ff */

	ROM_REGION( 0x10000, "soundcpu", 0 )	/* 64k for the audio CPU */
	ROM_LOAD( "cpu_4h.bin",   0x0000, 0x2000, CRC(e4a2fa87) SHA1(ed58187dbbcf59358496a98ffd6c227a87d6c433) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "cpuk_8k.bin",   0x00000, 0x08000, CRC(370bd8c0) SHA1(a4f140c8890608afdee4617ad1165b491cd103d8) )	/* characters */

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "vid_6k.bin",   0x00000, 0x08000, CRC(aba6db9e) SHA1(43eb6f4f92afb5fbc11adc7e2ab04878ab56cb17) )	/* sprites */
	ROM_LOAD( "vid_6j.bin",   0x08000, 0x08000, CRC(ae1f2ed6) SHA1(6e6a33e665ba0884b7f57e9ad69d3f51e41d9e7b) )	/* sprites */
	ROM_LOAD( "vid_6h.bin",   0x10000, 0x08000, CRC(46d9e7df) SHA1(a24e0bea310a03636af704a0ad3f1a9cc4aafe12) )	/* sprites */
	ROM_LOAD( "vid_6g.bin",   0x18000, 0x08000, CRC(45839c9a) SHA1(eaee5767d8b0b62b991c089ef51b922e89850b79) )	/* sprites */

	ROM_REGION( 0x20000, "gfx3", 0 )
	ROM_LOAD( "vid_6p.bin",   0x00000, 0x08000, CRC(9eae5f8e) SHA1(ed83b608ca57b9bf69fa866d9b8f55d16b7cff63) )
	ROM_LOAD( "vid_6o.bin",   0x08000, 0x08000, CRC(5a10a396) SHA1(12ebed3952ff35a2c275cb27c915f82183048cd4) )
	ROM_LOAD( "vid_6n.bin",   0x10000, 0x08000, CRC(7b12cf3f) SHA1(6b9d8cad6e15317df01bab0591fab09199ca6d40) )
	ROM_LOAD( "vid_6l.bin",   0x18000, 0x08000, CRC(3cea7eaa) SHA1(1dd194d5672dfe71c2b27d2d7b76f5a611cff76f) )

	ROM_REGION( 0x20000, "gfx4", 0 )
	ROM_LOAD( "vid_6f.bin",   0x00000, 0x08000, CRC(9840edd8) SHA1(f19a1a1d932214037144c533ad07ed81256c34e7) )
	ROM_LOAD( "vid_6e.bin",   0x08000, 0x08000, CRC(ff65e074) SHA1(513c1bad336ef5d871f15d6ba8943020f98d1f4a) )
	ROM_LOAD( "vid_6c.bin",   0x10000, 0x08000, CRC(89868c85) SHA1(f21550f40e7a177e95c40f2726c651f85ca8edce) )
	ROM_LOAD( "vid_6b.bin",   0x18000, 0x08000, CRC(35389a7b) SHA1(a887a89f9bbb5979bb589468d80efba1f243690b) )

	ROM_REGION( 0x4000, "adpcm", 0 )	/* ADPCM samples */
	ROM_LOAD( "cpu_1f.bin",   0x0000, 0x4000, CRC(3cc98c5a) SHA1(ea1035be939ed1a994f3273b33412c85dda0973e) )
ROM_END

GAME( 2002, geminit, 0, gemini, gemini, tecmo_state, gemini, ROT90, "Twisty", "Gemini Wing Hack", MACHINE_SUPPORTS_SAVE )
GAME( 1986, rygark,  0, rygar,  rygar,  tecmo_state, rygar,  ROT0,  "Hack", "Rygar (Korean)", MACHINE_SUPPORTS_SAVE )
