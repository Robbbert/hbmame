// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIRWATCHER_H
#define DIRWATCHER_H

#include <string>
typedef struct DirWatcher *PDIRWATCHER;

PDIRWATCHER DirWatcher_Init(HWND hwndTarget, UINT nMessage);
BOOL DirWatcher_Watch(PDIRWATCHER pWatcher, WORD nIndex, const std::string t, BOOL bWatchSubtrees);
void DirWatcher_Free(PDIRWATCHER pWatcher);

#endif
