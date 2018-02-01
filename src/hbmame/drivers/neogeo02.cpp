// license:BSD-3-Clause
// copyright-holders:Robbbert


/******************************************************************************/

#include "machine/watchdog.h"

// set up a hbmame-specific derived class, instead of hacking into src/mame/includes/neogeo.h
class neogeo2_hbmame : public neogeo2_state
{
public:
	neogeo2_hbmame(const machine_config &mconfig, device_type type, const char *tag)
		: neogeo2_state(mconfig, type, tag)
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
	void hbmame_kog(machine_config &config);
private:
	optional_device<kog_prot_device> m_kog_prot;
};

DRIVER_INIT_MEMBER( neogeo2_hbmame, cmc42sfix )
{
	DRIVER_INIT_CALL(neogeo);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

DRIVER_INIT_MEMBER( neogeo2_hbmame, cmc50sfix )
{
	DRIVER_INIT_CALL(neogeo);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}


/******************************************************************************/


#include "neogeohb.cpp"
#include "neogeo1.cpp"
///#include "kof94.cpp"
///#include "kof95.cpp"
///#include "kof96.cpp"
///#include "kof97.cpp"
///#include "kof98.cpp"
///#include "kof99.cpp"
#include "kof2000.cpp"
#include "kof2001.cpp"
#include "kof2002.cpp"
#include "kof2003.cpp"
#include "mgd2.cpp"
///#include "mslug.cpp"
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

