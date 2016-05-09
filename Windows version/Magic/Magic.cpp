#include "StdAfx.h"
#include "Magic.h"


#define  Z_Axle 0
#define  Y_Axle	1
#define  X_Axle	2

#define  FRONT_FACE  0
#define  UP_FACE  	 1
#define  SIDE_FACE	 2
#define  EPRECISION  0.001

#define   CHECK_COLOR 0


/** 创建一个程序的实例 */
CD3DApplication * CD3DApplication::Create(const char * class_name)
{
	CMagic * example = new CMagic(class_name);
	return reinterpret_cast<CD3DApplication *>(example);
}

CMagic::CMagic(const char * class_name):CD3DApplication(class_name)
{
	m_VB = NULL;
	m_IB = NULL;

	m_Font = NULL;
}

CMagic::~CMagic(void)
{
}

bool CMagic::Init()
{
	m_RotationAnimation.clear();
	m_RotationList.clear();
	
	m_Font = new CD3DFont("宋体", 16, 0);
	m_Font->InitDeviceObjects( m_Device );
	m_Font->RestoreDeviceObjects();

	srand(::GetTickCount());

	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type      = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse   = d3d::WHITE * 0.8f;
	dir.Specular  = d3d::WHITE * 0.8f;
	dir.Ambient   = d3d::WHITE * 0.8f;
	dir.Direction = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	m_Device->SetRenderState(D3DRS_LIGHTING, true);
	m_Device->SetLight(0, &dir);
	m_Device->LightEnable(0, true);

	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	float px = 550.0f; 
	float py = 400.0f; 
	float pz = -800.0f;

	D3DXMATRIX V;
	D3DXVECTOR3 position(px, py, pz);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	m_Device->SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)m_WindowWidth / (float)m_WindowHeight,
		1.0f,
		10000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &proj);

	m_RenderMode = true;
	m_AnimationBusy = false;
	m_bClick = false;
	m_bRotate    = false;
	m_Success = false;

	m_RotateAngle = 0.0f;
	m_RotateAxle = X_Axle;
	m_RotateFace = 0;

	m_LastPickFace = -1;
	m_PickFace     = -1;
	
	m_Number = 3;
	m_LastTime = 0;

	m_Start = -(CUBE_LEN*m_Number/2.0) + CUBE_LEN / 2.0;
	
	for (int k=0; k<m_Number; k++) //Z
	{
		for (int j=m_Number-1; j>=0; j--)//X
		{
			for (int i=m_Number-1; i>=0; i-- ) //Y
			{
				CCube TemCube;

				TemCube.SetDevice(m_Device);
				TemCube.LoadXFile(".\\resource\\mod.X");
				TemCube.SetPositon(m_Start+i*CUBE_LEN, m_Start+j*CUBE_LEN, m_Start+k*CUBE_LEN);

				m_Cubes.push_back(TemCube);
			}
		}
	}

	m_Device->CreateVertexBuffer(
		3 * 2 * 3 * sizeof(D3DXVECTOR3), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ,
		D3DPOOL_MANAGED,
		&m_VB,
		0);

	m_Device->CreateIndexBuffer(
		3 * 2 * 3 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		0);

	int Start = m_Start+(m_Number-0.5)*CUBE_LEN;
	int End   = m_Start-0.5*CUBE_LEN;

	//创建线框正方体
	D3DXVECTOR3* p = NULL;
	WORD* id	  = NULL;

	m_VB->Lock(0, 0, (void**)&p, 0);
	m_IB->Lock(0, 0, (void**)&id, 0);

	int k = 0;
	p[k+0] = D3DXVECTOR3(Start, End,   End);
	p[k+1] = D3DXVECTOR3(End, Start, End);
	p[k+2] = D3DXVECTOR3(Start,  Start, End);
	id[k+0] = k + 0; id[k+1] = k + 1; id[k+2] = k + 2;
	k = k + 3;


	p[k+0] = D3DXVECTOR3(Start, End,   End);
	p[k+1] = D3DXVECTOR3(End,   End, End);
	p[k+2] = D3DXVECTOR3(End,   Start, End);
	id[k+0] = k + 0; id[k+1] = k + 1; id[k+2] = k + 2;
	k = k  + 3;

	p[k+0] = D3DXVECTOR3(Start, Start, End);
	p[k+1] = D3DXVECTOR3(End,   Start, Start);
	p[k+2] = D3DXVECTOR3(Start, Start, Start);
	id[k+0] = k + 0; id[k+1] = k + 1; id[k+2] = k + 2;
	k = k  + 3;

	p[k+0] = D3DXVECTOR3(Start, Start, End);
	p[k+1] = D3DXVECTOR3(End,   Start, End);
	p[k+2] = D3DXVECTOR3(End,   Start, Start);
	id[k+0] = k + 0; id[k+1] = k + 1; id[k+2] = k + 2;
	k = k  + 3;

	p[k+0] = D3DXVECTOR3(Start, End, End);
	p[k+1] = D3DXVECTOR3(Start, Start, Start);
	p[k+2] = D3DXVECTOR3(Start, End, Start);
	id[k+0] = k + 0; id[k+1] = k + 1; id[k+2] = k + 2;
	k = k + 3;

	p[k+0] = D3DXVECTOR3(Start, End,   End);
	p[k+1] = D3DXVECTOR3(Start, Start, End);
	p[k+2] = D3DXVECTOR3(Start, Start, Start);
	id[k+0] = k + 0; id[k+1] = k + 1; id[k+2] = k + 2;
	k = k + 3;

	m_IB->Unlock();
	m_VB->Unlock();

	RandRotation();
	return true;
}


