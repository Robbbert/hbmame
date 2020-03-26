// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/r2dx_v33.cpp"

//PSmame (c) gaston90 used with permission

 /********************************************
      Proyecto Shadows Mame Build Plus
**********************************************/

/**************
 New Zero Team
***************/

ROM_START( nzeroteamas01 ) //nzeroteama, nzeroteamp
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE("prg1_ps1", 0x000000, 0x80000, CRC(cc45b0c6) SHA1(3d4212512b5fa33f543e1a94f7c6b33fe9562fb3) )
	ROM_LOAD16_BYTE("prg2_ps1", 0x000001, 0x80000, CRC(8537b97d) SHA1(5cb89ab54e9830a525bf2b68309b342fd577d2ca) )

	ROM_REGION( 0x20000, "math", 0 )
	ROM_LOAD( "copx-d3.bin", 0x00000, 0x20000, CRC(fa2cf3ad) SHA1(13eee40704d3333874b6e3da9ee7d969c6dc662a) )

	ROM_REGION( 0x20000, "audiocpu", 0 )
	ROM_LOAD( "sound",       0x000000, 0x08000, CRC(7ec1fbc3) SHA1(48299d6530f641b18764cc49e283c347d0918a47) )
	ROM_CONTINUE(            0x010000, 0x08000 )
	ROM_COPY( "audiocpu", 0x0000, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 )
	ROM_LOAD16_BYTE( "fix1", 0x000000, 0x010000, CRC(0c4895b0) SHA1(f595dbe5a19edb8a06ea60105ee26b95db4a2619) )
	ROM_LOAD16_BYTE( "fix2", 0x000001, 0x010000, CRC(07d8e387) SHA1(52f54a6a4830592784cdf643a5f255aa3db53e50) )

	ROM_REGION( 0x400000, "gfx2", 0 )
	ROM_LOAD( "back-1", 0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "back-2", 0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION32_LE( 0x800000, "gfx3", 0 )
	ROM_LOAD32_WORD( "obj-1", 0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "obj-2", 0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 ) 
	ROM_LOAD( "6.pcm", 0x00000, 0x40000, CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) )

	ROM_REGION( 0x200, "pld", 0 )
	ROM_LOAD( "sysv33b-1.u0222.bin", 0x000, 0x117, CRC(f514a11f) SHA1(dd83ee1f511915d3d5f65375f34583be7fa1158b) )
	ROM_LOAD( "sysv33b-2.u0227.bin", 0x000, 0x117, CRC(d9f4612f) SHA1(0c507b28dc0f50a67cc12d63092067dc3f7f4679) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// New Zero Team
GAME( 2018, nzeroteamas01,   zeroteam, nzerotea, nzerotea, r2dx_v33_state, init_nzerotea,  ROT0,   "hack",  "New Zero Team (Special Forces 2018-02-19)", MACHINE_SUPPORTS_SAVE)
