// license:BSD-3-Clause
// copyright-holders:Robbbert
/* BBUGTEST NOTES

With no documentation whatsoever, I have compiled what I know to share with you all.

I do not know if the graphics are complete, or if all tests are working.

Use the dipswitches to select a test pattern.

Pressing any player key, or start button will make a slight change to the colours.

If you move your mouse to the left, pressing the keys will alter the numbers across the bottom
of the screen. Moving the mouse to the right will revert to changing colours again.

2019-08-15 It seems the 3 boxes are representations of the 3 ram chips. A rectangle indicates
ram tested ok. A forward slash indicates ram was stuck at 0. A backslash indicates ram was stuck
at 1. And X indicates ram couldn't be reliably tested.

*/


#include "../mame/drivers/cinemat.cpp"

class cinemat_hbmame : public cinemat_color_state
{
public:
	using cinemat_color_state::cinemat_color_state;

	void init_bbugtest()
	{
		m_maincpu->space(AS_IO).install_read_handler(0x0c, 0x0f, read8sm_delegate(*this, FUNC(cinemat_hbmame::boxingb_dial_r)));
	}
};



/*************************************
 *
 *  Game drivers
 *
 *************************************/

ROM_START( bbugtest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "bbugtest.t7", 0x0000, 0x1000, CRC(92a460f1) SHA1(eaa8fea38d87f1fd6e2e52b92085fed43498e5ce) )
	ROM_LOAD16_BYTE( "bbugtest.p7", 0x0001, 0x1000, CRC(06d0bde2) SHA1(b0a224b7a74fa739b293ad7383236ffaa012f3c3) )

	ROM_REGION( 0x1a0, "proms", 0 )
	ROM_LOAD("prom.f14", 0x000, 0x100, CRC(9edbf536) SHA1(036ad8a231284e05f44b1106d38fc0c7e041b6e8) )
	ROM_LOAD("prom.e14", 0x100, 0x020, CRC(29dbfb87) SHA1(d8c40ab010b2ea30f29b2c443819e2b69f376c04) )
	ROM_LOAD("prom.d14", 0x120, 0x020, CRC(9a05afbf) SHA1(5d806a42424942ba5ef0b70a1d629315b37f931b) )
	ROM_LOAD("prom.c14", 0x140, 0x020, CRC(07492cda) SHA1(32df9148797c23f70db47b840139c40e046dd710) )
	ROM_LOAD("prom.j14", 0x160, 0x020, CRC(a481ca71) SHA1(ce145d61686f600cc16b77febfd5c783bf8c13b0) )
	ROM_LOAD("prom.e8",  0x180, 0x020, CRC(791ec9e1) SHA1(6f7fcce4aa3be9020595235568381588adaab88e) )
ROM_END

ROM_START( bbugtest1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "bbugtest1.t7", 0x0000, 0x1000, CRC(53154517) SHA1(63b21f7548dcd1de9ca193fe82ba768a2949a747) )
	ROM_LOAD16_BYTE( "bbugtest1.p7", 0x0001, 0x1000, CRC(ac5a63ee) SHA1(45b2940929a06e65cca8d50d79bdbda8acc6c628) )

	ROM_REGION( 0x1a0, "proms", 0 )
	ROM_LOAD("prom.f14", 0x000, 0x100, CRC(9edbf536) SHA1(036ad8a231284e05f44b1106d38fc0c7e041b6e8) )
	ROM_LOAD("prom.e14", 0x100, 0x020, CRC(29dbfb87) SHA1(d8c40ab010b2ea30f29b2c443819e2b69f376c04) )
	ROM_LOAD("prom.d14", 0x120, 0x020, CRC(9a05afbf) SHA1(5d806a42424942ba5ef0b70a1d629315b37f931b) )
	ROM_LOAD("prom.c14", 0x140, 0x020, CRC(07492cda) SHA1(32df9148797c23f70db47b840139c40e046dd710) )
	ROM_LOAD("prom.j14", 0x160, 0x020, CRC(a481ca71) SHA1(ce145d61686f600cc16b77febfd5c783bf8c13b0) )
	ROM_LOAD("prom.e8",  0x180, 0x020, CRC(791ec9e1) SHA1(6f7fcce4aa3be9020595235568381588adaab88e) )
ROM_END

GAME(1981, bbugtest,  0,        boxingb, boxingb, cinemat_hbmame, init_bbugtest, ORIENTATION_FLIP_Y, "Test Rom", "Test Rom - Boxing Bugs", MACHINE_NO_SOUND_HW | MACHINE_SUPPORTS_SAVE )
GAME(2018, bbugtest1, bbugtest, boxingb, boxingb, cinemat_hbmame, init_bbugtest, ORIENTATION_FLIP_Y, "Timothy Shiels", "Test Rom - Cinematic", MACHINE_NO_SOUND_HW | MACHINE_SUPPORTS_SAVE )