void  CMagic::Uninit()
{
	m_Cubes.clear();

	if (m_Font != NULL)
	{
		delete m_Font;
		m_Font = NULL;
	}


	if (m_VB != NULL)
	{
		m_VB->Release();
	}

	if (m_IB != NULL)
	{
		m_IB->Release();
	}
}


void  CMagic::Update(DWORD milliseconds)
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}
	/** 空格键切换绘制模式 */
	if(m_Keys.IsPressed(VK_SPACE))
	{
		m_RenderMode = !m_RenderMode;
		if (m_RenderMode)
		{
			printf("*******SetRenderState***********");
			m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		Sleep(200);
	}

	if (m_hWindow == ::GetFocus())
	{
		if(::GetAsyncKeyState('E') & 0x8000f )   /**<'E'键按下 */
		{
			Uninit();
			Init();

			Sleep(50);
		}

		if(::GetAsyncKeyState('C') & 0x8000f )   /**<'C'键按下 */
		{
			UnRotation();
			Sleep(50);
		}


		if(::GetAsyncKeyState('W') & 0x8000f )   /**<'W'键按下 */
		{
			Sleep(50);
		}

		if(::GetAsyncKeyState('X') & 0x8000f )   /**<'X'键按下 */
		{
			Sleep(50);
		}

		if(::GetAsyncKeyState('A') & 0x8000f ) /**< 'A'键按下 */
		{
			Sleep(50);
		}

		if(::GetAsyncKeyState('D') & 0x8000f ) /**<'D'键按下 */
		{
			Sleep(50);
		}
	}
	

	m_Speed = 25;
	if (::GetTickCount() - m_LastTime > m_Speed)
	{
		m_LastTime = ::GetTickCount();
	}

	DoAnimation();
}


void CMagic::OnMouseDown(WPARAM wParam, LPARAM lParam)
{
	if (m_AnimationBusy)
	{
		return;
	}

	m_LastPt.x = LOWORD(lParam);
	m_LastPt.y = HIWORD(lParam);

	m_RotateAngle = 0.0f;
	int PickFace = GePickFace(LOWORD(lParam), HIWORD(lParam));
	if (-1 != PickFace)
	{
		m_SelectIndex  = CalcPicking(LOWORD(lParam), HIWORD(lParam));
		if (m_SelectIndex < 0)
		{
			return;
		}

		m_StartPt.x = m_LastPt.x;
		m_StartPt.y = m_LastPt.y;

		
		m_LastPickFace = m_PickFace;
		m_PickFace     = PickFace;

		m_bClick = true;
		m_bRotate = true;
	}
}



void CMagic::OnMouseUp(WPARAM wParam, LPARAM lParam)	
{
	if (!m_bClick)
	{
		return;
	}

	m_bClick = false;
	if (m_bRotate) //在MouseMove 中没有移动,直接返回
	{
		return;
	}

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	float xDelt =  m_StartPt.x - x;
	float YDelt =  m_StartPt.y - y;

	float AllAng = xDelt / 100.0 + YDelt / 100.0; //计算总共移动了多少度？
	
	int flag = AllAng > 0 ? 1 : -1;                   //移动为正，还是负？
	int   n    = (int)( AllAng / (D3DX_PI*0.5) + (flag*0.75) ); //计算移动角度是90度的多少倍？如果倍数在大于等于0.25，就进为1倍
	float fix  = n*(D3DX_PI*0.5) - AllAng;     //计算与90度的倍的余数

	m_RotateAngle = fix; //额外移动“余数”，以使移动刚好为90度的倍数
	Rotation(m_RotateFace, 10, CHECK_COLOR);
}



