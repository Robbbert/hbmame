// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

/***************************************************************************

  treeview.c

  TreeView support routines - MSH 11/19/1998

***************************************************************************/

// standard windows headers
#include <windows.h>
#include <windowsx.h>

// standard C headers
#include <sys/stat.h>

#ifdef _MSC_VER
#include <direct.h>
#endif
#include <tchar.h>

// MAME/MAMEUI headers
#include "emu.h"
#include "mui_util.h"
#include "winui.h"
#include "treeview.h"
#include "resource.h"
#include "mui_opts.h"
#include "emu_opts.h"
#include "dialogs.h"
#include "winutf8.h"
#include "screen.h"
#include "drivenum.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#define MAX_EXTRA_FOLDERS 256

/***************************************************************************
    public structures
 ***************************************************************************/

#define ICON_MAX (sizeof(treeIconNames) / sizeof(treeIconNames[0]))

/* Name used for user-defined custom icons */
/* external *.ico file to look for. */

typedef struct
{
	int nResourceID;
	LPCSTR lpName;
} TREEICON;

static TREEICON treeIconNames[] =
{
	{ IDI_FOLDER_OPEN,         "foldopen" },
	{ IDI_FOLDER,              "folder" },
	{ IDI_FOLDER_AVAILABLE,    "foldavail" },
	{ IDI_FOLDER_MANUFACTURER, "foldmanu" },
	{ IDI_FOLDER_UNAVAILABLE,  "foldunav" },
	{ IDI_FOLDER_YEAR,         "foldyear" },
	{ IDI_FOLDER_SOURCE,       "foldsrc" },
	{ IDI_FOLDER_HORIZONTAL,   "horz" },
	{ IDI_FOLDER_VERTICAL,     "vert" },
	{ IDI_MANUFACTURER,        "manufact" },
	{ IDI_FOLDER_WORKING,      "working" },
	{ IDI_FOLDER_NONWORKING,   "nonwork" },
	{ IDI_YEAR,                "year" },
	{ IDI_SOUND,               "sound" },
	{ IDI_CPU,                 "cpu" },
	{ IDI_FOLDER_HARDDISK,     "harddisk" },
	{ IDI_SOURCE,              "source" }
};

/***************************************************************************
    private variables
 ***************************************************************************/

/* this has an entry for every folder eventually in the UI, including subfolders */
static TREEFOLDER **treeFolders = 0;
static UINT         numFolders  = 0;        /* Number of folder in the folder array */
static UINT         next_folder_id = MAX_FOLDERS;
static UINT         folderArrayLength = 0;  /* Size of the folder array */
static LPTREEFOLDER lpCurrentFolder = 0;    /* Currently selected folder */
static UINT         nCurrentFolder = 0;     /* Current folder ID */
static WNDPROC      g_lpTreeWndProc = 0;    /* for subclassing the TreeView */
static HIMAGELIST   hTreeSmall = 0;         /* TreeView Image list of icons */

/* this only has an entry for each TOP LEVEL extra folder + SubFolders*/
LPEXFOLDERDATA ExtraFolderData[MAX_EXTRA_FOLDERS * MAX_EXTRA_SUBFOLDERS];
static int numExtraFolders = 0;
static int numExtraIcons = 0;
static char *ExtraFolderIcons[MAX_EXTRA_FOLDERS];

// built in folders and filters
static LPCFOLDERDATA  g_lpFolderData;
static LPCFILTER_ITEM g_lpFilterList;

/***************************************************************************
    private function prototypes
 ***************************************************************************/

extern BOOL InitFolders(void);
static BOOL CreateTreeIcons(void);
static void TreeCtrlOnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static const char *ParseManufacturer(const char *s, int *pParsedChars );
static const char *TrimManufacturer(const char *s);
static BOOL AddFolder(LPTREEFOLDER lpFolder);
static LPTREEFOLDER NewFolder(const char *lpTitle, UINT nFolderId, int nParent, UINT nIconId, DWORD dwFlags);
static void DeleteFolder(LPTREEFOLDER lpFolder);
static LRESULT CALLBACK TreeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static int InitExtraFolders(void);
static void FreeExtraFolders(void);
static void SetExtraIcons(char *name, int *id);
static BOOL TryAddExtraFolderAndChildren(int parent_index);
static BOOL TrySaveExtraFolder(LPTREEFOLDER lpFolder);
static void SaveExternalFolders(int parent_index, const char *fname);

/***************************************************************************
    public functions
 ***************************************************************************/

BOOL win_move_file_utf8(const char *existingfilename, const char *newfilename)
{
	BOOL result = false;

	TCHAR *t_existingfilename = ui_wstring_from_utf8(existingfilename);
	if( !t_existingfilename )
		return result;

	TCHAR *t_newfilename = ui_wstring_from_utf8(newfilename);
	if( !t_newfilename ) {
		free(t_existingfilename);
		return result;
	}

	result = MoveFile(t_existingfilename, t_newfilename);

	free(t_newfilename);
	free(t_existingfilename);

	return result;
}

/**************************************************************************
 *      ci_strncmp - case insensitive character array compare
 *
 *      Returns zero if the first n characters of s1 and s2 are equal,
 *      ignoring case.
 *      stolen from datafile.c
 **************************************************************************/
static int ci_strncmp (const char *s1, const char *s2, int n)
{
	int c1 = 0, c2 = 0;
	while (n)
	{
		if ((c1 = tolower (*s1)) != (c2 = tolower (*s2)))
			return (c1 - c2);
		else
		if (!c1)
			break;
		--n;
		s1++;
		s2++;
	}
	return 0;
}



/* De-allocate all folder memory */
void FreeFolders(void)
{
	if (treeFolders)
	{
		if (numExtraFolders)
		{
			FreeExtraFolders();
			numFolders -= numExtraFolders;
		}

		for (int i = numFolders - 1; i >= 0; i--)
		{
			DeleteFolder(treeFolders[i]);
			treeFolders[i] = NULL;
			numFolders--;
		}
		free(treeFolders);
		treeFolders = NULL;
	}
	numFolders = 0;
}

/* Reset folder filters */
void ResetFilters(void)
{
	if (treeFolders > 0)
		for (int i = 0; i < (int)numFolders; i++)
			treeFolders[i]->m_dwFlags &= ~F_MASK;
}

void InitTree(LPCFOLDERDATA lpFolderData, LPCFILTER_ITEM lpFilterList)
{
	g_lpFolderData = lpFolderData;
	g_lpFilterList = lpFilterList;

	InitFolders();

	/* this will subclass the treeview (where WM_DRAWITEM gets sent for the header control) */
	LONG_PTR l = GetWindowLongPtr(GetTreeView(), GWLP_WNDPROC);
	g_lpTreeWndProc = (WNDPROC)l;
	SetWindowLongPtr(GetTreeView(), GWLP_WNDPROC, (LONG_PTR)TreeWndProc);
}

void SetCurrentFolder(LPTREEFOLDER lpFolder)
{
	lpCurrentFolder = (lpFolder == 0) ? treeFolders[0] : lpFolder;
	nCurrentFolder = (lpCurrentFolder) ? lpCurrentFolder->m_nFolderId : 0;
}

LPTREEFOLDER GetCurrentFolder(void)
{
	return lpCurrentFolder;
}

UINT GetCurrentFolderID(void)
{
	return nCurrentFolder;
}

int GetNumFolders(void)
{
	return numFolders;
}

LPTREEFOLDER GetFolder(UINT nFolder)
{
	return (nFolder < numFolders) ? treeFolders[nFolder] : NULL;
}

LPTREEFOLDER GetFolderByID(UINT nID)
{
	for (UINT i = 0; i < numFolders; i++)
		if (treeFolders[i]->m_nFolderId == nID)
			return treeFolders[i];

	return (LPTREEFOLDER)0;
}

void AddGame(LPTREEFOLDER lpFolder, UINT nGame)
{
	if (lpFolder)
		SetBit(lpFolder->m_lpGameBits, nGame);
}

void RemoveGame(LPTREEFOLDER lpFolder, UINT nGame)
{
	ClearBit(lpFolder->m_lpGameBits, nGame);
}

int FindGame(LPTREEFOLDER lpFolder, int nGame)
{
	return FindBit(lpFolder->m_lpGameBits, nGame, true);
}

// Called to re-associate games with folders
void ResetWhichGamesInFolders(void)
{
	int nGames = driver_list::total();

	for (UINT i = 0; i < numFolders; i++)
	{
		LPTREEFOLDER lpFolder = treeFolders[i];
		// setup the games in our built-in folders
		for (UINT k = 0; g_lpFolderData[k].m_lpTitle; k++)
		{
			if (lpFolder->m_nFolderId == g_lpFolderData[k].m_nFolderId)
			{
				if (g_lpFolderData[k].m_pfnQuery || g_lpFolderData[k].m_bExpectedResult)
				{
					SetAllBits(lpFolder->m_lpGameBits, false);
					for (UINT jj = 0; jj < nGames; jj++)
					{
						// invoke the query function
						BOOL b = g_lpFolderData[k].m_pfnQuery ? g_lpFolderData[k].m_pfnQuery(jj) : true;

						// if we expect false, flip the result
						if (!g_lpFolderData[k].m_bExpectedResult)
							b = !b;

						// if we like what we hear, add the game
						if (b)
							AddGame(lpFolder, jj);
					}
				}
				break;
			}
		}
	}
}


