// 格拉布斯筛选器Win.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include"格拉布斯筛选器Win.h"

// 全局变量: 
HINSTANCE hInst;								// 当前实例
double g0;
HWND hDlg;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  在此放置代码。
	hInst = hInstance;
	SetHook(TRUE);//设置全局快捷键
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, About);
	SetHook(FALSE);
	return GetLastError();
}

VOID OnInit()
{
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_WIN));
	SendMessage(hDlg, WM_SETICON, 0, (LPARAM)hIcon);

	HWND hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	ComboBox_AddString(hCombo, _T("0.9"));
	ComboBox_AddString(hCombo, _T("0.95"));
	ComboBox_AddString(hCombo, _T("0.975"));
	ComboBox_AddString(hCombo, _T("0.99"));
	ComboBox_AddString(hCombo, _T("0.995"));
}

BOOL IsClipDataValid(LPTSTR p)//检查剪贴板内容是否合法
{
	if (_tcschr(p, _T('±')) != NULL) {//注意 _T('±')为Unicode字符
		return FALSE;
	}
	return TRUE;
}


VOID OnCaculate()
{
	TCHAR szCp[10];
	GetDlgItemText(hDlg, IDC_COMBO1, szCp, _countof(szCp));
	double Confidence_probability = _ttof(szCp);
	if (Confidence_probability == 0) {
		MessageBox(hDlg, _T("请选择置信概率！"), _T("格拉布斯筛选器"), MB_ICONERROR);
		return;
	}

	if (!OpenClipboard(NULL)) {
		MessageBox(hDlg, _T("无法从剪贴板中读取数据！"), _T("格拉布斯筛选器"), MB_ICONERROR);
		return;
	}

	HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
	LPTSTR pData = (LPTSTR)GlobalLock(hMem);
	if (!IsClipDataValid(pData)){//剪贴板内容无需计算，直接退出
		GlobalUnlock(hMem);
		CloseClipboard();
		return;
	}


	HWND hList1 = GetDlgItem(hDlg, IDC_LIST1);
	HWND hList2 = GetDlgItem(hDlg, IDC_LIST2);
	ListBox_ResetContent(hList1);
	ListBox_ResetContent(hList2);

	int size = 0;
	for (LPTSTR p = pData; *p != '\0'; p++) {//替换分隔符为统一空格
		if (*p == '\n') {//纵向
			*(p - 1) = ' ';
			*p = ' ';
			size++;
		}
		else if (*p == '\t') {//横向
			*p = ' ';
			size++;
		}
	}

	g0 = GetG0(Confidence_probability, size);
	static int g0Counter = 0;//过滤信号

	if (g0 == -1) {//补充g0
		TCHAR szg0[10];
		GetDlgItemText(hDlg, IDC_EDIT1, szg0, _countof(szg0));
		g0 = _ttof(szg0);
		if (g0 == 0) {
			g0Counter++;
			if (g0Counter == 1) {
				MessageBox(NULL, _T("数据库中不存在相应g0值\n请输入补充g0！"), _T("格拉布斯筛选器"), MB_ICONERROR);//父窗口为NULL，无须弹出窗口
			}
			GlobalUnlock(hMem);
			CloseClipboard();
			return;
		}
	}


	double*arr = new double[size];
	LPTSTR p = pData;
	for (int i = 0; i < size; i++) {
		arr[i] = _ttof(p);
		while (true) {
			p++;
			if (*p == ' ') {//跳过空格
				p += 1;//空格转换后对数字无影响
				break;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		TCHAR szNum[40];
		swprintf_s(szNum, _countof(szNum), _T("%f"), arr[i]);
		ListBox_AddString(hList1, szNum);
	}

	sData* psdt = gelabusi(arr, size);


	TCHAR sz[80];
	HGLOBAL hM2 = GlobalAlloc(GMEM_MOVEABLE, sizeof(sz));
	LPTSTR pM2 = (LPTSTR)GlobalLock(hM2);
	swprintf_s(pM2, _countof(sz), _T("%f±%f"), psdt->Ave, psdt->biaozhuncha);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, pM2);
	GlobalUnlock(hM2);

	swprintf_s(sz, _countof(sz), _T("处理后数组长度：%d   平均数： %f±%f"), psdt->size, psdt->Ave, psdt->biaozhuncha);
	ListBox_AddString(hList2, sz);
	ListBox_AddString(hList2, _T("结果已复制到粘贴板"));


	g0Counter = 0;//初始化信号
	GlobalUnlock(hMem);
	CloseClipboard();


	delete psdt;
	delete[]arr;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		::hDlg = hDlg;
		OnInit();
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			OnCaculate();
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}