// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/cv1k.cpp"

// You can choose from 4 characters instead of 2
ROM_START( deathsmlh01 )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "u4", 0x000000, 0x200000, CRC(1a7b98bf) SHA1(07798a4a846e5802756396b34df47d106895c1f1) )
	ROM_RELOAD(0x200000,0x200000)

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "h01.u2", 0x000000, 0x8400000, CRC(d7dd7bad) SHA1(579f14a7f3ec399f6694b20eaae2990fa699aa72) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "u23", 0x000000, 0x400000, CRC(aab718c8) SHA1(0e636c46d06151abd6f73232bc479dafcafe5327) )
	ROM_LOAD16_WORD_SWAP( "u24", 0x400000, 0x400000, CRC(83881d84) SHA1(6e2294b247dfcbf0ced155dc45c706f29052775d) )
ROM_END

// CA017  Deathsmiles
GAME( 2007, deathsmlh01, deathsml, cv1k,   cv1k, cv1k_state, init_deathsml,  ROT0,   "Cave (AMI license)", "Death Smiles (Hidden elements full open + character enhancement)", 0 )

