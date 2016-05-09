
//========================================================
/**
*  @file      Cube.h
*
*  文件描述:  小立方体类
*  适用平台： Windows98/2000/NT/XP
*  
*
*/     
//========================================================

#pragma once

#include <string>
#include <vector>
#include "3DSModel.h"	
#include "3DSReader.h"


#define CUBE_LEN     (100.0)
#define CUBE_FACE_NUMBER  (6)     //正方体面数
#define CUBE_VERT_NUMBER  (8)     //正方体顶点数
#define SPEER_RADIO_FAC  (0.7) 

using namespace std;

class CCube
{

private:
	string m_FileName;
	C3DSModel                 *m_Model;
	
	float	          		   m_Rotation[16];
	float					   m_Transform[16];

	bool					   m_ShowWire;
	float					   m_PositionX;
	float					   m_PositionY;
	float					   m_PositionZ;

public:
	CCube(void);
	~CCube(void);

	bool LoadModelFile(string FileName);
	void Draw();

	float GetPPositionX();
	float GetPPositionY();
	float GetPPositionZ();

	void SetPositon(float x, float y, float z);
	void SetRotation(float AngleX, float AngleY, float AngleZ);
	void SetShowWire(bool Wire);
	bool GetShowWire();

	bool  TestHit(int x, int y, GLdouble*ModelMatrix, GLdouble*ProjMatrix, GLint* Viewport);

	int  GetFaceColor(int Face);;
	int  GetFaceVertex(int Face, Vector3 p[]);
};
