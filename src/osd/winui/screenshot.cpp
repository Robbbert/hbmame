// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

/***************************************************************************

  Screenshot.cpp
  
  Displays snapshots, control panels and other pictures.
  Files must be of type .PNG, .JPG or .JPEG .
  If Software tab is chosen, software-specific pictures can be displayed.
  Background picture must be PNG, and can be chosen from anywhere, but
    must be uncompressed (not in a zip file).

***************************************************************************/

// standard windows headers
#include <windows.h>
#include <windowsx.h>
#include <setjmp.h>

// MAME/MAMEUI headers
#include "png.h"
#include "unzip.h"
#include "mui_opts.h"
#include "mui_util.h"  // for DriverIsClone
#include "drivenum.h"
#include "libjpeg/jpeglib.h"

/***************************************************************************
    Static global variables
***************************************************************************/

/* these refer to the single image currently loaded by the ScreenShot functions */
static HGLOBAL   m_hDIB = nullptr;
static HPALETTE  m_hPal = nullptr;
static HANDLE m_hDDB = nullptr;

/* PNG variables */

static int   copy_size = 0;
static char* pixel_ptr = nullptr;
static int   row = 0;
static int   effWidth;

/***************************************************************************
    Functions
***************************************************************************/

// called by winui.cpp twice
BOOL ScreenShotLoaded(void)
{
	return m_hDDB != NULL;
}

// called by winui.cpp once
HANDLE GetScreenShotHandle()
{
	return m_hDDB;
}

// called by winui.cpp twice
int GetScreenShotWidth(void)
{
	return abs( ((LPBITMAPINFO)m_hDIB)->bmiHeader.biWidth);
}

// called by winui.cpp twice
int GetScreenShotHeight(void)
{
	return abs( ((LPBITMAPINFO)m_hDIB)->bmiHeader.biHeight);
}

// called by winui.cpp
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
}

/***************************************************************************
    JPEG graphics handling functions
***************************************************************************/

struct mameui_jpeg_error_mgr
{
	struct jpeg_error_mgr pub; /* "public" fields */
	jmp_buf setjmp_buffer; /* for return to caller */
};

METHODDEF(void) mameui_jpeg_error_exit(j_common_ptr cinfo)
{
	mameui_jpeg_error_mgr* myerr = (mameui_jpeg_error_mgr*)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

static bool jpeg_read_bitmap_gui(util::core_file &mfile, HGLOBAL *phDIB, HPALETTE *pPAL)
{
	uint64_t bytes = mfile.size();
	unsigned char* content = (unsigned char*)::malloc(bytes * sizeof(unsigned char));
	::memcpy(content, mfile.buffer(), bytes);

	*pPAL = NULL;
	HGLOBAL hDIB = NULL;
	jpeg_decompress_struct info;
	mameui_jpeg_error_mgr  err;
	info.err = jpeg_std_error(&err.pub);
	err.pub.error_exit = mameui_jpeg_error_exit;

	if (setjmp(err.setjmp_buffer))
	{
		jpeg_destroy_decompress(&info);
		::free(content);
		copy_size = 0;
		pixel_ptr = NULL;
		effWidth = 0;
		row = 0;
		if (hDIB)
			::GlobalFree(hDIB);
		return false;
	}

	jpeg_create_decompress(&info);
	jpeg_mem_src(&info, content, bytes);
	jpeg_read_header(&info, TRUE);
	if (info.num_components != 3 || info.out_color_space != JCS_RGB)
	{
		jpeg_destroy_decompress(&info);
		::free(content);
		return false;
	}

	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	LPVOID lpDIBBits = 0;
	int lineWidth = 0;
	LPSTR pRgb;
	copy_size = 0;
	pixel_ptr = NULL;
	row = info.image_height;
	lineWidth = info.image_width;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = info.image_width;
	bi.biHeight = -info.image_height; //top down bitmap
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 2835;
	bi.biYPelsPerMeter = 2835;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	effWidth = (long)(((long)lineWidth*bi.biBitCount + 31) / 32) * 4;
	int dibSize = (effWidth * info.image_height);
	hDIB = ::GlobalAlloc(GMEM_FIXED, bi.biSize + dibSize);

	if (!hDIB)
	{
		::free(content);
		return false;
	}

	jpeg_start_decompress(&info);

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	::memcpy(lpbi, &bi, sizeof(BITMAPINFOHEADER));
	pRgb = (LPSTR)lpbi + bi.biSize;
	lpDIBBits = (LPVOID)((LPSTR)lpbi + bi.biSize);

	while (info.output_scanline < info.output_height) // loop
	{
		unsigned char* cacheRow[1] = { (unsigned char*)pRgb };
		jpeg_read_scanlines(&info, cacheRow, 1);
		//rgb to win32 bgr
		for (JDIMENSION i = 0; i < info.output_width; ++i)
			std::swap(cacheRow[0][i * 3 + 0], cacheRow[0][i * 3 + 2]);
		pRgb += effWidth;
	}
	jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);
	copy_size = dibSize;
	pixel_ptr = (char*)lpDIBBits;
	*phDIB = hDIB;
	return true;
}


