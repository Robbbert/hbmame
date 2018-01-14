// license:BSD-3-Clause
// copyright-holders:Robbbert


/******************************************************************************/

#include "machine/watchdog.h"

// set up a hbmame-specific derived class, instead of hacking into src/mame/includes/neogeo.h
class neogeo_hbmame : public neogeo_noslot_state
{
public:
	neogeo_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo_noslot_state(mconfig, type, tag)
		, m_kog_prot(*this, "kog_prot")
	{}


	DECLARE_DRIVER_INIT(cmc42sfix);
	DECLARE_DRIVER_INIT(cmc50sfix);
	DECLARE_DRIVER_INIT(cthd2k3a);
	DECLARE_DRIVER_INIT(dbdrsp);
	DECLARE_DRIVER_INIT(fr2ch);
	DECLARE_DRIVER_INIT(garoud);
	DECLARE_DRIVER_INIT(jckeygpd);
	DECLARE_DRIVER_INIT(kof95sp);
	DECLARE_DRIVER_INIT(kof96ep);
	DECLARE_DRIVER_INIT(kof97pla);
	DECLARE_DRIVER_INIT(kof99bh);
	DECLARE_DRIVER_INIT(kof99d);
	DECLARE_DRIVER_INIT(kof2000d);
	DECLARE_DRIVER_INIT(kof2000h);
	DECLARE_DRIVER_INIT(kof2000m);
	DECLARE_DRIVER_INIT(kof2kbsd);
	DECLARE_DRIVER_INIT(kof2knd);
	DECLARE_DRIVER_INIT(kof2kxxx);
	DECLARE_DRIVER_INIT(kof2001d);
	DECLARE_DRIVER_INIT(kof2001m);
	DECLARE_DRIVER_INIT(kf2k1pls);
	DECLARE_DRIVER_INIT(kf2k1pa);
	DECLARE_DRIVER_INIT(kof2002d);
	DECLARE_DRIVER_INIT(kof2002m);
	DECLARE_DRIVER_INIT(kof2k2bd);
	DECLARE_DRIVER_INIT(kof2k2dbh);
	DECLARE_DRIVER_INIT(kof2k2h);
	DECLARE_DRIVER_INIT(kof2k2lb);
	DECLARE_DRIVER_INIT(kof2k2ps2a);
	DECLARE_DRIVER_INIT(kof2k2pl17);
	DECLARE_DRIVER_INIT(kof2003b);
	DECLARE_DRIVER_INIT(kof2003hb);
	DECLARE_DRIVER_INIT(kof2k3hd);
	DECLARE_DRIVER_INIT(kof2k3pcd);
	DECLARE_DRIVER_INIT(kof10thu);
	DECLARE_DRIVER_INIT(kogd);
	DECLARE_DRIVER_INIT(lbsp);
	DECLARE_DRIVER_INIT(matrima);
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
	DECLARE_DRIVER_INIT(pnyaad);
	DECLARE_DRIVER_INIT(rotdb);
	DECLARE_DRIVER_INIT(rotdd);
	DECLARE_DRIVER_INIT(rotdnd);
	DECLARE_DRIVER_INIT(sam5hb);
	DECLARE_DRIVER_INIT(sam5sphb);
	DECLARE_DRIVER_INIT(sengo3d);
	DECLARE_DRIVER_INIT(shockt2w);
	DECLARE_DRIVER_INIT(svchb);
	DECLARE_DRIVER_INIT(svcpcd);
private:
	optional_device<kog_prot_device> m_kog_prot;
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



static MACHINE_CONFIG_DERIVED( no_watchdog, neogeo_noslot )
	MCFG_WATCHDOG_MODIFY("watchdog")
	MCFG_WATCHDOG_TIME_INIT(attotime::from_seconds(0.0))
MACHINE_CONFIG_END


// used by samsho2sp, doubledrsp
ADDRESS_MAP_START( samsho2sp_map, AS_PROGRAM, 16, neogeo_state )
	AM_RANGE(0x900000, 0x91ffff) AM_ROM AM_REGION("maincpu", 0x200000) // extra rom
	AM_IMPORT_FROM( main_map_noslot )
ADDRESS_MAP_END

static MACHINE_CONFIG_DERIVED( samsho2sp, neogeo_noslot )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(samsho2sp_map)
MACHINE_CONFIG_END

// used by lbsp
ADDRESS_MAP_START( lbsp_map, AS_PROGRAM, 16, neogeo_state )
	AM_RANGE(0x900000, 0x91ffff) AM_ROM AM_REGION("maincpu", 0x700000) // extra rom
	AM_IMPORT_FROM( main_map_noslot )
ADDRESS_MAP_END

static MACHINE_CONFIG_DERIVED( lbsp, neogeo_noslot )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(lbsp_map)
MACHINE_CONFIG_END

/******************************************************************************/


#include "neogeohb.cpp"
#include "neogeo1.cpp"
#include "kof94.cpp"
#include "kof95.cpp"
#include "kof96.cpp"
#include "kof97.cpp"
#include "kof98.cpp"
#include "kof99.cpp"
#include "kof2000.cpp"
#include "kof2001.cpp"
#include "kof2002.cpp"
#include "kof2003.cpp"
#include "mgd2.cpp"
#include "mslug.cpp"
#include "ngdoubledr.cpp"
#include "nggaroup.cpp"
#include "ngkofmisc.cpp"
#include "nglastblade.cpp"
#include "ngrbff.cpp"
#include "ngrotd.cpp"
#include "ngsamsho.cpp"
#include "ngsengoku.cpp"
#include "ngsvc.cpp"
#include "ngwh.cpp"

