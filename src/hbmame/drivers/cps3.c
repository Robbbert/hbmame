// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/cps3.c"

TIMER_CALLBACK_MEMBER(cps3_state::fastboot_timer_callback)
{
	UINT32 *rom =  (UINT32*)m_decrypted_gamerom;

	m_maincpu->set_state_int(SH2_PC, rom[0]);
	m_maincpu->set_state_int(SH2_R15, rom[1]);
	m_maincpu->set_state_int(SH2_VBR, 0x6000000);
}

MACHINE_RESET_MEMBER( cps3_state, redeartn )
{
	m_current_table_address = -1;

	m_fastboot_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(cps3_state::fastboot_timer_callback),this));
	m_fastboot_timer->adjust(attotime::zero);

	// copy data from flashroms back into user regions + decrypt into regions we execute/draw from.
	copy_from_nvram();
}


MACHINE_CONFIG_DERIVED( redeartn, redearth )
	MCFG_MACHINE_RESET_OVERRIDE(cps3_state, redeartn)
MACHINE_CONFIG_END

static INPUT_PORTS_START ( cps3_ren )
	PORT_INCLUDE ( cps3 )
	PORT_MODIFY("INPUTS")
	PORT_BIT( 0x10000000, IP_ACTIVE_LOW, IPT_START1 ) PORT_NAME("P1 Start / Change Orb")
	PORT_BIT( 0x20000000, IP_ACTIVE_LOW, IPT_START2 ) PORT_NAME("P2 Start / Change Orb")

	PORT_START("REG")
	PORT_DIPNAME( 0x0000000f, 0x00000000, DEF_STR( Region ) )
	PORT_DIPSETTING( 0x00000000, "Default" )
	PORT_DIPSETTING( 0x00000001, DEF_STR( Japan ) )
	PORT_DIPSETTING( 0x00000002, DEF_STR( Asia ) )
	PORT_DIPSETTING( 0x00000003, DEF_STR( Europe ) )
	PORT_DIPSETTING( 0x00000004, DEF_STR( USA ) )
	PORT_DIPSETTING( 0x00000005, "Hispanic" )
	PORT_DIPSETTING( 0x00000006, "Brazil" )
	PORT_DIPSETTING( 0x00000007, "Oceania" )
	PORT_DIPSETTING( 0x00000008, "Asia NCD" )

	PORT_START("VER")
	PORT_DIPNAME( 0x000000f0, 0x00000060, DEF_STR( Version ) )
	PORT_DIPSETTING( 0x00000060, DEF_STR( Normal ) )
	PORT_DIPSETTING( 0x00000010, "Character Check" )
	PORT_DIPSETTING( 0x00000020, "Publicity" )
	PORT_DIPSETTING( 0x00000030, "Location Test" )
	PORT_DIPSETTING( 0x00000040, "Show" )
	PORT_DIPSETTING( 0x00000050, "???" )
INPUT_PORTS_END

