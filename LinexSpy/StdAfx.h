// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "resource.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <TLHELP32.H>
#include <shellapi.h>


extern BOOL IsOpened;
extern HWND g_hWnd, g_hCtrlWnd;
extern HWND g_hTarget;
extern HINSTANCE hIns;
//rs ctrl
extern HWND hTitle ,hHandle , hParent, hCls ,hPos ,hProcess ,hrcThread ;
extern HWND hBtn, hCtl, hCur, hPath, hViewPE;
extern HWND	hCaption ,hxPos, hyPos, hrPos, hbPos;
extern HCURSOR hCursor;

extern TCHAR g_szCaption[256];
extern RECT  g_rt;
extern TCHAR g_szFilePath[256];
extern int   g_PID;
extern TCHAR szCaption[256],szxPos[12], szyPos[12], szrPos[12], szbPos[12];

#include "Ctrl.h"
#include "PEView.h"


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