/* Used to build the GameList */
BOOL GameFiltered(int nGame, DWORD dwMask)
{
	LPTREEFOLDER lpFolder = GetCurrentFolder();
	LPTREEFOLDER lpParent = NULL;

	//Filter out the Bioses on all Folders, except for the Bios Folder
	if( lpFolder->m_nFolderId != FOLDER_BIOS )
	{
//      if( !( (driver_list::driver(nGame).flags & MACHINE_IS_BIOS_ROOT ) == 0) )
//          return true;
		if( driver_list::driver(nGame).name[0] == '_' )
			return true;
	}
	// Filter games--return true if the game should be HIDDEN in this view
	if( GetFilterInherit() )
	{
		if( lpFolder )
		{
			lpParent = GetFolder( lpFolder->m_nParent );
			if( lpParent )
			{
                /* Check the Parent Filters and inherit them on child,
                 * The inherited filters don't display on the custom Filter Dialog for the Child folder
                 * No need to promote all games to parent folder, works as is */
				dwMask |= lpParent->m_dwFlags;
			}
		}
	}

	if (strlen(GetSearchText()) && _stricmp(GetSearchText(), SEARCH_PROMPT))
		if (MyStrStrI(driver_list::driver(nGame).type.fullname(),GetSearchText()) == NULL &&
			MyStrStrI(driver_list::driver(nGame).name,GetSearchText()) == NULL)
				return true;

	/*Filter Text is already global*/
	if (MyStrStrI(driver_list::driver(nGame).type.fullname(),GetFilterText()) == NULL &&
		MyStrStrI(driver_list::driver(nGame).name,GetFilterText()) == NULL &&
		MyStrStrI(driver_list::driver(nGame).type.source(),GetFilterText()) == NULL &&
		MyStrStrI(driver_list::driver(nGame).manufacturer,GetFilterText()) == NULL)
	{
		return true;
	}
	// Are there filters set on this folder?
	if ((dwMask & F_MASK) == 0)
		return false;

	// Filter out clones?
	if (dwMask & F_CLONES && DriverIsClone(nGame))
		return true;

	for (int i = 0; g_lpFilterList[i].m_dwFilterType; i++)
		if (dwMask & g_lpFilterList[i].m_dwFilterType)
			if (g_lpFilterList[i].m_pfnQuery(nGame) == g_lpFilterList[i].m_bExpectedResult)
				return true;

	return false;
}

/* Get the parent of game in this view */
BOOL GetParentFound(int nGame) // not used
{
	LPTREEFOLDER lpFolder = GetCurrentFolder();

	if( lpFolder )
	{
		int nParentIndex = GetParentIndex(&driver_list::driver(nGame));

		/* return false if no parent is there in this view */
		if( nParentIndex == -1)
			return false;

		/* return false if the folder should be HIDDEN in this view */
		if (TestBit(lpFolder->m_lpGameBits, nParentIndex) == 0)
			return false;

		/* return false if the game should be HIDDEN in this view */
		if (GameFiltered(nParentIndex, lpFolder->m_dwFlags))
			return false;

		return true;
	}

	return false;
}

LPCFILTER_ITEM GetFilterList(void)
{
	return g_lpFilterList;
}

/***************************************************************************
    private functions
 ***************************************************************************/

void CreateSourceFolders(int parent_index)
{
	int i, k=0;
	int nGames = driver_list::total();
	int start_folder = numFolders;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits,false);
	for (int jj = 0; jj < nGames; jj++)
	{
		const char *s = GetDriverFilename(jj);

		if (s == NULL || s[0] == '\0')
			continue;

		// look for an existant source treefolder for this game
		// (likely to be the previous one, so start at the end)
		for (i=numFolders-1;i>=start_folder;i--)
		{
			if (strcmp(treeFolders[i]->m_lpTitle,s) == 0)
			{
				AddGame(treeFolders[i], jj);
				break;
			}
		}

		if (i == start_folder-1)
		{
			// nope, it's a source file we haven't seen before, make it.
			LPTREEFOLDER lpTemp = NewFolder(s, next_folder_id, parent_index, IDI_SOURCE, GetFolderFlags(numFolders));
			ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
			memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

			ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
			ExtraFolderData[next_folder_id]->m_nIconId = IDI_SOURCE;
			ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
			ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
			strcpy( ExtraFolderData[next_folder_id]->m_szTitle, s );
			ExtraFolderData[next_folder_id]->m_dwFlags = 0;

			// Increment next_folder_id here in case code is added above
			next_folder_id++;

			AddFolder(lpTemp);
			AddGame(lpTemp, jj);
		}
	}
	SetNumOptionFolders(k-1);
	const char *fname = "Source";
	SaveExternalFolders(parent_index, fname);
}

void CreateScreenFolders(int parent_index)
{
	int i, k=0;
	int nGames = driver_list::total();
	int start_folder = numFolders;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits,false);
	for (int jj = 0; jj < nGames; jj++)
	{
		int screens = DriverNumScreens(jj);
		char s[2];
		itoa(screens, s, 10);

		// look for an existant screens treefolder for this game
		// (likely to be the previous one, so start at the end)
		for (i=numFolders-1;i>=start_folder;i--)
		{
			if (strcmp(treeFolders[i]->m_lpTitle,s) == 0)
			{
				AddGame(treeFolders[i], jj);
				break;
			}
		}

		if (i == start_folder-1)
		{
			// nope, it's a screen file we haven't seen before, make it.
			LPTREEFOLDER lpTemp = NewFolder(s, next_folder_id, parent_index, IDI_SCREEN, GetFolderFlags(numFolders));
			ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
			memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

			ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
			ExtraFolderData[next_folder_id]->m_nIconId = IDI_SCREEN;
			ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
			ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
			strcpy( ExtraFolderData[next_folder_id]->m_szTitle, s );
			ExtraFolderData[next_folder_id]->m_dwFlags = 0;

			// Increment next_folder_id here in case code is added above
			next_folder_id++;

			AddFolder(lpTemp);
			AddGame(lpTemp, jj);
		}
	}
	SetNumOptionFolders(k-1);
	const char *fname = "Screen";
	SaveExternalFolders(parent_index, fname);
}


void CreateManufacturerFolders(int parent_index)
{
	int i;
	int nGames = driver_list::total();
	int start_folder = numFolders;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits,false);

	for (int jj = 0; jj < nGames; jj++)
	{
		const char *manufacturer = driver_list::driver(jj).manufacturer;
		int iChars = 0;
		while( manufacturer != NULL && manufacturer[0] != '\0' )
		{
			const char *s = ParseManufacturer(manufacturer, &iChars);
			manufacturer += iChars;
			//shift to next start char
			if( s != NULL && *s != 0 )
			{
				const char *t = TrimManufacturer(s);
				for (i=numFolders-1;i>=start_folder;i--)
				{
					//RS Made it case insensitive
					if (ci_strncmp(treeFolders[i]->m_lpTitle,t,20) == 0 )
					{
						AddGame(treeFolders[i],jj);
						break;
					}
				}

				if (i == start_folder-1)
				{
					// nope, it's a manufacturer we haven't seen before, make it.
					LPTREEFOLDER lpTemp = NewFolder(t, next_folder_id, parent_index, IDI_MANUFACTURER, GetFolderFlags(numFolders));
					ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
					memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

					ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
					ExtraFolderData[next_folder_id]->m_nIconId = IDI_MANUFACTURER;
					ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
					ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
					strcpy( ExtraFolderData[next_folder_id]->m_szTitle, s );
					ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
					AddFolder(lpTemp);
					AddGame(lpTemp,jj);
				}
			}
		}
	}
	const char *fname = "Manufacturer";
	SaveExternalFolders(parent_index, fname);
}

