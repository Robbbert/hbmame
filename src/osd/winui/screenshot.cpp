// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

/***************************************************************************

  Screenshot.c

    Win32 DIB handling.

      Created 7/1/98 by Mike Haaland (mhaaland@hypertech.com)

  0.145u5
  - Cleaned out unused code.
  - If Software tab is chosen, software-specific pictures can be displayed.
  - Background picture must be PNG, and can be chosen from anywhere, but
    must be uncompressed (not in a zip file).

***************************************************************************/

// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "png.h"
#include "unzip.h"
#include "mui_opts.h"
#include "mui_util.h"
#include "winui.h"
#include "drivenum.h"

/***************************************************************************
    Static global variables
***************************************************************************/

/* these refer to the single image currently loaded by the ScreenShot functions */
static HGLOBAL   m_hDIB = NULL;
static HPALETTE  m_hPal = NULL;
static HANDLE m_hDDB = NULL;

/* PNG variables */

static int   copy_size = 0;
static char* pixel_ptr = 0;
static int   row = 0;
static int   effWidth;
static BOOL LoadDIB(const char *filename, HGLOBAL *phDIB, HPALETTE *pPal, int pic_type);

/***************************************************************************
    Functions
***************************************************************************/

BOOL ScreenShotLoaded(void)
{
	return m_hDDB != NULL;
}

static BOOL LoadSoftwareScreenShot(const game_driver *drv, LPCSTR lpSoftwareName, int nType)
{
	BOOL result;
	char *s = (char*)alloca(strlen(drv->name) + 1 + strlen(lpSoftwareName) + 5);
	sprintf(s, "%s", lpSoftwareName);
	// oldest code --> sprintf(s, "%s/%s.png", drv->name, lpSoftwareName);
	//sprintf(s, "%s:%s", drv->name, lpSoftwareName);
	result = LoadDIB(s, &m_hDIB, &m_hPal, nType);
	free(s);
	return result;
}

/* Allow us to pre-load the DIB once for future draws */
BOOL LoadScreenShotEx(int nGame, LPCSTR lpSoftwareName, int nType)
{
	BOOL loaded = FALSE;

	/* Delete the last ones */
	FreeScreenShot();

	/* Load the DIB */

	if (lpSoftwareName)
	{
		int nParentIndex = -1;
		loaded = LoadSoftwareScreenShot(&driver_list::driver(nGame), lpSoftwareName, nType);
		if (!loaded && DriverIsClone(nGame) == TRUE)
		{
			nParentIndex = GetParentIndex(&driver_list::driver(nGame));
			loaded = LoadSoftwareScreenShot(&driver_list::driver(nParentIndex), lpSoftwareName, nType);
		}
	}

	if (!loaded)
		loaded = LoadDIB(driver_list::driver(nGame).name, &m_hDIB, &m_hPal, nType);

	/* If not loaded, see if there is a clone and try that */
	if (!loaded)
	{
		int nParentIndex = GetParentIndex(&driver_list::driver(nGame));
		if( nParentIndex >= 0)
		{
			loaded = LoadDIB(driver_list::driver(nParentIndex).name, &m_hDIB, &m_hPal, nType);
			nParentIndex = GetParentIndex(&driver_list::driver(nParentIndex));
			if (!loaded && nParentIndex >= 0)
				loaded = LoadDIB(driver_list::driver(nParentIndex).name, &m_hDIB, &m_hPal, nType);
		}
	}

	if (loaded)
	{
		HDC hdc = GetDC(GetMainWindow());
		m_hDDB = DIBToDDB(hdc, m_hDIB, NULL);
		ReleaseDC(GetMainWindow(),hdc);
	}

	return (loaded) ? TRUE : FALSE;
}

HANDLE GetScreenShotHandle()
{
	return m_hDDB;
}

int GetScreenShotWidth(void)
{
	return ((LPBITMAPINFO)m_hDIB)->bmiHeader.biWidth;
}

