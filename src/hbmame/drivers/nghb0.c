// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/neogeo.c"


/******************************************************************************/


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


#include "nghbhb.c"
#include "nghb1.c"
#include "ngkof96.c"
#include "ngkof97.c"
#include "ngkof98.c"
#include "ngkof99.c"
#include "ngkof2000.c"
#include "ngkof2001.c"
#include "ngkof2002.c"
#include "ngkof2003.c"
#include "ngmslug.c"
#include "ngmgd2.c"