/***************************************************************************
    PNG graphics handling functions
***************************************************************************/

static BOOL AllocatePNG(png_info *p, HGLOBAL *phDIB, HPALETTE *pPal)
{
	int nColors = 0;
	copy_size = 0;
	pixel_ptr = 0;
	row = p->height - 1;
	int lineWidth = p->width;

	BITMAPINFOHEADER bi;
	if (p->color_type != 2 && p->num_palette <= 256)
		nColors = p->num_palette;

	bi.biSize           = sizeof(BITMAPINFOHEADER);
	bi.biWidth          = p->width;
	bi.biHeight         = p->height;
	bi.biPlanes         = 1;
	bi.biBitCount       = (p->color_type == 3) ? 8 : 24; /* bit_depth; */
	bi.biCompression    = BI_RGB;
	bi.biSizeImage      = 0;
	bi.biXPelsPerMeter  = 0;
	bi.biYPelsPerMeter  = 0;
	bi.biClrUsed        = nColors;
	bi.biClrImportant   = nColors;

	effWidth = (long)(((long)lineWidth*bi.biBitCount + 31) / 32) * 4;

	int dibSize = effWidth * bi.biHeight;
	HGLOBAL hDIB = GlobalAlloc(GMEM_FIXED, bi.biSize + (nColors * sizeof(RGBQUAD)) + dibSize);

	if (!hDIB)
		return FALSE;

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)hDIB;
	memcpy(lpbi, &bi, sizeof(BITMAPINFOHEADER));
	RGBQUAD* pRgb = (RGBQUAD*)((LPSTR)lpbi + bi.biSize);
	LPVOID lpDIBBits = (LPVOID)((LPSTR)lpbi + bi.biSize + (nColors * sizeof(RGBQUAD)));

	if (nColors)
	{
		int i;
		/* Convert a PNG palette (3 byte RGBTRIPLEs) to a new color table (4 byte RGBQUADs) */
		for (i = 0; i < nColors; i++)
		{
			RGBQUAD rgb;
			rgb.rgbRed = p->palette[i * 3 + 0];
			rgb.rgbGreen = p->palette[i * 3 + 1];
			rgb.rgbBlue = p->palette[i * 3 + 2];
			rgb.rgbReserved = (BYTE)0;
			pRgb[i] = rgb;
		}
	}

	LPBITMAPINFO bmInfo = (LPBITMAPINFO)hDIB;

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
		int i;

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

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


/* Copied and modified from png.cpp
   logerror doesn't work here... changed to printf */
