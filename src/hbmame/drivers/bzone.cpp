// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/bzone.cpp"


// Battle Zone Plus
// 1. Battle Zone logo is corrupted (attract mode)
// 2. Can only go forward and back, can't turn.

ROM_START( bzone01 )
	ROM_REGION( 0x8000, "maincpu", 0 )
	ROM_LOAD( "036414-01.e1",  0x5000, 0x0800, CRC(55e0b5d5) SHA1(88cbf5c98b41cde5524e9bef537a03db9e783f17) )
	ROM_LOAD( "036413-01.h1",  0x5800, 0x0800, CRC(5d9d9111) SHA1(42638cff53a9791a0f18d316f62a0ea8eea4e194) )
	ROM_LOAD( "036412-01.j1",  0x6000, 0x0800, CRC(3e0931d7) SHA1(1366a8e9b18a5b2d3f23d9069845403b5d65d32d) )
	ROM_LOAD( "036411-01.k1",  0x6800, 0x0800, CRC(ad281297) SHA1(54c5e06b2e69eb731a6c9b1704e4340f493e7ea5) )
	ROM_LOAD( "036410-01.lm1", 0x7000, 0x0800, CRC(0b7bfaa4) SHA1(33ae0f68b4e2eae9f3aecbee2d0b29003ce460b2) )
//	ROM_LOAD( "036409-01.n1",  0x7800, 0x0800, CRC(debaea12) SHA1(d23cf62182e76562f4d4e9023a8be6243183da00) )
	ROM_LOAD( "036408.01.bin", 0x7800, 0x0800, CRC(7513fc40) SHA1(c01c8959df2c24435a50caa46104a88a9fb7f7fd) )
	ROM_LOAD( "036422-01.bc3", 0x3000, 0x0800, CRC(7414177b) SHA1(147d97a3b475e738ce00b1a7909bbd787ad06eda) )
	ROM_LOAD( "036421-01.a3",  0x3800, 0x0800, CRC(8ea8f939) SHA1(b71e0ab0e220c3e64dc2b094c701fb1a960b64e4) )

	ROM_REGION( 0x100, "avg:prom", 0 )
	ROM_LOAD( "036408-01.k7",   0x0000, 0x0100, CRC(5903af03) SHA1(24bc0366f394ad0ec486919212e38be0f08d0239) )
ROM_END


GAMEL(2000, bzone01, bzone, bzone, bzone, bzone_state, empty_init, ROT0, "Clay Cowgill", "Battle Zone Plus", MACHINE_SUPPORTS_SAVE, layout_bzone )

