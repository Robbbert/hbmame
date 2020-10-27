// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/tecmo.cpp"

ROM_START( geminit ) // same as gemini except for 1c
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "gw04-5s.rom",   0x00000, 0x10000, CRC(ff9de855) SHA1(34167af8456a081f68b338f10d4319ce1e703fd4) )
	ROM_LOAD( "gw05-6s.rom",   0x10000, 0x10000, CRC(5a6947a9) SHA1(18b7aeb0f0e2c396bc759118dd7c45fd6070b804) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "gw03-5h.rom",   0x0000,  0x8000,  CRC(9bc79596) SHA1(61de9ddd45140e8ed88173294bd26147e2abfa21) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "gw02-3h.rom",   0x00000, 0x08000, CRC(7acc8d35) SHA1(05056e9f077e7571b314390b508c72d56ad0f43b) )

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "geminit.1c",    0x00000, 0x10000, CRC(c47b1c97) SHA1(7d8680afe4bc76ac712ef2e86bb43bbe02c77199) )
	ROM_LOAD( "gw07-1d.rom",   0x10000, 0x10000, CRC(da42637e) SHA1(9885c52823279f26871092c77bdbe027df08268f) )
	ROM_LOAD( "gw08-1f.rom",   0x20000, 0x10000, CRC(0b4e8d70) SHA1(55069f3df1c8db83f306d46b8262fd23585e6013) )
	ROM_LOAD( "gw09-1h.rom",   0x30000, 0x10000, CRC(b65c5e4c) SHA1(699e1a9e72b8d94edae7382ba119fe5da113514d) )

	ROM_REGION( 0x40000, "gfx3", 0 )
	ROM_LOAD( "gw10-1n.rom",   0x00000, 0x10000, CRC(5e84cd4f) SHA1(e85320291027a16619c87fc2365448367bda454a) )
	ROM_LOAD( "gw11-2na.rom",  0x10000, 0x10000, CRC(08b458e1) SHA1(b3426faa57dca51dc053db44fa4968425d8bf3ee) )
	ROM_LOAD( "gw12-2nb.rom",  0x20000, 0x10000, CRC(229c9714) SHA1(f4f47d6b379c973c22f9ae7d7bec7041cdf3f737) )
	ROM_LOAD( "gw13-3n.rom",   0x30000, 0x10000, CRC(c5dfaf47) SHA1(c3202ca8c7f3c5c7dc9acdc09c1c894e168ef9fe) )

	ROM_REGION( 0x40000, "gfx4", 0 )
	ROM_LOAD( "gw14-1r.rom",   0x00000, 0x10000, CRC(9c10e5b5) SHA1(a81399b85d8f3ddca26883ec3535cb9044c35ada) )
	ROM_LOAD( "gw15-2ra.rom",  0x10000, 0x10000, CRC(4cd18cfa) SHA1(c197a098a7c1e5220aad039383a40702fe7c4f21) )
	ROM_LOAD( "gw16-2rb.rom",  0x20000, 0x10000, CRC(f911c7be) SHA1(3f49f6c4734f2b644d93c4a54249aae6ff080e1d) )
	ROM_LOAD( "gw17-3r.rom",   0x30000, 0x10000, CRC(79a9ce25) SHA1(74e3917b8e7a920ceb2135d7ef8fb2f2c5176b21) )

	ROM_REGION( 0x8000, "adpcm", 0 )
	ROM_LOAD( "gw01-6a.rom",   0x0000, 0x8000, CRC(d78afa05) SHA1(b02a739b045f5cddf943ce59226ef234463eeebe) )
ROM_END

ROM_START( rygark ) // same as rygarj except for gfx1
	ROM_REGION( 0x20000, "maincpu", 0 )

	ROM_LOAD( "cpuj_5p.bin",  0x00000, 0x08000, CRC(b39698ba) SHA1(01a5a12a71973ad117b0bbd763e470f89c439e45) )
	ROM_LOAD( "cpuj_5m.bin",  0x08000, 0x04000, CRC(3f180979) SHA1(c4c2e9f83b06b8677978800bfcc39f4ba3b344ab) )
	ROM_LOAD( "cpuj_5j.bin",  0x10000, 0x08000, CRC(69e44e8f) SHA1(e979760a3582e64788c043adf7e475f0e1b75033) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "cpu_4h.bin",   0x0000, 0x2000, CRC(e4a2fa87) SHA1(ed58187dbbcf59358496a98ffd6c227a87d6c433) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "rygark.8k",    0x00000, 0x08000, CRC(370bd8c0) SHA1(a4f140c8890608afdee4617ad1165b491cd103d8) )

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "vid_6k.bin",   0x00000, 0x08000, CRC(aba6db9e) SHA1(43eb6f4f92afb5fbc11adc7e2ab04878ab56cb17) )
	ROM_LOAD( "vid_6j.bin",   0x08000, 0x08000, CRC(ae1f2ed6) SHA1(6e6a33e665ba0884b7f57e9ad69d3f51e41d9e7b) )
	ROM_LOAD( "vid_6h.bin",   0x10000, 0x08000, CRC(46d9e7df) SHA1(a24e0bea310a03636af704a0ad3f1a9cc4aafe12) )
	ROM_LOAD( "vid_6g.bin",   0x18000, 0x08000, CRC(45839c9a) SHA1(eaee5767d8b0b62b991c089ef51b922e89850b79) )

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

	ROM_REGION( 0x4000, "adpcm", 0 )
	ROM_LOAD( "cpu_1f.bin",   0x0000, 0x4000, CRC(3cc98c5a) SHA1(ea1035be939ed1a994f3273b33412c85dda0973e) )