/* Make a reasonable name out of the one found in the driver array */
static const char *ParseManufacturer(const char *s, int *pParsedChars )
{
	static char tmp[256];
	char *ptmp;
	char *t;
	*pParsedChars= 0;

	if ( *s == '?' || *s == '<' || s[3] == '?' )
	{
		(*pParsedChars) = strlen(s);
		return "<unknown>";
	}

	ptmp = tmp;
	/*if first char is a space, skip it*/
	if( *s == ' ' )
	{
		(*pParsedChars)++;
		++s;
	}
	while( *s )
	{
		/* combinations where to end string */

		if (
			( (*s == ' ') && ( s[1] == '(' || s[1] == '/' || s[1] == '+' ) ) ||
			( *s == ']' ) || ( *s == '/' ) || ( *s == '?' ) )
		{
		(*pParsedChars)++;
			if( s[1] == '/' || s[1] == '+' )
				(*pParsedChars)++;
			break;
		}
		if( s[0] == ' ' && s[1] == '?' )
		{
			(*pParsedChars) += 2;
			s+=2;
		}

		/* skip over opening braces*/

		if ( *s != '[' )
		{
			*ptmp++ = *s;
		}
		(*pParsedChars)++;
		/*for "distributed by" and "supported by" handling*/
		if( ( (s[1] == ',') && (s[2] == ' ') && ( (s[3] == 's') || (s[3] == 'd') ) ) )
		{
			//*ptmp++ = *s;
			++s;
			break;
	}
		++s;
	}
	*ptmp = '\0';
	t = tmp;
	if( tmp[0] == '(' || tmp[strlen(tmp)-1] == ')' || tmp[0] == ',')
	{
		ptmp = strchr( tmp,'(' );
		if ( ptmp == NULL )
		{
			ptmp = strchr( tmp,',' );
			if( ptmp != NULL)
			{
				//parse the new "supported by" and "distributed by"
				ptmp++;

				if (ci_strncmp(ptmp, " supported by", 13) == 0)
				{
					ptmp += 13;
				}
				else if (ci_strncmp(ptmp, " distributed by", 15) == 0)
				{
					ptmp += 15;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				ptmp = tmp;
				if ( ptmp == NULL )
				{
					return NULL;
				}
			}
		}
		if( tmp[0] == '(' || tmp[0] == ',')
		{
			ptmp++;
		}
		if (ci_strncmp(ptmp, "licensed from ", 14) == 0)
		{
			ptmp += 14;
		}
		// for the licenced from case
		if (ci_strncmp(ptmp, "licenced from ", 14) == 0)
		{
			ptmp += 14;
		}

		while ( (*ptmp != ')' ) && (*ptmp != '/' ) && *ptmp != '\0')
		{
			if (*ptmp == ' ' && ci_strncmp(ptmp, " license", 8) == 0)
			{
				break;
			}
			if (*ptmp == ' ' && ci_strncmp(ptmp, " licence", 8) == 0)
			{
				break;
			}
			*t++ = *ptmp++;
		}

		*t = '\0';
	}

	*ptmp = '\0';
	return tmp;
}

/* Analyze Manufacturer Names for typical patterns, that don't distinguish between companies (e.g. Co., Ltd., Inc., etc. */
static const char *TrimManufacturer(const char *s)
{
	//Also remove Country specific suffixes (e.g. Japan, Italy, America, USA, ...)
	char strTemp[256];
	static char strTemp2[256];
	int j, k ,l;
	memset(strTemp, '\0', 256 );
	memset(strTemp2, '\0', 256 );
	//start analyzing from the back, as these are usually suffixes
	for(int i = strlen(s)-1; i>=0; i-- )
	{
		l = strlen(strTemp);
		for(k=l; k>=0; k--)
			strTemp[k+1] = strTemp[k];
		strTemp[0] = s[i];
		strTemp[++l] = '\0';
		switch (l)
		{
			case 2:
				if( ci_strncmp(strTemp, "co", 2) == 0 )
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 3:
				if( ci_strncmp(strTemp, "co.", 3) == 0 || ci_strncmp(strTemp, "ltd", 3) == 0 || ci_strncmp(strTemp, "inc", 3) == 0  || ci_strncmp(strTemp, "SRL", 3) == 0 || ci_strncmp(strTemp, "USA", 3) == 0)
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 4:
				if( ci_strncmp(strTemp, "inc.", 4) == 0 || ci_strncmp(strTemp, "ltd.", 4) == 0 || ci_strncmp(strTemp, "corp", 4) == 0 || ci_strncmp(strTemp, "game", 4) == 0)
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 5:
				if( ci_strncmp(strTemp, "corp.", 5) == 0 || ci_strncmp(strTemp, "Games", 5) == 0 || ci_strncmp(strTemp, "Italy", 5) == 0 || ci_strncmp(strTemp, "Japan", 5) == 0)
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 6:
				if( ci_strncmp(strTemp, "co-ltd", 6) == 0 || ci_strncmp(strTemp, "S.R.L.", 6) == 0)
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 7:
				if( ci_strncmp(strTemp, "co. ltd", 7) == 0  || ci_strncmp(strTemp, "America", 7) == 0)
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 8:
				if( ci_strncmp(strTemp, "co. ltd.", 8) == 0  )
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 9:
				if( ci_strncmp(strTemp, "co., ltd.", 9) == 0 || ci_strncmp(strTemp, "gmbh & co", 9) == 0 )
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 10:
				if( ci_strncmp(strTemp, "corp, ltd.", 10) == 0  || ci_strncmp(strTemp, "industries", 10) == 0  || ci_strncmp(strTemp, "of America", 10) == 0)
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 11:
				if( ci_strncmp(strTemp, "corporation", 11) == 0 || ci_strncmp(strTemp, "enterprises", 11) == 0 )
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			case 16:
				if( ci_strncmp(strTemp, "industries japan", 16) == 0 )
				{
					j=l;
					while( s[strlen(s)-j-1] == ' ' || s[strlen(s)-j-1] == ',' )
					{
						j++;
					}
					if( j!=l)
					{
						memset(strTemp2, '\0', 256 );
						strncpy(strTemp2, s, strlen(s)-j );
					}
				}
				break;
			default:
				break;
		}
	}
	if( *strTemp2 == 0 )
		return s;
	return strTemp2;
}

void CreateBIOSFolders(int parent_index)
{
	int i, nGames = driver_list::total();
	int start_folder = numFolders;
	const game_driver *drv;
	int nParentIndex = -1;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits, false);

	for (int jj = 0; jj < nGames; jj++)
	{
		if ( DriverIsClone(jj) )
		{
			nParentIndex = GetParentIndex(&driver_list::driver(jj));
			if (nParentIndex < 0) return;
			drv = &driver_list::driver(nParentIndex);
		}
		else
			drv = &driver_list::driver(jj);
		nParentIndex = GetParentIndex(drv);

		if (nParentIndex < 0 || !driver_list::driver(nParentIndex).type.fullname())
			continue;

		for (i = numFolders-1; i >= start_folder; i--)
		{
			if (strcmp(treeFolders[i]->m_lpTitle, driver_list::driver(nParentIndex).type.fullname()) == 0)
			{
				AddGame(treeFolders[i], jj);
				break;
			}
		}

		if (i == start_folder-1)
		{
			LPTREEFOLDER lpTemp = NewFolder(driver_list::driver(nParentIndex).type.fullname(), next_folder_id++, parent_index, IDI_CPU, GetFolderFlags(numFolders));
			AddFolder(lpTemp);
			AddGame(lpTemp, jj);
		}
	}
	const char *fname = "BIOS";
	SaveExternalFolders(parent_index, fname);
}

void CreateCPUFolders(int parent_index)
{
	int device_folder_count = 0;
	LPTREEFOLDER device_folders[512];
	LPTREEFOLDER folder;
	int nFolder = numFolders;

	for (int i = 0; i < driver_list::total(); i++)
	{
		machine_config config(driver_list::driver(i),MameUIGlobal());

		// enumerate through all devices
		for (device_execute_interface &device : execute_interface_iterator(config.root_device()))
		{
			// get the name
			std::string dev_name = device.device().name();

			// do we have a folder for this device?
			folder = NULL;
			for (int j = 0; j < device_folder_count; j++)
			{
				if (!strcmp(dev_name.c_str(), device_folders[j]->m_lpTitle))
				{
					folder = device_folders[j];
					break;
				}
			}

			// are we forced to create a folder?
			if (folder == NULL)
			{
				LPTREEFOLDER lpTemp = NewFolder(device.device().name(), next_folder_id, parent_index, IDI_CPU, GetFolderFlags(numFolders));
				ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
				memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));
				ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
				ExtraFolderData[next_folder_id]->m_nIconId = IDI_CPU;
				ExtraFolderData[next_folder_id]->m_nParent = treeFolders[parent_index]->m_nFolderId;
				ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
				strcpy( ExtraFolderData[next_folder_id]->m_szTitle, device.device().name() );
				ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
				AddFolder(lpTemp);
				folder = treeFolders[nFolder++];

				// record that we found this folder
				device_folders[device_folder_count++] = folder;
			}

			// cpu type #'s are one-based
			AddGame(folder, i);
		}
	}
	const char *fname = "CPU";
	SaveExternalFolders(parent_index, fname);
}

void CreateSoundFolders(int parent_index)
{
	int device_folder_count = 0;
	LPTREEFOLDER device_folders[512];
	LPTREEFOLDER folder;
	int nFolder = numFolders;

	for (int i = 0; i < driver_list::total(); i++)
	{
		machine_config config(driver_list::driver(i),MameUIGlobal());

		// enumerate through all devices

		for (device_sound_interface &device : sound_interface_iterator(config.root_device()))
		{
			// get the name
			std::string dev_name = device.device().name();

			// do we have a folder for this device?
			folder = NULL;
			for (int j = 0; j < device_folder_count; j++)
			{
				if (!strcmp(dev_name.c_str(), device_folders[j]->m_lpTitle))
				{
					folder = device_folders[j];
					break;
				}
			}

			// are we forced to create a folder?
			if (folder == NULL)
			{
				LPTREEFOLDER lpTemp = NewFolder(device.device().name(), next_folder_id, parent_index, IDI_SOUND, GetFolderFlags(numFolders));
				ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
				memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

				ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
				ExtraFolderData[next_folder_id]->m_nIconId = IDI_SOUND;
				ExtraFolderData[next_folder_id]->m_nParent = treeFolders[parent_index]->m_nFolderId;
				ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
				strcpy( ExtraFolderData[next_folder_id]->m_szTitle, device.device().name() );
				ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
				AddFolder(lpTemp);
				folder = treeFolders[nFolder++];

				// record that we found this folder
				device_folders[device_folder_count++] = folder;
			}

			// cpu type #'s are one-based
			AddGame(folder, i);
		}
	}
	const char *fname = "Sound";
	SaveExternalFolders(parent_index, fname);
}

