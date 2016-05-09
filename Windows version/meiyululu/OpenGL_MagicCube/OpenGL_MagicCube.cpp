
//=========================================================================
/**
*  @file      OpenGL_MagicCube.cpp
*
*  文件描述:  程序框架具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  在这个类中您必须重载如下几个虚函数
*																								
*	virtual bool Init();														
*		执行所有的初始化工作，如果成功函数返回true							
*																			
*	virtual void Uninit();													
*		执行所有的卸载工作										
*																			
*	virtual void Update(DWORD milliseconds);										
*		执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位
*																			
*	virtual void Draw();															
*		执行所有的绘制操作
*/
//=========================================================================

#include "stdafx.h"
#include "OpenGL_MagicCube.h"						    


/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	OpenGL_MagicCube * test = new OpenGL_MagicCube(class_name);
	return reinterpret_cast<GLApplication *>(test);
}


/** 构造函数 */
OpenGL_MagicCube::OpenGL_MagicCube(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	m_RenderMode = true;
	sp = false;
    
}

/** 设置光源 */
void OpenGL_MagicCube::SetLight()
{
	// 定义光源的属性值 
	GLfloat LightAmbient[]=  { 0.3f, 0.3f, 0.3f, 1.0f }; 	// 环境光参数 
	GLfloat LightDiffuse[]=  { 0.5f, 0.5f, 0.5f, 1.0f };		// 漫射光参数 
	GLfloat LightSpecular[]= { 0.6f, 0.6f, 0.6f, 1.0f };	// 镜面光参数 
	GLfloat LightDir[]= { -1.0f,  -1.0f,  -1.0f, 1.0f };	// 光源位置 

	// 设置光源的属性值 
	glLightfv(GL_LIGHT1, GL_AMBIENT,        LightAmbient);		// 设置环境光 
	glLightfv(GL_LIGHT1, GL_DIFFUSE,        LightDiffuse);		// 设置漫射光 
	glLightfv(GL_LIGHT1, GL_SPECULAR,       LightSpecular);	// 设置漫射光 
	
	// 启用光源 
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

}

/** 初始化OpenGL */
bool OpenGL_MagicCube::Init()									
{
	m_RotationAnimation.clear();
	m_RotationList.clear();

/** 用户自定义的初始化过程 */
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	
	SetLight();

	/** 初始化字体 */
	if(!m_Font.InitFont())
		MessageBox(NULL,"初始化字体失败!","错误",MB_OK);
	
	m_RenderMode = true;
	m_AnimationBusy = false;
	m_bClick = false;
	m_bRotate    = false;
	m_Success = false;

	m_AllAngle    = 0.0;
	m_RotateAngle = 0.0f;
	m_RotateAxle = X_Axle;
	m_RotateFace = 0;

	m_LastPickFace = -1;
	m_PickFace     = -1;
	m_Number = 3;
	
	m_Start = -(CUBE_LEN*m_Number/2.0) + CUBE_LEN / 2.0;
	for (int k=0; k<m_Number; k++) //Z
	{
		for (int j=m_Number-1; j>=0; j--)//X
		{
			for (int i=m_Number-1; i>=0; i-- ) //Y
			{
				CCube TemCube;
	
				TemCube.LoadModelFile("Mod.3ds");
				TemCube.SetPositon(m_Start+i*CUBE_LEN, m_Start+j*CUBE_LEN, -m_Start-k*CUBE_LEN);

				m_Cubes.push_back(TemCube);
			}
		}
	}


    /** 设置摄像机 */
	m_CameraPosition = Vector3( 600.0f, 400.0f, 800.0f);
	m_CameraView	 = Vector3(0.0f, 0.0f, 0.0f);
	m_CameraUp       = Vector3(0.0f, 1.0f, 0.0f);

	ResizeDraw(true);	                             /**< 改变OpenGL窗口大小，直接调用子类的函数 */

	RandRotation();
	return true;                                        /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void OpenGL_MagicCube::Uninit()									
{
	m_Cubes.clear();
}


/** 程序更新函数 */
void OpenGL_MagicCube::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

	/** 空格键切换绘制模式 */
	if(m_Keys.IsPressed(VK_SPACE) && !sp)
	{
		sp = true;
		m_RenderMode = !m_RenderMode;
		if(m_RenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (HWND(m_Window) == ::GetFocus())
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

	if(!m_Keys.IsPressed(VK_SPACE))
	{
		sp = false;
	}

	DoAnimation(); 
}

/** 输出文字信息 */
void  OpenGL_MagicCube::PrintText()
{
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */

	glColor3f(0.0f, 0.0f, 0.0f);                      /**< 设置文字颜色 */
	sprintf(string,"摄像机当前位置:X=%3.1f  Y=%3.1f  Z=%3.1f",   
		m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z); /**< 字符串赋值 */
	m_Font.PrintText(string, -5.0f, 3.8f);

	sprintf(string,"请按E键重新产生一个魔方!");              
	m_Font.PrintText(string, -5.0f, 3.5f);  

	sprintf(string,"请按C开始动画还原魔方!");              
	m_Font.PrintText(string, -5.0f, 3.2f);  

	if (m_Success)
	{
		sprintf(string,"已经成功还原魔方!");              
		m_Font.PrintText(string, -5.0f, 2.6f);             

	}

	glPopAttrib();
		
}


/** 显示3DS模型 */
void OpenGL_MagicCube::ShowCubes()
{
	for (int i=0; i<m_Number*m_Number*m_Number; i++)
	{
		glPushMatrix();
		m_Cubes[i].Draw();                         /**< 显示3DS模型 */
		glPopMatrix();
	}

	

}

/** 绘制函数 */
void OpenGL_MagicCube::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	/** 放置摄像机 */	
	gluLookAt(m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z,	
		m_CameraView.x,	 m_CameraView.y,     m_CameraView.z,	
		m_CameraUp.x, m_CameraUp.y, m_CameraUp.z);
	
	glGetDoublev(GL_MODELVIEW_MATRIX,  m_ModelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, m_ProjMatrix);
	glGetIntegerv(GL_VIEWPORT,		   m_Viewport);

	ShowCubes();
	
	/** 输出屏幕信息 */ 
	PrintText();
		
	glFlush();	                 /**< 强制执行所有的OpenGL命令 */
}


void OpenGL_MagicCube::OnMouseDown(WPARAM wParam, LPARAM lParam)
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
		m_AllAngle = 0.0;
	}
}



