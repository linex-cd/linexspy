
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////
//this file is built for module file portable execution information

//////////////////////////////////////////////////////////////////////////


HWND g_hPeViewWnd;
HWND g_hList;
LVITEM lvItem;
LVCOLUMN lvCol;
extern HINSTANCE hIns;
extern HWND g_hWnd;

TCHAR szModulePath[256];


//peinfo
typedef unsigned long ulong;

ulong ulDosStub;
ulong ulPeTable;
ulong SectionAddr;

typedef struct __SECTION
{
	char	szSetionName[16];
	ulong	ulSectionDespAddr;
//	__int64 
}SECTION;


IMAGE_NT_HEADERS g_img_head;



BOOL CALLBACK PeViewDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool LoadGui(TCHAR* szModuleFileName)
{
	g_hPeViewWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_PEVIEW), g_hWnd, PeViewDlgProc);

	if (g_hPeViewWnd == NULL)
	{
		MessageBox(0,TEXT("Init dialog error!"),0,0);
		return false;
	}

	strcpy(szModulePath, szModuleFileName);

	ShowWindow(g_hPeViewWnd, 1);
	//init dlg item handles


	g_hList    = GetDlgItem(g_hPeViewWnd, IDC_LIST1);
	

	//message loop here
	MSG msg;

	while(GetMessage(&msg, g_hPeViewWnd, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	EnableWindow(g_hCtrlWnd, TRUE);
	return true;
	

}

BOOL CALLBACK PeViewDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int wnId = (int)wParam;
	switch(uMsg)
	{
		case WM_CLOSE:
				DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			EndDialog(hWnd, WM_CLOSE);
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
			switch(wnId)
			{
				case IDB_WRITEBACK:
					//write pe info back to pe file
				break;

				case IDB_REFLESH:
					//reflesh pe information to list
				break;
			}
			break;

		case WM_DROPFILES:
			//get drag file info
			break;
		case WM_INITDIALOG:
			{	//init list view
				SetWindowText(g_hPeViewWnd, szModulePath);

			}
			break;
		default:

			return FALSE;

	}

	return TRUE;
}


bool LoadPEFile(TCHAR* szModuleFileName)
{
	
	return true;
}

bool AddItemInfo(UINT Addr, TCHAR* szName, UINT Value, TCHAR* ValueDesp)
{
	return true;
}