ROM_END

GAME( 2002, geminit, gemini, gemini, gemini, tecmo_state, init_gemini, ROT90, "Twisty", "Gemini Wing (Unknown Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1986, rygark,  rygar,  rygar,  rygar,  tecmo_state, init_rygar,  ROT0,  "hack", "Argus no Senshi (Translation Korean)", MACHINE_SUPPORTS_SAVE )

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /***********
 Gemini Wing
**************/

ROM_START( geminis01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "gw04-5shc01.rom",  0x00000, 0x10000, CRC(b73a2776) SHA1(20d8f203207f372f80f169b6e3a54a99967a93dc) )
	ROM_LOAD( "gw05-6s.rom",  0x10000, 0x10000, CRC(5a6947a9) SHA1(18b7aeb0f0e2c396bc759118dd7c45fd6070b804) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "gw03-5h.rom",  0x0000, 0x8000, CRC(9bc79596) SHA1(61de9ddd45140e8ed88173294bd26147e2abfa21) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "gw02-3hhc01.rom",  0x00000, 0x08000, CRC(3ddd13f3) SHA1(b7b1f3b0c1b457fa7744f6fedb694cdd855372c9) )

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "gw06-1c.rom",  0x00000, 0x10000, CRC(4ea51631) SHA1(9aee0f1ba210ac953dc193cfc739322966b6de8a) )
	ROM_LOAD( "gw07-1d.rom",  0x10000, 0x10000, CRC(da42637e) SHA1(9885c52823279f26871092c77bdbe027df08268f) )
	ROM_LOAD( "gw08-1f.rom",  0x20000, 0x10000, CRC(0b4e8d70) SHA1(55069f3df1c8db83f306d46b8262fd23585e6013) )
	ROM_LOAD( "gw09-1h.rom",  0x30000, 0x10000, CRC(b65c5e4c) SHA1(699e1a9e72b8d94edae7382ba119fe5da113514d) )

	ROM_REGION( 0x40000, "gfx3", 0 )
	ROM_LOAD( "gw10-1n.rom",  0x00000, 0x10000, CRC(5e84cd4f) SHA1(e85320291027a16619c87fc2365448367bda454a) )
	ROM_LOAD( "gw11-2na.rom", 0x10000, 0x10000, CRC(08b458e1) SHA1(b3426faa57dca51dc053db44fa4968425d8bf3ee) )
	ROM_LOAD( "gw12-2nb.rom", 0x20000, 0x10000, CRC(229c9714) SHA1(f4f47d6b379c973c22f9ae7d7bec7041cdf3f737) )
	ROM_LOAD( "gw13-3n.rom",  0x30000, 0x10000, CRC(c5dfaf47) SHA1(c3202ca8c7f3c5c7dc9acdc09c1c894e168ef9fe) )

	ROM_REGION( 0x40000, "gfx4", 0 )
	ROM_LOAD( "gw14-1r.rom",  0x00000, 0x10000, CRC(9c10e5b5) SHA1(a81399b85d8f3ddca26883ec3535cb9044c35ada) )
	ROM_LOAD( "gw15-2ra.rom", 0x10000, 0x10000, CRC(4cd18cfa) SHA1(c197a098a7c1e5220aad039383a40702fe7c4f21) )
	ROM_LOAD( "gw16-2rb.rom", 0x20000, 0x10000, CRC(f911c7be) SHA1(3f49f6c4734f2b644d93c4a54249aae6ff080e1d) )
	ROM_LOAD( "gw17-3r.rom",  0x30000, 0x10000, CRC(79a9ce25) SHA1(74e3917b8e7a920ceb2135d7ef8fb2f2c5176b21) )

	ROM_REGION( 0x8000, "adpcm", 0 )
	ROM_LOAD( "gw01-6a.rom",  0x0000, 0x8000, CRC(d78afa05) SHA1(b02a739b045f5cddf943ce59226ef234463eeebe) )
