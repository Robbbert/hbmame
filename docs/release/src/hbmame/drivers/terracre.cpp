// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/terracre.cpp"

ROM_START( terracrc )
	ROM_REGION( 0x20000, "maincpu", 0 )	/* 128K for 68000 code */
	ROM_LOAD16_BYTE( "terracrc.8",  0x00000, 0x4000, CRC(44e29950) SHA1(0446c803771e624a812b5b7bd422505a58fa2f9d) )
	ROM_LOAD16_BYTE( "terracrc.12", 0x00001, 0x4000, CRC(5d2d4fef) SHA1(b31c2c496013163a5504343f06b4f9cc9834f78a) )
	ROM_LOAD16_BYTE( "terracrc.7",  0x08000, 0x4000, CRC(907968e6) SHA1(8b8a17b87a08e2a5e4223d9bdbc4a7df0726a589) )
	ROM_LOAD16_BYTE( "terracrc.11", 0x08001, 0x4000, CRC(f5dcd168) SHA1(d58c76d1f199e664caae6a89b39d8dddb758d593) )
	ROM_LOAD16_BYTE( "terracrc.6",  0x10000, 0x4000, CRC(b2cb4442) SHA1(a7a3a2d2a509bfcc905c7bfda516147626e09782) )
	ROM_LOAD16_BYTE( "terracrc.10", 0x10001, 0x4000, CRC(66db3b3b) SHA1(637b6dec3e03bd80ba4efdb08e9604bf3deea49b) )
	ROM_LOAD16_BYTE( "terracrc.5",  0x18000, 0x4000, CRC(a1bea44f) SHA1(cda2f0f7feac9060f86392704907b86cb5857b47) )
	ROM_LOAD16_BYTE( "1a_9b.rom",   0x18001, 0x4000, CRC(69227b56) SHA1(58c8aa4baa1f5ddfc151f5ed6284a06e87866dd7) )
	ROM_FILL(0x1c16,1,1)
	ROM_FILL(0x1c17,1,0x1c)	// bypass rom checksum - one of the roms might be bad

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "2a_15b.rom",   0x0000, 0x4000, CRC(604c3b11) SHA1(c01d1ddae40fa8b65dfc72f959942cb9664a548b) )
	ROM_LOAD( "terracrc.19",  0x4000, 0x4000, CRC(20fd3b53) SHA1(baf494f64a66fc4ae9d2fef12ae7c6f61ad82d94) )
	ROM_LOAD( "2a_18b.rom",   0x8000, 0x4000, CRC(302dc0ab) SHA1(4db8f12e70f9adf1eb993c6a8af68b5edbf79773) )

	ROM_REGION( 0x02000, "gfx1", 0 )
	ROM_LOAD( "2a_16b.rom",   0x00000, 0x2000, CRC(591a3804) SHA1(e1b46f5652e7f9677d75f01c6132975ace4facdd) ) /* tiles */

	ROM_REGION( 0x10000, "gfx2", 0 )
	ROM_LOAD( "terracrc.4",   0x0000, 0x4000, CRC(3597f737) SHA1(443ad8b868a20a28a739df2f677803cb94012919) )
	ROM_LOAD( "terracrc.2",   0x4000, 0x4000, CRC(26474fd6) SHA1(7e8b725d5a29cc328925eea1ccdc2edaa2e7a927) )
	ROM_LOAD( "terracrc.3",   0x8000, 0x4000, CRC(f5f66a67) SHA1(d7761634fd0ea725e4fdaff698171285f9042f0f) )
	ROM_LOAD( "terracrc.1",   0xc000, 0x4000, CRC(55f80572) SHA1(a32749f8539a2a908532365f7f5650c92fc19f42) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "2a_6e.rom",    0x00000, 0x4000, CRC(bcf7740b) SHA1(8701862c35eb8fb1ec239253136a3858ebea4d0c) ) /* Sprites */
	ROM_LOAD( "2a_7e.rom",    0x04000, 0x4000, CRC(a70b565c) SHA1(153e5f5a9927c294660dd0d636a9f651d4984d6d) )
	ROM_LOAD( "2a_6g.rom",    0x08000, 0x4000, CRC(4a9ec3e6) SHA1(0a35b82fb49ecf7edafd02744a48490e744c0a00) )
	ROM_LOAD( "2a_7g.rom",    0x0c000, 0x4000, CRC(450749fc) SHA1(376ab98ab8db56ed45f7d97a221dfd52e389cb5a) )

	ROM_REGION( 0x0504, "proms", 0 )	/* first 4 not supplied - using from terracre */
	ROM_LOAD( "tc1a_10f.bin", 0x0000, 0x0100, CRC(ce07c544) SHA1(c3691cb420c88f1887a55e3035b5d017decbc17a) )	/* red component */
	ROM_LOAD( "tc1a_11f.bin", 0x0100, 0x0100, CRC(566d323a) SHA1(fe83585a0d9c7f942a5e54620b627a5a17a0fcf4) )	/* green component */
	ROM_LOAD( "tc1a_12f.bin", 0x0200, 0x0100, CRC(7ea63946) SHA1(d7b89694a80736c7605b5c83d25d8b706f4504ab) )	/* blue component */
	ROM_LOAD( "tc2a_2g.bin",  0x0300, 0x0100, CRC(08609bad) SHA1(e5daee3c3fea6620e3c2b91becd93bc4d3cdf011) )	/* sprite lookup table */
	/* This came with dump - no idea what it does - not hooked up */
	ROM_LOAD_OPTIONAL( "pal16r4.bin", 0x0400, 0x0104, CRC(119ef878) SHA1(4cc68200c755d646789f6af1c7a52273f07f597c) )

	ROM_REGION( 0x0100, "user1", 0 )	/* not supplied - using from terracre */
	ROM_LOAD( "tc2a_4e.bin",  0x0000, 0x0100, CRC(2c43991f) SHA1(312112832bee511b0545524295aa9bc2e756db0f) )	/* sprite palette bank */
ROM_END

GAME( 1985, terracrc, terracre, ym3526, terracre, driver_device, 0, ROT270,  "Nichibutsu", "Terra Cresta (alt)", MACHINE_SUPPORTS_SAVE )
