// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/8080bw.cpp"



ROM_START( sinvrdzm )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "sinvrdzm.1",  0x0000, 0x0400, CRC(f625f153) SHA1(2a19f7b4f5687e89eebf02cfdf3d1d23624879fb) )
	ROM_LOAD( "sinvrdzm.2",  0x0400, 0x0400, CRC(9d628753) SHA1(2b4468f64246da9263a384f2a940829e5a8cebde) )
	ROM_LOAD( "sinvrdzm.3",  0x0800, 0x0400, CRC(db342868) SHA1(f9ab63b3e89be9f22e453b59bf00732b7e265ee3) )
	ROM_LOAD( "sinvrdzm.4",  0x0c00, 0x0400, CRC(b635e8ac) SHA1(88a58e6ec550ec8f24443716b38f3d0aab57fc85) )
	ROM_LOAD( "sinvrdzm.5",  0x1000, 0x0400, CRC(014e761f) SHA1(1669c8f86e9d5522b009a517a75792b65902cee2) )
	ROM_LOAD( "sinvrdzm.6",  0x1400, 0x0400, CRC(599c6b78) SHA1(79e2d3a2d4e67bc13fea0d13301fffd92d5c11da) )
	ROM_LOAD( "sinvrdzm.7",  0x1800, 0x0400, CRC(0e8a75df) SHA1(e03c4a4d5aa6b7d97cc12f3bd3df84a15df4ad35) )
	ROM_LOAD( "sinvrdzm.8",  0x1c00, 0x0400, CRC(11392151) SHA1(ddeaf6678c898416e734d924f872496095ff294d) )
ROM_END

GAMEL(1978, sinvrdzm, invaders, invaders, superinv, mw8080bw_state, empty_init, ROT270, "Zenitone-Microsec Ltd", "Super Invaders (Ruffler & Deith)", MACHINE_SUPPORTS_SAVE, layout_invaders )


