// license:GPL_2.0
// copyright-holders:Robbbert
#include "../mame/drivers/williams.cpp"

namespace {

ROM_START( defender2 )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "10",         0x0d000, 0x1000, CRC(4d82fc2f) SHA1(b11dc3f3f2100ab14ad5b26cd333b212c454cc8d) )
	ROM_LOAD( "11",         0x0e000, 0x1000, CRC(561fad3f) SHA1(597851e78423f30341f682f3fc62bf24cf9e8a91) )
	ROM_LOAD( "12",         0x0f000, 0x1000, CRC(836eacca) SHA1(f788fc2a25adffa21e02a3dfe2074e8719405200) )
	ROM_LOAD( "01",         0x10000, 0x1000, CRC(88824d18) SHA1(f003a5a9319c4eb8991fa2aae3f10c72d6b8e81a) )
	ROM_LOAD( "02",         0x11000, 0x1000, CRC(afc614c5) SHA1(087c6da93318e8dc922d3d22e0a2af7b9759701c) )
	ROM_LOAD( "03",         0x12000, 0x1000, CRC(6dd57970) SHA1(1fdee9180bfb835f35ac786116e0f8d7833f2c3f) )
	ROM_LOAD( "04",         0x13000, 0x1000, CRC(a8b4bf0f) SHA1(6b4d47c2899fe9f14f9dab5928499f12078c437d) )
	ROM_LOAD( "05",         0x14000, 0x1000, CRC(2d306074) SHA1(54f871983699113e31bb756d4ca885c26c2d66b4) )
	ROM_LOAD( "06",         0x15000, 0x1000, CRC(53598dde) SHA1(54b02d944caf95283c9b6f0160e75ea8c4ccc97b) )
	ROM_LOAD( "07",         0x16000, 0x1000, CRC(55b5b0d2) SHA1(dcf392a899fb6c225c4e31034e242e575c3dc94d) )
	ROM_LOAD( "08",         0x17000, 0x1000, CRC(b4f22d4d) SHA1(2486e895628df9ae1268d9eb2434f240dba99f70) )
	ROM_LOAD( "09",         0x18000, 0x1000, CRC(6df7463d) SHA1(b80c2872fc47b4b604967f176e3931cd992dd692) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "sg.snd",      0xf800, 0x0800, CRC(2fcf6c4d) SHA1(9c4334ac3ff15d94001b22fc367af40f9deb7d57) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.4",   0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder.5",   0x0200, 0x0200, CRC(f921c5fe) SHA1(9cebb8bb935315101d248140d1b4503993ebdf8a) )
ROM_END

