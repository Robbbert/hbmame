// license:BSD-3-Clause
// copyright-holders:Robbbert


/******************************************************************************/


DRIVER_INIT_MEMBER( neogeo_state, cmc42sfix )
{
	DRIVER_INIT_CALL(neogeo);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

DRIVER_INIT_MEMBER( neogeo_state, cmc50sfix )
{
	DRIVER_INIT_CALL(neogeo);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}


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

