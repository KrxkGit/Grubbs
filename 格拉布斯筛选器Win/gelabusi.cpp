#include"stdafx.h"
#include"格拉布斯筛选器Win.h"

extern double g0;
extern HWND hDlg;


inline double power2(double x)
{
	return x*x;
}

double Aver(double* arr, int size)
{
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += arr[i];
	}
	return sum / size;
}

double biaozhuncha(double *arr, int size)
{
	double sum = 0;
	double ave = Aver(arr, size);
	for (int i = 0; i < size; i++) {
		sum += power2(arr[i] - ave);
	}
	double fangcha = sum / (size - 1);//测量列标准差
	return sqrt(fangcha);
}

void printAbnormalNum(double num, double gi)
{
	//随着筛选的进行，异常gi会逐步放大
	TCHAR szNum[200];
	swprintf_s(szNum, _countof(szNum), _T("异常数字：%f gi: %f"), num, gi);
	ListBox_AddString(GetDlgItem(hDlg, IDC_LIST2), szNum);
}

sData* gelabusi(double* arr, int size)
{
	double ave = Aver(arr, size);
	double bzc = biaozhuncha(arr, size);

	double giMax = abs(arr[size - 1] - ave) / bzc;
	double giMin = abs(arr[0] - ave) / bzc;

	bool minAbnormal = (giMin >= g0);
	bool maxAbnormal = (giMax >= g0);

	if (minAbnormal || maxAbnormal) {//异常
		if (minAbnormal) {
			printAbnormalNum(arr[0], giMin);
		}
		else {
			printAbnormalNum(arr[size - 1], giMax);
		}

		return gelabusi(arr + minAbnormal, size - minAbnormal - maxAbnormal);//递归筛选
	}
	sData* psdt = new sData{ arr, ave, bzc / sqrt((double)size), size };
	return psdt;//返回筛选后大小
}