void CMagic::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if (!m_bClick)
	{
		return;
	}


	float XDelt = m_LastPt.x - LOWORD(lParam);
	float YDelt = m_LastPt.y - HIWORD(lParam);

	if ( m_bRotate && abs( abs(XDelt) - abs(YDelt) ) < 10 )
	{
		return;
	}

	int PickFace = m_PickFace;
	if (m_bRotate)
	{
		m_bRotate = false;
		if (FRONT_FACE == PickFace && abs(XDelt) > abs(YDelt) ) // 前面, 水平方向
		{
			m_RotateAxle = Y_Axle ;
			m_RotateFace = GetYFace(m_SelectIndex); //取得Y是第几个面？
		}
		else if (FRONT_FACE == PickFace && abs(XDelt) < abs(YDelt) ) // 前面, 垂直方向
		{
			m_RotateAxle = X_Axle ;
			m_RotateFace = GetXFace(m_SelectIndex); //取得X是第几个面？
		}

		if (UP_FACE  == PickFace && abs(XDelt) > abs(YDelt) ) // 上面, 水平方向
		{
			m_RotateAxle = Z_Axle;
			m_RotateFace = GetZFace(m_SelectIndex); //取得Z是第几个面？
		}
		else if (UP_FACE  == PickFace  && abs(XDelt) < abs(YDelt) ) // 上面, 垂直方向
		{
			m_RotateAxle = X_Axle ;
			m_RotateFace = GetXFace(m_SelectIndex); //取得X是第几个面？
		}

		if (SIDE_FACE  == PickFace && abs(XDelt) > abs(YDelt) ) // 则面, 水平方向
		{
			m_RotateAxle = Y_Axle ;
			m_RotateFace = GetYFace(m_SelectIndex);  //取得Y是第几个面？
		}
		else if (SIDE_FACE  == PickFace && abs(XDelt) < abs(YDelt) ) // 则面, 垂直方向
		{
			m_RotateAxle = Z_Axle;
			m_RotateFace = GetZFace(m_SelectIndex); //取得Z是第几个面？
		}
	}

	m_RotateAngle = XDelt / 100.0 + YDelt / 100.0;
	RealRotation(m_RotateFace);

	m_LastPt.x = LOWORD(lParam);
	m_LastPt.y = HIWORD(lParam);

	//char DeStr[1024] = "";
	//sprintf(DeStr, "\n\n\n\n\na：%f, YDelt:%f, XDelt:%f\n\n", atan(YDelt/XDelt), YDelt, XDelt);
	//::OutputDebugString(DeStr);
}

int  CMagic::CalcPicking(int x, int y)
{
	    int Re = -1;
		for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
		{
			if ( m_Cubes[Index].TestHit(x, y) )
			{
				m_Cubes[Index].SetShowWire(!m_Cubes[Index].GetShowWire());
				Re = Index;
				break;
			}
		}

		return Re;
}

void  CMagic::Sort()
{
	for (int i=0; i<m_Number*m_Number*m_Number-1; i++) 
	{
		int k = i;
		for (int j=i+1; j<m_Number*m_Number*m_Number; j++) 
		{
			if ( m_Cubes[k].GetPPositionZ() - m_Cubes[j].GetPPositionZ() > EPRECISION)
			{
				k = j;
			}
			else if ( abs (m_Cubes[k].GetPPositionZ() - m_Cubes[j].GetPPositionZ()) < EPRECISION ) //Z相同, 比较Y
			{
				if ( m_Cubes[j].GetPPositionY() - m_Cubes[k].GetPPositionY() > EPRECISION)
				{
					k = j;
				}
				else if ( abs (m_Cubes[k].GetPPositionY() - m_Cubes[j].GetPPositionY()) < EPRECISION )  //Z相同，Y相同， 比较X
				{
					if ( m_Cubes[j].GetPPositionX() - m_Cubes[k].GetPPositionX() > EPRECISION )
					{
						k = j;
					}
				}
			}
		}

		if (k != i)
		{
			CCube Tem  = m_Cubes[k];
			m_Cubes[k] =  m_Cubes[i];
			m_Cubes[i] = Tem;
		}
	}

	/*char DeStr[1024] = "";
	sprintf(DeStr, "\n\n\n\n\n");
	::OutputDebugString(DeStr);
	for (int i=0; i<m_Number*m_Number*m_Number; i++) 
	{
		sprintf(DeStr, "%10.2f	%10.2f	%10.2f\n", m_Cubes[i].GetPPositionX(), m_Cubes[i].GetPPositionY(), m_Cubes[i].GetPPositionZ());
		::OutputDebugString(DeStr);

	}*/
}

