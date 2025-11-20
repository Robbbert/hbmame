// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/ddragon3.cpp"

 /**********************
 WWF Wrestling Festival
************************/

ROM_START( wwewfest )
	ROM_REGION( 0x80000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "31e13-0.ic19", 0x00001, 0x40000, CRC(4fda3f4c) SHA1(08bd8d3e0b51857681b58ddd43e9c54d4d62f462) )
	ROM_LOAD16_BYTE( "31e14-0.ic18", 0x00000, 0x40000, CRC(06e7b1bc) SHA1(71fbe292e124c5357088d1e9e9b1620bc483714b) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "31a11-2.ic42", 0x00000, 0x10000, CRC(5ddebfea) SHA1(30073963e965250d94f0dc3bd261a054850adf95) )

	ROM_REGION( 0x80000, "oki", 0 )
	ROM_LOAD( "31j10.ic73",   0x00000, 0x80000, CRC(6c522edb) SHA1(8005d59c94160638ba2ea7caf4e991fff03003d5) )

	ROM_REGION( 0x20000, "gfx1", 0 )
	ROM_LOAD( "31e12-0.ic33", 0x00000, 0x20000, CRC(5a5734f4) SHA1(21c42bc3b1873fda760ccfba4b8114e75fcb1244) )

	ROM_REGION( 0x800000, "gfx2", 0 )
	ROM_LOAD( "31j3.ic9",     0x000000, 0x100000, CRC(7f15f532) SHA1(00b9dfdd6e6a6dfaeaab09e7d0867c62a5068c8e) )
	ROM_LOAD( "31j2.ic8",     0x100000, 0x100000, CRC(5ea77cfe) SHA1(d3fc8eabb4b0ebf87715cb3f82d589ad0fd3636f) )
	ROM_LOAD( "31j5.ic11",    0x200000, 0x100000, CRC(baa3aa29) SHA1(e166513ccb76afc4eea0a5a6841f21a984b1abb1) )
	ROM_LOAD( "31j4.ic10",    0x300000, 0x100000, CRC(ebe3bef1) SHA1(f8f4f1c00cd53107d63b7a0dc2eccc4d7765d100) )
	ROM_LOAD( "31j6.ic12",    0x400000, 0x100000, CRC(54a60260) SHA1(2db35cba93ac7fd62b99851d9ea12b48db18c37d) )
	ROM_LOAD( "31j7.ic13",    0x500000, 0x100000, CRC(64005709) SHA1(ed40aa69c18560b764ac44aa435fcb35f02ab441) )
	ROM_LOAD( "31j9.ic15",    0x600000, 0x100000, CRC(81f63a7e) SHA1(3f72602e7de03e92d568aec56a9419d5c1d719c7) )
	ROM_LOAD( "31j8.ic14",    0x700000, 0x100000, CRC(abff2c7b) SHA1(84646fbc9d094c7a9c7f52432dca2ceb2242d205) )

	ROM_REGION( 0x80000, "gfx3", 0 )
	ROM_LOAD( "31j0.ic1",     0x40000, 0x40000, CRC(f6b4f9d3) SHA1(d6ebfbb2a07f740dfae17a625f53d5b48bead74e) )
	ROM_LOAD( "31j1.ic2",     0x00000, 0x40000, CRC(ec86da22) SHA1(0c95ff9bd64f850251390dbf3402c54c5ac81217) )
ROM_END

/*    YEAR  NAME         PARENT      MACHINE   INPUT     CLASS          INIT      MONITOR COMPANY                 FULLNAME               FLAGS */
GAME( 2025, wwewfest,  wwfwfest, wwfwfest,  wwfwfest,  wwfwfest_state, empty_init, ROT0, "Anton LeDec", "WWE WrestleFest (Basic Fun)",  MACHINE_SUPPORTS_SAVE )

