
#ifndef __CTRL_H__
#define __CTRL_H__

BOOL CALLBACK CtlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef int (*PFUN)();

int ModCaption();
int MovWindow();

int MaxWindow();
int NorWindow();
int MinWindow();
int CloseWin();

int HidWindow();
int RstWindow();
int AbleWindow();
int DisableWindow();

int ProtectWin();
int CloseThread();

int HardEnd();
int OpenPath();
int Restart();



typedef struct tagCMD
{
	int id;
	PFUN pFun;
}CMD;



#endif