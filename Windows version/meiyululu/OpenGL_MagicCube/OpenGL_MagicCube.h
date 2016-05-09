//=========================================================================
/**
*  @file      OpenGL_MagicCube.h
*
*  �ļ�����:  �����ܾ���ʵ���� 
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*  ����������������������¼����麯��
*																								
*	virtual bool Init();														
*		ִ�����еĳ�ʼ������������ɹ���������true							
*																			
*	virtual void Uninit();													
*		ִ�����е�ж�ع���										
*																			
*	virtual void Update(DWORD milliseconds);										
*		ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
*																			
*	virtual void Draw();															
*		ִ�����еĻ��Ʋ���
*/
//=========================================================================

#ifndef __TEST_H__
#define __TEST_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "GLFrame.h"												/**< ���������Ŀ���� */
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



/** ��GL_Application������һ������ */
class OpenGL_MagicCube : GLApplication								
{
public:
	bool	Init();							/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void	Uninit();						/**< ִ�����е�ж�ع��� */
	void	Update(DWORD milliseconds);		/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void	Draw();							/**< ִ�����еĻ��Ʋ��� */
	
	void	SetLight();
	void    PrintText();                    /**< ���������Ϣ */
	void    ShowCubes();           /**< ��ʾģ�� */

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
	friend class GLApplication;				/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	OpenGL_MagicCube(const char * class_name);	/**< ���캯�� */

	/** �û��Զ���ĳ������ */ 
	float			  m_Start;		
	int				  m_Number;
	vector <CCube>    m_Cubes;                         /**< 3DS�ļ������� */

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
	GLFont     m_Font;                        /**< ������ */  
	bool       m_RenderMode;		          /**< ����ģʽ */
	bool       sp;                            /**< �ո���Ƿ��ͷ� */

	GLdouble  m_ModelMatrix[16];
	GLdouble  m_ProjMatrix[16]; 
	GLint	  m_Viewport[4];

};


#endif	// __TEST_H__