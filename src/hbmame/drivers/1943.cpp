// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/1943.cpp"

ROM_START( 1943h )
	ROM_REGION( 0x30000, "maincpu", 0 ) /* 64k for code + 128k for the banked ROMs images */
	ROM_LOAD( "bme01addontext.12d", 0x00000, 0x08000, CRC(36ab7c07) SHA1(baa9342c6c509b96e3c28a79284300707461a0fc) )
	ROM_LOAD( "bme02.13d", 0x10000, 0x10000, CRC(073fc57c) SHA1(6824fa387badd3c420f5c9e2b68159ac8a3aaec7) )
	ROM_LOAD( "bme03.14d", 0x20000, 0x10000, CRC(835822c2) SHA1(2c2fad13f062069efa7721abb9d807fb5a7625b4) ) /* These 3 roms have a BLUE stripe on them */

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "bm04.5h", 0x00000, 0x8000, CRC(ee2bd2d7) SHA1(4d2d019a9f8452fbbb247e893280568a2e86073e) )

	ROM_REGION( 0x10000, "mcu", 0 ) /*  C8751H-88 MCU Code */
	ROM_LOAD( "bm.7k", 0x00000, 0x10000 , NO_DUMP ) /* can't be dumped */

	ROM_REGION( 0x8000, "gfx1", 0 )
	ROM_LOAD( "bm05.4k", 0x00000, 0x8000, CRC(46cb9d3d) SHA1(96fd0e714b91fe13a2ca0d185ada9e4b4baa0c0b) )    /* characters */

	ROM_REGION( 0x40000, "gfx2", 0 )
	ROM_LOAD( "bm15.10f", 0x00000, 0x8000, CRC(6b1a0443) SHA1(32337c840ccd6815fd5844c194365c58d708f6dc) )   /* bg tiles */
	ROM_LOAD( "bm16.11f", 0x08000, 0x8000, CRC(23c908c2) SHA1(42b83ff5781be9181802a21ff1b23c17ab1bc5a2) )
	ROM_LOAD( "bm17.12f", 0x10000, 0x8000, CRC(46bcdd07) SHA1(38feda668be25d1adc04aa36afc73b07c1545f89) )
	ROM_LOAD( "bm18.14f", 0x18000, 0x8000, CRC(e6ae7ba0) SHA1(959c306dc28b9be2adc54b3d46312d26764c7b8b) )
	ROM_LOAD( "bm19.10j", 0x20000, 0x8000, CRC(868ababc) SHA1(1c7be905f53c63bad25fbbd9b3cf82d2c7749bc3) )
	ROM_LOAD( "bm20.11j", 0x28000, 0x8000, CRC(0917e5d4) SHA1(62dd277bc1fa54cfe168ae2380bc147bd17f4205) )
	ROM_LOAD( "bm21.12j", 0x30000, 0x8000, CRC(9bfb0d89) SHA1(f1bae7ec46edcf46c7af84c054e89b322f8c8972) )
	ROM_LOAD( "bm22.14j", 0x38000, 0x8000, CRC(04f3c274) SHA1(932780c04abe285e1ec67b726b145175f73eafe0) )

	ROM_REGION( 0x10000, "gfx3", 0 )
	ROM_LOAD( "bm24.14k", 0x00000, 0x8000, CRC(11134036) SHA1(88da112ab9fc7e0d8f0e901f273715b950ae588c) )   /* fg tiles */
	ROM_LOAD( "bm25.14l", 0x08000, 0x8000, CRC(092cf9c1) SHA1(19fe3c714b1d52cbb21dea25cdee5af841f525db) )

	ROM_REGION( 0x40000, "gfx4", 0 )
	ROM_LOAD( "bm06.10a", 0x00000, 0x8000, CRC(97acc8af) SHA1(c9fa07cb61f6905408b355edabfe453fb652ff0d) )   /* sprites */
	ROM_LOAD( "bm07.11a", 0x08000, 0x8000, CRC(d78f7197) SHA1(6367c7e80e80d4a0d33d7840b5c843c63c80123e) )
	ROM_LOAD( "bm08.12a", 0x10000, 0x8000, CRC(1a626608) SHA1(755c27a07728fd686168e9d9e4dee3d8f274892a) )
	ROM_LOAD( "bm09.14a", 0x18000, 0x8000, CRC(92408400) SHA1(3ab299bad1ba115efead53ebd92254abe7a092ba) )
	ROM_LOAD( "bm10.10c", 0x20000, 0x8000, CRC(8438a44a) SHA1(873629b00cf3f6d8976a7fdafe63cd16e47b7491) )
	ROM_LOAD( "bm11.11c", 0x28000, 0x8000, CRC(6c69351d) SHA1(c213d5c3e76a5749bc32539604716dcef6dcb694) )
	ROM_LOAD( "bm12.12c", 0x30000, 0x8000, CRC(5e7efdb7) SHA1(fef271a38dc1a9e45a0c6e27e28e713c77c8f8c9) )
	ROM_LOAD( "bm13.14c", 0x38000, 0x8000, CRC(1143829a) SHA1(2b3a65e354a205c05a87f783e9938b64bc62396f) )

	ROM_REGION( 0x10000, "gfx5", 0 )    /* tilemaps */
	ROM_LOAD( "bm14.5f", 0x0000, 0x8000, CRC(4d3c6401) SHA1(ce4f6dbf8fa030ad45cbb5afd58df27fed2d4618) ) /* front background */
	ROM_LOAD( "bm23.8k", 0x8000, 0x8000, CRC(a52aecbd) SHA1(45b0283d84d394c16c35802463ca95d70d1062d4) ) /* back background */

	ROM_REGION( 0x0c00, "proms", 0 )
	ROM_LOAD( "bm1.12a",  0x0000, 0x0100, CRC(74421f18) SHA1(5b8b59f6f4e5ad358611de50608f47f41a5b0e51) )    /* red component */
	ROM_LOAD( "bm2.13a",  0x0100, 0x0100, CRC(ac27541f) SHA1(1796c4c9041dfe28e6319576f21df1dbcb8d12bf) )    /* green component */
	ROM_LOAD( "bm3.14a",  0x0200, 0x0100, CRC(251fb6ff) SHA1(d1118159b3d429d841e4efa938728ebedadd7ec5) )    /* blue component */
	ROM_LOAD( "bm5.7f",   0x0300, 0x0100, CRC(206713d0) SHA1(fa609f6d675af18c379838583505724d28bcff0e) )    /* char lookup table */
	ROM_LOAD( "bm10.7l",  0x0400, 0x0100, CRC(33c2491c) SHA1(13da924e4b182759c4aae49034f3a7cbe556ea65) )    /* foreground lookup table */
	ROM_LOAD( "bm9.6l",   0x0500, 0x0100, CRC(aeea4af7) SHA1(98f4570ee061e9aa58d8ed2d2f8ae59ce2ec5795) )    /* foreground palette bank */
	ROM_LOAD( "bm12.12m", 0x0600, 0x0100, CRC(c18aa136) SHA1(684f04d9a5b94ae1db5fb95763e65271f4cf8e01) )    /* background lookup table */
	ROM_LOAD( "bm11.12l", 0x0700, 0x0100, CRC(405aae37) SHA1(94a06f81b775c4e49d57d42fc064d3072a253bbd) )    /* background palette bank */
	ROM_LOAD( "bm8.8c",   0x0800, 0x0100, CRC(c2010a9e) SHA1(be9852500209066e2f0ff2770e0c217d1636a0b5) )    /* sprite lookup table */
	ROM_LOAD( "bm7.7c",   0x0900, 0x0100, CRC(b56f30c3) SHA1(9f5e6db464d21457a33ec8bdfdff069632b791db) )    /* sprite palette bank */
	ROM_LOAD( "bm4.12c",  0x0a00, 0x0100, CRC(91a8a2e1) SHA1(9583c87eff876f04bc2ccf7218cd8081f1bcdb94) )    /* priority encoder / palette selector (not used) */
	ROM_LOAD( "bm6.4b",   0x0b00, 0x0100, CRC(0eaf5158) SHA1(bafd4108708f66cd7b280e47152b108f3e254fc9) )    /* video timing (not used) */
ROM_END

GAME( 1987, 1943h, 1943,  1943, 1943, _1943_state, 1943, ROT270, "Capcom", "1943: The Battle of Midway (US, Rev C) (hacked)", MACHINE_SUPPORTS_SAVE )