int GetScreenShotHeight(void)
{
	return ((LPBITMAPINFO)m_hDIB)->bmiHeader.biHeight;
}

/* Delete the HPALETTE and Free the HDIB memory */
void FreeScreenShot(void)
{
	if (m_hDIB != NULL)
		GlobalFree(m_hDIB);
	m_hDIB = NULL;

	if (m_hPal != NULL)
		DeletePalette(m_hPal);
	m_hPal = NULL;

	if (m_hDDB != NULL)
		DeleteObject(m_hDDB);
	m_hDDB = NULL;

	///current_image_game = -1;
	///current_image_type = -1;
}

/***************************************************************************
    PNG graphics handling functions
***************************************************************************/

BOOL AllocatePNG(png_info *p, HGLOBAL *phDIB, HPALETTE *pPal)
{
	int 				dibSize;
	HGLOBAL 			hDIB;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER	lpbi;
	LPBITMAPINFO		bmInfo;
	LPVOID				lpDIBBits = 0;
	int 				lineWidth = 0;
	int 				nColors = 0;
	RGBQUAD*			pRgb;
	copy_size = 0;
	pixel_ptr = 0;
	row 	  = p->height - 1;
	lineWidth = p->width;

	if (p->color_type != 2 && p->num_palette <= 256)
		nColors =  p->num_palette;

	bi.biSize			= sizeof(BITMAPINFOHEADER);
	bi.biWidth			= p->width;
	bi.biHeight 		= p->height;
	bi.biPlanes 		= 1;
	bi.biBitCount		= (p->color_type == 3) ? 8 : 24; /* bit_depth; */

	bi.biCompression	= BI_RGB;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= nColors;
	bi.biClrImportant	= nColors;

	effWidth = (long)(((long)lineWidth*bi.biBitCount + 31) / 32) * 4;

	dibSize = (effWidth * bi.biHeight);
	hDIB = GlobalAlloc(GMEM_FIXED, bi.biSize + (nColors * sizeof(RGBQUAD)) + dibSize);

	if (!hDIB)
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	memcpy(lpbi, &bi, sizeof(BITMAPINFOHEADER));
	pRgb = (RGBQUAD*)((LPSTR)lpbi + bi.biSize);
	lpDIBBits = (LPVOID)((LPSTR)lpbi + bi.biSize + (nColors * sizeof(RGBQUAD)));
	if (nColors)
	{
		int i;
		/* Convert a PNG palette (3 byte RGBTRIPLEs) to a new color table (4 byte RGBQUADs) */
		for (i = 0; i < nColors; i++)
		{
			RGBQUAD rgb;

			rgb.rgbRed	= p->palette[i * 3 + 0];
			rgb.rgbGreen	= p->palette[i * 3 + 1];
			rgb.rgbBlue 	= p->palette[i * 3 + 2];
			rgb.rgbReserved = (BYTE)0;

			pRgb[i] = rgb;
		}
	}

	bmInfo = (LPBITMAPINFO)hDIB;

	/* Create a halftone palette if colors > 256. */
	if (0 == nColors || nColors > 256)
	{
		HDC hDC = CreateCompatibleDC(0); /* Desktop DC */
		*pPal = CreateHalftonePalette(hDC);
		DeleteDC(hDC);
	}
	else
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *)malloc(nSize);
		int  i;

		pLP->palVersion 	= 0x300;
		pLP->palNumEntries	= nColors;

		for (i = 0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed   = bmInfo->bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = bmInfo->bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue  = bmInfo->bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		*pPal = CreatePalette(pLP);

		free (pLP);
	}

	copy_size = dibSize;
	pixel_ptr = (char*)lpDIBBits;
	*phDIB = hDIB;
	return TRUE;
}

inline void store_pixels(UINT8 *buf, int len)
{
	if (pixel_ptr && copy_size)
	{
		memcpy(&pixel_ptr[row * effWidth], buf, len);
		row--;
		copy_size -= len;
	}
}