void CreateDeficiencyFolders(int parent_index)
{
	int nGames = driver_list::total();
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// create our subfolders
	LPTREEFOLDER lpProt, lpWrongCol, lpImpCol, lpImpGraph, lpMissSnd, lpImpSnd, lpFlip, lpArt;
	lpProt = NewFolder("Unemulated Protection", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Unemulated Protection" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpProt);
	lpWrongCol = NewFolder("Wrong Colors", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Wrong Colors" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpWrongCol);

	lpImpCol = NewFolder("Imperfect Colors", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Imperfect Colors" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpImpCol);

	lpImpGraph = NewFolder("Imperfect Graphics", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Imperfect Graphics" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpImpGraph);

	lpMissSnd = NewFolder("Missing Sound", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Missing Sound" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpMissSnd);

	lpImpSnd = NewFolder("Imperfect Sound", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Imperfect Sound" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpImpSnd);

	lpFlip = NewFolder("No Cocktail", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "No Cocktail" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpFlip);

	lpArt = NewFolder("Requires Artwork", next_folder_id, parent_index, IDI_FOLDER, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA) );
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Requires Artwork" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpArt);
	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits,false);

	for (int jj = 0; jj < nGames; jj++)
	{
		uint32_t cache = GetDriverCacheLower(jj);
		if (BIT(cache, 21))
			AddGame(lpWrongCol,jj);

		if (BIT(cache, 22))
			AddGame(lpProt,jj);

		if (BIT(cache, 20))
			AddGame(lpImpCol,jj);

		if (BIT(cache, 18))
			AddGame(lpImpGraph,jj);

		if (BIT(cache, 17))
			AddGame(lpMissSnd,jj);

		if (BIT(cache, 16))
			AddGame(lpImpSnd,jj);

		if (BIT(cache, 8))
			AddGame(lpFlip,jj);

		if (BIT(cache, 10))
			AddGame(lpArt,jj);
	}
}

void CreateDumpingFolders(int parent_index)
{
	BOOL bBadDump  = false;
	BOOL bNoDump = false;
	int nGames = driver_list::total();
	LPTREEFOLDER lpFolder = treeFolders[parent_index];
	const rom_entry *rom;
	const game_driver *gamedrv;

	// create our two subfolders
	LPTREEFOLDER lpBad, lpNo;
	lpBad = NewFolder("Bad Dump", next_folder_id, parent_index, IDI_FOLDER_DUMP, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER_DUMP;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "Bad Dump" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpBad);
	lpNo = NewFolder("No Dump", next_folder_id, parent_index, IDI_FOLDER_DUMP, GetFolderFlags(numFolders));
	ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
	memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

	ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
	ExtraFolderData[next_folder_id]->m_nIconId = IDI_FOLDER_DUMP;
	ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
	ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
	strcpy( ExtraFolderData[next_folder_id]->m_szTitle, "No Dump" );
	ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
	AddFolder(lpNo);

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits,false);

	for (int jj = 0; jj < nGames; jj++)
	{
		gamedrv = &driver_list::driver(jj);

		if (!gamedrv->rom)
			continue;
		bBadDump = false;
		bNoDump = false;
		/* Allocate machine config */
		machine_config config(*gamedrv,MameUIGlobal());

		for (device_t &device : device_iterator(config.root_device()))
		{
			for (const rom_entry *region = rom_first_region(device); region; region = rom_next_region(region))
			{
				for (rom = rom_first_file(region); rom; rom = rom_next_file(rom))
				{
					if (ROMREGION_ISROMDATA(region) || ROMREGION_ISDISKDATA(region) )
					{
						//name = ROM_GETNAME(rom);
						util::hash_collection hashes(ROM_GETHASHDATA(rom));
						if (hashes.flag(util::hash_collection::FLAG_BAD_DUMP))
							bBadDump = true;
						if (hashes.flag(util::hash_collection::FLAG_NO_DUMP))
							bNoDump = true;
					}
				}
			}
		}
		if (bBadDump)
			AddGame(lpBad,jj);

		if (bNoDump)
			AddGame(lpNo,jj);
	}
	const char *fname = "Dumping";
	SaveExternalFolders(parent_index, fname);
}


void CreateYearFolders(int parent_index)
{
	int i,jj;
	int nGames = driver_list::total();
	int start_folder = numFolders;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits, false);

	for (jj = 0; jj < nGames; jj++)
	{
		char s[strlen(driver_list::driver(jj).year)+1];
		strcpy(s,driver_list::driver(jj).year);

		if (s[0] == '\0' || s[0] == '?')
			continue;

		if (s[4] == '?')
			s[4] = '\0';

		// look for an extant year treefolder for this game
		// (likely to be the previous one, so start at the end)
		for (i=numFolders-1;i>=start_folder;i--)
		{
			if (strncmp(treeFolders[i]->m_lpTitle, s, 4) == 0)
			{
				AddGame(treeFolders[i], jj);
				break;
			}
		}
		if (i == start_folder-1)
		{
			// nope, it's a year we haven't seen before, make it.
			LPTREEFOLDER lpTemp;
			lpTemp = NewFolder(s, next_folder_id, parent_index, IDI_YEAR, GetFolderFlags(numFolders));
			ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
			memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

			ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
			ExtraFolderData[next_folder_id]->m_nIconId = IDI_YEAR;
			ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
			ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
			strcpy( ExtraFolderData[next_folder_id]->m_szTitle, s );
			ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
			AddFolder(lpTemp);
			AddGame(lpTemp, jj);
		}
	}
	const char *fname = "Year";
	SaveExternalFolders(parent_index, fname);
}

void CreateResolutionFolders(int parent_index)
{
	int i,jj;
	int nGames = driver_list::total();
	int start_folder = numFolders;
	char Screen[2048];
	const game_driver *gamedrv;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits, false);

	for (jj = 0; jj < nGames; jj++)
	{
		gamedrv = &driver_list::driver(jj);
		/* Allocate machine config */
		machine_config config(*gamedrv,MameUIGlobal());

		if (isDriverVector(&config))
			sprintf(Screen, "Vector");
		else
		{
			screen_device_iterator iter(config.root_device());
			const screen_device *screen = iter.first();
			if (screen == NULL)
				strcpy(Screen, "Screenless Game");
			else
			{
				for (screen_device &screen : screen_device_iterator(config.root_device()))
				{
					const rectangle &visarea = screen.visible_area();

					sprintf(Screen,"%d x %d (%c)", visarea.max_y - visarea.min_y + 1, visarea.max_x - visarea.min_x + 1,
						(driver_list::driver(jj).flags & ORIENTATION_SWAP_XY) ? 'V':'H');

					// look for an existant screen treefolder for this game
					// (likely to be the previous one, so start at the end)
					for (i=numFolders-1;i>=start_folder;i--)
					{
						if (strcmp(treeFolders[i]->m_lpTitle, Screen) == 0)
						{
							AddGame(treeFolders[i],jj);
							break;
						}
					}
					if (i == start_folder-1)
					{
						// nope, it's a screen we haven't seen before, make it.
						LPTREEFOLDER lpTemp = NewFolder(Screen, next_folder_id++, parent_index, IDI_SCREEN, GetFolderFlags(numFolders));
						ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
						memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

						ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
						ExtraFolderData[next_folder_id]->m_nIconId = IDI_SCREEN;
						ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
						ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
						strcpy( ExtraFolderData[next_folder_id]->m_szTitle, Screen );
						ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
						AddFolder(lpTemp);
						AddGame(lpTemp,jj);
					}
				}
			}
		}
	}
	const char *fname = "Resolution";
	SaveExternalFolders(parent_index, fname);
}

void CreateFPSFolders(int parent_index)
{
	int i,jj;
	int nGames = driver_list::total();
	int start_folder = numFolders;
	char Screen[2048];
	const game_driver *gamedrv;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	// no games in top level folder
	SetAllBits(lpFolder->m_lpGameBits, false);

	for (jj = 0; jj < nGames; jj++)
	{
		gamedrv = &driver_list::driver(jj);
		/* Allocate machine config */
		machine_config config(*gamedrv,MameUIGlobal());

		if (isDriverVector(&config))
			sprintf(Screen, "Vector");
		else
		{
			screen_device_iterator iter(config.root_device());
			const screen_device *screen = iter.first();
			if (screen == NULL)
				strcpy(Screen, "Screenless Game");
			else
			{
				for (screen_device &screen : screen_device_iterator(config.root_device()))
				{
					sprintf(Screen,"%f Hz", ATTOSECONDS_TO_HZ(screen.refresh_attoseconds()));

					// look for an existant screen treefolder for this game
					// (likely to be the previous one, so start at the end)
					for (i=numFolders-1;i>=start_folder;i--)
					{
						if (strcmp(treeFolders[i]->m_lpTitle, Screen) == 0)
						{
							AddGame(treeFolders[i],jj);
							break;
						}
					}
					if (i == start_folder-1)
					{
						// nope, it's a screen we haven't seen before, make it.
						LPTREEFOLDER lpTemp = NewFolder(Screen, next_folder_id++, parent_index, IDI_SCREEN, GetFolderFlags(numFolders));
						ExtraFolderData[next_folder_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
						memset(ExtraFolderData[next_folder_id], 0, sizeof(EXFOLDERDATA));

						ExtraFolderData[next_folder_id]->m_nFolderId = next_folder_id;
						ExtraFolderData[next_folder_id]->m_nIconId = IDI_SCREEN;
						ExtraFolderData[next_folder_id]->m_nParent = lpFolder->m_nFolderId;
						ExtraFolderData[next_folder_id]->m_nSubIconId = -1;
						strcpy( ExtraFolderData[next_folder_id]->m_szTitle, Screen );
						ExtraFolderData[next_folder_id++]->m_dwFlags = 0;
						AddFolder(lpTemp);
						AddGame(lpTemp,jj);
					}
				}
			}
		}
	}
	const char *fname = "Refresh";
	SaveExternalFolders(parent_index, fname);
}


#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

// adds these folders to the treeview
void ResetTreeViewFolders(void)
{
	HWND hTreeView = GetTreeView();

	// currently "cached" parent
	HTREEITEM shti, hti_parent = NULL;
	int index_parent = -1;

	BOOL res = TreeView_DeleteAllItems(hTreeView);

	//printf("Adding folders to tree ui indices %i to %i\n",start_index,end_index);

	TVINSERTSTRUCT tvs;
	tvs.hInsertAfter = TVI_SORT;

	TVITEM tvi;
	for (int i=0; i<numFolders; i++)
	{
		LPTREEFOLDER lpFolder = treeFolders[i];

		if (lpFolder->m_nParent == -1)
		{
			if (lpFolder->m_nFolderId < MAX_FOLDERS)
				// it's a built in folder, let's see if we should show it
				if (GetShowFolder(lpFolder->m_nFolderId) == false)
					continue;

			tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvs.hParent = TVI_ROOT;
			tvi.pszText = lpFolder->m_lptTitle;
			tvi.lParam = (LPARAM)lpFolder;
			tvi.iImage = GetTreeViewIconIndex(lpFolder->m_nIconId);
			tvi.iSelectedImage = 0;
			tvs.item = tvi;

			// Add root branch
			hti_parent = TreeView_InsertItem(hTreeView, &tvs);

			continue;
		}

		// not a top level branch, so look for parent
		if (treeFolders[i]->m_nParent != index_parent)
		{

			hti_parent = TreeView_GetRoot(hTreeView);
			while (1)
			{
				if (hti_parent == NULL)
					// couldn't find parent folder, so it's a built-in but
					// not shown folder
					break;

				tvi.hItem = hti_parent;
				tvi.mask = TVIF_PARAM;
				res = TreeView_GetItem(hTreeView,&tvi);
				if (((LPTREEFOLDER)tvi.lParam) == treeFolders[treeFolders[i]->m_nParent])
					break;

				hti_parent = TreeView_GetNextSibling(hTreeView,hti_parent);
			}

			// if parent is not shown, then don't show the child either obviously!
			if (hti_parent == NULL)
				continue;

			index_parent = treeFolders[i]->m_nParent;
		}

		tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvs.hParent = hti_parent;
		tvi.iImage = GetTreeViewIconIndex(treeFolders[i]->m_nIconId);
		tvi.iSelectedImage = 0;
		tvi.pszText = treeFolders[i]->m_lptTitle;
		tvi.lParam = (LPARAM)treeFolders[i];
		tvs.item = tvi;

		// Add it to this tree branch
		shti = TreeView_InsertItem(hTreeView, &tvs); // for current child branches
	}
}
#pragma GCC diagnostic error "-Wunused-but-set-variable"

void SelectTreeViewFolder(int folder_id)
{
	BOOL res = false;
	HWND hTreeView = GetTreeView();
	HTREEITEM hti;
	TVITEM tvi;
	memset(&tvi,0,sizeof(tvi));

	hti = TreeView_GetRoot(hTreeView);

	while (hti != NULL)
	{
		HTREEITEM hti_next;

		tvi.hItem = hti;
		tvi.mask = TVIF_PARAM;
		res = TreeView_GetItem(hTreeView,&tvi);

		if (((LPTREEFOLDER)tvi.lParam)->m_nFolderId == folder_id)
		{
			res = TreeView_SelectItem(hTreeView,tvi.hItem);
			SetCurrentFolder((LPTREEFOLDER)tvi.lParam);
			return;
		}

		hti_next = TreeView_GetChild(hTreeView,hti);
		if (hti_next == NULL)
		{
			hti_next = TreeView_GetNextSibling(hTreeView,hti);
			if (hti_next == NULL)
			{
				hti_next = TreeView_GetParent(hTreeView,hti);
				if (hti_next != NULL)
					hti_next = TreeView_GetNextSibling(hTreeView,hti_next);
			}
		}
		hti = hti_next;
	}

	// could not find folder to select
	// make sure we select something
	tvi.hItem = TreeView_GetRoot(hTreeView);
	tvi.mask = TVIF_PARAM;
	res = TreeView_GetItem(hTreeView,&tvi);
	res = TreeView_SelectItem(hTreeView,tvi.hItem);
	SetCurrentFolder((LPTREEFOLDER)tvi.lParam);
	res++;
}

/*
 * Does this folder have an INI associated with it?
 * Currently only true for FOLDER_VECTOR and children
 * of FOLDER_SOURCE.
 */
static BOOL FolderHasIni(LPTREEFOLDER lpFolder)
{
	LPCFOLDERDATA data = FindFilter(lpFolder->m_nFolderId);
	if (data)
		if (data->m_opttype < OPTIONS_MAX)
			return true;

	if (lpFolder->m_nParent != -1 && FOLDER_SOURCE == treeFolders[lpFolder->m_nParent]->m_nFolderId)
		return true;

	return false;
}

/* Add a folder to the list.  Does not allocate */
static BOOL AddFolder(LPTREEFOLDER lpFolder)
{
	TREEFOLDER **tmpTree = NULL;
	UINT oldFolderArrayLength = folderArrayLength;
	if (numFolders + 1 >= folderArrayLength)
	{
		folderArrayLength += 500;
		tmpTree = (TREEFOLDER **)malloc(sizeof(TREEFOLDER **) * folderArrayLength);
		memcpy(tmpTree,treeFolders,sizeof(TREEFOLDER **) * oldFolderArrayLength);
		if (treeFolders) free(treeFolders);
		treeFolders = tmpTree;
	}

	/* Is there an folder.ini that can be edited? */
	if (FolderHasIni(lpFolder))
		lpFolder->m_dwFlags |= F_INIEDIT;

	treeFolders[numFolders] = lpFolder;
	numFolders++;
	return true;
}

/* Allocate and initialize a NEW TREEFOLDER */
static LPTREEFOLDER NewFolder(const char *lpTitle, UINT nFolderId, int nParent, UINT nIconId, DWORD dwFlags)
{
	LPTREEFOLDER lpFolder = (LPTREEFOLDER)malloc(sizeof(TREEFOLDER));
	memset(lpFolder, '\0', sizeof (TREEFOLDER));
	lpFolder->m_lpTitle = (LPSTR)malloc(strlen(lpTitle) + 1);
	strcpy((char *)lpFolder->m_lpTitle,lpTitle);
	lpFolder->m_lptTitle = ui_wstring_from_utf8(lpFolder->m_lpTitle);
	lpFolder->m_lpGameBits = NewBits(driver_list::total());
	lpFolder->m_nFolderId = nFolderId;
	lpFolder->m_nParent = nParent;
	lpFolder->m_nIconId = nIconId;
	lpFolder->m_dwFlags = dwFlags;
	return lpFolder;
}

/* Deallocate the passed in LPTREEFOLDER */
static void DeleteFolder(LPTREEFOLDER lpFolder)
{
	if (lpFolder)
	{
		if (lpFolder->m_lpGameBits)
		{
			DeleteBits(lpFolder->m_lpGameBits);
			lpFolder->m_lpGameBits = 0;
		}
		free(lpFolder->m_lptTitle);
		lpFolder->m_lptTitle = 0;
		free(lpFolder->m_lpTitle);
		lpFolder->m_lpTitle = 0;
		free(lpFolder);
		lpFolder = 0;
	}
}

/* Can be called to re-initialize the array of treeFolders */
BOOL InitFolders(void)
{
	int i = 0;
	DWORD dwFolderFlags;
	if (treeFolders != NULL)
	{
		for (i = numFolders - 1; i >= 0; i--)
		{
			DeleteFolder(treeFolders[i]);
			treeFolders[i] = 0;
			numFolders--;
		}
	}
	numFolders = 0;
	if (folderArrayLength == 0)
	{
		folderArrayLength = 200;
		treeFolders = (TREEFOLDER **)malloc(sizeof(TREEFOLDER **) * folderArrayLength);
		if (!treeFolders)
		{
			folderArrayLength = 0;
			return 0;
		}
		else
			memset(treeFolders,'\0', sizeof(TREEFOLDER **) * folderArrayLength);
	}
	// built-in top level folders
	for (i = 0; g_lpFolderData[i].m_lpTitle; i++)
	{
		if (RequiredDriverCache() || (!RequiredDriverCache() && !g_lpFolderData[i].m_process))
		{
			LPCFOLDERDATA fData = &g_lpFolderData[i];
			/* get the saved folder flags */
			dwFolderFlags = GetFolderFlags(numFolders);
			/* create the folder */
			AddFolder(NewFolder(fData->m_lpTitle, fData->m_nFolderId, -1, fData->m_nIconId, dwFolderFlags));
		}
	}

	numExtraFolders = InitExtraFolders();

	for (i = 0; i < numExtraFolders; i++)
	{
		LPEXFOLDERDATA fExData = ExtraFolderData[i];
		// OR in the saved folder flags
		dwFolderFlags = fExData->m_dwFlags | GetFolderFlags(numFolders);
		// create the folder, but if we are building the cache, the name must not be a pre-built one
		int k = 0;
		if (RequiredDriverCache())
			for (int j = 0; g_lpFolderData[j].m_lpTitle; j++)
				if (strcmp(fExData->m_szTitle, g_lpFolderData[j].m_lpTitle)==0)
					k++;

		if (k == 0)
			AddFolder(NewFolder(fExData->m_szTitle,fExData->m_nFolderId,fExData->m_nParent, fExData->m_nIconId,dwFolderFlags));
	}

// creates child folders of all the top level folders, including custom ones
	int num_top_level_folders = numFolders;

	for (int i = 0; i < num_top_level_folders; i++)
	{
		LPTREEFOLDER lpFolder = treeFolders[i];
		LPCFOLDERDATA lpFolderData = NULL;

		for (int j = 0; g_lpFolderData[j].m_lpTitle; j++)
		{
			if (g_lpFolderData[j].m_nFolderId == lpFolder->m_nFolderId)
			{
				lpFolderData = &g_lpFolderData[j];
				break;
			}
		}

		if (lpFolderData)
		{
			if (lpFolderData->m_pfnCreateFolders)
			{
				if (RequiredDriverCache() && lpFolderData->m_process) // rebuild cache
					lpFolderData->m_pfnCreateFolders(i);
				else
				if (!lpFolderData->m_process) // build every time (CreateDeficiencyFolders)
					lpFolderData->m_pfnCreateFolders(i);
			}
		}
		else
		{
			if ((lpFolder->m_dwFlags & F_CUSTOM) == 0)
			{
				printf("Internal inconsistency with non-built-in folder, but not custom\n");
				continue;
			}

			//printf("Loading custom folder %i %i\n",i,lpFolder->m_nFolderId);

			// load the extra folder files, which also adds children
			if (TryAddExtraFolderAndChildren(i) == false)
				lpFolder->m_nFolderId = FOLDER_NONE;
		}
	}

	CreateTreeIcons();
	ResetWhichGamesInFolders();
	ResetTreeViewFolders();
	SelectTreeViewFolder(GetSavedFolderID());
	LoadFolderFlags();
	return true;
}

// create iconlist and Treeview control
static BOOL CreateTreeIcons()
{
	HICON hIcon;
	INT i;
	HINSTANCE hInst = GetModuleHandle(0);

	int numIcons = ICON_MAX + numExtraIcons;
	hTreeSmall = ImageList_Create (16, 16, ILC_COLORDDB | ILC_MASK, numIcons, numIcons);

	//printf("Trying to load %i normal icons\n",ICON_MAX);
	for (i = 0; i < ICON_MAX; i++)
	{
		hIcon = LoadIconFromFile(treeIconNames[i].lpName);
		if (!hIcon)
			hIcon = LoadIcon(hInst, MAKEINTRESOURCE(treeIconNames[i].nResourceID));

		if (ImageList_AddIcon (hTreeSmall, hIcon) == -1)
		{
			ErrorMsg("Error creating icon on regular folder, %i %i",i,hIcon != NULL);
			return false;
		}
	}

	//printf("Trying to load %i extra custom-folder icons\n",numExtraIcons);
	for (i = 0; i < numExtraIcons; i++)
	{
		if ((hIcon = LoadIconFromFile(ExtraFolderIcons[i])) == 0)
			hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_FOLDER));

		if (ImageList_AddIcon(hTreeSmall, hIcon) == -1)
		{
			ErrorMsg("Error creating icon on extra folder, %i %i",i,hIcon != NULL);
			return false;
		}
	}

	// Be sure that all the small icons were added.
	if (ImageList_GetImageCount(hTreeSmall) < numIcons)
	{
		ErrorMsg("Error with icon list--too few images.  %i %i", ImageList_GetImageCount(hTreeSmall),numIcons);
		return false;
	}

	// Be sure that all the small icons were added.

	if (ImageList_GetImageCount (hTreeSmall) < ICON_MAX)
	{
		ErrorMsg("Error with icon list--too few images.  %i < %i", ImageList_GetImageCount(hTreeSmall),(INT)ICON_MAX);
		return false;
	}

	// Associate the image lists with the list view control.
	(void)TreeView_SetImageList(GetTreeView(), hTreeSmall, TVSIL_NORMAL);

	return true;
}


