// Proyecto De Inicio:"2016/2017"
// Derechos Del Autor:Gaston90
#include "../mame/drivers/tecmosys.cpp"

/***************************************
	Proyecto Shadows Mame Build Plus
***************************************/

/******************************************************************************************
   Muchos Roms De Mi Proyecto Shadow Se Estará Deshabilitado. Por
   Los Motivo Es Que Se Empezó Agregar En El HBMame Oficial, Lo Cual Me Veré
   Obligado Deshabilitando Porque Si No Se Estaría Duplicado Archivo De Roms.
******************************************************************************************/

/*****************************************
 *
 *         ROM Definición(s)
 *
 ****************************************/
 
  /****************************************
        1943 Kai Midway Kaisen
*****************************************/

ROM_START( tkdenshos01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "aeprge-2hc01.pal", 0x00000, 0x80000, CRC(56ce787e) SHA1(3fee1acd962246b7f15fc9f578702207836a8e54) )
	ROM_LOAD16_BYTE( "aeprgo-2hc01.pau", 0x00001, 0x80000, CRC(dc0e29f2) SHA1(7234faeda7cc625bf5eb4d75f8524f8dc9b55c6b) )

	ROM_REGION( 0x040000, "audiocpu", 0 ) // Sound Program
	ROM_LOAD( "aesprg-2.z1", 0x000000, 0x020000, CRC(43550ab6) SHA1(2580129ef8ebd9295249175de4ba985c752e06fe) )
	ROM_RELOAD(              0x020000, 0x020000) // for banks

	ROM_REGION( 0x2200, "cpu2", 0 ) // MCU is a 68HC11A8 with 8k ROM, 512 bytes EEPROM
	ROM_LOAD( "tkdensho_68hc11a8.rom",    0x0000, 0x2000, NO_DUMP )
	ROM_LOAD( "tkdensho_68hc11a8.eeprom", 0x2000, 0x0200, NO_DUMP )

	ROM_REGION( 0x4000000, "gfx1", ROMREGION_ERASE00 ) // Graphics - mostly (maybe all?) not tile based
	ROM_LOAD16_BYTE( "ae100h.ah1",    0x0000000, 0x0400000, CRC(06be252b) SHA1(08d1bb569fd2e66e2c2f47da7780b31945232e62) )
	ROM_LOAD16_BYTE( "ae100.al1",     0x0000001, 0x0400000, CRC(009cdff4) SHA1(fd88f07313d14fd4429b09a1e8d6b595df3b98e5) )
	ROM_LOAD16_BYTE( "ae101h.bh1",    0x0800000, 0x0400000, CRC(f2469eff) SHA1(ba49d15cc7949437ba9f56d9b425a5f0e62137df) )
	ROM_LOAD16_BYTE( "ae101.bl1",     0x0800001, 0x0400000, CRC(db7791bb) SHA1(1fe40b747b7cee7a9200683192b1d60a735a0446) )
	ROM_LOAD16_BYTE( "ae102h.ch1",    0x1000000, 0x0200000, CRC(f9d2a343) SHA1(d141ac0b20be587e77a576ef78f15d269d9c84e5) )
	ROM_LOAD16_BYTE( "ae102.cl1",     0x1000001, 0x0200000, CRC(681be889) SHA1(8044ca7cbb325e6dcadb409f91e0c01b88a1bca7) )
	ROM_LOAD16_BYTE( "ae104.el1",     0x2000001, 0x0400000, CRC(e431b798) SHA1(c2c24d4f395bba8c78a45ecf44009a830551e856) )
	ROM_LOAD16_BYTE( "ae105.fl1",     0x2800001, 0x0400000, CRC(b7f9ebc1) SHA1(987f664072b43a578b39fa6132aaaccc5fe5bfc2) )
	ROM_LOAD16_BYTE( "ae106.gl1",     0x3000001, 0x0200000, CRC(7c50374b) SHA1(40865913125230122072bb13f46fb5fb60c088ea) )

	ROM_REGION( 0x080000, "gfx2", 0 ) // 8x8 4bpp tiles
	ROM_LOAD( "ae300w36.bd1",  0x000000, 0x0080000, CRC(e829f29e) SHA1(e56bfe2669ed1d1ae394c644def426db129d97e3) )

	ROM_REGION( 0x100000, "gfx3", 0 ) // 16x16 4bpp tiles
	ROM_LOAD( "ae200w74.ba1",  0x000000, 0x0100000, CRC(c1645041) SHA1(323670a6aa2a4524eb968cc0b4d688098ffeeb12) )

	ROM_REGION( 0x100000, "gfx4", 0 ) // 16x16 4bpp tiles
	ROM_LOAD( "ae201w75.bb1",  0x000000, 0x0100000, CRC(3f63bdff) SHA1(0d3d57fdc0ec4bceef27c11403b3631d23abadbf) )

	ROM_REGION( 0x100000, "gfx5", 0 ) // 16x16 4bpp tiles
	ROM_LOAD( "ae202w76.bc1",  0x000000, 0x0100000, CRC(5cc857ca) SHA1(2553fb5220433acc15dfb726dc064fe333e51d88) )

	ROM_REGION( 0x400000, "ymz", 0 ) // YMZ280B Samples
	ROM_LOAD( "ae400t23.ya1", 0x000000, 0x200000, CRC(c6ffb043) SHA1(e0c6c5f6b840f63c9a685a2c3be66efa4935cbeb) )
	ROM_LOAD( "ae401t24.yb1", 0x200000, 0x200000, CRC(d83f1a73) SHA1(412b7ac9ff09a984c28b7d195330d78c4aac3dc5) )

	ROM_REGION( 0x100000, "oki", 0 ) // M6295 Samples
	ROM_LOAD( "ae500w07.ad1", 0x080000, 0x080000, CRC(3734f92c) SHA1(048555b5aa89eaf983305c439ba08d32b4a1bb80) )