/* Copied and modified from png.c
   logerror doesn't work here... changed to printf */
static int png_read_bitmap_gui(LPVOID mfile, HGLOBAL *phDIB, HPALETTE *pPAL)
{
	png_info p;
	UINT32 i;
	int bytespp;

	if (png_read_file((core_file*)mfile, &p) != PNGERR_NONE)
		return 0;

	if (p.color_type != 3 && p.color_type != 2)
	{
		printf("PNG Unsupported color type %i (has to be 2 or 3)\n", p.color_type);
		//png_free(&p);
		//return 0;			Leave in so ppl can see incompatibility
	}

	if (p.interlace_method != 0)
	{
		printf("PNG Interlace unsupported\n");
		png_free(&p);
		return 0;
	}

	/* Convert < 8 bit to 8 bit */
	png_expand_buffer_8bit(&p);

	if (!AllocatePNG(&p, phDIB, pPAL))
	{
		printf("PNG Unable to allocate memory to display screenshot\n");
		png_free(&p);
		return 0;
	}

	bytespp = (p.color_type == 2) ? 3 : 1;

	for (i = 0; i < p.height; i++)
	{
		UINT8 *ptr = p.image + i * (p.width * bytespp);

		if (p.color_type == 2) /*(p->bit_depth > 8) */
		{
			int j;
			UINT8 bTmp;

			for (j = 0; j < p.width; j++)
			{
				bTmp = ptr[0];
				ptr[0] = ptr[2];
				ptr[2] = bTmp;
				ptr += 3;
			}
		}
		store_pixels(p.image + i * (p.width * bytespp), p.width * bytespp);
	}

	png_free(&p);

	return 1;
}



/***************************************************************************
    File search functions
***************************************************************************/

static const zip_file_header *zip_file_seek_file(zip_file *zip, const char *filename)
{
	const zip_file_header *header;

	// find the entry
	header = zip_file_first_file(zip);
	while(header && (core_stricmp(header->filename, filename)!=0))
		header = zip_file_next_file(zip);

	return header;
}

static file_error OpenBkgroundFile(const char *filename, core_file **file)
{
	file_error filerr;

	// clear out result
	*file = NULL;

	// look for the raw file
	std::string fname (filename);
	filerr = core_fopen(fname.c_str(), OPEN_FLAG_READ, file);

	return filerr;
}

static file_error OpenRawDIBFile(const char *dir_name, const char *filename, core_file **file)
{
	file_error filerr;

	// clear out result
	*file = NULL;

	// look for the raw file
	std::string fname = std::string(dir_name) + PATH_SEPARATOR + std::string(filename);
	filerr = core_fopen(fname.c_str(), OPEN_FLAG_READ, file);

	return filerr;
}

static file_error OpenZipDIBFile(const char *dir_name, const char *zip_name, const char *filename, core_file **file, void **buffer)
{
	file_error filerr = FILERR_NOT_FOUND;
	zip_error ziperr;
	zip_file *zip;
	const zip_file_header *zip_header;

	// clear out result
	*file = NULL;

	// look into zip file
	std::string fname = std::string(dir_name) + PATH_SEPARATOR + std::string(zip_name) + ".zip";
	ziperr = zip_file_open(fname.c_str(), &zip);

	if (ziperr == ZIPERR_NONE)
	{
		zip_header = zip_file_seek_file(zip, filename);
		if (zip_header != NULL)
		{
			*buffer = malloc(zip_header->uncompressed_length);
			ziperr = zip_file_decompress(zip, *buffer, zip_header->uncompressed_length);
			if (ziperr == ZIPERR_NONE)
			{
				filerr = core_fopen_ram(*buffer, zip_header->uncompressed_length, OPEN_FLAG_READ, file);
			}
		}
		zip_file_close(zip);
	}
	return filerr;
}

