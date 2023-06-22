// For licensing and usage information, read docs/release/winui_license.txt
//****************************************************************************

#ifndef WINUI_DIRWATCHER_H
#define WINUI_DIRWATCHER_H

#include <string>
typedef struct DirWatcher *PDIRWATCHER;

PDIRWATCHER DirWatcher_Init(HWND hwndTarget, UINT nMessage);
BOOL DirWatcher_Watch(PDIRWATCHER pWatcher, WORD nIndex, const std::string t, BOOL bWatchSubtrees);
void DirWatcher_Free(PDIRWATCHER pWatcher);

#endif

