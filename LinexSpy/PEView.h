
#define _WIN32_WINT 0x5000

#pragma once


typedef struct _LVITEM { 
    UINT   mask; 
    int    iItem; 
    int    iSubItem; 
    UINT   state; 
    UINT   stateMask; 
    LPTSTR  pszText; 
    int    cchTextMax; 
    int    iImage; 
    LPARAM lParam;
#if (_WIN32_IE >= 0x0300)
    int iIndent;
#endif
} LVITEM, FAR *LPLVITEM; 



typedef struct _LVCOLUMN { 
    UINT mask; 
    int fmt; 
    int cx; 
    LPTSTR pszText; 
    int cchTextMax; 
    int iSubItem; 
#if (_WIN32_IE >= 0x0300)
    int iImage;
    int iOrder;
#endif
} LVCOLUMN, FAR *LPLVCOLUMN; 




bool LoadGui(TCHAR* szModuleFileName);

bool LoadPEFile(TCHAR* szModuleFileName);

bool AddItemInfo(UINT Addr, TCHAR* szName, UINT Value, TCHAR* ValueDesp);

