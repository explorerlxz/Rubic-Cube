#include "StdAfx.h"
#include "D3DApplication.h"
#include "resource.h"

#pragma comment(lib, "d3d9.lib")	
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")


const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

/** ��������Ϣ */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);					/**< �����û��Զ����32λ�ĳ��򸽼�ֵ */
	if (user_data == 0)
	{	
		/// ��������һ������
		if (uMsg == WM_CREATE)											/**< �����ڴ�����Ϣ */
		{																
			/// ���ش��ڽṹ��ָ�룬������մ����ĳ���ʵ������
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			/// ��ó���ʵ����ָ��
			CD3DApplication * appl = reinterpret_cast<CD3DApplication *>(creation->lpCreateParams);
			/// �������ʵ����ָ��Ϊ�û��Զ���ĳ��򸽼�ֵ
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			appl->m_IsVisible = true;									/**< ���ó���ɼ� */
			return 0;													/**< ���� */
		}
	}
	else
	{	/// ������ǵ�һ�δ��ڣ����س���ʵ����ָ��
		CD3DApplication * appl = reinterpret_cast<CD3DApplication *>(user_data);
		return appl->Message(hWnd, uMsg, wParam, lParam);				/**< ���ó���ʵ���Լ�����Ϣ������ */
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */

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
		MessageBox(HWND_DESKTOP, "ע�ᴰ��ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														
	}

	// Set the window's initial style
	DWORD dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_EX_TOPMOST;

	int wid = GetSystemMetrics(SM_CXSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
	int hei = GetSystemMetrics(SM_CYSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
	int nX = (wid - m_WindowWidth) / 2;                    /**< ���㴰�ھ����� */
	int nY = (hei - m_WindowHeight) / 2;	

	// Create the render window
	m_hWindow = CreateWindow( m_ClassName, WindowText, dwWindowStyle,
		nX, nY,
		width, height, 0,
		NULL, hInstance, this);

	if( !m_hWindow )
	{
		::MessageBox(0, "��������ʧ�ܣ�", 0, 0);
		return false;
	}
	::ShowWindow(m_hWindow, SW_SHOW);
	::UpdateWindow(m_hWindow);	

	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if( !d3d9 )
	{
		::MessageBox(0, "Direct3DCreate9 ʧ�ܣ�", 0, 0);
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
			::MessageBox(0, "CreateDevice() ʧ�ܣ�", 0, 0);
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

/** ��Ϣѭ�� */
LRESULT CD3DApplication::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)														/**< ����ͬ��Windows��Ϣ */
	{
	case WM_SYSCOMMAND:												/**< �ػ�ϵͳ���� */
		switch (wParam)												
		{
		case SC_SCREENSAVE:										/**< �ػ���Ļ������������ */
		case SC_MONITORPOWER:									/**< �ػ���ʾ��ʡ��ģʽ�������� */
			return 0;											/**< ���������������� */
			break;
		}
		break;															/**< �˳� */

	case WM_CLOSE:													/**< �رմ��� */
		TerminateApplication();										/**< ����TerminateApplication���� */
		return 0;													
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		m_LastTickCount = GetTickCount();							/**< ���¼�������ֵ */
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

	case WM_SIZING:													/**< �������ڸı��С */
		{
			RECT * rect = (RECT *)lParam;
			return TRUE;
		}
		break;

	case WM_SIZE:													/**< ���ڸı��С�� */
		switch (wParam)												/**< ����ͬ�Ĵ���״̬ */
		{
		case SIZE_MINIMIZED:									/**< �Ƿ���С��? */
			m_IsVisible = false;								/**< ����ǣ������ò��ɼ� */
			return 0;											
			break;

		case SIZE_MAXIMIZED:									/**< �����Ƿ����? */
		case SIZE_RESTORED:										/**< ���ڱ���ԭ? */
			m_IsVisible = true;									/**< ����Ϊ�ɼ� */
			m_LastTickCount = GetTickCount();					/**< ���¼�������ֵ */
			return 0;											
			break;
		}
		break;															

	case WM_KEYDOWN:												/**< ���°�����Ϣ */
		m_Keys.SetPressed(wParam);									
		return 0;													
		break;

	case WM_KEYUP:													/**< �����ͷż���Ϣ */
		m_Keys.SetReleased(wParam);									
		return 0;													
		break;											
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */
}


int CD3DApplication::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, const char *WindowText, int nCmdShow)
{
	/// �����ʼ��D3Dʧ�ܣ����˳�
	if (InitD3D(hInstance, WindowText, m_WindowWidth, m_WindowHeight, true, D3DDEVTYPE_HAL) == false)									    /**< �����Զ���ĳ�ʼ������ */
	{															/**< ʧ�� */
		TerminateApplication();									/**< �رմ����˳����� */
	}

	/// �����ʼ��ʧ�ܣ����˳�
	if (Init() == false)									    /**< �����Զ���ĳ�ʼ������ */
	{															/**< ʧ�� */
		TerminateApplication();									/**< �رմ����˳����� */
	}

	MSG msg;																
	m_LastTickCount = GetTickCount();						
	m_Keys.Clear();	

	bool isMessagePumpActive = true;	
	while (isMessagePumpActive)											
	{														
		if (PeekMessage(&msg, m_hWindow, 0, 0, PM_REMOVE) != 0)
		{													
			/// ����Ƿ�ΪWM_QUIT��Ϣ
			if (msg.message != WM_QUIT)						
			{
				DispatchMessage(&msg);						
			}
			else
			{
				isMessagePumpActive = false;				/**< ����ǣ����˳� */
			}
		}
		else	 // ���û����Ϣ											
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

	UnregisterClass(m_ClassName, hInstance);							/**< ȡ��ע��Ĵ��� */
	return 0;
}
