// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/taito_f2.cpp"

class taitof2_hbmame : public taitof2_state
{
public:
	using taitof2_state::taitof2_state;

	void f2demo(machine_config &config);
};

void taitof2_hbmame::f2demo(machine_config &config)
{
	/* basic machine hardware */
	M68000(config, m_maincpu, 24000000/2); /* 12 MHz */
	m_maincpu->set_addrmap(AS_PROGRAM, &taitof2_hbmame::liquidk_map);
	m_maincpu->set_vblank_int("screen", FUNC(taitof2_hbmame::interrupt));

	Z80(config, m_audiocpu, 24000000/6);   /* 4 MHz */
	m_audiocpu->set_addrmap(AS_PROGRAM, &taitof2_hbmame::sound_map);

	WATCHDOG_TIMER(config, "watchdog");

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_refresh_hz(60);
	m_screen->set_vblank_time(ATTOSECONDS_IN_USEC(0));  /* frames per second, vblank duration */
	m_screen->set_size(120*8, 32*8);
	m_screen->set_visarea(40*8, 106*8-1, 2*8, 32*8-1);
	m_screen->set_screen_update(FUNC(taitof2_hbmame::screen_update_pri));
	m_screen->screen_vblank().set(FUNC(taitof2_hbmame::screen_vblank_partial_buffer_delayed));
	m_screen->set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_taitof2);
	PALETTE(config, m_palette).set_format(palette_device::RGBx_444, 4096);
	MCFG_VIDEO_START_OVERRIDE(taitof2_hbmame, megab)

	/* sound hardware */
	SPEAKER(config, "lspeaker").front_left();
	SPEAKER(config, "rspeaker").front_right();

	ym2610_device &ymsnd(YM2610(config, "ymsnd", 24000000/3)); /* Was 16000000/2, but only a 24Mhz OSC */
	ymsnd.irq_handler().set_inputline(m_audiocpu, 0);
	ymsnd.add_route(0, "lspeaker", 0.25);
	ymsnd.add_route(0, "rspeaker", 0.25);
	ymsnd.add_route(1, "lspeaker", 1.0);
	ymsnd.add_route(2, "rspeaker", 1.0);

	tc0140syt_device &tc0140syt(TC0140SYT(config, "tc0140syt", 0));
	tc0140syt.set_master_tag(m_maincpu);
	tc0140syt.set_slave_tag(m_audiocpu);

	TC0220IOC(config, m_tc0220ioc, 0);
	m_tc0220ioc->read_0_callback().set_ioport("DSWA");
	m_tc0220ioc->read_1_callback().set_ioport("DSWB");
	m_tc0220ioc->read_2_callback().set_ioport("IN0");
	m_tc0220ioc->read_3_callback().set_ioport("IN1");
	m_tc0220ioc->read_7_callback().set_ioport("IN2");

	TC0100SCN(config, m_tc0100scn[0], 0);
	m_tc0100scn[0]->set_offsets(3, 0);
	m_tc0100scn[0]->set_palette(m_palette);

	TC0360PRI(config, m_tc0360pri, 0);
}