ROM_END

 /***************
 Argus no Senshi
******************/

ROM_START( rygarjs01 )
	ROM_REGION( 0x20000, "maincpu", 0 )

	ROM_LOAD( "cpuj_5phc01.bin",  0x00000, 0x08000, CRC(0262b88b) SHA1(42135f50c2c416b749a364632ab174703a3781c3) )
	ROM_LOAD( "cpuj_5m.bin",  0x08000, 0x04000, CRC(3f180979) SHA1(c4c2e9f83b06b8677978800bfcc39f4ba3b344ab) )
	ROM_LOAD( "cpuj_5jhc1.bin",  0x10000, 0x08000, CRC(21eb55c0) SHA1(83ec2103ea3ce6d400f2f91d93d5f6f8bdb09e9e) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "cpu_4h.bin",   0x0000, 0x2000, CRC(e4a2fa87) SHA1(ed58187dbbcf59358496a98ffd6c227a87d6c433) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "cpuj_8khc01.bin",  0x00000, 0x08000, CRC(c020cfd3) SHA1(8e0d6cfad07e44d6e08dd4fda160497dd8667f42) )

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "vid_6k.bin",   0x00000, 0x08000, CRC(aba6db9e) SHA1(43eb6f4f92afb5fbc11adc7e2ab04878ab56cb17) )
	ROM_LOAD( "vid_6j.bin",   0x08000, 0x08000, CRC(ae1f2ed6) SHA1(6e6a33e665ba0884b7f57e9ad69d3f51e41d9e7b) )
	ROM_LOAD( "vid_6h.bin",   0x10000, 0x08000, CRC(46d9e7df) SHA1(a24e0bea310a03636af704a0ad3f1a9cc4aafe12) )
	ROM_LOAD( "vid_6g.bin",   0x18000, 0x08000, CRC(45839c9a) SHA1(eaee5767d8b0b62b991c089ef51b922e89850b79) )

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

	ROM_REGION( 0x4000, "adpcm", 0 )
	ROM_LOAD( "cpu_1f.bin",   0x0000, 0x4000, CRC(3cc98c5a) SHA1(ea1035be939ed1a994f3273b33412c85dda0973e) )
ROM_END

ROM_START( rygars01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "5_ps01.5p",         0x00000, 0x08000, CRC(0e13e0e4) SHA1(e4eed9e4f424838cd80303554d98048b42a197ef) )
	ROM_LOAD( "cpu_5m.bin",   0x08000, 0x04000, CRC(7ac5191b) SHA1(305f39d974f906f9bc24e9fe2ca58e647925ab63) )
	ROM_LOAD( "cpu_5j.bin",   0x10000, 0x08000, CRC(ed76d606) SHA1(39c8a07e9a1f218ad088d00a2c9dfc993efafb6b) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "cpu_4h.bin",   0x0000, 0x2000, CRC(e4a2fa87) SHA1(ed58187dbbcf59358496a98ffd6c227a87d6c433) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "cpu_8k.bin",   0x00000, 0x08000, CRC(4d482fb6) SHA1(57ad838b6d30b49dbd2d0ec425f33cfb15a67918) )

	ROM_REGION( 0x20000, "gfx2", 0 )
	ROM_LOAD( "vid_6k.bin",   0x00000, 0x08000, CRC(aba6db9e) SHA1(43eb6f4f92afb5fbc11adc7e2ab04878ab56cb17) )
	ROM_LOAD( "vid_6j.bin",   0x08000, 0x08000, CRC(ae1f2ed6) SHA1(6e6a33e665ba0884b7f57e9ad69d3f51e41d9e7b) )
	ROM_LOAD( "vid_6h.bin",   0x10000, 0x08000, CRC(46d9e7df) SHA1(a24e0bea310a03636af704a0ad3f1a9cc4aafe12) )
	ROM_LOAD( "vid_6g.bin",   0x18000, 0x08000, CRC(45839c9a) SHA1(eaee5767d8b0b62b991c089ef51b922e89850b79) )

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

	ROM_REGION( 0x4000, "adpcm", 0 )
	ROM_LOAD( "cpu_1f.bin",   0x0000, 0x4000, CRC(3cc98c5a) SHA1(ea1035be939ed1a994f3273b33412c85dda0973e) )
ROM_END

 /*********
 Silk Worm
************/