static bool png_read_bitmap_gui(util::core_file &mfile, HGLOBAL *phDIB, HPALETTE *pPAL)
{
	png_info p;
	if (p.read_file(mfile) != PNGERR_NONE)
		return false;

	if (p.color_type != 3 && p.color_type != 2)
	{
		printf("PNG Unsupported color type %i (has to be 2 or 3)\n", p.color_type);
		//return false;                    Leave in so ppl can see incompatibility
	}

	if (p.interlace_method != 0)
	{
		printf("PNG Interlace unsupported\n");
		return false;
	}

	/* Convert < 8 bit to 8 bit */
	p.expand_buffer_8bit();

	if (!AllocatePNG(&p, phDIB, pPAL))
	{
		printf("PNG Unable to allocate memory to display screenshot\n");
		return false;
	}

	int bytespp = (p.color_type == 2) ? 3 : 1;

	for (uint32_t i = 0; i < p.height; i++)
	{
		UINT8 *ptr = &p.image[i * (p.width * bytespp)];

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
		store_pixels(&p.image[i * (p.width * bytespp)], p.width * bytespp);
	}

	return true;
}



/***************************************************************************
    File search functions
***************************************************************************/

static osd_file::error OpenRawDIBFile(const char *dir_name, const char *filename, util::core_file::ptr &file)
{
	// clear out result
	file = NULL;

	// look for the raw file
	string fname = string(dir_name) + PATH_SEPARATOR + string(filename);
	return util::core_file::open(fname.c_str(), OPEN_FLAG_READ, file);
}

static osd_file::error OpenZipDIBFile(const char *dir_name, const char *zip_name, const char *filename, util::core_file::ptr &file, void **buffer)
{
	util::archive_file::error ziperr = util::archive_file::error::NONE;
	util::archive_file::ptr zip;

	// clear out result
	file = nullptr;

	// look for the raw file
	string fname = string(dir_name).append(PATH_SEPARATOR).append(filename);
	osd_file::error filerr = util::core_file::open(fname, OPEN_FLAG_READ, file);

	// did the raw file not exist?
	if (filerr != osd_file::error::NONE)
	{
		// look into zip file
		fname = std::string(dir_name).append(PATH_SEPARATOR).append(zip_name).append(".zip");
		ziperr = util::archive_file::open_zip(fname, zip);

		if (ziperr == util::archive_file::error::NONE)
		{
			int found = zip->search(filename, false);

			if (found >= 0)
			{
				*buffer = malloc(zip->current_uncompressed_length());
				ziperr = zip->decompress(*buffer, zip->current_uncompressed_length());

				if (ziperr == util::archive_file::error::NONE)
					filerr = util::core_file::open_ram(*buffer, zip->current_uncompressed_length(), OPEN_FLAG_READ, file);
			}

			zip.reset();
		}
	}

	if ((filerr != osd_file::error::NONE) || (ziperr != util::archive_file::error::NONE))
	{
		// look into 7z file
		fname = std::string(dir_name).append(PATH_SEPARATOR).append(zip_name).append(".7z");
		ziperr = util::archive_file::open_7z(fname, zip);

		if (ziperr == util::archive_file::error::NONE)
		{
			int found = zip->search(filename, false);

			if (found >= 0)
			{
				*buffer = malloc(zip->current_uncompressed_length());
				ziperr = zip->decompress(*buffer, zip->current_uncompressed_length());

				if (ziperr == util::archive_file::error::NONE)
					filerr = util::core_file::open_ram(*buffer, zip->current_uncompressed_length(), OPEN_FLAG_READ, file);
			}

			zip.reset();
		}
	}

	return filerr;
}

