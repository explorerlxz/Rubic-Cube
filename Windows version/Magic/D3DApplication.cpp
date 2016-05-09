#include "StdAfx.h"
#include "D3DApplication.h"
#include "resource.h"

#pragma comment(lib, "d3d9.lib")	
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")


const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

/** 处理窗口消息 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);					/**< 返回用户自定义的32位的程序附加值 */
	if (user_data == 0)
	{	
		/// 如果程序第一次运行
		if (uMsg == WM_CREATE)											/**< 处理窗口创建消息 */
		{																
			/// 返回窗口结构的指针，它保存刚创建的程序实例的类
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			/// 获得程序实例的指针
			CD3DApplication * appl = reinterpret_cast<CD3DApplication *>(creation->lpCreateParams);
			/// 保存程序实例的指针为用户自定义的程序附加值
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			appl->m_IsVisible = true;									/**< 设置程序可见 */
			return 0;													/**< 返回 */
		}
	}
	else
	{	/// 如果不是第一次窗口，返回程序实例的指针
		CD3DApplication * appl = reinterpret_cast<CD3DApplication *>(user_data);
		return appl->Message(hWnd, uMsg, wParam, lParam);				/**< 调用程序实例自己的消息处理函数 */
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< 调用默认的窗口消息处理函数 */

}


CD3DApplication::CD3DApplication(const char * class_name)
{
	m_ClassName = class_name;																						
	m_IsVisible = false;																								
	m_LastTickCount = 0;

	m_WindowWidth	= 1200;												
	m_WindowHeight	= 800;	

	m_Device = NULL;

}

CD3DApplication::~CD3DApplication(void)
{
}


BOOL CD3DApplication::InitD3D(HINSTANCE hInstance, const char *WindowText, int width, int height, bool windowed, D3DDEVTYPE deviceType)
{

	WNDCLASSEX windowClass;												
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						
	windowClass.cbSize			= sizeof(WNDCLASSEX);					
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				
	windowClass.hInstance		= hInstance;							
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	windowClass.lpszClassName	= m_ClassName;							
	if (RegisterClassEx(&windowClass) == 0)								
	{																	
		MessageBox(HWND_DESKTOP, "注册窗口失败!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														
	}

	// Set the window's initial style
	DWORD dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_EX_TOPMOST;

	int wid = GetSystemMetrics(SM_CXSCREEN);		/**< 获取当前屏幕宽 */
	int hei = GetSystemMetrics(SM_CYSCREEN);		/**< 获取当前屏幕高 */
	int nX = (wid - m_WindowWidth) / 2;                    /**< 计算窗口居中用 */
	int nY = (hei - m_WindowHeight) / 2;	

	// Create the render window
	m_hWindow = CreateWindow( m_ClassName, WindowText, dwWindowStyle,
		nX, nY,
		width, height, 0,
		NULL, hInstance, this);

	if( !m_hWindow )
	{
		::MessageBox(0, "创建窗口失败！", 0, 0);
		return false;
	}
	::ShowWindow(m_hWindow, SW_SHOW);
	::UpdateWindow(m_hWindow);	

	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if( !d3d9 )
	{
		::MessageBox(0, "Direct3DCreate9 失败！", 0, 0);
		return false;
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = m_hWindow;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		m_hWindow,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&m_Device);            // return created device

	if( FAILED(hr) )
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			m_hWindow,
			vp,
			&d3dpp,
			&m_Device);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() 失败！", 0, 0);
			return false;
		}
	}
	d3d9->Release(); // done with d3d9 object

	return true;
}


void CD3DApplication::TerminateApplication()							
{
	PostMessage(m_hWindow, WM_QUIT, 0, 0);								
}

/** 消息循环 */
LRESULT CD3DApplication::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)														/**< 处理不同的Windows消息 */
	{
	case WM_SYSCOMMAND:												/**< 截获系统命令 */
		switch (wParam)												
		{
		case SC_SCREENSAVE:										/**< 截获屏幕保护启动命令 */
		case SC_MONITORPOWER:									/**< 截获显示其省电模式启动命令 */
			return 0;											/**< 不启用这两个命令 */
			break;
		}
		break;															/**< 退出 */

	case WM_CLOSE:													/**< 关闭窗口 */
		TerminateApplication();										/**< 调用TerminateApplication函数 */
		return 0;													
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		m_LastTickCount = GetTickCount();							/**< 更新计数器的值 */
		return 0;
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		OnMouseDown(wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnMouseUp(wParam, lParam);
		break;
	case WM_MOVE:
		return 0;
		break;

	case WM_PAINT:
		Draw();	
		break;

	case WM_SIZING:													/**< 窗口正在改变大小 */
		{
			RECT * rect = (RECT *)lParam;
			return TRUE;
		}
		break;

	case WM_SIZE:													/**< 窗口改变大小后 */
		switch (wParam)												/**< 处理不同的窗口状态 */
		{
		case SIZE_MINIMIZED:									/**< 是否最小化? */
			m_IsVisible = false;								/**< 如果是，则设置不可见 */
			return 0;											
			break;

		case SIZE_MAXIMIZED:									/**< 窗口是否最大化? */
		case SIZE_RESTORED:										/**< 窗口被还原? */
			m_IsVisible = true;									/**< 设置为可见 */
			m_LastTickCount = GetTickCount();					/**< 更新计数器的值 */
			return 0;											
			break;
		}
		break;															

	case WM_KEYDOWN:												/**< 更新按键信息 */
		m_Keys.SetPressed(wParam);									
		return 0;													
		break;

	case WM_KEYUP:													/**< 更新释放键信息 */
		m_Keys.SetReleased(wParam);									
		return 0;													
		break;											
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< 调用默认的窗口消息处理函数 */
}


int CD3DApplication::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, const char *WindowText, int nCmdShow)
{
	/// 如果初始化D3D失败，则退出
	if (InitD3D(hInstance, WindowText, m_WindowWidth, m_WindowHeight, true, D3DDEVTYPE_HAL) == false)									    /**< 调用自定义的初始化函数 */
	{															/**< 失败 */
		TerminateApplication();									/**< 关闭窗口退出程序 */
	}

	/// 如果初始化失败，则退出
	if (Init() == false)									    /**< 调用自定义的初始化函数 */
	{															/**< 失败 */
		TerminateApplication();									/**< 关闭窗口退出程序 */
	}

	MSG msg;																
	m_LastTickCount = GetTickCount();						
	m_Keys.Clear();	

	bool isMessagePumpActive = true;	
	while (isMessagePumpActive)											
	{														
		if (PeekMessage(&msg, m_hWindow, 0, 0, PM_REMOVE) != 0)
		{													
			/// 检测是否为WM_QUIT消息
			if (msg.message != WM_QUIT)						
			{
				DispatchMessage(&msg);						
			}
			else
			{
				isMessagePumpActive = false;				/**< 如果是，则退出 */
			}
		}
		else	 // 如果没有消息											
		{
			if (m_IsVisible == false)						
			{
				WaitMessage();								
			}
			else											
			{												
				DWORD tickCount = GetTickCount();			
				Update(tickCount - m_LastTickCount);		
				m_LastTickCount = tickCount;				
				Draw();										
			}
		}
	}

	UnregisterClass(m_ClassName, hInstance);							/**< 取消注册的窗口 */
	return 0;
}