void CMagic::Draw()
{
	if( m_Device )
	{
		D3DXMATRIX Tran;
	
		m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFE9F6EC, 1.0f, 0);
		m_Device->BeginScene();

		for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
		{
			m_Device->GetTransform(D3DTS_WORLD, &Tran);
			m_Cubes[Index].Draw();
			m_Device->SetTransform(D3DTS_WORLD, &Tran);
		}
		
		m_Device->GetTransform(D3DTS_WORLD, &Tran);

		m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_Device->SetStreamSource(0, m_VB, 0, 12);
		m_Device->SetIndices(m_IB);
		m_Device->SetFVF(D3DFVF_XYZ );
		m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);

		m_Device->SetTransform(D3DTS_WORLD, &Tran);

		char Txt[1024] = "";
		
		sprintf(Txt, "Press Key E to Produce randomly!");
		m_Font->DrawText(20, 50, 0xff000000, Txt);	

		sprintf(Txt, "Press Key C to Restore!");
		m_Font->DrawText(20, 80, 0xff000000, Txt);	

		if (m_Success)
		{
			sprintf(Txt, "Restored!");
			m_Font->DrawText(100, 120, 0xff000000, Txt);	
		}

		m_Device->EndScene();
		m_Device->Present(0, 0, 0, 0);
	}
}



int  CMagic::GePickFace(int x, int y)
{
	int bSeleFace = -1;

	D3DXVECTOR3 p0;
	D3DXVECTOR3 p1;
	D3DXVECTOR3 p2;
	D3DXVECTOR3 RayPos;
	D3DXVECTOR3 RayDir;
	FLOAT U;                     
	FLOAT V;                     
	FLOAT Dist; 

	GetRay(x, y, &RayPos, &RayDir);

	int End = m_Start+(m_Number-0.5)*CUBE_LEN;
	int Start   = m_Start-0.5*CUBE_LEN;

	//XY面,两个三解形 
	p0 = D3DXVECTOR3(End, Start,   Start);
	p1 = D3DXVECTOR3(Start, End, Start);
	p2 = D3DXVECTOR3(End,  End, Start);
	if ( D3DXIntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 0;
	}

	p0 = D3DXVECTOR3(End, Start,   Start);
	p1 = D3DXVECTOR3(Start,   Start, Start);
	p2 = D3DXVECTOR3(Start,   End, Start);
	if ( D3DXIntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 0;
	}

	//XZ面,两个三解形
	p0 = D3DXVECTOR3(End, End, Start);
	p1 = D3DXVECTOR3(Start,   End, End);
	p2 = D3DXVECTOR3(End, End, End);
	if ( D3DXIntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 1;
	}

	p0 = D3DXVECTOR3(End, End, Start);
	p1 = D3DXVECTOR3(Start,   End, Start);
	p2 = D3DXVECTOR3(Start,   End, End);
	if ( D3DXIntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 1;
	}

	//YZ面,两个三解形
	p0 = D3DXVECTOR3(End, Start, Start);
	p1 = D3DXVECTOR3(End, End, End);
	p2 = D3DXVECTOR3(End, Start, End);
	if ( D3DXIntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 2;
	}

	p0 = D3DXVECTOR3(End, Start,   Start);
	p1 = D3DXVECTOR3(End, End, Start);
	p2 = D3DXVECTOR3(End, End, End);
	if ( D3DXIntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 2;
	}

	//debug code 
	char DeStr[1024] = "";
	sprintf(DeStr, "bSeleFace: %d\n", bSeleFace);
	::OutputDebugString(DeStr);

	return bSeleFace;

}