ROM_END

ROM_START( tkdenshoas01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "aeprgehc01.pal", 0x00000, 0x80000, CRC(56ce787e) SHA1(3fee1acd962246b7f15fc9f578702207836a8e54) )
	ROM_LOAD16_BYTE( "aeprgohc01.pau", 0x00001, 0x80000, CRC(dc0e29f2) SHA1(7234faeda7cc625bf5eb4d75f8524f8dc9b55c6b) )

	ROM_REGION( 0x040000, "audiocpu", 0 ) // Sound Program
	ROM_LOAD( "aesprg-2.z1", 0x000000, 0x020000, CRC(43550ab6) SHA1(2580129ef8ebd9295249175de4ba985c752e06fe) )
	ROM_RELOAD(              0x020000, 0x020000) // for banks

	ROM_REGION( 0x2200, "cpu2", 0 ) // MCU is a 68HC11A8 with 8k ROM, 512 bytes EEPROM
	ROM_LOAD( "tkdensho_68hc11a8.rom",    0x0000, 0x2000, NO_DUMP )
	ROM_LOAD( "tkdensho_68hc11a8.eeprom", 0x2000, 0x0200, NO_DUMP )

	ROM_REGION( 0x4000000, "gfx1", ROMREGION_ERASE00 ) // Graphics - mostly (maybe all?) not tile based
	ROM_LOAD16_BYTE( "ae100h.ah1",    0x0000000, 0x0400000, CRC(06be252b) SHA1(08d1bb569fd2e66e2c2f47da7780b31945232e62) )
	ROM_LOAD16_BYTE( "ae100.al1",     0x0000001, 0x0400000, CRC(009cdff4) SHA1(fd88f07313d14fd4429b09a1e8d6b595df3b98e5) )
	ROM_LOAD16_BYTE( "ae101h.bh1",    0x0800000, 0x0400000, CRC(f2469eff) SHA1(ba49d15cc7949437ba9f56d9b425a5f0e62137df) )
	ROM_LOAD16_BYTE( "ae101.bl1",     0x0800001, 0x0400000, CRC(db7791bb) SHA1(1fe40b747b7cee7a9200683192b1d60a735a0446) )
	ROM_LOAD16_BYTE( "ae102h.ch1",    0x1000000, 0x0200000, CRC(f9d2a343) SHA1(d141ac0b20be587e77a576ef78f15d269d9c84e5) )
	ROM_LOAD16_BYTE( "ae102.cl1",     0x1000001, 0x0200000, CRC(681be889) SHA1(8044ca7cbb325e6dcadb409f91e0c01b88a1bca7) )
	ROM_LOAD16_BYTE( "ae104.el1",     0x2000001, 0x0400000, CRC(e431b798) SHA1(c2c24d4f395bba8c78a45ecf44009a830551e856) )
	ROM_LOAD16_BYTE( "ae105.fl1",     0x2800001, 0x0400000, CRC(b7f9ebc1) SHA1(987f664072b43a578b39fa6132aaaccc5fe5bfc2) )
	ROM_LOAD16_BYTE( "ae106.gl1",     0x3000001, 0x0200000, CRC(7c50374b) SHA1(40865913125230122072bb13f46fb5fb60c088ea) )

	ROM_REGION( 0x080000, "gfx2", 0 ) // 8x8 4bpp tiles
	ROM_LOAD( "ae300w36.bd1",  0x000000, 0x0080000, CRC(e829f29e) SHA1(e56bfe2669ed1d1ae394c644def426db129d97e3) )

	ROM_REGION( 0x100000, "gfx3", 0 ) // 16x16 4bpp tiles
	ROM_LOAD( "ae200w74.ba1",  0x000000, 0x0100000, CRC(c1645041) SHA1(323670a6aa2a4524eb968cc0b4d688098ffeeb12) )

	ROM_REGION( 0x100000, "gfx4", 0 ) // 16x16 4bpp tiles
	ROM_LOAD( "ae201w75.bb1",  0x000000, 0x0100000, CRC(3f63bdff) SHA1(0d3d57fdc0ec4bceef27c11403b3631d23abadbf) )

	ROM_REGION( 0x100000, "gfx5", 0 ) // 16x16 4bpp tiles
	ROM_LOAD( "ae202w76.bc1",  0x000000, 0x0100000, CRC(5cc857ca) SHA1(2553fb5220433acc15dfb726dc064fe333e51d88) )

	ROM_REGION( 0x400000, "ymz", 0 ) // YMZ280B Samples
	ROM_LOAD( "ae400t23.ya1", 0x000000, 0x200000, CRC(c6ffb043) SHA1(e0c6c5f6b840f63c9a685a2c3be66efa4935cbeb) )
	ROM_LOAD( "ae401t24.yb1", 0x200000, 0x200000, CRC(d83f1a73) SHA1(412b7ac9ff09a984c28b7d195330d78c4aac3dc5) )

	ROM_REGION( 0x100000, "oki", 0 ) // M6295 Samples
	ROM_LOAD( "ae500w07.ad1", 0x080000, 0x080000, CRC(3734f92c) SHA1(048555b5aa89eaf983305c439ba08d32b4a1bb80) )
ROM_END

/*************************************
 *
 *  Game driver(s)
 *
 *************************************/
// Proyecto Shadows Mame Build Plus
/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// 1943 Kai: Midway Kaisen
GAME( 1996, tkdenshos01,  tkdensho, deroon, deroon, tecmosys_state, tkdensho,   ROT0, "Hacks", "Toukidenshou - Angel Eyes (VER. 960614)(Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
//GAME( 1996, tkdenshoas01, tkdensho, deroon, deroon, tecmosys_state, tkdensha,   ROT0, "Hacks", "Toukidenshou - Angel Eyes (VER. 960427)(Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE ) //Duplicado Al Original