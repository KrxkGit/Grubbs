// Work.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Work.h"


HHOOK hHook = NULL;
#define WINNAME "������˹ɸѡ�� & By LBX"
#define CALCOMMANDID 1000//��������ID

LRESULT CALLBACK Work(int code, WPARAM wParam, LPARAM lParam)
{
	PMSG pMsg = (PMSG)lParam;
	if (pMsg->message==WM_KEYUP) {
		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x8000;//����Ƿ���
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