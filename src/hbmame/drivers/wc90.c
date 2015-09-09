// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/wc90.c"

/****************************************************************************************************

Sounds are completely horrible.

Sprites are out by 16 pixels, so used a horrible hack.

****************************************************************************************************/

static INPUT_PORTS_START( pacwc90 )
	PORT_INCLUDE( wc90 )

	PORT_MODIFY("DSW1")
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Players ) )      PORT_DIPLOCATION("SW1:7")
	PORT_DIPSETTING(    0x02, "1" )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPNAME( 0x0d, 0x09, DEF_STR( Lives ) )        PORT_DIPLOCATION("SW1:8,6,5")
	PORT_DIPSETTING(    0x01, "1" )
	PORT_DIPSETTING(    0x05, "2" )
	PORT_DIPSETTING(    0x09, "3" )
	PORT_DIPSETTING(    0x0d, "5" )
	PORT_DIPNAME( 0x30, 0x00, DEF_STR( Bonus_Life ) )   PORT_DIPLOCATION("SW1:4,3")
	PORT_DIPSETTING(    0x00, "10000" )
	PORT_DIPSETTING(    0x10, "15000" )
	PORT_DIPSETTING(    0x20, "20000" )
	PORT_DIPSETTING(    0x30, DEF_STR( None ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Difficulty ) )   PORT_DIPLOCATION("SW1:2")
	PORT_DIPSETTING(    0x40, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x80, 0x80, "Ghost Names" )           PORT_DIPLOCATION("SW1:1")
	PORT_DIPSETTING(    0x80, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Alternate ) )

	PORT_MODIFY("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END

ROM_START( pacwc90 )
	ROM_REGION( 0x20000, "maincpu", 0 )
	//ROM_LOAD( "ic87_01.bin",  0x00000, 0x08000, CRC(4a1affbc) SHA1(bc531e97ca31c66fdac194e2d79d5c6ba1300556) )  /* c000-ffff is not used */
	ROM_LOAD( "ic87pac.bin",  0x00000, 0x08000, CRC(8af34306) SHA1(1a98adca74f46da36e3648d37bfcb56a328a031e) )
	ROM_LOAD( "ic95_02.bin",  0x10000, 0x10000, CRC(847d439c) SHA1(eade31050da9e84feb4406e327d050a7496871b7) )  /* banked at f000-f7ff */

	ROM_REGION( 0x20000, "sub", 0 )  /* Second CPU */
	//ROM_LOAD( "ic67_04.bin",  0x00000, 0x10000, CRC(dc6eaf00) SHA1(d53924070a59eee35dc0e6465702e4f04e61a073) )  /* c000-ffff is not used */
	ROM_LOAD( "ic67pac.bin",  0x00000, 0x10000, CRC(bc9bfdf2) SHA1(869e4012e5c577e501143cbfd75cce8cef919c86) )
	ROM_LOAD( "ic56_03.bin",  0x10000, 0x10000, CRC(1ac02b3b) SHA1(4f8dc049d404072150342f3c2df04789a73ce244) )  /* banked at f000-f7ff */

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "ic54_05.bin",  0x00000, 0x10000, CRC(27c348b3) SHA1(cf19ff4ae4f323ae3e5a905249b7af8ae342202a) )

	ROM_REGION( 0x010000, "gfx1", 0 )
	//ROM_LOAD( "ic85_07v.bin", 0x00000, 0x10000, CRC(c5219426) SHA1(95e21fcd7de7d418ec287ae7087f6244c6bce5a8) )  /* characters */
	ROM_LOAD( "ic85pac.bin", 0x00000, 0x10000, CRC(0b906dae) SHA1(0d14d6a7bbe0b8772143afb4c6c94c62313e4b9c) )

	ROM_REGION( 0x040000, "gfx2", 0 )
	ROM_LOAD( "ic86_08v.bin", 0x00000, 0x20000, CRC(8fa1a1ff) SHA1(ce624617ac8c8b54e41294cf5dca7a09c91f53ba) )  /* tiles #1 */
	ROM_LOAD( "ic90_09v.bin", 0x20000, 0x20000, CRC(99f8841c) SHA1(1969b4d78ca00924a7550826e1c4f4fa0588ef02) )  /* tiles #2 */

	ROM_REGION( 0x040000, "gfx3", 0 )
	ROM_LOAD( "ic87_10v.bin", 0x00000, 0x20000, CRC(8232093d) SHA1(59bf9c9a858b47326cf0c64b1ee6ac727a15a20b) )  /* tiles #3 */
	ROM_LOAD( "ic91_11v.bin", 0x20000, 0x20000, CRC(188d3789) SHA1(35654a99a20735bae09b32f74255f8132dee9af2) )  /* tiles #4 */

	ROM_REGION( 0x080000, "gfx4", 0 )
	//ROM_LOAD( "ic50_12v.bin", 0x00000, 0x20000, CRC(da1fe922) SHA1(5184053c2b7dd2bf1cd2e9f783686f2c0db7e47b) )  /* sprites  */
	ROM_LOAD( "ic50pac.bin", 0x00000, 0x10000, CRC(190852ea) SHA1(fad7eb3aa53d03917173dd5a040655cfd329db32) )
	//ROM_LOAD( "ic54_13v.bin", 0x20000, 0x20000, CRC(9ad03c2c) SHA1(1c1947f9b51a58002e9992fc7c0c1a1c59b4d740) )  /* sprites  */
	ROM_LOAD( "ic54pac.bin", 0x20000, 0x10000, CRC(1c4d17fd) SHA1(5abebf867de452cc3e85331e91b9110c26a8b050) )
	//ROM_LOAD( "ic60_14v.bin", 0x40000, 0x20000, CRC(499dfb1b) SHA1(ac67985d36fea18c82a4ea00019d9e6e4bcb5d0d) )  /* sprites  */
	ROM_LOAD( "ic60pac.bin", 0x40000, 0x10000, CRC(33effbea) SHA1(dbf6b735f3c8bacb695caf5d15ac8b7961bffc74) )
	ROM_LOAD( "ic65_15v.bin", 0x60000, 0x20000, CRC(d8ea5c81) SHA1(ccb3f7d565b1c1b8e874a2df91cda40dde2962ed) )  /* sprites  */

	ROM_REGION( 0x20000, "ymsnd", 0 )   /* 64k for ADPCM samples */
	// Whatever is here has no effect on the terrible sound
	//ROM_LOAD( "ic82_06.bin",  0x00000, 0x20000, CRC(2fd692ed) SHA1(0273dc39181504320bec0187d074b2f86c821508) )
	ROM_LOAD( "ic82pac.bin",  0x00000, 0x10000, CRC(abc61f3d) SHA1(c6f123d16a26c4d77c635617dd97bb4b906c463a) )
ROM_END


GAME( 1990, pacwc90, wc90, wc90, pacwc90, driver_device, 0, ROT90, "Macro (Mike Coates)", "Pacman on World Cup 90 HW", MACHINE_IMPERFECT_SOUND | MACHINE_NO_COCKTAIL | MACHINE_SUPPORTS_SAVE )
