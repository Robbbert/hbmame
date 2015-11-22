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


	DECLARE_DRIVER_INIT(fr2ch);
private:
	//optional_device<ngbootleg_prot_device> m_bootleg_prot;
	//optional_device<kog_prot_device> m_kog_prot;
};







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

#define NEO_SFIX_2K(name, hash) \
	ROM_Y_ZOOM \
	ROM_REGION( 0x20000, "fixedbios", 0 ) \
	ROM_LOAD( "sfix.sfix", 0, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) ) \
	ROM_REGION( 0x2000, "fixed", 0 ) \
	ROM_LOAD( name, 0x000000, 0x2000, hash )



/******************************************************************************/


#include "neogeohb.cpp"
//#include "neogeo1.cpp"
#include "kof94.cpp"
#include "kof95.cpp"
#include "kof96.cpp"
#include "kof97.cpp"
//#include "kof98.cpp"
//#include "kof99.cpp"
//#include "kof2000.cpp"
//#include "kof2001.cpp"
//#include "kof2002.cpp"
//#include "kof2003.cpp"
//#include "mslug.cpp"
#include "mgd2.cpp"
#include "ngextra.cpp"