void OpenGL_MagicCube::OnMouseUp(WPARAM wParam, LPARAM lParam)	
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

	float AllAng = m_AllAngle; //计算总共移动了多少度？
	m_AllAngle = 0.0;

	int flag = AllAng > 0 ? 1 : -1;                   //移动为正，还是负？
	int   n    = (int)( AllAng / 90 + (flag*0.75) ); //计算移动角度是90度的多少倍？如果大于等于 (1-0.75)倍数，就进为1倍
	float fix  = n*90 - AllAng;       //计算与90度的倍的余数
	
	m_RotateAngle = fix ; //额外移动“余数”，以使移动刚好为90度的倍数
	Rotation(m_RotateFace, 10, CHECK_COLOR);	
}



void OpenGL_MagicCube::OnMouseMove(WPARAM wParam, LPARAM lParam)
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

	if (   (SIDE_FACE  == PickFace   && m_RotateAxle == Z_Axle)
		|| (  UP_FACE  == PickFace ) && m_RotateAxle == Z_Axle)
	{
		m_RotateAngle = (XDelt/2.0 + YDelt/2.0);
	}
	else
	{
		m_RotateAngle = -(XDelt/2.0 + YDelt/2.0);
	}

	RealRotation(m_RotateFace);
	m_AllAngle = m_AllAngle + m_RotateAngle;

	m_LastPt.x = LOWORD(lParam);
	m_LastPt.y = HIWORD(lParam);
}


int  OpenGL_MagicCube::GePickFace(int x, int y)
{
	int bSeleFace = -1;

	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 RayPos;
	Vector3 RayDir;
	FLOAT U;                     
	FLOAT V;                     
	FLOAT Dist; 

	GetRay(x, y, &RayPos, &RayDir);

	int End = m_Start+(m_Number-0.5)*CUBE_LEN;
	int Start   = m_Start-0.5*CUBE_LEN;

	int ZEnd = -m_Start-(m_Number-0.5)*CUBE_LEN;
	int ZStart   = -m_Start+0.5*CUBE_LEN;

	//XY面,两个三解形 
	p0 = Vector3(End, Start,   ZStart);
	p1 = Vector3(Start, End,   ZStart);
	p2 = Vector3(End,  End,    ZStart);
	if ( IntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 0;
	}

	p0 = Vector3(End, Start,   ZStart);
	p1 = Vector3(Start,   Start, ZStart);
	p2 = Vector3(Start,   End, ZStart);
	if ( IntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 0;
	}

	//XZ面,两个三解形
	p0 = Vector3(End, End, ZStart);
	p1 = Vector3(Start,   End, ZEnd);
	p2 = Vector3(End, End, ZEnd);
	if ( IntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 1;
	}

	p0 = Vector3(End, End, ZStart);
	p1 = Vector3(Start,   End, ZStart);
	p2 = Vector3(Start,   End, ZEnd);
	if ( IntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 1;
	}

	//YZ面,两个三解形
	p0 = Vector3(End, Start, ZStart);
	p1 = Vector3(End, End, ZEnd);
	p2 = Vector3(End, Start, ZEnd);
	if ( IntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 2;
	}

	p0 = Vector3(End, Start,   ZStart);
	p1 = Vector3(End, End, ZStart);
	p2 = Vector3(End, End, ZEnd);
	if ( IntersectTri(&p0, &p1, &p2, &RayPos, &RayDir, &U, &V, &Dist) )
	{
		bSeleFace = 2;
	}

	//debug code 
	char DeStr[1024] = "";
	sprintf(DeStr, "bSeleFace: %d\n", bSeleFace);
	::OutputDebugString(DeStr);

	return bSeleFace;

}


