// Magic.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Magic.h"
#include "D3DApplication.h"

/** ��������� */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	CD3DApplication * appl = CD3DApplication::Create("D3DApplication");			
	if (appl != 0)
	{
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, "ħ��",nCmdShow);
		delete appl;													
	}
	else
	{														
		MessageBox(HWND_DESKTOP, "�����������", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}			