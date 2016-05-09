
//========================================================
/**
*  @file      Cube.cpp
*
*  文件描述:  小立方体类
*  适用平台： Windows98/2000/NT/XP
*  
*
*/     
//========================================================

#include "Cube.h"

CCube::CCube(void)
{
	m_ShowWire = false;
	m_Model = NULL;

	glPushMatrix();

	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform);

	glPopMatrix();
}

void CCube::SetShowWire(bool Wire)
{
	m_ShowWire = Wire;
}

bool CCube::GetShowWire()
{
	return m_ShowWire;
}

CCube::~CCube(void)
{
}


bool CCube::LoadModelFile(string FileName)
{
	if (m_Model != NULL)
	{
		delete m_Model;
		m_Model = NULL;
	}
	
	m_FileName = FileName;
	int pos = m_FileName.rfind('\\') + 1;
	string Path = m_FileName.substr(0, pos);

	m_Model = new C3DSModel();
	m_Model->Init();
	C3DSReader Loader;
	Loader.Reader(m_FileName.c_str(), m_Model);
	m_Model->doAfterMath();

	return true;
}

void CCube::Draw()
{

	glPushMatrix();

	//if (m_ShowWire)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	//else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	glMultMatrixf(m_Transform);
	m_Model->Draw();

	glPopMatrix();

	

}


void CCube::SetRotation(float AngleX, float AngleY, float AngleZ)
{
	glPushMatrix();

	glLoadIdentity();
	glRotatef(AngleX, 1.0, 0.0, 0.0);
	glRotatef(AngleY, 0.0, 1.0, 0.0);
	glRotatef(AngleZ, 0.0, 0.0, 1.0);
	glMultMatrixf(m_Transform);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform);

	glPopMatrix();
}


void  CCube::SetPositon(float x, float y, float z)
{
	 m_PositionX = x;
	 m_PositionY = y;
	 m_PositionZ = z;

	 glPushMatrix();

	  glLoadIdentity();
	  glTranslatef(x, y, z);
	  glMultMatrixf(m_Transform);
	  glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform);

	 glPopMatrix();
}


float CCube::GetPPositionX()
{
	Vector3 V(0.0, 0.0, 0.0);
	Vector3Transform(&V, &V, m_Transform);

	return V.x;
}


float CCube::GetPPositionY()
{
	Vector3 V(0.0, 0.0, 0.0);
	Vector3Transform(&V, &V, m_Transform);

	return V.y;
}

float CCube::GetPPositionZ()
{
	Vector3 V(0.0, 0.0, 0.0);
	Vector3Transform(&V, &V, m_Transform);

	return V.z;
}


bool  CCube::TestHit(int x, int y, GLdouble*ModelMatrix, GLdouble*ProjMatrix, GLint* Viewport)
{
	GLdouble  o1x, o1y, o1z;
	GLdouble  o2x, o2y, o2z;

	gluUnProject((GLdouble)x,  (GLdouble)(Viewport[3] - y - 1),  0.0,  ModelMatrix,  ProjMatrix,  Viewport,  &o1x,  &o1y,  &o1z);
	gluUnProject((GLdouble)x,  (GLdouble)(Viewport[3] - y - 1),  1.0,  ModelMatrix,  ProjMatrix,  Viewport,  &o2x,  &o2y,  &o2z);

	Vector3 RayStart  =  Vector3((GLfloat)o1x,  (GLfloat)o1y,  (GLfloat)o1z);
	Vector3 RayEnd    =  Vector3((GLfloat)o2x,  (GLfloat)o2y,  (GLfloat)o2z);

	float radius = CUBE_LEN*SPEER_RADIO_FAC;
	Vector3 U(0.0, 0.0, 0.0);
	Vector3Transform(&U, &U, m_Transform);
	Vector3 center(U.x, U.y, U.z);

	//获得中心点到射线的距离
	Vector3 v1 = center - RayEnd;
	v1.normalize();
	Vector3 v2 = RayStart - RayEnd;
	v2.normalize();

	GLfloat cosine = v2.dotProduct(v1);
	GLfloat sinine = sqrt(1.0f - cosine*cosine);
	GLfloat dist = (center - RayEnd).length() * sinine;

	if(dist > radius) return false;

	return true;
}

int CCube::GetFaceColor(int Face)
{
	if (Face < 1  || Face > CUBE_FACE_NUMBER)
	{
		return -1;
	}

	return Face;
}

int  CCube::GetFaceVertex(int Face, Vector3 p[])
{

//		     /4------------3/
//		    /              /|
//	       /       5      / |
//        /              /  |
//	     /1------------2/   |
//        |            |    |
//        |      1     | 2 / 7    
//        |            |  /
//        |5----------6| /
//
//


	if (Face < 1  || Face > CUBE_FACE_NUMBER)
	{
		return -1;
	}


	int L = CUBE_LEN;
	float Start = -L*0.5;
	float End   = L*0.5;

	Vector3 Vertexs[CUBE_VERT_NUMBER] = 
	{
		Vector3(Start, End, Start),  //1
		Vector3(End,   End, Start),
		Vector3(End,   End, End),
		Vector3(Start, End, End), //4
		Vector3(Start, Start, Start),
		Vector3(End,   Start, Start),//6
		Vector3(End,   Start, End),
		Vector3(Start, Start, End),
	};

	int FaceVertex[CUBE_FACE_NUMBER][3] = {
		{1, 2, 6},   // 1
		{6, 2, 3},
		{4, 3, 7},
		{5, 1, 4}, //4
		{1, 2, 4},
		{5, 6, 7},
	};

	int Index = Face - 1;

	for (int i=0; i<3; i++)
	{
		
		int k = FaceVertex[Index][i]-1;
		
		Vector3 V(Vertexs[k].x, Vertexs[k].y, Vertexs[k].z);
		Vector3Transform(&V, &V, m_Transform);

		 p[i].x = V.x;  p[i].y = V.y;  p[i].z = V.z;
	}

	return 1;
}