ROM_START( sinistar01 ) // balanced ver. First time run gives blank screen, press F3.
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "sinistar_rom_10-b_16-3004-62.4c", 0x0e000, 0x1000, CRC(3d670417) SHA1(81802622bee8dbea5c0f08019d87d941dcdbe292) )
	ROM_LOAD( "sinistar01_rom_11.4a", 0x0f000, 0x1000, CRC(fe83bce2) SHA1(7774602fbae7c345341be54eb281cf97ead04212) )
	ROM_LOAD( "sinistar_rom_1-b_16-3004-53.1d",  0x10000, 0x1000, CRC(f6f3a22c) SHA1(026d8cab07734fa294a5645edbe65a904bcbc302) )
	ROM_LOAD( "sinistar_rom_2-b_16-3004-54.1c",  0x11000, 0x1000, CRC(cab3185c) SHA1(423d1e3b0c07333ec582529bc4d0b7baf591820a) )
	ROM_LOAD( "sinistar_rom_3-b_16-3004-55.1a",  0x12000, 0x1000, CRC(1ce1b3cc) SHA1(5bc03d7249529d827dc60c087e074ab3e4ea7361) )
	ROM_LOAD( "sinistar_rom_4-b_16-3004-56.2d",  0x13000, 0x1000, CRC(6da632ba) SHA1(72c0c3d5a5ca87ca4d95fcedaf834206e4633950) )
	ROM_LOAD( "sinistar_rom_5-b_16-3004-57.2c",  0x14000, 0x1000, CRC(b662e8fc) SHA1(828a89d2ea13d8a362dae708f86bff54cb231887) )
	ROM_LOAD( "sinistar_rom_6-b_16-3004-58.2a",  0x15000, 0x1000, CRC(2306183d) SHA1(703e29e6446856615760a4897c0f5d79cc7bdfb2) )
	ROM_LOAD( "sinistar_rom_7-b_16-3004-59.3d",  0x16000, 0x1000, CRC(e5dd918e) SHA1(bf4e2ada6a59d246218544d822ba5355da925924) )
	ROM_LOAD( "sinistar01_rom_8.3c",  0x17000, 0x1000, CRC(9b693243) SHA1(04183031598cc4b152b107d583e4f8346a2f60eb) )
	ROM_LOAD( "sinistar01_rom_9.3a",  0x18000, 0x1000, CRC(5ed44beb) SHA1(f77a7d90ebb5e7bf516dbd947aca3b808ff3443d) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "3004_speech_ic7_r1_16-3004-52.ic7", 0xb000, 0x1000, CRC(e1019568) SHA1(442f4f3ccd2e1db2136d2ffb121ea442921f87ca) )
	ROM_LOAD( "3004_speech_ic5_r1_16-3004-50.ic5", 0xc000, 0x1000, CRC(cf3b5ffd) SHA1(d5d51c550581c9d46ab331dd4fd32541a2ef598e) )
	ROM_LOAD( "3004_speech_ic6_r1_16-3004-51.ic6", 0xd000, 0x1000, CRC(ff8d2645) SHA1(16fa2a602acbbc182dd96bab113ab18356f3daf0) )
	ROM_LOAD( "3004_speech_ic4_r1_16-3004-49.ic4", 0xe000, 0x1000, CRC(4b56a626) SHA1(44430cd5c110ec751b0bfb8ae99b26d443350db1) )
	ROM_LOAD( "video_sound_rom_9_std.808.ic12",    0xf000, 0x1000, CRC(b82f4ddb) SHA1(c70c7dd6e88897920d7709a260f27810f66aade1) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder_rom_4.3g", 0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder_rom_6.3c", 0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

ROM_START( splatsc1 )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "splatsc1.10",               0x0d000, 0x1000, CRC(6cc2988a) SHA1(bd0648185ab859afc981d4e8ab954ed1f39303e4) )
	ROM_LOAD( "splatsc1.11",               0x0e000, 0x1000, CRC(ee960115) SHA1(b3a03b14f7ee97d55b948ecc09c5e4006c31f0ce) )
	ROM_LOAD( "splatsc1.12",               0x0f000, 0x1000, CRC(21916fce) SHA1(6896afd10c30d90b807b60f90fced7114b1812e3) )
	ROM_LOAD( "splat_rom_1b_16-3011-1.e4", 0x10000, 0x1000, CRC(1cf26e48) SHA1(6ba4de6cc7d1359ed450da7bae1000552373f873) )
	ROM_LOAD( "splat_rom_2b_16-3011-2.c4", 0x11000, 0x1000, CRC(ac0d4276) SHA1(710aba98909d5d63c4b9b08579021f9c026b3111) )
	ROM_LOAD( "splat_rom_3b_16-3011-3.a4", 0x12000, 0x1000, CRC(74873e59) SHA1(727c9da682fd10353f3969ef02e9f1826d8cb77a) )
	ROM_LOAD( "splat_rom_4b_16-3011-4.e5", 0x13000, 0x1000, CRC(70a7064e) SHA1(7e6440585462b68b62d6d571d83635bf17149f1a) )
	ROM_LOAD( "splatsc1.05",               0x14000, 0x1000, CRC(6989e923) SHA1(383d831084cf79e3961125142d1e3bf4fad1ac66) )
	ROM_LOAD( "splatsc1.06",               0x15000, 0x1000, CRC(82b22e55) SHA1(d71c6598f6a43f47f6752c803069efeb877bf1f0) )
	ROM_LOAD( "splatsc1.07",               0x16000, 0x1000, CRC(e0c4230c) SHA1(2795732725f63e766ee23744147bf002373bc5f7) )
	ROM_LOAD( "splatsc1.08",               0x17000, 0x1000, CRC(42ff589f) SHA1(72c9b40c9291a6b97f637c6424265b30227d87c6) )
	ROM_LOAD( "splatsc1.09",               0x18000, 0x1000, CRC(f67564be) SHA1(3fda9032953e4f75df838616d1c7023a1f2e9861) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "video_sound_rom_13_std.ic12", 0xf000, 0x1000, CRC(a878d5f3) SHA1(f3347a354cb54ca228fe0971f0ae3bc778e2aecf) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder_rom_4.3g",          0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder_rom_6.3c",          0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

ROM_START( robotron201 )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "2084_rom_10b_3005-22.a7",  0x0d000, 0x1000, CRC(13797024) SHA1(d426a50e75dabe936de643c83a548da5e399331c) )
	ROM_LOAD( "robotronp.sbb",            0x0e000, 0x1000, CRC(e83a2eda) SHA1(4a62fcd2f91dfb609c3d2c300bd9e6cb60edf52e) )
	ROM_LOAD( "2084_rom_12b_3005-24.e7",  0x0f000, 0x1000, CRC(645d543e) SHA1(fad7cea868ebf17347c4bc5193d647bbd8f9517b) )
	ROM_LOAD( "2084_rom_1b_3005-13.e4",   0x10000, 0x1000, CRC(66c7d3ef) SHA1(f6d60e26c209c1df2cc01ac07ad5559daa1b7118) )
	ROM_LOAD( "2084_rom_2b_3005-14.c4",   0x11000, 0x1000, CRC(5bc6c614) SHA1(4d6e82bc29f49100f7751ccfc6a9ff35695b84b3) )
	ROM_LOAD( "robotron201.3",            0x12000, 0x1000, CRC(85eb583e) SHA1(b6c4280415515de6f56b358206dc3bd93a12bfce) )
	ROM_LOAD( "2084_rom_4b_3005-16.e5",   0x13000, 0x1000, CRC(afb1c561) SHA1(aaf89c19fd8f4e8750717169eb1af476aef38a5e) )
	ROM_LOAD( "robotronp.5",              0x14000, 0x1000, CRC(827cb5c9) SHA1(1732d16cd88e0662f1cffce1aeda5c8aa8c31338) )
	ROM_LOAD( "2084_rom_6b_3005-18.a5",   0x15000, 0x1000, CRC(bd2c853d) SHA1(f76ec5432a7939b33a27be1c6855e2dbe6d9fdc8) )
	ROM_LOAD( "2084_rom_7b_3005-19.e6",   0x16000, 0x1000, CRC(49ac400c) SHA1(06eae5138254723819a5e93cfd9e9f3285fcddf5) )
	ROM_LOAD( "2084_rom_8b_3005-20.c6",   0x17000, 0x1000, CRC(3a96e88c) SHA1(7ae38a609ed9a6f62ca003cab719740ed7651b7c) )
	ROM_LOAD( "2084_rom_9b_3005-21.a6",   0x18000, 0x1000, CRC(b124367b) SHA1(fd9d75b866f0ebbb723f84889337e6814496a103) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "video_sound_rom_3_std_767.ic12", 0xf000, 0x1000, CRC(c56c1d28) SHA1(15afefef11bfc3ab78f61ab046701db78d160ec3) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder_rom_4.3g",         0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder_rom_6.3c",         0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

ROM_START( robotronp )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "2084_rom_10b_3005-22.a7",  0x0d000, 0x1000, CRC(13797024) SHA1(d426a50e75dabe936de643c83a548da5e399331c) )
	ROM_LOAD( "robotronp.sbb",            0x0e000, 0x1000, CRC(e83a2eda) SHA1(4a62fcd2f91dfb609c3d2c300bd9e6cb60edf52e) )
	ROM_LOAD( "2084_rom_12b_3005-24.e7",  0x0f000, 0x1000, CRC(645d543e) SHA1(fad7cea868ebf17347c4bc5193d647bbd8f9517b) )
	ROM_LOAD( "2084_rom_1b_3005-13.e4",   0x10000, 0x1000, CRC(66c7d3ef) SHA1(f6d60e26c209c1df2cc01ac07ad5559daa1b7118) )
	ROM_LOAD( "2084_rom_2b_3005-14.c4",   0x11000, 0x1000, CRC(5bc6c614) SHA1(4d6e82bc29f49100f7751ccfc6a9ff35695b84b3) )
	ROM_LOAD( "2084_rom_3b_3005-15.a4",   0x12000, 0x1000, CRC(e99a82be) SHA1(06a8c8dd0b4726eb7f0bb0e89c8533931d75fc1c) )
	ROM_LOAD( "2084_rom_4b_3005-16.e5",   0x13000, 0x1000, CRC(afb1c561) SHA1(aaf89c19fd8f4e8750717169eb1af476aef38a5e) )
	ROM_LOAD( "robotronp.5",              0x14000, 0x1000, CRC(827cb5c9) SHA1(1732d16cd88e0662f1cffce1aeda5c8aa8c31338) )
	ROM_LOAD( "2084_rom_6b_3005-18.a5",   0x15000, 0x1000, CRC(bd2c853d) SHA1(f76ec5432a7939b33a27be1c6855e2dbe6d9fdc8) )
	ROM_LOAD( "2084_rom_7b_3005-19.e6",   0x16000, 0x1000, CRC(49ac400c) SHA1(06eae5138254723819a5e93cfd9e9f3285fcddf5) )
	ROM_LOAD( "2084_rom_8b_3005-20.c6",   0x17000, 0x1000, CRC(3a96e88c) SHA1(7ae38a609ed9a6f62ca003cab719740ed7651b7c) )
	ROM_LOAD( "2084_rom_9b_3005-21.a6",   0x18000, 0x1000, CRC(b124367b) SHA1(fd9d75b866f0ebbb723f84889337e6814496a103) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "video_sound_rom_3_std_767.ic12", 0xf000, 0x1000, CRC(c56c1d28) SHA1(15afefef11bfc3ab78f61ab046701db78d160ec3) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder_rom_4.3g",         0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder_rom_6.3c",         0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

ROM_START( robotrontie )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "robotrontie.a",            0x0d000, 0x1000, CRC(952bea55) SHA1(80f51d8e7ec62518afad7e56a47e0756f83f813c) )
	ROM_LOAD( "robotrontie.b",            0x0e000, 0x1000, CRC(4c05fd3c) SHA1(0d727458454826fd8222e4022b755d686ccb065f) )
	ROM_LOAD( "2084_rom_12b_3005-24.e7",  0x0f000, 0x1000, CRC(645d543e) SHA1(fad7cea868ebf17347c4bc5193d647bbd8f9517b) )
	ROM_LOAD( "2084_rom_1b_3005-13.e4",   0x10000, 0x1000, CRC(66c7d3ef) SHA1(f6d60e26c209c1df2cc01ac07ad5559daa1b7118) )
	ROM_LOAD( "2084_rom_2b_3005-14.c4",   0x11000, 0x1000, CRC(5bc6c614) SHA1(4d6e82bc29f49100f7751ccfc6a9ff35695b84b3) )
	ROM_LOAD( "2084_rom_3b_3005-15.a4",   0x12000, 0x1000, CRC(e99a82be) SHA1(06a8c8dd0b4726eb7f0bb0e89c8533931d75fc1c) )
	ROM_LOAD( "robotrontie.4",            0x13000, 0x1000, CRC(e8238019) SHA1(0ce29f4bf6bdee677c8e80c2d5e66fc556ba349f) )
	ROM_LOAD( "robotronp.5",              0x14000, 0x1000, CRC(827cb5c9) SHA1(1732d16cd88e0662f1cffce1aeda5c8aa8c31338) )
	ROM_LOAD( "2084_rom_6b_3005-18.a5",   0x15000, 0x1000, CRC(bd2c853d) SHA1(f76ec5432a7939b33a27be1c6855e2dbe6d9fdc8) )
	ROM_LOAD( "robotrontie.7",            0x16000, 0x1000, CRC(3ecf4620) SHA1(3c670a1f8df35d18451c82f220a02448bf5ef5ac) )
	ROM_LOAD( "robotrontie.8",            0x17000, 0x1000, CRC(752d7a46) SHA1(85dd58d14d527ca75d6c546d6271bf8ee5a82c8c) )
	ROM_LOAD( "2084_rom_9b_3005-21.a6",   0x18000, 0x1000, CRC(b124367b) SHA1(fd9d75b866f0ebbb723f84889337e6814496a103) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "video_sound_rom_3_std_767.ic12", 0xf000, 0x1000, CRC(c56c1d28) SHA1(15afefef11bfc3ab78f61ab046701db78d160ec3) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder_rom_4.3g",         0x0000, 0x0200, CRC(e6631c23) SHA1(9988723269367fb44ef83f627186a1c88cf7877e) )
	ROM_LOAD( "decoder_rom_6.3c",         0x0200, 0x0200, CRC(83faf25e) SHA1(30002643d08ed983a6701a7c4b5ee74a2f4a1adb) )
ROM_END

//------------------------------------------------------- CONQUEST ---------------------------------------------------------

// CONQUEST
/* Unfinished game from Williams.

The rotary encoder is of the "absolute" type - it outputs 4-bit 0 to 15 to
indicate the position. 0 = up, 8 = down, etc.

SUPPLIED NOTES:
This ROM set has been made available with the consent of its co-author, 
Larry DeMar.

ConquestA.ROM loads at $0000-$8FFF
ConquestB.ROM loads at $D000-$FFFF

Being an early prototype, these ROM images have a large amount of empty space.

CMOS is at $CC00-$CFFF, but it appears to expect 8-bits rather than the usual 
4-bit CMOS for a Williams game.

Architecture is similar to Robotron (including interrupts, raster counter, 
etc.) but doesn't use a blitter.

Unique PIA functions:

$C804: Bit 0 = Fire
       Bit 1 = Thrust
       Bit 2-3 = Bits 2-3 of encoder wheel
       Bit 4 = 2P Start
       Bit 5 = 1P Start

$C806: Bits 0-1 = Bits 0-1 of encoder wheel
       Bit 7 = Cocktail (unused)

The prototype appears to use the Defender's sound ROM.

Game play:

Basic game play has a ship that moves in a 2D scrolling world with scanner,
similar to Sinistar. Rotate, shoot and thrust. Destroy "Planets" while 
avoiding or destroying their hums. A wave ends when all planets are 
destroyed. There is no attract mode implemented so the ROM simply displays
a solid colour-shifting background until 1P Start or 2P Start is pressed.

Play an emulation of this ROM set at http://www.codemystics.com/conquest.

*/

class conquest_hbmame : public williams_state
{
public:
	conquest_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: williams_state(mconfig, type, tag)
	{ }

	ioport_value read_lower() { read_the_dial(); return BIT(m_dial, 5, 2); } // first digit '5' varies the speed of turning
	ioport_value read_upper() { read_the_dial(); return BIT(m_dial, 7, 2); } // first digit '7' must always be +2 of above first digit

private:

	u16 m_dial = 0U;
	void read_the_dial();
};

static INPUT_PORTS_START( conquest )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("Fire")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_NAME("Thrust")
	PORT_BIT( 0x0c, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(conquest_hbmame, read_lower) // custom handler wants 0-3, which gets upscaled
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("IN1")
	PORT_BIT( 0x03, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(conquest_hbmame, read_upper)
	PORT_BIT( 0xfc, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("IN2")
	// not used?

	PORT_START("DIAL")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT) PORT_NAME("Left")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT) PORT_NAME("Right")
INPUT_PORTS_END

void conquest_hbmame::read_the_dial()
{
	u8 data = ioport("DIAL")->read();
	if (data == 1)
		m_dial++;
	else
	if (data == 2)
		m_dial--;
	return;
}

ROM_START( conquest )
	ROM_REGION( 0x19000, "maincpu", 0 )
	ROM_LOAD( "conquest.b",               0x0d000, 0x3000, CRC(9ec28ac4) SHA1(bcf48af66eed3b9e2c0fcd2413fed8167262cfba) )
	ROM_LOAD( "conquest.a",               0x10000, 0x9000, CRC(66e77fe6) SHA1(b2cab7a20ebd29ef07e2bac02eede645f740c584) )

	ROM_REGION( 0x10000, "soundcpu", 0 )
	ROM_LOAD( "video_sound_rom_1.ic12",   0xf800, 0x0800, CRC(fefd5b48) SHA1(ceb0d18483f0691978c604db94417e6941ad7ff2) )

	ROM_REGION( 0x0400, "proms", 0 )
	ROM_LOAD( "decoder.2",                0x0000, 0x0200, CRC(8dd98da5) SHA1(da979604f7a2aa8b5a6d4a5debd2e80f77569e35) )
	ROM_LOAD( "decoder.3",                0x0200, 0x0200, CRC(c3f45f70) SHA1(d19036cbc46b130548873597b44b8b70758f25c4) )
ROM_END

} // anonymous namespace

GAME( 1982, conquest, 0, williams_base,  conquest, conquest_hbmame, init_robotron, ROT270, "Vid Kidz", "Conquest (prototype)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 1981, defender2,   defender, williams_base,  stargate, williams_state, init_stargate, ROT0, "Jeff Vavasour", "Defender II", MACHINE_SUPPORTS_SAVE )
GAME( 2024, sinistar01,  sinistar, sinistar,       sinistar, sinistar_state,       empty_init,    ROT270, "Synamax", "Sinistar (balanced ver)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, splatsc1,    splat,    williams_muxed, splat,    williams_muxed_state, init_joust,    ROT0, "Williams", "Splat! (sc1 hardware)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, robotron201, robotron, williams_base,  robotron, williams_state, init_robotron, ROT0, "Sean Riddle", "Robotron: 2084 (Hacked Patched Blue-201)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, robotronp,   robotron, williams_base,  robotron, williams_state, init_robotron, ROT0, "Christian Gingras / Sean Riddle", "Robotron: 2084 (Patched)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, robotrontie, robotron, williams_base,  robotron, williams_state, init_robotron, ROT0, "Vid Kidz", "Robotron: 2084 (Tie Die)", MACHINE_SUPPORTS_SAVE )