static void TreeCtrlOnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rcClip, rcClient;

	HBITMAP hBackground = GetBackgroundBitmap();
	MYBITMAPINFO *bmDesc = GetBackgroundInfo();

	HDC hDC = BeginPaint(hWnd, &ps);

	GetClipBox(hDC, &rcClip);
	GetClientRect(hWnd, &rcClient);

	// Create a compatible memory DC
	HDC memDC = CreateCompatibleDC(hDC);

	// Select a compatible bitmap into the memory DC
	HBITMAP bitmap = CreateCompatibleBitmap(hDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

	// First let the control do its default drawing.
	CallWindowProc(g_lpTreeWndProc, hWnd, uMsg, (WPARAM)memDC, 0);

	// Draw bitmap in the background
	// Now create a mask
	HDC maskDC = CreateCompatibleDC(hDC);

	// Create monochrome bitmap for the mask
	HBITMAP maskBitmap = CreateBitmap(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 1, 1, NULL);

	HBITMAP hOldMaskBitmap = (HBITMAP)SelectObject(maskDC, maskBitmap);
	SetBkColor(memDC, GetSysColor(COLOR_WINDOW));

	// Create the mask from the memory DC
	BitBlt(maskDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC, rcClient.left, rcClient.top, SRCCOPY);

	HDC tempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldHBitmap = (HBITMAP)SelectObject(tempDC, hBackground);

	HDC imageDC = CreateCompatibleDC(hDC);
	HBITMAP bmpImage = CreateCompatibleBitmap(hDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	HBITMAP hOldBmpImage = (HBITMAP)SelectObject(imageDC, bmpImage);

	HPALETTE hPAL = GetBackgroundPalette();
	if (hPAL == NULL)
		hPAL = CreateHalftonePalette(hDC);

	if (GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE && hPAL != NULL)
	{
		SelectPalette(hDC, hPAL, false);
		RealizePalette(hDC);
		SelectPalette(imageDC, hPAL, false);
	}

	// Get x and y offset
	RECT rcRoot;
	TreeView_GetItemRect(hWnd, TreeView_GetRoot(hWnd), &rcRoot, false);
	rcRoot.left = -GetScrollPos(hWnd, SB_HORZ);

	// Draw bitmap in tiled manner to imageDC
	for (int i = rcRoot.left; i < rcClient.right; i += bmDesc->bmWidth)
		for (int j = rcRoot.top; j < rcClient.bottom; j += bmDesc->bmHeight)
			BitBlt(imageDC,  i, j, bmDesc->bmWidth, bmDesc->bmHeight, tempDC, 0, 0, SRCCOPY);

	// Set the background in memDC to black. Using SRCPAINT with black and any other
	// color results in the other color, thus making black the transparent color
	SetBkColor(memDC, RGB(0,0,0));
	SetTextColor(memDC, RGB(255,255,255));
	BitBlt(memDC, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, maskDC, rcClip.left, rcClip.top, SRCAND);

	// Set the foreground to black. See comment above.
	SetBkColor(imageDC, RGB(255,255,255));
	SetTextColor(imageDC, RGB(0,0,0));
	BitBlt(imageDC, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, maskDC, rcClip.left, rcClip.top, SRCAND);

	// Combine the foreground with the background
	BitBlt(imageDC, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, memDC, rcClip.left, rcClip.top, SRCPAINT);

	// Draw the final image to the screen
	BitBlt(hDC, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, imageDC, rcClip.left, rcClip.top, SRCCOPY);

	SelectObject(maskDC, hOldMaskBitmap);
	SelectObject(tempDC, hOldHBitmap);
	SelectObject(imageDC, hOldBmpImage);

	DeleteDC(maskDC);
	DeleteDC(imageDC);
	DeleteDC(tempDC);
	DeleteBitmap(bmpImage);
	DeleteBitmap(maskBitmap);

	if (GetBackgroundPalette() == NULL)
	{
		DeletePalette(hPAL);
		hPAL = NULL;
	}

	SelectObject(memDC, hOldBitmap);
	DeleteBitmap(bitmap);
	DeleteDC(memDC);
	EndPaint(hWnd, &ps);
	ReleaseDC(hWnd, hDC);
}

/* Header code - Directional Arrows */
static LRESULT CALLBACK TreeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (GetBackgroundBitmap() != NULL)
	{
		switch (uMsg)
		{
			case WM_MOUSEMOVE:
			{
				if (MouseHasBeenMoved())
					ShowCursor(true);
				break;
			}

			case WM_KEYDOWN :
				if (wParam == VK_F2)
				{
					if (lpCurrentFolder->m_dwFlags & F_CUSTOM)
					{
						TreeView_EditLabel(hWnd,TreeView_GetSelection(hWnd));
						return true;
					}
				}
				break;

			case WM_ERASEBKGND:
				return true;

			case WM_PAINT:
				TreeCtrlOnPaint(hWnd, uMsg, wParam, lParam);
				UpdateWindow(hWnd);
				break;
		}
	}

	/* message not handled */
	return CallWindowProc(g_lpTreeWndProc, hWnd, uMsg, wParam, lParam);
}

