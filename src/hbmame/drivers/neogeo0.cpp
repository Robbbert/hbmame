// license:BSD-3-Clause
// copyright-holders:Robbbert
//#include "../mame/drivers/neogeo.cpp"


/******************************************************************************/

// set up a hbmame-specific derived class, instead of hacking into src/mame/includes/neogeo.h
class neogeo_hbmame : public neogeo_noslot_state //, public neogeo_noslot_kog_state
{
public:
	neogeo_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo_noslot_state(mconfig, type, tag)
		//, neogeo_noslot_kog_state(mconfig, type, tag)
	//	, m_bootleg_prot(*this, "bootleg_prot"),
	//	, m_kog_prot(*this, "kog_prot")
	{}


	DECLARE_DRIVER_INIT(cmc42sfix);
	DECLARE_DRIVER_INIT(cmc50sfix);
	DECLARE_DRIVER_INIT(fr2ch);
	DECLARE_DRIVER_INIT(garoud);
	DECLARE_DRIVER_INIT(kof96ep);
	DECLARE_DRIVER_INIT(kof97pla);
	DECLARE_DRIVER_INIT(kof99bh);
	DECLARE_DRIVER_INIT(kof99d);
	DECLARE_DRIVER_INIT(matrimd);
	DECLARE_DRIVER_INIT(matrmehc);
	DECLARE_DRIVER_INIT(mslug3d);
	DECLARE_DRIVER_INIT(mslug3fr);
	DECLARE_DRIVER_INIT(mslug3n);
	DECLARE_DRIVER_INIT(mslug4d);
	DECLARE_DRIVER_INIT(mslug4dh);
	DECLARE_DRIVER_INIT(mslug4it);
	DECLARE_DRIVER_INIT(mslug4m);
	DECLARE_DRIVER_INIT(mslug5b);
	DECLARE_DRIVER_INIT(mslug5d);
	DECLARE_DRIVER_INIT(mslug5hd);
	DECLARE_DRIVER_INIT(mslug5nd);
	DECLARE_DRIVER_INIT(ms5pcbd);
	DECLARE_DRIVER_INIT(mslug5d1);
	DECLARE_DRIVER_INIT(mslug5fr1);
	DECLARE_DRIVER_INIT(ms5boot);
//	DECLARE_DRIVER_INIT(kof2001d);
//	DECLARE_DRIVER_INIT(kof2000d);
//	DECLARE_DRIVER_INIT(kof2000h);
//	DECLARE_DRIVER_INIT(kof2000m);
//	DECLARE_DRIVER_INIT(kof2001m);
//	DECLARE_DRIVER_INIT(kf2k1pls);
//	DECLARE_DRIVER_INIT(kf2k1pa);
//	DECLARE_DRIVER_INIT(kof2002d);
//	DECLARE_DRIVER_INIT(kof2002m);
//	DECLARE_DRIVER_INIT(kof2k2dbh);
//	DECLARE_DRIVER_INIT(kof2k2h);
//	DECLARE_DRIVER_INIT(kof2003b);
	DECLARE_DRIVER_INIT(rotdb);
	DECLARE_DRIVER_INIT(rotdd);
	DECLARE_DRIVER_INIT(rotdnd);
	DECLARE_DRIVER_INIT(samsh5sd);
	DECLARE_DRIVER_INIT(samsho5d);
	DECLARE_DRIVER_INIT(samsh5hd);
	DECLARE_DRIVER_INIT(samsh5spd);
	DECLARE_DRIVER_INIT(samsho5hd);
	DECLARE_DRIVER_INIT(sengo3d);
	DECLARE_DRIVER_INIT(shockt2w);
	DECLARE_DRIVER_INIT(svcd);
private:
	//optional_device<ngbootleg_prot_device> m_bootleg_prot;
	//optional_device<kog_prot_device> m_kog_prot;
};

DRIVER_INIT_MEMBER( neogeo_hbmame, cmc42sfix )
{
	DRIVER_INIT_CALL(neogeo);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

DRIVER_INIT_MEMBER( neogeo_hbmame, cmc50sfix )
{
	DRIVER_INIT_CALL(neogeo);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}







	/* The Encrypted Boards do not have a s1 rom, data for it comes from the Cx ROMs */
#define NEO_SFIX_MT_128K \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix",  0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x20000, "fixed", ROMREGION_ERASE00 )

#define NEO_SFIX_MT_512K \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix",  0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x80000, "fixed", ROMREGION_ERASE00 )

#define NEO_SFIX_256K(name, hash) \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix", 0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x40000, "fixed", 0 ) \
	ROM_LOAD( name, 0x000000, 0x40000, hash )

// Specific for MSLUG hacks
#define NEO_SFIX_MSLUG(name, hash) \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix", 0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x80000, "fixed", 0 ) \
	ROM_LOAD( name, 0x000000, 0x20000, hash ) \
	ROM_RELOAD( 0x020000, 0x20000 ) \
	ROM_RELOAD( 0x040000, 0x20000 ) \
	ROM_RELOAD( 0x060000, 0x20000 )

#define NEO_SFIX_2K(name, hash) \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix", 0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x2000, "fixed", 0 ) \
	ROM_LOAD( name, 0x000000, 0x2000, hash )



/******************************************************************************/


#include "neogeohb.cpp"
#include "neogeo1.cpp"
#include "kof94.cpp"
#include "kof95.cpp"
#include "kof96.cpp"
#include "kof97.cpp"
#include "kof98.cpp"
#include "kof99.cpp"
//#include "kof2000.cpp"
//#include "kof2001.cpp"
//#include "kof2002.cpp"
//#include "kof2003.cpp"
#include "mgd2.cpp"
#include "mslug.cpp"
#include "ngdoubledr.cpp"
#include "ngextra.cpp"
#include "nggaroup.cpp"
#include "nglastblade.cpp"
#include "ngrbff.cpp"
#include "ngrotd.cpp"
#include "ngsamsho.cpp"
#include "ngsengoku.cpp"
#include "ngwh.cpp"

