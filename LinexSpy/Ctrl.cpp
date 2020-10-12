#include "StdAfx.h"

CMD g_Cmd[] = 
{
	{IDB_MOD, ModCaption},
	{IDB_MOVE,MovWindow	},
	
	{IDB_MAX, MaxWindow },
	{IDB_NOR, NorWindow },
	{IDB_MIN, MinWindow },
	{IDB_CLO, CloseWin  },
	
	{IDB_HIDE,HidWindow },
	{IDB_SHOW,RstWindow },
	{IDB_HIDE,AbleWindow },
	{IDB_SHOW,DisableWindow },

	{IDB_PRO, ProtectWin},
	{IDB_TERN,CloseThread},
	
	{IDB_END, HardEnd   },
	{IDB_OPEN,OpenPath  },
	{IDB_REB, Restart   }
};


DWORD WINAPI OpenPEWnd(LPVOID argu);
int ProcessCmd(int cmdId);



DWORD WINAPI OpenCtrlWnd(LPVOID argu)
{
	if (TRUE == IsOpened)
	{
		return -1;
	}

	g_hCtrlWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_CTL),g_hWnd, CtlProc);
	IsOpened = TRUE;

	ShowWindow(g_hCtrlWnd,1);
	hCaption = GetDlgItem(g_hCtrlWnd, IDC_CAP);
	hxPos = GetDlgItem(g_hCtrlWnd, IDC_X);
	hyPos = GetDlgItem(g_hCtrlWnd, IDC_Y);
	hrPos = GetDlgItem(g_hCtrlWnd, IDC_R);
	hbPos = GetDlgItem(g_hCtrlWnd, IDC_B);


	itoa(g_rt.left, szxPos,10);
	itoa(g_rt.top, szyPos,10);
	itoa(g_rt.right, szrPos,10);
	itoa(g_rt.bottom, szbPos,10);
	strcpy(szCaption, g_szCaption);
	
	SetWindowText(hCaption, szCaption);
	SetWindowText(hxPos, szxPos);
	SetWindowText(hyPos, szyPos);
	SetWindowText(hrPos, szrPos);
	SetWindowText(hbPos, szbPos);


	MSG msg;
	while(GetMessage(&msg, g_hCtrlWnd, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	IsOpened = FALSE;
	g_hCtrlWnd = 0;
	return 0;
}


BOOL CALLBACK CtlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int wmid = LOWORD(wParam);
	switch(uMsg)
	{
		case WM_CLOSE:
			IsOpened = FALSE;
			EnableWindow(hWnd, TRUE);
			EndDialog(hWnd,0);
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
				
			if (wmid == IDB_ANALY)
			{
				CloseHandle(CreateThread(0,0,OpenPEWnd, 0,0,0));
				EnableWindow(g_hCtrlWnd, FALSE);
			}
			else
			{
				ProcessCmd( LOWORD(wParam) );
			}
			
			break;
		default:
			return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////

DWORD WINAPI OpenPEWnd(LPVOID argu)
{
	
	
	if(strlen(g_szFilePath) > 5)
	{
		EnableWindow((HWND)argu, FALSE);
		LoadGui(g_szFilePath);
		EnableWindow((HWND)argu, TRUE);
	}
	
	
	
	return 0;
}



int ProcessCmd(int cmdId)
{
	for (int i = 0; i < 13; i++)
	{
		if (g_Cmd[i].id == cmdId)
		{
			g_Cmd[i].pFun();
		}
	}

	return 0;
}


int ModCaption()
{
	TCHAR szBuf[128];
	GetWindowText(hCaption, szBuf, 128);
//	SetWindowText(g_hTarget, szBuf);
	SendMessage(g_hTarget,EM_LIMITTEXT ,(WPARAM)100,0);
	SendMessage(g_hTarget,WM_SETTEXT,0,( LPARAM )szBuf);
	return 0;
}

int MovWindow()
{
	RECT rt;
	TCHAR px[8],py[8],pr[8],pb[8];
	GetWindowText(hxPos, px,8);
	GetWindowText(hyPos, py,8);
	GetWindowText(hrPos, pr,8);
	GetWindowText(hbPos, pb,8);

	rt.left = atoi(px);
	rt.top = atoi(py);
	rt.right = atoi(pr);
	rt.bottom = atoi(pb);

	MoveWindow(g_hTarget, rt.left, rt.top, rt.right - rt.left ,rt.bottom - rt.top,TRUE );

	return 0;
}


int MaxWindow()
{
	ShowWindow(g_hTarget, SW_MAXIMIZE);
	return 0;
}

int NorWindow()
{
	ShowWindow(g_hTarget, SW_NORMAL);
	return 0;
}

int MinWindow()
{
	ShowWindow(g_hTarget, SW_MINIMIZE);
	return 0;
}

int CloseWin()
{
	CloseWindow(g_hTarget);
	return 0;
}


int HidWindow()
{
	ShowWindow(g_hTarget, SW_HIDE);
	return 0;
}

int RstWindow()
{
	ShowWindow(g_hTarget, SW_SHOW);
	return 0;
}

int ProtectWin()
{
	
	return 0;
}

int CloseThread()
{
//	TerminateThread(GetWindowThreadProcessId(g_hTarget, NULL),0);
	return 0;
}



int HardEnd()
{
	
	return 0;
}

int OpenPath()
{

// 
// 	STARTUPINFO si;
// 	PROCESS_INFORMATION pi;
// 	memset((void*)&si,0,sizeof(STARTUPINFO));
// 	si.cb = sizeof(si);
// 	CreateProcess("C:\\windows\\system32\\explorer.exe ", NULL, NULL,NULL,/*FALSE*/ NULL,/*CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS*/ NULL,NULL,NULL,&si,&pi);
	TCHAR szBuf[256] = TEXT("");
	strcpy(szBuf, TEXT(" /select, "));
	strcat(szBuf, g_szFilePath);
	strcat(szBuf, "\\");
	ShellExecute(NULL,NULL,TEXT("explorer") ,szBuf, NULL, SW_NORMAL);
	
	return 0;
}

int Restart()
{
	
	return 0;
}

int AbleWindow()
{
	if (g_hTarget == g_hWnd || g_hCtrlWnd)
	{
		return -1;
	}

	EnableWindow(g_hTarget, TRUE);
	return 0;
}

int DisableWindow()
{
	if (g_hTarget == g_hWnd || g_hCtrlWnd)
	{
		return -1;
	}

	EnableWindow(g_hTarget, FALSE);
	return 0;
}