void CMagic::GetRay(int x, int y, D3DXVECTOR3 *origin, D3DXVECTOR3 *direction)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	m_Device->GetViewport(&vp);

	D3DXMATRIX proj;
	m_Device->GetTransform(D3DTS_PROJECTION, &proj);

	px = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	*origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	*direction = D3DXVECTOR3(px, py, 1.0f);


	D3DXMATRIX view;
	m_Device->GetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse,	0, &view);

	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		origin,
		origin,
		&viewInverse);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		direction,
		direction,
		&viewInverse);

	// normalize the direction
	D3DXVec3Normalize(direction, direction);
}


int  CMagic::GetXFace(int Index)
{
	float X = m_Cubes[Index].GetPPositionX();

	for (int i=0; i<m_Number; i++)
	{
		if (abs( X -  (m_Start+i*CUBE_LEN))   < CUBE_LEN*0.1 )
		{
			return i;
		}
	}
	
}

int  CMagic::GetYFace(int Index)
{
	float Y = m_Cubes[Index].GetPPositionY();

	for (int i=0; i<m_Number; i++)
	{
		if ( abs( Y -  (m_Start+i*CUBE_LEN))   < CUBE_LEN*0.1 ) 
		{
			return i;
		}
	}
}

int  CMagic::GetZFace(int Index)
{
	float Z = m_Cubes[Index].GetPPositionZ();

	for (int i=0; i<m_Number; i++)
	{
		if ( abs( Z -  (m_Start+i*CUBE_LEN))   < CUBE_LEN*0.1 ) 
		{
			return i;
		}
	}
}

void CMagic::Rotation(int Face, int Speed, int Type)
{
	CRotationAnimation Tem;
	Tem.RotateAxle = m_RotateAxle;
	Tem.RotateFace = Face;
	Tem.RotateX    = m_RotateAngle;
	Tem.Step       = (m_RotateAngle / 20.0);
	Tem.Speed      = Speed;
	Tem.Type       = Type;

	m_RotationAnimation.push_back(Tem);
}

void CMagic::DoAnimation()
{

	if (m_RotationAnimation.size() <= 0)
	{
		return;
	}

	m_AnimationBusy = true;
	m_RotateAxle = m_RotationAnimation[0].RotateAxle;
	m_RotateAngle    = m_RotationAnimation[0].Step;
	RealRotation(m_RotationAnimation[0].RotateFace);

	if (m_RotationAnimation[0].Speed <= 0)
	{
		m_RotationAnimation[0].Speed = 5;
	}
	::Sleep(m_RotationAnimation[0].Speed);
	
	float AnimaRotateX = m_RotationAnimation[0].RotateX;
	float Step = m_RotationAnimation[0].Step;

	AnimaRotateX = AnimaRotateX - Step;
	if (abs(AnimaRotateX) < EPRECISION)
	{
		if (m_RotationAnimation[0].Type == CHECK_COLOR)
		{
			Sort();
			m_Success = CheckAllFaceColor();
		}

		m_RotationAnimation.erase(m_RotationAnimation.begin());
	}
	else
	{
		m_RotationAnimation[0].RotateX = AnimaRotateX;
	}
	
	if (m_RotationAnimation.size() <= 0)
	{
		m_AnimationBusy = false;
	}
}


void CMagic::RealRotation(int Face)
{
	if (m_RotateAxle == Z_Axle)
	{
		RotationXYFace(Face);
	}
	else if (m_RotateAxle == Y_Axle )
	{
		RotationXZFace(Face);
	}
	else if (m_RotateAxle == X_Axle )
	{
		RotationYZFace(Face);
	}
}


void CMagic::RotationXYFace(int Value)
{
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if (   abs( m_Cubes[Index].GetPPositionZ() - (m_Start+Value*CUBE_LEN) )  < CUBE_LEN*0.1 )
		{
			 m_Cubes[Index].SetRotation(0.0f, 0.0f, m_RotateAngle);
		}
	}
	
}


void CMagic::RotationXZFace(int Value)
{
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if ( abs( m_Cubes[Index].GetPPositionY() - (m_Start+Value*CUBE_LEN) )  < CUBE_LEN*0.1 )
		{
			m_Cubes[Index].SetRotation(0.0f, m_RotateAngle, 0.0f);
		}
	}
}

void CMagic::RotationYZFace(int Value)
{
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if ( abs( m_Cubes[Index].GetPPositionX() - (m_Start+Value*CUBE_LEN) )  < CUBE_LEN*0.1 )
		{
			m_Cubes[Index].SetRotation(m_RotateAngle, 0.0f, 0.0f);
		}
	}
}

