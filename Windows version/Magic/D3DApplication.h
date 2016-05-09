#pragma once

#include <d3dx9.h>

struct Vertex
{
	Vertex(){}

	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		_x  = x;  _y  = y;	_z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;   _v = v;
	}
	float  _x,  _y,  _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};


/** 定义键盘类 */
class Keys														 
{
public:
	/** 构造函数 */
	Keys() { Clear(); }
	/** 清空所有的按键信息 */
	void Clear() { ZeroMemory(&m_KeyDown, sizeof(m_KeyDown)); }
	/** 判断某个键是否按下 */
	bool IsPressed(unsigned int key)	{ return (key < MAX_KEYS) ? (m_KeyDown[key] == true) : false; }
	/** 设置某个键被按下 */
	void SetPressed(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = true; }
	/** 设置某个键被释放 */
	void SetReleased(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = false; }

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_KeyDown[MAX_KEYS];									/**< 保存256个按键的状态 */
};

class CD3DApplication
{
protected:
	IDirect3DDevice9* m_Device;

	Keys	m_Keys;			

	int		m_WindowWidth;												/**< 窗口的宽度 */
	int		m_WindowHeight;												/**< 窗口的高度 */

	HWND        m_hWindow;
	const char* m_ClassName;									/**< 程序名 */
	bool		m_IsVisible;									/**< 窗口是否可见 */
	DWORD		m_LastTickCount;								/**< 上一次计时器的值 */
public:
	CD3DApplication(const char * class_name);
	~CD3DApplication(void);

	static			CD3DApplication * Create(const char * class_name);	
	BOOL			InitD3D(HINSTANCE hInstance, const char *WindowText, int width, int height, bool windowed, D3DDEVTYPE deviceType);
	LRESULT			Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, const char *WindowText, int nCmdShow);
	void			TerminateApplication();

	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool	Init() = 0;							
	virtual void	Uninit() = 0;						
	virtual void	Update(DWORD milliseconds) = 0;		
	virtual void	Draw() = 0;		

	virtual void	OnMouseDown(WPARAM wParam, LPARAM lParam) = 0;	
	virtual void	OnMouseUp(WPARAM wParam, LPARAM lParam) = 0;
	virtual void	OnMouseMove(WPARAM wParam, LPARAM lParam) = 0;
	
};