// called from winui.c to display the background
BOOL LoadDIBBG(HGLOBAL *phDIB, HPALETTE *pPal)
{
	file_error filerr = FILERR_NOT_FOUND;
	core_file *file = NULL;
	BOOL success = FALSE;
	const char *dir_name;
	if (pPal != NULL )
		DeletePalette(pPal);

	dir_name = GetBgDir();
	filerr = OpenBkgroundFile(dir_name, &file);
	if (filerr == FILERR_NONE)
	{
		success = png_read_bitmap_gui(file, phDIB, pPal);
		core_fclose(file);
	}

	return success;
}

// display a snap, cabinet, title, flyer, marquee, pcb, control panel
static BOOL LoadDIB(const char *filename, HGLOBAL *phDIB, HPALETTE *pPal, int pic_type)
{
	file_error filerr = FILERR_NOT_FOUND; // defined in osdcore.h
	core_file *file = NULL;
	BOOL success = FALSE;
	const char *dir_name;
	const char *zip_name;
	void *buffer = NULL;
	std::string fname;
	if (pPal != NULL )
		DeletePalette(pPal);

	char *system_name = 0;
	char *file_name = 0;
	char* dir_name1 = 0;
	int i,j;
	bool ok = FALSE; // TRUE indicates split success
	// allocate space
	system_name = (char*)malloc(strlen(filename) + 1);
	file_name = (char*)malloc(strlen(filename) + 1);

	switch (pic_type)
	{
		case TAB_SCREENSHOT:
			dir_name = GetImgDir();
			zip_name = "snap";
			break;
		case TAB_FLYER:
			dir_name = GetFlyerDir();
			zip_name = "flyers";
			break;
		case TAB_CABINET:
			dir_name = GetCabinetDir();
			zip_name = "cabinets";
			break;
		case TAB_MARQUEE:
			dir_name = GetMarqueeDir();
			zip_name = "marquees";
			break;
		case TAB_TITLE:
			dir_name = GetTitlesDir();
			zip_name = "titles";
			break;
		case TAB_CONTROL_PANEL:
			dir_name = GetControlPanelDir();
			zip_name = "cpanel";
			break;
		case TAB_PCB :
			dir_name = GetPcbDir();
			zip_name = "pcb";
			break;
		default :
			// in case a non-image tab gets here, which can happen
			return FALSE;
	}

	// if the filename contains a system, split them

	// get system = find colon and truncate there
	for (i = 0; filename[i] && !ok; i++)
	{
		if (filename[i] != ':')
			system_name[i] = filename[i];
		else
		{
			ok = TRUE;
			system_name[i] = '\0';
		}
	}
	system_name[i] = '\0'; // 'i' is one more here than above but needed if no colon

	// now left-justify the filename
	if (ok)
	{
		for (j = 0; filename[i]; j++,i++)
			file_name[j] = filename[i];
		file_name[j] = '\0';
	}
	else
	// it wasn't a software title, copy over to get around const nonsense
	{
		for (i = 0; filename[i]; i++)
			file_name[i] = filename[i];
		file_name[i] = '\0';
	}

	// another const workaround
	dir_name1 = (char*)malloc(strlen(dir_name) + 2);
	for (i = 0; dir_name[i]; i++)
		dir_name1[i] = dir_name[i];
	dir_name1[i++] = ';';
	dir_name1[i] = '\0';

	// Support multiple paths
	char* dir_one = strtok(dir_name1, ";");

	while (dir_one && filerr != FILERR_NONE)
	{
		//Add handling for the displaying of all the different supported snapshot pattern types
		//%g

		// Do software checks first
		if (ok)
		{
			// Try dir/system/game.png
			fname = std::string(system_name) + PATH_SEPARATOR + std::string(file_name) + ".png";
			filerr = OpenRawDIBFile(dir_one, fname.c_str(), &file);

			// Try dir/system.zip/game.png
			if (filerr != FILERR_NONE)
			{
				fname = std::string(file_name) + ".png";
				filerr = OpenZipDIBFile(dir_one, system_name, fname.c_str(), &file, &buffer);
			}

			// Try dir/system.zip/system/game.png
			if (filerr != FILERR_NONE)
			{
				fname = std::string(system_name) + "/" + std::string(file_name) + ".png";
				filerr = OpenZipDIBFile(dir_one, system_name, fname.c_str(), &file, &buffer);
			}

			// Try dir/zipfile/system/game.png
			if (filerr != FILERR_NONE)
			{
				filerr = OpenZipDIBFile(dir_one, zip_name, fname.c_str(), &file, &buffer);
			}
		}

		// give up on software-specific, try dir/system.png
		if (filerr != FILERR_NONE)
		{
			fname = std::string(system_name) + ".png";
			filerr = OpenRawDIBFile(dir_one, fname.c_str(), &file);
		}

		// For SNAPS only, try filenames with 0000.
		if (pic_type == TAB_SCREENSHOT)
		{
			if (filerr != FILERR_NONE)
			{
				//%g/%i
				fname = std::string(system_name) + PATH_SEPARATOR + "0000.png";
				filerr = OpenRawDIBFile(dir_one, fname.c_str(), &file);
			}
			if (filerr != FILERR_NONE)
			{
				//%g%i
				fname = std::string(system_name) + "0000.png";
				filerr = OpenRawDIBFile(dir_one, fname.c_str(), &file);
			}
			if (filerr != FILERR_NONE)
			{
				//%g/%g%i
				fname = std::string(system_name) + PATH_SEPARATOR + std::string(system_name) + "0000.png";
				filerr = OpenRawDIBFile(dir_one, fname.c_str(), &file);
			}
		}

		// Try dir/zipfile/system.png
		if (filerr != FILERR_NONE)
		{
			fname = std::string(file_name) + ".png";
			filerr = OpenZipDIBFile(dir_one, zip_name, fname.c_str(), &file, &buffer);
		}

		dir_one = strtok(NULL, ";");
	}

	if (filerr == FILERR_NONE)
	{
		success = png_read_bitmap_gui(file, phDIB, pPal);
		core_fclose(file);
	}

	// free the buffer if we have to
	if (buffer != NULL)
		free(buffer);
	free(system_name);
	free(file_name);
	free(dir_name1);

	return success;
}

