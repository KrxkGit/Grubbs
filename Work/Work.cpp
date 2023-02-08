// Work.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Work.h"


HHOOK hHook = NULL;
#define WINNAME "格拉布斯筛选器 & By LBX"
#define CALCOMMANDID 1000//计算命令ID

LRESULT CALLBACK Work(int code, WPARAM wParam, LPARAM lParam)
{
	PMSG pMsg = (PMSG)lParam;
	if (pMsg->message==WM_KEYUP) {
		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x8000;//检测是否按下
		if (bCtrl && pMsg->wParam=='C') {
			HWND hWnd = FindWindow(_T("#32770"), _T(WINNAME));
			SendMessage(hWnd, WM_COMMAND, CALCOMMANDID, NULL);
			return FALSE;
		}
	}
	return CallNextHookEx(hHook, code, wParam, lParam);
}


VOID SetHook(BOOL bSet)
{
	if (bSet) {
		if (hHook == NULL) {
			hHook = SetWindowsHookEx(WH_GETMESSAGE, Work, GetModuleHandle(_T("Work.dll")), 0);
		}
	}
	else {
		if (hHook != NULL) {
			UnhookWindowsHookEx(hHook);
			hHook = NULL;
		}
	}
}