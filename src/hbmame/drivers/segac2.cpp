// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/segac2.cpp"

/* http://frogfeast.rastersoft.net/SegaC2.html
    It appears from the source that the sound is going through a Z80 - but the C2 hardware doesn't have one...
    Also there are unmapped writes to A00000-A00100, A11100, A11200. */

ROM_START( c2frog )
	ROM_REGION( 0x1000000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c2frog.ic32", 0x000000, 0x020000, CRC(d7039bd3) SHA1(82b9e3a760ebc845afe7bd78f22b897e1a530651) )
	ROM_LOAD16_BYTE( "c2frog.ic31", 0x000001, 0x020000, CRC(754b111a) SHA1(5542fc3b955df843e15e7c98aebc4f80c7d44b69) )
ROM_END

GAME( 2007, c2frog, 0, segac, columns, segac2_state, init_columns, ROT0, "Rastersoft", "Frog Feast (Sega C2 hardware)", MACHINE_NO_SOUND )



static INPUT_PORTS_START( ssonicbr )
	PORT_INCLUDE( systemc_generic )
	PORT_MODIFY("P1")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNUSED )     /* Button 2 Unused */
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNUSED )     /* Button 3 Unused */
	PORT_MODIFY("P2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNUSED )     /* Button 2 Unused */
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNUSED )     /* Button 3 Unused */
	PORT_MODIFY("DSW")
	PORT_DIPNAME( 0x01, 0x01, DEF_STR( Demo_Sounds ) ) PORT_DIPLOCATION("SW2:1")
	PORT_DIPSETTING(    0x01, DEF_STR( On ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPUNKNOWN_DIPLOC( 0x02, IP_ACTIVE_LOW, "SW2:2" )
	PORT_DIPUNKNOWN_DIPLOC( 0x04, IP_ACTIVE_LOW, "SW2:3" )
	PORT_DIPUNKNOWN_DIPLOC( 0x08, IP_ACTIVE_LOW, "SW2:4" )
	PORT_DIPUNKNOWN_DIPLOC( 0x10, IP_ACTIVE_LOW, "SW2:5" )
	PORT_DIPUNKNOWN_DIPLOC( 0x20, IP_ACTIVE_LOW, "SW2:6" )
	PORT_DIPUNKNOWN_DIPLOC( 0x40, IP_ACTIVE_LOW, "SW2:7" )
	PORT_DIPUNKNOWN_DIPLOC( 0x80, IP_ACTIVE_LOW, "SW2:8" )
INPUT_PORTS_END



ROM_START( ssonicbr )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "ssonicbr.ic32", 0x000000, 0x040000, CRC(cf254ecd) SHA1(4bb295ec80f8ddfeab4e360eebf12c5e2dfb9800) )
	ROM_LOAD16_BYTE( "ssonicbr.ic31", 0x000001, 0x040000, CRC(03709746) SHA1(0b457f557da77acd3f43950428117c1decdfaf26) )
	ROM_REGION( 0x020000, "upd", 0 )
	ROM_LOAD( "ssonicbr.ic4", 0x000000, 0x020000, CRC(78e56a51) SHA1(8a72c12975cd74919b4337e0f681273e6b5cbbc6) )
ROM_END

ROM_START( ooparts )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "ooparts.ic32", 0x000000, 0x080000, CRC(8dcf2940) SHA1(f72630e8a26e7f2089da56878a1599268c355246) )
	ROM_LOAD16_BYTE( "ooparts.ic31", 0x000001, 0x080000, CRC(35381899) SHA1(524f6e1b1292542079589275e20f45c2eb68605c) )
	ROM_LOAD16_BYTE( "ooparts.ic34", 0x100000, 0x080000, CRC(7192ac29) SHA1(d3028a9bbb7faa733285cf7e47fd840ec0d0bf69) )
	ROM_LOAD16_BYTE( "ooparts.ic33", 0x100001, 0x080000, CRC(42755dc2) SHA1(cd0aa79418b922266c5d41bf24b9136f9f105dc5) )

	ROM_REGION( 0x040000, "upd", 0 )
	ROM_LOAD( "ooparts.ic4", 0x000000, 0x040000, CRC(e09961f6) SHA1(e109b5f41502b765d191f22e3bbcff97d6defaa1) )
ROM_END


GAME( 1992, ssonicbr,  0, segac2, ssonicbr, segac2_state, init_bloxeedc, ROT0, "Sega", "SegaSonic Bros. (Japan, prototype)", 0 )
GAME( 1992, ooparts,   0, segac2, ichir,    segac2_state, init_tantr,    ROT270, "Success", "OOParts", 0 )