HBITMAP DIBToDDB(HDC hDC, HANDLE hDIB, LPMYBITMAPINFO desc)
{
	LPBITMAPINFOHEADER	lpbi;
	HBITMAP			hBM;
	int 			nColors;
	BITMAPINFO *		bmInfo = (LPBITMAPINFO)hDIB;
	LPVOID			lpDIBBits;

	if (hDIB == NULL)
		return NULL;

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 << lpbi->biBitCount;

	if (bmInfo->bmiHeader.biBitCount > 8)
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo->bmiColors +
			bmInfo->bmiHeader.biClrUsed) +
			((bmInfo->bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo->bmiColors + nColors);

	if (desc != 0)
	{
		/* Store for easy retrieval later */
		desc->bmWidth  = bmInfo->bmiHeader.biWidth;
		desc->bmHeight = bmInfo->bmiHeader.biHeight;
		desc->bmColors = (nColors <= 256) ? nColors : 0;
	}

	hBM = CreateDIBitmap(hDC,		/* handle to device context */
		(LPBITMAPINFOHEADER)lpbi,	/* pointer to bitmap info header  */
		(LONG)CBM_INIT,			/* initialization flag */
		lpDIBBits,			/* pointer to initialization data  */
		(LPBITMAPINFO)lpbi,		/* pointer to bitmap info */
		DIB_RGB_COLORS);		/* color-data usage  */

	return hBM;
}

BOOL LoadScreenShot(int nGame, int nType)
{
	return LoadScreenShotEx(nGame, NULL, nType);
}

/* End of source */
