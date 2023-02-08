// ������˹ɸѡ��Win.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"������˹ɸѡ��Win.h"

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
double g0;
HWND hDlg;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  �ڴ˷��ô��롣
	hInst = hInstance;
	SetHook(TRUE);//����ȫ�ֿ�ݼ�
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

BOOL IsClipDataValid(LPTSTR p)//�������������Ƿ�Ϸ�
{
	if (_tcschr(p, _T('��')) != NULL) {//ע�� _T('��')ΪUnicode�ַ�
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
		MessageBox(hDlg, _T("��ѡ�����Ÿ��ʣ�"), _T("������˹ɸѡ��"), MB_ICONERROR);
		return;
	}

	if (!OpenClipboard(NULL)) {
		MessageBox(hDlg, _T("�޷��Ӽ������ж�ȡ���ݣ�"), _T("������˹ɸѡ��"), MB_ICONERROR);
		return;
	}

	HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
	LPTSTR pData = (LPTSTR)GlobalLock(hMem);
	if (!IsClipDataValid(pData)){//����������������㣬ֱ���˳�
		GlobalUnlock(hMem);
		CloseClipboard();
		return;
	}


	HWND hList1 = GetDlgItem(hDlg, IDC_LIST1);
	HWND hList2 = GetDlgItem(hDlg, IDC_LIST2);
	ListBox_ResetContent(hList1);
	ListBox_ResetContent(hList2);

	int size = 0;
	for (LPTSTR p = pData; *p != '\0'; p++) {//�滻�ָ���Ϊͳһ�ո�
		if (*p == '\n') {//����
			*(p - 1) = ' ';
			*p = ' ';
			size++;
		}
		else if (*p == '\t') {//����
			*p = ' ';
			size++;
		}
	}

	g0 = GetG0(Confidence_probability, size);
	static int g0Counter = 0;//�����ź�

	if (g0 == -1) {//����g0
		TCHAR szg0[10];
		GetDlgItemText(hDlg, IDC_EDIT1, szg0, _countof(szg0));
		g0 = _ttof(szg0);
		if (g0 == 0) {
			g0Counter++;
			if (g0Counter == 1) {
				MessageBox(NULL, _T("���ݿ��в�������Ӧg0ֵ\n�����벹��g0��"), _T("������˹ɸѡ��"), MB_ICONERROR);//������ΪNULL�����뵯������
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
			if (*p == ' ') {//�����ո�
				p += 1;//�ո�ת�����������Ӱ��
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
	swprintf_s(pM2, _countof(sz), _T("%f��%f"), psdt->Ave, psdt->biaozhuncha);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, pM2);
	GlobalUnlock(hM2);

	swprintf_s(sz, _countof(sz), _T("��������鳤�ȣ�%d   ƽ������ %f��%f"), psdt->size, psdt->Ave, psdt->biaozhuncha);
	ListBox_AddString(hList2, sz);
	ListBox_AddString(hList2, _T("����Ѹ��Ƶ�ճ����"));


	g0Counter = 0;//��ʼ���ź�
	GlobalUnlock(hMem);
	CloseClipboard();


	delete psdt;
	delete[]arr;
}

// �����ڡ������Ϣ�������
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