// display a snap, cabinet, title, flyer, marquee, pcb, control panel
static BOOL LoadDIB(const char *filename, HGLOBAL *phDIB, HPALETTE *pPal, int pic_type)
{
	osd_file::error filerr = osd_file::error::NOT_FOUND; // defined in osdcore.h
	util::core_file::ptr file = NULL;
	char fullpath[2048];
	const char* zip_name;
	string t;

	if (pPal)
		DeletePalette(pPal);

	switch (pic_type)
	{
		case TAB_ARTWORK:
			t = GetArtworkDir();
			zip_name = "artpreview";
			break;
		case TAB_BOSSES:
			t = GetBossesDir();
			zip_name = "bosses";
			break;
		case TAB_CABINET:
			t = GetCabinetDir();
			zip_name = "cabinets";
			break;
		case TAB_CONTROL_PANEL:
			t = GetControlPanelDir();
			zip_name = "cpanel";
			break;
		case TAB_COVER:
			t = GetCoversDir();
			zip_name = "covers";
			break;
		case TAB_ENDS:
			t = GetEndsDir();
			zip_name = "ends";
			break;
		case TAB_FLYER:
			t = GetFlyerDir();
			zip_name = "flyers";
			break;
		case TAB_GAMEOVER:
			t = GetGameOverDir();
			zip_name = "gameover";
			break;
		case TAB_HOWTO:
			t = GetHowToDir();
			zip_name = "howto";
			break;
		case TAB_LOGO:
			t = GetLogoDir();
			zip_name = "logo";
			break;
		case TAB_MARQUEE:
			t = GetMarqueeDir();
			zip_name = "marquees";
			break;
		case TAB_PCB:
			t = GetPcbDir();
			zip_name = "pcb";
			break;
		case TAB_SCORES:
			t = GetScoresDir();
			zip_name = "scores";
			break;
		case TAB_SCREENSHOT:
			t = GetImgDir();
			zip_name = "snap";
			break;
		case TAB_SELECT:
			t = GetSelectDir();
			zip_name = "select";
			break;
		case TAB_TITLE:
			t = GetTitlesDir();
			zip_name = "titles";
			break;
		case TAB_VERSUS:
			t = GetVersusDir();
			zip_name = "versus";
			break;
		default :
			// shouldn't get here
			return false;
	}

	string ext;
	BOOL success;
	void *buffer = NULL;

	for (u8 extnum = 0; extnum < 3; extnum++)
	{
		switch (extnum)
		{
			case 1:
				ext = ".jpg";
				break;
			case 2:
				ext = ".jpeg";
				break;
			default:
				ext = ".png";
		}
		// we need to split the filename into the game name (system_name), and the software-list item name (file_name)
		strcpy(fullpath, t.c_str());
		char tempfile[2048];
		strcpy(tempfile, filename);
		char* system_name = strtok(tempfile, ":");
		char* file_name = strtok(NULL, ":");
		string fname;
		buffer = 0;
		success = false;

		// Support multiple paths
		char* partpath = strtok(fullpath, ";");

		while (partpath && filerr != osd_file::error::NONE)
		{
			//Add handling for the displaying of all the different supported snapshot pattern types

			// Do software checks first
			if (file_name)
			{
				// Try dir/system/game.png
				fname = string(system_name) + PATH_SEPARATOR + string(file_name) + ext;
				filerr = OpenRawDIBFile(partpath, fname.c_str(), file);

				// Try dir/system.zip/game.png
				if (filerr != osd_file::error::NONE)
				{
					fname = string(file_name) + ext;
					filerr = OpenZipDIBFile(partpath, system_name, fname.c_str(), file, &buffer);
				}

				// Try dir/system.zip/system/game.png
				if (filerr != osd_file::error::NONE)
				{
					fname = string(system_name) + "/" + string(file_name) + ext;
					filerr = OpenZipDIBFile(partpath, system_name, fname.c_str(), file, &buffer);
				}

				// Try dir/zipfile/system/game.png
				if (filerr != osd_file::error::NONE)
				{
					filerr = OpenZipDIBFile(partpath, zip_name, fname.c_str(), file, &buffer);
				}
			}

			// give up on software-specific.
			// For SNAPS only, try filenames with 0000.png
			if ((pic_type == TAB_SCREENSHOT) && (extnum == 0))
			{
				if (filerr != osd_file::error::NONE)
				{
					//%g/%i
					fname = string(system_name) + PATH_SEPARATOR + "0000.png";
					filerr = OpenRawDIBFile(partpath, fname.c_str(), file);
				}
			}

			// Try dir/system.png  %g
			if (filerr != osd_file::error::NONE)
			{
				fname = string(system_name) + ext;
				filerr = OpenRawDIBFile(partpath, fname.c_str(), file);
			}

			//%g/%g
			if (filerr != osd_file::error::NONE)
			{
				fname = string(system_name) + PATH_SEPARATOR + string(system_name) + ext;
				filerr = OpenRawDIBFile(partpath, fname.c_str(), file);
			}

			// Try dir/zipfile/system.png
			if (filerr != osd_file::error::NONE)
			{
				fname = string(system_name) + ext;
				filerr = OpenZipDIBFile(partpath, zip_name, fname.c_str(), file, &buffer);
			}

			partpath = strtok(NULL, ";");
		}

		if (filerr == osd_file::error::NONE)
		{
			if (extnum)
				success = jpeg_read_bitmap_gui(*file, phDIB, pPal);
			else
				success = png_read_bitmap_gui(*file, phDIB, pPal);
			file.reset();
		}
		if (success)
			break;
	}

	// free the buffer if we have to
	if (buffer)
		free(buffer);

	return success;
}

