/*************************************
 *
 *  Make Trax input handlers
 *
 *************************************/

READ8_MEMBER(pacman_state::maketrax_special_port2_r)
{
	int data = ioport("DSW1")->read();
	int pc = space.device().safe_pcbase();

	if ((pc == 0x1973) || (pc == 0x2389)) return data | 0x40;

	switch (offset)
	{
		case 0x01:
		case 0x04:
			data |= 0x40; break;
		case 0x05:
			data |= 0xc0; break;
		default:
			data &= 0x3f; break;
	}

	return data;
}

READ8_MEMBER(pacman_state::maketrax_special_port3_r)
{
	int pc = space.device().safe_pcbase();

	if (pc == 0x040e) return 0x20;

	if ((pc == 0x115e) || (pc == 0x3ae2)) return 0x00;

	switch (offset)
	{
		case 0x00:
			return 0x1f;
		case 0x09:
			return 0x30;
		case 0x0c:
			return 0x00;
		default:
			return 0x20;
	}
}

WRITE8_MEMBER(pacman_state::piranha_interrupt_vector_w)
{
	if (data == 0xfa) data = 0x78;
	m_maincpu->set_input_line_vector(0, data );
}



/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

/*************************************
 *
 *  Main CPU port handlers
 *
 *************************************/

static ADDRESS_MAP_START( piranha_writeport, AS_IO, 8, pacman_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_WRITE(piranha_interrupt_vector_w)
ADDRESS_MAP_END


/*************************************
 *
 *  Port definitions
 *
 *************************************/

static INPUT_PORTS_START( eyes )
	PORT_START ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_SERVICE( 0x10, IP_ACTIVE_LOW )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_TILT )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START ("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_COCKTAIL

	PORT_START ("DSW1")
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x08, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x0c, "2" )
	PORT_DIPSETTING(    0x08, "3" )
	PORT_DIPSETTING(    0x04, "4" )
	PORT_DIPSETTING(    0x00, "5" )
	PORT_DIPNAME( 0x30, 0x30, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x30, "50000" )
	PORT_DIPSETTING(    0x20, "75000" )
	PORT_DIPSETTING(    0x10, "100000" )
	PORT_DIPSETTING(    0x00, "125000" )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )  /* Not accessed */
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START ("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( maketrax )
	PORT_START ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_DIPNAME( 0x10, 0x00, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Cocktail ) )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE1 )

	PORT_START ("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection */
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection */

	PORT_START ("DSW1")
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x04, "4" )
	PORT_DIPSETTING(    0x08, "5" )
	PORT_DIPSETTING(    0x0c, "6" )
	PORT_DIPNAME( 0x10, 0x10, "First Pattern" )
	PORT_DIPSETTING(    0x10, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x20, 0x20, "Teleport Holes" )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
 	PORT_BIT( 0xc0, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection */

	PORT_START ("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END



/*************************************
 *
 *  Graphics layouts
 *
 *************************************/

/*************************************
 *
 *  Machine drivers
 *
 *************************************/

static MACHINE_CONFIG_DERIVED( piranha, pacman )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_IO_MAP(piranha_writeport)
MACHINE_CONFIG_END


/*************************************
 *
 *  Driver initialization
 *
 *************************************/

void pacman_state::maketrax_rom_decode()
{
	UINT8 *rom = memregion("maincpu")->base();

	/* patch protection using a copy of the opcodes so ROM checksum */
	/* tests will not fail */

	memcpy(m_patched_opcodes,rom,0x4000);

	m_patched_opcodes[0x0415] = 0xc9;
	m_patched_opcodes[0x1978] = 0x18;
	m_patched_opcodes[0x238e] = 0xc9;
	m_patched_opcodes[0x3ae5] = 0xe6;
	m_patched_opcodes[0x3ae7] = 0x00;
	m_patched_opcodes[0x3ae8] = 0xc9;
	m_patched_opcodes[0x3aed] = 0x86;
	m_patched_opcodes[0x3aee] = 0xc0;
	m_patched_opcodes[0x3aef] = 0xb0;
}

DRIVER_INIT_MEMBER(pacman_state,maketrax)
{
	/* set up protection handlers */
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x5080, 0x50bf, read8_delegate(FUNC(pacman_state::maketrax_special_port2_r),this));
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x50c0, 0x50ff, read8_delegate(FUNC(pacman_state::maketrax_special_port3_r),this));

	maketrax_rom_decode();
}

/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( aa )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "aa.1",         0x0000, 0x1000, CRC(7b73ff28) SHA1(3b05c9ecaa418291b9b3501fbfd4a1e48be7281e) )
	ROM_LOAD( "aa.2",         0x1000, 0x1000, CRC(848ca2fa) SHA1(d11e874a0bd0dcf88ed0781d7dc7b7d98b4ac1e8) )
	ROM_LOAD( "aa.3",         0x2000, 0x1000, CRC(b3d3ff37) SHA1(0df28470eb70f4a84f24c2a86b4b9d338b9b6a76) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "aa.5e",        0x0000, 0x1000, CRC(e69596af) SHA1(b53ef6fce2d9fa1163f722a1a6be56085bde415c) )
	ROM_LOAD( "aa.5f",        0x1000, 0x1000, CRC(c26ecd63) SHA1(40d618b171c7ea164384c2ded098520c77941cbc) )

	PACMAN_PROMS
