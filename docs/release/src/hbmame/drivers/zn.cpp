// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/zn.cpp"

// 12 characters to choose from instead of 10
ROM_START( doapph01 )
	TPS_BIOS

	ROM_REGION32_LE( 0x02800000, "bankedroms", 0 )
	ROM_LOAD16_BYTE( "doapp119.h01", 0x0000001, 0x100000, CRC(f3691595) SHA1(470ea8989229b6400cb7adac30939a2f635dccd9) )
	ROM_LOAD16_BYTE( "doapp120.h01", 0x0000000, 0x100000, CRC(3a8e9786) SHA1(8632bcba3ecd42c112885821792c78f7b0f51cd9) )
	ROM_LOAD( "doapp-0.216",         0x0400000, 0x400000, CRC(acc6c539) SHA1(a744567a3d75634098b1749103307981be9acbdd) )
	ROM_LOAD( "doapp-1.217",         0x0800000, 0x400000, CRC(14b961c4) SHA1(3fae1fcb4665ba8bad391881b26c2d087718d42f) )
	ROM_LOAD( "doapp-2.218",         0x0c00000, 0x400000, CRC(134f698f) SHA1(6422972cf5d30a0f09f0c20f042691d5969207b4) )
	ROM_LOAD( "doapp-3.219",         0x1000000, 0x400000, CRC(1c6540f3) SHA1(8631fde93a1da6325d7b31c7edf12c964f0ac4fc) )
	ROM_LOAD( "doapp-4.220",         0x1400000, 0x400000, CRC(f83bacf7) SHA1(5bd66da993f0db966581dde80dd7e5b377754412) )
	ROM_LOAD( "doapp-5.221",         0x1800000, 0x400000, CRC(e11e8b71) SHA1(b1d1b9532b5f074ce216a603436d5674d136865d) )

	ROM_REGION( 0x8, "cat702_2", 0 )
	ROM_LOAD( "mg05", 0x000000, 0x000008, CRC(5748a4ca) SHA1(c88d73f6a646a9ddefdfd84cba70d591759c069f) )
ROM_END

HACK( 200?, doapph01, coh1002m, coh1002m, zn, zn_state, 0, ROT0, "yumeji", "Dead Or Alive ++ (Enable hidden characters)(Japan)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_IMPERFECT_SOUND )