void OpenGL_MagicCube::GetRay(int x, int y, Vector3 *origin, Vector3 *direction)
{
	GLdouble  o1x, o1y, o1z;
	GLdouble  o2x, o2y, o2z;

	gluUnProject((GLdouble)x,  (GLdouble)(m_Viewport[3] - y),  0.0,  m_ModelMatrix,  m_ProjMatrix,  m_Viewport,  &o1x,  &o1y,  &o1z);
	gluUnProject((GLdouble)x,  (GLdouble)(m_Viewport[3] - y),  1.0,  m_ModelMatrix,  m_ProjMatrix,  m_Viewport,  &o2x,  &o2y,  &o2z);

	*origin     =  Vector3((GLfloat)o1x,  (GLfloat)o1y,  (GLfloat)o1z);
	*direction  =  Vector3((GLfloat)o2x,  (GLfloat)o2y,  (GLfloat)o2z);

	*direction = *direction - *origin;
	direction->normalize();

}


int  OpenGL_MagicCube::GetXFace(int Index)
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

int  OpenGL_MagicCube::GetYFace(int Index)
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

int  OpenGL_MagicCube::GetZFace(int Index)
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

void OpenGL_MagicCube::Rotation(int Face, int Speed, int Type)
{
	CRotationAnimation Tem;
	Tem.RotateAxle = m_RotateAxle;
	Tem.RotateFace = Face;
	Tem.RotateX    = m_RotateAngle;
	Tem.Step       = (m_RotateAngle / 10.0);
	Tem.Speed      = Speed;
	Tem.Type       = Type;

	m_RotationAnimation.push_back(Tem);
}

void OpenGL_MagicCube::DoAnimation()
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


void OpenGL_MagicCube::RealRotation(int Face)
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


void OpenGL_MagicCube::RotationXYFace(int Value)
{
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if (   abs( m_Cubes[Index].GetPPositionZ() - (m_Start+Value*CUBE_LEN) )  < CUBE_LEN*0.1 )
		{
			 m_Cubes[Index].SetRotation(0.0f, 0.0f, m_RotateAngle);
		}
	}
	
}


void OpenGL_MagicCube::RotationXZFace(int Value)
{
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if ( abs( m_Cubes[Index].GetPPositionY() - (m_Start+Value*CUBE_LEN) )  < CUBE_LEN*0.1 )
		{
			m_Cubes[Index].SetRotation(0.0f, m_RotateAngle, 0.0f);
		}
	}
}

void OpenGL_MagicCube::RotationYZFace(int Value)
{
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if ( abs( m_Cubes[Index].GetPPositionX() - (m_Start+Value*CUBE_LEN) )  < CUBE_LEN*0.1 )
		{
			m_Cubes[Index].SetRotation(m_RotateAngle, 0.0f, 0.0f);
		}
	}
}


int  OpenGL_MagicCube::CalcPicking(int x, int y)
{
	int Re = -1;
	for (int Index=0; Index<m_Number*m_Number*m_Number; Index++) 
	{
		if ( m_Cubes[Index].TestHit(x, y, m_ModelMatrix, m_ProjMatrix, m_Viewport) )
		{
			m_Cubes[Index].SetShowWire(!m_Cubes[Index].GetShowWire());
			Re = Index;
			break;
		}
	}

	return Re;
}

void  OpenGL_MagicCube::Sort()
{
	for (int i=0; i<m_Number*m_Number*m_Number-1; i++) 
	{
		int k = i;
		for (int j=i+1; j<m_Number*m_Number*m_Number; j++) 
		{
			if ( m_Cubes[j].GetPPositionZ() - m_Cubes[k].GetPPositionZ() > EPRECISION)
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

bool OpenGL_MagicCube::CheckAllFaceColor(void)
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
			Vector3 p[3];
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



int OpenGL_MagicCube::GetFaceInMagicFace(Vector3 *p)
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

void OpenGL_MagicCube::RandRotation()
{
	srand(GetTickCount());

	m_RotationList.clear();
	int N = rand() % 50;
	for (int i=0; i< 30 + N ; i++)
	{
		m_RotateAngle = 180*0.5;
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

void OpenGL_MagicCube::UnRotation()
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

