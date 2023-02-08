#pragma once

#include "resource.h"
#include"G0.h"
#include"..\Work\Work.h"
#pragma comment(lib,"..\\Release\\Work.lib")

struct sData
{
	double*arrOut;//处理后数组首地址
	double Ave;//平均数
	double biaozhuncha;//平均值的标准差
	int size;//数组大小
};

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
sData* gelabusi(double* arr, int size);