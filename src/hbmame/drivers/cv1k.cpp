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

// Cavedwellers hack
ROM_START( sdojak )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdojak.u4", 0x000000, 0x400000, CRC(a878ff4c) SHA1(cf5f2f987a775099992009c376466c44170e0e3f) )

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "sdojak.u2", 0x000000, 0x8400000, CRC(54353425) SHA1(5c749cd2e93a1b51bfdf1a0d4fd436b4d17acf9f) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdojak.u23", 0x000000, 0x400000, CRC(32b91544) SHA1(49e914bfdf33a328fbfa7efb42c5b89915e37643) )
	ROM_LOAD16_WORD_SWAP( "sdojak.u24", 0x400000, 0x400000, CRC(7b9e749f) SHA1(374085a60098cc62501abae4d148bfd88c88149f) )
ROM_END

GAME( 2021, sdojak, 0, cv1k_d, cv1k, cv1k_state, init_ddpdfk, ROT270, "CaveDwellers", "DoDonPachi SaiDaiOuJou & Knuckles (2021/12/01)", 0 )