ROM_START( silkworms01 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	ROM_LOAD( "silkwormhc01.4",   0x00000, 0x10000, CRC(73f3eb16) SHA1(89ec812f9b1d201e4c9c9774e8e8be1848eba334) )
	ROM_LOAD( "silkworm.5",   0x10000, 0x10000, CRC(a6c7bb51) SHA1(75f6625459ab65f2d47a282c1295d4db38f5fe51) )

	ROM_REGION( 0x20000, "soundcpu", 0 )
	ROM_LOAD( "silkworm.3",   0x0000, 0x8000, CRC(b589f587) SHA1(0be5e2bf3daf3e28d63fdc8c89bb6fe7c48c6c3f) )

	ROM_REGION( 0x08000, "gfx1", 0 )
	ROM_LOAD( "silkwormhc01.2",   0x00000, 0x08000, CRC(dd312a50) SHA1(fe398f4e0991c5c1f746851f224dcad6b8a667df) )

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "silkworm.6",   0x00000, 0x10000, CRC(1138d159) SHA1(3b938606d448c4effdfe414bbf495b50cc3bc1c1) )
	ROM_LOAD( "silkworm.7",   0x10000, 0x10000, CRC(d96214f7) SHA1(a5b2be3ae6a6eb8afef2c18c865a998fbf4adf93) )
	ROM_LOAD( "silkworm.8",   0x20000, 0x10000, CRC(0494b38e) SHA1(03255f153824056e430a0b8595103f3b58b1fd97) )
	ROM_LOAD( "silkworm.9",   0x30000, 0x10000, CRC(8ce3cdf5) SHA1(635248514c4e1e5aab7a2ed4d620a5b970d4a43a) )

	ROM_REGION( 0x40000, "gfx3", 0 )
	ROM_LOAD( "silkworm.10",  0x00000, 0x10000, CRC(8c7138bb) SHA1(0cfd69fa77d5b546f7dad80537d8d2497ae758bc) )
	ROM_LOAD( "silkworm.11",  0x10000, 0x10000, CRC(6c03c476) SHA1(79ad800a2f4ba6d44ba5a31210cbd8566bb357b6) )
	ROM_LOAD( "silkworm.12",  0x20000, 0x10000, CRC(bb0f568f) SHA1(b66c6d0407ed0b068c6bf07987f1b923d4a6e4f8) )
	ROM_LOAD( "silkworm.13",  0x30000, 0x10000, CRC(773ad0a4) SHA1(f7576e1ac8c779b33d7ec393555fd097a34257fa) )

	ROM_REGION( 0x40000, "gfx4", 0 )
	ROM_LOAD( "silkworm.14",  0x00000, 0x10000, CRC(409df64b) SHA1(cada970bf9cc8f6522e7a71e00fe873568852873) )
	ROM_LOAD( "silkworm.15",  0x10000, 0x10000, CRC(6e4052c9) SHA1(e2e3d7221b75cb044449a25a076a93c3def1f11b) )
	ROM_LOAD( "silkworm.16",  0x20000, 0x10000, CRC(9292ed63) SHA1(70aa46fcc187b8200c5d246870e2e2dc4b2985cb) )
	ROM_LOAD( "silkworm.17",  0x30000, 0x10000, CRC(3fa4563d) SHA1(46e3cc41491d63efcdda43c84c7ac1385a1926d0) )

	ROM_REGION( 0x8000, "adpcm", 0 )
	ROM_LOAD( "silkworm.1",   0x0000, 0x8000, CRC(5b553644) SHA1(5d39d2251094c17f7b732b4861401b3516fce9b1) )
ROM_END

/*    YEAR  NAME          PARENT    MACHINE   INPUT     CLASS        INIT           MONITOR COMPANY                 FULLNAME FLAGS */
// Gemini Wing
GAME( 1987, geminis01,    gemini,   gemini,   gemini,   tecmo_state, init_gemini,   ROT90, "hack",   "Gemini Wing (Translation Chinese)",          MACHINE_SUPPORTS_SAVE )
// Argus no Senshi
GAME( 1986, rygarjs01,    rygar,    rygar,    rygar,    tecmo_state, init_rygar,    ROT0,  "hack",   "Argus no Senshi (Translation Chinese)",      MACHINE_SUPPORTS_SAVE )
GAME( 1986, rygars01,     rygar,    rygar,    rygar,    tecmo_state, init_rygar,    ROT0,  "Tecmo",   "Rygar (US, bootleg)",             MACHINE_SUPPORTS_SAVE )
// Silk Worm
GAME( 1988, silkworms01,  silkworm, silkworm, silkworm, tecmo_state, init_silkworm, ROT0,  "hack",   "Silk Worm (Translation Chinese)",            MACHINE_SUPPORTS_SAVE )


