#pragma once

#include "resource.h"
#include"G0.h"
#include"..\Work\Work.h"
#pragma comment(lib,"..\\Release\\Work.lib")

struct sData
{
	double*arrOut;//����������׵�ַ
	double Ave;//ƽ����
	double biaozhuncha;//ƽ��ֵ�ı�׼��
	int size;//�����С
};

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
sData* gelabusi(double* arr, int size);