// LinexSpy.cpp : Defines the entry point for the application.
//

#include "stdafx.h"



#include "PEView.h"

#define _WINVER > 0x5000




BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI OpenCtrlWnd(LPVOID argu);


VOID CALLBACK SpyProc(HWND hwnd, UINT uMsg, UINT idEvent,  DWORD dwTime );

VOID GetInfoWindow(VOID);
TCHAR* GetFileNameFromPID(UINT PID, TCHAR *lpName);


BOOL IsOpened = FALSE;
HWND g_hWnd =  NULL;
HWND g_hCtrlWnd =  NULL;
HWND g_hTarget = NULL;
HINSTANCE hIns;
//rs ctrl
HWND hTitle ,hHandle , hParent, hCls ,hPos ,hProcess ,hrcThread ;
HWND hBtn, hCtl, hCur, hPath, hViewPE;

//ctrl 


HCURSOR hCursor;

TCHAR g_szCaption[256];
RECT  g_rt;
TCHAR g_szFilePath[256];
int   g_PID;

HWND	hCaption ,hxPos, hyPos, hrPos, hbPos;
TCHAR	szCaption[256],szxPos[12], szyPos[12], szrPos[12], szbPos[12];


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    HWND hDlg = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_FRAME), NULL, DlgProc);
	g_hWnd = hDlg;
	hIns = hInstance;
	ShowWindow(hDlg,nCmdShow);

	hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));

	RegisterHotKey(hDlg ,101, 0, VK_F7);
	RegisterHotKey(hDlg ,102, 0, VK_F8);

 	// TODO: Place code here.

	hTitle = GetDlgItem(g_hWnd,IDC_EDIT1);
	hHandle = GetDlgItem(g_hWnd,IDC_EDIT2);
	hParent = GetDlgItem(g_hWnd,IDC_EDIT3);
	hCls = GetDlgItem(g_hWnd,IDC_EDIT4);
	hPos = GetDlgItem(g_hWnd,IDC_EDIT5);
	hProcess = GetDlgItem(g_hWnd,IDC_EDIT6);
	hrcThread = GetDlgItem(g_hWnd,IDC_EDIT7);
	hBtn = GetDlgItem(g_hWnd, IDC_GETWIN);
	hCtl = GetDlgItem(g_hWnd, IDC_CTLWIN);
	hCur = GetDlgItem(g_hWnd, IDC_MOUSE);
	hPath = GetDlgItem(g_hWnd, IDC_EDIT8);
	hViewPE = GetDlgItem(g_hWnd, IDB_ANALY);






	MSG msg;
	while(GetMessage(&msg,NULL,0,0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LPDWORD dwTID = 0;
HANDLE hThread ;

BOOL bSpy = FALSE;

BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	UINT vk ;
	
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			RECT rt;
			POINT pt;
			GetWindowRect(hCur, &rt);
			GetCursorPos(&pt);
			if(PtInRect(&rt, pt))
			{
				SetCapture(hWnd);
				SetCursor(hCursor);
				bSpy = TRUE;
				SetWindowText(hBtn,TEXT("停止(F8)"));
				EnableWindow(hCtl, FALSE);
				EnableWindow(hViewPE, FALSE);
				SetTimer(hWnd, 1001, 100, SpyProc);
				
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			if(TRUE == bSpy)
			{
				bSpy = FALSE;
				SetWindowText(hBtn,TEXT("开始(F7)"));
				EnableWindow(hCtl, TRUE);
				EnableWindow(hViewPE, TRUE);
				KillTimer(hWnd,1001);
			}
		}
		break;
	case WM_HOTKEY:
		{
			vk = (UINT) HIWORD(lParam);
			switch(vk)
			{
				case VK_F7:
				{
					if(FALSE == bSpy)
					{
						bSpy = TRUE;
						SetWindowText(hBtn,TEXT("停止(F8)"));
						EnableWindow(hCtl, FALSE);
						SetTimer(hWnd, 1001, 100, SpyProc);
					}
					break;
				}
				case VK_F8:
				{
					if(TRUE == bSpy)
					{
						bSpy = FALSE;
						SetWindowText(hBtn,TEXT("开始(F7)"));
						EnableWindow(hCtl, TRUE);
						KillTimer(hWnd,1001);
					}
					break;
				}
			}

			break;
		}
		//system quit
	case WM_COMMAND:
		{
			int wmid = LOWORD(wParam);
			switch(wmid)
			{
				case IDC_GETWIN:
					{
						if(FALSE == bSpy)
						{
							bSpy = TRUE;
							SetWindowText(hBtn,TEXT("停止(F8)"));
							EnableWindow(hCtl, FALSE);
							SetTimer(hWnd, 1001, 100, SpyProc);
						}
						else
						{
							bSpy = FALSE;
							SetWindowText(hBtn,TEXT("开始(F7)"));
							EnableWindow(hCtl, TRUE);
							KillTimer(hWnd,1001);
						}
					}
					break;
				case IDC_CTLWIN:
					CloseHandle(CreateThread(0,0,OpenCtrlWnd, 0,0,0));
					EnableWindow(hWnd, FALSE);
					
					break;

			

			}
		}
		break;
	case WM_CLOSE:
		UnregisterHotKey(hWnd, 101);
		UnregisterHotKey(hWnd, 102);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		EndDialog(hWnd, WM_CLOSE);
		PostQuitMessage(0);
		break;

	default:
		return FALSE;
	}
	
	return TRUE ;
}

