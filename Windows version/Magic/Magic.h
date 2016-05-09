#pragma once

#include "D3DApplication.h"
#include "d3dfont.h"
#include "d3dUtility.h"
#include "Cube.h"

#include <string>
#include <fstream>
#include <vector>

using namespace std;


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


class CMagic :  CD3DApplication	
{

private:
	
	CD3DFont* m_Font;

	int	  m_Number;
	bool  m_RenderMode;

	int   m_RotateAxle;
	int   m_RotateFace;

	float m_RotateAngle;
	DWORD m_LastTime;
	DWORD m_Speed;

	POINT m_LastPt;
	POINT m_StartPt;
	bool  m_bClick;
	bool  m_bRotate;

	vector<CCube> m_Cubes;

	IDirect3DVertexBuffer9*  m_VB;
	IDirect3DIndexBuffer9*   m_IB;

	float m_Start;

	int m_SelectIndex;
	int m_PickFace;
	int m_LastPickFace;

	bool m_Success;

	vector <CRotation> m_RotationList;

	vector <CRotationAnimation> m_RotationAnimation;
	bool   m_AnimationBusy;

	

public:
	CMagic(const char * class_name);
	~CMagic(void);

	bool	Init();							/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void	Uninit();						/**< ִ�����е�ж�ع��� */
	void	Update(DWORD milliseconds);		/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void	Draw();							/**< ִ�����еĻ��Ʋ��� */

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
	void	GetRay(int x, int y, D3DXVECTOR3 *origin, D3DXVECTOR3 *direction);

	int  GetXFace(int Index);
	int  GetYFace(int Index);
	int  GetZFace(int Index);

	void  Sort();

	bool CheckAllFaceColor(void);
	int  GetFaceInMagicFace(D3DXVECTOR3 *p);
	void RandRotation();
	void UnRotation();
	void DoAnimation();
};