ROM_START( redeartn )
	ROM_REGION32_BE( 0x080000, "bios", 0 ) /* bios region */
	// need the correct no-cd bios for this game
	//ROM_LOAD( "warzard_euro.29f400.u2", 0x000000, 0x080000, CRC(02e0f336) SHA1(acc37e830dfeb9674f5a0fb24f4cc23217ae4ff5) )
	// This one mostly works:
	//Only the enemies fight in attract mode, the good guys just stand there.
	//Some graphics glitches.
	//Could possibly be more problems ingame.
	ROM_LOAD( "sfiii_asia_nocd.29f400.u2", 0x000000, 0x080000, CRC(ca2b715f) SHA1(86319987f9af4afd272a2488e73de8382743cb37) ) // this is a different VERSION of the bios compared to all other sets, not just an alt region code

	ROM_REGION( 0x200000, "simm1.0", 0 )
	ROM_LOAD( "redeartn-simm1.0", 0x00000, 0x200000, CRC(cad468f8) SHA1(b3aa4f7d3fae84e8821417ccde9528d3eda2b7a6) )
	ROM_REGION( 0x200000, "simm1.1", 0 )
	ROM_LOAD( "redeartn-simm1.1", 0x00000, 0x200000, CRC(e9721d89) SHA1(5c63d10bdbce52d50b6dde14d4a0f1369383d656) )
	ROM_REGION( 0x200000, "simm1.2", 0 )
	ROM_LOAD( "redeartn-simm1.2", 0x00000, 0x200000, CRC(2889ec98) SHA1(a94310eb4777f908d87e9d90969db8504b4140ff) )
	ROM_REGION( 0x200000, "simm1.3", 0 )
	ROM_LOAD( "redeartn-simm1.3", 0x00000, 0x200000, CRC(5a6cd148) SHA1(d65c6e8378a91828474a16a3bbcd13c4b3b15f13) )

	ROM_REGION( 0x200000, "simm3.0", 0 )
	ROM_LOAD( "redeartn-simm3.0", 0x00000, 0x200000, CRC(83350cc5) SHA1(922b1abf80a4a89f35279b66311a7369d3965bd0) )
	ROM_REGION( 0x200000, "simm3.1", 0 )
	ROM_LOAD( "redeartn-simm3.1", 0x00000, 0x200000, CRC(56734de6) SHA1(75699fa6efe5bec335e4b02e15b3c45726b68fa8) )
	ROM_REGION( 0x200000, "simm3.2", 0 )
	ROM_LOAD( "redeartn-simm3.2", 0x00000, 0x200000, CRC(800ea0f1) SHA1(33871ab56dc1cd24441389d53e43fb8e43b149d9) )
	ROM_REGION( 0x200000, "simm3.3", 0 )
	ROM_LOAD( "redeartn-simm3.3", 0x00000, 0x200000, CRC(97e9146c) SHA1(ab7744709615081440bee72f4080d6fd5b938668) )
	ROM_REGION( 0x200000, "simm3.4", 0 )
	ROM_LOAD( "redeartn-simm3.4", 0x00000, 0x200000, CRC(0cb1d648) SHA1(7042a590c2b7ec55323062127e254da3cdc790a1) )
	ROM_REGION( 0x200000, "simm3.5", 0 )
	ROM_LOAD( "redeartn-simm3.5", 0x00000, 0x200000, CRC(7a1099f0) SHA1(c6a92ec86eb24485f1db530e0e78f647e8432231) )
	ROM_REGION( 0x200000, "simm3.6", 0 )
	ROM_LOAD( "redeartn-simm3.6", 0x00000, 0x200000, CRC(aeff8f54) SHA1(fd760e237c2e5fb2da45e32a1c12fd3defb4c3e4) )
	ROM_REGION( 0x200000, "simm3.7", 0 )
	ROM_LOAD( "redeartn-simm3.7", 0x00000, 0x200000, CRC(f770acd0) SHA1(4b3ccb6f91568f95f04ede6c574144918d131201) )
	ROM_REGION( 0x200000, "simm4.0", 0 )

	ROM_LOAD( "redeartn-simm4.0", 0x00000, 0x200000, CRC(301e56f2) SHA1(4847d971bff70a2aeed4599e1201c7ec9677da60) )
	ROM_REGION( 0x200000, "simm4.1", 0 )
	ROM_LOAD( "redeartn-simm4.1", 0x00000, 0x200000, CRC(2048e103) SHA1(b21f95b05cd99749bd3f25cc71b2671c2026847b) )
	ROM_REGION( 0x200000, "simm4.2", 0 )
	ROM_LOAD( "redeartn-simm4.2", 0x00000, 0x200000, CRC(c9433455) SHA1(63a269d76bac332c2e991d0f6a20c35e0e88680a) )
	ROM_REGION( 0x200000, "simm4.3", 0 )
	ROM_LOAD( "redeartn-simm4.3", 0x00000, 0x200000, CRC(c02171a8) SHA1(2e9228729b27a6113d9f2e42af310a834979f714) )
	ROM_REGION( 0x200000, "simm4.4", 0 )
	ROM_LOAD( "redeartn-simm4.4", 0x00000, 0x200000, CRC(2ddbf276) SHA1(b232baaa8edc8db18f8a3bdcc2d38fe984a94a34) )
	ROM_REGION( 0x200000, "simm4.5", 0 )
	ROM_LOAD( "redeartn-simm4.5", 0x00000, 0x200000, CRC(fea820a6) SHA1(55ee8ef95751f5a509fb126513e1b2a70a3414e5) )
	ROM_REGION( 0x200000, "simm4.6", 0 )
	ROM_LOAD( "redeartn-simm4.6", 0x00000, 0x200000, CRC(c7528df1) SHA1(aa312f80c2d7759d18d1aa8d416cf932b2850824) )
	ROM_REGION( 0x200000, "simm4.7", 0 )
	ROM_LOAD( "redeartn-simm4.7", 0x00000, 0x200000, CRC(2449cf3b) SHA1(c60d8042136d74e547f668ad787cae529c42eed9) )

	ROM_REGION( 0x200000, "simm5.0", 0 )
	ROM_LOAD( "redeartn-simm5.0", 0x00000, 0x200000, CRC(424451b9) SHA1(250fb92254c9e7ff5bc8dbeea5872f8a771dc9bd) )
	ROM_REGION( 0x200000, "simm5.1", 0 )
	ROM_LOAD( "redeartn-simm5.1", 0x00000, 0x200000, CRC(9b8cb56b) SHA1(2ff1081dc99bb7c2f1e036f4c112137c96b83d23) )