bool CMagic::CheckAllFaceColor(void)
{
	//   1--蓝色
	//   2--红色
	//	 3--绿色
	//	 4--橙色
	//	 5--黄色
	//	 6--白色
	
	vector <vector<int>> FacesColors;
	FacesColors.clear();
	FacesColors.resize(CUBE_FACE_NUMBER);

	for (int i=0; i<CUBE_FACE_NUMBER; i++)
	{
		vector<int> Tem;
		Tem.clear();
		FacesColors[i] = Tem;
	}

	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		for (int i=1; i<=CUBE_FACE_NUMBER; i++)
		{
			D3DXVECTOR3 p[3];
			m_Cubes[Index].GetFaceVertex(i, p);

			int k = GetFaceInMagicFace(p);
			if (k > 0)
			{
				k = k - 1;
				FacesColors[k].push_back(m_Cubes[Index].GetFaceColor(i));
			}
		}
	}

	bool Re = true;
	for (int i=0; i<CUBE_FACE_NUMBER; i++)
	{
		int k = FacesColors[i][0];
		for (int j=1; j<m_Number*m_Number; j++)
		{
			if (k != FacesColors[i][j])
			{
				Re = false;
				return Re;
			}
		}
	}

	return Re;
}



int CMagic::GetFaceInMagicFace(D3DXVECTOR3 *p)
{
	float Start = m_Start-0.5*CUBE_LEN;
	float End   = m_Start+(m_Number-0.5)*CUBE_LEN;

	if (   abs(p[0].z - p[1].z) < EPRECISION*CUBE_LEN 
		&& abs(p[1].z - p[2].z) < EPRECISION*CUBE_LEN )
	{
		if (abs(p[0].z - Start) < EPRECISION*CUBE_LEN)
		{
			return 1;
		}
		else if (abs(p[0].z - End) < EPRECISION*CUBE_LEN)
		{
			return 3;
		}
	}

	if (   abs(p[0].x - p[1].x) < EPRECISION*CUBE_LEN 
		&& abs(p[1].x - p[2].x) < EPRECISION*CUBE_LEN )
	{
		if (abs(p[0].x - Start) < EPRECISION*CUBE_LEN)
		{
			return 4;
		}
		else if (abs(p[0].x - End) < EPRECISION*CUBE_LEN)
		{
			return 2;
		}

		//char DeStr[1024] = "";
		//sprintf(DeStr, " x1:%f	x2:%f	x3:%f\n", p[0].x, p[1].x, p[2].x);
		//::OutputDebugString(DeStr);
	}

	if (   abs(p[0].y - p[1].y) < EPRECISION*CUBE_LEN 
		&& abs(p[1].y - p[2].y) < EPRECISION*CUBE_LEN )
	{
		if (abs(p[0].y - Start) < EPRECISION*CUBE_LEN)
		{
			return 6;
		}
		else if (abs(p[0].y - End) < EPRECISION*CUBE_LEN)
		{
			return 5;
		}

		/*char DeStr[1024] = "";
		sprintf(DeStr, " y1:%f	y2:%f	y3:%f\n", p[0].y, p[1].y, p[2].y);
		::OutputDebugString(DeStr);*/
	}

	return -1;

}

void CMagic::RandRotation()
{
	srand(GetTickCount());

	m_RotationList.clear();
	int N = rand() % 50;
	for (int i=0; i< 30 + N ; i++)
	{
		m_RotateAngle = D3DX_PI*0.5;
		if (rand() < 0.5)
		{
			m_RotateAngle = -m_RotateAngle;
		}

		int RotateFace = rand() % m_Number;
		m_RotateAxle = rand() % 3;

		RealRotation(RotateFace);

		CRotation tem;
		tem.RotateX = m_RotateAngle;
		tem.RotateAxle = m_RotateAxle;
		tem.RotateFace = RotateFace;
		m_RotationList.push_back(tem);
	}	

	Sort();
}

void CMagic::UnRotation()
{
	if (m_RotationList.size() <= 0)
	{
		return;
	}

	for (int i=m_RotationList.size()-1; i>=0; i--)
	{
		CRotation tem = m_RotationList[i];

		m_RotateAxle = tem.RotateAxle;
		m_RotateAngle    = -tem.RotateX;

		int Type = -1;
		if (i == 0)
		{
			Type = CHECK_COLOR;
		}
		Rotation(tem.RotateFace, 20, Type);

	}

	m_RotationList.clear();
}