ROM_END

ROM_START( abscam )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "as0.bin",      0x0000, 0x0800, CRC(0b102302) SHA1(97f6399182db4f84efe482bf3a710aa45ca104ec) )
	ROM_LOAD( "as4.bin",      0x0800, 0x0800, CRC(3116a8ec) SHA1(259169bcc8fbe9fc73ca5100c3835a233351f530) )
	ROM_LOAD( "as1.bin",      0x1000, 0x0800, CRC(bc0281e0) SHA1(bcc6d63ede728d9b29f903489bfa80d94ec5cc00) )
	ROM_LOAD( "as5.bin",      0x1800, 0x0800, CRC(428ee2e8) SHA1(1477d1a86b32483ac0fdeea93512f517c9f66ce2) )
	ROM_LOAD( "as2.bin",      0x2000, 0x0800, CRC(e05d46ad) SHA1(87da57dbbe6ab5e1dd005fd68a982f1df917459c) )
	ROM_LOAD( "as6.bin",      0x2800, 0x0800, CRC(3ae9a8cb) SHA1(72896ad32cbdde90793788182958a943e35672f9) )
	ROM_LOAD( "as3.bin",      0x3000, 0x0800, CRC(b39eb940) SHA1(e144a1553c76ddee1c22ad1ed0cca241c2d03998) )
	ROM_LOAD( "as7.bin",      0x3800, 0x0800, CRC(16cf1c67) SHA1(0015fe64d476de87f1a030e7f2e735380dfcfd41) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "as8.bin",      0x0000, 0x0800, CRC(61daabe5) SHA1(00503916d1d1011afe68898e3416718c0e63a298) )
	ROM_LOAD( "as10.bin",     0x0800, 0x0800, CRC(81d50c98) SHA1(6b61c666f68b5948e4facb8bac1378f986f993a7) )
	ROM_LOAD( "as9.bin",      0x1000, 0x0800, CRC(a3bd1613) SHA1(c59bb0a4d1fa5cbe596f41ee7b1a4a661ab5614b) )
	ROM_LOAD( "as11.bin",     0x1800, 0x0800, CRC(9d802b68) SHA1(4e8f37c2faedcfce91221a34c14f6490d578c80a) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "as4a.bin",     0x0020, 0x0100, CRC(1605b324) SHA1(336fce22caedbe69bcba9cea2b43e00f6f8e8067) )

	PACMAN_SOUND_PROMS
ROM_END

