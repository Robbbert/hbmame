// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/midtunit.cpp"

/*************
 Mortal Kombat
****************/
ROM_START( mk01 )
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u3_sound_rom.u3", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u12_sound_rom.u12", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(              0x40000, 0x40000 )
	ROM_LOAD( "sl1_mortal_kombat_u13_sound_rom.u13", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(              0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk01.uj12", 0x00000, 0x80000, CRC(4192e38f) SHA1(abe2e6f643a36df48a23aaa2f9027c78c6078d2d) )
	ROM_LOAD16_BYTE( "mk01.ug12", 0x00001, 0x80000, CRC(8799979d) SHA1(4f094540e68c03fbd03cd8731797c55e455f61e6) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug14_game_rom.ug14", 0x000000, 0x80000, CRC(9e00834e) SHA1(2b97b63f52ba1dba6af6ae56c223519a52b2ab9d) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj14_game_rom.uj14", 0x000001, 0x80000, CRC(f4b0aaa7) SHA1(4cc6ee34c89e3cde325ad24b29511f70ae6a5a72) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug19_game_rom.ug19", 0x000002, 0x80000, CRC(2d8c7ba1) SHA1(f891d6eb618dbf3e77f02e0f93da216e20571905) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj19_game_rom.uj19", 0x000003, 0x80000, CRC(33b9b7a4) SHA1(e8ceca4c049e1f55d480a03ff793b595bd04d344) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug16_game_rom.ug16", 0x200000, 0x80000, CRC(52c9d1e5) SHA1(7b1880fca0a11544782b70365c7dd96381ac48e7) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj16_game_rom.uj16", 0x200001, 0x80000, CRC(c94c58cf) SHA1(974d75667eee779497325d5be8df937f15417edf) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug20_game_rom.ug20", 0x200002, 0x80000, CRC(2f7e55d3) SHA1(bda6892ee6fcb46959e4d0892bbe7d9fc6072dd3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj20_game_rom.uj20", 0x200003, 0x80000, CRC(eae96df0) SHA1(b40532312ba61e4065abfd733dd0c93eecad48e9) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug17_game_rom.ug17", 0x400000, 0x80000, CRC(e34fe253) SHA1(6b010bee795c1770297c9557ded1fe83425857f2) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj17_game_rom.uj17", 0x400001, 0x80000, CRC(a56e12f5) SHA1(5db637c4710990cd06bb0069714b19621532e431) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug22_game_rom.ug22", 0x400002, 0x80000, CRC(b537bb4e) SHA1(05a447deee2e89b49bdb3ca2161a021d7ec5f11e) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj22_game_rom.uj22", 0x400003, 0x80000, CRC(5e12523b) SHA1(468f93ef9bb6addb45c1c939d24b6511f255426a) )
ROM_END

ROM_START( mk02 )
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u3_sound_rom.u3", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u12_sound_rom.u12", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(              0x40000, 0x40000 )
	ROM_LOAD( "sl1_mortal_kombat_u13_sound_rom.u13", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(              0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk02.uj12", 0x00000, 0x80000, CRC(2f47f224) SHA1(44c10853696507e0c73eaeb1d5bda095af9ae62f) )
	ROM_LOAD16_BYTE( "mk02.ug12", 0x00001, 0x80000, CRC(39dd89b7) SHA1(e2cfa470ed07824de2d1d43b7a6e7fb4bb8dacf3) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug14_game_rom.ug14", 0x000000, 0x80000, CRC(9e00834e) SHA1(2b97b63f52ba1dba6af6ae56c223519a52b2ab9d) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj14_game_rom.uj14", 0x000001, 0x80000, CRC(f4b0aaa7) SHA1(4cc6ee34c89e3cde325ad24b29511f70ae6a5a72) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug19_game_rom.ug19", 0x000002, 0x80000, CRC(2d8c7ba1) SHA1(f891d6eb618dbf3e77f02e0f93da216e20571905) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj19_game_rom.uj19", 0x000003, 0x80000, CRC(33b9b7a4) SHA1(e8ceca4c049e1f55d480a03ff793b595bd04d344) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug16_game_rom.ug16", 0x200000, 0x80000, CRC(52c9d1e5) SHA1(7b1880fca0a11544782b70365c7dd96381ac48e7) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj16_game_rom.uj16", 0x200001, 0x80000, CRC(c94c58cf) SHA1(974d75667eee779497325d5be8df937f15417edf) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug20_game_rom.ug20", 0x200002, 0x80000, CRC(2f7e55d3) SHA1(bda6892ee6fcb46959e4d0892bbe7d9fc6072dd3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj20_game_rom.uj20", 0x200003, 0x80000, CRC(eae96df0) SHA1(b40532312ba61e4065abfd733dd0c93eecad48e9) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug17_game_rom.ug17", 0x400000, 0x80000, CRC(e34fe253) SHA1(6b010bee795c1770297c9557ded1fe83425857f2) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj17_game_rom.uj17", 0x400001, 0x80000, CRC(a56e12f5) SHA1(5db637c4710990cd06bb0069714b19621532e431) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug22_game_rom.ug22", 0x400002, 0x80000, CRC(b537bb4e) SHA1(05a447deee2e89b49bdb3ca2161a021d7ec5f11e) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj22_game_rom.uj22", 0x400003, 0x80000, CRC(5e12523b) SHA1(468f93ef9bb6addb45c1c939d24b6511f255426a) )
ROM_END

ROM_START( mk03 ) //mkk
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u3_sound_rom.u3", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u12_sound_rom.u12", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(              0x40000, 0x40000 )
	ROM_LOAD( "sl1_mortal_kombat_u13_sound_rom.u13", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(              0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk03.uj12", 0x00000, 0x80000, CRC(f04ded75) SHA1(0ba305c8b63085889e1827e5231ce08b5b1adb6c) )
	ROM_LOAD16_BYTE( "mk03.ug12", 0x00001, 0x80000, CRC(30fd0268) SHA1(2336e5d8cbaaeb6b08f7c68c32af3d02492f81f8) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug14_game_rom.ug14", 0x000000, 0x80000, CRC(9e00834e) SHA1(2b97b63f52ba1dba6af6ae56c223519a52b2ab9d) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj14_game_rom.uj14", 0x000001, 0x80000, CRC(f4b0aaa7) SHA1(4cc6ee34c89e3cde325ad24b29511f70ae6a5a72) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug19_game_rom.ug19", 0x000002, 0x80000, CRC(2d8c7ba1) SHA1(f891d6eb618dbf3e77f02e0f93da216e20571905) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj19_game_rom.uj19", 0x000003, 0x80000, CRC(33b9b7a4) SHA1(e8ceca4c049e1f55d480a03ff793b595bd04d344) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug16_game_rom.ug16", 0x200000, 0x80000, CRC(52c9d1e5) SHA1(7b1880fca0a11544782b70365c7dd96381ac48e7) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj16_game_rom.uj16", 0x200001, 0x80000, CRC(c94c58cf) SHA1(974d75667eee779497325d5be8df937f15417edf) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug20_game_rom.ug20", 0x200002, 0x80000, CRC(2f7e55d3) SHA1(bda6892ee6fcb46959e4d0892bbe7d9fc6072dd3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj20_game_rom.uj20", 0x200003, 0x80000, CRC(eae96df0) SHA1(b40532312ba61e4065abfd733dd0c93eecad48e9) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug17_game_rom.ug17", 0x400000, 0x80000, CRC(e34fe253) SHA1(6b010bee795c1770297c9557ded1fe83425857f2) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj17_game_rom.uj17", 0x400001, 0x80000, CRC(a56e12f5) SHA1(5db637c4710990cd06bb0069714b19621532e431) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug22_game_rom.ug22", 0x400002, 0x80000, CRC(b537bb4e) SHA1(05a447deee2e89b49bdb3ca2161a021d7ec5f11e) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj22_game_rom.uj22", 0x400003, 0x80000, CRC(5e12523b) SHA1(468f93ef9bb6addb45c1c939d24b6511f255426a) )
ROM_END

ROM_START( mk04 ) // mkturbod
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u3_sound_rom.u3", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x200000, "adpcm:oki", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u12_sound_rom.u12", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(              0x40000, 0x40000 )
	ROM_LOAD( "sl1_mortal_kombat_u13_sound_rom.u13", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(              0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk04.uj12", 0x00000, 0x80000, CRC(7a441f2d) SHA1(3b731bcbd73721ea0cc20157ec5181d25922523c) )
	ROM_LOAD16_BYTE( "mk04.ug12", 0x00001, 0x80000, CRC(bfab4bb6) SHA1(fc48b4b1fd4f32d9a597e292a1121916c98968d1) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug14_game_rom.ug14", 0x000000, 0x80000, CRC(9e00834e) SHA1(2b97b63f52ba1dba6af6ae56c223519a52b2ab9d) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj14_game_rom.uj14", 0x000001, 0x80000, CRC(f4b0aaa7) SHA1(4cc6ee34c89e3cde325ad24b29511f70ae6a5a72) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug19_game_rom.ug19", 0x000002, 0x80000, CRC(2d8c7ba1) SHA1(f891d6eb618dbf3e77f02e0f93da216e20571905) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj19_game_rom.uj19", 0x000003, 0x80000, CRC(33b9b7a4) SHA1(e8ceca4c049e1f55d480a03ff793b595bd04d344) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug16_game_rom.ug16", 0x200000, 0x80000, CRC(52c9d1e5) SHA1(7b1880fca0a11544782b70365c7dd96381ac48e7) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj16_game_rom.uj16", 0x200001, 0x80000, CRC(c94c58cf) SHA1(974d75667eee779497325d5be8df937f15417edf) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug20_game_rom.ug20", 0x200002, 0x80000, CRC(2f7e55d3) SHA1(bda6892ee6fcb46959e4d0892bbe7d9fc6072dd3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj20_game_rom.uj20", 0x200003, 0x80000, CRC(eae96df0) SHA1(b40532312ba61e4065abfd733dd0c93eecad48e9) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug17_game_rom.ug17", 0x400000, 0x80000, CRC(e34fe253) SHA1(6b010bee795c1770297c9557ded1fe83425857f2) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj17_game_rom.uj17", 0x400001, 0x80000, CRC(a56e12f5) SHA1(5db637c4710990cd06bb0069714b19621532e431) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug22_game_rom.ug22", 0x400002, 0x80000, CRC(b537bb4e) SHA1(05a447deee2e89b49bdb3ca2161a021d7ec5f11e) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj22_game_rom.uj22", 0x400003, 0x80000, CRC(5e12523b) SHA1(468f93ef9bb6addb45c1c939d24b6511f255426a) )
ROM_END

ROM_START( mk05 ) // mktturboen
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u3_sound_rom.u3", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u12_sound_rom.u12", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(              0x40000, 0x40000 )
	ROM_LOAD( "sl1_mortal_kombat_u13_sound_rom.u13", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(              0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk05.uj12", 0x00000, 0x80000, CRC(53069455) SHA1(f86da6036844b3e996fa9b3769e4f9979a920b28) )
	ROM_LOAD16_BYTE( "mk05.ug12", 0x00001, 0x80000, CRC(45bed5a1) SHA1(dba2c21878925afdcaf61520c18ebefd5e9617db) )
	ROM_FILL(0xF7914, 1, 0x60)
	ROM_FILL(0xF7915, 1, 0x09)

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug14_game_rom.ug14", 0x000000, 0x80000, CRC(9e00834e) SHA1(2b97b63f52ba1dba6af6ae56c223519a52b2ab9d) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj14_game_rom.uj14", 0x000001, 0x80000, CRC(f4b0aaa7) SHA1(4cc6ee34c89e3cde325ad24b29511f70ae6a5a72) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug19_game_rom.ug19", 0x000002, 0x80000, CRC(2d8c7ba1) SHA1(f891d6eb618dbf3e77f02e0f93da216e20571905) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj19_game_rom.uj19", 0x000003, 0x80000, CRC(33b9b7a4) SHA1(e8ceca4c049e1f55d480a03ff793b595bd04d344) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug16_game_rom.ug16", 0x200000, 0x80000, CRC(52c9d1e5) SHA1(7b1880fca0a11544782b70365c7dd96381ac48e7) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj16_game_rom.uj16", 0x200001, 0x80000, CRC(c94c58cf) SHA1(974d75667eee779497325d5be8df937f15417edf) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug20_game_rom.ug20", 0x200002, 0x80000, CRC(2f7e55d3) SHA1(bda6892ee6fcb46959e4d0892bbe7d9fc6072dd3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj20_game_rom.uj20", 0x200003, 0x80000, CRC(eae96df0) SHA1(b40532312ba61e4065abfd733dd0c93eecad48e9) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug17_game_rom.ug17", 0x400000, 0x80000, CRC(e34fe253) SHA1(6b010bee795c1770297c9557ded1fe83425857f2) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj17_game_rom.uj17", 0x400001, 0x80000, CRC(a56e12f5) SHA1(5db637c4710990cd06bb0069714b19621532e431) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug22_game_rom.ug22", 0x400002, 0x80000, CRC(b537bb4e) SHA1(05a447deee2e89b49bdb3ca2161a021d7ec5f11e) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj22_game_rom.uj22", 0x400003, 0x80000, CRC(5e12523b) SHA1(468f93ef9bb6addb45c1c939d24b6511f255426a) )
ROM_END

ROM_START( mk06 ) // mktturbopi
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u3_sound_rom.u3", 0x10000, 0x40000, CRC(c615844c) SHA1(5732f9053a5f73b0cc3b0166d7dc4430829d5bc7) )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "sl1_mortal_kombat_u12_sound_rom.u12", 0x00000, 0x40000, CRC(258bd7f9) SHA1(463890b23f17350fb9b8a85897b0777c45bc2d54) )
	ROM_RELOAD(              0x40000, 0x40000 )
	ROM_LOAD( "sl1_mortal_kombat_u13_sound_rom.u13", 0x80000, 0x40000, CRC(7b7ec3b6) SHA1(6eec1b90d4a4855f34a7ebfbf93f3358d5627db4) )
	ROM_RELOAD(              0xc0000, 0x40000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk06.uj12", 0x00000, 0x80000, CRC(f4a9a289) SHA1(a94b0a5796457c6f2789e9fd5a1cb80fd6dcf22b) )
	ROM_LOAD16_BYTE( "mk05.ug12", 0x00001, 0x80000, CRC(45bed5a1) SHA1(dba2c21878925afdcaf61520c18ebefd5e9617db) )
	ROM_FILL(0xF7914, 1, 0x60)
	ROM_FILL(0xF7915, 1, 0x09)

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug14_game_rom.ug14", 0x000000, 0x80000, CRC(9e00834e) SHA1(2b97b63f52ba1dba6af6ae56c223519a52b2ab9d) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj14_game_rom.uj14", 0x000001, 0x80000, CRC(f4b0aaa7) SHA1(4cc6ee34c89e3cde325ad24b29511f70ae6a5a72) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug19_game_rom.ug19", 0x000002, 0x80000, CRC(2d8c7ba1) SHA1(f891d6eb618dbf3e77f02e0f93da216e20571905) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj19_game_rom.uj19", 0x000003, 0x80000, CRC(33b9b7a4) SHA1(e8ceca4c049e1f55d480a03ff793b595bd04d344) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug16_game_rom.ug16", 0x200000, 0x80000, CRC(52c9d1e5) SHA1(7b1880fca0a11544782b70365c7dd96381ac48e7) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj16_game_rom.uj16", 0x200001, 0x80000, CRC(c94c58cf) SHA1(974d75667eee779497325d5be8df937f15417edf) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug20_game_rom.ug20", 0x200002, 0x80000, CRC(2f7e55d3) SHA1(bda6892ee6fcb46959e4d0892bbe7d9fc6072dd3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj20_game_rom.uj20", 0x200003, 0x80000, CRC(eae96df0) SHA1(b40532312ba61e4065abfd733dd0c93eecad48e9) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug17_game_rom.ug17", 0x400000, 0x80000, CRC(e34fe253) SHA1(6b010bee795c1770297c9557ded1fe83425857f2) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj17_game_rom.uj17", 0x400001, 0x80000, CRC(a56e12f5) SHA1(5db637c4710990cd06bb0069714b19621532e431) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_ug22_game_rom.ug22", 0x400002, 0x80000, CRC(b537bb4e) SHA1(05a447deee2e89b49bdb3ca2161a021d7ec5f11e) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_t-unit_uj22_game_rom.uj22", 0x400003, 0x80000, CRC(5e12523b) SHA1(468f93ef9bb6addb45c1c939d24b6511f255426a) )
ROM_END

/***************
 Mortal Kombat II
******************/
ROM_START( mk2s01 ) //mk2k
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk2s01.uj12", 0x00000, 0x80000, CRC(049aa957) SHA1(5a173938d7786a55fbf78db3df25aa9be8efea9c) )
	ROM_LOAD16_BYTE( "mk2s01.ug12", 0x00001, 0x80000, CRC(6eb2bd15) SHA1(e767ba4e5878a2e139803f2ec9cc60a4baaad556) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( mk2s02 ) // mk2fr
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk2s02.uj12", 0x00000, 0x80000, CRC(0f801e9b) SHA1(b0dc620e1be6b89ff0a1dcd64eec678e81a2ea69) )
	ROM_LOAD16_BYTE( "mk2s02.ug12", 0x00001, 0x80000, CRC(d896c9b6) SHA1(61e26c6acaf810e0788aef31000056e524b87b06) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( mk2s03 ) // mk2pb1
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk2s03.uj12", 0x00000, 0x80000, CRC(87a755b5) SHA1(d1796dfae89ca1163cb3db3638066d4bd54d32a8) )
	ROM_LOAD16_BYTE( "mk2s03.ug12", 0x00001, 0x80000, CRC(b59368c4) SHA1(987ff2ee0944955289b6b156c375b183819072bd) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( mk2s04 ) // mk2pb2
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mk2pb2.u2", 0x000000, 0x80000, CRC(65d11dd7) SHA1(bb94cca5a35f8f285d32314f8762c3c52ec48f5b) )
	ROM_RELOAD(                                             0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                                             0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                                             0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                                             0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                                             0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                                             0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk2s04.uj12", 0x00000, 0x80000, CRC(05ff15a9) SHA1(23e9c47644c756d7568b6e5122801b19cf594e9b) )
	ROM_LOAD16_BYTE( "mk2s04.ug12", 0x00001, 0x80000, CRC(b6d8ff5c) SHA1(ad974cc0dcdf35c7175f7cb8c620b58b82843523) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "mk2s04.ug16", 0x400000, 0x100000, CRC(b2af2798) SHA1(3d679fb59753b72f961fe60a65e4e03a76b64266) )
	ROM_LOAD32_BYTE( "mk2s04.uj16", 0x400001, 0x100000, CRC(d70dd149) SHA1(1add61f22a68c0bd8dba3fa0da18f5ecc7f51c6f) )
	ROM_LOAD32_BYTE( "mk2s04.ug20", 0x400002, 0x100000, CRC(d05e970a) SHA1(0f07d72c0ae9ed77c79b4a39abbc047df68a6b1d) )
	ROM_LOAD32_BYTE( "mk2s04.uj20", 0x400003, 0x100000, CRC(0f9c9a12) SHA1(6c593ab037f9e203e50a13bef1cdbba765a45845) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( mk2s05 ) // mk2r33
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk2s05.uj12", 0x00000, 0x80000, CRC(84cb0bf4) SHA1(098e0b5be75c60733f3a63893431506cb99f0981) )
	ROM_LOAD16_BYTE( "mk2s05.ug12", 0x00001, 0x80000, CRC(2021ec4f) SHA1(7f357abd7ae66eda8eae2b318426da668cfe1deb) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( mk2s06 ) // mk2la03
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk2s06.uj12", 0x00000, 0x80000, CRC(a755a400) SHA1(0c5ca8885369897f08a81f0b9e49f05919e3efcf) )
	ROM_LOAD16_BYTE( "mk2s06.ug12", 0x00001, 0x80000, CRC(d1fef398) SHA1(10aa6bf25f89327207697b968d9f57d2c6127328) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

/**************************
 Ultimate Mortal Kombat II
***************************/
ROM_START( umk2s01 ) // umk2te50
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "umk2s01.uj12", 0x00000, 0x80000, CRC(62ef20f8) SHA1(0e0392bb4b59fb4e01369f533fdcfa6ee5fb6939) )
	ROM_LOAD16_BYTE( "umk2s01.ug12", 0x00001, 0x80000, CRC(8c66e999) SHA1(ddb9f39149a7ded105f2e993a4c39c473067adf6) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( umk2s02 ) // umk2te51
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "umk2s02.uj12", 0x00000, 0x80000, CRC(fbf97b24) SHA1(902614ce3a4527d1a78e148a876cd321ea221e8f) )
	ROM_LOAD16_BYTE( "umk2s02.ug12", 0x00001, 0x80000, CRC(8d67c168) SHA1(699bf8e6924429221ace092fb66ae4c85eede8d2) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( umk2s03 ) // umk2te52
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "umk2s03.uj12", 0x00000, 0x80000, CRC(a027088c) SHA1(10aced067d8e3a5fe905c07e2ee27904e3dd8855) )
	ROM_LOAD16_BYTE( "umk2s03.ug12", 0x00001, 0x80000, CRC(474cf238) SHA1(fee96dc9390ade206be3768812040f3bb4ea1e2c) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( umk2s04 ) //mk2tek
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "umk2s04.uj12", 0x00000, 0x80000, CRC(5ca32ce4) SHA1(dce19a8c704dba7d78910be13580a7e7defd16b8) )
	ROM_LOAD16_BYTE( "umk2s04.ug12", 0x00001, 0x80000, CRC(90a3ee29) SHA1(0b5b84f1f14a6722cfcbd701537918e4624d0407) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END

ROM_START( umk2s05 ) // umk2te53
	ROM_REGION16_LE( 0xc00000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u2.u2", 0x000000, 0x80000, CRC(5f23d71d) SHA1(54c2afef243759e0f3dbe2907edbc4302f5c8bad) )
	ROM_RELOAD(                0x100000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u3.u3", 0x200000, 0x80000, CRC(d6d92bf9) SHA1(397351c6b707f2595e36360471015f9fa494e894) )
	ROM_RELOAD(                0x300000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u4.u4", 0x400000, 0x80000, CRC(eebc8e0f) SHA1(705ab63ff7672a4857d546afda6dca4973cce1ad) )
	ROM_RELOAD(                0x500000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u5.u5", 0x600000, 0x80000, CRC(2b0b7961) SHA1(1cdc64aab74d14afbd8c3531e3d0bd49271a281f) )
	ROM_RELOAD(                0x700000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u6.u6", 0x800000, 0x80000, CRC(f694b27f) SHA1(d43e38a124665f49ebb4ffc5a55e8f19a1a64686) )
	ROM_RELOAD(                0x900000, 0x80000 )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_ii_sound_rom_u7.u7", 0xa00000, 0x80000, CRC(20387e0a) SHA1(505d05173b2a1f1ee3ebc2898ccd3a95c98dd04a) )
	ROM_RELOAD(                0xb00000, 0x80000 )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "umk2s05.uj12", 0x00000, 0x80000, CRC(82c0ef47) SHA1(ae157efe65a3faf9f638e30858475f0c0c2a5a62) )
	ROM_LOAD16_BYTE( "umk2s05.ug12", 0x00001, 0x80000, CRC(bad41b9f) SHA1(1dc178a5a1fa7e84892f8c2fb1baaad01d5549b6) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug14.ug14", 0x000000, 0x100000, CRC(01e73af6) SHA1(6598cfd704cc92a7f358a0e1f1c973ab79dcc493) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj14.uj14", 0x000001, 0x100000, CRC(d4985cbb) SHA1(367865da7efae38d83de3c0868d02a705177ae63) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug19.ug19", 0x000002, 0x100000, CRC(fec137be) SHA1(f11ecb8a7993f5c4f4449564b4911f69bd6e9bf8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj19.uj19", 0x000003, 0x100000, CRC(2d763156) SHA1(06536006da49ab5fb6b75b25f801b83fad000ff5) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug16.ug16", 0x400000, 0x100000, CRC(8ba6ae18) SHA1(465fe907de4a1e502180c4e41642998dd3abc8e6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj16.uj16", 0x400001, 0x100000, CRC(39d885b4) SHA1(2251826d247c3c6df421124718401fb35a672f83) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug20.ug20", 0x400002, 0x100000, CRC(809118c1) SHA1(86153e648834c749e34573151cd4fee403a81962) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj20.uj20", 0x400003, 0x100000, CRC(b96824f0) SHA1(d42b122f9a57da330192abc7e5f97abc4065d718) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug17.ug17", 0x800000, 0x100000, CRC(937d8620) SHA1(8b9f80a460b124a747a6d1495b53f01f580e28f1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj17.uj17", 0x800001, 0x100000, CRC(218de160) SHA1(87aea173720d2a33d8183903f4fe8ba1d47e3348) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_ug22.ug22", 0x800002, 0x100000, CRC(154d53b1) SHA1(58ff0aa59101f40a9a3b5fbae1c904d0b0b31612) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_ii_game_rom_uj22.uj22", 0x800003, 0x100000, CRC(8891d785) SHA1(fd460df1ef8f4306ea42f7dc41488a80fd2c8f53) )
ROM_END


/**********
 NBA Jam
**********/
ROM_START( nbajam01 ) // nbajamtexxx
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD(  "l1_nba_jam_tournament_u3_sound_rom.u3", 0x010000, 0x20000, CRC(d4551195) SHA1(e8908fbe4339fb8c93f7e74113dfd25dda1667ea) )
	ROM_RELOAD(             0x030000, 0x20000 )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "nbajam01.u12", 0x000000, 0x80000, CRC(dd0b81b1) SHA1(1ea938138e2267b30250de9a7f643ba624923204) )
	ROM_LOAD( "nbajam01.u13", 0x080000, 0x80000, CRC(b42a0374) SHA1(6a565fc01b662d116db065172c057ee596b3a538) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l4_nba_jam_tournament_game_rom_uj12.uj12", 0x00000, 0x80000, CRC(d7c21bc4) SHA1(e05f0299b955500df6a08b1c0b24b932a9cdfa6a) )
	ROM_LOAD16_BYTE( "l4_nba_jam_tournament_game_rom_ug12.ug12", 0x00001, 0x80000, CRC(7ad49229) SHA1(e9ceedb0e620809d8a4d42087d806aa296a4cd59) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug14.ug14", 0x000000, 0x80000, CRC(04bb9f64) SHA1(9e1a8c37e14cb6fe67f4aa3caa9022f356f1ca64) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj14.uj14", 0x000001, 0x80000, CRC(b34b7af3) SHA1(0abb74d2f414bc9da0380a81beb134f3a87c1a0a) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug19.ug19", 0x000002, 0x80000, CRC(a8f22fbb) SHA1(514208a9d6d0c8c2d7847cc02d4387eac90be659) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj19.uj19", 0x000003, 0x80000, CRC(8130a8a2) SHA1(f23f124024285d07d8cf822817b62e42c38b82db) )

	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug16.ug16", 0x200000, 0x80000, CRC(c7ce74d0) SHA1(93861cd909e0f28ed112096d6f9fc57d0d31c57c) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj16.uj16", 0x200001, 0x80000, CRC(905ad88b) SHA1(24c336ccc0e2ac0ee96a34ad6fe4aa7464de0009) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug20.ug20", 0x200002, 0x80000, CRC(8a48728c) SHA1(3684099b4934b027336c319c77d9e0710b8c22dc) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj20.uj20", 0x200003, 0x80000, CRC(bf263d61) SHA1(b5b59e8df55f8030eff068c1d8b07dad8521bf5d) )

	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug17.ug17", 0x400000, 0x80000, CRC(9401be62) SHA1(597413a8a1eb66a7ad89af2f548fa3062e5e8efb) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj17.uj17", 0x400001, 0x80000, CRC(8a852b9e) SHA1(604c7f4305887e9505320630027765ea76607c58) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug22.ug22", 0x400002, 0x80000, CRC(3b05133b) SHA1(f6067abb92b8751afe7352a4f1b1a22c9528002b) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj22.uj22", 0x400003, 0x80000, CRC(39791051) SHA1(7aa02500ddacd31fca04044a22a38f36452ca300) )

	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug18.ug18", 0x600000, 0x80000, CRC(6fd08f57) SHA1(5b7031dffc88374c5bfdf3021aa01ec4e28d0631) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj18.uj18", 0x600001, 0x80000, CRC(4eb73c26) SHA1(693bf45f777da8e55b7bcd8699ea5bd711964941) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug23.ug23", 0x600002, 0x80000, CRC(854f73bc) SHA1(242cc8ce28711f6f0787524a1070eb4b0956e6ae) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj23.uj23", 0x600003, 0x80000, CRC(f8c30998) SHA1(33e2f982d74e9f3686b1f4a8172c49fb8b604cf5) )
ROM_END

ROM_START( nbajam02 ) // nbajamre
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD(  "l1_nba_jam_tournament_u3_sound_rom.u3", 0x010000, 0x20000, CRC(d4551195) SHA1(e8908fbe4339fb8c93f7e74113dfd25dda1667ea) )
	ROM_RELOAD(             0x030000, 0x20000 )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "nbajam02.u12", 0x000000, 0x80000, CRC(cd5d4532) SHA1(63ff4b5b22e02ee90cdcb745acc6bf9dbc1c3140) )
	ROM_LOAD( "nbajam02.u13", 0x080000, 0x80000, CRC(e92fb0d3) SHA1(bac12a69f74051cb97326c05eb77d88acdf81a63) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "nbajam02.uj12", 0x00000, 0x80000, CRC(9b3fc483) SHA1(4e7b6b85d9972063ebdb08ad2c860e082b6e886b) )
	ROM_LOAD16_BYTE( "nbajam02.ug12", 0x00001, 0x80000, CRC(18e75204) SHA1(e33eaa969dbc7ac19641238fc62d2af4d75bddef) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug14.ug14", 0x000000, 0x80000, CRC(04bb9f64) SHA1(9e1a8c37e14cb6fe67f4aa3caa9022f356f1ca64) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj14.uj14", 0x000001, 0x80000, CRC(b34b7af3) SHA1(0abb74d2f414bc9da0380a81beb134f3a87c1a0a) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug19.ug19", 0x000002, 0x80000, CRC(a8f22fbb) SHA1(514208a9d6d0c8c2d7847cc02d4387eac90be659) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj19.uj19", 0x000003, 0x80000, CRC(8130a8a2) SHA1(f23f124024285d07d8cf822817b62e42c38b82db) )

	ROM_LOAD32_BYTE( "nbajam02.ug16", 0x200000, 0x80000, CRC(b9a07a6f) SHA1(0b9410ae9d761928b828449f45302f0951a6a8af) )
	ROM_LOAD32_BYTE( "nbajam02.uj16", 0x200001, 0x80000, CRC(ffa7db04) SHA1(984a1011f378d7e77acf43f285f7c72f035fc7ee) )
	ROM_LOAD32_BYTE( "nbajam02.ug20", 0x200002, 0x80000, CRC(67c8646b) SHA1(a0584e79cdd5c83cd62bda09470f67082a670fe1) )
	ROM_LOAD32_BYTE( "nbajam02.uj20", 0x200003, 0x80000, CRC(71d028f8) SHA1(6c9d3cb6b937a53af8be0c7c5e209fb8740d6d9a) )

	ROM_LOAD32_BYTE( "nbajam02.ug17", 0x400000, 0x80000, CRC(34c6bdb8) SHA1(fe613fc3256eafc368ba566dd989c05293df1829) )
	ROM_LOAD32_BYTE( "nbajam02.uj17", 0x400001, 0x80000, CRC(3af5b32e) SHA1(397d2524a5749f8e8419280cc3c48ec3081834b5) )
	ROM_LOAD32_BYTE( "nbajam02.ug22", 0x400002, 0x80000, CRC(d41234d2) SHA1(9dab65c7843758572634c4805227fee1850b0349) )
	ROM_LOAD32_BYTE( "nbajam02.uj22", 0x400003, 0x80000, CRC(42196c84) SHA1(7f30204a4764b4afd38cf82f8be909c9ee5a0a0b) )

	ROM_LOAD32_BYTE( "nbajam02.ug18", 0x600000, 0x80000, CRC(fe18a6ef) SHA1(58b8bbf257b3de5d7536421dda302e2d93f51999) )
	ROM_LOAD32_BYTE( "nbajam02.uj18", 0x600001, 0x80000, CRC(9a6d36de) SHA1(1bb24ff5fdbdc1f7265da0ba43e3d96ab589548f) )
	ROM_LOAD32_BYTE( "nbajam02.ug23", 0x600002, 0x80000, CRC(8d1af1a6) SHA1(4de21240ec0ffce9346e75b9719d9ae71bfbdb8e) )
	ROM_LOAD32_BYTE( "nbajam02.uj23", 0x600003, 0x80000, CRC(b5bf66f9) SHA1(de12e47e44f9cf7c611d674fe6c9b3466fde2081) )
ROM_END

ROM_START( nbajam03 ) // nbajamse
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD(  "l1_nba_jam_tournament_u3_sound_rom.u3", 0x010000, 0x20000, CRC(d4551195) SHA1(e8908fbe4339fb8c93f7e74113dfd25dda1667ea) )
	ROM_RELOAD(             0x030000, 0x20000 )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "l1_nba_jam_tournament_u12_sound_rom.u12", 0x000000, 0x80000, CRC(4fac97bc) SHA1(bd88d8c3edab0e35ad9f9350bcbaa17cda61d87a) )
	ROM_LOAD( "l1_nba_jam_tournament_u13_sound_rom.u13", 0x080000, 0x80000, CRC(6f27b202) SHA1(c1f0db15624d1e7102ce9fd1db49ccf86e8611d6) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "nbajam03.uj12", 0x00000, 0x80000, CRC(a317f3bf) SHA1(059cec300e8ea856b3e89fb80b6d53dcb7461598) )
	ROM_LOAD16_BYTE( "nbajam03.ug12", 0x00001, 0x80000, CRC(90e66e60) SHA1(3ab49ab0c8fcd3fc9344426140c28218e0a9fbe7) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug14.ug14", 0x000000, 0x80000, CRC(04bb9f64) SHA1(9e1a8c37e14cb6fe67f4aa3caa9022f356f1ca64) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj14.uj14", 0x000001, 0x80000, CRC(b34b7af3) SHA1(0abb74d2f414bc9da0380a81beb134f3a87c1a0a) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug19.ug19", 0x000002, 0x80000, CRC(a8f22fbb) SHA1(514208a9d6d0c8c2d7847cc02d4387eac90be659) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj19.uj19", 0x000003, 0x80000, CRC(8130a8a2) SHA1(f23f124024285d07d8cf822817b62e42c38b82db) )

	ROM_LOAD32_BYTE( "nbajam03.ug16", 0x200000, 0x80000, CRC(c8da980e) SHA1(a907d579e7ce699f14787d45110b0723e8ac8577) )
	ROM_LOAD32_BYTE( "nbajam03.uj16", 0x200001, 0x80000, CRC(205c5fb7) SHA1(9e5ae65fb2d38643406149ffde081ee3845e1cd5) )
	ROM_LOAD32_BYTE( "nbajam03.ug20", 0x200002, 0x80000, CRC(9cd4f985) SHA1(97ebe1df160803a2c48cbad48e66a7fba9e7f7f0) )
	ROM_LOAD32_BYTE( "nbajam03.uj20", 0x200003, 0x80000, CRC(7a47f364) SHA1(55ef88680a5a26da3140fe49d1c102f472d41a89) )

	ROM_LOAD32_BYTE( "nbajam03.ug17", 0x400000, 0x80000, CRC(1b7ddbe9) SHA1(198309e4eabd33f23b28b99a589cf54f8f5690e8) )
	ROM_LOAD32_BYTE( "nbajam03.uj17", 0x400001, 0x80000, CRC(cb08465f) SHA1(856bd3475bed9652252ba14095e4bb56df7112ac) )
	ROM_LOAD32_BYTE( "nbajam03.ug22", 0x400002, 0x80000, CRC(5427ca82) SHA1(bf206c8fd47e4fa93abf344fcb4f56ab7cdc9a14) )
	ROM_LOAD32_BYTE( "nbajam03.uj22", 0x400003, 0x80000, CRC(569b1ae5) SHA1(c8f73b24b24bb50e9972a7ef4b94fdc3f3d4d70d) )

	ROM_LOAD32_BYTE( "nbajam03.ug18", 0x600000, 0x80000, CRC(741f313e) SHA1(f067c7b92dd6d4db96da7e68062d8103f4dda30f) )
	ROM_LOAD32_BYTE( "nbajam03.uj18", 0x600001, 0x80000, CRC(5c1d6d77) SHA1(e986269cb750c1e0e2afbee77332317858885cb4) )
	ROM_LOAD32_BYTE( "nbajam03.ug23", 0x600002, 0x80000, CRC(168d93ce) SHA1(2f7ceb88e6d873edfc5bb51e9bdd2e9e794328fa) )
	ROM_LOAD32_BYTE( "nbajam03.uj23", 0x600003, 0x80000, CRC(424fbd82) SHA1(fae7e23dd7807de8c2e467f21afda1a427260883) )
ROM_END

ROM_START( nbajam04 )
	ROM_REGION( 0x50000, "adpcm:cpu", 0 )
	ROM_LOAD(  "l1_nba_jam_tournament_u3_sound_rom.u3", 0x010000, 0x20000, CRC(d4551195) SHA1(e8908fbe4339fb8c93f7e74113dfd25dda1667ea) )
	ROM_RELOAD(             0x030000, 0x20000 )

	ROM_REGION( 0x100000, "adpcm:oki", 0 )
	ROM_LOAD( "nbajam02.u12", 0x000000, 0x80000, CRC(cd5d4532) SHA1(63ff4b5b22e02ee90cdcb745acc6bf9dbc1c3140) )
	ROM_LOAD( "nbajam04.u13", 0x080000, 0x80000, CRC(c4cbede2) SHA1(8b1a26ee852281de1564af35fc3605a89eb0b7dd) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "nbajam02.uj12", 0x00000, 0x80000, CRC(9b3fc483) SHA1(4e7b6b85d9972063ebdb08ad2c860e082b6e886b) )
	ROM_LOAD16_BYTE( "nbajam02.ug12", 0x00001, 0x80000, CRC(18e75204) SHA1(e33eaa969dbc7ac19641238fc62d2af4d75bddef) )

	ROM_REGION( 0xc00000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug14.ug14", 0x000000, 0x80000, CRC(04bb9f64) SHA1(9e1a8c37e14cb6fe67f4aa3caa9022f356f1ca64) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj14.uj14", 0x000001, 0x80000, CRC(b34b7af3) SHA1(0abb74d2f414bc9da0380a81beb134f3a87c1a0a) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_ug19.ug19", 0x000002, 0x80000, CRC(a8f22fbb) SHA1(514208a9d6d0c8c2d7847cc02d4387eac90be659) )
	ROM_LOAD32_BYTE( "l1_nba_jam_tournament_game_rom_uj19.uj19", 0x000003, 0x80000, CRC(8130a8a2) SHA1(f23f124024285d07d8cf822817b62e42c38b82db) )

	ROM_LOAD32_BYTE( "nbajam02.ug16", 0x200000, 0x80000, CRC(b9a07a6f) SHA1(0b9410ae9d761928b828449f45302f0951a6a8af) )
	ROM_LOAD32_BYTE( "nbajam02.uj16", 0x200001, 0x80000, CRC(ffa7db04) SHA1(984a1011f378d7e77acf43f285f7c72f035fc7ee) )
	ROM_LOAD32_BYTE( "nbajam02.ug20", 0x200002, 0x80000, CRC(67c8646b) SHA1(a0584e79cdd5c83cd62bda09470f67082a670fe1) )
	ROM_LOAD32_BYTE( "nbajam02.uj20", 0x200003, 0x80000, CRC(71d028f8) SHA1(6c9d3cb6b937a53af8be0c7c5e209fb8740d6d9a) )

	ROM_LOAD32_BYTE( "nbajam02.ug17", 0x400000, 0x80000, CRC(34c6bdb8) SHA1(fe613fc3256eafc368ba566dd989c05293df1829) )
	ROM_LOAD32_BYTE( "nbajam02.uj17", 0x400001, 0x80000, CRC(3af5b32e) SHA1(397d2524a5749f8e8419280cc3c48ec3081834b5) )
	ROM_LOAD32_BYTE( "nbajam02.ug22", 0x400002, 0x80000, CRC(d41234d2) SHA1(9dab65c7843758572634c4805227fee1850b0349) )
	ROM_LOAD32_BYTE( "nbajam02.uj22", 0x400003, 0x80000, CRC(42196c84) SHA1(7f30204a4764b4afd38cf82f8be909c9ee5a0a0b) )

	ROM_LOAD32_BYTE( "nbajam02.ug18", 0x600000, 0x80000, CRC(fe18a6ef) SHA1(58b8bbf257b3de5d7536421dda302e2d93f51999) )
	ROM_LOAD32_BYTE( "nbajam02.uj18", 0x600001, 0x80000, CRC(9a6d36de) SHA1(1bb24ff5fdbdc1f7265da0ba43e3d96ab589548f) )
	ROM_LOAD32_BYTE( "nbajam02.ug23", 0x600002, 0x80000, CRC(8d1af1a6) SHA1(4de21240ec0ffce9346e75b9719d9ae71bfbdb8e) )
	ROM_LOAD32_BYTE( "nbajam02.uj23", 0x600003, 0x80000, CRC(b5bf66f9) SHA1(de12e47e44f9cf7c611d674fe6c9b3466fde2081) )
ROM_END


/*    YEAR  NAME         PARENT    MACHINE      INPUT     CLASS           INIT          MONITOR COMPANY                 FULLNAME FLAGS */
// Mortal Kombat
GAME( 2007, mk01,        mk,       tunit_adpcm, mk,       midtunit_state, init_mktunit,  ROT0, "Arkatrad", "Mortal Kombat (French Rev.1)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, mk02,        mk,       tunit_adpcm, mk,       midtunit_state, init_mktunit,  ROT0, "S.Arkames", "Mortal Kombat (French Rev.2)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mk03,        mk,       tunit_adpcm, mk,       midtunit_state, init_mktunit,  ROT0, "hack", "Mortal Kombat (5.0K, 2018-02-20)", MACHINE_SUPPORTS_SAVE )
GAME( 1992, mk04,        mk,       tunit_adpcm, mk,       midtunit_state, init_mktunit,  ROT0, "hack", "Mortal Kombat (Turbo Ninja T-Unit 03/19/93, hack/set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, mk05,        mk,       tunit_adpcm, mk,       midtunit_state, init_mktunit,  ROT0, "Zenomorp", "Mortal Kombat (Turbo Endurance)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, mk06,        mk,       tunit_adpcm, mk,       midtunit_state, init_mktunit,  ROT0, "Zenomorp", "Mortal Kombat (Projectile Insanity)", MACHINE_SUPPORTS_SAVE )
// Mortal Kombat II
GAME( 2020, mk2s01,      mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "TEAM UMK 3", "Mortal Kombat II (3.1K, 2020-06-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, mk2s02,      mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "Arkatrad", "Mortal Kombat II (French, 2007-02-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mk2s03,      mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "mortalkombatplus.com", "Mortal Kombat II (2.8.0 plus beta 1, 2017-09-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mk2s04,      mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "mortalkombatplus.com", "Mortal Kombat II (2.8.5 plus beta 2, 2018-12-23)", MACHINE_SUPPORTS_SAVE )
GAME( 1993, mk2s05,      mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "Midway", "Mortal Kombat II (3.3 prototype)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mk2s06,      mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "hack", "Mortal Kombat II (SP4K, 2021-08-05)", MACHINE_SUPPORTS_SAVE )
// Ultimate Mortal Kombat II
GAME( 2014, umk2s01,     mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "SK", "Ultimate Mortal Kombat II (Tournament Edition 5.0.050)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, umk2s02,     mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "SK", "Ultimate Mortal Kombat II (Tournament Edition 5.0.051)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, umk2s03,     mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "SK", "Ultimate Mortal Kombat II (Tournament Edition 5.0.052)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, umk2s04,     mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "hack", "Ultimate Mortal Kombat II (Tournament Edition, 5.0.052K, 2020-07-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, umk2s05,     mk2,      tunit_dcs,   mk2,      midtunit_state, init_mk2,      ROT0, "SK", "Ultimate Mortal Kombat II (Tournament Edition 5.0.053)", MACHINE_SUPPORTS_SAVE )
// NBA Jam
GAME( 1994, nbajam01,    nbajamte, tunit_adpcm, nbajamte, midtunit_state, init_nbajamte, ROT0, "Midway", "NBA Jam TE (rev XXX)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, nbajam02,    nbajamte, tunit_adpcm, nbajamte, midtunit_state, init_nbajamte, ROT0, "Asure", "NBA Jam Rewind (rev 1.1)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, nbajam03,    nbajamte, tunit_adpcm, nbajamte, midtunit_state, init_nbajamte, ROT0, "Asure", "NBA Jam Tournament Edition SE (rev 5.0)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, nbajam04,    nbajamte, tunit_adpcm, nbajamte, midtunit_state, init_nbajamte, ROT0, "Asure", "NBA Jam Rewind (rev 1.2)", MACHINE_SUPPORTS_SAVE )