// Game has no sound
ROM_START( f2demo )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "lq09.bin",  0x00000, 0x20000, CRC(809a968b) SHA1(06e8be120869371d33c61fa14c48fe4e5fd659ec) )
	ROM_LOAD16_BYTE( "lq11.bin",  0x00001, 0x20000, CRC(7ba3a5cb) SHA1(08f5392296cceadcae0cd34e9dfe76c835583231) )

	ROM_REGION( 0x080000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "f2d_scr.bin", 0x000000, 0x80000, CRC(b178fb05) SHA1(3d33dd822622579c75b55971926f6eb04d33f5a6) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_WORD_SWAP( "f2d_obj0.bin", 0x00000, 0x80000, CRC(1bb8aa37) SHA1(9051270485a7a995779c80b4e692b8395503d6c3) )
	ROM_LOAD16_WORD_SWAP( "f2d_obj1.bin", 0x80000, 0x80000, CRC(75660aac) SHA1(6a521e1d2a632c26e53b83d2cc4b0edecfc1e68c) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c49-08.ic32", 0x00000, 0x10000, CRC(413c310c) SHA1(cecb1c0c9fe3c8b744f95ce29009650a289107ab) )

	ROM_REGION( 0x80000, "ymsnd", 0 )
	ROM_LOAD( "lk_snd.bin",  0x00000, 0x80000, CRC(474d45a4) SHA1(20cb818d753a185973098007e645f1aa75c5528d) )
ROM_END

GAME( 1990, f2demo, 0, f2demo, liquidk, taitof2_hbmame, empty_init, ROT180, "Charles Doty", "Demo - Taito F2", MACHINE_SUPPORTS_SAVE )


// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /********
 Cameltry
***********/

ROM_START( cameltryjs01 )
	ROM_REGION( 0x40000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c38-09hc01.bin", 0x00000, 0x20000, CRC(e2f14391) SHA1(b0a8b30c5e6385f32ebe60d0e6db0e2550713f01) )
	ROM_LOAD16_BYTE( "c38-10hc01.bin", 0x00001, 0x20000, CRC(822b5c5d) SHA1(e1cf535e11808874cb6ff6f90ae0c86296df9958) )

	ROM_REGION( 0x100000, "tc0100scn_1", ROMREGION_ERASEFF )

	ROM_REGION( 0x080000, "sprites", 0 )
	ROM_LOAD( "c38-01hc01.bin", 0x00000, 0x80000, CRC(3ab7fee5) SHA1(f91f9682a1d952a7e4bbcd88eb3c3d6d38dfb852) )

	ROM_REGION( 0x080000, "tc0280grd", 0 )
	ROM_LOAD( "c38-02hc01.bin", 0x00000, 0x20000, CRC(5874058f) SHA1(901a5245024eb8abc00bc53945ffe0813850aff8) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c38-08.bin", 0x00000, 0x10000, CRC(7ff78873) SHA1(6574f1c707b8911fa957dd057e1cddc7a1cea99b) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "c38-03.bin", 0x000000, 0x020000, CRC(59fa59a7) SHA1(161f11b96a47c8431c33e300f6a509bf804309af) )
ROM_END

 /***************
 Dead Connection
******************/

ROM_START( deadconxjs01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "d28-06hc01.3",  0x00000, 0x40000, CRC(b652bd0f) SHA1(653d70af3ef284ade260a81cdb84928731adee78) )
	ROM_LOAD16_BYTE( "d28-07hc01.5",  0x00001, 0x40000, CRC(6763584c) SHA1(cd2cefaaf8d5e24ce5ed032a65e9b2880ef55821) )
	ROM_LOAD16_BYTE( "d28-09hc01.2",  0x80000, 0x40000, CRC(39e1c0fc) SHA1(f03c25c13224b4cf63fd8c6fb6b6b45b4f78e333) )
	ROM_LOAD16_BYTE( "d28-08hc01.4",  0x80001, 0x40000, CRC(2254bdc3) SHA1(462af938c473f445278e56578120400fed75d401) )

	ROM_REGION( 0x100000, "tc0480scp", 0 )
	ROM_LOAD32_WORD( "d28-04hc01.16", 0x00000, 0x80000, CRC(3312d91a) SHA1(068b57dc9fe545479dfa0be531df44cc7f31e03c) )
	ROM_LOAD32_WORD( "d28-05hc01.17", 0x00002, 0x80000, CRC(7e4a1032) SHA1(38ad9fe1fe1adc767c8a25e9295dbe21d3ca9bad) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD( "d28-01hc01.8", 0x000000, 0x100000, CRC(cff6dbca) SHA1(302498e5df5967ecb3772270c04d96f9318b2bdd) )
	ROM_LOAD( "d28-02.9", 0x100000, 0x100000, CRC(d301771c) SHA1(2756bb834b50a657bbcf4e9bec02f4af595ac7e9) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "d28-10.6", 0x00000, 0x10000, CRC(40805d74) SHA1(172114be692c766622d1235e5c4aa83ad438d9d9) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "d28-03.10", 0x000000, 0x100000, CRC(a1804b52) SHA1(f252c473a71a4cf80d9f984fec08e5ae7524a620) )
ROM_END

 /********
 Dino Rex
***********/

ROM_START( dinorexs01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "d39-14_ph01.9",    0x000000, 0x080000, CRC(c584035b) SHA1(383a7e0efd8b391a140bd7b2b78c9cfcbf88a119) )
	ROM_LOAD16_BYTE( "d39-16_ph01.8",    0x000001, 0x080000, CRC(a16dd401) SHA1(5e8c540145eceda808857b77334db3869a2c979b) )
	ROM_LOAD16_WORD_SWAP( "d39-04.6",   0x100000, 0x100000, CRC(3800506d) SHA1(a75067e94071617cd5dafdd0ae0ec096dded520a) )  /* data rom */
	ROM_LOAD16_WORD_SWAP( "d39-05.7",   0x200000, 0x100000, CRC(e2ec3b5d) SHA1(143b72d0f2f5c40dbaeed1eee0672c3b95c2bda6) )  /* data rom */

	ROM_REGION( 0x100000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "d39-06.2",   0x000000, 0x100000, CRC(52f62835) SHA1(251c4f17bc98a5e81c224864fb81352cf1234377) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD( "d39-01.29",  0x000000, 0x200000, CRC(d10e9c7d) SHA1(42c13b271a91fac37be4ea92eb358ad6a6c540cf) )
	ROM_LOAD( "d39-02.28",  0x200000, 0x200000, CRC(6c304403) SHA1(97cd58bd7d00550b7ed5f77b066216c05206b513) )
	ROM_LOAD( "d39-03.27",  0x400000, 0x200000, CRC(fc9cdab4) SHA1(ce2dfac922d6ac0f008b7dfb92d76c1671ccabbd) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "d39-12.5",   0x00000, 0x10000, CRC(8292c7c1) SHA1(2ff20726f6cc6d98d860d96b8eb3c10f46b87d58) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "d39-07.10",  0x000000, 0x100000, CRC(28262816) SHA1(6df9a31a2edf1dfc23070b41b1da2c0a9e91d1b0) )

	ROM_REGION( 0x080000, "ymsnd.deltat", 0 )
	ROM_LOAD( "d39-08.4",   0x000000, 0x080000, CRC(377b8b7b) SHA1(4905ac3b9b52c70afe5f451f860fb9acd81a1dbb) )
ROM_END

 /************
 Don Doko Don
****************/

ROM_START( dondokodjs01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "b95-12hc01.bin",   0x00000, 0x20000, CRC(717c4ee8) SHA1(f789773694ad2d0ab49ecb9ab6f9834cced5fda9) )
	ROM_LOAD16_BYTE( "b95-11-1hc01.bin", 0x00001, 0x20000, CRC(2f2f56ef) SHA1(e487fcb792192cd8f8a1518db6e34906c6ee209b) )
	ROM_LOAD16_BYTE( "b95-10hc01.bin",   0x40000, 0x20000, CRC(add6563a) SHA1(3e93cba5198dd9c69d3bb9a42c528e2f11dee7da) )
	ROM_LOAD16_BYTE( "b95-09hc01.bin",   0x40001, 0x20000, CRC(b60afcdd) SHA1(4b2bdbb51a6c0dbafbedbe60c97b75bb10fae0ee) )

	ROM_REGION( 0x080000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "b95-02hc01.bin", 0x00000, 0x80000, CRC(76a18cbf) SHA1(e8766586c6f65b24fab4dd773ebf96a6823bb4e6) )

	ROM_REGION( 0x080000, "sprites", 0 )
	ROM_LOAD( "b95-01.bin", 0x00000, 0x80000, CRC(51c176ce) SHA1(2866e8bd57b301a0d4690f194be95784c53f9fcb) )

	ROM_REGION( 0x080000, "tc0280grd", 0 )
	ROM_LOAD( "b95-03hc01.bin", 0x00000, 0x80000, CRC(d3f76493) SHA1(a17f39b637089bf6878c9d93cca7085f0a5ede64) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "b95-08.bin",  0x00000, 0x10000, CRC(b5aa49e1) SHA1(83b0a3434e0d0b9aa581c1acdd0c70308362b923) )

	ROM_REGION( 0x80000, "ymsnd", 0 )
	ROM_LOAD( "b95-04.bin",  0x00000, 0x80000, CRC(ac4c1716) SHA1(06a9def7fa3bd739438f4a1d7b55f70eb904bf54) )
ROM_END

 /************
 Gun Frontier
***************/

ROM_START( gunfrontjs01 )
	ROM_REGION( 0xc0000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c71-09hc01.ic42",  0x00000, 0x20000, CRC(f72f2d39) SHA1(260f94e22b094f7f3b434db1a2cd20ae3b28068c) )
	ROM_LOAD16_BYTE( "c71-08hc01.ic41",  0x00001, 0x20000, CRC(794aa220) SHA1(89b6a6fea6a4ba481802a34b0ba3006020b24445) )
	ROM_LOAD16_BYTE( "c71-10hc01.ic40",  0x40000, 0x20000, CRC(1995e8e3) SHA1(057b66639aa5e3951a9965166e0ba77d0f3ff580) )
	ROM_LOAD16_BYTE( "c71-11hc01.ic39",  0x40001, 0x20000, CRC(9f909465) SHA1(af8dc7a7899fb60a8ab43dcde933b0e1d7493d40) )
	ROM_LOAD16_BYTE( "c71-16.ic38",  0x80000, 0x20000, CRC(1bbcc2d4) SHA1(fe664f8d2b6d902f034cf51f42378cc68c970b53) )
	ROM_LOAD16_BYTE( "c71-15.ic37",  0x80001, 0x20000, CRC(df3e00bb) SHA1(9fe2ece7289945692099eba92f02e5a97a4d148c) )

	ROM_REGION( 0x100000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "c71-02hc01.ic59", 0x000000, 0x100000, CRC(806d3abc) SHA1(6db72a768755db02860f8eb73df617a50287c73f) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "c71-03hc01.ic19", 0x000000, 0x100000, CRC(1312289b) SHA1(f4bf010aea994757df4d704f0df003300250e92a) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c71-12.ic49", 0x00000, 0x10000, CRC(0038c7f8) SHA1(405def36e67949219b6f9394333278ec60ad5783) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "c71-01.ic29", 0x000000, 0x100000, CRC(0e73105a) SHA1(c5c9743f68a43273e16f5e5179557f2392505a1e) )
ROM_END

 /**********
 Mega Blast
*************/

ROM_START( megablstjs01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c11-07hc01.55",  0x00000, 0x20000, CRC(2331740a) SHA1(e3d8d9caa78be9490e8c2d8f0ebd07a1dadc1ffb) ) // c11-07.17
	ROM_LOAD16_BYTE( "c11-08hc01.39",  0x00001, 0x20000, CRC(0ecf7c5f) SHA1(3fe6fc48ee9489896035b6a66bf1f21906a15f85) ) // c11-08.19
	ROM_LOAD16_BYTE( "c11-06hc01.54",  0x40000, 0x20000, CRC(bdc129ba) SHA1(fe5f333ca558982d3e7e5295952bb23dd2ffe91f) ) // c11-06.16
	ROM_LOAD16_BYTE( "c11-09hc01.38",  0x40001, 0x20000, CRC(d3373aea) SHA1(f3fef101fac7eabc954fe0881529682c061bb81b) ) // c11-09.18

	ROM_REGION( 0x2000, "cchip:cchip_eprom", 0 )
	ROM_LOAD( "cchip_c11", 0x0000, 0x2000, NO_DUMP )

	ROM_REGION( 0x080000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "c11-05.58", 0x00000, 0x80000, CRC(733e6d8e) SHA1(47f3360f7c41b7e4a42e8198fc1bcce4e819181f) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "c11-03hc01.32", 0x00000, 0x80000, CRC(a58b1256) SHA1(797ea1f0bbbf15fb83633a1665bf953ee8088a64) )
	ROM_LOAD16_BYTE( "c11-04hc01.31", 0x00001, 0x80000, CRC(b5c4387f) SHA1(2540307a01895f25472f03107e53b77b5e7cce55) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c11-12.3", 0x00000, 0x10000, CRC(b11094f1) SHA1(a01e9f7d1f616bb379eaa85ad81b94173b067782) )

	ROM_REGION( 0x80000, "ymsnd", 0 )
	ROM_LOAD( "c11-01.29", 0x00000, 0x80000, CRC(fd1ea532) SHA1(481698b747a421a17bfb8cef96065712d4f3997f) )

	ROM_REGION( 0x80000, "ymsnd.deltat", 0 )
	ROM_LOAD( "c11-02.30", 0x00000, 0x80000, CRC(451cc187) SHA1(a682f70bbe6cba2fe2c0a6791e8d33db34eb2cee) )
ROM_END

 /******************
 Mizubaku Daibouken
*********************/

ROM_START( mizubakus01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c49-09hc01.ic47",  0x00000, 0x20000, CRC(1c37f50e) SHA1(c393f073f83b39198bb94fc35d1291f802f0696d) )
	ROM_LOAD16_BYTE( "c49-11hc01.ic48",  0x00001, 0x20000, CRC(9fcf7877) SHA1(922e7af3fb581c23adaccf11dc8e9b0bf42339e8) )
	ROM_LOAD16_BYTE( "c49-10hc01.ic45",  0x40000, 0x20000, CRC(7e536034) SHA1(79d2ce3524a08f670c970d7ac341e7242e9fab97) )
	ROM_LOAD16_BYTE( "c49-13hc01.ic46",  0x40001, 0x20000, CRC(9f63a664) SHA1(476aefc3f0724dae96c406c69db8e83106db94b0) )

	ROM_REGION( 0x080000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "c49-03.ic76",  0x00000, 0x80000, CRC(c3364f9b) SHA1(3512a8c352df8b8f19590c859afb8fdec758eb91) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "c49-01.ic54", 0x00000, 0x80000, CRC(67cc3163) SHA1(f898d52c136f07497ec6be214f016cbadc700055) )
	ROM_LOAD( "c49-02hc01.ic53", 0x80000, 0x80000, CRC(ed8d37b2) SHA1(bc0c69ab93f8d334f4f2b8d7ff31963910b5f394) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c49-08.ic32", 0x00000, 0x10000, CRC(413c310c) SHA1(cecb1c0c9fe3c8b744f95ce29009650a289107ab) )

	ROM_REGION( 0x80000, "ymsnd", 0 )
	ROM_LOAD( "c49-04.ic33",  0x00000, 0x80000, CRC(474d45a4) SHA1(20cb818d753a185973098007e645f1aa75c5528d) )
ROM_END

 /******
 Runark
*********/

ROM_START( runarks01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c74-10hc01.ic59",  0x00000, 0x40000, CRC(fcad0393) SHA1(0ef0e8fa6200d69790d2c5e899529c3bba84acba) )
	ROM_LOAD16_BYTE( "c74-08hc01.ic61",  0x00001, 0x40000, CRC(d2ab3dd8) SHA1(d4a42629436721e3fd3b9d921cfebbb2841bff09) )
	ROM_LOAD16_BYTE( "c74-11hc01.ic58",  0x80000, 0x40000, CRC(1f22c042) SHA1(5a17a625692c1230e7dc94b273bd71a2874ac0d8) )
	ROM_LOAD16_BYTE( "c74-09hc01.ic14",  0x80001, 0x40000, CRC(a52893c6) SHA1(41587db25eb3850434a4424e5de9e967a1e31d12) )

	ROM_REGION( 0x100000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "c74-01hc01.ic34",   0x000000, 0x100000, CRC(a3b16d89) SHA1(be1a45a588a022ee6224a082a225371dbf3bf7ed) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD( "c74-03hc01.ic12",   0x000000, 0x100000, CRC(d01d6069) SHA1(ea821841085977745117adb2e7c2d98a744d3873) )
	ROM_LOAD( "c74-02hc01.ic11",   0x100000, 0x100000, CRC(5624d1a1) SHA1(fd7c31c343f6f3f3283e9b7c27ca4d7835be6fbc) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c74-12.ic62", 0x00000, 0x10000, CRC(bb6ed668) SHA1(e8c3a15ccbc788ac57d42bd2cabcdb2db6305489) )

	ROM_REGION( 0x100000, "ymsnd", 0 )
	ROM_LOAD( "c74-04.ic28",   0x000000, 0x100000, CRC(2d97edf2) SHA1(d3a995303facdad4f8e1fdda04eaaec4440ff371) )

	ROM_REGION( 0x080000, "ymsnd.deltat", 0 )
	ROM_LOAD( "c74-05.ic29",   0x000000, 0x080000, CRC(e29c0828) SHA1(f541d724f118130bb7a8f9e790582c68779cc6b6) )
ROM_END

 /***********
 Thunder Fox
**************/

ROM_START( thundfoxjs01 )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c28-13hc01-1.51",  0x00000, 0x20000, CRC(a544a7db) SHA1(2d056237119a4e21f172c67c24be18917f0344d4) )
	ROM_LOAD16_BYTE( "c28-12hc01-1.40",  0x00001, 0x20000, CRC(84c90906) SHA1(a27fb206e22c59ca118a79651dd82ac2fcda7f1d) )
	ROM_LOAD16_BYTE( "c28-08hc01.50",    0x40000, 0x20000, CRC(fa06e4eb) SHA1(0d0324a4bed4670ea79b68d246e91240f30b7be0) )
	ROM_LOAD16_BYTE( "c28-07hc01.39",    0x40001, 0x20000, CRC(c0912703) SHA1(7aefc065601d599aa6430c286092aad96414de02) )

	ROM_REGION( 0x080000, "tc0100scn_1", 0 )
	ROM_LOAD16_WORD_SWAP( "c28-02.61", 0x000000, 0x80000, CRC(6230a09d) SHA1(780aff5d4511c5e08cbf78784b163d60358f9283) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "c28-03.29", 0x00000, 0x80000, CRC(51bdc7af) SHA1(e36a063932fa5bd6609930c3708fee1e6feb5389) )
	ROM_LOAD16_BYTE( "c28-04.28", 0x00001, 0x80000, CRC(ba7ed535) SHA1(be7e010f6788d1b82cebc932c793a0a976647832) )

	ROM_REGION( 0x080000, "tc0100scn_2", 0 )
	ROM_LOAD16_WORD_SWAP( "c28-01hc01.63", 0x000000, 0x80000, CRC(34ad98e4) SHA1(96e260de01610f9e6ca886a12e75661a54168440) )

	ROM_REGION( 0x1c000, "audiocpu", 0 )
	ROM_LOAD( "c28-14.3",  0x00000, 0x10000, CRC(45ef3616) SHA1(97bf1de7fd32a378839df1845f7522dae776d997) )

	ROM_REGION( 0x80000, "ymsnd", 0 )
	ROM_LOAD( "c28-06.41", 0x00000, 0x80000, CRC(db6983db) SHA1(b72541aa35c48624478060e7453f01956ff1ceb2) )

	ROM_REGION( 0x80000, "ymsnd.deltat", 0 )
	ROM_LOAD( "c28-05.42", 0x00000, 0x80000, CRC(d3b238fa) SHA1(b4a0cdd7174e60527e7a47018d6117adc5518da1) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Cameltry
GAME( 1989, cameltryjs01,  cameltry, cameltry,  cameltryj,  taitof2_state, init_cameltry, ROT0,   "hack",         "Cameltry (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Dead Connection
GAME( 1992, deadconxjs01,  deadconx, deadconxj, deadconxj,  taitof2_state, empty_init, ROT0,   "hack",         "Dead Connection (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Don Doko Don
GAME( 1989, dondokodjs01,  dondokod, dondokod,  dondokodj,  taitof2_state, empty_init, ROT0,   "hack",         "Don Doko Don (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Dino Rex
GAME( 1992, dinorexs01,    dinorex,  dinorex,   dinorex,    taitof2_state, empty_init, ROT0,   "DDJ",             "Dino Rex (Easy Move)", MACHINE_SUPPORTS_SAVE )
// Gun Frontier
GAME( 1990, gunfrontjs01,  gunfront, gunfront,  gunfrontj,  taitof2_state, empty_init, ROT270, "hack",         "Gun Frontier (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Mega Blast
GAME( 1989, megablstjs01,  megablst, megab,     megabj,     taitof2_state, empty_init, ROT0,   "hack",         "Mega Blast (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Mizubaku Daibouken
GAME( 1990, mizubakus01,   liquidk,  liquidk,   mizubaku,   taitof2_state, empty_init, ROT0,   "hack",         "Mizubaku Daibouken (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Runark
GAME( 1990, runarks01,     growl,    growl,     runark,     taitof2_state, empty_init, ROT0,   "hack",         "Runark (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
// Thunder Fox
GAME( 1990, thundfoxjs01,  thundfox, thundfox,  thundfoxj,  taitof2_state, empty_init, ROT0,   "hack",         "Thunder Fox (Translation Chinese)", MACHINE_SUPPORTS_SAVE )

