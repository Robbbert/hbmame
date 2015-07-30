// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/bagman.c"

ROM_START( bagturbo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "bagt_a4_9e.bin",  0x0000, 0x1000, CRC(c070635c) SHA1(98c920f9ab377258d376e62337f14df3e51badaa) )
	ROM_LOAD( "a5-9f",           0x1000, 0x1000, CRC(2ddf6bb9) SHA1(151068dddc55163bb6f925f68e5d04e347ded6a5) )
	ROM_LOAD( "a4_9j.bin",       0x2000, 0x1000, CRC(b2da8b77) SHA1(ea36cd6be42c5548a9a91054aeebb4b985ba15c9) )
	ROM_LOAD( "a5-9k",           0x3000, 0x1000, CRC(f91d617b) SHA1(a3323b51277e08747701cc4e2d3a9c466e96d4c1) )
	ROM_LOAD( "bagt_a4_9m.bin",  0x4000, 0x1000, CRC(8cb278fe) SHA1(807a4f1d89dcbd7482ef86f4b2e88cc2f64abbf4) )
	ROM_LOAD( "a5-9n",           0x5000, 0x1000, CRC(68e4b64d) SHA1(55950d7c07c621cafa001d5d3bfec6bbc02712e2) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "a2_1e.bin",       0x0000, 0x1000, CRC(f217ac09) SHA1(a9716674401dff27344a01df8121b6b648688680) )
	ROM_LOAD( "j1_b04.bin",      0x1000, 0x1000, CRC(c680ef04) SHA1(79406bc786374abfcd9f548268c445b5c8d8858d) )

	ROM_REGION( 0x2000, "gfx2", 0 )
	ROM_LOAD( "a2_1c.bin",       0x0000, 0x1000, CRC(f3e11bd7) SHA1(43ee00ff777008c89f619eb183e7c5e63f6c7694) )
	ROM_LOAD( "a2_1f.bin",       0x1000, 0x1000, CRC(d0f7105b) SHA1(fb382703850a4ded567706e02ebb7f3e22531b7c) )

	ROM_REGION( 0x0060, "proms", 0 )
	ROM_LOAD( "p3.bin",          0x0000, 0x0020, CRC(2a855523) SHA1(91e032233fee397c90b7c1662934aca9e0671482) )
	ROM_LOAD( "r3.bin",          0x0020, 0x0020, CRC(ae6f1019) SHA1(fd711882b670380cb4bd909c840ba06277b8fbe3) )

	ROM_REGION( 0x0060, "5110ctrl", 0)
	ROM_LOAD( "r6.bin",          0x0000, 0x0020, CRC(c58a4f6a) SHA1(35ef244b3e94032df2610aa594ea5670b91e1449) ) /*state machine driving TMS5110*/

	ROM_REGION( 0x2000, "tmsprom", 0 ) /* data for the TMS5110 speech chip */
	ROM_LOAD( "r9_b11.bin",      0x0000, 0x1000, CRC(2e0057ff) SHA1(33e3ffa6418f86864eb81e5e9bda4bf540c143a6) )
	ROM_LOAD( "t9_b12.bin",      0x1000, 0x1000, CRC(b2120edd) SHA1(52b89dbcc749b084331fa82b13d0876e911fce52) )
ROM_END

ROM_START( pickinpi )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "9e",           0x0000, 0x1000, CRC(efd0bd43) SHA1(b70a471a809c08286a82934046357fb46556f641) )
	ROM_LOAD( "9f",           0x1000, 0x1000, CRC(b5785a23) SHA1(9eddda5695981cb0470dfea68d5e2e8e220382b1) )
	ROM_LOAD( "9j",           0x2000, 0x1000, CRC(65ee9fd4) SHA1(2efa40c19a7b0644ef4f4b2ce6a025b2b880239d) )
	ROM_LOAD( "9k",           0x3000, 0x1000, CRC(7b23350e) SHA1(dff19602a0e46ca0bcdbdf2a1d61fd2c80ac70e7) )
	ROM_LOAD( "9m",           0x4000, 0x1000, CRC(935a7248) SHA1(d9af4405d51ce1ff6c4b84709dc85c0db88b1d54) )
	ROM_LOAD( "9n",           0x5000, 0x1000, CRC(52485d1d) SHA1(c309eec506f978388463f20d56d958e6639c31e8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "1f",           0x0000, 0x1000, CRC(c5e96ac6) SHA1(b2d740b6d07c765e8eb2dce31fe285a15a9fe597) )
	ROM_LOAD( "1j",           0x1000, 0x1000, CRC(41c4ac1c) SHA1(aac58a9d675a9b70140d82341231bcf6c77c7b41) )

	/* no gfx2 */

	ROM_REGION( 0x0040, "proms", 0 )
	ROM_LOAD( "6331-1pi.3p",  0x0000, 0x0020, CRC(b1dd1e68) SHA1(059c5f6c43e62b9381f4a0fa322dbe59c29812f5) )
	ROM_LOAD( "6331-1pi.3r",  0x0020, 0x0020, CRC(47fadc93) SHA1(3ba46957e664a2ea639a97a0c199a9636861ca50) )
ROM_END

GAME(1991, bagturbo, 0, bagman, bagmans, bagman_state,  bagman, ROT270, "Tim Arcadecollecting", "Bagman Turbo (Stern set 1) [c]", 0 ) // http://www.arcadecollecting.com/hacks/bagman
GAME(1983, pickinpi, 0, pickin, pickin,  driver_device, 0,      ROT270, "Pi", "Pickin' (Color Hack)", 0 )