// called from winui.cpp and here
HBITMAP DIBToDDB(HDC hDC, HANDLE hDIB, LPMYBITMAPINFO desc)
{
	BITMAPINFO * bmInfo = (LPBITMAPINFO)hDIB;

	if (hDIB == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)hDIB;
	int nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 << lpbi->biBitCount;

	LPVOID lpDIBBits;
	if (bmInfo->bmiHeader.biBitCount > 8)
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo->bmiColors + bmInfo->bmiHeader.biClrUsed) +
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

	HBITMAP hBM = CreateDIBitmap(hDC,  // handle to device context
		(LPBITMAPINFOHEADER)lpbi,      // pointer to bitmap info header
		(LONG)CBM_INIT,                // initialization flag
		lpDIBBits,                     // pointer to initialization data
		(LPBITMAPINFO)lpbi,            // pointer to bitmap info
		DIB_RGB_COLORS);               // color-data usage

	return hBM;
}



// main call from winui to display a picture
BOOL LoadScreenShot(int nGame, LPCSTR lpSoftwareName, int nType)
{
	/* Delete the last ones */
	//printf("LoadScreenShot: A\n");fflush(stdout);
	FreeScreenShot();

	//printf("LoadScreenShot: B\n");fflush(stdout);
	BOOL loaded = false;
	BOOL isclone = DriverIsClone(nGame);
	int nParentIndex = -1;
	if (isclone)
		nParentIndex = GetParentIndex(&driver_list::driver(nGame));

	// If software item, see if picture exist (correct parent is passed in lpSoftwareName)
	//printf("LoadScreenShot: C\n");fflush(stdout);
	if (lpSoftwareName)
		loaded = LoadDIB(lpSoftwareName, &m_hDIB, &m_hPal, nType);

	// If game, see if picture exist. Or, if no picture for the software, use game's picture.
	//printf("LoadScreenShot: D\n");fflush(stdout);
	if (!loaded)
	{
		//printf("LoadScreenShot: E\n");fflush(stdout);
		loaded = LoadDIB(driver_list::driver(nGame).name, &m_hDIB, &m_hPal, nType);
		// none? try parent
		//printf("LoadScreenShot: F\n");fflush(stdout);
		if (!loaded && isclone)
		{
			//printf("LoadScreenShot: G\n");fflush(stdout);
			loaded = LoadDIB(driver_list::driver(nParentIndex).name, &m_hDIB, &m_hPal, nType);
		}
	}

	//printf("LoadScreenShot: K\n");fflush(stdout);
	if (loaded)
	{
		HDC hdc = GetDC(GetMainWindow());
		m_hDDB = DIBToDDB(hdc, m_hDIB, NULL);
		ReleaseDC(GetMainWindow(),hdc);
	}

	printf("LoadScreenShot: Finished\n");fflush(stdout);
	return loaded;
}

// called from winui.cpp to display the background
BOOL LoadDIBBG(HGLOBAL *phDIB, HPALETTE *pPal)
{
	util::core_file::ptr file = NULL;
	BOOL success = false;
	if (pPal)
		DeletePalette(pPal);

	// look for the raw file
	string fname = GetBgDir();
	if (util::core_file::open(fname.c_str(), OPEN_FLAG_READ, file) == osd_file::error::NONE)
	{
		success = png_read_bitmap_gui(*file, phDIB, pPal);
		file.reset();
	}

	return success;
}

