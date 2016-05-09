//=========================================================================
/**
*  @file      OpenGL_MagicCube.h
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

#ifndef __TEST_H__
#define __TEST_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "GLFrame.h"												/**< 包含基本的框架类 */
#include "Font.h"

#include "glStructures.h"	
#include "3DSReader.h"		
#include "3DSModel.h"		
#include "Cube.h"


#include <string>
#include <fstream>
#include <vector>

using namespace std;


#define  Z_Axle 0
#define  Y_Axle	1
#define  X_Axle	2

#define  FRONT_FACE  0
#define  UP_FACE  	 1
#define  SIDE_FACE	 2


#define   CHECK_COLOR 0


struct CRotation
{
	int   RotateAxle;
	int   RotateFace;
	float RotateX;
};

struct CRotationAnimation
{
	int   RotateAxle;
	int   RotateFace;
	float RotateX;

	float Step;
	int   Speed;
	int   Type;

};



/** 从GL_Application派生出一个子类 */
class OpenGL_MagicCube : GLApplication								
{
public:
	bool	Init();							/**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						/**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		/**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							/**< 执行所有的绘制操作 */
	
	void	SetLight();
	void    PrintText();                    /**< 输出文字信息 */
	void    ShowCubes();           /**< 显示模型 */

	void	Rotation(int Face, int Speed=10, int Type=-1);
	void	RealRotation(int Face);
	void	RotationXYFace(int Value);
	void	RotationXZFace(int Value);
	void	RotationYZFace(int Value);

	void	OnMouseDown(WPARAM wParam, LPARAM lParam);	
	void	OnMouseUp(WPARAM wParam, LPARAM lParam);
	void	OnMouseMove(WPARAM wParam, LPARAM lParam);

	int	    CalcPicking(int x, int y);
	int     GePickFace(int x, int y);
	void	GetRay(int x, int y, Vector3 *origin, Vector3 *direction);

	int  GetXFace(int Index);
	int  GetYFace(int Index);
	int  GetZFace(int Index);

	void  Sort();

	bool CheckAllFaceColor(void);
	int  GetFaceInMagicFace(Vector3 *p);
	void RandRotation();
	void UnRotation();
	void DoAnimation();
		
	
private:
	friend class GLApplication;				/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	OpenGL_MagicCube(const char * class_name);	/**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	float			  m_Start;		
	int				  m_Number;
	vector <CCube>    m_Cubes;                         /**< 3DS文件载入类 */

	float m_AllAngle;		
	//float m_RotaAngle;
	int   m_RotateAxle;
	int   m_RotateFace;

	float m_RotateAngle;
	DWORD m_LastTime;
	DWORD m_Speed;

	POINT m_LastPt;
	POINT m_StartPt;
	bool  m_bClick;
	bool  m_bRotate;

	int m_SelectIndex;
	int m_PickFace;
	int m_LastPickFace;
	bool m_Success;

	vector <CRotation> m_RotationList;
	vector <CRotationAnimation> m_RotationAnimation;
	bool   m_AnimationBusy;


	Vector3	    m_CameraPosition;
	Vector3		m_CameraView;
	Vector3		m_CameraUp;
	GLFont     m_Font;                        /**< 字体类 */  
	bool       m_RenderMode;		          /**< 绘制模式 */
	bool       sp;                            /**< 空格键是否释放 */

	GLdouble  m_ModelMatrix[16];
	GLdouble  m_ProjMatrix[16]; 
	GLint	  m_Viewport[4];

};


#endif	// __TEST_H__