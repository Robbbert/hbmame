// For licensing and usage information, read docs/winui_license.txt
// MASTER
//****************************************************************************

/* Written by Mike Haaland <mhaaland@hypertech.com> */

#ifndef SPLITTER_H
#define SPLITTER_H

#if !defined(MAX)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#include "resource.h"
#include "winui.h"
#define SPLITTER_WIDTH 4
#define MIN_VIEW_WIDTH 10

enum eSplitterHits
{
	SPLITTER_HITNOTHING = 0,
	SPLITTER_HITITEM
};

typedef struct horzSplitter
{
	HWND m_hWnd;
	HWND m_hWndLeft;
	HWND m_hWndRight;
	RECT m_limitRect;
	RECT m_dragRect;
	void (*m_func)(HWND hWnd, LPRECT lpRect);
} HZSPLITTER, *LPHZSPLITTER;

/* Splitter routines */
void    OnMouseMove(HWND hWnd, UINT nFlags, POINTS p);
void    OnLButtonDown(HWND hWnd, UINT nFlags, POINTS p);
void    OnLButtonUp(HWND hWnd, UINT nFlags, POINTS p);
void    OnSizeSplitter(HWND hWnd);
void    AddSplitter(HWND hWnd, HWND hWndLeft, HWND hWndRight, void (*func)(HWND hWnd,LPRECT lpRect));
void    RecalcSplitters(void);
void    AdjustSplitter2Rect(HWND hWnd, LPRECT lpRect);
void    AdjustSplitter1Rect(HWND hWnd, LPRECT lpRect);
BOOL    InitSplitters(void);
void    SplittersExit(void);
int     GetSplitterCount(void);

extern int *nSplitterOffset;

typedef struct
{
	double dPosition;
	int nSplitterWindow;
	int nLeftWindow;
	int nRightWindow;
	void (*pfnAdjust)(HWND hWnd,LPRECT lpRect);
} SPLITTERINFO;

#ifdef MESS
const SPLITTERINFO g_splitterInfo[] =
{
	{ 0.15,  IDC_SPLITTER,  IDC_TREE,  IDC_LIST,    AdjustSplitter1Rect },
	{ 0.8,  IDC_SPLITTER2, IDC_LIST,  IDC_SWTAB,   AdjustSplitter1Rect },
	{ 0.9, IDC_SPLITTER3, IDC_SWTAB, IDC_SSFRAME, AdjustSplitter2Rect },
	{ -1 }
};
#else
const SPLITTERINFO g_splitterInfo[] =
{
	{ 0.25, IDC_SPLITTER,  IDC_TREE,  IDC_LIST,    AdjustSplitter1Rect },
	{ 0.5,  IDC_SPLITTER2, IDC_LIST,  IDC_SSFRAME, AdjustSplitter2Rect },
	{ -1 }
};
#endif

//extern const SPLITTERINFO g_splitterInfo[];


#endif /* SPLITTER_H */
