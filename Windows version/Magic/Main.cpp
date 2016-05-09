// Magic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Magic.h"
#include "D3DApplication.h"

/** 主程序入口 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	CD3DApplication * appl = CD3DApplication::Create("D3DApplication");			
	if (appl != 0)
	{
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, "魔方",nCmdShow);
		delete appl;													
	}
	else
	{														
		MessageBox(HWND_DESKTOP, "创建程序出错", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}			