/*
 * Filter code
 * Added 01/09/99 - MSH <mhaaland@hypertech.com>
 */

/* find a FOLDERDATA by folderID */
LPCFOLDERDATA FindFilter(DWORD folderID)
{
	for (int i = 0; g_lpFolderData[i].m_lpTitle; i++)
		if (g_lpFolderData[i].m_nFolderId == folderID)
			return &g_lpFolderData[i];

	return (LPFOLDERDATA) 0;
}

LPTREEFOLDER GetFolderByName(int nParentId, const char *pszFolderName)
{
	//First Get the Parent TreeviewItem
	//Enumerate Children
	for(int i = 0; i < numFolders/* ||treeFolders[i] != NULL*/; i++)
	{
		if (!strcmp(treeFolders[i]->m_lpTitle, pszFolderName))
		{
			int nParent = treeFolders[i]->m_nParent;
			if ((nParent >= 0) && treeFolders[nParent]->m_nFolderId == nParentId)
				return treeFolders[i];
		}
	}
	return NULL;
}

static int InitExtraFolders(void)
{
	struct stat     stat_buffer;
	struct _finddata_t files;
	int             i, count = 0;
	char *          ext;
	char            buf[2048];
	char            curdir[MAX_PATH];
	const std::string    t = dir_get_value(24);
	const char *dir = t.c_str();
	memset(ExtraFolderData, 0, (MAX_EXTRA_FOLDERS * MAX_EXTRA_SUBFOLDERS)* sizeof(LPEXFOLDERDATA));

	/* NPW 9-Feb-2003 - MSVC stat() doesn't like stat() called with an empty string */
	if (!dir)
		dir = ".";

	// Why create the directory if it doesn't exist, just return 0 folders.
	if (stat(dir, &stat_buffer) != 0)
		return 0;

	_getcwd(curdir, MAX_PATH);

	chdir(dir);

	long hLong = _findfirst("*", &files);

	for (i = 0; i < MAX_EXTRA_FOLDERS; i++)
		ExtraFolderIcons[i] = NULL;

	numExtraIcons = 0;

	while (!_findnext(hLong, &files))
	{
		if ((files.attrib & _A_SUBDIR) == 0)
		{
			FILE *fp;

			fp = fopen(files.name, "r");
			if (fp != NULL)
			{
				int icon[2] = { 0, 0 };
				char *p, *name;

				while (fgets(buf, 256, fp))
				{
					if (buf[0] == '[')
					{
						p = strchr(buf, ']');
						if (p == NULL)
							continue;

						*p = '\0';
						name = &buf[1];
						if (!strcmp(name, "FOLDER_SETTINGS"))
						{
							while (fgets(buf, 256, fp))
							{
								name = strtok(buf, " =\r\n");
								if (name == NULL)
									break;

								if (!strcmp(name, "RootFolderIcon"))
								{
									name = strtok(NULL, " =\r\n");
									if (name != NULL)
										SetExtraIcons(name, &icon[0]);
								}
								if (!strcmp(name, "SubFolderIcon"))
								{
									name = strtok(NULL, " =\r\n");
									if (name != NULL)
										SetExtraIcons(name, &icon[1]);
								}
							}
							break;
						}
					}
				}
				fclose(fp);

				strcpy(buf, files.name);
				ext = strrchr(buf, '.');

				if (ext && *(ext + 1) && (core_stricmp(ext + 1, "ini")==0))
				{
					ExtraFolderData[count] =(EXFOLDERDATA*) malloc(sizeof(EXFOLDERDATA));
					if (ExtraFolderData[count])
					{
						*ext = '\0';

						memset(ExtraFolderData[count], 0, sizeof(EXFOLDERDATA));

						strncpy(ExtraFolderData[count]->m_szTitle, buf, 63);
						ExtraFolderData[count]->m_nFolderId   = next_folder_id++;
						ExtraFolderData[count]->m_nParent     = -1;
						ExtraFolderData[count]->m_dwFlags     = F_CUSTOM;
						ExtraFolderData[count]->m_nIconId     = icon[0] ? -icon[0] : IDI_FOLDER;
						ExtraFolderData[count]->m_nSubIconId  = icon[1] ? -icon[1] : IDI_FOLDER;
						//printf("extra folder with icon %i, subicon %i\n",
						//ExtraFolderData[count]->m_nIconId,
						//ExtraFolderData[count]->m_nSubIconId);
						count++;
					}
				}
			}
		}
	}

	chdir(curdir);
	return count;
}

void FreeExtraFolders(void)
{
	int i;

	for (i = 0; i < numExtraFolders; i++)
	{
		if (ExtraFolderData[i])
		{
			free(ExtraFolderData[i]);
			ExtraFolderData[i] = NULL;
		}
	}

	for (i = 0; i < numExtraIcons; i++)
		free(ExtraFolderIcons[i]);

	numExtraIcons = 0;

}


static void SetExtraIcons(char *name, int *id)
{
	char *p = strchr(name, '.');

	if (p != NULL)
		*p = '\0';

	ExtraFolderIcons[numExtraIcons] = (char*)malloc(strlen(name) + 1);
	if (ExtraFolderIcons[numExtraIcons])
	{
		*id = ICON_MAX + numExtraIcons;
		strcpy(ExtraFolderIcons[numExtraIcons], name);
		numExtraIcons++;
	}
}


// Called to add child folders of the top level extra folders already created
BOOL TryAddExtraFolderAndChildren(int parent_index)
{
	FILE* fp = NULL;
	char fname[MAX_PATH];
	char readbuf[256];
	char *p;
	char *name;
	int i;
	LPTREEFOLDER lpTemp = NULL;
	LPTREEFOLDER lpFolder = treeFolders[parent_index];

	int current_id = lpFolder->m_nFolderId;

	int id = lpFolder->m_nFolderId - MAX_FOLDERS;

	/* "folder\title.ini" */

	const std::string t = dir_get_value(24);
	sprintf( fname, "%s\\%s.ini", t.c_str(), ExtraFolderData[id]->m_szTitle);

	fp = fopen(fname, "r");
	if (fp == NULL)
		return false;

	while ( fgets(readbuf, 256, fp) )
	{
		/* do we have [...] ? */

		if (readbuf[0] == '[')
		{
			p = strchr(readbuf, ']');
			if (p == NULL)
			{
				continue;
			}

			*p = '\0';
			name = &readbuf[1];

			/* is it [FOLDER_SETTINGS]? */

			if (strcmp(name, "FOLDER_SETTINGS") == 0)
			{
				current_id = -1;
				continue;
			}
			else
			{
				/* it it [ROOT_FOLDER]? */

				if (!strcmp(name, "ROOT_FOLDER"))
				{
					current_id = lpFolder->m_nFolderId;
					lpTemp = lpFolder;

				}
				else
				{
					/* must be [folder name] */

					current_id = next_folder_id++;
					/* create a new folder with this name,
					   and the flags for this folder as read from the registry */
					lpTemp = NewFolder(name,current_id,parent_index,
						ExtraFolderData[id]->m_nSubIconId,
						GetFolderFlags(numFolders) | F_CUSTOM);
					ExtraFolderData[current_id] = (EXFOLDERDATA*)malloc(sizeof(EXFOLDERDATA));
					memset(ExtraFolderData[current_id], 0, sizeof(EXFOLDERDATA));

					ExtraFolderData[current_id]->m_nFolderId = current_id - MAX_EXTRA_FOLDERS;
					ExtraFolderData[current_id]->m_nIconId = ExtraFolderData[id]->m_nSubIconId;
					ExtraFolderData[current_id]->m_nParent = ExtraFolderData[id]->m_nFolderId;
					ExtraFolderData[current_id]->m_nSubIconId = -1;
					strcpy( ExtraFolderData[current_id]->m_szTitle, name );
					ExtraFolderData[current_id]->m_dwFlags = ExtraFolderData[id]->m_dwFlags;
					AddFolder(lpTemp);
				}
			}
		}
		else if (current_id != -1)
		{
			/* string on a line by itself -- game name */

			name = strtok(readbuf, " \t\r\n");
			if (name == NULL)
			{
				current_id = -1;
				continue;
			}

			/* IMPORTANT: This assumes that all driver names are lowercase! */
			for (i = 0; name[i]; i++)
				name[i] = tolower(name[i]);

			if (lpTemp == NULL)
			{
				ErrorMsg("Error parsing %s: missing [folder name] or [ROOT_FOLDER]", fname);
				current_id = lpFolder->m_nFolderId;
				lpTemp = lpFolder;
			}
			AddGame(lpTemp,GetGameNameIndex(name));
		}
	}

	if ( fp )
		fclose( fp );

	return true;
}


void GetFolders(TREEFOLDER ***folders,int *num_folders)
{
	*folders = treeFolders;
	*num_folders = numFolders;
}

static BOOL TryRenameCustomFolderIni(LPTREEFOLDER lpFolder, const char *old_name, const char *new_name)
{
	char filename[MAX_PATH];
	char new_filename[MAX_PATH];
	LPTREEFOLDER lpParent = NULL;
	string ini_dir = GetIniDir();
	const char* inidir = ini_dir.c_str();
	if (lpFolder->m_nParent >= 0)
	{
		//it is a custom SubFolder
		lpParent = GetFolder( lpFolder->m_nParent );
		if( lpParent )
		{
			snprintf(filename,ARRAY_LENGTH(filename),"%s\\%s\\%s.ini",inidir,lpParent->m_lpTitle, old_name );
			snprintf(new_filename,ARRAY_LENGTH(new_filename),"%s\\%s\\%s.ini",inidir,lpParent->m_lpTitle, new_name );
			win_move_file_utf8(filename,new_filename);
		}
	}
	else
	{
		//Rename the File, if it exists
		snprintf(filename,ARRAY_LENGTH(filename),"%s\\%s.ini",inidir,old_name );
		snprintf(new_filename,ARRAY_LENGTH(new_filename),"%s\\%s.ini",inidir, new_name );
		win_move_file_utf8(filename,new_filename);
		//Rename the Directory, if it exists
		snprintf(filename,ARRAY_LENGTH(filename),"%s\\%s",inidir,old_name );
		snprintf(new_filename,ARRAY_LENGTH(new_filename),"%s\\%s",inidir, new_name );
		win_move_file_utf8(filename,new_filename);
	}
	return true;
}

BOOL TryRenameCustomFolder(LPTREEFOLDER lpFolder, const char *new_name)
{
	char filename[MAX_PATH];
	char new_filename[MAX_PATH];

	if (lpFolder->m_nParent >= 0)
	{
		// a child extra folder was renamed, so do the rename and save the parent

		// save old title
		char *old_title = lpFolder->m_lpTitle;

		// set new title
		lpFolder->m_lpTitle = (char *)malloc(strlen(new_name) + 1);
		strcpy(lpFolder->m_lpTitle,new_name);

		// try to save
		if (TrySaveExtraFolder(lpFolder) == false)
		{
			// failed, so free newly allocated title and restore old
			free(lpFolder->m_lpTitle);
			lpFolder->m_lpTitle = old_title;
			return false;
		}
		TryRenameCustomFolderIni(lpFolder, old_title, new_name);
		// successful, so free old title
		free(old_title);
		return true;
	}

	// a parent extra folder was renamed, so rename the file

	const std::string t = dir_get_value(24);
	snprintf(new_filename,ARRAY_LENGTH(new_filename),"%s\\%s.ini", t.c_str(), new_name);
	snprintf(filename,ARRAY_LENGTH(filename),"%s\\%s.ini", t.c_str(), lpFolder->m_lpTitle);

	BOOL retval = win_move_file_utf8(filename,new_filename);

	if (retval)
	{
		TryRenameCustomFolderIni(lpFolder, lpFolder->m_lpTitle, new_name);
		free(lpFolder->m_lpTitle);
		lpFolder->m_lpTitle = (char *)malloc(strlen(new_name) + 1);
		strcpy(lpFolder->m_lpTitle,new_name);
	}
	else
	{
		char buf[2048];
		snprintf(buf,ARRAY_LENGTH(buf),"Error while renaming custom file %s to %s", filename,new_filename);
		win_message_box_utf8(GetMainWindow(), buf, MAMEUINAME, MB_OK | MB_ICONERROR);
	}
	return retval;
}

void AddToCustomFolder(LPTREEFOLDER lpFolder,int driver_index)
{
	if ((lpFolder->m_dwFlags & F_CUSTOM) == 0)
	{
		win_message_box_utf8(GetMainWindow(),"Unable to add game to non-custom folder", MAMEUINAME,MB_OK | MB_ICONERROR);
		return;
	}

	if (TestBit(lpFolder->m_lpGameBits,driver_index) == 0)
	{
		AddGame(lpFolder,driver_index);
		if (TrySaveExtraFolder(lpFolder) == false)
			RemoveGame(lpFolder,driver_index); // undo on error
	}
}

void RemoveFromCustomFolder(LPTREEFOLDER lpFolder,int driver_index)
{
	if ((lpFolder->m_dwFlags & F_CUSTOM) == 0)
	{
		win_message_box_utf8(GetMainWindow(),"Unable to remove game from non-custom folder", MAMEUINAME,MB_OK | MB_ICONERROR);
		return;
	}

	if (TestBit(lpFolder->m_lpGameBits,driver_index) != 0)
	{
		RemoveGame(lpFolder,driver_index);
		if (TrySaveExtraFolder(lpFolder) == false)
			AddGame(lpFolder,driver_index); // undo on error
	}
}

BOOL TrySaveExtraFolder(LPTREEFOLDER lpFolder)
{
	char fname[MAX_PATH];
	FILE *fp;
	BOOL error = false;
	int i,j;

	LPTREEFOLDER root_folder = NULL;
	LPEXFOLDERDATA extra_folder = NULL;

	for (i=0; i<numExtraFolders; i++)
	{
		if (ExtraFolderData[i]->m_nFolderId == lpFolder->m_nFolderId)
		{
			root_folder = lpFolder;
			extra_folder = ExtraFolderData[i];
			break;
		}

		if (lpFolder->m_nParent >= 0 &&
			ExtraFolderData[i]->m_nFolderId == treeFolders[lpFolder->m_nParent]->m_nFolderId)
		{
			root_folder = treeFolders[lpFolder->m_nParent];
			extra_folder = ExtraFolderData[i];
			break;
		}
	}

	if (extra_folder == NULL || root_folder == NULL)
	{
		MessageBox(GetMainWindow(), TEXT("Error finding custom file name to save"), TEXT(MAMEUINAME), MB_OK | MB_ICONERROR);
		return false;
	}
	/* "folder\title.ini" */

	const std::string t = dir_get_value(24);
	snprintf( fname, sizeof(fname), "%s\\%s.ini", t.c_str(), extra_folder->m_szTitle);

	fp = fopen(fname, "wt");
	if (fp == NULL)
		error = true;
	else
	{
		TREEFOLDER *folder_data;

		fprintf(fp,"[FOLDER_SETTINGS]\n");
		// negative values for icons means it's custom, so save 'em
		if (extra_folder->m_nIconId < 0)
		{
			fprintf(fp, "RootFolderIcon %s\n", ExtraFolderIcons[(-extra_folder->m_nIconId) - ICON_MAX]);
		}
		if (extra_folder->m_nSubIconId < 0)
		{
			fprintf(fp,"SubFolderIcon %s\n", ExtraFolderIcons[(-extra_folder->m_nSubIconId) - ICON_MAX]);
		}

		/* need to loop over all our TREEFOLDERs--first the root one, then each child. Start with the root */

		folder_data = root_folder;

		fprintf(fp,"\n[ROOT_FOLDER]\n");

		for (i=0;i<driver_list::total();i++)
		{
			if (TestBit(folder_data->m_lpGameBits, i))
			{
				fprintf(fp,"%s\n",driver_list::driver(i).name);
			}
		}

		/* look through the custom folders for ones with our root as parent */
		for (j=0;j<numFolders;j++)
		{
			folder_data = treeFolders[j];

			if (folder_data->m_nParent >= 0 &&
				treeFolders[folder_data->m_nParent] == root_folder)
			{
				fprintf(fp,"\n[%s]\n",folder_data->m_lpTitle);

				for (i=0;i<driver_list::total();i++)
				{
					if (TestBit(folder_data->m_lpGameBits, i))
					{
						fprintf(fp,"%s\n",driver_list::driver(i).name);
					}
				}
			}
		}
		if (fclose(fp) != 0)
			error = true;
	}

	if (error)
	{
		char buf[500];
		snprintf(buf,ARRAY_LENGTH(buf),"Error while saving custom file %s",fname);
		win_message_box_utf8(GetMainWindow(), buf, MAMEUINAME, MB_OK | MB_ICONERROR);
	}
	return !error;
}

HIMAGELIST GetTreeViewIconList(void)
{
	return hTreeSmall;
}

int GetTreeViewIconIndex(int icon_id)
{
	if (icon_id < 0)
		return -icon_id;

	for (int i = 0; i < sizeof(treeIconNames) / sizeof(treeIconNames[0]); i++)
		if (icon_id == treeIconNames[i].nResourceID)
			return i;

	return -1;
}

static void SaveExternalFolders(int parent_index, const char *fname)
{
	string val = dir_get_value(24);
	char s[val.size()+1];
	strcpy(s, val.c_str());
	char *fdir = strtok(s, ";"); // get first dir

	// create directory if needed
	wchar_t *temp = ui_wstring_from_utf8(fdir);
	BOOL res = CreateDirectory(temp, NULL);
	free(temp);
	if (!res)
	{
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			printf("SaveExternalFolders: Unable to create the directory \"%s\".\n",fdir);
			return;
		}
	}

	// create/truncate file
	string filename = fdir + string("\\") + fname + string(".ini");
	FILE *f = fopen(filename.c_str(), "w");
	if (f == NULL)
	{
		printf("SaveExternalFolders: Unable to open file %s for writing.\n",filename.c_str());
		return;
	}

	// Populate the file
	fprintf(f, "[FOLDER_SETTINGS]\n");
	fprintf(f, "RootFolderIcon custom\n");
	fprintf(f, "SubFolderIcon custom\n");

	/* need to loop over all our TREEFOLDERs--first the root one, then each child.
	start with the root */
	LPTREEFOLDER lpFolder = treeFolders[parent_index];
	TREEFOLDER *folder_data = lpFolder;
	fprintf(f, "\n[ROOT_FOLDER]\n");

	int i;
	for (i = 0; i < driver_list::total(); i++)
	{
		if (TestBit(folder_data->m_lpGameBits, i))
			fprintf(f, "%s\n", GetGameName(i).c_str());
	}

	/* look through the custom folders for ones with our root as parent */
	for (int jj = 0; jj < numFolders; jj++)
	{
		folder_data = treeFolders[jj];

		if (folder_data->m_nParent >= 0 && treeFolders[folder_data->m_nParent] == lpFolder)
		{
			fprintf(f, "\n[%s]\n", folder_data->m_lpTitle);

			for (i = 0; i < driver_list::total(); i++)
			{
				if (TestBit(folder_data->m_lpGameBits, i))
					fprintf(f, "%s\n", GetGameName(i).c_str());
			}
		}
	}

	fclose(f);
	printf("SaveExternalFolders: Saved file %s.\n",filename.c_str());
}

/* End of source file */
