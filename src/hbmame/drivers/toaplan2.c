// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/toaplan2.c"

ROM_START( kgpe )
	ROM_REGION( 0x100000, "maincpu", 0 )			/* Main 68K code */
	ROM_LOAD16_BYTE( "ma02rom1.bin", 0x000000, 0x080000, CRC(a678b149) SHA1(8c1a631e023dbba0a3fa6cd1b7d10dec1663213a) )
	ROM_LOAD16_BYTE( "ma02rom0.bin", 0x000001, 0x080000, CRC(f226a212) SHA1(526acf3d05fdc88054a772fbea3de2af532bf3d2) )

	ROM_REGION( 0x10000, "audiocpu", 0 )			/* Sound Z80 code */
	ROM_LOAD( "ma02rom2.bin", 0x00000, 0x10000, CRC(dde8a57e) SHA1(f522a3f17e229c71512464349760a9e27778bf6a) )

	ROM_REGION( 0x400000, "gp9001", 0 )
	ROM_LOAD( "ma02rom3.bin",  0x000000, 0x200000, CRC(0e797142) SHA1(a480ccd151e49b886d3175a6deff56e1f2c26c3e) )
	ROM_LOAD( "ma02rom4.bin",  0x200000, 0x200000, CRC(72a6fa53) SHA1(ce92e65205b84361cfb90305a61e9541b5c4dc2f) )

	ROM_REGION( 0x008000, "text", 0 )
	ROM_LOAD( "kgpe.05",  0x000000, 0x008000, CRC(a0707702) SHA1(b6105291c1ae3d0792e69fd2495c31b56fbf61b7) )

	ROM_REGION( 0x80000, "oki", 0 )			/* ADPCM Samples */
	ROM_LOAD( "ma02rom6.bin", 0x00000, 0x80000, CRC(199e7cae) SHA1(0f5e13cc8ec42c80bb4bbff90aba29cdb15213d4) )
ROM_END

GAME( 1994, kgpe, 0, shippumd, kingdmgp, driver_device, 0, ROT270, "Neil Corlett", "Kingdom Grand Prix" , MACHINE_SUPPORTS_SAVE )

/* This game can be obtained from the Raine site, use a hex editor to zero out the last 4 bytes,
     and rename the result to "kgpe.05". Thanks goes to the Raine devs for the idea and the rom,
     and Tourniquet for the byte modification. Without this mod, the rom has the same crc as "ma02rom5.eng"
     from "kingdmgp", which caused all sorts of confusion and problems. */