HWND hTarget = NULL;

VOID CALLBACK SpyProc(HWND hwnd, UINT uMsg, UINT idEvent,  DWORD dwTime )
{

	POINT pnt;
	RECT rc;

	HWND DeskHwnd = GetDesktopWindow(); //取得桌面句柄
	HDC DeskDC = GetWindowDC(DeskHwnd); //取得桌面设备场景
	int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);
	GetCursorPos(&pnt);
	HWND UnHwnd = WindowFromPoint(pnt) ; //取得鼠标指针处窗口句柄
	HWND hPar = GetParent(hTarget);

	
	if(hTarget != UnHwnd)
	{
		ValidateRect(hTarget, &rc);
	}
/*
	if(UnHwnd == g_hWnd || hPar == g_hWnd)
	{
		hTarget = UnHwnd;
	}
	else
	{*/
		hTarget = UnHwnd;
//	}
	
	

	GetWindowRect(hTarget, &rc); //获得窗口矩形

	if( rc.left < 0 )
	{
		rc.left = 0;
	}
	if (rc.top < 0 )
	{
		rc.top = 0;
	}

	HPEN newPen = CreatePen(PS_SOLID, 3, 0x00ff00); //建立新画笔,载入DeskDC
	HGDIOBJ oldPen = SelectObject(DeskDC, newPen);

	Rectangle(DeskDC, rc.left, rc.top, rc.right, rc.bottom); //在窗口周围显示闪烁矩形

	Sleep(400); //设置闪烁时间间隔
	Rectangle( DeskDC, rc.left, rc.top, rc.right, rc.bottom);
	SetROP2(DeskDC, oldRop2);
	SelectObject( DeskDC, oldPen);
	DeleteObject(newPen);

	ReleaseDC( DeskHwnd, DeskDC);
	DeskDC = NULL;
	ValidateRect(hTarget, &rc);

	GetInfoWindow();

}



VOID GetInfoWindow(VOID)
{
	if(NULL != hTarget)
	{
		TCHAR szBuf[256] = TEXT("");

		//title
		GetWindowText(hTarget, szBuf , 256);
		SendMessage(hTarget, WM_GETTEXT,0,(LPARAM)szBuf);
	
		
		g_hTarget = hTarget;
		strcpy(g_szCaption, szBuf);
		SetWindowText(hTitle,szBuf);

		//handle
		sprintf(szBuf, "%x", hTarget);
		SetWindowText(hHandle,szBuf);

		//parent handle
		HWND hPar = GetParent(hTarget);
		sprintf(szBuf, "%x", hPar );
		SetWindowText(hParent,szBuf);

		//class
		/*
		DWORD dwStyle = (DWORD)GetWindowLong(hTarget, GWL_STYLE);
		sprintf(szBuf, "%x", dwStyle);
		SetWindowText(hCls,szBuf);
*/
	    //class
	
		TCHAR szClsNameBuf[32] ;
		GetClassName(hTarget,szClsNameBuf,32 );
		sprintf(szBuf, "%s", szClsNameBuf);
		SetWindowText(hCls,szBuf);


		//pos
		RECT rt ;
		GetWindowRect(hTarget, &rt);
		g_rt = rt;
		sprintf(szBuf, "x1:%d,y1:%d,x2:%d,y=%d", rt.left, rt.top, rt.right, rt.bottom);
		SetWindowText(hPos,szBuf);

		
		DWORD pid,tid;
		tid = GetWindowThreadProcessId(hTarget, &pid);
		g_PID = tid;
		//hProcess
		sprintf(szBuf, "%d", pid);
		SetWindowText(hProcess,szBuf);

//		GetFileNameFromPID(pid, szBuf);
	
		
		HINSTANCE hModule =  (HINSTANCE)GetWindowLong(g_hTarget, GWL_HINSTANCE);
//	    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		GetModuleFileName(hModule, szBuf, 256 );
		SetWindowText(hPath,szBuf);
		strcpy(g_szFilePath,szBuf);
		//hThread
		sprintf(szBuf, "%d", tid);
		SetWindowText(hrcThread,szBuf);

		//path
		

	}
}



//////////////////////////////////////////////////////////////////////////

TCHAR* GetFileNameFromPID(UINT PID, TCHAR *lpName)
{
	if(NULL == lpName)
	{
		return FALSE;
	}
	HANDLE handle32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == handle32Snapshot)
	{
		return NULL;
	}
	
	PROCESSENTRY32 pEntry;       
	pEntry.dwSize = sizeof( PROCESSENTRY32 );
	
	//Search for all the process and terminate it
	if(Process32First(handle32Snapshot, &pEntry))
	{
		BOOL bFound = FALSE;
		if (pEntry.th32ProcessID == PID)
		{
			bFound = TRUE;
		}
		while((!bFound)&&Process32Next(handle32Snapshot, &pEntry))
		{
			if (pEntry.th32ProcessID == PID)
			{
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			strcpy(lpName, pEntry.szExeFile);
			CloseHandle(handle32Snapshot);
			//		HANDLE handLe =  OpenProcess(PROCESS_TERMINATE , FALSE, pEntry.th32ProcessID);
			//		BOOL bResult = TerminateProcess(handLe,0);
			return lpName;
		}
	}
	
	CloseHandle(handle32Snapshot);
    return NULL;


}
