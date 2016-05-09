//========================================================
/**
*  @file      Vector.cpp
*
*  文件描述:  向量类  
*  适用平台： Windows98/2000/NT/XP
*  
*
*/     
//========================================================

#include "Vector.h"                       /**< 包含头文件 */

/** 计算向量的长度 */
inline float Vector3::length()
{
	return sqrt( (float)( x * x + y * y + z * z ) );
}

/** 单位化一向量 */
Vector3 Vector3::normalize()
{
	float len = length();                  /**< 计算向量长度 */
	if( len == 0 )
		len = 1;
	x = x / len;
	y = y / len;
	z = z / len;

	return *this;
}

/** 点积 */
 float Vector3::dotProduct(const Vector3& v)
{
	return ( x * v.x + y * v.y + z * v.z );
}

/** 叉积 */
Vector3 Vector3::crossProduct(const Vector3& v)
{
	Vector3 vec;

	vec.x = y * v.z - z * v.y;
	vec.y = z * v.x - x * v.z;
	vec.z = x * v.y - y * v.x;
	
	return vec;
}

/** 操作符 + */
 Vector3 Vector3::operator +(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
}

/** 操作符 - */
 Vector3 Vector3::operator -(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;

	return vec;
}

/** 操作符 * */
 Vector3 Vector3::operator *(float scale)
{
	x = x * scale;
	y = y * scale;
	z = z * scale;
	
	return *this;
}

/** 操作符 / */
 Vector3 Vector3::operator /(float scale)
{
	if(scale != 0.0)
	{	
	   x = x / scale;
	   y = y / scale;
	   z = z / scale;
	}
	return *this;
}

/** 负号 */
 Vector3 Vector3::operator -()
{
	Vector3 vec( - x,- y, - z);
	return vec;
}


Vector3 *Vector3Transform(Vector3 * pOut, Vector3 * pIn, float *M)
{
	Vector3 Tem = *pOut;
	Tem.x =   M[0]*pIn->x +  M[4]*pIn->y +  M[8] *pIn->z +  M[12];
	Tem.y =   M[1]*pIn->x +  M[5]*pIn->y +  M[9] *pIn->z +  M[13];
	Tem.z =   M[2]*pIn->x +  M[6]*pIn->y +  M[10]*pIn->z +  M[14];
	float w = M[3]*pIn->x +  M[7]*pIn->y +  M[11]*pIn->z +  M[15];

	Tem.x = Tem.x / w;
	Tem.y = Tem.y / w;
	Tem.z = Tem.z / w;

	*pOut = Tem;

	return pOut;
}

bool IntersectTri(Vector3 *v0, Vector3 *v1, Vector3 *v2, Vector3 *orig, Vector3 *dir, float *u, float *v, float *Dist)
{
	// Find vectors for two edges sharing vert0
	Vector3 edge1 = *v1 - *v0;
	Vector3 edge2 = *v2 - *v0;

	// Begin calculating determinant - also used to calculate U parameter
	Vector3 pvec = dir->crossProduct(edge2);


	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = edge1.dotProduct(pvec); 

	Vector3 tvec;
	if( det > 0 )
	{
		tvec = *orig - *v0;
	}
	else
	{
		tvec = *v0 - *orig;
		det = -det;
	}

	if( det < 0.0001f )
		return FALSE;

	// Calculate U parameter and test bounds
	*u = tvec.dotProduct(pvec);
	if( *u < 0.0f || *u > det )
		return FALSE;

	// Prepare to test V parameter
	Vector3 qvec = tvec.crossProduct(edge1);

	// Calculate V parameter and test bounds
	*v = dir->dotProduct(qvec);
	if( *v < 0.0f || *u + *v > det )
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*Dist = edge2.dotProduct(qvec);
	FLOAT fInvDet = 1.0f / det;
	*Dist *= fInvDet;
	*u    *= fInvDet;
	*v    *= fInvDet;

	return true;
}