ROM_END

ROM_START( sfiii3an )
	ROM_REGION32_BE( 0x080000, "bios", 0 ) /* bios region */
	ROM_LOAD( "sfiii3_japan_nocd.29f400.u2", 0x000000, 0x080000, CRC(1edc6366) SHA1(60b4b9adeb030a33059d74fdf03873029e465b52) )

	ROM_REGION( 0x200000, "simm1.0", 0 )
	ROM_LOAD( "sfiii3n-simm1.0", 0x000000, 0x200000, CRC(11dfd3cd) SHA1(dba1f77c46e80317e3279298411154dfb6db2309) )
	ROM_REGION( 0x200000, "simm1.1", 0 )
	ROM_LOAD( "sfiii3n-simm1.1", 0x000000, 0x200000, CRC(c50585e6) SHA1(a289237957ea1c7f58b1c65e24c54ceb34cb1712) )
	ROM_REGION( 0x200000, "simm1.2", 0 )
	ROM_LOAD( "sfiii3n-simm1.2", 0x000000, 0x200000, CRC(8e011d9b) SHA1(e0861bcd3c4f865474d7ce47aa9eeec7b3d28da6) )
	ROM_REGION( 0x200000, "simm1.3", 0 )
	ROM_LOAD( "sfiii3n-simm1.3", 0x000000, 0x200000, CRC(dca8d92f) SHA1(7cd241641c943df446e2c75b88b5cf2d2ebf7b2e) )

	ROM_REGION( 0x200000, "simm2.0", 0 )
	ROM_LOAD( "sfiii3-simm2.0", 0x00000, 0x200000, CRC(06eb969e) SHA1(d89f6a6585b76692d57d337f0f8186398fb056da) )
	ROM_REGION( 0x200000, "simm2.1", 0 )
	ROM_LOAD( "sfiii3-simm2.1", 0x00000, 0x200000, CRC(e7039f82) SHA1(8e81e66b5a4f45ae14b070a491bde47a6a74499f) )
	ROM_REGION( 0x200000, "simm2.2", 0 )
	ROM_LOAD( "sfiii3-simm2.2", 0x00000, 0x200000, CRC(645c96f7) SHA1(06d5a54874d4bf100b776131ec9060da209ad037) )
	ROM_REGION( 0x200000, "simm2.3", 0 )
	ROM_LOAD( "sfiii3-simm2.3", 0x00000, 0x200000, CRC(610efab1) SHA1(bbc21ed6ff6220ff6017a3f02ebd9a341fbc9040) )

	ROM_REGION( 0x200000, "simm3.0", 0 )
	ROM_LOAD( "sfiii3-simm3.0", 0x00000, 0x200000, CRC(7baa1f79) SHA1(3f409df28c24dd7221966b5340d59898ea756b6f) )
	ROM_REGION( 0x200000, "simm3.1", 0 )
	ROM_LOAD( "sfiii3-simm3.1", 0x00000, 0x200000, CRC(234bf8fe) SHA1(2191781ae4d726cab28de97f27efa4a13f3bdd69) )
	ROM_REGION( 0x200000, "simm3.2", 0 )
	ROM_LOAD( "sfiii3-simm3.2", 0x00000, 0x200000, CRC(d9ebc308) SHA1(af6a0dca77e5181c9f20533a06760a782c5fd51d) )
	ROM_REGION( 0x200000, "simm3.3", 0 )
	ROM_LOAD( "sfiii3-simm3.3", 0x00000, 0x200000, CRC(293cba77) SHA1(294604cacdc24261aec4d39e489de91c41fa1758) )
	ROM_REGION( 0x200000, "simm3.4", 0 )
	ROM_LOAD( "sfiii3-simm3.4", 0x00000, 0x200000, CRC(6055e747) SHA1(3813852c5a4a5355ef739ca8f0913bbd390b984b) )
	ROM_REGION( 0x200000, "simm3.5", 0 )
	ROM_LOAD( "sfiii3-simm3.5", 0x00000, 0x200000, CRC(499aa6fc) SHA1(5b9b6eab3e99ff3e1d7c1f50b9d8bc6a81f3f8a9) )
	ROM_REGION( 0x200000, "simm3.6", 0 )
	ROM_LOAD( "sfiii3-simm3.6", 0x00000, 0x200000, CRC(6c13879e) SHA1(de189b0b8f42bc7dd89983e62bc2ecb4237b3277) )
	ROM_REGION( 0x200000, "simm3.7", 0 )
	ROM_LOAD( "sfiii3-simm3.7", 0x00000, 0x200000, CRC(cf4f8ede) SHA1(e0fb68fcb0e445f824c62fa828d6e1dcd7e3683a) )

	ROM_REGION( 0x200000, "simm4.0", 0 )
	ROM_LOAD( "sfiii3-simm4.0", 0x00000, 0x200000, CRC(091fd5ba) SHA1(3327ad7c2623c119bf728af717ea2ce3b74673a9) )
	ROM_REGION( 0x200000, "simm4.1", 0 )
	ROM_LOAD( "sfiii3-simm4.1", 0x00000, 0x200000, CRC(0bca8917) SHA1(b7b284e2f16f46d46bcfaae779b232c5b980924f) )
	ROM_REGION( 0x200000, "simm4.2", 0 )
	ROM_LOAD( "sfiii3-simm4.2", 0x00000, 0x200000, CRC(a0fd578b) SHA1(100c9db9f00ecd88d518076f5a0822e6ac3695b3) )
	ROM_REGION( 0x200000, "simm4.3", 0 )
	ROM_LOAD( "sfiii3-simm4.3", 0x00000, 0x200000, CRC(4bf8c699) SHA1(2c0b4288b5ebc5e54d9e782dfc39eb8c78fd4c21) )
	ROM_REGION( 0x200000, "simm4.4", 0 )
	ROM_LOAD( "sfiii3-simm4.4", 0x00000, 0x200000, CRC(137b8785) SHA1(56a579520a8ce2abbf36be57777f024e80474eee) )
	ROM_REGION( 0x200000, "simm4.5", 0 )
	ROM_LOAD( "sfiii3-simm4.5", 0x00000, 0x200000, CRC(4fb70671) SHA1(9aba83c18cfc099a5ce18793119bff0c2b9c777f) )
	ROM_REGION( 0x200000, "simm4.6", 0 )
	ROM_LOAD( "sfiii3-simm4.6", 0x00000, 0x200000, CRC(832374a4) SHA1(c84629e32fbf47cb7b5b4ee7555bfc2ac9b3857f) )
	ROM_REGION( 0x200000, "simm4.7", 0 )
	ROM_LOAD( "sfiii3-simm4.7", 0x00000, 0x200000, CRC(1c88576d) SHA1(0f039944d0c2305999ed5dbd351c3eb87812dc3b) )

	ROM_REGION( 0x200000, "simm5.0", 0 )
	ROM_LOAD( "sfiii3-simm5.0", 0x00000, 0x200000, CRC(c67d9190) SHA1(d265475244099d0ec153059986f3445c7bd910a3) )
	ROM_REGION( 0x200000, "simm5.1", 0 )
	ROM_LOAD( "sfiii3-simm5.1", 0x00000, 0x200000, CRC(6cb79868) SHA1(c94237f30e05bfcb2e23945530c812d9e4c73416) )
	ROM_REGION( 0x200000, "simm5.2", 0 )
	ROM_LOAD( "sfiii3-simm5.2", 0x00000, 0x200000, CRC(df69930e) SHA1(c76b7c559a1d5558138afbc796249efa2f49f6a8) )
	ROM_REGION( 0x200000, "simm5.3", 0 )
	ROM_LOAD( "sfiii3-simm5.3", 0x00000, 0x200000, CRC(333754e0) SHA1(4c18a569c26524a492ecd6f4c8b3c8e803a077d3) )
	ROM_REGION( 0x200000, "simm5.4", 0 )
	ROM_LOAD( "sfiii3-simm5.4", 0x00000, 0x200000, CRC(78f6d417) SHA1(a69577cc5399fcf0a24548661168f27f3e7e8e40) )
	ROM_REGION( 0x200000, "simm5.5", 0 )
	ROM_LOAD( "sfiii3-simm5.5", 0x00000, 0x200000, CRC(8ccad9b1) SHA1(f8bda399f87be2497b7ac39e9661f9863bf4f873) )
	ROM_REGION( 0x200000, "simm5.6", 0 )
	ROM_LOAD( "sfiii3n-simm5.6", 0x000000, 0x200000, CRC(85de59e5) SHA1(748b5c91f15777b85d8c1d35b685cd90d3185ec6) )
	ROM_REGION( 0x200000, "simm5.7", 0 )
	ROM_LOAD( "sfiii3n-simm5.7", 0x000000, 0x200000, CRC(ee7e29b3) SHA1(63dc30c6904ca2f58d229249bee5eef51fafa158) )

	ROM_REGION( 0x200000, "simm6.0", 0 )
	ROM_LOAD( "sfiii3-simm6.0", 0x00000, 0x200000, CRC(8da69042) SHA1(fd3d08295342635b2136e48d543c9350d287bb22) )
	ROM_REGION( 0x200000, "simm6.1", 0 )
	ROM_LOAD( "sfiii3-simm6.1", 0x00000, 0x200000, CRC(1c8c7ac4) SHA1(ac9f8353a4c356ef98aa7c226baba00b01f5a80f) )
	ROM_REGION( 0x200000, "simm6.2", 0 )
	ROM_LOAD( "sfiii3-simm6.2", 0x00000, 0x200000, CRC(a671341d) SHA1(636f4c04962bc1e1ddb29d2e01244b00389b234f) )
	ROM_REGION( 0x200000, "simm6.3", 0 )
	ROM_LOAD( "sfiii3-simm6.3", 0x00000, 0x200000, CRC(1a990249) SHA1(2acc639e2c0c53bf24096b8620eab090bc25d03b) )
	ROM_REGION( 0x200000, "simm6.4", 0 )
	ROM_LOAD( "sfiii3-simm6.4", 0x00000, 0x200000, CRC(20cb39ac) SHA1(7d13a0fea1ef719dd2ff77dfb547d53c6023cc9e) )
	ROM_REGION( 0x200000, "simm6.5", 0 )
	ROM_LOAD( "sfiii3-simm6.5", 0x00000, 0x200000, CRC(5f844b2f) SHA1(564e4934f89ed3b92a4c4874519f8f00f3b48696) )
	ROM_REGION( 0x200000, "simm6.6", 0 )
	ROM_LOAD( "sfiii3-simm6.6", 0x00000, 0x200000, CRC(450e8d28) SHA1(885db658132aa27926df617ec2d2a1f38abdbb60) )
	ROM_REGION( 0x200000, "simm6.7", 0 )
	ROM_LOAD( "sfiii3-simm6.7", 0x00000, 0x200000, CRC(cc5f4187) SHA1(248ddace21ed4736a56e92f77cc6ad219d7fef0b) )