// WARRIOR modifications. See http://www.outerworldarcade.com/arcade/cinematronics/mods/warrior/warrior_4j_mod.html

static INPUT_PORTS_START( warrior4j )
	PORT_START("INPUTS")
	// Player 2 movement
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_RIGHT )  PORT_PLAYER(2)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_LEFT )   PORT_PLAYER(2)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP )     PORT_PLAYER(2)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN )   PORT_PLAYER(2)
	// Player 2 sword
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_RIGHT ) PORT_PLAYER(2) // WAS FIRE
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_LEFT )  PORT_PLAYER(2)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP )    PORT_PLAYER(2)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN )  PORT_PLAYER(2)
	// Player 1 movement
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_RIGHT )  PORT_PLAYER(1)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_LEFT )   PORT_PLAYER(1)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP )     PORT_PLAYER(1)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN )   PORT_PLAYER(1)
	// Player 1 sword
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_RIGHT ) PORT_PLAYER(1) // WAS FIRE
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_LEFT )  PORT_PLAYER(1)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP )    PORT_PLAYER(1)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN )  PORT_PLAYER(1)

	PORT_START("SWITCHES")
	PORT_DIPNAME( 0x03, 0x02, "Time" )      PORT_DIPLOCATION("SW1:!1,!2")
	PORT_DIPSETTING(    0x00, "0:30/coin" )
	PORT_DIPSETTING(    0x02, "1:00/coin" )
	PORT_DIPSETTING(    0x01, "1:30/coin" )
	PORT_DIPSETTING(    0x03, "2:00/coin" )
	PORT_SERVICE_DIPLOC( 0x04, IP_ACTIVE_HIGH, "SW1:!3" )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Coinage ) )  PORT_DIPLOCATION("SW1:!4")
	PORT_DIPSETTING(    0x00, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 1C_1C ) )
	PORT_DIPUNUSED_DIPLOC( 0x10, IP_ACTIVE_HIGH, "SW1:!5" )
	PORT_DIPUNUSED_DIPLOC( 0x20, IP_ACTIVE_HIGH, "SW1:!6" )
	PORT_DIPUNUSED_DIPLOC( 0x40, IP_ACTIVE_HIGH, "SW1:!7" )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN1 ) PORT_CHANGED_MEMBER(DEVICE_SELF, cinemat_state,coin_inserted, 0)
INPUT_PORTS_END

ROM_START( warrior4j1 )
	ROM_REGION( 0x2000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "warrior4j1.t7", 0x0000, 0x0800, CRC(2794ae97) SHA1(d247591a628682c4bf05af8e214a421c6aad27ac) )
	ROM_LOAD16_BYTE( "warrior4j1.p7", 0x0001, 0x0800, CRC(0174aec4) SHA1(763d9518eb10c48dbad8f1bc445a20dbc1e283cd) )
	ROM_LOAD16_BYTE( "warrior4j1.u7", 0x1000, 0x0800, CRC(aa9c5a41) SHA1(15a78813bde8b90dc2431de9c74a5162e6cc81b0) )
	ROM_LOAD16_BYTE( "warrior4j1.r7", 0x1001, 0x0800, CRC(6bd52cdd) SHA1(c702cac293703dc5abf8c9d4f1b4d9b183ebacf7) )

	CCPU_PROMS
ROM_END

ROM_START( warrior4j2 )
	ROM_REGION( 0x2000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "warrior4j2.t7", 0x0000, 0x0800, CRC(fdf360c8) SHA1(4a1eb1d38a0a1523e1b5da5f95011136ed34e0ab) )
	ROM_LOAD16_BYTE( "warrior4j2.p7", 0x0001, 0x0800, CRC(b9b94d47) SHA1(18964b0c05841003d0b67a08f4f5760122668dce) )
	ROM_LOAD16_BYTE( "warrior4j2.u7", 0x1000, 0x0800, CRC(f7de23b2) SHA1(a5e2efbfa10e0aaf081f81b1c88ab0042b6b7126) )
	ROM_LOAD16_BYTE( "warrior4j2.r7", 0x1001, 0x0800, CRC(ba00c108) SHA1(3b37f24bc66ba9607dc11655bbbd8f53bd77cda5) )

	CCPU_PROMS
ROM_END

GAME(2014, warrior4j1, warrior, warrior, warrior4j, cinemat_state, empty_init, ORIENTATION_FLIP_Y, "Timothy Shiels", "Warrior (4 joysticks, v1)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME(2014, warrior4j2, warrior, warrior, warrior4j, cinemat_state, empty_init, ORIENTATION_FLIP_Y, "Timothy Shiels", "Warrior (4 joysticks, v2)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )

