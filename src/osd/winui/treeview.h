// license:BSD-3-Clause
// For licensing and usage information, read docs/release/winui_license.txt

#ifndef WINUI_TREEVIEW_H
#define WINUI_TREEVIEW_H

#define MAX_EXTRA_FOLDERS 			256
#define MAX_EXTRA_SUBFOLDERS 		256
#define ICON_MAX		std::size(treeIconNames)
#include "bitmask.h"
#include <stdint.h>
#include "emu_opts.h"

/***************************************************************************
    Folder And Filter Definitions
 ***************************************************************************/

typedef struct
{
	const char *m_lpTitle; 									// Folder Title
	const char *short_name;  								// for saving in the .ini
	UINT        m_nFolderId; 								// ID
	UINT        m_nIconId; 									// if >= 0, resource id of icon (IDI_xxx), otherwise index in image list
	DWORD       m_dwUnset; 									// Excluded filters
	DWORD       m_dwSet;   									// Implied filters
	BOOL        m_process;      // (not used yet)
	void        (*m_pfnCreateFolders)(int parent_index); 	// Constructor for special folders
	BOOL        (*m_pfnQuery)(uint32_t nDriver);			  	// Query function
	BOOL        m_bExpectedResult;					   	// Expected query result
	OPTIONS_TYPE m_opttype = OPTIONS_MAX;                                // Has an ini file (vector.ini, etc)
} FOLDERDATA, *LPFOLDERDATA;

typedef const FOLDERDATA *LPCFOLDERDATA;

typedef struct
{
	DWORD m_dwFilterType;				/* Filter value */
	DWORD m_dwCtrlID;					/* Control ID that represents it */
	BOOL (*m_pfnQuery)(uint32_t nDriver);	/* Query function */
	BOOL m_bExpectedResult;				/* Expected query result */
} FILTER_ITEM, *LPFILTER_ITEM;

typedef const FILTER_ITEM *LPCFILTER_ITEM;

/* TreeView structures */
enum
{
	FOLDER_NONE = 0,
	FOLDER_ALLGAMES,
	FOLDER_AVAILABLE,
	FOLDER_ARCADE,
	FOLDER_BIOS,
	FOLDER_CLONES,
	FOLDER_CPU,
	FOLDER_DEFICIENCY,
	FOLDER_DUMPING,
	FOLDER_FPS,
	FOLDER_HARDDISK,
	FOLDER_HORIZONTAL,
	FOLDER_LIGHTGUN,
	FOLDER_MANUFACTURER,
	FOLDER_MECHANICAL,
	FOLDER_MODIFIED,
	FOLDER_MOUSE,
	FOLDER_NONMECHANICAL,
	FOLDER_NONWORKING,
	FOLDER_ORIGINAL,
	FOLDER_RASTER,
	FOLDER_RESOLUTION,
	FOLDER_SAMPLES,
	FOLDER_SAVESTATE,
	FOLDER_SCREENS,
	FOLDER_SND,
	FOLDER_SOURCE,
	FOLDER_STEREO,
	FOLDER_TRACKBALL,
	FOLDER_UNAVAILABLE,
	FOLDER_VECTOR,
	FOLDER_VERTICAL,
	FOLDER_WORKING,
	FOLDER_YEAR,
	MAX_FOLDERS,
};

typedef enum
{
	F_CLONES        = 0x00000001,
	F_NONWORKING    = 0x00000002,
	F_UNAVAILABLE   = 0x00000004,
	F_VECTOR        = 0x00000008,
	F_RASTER        = 0x00000010,
	F_ORIGINALS     = 0x00000020,
	F_WORKING       = 0x00000040,
	F_AVAILABLE     = 0x00000080,
	F_HORIZONTAL    = 0x00000100,
	F_VERTICAL      = 0x00000200,
	F_MECHANICAL    = 0x00000400,
	F_ARCADE        = 0x00000800,
	F_MESS          = 0x00001000,
	F_COMPUTER      = 0x00002000,
	F_CONSOLE       = 0x00004000,
	F_MODIFIED      = 0x00008000,
	F_MASK          = 0x0000FFFF,
	F_INIEDIT       = 0x00010000, // There is an .ini that can be edited. MSH 20070811
	F_CUSTOM        = 0x01000000  // for current .ini custom folders
} FOLDERFLAG;

typedef struct
{
	char        *m_lpTitle;       // String contains the folder name
	wchar_t     *m_lptTitle;      // String contains the folder name as wchar_t*
	UINT        m_nFolderId;      // Index / Folder ID number
	int         m_nParent;        // Parent folder index in treeFolders[]
	int         m_nIconId;        // negative icon index into the ImageList, or IDI_xxx resource id
	DWORD       m_dwFlags;        // Misc flags
	LPBITS      m_lpGameBits;     // Game bits, represent game indices
} TREEFOLDER, *LPTREEFOLDER;

typedef struct
{
	char        m_szTitle[64];  	// Folder Title
	UINT        m_nFolderId;    	// ID
	int         m_nParent;      	// Parent Folder index in treeFolders[]
	DWORD       m_dwFlags;      	// Flags - Customizable and Filters
	int         m_nIconId;      	// negative icon index into the ImageList, or IDI_xxx resource id
	int         m_nSubIconId;   	// negative icon index into the ImageList, or IDI_xxx resource id
} EXFOLDERDATA, *LPEXFOLDERDATA;

typedef struct
{
	int nResourceID;
	const char* lpName;
} TREEICON;

void FreeFolders(void);
void ResetFilters(void);
void InitTree(LPCFOLDERDATA lpFolderData, LPCFILTER_ITEM lpFilterList);
void SetCurrentFolder(LPTREEFOLDER lpFolder);
UINT GetCurrentFolderID(void);
LPTREEFOLDER GetCurrentFolder(void);
int GetNumFolders(void);
LPTREEFOLDER GetFolder(UINT nFolder);
LPTREEFOLDER GetFolderByID(UINT nID);
LPTREEFOLDER GetFolderByName(int nParentId, const char *pszFolderName);
void AddGame(LPTREEFOLDER lpFolder, UINT nGame);
void RemoveGame(LPTREEFOLDER lpFolder, UINT nGame);
int  FindGame(LPTREEFOLDER lpFolder, int nGame);
void ResetWhichGamesInFolders(void);
LPCFOLDERDATA FindFilter(DWORD folderID);
bool GameFiltered(int nGame, DWORD dwFlags);
bool GetParentFound(int nGame);
LPCFILTER_ITEM GetFilterList(void);
void SetTreeIconSize(HWND hWnd, bool bLarge);
bool GetTreeIconSize(void);
void GetFolders(TREEFOLDER ***folders,int *num_folders);
bool TryRenameCustomFolder(LPTREEFOLDER lpFolder,const char *new_name);
void AddToCustomFolder(LPTREEFOLDER lpFolder,int driver_index);
void RemoveFromCustomFolder(LPTREEFOLDER lpFolder,int driver_index);
HIMAGELIST GetTreeViewIconList(void);
int GetTreeViewIconIndex(int icon_id);
void ResetTreeViewFolders(void);
void SelectTreeViewFolder(int folder_id);

void CreateManufacturerFolders(int parent_index);
void CreateYearFolders(int parent_index);
void CreateSourceFolders(int parent_index);
void CreateScreenFolders(int parent_index);
void CreateResolutionFolders(int parent_index);
void CreateFPSFolders(int parent_index);
void CreateBIOSFolders(int parent_index);
void CreateCPUFolders(int parent_index);
void CreateSoundFolders(int parent_index);
void CreateOrientationFolders(int parent_index);
void CreateDeficiencyFolders(int parent_index);
void CreateDumpingFolders(int parent_index);

#endif