/* This game is not complete. Should a better version emerge, it will replace this one */
/* QuadBlok works, you need to move joystick to select it, then insert coin */
/* No sound */
ROM_START( absurd )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "absurd.1",     0x0000, 0x1000, CRC(cbda6dcb) SHA1(e17628ecc06b8877c6c9e55e2c8983272dd38e1e) )
	ROM_LOAD( "absurd.2",     0x1000, 0x1000, CRC(3b669ec6) SHA1(94db6cb04f74384a2ef1830e48b9fb79e1636739) )
	ROM_LOAD( "absurd.3",     0x2000, 0x1000, CRC(beb9024b) SHA1(9fb296e6f27373a1364a7ef439e36276e19e09f6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "absurd.5e",    0x0000, 0x1000, CRC(0a4f5731) SHA1(0023b3175da4f25b5c59550c20b0435ac85cec2f) )
	ROM_LOAD( "absurd.5f",    0x1000, 0x1000, CRC(ae9805ee) SHA1(fd90b8bb2492a39f27216e0cda09f9594a9dada7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "glob.7f",      0x0000, 0x0020, CRC(1f617527) SHA1(448845cab63800a05fcb106897503d994377f78f) )
	ROM_LOAD( "absurd.4a",    0x0020, 0x0100, CRC(9379a246) SHA1(88c84668f67afc4a5ff12e794ac37a0636bd7153) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( alpaca7 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "alpaca7.6e",    0x0000, 0x1000, CRC(AF4AFCBF) SHA1(F11E2FE309818B41CB2A28408B06D18419879C09) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "alpaca.5e",     0x0000, 0x1000, CRC(3E879F02) SHA1(0B084DD449E57476231E59F15F85A209A919959C) )
	ROM_LOAD( "alpaca7.5f",    0x1000, 0x1000, CRC(A6103847) SHA1(4376996FF8C19AFD65F1757CE159B70071A4BD3B) )

	PACMAN_PROMS
ROM_END

ROM_START( alpaca8 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "alpaca8.6e",    0x0000, 0x1000, CRC(86027944) SHA1(C47FC62522A3BAE0D49F4B68C218F73C43ED19B5) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "alpaca.5e",     0x0000, 0x1000, CRC(3E879F02) SHA1(0B084DD449E57476231E59F15F85A209A919959C) )
	ROM_LOAD( "alpaca8.5f",    0x1000, 0x1000, CRC(856E53AE) SHA1(95460212107B3371600569DBD4DA482EC631ABDB) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",     0x0000, 0x0020, CRC(2FC650BD) SHA1(8D0268DEE78E47C712202B0EC4F1F51109B1F2A5) )
	ROM_LOAD( "crush.4a",      0x0020, 0x0100, CRC(2BC5D339) SHA1(446E234DF94D9EF34C3191877BB33DD775ACFDF5) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( bace )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "bace.1",       0x0000, 0x1000, CRC(8b60ff7c) SHA1(c787246f0ae080fc9088631acb923a2df37c14b2) )
	ROM_LOAD( "bace.2",       0x1000, 0x1000, CRC(25d8361a) SHA1(20792c3d4c583dfc0297c6b5db1c14194e053088) )
	ROM_LOAD( "bace.3",       0x2000, 0x1000, CRC(fc38d994) SHA1(319691caf79292f21282006337df3c637c033602) )
	ROM_LOAD( "bace.4",       0x3000, 0x1000, CRC(5853f341) SHA1(0794324ee1bab92baaa6f5bf2cd647ce10614200) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "bace.5e",      0x0000, 0x1000, CRC(6da99c7b) SHA1(dcd488e0117662caf77f453b73330740ae34ca55) )
	ROM_LOAD( "bace.5f",      0x1000, 0x1000, CRC(b81cdc64) SHA1(67f4cde661dbcdcf0192289e31ee80cbd0dc9dc5) )

	PACMAN_PROMS
ROM_END

ROM_START( crashh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "crashh.1",     0x0000, 0x1000, CRC(04353b41) SHA1(28b774fd5b429569f44651cfe4df6a9929310190) )
	ROM_LOAD( "crashh.2",     0x1000, 0x1000, CRC(e03205c0) SHA1(8c5ae42a9e2d551f9184e952723a750abc3dfb52) )
	ROM_LOAD( "crashh.3",     0x2000, 0x1000, CRC(b0fa8e46) SHA1(2caf94d1a14d96ddc401375b54bf2b1edb13c68b) )
	ROM_LOAD( "crashh.4",     0x3000, 0x1000, CRC(bfa4d2fe) SHA1(76ef449173c01f8943049330dba9c6a54a5e760a) )
	ROM_LOAD( "crashh.5",     0x8000, 0x1000, CRC(12f2f224) SHA1(95528aaef908173e1374a7c6ed1a9cda4b685a20) )
	ROM_LOAD( "pacman.6j",    0x9000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "crashh.5e",    0x0000, 0x1000, CRC(0a25969b) SHA1(2cd54477a5cc15f91ae2cba3c4270002ac15967e) )
	ROM_LOAD( "crashh.5f",    0x1000, 0x1000, CRC(447ea79c) SHA1(886d0113608616b01e4395485c6106586adcb65a) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2bc5d339) SHA1(446e234df94d9ef34c3191877bb33dd775acfdf5) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( dderby )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "dderby.6e",     0x0000, 0x1000, CRC(6f373bd4) SHA1(e2c4f9def801c5664cf2b9684a36f762e97d12d6) )
	ROM_LOAD( "dderby.6f",     0x1000, 0x1000, CRC(2fbf16bf) SHA1(9f3714ecc40707cfa4ff1942d5fe87a03888f548) )
	ROM_LOAD( "dderby.6h",     0x2000, 0x1000, CRC(6e16cd16) SHA1(7f13a2b181e4a62d7d71294cac8fe09fd9ac64a9) )
	ROM_LOAD( "dderby.6j",     0x3000, 0x1000, CRC(f7e09874) SHA1(7080aaa47e0bc893cd8bd038de7af769ef4eddf6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "dderby.5e",     0x0000, 0x1000, CRC(7e2c0a53) SHA1(c3c62b32927ac3c5948faeff28d4c2148fe18d0b) )
	ROM_LOAD( "dderby.5f",     0x1000, 0x1000, CRC(cb2dd072) SHA1(e5813cc4247b9799e9513ff1a624fabc066582e3) )

	PACMAN_PROMS
ROM_END

ROM_START( eyes )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "eyes.7d",      0x0000, 0x1000, CRC(3b09ac89) SHA1(a8f1c918da74495bb73172f39364dada38ae4713) )
	ROM_LOAD( "eyes.7f",      0x1000, 0x1000, CRC(97096855) SHA1(10d3b164bbbe5eee86e881a1434f0c114ee8adff) )
	ROM_LOAD( "eyes.7h",      0x2000, 0x1000, CRC(731e294e) SHA1(96c0308c146dbd85e244c4530af9ae8df78c86de) )
	ROM_LOAD( "eyes.7j",      0x3000, 0x1000, CRC(22f7a719) SHA1(eb000b606ecedd52bebbb232e661fb1ef205f8b0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "eyes.5d",      0x0000, 0x1000, CRC(d6af0030) SHA1(652b779533e3f00e81cc102b78d367d503b06f33) )
	ROM_LOAD( "eyes.5e",      0x1000, 0x1000, CRC(a42b5201) SHA1(2e5cede3b6039c7bd5230de27d02aaa3f35a7b64) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	PACMAN_SOUND_PROMS
ROM_END

/* E204 - Rockola presents and copyright notice are all removed. Where it should say "EYES" on title screen,
	  it has some corrupt gfx instead. The rest is fine. */
ROM_START( eyesb )	/* E204 */
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "eyesb.7d",     0x0000, 0x1000, CRC(7e7dd02d) SHA1(a0026fec57c089dd500efa107b0e884ed9edd4d6) )
	ROM_LOAD( "eyesb.7f",     0x1000, 0x1000, CRC(d6d73eb5) SHA1(b0c51afc09dd62bdda70710d57ae5b90a5e981ac) )
	ROM_LOAD( "eyesb.7h",     0x2000, 0x1000, CRC(951ad5dd) SHA1(9aed1044256070ae6555d561856d0bea454f97a8) )
	ROM_LOAD( "eyesb.7j",     0x3000, 0x1000, CRC(acc9cd8b) SHA1(d7fcf1b4b3466ee2187f82080634346a5427385e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "eyes.5d",      0x0000, 0x1000, CRC(d6af0030) SHA1(652b779533e3f00e81cc102b78d367d503b06f33) )
	ROM_LOAD( "eyes.5e",      0x1000, 0x1000, CRC(a42b5201) SHA1(2e5cede3b6039c7bd5230de27d02aaa3f35a7b64) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( kangaroh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "kangaroh.1",   0x0000, 0x1000, CRC(bcdd9dff) SHA1(0243d9aaeaa82afb04d40eee192fdd65197fb87e) )
	ROM_LOAD( "kangaroh.2",   0x1000, 0x1000, CRC(1a381ee4) SHA1(04c6ceb5baa9f94cfd7235a89adf0bc0dbc8b4d1) )
	ROM_LOAD( "kangaroh.3",   0x2000, 0x1000, CRC(d4018655) SHA1(fb1720045c7c7ab58af894a5d2b86d289a6cd32d) )
	ROM_LOAD( "kangaroh.4",   0x3000, 0x1000, CRC(6c3f60eb) SHA1(51fc5df0ea3effdff765ea20d69f9f14575794cf) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "kangaroh.5e",  0x0000, 0x1000, CRC(c2afaa18) SHA1(138321b5efca381fb6be053aa76b37fdc7ad6afb) )
	ROM_LOAD( "kangaroh.5f",  0x1000, 0x1000, CRC(dd792694) SHA1(ded7dd8e5d184ec09d25aa3429c29e9a4ead6e92) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2bc5d339) SHA1(446e234df94d9ef34c3191877bb33dd775acfdf5) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( ladybugh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",   0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazybug.2",   0x1000, 0x1000, CRC(c17a5571) SHA1(24bae96cf95f009825fd06916eeae8a06e2e35ae) )
	ROM_LOAD( "lazybug.3",   0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "ladybugh.4",  0x3000, 0x1000, CRC(4ac90450) SHA1(7777777777777777777777777777777777777777) ) /* unavailable */
	ROM_LOAD( "lazybug.5",   0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",   0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",  0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",  0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( lazybug )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",   0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazybug.2",   0x1000, 0x1000, CRC(c17a5571) SHA1(24bae96cf95f009825fd06916eeae8a06e2e35ae) )
	ROM_LOAD( "lazybug.3",   0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "lazybug.4",   0x3000, 0x1000, CRC(22a4e136) SHA1(e41e4d3cc0b3ca29ae8cb9591bf36e009c76f2b2) )
	ROM_LOAD( "lazybug.5",   0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",   0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",  0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",  0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( lazybug1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",   0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazyalt.2",   0x1000, 0x1000, CRC(3df8767b) SHA1(67ee2c2357a9a6e1f4d6ab95427d0c44dc3ebc1f) )
	ROM_LOAD( "lazybug.3",   0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "lazybug.4",   0x3000, 0x1000, CRC(22a4e136) SHA1(e41e4d3cc0b3ca29ae8cb9591bf36e009c76f2b2) )
	ROM_LOAD( "lazybug.5",   0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",   0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",  0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",  0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( mtturbo )
	ROM_REGION( 2*0x10000, "maincpu", 0 )	/* 64k for code + 64k for opcode copy to hack protection */
	ROM_LOAD( "maketrax.6e",  0x0000, 0x1000, CRC(0150fb4a) SHA1(ba41582d5432670654479b4bf6d938d2168858af) )
	ROM_LOAD( "maketrax.6f",  0x1000, 0x1000, CRC(77531691) SHA1(68a450bcc8d832368d0f1cb2815cb5c03451796e) )
	ROM_LOAD( "mtturbo.6h",   0x2000, 0x1000, CRC(77e0e153) SHA1(8be5cf8c0337e05eaf4635f19580d6c1477e6bcc) )
	ROM_LOAD( "maketrax.6j",  0x3000, 0x1000, CRC(0b4b5e0a) SHA1(621aece612df612065f776696956ef3671421fac) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "maketrax.5e",  0x0000, 0x1000, CRC(91bad2da) SHA1(096197d0cb6d55bf72b5be045224f4bd6a9cfa1b) )
	ROM_LOAD( "maketrax.5f",  0x1000, 0x1000, CRC(aea79f55) SHA1(279021e6771dfa5bd0b7c557aae44434286d91b7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( pachello )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pachello.6e",  0x0000, 0x1000, CRC(A8227949) SHA1(793B7BA579C7E4771094D281C2589435A1BF2FDA) )
	ROM_LOAD( "pachello.6f",  0x1000, 0x1000, CRC(D5690D97) SHA1(51A4D63265F1D848359CA8C78BA111D72A3BC2CF) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmatri )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacmatri.6e",  0x0000, 0x1000, CRC(29B0497E) SHA1(E45B225AABDF2F0549718885C02AE8A8EEF3BAEB) )
	ROM_LOAD( "pacmatri.6f",  0x1000, 0x1000, CRC(EAA7B145) SHA1(4C0ABF30F2C962B6EB2BDDA833236B9D58544A89) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( pactest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pactest.1",    0x0000, 0x1000, CRC(c0423406) SHA1(51630780dd7fa32a827d81a060906130fbe57345) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ptest.5e",     0x0000, 0x1000, CRC(a650e73e) SHA1(e51df8e976259ea8b431dd13b6bc4728ba213d19) )
	ROM_LOAD( "ptest.5f",     0x1000, 0x1000, CRC(51aa0303) SHA1(59fdfc5393e883d65bf00f8b3bbfb7173440dec3) )

	PACMAN_PROMS
ROM_END

ROM_START( piranha )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "pir1.bin",     0x0000, 0x0800, CRC(69a3e6ea) SHA1(c54e5d039a03d3cbee7a5e21bf1e23f4fd913ea6) )
	ROM_LOAD( "pir5.bin",     0x0800, 0x0800, CRC(245e753f) SHA1(4c1183b8449e4e7995f81079953fe0e251251c60) )
	ROM_LOAD( "pir2.bin",     0x1000, 0x0800, CRC(62cb6954) SHA1(0e01c8463b130ab5518ce23368ad028c86cd0a32) )
	ROM_LOAD( "pir6.bin",     0x1800, 0x0800, CRC(cb0700bc) SHA1(1f5e91791ea25eb58d26b9627e98e0b6c1d9becf) )
	ROM_LOAD( "pir3.bin",     0x2000, 0x0800, CRC(843fbfe5) SHA1(6671a3c55ef70447f2a127438e0c39857f8bf6b1) )
	ROM_LOAD( "pir7.bin",     0x2800, 0x0800, CRC(73084d5e) SHA1(cb04a4c9dbf1672ddf478d2fe92b0ffd0159bb9e) )
	ROM_LOAD( "pir4.bin",     0x3000, 0x0800, CRC(4cdf6704) SHA1(97af8bbd08896dffd73e359ec46843dd673c4c9c) )
	ROM_LOAD( "pir8.bin",     0x3800, 0x0800, CRC(b86fedb3) SHA1(f5eaf7ccc1ecaa2417bcc077561efca8e7cb691a) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "pir9.bin",     0x0000, 0x0800, CRC(0f19eb28) SHA1(0335189a06be01b97ca376d3682ed54df9b121e8) )
	ROM_LOAD( "pir11.bin",    0x0800, 0x0800, CRC(5f8bdabe) SHA1(eb6a0515a381a885b087d165aaefb0277a223715) )
	ROM_LOAD( "pir10.bin",    0x1000, 0x0800, CRC(d19399fb) SHA1(c0a75a08f77adb9d0010511c4b6ea99324c33c50) )
	ROM_LOAD( "pir12.bin",    0x1800, 0x0800, CRC(cfb4403d) SHA1(1642a4917be0621ebf5f705c7f68a2b75d1c78d3) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "piranha.4a",   0x0020, 0x0100, CRC(08c9447b) SHA1(5e4fbfcc7179fc4b1436af9bb709ffc381479315) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( piranhah )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pr1.cpu",      0x0000, 0x1000, CRC(bc5ad024) SHA1(a3ed781b514a1068b24a7146a28f0a2adfaa2719) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pr3.cpu",      0x2000, 0x1000, CRC(473c379d) SHA1(6e7985367c3e544b4cb98ba8291908df88eafe7f) )
	ROM_LOAD( "pr4.cpu",      0x3000, 0x1000, CRC(63fbf895) SHA1(d328bf3b8f307fb774614834edec211117148e64) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pr5.cpu",      0x0000, 0x0800, CRC(3fc4030c) SHA1(5e45f0c19cf96daa17afd2fa1c628d7ac7f4a79c) )
	ROM_LOAD( "pr7.cpu",      0x0800, 0x0800, CRC(30b9a010) SHA1(b0ba8b6cd430feb32d11d092e1959b9f5d240f1b) )
	ROM_LOAD( "pr6.cpu",      0x1000, 0x0800, CRC(f3e9c9d5) SHA1(709a75b2457f21f0f1a3d9e7f4c8579468ee5cad) )
	ROM_LOAD( "pr8.cpu",      0x1800, 0x0800, CRC(133d720d) SHA1(8af75ed9e115a996379acedd44d0c09332ec5a03) )

	PACMAN_PROMS
ROM_END

ROM_START( piranhao )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "p1.bin",       0x0000, 0x0800, CRC(c6ce1bfc) SHA1(da145d67331cee292654a185fb09e773dd9d40cd) )
	ROM_LOAD( "p5.bin",       0x0800, 0x0800, CRC(a2655a33) SHA1(2253dcf5c8cbe278118aa1569cf456b13d8cf029) )
	ROM_LOAD( "pir2.bin",     0x1000, 0x0800, CRC(62cb6954) SHA1(0e01c8463b130ab5518ce23368ad028c86cd0a32) )
	ROM_LOAD( "pir6.bin",     0x1800, 0x0800, CRC(cb0700bc) SHA1(1f5e91791ea25eb58d26b9627e98e0b6c1d9becf) )
	ROM_LOAD( "pir3.bin",     0x2000, 0x0800, CRC(843fbfe5) SHA1(6671a3c55ef70447f2a127438e0c39857f8bf6b1) )
	ROM_LOAD( "pir7.bin",     0x2800, 0x0800, CRC(73084d5e) SHA1(cb04a4c9dbf1672ddf478d2fe92b0ffd0159bb9e) )
	ROM_LOAD( "p4.bin",       0x3000, 0x0800, CRC(9363a4d1) SHA1(4cb4a86d92a1f9bf233cac01aa266485a8bb7a34) )
	ROM_LOAD( "p8.bin",       0x3800, 0x0800, CRC(2769979c) SHA1(581592da26199b325de51791ddab66b474ab0413) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "p9.bin",       0x0000, 0x0800, CRC(94eb7563) SHA1(c99741ce1aebdfb89628fbfaecf5ae6b2719a0ca) )
	ROM_LOAD( "p11.bin",      0x0800, 0x0800, CRC(a3606973) SHA1(72297e1a33102c6a48b4c65f2a0b9bfc75a2df36) )
	ROM_LOAD( "p10.bin",      0x1000, 0x0800, CRC(84165a2c) SHA1(95b24620fbf9bd0ec4dd2aeeb6d9305bd475dce2) )
	ROM_LOAD( "p12.bin",      0x1800, 0x0800, CRC(2699ba9e) SHA1(b91ff586defe65b200bea5ade7374c2c7579cd80) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "piranha.4a",   0x0020, 0x0100, CRC(08c9447b) SHA1(5e4fbfcc7179fc4b1436af9bb709ffc381479315) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( ppong )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ppong.1",      0x0000, 0x1000, CRC(abf90af3) SHA1(785059f68b69d031e8017fc5c6f44d22208ba1d1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ppong.5e",     0x0000, 0x1000, CRC(57fe0ace) SHA1(87b9e980c8c30b73ebf7c2750dd40806754bed51) )
	ROM_LOAD( "ppong.5f",     0x1000, 0x1000, CRC(8bbd8de5) SHA1(feec6c9d413c6767edf2a521da439b21ccdf3d9a) )

	PACMAN_PROMS
ROM_END

ROM_START( ppong2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ppong2.1",     0x0000, 0x1000, CRC(b0d6d512) SHA1(1d1ccc0cf0436680fc3b2bbd1a77292506b7514b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ppong.5e",     0x0000, 0x1000, CRC(57fe0ace) SHA1(87b9e980c8c30b73ebf7c2750dd40806754bed51) )
	ROM_LOAD( "ppong.5f",     0x1000, 0x1000, CRC(8bbd8de5) SHA1(feec6c9d413c6767edf2a521da439b21ccdf3d9a) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "ppong2.7f",    0x0000, 0x0020, CRC(e3a96194) SHA1(6eb54a64e9d1df6052fecb754852bd44dd9ed69e) )
	ROM_LOAD( "ppong2.4a",    0x0020, 0x0100, CRC(dbd17cc2) SHA1(072e20975ae0453df57cfc54c344238aa311c373) )

	ROM_REGION( 0x0200, "namco", 0 )	/* sound PROMs */
	ROM_LOAD( "ppong2.1m",    0x0000, 0x0100, CRC(0d968558) SHA1(b376885ac8452b6cbf9ced81b1080bfd570d9b91) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )	/* timing - not used */
ROM_END

ROM_START( rainboh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "rainboh.1",    0x0000, 0x1000, CRC(676b742e) SHA1(0ec4bd459533f3f66782d0b978a0972c66b69ed4) )
	ROM_LOAD( "rainboh.2",    0x1000, 0x1000, CRC(3c9f1ada) SHA1(8951170cf75e5397136193ed69d920d2b9744f29) )
	ROM_LOAD( "rainboh.3",    0x2000, 0x1000, CRC(fbad5604) SHA1(4cdc2a87712471ed2bba7dc215b584e9e2c37c39) )
	ROM_LOAD( "rainboh.4",    0x3000, 0x1000, CRC(1af32a9b) SHA1(9830816c11342b40c056d555d6208ef1c6537557) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "rainboh.5e",   0x0000, 0x1000, CRC(c3292da2) SHA1(11d2b8cb36dfa86c08f6072e603f18074d56c2c2) )
	ROM_LOAD( "rainboh.5f",   0x1000, 0x1000, CRC(2b7455d8) SHA1(9decb7671a97e7fd248cc05798119d82b7830097) )

	PACMAN_PROMS
ROM_END

ROM_START( seq1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "seq1.6e",      0x0000, 0x1000, CRC(7063B724) SHA1(3A291D26BDFBF5C895D5F6AA70FC164299E8D9F1) )
	ROM_LOAD( "seq1.6f",      0x1000, 0x1000, CRC(95DB4723) SHA1(A0C47EA05E14CEBC6493705CD2D46D1E3D12B23A) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",	  0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( seq2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "seq2.6e",      0x0000, 0x1000, CRC(0EC135BE) SHA1(F2BA6F8EE4256420E6149853A34A2B11A3545EEA) )
	ROM_LOAD( "seq2.6f",      0x1000, 0x1000, CRC(1B95F321) SHA1(F9D76544CCE43CE779A1BF59A01AC00297BAA82C) )
	ROM_LOAD( "seq2.6h",      0x2000, 0x1000, CRC(5892650A) SHA1(7C63F42EA3F75FAA15C6F5864569CB6A8F8C7C79) )
	ROM_LOAD( "seq2.6j",      0x3000, 0x1000, CRC(8612016A) SHA1(4E144069F194D240E503B24DF9B10BCDE68B2C6C) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

/* note: the original rom was only 3154 bytes with a CRC of 0E67D062 - the one here is padded out to normal size */
ROM_START( tst_pacm )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "test.6e",      0x0000, 0x1000, CRC(fb645998) SHA1(ff57155533e1ec0fdc5c407dce8d270e86d33882) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( wavybug )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",    0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazybug.2",    0x1000, 0x1000, CRC(c17a5571) SHA1(24bae96cf95f009825fd06916eeae8a06e2e35ae) )
	ROM_LOAD( "lazybug.3",    0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "wavybug.4",    0x3000, 0x1000, CRC(830c47fc) SHA1(bc802de8c89417c4df91c26c2f365e471ea5efaf) )
	ROM_LOAD( "lazybug.5",    0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",    0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",   0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",   0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( zap )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zap.1",        0x0000, 0x1000, CRC(74e6e3a0) SHA1(3d953bfd57aa56f8fe61d1abd01db84d027df3bb) )
	ROM_LOAD( "zap.2",        0x1000, 0x1000, CRC(40e1884b) SHA1(ce303b6a6c9e765c256997c19f64e9ebfd8f435e) )
	ROM_LOAD( "blank004k.3",  0x2000, 0x1000, CRC(c71c0011) SHA1(1ceaf73df40e531df3bfb26b4fb7cd95fb7bff1d) )
	ROM_LOAD( "zap.4",        0x3000, 0x1000, CRC(17c357e6) SHA1(6486f58592b784d53a5c14f59b25c8cd513b2a0d) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "zap.5e",       0x0000, 0x1000, CRC(a0578404) SHA1(329f3e869eecff96191642e54dbfc7b6b268f026) )
	ROM_LOAD( "zap.5f",       0x1000, 0x1000, CRC(039b5c78) SHA1(36a96c996f20d1b5fb4f674393a03d10cdf4a091) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2bc5d339) SHA1(446e234df94d9ef34c3191877bb33dd775acfdf5) )

	PACMAN_SOUND_PROMS
ROM_END

/*************************************
 *
 *  Game drivers
 *
 *************************************/

/* Official MAME */

/*          rom       parent    machine   inp       init */
GAME( 1981, abscam,   puckman,  piranha,  mspacman, pacman_state,  eyes,     ROT90,  "GL (US Billiards License)", "Abscam", GAME_SUPPORTS_SAVE )
GAME( 1982, eyes,     0,        pacman,   eyes,     pacman_state,  eyes,     ROT90,  "Digitrex Techstar (Rock-ola license)", "Eyes (Digitrex Techstar)", GAME_SUPPORTS_SAVE )
GAME( 1981, piranha,  puckman,  piranha,  mspacman, pacman_state,  eyes,     ROT90,  "GL (US Billiards License)", "Piranha", GAME_SUPPORTS_SAVE )
GAME( 1981, piranhah, puckman,  pacman,   mspacman, driver_device, 0,        ROT90,  "hack", "Piranha (hack)", GAME_SUPPORTS_SAVE )
GAME( 1981, piranhao, puckman,  piranha,  mspacman, pacman_state,  eyes,     ROT90,  "GL (US Billiards License)", "Piranha (older)", GAME_SUPPORTS_SAVE )


/* Dave Widel's Games - http://www.widel.com */

GAME( 2003, aa,       0, 	widel,    mspacpls, driver_device, 0,        ROT90, "David Widel", "Alien Armada", GAME_SUPPORTS_SAVE )
GAME( 2003, bace,     0, 	widel,    mspacpls, driver_device, 0,        ROT90, "David Widel", "Balloon Ace", GAME_SUPPORTS_SAVE )
GAME( 2003, dderby,   0, 	widel,    mspacpls, driver_device, 0,	     ROT90, "David Widel", "Death Derby", GAME_SUPPORTS_SAVE )
GAME( 2003, kangaroh, 0,        woodpek,  mspacpls, driver_device, 0,	     ROT90, "David Widel", "Kagaroo (Qbertish) (incomplete)", GAME_SUPPORTS_SAVE )
GAME( 2003, ladybugh, lazybug,  woodpek,  mspacpls, driver_device, 0,        ROT90, "David Widel", "Ladybug on Pacman Hardware", GAME_SUPPORTS_SAVE )
GAME( 1981, lazybug,  0,        woodpek,  mspacpls, driver_device, 0,        ROT90, "David Widel", "Lazy Bug", GAME_SUPPORTS_SAVE )
GAME( 1981, lazybug1, lazybug,  woodpek,  mspacpls, driver_device, 0,        ROT90, "David Widel", "Lazy Bug (Slower)", GAME_SUPPORTS_SAVE )
GAME( 19??, pactest,  0,        woodpek,  mspacpls, driver_device, 0,        ROT90, "David Widel", "Test - Pacman board test", GAME_SUPPORTS_SAVE )
GAME( 2003, ppong,    0,        woodpek,  mspacpls, driver_device, 0,	     ROT90, "David Widel", "Pong (Pacman Hardware)", GAME_SUPPORTS_SAVE )
GAME( 2003, ppong2,   ppong,    woodpek,  mspacpls, driver_device, 0,	     ROT90, "David Widel", "Pong (Pacman Hardware) v2", GAME_SUPPORTS_SAVE )
GAME( 2003, rainboh,  0,        pacman,   mspacpls, driver_device, 0,	     ROT90, "David Widel", "Rainbow (Incomplete)", GAME_SUPPORTS_SAVE )
GAME( 1981, wavybug,  lazybug,  woodpek,  mspacpls, driver_device, 0,        ROT90, "David Widel", "Wavy Bug", GAME_SUPPORTS_SAVE )
GAME( 2003, zap,      0,        woodpek,  mspacpls, driver_device, 0,	     ROT90, "David Widel", "Space Zap Tribute", GAME_SUPPORTS_SAVE )

/* Jerronimo's Progs - www.umlautllama.com */

GAME( 2006, absurd,   0,        pacman,   mspacman, driver_device, 0,        ROT90, "Scott Lawrence", "Absurd!/QuadBlok (non-working alpha ver 3)", GAME_SUPPORTS_SAVE )
GAME( 2003, alpaca7,  alpaca8, 	pacman,   pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "Alpaca v0.7 (Pacman Hardware)", GAME_SUPPORTS_SAVE )
GAME( 2003, alpaca8,  0,        pacman,   pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "Alpaca v0.8 (Pacman Hardware)", GAME_SUPPORTS_SAVE )
GAME( 2001, pachello, 0, 	pachack,  pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "Hello, World!", GAME_SUPPORTS_SAVE )
GAME( 2001, pacmatri, 0, 	pachack,  pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "Matrix Effect", GAME_SUPPORTS_SAVE )
GAME( 2003, seq1,     0, 	pachack,  pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "16 Step Simple Sequencer", GAME_SUPPORTS_SAVE )
GAME( 2003, seq2,     seq1, 	pachack,  pacman0,  driver_device, 0,        ROT90, "Scott Lawrence", "Sequencer and Music Player", GAME_SUPPORTS_SAVE )

/* Other Misc Hacks */

GAME( 2002, crashh,   0,        woodpek,  mspacpls, driver_device, 0,        ROT90, "hack", "Crash", GAME_SUPPORTS_SAVE )
GAME( 1981, mtturbo,  0,	pacmanp,  maketrax, pacman_state,  maketrax, ROT270,"Tim Arcadecollecting", "Make Trax (Turbo Hack)", GAME_SUPPORTS_SAVE ) // http://www.arcadecollecting.com/hacks/maketrax
GAME( 1999, tst_pacm, 0,        pacman,   mspacpls, driver_device, 0,        ROT90, "David Caldwell", "Test - Pacman Hardware", GAME_SUPPORTS_SAVE ) // http://www.porkrind.org/arcade/

/* Other misc unemulated dumps */

GAME( 1982, eyesb,    eyes,     pacman,   eyes,     pacman_state,  eyes,     ROT90,  "bootleg", "Eyes (unknown bootleg)", GAME_SUPPORTS_SAVE )


/*************************************************************************************************************************/