ROM_END

ROM_START( sfiii4rd )
	ROM_REGION32_BE( 0x080000, "bios", 0 ) /* bios region */
	ROM_LOAD( "sfiii3_japan_nocd.29f400.u2", 0x000000, 0x080000, CRC(1edc6366) SHA1(60b4b9adeb030a33059d74fdf03873029e465b52) )

	ROM_REGION( 0x200000, "simm1.0", 0 )
	ROM_LOAD( "4rd-simm1.0", 0x00000, 0x200000, CRC(6bef26b4) SHA1(fb643d5fc5be4464374773a651ab4b5c51826047) ) // sldh
	ROM_REGION( 0x200000, "simm1.1", 0 )
	ROM_LOAD( "4rd-simm1.1", 0x00000, 0x200000, CRC(37ee88ed) SHA1(180e5ee309ad3898ac385b9add38fbec80cc1dce) ) // sldh
	ROM_REGION( 0x200000, "simm1.2", 0 )
	ROM_LOAD( "4rd-simm1.2", 0x00000, 0x200000, CRC(120139f0) SHA1(a5365ac00c75ee0e20f6c7b5a0bcee543c8582be) ) // sldh
	ROM_REGION( 0x200000, "simm1.3", 0 )
	ROM_LOAD( "4rd-simm1.3", 0x00000, 0x200000, CRC(b32e4fcc) SHA1(32bd359ca1747fa92691bd5471ba767456cc4dbb) ) // sldh

	ROM_REGION( 0x200000, "simm2.0", 0 )
	ROM_LOAD( "sfiii3-simm2.0", 0x00000, 0x200000, CRC(06eb969e) SHA1(d89f6a6585b76692d57d337f0f8186398fb056da) )
	ROM_REGION( 0x200000, "simm2.1", 0 )
	ROM_LOAD( "sfiii3-simm2.1", 0x00000, 0x200000, CRC(e7039f82) SHA1(8e81e66b5a4f45ae14b070a491bde47a6a74499f) )
	ROM_REGION( 0x200000, "simm2.2", 0 )
	ROM_LOAD( "sfiii3-simm2.2", 0x00000, 0x200000, CRC(645c96f7) SHA1(06d5a54874d4bf100b776131ec9060da209ad037) )
	ROM_REGION( 0x200000, "simm2.3", 0 )
	ROM_LOAD( "sfiii3-simm2.3", 0x00000, 0x200000, CRC(610efab1) SHA1(bbc21ed6ff6220ff6017a3f02ebd9a341fbc9040) )

	ROM_REGION( 0x200000, "simm3.0", 0 )
	ROM_LOAD( "sfiii3-simm3.0", 0x00000, 0x200000, CRC(7baa1f79) SHA1(3f409df28c24dd7221966b5340d59898ea756b6f) )
	ROM_REGION( 0x200000, "simm3.1", 0 )
	ROM_LOAD( "sfiii3-simm3.1", 0x00000, 0x200000, CRC(234bf8fe) SHA1(2191781ae4d726cab28de97f27efa4a13f3bdd69) )
	ROM_REGION( 0x200000, "simm3.2", 0 )
	ROM_LOAD( "sfiii3-simm3.2", 0x00000, 0x200000, CRC(d9ebc308) SHA1(af6a0dca77e5181c9f20533a06760a782c5fd51d) )
	ROM_REGION( 0x200000, "simm3.3", 0 )
	ROM_LOAD( "sfiii3-simm3.3", 0x00000, 0x200000, CRC(293cba77) SHA1(294604cacdc24261aec4d39e489de91c41fa1758) )
	ROM_REGION( 0x200000, "simm3.4", 0 )
	ROM_LOAD( "sfiii3-simm3.4", 0x00000, 0x200000, CRC(6055e747) SHA1(3813852c5a4a5355ef739ca8f0913bbd390b984b) )
	ROM_REGION( 0x200000, "simm3.5", 0 )
	ROM_LOAD( "sfiii3-simm3.5", 0x00000, 0x200000, CRC(499aa6fc) SHA1(5b9b6eab3e99ff3e1d7c1f50b9d8bc6a81f3f8a9) )
	ROM_REGION( 0x200000, "simm3.6", 0 )
	ROM_LOAD( "sfiii3-simm3.6", 0x00000, 0x200000, CRC(6c13879e) SHA1(de189b0b8f42bc7dd89983e62bc2ecb4237b3277) )
	ROM_REGION( 0x200000, "simm3.7", 0 )
	ROM_LOAD( "sfiii3-simm3.7", 0x00000, 0x200000, CRC(cf4f8ede) SHA1(e0fb68fcb0e445f824c62fa828d6e1dcd7e3683a) )

	ROM_REGION( 0x200000, "simm4.0", 0 )
	ROM_LOAD( "sfiii3-simm4.0", 0x00000, 0x200000, CRC(091fd5ba) SHA1(3327ad7c2623c119bf728af717ea2ce3b74673a9) )
	ROM_REGION( 0x200000, "simm4.1", 0 )
	ROM_LOAD( "sfiii3-simm4.1", 0x00000, 0x200000, CRC(0bca8917) SHA1(b7b284e2f16f46d46bcfaae779b232c5b980924f) )
	ROM_REGION( 0x200000, "simm4.2", 0 )
	ROM_LOAD( "sfiii3-simm4.2", 0x00000, 0x200000, CRC(a0fd578b) SHA1(100c9db9f00ecd88d518076f5a0822e6ac3695b3) )
	ROM_REGION( 0x200000, "simm4.3", 0 )
	ROM_LOAD( "sfiii3-simm4.3", 0x00000, 0x200000, CRC(4bf8c699) SHA1(2c0b4288b5ebc5e54d9e782dfc39eb8c78fd4c21) )
	ROM_REGION( 0x200000, "simm4.4", 0 )
	ROM_LOAD( "sfiii3-simm4.4", 0x00000, 0x200000, CRC(137b8785) SHA1(56a579520a8ce2abbf36be57777f024e80474eee) )
	ROM_REGION( 0x200000, "simm4.5", 0 )
	ROM_LOAD( "sfiii3-simm4.5", 0x00000, 0x200000, CRC(4fb70671) SHA1(9aba83c18cfc099a5ce18793119bff0c2b9c777f) )
	ROM_REGION( 0x200000, "simm4.6", 0 )
	ROM_LOAD( "sfiii3-simm4.6", 0x00000, 0x200000, CRC(832374a4) SHA1(c84629e32fbf47cb7b5b4ee7555bfc2ac9b3857f) )
	ROM_REGION( 0x200000, "simm4.7", 0 )
	ROM_LOAD( "sfiii3-simm4.7", 0x00000, 0x200000, CRC(1c88576d) SHA1(0f039944d0c2305999ed5dbd351c3eb87812dc3b) )

	ROM_REGION( 0x200000, "simm5.0", 0 )
	ROM_LOAD( "sfiii3-simm5.0", 0x00000, 0x200000, CRC(c67d9190) SHA1(d265475244099d0ec153059986f3445c7bd910a3) )
	ROM_REGION( 0x200000, "simm5.1", 0 )
	ROM_LOAD( "sfiii3-simm5.1", 0x00000, 0x200000, CRC(6cb79868) SHA1(c94237f30e05bfcb2e23945530c812d9e4c73416) )
	ROM_REGION( 0x200000, "simm5.2", 0 )
	ROM_LOAD( "sfiii3-simm5.2", 0x00000, 0x200000, CRC(df69930e) SHA1(c76b7c559a1d5558138afbc796249efa2f49f6a8) )
	ROM_REGION( 0x200000, "simm5.3", 0 )
	ROM_LOAD( "sfiii3-simm5.3", 0x00000, 0x200000, CRC(333754e0) SHA1(4c18a569c26524a492ecd6f4c8b3c8e803a077d3) )
	ROM_REGION( 0x200000, "simm5.4", 0 )
	ROM_LOAD( "sfiii3-simm5.4", 0x00000, 0x200000, CRC(78f6d417) SHA1(a69577cc5399fcf0a24548661168f27f3e7e8e40) )
	ROM_REGION( 0x200000, "simm5.5", 0 )
	ROM_LOAD( "sfiii3-simm5.5", 0x00000, 0x200000, CRC(8ccad9b1) SHA1(f8bda399f87be2497b7ac39e9661f9863bf4f873) )
	ROM_REGION( 0x200000, "simm5.6", 0 )
	ROM_LOAD( "4rd-simm5.6", 0x00000, 0x200000, CRC(e9b61a56) SHA1(9277ac5da4715aa5325ae9f8c055f1e826b4ce54) )
	ROM_REGION( 0x200000, "simm5.7", 0 )
	ROM_LOAD( "4rd-simm5.7", 0x00000, 0x200000, CRC(8db3a249) SHA1(db38963ce011535a95f11426c17fb3eeda300917) )

	ROM_REGION( 0x200000, "simm6.0", 0 )
	ROM_LOAD( "sfiii3-simm6.0", 0x00000, 0x200000, CRC(8da69042) SHA1(fd3d08295342635b2136e48d543c9350d287bb22) )
	ROM_REGION( 0x200000, "simm6.1", 0 )
	ROM_LOAD( "sfiii3-simm6.1", 0x00000, 0x200000, CRC(1c8c7ac4) SHA1(ac9f8353a4c356ef98aa7c226baba00b01f5a80f) )
	ROM_REGION( 0x200000, "simm6.2", 0 )
	ROM_LOAD( "sfiii3-simm6.2", 0x00000, 0x200000, CRC(a671341d) SHA1(636f4c04962bc1e1ddb29d2e01244b00389b234f) )
	ROM_REGION( 0x200000, "simm6.3", 0 )
	ROM_LOAD( "sfiii3-simm6.3", 0x00000, 0x200000, CRC(1a990249) SHA1(2acc639e2c0c53bf24096b8620eab090bc25d03b) )
	ROM_REGION( 0x200000, "simm6.4", 0 )
	ROM_LOAD( "sfiii3-simm6.4", 0x00000, 0x200000, CRC(20cb39ac) SHA1(7d13a0fea1ef719dd2ff77dfb547d53c6023cc9e) )
	ROM_REGION( 0x200000, "simm6.5", 0 )
	ROM_LOAD( "sfiii3-simm6.5", 0x00000, 0x200000, CRC(5f844b2f) SHA1(564e4934f89ed3b92a4c4874519f8f00f3b48696) )
	ROM_REGION( 0x200000, "simm6.6", 0 )
	ROM_LOAD( "sfiii3-simm6.6", 0x00000, 0x200000, CRC(450e8d28) SHA1(885db658132aa27926df617ec2d2a1f38abdbb60) )
	ROM_REGION( 0x200000, "simm6.7", 0 )
	ROM_LOAD( "sfiii3-simm6.7", 0x00000, 0x200000, CRC(cc5f4187) SHA1(248ddace21ed4736a56e92f77cc6ad219d7fef0b) )
ROM_END


GAME( 1996, redeartn,  redearth, redeartn, cps3_ren,  cps3_state,  redearth, ROT0, "Capcom", "War-zard (961121, NO CD)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 2013, sfiii3an,  sfiii3,   sfiii3,   cps3,      cps3_state,  sfiii3,   ROT0, "Hack",   "Street Fighter III 3rd Strike: Ford Strike (Japan 990608, NO CD)", 0 )
GAME( 2013, sfiii4rd,  sfiii3,   sfiii3,   cps3,      cps3_state,  sfiii3,   ROT0, "Hack",   "Street Fighter III 3rd Strike: 4rd Arrange Edition (Japan 990608, NO CD)", 0 )
