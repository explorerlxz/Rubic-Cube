//========================================================
/**
*  @file      Vector.h
*
*  �ļ�����:  ������  
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*
*/     
//========================================================

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "stdafx.h"

#define  EPRECISION  0.001

//PI�ĺ궨��
#define M_PI        3.141592653589
#define M_2PI       6.283185307178


/** ������ */
class Vector3
{
public:
	/** ���캯�� */
	Vector3()  { x = 0.0; y = 0.0; z = 0.0; }
	Vector3( float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	Vector3(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
    
	/** ��Ա���� */
	inline float length();                       /**< ������������ */
	Vector3 normalize();                         /**< ��λ������ */
	float dotProduct(const Vector3& v);          /**< ������ */
	Vector3 crossProduct(const Vector3& v);      /**< ������ */

	/** ���ز����� */
	Vector3 operator + (const Vector3& v);
	Vector3 operator - (const Vector3& v);
	Vector3 operator * (float scale);
	Vector3 operator / (float scale);
	Vector3 operator - ();
	
public:
	  float x,y,z;

};

/** ��ά������ */
class Vector2
{
public:
	/** ���캯�� */
	Vector2(float xx = 0.0,float yy = 0.0) { x = xx; y = yy;  }
	Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

public:
	float x,y;
};

Vector3 *Vector3Transform(Vector3 * pOut, Vector3 * pIn, float *M);
bool IntersectTri(Vector3 *p0, Vector3 *p1, Vector3 *p2, Vector3 *RayPos, Vector3 *RayDir, float *U, float *V, float *Dist);

#endif //__VECTOR_H__